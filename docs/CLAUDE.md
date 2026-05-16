# docs/CLAUDE.md

Design notes, walkthroughs, and reference material for c6502. These
are written for humans (and Claude) reading about the compiler — not
parsed by any tool.

## Long-form docs

- `optimization.md` — from-scratch tour of how `--optimize` turns
  "correct but slow" TAC into "correct and faster" TAC. Written for
  someone who has read the root CLAUDE.md but never touched the
  optimizer code.
- `leaf_zp_abi.md` — design for `__attribute__((zp_abi))` and the
  call-graph-disjoint ZP allocator: leaf functions with bare-RTS
  prologue / epilogue when arg bytes fit in a private ZP slice.
- `sim_findings.md` — bugs and gaps found by running the 6502
  simulator across the chapter corpus. Each one is cross-referenced
  from `tests/test_sim_asm.py`'s `SKIPS` table.

## Reference grammars

`docs/*_grammar.txt` are reference documentation for the spec
grammars that `c99.lark` implements. They aren't parsed by any tool —
they exist so the implementation choices in `c99.lark` (operator
precedence, dangling-else resolution, cast-vs-paren disambiguation)
can be cross-checked against the spec without leaving the repo.

- `chars_grammar.txt` — C99 §6.4.4.4 character-constant grammar.
- `floats_grammar.txt` — C99 §6.4.4.2 floating-constant grammar.
- `ints_grammar.txt` — C99 §6.4.4.1 integer-constant grammar.
- `strings_grammar.txt` — C99 §6.4.5 string-literal grammar.

## Drafts

`drafts/` holds in-progress blog-post material and session logs.
Branched into `drafts/log/` (per-session retrospectives, lessons
learned) and `drafts/r6502/` (article-style writeups for an outside
audience).
