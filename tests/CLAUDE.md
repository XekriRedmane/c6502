# tests/CLAUDE.md

## Running

```sh
cpp/build/bin/c6502_tests                         # every doctest case
cpp/build/bin/c6502_tests -tc="apply_bobble_sim*" # the cases whose names match
ctest --test-dir cpp/build -C Release --output-on-failure   # the cases and the checks below
cpp/build/bin/c6502cpp check-golden -j 12         # the corpus against golden/
cpp/build/bin/c6502cpp check-frozen               # the passes against cpp/tests/frozen
```

The tests are doctest cases in cpp/tests, which need the whole C++
build (cpp/README.md, "Building"). `C6502_CHECK_IR=1` makes
`c6502cpp compile` check the IR after every optimizer pass and at the
asm stage boundaries (cpp/src/passes/ir_check.h). The fuzzers
(`c6502cpp fuzz`) always run with them.

## What is here

tests/ holds no test code. It holds the chapter corpora, which
check-golden compiles and the doctest cases read, STATUS.md, and
ir_dump/, the four IRs of examples/paint_hud_strip_p1.c as the Python
compiler dumped them, which cpp/tests/test_ir_dump_files.cpp reads and
writes again. Step 9 of the port moved the Python suite to C++
(docs/cpp_port.md). Its group G1 moved the tests that run no
simulator, the TAC interpreter's among them, to doctest cases in
cpp/tests (cpp/README.md, "The tests of whole compiles"), group G4
moved the tests that run the simulator (cpp/README.md, "The
simulator's harness"), and group R removed the Python modules that
were left. A new test is a doctest case: of a
compile's text, its errors, or its command line in
cpp/tests/test_<topic>.cpp, and of a program's run in
cpp/tests/test_<topic>_sim.cpp, through `suite::build_sim`
(cpp/tests/sim_case.h).

## The multi-unit examples

The three programs in `examples/` that are linked from several units
with a linker script are covered by golden/O.txt, which holds each
unit's `-S -O` text, golden/links.txt, which holds each linked
program's digest, and the doctest cases of step 9's G5, which play the
games on the C++ machines (cpp/README.md, "The games' machines"). The
plays need files from the reverse-engineering projects, which aren't in
this repository, and are skipped without them. On Robert's machine
those projects are cloned in F:/Projects, and c6502_tests runs the
plays with these set (and its comparisons with dasm when
`F:/dasm-2.20.14.1-win-x64` is on the PATH):

```sh
ZORK_DSK="F:/Projects/zorki-r15-reveng/Zork I r15 (4am crack).dsk"
ZORK_MAIN_BIN=F:/Projects/zorki-r15-reveng/main.bin
LODE_RUNNER_DIR=F:/Projects/lode_runner_reveng
ALI_BABA_DIR=F:/Projects/ali_baba_reveng
```

- cpp/tests/test_zork_interpreter.cpp compares the linked program's
  `c6502cpp objdump -D` listing with its checked-in copy,
  examples/zork/interpreter.asm, and
  test_zork_interpreter_sim.cpp plays two sessions on the C interpreter
  and on Zork's own, read from the disk image, and compares every
  Monitor and RWTS call.
- cpp/tests/test_lode_runner_sim.cpp plays three scripts of keys, the
  attract mode, a game that ends in the high-score table, and an edit,
  a save, a cleared level, and a game played from the editor, on the C
  program and on the original. The two streams of soft-switch accesses
  and RWTS calls must be the same up to the event at which the
  original's run ends, and so must the disks at the end. On a
  difference, the failure message shows the events around the first
  one, each with the routine or function it came from.
- cpp/tests/test_ali_baba_sim.cpp plays six scripts on the original
  (the attract mode, a game, a save to a blank disk, a load, a visit to
  the forest's chests and shops, and a game in which a second character
  joins and Ali Baba retires) and checks what each reaches, then plays
  them on the linked C program too and requires the same events and
  disks, and plays the game on the program linked by the README's
  command, with no `--defsym`. test_disk2.cpp round-trips disk images
  through the Disk II. `c6502cpp game-run ali_baba coverage` lists the
  original's routines no script reaches.

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
