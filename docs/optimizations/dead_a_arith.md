# Dead A arithmetic

`apply_dead_a_arith_elimination` in
[passes/dead_a_arith.py](../../passes/dead_a_arith.py) deletes an
instruction whose only effect is a write to A and the flags, when
nothing reads that A value or those flags afterward. It runs in the
peephole fixed point, in both the optimized and the unoptimized
pipelines. Across the chapter test corpus and `examples/` it has
fired on one program in the unoptimized pipeline, because that
pipeline stores almost every value it computes to the soft stack, and
the store reads A.

Integer promotion makes `tac_to_asm` compute the high byte of `int`
arithmetic on byte-sized values. When the result is truncated back to
a byte, the asm SSA passes delete the store of the high byte, and the
arithmetic that fed it is left computing a value that nothing reads.
Each such `LDA`, `EOR`, or `ADC` with an immediate or zero-page
operand costs 2 bytes and 2 or 3 cycles.

## Example

```c
#include <stdint.h>
int8_t s;
void neg(void) { s = -s; }
```

### Before

```
   LDA   s
   STA   __local_neg__0
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_neg__1
   LDA   __local_neg__0
   EOR   #$FF
   CLC
   ADC   #$01
   STA   __local_neg__0
   LDA   __local_neg__1
   EOR   #$FF
   ADC   #$00
   LDA   __local_neg__0
   STA   s
   RTS
```

### After

```
   LDA   s
   STA   __local_neg__0
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_neg__1
   LDA   __local_neg__0
   EOR   #$FF
   CLC
   ADC   #$01
   STA   __local_neg__0
   LDA   __local_neg__0
   STA   s
   RTS
```

The three instructions go in one call. The pass repeats its sweep
until nothing changes, so removing `ADC #$00` leaves `EOR #$FF` with a
dead result, and removing that leaves `LDA __local_neg__1` with one.
After `asm_dead_store` deletes `STA __local_neg__1`, a later call
deletes the `LDA #$00` and `LDA #$FF` of the sign extension, and the
function ends up as `LDA s; EOR #$FF; CLC; ADC #$01; STA s; RTS`.

## When it applies

- The instruction is one of `LDA`, `TXA`, `TYA`, `ADC`, `SBC`, `AND`,
  `ORA`, or `EOR` writing A, or `ASL`, `LSR`, `ROL`, or `ROR` on A. A
  memory operand must be immediate, a label's low or high byte,
  absolute, zero page, or absolute indexed (`Imm`, `ImmLabelLow`,
  `ImmLabelHigh`, `Data`, `ZP`, or `IndexedData`). An `LDA` must not
  be volatile.
- A is dead after it. The walk (`asm_liveness.a_dead_at`) follows
  both arms of every branch and the target of every jump, and A is
  dead on a path that writes A without reading it, or returns without
  a value in A, before anything reads A.
- For `ADC`, `SBC`, and the shifts, which also write C, every flag is
  dead (`all_flags_dead_at`). That walk fails at any branch, at a
  later `ADC`, `SBC`, `ROL`, or `ROR`, and at a carry-flag return. For
  the other instructions only N and Z have to be dead
  (`flags_dead_at`), and that walk fails at any branch.
- A `TXA` or `TYA` whose flags a branch reads can still go when the
  last flag-setting instruction before it, in the same block, wrote
  the same register. In `LDX m; TXA; BMI L` the `LDX` already set N
  and Z from X, so `LDX m; BMI L` branches the same way.

The pass also drops a load into X or Y (`LDX` / `LDY` of an
immediate or a non-volatile `Data` or `ZP` byte) when the register
and N/Z are dead after it (`x_dead_at` / `y_dead_at`,
`flags_dead_at`). Such a load is left when a constant stored through
Y by [redundant load elimination](redundant_load.md) loses its store
to a later pass.

The sweep repeats inside one call because another pass in the same
round, such as [volatile read to CMP](volatile_void_read_cmp.md), can
make A live across an instruction this pass would otherwise have
removed on its next call.

## What it leaves alone

- A volatile load, since the read itself is the effect of a volatile
  access.
- An operand on the soft stack or behind a pointer (`Frame`, `Stack`,
  and the `Indirect` forms). The emitted code for those starts with
  `LDY #off`, which changes Y.
- An instruction whose N or Z a branch reads, apart from the `TXA` /
  `TYA` case above. The backward search for that case stops at a
  label, a branch, a jump, a return, a call, and the compound
  `LoadAddress`, `FunctionPrologue`, and `AllocateStack` atoms.
- An `ADC` or `SBC` whose carry feeds the next byte of a multi-byte
  add or subtract, as in `g = a + 1;` with an `int g`, where
  `LDA #$00; ADC #$00; STA g+1` stays.
- Instructions that write memory, and ones that write X or Y other
  than the loads above.

## Related

[Asm dead store](asm_dead_store.md) removes the stores whose removal
lets this pass fire. [Byte DCE](byte_dce.md) removes dead byte
computations earlier, while the asm is still in SSA form.
[TXA past a branch](transfer_past_branch.md) moves a `TXA` or `TYA`
when A is dead on only one arm of the branch after it.
