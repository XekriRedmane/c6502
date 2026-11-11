# CMP for a discarded volatile read

`apply_volatile_void_read_cmp` in
[passes/volatile_void_read_cmp.py](../../passes/volatile_void_read_cmp.py)
turns a volatile load through a pointer whose value is thrown away
into a `CMP` of the same byte. It runs in the peephole fixed point, in
both pipelines, but only fires with `--optimize`. The unoptimized
pipeline stores the loaded byte to a soft-stack temporary, and that
store reads A.

`(void)*speaker` is how C spells a read whose only purpose is the
read, such as toggling the Apple II speaker. It has to stay in the
program, so `tac_to_asm` lowers it as `LDA (DPTR),Y`, which overwrites
A. `CMP (DPTR),Y` performs the same memory read and leaves A alone.
Both take 2 bytes and 5 cycles, or 6 when the access crosses a page.
The gain is that A keeps its value across the read, so a value the
code keeps in A doesn't have to be saved and reloaded around it. In
`examples/snd_delay_up.c` the click inside the loop becomes a `CMP`,
and [promote loop-carried to A](promote_loop_carried_to_a.md) then
keeps `pitch` in A across the whole loop, which it does only when
nothing in the loop body writes A.

## Example

```c
#include <stdint.h>
extern const volatile uint8_t *speaker;
void click(void) { (void)*speaker; }
```

### Before

```
   LDA   speaker
   STA   DPTR
   LDA   speaker+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   RTS
```

### After

```
   LDA   speaker
   STA   DPTR
   LDA   speaker+1
   STA   DPTR+1
   LDY   #$00
   CMP   (DPTR),Y
   RTS
```

## When it applies

- A volatile load into A through a pointer, that is, an `Indirect`,
  `IndirectY`, `IndirectZp`, or `IndirectZpY` operand.
- The loaded value is dead (`asm_liveness.a_dead_at`).
- N and Z are dead (`flags_dead_at`), because `CMP` sets them from
  `A - byte` where `LDA` set them from the byte.
- C is dead (`c_dead_at`), because `CMP` writes C and `LDA` leaves it
  alone. A carry chain or a carry-flag return can read the C from
  before the load.

An earlier version accepted any following instruction that wrote A
and the flags, including `AND`, `ORA`, `ADC`, and `CMP`, which also
read A. That turned `while (!(*p & 0x80)) ;` into a loop that tested
the stale A instead of the byte it had read.

## What it leaves alone

- A read whose value is used. In `while (!(*kbd & 0x80)) { }` the
  loop branches on the N flag of `LDA (DPTR),Y`, so the load stays.
- A volatile read of a named object, as in `(void)spk;` with `extern
  volatile uint8_t spk;`. It stays `LDA spk`, since the pass matches
  only the pointer operand forms.
- A read followed by an instruction that reads C before anything
  writes it.
- A non-volatile read, which the dead-code passes delete outright.

## Related

[Y known zero after a DEY loop](dec_branch_indirect_y_fold.md) drops
the `LDY #$00` in front of the `CMP` when a `DEY` loop just left Y at
zero. [Dead A arithmetic](dead_a_arith.md) handles non-volatile loads
into a dead A.
