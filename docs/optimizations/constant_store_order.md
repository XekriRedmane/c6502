# Constant store order

`apply_constant_store_order` in
[passes/constant_store_order.py](../../passes/constant_store_order.py)
groups the stores of a run of constant stores by value, so that each
value is loaded into A once. It runs in the asm peephole fixed point,
in both pipelines.

Code that sets up several variables stores one constant after
another, in the order of the C statements, and `tac_to_asm` loads each
constant into A for its store. When a value comes back after another
one, it's loaded again, 2 bytes and 2 cycles each time.

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

## When it applies

- The run is a straight sequence of `LDA #k`, where `k` is an
  immediate or a half of a label's address, each followed by one or
  more `STA m`. The asm IR's `Mov(#k, m)`, which `asm_emit` writes as
  `LDA #k; STA m`, counts as a load and a store, and the pass writes
  the run back as separate loads and stores.
- Every `m` is a `Data` byte that no other store in the run names.
  Nothing in the run reads memory, so the order of stores to
  different bytes can't be seen.
- No store is volatile.
- The run stores into the slots (`__zpabi_*` and `__local_*`) of one
  function at most, since the linker may give two functions that
  can't be on the call stack together the same bytes.
- At least one load goes. Each value's stores go after its first
  load, and the value the run loaded last stays last, so A and N/Z
  end the run as they did.

## What it leaves alone

- A store from X or Y, and a store with an index.
- A run with anything else in it, such as a label, a load from
  memory, or a call. The value loaded before a label can be reused
  after it only by [redundant load elimination](redundant_load.md),
  which follows A across the control flow.
- A store of the same byte twice in one run.

## Related

[Redundant load elimination](redundant_load.md) drops a load of a
value A already holds, which is what remains of each group's loads.
