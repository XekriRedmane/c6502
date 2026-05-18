# When mem-to-mem doesn't exist, your IR can lie to you

The 6502 has no `MOV mem, mem`. Every memory-to-memory copy is
`LDA src; STA dst` — two cycles, two opcodes, with A clobbered as
a side effect. Compilers know this. But IRs don't always represent
it, and that mismatch can hide bugs.

I just caught one in [c6502](https://github.com/XekriRedmane/c6502),
a C99 compiler I'm writing in Python that targets the 6502.

## The crime scene

I was compiling a game-engine routine `companion_update` — a
per-frame update for two on-screen "companion" sprites. The
function is `__attribute__((zp_abi))` (params live in zero page,
not on the soft stack) and contains the loop:

```c
for (int8_t slot = 1; slot >= 0; slot--) {
    // ... state-machine dispatch ...
    op_a((uint8_t)slot);
    op_b((uint8_t)slot, ...);
}
```

The optimized output for two slots was completely wrong: slot 0
never got updated, and slot 1 got updated twice. Sim returned
`0x0020` instead of `0x1010`.

## The buggy asm

The compiler had promoted `slot` to `Reg(X)` for indexed access
and the loop tail's `DEX`. It also used a zero-page slot
`__local_caller__slot` (call it `M`) as the save-home around each
`JSR`. The generated code looked like:

```asm
    LDA #1
    STA __local_caller__slot   ; M = 1
    TAX                         ; X = 1
.loop:
    LDA state,X                 ; X-indexed access, fine
    BPL .other
    LDA __local_caller__slot   ; ← STALE M
    STA __zpabi_op_a__slot
    STX __local_caller__slot   ; sync M (too late)
    JSR op_a
    LDX __local_caller__slot   ; restore X
    ...
.loop_continue:
    DEX                         ; X--, M not synced!
    BPL .loop
```

After `DEX`, X is fresh but M is the previous iteration's value.
On the next iteration, `LDA __local_caller__slot` reads stale
`M = old_slot`, and the callee receives the wrong slot index.
Slot 0 gets called with slot=1 (the stale value), and that's
why slot 0 never gets updated.

## Why the IR hid it

c6502 has an `asm_ast` IR with byte-typed atoms — almost. There's
one wart: a `Mov(src, dst)` atom can have both src and dst be
memory operands. It's a single IR atom that emits at asm-emit time
as `LDA src; STA dst`. The implicit `LDA` doesn't exist at the IR
level.

I wrote a peephole to fix the stale-M bug:

> For each `Mov(M, Reg(A))` (LDA M) where M is an X-save slot,
> rewrite to `Mov(Reg(X), Reg(A))` (TXA).

It worked on a unit test. End-to-end? No effect. The buggy code
still had `LDA M; STA __zpabi_callee_slot`.

Looking at the IR: that pair wasn't `Mov(M, Reg(A))` followed by
`Mov(Reg(A), Data(...))`. It was a single `Mov(M, Data(...))` —
a mem-to-mem atom. The pass never saw an `Mov(M, Reg(A))` to
rewrite.

The fix was to extend the peephole: also rewrite `Mov(M, Data|ZP)`
to `Mov(Reg(X), Data|ZP)`. The single mem-to-mem atom becomes a
single `STX __zpabi_callee_slot`. Bonus: the now-redundant `LDA M`
is gone entirely (it was only ever implicit in the emit), so the
fix shrinks the example's `.asm` from 1154 to 1142 lines.

## The lesson

When designing an IR for a target without mem-to-mem moves, you
have two choices:

1. **Atomic mem-to-mem Movs**, with the LDA hidden in the emitter.
   Cleaner IR, fewer atoms. But every pass that reasons about A's
   value, A's liveness, or shapes of the form `LDA; STA` has to
   know that some Movs are mem-to-mem and behave differently.

2. **Always two atoms**: `Mov(src, Reg(A))` then `Mov(Reg(A), dst)`.
   The IR mirrors the machine. Peepholes see what's there. But the
   IR is more verbose, and you pay a copy-prop pass to elide the
   intermediate `Reg(A)` when it's unused.

c6502 picked option 1. It's fine for most passes, but every
post-coloring peephole that matches on register reads has to
remember the hidden LDA. I have a memory note titled "mem-to-mem
Mov hides emit-time LDA" — and yet I still wrote v1 of this pass
without consulting it.

Worth pausing on: does the same gotcha apply to your IR? If your
6502 compiler / assembler / disassembler has any "shorthand"
atom that represents multiple opcodes, audit every pass that
matches on opcode sequences to confirm it sees through the
shorthand.

Fix is one new pass (`passes/x_save_slot_load.py`), about 150
lines including the docstring, eight unit tests, and a
regression sim-diff test on the example. Net `.asm` is shorter
than before. End-to-end test suite: 2585 passing.
