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

## Thread unconditional JMP-to-JMP chains in the optimized pipeline

`--optimize` can leave a `JMP` whose target is another `JMP`. The
unoptimized pipeline doesn't, so on small programs the unoptimized
build can run in fewer cycles than the optimized one.

The case that shows it is `tests/chapter_6/valid/extra_credit/goto_backwards.c`.

```c
int main(void) {
    if (0)
    label:
        return 5;
    goto label;
    return 0;
}
```

The TAC optimizer folds `if (0)` to an unconditional `Jump`, and the
optimized output keeps the chain.

```
   JMP   .if_end@0
.main@label:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@0:
   JMP   .main@label
```

That takes 32 cycles. The unoptimized build lowers `if (0)` to a
compare and a conditional branch, which `branch_through_jump`
retargets, and it comes out as a straight `LDX #0; LDA #5; RTS` in
26 cycles.

`branch_through_jump` only rewrites a conditional `Branch` whose
target label is followed by a `Jump`. Extending it (or adding a
sibling pass) to rewrite `Jump(L1)` when `L1:` is followed by
`Jump(L2)` into `Jump(L2)`, and then dropping a `Jump` to the label
immediately after it, should collapse this shape.

When it's fixed, remove the `goto_backwards.c` entry from
`_OPT_SLOWER` in `tests/test_sim_differential.py`, which currently
exempts it from the "optimized is never slower" check.

## Call through an arbitrary function-pointer expression

The callee of a call must reduce to a single name. `f(x)`, `fp(x)`,
`(fp)(x)`, and `(*fp)(x)` compile, but these valid C99 calls don't.

```c
pick()(3);        /* calling a function's returned pointer */
(pick())(3);
t[0](3);          /* an element of a table of function pointers */
s.g(3);           /* a struct member */
p->g(3);
```

`t[i](x)` and `s->g(x)` are the usual shapes of dispatch tables and
callback structs, so these matter more than the first two.

The limit is in the IR. `c99_ast.FunctionCall` carries the callee as
an identifier string, and the parser's `indirect_function_call`
transformer accepts only callees that boil down to one name (a Var,
`*Var`, or `&Var`). `tac_ast.IndirectCall` already takes a `val`
pointer, so the backend can call through any computed pointer. The
work is in front of it:

- give `c99_ast` a call node whose callee is an expression (or add
  an `IndirectCall(exp callee, ...)` variant beside `FunctionCall`);
- let the grammar's postfix level parse `postfix_exp LPAREN
  arg_list? RPAREN`, which also removes the `LPAREN exp RPAREN
  LPAREN` special case and its shift-reduce note in `c99.lark`;
- type-check the callee as a pointer to function (after decay) and
  check the arguments against its `FunType`;
- in `c99_to_tac`, evaluate the callee expression to a temp and emit
  `IndirectCall` with it.

A function containing such a call becomes ineligible for zp_abi,
like any function with an `IndirectCall` today.

## Let zp_abi functions call soft-stack functions

A function only gets zp_abi when every function it calls directly has
it too (`abi_selection._demote_soft_stack_callers`). The reason is
that a soft-stack function uses $80–$BF as caller-saved scratch, and
the zp_abi parameter slots and private local pools come from the
same window. Before the rule, `int f(int p) { int l = rec(3);
return p + l; }` with a recursive `rec` read a clobbered `p`.

The rule costs a lot. One recursive function, one function whose
address is taken, or one indirect call demotes every function that
can reach it, and a demoted function loses zp_abi parameters and its
private pool. A function marked `__attribute__((zp_abi))` or using
`register` can't be demoted, so calling a soft-stack function makes
it fail to compile instead.

A recursive function whose recursive calls are all under `#pragma
c6502 call save(hardware_stack)` needn't demote anything. It still
takes its parameters on the soft stack, but its locals are in a
private pool and it writes nothing outside the pools, so it's as safe
to call as a zp_abi function. It demotes its callers today only
because `select_abi` runs before `recursive_pool_functions` decides
which recursive functions get pools.

Possible approaches, roughly in order of effort:

- **Only demote when something is live across the call.** If no
  parameter slot or pool byte is read after the soft-stack call
  (the call is the last use, or its result is returned directly),
  nothing can be clobbered. This needs the asm-level liveness the
  regalloc already computes, or a TAC-level approximation.
- **Save and restore around the call.** Treat the caller's live
  slot and pool bytes like caller-saved registers: copy them to the
  hardware stack or a frame slot before the JSR and back after. That
  keeps zp_abi for the callers and moves the cost to the soft-stack
  call sites, which are already the expensive ones.
- **Keep soft-stack scratch out of the zp_abi window.** Give
  soft-stack functions a caller-saved range disjoint from where
  zp_abi slots and pools are allocated, or allocate the slots of
  functions that call soft-stack code from the callee-saved
  $C0–$FF range, which soft-stack prologues already save and
  restore. This needs a look at ZP pressure, since both pools would
  shrink.

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

## Branch on a carry-struct flag without storing it

A `return_bool_in_carry` function can return a struct of a value and
a `bool`. The caller stores both members into the struct local, and
`if (c.end)` then reads the flag back through a pointer, since a
struct local is address-taken. Keeping a struct local whose address
doesn't escape in per-member scalars (scalar replacement of
aggregates) would let `carry_branch_fold` turn the test into a `BCS`
right after the `JSR`, and would drop the pointer setup for every
member access. That would help every struct local, not only these.

## Optimizations found by the Zork string printer

`examples/zork_print_string_literal.c` first compiled to 1984 bytes of
code, about twice the 980 bytes of Zork's own code for the same
routines. Inline lowering of 16-bit shifts by 7 and 15, and direct
access to struct members at constant offsets
(`passes/optimization/direct_member_access.py`), brought that to
1510. Saving the decoder's state on the hardware stack around the
call that recurses (`#pragma c6502 call save(hardware_stack)`,
`passes/recursion_saves.py`) took off another 180, leaving
print_zstring at 277 bytes against Zork's 216. What's left, roughly
in order of size:
- **A loop count passed in X stays in memory.** cout_string takes its
  count with `in_x`, as Zork's does, then stores X to a slot and
  counts the slot down with `DEC`. COUT1 preserves X, so the count
  could stay in X with `DEX`, as in the original.
- **A shift by one stages through a slot.** `(uint8_t)(w << 1)` is
  `LDA w; STA t; ASL t; LDA t`, where `LDA w; ASL A` does the same
  when `t` is dead after.
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

## Zero-page globals across translation units, and automatic placement

`__attribute__((zeropage))` and `zeropage(addr)` place a global in
zero page within one translation unit (`passes/zp_globals.py`). Two
pieces are left.

- **Linking.** `compile.py --link` rejects a unit with zero-page
  globals. Each unit places its bare `zeropage` objects in `$26`-`$7F`
  without knowing about the others, so two units would collide, and
  the linker rebuilds the EQU block from slot symbols only, so the
  globals' EQUs would be lost. Each unit's `__zp_init` would also
  clash by name. The link metadata block would need to list each
  unit's zero-page objects (size, fixed address or not), so the
  linker can place the bare ones globally, reserve every unit's
  bytes when it re-allocates slots, and merge the init routines into
  one.
- **Automatic promotion under `--optimize`.** Rank globals by static
  access count (weighting loop bodies) and move the top ones into
  whatever of `$26`-`$7F` is left. Only safe within one translation
  unit, or with the linker deciding.

## More system-software headers, and their zero page when linking

`include/apple2/monitor_io.h` and `interactive_monitor.h` reserve the
Monitor's zero page, and the runtime's block moves around it
(`passes/runtime_layout.py`). Two pieces are left.

- **Headers for DOS 3.3, ProDOS, and Applesoft.** Each declares the
  zero-page bytes that software uses, the way `monitor_io.h` does, from
  its listing. A program that includes one keeps those bytes free.
- **Linking.** `--link` rejects a unit with zero-page objects, so a
  unit that includes `monitor_io.h` can't be linked yet (see "Zero-page
  globals across translation units"). Once it can, the linker has to
  choose one runtime layout from every unit's reserved bytes, and
  the HARGS size from every unit's need. Today it only checks that
  the units' layouts match.

## Keep values in X or Y across preserving calls in more passes

`__attribute__((preserves_x))` / `preserves_y` reach the asm passes
on each `Call` (`asm_liveness.call_preserves`). Liveness,
`loop_counter_to_x`, and `counter_to_index_reg` use them. Every other
pass still treats a call as writing X and Y, which is correct but
gives up some wins. `redundant_load` forgets what X and Y hold at
every call, so an `LDX #k` repeated after `JSR COUT1` stays, and the
register allocator doesn't keep a `register` local in Y across a
preserving call. Either could keep an immediate it tracks in X or Y
across such a call. A memory equivalence (X equal to some slot)
can't survive, since the callee may write the slot.

## Aliasing rule for two `(zp),Y` bases

`asm_aliasing.may_alias` says two `IndirectZp` / `IndirectZpY`
accesses through different zero-page pointers never alias. Two
pointer variables can hold the same address, so the rule isn't sound
in general. No pass seems to be able to act on it today: every store
through a pointer goes through A, so a register tracker still sees
the right value when the pointers are equal, and the dead-store and
store-tracking passes only look at `Data` / `ZP` targets. Programs
with two zero-page pointers to one buffer compiled correctly in a
spot check. It should become "may alias" unless the two pointers are
known to differ, before a pass that would act on it appears.

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
