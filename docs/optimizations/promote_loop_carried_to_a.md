# Loop-carried value in A

`apply_promote_loop_carried_to_a` in
[passes/promote_loop_carried_to_a.py](../../passes/promote_loop_carried_to_a.py)
moves a `register` parameter that a loop reads and increments out of
its zero-page save byte and into A. It runs once under `--optimize`,
after the first peephole fixed point and just before the
[X-pivot promotion](loop_counter_to_x.md), and the second peephole
fixed point cleans up after it. The unoptimized pipeline doesn't run
it.

A parameter that arrives in A has to leave A when the code before the
loop uses A for something else, such as staging a pointer in `DPTR`.
The register allocator then saves it to a body-local byte `M` at
entry, and the loop reads it with `LDY M` and steps it with `INC M`.
The pass parks the value in Y during the code before the loop, which
doesn't use Y, and moves it back to A with `TYA` just before the loop
label. Inside the loop `LDY M` becomes `TAY` and `INC M` becomes
`CLC; ADC #$01`, so A carries the next value around the back edge.
The `LDY zp; INC zp` pair takes 4 bytes and 8 cycles, and `TAY; CLC;
ADC #$01` takes 4 bytes and 6 cycles, so each iteration saves 2
cycles. Before the loop, `STA zp` (2 bytes, 3 cycles) becomes `TAY`
and `TYA` (2 bytes, 4 cycles). Nothing references `M` afterwards, so
its `EQU` line is dropped and the zero-page byte is free.

## Example

This is `examples/snd_delay_up.c` without its comments.

```c
extern const volatile uint8_t *sfx_click_ptr;

void snd_delay_up(register uint8_t pitch, register uint8_t clicks)
{
    do {
        register volatile uint8_t y = pitch;
        pitch = (uint8_t)(pitch + 1);
        while (--y != 0) { }
        (void)*sfx_click_ptr;
    } while (--clicks != 0);
}
```

### Before

```
snd_delay_up:
   SUBROUTINE

   STA   __zpabi_snd_delay_up__pitch
   STX   __zpabi_snd_delay_up__clicks
   STA   __local_snd_delay_up__pitch
   LDA   sfx_click_ptr
   STA   DPTR
   LDA   sfx_click_ptr+1
   STA   DPTR+1
.loop@0_start:
   LDY   __local_snd_delay_up__pitch
   INC   __local_snd_delay_up__pitch
.loop@1_continue:
   DEY
   BNE   .loop@1_continue
   CMP   (DPTR),Y
   DEX
   BNE   .loop@0_start
   RTS
```

### After

```
snd_delay_up:
   SUBROUTINE

   STA   __zpabi_snd_delay_up__pitch
   STX   __zpabi_snd_delay_up__clicks
   TAY
   LDA   sfx_click_ptr
   STA   DPTR
   LDA   sfx_click_ptr+1
   STA   DPTR+1
   TYA
.loop@0_start:
   TAY
   CLC
   ADC   #$01
.loop@1_continue:
   DEY
   BNE   .loop@1_continue
   CMP   (DPTR),Y
   DEX
   BNE   .loop@0_start
   RTS
```

The `CMP (DPTR),Y` is the volatile read `(void)*sfx_click_ptr`.
After the rewrite it compares against the pitch in A instead of the
pointer's high byte, which changes nothing because `DEX` overwrites N
and Z before the `BNE` reads them. The two entry stores to
`__zpabi_*` slots are removed later by the
[entry stub drop](dead_reg_entry_stub.md).

## When it applies

- The function's first instruction, after any labels and the
  `STA` / `STX` / `STY` entry stubs into its own `__zpabi_<fn>__*`
  slots, is a non-volatile `STA __local_<fn>__*`. That byte is `M`.
- Later in the function, `LDY M` is directly followed by `INC M`, and
  the `LDY` directly follows a label or a run of labels. The first
  label of that run is taken as the loop start.
- `M` appears in no other instruction in the function.
- Nothing between the save and the loop start label reads or writes
  Y, since Y holds the value there.
- A branch or jump after the `INC` targets the loop start label. The
  first one found is the back edge.
- Between the `INC` and that back edge, nothing writes A, and the
  only instruction that may read A is a `CMP` whose N and Z are
  overwritten before the next branch.

The `LDY M` may be volatile, as it is here because `y` is a volatile
`register` local. The `TAY` that replaces it keeps the volatile mark,
since it writes the same Y.

## What it leaves alone

- A parameter that is also read after the loop. With `last = pitch;`
  added after the loop in the example, the loop keeps the sum in A and
  stores it back to `M` on every iteration, so there's no `LDY M; INC
  M` pair and `M` has other accesses.
- A loop that writes A after the step. Replacing the volatile read
  with `out = 5;` makes the inner delay loop count with `TYA; SEC;
  SBC #$01`, and the pass leaves the function unchanged.
- A function that opens with anything other than the save, or saves
  the parameter from X or Y. Only a value that arrives in A has an A
  to return to.
- A function that uses Y before the loop, because the parked value
  would be overwritten.

## Related

[Loop counter in X](loop_counter_to_x.md) does the same kind of
promotion for an index counter, and
[count-down counter in X or Y](counter_to_index_reg.md) for a counter
used only by its `DEC`.
