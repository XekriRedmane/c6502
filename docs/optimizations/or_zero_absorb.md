# OR-with-zero absorb

`absorb_zero_load` in
[passes/optimization_asm/or_zero_absorb.py](../../passes/optimization_asm/or_zero_absorb.py)
rewrites `LDA #0; ORA x` as `LDA x`. It runs last in each round of
the asm SSA fixed point inside `optimize_program`, after
`byte_dce`, so it runs before move coalescing and register
allocation. It runs only under `--optimize`.

Building a 16-bit value as `((uint16_t)hi << 8) | lo` lowers the low
byte of the shift to the constant 0 and then ORs the zero-extended
`lo` into it. After [asm copy propagation](asm_copy_propagation.md)
the low byte reads `LDA #0; ORA lo`. `LDA lo` leaves the same value
in A and sets N and Z the same way, and neither form touches C or V.
The rewrite saves 2 bytes and 2 cycles directly. It also turns the
low byte into a plain copy through A, which
[move coalescing](coalescing.md) can merge so that both ends share
one zero-page byte.

## Example

```c
unsigned char *base;
unsigned char *entry(void) {
    unsigned int off = ((unsigned int)base[0] << 8) | base[1];
    return base + off;
}
```

### Before

```
...
Mov(Imm(1), Y)
Mov(IndirectY, A)
Mov(A, Pseudo(%5.1.b0.v1, 0))
Mov(Imm(0), A)
Or(Pseudo(%5.1.b0.v1, 0), A)
Mov(A, Pseudo(%9.1.b0.v1, 0))
Mov(Pseudo(%1.1.b0.v1, 0), A)
Or(Imm(0), A)
Mov(A, Pseudo(%9.1.b1.v1, 0))
...
```

### After

```
...
Mov(Imm(1), Y)
Mov(IndirectY, A)
Mov(A, Pseudo(%5.1.b0.v1, 0))
Mov(Pseudo(%5.1.b0.v1, 0), A)
Mov(A, Pseudo(%9.1.b0.v1, 0))
Mov(Pseudo(%1.1.b0.v1, 0), A)
Or(Imm(0), A)
Mov(A, Pseudo(%9.1.b1.v1, 0))
...
```

The final code loads `base[0]` and `base[1]` into two zero-page
bytes and adds them to `base` with no copying in between. With the
pass turned off, the function has an extra `LDA __local_entry__2;
STA __local_entry__0` before the add and a third zero-page byte in
its private pool.

## When it applies

The pass matches two adjacent instructions, a non-volatile
`Mov(Imm(0), A)` followed by `Or(x, A)` for any operand `x`, and
replaces them with a non-volatile `Mov(x, A)`. Pairs are matched left
to right without overlap.

## What it leaves alone

- The mirrored form `LDA x; ORA #0`, which builds the high byte in
  the example. [Constant arithmetic folding](const_arith_fold.md)
  drops that `ORA #0` in the peephole loop. Dropping it here is
  unsafe, because at the asm SSA level an `ORA #0` can be the
  instruction that sets N and Z from A for a branch after a join. The
  module docstring gives the code for `!` as the case.
- An `LDA #0` and `ORA` with any instruction or label between them.
- Any other constant in the `LDA`, since `ORA` with a nonzero value
  changes the result.
