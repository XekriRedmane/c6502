# Copy propagation

`copy_propagate` in
[passes/optimization/copy_propagation.py](../../passes/optimization/copy_propagation.py)
replaces each use of a `Copy`'s destination with the `Copy`'s
source. It runs on SSA-form TAC in the optimizer's fixed point, after
loop-invariant hoisting and just before dead-store elimination, and
only under `--optimize`.

In SSA form each renamed variable is written exactly once, so after
`Copy(y, x)` the name `x` holds the value of `y` at every place that
reads it. The pass collects every such `Copy` in the function,
follows chains of them (`x` from `y`, `y` from `z`) to the first
value that isn't a copy, and rewrites every read to that value,
including reads in `Phi` arguments, call arguments, and conditional
jumps. The `Copy` instructions are left in place with no readers,
and [dead-store elimination](dead_store_elimination.md) removes them
next. On the 6502 each copy that reaches code generation costs a
load and a store per byte, plus the zero-page byte that holds it. A
propagated constant also gives [constant folding](constant_folding.md)
an operand to fold on the next round.

## Example

```c
#include <stdint.h>
uint8_t scale(uint8_t x) {
    uint8_t y = x;
    uint8_t z = y;
    return z + y;
}
```

### Before

```
Copy(@0.x, @1.y.1)
Copy(@1.y.1, @2.z.1)
ZeroExtend(@2.z.1, %0.1)
ZeroExtend(@1.y.1, %1.1)
Binary(+, %0.1, %1.1, %2.1)
Truncate(%2.1, %3.1)
Ret(%3.1)
```

### After

```
Copy(@0.x, @1.y.1)
Copy(@0.x, @2.z.1)
ZeroExtend(@0.x, %0.1)
ZeroExtend(@0.x, %1.1)
Binary(+, %0.1, %1.1, %2.1)
Truncate(%2.1, %3.1)
Ret(%3.1)
```

The chain from `z` through `y` resolves to the parameter `x`. Once
dead-store elimination drops the two copies, the function compiles to
`LDA __zpabi_scale__x; CLC; ADC __zpabi_scale__x; RTS`.

## When it applies

A `Copy(src, dst)` is propagated when all of these are true.

- `dst` is a name that SSA construction renamed, so it has one
  definition. A promotable parameter keeps its source spelling, such
  as `@0.x`, and counts as renamed.
- `src` is a constant, or a renamed name with the same signedness as
  `dst`.

A constant whose signedness differs from `dst`'s type is propagated
after it's re-expressed in `dst`'s constant variant, with the same
bit pattern. The pass makes no use of the control-flow graph. Since
every renamed name has one definition that dominates its uses, one
map from destination to source covers the whole function.

## What it leaves alone

- A `Copy` from a static, from an address-taken local, or from any
  other name SSA didn't rename. That `Copy` reads the variable at one
  point in the program, and a store, a call, or a write through a
  pointer can change it before a later read. In `uint8_t a = g; g =
  0; return a;` the return keeps reading `a`.
- A `Copy` into such a name, such as a store to a static, because
  other code can observe the write and later writes can replace it.
- A `Copy` between a signed and an unsigned variable of the same
  width, which is how `c99_to_tac` lowers a cast like
  `(unsigned int)s`. Division, remainder, right shift, and the
  ordering compares take their signedness from the operand's type, so
  replacing the unsigned name with `s` would turn `u >> 1` into an
  arithmetic shift.
- A `Phi`, which merges values from several predecessors and so isn't
  a copy of any one of them.
- The operand of `GetAddress`. It names a storage location, and a
  constant or another name can't take the place of that location.
- The operands of `IndexedConstLoad`, `IndirectIndexedLoad`, and
  `IndirectIndexedStore`, which the rewriter has no case for. Their
  copies stay live, so this costs only the missed propagation.

## Related

[Asm copy propagation](asm_copy_propagation.md) does the same job on
the asm-level SSA form after `tac_to_asm`. [Copy
folding](copy_folding.md) removes the copies this pass can't, where
the destination is a static or an address-taken local.
