# Asm copy propagation

`copy_propagate` in
[passes/optimization_asm/copy_propagation.py](../../passes/optimization_asm/copy_propagation.py)
replaces each read of a byte that a direct `Mov` copied with the
`Mov`'s source. It runs in the asm SSA fixed point inside
`optimize_program`, which repeats `copy_propagate`,
`backward_copy_propagate`, `byte_dce`, and `absorb_zero_load` until a
round changes nothing. It runs only under `--optimize`.

After asm SSA construction, each byte of a temporary has exactly one
definition, so a temporary written by `Mov(src, t)` holds `src` at
every read of `t`. Most temporaries are written through A, but some
lowerings write one directly. A zero extension of an `unsigned char`
copies the source byte into the low byte of the wide temporary and
`Imm(0)` into the high byte. Reading the source in place of the
temporary saves the copy and the temporary's zero-page byte, and the
`Mov` left behind has no reads, so [byte DCE](byte_dce.md) drops it
in the same round.

## Example

```c
unsigned int widen(unsigned char x, unsigned int y) { return y - x; }
```

### Before

```
Label(.widen@asm_ssa_block@0)
Mov(Pseudo(@0.x, 0), Pseudo(%1.1.b0.v1, 0))
Mov(Imm(0), Pseudo(%1.1.b1.v1, 0))
Mov(Pseudo(@1.y, 0), A)
SetCarry
Sub(Pseudo(%1.1.b0.v1, 0), A)
Mov(A, Pseudo(%2.1.b0.v1, 0))
Mov(Pseudo(@1.y, 1), A)
Sub(Pseudo(%1.1.b1.v1, 0), A)
Mov(A, Pseudo(%2.1.b1.v1, 0))
...
```

### After

```
Label(.widen@asm_ssa_block@0)
Mov(Pseudo(@0.x, 0), Pseudo(%1.1.b0.v1, 0))
Mov(Imm(0), Pseudo(%1.1.b1.v1, 0))
Mov(Pseudo(@1.y, 0), A)
SetCarry
Sub(Pseudo(@0.x, 0), A)
Mov(A, Pseudo(%2.1.b0.v1, 0))
Mov(Pseudo(@1.y, 1), A)
Sub(Imm(0), A)
Mov(A, Pseudo(%2.1.b1.v1, 0))
...
```

The final code is `LDA y; SEC; SBC x; STA t; LDA y+1; SBC #$00; TAX;
LDA t; RTS`. With the pass turned off, the function also has `LDA
__zpabi_widen__x; STA __local_widen__0` in front, which is 4 bytes
and 6 cycles more, and its private pool takes a second zero-page
byte. The peephole passes recover the `SBC #$00` on their own, but
not the copy of `x`.

## When it applies

A `Mov(src, dst)` counts as a copy when both of these are true.

- `dst` is a `Pseudo` whose name is not a static and is not in the
  set the SSA construction leaves unrenamed
  (`excluded_pseudo_names`). That set holds address-taken names, the
  destinations of `LoadAddress`, the targets of `INC`, `DEC`, and
  in-place shifts, pointers staged into DPTR byte by byte, and names
  any volatile `Mov` touches.
- `src` is an `Imm`, an `ImmLabelLow`, an `ImmLabelHigh`, or a
  `Pseudo` that passes the same name test.

Chains of copies are followed to their first source. The pass then
substitutes every read of a copied `(name, offset)`. A read is the
source of a `Mov` or `Push`, either operand of `Add`, `Sub`, `And`,
`Or`, or `Compare`, a source of `Xor`, the operand of `Inc`, `Dec`,
or a shift or rotate, and each argument of a `Phi`, so a constant or
a parameter can flow into a loop's `Phi`. Definitions keep their
names.

## What it leaves alone

- A copy from memory a later instruction or call can change, which
  is any static, `Data`, `Stack`, `Frame`, `ZP`, or `Indirect`
  source, or from a register. In `total = total + c` with a static
  `unsigned char c`, the temporary that holds `c` stays and only its
  `Imm(0)` high byte is propagated.
- A copy through A, `Mov(p, A); Mov(A, q)`. The pass doesn't follow
  A. [Backward copy propagation](backward_copy_propagation.md) and
  [move coalescing](coalescing.md) handle these pairs.
- A `Phi`, which merges several values and so isn't a copy of one.
- The source of a `LoadAddress`, which names a storage cell and has
  no value to substitute.

## Related

[Copy propagation](copy_propagation.md) is the TAC-level pass this
one mirrors. [Byte DCE](byte_dce.md) removes the copies this pass
leaves unread.
