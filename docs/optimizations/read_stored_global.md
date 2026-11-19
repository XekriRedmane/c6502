# A global read after it's stored

`ReadStoredGlobals` in
[passes/optimization/read_stored_global.py](../../passes/optimization/read_stored_global.py)
makes an instruction read a global just stored from a temporary in
place of the temporary, so that the temporary's only reader is the
store and [copy folding](copy_folding.md) can make the computation
write the global directly. It runs in the TAC fixed point, right
before copy folding. It doesn't run in the unoptimized pipeline.

`*++P` on a global pointer computes `P + 1` into a temporary, copies
it into `P`, and loads through the temporary. Read twice, the
temporary can't be folded away, so the add goes through two
zero-page bytes, which are copied into `P`. Ali Baba's `tune_player`,
which reads a note's duration with `*++TUNE_PC`, went from 374 bytes to
361, and Lode Runner's `move_guard` from 95 to 88.

## Example

```c
#include <stdint.h>
extern const uint8_t *P [[c6502::zeropage(0xEC)]];
uint8_t f(void) { return *++P; }
```

### Before

```
Binary(+, P, 1, %0.1)
Copy(%0.1, P)
Load(%0.1, %1.1)
Ret(%1.1)
```

### After

```
Binary(+, P, 1, %0.1)
Copy(%0.1, P)
Load(P, %1.1)
Ret(%1.1)
```

Copy folding then makes it `Binary(+, P, 1, P)`, and the code goes
from `LDA P; CLC; ADC #1; STA t0; LDA P+1; ADC #0; STA t1; STA P+1;
LDA t0; STA P; LDY #0; LDA (t0),Y`, 23 bytes, to `INC P; BNE; INC
P+1; LDY #0; LDA (P),Y`, 10.

## When it applies

- `Copy(T, G)` comes right after the `Binary` or `Unary` that defines
  `T`, a name SSA construction renamed, and `G` is a variable of the
  same type (or both are pointers) that SSA construction didn't
  rename and that isn't volatile.
- `T` has one read besides the `Copy`, later in the same block with
  no label or jump between, in an instruction that isn't a `Phi`, an
  extend, or a `Truncate`. When that instruction loads or stores
  through a pointer, `T` is the pointer and nothing else.
- Nothing between the `Copy` and the read writes `G` by name, stores
  through a pointer or at a fixed address, stores into `G` as an
  object, or calls a function.

`G` then holds `T`'s value at the read. A read of `G` costs at most a
byte more than one of `T` (an absolute address in place of a zero-page
one), and the store into `T` it saves costs two or more.

## What it leaves alone

- A `T` defined by an extend. `tac_to_asm` lowers a value whose high
  byte comes from a `ZeroExtend` of a byte with that byte as 0, and
  has no such fact about `G`. Zork's `main`
  grew by 2 bytes when `disk_sector`, stored from such a `T`, was read
  in its place.
- A read in an extend or a `Truncate`. The narrowing passes look
  through an extend to its source, and a byte comparison is narrowed
  through a `ZeroExtend` of a global only when the global is unchanged
  up to the comparison. In Ali Baba's `handle_event_slot` a call came
  between them, and `(value & 0x1F) >= 0x1B` became a signed 16-bit
  comparison, 10 bytes more.
- A volatile `G`, since the rewrite adds a read of it.
