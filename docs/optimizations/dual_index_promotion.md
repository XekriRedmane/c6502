# Second index in Y

`apply_dual_index_promotion` in
[passes/dual_index_promotion.py](../../passes/dual_index_promotion.py)
loads a byte that a function reloads into X several times into Y once
at entry, and switches the `abs,X` accesses that use it to `abs,Y`.
It runs once under `--optimize`, after the second peephole fixed point
and before the third, which cleans up after it. The unoptimized
pipeline doesn't run it.

When a function indexes with two different bytes, both compete for X,
and each switch back to a byte costs another `LDX`. If the function
doesn't use Y, one of the two can live in Y for the whole function.
The pass picks the byte loaded into X most often, inserts `LDY D` at
the top of the function, drops every `LDX D`, and rewrites each
`abs,X` operand that reads X while X holds `D`. An `abs,Y` operand is the same size and speed as
`abs,X`, so the saving is the dropped loads, 2 bytes and 3 cycles
each for a zero-page `D` (3 bytes and 4 cycles for an absolute one),
less one `LDY` at entry. When the loads sit on different branches, as
below, each call runs the same number of loads and the saving is code
size only. When several of them run on one path, it also saves
cycles.

## Example

This is `examples/apply_bobble.c` without its comments.

```c
extern uint8_t entity_floor_pos[20];
extern const uint8_t rescue_bobble[];

__attribute__((zp_abi))
static void apply_bobble(uint8_t slot, uint8_t bobble_idx)
{
    uint8_t bobble    = rescue_bobble[bobble_idx];
    uint8_t magnitude = bobble & 0x7F;
    if (bobble & 0x80) {
        entity_floor_pos[slot] = (uint8_t)(entity_floor_pos[slot] + magnitude);
    } else {
        entity_floor_pos[slot] = (uint8_t)(entity_floor_pos[slot] - magnitude);
    }
}
```

### Before

```
apply_bobble:
   SUBROUTINE

   LDX   __zpabi_apply_bobble__bobble_idx
   LDA   rescue_bobble,X
   BPL   .if_else@1
   AND   #$7F
   LDX   __zpabi_apply_bobble__slot
   CLC
   ADC   entity_floor_pos,X
   STA   entity_floor_pos,X
   JMP   .if_end@0
.if_else@1:
   AND   #$7F
   STA   __local_apply_bobble__1
   LDX   __zpabi_apply_bobble__slot
   LDA   entity_floor_pos,X
   SEC
   SBC   __local_apply_bobble__1
   STA   entity_floor_pos,X
.if_end@0:
   RTS
```

### After

```
apply_bobble:
   SUBROUTINE

   LDY   __zpabi_apply_bobble__slot
   LDX   __zpabi_apply_bobble__bobble_idx
   LDA   rescue_bobble,X
   BPL   .if_else@1
   AND   #$7F
   CLC
   ADC   entity_floor_pos,Y
   STA   entity_floor_pos,Y
   JMP   .if_end@0
.if_else@1:
   AND   #$7F
   STA   __local_apply_bobble__1
   LDA   entity_floor_pos,Y
   SEC
   SBC   __local_apply_bobble__1
   STA   entity_floor_pos,Y
.if_end@0:
   RTS
```

## When it applies

- Some `Data` byte `D` is the source of at least two `LDX D`
  instructions. When more than one byte qualifies, the one with the
  most loads is chosen, with ties broken by name.
- Nothing in the function reads or writes Y, uses an `abs,Y` operand,
  or uses an operand whose emitted form reads or loads Y. Those are
  the soft-stack operands, the `(DPTR),Y` forms, and both `(zp),Y`
  forms, the one that uses Y as it is and the one emitted with an
  `LDY #off` in front. A call that passes an argument in Y counts
  too.
- Some other instruction loads X from a source other than `D`.
  Otherwise `D` could stay in X with no reloads.
- The function makes no call, since a callee may change Y or write
  `D`, and nothing in the function writes `D`. Y is loaded from `D`
  once, so `D` must still hold that value at every dropped `LDX`.
- None of the `abs,X` operands to be rewritten is the target of an
  `INC`, `DEC`, `ASL`, `LSR`, `ROL`, or `ROR`, since the 6502 has no
  `abs,Y` form of those.
- Every read of X that a dropped `LDX D` reaches reads `D` on every
  path to it. The pass follows the control flow across labels,
  branches, and jumps to find out what X holds at each `abs,X`
  operand. An operand where X holds `D` on some paths and another
  byte on others disqualifies the function, and so does a read of X
  as a value, such as a `TXA`, an `STX`, or a 2-byte return's high
  byte, where X may hold `D`. An operand where X holds another byte
  on every path stays `abs,X`.
- Nothing reads the N or Z flag that a dropped `LDX D` or the added
  `LDY D` sets.

## What it leaves alone

- A function with a call in it. Adding a call to `tick()` in the
  `else` branch of the example leaves both `LDX` loads in place.
- A function that uses Y anywhere, including through a soft-stack
  access or a pointer dereference.
- An `abs,X` operand after a join where X holds `D` on one incoming
  path and another byte on the other.
- A byte loaded into X only once, and a function whose only X loads
  are of `D`.
- A function that writes `D`, such as a temporary stored and then
  reloaded into X.
- Only one byte per function is moved, since Y holds one value.

## Related

[Loop counter in X](loop_counter_to_x.md) moves a second index into
Y inside a loop whose counter holds X.
