"""Tests for tools/rtlib_table.py, the C++ port's copy of the runtime
helpers of rtlib/helpers.py (cpp/generated/rtlib_helpers.cpp), and for
the HARGS footprints the C++ side measures on it."""
from __future__ import annotations

import subprocess
import sys
import unittest
from pathlib import Path

_ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(_ROOT / "tools"))

import asm_ast  # noqa: E402
import rtlib_table  # noqa: E402
from tests.test_ir_dump import cpp_cli  # noqa: E402


class TestCheckedInTable(unittest.TestCase):
    def test_regenerating_gives_the_checked_in_file(self):
        # After editing rtlib/helpers.py, regenerate with
        # `uv run python tools/rtlib_table.py`.
        with open(rtlib_table.OUTPUT, "rb") as f:
            checked_in = f.read().decode("utf-8")
        self.assertEqual(rtlib_table.generate(), checked_in)

    def test_the_file_has_lf_line_endings(self):
        with open(rtlib_table.OUTPUT, "rb") as f:
            self.assertNotIn(b"\r", f.read())


@unittest.skipUnless(cpp_cli() is not None, "c6502cpp isn't built")
class TestCppFootprints(unittest.TestCase):
    def test_every_helper_matches_python(self):
        from passes.runtime_layout import helper_footprints
        done = subprocess.run([str(cpp_cli()), "helper-footprints"],
                              check=True, capture_output=True, text=True)
        from rtlib.helpers import all_helper_functions
        # The C++ side lists the helpers in the table's order, and
        # Python's dict holds a callee before the first helper that
        # calls it.
        fp = helper_footprints()
        expected = [f"{fn.name} {fp[fn.name]}" for fn in all_helper_functions()]
        self.assertEqual(len(fp), len(expected))
        self.assertEqual(done.stdout.splitlines(), expected)


class TestCycleCheck(unittest.TestCase):
    def _fn(self, name, *instrs):
        return asm_ast.Function(name=name, is_global=True, params=[],
                                instructions=list(instrs))

    def test_refuses_helpers_that_call_each_other_in_a_cycle(self):
        # Python's `helper_footprints` follows the calls in a set's
        # order, which could change a footprint only through a cycle.
        fns = [
            self._fn("f", asm_ast.Call(name="g")),
            self._fn("g", asm_ast.Jump(target="f")),
        ]
        with self.assertRaisesRegex(ValueError, "f -> g -> f"):
            rtlib_table._check_acyclic(fns)

    def test_takes_a_helper_called_from_two_others(self):
        fns = [
            self._fn("f", asm_ast.Call(name="h")),
            self._fn("g", asm_ast.Call(name="h"), asm_ast.Call(name="fadd")),
            self._fn("h"),
        ]
        rtlib_table._check_acyclic(fns)


if __name__ == "__main__":
    unittest.main()
