# include/CLAUDE.md

C standard headers, c6502 flavor: C99's, except for C23's
`<stdbool.h>`, `<stddef.h>`, and `<stdbit.h>`. Provided so user programs can
`#include <stdint.h>` / `#include <limits.h>` and get the right
fixed-width types and limit macros for c6502's integer model.

The preprocessor (`pcpp` via `preprocessor.preprocess`) picks them up
automatically; no `-I` flag needed.

## Module roster

- `limits.h` (C99 §7.10) — `CHAR_BIT`, `INT_MIN` / `INT_MAX`, etc.,
  for c6502's integer model. The values reflect c6502's actual widths
  — Int is 16 bits, Long is 32, LongLong is 64, char is unsigned 8-
  bit. `BITINT_MAXWIDTH` (C23 §5.2.5.3.2) is 64, the widest N of
  `_BitInt(N)` and `unsigned _BitInt(N)`, which is the least C23
  allows (the width of `unsigned long long`), and it has to match
  `bit_precise.BITINT_MAXWIDTH`, which the parser checks N against.
- `stdint.h` (C99 §7.18) — `int8_t` / `uint8_t` / `int16_t` /
  `uint16_t` / `int32_t` / `uint32_t` / `int64_t` / `uint64_t`
  typedefs mapping to c6502's integer model, plus the C99 limit and
  literal-suffix macros (`INT8_MAX`, `INT8_C`, etc.), and the limits
  `PTRDIFF_MIN` / `PTRDIFF_MAX` (an `int`'s), `SIZE_MAX` (an
  `unsigned int`'s), and `WCHAR_MIN` / `WCHAR_MAX` (an `int`'s) of
  the types `<stddef.h>` declares.
- `stddef.h` (C23 §7.21) — `ptrdiff_t` is `int` and `size_t` is
  `unsigned int`, the types the subtraction of two pointers and
  `sizeof` give. `max_align_t` is `char`, since nothing in c6502 has
  an alignment above 1, and `wchar_t` is `int`. `nullptr_t` is
  `typeof_unqual(nullptr)`, as C23 declares it. `NULL` is `((void
  *)0)`, which compares equal to `0` and to `nullptr` and can't be
  assigned to an integer.
  `offsetof(type, member)` is `((size_t)&((type *)0)->member)`, which
  the type checker folds to a constant (`_null_based_offset`). It
  works in static initializers and case labels, but not in array
  sizes or enumeration values, which the parser folds before any
  struct layout exists. `unreachable()` is `((void)0)`, and
  `__STDC_VERSION_STDDEF_H__` is `202311L`.
- `stdbit.h` (C23 §7.18) — the bit and byte utilities. Each of the
  14 operations (`stdc_leading_zeros` through `stdc_bit_ceil`) is
  declared as a function in its forms of one type, `_uc`, `_us`,
  `_ui`, `_ul`, and `_ull`, each of which is also a macro for the
  builtin `__c6502_stdc_<op>_<suffix>`, and its type-generic form
  `stdc_<op>` is a macro for `__c6502_stdc_<op>`
  (cpp/src/builtin_functions.h). The compiler folds a constant argument and
  writes a loop in place for any other, and a unit that takes a
  function's address, or calls it with the macro suppressed, gets a
  definition of it with internal linkage
  (cpp/src/passes/stdbit_functions.h), which also drops the declarations a
  unit doesn't refer to. The parameter names are `__value`, which a
  program's macros can't touch. `__STDC_VERSION_STDBIT_H__` is
  `202311L`, `__STDC_ENDIAN_LITTLE__` 1234, `__STDC_ENDIAN_BIG__`
  4321, and `__STDC_ENDIAN_NATIVE__` the little one. As §7.18.1p1
  specifies, it declares `size_t` and the exact- and minimum-width
  integer types too, the same typedefs as `<stddef.h>` and
  `<stdint.h>` (C23 allows a typedef to be repeated), and nothing
  else of theirs. User guide: `docs/stdbit.md`.
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
  zero-page block off them (cpp/src/passes/runtime_layout.h). Zero-page
  objects can't overlap, so the bytes are split into named objects
  (`CH`, `CV`, `CSW`, `RND`, ...) and `__monitor_*` objects. User guide:
  `docs/attributes.md`.
- `apple2/interactive_monitor.h` — includes `monitor_io.h` and adds
  the bytes only the Monitor's interactive part uses (command loop
  and memory tools, disassembler, register display and BRK,
  cassette, power-up slot scan): `$00`-`$01`, `$2F`, `$31`, `$34`,
  `$3A`-`$44`, `$46`-`$47`, `$49`.
