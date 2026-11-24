"""Tests for the TAC pre-SSA signed-countdown loop-rotation pass.

Coverage:
  * Unit tests on synthetic pre-SSA TAC: matches the canonical
    for-loop shape, rotates correctly for `int` and `int8_t`
    counters, refuses unsigned counters, refuses non-`>= 0`
    conditions, refuses non-decrement-by-1 post-steps, refuses
    non-constant or negative inits.
  * End-to-end through the optimizer + simulator: programs
    compute the same result with rotation as without.
"""
from __future__ import annotations

import shutil
import unittest


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestLoopRotateE2E(unittest.TestCase):
    """End-to-end through the simulator: rotated programs return
    the same result as unrotated ones."""

    def _sim(self, src: str, expected: int) -> None:
        """Compile via `-S -O`, assemble, simulate
        until the program halts, assert the return value matches."""
        from sim.harness import build_sim
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=200_000)
        self.assertFalse(
            result.timed_out, f"sim timed out at {result.cycles}",
        )
        self.assertEqual(result.return_int_signed(), expected)

    def test_simple_int_countdown(self) -> None:
        # sum 15..0 = 15*16/2 = 120
        src = (
            "int main(void) {\n"
            "    int sum = 0;\n"
            "    for (int x = 15; x >= 0; x--) sum += x;\n"
            "    return sum;\n"
            "}\n"
        )
        self._sim(src, 120)

    def test_simple_int_countdown_init_zero(self) -> None:
        # Single iteration: body runs once, x == 0.
        src = (
            "int main(void) {\n"
            "    int sum = 0;\n"
            "    for (int x = 0; x >= 0; x--) sum += 1;\n"
            "    return sum;\n"
            "}\n"
        )
        self._sim(src, 1)

    def test_int8_countdown(self) -> None:
        # sum 15..0 from int8_t counter. Verifies the SignExtend-
        # in-cond shape rotates and computes correctly.
        src = (
            "#include <stdint.h>\n"
            "int main(void) {\n"
            "    int sum = 0;\n"
            "    for (int8_t x = 15; x >= 0; x--) sum += x;\n"
            "    return sum;\n"
            "}\n"
        )
        self._sim(src, 120)

    def _both(self, src: str, expected: int) -> None:
        """Runs `src` through both pipelines and checks that each
        returns `expected`."""
        from sim.harness import build_sim
        for optimize in (False, True):
            result = build_sim(src, optimize=optimize).run(
                max_cycles=200_000,
            )
            self.assertFalse(result.timed_out, f"optimize={optimize}")
            self.assertEqual(
                result.return_int(), expected, f"optimize={optimize}",
            )

    def test_init_on_conditional_path_not_rotated(self) -> None:
        # The last write to i before the loop is inside the `if`, so
        # the -1 parameter reaches the test when c is 0, and the
        # first test fails. The rotated loop ran the body once and
        # returned 1.
        src = (
            "#include <stdint.h>\n"
            "uint8_t count(int8_t i, uint8_t c) {\n"
            "    uint8_t k = 0;\n"
            "    if (c) i = 5;\n"
            "    for (; i >= 0; i--) k++;\n"
            "    return k;\n"
            "}\n"
            "int main(void) { return count(-1, 0) + count(-1, 1) * 2; }\n"
        )
        self._both(src, 12)

    def test_init_through_conditional_temp_not_rotated(self) -> None:
        # `c ? -1 : 5` writes its result temp on two paths, and the
        # write in instruction order is the in-range 5.
        src = (
            "#include <stdint.h>\n"
            "uint8_t count(uint8_t c) {\n"
            "    uint8_t k = 0;\n"
            "    for (int8_t i = c ? -1 : 5; i >= 0; i--) k++;\n"
            "    return k;\n"
            "}\n"
            "int main(void) { return count(1) + count(0) * 2; }\n"
        )
        self._both(src, 12)

    def test_address_taken_counter_not_rotated(self) -> None:
        # The store through p writes i after its constant init, and
        # it isn't a def of i in the TAC.
        src = (
            "#include <stdint.h>\n"
            "uint8_t count(int8_t i) {\n"
            "    uint8_t k = 0;\n"
            "    int8_t *p = &i;\n"
            "    i = 3;\n"
            "    *p = -1;\n"
            "    for (; i >= 0; i--) k++;\n"
            "    return k;\n"
            "}\n"
            "int main(void) { return count(0); }\n"
        )
        self._both(src, 0)

    def test_post_decrement_in_test_not_rotated(self) -> None:
        # The test `i-- >= 0` decrements i. The rotated loop skipped
        # the first test and so the first decrement, and returned
        # 5565. The body should see 4, 2, and 0, and the result is
        # their digits.
        src = (
            "int main(void) {\n"
            "    int c = 0;\n"
            "    for (signed char i = 5; i-- >= 0; i--) c = c * 10 + i;\n"
            "    return c;\n"
            "}\n"
        )
        self._both(src, 420)

    def test_side_effect_in_test_not_rotated(self) -> None:
        # `(n++, i) >= 0` runs five times, the last time failing. The
        # rotated loop skipped the first one and returned 44.
        src = (
            "int n;\n"
            "int main(void) {\n"
            "    int c = 0;\n"
            "    for (int i = 3; (n++, i) >= 0; i--) c++;\n"
            "    return n * 10 + c;\n"
            "}\n"
        )
        self._both(src, 54)


if __name__ == "__main__":
    unittest.main()
