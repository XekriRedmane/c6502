"""Finds the TAC optimizer pass that changes a program's result.

Simulates the whole program with the TAC interpreter after every pass
of the TAC optimizer (`PhaseDriver.observer`), with the function being
optimized at that step and the rest as they stand, and reports the
first pass after which `main` returns something other than it did on
unoptimized TAC. The program is in SSA form between passes, which
`tac_sim.py` runs too.

That names the pass and the function without reducing the program
first. When no TAC pass changes the result but the `--optimize`
asm result is still wrong, the bug is in `tac_to_asm` or an asm pass.

    uv run python -m fuzz.tac_passes fuzz_out/stmts/stmts1549.c
"""
from __future__ import annotations

import sys
from pathlib import Path

import tac_ast
from passes.optimization import optimize_program
from passes.optimization.framework.driver import PhaseDriver
from tac_sim import Simulator


def _front_end(src: str):
    import sim.harness as H
    ast = H.label_loops(H.resolve_labels(H.lift_strings(
        H.resolve_identifiers(H.parse(H.preprocess(src))),
    )))
    ast, syms, types = H.type_check_program(ast)
    return H.translate_to_tac(ast, syms, types), syms, types


def first_changing_pass(src: str) -> tuple[str, str, object, object] | None:
    """`(function, pass, expected, got)` for the first pass after which
    `main` returns something other than on unoptimized TAC (or raises),
    or None when no pass changes it."""
    tac, syms, types = _front_end(src)

    def run(top_level) -> object:
        try:
            return Simulator(
                tac_ast.Program(top_level=list(top_level)), syms, types,
            ).call("main", [])
        except Exception as e:  # noqa: BLE001 — a crash is a change too
            return f"ERR {type(e).__name__}: {e}"

    expected = run(tac.top_level)
    current = list(tac.top_level)
    index = {
        tl.name: k for k, tl in enumerate(current)
        if isinstance(tl, tac_ast.Function)
    }
    found: list = []

    def observe(fn: tac_ast.Function, after: str) -> None:
        if found:
            return
        current[index[fn.name]] = fn
        got = run(current)
        if got != expected:
            found.append((fn.name, after, expected, got))

    PhaseDriver.observer = observe
    try:
        optimize_program(tac, syms)
    finally:
        PhaseDriver.observer = None
    return found[0] if found else None


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
