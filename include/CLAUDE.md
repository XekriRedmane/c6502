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

`size_t`, `ptrdiff_t`, and friends aren't modeled yet — the chapter
corpus doesn't need them, and c6502 has no `stddef.h` to host them.
