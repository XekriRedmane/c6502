# Loop counter in X

`apply_loop_counter_to_x` in
[passes/loop_counter_to_x.py](../../passes/loop_counter_to_x.py)
moves a one-byte loop counter that the body uses as an `abs,X` index
out of its zero-page byte and into X for the whole loop. It runs once
under `--optimize`, after the first peephole fixed point, between the
[loop-carried A promotion](promote_loop_carried_to_a.md) and the
[count-down counter promotion](counter_to_index_reg.md). The second
peephole fixed point cleans up the loads and stores it leaves. The
unoptimized pipeline doesn't run it.

A counter that lives in memory `M` gets reloaded with `LDX M` at the
top of every iteration, and again after anything else in the body
loads X. The pass makes X the counter's home and does three things to
keep it there.

1. It turns each other `LDX v` into `LDY v`, and each `abs,X` access
   that reads that value into `abs,Y`. This is the "Y pivot". It
   applies from the `LDX` to the next label, branch, jump, call,
   return, X write, or use of `M`. An `LDX` that comes where the
   counter is dead, such as the `LDX` of a 2-byte return value after
   the loop, stays as it is.
2. It puts `STX M` before each call that may change X and `LDX M`
   after it, when the counter is read again after the call.
3. It drops every `LDX M`, adds `TAX` after the store that
   initializes `M`, and turns the step into `DEX` or `INX`. An init
   `STX M` needs nothing added, and an init `STY M` gets an `LDX M`.

The result matches the hand-written idiom, where the counter sits in
X and a second index uses Y. Each dropped `LDX zp` saves 2 bytes and
3 cycles, and `DEX` in place of `DEC zp` saves 1 byte and 3 cycles.
The `STX` / `LDX` pair around a call costs 4 bytes and 6 cycles, so
the gain per iteration depends on how many reloads the loop had. The
pass was written for the outer loop of
`examples/refresh_hit_entities.c`.

## Example

```c
extern uint8_t ent_y[8];
extern uint8_t ent_row[8];
extern uint8_t spr_w[4];
void draw(uint8_t y, uint8_t w);

void refresh(uint8_t n, uint8_t spr)
{
    uint8_t i = n;
    do {
        uint8_t y = ent_y[i];
        uint8_t w = spr_w[spr];
        draw((uint8_t)(y + ent_row[i]), w);
        i--;
    } while ((i & 0x80) == 0);
}
```

### Before

```
refresh:
   SUBROUTINE

   LDA   __zpabi_refresh__n
   STA   __local_refresh__i
.loop@0_start:
   LDX   __local_refresh__i
   LDA   ent_y,X
   STA   __local_refresh__2
   LDX   __zpabi_refresh__spr
   LDA   spr_w,X
   STA   __local_refresh__1
   LDX   __local_refresh__i
   LDA   ent_row,X
   STA   __local_refresh__0
   LDA   __local_refresh__2
   CLC
   ADC   __local_refresh__0
   STA   __zpabi_draw__y
   LDA   __local_refresh__1
   STA   __zpabi_draw__w
   JSR   draw
   DEC   __local_refresh__i
   BPL   .loop@0_start
   RTS
```

### After

```
refresh:
   SUBROUTINE

   LDA   __zpabi_refresh__n
   STA   __local_refresh__i
   TAX
.loop@0_start:
   LDA   ent_y,X
   STA   __local_refresh__2
   LDY   __zpabi_refresh__spr
   LDA   spr_w,Y
   STA   __local_refresh__1
   LDA   ent_row,X
   STA   __local_refresh__0
   LDA   __local_refresh__2
   CLC
   ADC   __local_refresh__0
   STA   __zpabi_draw__y
   LDA   __local_refresh__1
   STA   __zpabi_draw__w
   STX   __local_refresh__i
   JSR   draw
   LDX   __local_refresh__i
   DEX
   BPL   .loop@0_start
   RTS
```

The later peepholes fold `LDA n; STA i; TAX` into `LDX
__zpabi_refresh__n`, since the `STX` before the call writes `i`
before anything reads it.

A loop that counts up and tests the counter at the top is promoted
the same way, with `INC M` becoming `INX`. In
`examples/zork_dump_buffer.c` the test `LDA M; CMP BUFF_END` becomes
`CPX BUFF_END`, which is done whenever the `CMP` operand is an
immediate or a direct memory operand and A is dead after it. Any
other `LDA M` becomes `TXA`. The call in that loop is to `COUT1`,
which `<apple2/monitor_io.h>` declares `preserves_x`, so it gets no
`STX` / `LDX` wrap.

## When it applies

The pass looks for one memory byte `M` in the function with this use
pattern, and promotes only the first one it finds.

- Exactly one instruction stores to `M`. That's the initialization.
- One or more `LDX M` instructions read it.
- Exactly one `DEC M` or exactly one `INC M` steps it, but not both.
- `LDA M` may read it. No other instruction may name it, so an `ADC
  M`, a `CMP M`, a shift or rotate of `M`, a `BIT M`, or a push
  disqualifies it.
- For a `DEC` step, the next instruction after any labels is a `BEQ`,
  `BNE`, `BMI`, or `BPL` whose target, after following jumps, is one
  of the `LDX M` reloads. That reload is the loop top. None of the
  labels between the `DEC` and the branch may be a jump or branch
  target, since a path through one would skip the `DEC`.

Every other instruction in the function that writes X must be one of
these.

- A call. A call to a routine declared `preserves_x` doesn't count as
  writing X. A call that returns a value in X, which is read after
  the call before X is written again, disqualifies the whole
  function, because the `LDX M` after it would overwrite that value.
- An `LDX` at a point where the counter is dead, which means no
  path from it reads `M` before the init stores to `M` again. It's
  left alone.
- An `LDX` of an immediate or a direct memory operand that starts a
  valid pivot range. No instruction in the range may read or write
  Y. That rules out an `abs,Y` operand, an `(zp),Y` operand, and a
  soft-stack or `Indirect` operand, whose `LDY #off` setup writes Y.
  Every X read in the range has to be an `LDA`, `STA`, `ADC`, `SBC`,
  `AND`, `ORA`, or `CMP` of an `abs,X` operand, since those are the
  ones with an `abs,Y` form. A `TXA`, an `STX`, a `CPX`, or an `INC
  abs,X` disqualifies the range. After the range, neither the value
  the `LDX` loaded nor the value Y held before it may be read again
  before X or Y is written. The pivot leaves the counter in X and the
  loaded value in Y, so such a read would get the wrong one.

`TAX`, `INX`, `DEX`, or a pull into X anywhere else disqualifies the
function.

## What it leaves alone

- A counter with any other use. In a loop whose body is `out1 =
  (uint8_t)(ent_y[i] + i); g();`, the body adds `M` to A with `ADC
  __local_f__i`, and the pass leaves the function unchanged.
- A count-down loop whose `DEC` is followed by a label that something
  else branches to.
- A function that loads X with `TAX`, or steps a second value with
  `INX` / `DEX`, because the pivot only rewrites an `LDX` from
  memory.
- An `LDX` whose range uses Y. An `LDA (zp),Y` between the `LDX` and
  its `abs,X` use would read Y as its index, and moving the index to
  Y would change the address.
- A loop around a call whose X result is read after the call, such as
  the high byte of a 2-byte return value.
- An `LDX` whose value is read past the end of its range, as in `LDX
  k; LDA t,X; .mid: LDA u,X`, where the label ends the range and the
  `LDA u,X` after it still reads `k`.

## Related

[Count-down counter in X or Y](counter_to_index_reg.md) handles a
counter that is never used as an index,
[reads of X's save slot](x_save_slot_load.md) repairs `LDA M` reads
of a slot that X shadows, and
[second index in Y](dual_index_promotion.md) replaces the repeated
`LDX` loads of a second index with one `LDY` at function entry.
