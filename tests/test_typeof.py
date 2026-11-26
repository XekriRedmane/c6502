"""Tests for C23's typeof operators, `typeof` and `typeof_unqual`
(§6.7.3.6), which the parser puts in place as the type they name.
Most cases are the standard's own examples."""

from __future__ import annotations

import shutil
import unittest

from sim.harness import build_sim
from tests.test_sim_chapters import _run_program as _run_tac


_PROGRAM = """\
int count;
int bump(void) { return ++count; }

int main(void) {
    int x = 0;
    typeof(x++) y = 5;
    typeof(bump()) z = 7;
    if (x != 0 || count != 0) return 1;
    if (y + z != 12) return 2;
    float val = 6.0f;
    int argc = 1;
    if ((typeof(typeof_unqual(typeof(argc))))val != 6) return 3;
    typeof(char[4]) four = { 1, 2, 3, 4 };
    if (sizeof four != 4 || four[3] != 4) return 4;
    return 0;
}
"""


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestPrograms(unittest.TestCase):
    """The operand isn't evaluated (C23 §6.7.3.6p4), so `x++` and the
    call of `bump` don't happen. EXAMPLE 4's nested typeof is a cast
    to `int`."""

    def test_tac_sim(self) -> None:
        self.assertEqual(_run_tac(_PROGRAM), 0)

    def test_asm_sim(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(_PROGRAM, optimize=optimize).run(
                    max_cycles=2_000_000,
                )
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int_signed(), 0)


if __name__ == "__main__":
    unittest.main()
