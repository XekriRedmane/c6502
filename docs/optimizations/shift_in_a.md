# Shift in A

`apply_shift_in_a` in [passes/shift_in_a.py](../../passes/shift_in_a.py)
moves a run of shifts and rotates of a byte that was just stored from
A onto A itself. It runs in the peephole fixed point, in both
pipelines.

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

from `get_next_zchar` in `examples/zork_print_string_literal.c`,
where `ZCHARS` is a 16-bit zero-page global. The shift by 10 moves
the high byte down and shifts it twice more, and the bytes of the
result above the low one are dead. The "Before" listing is the same
`--optimize` compile without this pass.

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

## What it leaves alone

- `INC` and `DEC`. The NMOS 6502 has no form of them that works on
  A.
- A shift with anything other than shifts of other bytes, or the
  constant stores above, between it and the store.
- A shift of a byte that wasn't just stored from A, such as an
  in-place `w <<= 1`, which is already one instruction per byte.

## Related

[Narrowing widened arithmetic](narrow_widened_arith.md) removes a
different source of the same shape. It turns a wide shift whose high
bytes are thrown away into a 1-byte shift, which `tac_to_asm` already
lowers in A. [Load after read-modify-write](redundant_load_after_rmw.md)
deletes an `LDA M` right after a shift of `M` when A is dead.
