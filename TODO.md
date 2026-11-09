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
private pool. A function that uses `register` or
`__attribute__((zp_abi))` can't be demoted, so it fails to compile
instead.

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
