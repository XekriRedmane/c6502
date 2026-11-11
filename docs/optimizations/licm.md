# Loop-invariant extends and truncates

`hoist_loop_invariants` (the `HoistLoopInvariants` pass) in
[passes/optimization/licm.py](../../passes/optimization/licm.py) moves
a `ZeroExtend`, `SignExtend`, or `Truncate` whose operand doesn't
change inside a loop out to the loop's preheader, so it runs once
instead of on every iteration. It runs in the TAC fixed-point loop
under `--optimize`, on SSA form, right after
[common subexpression elimination](cse.md). The unoptimized pipeline
doesn't run it.

The usual source is a byte added to a wider value inside a loop, as in
`entry += size` with a `uint8_t size`. The type checker widens `size`
to the pointer's 2-byte width, so the loop body zero-extends it on
every iteration, and the widened copy has to stay live beside the
byte. Only the extends and `Truncate` are
moved. They have no side effects, so running one before a loop that
would have skipped it changes only the time taken, and they lower to
byte copies plus a zero or sign byte, so moving one out never adds
work. The module docstring records that moving adds as well made
`floor_enemy_advance` two instructions longer, because an add used
once on a conditional path costs a store and a load to hold across the
loop.

## Example

```c
#include <stdint.h>
const uint8_t *find(const uint8_t *entry, uint8_t size, uint8_t key) {
    while (*entry != key)
        entry += size;
    return entry;
}
```

### Before

```
Label(.find@ssa_preheader@0)
Label(.loop@0_continue)
Phi(@0.entry.1, [PhiArg(.find@ssa_preheader@0, @0.entry), PhiArg(.find@ssa_block@0, @0.entry.2)])
Load(@0.entry.1, %0.1)
ZeroExtend(%0.1, %1.1)
ZeroExtend(@2.key, %2.1)
JumpIfCmp(==, %0.1, @2.key, .loop@0_break)
Label(.find@ssa_block@0)
ZeroExtend(@1.size, %4.1)
Binary(+, @0.entry.1, %4.1, %5.1)
Copy(%5.1, @0.entry.2)
Jump(.loop@0_continue)
Label(.loop@0_break)
Ret(@0.entry.1)
```

### After

```
Label(.find@ssa_preheader@0)
ZeroExtend(@2.key, %2.1)
ZeroExtend(@1.size, %4.1)
Label(.loop@0_continue)
Phi(@0.entry.1, [PhiArg(.find@ssa_preheader@0, @0.entry), PhiArg(.find@ssa_block@0, @0.entry.2)])
Load(@0.entry.1, %0.1)
ZeroExtend(%0.1, %1.1)
JumpIfCmp(==, %0.1, @2.key, .loop@0_break)
Label(.find@ssa_block@0)
Binary(+, @0.entry.1, %4.1, %5.1)
Copy(%5.1, @0.entry.2)
Jump(.loop@0_continue)
Label(.loop@0_break)
Ret(@0.entry.1)
```

The extends of the parameters `size` and `key` move out. The extend
of the loaded byte `%0.1` stays, because the load is inside the loop.
The extend of `key` has no reader and is removed later by dead-store
elimination.

For this function, and for every C file in `examples/`, the final
assembly is the same with the pass disabled. The pass moves the
`search_size` extend in `examples/zork_match_dictionary_word.c`, which
was its motivating case, and that function's output is also unchanged
without it.

## When it applies

- The loop is a natural loop whose header has exactly one predecessor
  outside the loop. That predecessor is the preheader, and it must
  have the header as its only successor and contain at least one
  instruction, so the moved code runs only on the way into the loop.
- The instruction is a `ZeroExtend`, `SignExtend`, or `Truncate`
  anywhere in the loop body, and its dst is an SSA value.
- Its operand is a Constant, or an SSA value that no instruction in
  the loop defines. An instruction whose operand was defined by a
  moved instruction becomes movable in turn, so chains move together.

The moved instructions go at the end of the preheader, in front of its
final jump if it has one.

## What it leaves alone

- An add, or any other `Binary`, even when its operands are
  invariant. In `while (n--) sum += a + b;` the `a + b` stays inside
  the loop.
- An extend of a static or an address-taken local, such as `sum += g`
  with a global `uint8_t g`. Those names aren't SSA values, so the pass
  can't show that the loop doesn't change them.
- An extend of a value computed in the loop, such as the loaded byte
  in the example.
- A loop entered from more than one block outside it, or whose
  preheader branches somewhere other than the header.

## Related

[Asm-level LICM](asm_licm.md) hoists constant stores out of loops in
the 6502 assembly. [Common subexpression elimination](cse.md) runs
just before this pass.
