# Memory constant propagation

`apply_mem_const_prop` in
[passes/mem_const_prop.py](../../passes/mem_const_prop.py) tracks
which memory bytes hold a known constant within a basic block, and
replaces a later read of such a byte with the constant as an
immediate operand. It runs in the peephole fixed point, in both the
optimized and the unoptimized pipelines, just before [constant AND
and ORA fold](const_arith_fold.md). Across the chapter test corpus it
fired on one program in the unoptimized pipeline, because every store
to the soft stack there makes it forget everything it tracked.

`LDA #$2C` takes 2 bytes and 2 cycles, against 3 bytes and 4 cycles
for an absolute `LDA count`, or 2 bytes and 3 cycles from zero page.
The larger effect is on later passes. An immediate can be folded by
[constant AND and ORA fold](const_arith_fold.md), and a store whose
every read has been replaced becomes dead for [asm dead
store](asm_dead_store.md).

## Example

```c
#include <stdint.h>
int count;
uint8_t g;
void reset(void)
{
    int *p = &count;
    count = 300;
    if (*p != 300)
        g = 1;
}
```

### Before

```
   LDA   #$2C
   STA   count
   LDA   #$01
   STA   count+1
   LDA   count
   STA   __local_reset__1
   LDA   count+1
   CMP   #$01
   BNE   .jcmp_skip@0
   LDA   __local_reset__1
   CMP   #$2C
   BEQ   .if_end@0
.jcmp_skip@0:
   LDA   #$01
   STA   g
.if_end@0:
   RTS
```

### After

```
   LDA   #$2C
   STA   count
   LDA   #$01
   STA   count+1
   LDA   #$2C
   STA   __local_reset__1
   LDA   #$01
   CMP   #$01
   BNE   .jcmp_skip@0
   LDA   __local_reset__1
   CMP   #$2C
   BEQ   .if_end@0
.jcmp_skip@0:
   LDA   #$01
   STA   g
.if_end@0:
   RTS
```

The `BNE` ends the block, so this pass leaves `LDA __local_reset__1`
alone. In the final code a later pass has replaced it with `LDA #$2C`
too. The compares of two constants stay, since no asm pass folds a
branch on them.

## When it applies

The pass walks each function forward and keeps two pieces of state,
the value of A when it's a known constant and the value of each
tracked byte.

- `LDA #k` makes A known. `AND #k` and `ORA #k` keep it known when it
  was. Any other write to A makes it unknown.
- A store of a known A, or of an immediate, to a `Data` or `ZP` byte
  records that byte's value. A store from X, Y, or an unknown A
  forgets it.
- A read of a recorded byte is replaced by its value in a
  non-volatile `LDA`, in the source of `ADC`, `SBC`, `AND`, or `ORA`,
  and in the right operand of `CMP`, `CPX`, or `CPY`.

Everything is forgotten at a label, a branch, a jump, a return, a
call, and the compound `LoadAddress`, `FunctionPrologue`, and
`AllocateStack` atoms. A write through an indexed `name,X` or
`name,Y` operand forgets every byte of `name`. A write through the
soft stack or a pointer forgets everything, and so does an `INC`,
`DEC`, or shift of anything other than A or a `Data` or `ZP` byte,
which includes `INX` and `DEY`.

## What it leaves alone

- A read in another block, as with `LDA __local_reset__1` above.
- A read after a call, such as `count = 300; tick(); if (*p != 300)`,
  since the callee could write any byte.
- A volatile object. A volatile load always reads memory, and a
  volatile store doesn't make the byte's value known.
- A byte whose value came from `ADC`, `SBC`, or `EOR`, which the pass
  doesn't evaluate.

## Related

[Memory value propagation](memory_value_propagation.md) does a
CFG-wide version for zero-page bytes and the values they mirror.
[Constant AND and ORA fold](const_arith_fold.md) folds the immediates
this pass produces.
