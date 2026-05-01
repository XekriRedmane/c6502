# Test status

This file enumerates every chapter\_N test from
nlsandler/writing-a-c-compiler-tests that doesn't pass under c6502
today, with the reason. Every other test passes.

The harnesses (`tests/test_chapter_<N>.py`) carry these as frozen
sets — `_INCOMPATIBLE_VALID` (skipped), `_EXPECTED_FAILURES_CODEGEN`
or its chapter-18 inverse `_VALID_PASSES_TODAY` (must currently
fail / pass), and `_NOT_REJECTED_TODAY` (must currently not be
rejected). Each pinned entry is asserted at its current behavior,
so a regression OR a fix flips the test in either direction and
prompts a drop from the list.

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

Literal magnitudes scale accordingly. Some files were also
restructured to split a large frame across helper functions
(c6502's local frame is capped at ~253 bytes by `LDY` immediate
addressing).

---

## Chapter 13 — malformed FP exponent (`invalid_lex`)

The C standard treats `1.0e10.0` as a single preprocessing-number
that can't be converted to a constant. c6502's lexer has no
preprocessing-number concept and tokenises it as two CONSTANTs
(`1.0e10` and `.0`).

- `malformed_exponent.c`

## Chapter 17 — sizeof literal beyond `unsigned long long` (`valid`, skipped)

c6502's widest integer type is `unsigned long long` (4 bytes,
0..2^32 - 1).

- `sizeof/sizeof_derived_types.c` — uses the literal `4294967297L`
  (= 2^32 + 1) as an array dimension; the parser rejects it before
  any sizeof folding gets a chance.

## Chapter 18 — `valid` (must currently fail at codegen)

### Integer literals beyond `unsigned long long`

Same pre-existing limitation as the chapter 17 sizeof case above —
the parser rejects literals > 2^32 - 1.

- `extra_credit/member_access/static_union_access.c`
- `extra_credit/member_access/union_init_and_member_access.c`
- `extra_credit/member_access/union_temp_lifetime.c`
- `extra_credit/other_features/bitwise_ops_struct_members.c`
- `extra_credit/other_features/compound_assign_struct_members.c`
- `extra_credit/other_features/incr_struct_members.c`
- `extra_credit/semantic_analysis/union_namespace.c`
- `extra_credit/union_copy/copy_thru_pointer.c`
- `no_structure_parameters/scalar_member_access/arrow.c`
- `no_structure_parameters/scalar_member_access/dot.c`

### Frame > 253 bytes

`replace_pseudoregisters` lays out a frame larger than `LDY`
immediate addressing can reach.

- `no_structure_parameters/scalar_member_access/nested_struct.c`

## Chapter 18 — `invalid_lex` (currently not rejected)

c6502's lexer has no preprocessing-number concept, so `.1l` (a
DOT followed by a valid LONG_INTEGER) lexes cleanly even though
the standard would reject the whole sequence as one ill-formed
pp-number. The companion case `.0foo` IS caught by c6502's
`INVALID_NUMBER` regex. The file does fail at parse time because
of the surrounding struct keyword, but that's at the wrong stage
for this bucket.

- `dot_bad_token.c`

## Chapter 18 — `invalid_types` (currently not rejected)

Diagnostic gaps — additional check sites would need to be added
for struct-as-controlling-expression, and for the four
incomplete-type use sites listed.

- `extra_credit/scalar_required/union_as_controlling_expression.c`
- `invalid_incomplete_structs/assign_to_incomplete_var.c`
- `invalid_incomplete_structs/cast_incomplete_struct.c`
- `invalid_incomplete_structs/incomplete_return_type_funcall.c`
- `invalid_incomplete_structs/incomplete_struct_full_expr.c`
- `scalar_required/struct_controlling_expression.c`
