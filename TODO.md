# TODO

## Rewrite the README and add an architecture document

README.md has grown into a wall of low-level detail. It should
describe the project in a few paragraphs and then cover how to
download and install it, how to run it, and how to compile the code
and run the tests. It should also include one small C example with
the output from each of the main flags (`--tac`, `--codegen`, and
`--codegen --optimize`), and link to
[docs/calling_conventions.md](docs/calling_conventions.md) and
[docs/attributes.md](docs/attributes.md).

The low-level material moves to a new `docs/architecture.md`, which
describes the pipeline, the four IRs, and the runtime conventions.
That document should be organized around diagrams, tables, and short
sections with headings, so that a reader can find one stage without
reading the whole thing.

## Let zp_abi functions call soft-stack functions without a pool

A function only gets zp_abi when every function it calls directly
either has it too or has a private pool
(`abi_selection._demote_soft_stack_callers`). A soft-stack function
without a pool uses $80–$BF as caller-saved scratch, and the zp_abi
parameter slots and private local pools come from the same window.
Before the rule, `int f(int p) { int l = rec(3); return p + l; }`
with a recursive `rec` read a clobbered `p`. A soft-stack function
with a pool, such as one whose address is taken or a recursive one
under `#pragma c6502 call save(hardware_stack)`, writes only its
pool, so calling it doesn't demote anything.

The callees that still demote are the ones whose writes are
unbounded: recursion without the pragma, a function that calls
through a pointer, and an `extern` that isn't zp_abi (such as a ROM
routine declared without `zp_abi`). One of these demotes every
function that can reach it, and a demoted function loses zp_abi
parameters and its private pool. A function marked
`__attribute__((zp_abi))` or using `register` can't be demoted, so
calling one makes it fail to compile instead.

Possible approaches, roughly in order of effort:

- **Save and restore around the call.** Treat the caller's live
  slot and pool bytes like caller-saved registers, pushing them on
  the hardware stack before the JSR and pulling them back after.
  `passes/recursion_saves.py` already does this for calls that
  recurse (`plan_saves` picks the values, `tac_to_asm` emits `PHA` /
  `PLA`, and the asm optimizer checks nothing colorable is live
  across). Three differences:
  - The caller isn't on a cycle, so it appears at most once in any
    chain of calls, and the hardware stack these saves use is
    bounded by the call graph. The compiler can add it up and fall
    back to demotion past a budget, with no pragma.
  - The save set has to include the caller's zp_abi parameters,
    which `plan_saves` leaves out because a recursive function keeps
    them in its frame.
  - The saving spreads upward. If `h` calls `f` and `f` calls an
    unbounded `g`, `g` can overwrite `h`'s bytes too, so `h`'s call
    to `f` needs saves as well. That replaces today's upward
    demotion with upward saves, and with several call sites it can
    cost more than demotion, so it needs a per-function estimate.
  Demoting only when nothing is live across the call is the case of
  this with nothing to push.
- **Keep soft-stack scratch out of the zp_abi window.** Allocate
  the slots and pools of functions that call unbounded code from the
  callee-saved $C0–$FF range, which soft-stack prologues already
  save and restore. A pool there breaks an unbounded function that
  calls into it, since that function expects $C0–$FF to survive its
  calls, so every function reachable from unbounded code has to stay
  out of it. This needs a look at ZP pressure, since both pools
  would shrink.

Whatever approach is taken, the call-graph fuzzer from Nov 2026
(random DAGs mixing zp_abi, recursive, and pointer-called functions
with values live across calls) is the way to check it, because
every bug found in this area showed up only with a value live
across a call into a different calling convention.

## Return a boolean in the carry flag without an annotation

`__attribute__((return_bool_in_carry))` (README, "Returning a bool in
the carry flag") gives a function the convention explicitly. The
other way to opt in was **under zp_abi, for a `bool` return**: every
caller of a zp_abi function is known and compiled with it, so the
convention could change without an annotation.

A `_Bool` return type tells a `bool` return from an `unsigned char`
one, and its value is always 0 or 1. It also changes what the function's
callers do with the result, so it has to be decided before
`tac_to_asm` lowers any call to it, and the multi-TU linker would need
the choice in its metadata.

## Optimizations found by the Zork string printer

`examples/zork_print_string_literal.c` first compiled to 1984 bytes of
code, about twice the 980 bytes of Zork's own code for the same
routines. Inline lowering of 16-bit shifts by 7 and 15, and direct
access to struct members at constant offsets
(`passes/optimization/direct_member_access.py`), brought that to
1510. Saving the decoder's state on the hardware stack around the
call that recurses (`#pragma c6502 call save(hardware_stack)`,
`passes/recursion_saves.py`) took off another 180, leaving
print_zstring at 277 bytes against Zork's 216. Shifting single bytes
in A instead of in a slot (`narrow_widened_arith` on a wide operand,
and `passes/shift_in_a.py`) took off 11 more. What's left, roughly in
order of size:
- **A loop count passed in X stays in memory.** cout_string takes its
  count with `in_x`, as Zork's does, then stores X to a slot and
  counts the slot down with `DEC`. COUT1 preserves X, so the count
  could stay in X with `DEX`, as in the original.
- **A widened byte shifted through memory.** `(int)c << 3` is `LDA c;
  STA lo; ASL lo; LDA #0; STA hi; ROL hi; ASL lo; ROL hi; ...; LDA
  lo`, where `LDA #0; STA hi; LDA c; ASL A; ROL hi; ASL A; ROL hi;
  ...` keeps the low byte in A, about 7 bytes and 9 cycles less.
  `shift_in_a` only moves the first `ASL` to A, because the `LDA #0;
  STA hi` that comes between it and the rest has to move first.
  Doing it in `tac_to_asm` instead would make an in-place `w <<= 1`
  longer.
- **Operands staged through a temporary.** `sector >=
  SECTORS_PER_TRACK` copies `SECTORS_PER_TRACK` into a slot and
  subtracts the slot, where `SBC SECTORS_PER_TRACK` reads it in
  place.
- **Carry chains across statements.** load_packed_address computes
  three bytes of `2 * SCRATCH2` with one `ASL` / `ROL` / `ROL` chain
  in the original. In C those are three expressions, and each one
  recomputes the shift.

## Volatile structs

A `volatile struct s g;` object crashes `c99_to_tac` with a Python
traceback when a member is accessed (`_member_offset` reads `.tag`
from the `Volatile` wrapper), and a volatile-qualified member
(`struct s { volatile unsigned char b; };`) is rejected by the type
checker as "static-storage object can't have non-object type
Volatile". Both should compile, with every access to a volatile
member kept and in order, which `direct_member_access` already
preserves through the `is_volatile` bit. Until then, the crash should
at least be a one-line error.

## Arrays sized by their initializer

`int t[] = {1, 2, 3};` and `char s[] = "abc";` are rejected ("incomplete
array type (`[]`) is allowed only at the outermost type of an `extern`
declaration"). C99 §6.7.8.22 completes the type from the initializer:
the largest index it gives, plus one, or the string's length plus the
null. The type checker would size the array from the initializer
before checking it, at file scope and block scope.

## More system-software headers

`include/apple2/monitor_io.h` and `interactive_monitor.h` reserve the
Monitor's zero page, and the runtime's block moves around it
(`passes/runtime_layout.py`). Headers for DOS 3.3, ProDOS, and
Applesoft would do the same: each declares the zero-page bytes that
software uses, from its listing, so a program that includes one keeps
those bytes free. `examples/zork/instr_save/zork.h` declares the bytes
in `$26`-`$4F` that RWTS uses by hand.

## Rotate top-tested loops

`for` and `while` loops test at the top and end with a `JMP` back to
the test, which costs 3 cycles per iteration. Rotating them to a
guard plus a test at the bottom removes that `JMP`. A first version
(Nov 2026, while adding `examples/zork_find_index_of_page_table.c`)
copied the test in front of the loop as a guard, moved it to the
bottom as `JumpIfTrue`, and let constant folding drop the guard when
the first test is constant. On the natural `for` version of the Zork
page-table search it cut a non-matching entry from 21 cycles to 19,
and with `loop_invariant_a` to 16.

It was taken out because it made other examples worse:
`draw_sprite_opaque` went from 47 instructions to 89 and gained a
soft-stack frame, and `snd_delay_up` and `sfx_tone` grew by about a
third. In `draw_sprite_opaque` the frame holds a variable that is
read on the way into the loop but never written, so it gets no
zero-page byte. The rotated loop's Phis at both the header and the
exit seem to produce it somewhere in the asm-level SSA round trip;
that's the thing to find first. A register-pinned local in a rotated
loop also lost its Y placement for one of its versions.

Rotation also exposed two bugs that are fixed: `from_ssa` placed the
copies for both successors of a conditional block in front of its
jump (wrong for a do/while's bottom test, which already had this
shape), and a soft-stack prologue overwrote a `register` parameter
arriving in A.

## Catch a sim program that grows into the soft stack

The simulator loads code and data at `$0800` (`sim/harness.py`,
`DEFAULT_ORIGIN`), and the soft stack grows down from `$7FFF`
(`sim/runtime.py`, `SSP_INIT`). Nothing checks that the image ends
below the stack, so a program big enough to cross into it runs with
its own code or data being overwritten by stack frames, and the
results come back wrong with no error. An unoptimized program with
148 of the 64-bit shift and multiply functions in
`tests/test_const_shift_mul_sim.py` reached `$BF0E`, and its 64-bit
multiplies returned 0. That test now splits its functions into
programs of 40 to stay clear of it.

`build_sim` could raise when the image's end address is above some
margin below `SSP_INIT`, and the runtime could also trap a store
below the image's end through `SSP` or `FP`. The first check is
cheap and would have pointed at the problem immediately.
