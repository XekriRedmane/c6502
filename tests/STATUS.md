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

### Struct / union (chapter 18)

c6502 supports struct and union end-to-end through `--codegen`:

- declarations (file- and block-scope), forward declarations,
  forward references through pointers (auto-introduce per C99
  §6.7.2.3.5)
- member access via `.` / `->` (chained, nested, address-of)
- compound initializers (with nested struct / array members,
  string literal initialisers for char-array members)
- struct copy in every direction (Var=Var, Var=Member,
  Member=Var, Member=Member, Var=Conditional, Var=FunctionCall)
- pointer-to-struct, address-of struct member, sizeof on
  struct/union types
- block-scope tag shadowing — file-scope tags keep their source
  name; block-scope tag declarations get an `@<N>.<source>`
  rename in `passes.identifier_resolution`'s tag scope, so two
  unrelated `struct s` declarations in different inner blocks
  land in the type checker's TypeTable as distinct entries
- struct-by-value parameter passing — a struct argument
  contributes `sizeof(struct)` bytes to the caller's soft-stack
  arg block; callee reads via `Frame(M+3+offset)`. No new
  mechanism, the existing per-byte arg-write loop covers any
  width
- struct-by-value returns via sret — caller allocates a
  return slot, passes its address as a hidden first parameter
  `.sret.<funcname>`; `return e;` lowers to
  `Store(e, .sret) + Ret(None)`; the FunctionCall expression's
  "result" is the slot itself (treated as a temporary-lifetime
  lvalue: `f().m` reads through it but assignment via
  `f().m = …` is rejected by the structural lvalue check)
- unions — member access, copy, address-of, including unions
  containing arrays / structs

Diagnostic gaps (`_INVALID_TYPES_NOT_REJECTED_TODAY` in
`tests/test_chapter_18.py`):

- Struct-as-controlling-expression in `if` / `while`
  (`scalar_required/struct_controlling_expression.c`,
  `extra_credit/scalar_required/union_as_controlling_expression.c`)
- A few `invalid_incomplete_structs/*` cases: assignment to
  an incomplete-typed variable, cast-to-incomplete-struct,
  incomplete struct as a function-call return type, and
  incomplete struct in a full expression. The base
  "dereference of pointer to incomplete" case IS rejected;
  these others would need additional check sites.

Of the 108 valid chapter\_18 programs, 46 compile end-to-end
today — covering all of `no_structure_parameters/`,
`parameters/`, and `params_and_returns/`, plus most of
`extra_credit/{semantic_analysis,member_access,size_and_offset,
union_copy}/`. The remaining failures are unrelated to struct
support per se:

- **FP arithmetic** — many tests (especially
  `scalar_member_access/{dot,arrow}.c`, `nested_struct.c`)
  exercise `double` / `float` operands. The arithmetic helpers
  aren't in the runtime yet.
- **Integer literals beyond `unsigned long long`** — chapter 18
  uses 8-byte hex literals like `0xFFFFFFFFFFFFFFFFUL` in a
  handful of `union_init_and_member_access.c` /
  `compound_assign_struct_members.c` / `bitwise_ops_struct_
  members.c` / `incr_struct_members.c` style tests. Same
  pre-existing limitation as the chapter 17 sizeof case above.
- **253-byte frame limit** — `nested_struct.c` builds a frame
  larger than the LDY-immediate addressing window.
- **A few extra_credit features** that need other work
  (typedef-style alias declarations the grammar doesn't
  accept).

The file-by-file split is in `tests/test_chapter_18.py`'s
`_VALID_PASSES_TODAY` (the explicit must-compile set; everything
else fails at codegen and is checked-via-`assertRaises`),
`_INVALID_TYPES_NOT_REJECTED_TODAY`,
`_INVALID_LEX_NOT_REJECTED_TODAY`,
`_INVALID_PARSE_NOT_REJECTED_TODAY`,
`_INVALID_STRUCT_TAGS_NOT_REJECTED_TODAY` (currently empty —
all `invalid_struct_tags/` files now correctly reject).

---

## Pinned: real bugs

(none currently)
