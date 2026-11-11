# Count-down counter in X or Y

`apply_counter_to_index_reg` in
[passes/counter_to_index_reg.py](../../passes/counter_to_index_reg.py)
moves a loop counter that is read only by its own `DEC` from memory
into X, or into Y when the loop uses X. It runs once under
`--optimize`, after the first peephole fixed point and the
[X-pivot promotion](loop_counter_to_x.md), and before the second
peephole fixed point, which folds the load it leaves. Within a
function it repeats until no more counters move. The unoptimized
pipeline doesn't run it.

`do { ... } while (--count != 0);` with `count` used for nothing else
lowers to an `STA M` before the loop and a `DEC M; BNE` back edge.
The pass turns the store into a transfer to the register and the
`DEC M` into `DEX` or `DEY`. `DEX` takes 1 byte and 2 cycles where
`DEC zp` takes 2 bytes and 5 cycles, so each iteration saves 3
cycles. It sets N and Z from the new value the same way, so the
branch after it is unchanged. The peepholes then fold `LDA n; TAX`
into `LDX n`, which saves another byte and 2 cycles before the loop.

## Example

```c
extern volatile uint8_t speaker;
extern uint8_t buf[256];

void fill(uint8_t n, uint8_t v)
{
    uint8_t i = 0;
    uint8_t c = n;
    do {
        buf[i] = v;
        i++;
        (void)speaker;
    } while (--c != 0);
}
```

### Before

```
fill:
   SUBROUTINE

   LDX   #$00
   LDA   __zpabi_fill__n
   STA   __local_fill__c
.loop@0_start:
   LDA   __zpabi_fill__v
   STA   buf,X
   INX
   LDA   speaker
   DEC   __local_fill__c
   BNE   .loop@0_start
   RTS
```

### After

```
fill:
   SUBROUTINE

   LDX   #$00
   LDA   __zpabi_fill__n
   TAY
.loop@0_start:
   LDA   __zpabi_fill__v
   STA   buf,X
   INX
   LDA   speaker
   DEY
   BNE   .loop@0_start
   RTS
```

The index `i` already lives in X, so the counter goes in Y. The final
assembly starts the loop with `LDY __zpabi_fill__n`. Without the
`buf[i]` store, as in a loop of bare volatile reads of `speaker`, the
counter goes in X and the loop is `LDA speaker; DEX; BNE`.

## When it applies

- A `Data` or `ZP` byte `M` is named by exactly two instructions in
  the function. The first is a non-volatile store into `M` from an
  immediate, a register, a `Data` or `ZP` byte, or an `abs,X` /
  `abs,Y` byte. The second is `DEC M`.
- The `DEC` is directly followed by a `BNE` or `BEQ` whose target
  label lies after the store and at or before the `DEC`. That branch
  is the back edge.
- No jump or branch from outside the range between the store and the
  back edge targets a label inside it, so every path to the `DEC`
  passes through the store.
- Between the store and the back edge, no instruction from which the
  `DEC` can still be reached reads or writes the register. That
  includes an index use of it, and for Y the `LDY` that a soft-stack
  or `(zp),Y` operand implies. A call counts as a use unless the
  callee is declared `preserves_x` or `preserves_y` and doesn't take
  the register as an argument. An exit path that leaves the loop for
  good may use the register, because the counter is dead there.
- The register is dead at the store, so overwriting it there can't
  change a later read.
- The store's source can be loaded into the register. The source
  can't be the register itself, and an `abs,X` source can't be moved
  into X (or `abs,Y` into Y).

X is tried first, then Y.

## What it leaves alone

- A loop with a call in it. With `do { g(); } while (--c != 0);` the
  callee may change both registers, and the counter stays in memory.
- A counter that is read anywhere else, such as one also used as an
  index. [Loop counter in X](loop_counter_to_x.md) handles those.
- A loop that uses both X and Y on the path back to the `DEC`.
- A counter whose back edge is `BPL` or `BMI`, as in a loop that runs
  down to zero inclusive. Only `BNE` and `BEQ` are matched.

## Related

[Loop counter in X](loop_counter_to_x.md) and
[loop-carried value in A](promote_loop_carried_to_a.md) are the other
register promotions that run at this point.
