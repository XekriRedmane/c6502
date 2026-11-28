"""Compares the C++ fuzzers (`c6502cpp fuzz`, cpp/src/fuzz/) with the
Python ones in fuzz/, for group G6 of step 9 of the port
(docs/cpp_port.md). It goes with fuzz/'s Python.

    uv run python tools/compare_fuzzers.py generators [--count 300]
    uv run python tools/compare_fuzzers.py runner [-j 10]
    uv run python tools/compare_fuzzers.py reduce [--tac-opt] FILE...
    uv run python tools/compare_fuzzers.py tac-passes FILE...

`generators` checks that each generator writes the same program, and
the same expected return, for seeds 0 to count - 1 in both languages.
`runner` runs seeds 0 to 299 of exprs, calls, and stmts and 0 to 99 of
yarpgen through `c6502cpp fuzz GENERATOR --verbose -j N` and through the
Python runner's `_check_seed`, and compares each seed's verdict (ok,
rejected, or WRONG) and its three results. `reduce` reduces copies of
each file with both reducers and compares the `.min.c` files and what
each wrote, and `tac-passes` compares the two `tac-passes` outputs.

C6502_CPP names the c6502cpp the Python side runs, and it is the one
compared, so both sides run the same compiler."""

from __future__ import annotations

import argparse
import multiprocessing
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO))

RUNS = (("exprs", 300), ("calls", 300), ("stmts", 300), ("yarpgen", 100))


def cpp() -> str:
    exe = os.environ.get("C6502_CPP")
    if not exe:
        sys.exit("set C6502_CPP to the c6502cpp to compare")
    return exe


def generator(name: str):
    import importlib
    return importlib.import_module(f"fuzz.{name}").make_program


def compare_generators(count: int) -> int:
    differing = 0
    for name in ("exprs", "calls", "stmts"):
        make_program = generator(name)
        with tempfile.TemporaryDirectory() as d:
            out = subprocess.run(
                [cpp(), "fuzz", "programs", name, "--start", "0", "--count",
                 str(count), "--out", d],
                check=True, capture_output=True, text=True,
            ).stdout
            expected = dict(line.split() for line in out.splitlines())
            same = 0
            for seed in range(count):
                src, exp = make_program(seed)
                got = (Path(d) / f"{name}{seed}.c").read_bytes().decode("utf-8")
                if got == src and expected[str(seed)] == str(exp):
                    same += 1
                else:
                    differing += 1
                    print(f"{name} seed {seed}: the programs differ")
        print(f"{name}: {same} of {count} programs the same")
    return 1 if differing else 0


def _python_verdict(job: tuple[str, int]) -> tuple[str, int, str]:
    name, seed = job
    from fuzz.runner import _check_seed, format_result, is_backend_limit
    src, res, bad = _check_seed(generator(name), seed)
    if bad and "tac" not in bad and all(is_backend_limit(res[m]) for m in bad):
        verdict = "rejected"
    elif bad:
        verdict = "WRONG"
    else:
        verdict = "ok"
    return name, seed, f"{verdict} {format_result(res)}"


_LINE = re.compile(r"seed (\d+): (ok|rejected|WRONG) (.*?)(?: -> .*)?$")


def compare_runner(jobs: int) -> int:
    differing = 0
    for name, count in RUNS:
        with tempfile.TemporaryDirectory() as d:
            out = subprocess.run(
                [cpp(), "fuzz", name, "--start", "0", "--count", str(count),
                 "--out", d, "-j", str(jobs), "--verbose"],
                capture_output=True, text=True,
            ).stdout
        got = {}
        for line in out.splitlines():
            m = _LINE.match(line)
            if m:
                got[int(m.group(1))] = f"{m.group(2)} {m.group(3)}"
        with multiprocessing.get_context("spawn").Pool(jobs) as pool:
            want = {seed: v for _, seed, v in pool.imap(
                _python_verdict, [(name, s) for s in range(count)], chunksize=2)}
        same = 0
        counts: dict[str, int] = {}
        for seed in range(count):
            verdict = want[seed].split(" ", 1)[0]
            counts[verdict] = counts.get(verdict, 0) + 1
            if got.get(seed) == want[seed]:
                same += 1
            else:
                differing += 1
                print(f"{name} seed {seed}:\n  Python {want[seed]}\n  C++    {got.get(seed)}")
        tally = ", ".join(f"{n} {v}" for v, n in sorted(counts.items()))
        print(f"{name}: {same} of {count} seeds the same (Python: {tally})", flush=True)
    return 1 if differing else 0


def compare_reduce(files: list[str], tac_opt: bool) -> int:
    from fuzz import reduce as py_reduce
    from fuzz.runner import format_result, run_all
    differing = 0
    for f in files:
        with tempfile.TemporaryDirectory() as d:
            copy = Path(d) / Path(f).name
            shutil.copyfile(f, copy)
            src = copy.read_text()
            still_fails = py_reduce.make_predicate(src, tac_opt)
            if not still_fails(src):
                print(f"{f}: the input doesn't fail in Python")
                differing += 1
                continue
            want = py_reduce.reduce(src, still_fails)
            done = subprocess.run(
                [cpp(), "fuzz", "reduce", *(["--tac-opt"] if tac_opt else []),
                 str(copy)],
                capture_output=True, text=True,
            )
            got = copy.with_suffix(".min.c").read_bytes().decode("utf-8")
            tail = (f"tac plain/opt: {py_reduce._tac_results(want)}" if tac_opt
                    else format_result(run_all(want)))
            same = got == want and tail in done.stdout
            differing += 0 if same else 1
            print(f"{f}: {'the same' if same else 'DIFFERENT'} "
                  f"({len(src.splitlines())} lines to {len(want.splitlines())}; {tail})")
            if not same:
                print(f"Python:\n{want}\nC++:\n{got}\n{done.stdout}{done.stderr}")
    return 1 if differing else 0


def compare_tac_passes(files: list[str]) -> int:
    from fuzz.tac_passes import first_changing_pass
    differing = 0
    for f in files:
        res = first_changing_pass(Path(f).read_text())
        if res is None:
            want = f"{f}: no TAC pass changes main()'s result"
        else:
            fn, after, expected, got = res
            want = (f"{f}: after {after} in {fn}, main() returns {got} "
                    f"instead of {expected}")
        out = subprocess.run([cpp(), "fuzz", "tac-passes", f],
                             capture_output=True, text=True).stdout.strip()
        same = out == want
        differing += 0 if same else 1
        print(f"{'the same' if same else 'DIFFERENT'}: {want}"
              + ("" if same else f"\n  C++: {out}"))
    return 1 if differing else 0


def main() -> int:
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="what", required=True)
    g = sub.add_parser("generators")
    g.add_argument("--count", type=int, default=300)
    r = sub.add_parser("runner")
    r.add_argument("-j", type=int, default=10)
    d = sub.add_parser("reduce")
    d.add_argument("--tac-opt", action="store_true")
    d.add_argument("files", nargs="+")
    t = sub.add_parser("tac-passes")
    t.add_argument("files", nargs="+")
    args = ap.parse_args()
    if args.what == "generators":
        return compare_generators(args.count)
    if args.what == "runner":
        return compare_runner(args.j)
    if args.what == "reduce":
        return compare_reduce(args.files, args.tac_opt)
    return compare_tac_passes(args.files)


if __name__ == "__main__":
    sys.exit(main())
