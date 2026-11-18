# include/CLAUDE.md

C standard headers, c6502 flavor: C99's, except for C23's
`<stdbool.h>` and `<stddef.h>`. Provided so user programs can
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
  literal-suffix macros (`INT8_MAX`, `INT8_C`, etc.), and the limits
  `PTRDIFF_MIN` / `PTRDIFF_MAX` (an `int`'s), `SIZE_MAX` (an
  `unsigned long`'s), and `WCHAR_MIN` / `WCHAR_MAX` (an `int`'s) of
  the types `<stddef.h>` declares.
- `stddef.h` (C23 §7.21) — `ptrdiff_t` is `int` and `size_t` is
  `unsigned long`, the types the subtraction of two pointers and
  `sizeof` give. `max_align_t` is `char`, since nothing in c6502 has
  an alignment above 1, and `wchar_t` is `int`. `nullptr_t` is a
  typedef of `__c6502_nullptr_t`, c6502's keyword for the type of
  `nullptr`, because C23's `typeof_unqual(nullptr)` needs a `typeof`
  that c6502 doesn't have. `NULL` is `((void *)0)`, which compares
  equal to `0` and to `nullptr` and can't be assigned to an integer.
  `offsetof(type, member)` is `((size_t)&((type *)0)->member)`, which
  the type checker folds to a constant (`_null_based_offset`). It
  works in static initializers and case labels, but not in array
  sizes or enumeration values, which the parser folds before any
  struct layout exists. `unreachable()` is `((void)0)`, and
  `__STDC_VERSION_STDDEF_H__` is `202311L`.
- `stdbool.h` (C23 §7.19) — only `__bool_true_false_are_defined`,
  as 1. `bool`, `true`, and `false` are keywords in C23 (§6.4.1), so
  the header no longer defines them as macros, and including it
  doesn't warn the way a `_Bool` does.
- `apple2/monitor_io.h` — the Apple II Monitor ROM routines a program
  calls: `HOME`, `CLREOL`, `RDKEY`, `COUT`, `COUT1`, `WAIT`, `VTAB`,
  and `GETLN1` as `zp_abi` routines with `address`, each but the last
  two with the `preserves_x` / `preserves_y` the ROM and 80-column
  firmware listings support (GETLN1 returns its length in X, which C
  can't read, so a caller looks for the `$8D` it stores after the
  line), and the zero-page bytes
  those listings show its text, keyboard, line-input, lo-res
  graphics, and delay routines, the IRQ entry, INIT, and the
  firmware's KEYIN using (`$1F`-`$2E`, `$30`, `$32`-`$33`,
  `$35`-`$39`, `$45`, `$48`, `$4E`-`$4F`) as `extern` objects with
  `address`, so a program that includes it keeps the runtime's
  zero-page block off them (`passes/runtime_layout.py`). Zero-page
  objects can't overlap, so the bytes are split into named objects
  (`CH`, `CV`, `CSW`, `RND`, ...) and `__monitor_*` objects. User guide:
  `docs/attributes.md`.
- `apple2/interactive_monitor.h` — includes `monitor_io.h` and adds
  the bytes only the Monitor's interactive part uses (command loop
  and memory tools, disassembler, register display and BRK,
  cassette, power-up slot scan): `$00`-`$01`, `$2F`, `$31`, `$34`,
  `$3A`-`$44`, `$46`-`$47`, `$49`.
