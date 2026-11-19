# One add with the carry set for `p + b + 1`

`apply_carry_in_add` in
[passes/carry_in_add.py](../../passes/carry_in_add.py) folds the two
16-bit adds of `p + b + 1`, where the second operand's high byte is 0
(a byte or a small constant), into one add that starts with `SEC`. It
runs in the peephole fixed point, which both pipelines run, before
[carry INC](carry_inc.md) turns an `ADC #$00` into `BCC; INC`.

The low byte of `L + m + 1` is at most 511, so one carry into the high
byte covers both adds, and the high byte and A come out the same. The
two adds' final C and V can differ from the one add's, so both must
be dead after the last store. The temporaries the first add leaves
must be dead after their loads, and the high byte read after the
first store must be a constant or one of the function's own slots,
which the store can't reach.

## Example

From `next_string` in `examples/ali_baba/text.c`, which returns the
string after the length-prefixed string `s`:

```c
return s + s[0] + 1;
```

### Before

```
   STA   __local_next_string__s
   STX   __local_next_string__1
   LDY   #$00
   CLC
   ADC   (__local_next_string__s),Y
   BCC   .carry_inc@0
   INC   __local_next_string__1
   CLC
.carry_inc@0:
   ADC   #$01
   STA   next_string@ret
   LDA   __local_next_string__1
   ADC   #$00
   STA   next_string@ret+1
   RTS
```

### After

```
   STA   __local_next_string__s
   STX   __local_next_string__1
   LDY   #$00
   SEC
   ADC   (__local_next_string__s),Y
   STA   next_string@ret
   TXA
   ADC   #$00
   STA   next_string@ret+1
   RTS
```

That's the original's `SEC; ADC` in DISPLAY_MESSAGE. The pass saves 8
bytes here and 8 in Ali Baba's `map_value_to_string`
(`examples/ali_baba/combat.c`).
