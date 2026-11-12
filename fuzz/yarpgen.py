"""Random programs from YARPGen, run through every c6502 path.

YARPGen generates loop nests over arrays and scalars of every integer
type, with values it has checked for undefined behavior under its
model of C. `fuzz/build_yarpgen.py` builds it with that model's `int`
16 bits wide, as c6502's is, so a program it accepts is well-defined
for c6502 too. Each seed's test is three files, which `merge` turns
into one c6502 program:

  * `init.h` (declarations), `func.c` (`test()`), and `driver.c`
    (definitions, `init()`, `checksum()`, and `main`), in that order.
  * `main` returns the checksum, folded to 15 bits, instead of
    printing it, since c6502 has no `printf`. The TAC interpreter's
    result is the reference, as in `fuzz.calls`.
  * `size_t` is `unsigned int`, and `short` is `int`, which c6502
    doesn't have yet. With a 16-bit `int` the two are the same width
    and promote the same way.
  * Unary `+`, which c6502 doesn't have yet either, is dropped.
    YARPGen writes every implicit conversion as a cast, so its
    operand is already promoted and the `+` does nothing.

Inputs are globals (`--inp-as-args=none`), since a `test()` taking them
as parameters needs more than the 253 bytes a soft-stack frame can
hold. Arrays have one dimension, loops at most 6 iterations, and a
test at most 25 statements, which keeps a program to a few seconds in
the simulator. A large `test()` still needs more than 253 bytes of
temporaries without --optimize, which the runner counts as rejected.

    uv run python -m fuzz.build_yarpgen          # once
    uv run python -m fuzz.yarpgen --start 0 --count 100
"""
from __future__ import annotations

import subprocess
import sys
import tempfile
from pathlib import Path

from fuzz.build_yarpgen import binary
from fuzz.runner import fuzz_main

OPTIONS = [
    "--std=c", "--emit-align-attr=none", "--emit-pragmas=none",
    "--max-array-dims=1", "--inp-as-args=none",
    "--max-loop-iters=6", "--max-stmts=25",
]


def merge(init: str, func: str, driver: str) -> str:
    """One c6502 program from a YARPGen test's three files."""
    func = func.replace('#include "init.h"', "")
    driver = driver.replace('#include "init.h"', "")
    driver = driver.replace("#include <stdio.h>", "")
    printf = 'printf("%llu\\n", seed);'
    if printf not in driver:
        raise ValueError("driver.c doesn't print the checksum")
    driver = driver.replace(
        printf,
        "return (int)((seed ^ (seed >> 16) ^ (seed >> 32) ^ (seed >> 48))"
        " & 0x7FFF);",
    )
    func = func.replace("(+(", "((")
    driver = driver.replace("(+(", "((")
    prelude = "typedef unsigned int size_t;\n#define short int\n"
    return prelude + init + func + driver


def make_program(seed: int) -> tuple[str, None]:
    """The program for `seed`. YARPGen reserves seed 0 for a random
    one, so it gets `seed + 1`."""
    exe = binary()
    if not exe.exists():
        sys.exit(f"{exe} not found; run python -m fuzz.build_yarpgen")
    with tempfile.TemporaryDirectory() as d:
        subprocess.run(
            [str(exe), f"--seed={seed + 1}", f"--out-dir={d}", *OPTIONS],
            check=True, capture_output=True,
        )
        files = [(Path(d) / n).read_text() for n in ("init.h", "func.c",
                                                      "driver.c")]
    return merge(*files), None


if __name__ == "__main__":
    sys.exit(fuzz_main(make_program, "yarpgen", sys.argv[1:]))
