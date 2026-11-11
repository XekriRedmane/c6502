# Direct load into X or Y

`apply_direct_index_load` in
[passes/direct_index_load.py](../../passes/direct_index_load.py)
rewrites a load into A followed by `TAX` or `TAY` as a single `LDX` or
`LDY`, when nothing reads A afterwards. It runs in the peephole fixed
point, in both the optimized and the unoptimized pipelines.

`tac_to_asm` always moves a value into X or Y through A, as `LDA M;
TAX`. When it lowers the code, M is still a pseudoregister that might
end up on the soft stack, where only `(FP),Y` can reach it, and `LDX`
and `LDY` have no such mode. After pseudoregister replacement M is
concrete, and when it's an immediate, an absolute address, or a
zero-page byte, `LDX` or `LDY` can load it directly. That saves the
1-byte, 2-cycle transfer each time. `LDX M` sets N and Z from M, the
same as `LDA M; TAX`, so a branch afterwards reads the same flags.

The most common source is a 2-byte return value, whose high byte goes
in X, but the pair also appears before an indexed access, such as
`buf[i]`, wherever the asm optimizer hasn't already loaded the index
directly.

## Example

```c
static int score;
int get(void) { return score; }
```

### Before

```
   LDA   score+1
   TAX
   LDA   score
   RTS
```

### After

```
   LDX   score+1
   LDA   score
   RTS
```

The unoptimized pipeline produces the same pair for this function and
the same result. There, it also fires on the high byte of every
constant `int` return, where `LDA #$00; TAX` becomes `LDX #$00`.

## When it applies

The two instructions must be consecutive, a `Mov(M, A)` followed by a
`Mov(A, X)` or `Mov(A, Y)`. M must be an `Imm`, `ImmLabelLow`,
`ImmLabelHigh`, `Data`, or `ZP` operand. A must be dead after the
transfer along every path, which `asm_liveness.a_dead_at` checks by
walking forward through branches and jumps until each path writes A
without reading it.

A volatile load stays volatile. The rewritten `LDX M` is the same one
memory access as the `LDA M`, so the pass copies the volatile bit.

## What it leaves alone

- A load from the soft stack or through a pointer. For `int get(int
  x) { return x; }` under `--unopt`, the high byte of `x` is `LDY
  #$04; LDA (FP),Y; TAX`, and the pass reports 0 changes.
- A pair where A is read afterwards, since the `LDX` form never puts
  the value in A. This case is in the unit test
  `test_a_live_after_does_not_fold`.
- A load into A that a different instruction separates from the
  transfer.

## Related

[Redundant load elimination](redundant_load.md) often drops the new
`LDX` when X already holds the value, and [PHA and PLA
removal](dead_pha_pla.md) can remove a save of A that this rewrite
has made unnecessary.
