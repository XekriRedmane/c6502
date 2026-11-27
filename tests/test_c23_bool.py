"""Tests for C23's boolean type (C23 §6.2.5, §6.4.1, §6.4.4.6, §7.19).

Coverage:

  * `bool`, `true`, and `false` are keywords. `bool` is the type that
    `_Bool` names, and `true` and `false` are constants of type `bool`
    with the values 1 and 0, so `sizeof(true)` is 1 and `true + true`
    is the `int` 2. They work in constant expressions: array sizes,
    enumeration values, case labels, and static initializers.
  * A name spelled `bool`, `true`, or `false` is an error, since C23
    made them keywords.
  * `_Bool` still compiles, with a warning for each use that names
    `bool`, on the line of the use. compile.py prints the warnings to
    stderr and exits 0, and `diagnostics.collecting()` gives them to a
    caller of the library.
  * `<stdbool.h>` is C23's. It includes without a warning, leaves
    `bool`, `true`, and `false` alone, and defines
    `__bool_true_false_are_defined` to 1.
  * In `#if`, `true` is 1 and `false` is 0 (C23 §6.10.2p13).
  * A program using all of these runs to the same result on the TAC
    interpreter and in the simulator, in both pipelines.

c6502 has no `_Generic`, so the type of `true` is checked through
`sizeof` and the type checker's `data_type` instead.

The tests of the warning, of <stdbool.h>, and of the program on the TAC
interpreter are doctest cases in cpp/tests/test_c23_bool.cpp.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


_PROGRAM = """\
#include <stdbool.h>

enum { TWO = true + true };
static bool flags[TWO + 1] = { true, false, true };
static int seen = false;
bool global = 7;

[[c6502::return_bool_in_carry]] bool positive(int x) { return x > 0; }

bool odd(unsigned char x) { return x & 1; }

int classify(bool b) {
    switch (b) {
    case true:
        return 10;
    case false:
        return 20;
    }
    return 0;
}

int main(void) {
    bool b = 256;
    int r = 0;
    for (unsigned char i = 0; i < sizeof flags; i++)
        if (flags[i] == true)
            r += 1;
    if (positive(5) && !positive(-5))
        r += 100;
    r += classify(odd(3)) + classify(odd(4));
    r += b + global + seen + TWO + sizeof(true) + (true << 9);
    return r;
}
"""

# 2 flags + 100 + 10 + 20 + 1 + 1 + 0 + 2 + 1 + 512
_EXPECTED = 649


@needs_c6502cpp
class TestPrograms(unittest.TestCase):
    def test_asm_sim(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(_PROGRAM, optimize=optimize).run(
                    max_cycles=2_000_000,
                )
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int_signed(), _EXPECTED)


if __name__ == "__main__":
    unittest.main()
