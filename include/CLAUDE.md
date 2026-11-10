# include/CLAUDE.md

C99 standard headers, c6502 flavor. Provided so user programs can
`#include <stdint.h>` / `#include <limits.h>` and get the right
fixed-width types and limit macros for c6502's integer model.

The preprocessor (`pcpp` via `preprocessor.preprocess`) picks them up
automatically; no `-I` flag needed.

## Module roster

- `limits.h` (C99 §7.10) — `CHAR_BIT`, `INT_MIN` / `INT_MAX`, etc.,
  for c6502's integer model. The values reflect c6502's actual widths
  — Int is 16 bits, Long is 32, LongLong is 64, char is unsigned 8-
  bit.
- `stdint.h` (C99 §7.18) — `int8_t` / `uint8_t` / `int16_t` /
  `uint16_t` / `int32_t` / `uint32_t` / `int64_t` / `uint64_t`
  typedefs mapping to c6502's integer model, plus the C99 limit and
  literal-suffix macros (`INT8_MAX`, `INT8_C`, etc.).
- `stdbool.h` (C99 §7.16) — `bool` / `true` / `false` /
  `__bool_true_false_are_defined`. `bool` aliases `unsigned char`
  (c6502 doesn't model `_Bool` as a distinct type).
- `apple2/monitor.h` — the Apple II Monitor ROM: `HOME`, `RDKEY`,
  `COUT`, `COUT1` as `zp_abi` routines with `address`, and every
  zero-page byte the unenhanced IIe F8 ROM listing shows the Monitor
  using (`$00`-`$01`, `$1F`, `$20`-`$49`) as `extern` objects with
  `address`, so a program that includes it keeps the runtime's
  zero-page block off them (`passes/runtime_layout.py`). Zero-page
  objects can't overlap, so the bytes are split into named objects
  (`CH`, `CV`, `CSW`, ...) and `__monitor_*` arrays. User guide:
  `docs/attributes.md`.

`size_t`, `ptrdiff_t`, and friends aren't modeled yet — the chapter
corpus doesn't need them, and c6502 has no `stddef.h` to host them.
