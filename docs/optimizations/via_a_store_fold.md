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

## A copy while A is busy

`apply_copy_through_index`, in the same module, copies a byte through
X or Y when the code parks A in a slot to make the copy:

```
   STA   __local_do_var__n          LDX   OPERANDS
   LDA   OPERANDS            ->     STX   __local_do_var__3
   STA   __local_do_var__3
   LDA   __local_do_var__n
```

That's the entry of `do_var` in `examples/zork/decode.c`,
which copies `OPERANDS[0]` into a slot while A holds the opcode it
dispatches on, and it saves 4 bytes. It applies when the slot is one
of the function's own `__local_*` slots that nothing reads after the
`LDA` back, the source is a byte or a constant `LDX` can load, the
destination a byte `STX` can store, and N and Z are dead after, since
they now come from the source. X must be dead from the `STA` on, or
else Y, which takes its place. A ends with the value it had.

The copies may be several, each a load and any number of stores or a
memory-to-memory `Mov`, and a constant already in the register isn't
loaded again. `print_bottom_centered` in `examples/ali_baba/text.c`
gets its pointer in A and X and sets the cursor before passing it on.
The tail call reads X, so Y carries the constants:

```
   STA   __local_print_bottom_centered__s       LDY   #$17
   LDA   #$17                                   STY   TEXT_ROW
   STA   TEXT_ROW                        ->     LDY   #$00
   LDA   #$00                                   STY   TEXT_COL
   STA   TEXT_COL                               JMP   print_centered_plus_col
   LDA   __local_print_bottom_centered__s
   JMP   print_centered_plus_col
```

A store before the first load stays `STA`, since A still holds the
parked value there. That form, an argument in A held around constant
stores at a function's entry, saved 4 bytes each in Ali Baba's
`print_bottom_centered`, `print_at`, `apply_damage`,
`event_faction_scene_cont`, `drop_gold_exec`, and `rwts_dispatch`,
Zork's `branch_to_offset`, `do_2op`, and `instr_mod`, and Lode
Runner's `drop_player_in_hole`, 40 bytes in all.

It runs once, after the last peephole run, `carry_known`, and the
[jump tables](jump_table.md). Inside the peephole fixed point it took
the `LDA` of copies that other folds remove outright, and Zork's
`branch_to_offset` came out 6 bytes longer. Before the jump table,
`do_var`'s dispatch began with a `BEQ` that read the Z from the `LDA`
back, and the jump table's bounds check makes those flags dead.

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
