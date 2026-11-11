# Byte-granular dead-code elimination

`byte_dce` in
[passes/optimization_asm/byte_dce.py](../../passes/optimization_asm/byte_dce.py)
drops each `Mov` or `Phi` that writes a byte of a temporary nothing
reads. It runs in the asm SSA fixed point inside `optimize_program`,
after the two copy propagations in each round, and repeats internally
until it changes nothing. It runs only under `--optimize`.

After asm SSA construction, each byte of a multi-byte value is a
separate name, so the pass can find a dead high byte of a value whose
low byte is live. TAC-level [dead store elimination](dead_store_elimination.md)
works on whole values and keeps the value if any part is read. Dropping
the store before register allocation keeps the byte out of the
interference graph, so it never takes a zero-page byte. The pass also
removes the copies that [asm copy propagation](asm_copy_propagation.md)
leaves with no reads.

## Example

```c
unsigned char out;
void low_sum(unsigned int a, unsigned int b) {
    unsigned int s = a + b;
    out = (unsigned char)s;
}
```

### Before

```
Label(.low_sum@asm_ssa_block@0)
Mov(Pseudo(@0.a, 0), A)
ClearCarry
Add(Pseudo(@1.b, 0), A)
Mov(A, Pseudo(%0.1.b0.v1, 0))
Mov(Pseudo(@0.a, 1), A)
Add(Pseudo(@1.b, 1), A)
Mov(A, Pseudo(%0.1.b1.v1, 0))
Mov(Pseudo(%0.1.b0.v1, 0), Pseudo(out, 0))
Return(False)
```

### After

```
Label(.low_sum@asm_ssa_block@0)
Mov(Pseudo(@0.a, 0), A)
ClearCarry
Add(Pseudo(@1.b, 0), A)
Mov(A, Pseudo(%0.1.b0.v1, 0))
Mov(Pseudo(@0.a, 1), A)
Add(Pseudo(@1.b, 1), A)
Mov(Pseudo(%0.1.b0.v1, 0), Pseudo(out, 0))
Return(False)
```

The load and add of the high bytes stay, because their destination is
A. The peephole [dead A arithmetic](dead_a_arith.md) pass drops them
later, and the function ends as `LDA a; CLC; ADC b; STA out; RTS`.
With `byte_dce` turned off, the peephole passes reach the same code,
but the function's private pool holds two zero-page bytes where it
holds one with the pass.

## When it applies

An instruction is dropped when it is a `Mov` or a `Phi` and all of
these are true.

- The `Mov` isn't volatile.
- Its destination is a `Pseudo` whose name isn't a static and isn't
  in the set the SSA construction leaves unrenamed. That set holds
  address-taken names, `LoadAddress` destinations, `INC`, `DEC`, and
  shift targets, DPTR-staged pointers, and names volatile `Mov`s
  touch.
- No instruction in the function reads that `(name, offset)`. A read
  is any source operand, either operand of `Add`, `Sub`, `And`, `Or`,
  or `Compare`, the operand of `Inc`, `Dec`, or a shift or rotate,
  the source of a `LoadAddress`, or a `Phi` argument.

The test is by name across the whole function. Under SSA each name
has one definition, so a name with no reads anywhere is dead
everywhere.

## What it leaves alone

- Every other kind of definition. A `LoadAddress` stays because its
  source must still have a storage location, and a `Pop` stays
  because it moves the stack pointer. Arithmetic and loads into A
  stay because the pass treats registers as always live.
- A store to a static, which another function can read.
- A store to an address-taken object. In a function that sets
  `buf[1] = 0` on a local array and then passes `buf` to `fill`, no
  instruction reads byte 1 of `buf` by name, since `fill` reads it
  through the pointer. The pass reports no change there.
- A volatile store.

## Related

[Asm dead store elimination](asm_dead_store.md) is the CFG-wide pass
over concrete memory operands in the peephole loop.
[Backward copy propagation](backward_copy_propagation.md) and
[asm copy propagation](asm_copy_propagation.md) run with this pass in
the same fixed point.
