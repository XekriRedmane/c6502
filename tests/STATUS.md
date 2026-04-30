# Test status

This file enumerates every chapter\_N test from
nlsandler/writing-a-c-compiler-tests that doesn't pass under c6502
today, with the reason. Every other test passes.

The harnesses (`tests/test_chapter_<N>.py`) carry these as frozen
sets — `_INCOMPATIBLE_VALID` (skipped), `_EXPECTED_FAILURES_CODEGEN`
(must currently fail), and `_NOT_REJECTED_TODAY` (must currently
not be rejected). Each pinned entry is asserted at its current
behavior, so a regression OR a fix flips the test in either
direction and prompts a drop from the list.

Categories below:

- **Pinned: feature gaps** — features c6502 plans to add (preprocessing
  number lexing, etc.). Pinned at current accept/reject behavior.
- **Pinned: real bugs** — the compiler crashes or wrongly accepts
  programs the spec rejects. Pinned at current behavior.

Multi-TU `libraries/` subdirs (and platform-specific `.s` files)
aren't listed: they're skipped at import time, not at harness time.

## Locally adapted tests

Many upstream files were locally rewritten to fit c6502's narrow
integer / 256-byte-frame / 1-byte-int model. The adaptations
substitute c6502's wider types for upstream's:

- upstream `int`     (4 B) → c6502 `long`     (2 B)
- upstream `long`    (8 B) → c6502 `long long` (4 B)
- upstream `unsigned int`  → c6502 `unsigned long`
- upstream `unsigned long` → c6502 `unsigned long long`

Literal magnitudes scale accordingly. The chapter-by-chapter
test semantics (multi-byte arithmetic, common-type promotion,
sign-/zero-extension, truncation, switch-on-wide-int, etc.) all
survive — just at narrower widths than upstream. Some files were
also restructured to split a large frame across helper functions
(c6502's local frame is capped at ~253 bytes by `LDY` immediate
addressing).

---

## Pinned: feature gaps

### Lexer accepts malformed FP exponent

The C standard treats `1.0e10.0` as a single preprocessing-number
that can't be converted to a constant. c6502's lexer has no
preprocessing-number concept and tokenises it as two CONSTANTs
(`1.0e10` and `.0`).

- **chapter\_13** invalid_lex:
  - `malformed_exponent.c`

### Integer literals beyond `unsigned long long`

c6502's widest integer type is `unsigned long long` (4 bytes,
0..2^32 - 1). One chapter 17 sizeof test specifies an array
type whose size literal exceeds that range, so the parser
rejects it before any sizeof folding gets a chance.

- **chapter\_17** valid (skipped via `_INCOMPATIBLE_VALID`):
  - `sizeof/sizeof_derived_types.c` — uses the literal
    `4294967297L` (= 2^32 + 1) as an array dimension.

---

## Pinned: real bugs

(none currently)
