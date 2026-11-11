# Address of a zero-page local

`lower_data_load_address` in
[passes/lower_data_load_address.py](../../passes/lower_data_load_address.py)
splits a `LoadAddress` whose source is a fixed address into two
single-byte stores of immediates. It runs once in the `--optimize`
pipeline, after `replace_pseudoregisters_bare_exit` and before
prologue synthesis and the peephole fixed point. The unoptimized
pipeline doesn't run it.

The pass saves nothing by itself, because `asm_emit` prints a
`LoadAddress` from a `Data` source as the same `LDA #<x; STA lo; LDA
#>x; STA hi`. It exposes those two stores to the peepholes.
`memory_value_propagation` clears every fact it was tracking at a
`LoadAddress`, so a later read of the
pointer's bytes stays a read. Once the stores are separate `Mov`s,
the pass records that the two cells hold `#<x` and `#>x`, and each
later `LDA` of them becomes an `LDA #`. The stores then have no
readers, and `apply_remat` drops them.

The pass also fixes the name of the high byte. When the destination
is a 2-byte value in the function's private pool, each byte has its
own slot symbol, and reads of the second byte use `__local_f__1`.
The emitted form of a `LoadAddress` writes the high byte as
`__local_f__0+1`, which is the same address under a different name.
The name-keyed dead-store passes would find no read of the written
name and drop the write, so the pass writes the high byte under the
name its reads use.

## Example

```c
#include <stdint.h>
void put(uint8_t *p) { *p = 5; }
uint8_t caller(void) { uint8_t x; put(&x); return x; }
```

`x` has its address taken, and the address-taken pass puts it at a
fixed byte of `caller`'s pool, `__local_caller__x`. Its address is
then a link-time constant, and the `LoadAddress` that computes `&x`
has a `Data` source. These listings are in IR notation (`--raw`),
because the text form of the `LoadAddress` is the same four
instructions the pass produces.

### Before

```
Label(.caller@asm_ssa_block@0)
LoadAddress(Data(__local_caller__x, 0), Data(__local_caller__0, 0))
Mov(Data(__local_caller__0, 0), Data(__zpabi_put__p_0, 0))
Mov(Data(__local_caller__1, 0), Data(__zpabi_put__p_1, 0))
Call(put, [])
Mov(Data(__local_caller__x, 0), A)
Return(True)
```

### After

```
Label(.caller@asm_ssa_block@0)
Mov(ImmLabelLow(__local_caller__x, 0), Data(__local_caller__0, 0))
Mov(ImmLabelHigh(__local_caller__x, 0), Data(__local_caller__1, 0))
Mov(Data(__local_caller__0, 0), Data(__zpabi_put__p_0, 0))
Mov(Data(__local_caller__1, 0), Data(__zpabi_put__p_1, 0))
Call(put, [])
Mov(Data(__local_caller__x, 0), A)
Return(True)
```

In the peephole fixed point, `memory_value_propagation` rewrites the
two reads of `__local_caller__0` and `__local_caller__1` to `LDA
#<__local_caller__x` and `LDA #>__local_caller__x`. `apply_remat`
then drops the two stores to the temporary, which nothing reads any
more, and `dead_a_arith` drops the two loads that fed them. The final
code for `caller` is the following.

```
   LDA   #<__local_caller__x
   STA   __zpabi_put__p_0
   LDA   #>__local_caller__x
   STA   __zpabi_put__p_1
   JSR   put
   LDA   __local_caller__x
   RTS
```

With the pass disabled, the final code still has all eight instructions
of the staging round trip in front of the `JSR`, which is 16 bytes
and 22 cycles where the code above takes 8 bytes and 10 cycles.

## When it applies

The pass rewrites every `LoadAddress` whose source is a `Data`
operand and whose destination is a memory operand (`Data`, `ZP`,
`Frame`, or `Stack`). The low byte goes to the destination as
written. For the high byte, when the destination is a `Data` slot
symbol and the byte after it is a pool byte with a slot symbol of its
own, the store uses that symbol. Otherwise it uses the destination
with its offset raised by 1.

A `Data` source arises only for an automatic variable that the
address-taken pass placed in the function's private pool (see
[address-taken locals in zero page](address_taken_zp.md)). The
address of a static object never reaches this pass as a
`LoadAddress`, because `tac_to_asm` already lowers it to the two
`ImmLabelLow` / `ImmLabelHigh` stores.

## What it leaves alone

- A `LoadAddress` with a `Frame` source, which is the address of a
  local on the soft stack. The address is `FP` plus an offset, which
  takes the `CLC` / `ADC` chain at run time. This covers every
  address-taken local in a function without a private pool, such as
  a recursive one, and any address-taken local that found no free run
  of bytes in its pool.
- The high byte of a destination inside a wider object, such as the
  second byte of an address-taken struct, which has no slot symbol of
  its own. It stays an offset from the destination's name.
- Every function in the unoptimized pipeline, where no local is in
  zero page and so every `LoadAddress` has a `Frame` source.

## Related

[Memory value propagation](memory_value_propagation.md) does the
folding the split enables, and [rematerialization](asm_remat.md) and
[dead A arithmetic](dead_a_arith.md) clean up after it. [Address-taken
locals in zero page](address_taken_zp.md) produces the `Data`
sources.
