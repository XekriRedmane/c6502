"""Builds YARPGen, patched for c6502, into fuzz_out/yarpgen-src.

YARPGen (https://github.com/intel/yarpgen) generates random C programs
that are free of undefined behavior under its own model of C's types.
`fuzz/yarpgen.patch` adapts it to c6502:

  * `-DYARPGEN_INT_BITS=16` makes the model's `int` and `unsigned int`
    16 bits wide, with the evaluator's arithmetic and overflow checks
    done at that width, and `unsigned short` promoting to `unsigned
    int` when `int` can't hold it.
  * `min` and `max` are plain C99 macros instead of GNU statement
    expressions.
  * `--max-loop-iters` and `--max-stmts` keep programs small enough
    for the 6502's memory and the simulator's speed.

This checks out the pinned commit, applies the patch, and builds the
generator with a C++ compiler that needs nothing installed: Zig's
bundled clang (the `ziglang` package) and CMake and Ninja from PyPI,
all fetched through `uvx`.

    uv run python -m fuzz.build_yarpgen
"""
from __future__ import annotations

import os
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
SRC = REPO / "fuzz_out" / "yarpgen-src"
URL = "https://github.com/intel/yarpgen.git"
# The commit `fuzz/yarpgen.patch` applies to.
PIN = "1adb290f453505838f3aa33dc571f292b0c1810f"
PATCH = REPO / "fuzz" / "yarpgen.patch"


def binary() -> Path:
    """The built generator."""
    exe = "yarpgen.exe" if os.name == "nt" else "yarpgen"
    return SRC / "build" / exe


def _run(*cmd, cwd=None, capture=False) -> str:
    out = subprocess.run(
        [str(c) for c in cmd], cwd=cwd, check=True, text=True,
        capture_output=capture,
    )
    return out.stdout.strip() if capture else ""


def _uvx_python(package: str, code: str) -> str:
    return _run("uvx", "--from", package, "python", "-c", code, capture=True)


def _wrappers(tools: Path) -> dict[str, Path]:
    """Scripts that run `zig c++` / `zig ar` / `zig ranlib`, since
    CMake wants one executable per tool."""
    zig_dir = _uvx_python(
        "ziglang", "import ziglang, os; print(os.path.dirname(ziglang.__file__))",
    )
    zig = Path(zig_dir) / ("zig.exe" if os.name == "nt" else "zig")
    tools.mkdir(parents=True, exist_ok=True)
    out = {}
    for name, sub in (("cxx", "c++"), ("ar", "ar"), ("ranlib", "ranlib")):
        if os.name == "nt":
            path = tools / f"z{name}.cmd"
            path.write_text(f'@"{zig}" {sub} %*\r\n')
        else:
            path = tools / f"z{name}"
            path.write_text(f'#!/bin/sh\nexec "{zig}" {sub} "$@"\n')
            path.chmod(0o755)
        out[name] = path
    return out


def build() -> Path:
    if not (SRC / ".git").exists():
        SRC.parent.mkdir(parents=True, exist_ok=True)
        _run("git", "clone", "-q", URL, SRC)
    _run("git", "checkout", "-q", "--force", PIN, cwd=SRC)
    _run("git", "apply", PATCH, cwd=SRC)
    tools = _wrappers(SRC / "tools")
    ninja_dir = _uvx_python("ninja", "import ninja; print(ninja.BIN_DIR)")
    ninja = Path(ninja_dir) / ("ninja.exe" if os.name == "nt" else "ninja")
    build_dir = SRC / "build"
    _run(
        "uvx", "cmake", "-S", SRC, "-B", build_dir, "-G", "Ninja",
        f"-DCMAKE_MAKE_PROGRAM={ninja}",
        f"-DCMAKE_CXX_COMPILER={tools['cxx']}",
        f"-DCMAKE_AR:FILEPATH={tools['ar']}",
        f"-DCMAKE_RANLIB:FILEPATH={tools['ranlib']}",
        "-DYARPGEN_INT_BITS=16",
    )
    # YARPGen's own unit tests assume a 32-bit int, so only the
    # generator is built.
    _run("uvx", "cmake", "--build", build_dir, "--target", "yarpgen")
    return binary()


if __name__ == "__main__":
    print(build())
    sys.exit(0)
