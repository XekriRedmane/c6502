"""Line-based reducer for a failing fuzz program.

Repeatedly deletes single lines, then replaces binary subexpressions
`((a) op (b))` with one operand, keeping every change under which the
program still fails the same way. The generators put one statement
per line, so deleting a line usually leaves valid C. The result is
written next to the input as `<name>.min.c`.

"Fails the same way" means the same set of paths gives a wrong value
as in the original (see `fuzz.runner.disagreements`), and each path
still compiles or still errors as it did, so a reduction can't wander
off to a different bug or delete `main`. With `--tac-opt`, the failure is
instead that the optimized TAC simulates to a different main() result
than the unoptimized TAC, which narrows a TAC-optimizer bug without
compiling to asm at every step.

Deleting a `return` can make a function fall off its end. c6502 gives
such a function an implicit `Ret(0)`, but C99 6.9.1p12 makes the use
of its value undefined, and the -O pipeline may return something
else there. A candidate whose run on the TAC interpreter
reaches the end of a function that returns a value, other than
`main`, is therefore never kept (`falls_off_end`). That is stricter
than C, which allows the fall-off when the caller ignores the value,
and the generators always end such a function with a `return`.

    uv run python -m fuzz.reduce fuzz_out/calls/calls534.c
    uv run python -m fuzz.reduce --tac-opt fuzz_out/calls/calls23.c
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

from fuzz.runner import (
    MODES, _expected_of, disagreements, format_result, is_error, run_all,
)

_BINARY = re.compile(r"\(\(([^()]*)\) ([^()\s]+) \(([^()]*)\)\)")


def falls_off_end(src: str) -> bool:
    """Tells whether running `src` on the TAC interpreter reaches the
    end of a function, other than `main`, whose return type isn't
    `void`. Returns False for a program that the front end rejects or
    that the interpreter can't run, which the predicate rejects on its
    own. The C++ port (`cpp_backend.tac_falls_off_end`) declares a
    function that no program defines, `__reduce_fell_off`, and calls it
    at the end of each such function, so the interpreter raises when a
    run reaches one of those ends."""
    import cpp_backend
    return cpp_backend.tac_falls_off_end(src)


def _tac_results(src: str) -> tuple[int, int]:
    """main()'s result on unoptimized and optimized TAC, from the C++
    port's TAC interpreter (`cpp_backend.tac_results`)."""
    import cpp_backend
    return cpp_backend.tac_results(src)


def make_predicate(src: str, tac_opt: bool):
    """A function telling whether a candidate program still shows the
    failure that `src` shows. A candidate that falls off the end of a
    function returning a value (`falls_off_end`) never does."""
    if tac_opt:
        def still_fails(cand: str) -> bool:
            if falls_off_end(cand):
                return False
            try:
                plain, opt = _tac_results(cand)
            except Exception:  # noqa: BLE001 — invalid candidate
                return False
            return plain != opt
        return still_fails
    expected = _expected_of(src)
    original = run_all(src)
    signature = (
        disagreements(original, expected),
        tuple(is_error(original[m]) for m in MODES),
    )

    def still_fails(cand: str) -> bool:
        if falls_off_end(cand):
            return False
        res = run_all(cand)
        # Same wrong paths, and each path still compiles (or still
        # errors) as before, so deleting `main` doesn't count.
        return (
            disagreements(res, expected),
            tuple(is_error(res[m]) for m in MODES),
        ) == signature
    return still_fails


def reduce(src: str, still_fails) -> str:
    """The smallest variant of `src` this simple search finds."""
    lines = src.splitlines()

    def ok(cand_lines):
        return still_fails("\n".join(cand_lines) + "\n")

    changed = True
    while changed:
        changed = False
        i = 0
        while i < len(lines):
            # Keep the `// expect: N` line so the output still says
            # what main() should return.
            if "expect:" in lines[i]:
                i += 1
                continue
            cand = lines[:i] + lines[i + 1:]
            if ok(cand):
                lines = cand
                changed = True
            else:
                i += 1
        for i in range(len(lines)):
            matches = list(_BINARY.finditer(lines[i]))
            for m in reversed(matches):
                line = lines[i]
                for operand in (m.group(1), m.group(3)):
                    cand_line = line[:m.start()] + f"({operand})" + line[m.end():]
                    if ok(lines[:i] + [cand_line] + lines[i + 1:]):
                        lines[i] = cand_line
                        changed = True
                        break
    return "\n".join(lines) + "\n"


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(prog="python -m fuzz.reduce")
    ap.add_argument("path", type=Path)
    ap.add_argument(
        "--tac-opt", action="store_true",
        help="reduce a TAC-optimizer failure (plain vs optimized TAC)",
    )
    args = ap.parse_args(argv)
    src = args.path.read_text()
    still_fails = make_predicate(src, args.tac_opt)
    if not still_fails(src):
        print("the input doesn't fail", file=sys.stderr)
        return 1
    out = reduce(src, still_fails)
    dst = args.path.with_suffix(".min.c")
    dst.write_text(out, newline="\n")
    print(out)
    if args.tac_opt:
        print("tac plain/opt:", _tac_results(out))
    else:
        print(format_result(run_all(out)))
    print(f"-> {dst}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
