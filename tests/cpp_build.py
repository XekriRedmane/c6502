"""The C++ build that the end-to-end tests compile with.

compile.py, nm.py, and objdump.py run in `c6502cpp` under
`C6502_CPP_BACKEND=1`, and only there once the Python compiler is
removed (docs/cpp_port.md, step 8), so a test that compiles a program
is skipped when `c6502cpp` isn't built. It is found as cpp_backend.py
finds it, at the path in `C6502_CPP` or in cpp/build/bin.

`needs_c6502cpp` decorates a test class or method, and `BUILT` and
`REASON` serve a test that skips itself or combines the condition with
another, such as dasm being installed."""

import os
import unittest

import cpp_backend


def _built() -> bool:
    path = cpp_backend._cpp_cli()
    return path is not None and os.path.exists(path)


BUILT = _built()
REASON = "c6502cpp isn't built (cpp/README.md has the build)"

needs_c6502cpp = unittest.skipUnless(BUILT, REASON)
