# Transfer plus or minus one

`apply_transfer_pm1_store` in
[passes/transfer_pm1_store.py](../../passes/transfer_pm1_store.py)
turns `TXA; SEC; SBC #1; STA M` into `DEX; STX M`, and `TXA; CLC;
ADC #1; STA M` into `INX; STX M`, with the same rewrites for Y. It
runs last in the peephole fixed point, which both pipelines run, but
no program in the test corpus makes it fire in the unoptimized
pipeline.

Storing `x - 1` or `x + 1` when the value is already in X routes it
through A, because A is the 6502's only register with an adder. `DEX`
or `INX` computes the same byte in X, and `STX` stores it. The four
instructions take 7 bytes and 10 cycles for an absolute `M`, and the
two that replace them take 4 bytes and 6. The rewrite also leaves A
untouched, so when register allocation had saved A around the
arithmetic, that save and its reload become dead, and later sweeps
remove them.

The pass runs after every other pass in the sweep, for a reason
given in a comment in `compile._peephole_fixedpoint`. Register
allocation can route the result through a temporary, as in `TXA;
SEC; SBC #1; STA __local; LDA __local; STA M`. By the end of the
sweep, `round_trip_load_drop` and `asm_dead_store` have collapsed
that to the plain `STA M` form, so the store this pass rewrites is
the one to `M`. If this pass ran first, it would produce `DEX; STX
__local; LDA __local; STA M`. The comment states that no later pass
collapses that shape, because the `LDA` reads a slot that was
written from X.

## Example

```c
unsigned char table[8];
unsigned char idx, value, prev;
void fetch(void) {
    value = table[idx];
    prev = idx - 1;
}
```

### Before

```
fetch:
   LDX   idx
   LDA   table,X
   STA   value
   TXA
   SEC
   SBC   #$01
   STA   prev
   RTS
```

### After

```
fetch:
   LDX   idx
   LDA   table,X
   STA   value
   DEX
   STX   prev
   RTS
```

With `next = idx + 1;` instead, the pass turns `TXA; CLC; ADC #$01;
STA next` into `INX; STX next`.

## When it applies

- The four instructions are adjacent. The first is a non-volatile
  `TXA` or `TYA`.
- The second and third are `SEC` and `SBC #1`, or `CLC` and `ADC #1`,
  with A as the destination.
- The fourth is a non-volatile `STA` to a `Data` or `ZP` operand.
  `STX` and `STY` have no `(zp),Y` mode, and the pass doesn't use
  their indexed modes.
- After the store, A is dead, the register that was transferred is
  dead (it now holds the new value), and C and V are dead (`c_dead_at`,
  `v_dead_at`). `DEX` and `INX` leave C and V as they were, where the
  `SBC` or `ADC` set them, but set N and Z from the new value just as
  the `SBC` or `ADC` did, so a branch on N or Z may follow. That was
  `get_next_code_byte2`'s `if (++Z_PC2_L == 0)` in
  examples/zork/interpreter/pages.c, which became `INY; STY
  Z_PC2_L; BNE`, so the byte it read stayed in A. (Z_PC2 is now a
  24-bit struct, whose increment is `INC`; see [members computed in
  place](in_place_member_update.md).)

### An index register that already holds the byte

The converse shape steps a byte into the register that already holds
it. After `LDY token_idx; STA (parse),Y`, Zork's
`parse[(uint8_t)(token_idx + 1)]` computed `LDA token_idx; CLC; ADC
#$01; TAY`, which is `INY`. The rule takes `ADC #1` and `ADC #2`
(one or two `INY`), `SBC` for `DEY`, and the X forms, when the last
write to the register in the block is a load of the byte and nothing
since writes the byte or might. A store through a pointer might,
unless the byte is one no pointer can point to, a `static` or one of
the function's own zero-page slots whose address the unit never takes
(`asm_aliasing.unreachable_by_pointer`). A, C, and V must be dead after. Any
write to the register stops the walk, `INX` included, which
`kills_x` doesn't count since it reads X too. Missing that turned
`LDA m; CLC; ADC #2; TAX` into `INX; INX` after an `INX` in
`tests/test_beam_target_tick_diff.py`'s program.

### A step on either side of a transfer

When the register doesn't hold the byte, the step can still move
from A to the register on the other side of the transfer:

```
   CLC                              TAY
   ADC   #$01             ->        INY
   TAY

   TYA                              INY
   CLC                    ->        TYA
   ADC   #$01
```

The first requires A, C, and V to be dead after it, since A keeps its
old value. The second requires Y, C, and V to be dead after it, since
Y ends up with the sum too. A `TAY` back after the second makes all four
instructions one `INY`, when A, C, and V are dead after it. N and Z
come out the same in each, from the sum. Each saves 2 bytes and 2
cycles for a step of 1, and 1 byte for a step of 2. Zork's
`match_dictionary_word` (examples/zork/interpreter/input.c) steps its
index through the dictionary's header with `y++`, which gave both
shapes:

```
   LDA   (entry),Y                  LDA   (entry),Y
   CLC                              TAY
   ADC   #$01             ->        INY
   TAY                              ...
   ...                              INY
   TYA                              TYA
   CLC                              CLC
   ADC   #$01                       ADC   entry
   CLC
   ADC   entry
```

### A step stored and loaded into an index register

When the stepped byte is stored back and then loaded into an index
register, the step can be made in the register:

```
   LDA   __local_return_value__x          LDX   __local_return_value__x
   SEC                                    DEX
   SBC   #$02                   ->        DEX
   STA   __local_return_value__x          STX   __local_return_value__x
   LDX   __local_return_value__x
```

for a step of 1 or 2, `CLC; ADC #k` with `INX`, and the Y forms, when
A, C, and V are dead after (`_rewrite_step_then_reload`). X and the
byte end up with the same value as before, and N and Z come out the
same, from the last step. That's `x -= 2` in Zork's `return_value`
(examples/zork/interpreter/decode.c), and it saves 3 bytes there, 4
for a step of 1.

## What it leaves alone

- A read of the original register afterward. In

  ```c
  value = table[idx];
  prev = idx - 1;
  other = table[idx];
  ```

  the third statement indexes with X again, so `TXA; SEC; SBC #$01;
  STA prev` stays.
- A carry that a later instruction reads, as when the add is the low
  byte of a wider sum.
- A store to the soft stack, through a pointer, or to an indexed
  address.
- An add or subtract of any constant other than 1.

## Related

[Store X or Y directly](via_a_store_fold.md) handles a transfer with
no arithmetic before the store, and [DEC on a register](dec_register_peephole.md)
folds a register decrement that feeds a branch.
