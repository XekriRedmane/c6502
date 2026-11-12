"""Runs a C program through the three c6502 execution paths and
compares main()'s return value.

The three paths:
  * `tac`   — the TAC interpreter (`tac_sim`) on unoptimized TAC.
  * `unopt` — the asm simulator on `compile.py --codegen` output.
  * `opt`   — the asm simulator on `compile.py --codegen --optimize`
              output.

Each result is main()'s return as a signed 16-bit int, or a string
starting with "ERR" when that path raised (a compile-time rejection,
an assembler error, a sim error) or "TIMEOUT".

Run as a script to check hand-written programs. Each file may carry
a `// expect: N` line, in which case every path must return N;
otherwise the paths must agree with each other.

    uv run python -m fuzz.runner prog1.c prog2.c dir_of_programs/
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

from passes import ir_check
from sim.harness import build_sim
from tests.test_sim_chapters import _run_program as _tac_run

# Check the IR after every pass (passes/ir_check.py), so a pass that
# breaks it fails there even when the program's result comes out
# right. A failed check is an `ERR` result like any other.
ir_check.enable()

MODES = ("tac", "unopt", "opt")
MAX_CYCLES = 20_000_000


def s16(v: int) -> int:
    """`v` as a signed 16-bit int (main() returns int)."""
    v &= 0xFFFF
    return v - 0x10000 if v & 0x8000 else v


def run_tac(src: str) -> int | str:
    try:
        v = _tac_run(src)
    except Exception as e:  # noqa: BLE001 — any failure is a result
        return f"ERR {type(e).__name__}: {str(e)[:120]}"
    return s16(v) if isinstance(v, int) else f"ERR non-int result {v!r}"


def run_asm(src: str, optimize: bool) -> int | str:
    try:
        result = build_sim(src, optimize=optimize).run(max_cycles=MAX_CYCLES)
    except Exception as e:  # noqa: BLE001 — any failure is a result
        return f"ERR {type(e).__name__}: {str(e)[:120]}"
    if result.timed_out:
        return "TIMEOUT"
    return result.return_int_signed()


def run_all(src: str) -> dict[str, int | str]:
    """main()'s result on every path, keyed by `MODES`."""
    return {
        "tac": run_tac(src),
        "unopt": run_asm(src, False),
        "opt": run_asm(src, True),
    }


def is_error(v: int | str) -> bool:
    return isinstance(v, str)


# Assembler errors that mark a backend limit rather than a miscompile:
# the soft-stack frame is addressed through `(FP),Y`, so a frame over
# 253 bytes (or an offset past 255) can't be built.
_BACKEND_LIMITS = ("local_bytes", "frame offset")


def is_backend_limit(v: int | str) -> bool:
    return isinstance(v, str) and any(s in v for s in _BACKEND_LIMITS)


def disagreements(
    res: dict[str, int | str], expected: int | None,
) -> tuple[str, ...]:
    """The paths whose result is wrong. With `expected` None, the
    TAC result is the reference and only the asm paths can be wrong
    (a TAC error makes every path count as wrong, since there's no
    reference)."""
    if expected is not None:
        return tuple(m for m in MODES if res[m] != expected)
    ref = res["tac"]
    if is_error(ref):
        return MODES
    return tuple(m for m in ("unopt", "opt") if res[m] != ref)


def format_result(res: dict[str, int | str]) -> str:
    return " ".join(f"{m}={res[m]}" for m in MODES)


def fuzz_main(make_program, prog: str, argv: list[str] | None) -> int:
    """Shared command line for the generators. `make_program(seed)`
    returns `(source, expected)`, where `expected` is main()'s
    correct return, or None to use the TAC result as the reference.

    A program where some path gives a wrong value is a failure and is
    written to the output directory as `<prog><seed>.c`. So is one
    the front end rejects, because the generators only produce valid
    C. A program that only an asm path rejects with a known backend
    limit (`is_backend_limit`, an unoptimized frame over 253 bytes)
    while the TAC result is right is counted separately and not
    saved. Any other asm-path error is a failure: `fuzz.stmts` seed
    987 found a duplicate label that way, which had been counted as a
    rejection."""
    ap = argparse.ArgumentParser(prog=f"python -m fuzz.{prog}")
    ap.add_argument("--start", type=int, default=0, help="first seed")
    ap.add_argument("--count", type=int, default=100, help="number of seeds")
    ap.add_argument(
        "--out", type=Path, default=Path("fuzz_out") / prog,
        help="directory for failing programs (default fuzz_out/<name>)",
    )
    args = ap.parse_args(argv)
    args.out.mkdir(parents=True, exist_ok=True)
    wrong = rejected = 0
    for seed in range(args.start, args.start + args.count):
        src, expected = make_program(seed)
        res = run_all(src)
        bad = disagreements(res, expected)
        if (
            bad and "tac" not in bad
            and all(is_backend_limit(res[m]) for m in bad)
        ):
            rejected += 1
            print(f"seed {seed}: rejected {format_result(res)}", flush=True)
            continue
        if bad:
            wrong += 1
            path = args.out / f"{prog}{seed}.c"
            path.write_text(src, newline="\n")
            print(f"seed {seed}: WRONG {format_result(res)} -> {path}",
                  flush=True)
    print(f"{wrong} wrong, {rejected} rejected, of {args.count} programs")
    return 1 if wrong else 0


def _expected_of(src: str) -> int | None:
    m = re.search(r"//\s*expect:\s*(-?\d+)", src)
    return s16(int(m.group(1))) if m else None


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(
        description="Run C files through tac_sim and both asm pipelines.",
    )
    ap.add_argument("paths", nargs="+", type=Path)
    args = ap.parse_args(argv)
    files: list[Path] = []
    for p in args.paths:
        files += sorted(p.glob("*.c")) if p.is_dir() else [p]
    bad = 0
    for f in files:
        src = f.read_text()
        expected = _expected_of(src)
        res = run_all(src)
        wrong = disagreements(res, expected)
        bad += bool(wrong)
        flag = "FAIL" if wrong else "ok  "
        print(f"{flag} {f.name:32} exp={expected} {format_result(res)}")
    print(f"{bad} failing of {len(files)}")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
