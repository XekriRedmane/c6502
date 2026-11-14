# Operands without a staging temporary

`apply_cmp_through_tmp_fold` in
[passes/cmp_through_tmp_fold.py](../../passes/cmp_through_tmp_fold.py)
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
   LDA   __zpabi_check__idx
   AND   #$0F
   TAX
   LDA   table,X
   STA   __local_check__0
   LDA   y
   CMP   __local_check__0
   BNE   .if_end@0
   LDA   #$01
   STA   hit
.if_end@0:
   RTS
```

### After

```
check:
   LDA   __zpabi_check__idx
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

A byte read through a pointer folds the same way. For `if (y == *p)`
the pass turns `LDY #$00; LDA ($80),Y; STA __local_band__0; LDA
__zpabi_band__y; CMP __local_band__0` into `LDA __zpabi_band__y; LDY
#$00; CMP ($80),Y`, where `$80` is the zero-page slot that holds `p`.

### Arithmetic

The same staging happens for the right operand of an addition or a
subtraction, such as a global widened to 16 bits. In
`do_rwts_on_sector` (`examples/zork/instr_save/disk.c`), `sector -
SECTORS_PER_TRACK` subtracted a copy:

```
   LDA   SECTORS_PER_TRACK
   STA   __local_do_rwts_on_sector__command
   LDA   __local_do_rwts_on_sector__sector_0
   SEC
   SBC   __local_do_rwts_on_sector__command
```

and now subtracts the global itself:

```
   LDA   __local_do_rwts_on_sector__sector_0
   SEC
   SBC   SECTORS_PER_TRACK
```

That takes a pass through the loop from 42 cycles to 36. An
operand that reaches the temporary from a table or a global does the
same in `s += arr[i]`, which becomes `CLC; ADC arr,X` with the sum
kept in A.

## When it applies

- The four instructions are adjacent, except for a `CLC` or `SEC`
  before an `ADC` or `SBC`, and none is volatile.
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
  `CMP` on every path, by `asm_dead_store.mem_dead_after`, or every
  read of `tmp` anywhere in the function is the `CMP` of some
  matching window, in which case the pass folds all of those windows
  together. The second rule covers several comparisons that each
  stage their own value through the same slot.

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
  access.
- Arithmetic on a byte read through a pointer, which the emitter
  doesn't give `ADC` and the others.

## Related

[Redundant compare](redundant_compare.md) removes a second compare
of the same operands, and [Asm dead store](asm_dead_store.md)
supplies the dead-after check.
