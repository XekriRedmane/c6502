"""Finds the TAC optimizer pass that changes a program's result.

Simulates the whole program with the TAC interpreter after every pass
of the TAC optimizer, with the function being optimized at that step
and the rest as they stand, and reports the first pass after which
`main` returns something other than it did on unoptimized TAC. The C++
port does the work (`cpp_backend.tac_first_changing_pass`): its TAC
optimizer's driver calls an observer after each step, and its
interpreter runs the program in the SSA form it has between passes.

That names the pass and the function without reducing the program
first. When no TAC pass changes the result but the `-O`
asm result is still wrong, the bug is in `tac_to_asm` or an asm pass.

    uv run python -m fuzz.tac_passes fuzz_out/stmts/stmts1549.c
"""
from __future__ import annotations

import sys
from pathlib import Path


def first_changing_pass(src: str) -> tuple[str, str, object, object] | None:
    """`(function, pass, expected, got)` for the first pass after which
    `main` returns something other than on unoptimized TAC (or raises),
    or None when no pass changes it. A result is an int, None, or the
    text `ERR <type>: <message>` of what the interpreter raised."""
    import cpp_backend
    return cpp_backend.tac_first_changing_pass(src)


def main(argv: list[str]) -> int:
    for path in argv:
        res = first_changing_pass(Path(path).read_text())
        if res is None:
            print(f"{path}: no TAC pass changes main()'s result")
        else:
            fn, after, expected, got = res
            print(f"{path}: after {after} in {fn}, main() returns {got} "
                  f"instead of {expected}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
