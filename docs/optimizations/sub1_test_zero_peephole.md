# Subtract-and-test fusion

`apply_sub1_test_zero_peephole` in
[passes/sub1_test_zero_peephole.py](../../passes/sub1_test_zero_peephole.py)
drops the separate test against zero in a post-decrement loop test
such as `i-- > 0`, and branches on the carry that the subtract has
already set. It runs in two places. The asm optimizer
(`_optimize_function` in
[passes/optimization_asm/optimizer.py](../../passes/optimization_asm/optimizer.py))
runs it on each function before asm SSA construction, which happens
only in the optimized pipeline. It runs again in the peephole fixed
point, in both pipelines.

For `for (uint8_t i = N; i-- > 0; )`, the loop test reads the old
`i` twice, once to compute `i - 1` and once to compare `i` with zero.
`tac_to_asm` lowers the comparison as `LDA #0; CMP i` followed by a
branch on C, and after that `CMP`, C is set exactly when `i` is
zero. The `SBC #1` before it has already set C to the opposite
value, since it borrows exactly when `i` is zero. So the pass removes
the `LDA #0; CMP i` and inverts the branch, from `BCS` to `BCC` or
the reverse. That saves 4 to 5 bytes and 5 to 6 cycles per loop
test, depending on whether `i` is in zero page.

The pre-SSA run matters more than the saving itself. The register
allocator can't put a value in X or Y if it's the right operand of a
`Compare`, because `CPX` and `CPY` take only memory or an immediate
there. Once the `CMP i` is gone, the counter can go in X, and the
whole loop test becomes `TXA; SEC; SBC #1; TAX; BCC`.

## Example

```c
#include <stdint.h>
static uint8_t buf[16];
void clear(void) {
    for (uint8_t i = 16; i-- > 0; ) buf[i] = 0;
}
```

The listings are the pre-SSA run's input and output, so the operands
are still pseudoregisters.

### Before

```
Label(.loop@0_start)
Mov(Pseudo(@0.i.2, 0), A)
SetCarry
Sub(Imm(1), A)
Mov(A, Pseudo(%2.1, 0))
Mov(Imm(0), A)
Compare(A, Pseudo(@0.i.2, 0))
Branch(CS, .loop@0_break)
Mov(Pseudo(%2.1, 0), A)
...
```

### After

```
Label(.loop@0_start)
Mov(Pseudo(@0.i.2, 0), A)
SetCarry
Sub(Imm(1), A)
Mov(A, Pseudo(%2.1, 0))
Branch(CC, .loop@0_break)
Mov(Pseudo(%2.1, 0), A)
...
```

The register allocator then puts `i` in X. The function compiles to
`LDX #$10`, and a loop head of `TXA; SEC; SBC #$01; TAX; BCC
.loop@0_break`.

## When it applies

The pass matches seven consecutive atoms.

```
Mov(M, A)
SetCarry
Sub(Imm(1), A)
Mov(A, dst)
Mov(Imm(0), A)
Compare(A, M)
Branch(CC | CS, target)
```

The two M operands must be the same `Data`, `ZP`, or `Pseudo`
operand, and the load mustn't be volatile. The pass accepts `Pseudo`
so that the pre-SSA run can fire. `dst` can be anything except M,
and a store to `dst` doesn't change C. The match is replaced by its
first four atoms and the branch with its condition inverted.

## Two positions

The pre-SSA run is the one that fires in practice. It was added after
the fixed-point copy, to remove the `Compare` before the register
allocator checks which values can go in X or Y.

The copy in the peephole fixed point is the original position. Its
operands are concrete after pseudoregister replacement, and would
catch the shape if it only became consecutive after register
allocation or after another peephole. It made no change on any
`examples/` program or any program under `tests/chapter_*/valid/`,
in either pipeline. In the optimized pipeline the pre-SSA run has
already removed every match. In the unoptimized pipeline the shape
never appears, as described below.

## What it leaves alone

- An in-place decrement, where `dst` is M. That's the job of
  [DEC](dec_peephole.md).
- A branch on Z or N after the compare. After `LDA #0; CMP M`, Z is
  set when M is zero, but after `SBC #1` it's set when M is 1, so
  only the carry maps across.
- The unoptimized pipeline's loop tests. The pass reports 0 changes
  on the example under `--unopt`. There `i` lives on the soft stack,
  as a `Frame` operand the pass doesn't accept, and `i-- > 0` is
  compared at `int` width with a signed compare sequence, because the
  narrowing of the comparison is done by the TAC optimizer.

The pass doesn't check whether A, N, or Z are read after the branch.
Before the rewrite A holds 0 there and N and Z come from the `CMP`.
After it, A holds `M - 1` and N and Z come from the `SBC`. The code
that `tac_to_asm` emits after a conditional jump loads each value it
uses, so the shape as lowered doesn't read them. An earlier rewrite
that relied on A being 0 after the branch, such as a load of `#0`
dropped by [redundant load elimination](redundant_load.md), would
leave a read of A that this pass then changes.

## Related

[DEC](dec_peephole.md) handles the in-place form, and
[CPX and CPY](cpx_cpy_peephole.md) compares a counter that's already
in X or Y.
