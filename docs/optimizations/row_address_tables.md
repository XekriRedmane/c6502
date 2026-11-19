# Row-address tables

`apply_row_address_tables` in
[passes/optimization/row_address_tables.py](../../passes/optimization/row_address_tables.py)
computes the address of a row of a 2-D array by looking it up in a
table of the rows' addresses, when the rows aren't a power of two
bytes long. `compile.optimized_asm` calls it on the whole program
before the TAC optimizer, so its input is the TAC from `c99_to_tac`.
It doesn't run in the unoptimized pipeline.

`A[i][j]` on an array of N rows of R bytes, more than 255 bytes in
all, computes `&A + i * R` at 16 bits. With R a sum or difference of
two powers of two, such as 28 = 32 - 4, strength reduction makes that
two 16-bit shifts and a subtract, and the add of the array's address
follows, about 40 bytes in all. Any other R calls `mul16`. 6502
programmers keep a table of row addresses instead, split into low and
high bytes as [split tables](split_tables.md) are, and the pass does
the same:

    LDX i; LDA lo,X; STA p; LDA hi,X; STA p+1

The table holds the N+1 addresses `A`, `A+R`, ..., `A+N*R`. The last
one is the address one past the last row, which `&A[N]` may compute.
Any other row index is undefined (C99 §6.5.6.8), so the table needs
no more entries.

The two halves are objects of their own, `A@lo` and `A@hi`, named
after the array. `A[i + k]` and `A[i - k]` look up `A@lo+k,X` and
`A@lo-k,X`. For an array with a fixed address
(`[[c6502::address(addr)]]`) the halves hold numbers, and for any other
static array they hold `AddressByteInit` bytes, written `DC.B
<(A+R*r)` and `DC.B >(A+R*r)`, which the linker fills in.

In the TAC, the lookup is an `IndexedLoad` of a table `A@rows` that
holds both halves, with `plane` set to N+1, so constant folding and
`tac_sim` read it as they read any table split into byte planes
([split tables](split_tables.md)). `A@rows` is never emitted. Its
symbol names the two objects (`StaticAttr.planes`), and `tac_to_asm`
emits them in its place and reads byte j of a lookup from the j-th of
them.

## Example

```c
#include <stdint.h>
extern uint8_t MAP [[c6502::address(0x0800)]][16][28];
uint8_t at(uint8_t row, uint8_t col) { return MAP[row][col]; }
uint8_t below(uint8_t row, uint8_t col) { return MAP[row + 1][col]; }
```

These listings compare two complete compiles, with and without the
pass. `below` shrinks from 68 bytes to 19 and `at` from 62 to 19, and
the 34-byte table brings the unit from 130 bytes to 72.

### Before

```
   STA   __local_below__row
   TXA
   TAY
   LDA   __local_below__row
   CLC
   ADC   #$01
   STA   __local_below__2
   LDA   #$00
   ADC   #$00
   ASL   __local_below__2
   ROL
   ASL   __local_below__2
   ROL
   STA   __local_below__3
   LDA   __local_below__2
   ASL
   STA   __local_below__0
   LDA   __local_below__3
   ROL
   ASL   __local_below__0
   ROL
   ASL   __local_below__0
   ROL
   STA   __local_below__1
   LDA   __local_below__0
   SEC
   SBC   __local_below__2
   STA   __local_below__row
   LDA   __local_below__1
   SBC   __local_below__3
   STA   __local_below__1
   LDA   #<MAP
   CLC
   ADC   __local_below__row
   STA   __local_below__row
   LDA   #>MAP
   ADC   __local_below__1
   STA   __local_below__0
   LDA   (__local_below__row),Y
   RTS
```

### After

```
   STA   __local_below__row
   TXA
   TAY
   LDX   __local_below__row
   LDA   MAP@lo+1,X
   STA   __local_below__row
   LDA   MAP@hi+1,X
   STA   __local_below__0
   LDA   (__local_below__row),Y
   RTS
...
MAP@lo:
   DC.B  $00, $1C, $38, $54, $70, $8C, $A8, $C4, $E0, $FC, $18, $34, $50, $6C, $88, $A4
   DC.B  $C0

MAP@hi:
   DC.B  $08, $08, $08, $08, $08, $08, $08, $08, $08, $08, $09, $09, $09, $09, $09, $09
   DC.B  $09
```

## When it applies

The pass looks for this TAC, where every temporary is defined once and
read once in the function.

```
GetAddress(A, a)
ZeroExtend(i, z)
Binary(+ or -, z, k, x)      ; any number of these, or none
Binary(*, x, R, m)
Binary(+, a, m, p)
```

- `A` is an array with static storage outside zero page, defined in
  the unit, declared `extern`, or at a fixed address. R, the size of
  its elements, is at least 3 and isn't a power of two.
- `i` is an unsigned one-byte variable that isn't volatile.
- The constants add up to a `k` from -255 to N.

`c99_to_tac` writes this shape for `A[i]` whenever A spans more than
255 bytes, so `A[i][j]` as a load or a store, `&A[i]`, `&A[i][j]`,
`A[i]` as a pointer to its row, an array of 2-byte elements, and an
array of structs all qualify. The lookup goes where the `ZeroExtend`
read `i`, so `i` is read at the same point as before.

A table costs 2(N+1) bytes in each unit that makes it. A lookup saves
about 40 bytes over shifts and an add or subtract, and about 10 over
a call of `mul16`, whose setup `outline_runs` often shares between
accesses. Those figures were measured on `M[r][c]`, `M[r][c] = v`, and
`M[r]` for row sizes from 20 to 104. The pass makes the tables for the
set of arrays whose accesses save the most bytes beyond what their
tables cost, and makes none when no set saves bytes. One access to a
16-row array saves 40 bytes against a 34-byte table, and two accesses
to a 40-row array save 80 against 82, so the first gets a table and
the second doesn't.

Arrays with the same number of rows share a half when its bytes are
the same, and the half is one object, named after the first array in
alphabetical order that has it. Lode Runner's `LEVEL_MAP` and
`LEVEL_BG` are 0x200 bytes apart, so their rows' low bytes are the
same, and a unit that indexes both has `LEVEL_BG@lo`, `LEVEL_BG@hi`,
and `LEVEL_MAP@hi`, 51 bytes in all. `LEVEL_MAP`'s lookups read
`LEVEL_BG@lo,X` for the low byte and `LEVEL_MAP@hi,X` for the high
byte. Since sharing makes an array cheaper once another is chosen,
the pass tries every set of the arrays with the same number of rows
when there are at most 10 of them.

## Across units

Each unit that indexes an array makes its own halves, and the linker
keeps one copy of the halves whose bytes are the same
([linker scripts](../linker_scripts.md#tables-several-files-make)).
The pass marks each half `merge=true` in the unit's link metadata.
Nothing but these lookups reads a half, nothing writes one, and
nothing takes the address of one, so a program behaves the same
whichever copy its lookups read. A half is the same in two units when its bytes and
its relocations are, so the halves of one `extern` array merge, and
the halves of two units' `static` arrays of one name stay apart.

Lode Runner indexes `LEVEL_MAP` and `LEVEL_BG` in five units. Four of
them made all three halves and `editor.c` made `LEVEL_MAP@lo` and
`LEVEL_MAP@hi`, which has the same bytes as `LEVEL_BG@lo`, 238 bytes
in all. The linked program keeps the three halves of `level.c`, and its
data shrank from 1784 bytes to 1597.

## What it leaves alone

- A row size that's a power of two, which costs only shifts.
- An array of at most 255 bytes, whose offsets `c99_to_tac` computes
  in a byte.
- An index wider than a byte, or a signed one, which the table can't
  take as an X index.
- An array reached through a pointer, or a local array, whose address
  isn't known at link time.
- A unit whose accesses don't save more than the tables cost.

The cost model counts a unit's halves at their full size, although
the linker may keep another unit's copy and drop this one's. The
compile of one unit has nothing to go on about which halves other
units make.

## Related

[Split tables](split_tables.md) stores existing tables the same way.
[Indirect-indexed access through a pointer](recognize_indirect_indexed.md)
turns the read through the row's address into `(p),Y`.
