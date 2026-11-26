# Operands without a staging temporary

`apply_cmp_through_tmp_fold` in
[cpp/src/passes/cmp_through_tmp_fold.h](../../cpp/src/passes/cmp_through_tmp_fold.h)
rewrites `LDA src; STA tmp; LDA other; CMP tmp` as `LDA other; CMP
src`, and does the same when the last instruction is an `ADC`, `SBC`,
`AND`, `ORA`, or `EOR` of `tmp`, after an optional `CLC` or `SEC`. It
runs in the peephole fixed point, which both pipelines run, but no
program in the test corpus makes it fire in the unoptimized pipeline,
where the staging temporary is a soft-stack slot and the pass accepts
only a `Data` or `ZP` one.

When the right-hand side of a comparison is computed first, the
lowering parks it in a temporary, loads the left-hand side, and
compares against the temporary. `CMP` has every addressing mode the
staging `LDA` used, including `abs,X`, `abs,Y`, and `(zp),Y`, so it
can read the right-hand side where it already is. For a table read
through X and a zero-page temporary, the four instructions take 10
bytes and 14 cycles, and the two that replace them take 6 bytes and 8
cycles. A ends up holding `other` either way, and the flags come from
comparing the same two bytes.

## Example

```c
unsigned char table[16];
unsigned char y;
unsigned char hit;
void check(unsigned char idx) {
    if (y == table[idx & 0x0F])
        hit = 1;
}
```

### Before

```
check:
   AND   #$0F
   TAX
   LDA   table,X
   STA   __local_check__idx
   LDA   y
   CMP   __local_check__idx
   BNE   .if_end@0
   LDA   #$01
   STA   hit
.if_end@0:
   RTS
```

### After

```
check:
   AND   #$0F
   TAX
   LDA   y
   CMP   table,X
   BNE   .if_end@0
   LDA   #$01
   STA   hit
.if_end@0:
   RTS
```

`idx` arrives in A, so the `AND` needs no load in front of it. A byte read through a pointer folds the
same way. For `if (y == *p)` in `void band(unsigned char y, unsigned
char *p)`, the pass turns `LDY #$00; LDA ($80),Y; STA
__local_band__0; LDA __local_band__y; CMP __local_band__0` into `LDA
__local_band__y; LDY #$00; CMP ($80),Y`, where `$80` is the
zero-page slot that holds `p`. `y` arrives in A and is stored in
`__local_band__y` at entry.

### Arithmetic

The same staging happens for the right operand of an addition or a
subtraction, such as a global widened to 16 bits. In
`do_rwts_on_sector` (`examples/zork/disk.c`), when this pass was
added, `sector - SECTORS_PER_TRACK` subtracted a copy:

```
   LDA   SECTORS_PER_TRACK
   STA   __local_do_rwts_on_sector__command
   LDA   __local_do_rwts_on_sector__sector_0
   SEC
   SBC   __local_do_rwts_on_sector__command
```

and the pass made it subtract the global itself:

```
   LDA   __local_do_rwts_on_sector__sector_0
   SEC
   SBC   SECTORS_PER_TRACK
```

That took a pass through the loop from 42 cycles to 36. The loop
now keeps `sector` in A and X, and runs `SBC SECTORS_PER_TRACK` with
no load in front of it. An
operand that reaches the temporary from a table or a global does the
same in `s += arr[i]`, which becomes `CLC; ADC arr,X` with the sum
kept in A.

A byte read through a zero-page pointer with Y already loaded,
`(zp),Y`, folds the same way, with `other` one that loads without
setting Y, so Y still holds the offset at the `SBC`. In
`print_centered_plus_col` (`examples/ali_baba/text.c`), `40 - s[0]`
had been

```
   LDY   #$00
   LDA   (__local_print_centered_plus_col__s),Y
   STA   __local_print_centered_plus_col__0
   LDA   #$28
   SEC
   SBC   __local_print_centered_plus_col__0
```

and is now the original's `LDY #$00; LDA #$28; SEC; SBC
(__local_print_centered_plus_col__s),Y`, 4 bytes shorter. A `(zp),Y`
source that loads its own Y (`IndirectZp`) is left for the compare
alone.

## When it applies

- The four instructions are adjacent, except for a `CLC` or `SEC`
  before an `ADC` or `SBC`. Only the first load may be volatile: the
  `CMP` (or arithmetic instruction) then reads that byte once, after
  the load of `other`, which mustn't be volatile too.
  `dump_buffer_with_more`'s `++CURR_LINE >= WNDBTM`
  (examples/zork/output.c), with the Monitor's volatile
  `WNDBTM`, became Zork's own `INC CURR_LINE; LDA CURR_LINE; CMP
  WNDBTM`.
- `tmp` is a `Data` or `ZP` operand, and the `STA` and the `CMP` (or
  arithmetic instruction) name the same one.
- For `ADC`, `SBC`, `AND`, `ORA`, and `EOR`, `src` is an immediate, a
  `Data` or `ZP` operand, or an `abs,X` or `abs,Y` table read, the
  modes `asm_emit` gives those instructions.
- `src` is an immediate, a `Data` or `ZP` operand, an `abs,X` or
  `abs,Y` table read, or a read through a pointer with Y (`(zp),Y`,
  either with its own `LDY` setup or with Y already loaded). When
  `src` uses Y, `other` must not be a soft-stack or pointer operand,
  because loading one of those emits an `LDY` first.
- `other` is an immediate, or a `Data` or `ZP` operand that differs
  from `tmp`. Any indexed, indirect, or soft-stack `other` is
  rejected, since the pass can't rule out that it addresses `tmp`.
- Dropping the store loses nothing. Either `tmp` is dead after the
  `CMP` on every path, by `asm_dead_store.mem_dead_after`, or `tmp` is
  one of the function's own `__local_*` slots, whose address it never
  takes, and every read of `tmp` anywhere in the function is the `CMP`
  of some matching window, in which case the pass folds all of those
  windows together. The second rule covers several comparisons that
  each stage their own value through the same slot. It used to take
  any `tmp`, and in a loop with `g3 = p; mix(w + g3);` it dropped the
  store to the zero-page global `g3`, whose other readers are in other
  functions (`fuzz.stmts`, seed 1809).

## What it leaves alone

- An `other` read through a pointer. In

  ```c
  void band(unsigned char *p, unsigned char idx) {
      unsigned char t = table[idx & 0x0F];
      if (*p == t) hit = 1;
      if (p[1] == t) hit = 2;
  }
  ```

  both windows load `(p),Y` as `other`, and the pass reports no
  change. The same function also reads the temporary twice from one
  store, which the ownership rule rejects on its own.
- A temporary that is read somewhere else in the function while it
  still holds the staged value, by any instruction, a `BIT` included.
- A temporary on the soft stack, and any window with a volatile
  access other than the first load.
- Arithmetic on a byte read through a pointer, which the emitter
  doesn't give `ADC` and the others.

## Related

[Redundant compare](redundant_compare.md) removes a second compare
of the same operands, and [Asm dead store](asm_dead_store.md)
supplies the dead-after check.
