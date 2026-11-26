"""The C++ build that the end-to-end tests compile with.

compile.py, nm.py, and objdump.py are shims over `c6502cpp`, the
compiler written in C++, so a test that compiles a program is skipped
when `c6502cpp` isn't built. It is found as cpp_backend.py
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
