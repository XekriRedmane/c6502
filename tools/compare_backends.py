"""Compiles, assembles, and links the corpus with the Python compiler and
with the C++ port (`C6502_CPP_BACKEND=1`, which runs the whole compile
of a C source from the preprocessor on, the assembly of `-c`, and the
link) and checks that the two give the same output.

    uv run python tools/compare_backends.py -j 12
    uv run python tools/compare_backends.py --corpus examples,games --modes O
    uv run python tools/compare_backends.py --corpus links --modes c

The corpus is the capture harness's (`tools/capture_passes.corpus`):
the examples, the games' units, every C file under `tests/chapter_*`,
and seeds `--fuzz-seeds` of `fuzz.stmts`, `fuzz.calls`, and
`fuzz.exprs`. Each program is compiled with `-S` and with `-S -O`
(the modes `unopt` and `O`), and with the modes `c` and `cO` it is
compiled with `-c` and with `-c -O` and the object linked alone, without
a script and with tools/capture_binutils.py's script of ROM and RAM
regions and the `--defsym`s that harness gives, as that harness's
corpus does. The modes `lex`, `parse`, `resolve`, `resolveO`, `tac`,
and `tacO`, which aren't among the defaults, run `--lex`, `--parse`,
`--resolve`, `--resolve -O`, `--tac`, and `--tac -O` on each program.
The corpus `links` holds the jobs of that harness's
`links` job: the three games, each unit compiled with `-c -O` and the
objects linked with the game's script and without one (and Ali Baba
with the simulator's helpers too), and besides those every unit
compiled by one `-c -O` and the program linked from its C sources by
one command, and then the damaged files of `capture_binutils._damaged`
linked and the command lines of `_BAD_LINKS`. The corpus `cli` holds a
job for each terminal width of `_CLI_COLUMNS`, which runs the command
lines of `_cli_command_lines` with `COLUMNS` set to that width: the
help, the usage errors of argparse (each pair of stage flags, a bad
`--origin`, an option without its value, and flags run together), the
errors of compile.py's own checks of the stage and the inputs, and
links with each spelling of `-Ttext` and `--origin`.

Each job runs its commands once with the variable unset and once with
it set to 1, in the same worker process and the same directory, which
is emptied between the two, since compile.py reads the variable at each
compile. The two runs must agree on everything they give: for each
command the exit status, what compile.py writes to stdout and stderr
(the errors, the warnings, and what the preprocessor reports there),
and, for a command that raises, the exception's type and message; and
the files the job leaves in the directory, byte for byte, which are the
`-S` text, the objects, and the executables. The exit status is 0 when
every pair agrees. c6502cpp comes from `C6502_CPP` or cpp/build/bin, as
for tools/check_captures.py.
"""
from __future__ import annotations

import argparse
import contextlib
import importlib
import io
import os
import shutil
import sys
import tempfile
import time
from concurrent.futures import ProcessPoolExecutor
from pathlib import Path

_ROOT = Path(__file__).resolve().parent.parent
if str(_ROOT) not in sys.path:
    sys.path.insert(0, str(_ROOT))
if str(_ROOT / "tools") not in sys.path:
    sys.path.insert(0, str(_ROOT / "tools"))

from tools.capture_passes import corpus  # noqa: E402
from tools.check_captures import cpp_cli  # noqa: E402

_VARIABLE = "C6502_CPP_BACKEND"

# Link command lines that compile.py rejects, with `{f}` for a good
# object, each run in the `links` job's errors.
_BAD_LINKS = (
    ["missing.o"], ["{f}", "missing.o"], ["missing.c"], ["{f}", "missing.asm"],
    ["{f}", "-T", "missing.ld"], ["{f}", "-T", "bad.ld"],
    ["{f}", "-T", "small.ld"], ["{f}", "--defsym", "x"],
    ["{f}", "--defsym", "x=y"], ["{f}", "-Ttext=0x10000"],
    ["{f}", "-o", "nodir/prog"], ["{f}", "-o", "."],
    ["{f}", "notutf8.c"], ["{f}", "notutf8.asm"], ["{f}", "bad.asm"],
    ["undefined.asm"], ["{f}", "{f}"],
    ["-c", "{f}"], ["-c", "a.c", "{f}"], ["-c", "a.c", "b.c", "-o", "x.o"],
    ["-c", "-"],
)
_BAD_FILES = {
    "bad.ld": "SECTIONS { .text : { *(.text) } > nowhere }\n",
    "small.ld": "MEMORY { ram : ORIGIN = 0x0800, LENGTH = 4 }\n"
                "SECTIONS { .text : { *(.text) } > ram }\n",
    "bad.asm": "f:\n BNE 5\n",
    "undefined.asm": "main:\n JSR nowhere\n RTS\n",
    "a.c": "int main(void) { return 0; }\n",
    "b.c": "int f(void) { return 1; }\n",
}


# The terminal widths of the `cli` jobs, which wrap argparse's usage and
# help in each of its ways: a width under 4, which `max_help_position`
# keeps at 4, the program's name on a line of its own (under 23), the
# usage over several lines, and the usage on one line (200).
_CLI_COLUMNS = (1, 20, 41, 80, 132, 200)

_STAGE_FLAGS = ("--lex", "--parse", "--resolve", "--tac", "-S", "-c")

# The files the `cli` jobs' command lines name.
_CLI_FILES = {
    "a.c": "int main(void) { return 1; }\n",
    "b.asm": "f:\n RTS\n",
}


def _cli_command_lines() -> list[list[str]]:
    """The command lines of the `cli` jobs, after the program's name."""
    lines = [
        # The help, before and after an error.
        ["-h"], ["--help"], ["--he"], ["a.c", "-S", "-h"],
        ["-h", "--lex", "-S"], ["--lex", "-S", "-h"],
        # No inputs.
        [], ["-O"], ["--", "-S"],
    ]
    # Each pair of stage flags, which the mutually exclusive group
    # rejects, apart and run together.
    for i, first in enumerate(_STAGE_FLAGS):
        for second in _STAGE_FLAGS[i + 1:]:
            lines.append(["a.c", first, second])
    lines += [
        ["a.c", "-Sc"], ["a.c", "-cS"], ["a.c", "--tac", "--tac", "-o", "t.txt"],
        # An explicit argument that a flag ignores.
        ["a.c", "-O2"], ["a.c", "--lex=yes"], ["a.c", "-S="], ["a.c", "-Sx"],
        # A bad `--origin`, which `type=_number` rejects.
        ["a.c", "--origin", "xyz"], ["a.c", "--origin="], ["a.c", "--origin", "$"],
        ["a.c", "--origin", "0b2"], ["a.c", "--origin", "08"],
        ["a.c", "--origin", "1__0"], ["a.c", "--origin", "0x_"],
        ["a.c", "--ori", "_1"], ["a.c", "-Ttext=0x"], ["a.c", "-Ttext", "12_"],
        ["-Ttext"], ["a.c", "--origin", "'q'"], ["a.c", "--origin", "\u00e9"],
        ["a.c", "--origin", "$_10"],
        # An option without its value.
        ["a.c", "-o"], ["a.c", "-T"], ["a.c", "--origin"], ["a.c", "--defsym"],
        ["a.c", "-o", "-S"], ["a.c", "--defsym", "--lex"], ["a.c", "-T", "-O"],
        # compile.py's own checks of the stage and the inputs.
        ["-S", "b.asm"], ["--lex", "x.o"], ["-S", "a.c", "a.c", "-o", "x.s"],
        ["-S", "a.c", "-o", "x.txt"], ["-c", "a.c", "a.c", "-o", "x.o"],
        ["-c", "-"], ["-c", "x.o"], ["a.c", "--defsym", "x"],
        ["a.c", "--defsym", "x=$"],
        # The stages, with options abbreviated and run together, and
        # arguments that go to the preprocessor.
        ["a.c", "--le", "-o", "l.txt"], ["a.c", "--pa", "-o", "-"],
        ["a.c", "-S", "-ox1.s"], ["a.c", "-S", "-o=x2.s"], ["a.c", "-SOo", "x3.s"],
        ["-S", "--", "a.c", "-o", "-"], ["-D", "X", "a.c", "-S", "-o", "-"],
        ["a.c", "-S", "-o", "-", "-DX=1"],
        # Links with each spelling of the origin.
        ["a.c", "-Ttext=0x1000", "-o", "p1"], ["a.c", "-Ttext", "$2000", "-o", "p2"],
        ["a.c", "--origin=4096", "-o", "p3"], ["a.c", "--orig", "0X9_00", "-o", "p4"],
        ["a.c", "-Ttext= +0x900 ", "-o", "p5"], ["a.c", "-Ttext=$ 0x900", "-o", "p6"],
        ["a.c", "-Ttext=0o4000", "-o", "p7"],
        ["a.c", "-Ttext=0b100000000000", "-o", "p8"], ["a.c", "-Ttext=00", "-o", "p9"],
        ["a.c", "-Ttext=-1", "-o", "p10"], ["a.c", "-Ttext=0x10000"],
        ["a.c", "--origin", "0x900", "-Ttext=0xA00", "-o", "p11"],
        ["a.c", "-Ttextual"], ["b.asm", "a.c", "--defsym=x=1", "--def", "y=$10", "-o", "p12"],
    ]
    return lines


def _cli_commands(run, columns: int) -> None:
    """Runs the command lines of `_cli_command_lines` through `run` with
    the terminal `columns` wide."""
    for name, text in _CLI_FILES.items():
        Path(name).write_text(text, encoding="utf-8", newline="\n")
    old = os.environ.get("COLUMNS")
    os.environ["COLUMNS"] = str(columns)
    try:
        for cmd in _cli_command_lines():
            run(["compile.py", *cmd])
    finally:
        if old is None:
            os.environ.pop("COLUMNS", None)
        else:
            os.environ["COLUMNS"] = old


def _compile(argv: list[str], cpp: bool) -> tuple:
    """Runs `compile.main(argv)` with the C++ back end on or off and
    returns what it gave: the exit status, the text written to stdout
    and stderr, and the type and message of an exception it raised
    (None when it raised none)."""
    import compile as compile_module
    if cpp:
        os.environ[_VARIABLE] = "1"
    else:
        os.environ.pop(_VARIABLE, None)
    sink = io.StringIO()
    status = None
    raised = None
    try:
        with contextlib.redirect_stderr(sink), contextlib.redirect_stdout(sink):
            status = compile_module.main(argv)
    except SystemExit as e:
        status = e.code
    except Exception as e:  # noqa: BLE001 - a crash is compared too
        raised = (type(e).__name__, str(e))
    finally:
        os.environ.pop(_VARIABLE, None)
    return status, sink.getvalue(), raised


def _files(work: Path) -> dict[str, bytes]:
    """The bytes of every file under `work`, by path."""
    return {
        p.relative_to(work).as_posix(): p.read_bytes()
        for p in sorted(work.rglob("*")) if p.is_file()
    }


def _source_args(args: list[str], tmp: str) -> list[str]:
    """The compile.py arguments of a corpus program, with a fuzz
    program written to a file in `tmp`."""
    if args[0].startswith("fuzz:"):
        _, tool, seed = args[0].split(":")
        src = importlib.import_module(f"fuzz.{tool}").make_program(int(seed))[0]
        path = os.path.join(tmp, f"{tool}{seed}.c")
        with open(path, "w", encoding="utf-8", newline="\n") as f:
            f.write(src)
        return [path] + args[1:]
    return args


# The modes that stop at a stage before code generation, with the flags
# of each.
_TEXT_MODES = {
    "lex": ("--lex", []),
    "parse": ("--parse", []),
    "resolve": ("--resolve", []),
    "resolveO": ("--resolve", ["-O"]),
    "tac": ("--tac", []),
    "tacO": ("--tac", ["-O"]),
}


def _program_commands(run, args: list[str], mode: str) -> None:
    """Runs the commands of a corpus program in `mode` through `run`,
    which takes a command line and returns whether it succeeded."""
    if mode in ("O", "unopt"):
        run(["compile.py", *args, "-S", "-o", "out.s",
             *(["-O"] if mode == "O" else [])])
        return
    if mode in _TEXT_MODES:
        stage, flag = _TEXT_MODES[mode]
        run(["compile.py", *args, stage, *flag, "-o", "out.txt"])
        return
    import capture_binutils
    from sim.linked import defsym_args
    flag = ["-O"] if mode == "cO" else []
    if not run(["compile.py", *args, "-c", *flag, "-o", "unit.o"]):
        return
    Path("script.ld").write_text(capture_binutils._SCRIPT, encoding="utf-8")
    scripted = ["-T", "script.ld", *defsym_args(),
                *capture_binutils._extern_defsyms("unit.o")]
    for extra, exe in (([], "prog"), (scripted, "prog_t")):
        run(["compile.py", "unit.o", *extra, "-o", exe])


def _game_commands(run, game: str) -> None:
    """Runs the commands that compile and link the game `game`."""
    import capture_binutils
    from sim.linked import defsym_args
    script, units = capture_binutils._GAMES[game]
    d = _ROOT / "examples" / game
    objects = []
    for u in units:
        if run(["compile.py", str(d / f"{u}.c"), "-c", "-O", "-o", f"{u}.o"]):
            objects.append(f"{u}.o")
    links = [(["-T", str(d / script)], game), ([], f"{game}_default")]
    if game == "ali_baba":
        links.append((["-T", str(d / script), *defsym_args()], f"{game}_sim"))
    for extra, exe in links:
        run(["compile.py", *extra, *objects, "-o", exe])
    # Every unit in one -c, and the program linked from its sources, so
    # that one C++ process compiles several units in a row.
    os.mkdir("all")
    sources = [str(d / f"{u}.c") for u in units]
    os.chdir("all")
    try:
        run(["compile.py", "-c", "-O", *sources])
    finally:
        os.chdir("..")
    run(["compile.py", "-O", "-T", str(d / script), *sources,
         "-o", f"{game}_sources"])


def _error_commands(run) -> None:
    """Runs the links of damaged objects and the bad link command
    lines."""
    import capture_binutils
    src = _ROOT / "examples" / "apply_bobble.c"
    if not run(["compile.py", str(src), "-c", "-O", "-o", "good.o"]):
        return
    bad = capture_binutils._damaged(Path("good.o").read_bytes())
    for name, data in bad.items():
        Path(name).write_bytes(data)
    for name, text in _BAD_FILES.items():
        Path(name).write_text(text, encoding="utf-8", newline="\n")
    Path("notutf8.c").write_bytes(b"int x = 1; /* \xff */\n")
    Path("notutf8.asm").write_bytes(b"; \xff\n")
    for name in bad:
        run(["compile.py", name, "-o", "prog"])
        run(["compile.py", "good.o", name, "-o", "prog"])
    for cmd in _BAD_LINKS:
        run(["compile.py", *[a.replace("{f}", "good.o") for a in cmd]])


def _run_commands(job, cpp: bool, work: Path, tmp: str) -> tuple[list, dict]:
    """Runs the commands of `job` in the empty directory `work` with
    the C++ back end on or off, and returns each command's result and
    the files left in `work`."""
    kind, label, args, mode = job
    results = []

    def run(argv: list[str]) -> bool:
        got = _compile(argv, cpp)
        results.append((argv, got))
        return got[0] == 0 and got[2] is None

    here = os.getcwd()
    os.chdir(work)
    try:
        if kind == "program":
            _program_commands(run, _source_args(args, tmp), mode)
        elif kind == "game":
            _game_commands(run, label)
        elif kind == "cli":
            _cli_commands(run, int(args[0]))
        else:
            _error_commands(run)
    finally:
        os.chdir(here)
    return results, _files(work)


def _run_job(job) -> tuple[str, int, bool, str]:
    """Runs one job with each back end and returns its label, the
    number of commands it ran, whether the two runs agreed, and, when
    they didn't, what differed."""
    kind, label, args, mode = job
    source = f"{label} {mode}" if kind == "program" else label
    with tempfile.TemporaryDirectory() as tmp:
        work = Path(tmp) / "work"
        got = []
        for cpp in (False, True):
            if work.exists():
                shutil.rmtree(work)
            work.mkdir()
            got.append(_run_commands(job, cpp, work, tmp))
    (py_results, py_files), (cpp_results, cpp_files) = got
    if got[0] == got[1]:
        return source, len(py_results), True, ""
    for (argv, a), (_, b) in zip(py_results, cpp_results):
        if a != b:
            names = ("exit status", "stdout and stderr", "exception")
            diffs = [n for n, x, y in zip(names, a, b) if x != y]
            detail = f"{' '.join(argv[1:])}: {', '.join(diffs)}"
            if a[2] != b[2]:
                detail += f" (Python: {a[2]}, C++: {b[2]})"
            elif a[1] != b[1]:
                detail += f" (Python: {a[1][-300:]!r}, C++: {b[1][-300:]!r})"
            return source, len(py_results), False, detail
    if len(py_results) != len(cpp_results):
        return source, len(py_results), False, "the number of commands"
    names = sorted(set(py_files) | set(cpp_files))
    differ = [n for n in names if py_files.get(n) != cpp_files.get(n)]
    return source, len(py_results), False, f"the files {differ}"


def cli_jobs() -> list[tuple]:
    """The jobs of the corpus `cli`, one for each width of
    `_CLI_COLUMNS`."""
    return [("cli", f"columns {w}", [str(w)], "") for w in _CLI_COLUMNS]


def _parse_seeds(text: str) -> range:
    lo, _, hi = text.partition("-")
    return range(int(lo), int(hi or lo) + 1)


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--corpus", default="examples,games,chapters,fuzz,links,cli")
    ap.add_argument("--fuzz-seeds", default="0-99")
    ap.add_argument("--modes", default="O,unopt,cO,c",
                    help="O for -S -O, unopt for -S, cO for -c -O and the "
                         "links, c for -c and the links, lex for --lex, "
                         "parse for --parse, resolve and resolveO for "
                         "--resolve without and with -O, and tac and tacO "
                         "for --tac without and with -O")
    ap.add_argument("-j", type=int, default=os.cpu_count() or 1)
    ns = ap.parse_args(argv)
    if cpp_cli() is None:
        print("compare_backends: build the C++ port first (cpp/README.md)",
              file=sys.stderr)
        return 2
    kinds = ns.corpus.split(",")
    jobs = []
    if "links" in kinds:
        import capture_binutils
        jobs += [("game", g, [], "") for g in capture_binutils._GAMES]
        jobs.append(("errors", "damaged files and bad links", [], ""))
    if "cli" in kinds:
        jobs += cli_jobs()
    jobs += [
        ("program", label, args, mode)
        for label, args in corpus([k for k in kinds if k not in ("links", "cli")],
                                  _parse_seeds(ns.fuzz_seeds))
        for mode in ns.modes.split(",")
    ]
    start = time.time()
    differ = 0
    commands = 0
    with ProcessPoolExecutor(max_workers=ns.j) as pool:
        for source, n, same, detail in pool.map(_run_job, jobs, chunksize=2):
            commands += n
            if not same:
                differ += 1
                print(f"DIFFER {source}: {detail}", flush=True)
    print(f"{len(jobs)} jobs of {commands} commands compared in "
          f"{time.time() - start:.0f} s, {differ} differ")
    return 1 if differ else 0


if __name__ == "__main__":
    raise SystemExit(main())
