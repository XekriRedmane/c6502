# tests/CLAUDE.md

## Running

```sh
uv run python -m unittest                                          # all
uv run python -m unittest tests.test_asm_emit                      # one module
uv run python -m unittest tests.test_chapter_1.TestChapter1Valid   # one class
```

Every harness class is `@unittest.skipUnless(shutil.which("pcpp"),
…)`.

## Layout

`tests/test_<topic>.py` is one harness per topic. The two main
categories:

- **Per-pass / per-module unit tests** (e.g. `test_parser.py`,
  `test_type_checking.py`, `test_redundant_load.py`,
  `test_replace_pseudoregisters.py`) — focused tests against the
  module of the same name.
- **Chapter end-to-end tests** (`test_chapter_<N>.py` with the
  matching `chapter_<N>/` directory of C sources) — drive each
  chapter end-to-end through `--codegen`.

## Chapter harnesses

`tests/chapter_<N>/` holds sample programs from
nlsandler/writing-a-c-compiler-tests, checked in verbatim. Each
chapter has the same harness shape: per-bucket test methods (`valid`
must compile, `invalid_lex` / `invalid_parse` must reject at the
named stage, `invalid_*` semantic buckets must be rejected somewhere
in the pipeline).

Two filter sets thread through:

- `_INCOMPATIBLE_VALID` — files c6502 can't compile under its narrow
  integer / soft-stack model (e.g. literals beyond 16-bit Long, frames
  beyond 253 bytes).
- `_EXPECTED_FAILURES_CODEGEN` / `_NOT_REJECTED_TODAY` — feature gaps
  that pin current behavior so a regression OR a fix flips the test
  in either direction.

Multi-TU `libraries/` subdirs and platform-specific `.s` files aren't
applicable to c6502 and are skipped at import time.

## Status

For a working-feature checklist (every C99 §6.x construct c6502
accepts end-to-end), see the README's `## Status` section and
`STATUS.md` (chapter-by-chapter pass/fail). The chapter harnesses are
the authoritative list of what compiles and runs.

Where to look for more:

- `STATUS.md` — chapter_18 file-by-file status.
- `test_sim_differential.py` — opt-vs-unopt sim differential across
  the full chapter corpus; the `_OPT_DIVERGES` dict at the top is the
  live list of optimizer bugs.
- `test_sim_asm_optimized.py` — chapter_1..12 corpus run through
  `--optimize` with end-to-end return-value assertions.
