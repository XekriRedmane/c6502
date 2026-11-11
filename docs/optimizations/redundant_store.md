# Redundant store elimination

`apply_redundant_store_elimination` in
[passes/redundant_store.py](../../passes/redundant_store.py) drops a
copy `LDA S; STA D` when D already holds the value of S, because the
same copy ran earlier in the block and nothing since has written S
or D. It runs in the peephole fixed point, in both the optimized and
the unoptimized pipelines, right after
[redundant load elimination](redundant_load.md).

The common source is pointer staging. Each dereference of a pointer
in memory copies the pointer's two bytes into `DPTR` before reading
`(DPTR),Y`, and two dereferences of the same pointer stage it twice.
The read through `(DPTR),Y` overwrites A between the stagings, so
the redundant load pass, which tracks what each register holds,
finds no register that still holds a pointer byte. This pass
instead records which source cell each destination cell was last
copied from, and overwriting A doesn't change that record. For a
pointer in absolute memory, the second staging is 10 bytes and 14
cycles.

## Example

```c
#include <stdint.h>
uint8_t *p;
uint8_t a, b;
void copy2(void) {
    a = *p;
    b = *p;
}
```

### Before

```
   LDA   p
   STA   DPTR
   LDA   p+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   a
   LDA   p
   STA   DPTR
   LDA   p+1
   STA   DPTR+1
   LDA   (DPTR),Y
   STA   b
   RTS
```

### After

```
   LDA   p
   STA   DPTR
   LDA   p+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   a
   LDA   (DPTR),Y
   STA   b
   RTS
```

`STA a` writes a static in the data segment, which can't be `DPTR`
or `p`, so the store leaves both records in place.

When A is read after the copy, the pass drops only the `STA` and
keeps the `LDA`. The unit test
`test_load_kept_when_a_is_read_after_the_pair` in
[tests/test_redundant_store.py](../../tests/test_redundant_store.py)
has this shape, where the copy reloads the minuend of a following
`SBC`.

## When it applies

- The copy is a `Mov` of memory into A followed directly by a `Mov`
  of A into memory, neither volatile.
- Both cells have fixed addresses, which means `ZP` or `Data`
  operands. A runtime symbol (`SSP`, `FP`, `HARGS`, `DPTR`) resolves
  to its zero-page address, so `Data(DPTR)` and the matching `ZP`
  byte are the same cell. Any other `Data` name is a cell of its
  own, assumed to overlap no other name and no zero-page byte.
- An earlier copy in the same basic block wrote S into D, and no
  instruction since may have written S or D.
- N and Z are dead after the copy (`asm_liveness.flags_dead_at`),
  since dropping the `LDA` skips setting them. The pass checks this
  even when it keeps the `LDA`.

A store or read-modify-write on a `ZP` or `Data` cell removes the
records that name it. A store through `abs,X` or `abs,Y` whose base
is a raw address or a runtime symbol removes the records of bytes
within 256 bytes of that base, along with every record that names a
static. When the base is a static, whose address is set at link
time, the store clears every record. `PHA` and `PLA` touch only page
1 and leave the records alone.

## What it leaves alone

- A copy in another basic block. Every label, branch, jump, return,
  and `JSR` clears the records. In `a = *p; if (c) b = *p;` the
  second staging follows a `BEQ`, so the pass reports no change.
- A copy after a store through `(FP),Y`, `(SSP),Y`, or `(DPTR),Y`,
  whose target is a runtime address, so the store clears the
  records. In the unoptimized pipeline every temporary lives in the
  soft-stack frame, so the pass fires there only between such
  stores. `a = b; c = d; a = b;` on file-scope statics loses its
  second `LDA b; STA a` in both pipelines.
- A copy from or to a soft-stack slot, a pointer target, or an
  indexed address. The byte these name depends on a pointer or an
  index register.
- A copy whose `LDA` sets flags that a branch reads. The `STA` could
  go in that case, but the pass keeps both.
- A copy that isn't the `LDA; STA` pair, such as `LDX S; STX D` or a
  volatile memory-to-memory `Mov` that
  [mem-to-mem splitting](split_mem_to_mem.md) leaves whole.

## Related

[Redundant load elimination](redundant_load.md) removes loads into a
register that already holds the value, and
[asm dead store elimination](asm_dead_store.md) removes stores that
nothing reads.
