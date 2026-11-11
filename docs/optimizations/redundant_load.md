# Redundant load elimination

`apply_redundant_load_elimination` in
[passes/redundant_load.py](../../passes/redundant_load.py) drops a
load into A, X, or Y when the register already holds the value being
loaded. It runs in the peephole fixed point, in both the optimized
and the unoptimized pipelines, later in each iteration than
`apply_direct_index_load`, so that its input holds `LDX M` where the lowering wrote
`LDA M; TAX`.

The pass records, for each register, the list of operands whose
current value the register holds. `LDA M` puts M in A's list, and a
following `STA N` adds N to the same list, since the store leaves A
unchanged. When a later `LDA M` or `LDA N` finds the operand already
in A's list, the load goes. A load from zero page is 2 bytes and 3
cycles and an absolute load is 3 bytes and 4 cycles. The lists are
computed over the function's control-flow graph, so a register's
list at the start of a block keeps an operand when every path into
that block leaves the operand's value in the register.

The pass has two more rewrites that use the same lists.

- A transfer between registers (`TAX`, `TAY`, `TXA`, `TYA`) goes
  when both registers already hold the same value. In `LDX state;
  TXA; BPL .if_end@0; TXA; AND #$0F`, the second `TXA` is dropped.
- `LDA M` becomes `TXA` (or `TYA`) when A doesn't hold M but X (or
  Y) does. With `void ident(unsigned char i) { tab[i] = i; }`, the
  pass turns `LDX __zpabi_ident__i; LDA __zpabi_ident__i` into `LDX
  __zpabi_ident__i; TXA`. That saves a byte and a cycle for a
  zero-page operand, and 2 bytes and 2 cycles for an absolute one.
  A then holds M too, so `apply_via_a_store_fold` can turn a
  following `TXA; STA N` into `STX N` when A is dead.

## Example

```c
unsigned char row0[8], row1[8], row2[8];
void fill(unsigned char col, unsigned char color) {
    row0[col] = color;
    row1[col] = color;
    row2[col] = color;
}
```

### Before

```
   LDX   __zpabi_fill__col
   LDA   __zpabi_fill__color
   STA   row0,X
   LDX   __zpabi_fill__col
   LDA   __zpabi_fill__color
   STA   row1,X
   LDX   __zpabi_fill__col
   LDA   __zpabi_fill__color
   STA   row2,X
   RTS
```

### After

```
   LDX   __zpabi_fill__col
   LDA   __zpabi_fill__color
   STA   row0,X
   STA   row1,X
   STA   row2,X
   RTS
```

The parameters are zero-page slots, and an indexed store into a
static array can't reach zero page, so `STA row0,X` leaves both
lists intact. The pass removes 8 bytes and 12 cycles here. The win
grows with loop unrolling, where the same loads repeat once per
unrolled iteration.

## When it applies

- The instruction is a non-volatile `Mov` into a register, and the
  register's list holds an operand equal to the source. Operands
  compare equal when they're the same immediate, the same `ZP` or
  `Data` byte, the same `Frame` or `Stack` offset, the same
  `Indirect` offset, or the same `IndexedData` name and offset.
- The load's effect on N and Z can be skipped. Either no branch
  reads N or Z before another instruction sets them
  (`asm_liveness.flags_dead_at`), or Z already reflects whether the
  source is zero, because the instruction that last set the flags
  set them from the same value.
- A block's starting lists are the intersection of the lists at the
  end of every predecessor. What Z reflects is carried only into a
  block with a single predecessor, because on a merge the flags may
  come from a different instruction on each path, and later passes
  that delete one of those instructions don't track the dependence.

A write to memory removes from every list each operand the write
may alias (`asm_aliasing.may_alias`). Changing X or Y, by a load,
`INX` / `DEX`, or the `LDY #off` that sets up a `Frame`, `Stack`, or
`Indirect` access, also removes every indexed operand whose address
depends on that register.

## What it leaves alone

- A volatile load, which C99 §6.7.3 requires to read memory every
  time. With `volatile unsigned char port;`, `a = port; b = port;`
  keeps both `LDA port`.
- A load of a static scalar after a store through an indexed static
  array. The array's address isn't known before linking, so `STA
  row0,X` may overwrite any static. With `row0`, `col`, and `color`
  all file-scope statics, the example above keeps all six loads.
- Anything after a `JSR`, which may change every register. A
  `FunctionPrologue`, `AllocateStack`, or `LoadAddress` also clears
  the lists, because each expands into several instructions later.
- A load at a merge point where one incoming path changed the
  register or the memory.
- A load whose N and Z feed a later branch when Z doesn't already
  reflect the source, including any such load right after a label
  that two paths reach.
- A read through `(DPTR),Y` with Y set elsewhere, or through a
  zero-page pointer (`IndirectY`, `IndirectZp`, `IndirectZpY`).
  These never compare equal, since the byte they read depends on a
  pointer or on Y.

## Related

[Redundant store elimination](redundant_store.md) catches the
memory-to-memory copies that this pass can't track once A is
overwritten. [Direct index load](direct_index_load.md) runs first
and produces the `LDX M` form tracked here.
