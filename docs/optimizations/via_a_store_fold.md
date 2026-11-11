# Store X or Y directly

`apply_via_a_store_fold` in
[passes/via_a_store_fold.py](../../passes/via_a_store_fold.py) turns
`TXA; STA M` into `STX M` and `TYA; STA M` into `STY M`. It runs in
the peephole fixed point, which both pipelines run, but no program in
the test corpus makes it fire in the unoptimized pipeline.

`STX` and `STY` store an index register without going through A, so
the transfer is unnecessary when nothing afterward reads the copy in
A or the N and Z flags that `TXA` set. The fold saves 1 byte and 2
cycles. The pattern appears after other passes have put a value in X
or Y and then rewritten a later load of the same value into a
transfer. `redundant_load` turns `LDA M` into `TXA` when X already
holds M, which is where the `TXA` in the example comes from, and
`x_save_slot_load` does the same for a counter kept in X. It also
appears when `split_mem_to_mem` splits a memory-to-memory
copy whose load half was then rewritten that way.

## Example

```c
unsigned char table[8];
unsigned char idx, value, seen;
void fetch(void) {
    value = table[idx];
    seen = idx;
}
```

### Before

```
fetch:
   LDX   idx
   LDA   table,X
   STA   value
   TXA
   STA   seen
   RTS
```

### After

```
fetch:
   LDX   idx
   LDA   table,X
   STA   value
   STX   seen
   RTS
```

## When it applies

- A non-volatile `TXA` or `TYA` is directly followed by a non-volatile
  `STA` to a `Data` or `ZP` operand. `STX` and `STY` have zero-page
  and absolute modes, which cover both.
- A is dead after the store, by `a_dead_at`.
- N and Z are dead after the store, by `flags_dead_at`.

## What it leaves alone

- A read of A afterward. If `fetch` ends with `return idx;`, the
  value comes back in A, so `TXA; STA seen` stays.
- A branch on N or Z after the store that reads the flags `TXA` set.
- A store to the soft stack, through a pointer, or to an indexed
  address. Those use `(zp),Y` or indexed modes, and `STX` and `STY`
  have no `(zp),Y` mode.

## Related

[Redundant load](redundant_load.md) produced the `TXA` in the
example, [Split memory-to-memory moves](split_mem_to_mem.md) and
[X save-slot load](x_save_slot_load.md) produce others, and
[Transfer plus or minus one](transfer_pm1_store.md) handles a
transfer followed by an add or subtract of 1 before the store.
