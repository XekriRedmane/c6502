# Memory-to-memory move splitting

`apply_split_mem_to_mem` in
[passes/split_mem_to_mem.py](../../passes/split_mem_to_mem.py)
rewrites a `Mov` whose source and destination are both memory as a
load into A followed by a store from A. It runs first in each
iteration of the peephole fixed point, in both the optimized and the
unoptimized pipelines.

The asm IR lets one `Mov` atom copy memory to memory, and `asm_emit`
prints it as `LDA src; STA dst`, since the 6502 has no memory-to-memory
move. The split changes nothing in the emitted code by itself. It
changes which atoms the other peepholes match. While the load is hidden
inside the compound atom, [redundant load
elimination](redundant_load.md) can't drop it when A already holds the
source, and [asm dead-store elimination](asm_dead_store.md) can't drop
just the store. Once the two halves are separate atoms, every pass in
the fixed point treats them like any other load and store.

A self-move `Mov(M, M)` is dropped outright. `asm_emit` already prints
nothing for it, and splitting it would produce a useless `LDA M; STA
M`. SSA destruction leaves self-moves behind when a phi's source and
destination are coalesced into the same byte.

## Example

```c
static unsigned char a, b, c;
void copy(void) { a = b; c = b; }
```

The emitted text is the same before and after the split, so the
listings are in the IR's constructor notation.

### Before

```
Label(.copy@asm_ssa_block@0)
Mov(Data(b, 0), Data(a, 0))
Mov(Data(b, 0), Data(c, 0))
Return(False)
```

### After

```
Label(.copy@asm_ssa_block@0)
Mov(Data(b, 0), A)
Mov(A, Data(a, 0))
Mov(Data(b, 0), A)
Mov(A, Data(c, 0))
Return(False)
```

Later in the same iteration, redundant load elimination drops the
second `Mov(Data(b, 0), A)` because A still holds `b`. The function
ends up as `LDA b; STA a; STA c; RTS`, which is 3 bytes and 4 cycles
shorter than the two compound moves. The unoptimized pipeline produces
the same moves for statics and the same final code.

## When it applies

The pass rewrites every `Mov` that isn't volatile and whose source and
destination are both one of `Data`, `ZP`, `Frame`, `Stack`,
`Indirect`, `IndirectY`, `IndirectZp`, `IndirectZpY`, and
`IndexedData`. A source equal to the destination, field for field,
removes the atom instead.

Running at the top of the fixed point matters because
[memory value propagation](memory_value_propagation.md) can create a
new memory-to-memory move when it substitutes a tracked source into a
move. The next iteration splits that move before the other passes run
on it.

## What it leaves alone

- A volatile memory-to-memory move. The atom's `is_volatile` bit is
  set when either operand is volatile, and it doesn't record which
  one. Splitting it would mark both halves volatile, and redundant
  load elimination never drops a volatile load. That pass already
  handles the compound volatile move by recording the source as held
  in A. For `a = port; a = port;` with a `volatile` `port`, the pass
  reports 0 changes.
- A move from an immediate or a label half (`Mov(Imm, mem)`,
  `Mov(ImmLabelLow, mem)`). It's also printed as a load and a store,
  but [rematerialization](asm_remat.md) and memory value propagation
  match it more easily in its compound form.
- A move with a register on either side, which is already one
  instruction.

The module docstring says the `is_volatile` bit is copied to both
halves of the split. Since a volatile move is never split, the copied
bit is always false.

## Related

[Via-A store fold](via_a_store_fold.md) and [AND sign-bit
branch](and_sign_bit_branch.md) each recover a shape that the split
breaks apart.
