"""The TAC interpreter's run of a program, for the tests that import it."""
from __future__ import annotations


def _run_program(source: str) -> int | None:
    """Returns what `main` of the C source `source` returns on its
    unoptimized TAC, which the C++ port's front end and TAC interpreter
    compute (`cpp_backend.tac_main`)."""
    import cpp_backend
    return cpp_backend.tac_main(source)
