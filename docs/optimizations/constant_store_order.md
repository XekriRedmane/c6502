# Constant store order

`apply_constant_store_order` in
[passes/constant_store_order.py](../../passes/constant_store_order.py)
groups the stores of a run of stores by the value stored, so that each
value is loaded into A once. The values are constants, or bytes loaded
from memory that the run doesn't change. It runs in the asm peephole
fixed point, in both pipelines.

Code that sets up several variables stores one constant after
another, in the order of the C statements, and `tac_to_asm` loads each
constant into A for its store. When a value comes back after another
one, it's loaded again, 2 bytes and 2 cycles each time.

A value copied from memory into a local and then from the local into
a global comes back the same way. [Rematerialization](asm_remat.md)
and [memory value propagation](memory_value_propagation.md) replace
the copy from the local with a second load of the memory byte, which
reads the same value as long as nothing writes that byte or the index
register in between. The local is often read again later, so its
store stays, and the second load costs a byte more than the load of
the local it replaced. Grouping the stores drops the second load.

## Example

Zork's `main` (examples/zork/main.c) sets the printer's
`CSW` to `$C100` and then clears several of the interpreter's bytes.

### Before

```
   LDA   #$00
   STA   PRINTER_CSW
   LDA   #$C1
   STA   PRINTER_CSW+1
   LDA   #$00
   STA   ZCODE_PAGE_VALID
   STA   ZCODE_PAGE_VALID2
   STA   PAGE_TABLE_INDEX
   STA   PAGE_TABLE_INDEX2
   STA   BUFF_END
```

### After

```
   LDA   #$C1
   STA   PRINTER_CSW+1
   LDA   #$00
   STA   PRINTER_CSW
   STA   ZCODE_PAGE_VALID
   STA   ZCODE_PAGE_VALID2
   STA   PAGE_TABLE_INDEX
   STA   PAGE_TABLE_INDEX2
   STA   BUFF_END
```

These are two compiles of the unit, with the pass turned off and on.
The stores of 0 share one load. Further on, with the pass off, `main`
loads `#<page_tables` a second time for the third of the page cache's
four table pointers, which starts 256 bytes after the first. The pass
takes 4 bytes off `main` in all.

## Example with values loaded from memory

Lode Runner's `fill_hole` (examples/lode_runner/timers.c), inlined into
`handle_timers` with the brick's number in X, starts

```c
uint8_t row = BRICK_DIG_ROWS[i];
uint8_t col = BRICK_DIG_COLS[i];
GAME_ROWNUM = row;
GAME_COLNUM = col;
```

and reads `row` and `col` again further on.

### Before

```
   LDA   BRICK_DIG_ROWS,X
   STA   __local_handle_timers__2
   LDA   BRICK_DIG_COLS,X
   STA   __local_handle_timers__saved_guard_num
   LDA   BRICK_DIG_ROWS,X
   STA   GAME_ROWNUM
   LDA   BRICK_DIG_COLS,X
   STA   GAME_COLNUM
```

### After

```
   LDA   BRICK_DIG_ROWS,X
   STA   __local_handle_timers__2
   STA   GAME_ROWNUM
   LDA   BRICK_DIG_COLS,X
   STA   __local_handle_timers__saved_guard_num
   STA   GAME_COLNUM
```

These are compiles of timers.c before and after the pass took values
loaded from memory. The original game keeps the row in Y for the
same effect. The change takes 6 bytes off `handle_timers` (475 to
469), and 5 off `enable_next_level_ladders` in level.c, where the
column's copy was already in A.

## When it applies

- The run is a straight sequence of loads into A, each followed by
  one or more `STA m`. A load is `LDA #k`, where `k` is an immediate
  or a half of a label's address, `LDA n` of a `Data` byte, or `LDA
  n+k,X` or `LDA n+k,Y`. The asm IR's `Mov(#k, m)`, which `asm_emit`
  writes as `LDA #k; STA m`, counts as a load and a store, and the
  pass writes the run back as separate loads and stores.
- Two loads load the same value when their operands are the same,
  index register included. Nothing in the run writes X or Y, so `n,X`
  is the same byte at each load.
- Every `m` is a `Data` byte that no other store in the run names.
- No load and no store is volatile.
- No store in the run writes a byte that a load in it reads, since
  the pass moves stores ahead of loads and the last value's load
  behind stores. For a `Data` load that means no store to the same
  byte. For an indexed load of `n` it means no store to any byte of
  `n`. The index is taken to stay inside the object, as C requires of
  a subscript, and the rematerializations that made the second load
  are sound only on the same premise. An indexed load of a compiler slot
  (`__zpabi_*`, `__local_*`) or a runtime symbol such as `HARGS`
  leaves the run alone, since other names may cover those bytes.
- The run stores into and loads from the slots (`__zpabi_*` and
  `__local_*`) of one function at most, since the linker may give two functions that
  can't be on the call stack together the same bytes.
- At least one load goes. Each value's stores go after its first
  load, and the value the run loaded last stays last, so A and N/Z
  end the run as they did.

## What it leaves alone

- A store from X or Y, and a store with an index.
- A run with anything else in it, such as a label, a load into X or
  Y, a load through a pointer or from the soft stack, or a call. The
  value loaded before a label can be reused after it only by
  [redundant load elimination](redundant_load.md), which follows A
  across the control flow.
- A store of the same byte twice in one run.
- A load of a `Data` byte that the run stores, and an indexed load
  of an object the run stores into.

## Related

[Redundant load elimination](redundant_load.md) drops a load of a
value A already holds, which is what remains of each group's loads.
