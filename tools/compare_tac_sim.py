"""Runs `main` of each program of the corpus on its TAC with tac_sim.py
and with the C++ port's TAC interpreter (cpp/src/tac_sim.h), and checks
that the two give the same results.

    uv run python tools/compare_tac_sim.py -j 12
    uv run python tools/compare_tac_sim.py --corpus examples --no-observe

The corpus is tools/compare_backends.py's (`tools/capture_passes.corpus`):
the examples, the games' units, every C file under `tests/chapter_*`,
and seeds `--fuzz-seeds` of `fuzz.stmts`, `fuzz.calls`, and
`fuzz.exprs`. Each program is preprocessed with its file name and its
`-I` arguments, so a game's unit finds its headers, and then goes through
the front end of `tests.test_sim_chapters._run_program` and `c99_to_tac`
in each compiler. Each comparison is of the front end's error (its
class and message), or of `main`'s result on the unoptimized TAC and,
when that is a value, on the TAC after the TAC optimizer, each result
being the value or the class and message of what the interpreter
raised. With the observer on, which is the default, the result of
fuzz/tac_passes.py's `first_changing_pass` is compared too, and every
program must give None in both, since no TAC pass may change a
program's result. A program whose `main` recurses more deeply than
Python's limit allows (about 330 calls) gives Python's RecursionError and
a result in C++, which the tool reports as a difference.

The exit status is 0 when every program agrees. c6502cpp comes from
`C6502_CPP` or cpp/build/bin, as for tools/check_captures.py.
"""
from __future__ import annotations

import argparse
import importlib
import os
import sys
import time
from concurrent.futures import ProcessPoolExecutor
from pathlib import Path

_ROOT = Path(__file__).resolve().parent.parent
if str(_ROOT) not in sys.path:
    sys.path.insert(0, str(_ROOT))

from tools.capture_passes import corpus  # noqa: E402
from tools.check_captures import cpp_cli  # noqa: E402


def _outcome(run) -> tuple:
    """`("value", v)` for what `run()` returns, or `("error", class,
    message)` for what it raises."""
    try:
        return ("value", run())
    except Exception as e:  # noqa: BLE001 - an error is a result
        return ("error", type(e).__name__, str(e))


def _python(source: str, args: list[str], filename: str | None,
            observe: bool) -> tuple:
    """The front end's error, or the results on the unoptimized and the
    optimized TAC and the observer's, from the Python compiler."""
    import tac_ast
    from c99_to_tac import translate_program
    from parser import parse
    from passes.identifier_resolution import resolve_program as resolve_identifiers
    from passes.label_resolution import resolve_program as resolve_labels
    from passes.loop_labeling import label_program
    from passes.optimization import optimize_program
    from passes.optimization.framework.driver import PhaseDriver
    from passes.string_lifting import lift_program
    from passes.type_checking import check_program
    from preprocessor import preprocess
    from tac_sim import Simulator

    def front():
        ast = label_program(resolve_labels(lift_program(resolve_identifiers(
            parse(preprocess(source, args, filename))))))
        ast, syms, types = check_program(ast)
        return translate_program(ast, syms, types), syms, types

    try:
        tac, syms, types = front()
    except Exception as e:  # noqa: BLE001
        return ("front", type(e).__name__, str(e))
    plain = _outcome(lambda: Simulator(tac, syms, types).call("main", []))
    found = None
    if observe:
        def run(top_level):
            try:
                return Simulator(
                    tac_ast.Program(top_level=list(top_level)), syms, types,
                ).call("main", [])
            except Exception as e:  # noqa: BLE001
                return f"ERR {type(e).__name__}: {e}"
        expected = run(tac.top_level)
        current = list(tac.top_level)
        index = {tl.name: k for k, tl in enumerate(current)
                 if isinstance(tl, tac_ast.Function)}
        hits: list = []

        def observer(fn, after):
            if hits:
                return
            current[index[fn.name]] = fn
            got = run(current)
            if got != expected:
                hits.append((fn.name, after, expected, got))
        PhaseDriver.observer = observer
    try:
        opt = optimize_program(tac, syms) if plain[0] == "value" or observe else None
    finally:
        PhaseDriver.observer = None
    if observe:
        found = hits[0] if hits else None
    optimized = None
    if plain[0] == "value":
        optimized = _outcome(lambda: Simulator(opt, syms, types).call("main", []))
    return ("ran", plain, optimized, found)


def _cpp(source: str, args: list[str], filename: str | None, observe: bool) -> tuple:
    """The same from the C++ port's modes."""
    import cpp_backend
    kw = {"pcpp_args": args, "filename": filename}

    def request(mode, **extra):
        lines, _ = cpp_backend._tac_request(mode, source, **kw, **extra)
        return lines

    try:
        lines = request("tac-run", optimized=True)
    except Exception as e:  # noqa: BLE001
        return ("front", type(e).__name__, str(e))
    plain = _outcome(lambda: cpp_backend._tac_value(lines["result"]))
    optimized = None
    if "optimized" in lines:
        optimized = _outcome(lambda: cpp_backend._tac_value(lines["optimized"]))
    found = None
    if observe:
        import json
        found = json.loads(request("tac-observe")["found"])
        found = None if found is None else tuple(found)
    return ("ran", plain, optimized, found)


def _source(args: list[str]) -> tuple[str, list[str], str | None]:
    """The source text of a corpus program, its preprocessor arguments,
    and its file name."""
    if args[0].startswith("fuzz:"):
        _, tool, seed = args[0].split(":")
        src = importlib.import_module(f"fuzz.{tool}").make_program(int(seed))[0]
        return src, [], None
    path = args[0]
    return Path(path).read_text(encoding="utf-8"), args[1:], path


def _run_job(job) -> tuple[str, bool, str, bool]:
    """Compares one program and returns its label, whether the two
    agreed, what differed, and whether some pass changed its result."""
    label, args, observe = job
    source, pcpp_args, filename = _source(args)
    # The Python side reads the variable, so it is off for it.
    os.environ.pop("C6502_CPP_BACKEND", None)
    py = _python(source, pcpp_args, filename, observe)
    cpp = _cpp(source, pcpp_args, filename, observe)
    changed = py[0] == "ran" and (py[3] is not None or cpp[3] is not None)
    if py == cpp:
        return label, True, "", changed
    return label, False, f"Python {py!r:.400}, C++ {cpp!r:.400}", changed


def _parse_seeds(text: str) -> range:
    lo, _, hi = text.partition("-")
    return range(int(lo), int(hi or lo) + 1)


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--corpus", default="examples,games,chapters,fuzz")
    ap.add_argument("--fuzz-seeds", default="0-99")
    ap.add_argument("--no-observe", action="store_true",
                    help="leave out the observer of fuzz/tac_passes.py")
    ap.add_argument("-j", type=int, default=os.cpu_count() or 1)
    ns = ap.parse_args(argv)
    if cpp_cli() is None:
        print("compare_tac_sim: build the C++ port first (cpp/README.md)",
              file=sys.stderr)
        return 2
    jobs = [
        (label, args, not ns.no_observe)
        for label, args in corpus(ns.corpus.split(","), _parse_seeds(ns.fuzz_seeds))
    ]
    start = time.time()
    differ = changed = 0
    with ProcessPoolExecutor(max_workers=ns.j) as pool:
        for label, same, detail, change in pool.map(_run_job, jobs, chunksize=2):
            if not same:
                differ += 1
                print(f"DIFFER {label}: {detail}", flush=True)
            if change:
                changed += 1
                print(f"CHANGED {label}: a TAC pass changes main's result", flush=True)
    print(f"{len(jobs)} programs compared in {time.time() - start:.0f} s, "
          f"{differ} differ, {changed} changed by a pass")
    return 1 if differ or changed else 0


if __name__ == "__main__":
    raise SystemExit(main())
