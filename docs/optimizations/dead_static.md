# Dead static elimination

`apply_dead_static_elimination` in
[passes/optimization_asm/dead_static.py](../../passes/optimization_asm/dead_static.py)
drops each internal-linkage static object that no instruction and no
initializer refers to. It runs at the end of each call of
`optimize_program`, after every function has been through the asm SSA
round trip, so it runs twice per compile. It runs only under
`--optimize`.

Several folds remove every read of a static and leave its storage in
place. The TAC [constant folding](constant_folding.md) turns
`gain[2]` on a const array into a constant, and after the
[unroller](unroll.md) has made every index of a loop constant, it can
fold all the reads of a table. This pass then removes the table's
`DC.B` or `DS.B` bytes from the output. It runs after the
per-function passes so that a static whose last read was a dead load,
dropped by [byte DCE](byte_dce.md) or a copy propagation, is counted
as unreferenced.

## Example

```c
static const unsigned char gain[4] = {10, 20, 30, 40};
unsigned char loud(void) { return gain[2]; }
```

Since the pass changes only the list of top-level objects, the
listings are two complete compiles with `--optimize`, the first with
the pass replaced by one that returns the program unchanged.

### Before

```
loud:
   SUBROUTINE

   LDA   #$1E
   RTS

gain:
   DC.B  $0A, $14, $1E, $28
```

### After

```
loud:
   SUBROUTINE

   LDA   #$1E
   RTS
```

## When it applies

A `StaticVariable` is dropped when all of these are true.

- It has internal linkage (`is_global` is False). It doesn't have to
  be const, so an unused `static unsigned char spare[4];` goes too.
- No operand of any instruction in any function names it. The scan
  looks at every operand field of every instruction and counts a
  `Pseudo`, `Data`, `IndexedData`, `ImmLabelLow`, or `ImmLabelHigh`
  with that name.
- No `AddressInit` in any static's initializer names it.

## What it leaves alone

- An object with external linkage, even when nothing in the file
  reads it, because another translation unit can refer to it by
  name. `const unsigned char gain[4]` without `static` keeps its
  storage.
- A static named only in the initializer of another dead static. The
  pass collects the referenced names once and then filters, so with
  `static unsigned char x; static unsigned char *p = &x;` and no code
  using either, `p` is dropped and `x` stays. Each call of
  `optimize_program` starts again from the same `tac_to_asm` output,
  so the second call doesn't drop `x` either.
- Functions, which are never `StaticVariable`s.

## Related

[Const-static fold](const_static_fold.md) drops the storage of a
scalar const static itself, after rewriting its reads.
