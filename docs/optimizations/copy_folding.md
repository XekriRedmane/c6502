# Copy folding

`FoldCopiesInFixedpoint` and `FoldCopiesPostDestruction` in
[passes/optimization/copy_folding.py](../../passes/optimization/copy_folding.py)
fuse an instruction that writes a temporary with the `Copy` that
moves the temporary somewhere else. The first runs in the TAC
fixed-point loop, right after dead-store elimination. The second
runs once after SSA destruction. Both share one implementation, and
neither runs in the unoptimized pipeline.

`c99_to_tac` writes every result into a fresh temporary, so `x += 1`
on a static `x` becomes `Binary(+, x, 1, %t); Copy(%t, x)`. Copy
propagation only forwards copies into SSA-renamed names, and a
static is never renamed, so the temporary survives. In the asm it
costs a load and a store per byte, plus the zero-page bytes to hold
it. The pass rewrites the producer to write `x` directly and drops
the `Copy`. The result is an in-place add, which the
[multi-byte INC](inc_peephole.md) peephole later turns into `INC`
instructions. For the example below, the function body shrinks from
25 bytes to 8.

After SSA destruction the same shape comes back in another form.
Each Phi becomes a `Copy` at the end of each predecessor block, so a
loop counter's `i++` reads `Binary(+, i, 1, %t); Copy(%t, i)` at the
bottom of the loop. The post-destruction run folds those, and
nothing later in the TAC pipeline produces new ones.

## Example

```c
static unsigned int count;
void tick(void) { count += 1; }
```

### Before

```
Binary(+, count, 1u, %1.1)
Copy(%1.1, count)
Ret(None)
```

### After

```
Binary(+, count, 1u, count)
Ret(None)
```

The final assembly is `INC count; BNE .inc_done@0; INC count+1`.
With both copy-folding passes switched off, the add goes through two
`__local_tick__*` bytes and is copied back with four more loads and
stores.

For `for (uint8_t i = 0; i < 16; i++) buf[i] = 0;`, the
post-destruction run turns `Binary(+, @0.i.2, 1uc, %7.1);
Copy(%7.1, @0.i.2)` into `Binary(+, @0.i.2, 1uc, @0.i.2)`, which
ends up as an `INX`.

## When it applies

The pass looks at each pair of adjacent instructions. It folds the
pair when all of these are true.

- The second instruction is `Copy(%t, X)` with a variable as its
  source.
- The first instruction writes `%t` as its single destination. The
  instructions that qualify are the extends and `Truncate`, the six
  floating-point conversions, `Unary`, `Binary`, `Copy`,
  `GetAddress`, `Load`, `IndexedLoad`, and a `FunctionCall` or
  `IndirectCall` that has a destination.
- `%t` is read exactly once in the whole function, by this `Copy`.
- `%t` isn't a static and its address is never taken in the
  function.

`X` can be anything. If it's an SSA-renamed name it still has one
definition after the fold, and if it's a static it keeps the same
number of definitions it had.

Adjacency is what makes the fold safe without any aliasing analysis.
Nothing runs between the producer and the `Copy`, so nothing can
read `X` or `%t` in between, or write `X` and have that write
overwritten by the `Copy`.

## What it leaves alone

- A producer and a `Copy` with anything between them. In
  `unsigned t = v + 1; a = 3; h = t;` the store to `a` sits between
  the add and the copy into the global `h`, so the add still writes
  a temporary.
- A temporary with a second reader, because the second reader still
  reads the value under its old name.
- A static or address-taken temporary. Another function can read a
  static, and a `Load` through a pointer can read an address-taken
  local, and neither read shows up in the use count. For
  `g = 7; h = g;` with a global `g`, the fold would store 7 into `h`
  and never write `g`.
- A `Phi`. Its destination is always an SSA-renamed name, and copy
  propagation and dead-store elimination already clean up a `Phi`
  followed by a `Copy`.
- `IndexedConstLoad` and `IndirectIndexedLoad` producers, which
  aren't in the list of redirectable instructions.

The copies that the asm-level SSA round trip creates for its own
Phis are a separate matter, handled by the asm passes such as
[move coalescing](coalescing.md).

## Related

[Copy propagation](copy_propagation.md) handles the SSA-renamed
case inside the fixed point, and
[dead-store elimination](dead_store_elimination.md) removes the
definitions a fold leaves without a reader.
