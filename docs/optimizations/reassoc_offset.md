# Moving a constant out of an offset

`ReassocOffsetConstant` in
[passes/optimization/reassoc_offset.py](../../passes/optimization/reassoc_offset.py)
rewrites the address `p + (x + k)` as `(p + x) + k`, where `p` is a
pointer variable and `k` a constant. It runs in the TAC fixed-point
loop, right after [constant combining](reassoc_const.md). It doesn't
run in the unoptimized pipeline.

The rewrite keeps the same two adds, so on its own it saves nothing.
What it changes is which add is shared. For `d[n + 1]` and
`d[n + 2]`, the original order computes `n + 1` and `n + 2` and then
adds `d` to each, which is two different 16-bit sums. After the
rewrite both subscripts start with `d + n`, which
[CSE](cse.md) computes once, and each one ends with a small constant
added to it. The [indirect-indexed](recognize_indirect_indexed.md)
recognizer turns `Load(ptr + k)` with `k` in 0 to 255 into a
`(zp),Y` access with `LDY #k`, so the constant adds disappear too.

## Example

```c
#include <stdint.h>
uint8_t sum2(const uint8_t *d, uint16_t n) {
    return d[n + 1] + d[n + 2];
}
```

### Before

```
Binary(+, @1.n, 1u, %2.1)
Binary(+, @0.d, %2.1, %3.1)
Load(%3.1, %4.1)
ZeroExtend(%4.1, %5.1)
Binary(+, @1.n, 2u, %8.1)
Binary(+, @0.d, %8.1, %9.1)
Load(%9.1, %10.1)
...
```

### After

```
Binary(+, @0.d, @1.n, %3.1.ra)
Binary(+, %3.1.ra, 1u, %3.1)
Load(%3.1, %4.1)
ZeroExtend(%4.1, %5.1)
Binary(+, @0.d, @1.n, %9.1.ra)
Binary(+, %9.1.ra, 2u, %9.1)
Load(%9.1, %10.1)
...
```

The final assembly has one 16-bit add of `d` and `n` into a pair of
zero-page bytes, followed by `LDY #$01; LDA (__local_sum2__2),Y` and
`INY; LDA (__local_sum2__2),Y` for the two loads.

## When it applies

- The outer instruction is an `Add` whose destination is an
  SSA-renamed name. One operand `p` is a variable, and the other is
  an SSA-renamed `%off` that nothing else reads.
- `%off` is defined by an `Add` of a constant `k` and a variable `x`,
  in either order.
- `p`, `x`, `%off`, and the outer destination all have the same byte
  width. A pointer counts as 2 bytes. Addition at one fixed width
  wraps, so `p + (x + k)` and `(p + x) + k` give the same bits.

The two new adds take the outer add's place in the listing, and the
inner add is dropped. The intermediate sum is a new temporary named
after the outer destination with `.ra` appended, with the outer
destination's type.

## What it leaves alone

- A constant base. For `((const uint8_t *)0x2000)[n + 1]` the pass
  makes no change, and constant combining folds the two constants
  into `Binary(+, 8193u, @0.n, %4.1)` instead.
- An offset that has another reader. In
  `uint16_t m = n + 1; g = m; return d[m];` the sum `n + 1` is also
  stored to `g`, so it stays.
- Operands of different widths, and any offset whose variable part
  isn't a plain variable, such as a constant.
- A function outside SSA form, where the pass does nothing.

## Related

[Combining added constants](reassoc_const.md) handles a constant
base, and [indirect-indexed access](recognize_indirect_indexed.md)
turns the remaining `+ k` into a Y index.
