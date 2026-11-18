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

The pass has five more rewrites that use the same lists.

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
- A store through A, `Mov(M, D)`, which emits as `LDA M; STA D`,
  becomes `STA D` alone when A already holds M. With `SCRATCH1 =
  0xFFFF;`, `LDA #$FF; STA SCRATCH1; LDA #$FF; STA SCRATCH1+1` loses
  its second `LDA`, as in Zork's `please_insert_save_diskette`. (The
  first `Mov(#$FF, SCRATCH1)` now leaves both `SCRATCH1` and `#$FF`
  in A's list.)
- `LDA X; STA D1; LDA M; STA D2`, with A already holding M, becomes
  `STA D2; LDA X; STA D1`, when neither store writes what the other
  pair reads or stores (D1 against M and D2, D2 against X) and A
  and N/Z are dead after. Nor may D1 be a byte of the pointer that
  D2 is stored through, or D2 a byte of X's pointer, which the pass
  checks with the addresses of the unit's zero-page slots
  (`zp_symbol_addrs`). Once the groups dropped a `LDA #7` before
  `STA (p),Y`, `LDA LEVEL_BG@hi,X; STA p+1; STA (p),Y` had become
  `STA (p),Y; LDA LEVEL_BG@hi,X; STA p+1`, which stored through the
  old pointer. Every case of `floor_enemy_draw`'s second
  dispatch leaves the high byte of the sprite pointer in A, and the
  join stored the low byte to `draw_sprite`'s parameter first:

  ```
  .dispatch@1@end:                      .dispatch@1@end:
     LDA   __local_floor_enemy_draw__1     STA   __zpabi_draw_sprite__tile_src_1
     STA   __zpabi_draw_sprite__tile_src_0 LDA   __local_floor_enemy_draw__1
     LDA   __local_floor_enemy_draw__0     STA   __zpabi_draw_sprite__tile_src_0
     STA   __zpabi_draw_sprite__tile_src_1
  ```

  The cases' stores of the high byte to `__local_floor_enemy_draw__0`
  are then dead, and `asm_dead_store` drops them.

  Two stores through one index, `name+k1,X` and `name+k2,X` with
  different offsets, reach different bytes, since nothing in the four
  instructions changes X. The same goes for Y when none of the four
  has the `LDY #off` of a soft-stack or indirect operand
  (`_same_index_apart`). Zork's `instr_call` stores a local's two
  bytes that way, and its second byte no longer goes through a slot:

  ```
     JSR   get_next_code_byte              JSR   get_next_code_byte
     STA   __local_instr_call__0           LDX   __local_instr_call__args
     LDX   __local_instr_call__args        STA   LOCAL_ZVARS+1,X
     LDA   __local_instr_call__1           LDA   __local_instr_call__1
     STA   LOCAL_ZVARS,X                   STA   LOCAL_ZVARS,X
     LDA   __local_instr_call__0
     STA   LOCAL_ZVARS+1,X
  ```

- `LDA #k; STA M`, or the one atom `Mov(#k, M)`, becomes `LDY #k;
  STY M` (or `LDX` / `STX`) when A holds a value from memory that's
  loaded again after the store, `k` isn't read afterwards, and Y (or
  X) is dead. The reload then goes in the second phase. `get_alphabet`
  (examples/zork/zstring.c) returns the alphabet it
  loaded after `SHIFT_ALPHABET = 0xFF`, and became Zork's own `LDY
  #$FF; STY SHIFT_ALPHABET; RTS` with no copy in a local.

These three change what A or Z holds at the end of a block, which would
make the lists a later block starts with wrong. So they're a phase of
their own: the pass applies them (keeping the swapped `LDA M`),
computes the lists again on the result, and only then drops loads.

A store from a register into a byte its list holds goes, since the
byte already holds the value.

When a register is loaded with something else, the bytes its list
held still hold one value. Two or more of them stay together as a
memory group, and a load of any byte of a group gives the register
the whole group as its list. So a store into a byte of the group then
goes. Lode Runner's tables of row addresses for `LEVEL_MAP` and
`LEVEL_BG` share their low bytes, and a function that reads a cell of
`LEVEL_BG` and then writes the same cell of `LEVEL_MAP` sets up the
second pointer with the same low byte. In `guard_fall_into_row_below`
(examples/lode_runner/guards.c)

```
   LDA   LEVEL_BG@lo,X                 LDA   LEVEL_BG@lo,X
   STA   p                             STA   p
   LDA   LEVEL_BG@hi,X                 LDA   LEVEL_BG@hi,X
   STA   p+1                           STA   p+1
   LDA   (p),Y                 ->      LDA   (p),Y
   BNE   .if_else@45                   BNE   .if_else@45
   LDA   LEVEL_BG@lo,X                 LDA   LEVEL_MAP@hi,X
   STA   p                             STA   p+1
   LDA   LEVEL_MAP@hi,X
   STA   p+1
```

`p` and `LEVEL_BG@lo,X` form a group when A is loaded from
`LEVEL_BG@hi,X`, and the group survives the load through the pointer
and the branch. The store through `p` that follows doesn't end it
either, since no pointer reaches the table, a `static` whose address
the unit never takes (`asm_aliasing.may_alias` now says so for an
indexed access to such an object too). The `LDA LEVEL_BG@lo,X` left
without its store goes as a dead load. The groups take 50 bytes off
Lode Runner.

A group ends where a list would lose the same operands: a write that
may alias a byte of it removes that byte, and a change to X or Y
removes every operand indexed by the register. That includes a
transfer into X or Y, `TAX` after `LDA t,X`, which the lists hadn't
been checked for, and which the groups needed, since a group outlives
the register. Two groups at a join keep what both paths agree on.

A load into A of a value A already holds, kept because a branch
reads the flags it sets, becomes `ORA #$00`: the same N and Z from A,
which equals the operand, in 2 cycles instead of 3 or 4.
`print_zstring`'s `STA c; BCC .l; RTS; .l: LDA c; BNE` became `...
.l: ORA #$00; BNE`.

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
  `Indirect` offset, or the same `IndexedData` name, offset, and
  index register. `tab,X` and `tab,Y` are different operands, so in
  `j = tab[i]; if (j == 0) return 0; g = tab[j];` the `LDA tab,Y`
  stays after `LDY tab,X; TYA`.
- The load's effect on N and Z can be skipped. Either no branch
  reads N or Z before another instruction sets them
  (`asm_liveness.flags_dead_at`), or Z already reflects whether the
  source is zero, because the instruction that last set the flags
  set them from the same value. An `INC`, `DEC`, shift, or rotate of
  a byte sets the flags from that byte alone, so afterwards Z
  reflects only it. A store that changes a byte ends Z's reflection
  of it and of every byte the store may alias. In `bool moves =
  pattern & 1; pattern >>= 1; if (moves) f();`, Z still counted as
  reflecting `moves` after `LSR pattern`, the `LDA moves` before the
  `BEQ` went, and the branch tested the shifted pattern. The load is
  now `ORA #$00`, since A still holds `moves`.
- A block's starting lists are the intersection of the lists at the
  end of every predecessor. What Z reflects is carried only into a
  block with a single predecessor, because on a merge the flags may
  come from a different instruction on each path, and later passes
  that delete one of those instructions don't track the dependence.

A write to memory removes from every list each operand the write
may alias (`asm_aliasing.may_alias`). A store through a pointer
leaves a byte no pointer can point to
(`asm_aliasing.unreachable_by_pointer`), which is a `static` whose
address the unit never takes or a function's own zero-page slot
(`__local_f__*`, `__zpabi_f__*`) whose address the unit never takes.
Zork's `read_line` (examples/zork/input.c) stores its
`c` through `text` and then compares it, and the second `LDA
__local_read_line__c` goes:

```
   LDA   __local_read_line__c          LDA   __local_read_line__c
   STA   (text),Y              ->      STA   (text),Y
   LDA   __local_read_line__c          CMP   #$0D
   CMP   #$0D
```

The slots aren't all out of reach, since `address_taken_zp` can put
an address-taken local in one, and only that function's code can take
its address. Changing X or Y, by a load,
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
- Anything after a `JSR`, which may change every register, except
  an immediate in X or Y when the routine is declared `preserves_x`
  or `preserves_y`. With `keep` declared `preserves_x` and taking its
  argument in X, `keep(5); keep(5);` loads X once. A memory value
  doesn't survive even then, since the routine may write the memory,
  so `keep(g); keep(g);` loads `g` twice. A `FunctionPrologue`,
  `AllocateStack`, or `LoadAddress` also clears the lists, because
  each expands into several instructions later.
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
