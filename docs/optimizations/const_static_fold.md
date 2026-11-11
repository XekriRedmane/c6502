# Const-static fold

`fold_const_statics` in
[passes/optimization_asm/const_static_fold.py](../../passes/optimization_asm/const_static_fold.py)
replaces each read of a byte of a `static` const scalar with an
immediate holding that byte, and drops the static's storage. It runs
once at the start of each call of `optimize_program`, before the
per-function asm SSA round trip, so it runs twice per compile (the
preliminary sizing run and the final run). It runs only under
`--optimize`.

A read of a static in memory is an absolute load, and `LDA name`
takes 3 bytes and 4 cycles, where `LDA #imm` takes 2 bytes and 2
cycles. The static's own storage goes too, which is 2 bytes for a
pointer. The canonical case is a pointer to a fixed address, such as
a screen page or an I/O register, declared `static T * const`.

The TAC-level [static const fold](static_const_fold.md) runs earlier
and replaces every `Var` read of such a static with a constant, so in
a program compiled from C this pass finds no reads left to rewrite. A
file that reads such statics in a return, a call argument, a store, a
compound assignment, an array index, a `switch` arm, and a
block-scope `static const` gave 0 changed functions. What remains is
the second half of the job, dropping the storage of a static the TAC
pass made unreferenced. [Dead static elimination](dead_static.md)
would drop it later in the same call anyway.

## Example

```c
static unsigned char * const screen = (unsigned char * const)0x2000;
void clear(unsigned char i) { screen[i] = 0; }
```

The listings come from a compile with the TAC-level fold turned off,
because with it on, the TAC pass has already removed both reads.

### Before

```
Mov(Pseudo(screen, 0), A)
Mov(A, Data(DPTR, 0))
Mov(Pseudo(screen, 1), A)
Mov(A, Data(DPTR, 1))
...
```

### After

```
Mov(Imm(0), A)
Mov(A, Data(DPTR, 0))
Mov(Imm(32), A)
Mov(A, Data(DPTR, 1))
...
```

The `screen: DC.W $2000` storage is gone from the output as well.
With the TAC-level fold on, `clear` compiles to `LDX
__zpabi_clear__i; LDA #$00; STA $2000,X; RTS`, since the constant
address reaches the TAC indexed-store recognizer and the pointer
never goes through DPTR.

## When it applies

A `StaticVariable` is a candidate when all of these are true.

- It has internal linkage (`is_global` is False), which covers a
  file-scope `static` and any block-scope `static`.
- Its symbol-table type has a `Const` qualifier at the top level, and
  no `Volatile` in that qualifier layer. The check stops at the first
  non-qualifier type, so `const unsigned char *p` (a pointer to const)
  isn't a candidate.
- Its initializer is a single `CharInit`, `IntInit`, `LongInit`,
  `LongLongInit`, `FloatInit`, or `DoubleInit`.

A candidate is dropped from the set if the program names it as the
source of a `LoadAddress`, in an `ImmLabelLow` or `ImmLabelHigh`
operand (which is how `&name` lowers for a static), in an
`IndexedData` operand, in an `AddressInit` of another static, or as
the destination of any write (`Mov`, `Add`, `Sub`, `And`, `Or`,
`Xor`, `Inc`, `Dec`, a shift or rotate, or `Pop`). The type checker
rejects a write to a const object, so the last check is a safety
net.

For each surviving candidate, the pass rewrites `Pseudo(name, k)`
into `Imm` of byte `k` of the initializer, little-endian, in every
read position. Those are the source of `Mov`, `Add`, `Sub`, `And`,
and `Or`, both sources of `Xor`, the source of `Push`, and both sides
of `Compare`. Then it drops the `StaticVariable`. The pass does
nothing when no symbol table is passed in.

## What it leaves alone

- A const object without `static`. Another translation unit can read
  the symbol by name, so its storage has to stay. A file-scope
  `unsigned char * const screen` keeps its `DC.W $2000` and its `LDA
  screen` reads.
- A static whose address is taken. `return &page2;` lowers to `LDA
  #<page2` and `LDA #>page2`, which require the symbol to exist.
- Arrays, structs, strings, and a pointer initialized with the
  address of another object (`AddressInit`). Constant reads of array
  elements are folded at the TAC level instead, by
  [constant folding](constant_folding.md).
- A zero initializer. `static const unsigned char z = 0;` is emitted
  as `ZeroInit`, which this pass skips. The TAC-level fold handles it,
  so in a normal compile `z` is still read as `#$00` and dropped.
- A `const volatile` static, because each read of a volatile object
  is a side effect that an immediate would remove.

## Related

[Static const fold](static_const_fold.md) is the TAC-level pass that
does most of the work. [Dead static elimination](dead_static.md)
drops statics that other folds left unreferenced.
