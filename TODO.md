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
- **A widened byte shifted through memory.** `(int)c << 3` is `LDA c;
  STA lo; ASL lo; LDA #0; STA hi; ROL hi; ASL lo; ROL hi; ...; LDA
  lo`, where `LDA #0; STA hi; LDA c; ASL A; ROL hi; ASL A; ROL hi;
  ...` keeps the low byte in A, about 7 bytes and 9 cycles less.
  `shift_in_a` only moves the first `ASL` to A, because the `LDA #0;
  STA hi` that comes between it and the rest has to move first.
  Doing it in `tac_to_asm` instead would make an in-place `w <<= 1`
  longer.
- **Carry chains across statements.** load_packed_address computes
  three bytes of `2 * SCRATCH2` with one `ASL` / `ROL` / `ROL` chain
  in the original. In C those are three expressions, and each one
  recomputes the shift.
- **Adding `0x100`.** `p += 0x100` on a 2-byte value in memory is
  `LDA p; CLC; ADC #0; STA p; LDA p+1; ADC #1; STA p+1`, where `INC
  p+1` does the same. instr_save's page loop in
  `examples/zork/instr_save/save.c` has one, against Zork's `INC
  SCRATCH2+1`.

## Optimizations found by Zork's save instruction

`examples/zork/instr_save` is 689 bytes of code against Zork's 627, and
its first save takes 19,758 cycles against 19,561.
`get_prompted_number_from_user` accounts for most of what's left of
both, 44 bytes and 273 cycles. Roughly in order of payoff:

- **Index arithmetic narrowed by the array's bounds.** `prompts[which]`
  on `struct prompt prompts[3]` (16 bytes each) computes `which * 16`
  as a 16-bit shift in memory and adds it to the address. A valid
  index keeps every offset below 48, since indexing outside the array
  is undefined, so the offset fits a byte: `ASL A` four times, `TAX`,
  and `LDA prompts+15,X`, as Zork does.
- **An address kept across calls.** `&prompts[which]` doesn't change
  in `get_prompted_number_from_user`, but it's computed again after
  each call instead of kept in the function's pool.
- **Falling into a tail call.** `write_next_sector` ends in `JMP
  inc_sector_and_write`. Placing a function just before the one it
  tail-calls would drop the `JMP`, which is how Zork chains these
  routines.
- **A callee that leaves its parameter alone.** `instr_save` stores
  `#$80` into `copy_data_to_buff`'s pointer again before the second
  call, though `copy_data_to_buff` never writes it.
  `memory_value_propagation` forgets everything at a `Call`, but a
  callee's own parameter bytes survive when the callee doesn't change
  them, which `slot_sharing` already works out.
- **An immediate A already holds.** `LDA #$FF; STA SCRATCH1; LDA #$FF;
  STA SCRATCH1+1` keeps the second `LDA`.
- **A counted loop that tests at the top.** `please_insert_save_diskette`'s
  `for (n = ...; n != 0; n--)` ends in a `JMP` back to its test, with
  `n` in memory. Rotated (see "Rotate top-tested loops"), it would be
  `DEC; BNE`, and `counter_to_index_reg` could put `n` in X.

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

## Looser rules for `in_*` parameters

Two changes to the parameter attributes `in_a`, `in_x`, `in_y`, and
the pairs such as `in_ay` (`parser._register_marker`,
`abi_selection._assign_param_registers`, docs/attributes.md):

- **`register` is optional.** An `in_*` attribute already says the
  parameter arrives in a register, so a parameter with one is taken to
  be `register` whether it says so or not. Today the attribute without
  `register` is an error. A plain `register` parameter, without an
  attribute, keeps the positional rule (A, then X).
- **Any type of the right size.** A parameter with an `in_*` attribute
  is accepted whenever its size matches the number of registers named:
  1 byte for one, 2 for a pair. That includes pointers, which today
  can't be `register` parameters at all, so DOS's RWTS can be declared
  as it's called:

  ```c
  __attribute__((zp_abi, return_bool_in_carry, address(0x2900)))
  bool RWTS(struct iob *iob __attribute__((in_ya)));

  bool read_sector(void) { return RWTS(&iob); }
  ```

  instead of taking the address's high and low bytes as two `uint8_t`
  parameters and splitting `(uint16_t)&iob` at every call. The rule
  against pointers is there because a function the compiler compiles
  can only use a pointer from a zero-page pair, so it would store the
  registers there first, which is what passing it in its slot already
  does. A routine with a fixed address has no body the compiler
  compiles, so the rule doesn't apply. A compiled function with a
  pointer `in_*` parameter copies it into a zero-page pair of its pool
  on entry. The `RWTS` declaration in `examples/zork/instr_save/zork.h`
  and its call in `disk.c` change to match.

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
