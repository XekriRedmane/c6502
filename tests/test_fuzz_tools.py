"""Smoke tests for the random-program fuzzers in `fuzz/`.

A few seeds of each generator must produce programs that compile and
agree on every path. This keeps the generators in step with the
compiler (a new rejection of something a generator emits shows up
here) without the minutes a real fuzz run takes."""

from __future__ import annotations

import unittest

from fuzz import calls, exprs
from fuzz.runner import disagreements, format_result, run_all
from tests.cpp_build import needs_c6502cpp


@needs_c6502cpp
class TestFuzzGenerators(unittest.TestCase):

    def _check(self, make_program, seeds):
        for seed in seeds:
            with self.subTest(seed=seed):
                src, expected = make_program(seed)
                res = run_all(src)
                self.assertEqual(
                    disagreements(res, expected), (),
                    msg=f"{format_result(res)}\n{src}",
                )

    def test_exprs(self):
        self._check(exprs.make_program, range(3))

    def test_calls(self):
        # Seed 44 has every feature: the recursive function, the
        # pointer-called function, `register`, pointer parameters,
        # and a call in a loop. Seed 1 has `register` only.
        self._check(calls.make_program, (1, 44))

    def test_seeds_are_deterministic(self):
        self.assertEqual(exprs.make_program(7), exprs.make_program(7))
        self.assertEqual(calls.make_program(7), calls.make_program(7))


@needs_c6502cpp
class TestReducerRejectsFallingOff(unittest.TestCase):
    """The reducer never keeps a candidate that reaches the end of a
    function returning a value, whose value C99 6.9.1p12 leaves
    undefined. Reducing `fuzz.calls` seed 335 emptied three functions
    whose results were still used, which made the reduced program's
    disagreement meaningless."""

    def test_falls_off_end(self):
        from fuzz.reduce import falls_off_end
        cases = {
            "int f(int x) { return x + 1; }\n"
            "int main(void) { return f(2); }\n": False,
            "int f(int x) { x = x + 1; }\n"
            "int main(void) { return f(2); }\n": True,
            # Only the path the run takes counts.
            "int f(int x) { if (x) return 1; }\n"
            "int main(void) { return f(2); }\n": False,
            "int f(int x) { if (x) return 1; }\n"
            "int main(void) { return f(0); }\n": True,
            "int g; void f(void) { g = 1; }\n"
            "int main(void) { f(); return g; }\n": False,
            # C99 5.1.2.2.3 defines `main` falling off as returning 0.
            "int main(void) { }\n": False,
        }
        for src, expected in cases.items():
            with self.subTest(src=src):
                self.assertEqual(falls_off_end(src), expected)

    def test_predicate_rejects_falling_off(self):
        from fuzz.reduce import make_predicate
        # A program that fails on every path, so that any candidate
        # failing the same way would be kept.
        src = "int f(int x) { return x; }\nint main(void) { return f(3); }\n// expect: 4\n"
        still_fails = make_predicate(src, tac_opt=False)
        self.assertTrue(still_fails(src))
        self.assertFalse(still_fails(
            "int f(int x) { x = 3; }\nint main(void) { return f(3); }\n// expect: 4\n",
        ))


if __name__ == "__main__":
    unittest.main()
