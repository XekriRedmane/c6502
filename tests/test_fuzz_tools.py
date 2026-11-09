"""Smoke tests for the random-program fuzzers in `fuzz/`.

A few seeds of each generator must produce programs that compile and
agree on every path. This keeps the generators in step with the
compiler (a new rejection of something a generator emits shows up
here) without the minutes a real fuzz run takes."""

from __future__ import annotations

import shutil
import unittest

from fuzz import calls, exprs
from fuzz.runner import disagreements, format_result, run_all


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
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


if __name__ == "__main__":
    unittest.main()
