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

## Return a boolean in the carry flag without an annotation

`__attribute__((return_bool_in_carry))` (README, "Returning a bool in
the carry flag") gives a function the convention explicitly. The
other way to opt in was **under zp_abi, for a `bool` return**: every
caller of a zp_abi function is known and compiled with it, so the
convention could change without an annotation.

That needs a way to tell a `bool` return from an `unsigned char` one,
because `<stdbool.h>` makes `bool` a macro for `unsigned char`. Either
c6502 grows a real `_Bool` type, or the compiler proves that every
`return` in the function yields 0 or 1 (a comparison, `!`, `&&`,
`||`, or a constant 0 or 1). It also changes what the function's
callers do with the result, so it has to be decided before
`tac_to_asm` lowers any call to it, and the multi-TU linker would need
the choice in its metadata.

## Two return values, one of them a bool in the carry flag

`return_bool_in_carry` covers a function whose only result is a
bool. The Zork routine returns two things: the slot index in A and
"not found" in the carry. C99 has no multiple return values, so this
needs a C spelling that c6502 can map onto A (and X) plus C.
Candidates:

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
way, the `Ret` / `Return` atoms would carry both `save_a` and
`save_c`, which the `return_bool_in_carry` epilogue already handles
separately.

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
(`passes/runtime_layout.py`). Three pieces are left.

- **Headers for DOS 3.3, ProDOS, and Applesoft.** Each declares the
  zero-page bytes that software uses, the way `monitor_io.h` does, from
  its listing. A program that includes one keeps those bytes free.
- **The IIe 80-column firmware.** `HOME`, `CLREOL`, scrolling, and
  `KEYIN` continue into the firmware at `$C100`, which the F8 listing
  doesn't cover, so `monitor_io.h` may miss bytes it uses.
- **Linking.** `--link` rejects a unit with zero-page objects, so a
  unit that includes `monitor_io.h` can't be linked yet (see "Zero-page
  globals across translation units"). Once it can, the linker has to
  choose one runtime layout from every unit's reserved bytes, and
  the HARGS size from every unit's need. Today it only checks that
  the units' layouts match.

## Declare which registers an external routine preserves

A call is taken to clobber A, X, and Y. COUT1 preserves Y, and the
F8 ROM code it runs never writes X. Its scroll path jumps into the
80-column firmware at `$C100`, which isn't in the listing, so X
there is unverified. Zork's loop keeps its index in X across `JSR
COUT1`. Something like
`__attribute__((preserves(x, y)))` on the declaration would let
`loop_counter_to_x` and the regalloc keep a value in X or Y across
the call. The Call atom would carry the set, as `reg_args` does
today, and each pass that treats a Call as writing X or Y would
read it.

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
