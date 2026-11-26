# Shift in A

`apply_shift_in_a` in
[cpp/src/passes/shift_in_a.h](../../cpp/src/passes/shift_in_a.h) moves
a run of shifts and rotates of a byte that was just stored from A onto
A itself. It runs in the peephole fixed point, in both pipelines.

A shift or rotate of a zero-page byte takes 2 bytes and 5 cycles. The
same operation on A takes 1 byte and 2 cycles. When A already holds
the byte, because the code just stored it, the shifts can work on A,
and one store at the end leaves memory as it would have been:

```
   STA   t                ASL   A
   ASL   t        →       STA   t
   LDA   t
```

The `LDA t` that read the result back goes away, because A now holds
it. If nothing reads `t` afterwards, [asm dead-store
elimination](asm_dead_store.md) then deletes the `STA t`.

The flags come out the same. `ASL A` sets C, N, and Z as `ASL t`
does, `STA` changes no flags, and a rotate reads the same carry
either way. The `LDA t` set N and Z from `t`'s final value, which the
last shift already set them to.

## Example

```c
code = (uint8_t)(ZCHARS >> 10) & 0x1F;
```

from `get_next_zchar` in `examples/zork/zstring.c`,
where `ZCHARS` is a 16-bit zero-page global. The shift by 10 moves
the high byte down and shifts it twice more, and the bytes of the
result above the low one are dead. The "Before" listing is the same
`-O` compile without this pass.

### Before

```
   LDA   SCRATCH2+1
   STA   ZCHARS+1
   STA   __local_get_next_zchar__compound_literal_1
   LSR   __local_get_next_zchar__compound_literal_1
   LSR   __local_get_next_zchar__compound_literal_1
   LDA   __local_get_next_zchar__compound_literal_1
   AND   #$1F
```

### After

```
   LDA   SCRATCH2+1
   STA   ZCHARS+1
   LSR
   LSR
   AND   #$1F
```

The temporary isn't read again, so its store is gone too. The four
instructions that shifted through it, 8 bytes and 16 cycles, become
two `LSR A`s, 2 bytes and 4 cycles.

### A widened byte

`out = (unsigned int)c << 2` is lowered by `tac_to_asm` as the low
byte shifted once in A, the bit it shifts out rotated into a zero,
and then one more shift of both bytes in memory:

```
   LDA   c
   ASL
   STA   out
   LDA   #$00
   ROL
   STA   out+1
   ASL   out
   ROL   out+1
```

The `ASL out` of the other byte stays where it is, and the `ROL` of
the high byte moves to A, with the store after it:

```
   LDA   c
   ASL
   STA   out
   LDA   #$00
   ROL
   ASL   out
   ROL
   STA   out+1
```

That's 17 bytes and 26 cycles instead of 19 and 30. A shift of a
widened byte by 3 or more doesn't get here: `tac_to_asm` lowers it as
two byte shifts in A (see [strength reduction](strength_reduction.md)).

## When it applies

- An `STA M` stores A to a zero-page or absolute byte, and isn't
  volatile.
- After it come shifts (`ASL`, `LSR`, `ROL`, `ROR`) of `M`, which
  move to A, mixed with shifts of other zero-page or absolute bytes
  of the same kind as `M` and known to differ from it, which stay.
  Shifts of other bytes after the last shift of `M` aren't part of
  the run.
- Constant stores right after the `STA` (`LDA #k; STA H`, or one
  `Mov(#k, H)`) move up in front of the code that computed A for the
  `STA`: a load into A followed only by operations on A, none of which
  reads `H`. If there's no such code, nothing changes.
- After the run comes `LDA M`, not volatile, which the rewrite
  deletes. Without it, A must be dead after the shifts, since the
  rewrite leaves the shifted value in A, not the value that was
  stored.

### An increment

`STA M; INC M; LDA M` becomes `CLC; ADC #1; STA M`, and the `DEC` form
`SEC; SBC #1; STA M`, which is 3 bytes and 7 cycles less in zero
page. The NMOS 6502 has no `INC A`. The `ADC` and `SBC` set C and V,
which `INC` and `DEC` leave alone, so both must be dead after the
load. Zork's `parse[1]++` reads its byte through a pointer, and since
`INC` has no `(zp),Y` mode, it went through a slot:

```
   LDY   #$01                          LDY   #$01
   LDA   (parse),Y                     LDA   (parse),Y
   STA   __local_instr_sread__0  ->    CLC
   INC   __local_instr_sread__0        ADC   #$01
   LDA   __local_instr_sread__0        STA   (parse),Y
   STA   (parse),Y
```

`carry_known` then moves the `CLC` in front of the `LDY`, where C is
known.

## What it leaves alone

- An `INC` or `DEC` whose C or V a later instruction reads, or one
  without the `LDA M` after it.
- A shift with anything other than shifts of other bytes, or the
  constant stores above, between it and the store.
- A shift of a byte that wasn't just stored from A, such as an
  in-place `w <<= 1`, which is already one instruction per byte.

### A shift loaded from and stored back to the same byte

The converse goes the other way. `LDA M; ASL; STA M` is `ASL M` when
A is dead after the store, for one or two shifts of a non-volatile
zero-page or absolute byte:

```
   LDA   g                             ASL   g
   ASL                           ->
   STA   g
```

That's 2 bytes and 5 cycles against 5 bytes and 8 cycles for one
shift of a zero-page byte. Two shifts take the same cycles either way
and 2 bytes fewer, and three would take more cycles, so the rule
stops at two. The flags come out the same, from the last shift. A
volatile byte is left alone, since a read-modify-write instruction on
the NMOS 6502 writes the old value back before the new one, which
matters for an I/O register. That's why Zork's `instr_random`
(examples/zork/numbers.c) still shifts `RND`'s high byte
in A.

The byte may also be an array element indexed by X, which the
adjacent load and store name with the same X. `a[i] >>= 1` is `TAX;
LSR a,X`. One shift is 3 bytes and 7 cycles against 7 bytes and 11
cycles, and two are 6 bytes and 14 cycles against 8 bytes and 13
cycles. An element indexed by Y stays in A, since the shifts have no
abs,Y mode.

### A loop of one-bit shifts

[Shifts by a loop](shift_loop.md) shifts a byte by a variable count in
memory. When the byte was just stored from A and is loaded again right
after the loop, the loop shifts A and the load becomes the store
(`shift_loop_in_a`). This listing compares two complete compiles of
`uint8_t f(uint8_t x, uint8_t n) { return x >> n; }`, whose `x`
arrives in A and `n` in X.

```
   STA   __local_f__x                  CPX   #$00
   CPX   #$00                          BEQ   .shift_done@1
   BEQ   .shift_done@1           ->  .shift_loop@0:
.shift_loop@0:                         LSR
   LSR   __local_f__x                  DEX
   DEX                                 BNE   .shift_loop@0
   BNE   .shift_loop@0               .shift_done@1:
.shift_done@1:                         RTS
   LDA   __local_f__x
   RTS
```

Before the loop there may only be the count's setup, a load of X
(`LDX n` or `LDX #k`), `CPX #0`, and the `BEQ` past the loop, none of
which writes A or the byte. The loop is the shift, `DEX`, and `BNE`
back, its label named by that `BNE` alone, and the label after it by
the `BEQ` alone. The `STA` at the end leaves the byte as the loop did,
on both ways out. N and Z come from `DEX` there where the `LDA` set
them from the byte, so they must be dead after it. The first `STA`
stays, and [asm dead-store elimination](asm_dead_store.md) deletes it
when nothing reads the byte before the new one. A loop that shifts a
byte left in memory with no load after it, as Ali Baba's
`random_in_range` does three times, is left alone, since a load of
the byte into A before the loop and a store after it would cost more
than the loop saves.

## Related

[Narrowing widened arithmetic](narrow_widened_arith.md) removes a
different source of the same shape. It turns a wide shift whose high
bytes are thrown away into a 1-byte shift, which `tac_to_asm` already
lowers in A. [Load after read-modify-write](redundant_load_after_rmw.md)
deletes an `LDA M` right after a shift of `M` when A is dead.
