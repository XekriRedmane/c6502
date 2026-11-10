# TODO

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

## Return a boolean in the carry flag

6502 code often returns a yes/no answer in the carry flag, with the
caller branching on it right after the `JSR`. Zork I's
`find_index_of_page_table` returns the slot index in A and "not
found" as carry set, and its callers do `JSR find_index_of_page_table;
STA index; BCS not_found`. c6502 has no way to say this, so
`examples/find_index_of_page_table.c` returns a 16-bit value with the
flag in the high byte (X), which costs an `LDX #` in the callee and a
compare in the caller.

Two ways to opt in:

- **Under zp_abi, for a `bool` return.** Every caller of a zp_abi
  function is known and compiled with it, so the convention can
  change without an annotation. The callee ends in `CLC` / `SEC`
  instead of loading A.
- **With an attribute**, such as
  `__attribute__((return_bool_in_carry))`, so it's visible in a
  header and works for externs. A variant could name an out-flag
  alongside a byte return in A, for the index-plus-found shape above.

What the caller does with the result:

- `if (f())` / `while (!f())` lower to `JSR f; BCC` / `BCS` directly.
- A use as a value materializes it with `LDA #0; ROL A` (or
  `ADC #0` after a known-zero A).

Things that would need to change:

- `Ret` / `Return` would need a flag (like `save_a` / `save_x`)
  saying C is live at the return. `flags_dead_at` and
  `all_flags_dead_at` currently treat a return as the end of flag
  liveness, so passes like `dead_a_arith` would otherwise delete the
  `CLC` / `SEC`.
- The soft-stack epilogue rebuilds SSP with `CLC; ADC`, which
  clobbers C, so either limit this to bare-`RTS` zp_abi functions or
  wrap the epilogue in `PHP` / `PLP`.
- On the caller side, a `Call` currently counts as overwriting every
  flag. A call to such a function defines C instead, and the branch
  that reads it must not be separated from the `JSR` by anything
  that touches C.

## Two return values, one of them a bool in the carry flag

The previous item covers a function whose only result is a bool. The
Zork routine returns two things: the slot index in A and "not found"
in the carry. C99 has no multiple return values, so this needs a C
spelling that c6502 can map onto A (and X) plus C. Candidates:

- **A small struct return.** `typedef struct { uint8_t index; bool
  found; } lookup_t; lookup_t f(uint16_t page);` and a rule that a
  struct of one scalar of at most 2 bytes plus one `bool` returns the
  scalar in A (and X) and the bool in C. It's plain C99 and compiles
  unchanged with any other compiler. Today a struct return goes
  through a hidden sret pointer and disqualifies zp_abi, so this
  would be a new return path, and the caller would need to see
  `r.found` lower to a branch on C rather than to a memory read.
- **A flagged out-parameter.** `uint8_t f(uint16_t page, bool
  *found)` with something like `__attribute__((carry_out(found)))`.
  The callee's `*found = ...` stores become `CLC` / `SEC`, and the
  caller's `&local` argument receives C after the `JSR`. This needs
  restrictions to be sound: the callee only stores to `*found` (no
  reads, no passing the pointer on), and every caller passes the
  address of a local `bool`.

The struct form reads more naturally and keeps the source portable;
the out-parameter form matches how the assembly is usually
documented ("returns index in A, carry set if not found"). Either
way, `Ret` would need to carry "C is live" the same way the previous
item describes.

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
(Nov 2026, while adding `examples/find_index_of_page_table.c`) copied
the test in front of the loop as a guard, moved it to the bottom as
`JumpIfTrue`, and let constant folding drop the guard when the first
test is constant. On the natural `for` version of the Zork page-table
search it cut a non-matching entry from 21 cycles to 19, and with
`loop_invariant_a` to 16.

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
