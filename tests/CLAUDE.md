# tests/CLAUDE.md

## Running

```sh
uv run python -m unittest                                          # all
uv run python -m unittest tests.test_ir_dump                        # one module
uv run python -m unittest tests.test_binutils.TestElf               # one class
C6502_CHECK_IR=1 uv run python -m unittest                         # with IR checks
```

The suite runs the compiler in `c6502cpp`, so it needs the C++ build
(cpp/README.md), and a test that compiles a program is skipped without
it. `C6502_CHECK_IR=1` makes c6502cpp check the IR after every
optimizer pass and at the asm stage boundaries
(cpp/src/passes/ir_check.h), and a test that needs the checks whatever
the variable says calls `cpp_backend.enable_ir_check()`. The fuzzers
always run with them. `C6502_SIM_LOCKSTEP=1` runs the C++ CPU beside
py65 in every `Simulation.run` and fails a test at the first
instruction where they differ (sim/lockstep.py).

In parallel, with pytest and pytest-xdist (dev dependencies; `uv
sync` installs them):

```sh
C6502_CHECK_IR=1 uv run pytest -n auto --dist worksteal -q
```

A harness class that compiles a program is decorated with
`tests.cpp_build.needs_c6502cpp`, which skips it when `c6502cpp` isn't
built, since compile.py, nm.py, and objdump.py are shims over it. A
test that skips itself, or combines the condition with another, reads
`cpp_build.BUILT` and `cpp_build.REASON`.

## What is left here

Step 9 of the port moves the suite to C++ (docs/cpp_port.md). Its
group G1 moved the tests that run no simulator, the TAC interpreter's
among them, to doctest cases in cpp/tests (cpp/README.md, "The tests of
whole compiles"), and group G4 moved the tests that run the simulator
(cpp/README.md, "The simulator's harness"). A new test is a doctest
case: of a compile's text, its errors, or its command line in
cpp/tests/test_<topic>.cpp, and of a program's run in
cpp/tests/test_<topic>_sim.cpp, through `suite::build_sim`
(cpp/tests/sim_case.h).

The modules left test Python code that a later group ports or removes.

- `test_zork_interpreter.py`, `test_lode_runner.py`, and
  `test_ali_baba.py` run the games on sim/'s machines, which group G5
  ports (below).
- `test_fuzz_tools.py` runs the Python fuzzers on a few seeds, for
  group G6, and `test_sim_assembler.py` tests sim/assembler.py, which
  the fuzzers' runner still uses.
- `test_asdl.py`, `test_ir_dump.py`, `test_type_checking.py`, and
  `test_binutils.py` test asdl.py, ir_dump.py, the IR classes'
  equality, and binutils/elf.py, which go in group R.
- `test_sim_chapters.py` holds `_run_program`, the TAC interpreter's
  run of a program, which fuzz/runner.py imports.
- `cpp_build.py`, `link_helpers.py` (`linked_text`, which
  test_zork_interpreter.py reads), `no_inlining.py`, and
  `asm_compare.py`, which nothing imports, go in group R.

## The multi-unit examples

`test_zork_interpreter.py`, `test_lode_runner.py`, and
`test_ali_baba.py` cover the three programs in `examples/` that are
linked from several units with a linker script. golden/O.txt holds
each unit's `-S -O` text and golden/links.txt each linked program's
digest, so these modules hold only the runs of the programs, which
need files from the reverse-engineering projects, aren't in this
repository, and are skipped without them. On Robert's machine those
projects are cloned in F:/Projects, and the full suite runs with these
set (and c6502_tests runs its comparisons with dasm when
`F:/dasm-2.20.14.1-win-x64` is on the PATH):

```sh
ZORK_DSK="F:/Projects/zorki-r15-reveng/Zork I r15 (4am crack).dsk"
ZORK_MAIN_BIN=F:/Projects/zorki-r15-reveng/main.bin
LODE_RUNNER_DIR=F:/Projects/lode_runner_reveng
ALI_BABA_DIR=F:/Projects/ali_baba_reveng
```

- cpp/tests/test_zork_interpreter.cpp compares the linked program's
  `objdump.py -D` listing with its checked-in copy. With `ZORK_DSK` set
  to Zork I's disk image, test_zork_interpreter.py plays two sessions
  on the C interpreter and on Zork's own, read from the same image, and
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
- `test_ali_baba.py` round-trips disk images through `sim/disk2.py`.
  With `ALI_BABA_DIR` set to the ali_baba_reveng directory (after its
  `build.py`), `TestOriginal` plays six scripts on the original (the
  attract mode, a game, a save to a blank disk, a load, a visit to the
  forest's chests and shops, and a game in which a second character
  joins and Ali Baba retires) and checks what each reaches, and
  `TestRuns` plays them on the linked C program too and requires the
  same events, on `sim/ali_baba.py`, and the same disks.
  `TestReadmeLink` links the program by the README's command, with no
  `--defsym`, and plays the game script on it. `uv run python -m
  tests.test_ali_baba coverage` lists the original's routines no script
  reaches.

## Chapter corpora

`tests/chapter_<N>/` holds sample programs from
nlsandler/writing-a-c-compiler-tests, checked in verbatim or adapted
to c6502's widths. A program in `valid/` must compile, and one in an
`invalid_*` bucket must be rejected with a one-line error. The golden
files hold each program in each mode (golden/unopt.txt and O.txt the
exit status and message of `-S`, lex.txt, parse.txt, resolve.txt, and
tac.txt the text stages, and tacsim.txt what `main` returns in the TAC
interpreter), so `c6502cpp check-golden` reports any change. The
Python harnesses `test_chapter_<N>.py` and test_invalid_programs.py,
which checked the same and the class of each error, were dropped on 26
Nov 2026 (docs/cpp_port.md, "The tests golden/ covers"). The
`invalid_lex` and `invalid_parse` buckets' parses, with each error's
class, are doctest cases (cpp/tests/test_chapter_<N>_calls.cpp).
STATUS.md lists the programs c6502 doesn't handle as their bucket says.

`expected_returns()` in cpp/tests/sim_chapters.cpp gives what `main`
returns for 515 of the programs, traced by hand, which the simulator's
runs of the corpus check in both pipelines
(cpp/tests/test_sim_corpus.cpp). It was test_sim_chapters.py's
`EXPECTED_RETURNS` until step 9's G4.

## Status

For what works and what's missing, see the README's `## Status`
section and `STATUS.md`.

Where to look for more:

- `STATUS.md` — chapter_18 file-by-file status.
- cpp/tests/test_sim_corpus.cpp has the simulator's runs of the
  chapter corpus, and `opt_diverges()` in cpp/tests/sim_chapters.cpp is
  the live list of optimizer bugs.
- cpp/tests/test_op_matrix_sim.cpp runs every assignment operator (`++`
  / `--` both ways, the compound assignments) over every integer type,
  seven storage classes (local, block `static`, global, zero-page
  global, `*p`, `a[i]`, struct member), and four uses of the result, in
  both pipelines. It covers a grid the chapter corpus and the fuzzers
  sample sparsely; it's what found `return ++c;` on a static `unsigned
  char` losing A.
- cpp/tests/test_bitint_sim.cpp runs every operator, and the
  conversions to and from `long long`, `int`, `unsigned char`,
  `double`, and `float`, on every bit-precise type, `_BitInt(2)` to
  `_BitInt(64)`
  and `unsigned _BitInt(1)` to `unsigned _BitInt(64)`, with each result
  compared byte for byte, padding included, against the Python model's
  bytes frozen in cpp/tests/frozen/sim/bitint_sim.txt; and `++`, `--`,
  and compound assignments on bit-precise objects in the seven storage
  classes of the operator grid. Each program runs in both pipelines.
