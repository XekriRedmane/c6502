# Loop constant in A

`apply_loop_constant_a` in
[cpp/src/passes/loop_constant_a.h](../../cpp/src/passes/loop_constant_a.h)
loads a constant into A once, in front of a loop, when every write of
A in the loop is a load of that constant or a `TXA` / `TYA` whose
value nothing reads. It removes the loads and the transfers from the
loop. It runs in the peephole fixed point, right before
[dead A arithmetic](dead_a_arith.md), in both pipelines.

The loop that clears an automatic aggregate's bytes
(`emit_zero_bytes` in cpp/src/c99_to_tac_lvalues.cpp) is the case it was written for. It
counts a byte index down in Y, loads 0 into A for the store in each
pass, and tests Y with `TYA; BNE` at the bottom. `dead_a_arith`
drops a `TYA` when the flags already reflect Y, which `DEY` set, but
the `LDA #0` sets them again between the two. The `LDA #0` in turn
can't leave the loop as long as the `TYA` overwrites A. So this pass
takes both steps at once. The loop comes out a byte smaller and 4
cycles a byte faster.

## Example

```c
#include <stdint.h>
void use(uint8_t *p);
void f(void) {
    uint8_t buf[20] = {};
    use(buf);
}
```

### Before

```
f:
   LDY   #$14
.clear@0:
   DEY
   LDA   #$00
   STA   __local_f__buf,Y
   TYA
   BNE   .clear@0
   ...
```

### After

```
f:
   LDY   #$14
   LDA   #$00
.clear@0:
   DEY
   STA   __local_f__buf,Y
   BNE   .clear@0
   ...
```

The `BNE` reads the flags `DEY` set, since `STA` leaves them alone.

Lode Runner's `editor_clear_level` clears a page with `do
DISK_BUFFER[i] = 0; while (++i != 0);`, an index in X that counts up.
There's no transfer in that loop, and the pass only moves the `LDA
#0` to the front, next to `LDX #0`. There
[redundant load elimination](redundant_load.md) turns it into `TXA`,
a byte shorter.

## When it applies

- The loop runs from its header label to a branch or jump back to
  it, later in the code. Nothing outside the loop branches to the
  header or to a label inside the loop, and the instruction before
  the header isn't a `JMP`, `RTS`, or jump table, so the code before
  the loop runs into the new load.
- Each load is `LDA #k` of one constant `k` (a number or a label's
  half), and N and Z are dead after it.
- A is dead after each `TXA` or `TYA`, and its N and Z are either
  dead or the same as the flags an earlier instruction in the block
  set from the same register, with the loads gone
  (`flags_reflect_src_reg` in cpp/src/passes/dead_a_arith.h).
- Every other instruction in the loop leaves A alone as `asm_emit`
  writes it (`leaves_a`). These are labels, branches, jumps that
  pass no register, `SEC` / `CLC` / `BIT`, compares of A, `PHA`,
  stores of A, X, or Y, loads of X and Y from a constant or a byte,
  and `INC`, `DEC`, and shifts of a register or of a byte that isn't
  indexed by Y.
- A, N, and Z are dead at the header.

After the change A holds `k` everywhere in the loop. Each read of A
used to get its value from one of the loads, since A is dead at the
header and after each transfer, and that value was `k` too. The
flags are the same wherever something reads them. A removed load's
flags were dead, a removed transfer's were already set by the earlier
instruction, and the new load's are dead at the header.

## What it leaves alone

- A loop with any other write of A. In a soft-stack frame, the
  clearing loop computes the object's address in A in every pass
  (`LDA FP; ADC #off`), and the `LDA #0` and `TYA` stay.
- A loop with a call, a pull, or a soft-stack or indirect operand of
  X or Y, all of which count as writing A.
- Loads of two different constants, such as `LDA #0` for one store
  and `LDA #$FF` for another.
- A transfer whose value is read, such as a `TYA` that a store after
  it writes to memory.
