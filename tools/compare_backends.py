"""Compiles the corpus with the Python back end and with the C++ one
(`C6502_CPP_BACKEND=1`) and checks that the two give the same output.

    uv run python tools/compare_backends.py -j 12
    uv run python tools/compare_backends.py --corpus examples,games --modes O

The corpus is the capture harness's (`tools/capture_passes.corpus`):
the examples, the games' units, every C file under `tests/chapter_*`,
and seeds `--fuzz-seeds` of `fuzz.stmts`, `fuzz.calls`, and
`fuzz.exprs`. Each program is compiled with `-S` and with `-S -O`
(`--modes`), once with the variable unset and once with it set to 1, in
the same worker process, since compile.py reads the variable at each
compile. The two compiles of a program must agree on everything they
give: the `-S` text, byte for byte, the exit status, what compile.py
writes to stdout and stderr (the errors and warnings), and, for a
compile that raises, the exception's type and message. The exit status
is 0 when every pair agrees. c6502cpp comes from `C6502_CPP` or
cpp/build/bin, as for tools/check_captures.py.
"""
from __future__ import annotations

import argparse
import contextlib
import importlib
import io
import os
import sys
import tempfile
import time
from concurrent.futures import ProcessPoolExecutor
from pathlib import Path

_ROOT = Path(__file__).resolve().parent.parent
if str(_ROOT) not in sys.path:
    sys.path.insert(0, str(_ROOT))

from tools.capture_passes import corpus  # noqa: E402
from tools.check_captures import cpp_cli  # noqa: E402

_VARIABLE = "C6502_CPP_BACKEND"


def _compile(argv: list[str], out: str, cpp: bool) -> tuple:
    """Runs `compile.main(argv)` with the C++ back end on or off and
    returns what it gave: the exit status, the text written to stdout
    and stderr, the `-S` output's bytes (None when there's no file), and
    the type and message of an exception it raised (None when it
    raised none)."""
    import compile as compile_module
    if cpp:
        os.environ[_VARIABLE] = "1"
    else:
        os.environ.pop(_VARIABLE, None)
    if os.path.exists(out):
        os.remove(out)
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
    text = None
    if os.path.exists(out):
        with open(out, "rb") as f:
            text = f.read()
    # Both compiles read the same source file and write the same output
    # path, so messages that name either are equal.
    return status, sink.getvalue(), text, raised


def _run_job(job) -> tuple[str, bool, str]:
    """Compiles one program of the corpus in one mode with each back end
    and returns its label, whether the two agreed, and, when they
    didn't, what differed."""
    label, args, mode = job
    source = f"{label} -S{' -O' if mode == 'O' else ''}"
    with tempfile.TemporaryDirectory() as tmp:
        if args[0].startswith("fuzz:"):
            _, tool, seed = args[0].split(":")
            src = importlib.import_module(f"fuzz.{tool}").make_program(int(seed))[0]
            path = os.path.join(tmp, f"{tool}{seed}.c")
            with open(path, "w", encoding="utf-8", newline="\n") as f:
                f.write(src)
            args = [path] + args[1:]
        out = os.path.join(tmp, "out.s")
        argv = ["compile.py", *args, "-S", "-o", out]
        if mode == "O":
            argv.append("-O")
        python = _compile(argv, out, cpp=False)
        cpp = _compile(argv, out, cpp=True)
    if python == cpp:
        return source, True, ""
    names = ("exit status", "stdout and stderr", "-S output", "exception")
    diffs = [n for n, a, b in zip(names, python, cpp) if a != b]
    detail = ", ".join(diffs)
    if python[3] != cpp[3]:
        detail += f" (Python: {python[3]}, C++: {cpp[3]})"
    elif python[1] != cpp[1]:
        detail += f" (Python: {python[1][-200:]!r}, C++: {cpp[1][-200:]!r})"
    return source, False, detail


def _parse_seeds(text: str) -> range:
    lo, _, hi = text.partition("-")
    return range(int(lo), int(hi or lo) + 1)


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--corpus", default="examples,games,chapters,fuzz")
    ap.add_argument("--fuzz-seeds", default="0-99")
    ap.add_argument("--modes", default="O,unopt",
                    help="O for -S -O, unopt for -S")
    ap.add_argument("-j", type=int, default=os.cpu_count() or 1)
    ns = ap.parse_args(argv)
    if cpp_cli() is None:
        print("compare_backends: build the C++ port first (cpp/README.md)",
              file=sys.stderr)
        return 2
    jobs = [
        (label, args, mode)
        for label, args in corpus(ns.corpus.split(","),
                                  _parse_seeds(ns.fuzz_seeds))
        for mode in ns.modes.split(",")
    ]
    start = time.time()
    differ = 0
    with ProcessPoolExecutor(max_workers=ns.j) as pool:
        for source, same, detail in pool.map(_run_job, jobs, chunksize=4):
            if not same:
                differ += 1
                print(f"DIFFER {source}: {detail}")
    print(f"{len(jobs)} compiles compared in {time.time() - start:.0f} s, "
          f"{differ} differ")
    return 1 if differ else 0


if __name__ == "__main__":
    raise SystemExit(main())
