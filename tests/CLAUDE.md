# tests/CLAUDE.md

## Running

```sh
uv run python -m unittest                                          # all
uv run python -m unittest tests.test_cpp_backend                    # one module
uv run python -m unittest tests.test_chapter_1.TestChapter1Valid   # one class
C6502_CHECK_IR=1 uv run python -m unittest                         # with IR checks
```

`C6502_CHECK_IR=1` checks the IR after every optimizer pass and at the
asm stage boundaries (`passes/ir_check.py`). It adds about 3% to the
run. The fuzzers always run with it.

In parallel, with pytest and pytest-xdist (dev dependencies; `uv
sync` installs them):

```sh
C6502_CHECK_IR=1 uv run pytest -n auto --dist worksteal -q
```

That's about 70 seconds against about 4 minutes in one process. Work
stealing matters: with the default `--dist load` the run takes about
2 minutes. The slowest tests are split into shards, one test method
each, so the workers can share them: `test_op_matrix_sim` (8 per
pipeline), `test_bitint_sim` (16 per pipeline for the operators and 4
for the storage grid), `test_sim_differential` and `test_sim_asm` (4
each).

Every harness class is `@unittest.skipUnless(shutil.which("pcpp"),
…)`.

## Layout

`tests/test_<topic>.py` is one harness per topic. The two main
categories:

- **Per-pass / per-module unit tests** (e.g. `test_parser.py`,
  `test_type_checking.py`, `test_redundant_load.py`) — focused tests
  against the module of the same name. Step 8 of the C++ port moves
  the tests that call a pass on IR they build to doctest cases in
  cpp/tests (docs/cpp_port.md, "The tests"), so many of these modules
  hold only their end-to-end tests, and some are gone.
- **Chapter end-to-end tests** (`test_chapter_<N>.py` with the
  matching `chapter_<N>/` directory of C sources) — drive each
  chapter end-to-end through `-S`.

Under `-O` a small `static` function is inlined into its callers and
dropped (`passes/optimization/inline_functions.py`). A test that
checks such a function's own code, its calling convention, or the
call graph decorates its class or method with
`tests.no_inlining.no_inlining()`, which turns inlining off for the
compiles made in the test's process.

Tests that link go through `link_helpers.py`: `compile_object` (`-c
-O` on a source string), `link` (with the runtime helpers' addresses,
so the simulator can run the executable), `linked_text` (the
executable's disassembly and link metadata, to search, with the
executable's bytes in `.exe` for `sim.linked.build_linked_sim`), and
`listing` (the `objdump.py -D` text the multi-unit examples check
in). `test_runtime_library.py` links without the helpers' addresses,
so that each program holds the helpers it calls from the runtime
library, and runs each helper from C against Python with the
simulator's own helpers removed. `test_binutils.py` covers the ELF files, the assembler,
relaxation, `nm.py`, and `objdump.py`; its `TestAgainstDasm` compares
the assembler with dasm and runs only where dasm is installed.

## The multi-unit examples

`test_zork_interpreter.py`, `test_lode_runner.py`, and
`test_ali_baba.py` cover the three programs in `examples/` that are
linked from several units with a linker script. Each compiles every unit with `-O` and compares the
output with its checked-in `.asm` file, which runs everywhere. The
runs of the programs need files from the reverse-engineering projects,
which aren't in this repository, and are skipped without them. On
Robert's machine those projects are cloned in F:/Projects, and the
full suite runs with these set, and with dasm (`F:/dasm-2.20.14.1-win-x64`)
on the PATH:

```sh
ZORK_DSK="F:/Projects/zorki-r15-reveng/Zork I r15 (4am crack).dsk"
ZORK_MAIN_BIN=F:/Projects/zorki-r15-reveng/main.bin
LODE_RUNNER_DIR=F:/Projects/lode_runner_reveng
ALI_BABA_DIR=F:/Projects/ali_baba_reveng
```

- `test_zork_interpreter.py` also compares the linked program's
  `objdump.py -D` listing with its checked-in copy. With `ZORK_DSK`
  set to Zork I's disk image, it plays two sessions on the C
  interpreter and on Zork's own, read from the same image, and
  compares every Monitor and RWTS call.
- `test_lode_runner.py` builds the program in `setUpClass`. With
  `LODE_RUNNER_DIR` set to the lode_runner_reveng directory, `TestRuns`
  plays three scripts of keys, `test_demo` (the attract mode),
  `test_game` (a game that ends in the high-score table), and
  `test_editor` (an edit, a save, a cleared level, and a game played
  from the editor), on the C program and on the original, on
  `sim/lode_runner.py`'s stand-in Apple II. The two streams of
  soft-switch accesses and RWTS calls must be the same up to the
  event at which the original's run ends, and so must the disks at
  the end. On a difference, the failure message shows the events
  around the first one, each with the routine or function it came
  from. The demo, which runs both programs for about four minutes, is
  the slowest test of the suite, so each script is a test method of
  its own, which a worker of `-n auto` takes alone.
- `test_ali_baba.py` checks each unit's output against its `.asm`
  file and round-trips disk images through `sim/disk2.py`. With
  `ALI_BABA_DIR` set to the ali_baba_reveng directory (after its
  `build.py`), `TestOriginal` plays six scripts on the original
  (the attract mode, a game, a save to a blank disk, a load, a visit
  to the forest's chests and shops, and a game in which a second
  character joins and Ali Baba retires) and checks what each
  reaches, and `TestRuns` plays them on the linked C program too and
  requires the same events, on `sim/ali_baba.py`, and the same
  disks. `TestReadmeLink` links the program by the README's command,
  with no `--defsym`, and plays the game script on it. `uv run python -m tests.test_ali_baba coverage`
  lists the original's routines no script reaches.

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

For what works and what's missing, see the README's `## Status`
section and `STATUS.md` (chapter-by-chapter pass/fail). The chapter harnesses are
the authoritative list of what compiles and runs.

Where to look for more:

- `STATUS.md` — chapter_18 file-by-file status.
- `test_sim_differential.py` — opt-vs-unopt sim differential across
  the full chapter corpus; the `_OPT_DIVERGES` dict at the top is the
  live list of optimizer bugs.
- `test_sim_asm_optimized.py` — chapter_1..12 corpus run through
  `-O` with end-to-end return-value assertions.
- `test_op_matrix_sim.py` — every assignment operator (`++` / `--`
  both ways, the compound assignments) over every integer type, seven
  storage classes (local, block `static`, global, zero-page global,
  `*p`, `a[i]`, struct member), and four uses of the result, with
  expected values computed in Python, in both pipelines. It covers a
  grid the chapter corpus and the fuzzers sample sparsely; it's what
  found `return ++c;` on a static `unsigned char` losing A.
- `test_bitint_sim.py` — every operator, and the conversions to and
  from `long long`, `int`, `unsigned char`, `double`, and `float`, on
  every bit-precise type, `_BitInt(2)` to `_BitInt(64)` and
  `unsigned _BitInt(1)` to `unsigned _BitInt(64)`, with each result
  compared byte for byte, padding included, against a Python model;
  and `++`, `--`, and compound assignments on bit-precise objects in
  the seven storage classes of `test_op_matrix_sim.py`. Each program
  runs in both pipelines. `test_bitint.py` has the front end's rules
  for these types.
