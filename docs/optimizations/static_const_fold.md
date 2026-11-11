# Static const reads

`FoldStaticConstReads` (`fold_static_const_reads`) in
[passes/optimization/static_const_fold.py](../../passes/optimization/static_const_fold.py)
replaces each read of a `static const` scalar with its initial value.
It runs once per function under `--optimize`, after SSA construction
and before the TAC fixed-point loop.

C's type system rejects every write to a `const` object, so a
`static const` scalar with a numeric initializer holds that value for
the whole run. The pass rewrites each `Var` operand that names such
an object to a `Constant`, which puts the value in front of
[constant folding](constant_folding.md) and the other fixed-point
passes. The largest gain is for a `const` pointer to a fixed address,
such as a screen buffer. Once the pointer is a constant, the address
arithmetic folds and the recognizers can lower `p[i]` to an
absolute,X access instead of copying the pointer into DPTR for a
`(zp),Y` one. When no read is left, the asm-level
[const static fold](const_static_fold.md) drops the object's
storage.

## Example

```c
static uint8_t * const screen = (uint8_t *)0x0400;
static const uint8_t blank = 0xA0;
void clear_cell(uint8_t col) {
    screen[col] = blank;
}
```

### Before

```
Label(.clear_cell@ssa_block@0)
ZeroExtend(@0.col, %0.1)
Binary(+, screen, %0.1, %1.1)
Store(blank, %1.1)
Ret(None)
```

### After

```
Label(.clear_cell@ssa_block@0)
ZeroExtend(@0.col, %0.1)
Binary(+, 1024u, %0.1, %1.1)
Store(160uc, %1.1)
Ret(None)
```

The [indexed store recognizer](recognize_indexed_store.md) then
turns the add and the store into one `IndexedStore`, and the function
compiles to this.

```
   LDX   __zpabi_clear_cell__col
   LDA   #$A0
   STA   $0400,X
   RTS
```

With the two `const` qualifiers removed from the source, the pass
makes no change, and the body copies `screen` into DPTR, loads
`blank`, and stores through `(DPTR),Y`. That body takes 17 bytes and
27 cycles, where the one above takes 7 bytes and 10 cycles, and the
two objects no longer take 3 bytes of data.

## When it applies

A `Var` operand in a read position is replaced when the object it
names meets all of these conditions.

- It has static storage, which covers file-scope objects and
  block-scope `static`s, with either linkage.
- Its initializer is a number (`Initial` holding an `int` or a
  `float`). A pointer initialized from an integer cast, such as
  `(uint8_t *)0x0400`, qualifies.
- Its type is `const` at the outermost level and is not `volatile`.
- The type under the qualifier is a scalar, meaning a character or
  integer type, `float`, `double`, or a pointer.

The pass builds the table of such objects once per function from the
symbol table, and does nothing when there's no symbol table.

## What it leaves alone

- A static without `const`, even one the program never writes, since
  another function or translation unit may write it. A plain `static
  uint8_t *screen` reports no change.
- A pointer to `const` data, such as `static const uint8_t *p`. The
  pointer itself can still be reassigned, and only a `const` at the
  outermost level qualifies.
- A `const volatile` object, because each read of a `volatile` object
  is a side effect that has to happen.
- A pointer initialized with the address of an object, such as
  `static uint8_t * const screen = cells;`. Its value is an
  `AddressInit` that the assembler resolves, so it can't be a TAC
  constant, and the pass reports no change.
- Arrays, structures, and unions. An element read from a `const`
  array at a constant index is folded by the const-array-subscript
  fold in [constant folding](constant_folding.md).
- A tentative or uninitialized `static const`, which has no
  `Initial` value.
- The name in an `IndexedLoad`, which identifies the array being
  indexed, and every write position.

## Related

[Constant folding](constant_folding.md) evaluates the arithmetic the
new constants feed, and [const static fold](const_static_fold.md) is
the asm-level pass that removes the unread storage.
