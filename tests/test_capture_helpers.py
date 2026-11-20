"""Tests for tools/capture_helpers.py, the recorder of the shared asm
helpers' values that the C++ port of the helpers is checked against."""
from __future__ import annotations

import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

import asm_ast
import compile as compile_module
import ir_dump

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), "tools"))
import capture_helpers  # noqa: E402
import capture_passes  # noqa: E402

from tests.test_ir_dump import cpp_cli  # noqa: E402


_ROOT = Path(__file__).resolve().parent.parent


class TestCaptureHelpers(unittest.TestCase):
    """Records the helpers' values on one example and checks the C++
    port against them."""

    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.TemporaryDirectory()
        cls.out = Path(cls.tmp.name) / "helpers"
        names = [*capture_passes.PASSES.values(), "_peephole_fixedpoint",
                 "select_abi"]
        saved = {n: getattr(compile_module, n) for n in names}
        try:
            capture_helpers.install(cls.out)
            capture_helpers._SOURCE = "examples/sfx_tone.c -S -O"
            src = _ROOT / "examples" / "sfx_tone.c"
            assert compile_module.main([
                "compile.py", str(src), "-S", "-O",
                "-o", os.path.join(cls.tmp.name, "out.s"),
            ]) == 0
        finally:
            for n, f in saved.items():
                setattr(compile_module, n, f)
            capture_helpers._SLOTS = None
            capture_helpers._DONE.clear()

    @classmethod
    def tearDownClass(cls):
        cls.tmp.cleanup()

    def test_records_are_written(self):
        for kind in ("fn", "prog", "tac"):
            with self.subTest(kind=kind):
                files = list((self.out / kind).glob("*.txt"))
                self.assertTrue(files)
                head = files[0].read_bytes().decode("utf-8").split("\n")
                self.assertEqual(head[0], "c6502-helpers 1")
                self.assertEqual(head[1], "source examples/sfx_tone.c -S -O")

    def test_function_record_holds_the_walks(self):
        text = next((self.out / "fn").glob("*.txt")).read_text("utf-8")
        values = dict(
            line.split(" ", 1)
            for line in text.partition("\nvalues\n")[2].splitlines()
        )
        dump = text.partition("\nfunction\n")[2].partition("\nvalues\n")[0]
        n = len(ir_dump.load(dump + "\n", asm_ast).instructions)
        # One value per index, the one past the end included.
        self.assertEqual(len(values["a_dead_at"]), n + 1)
        self.assertEqual(len(values["reads_a"]), n)

    @unittest.skipUnless(cpp_cli(), "the C++ CLI isn't built (cpp/README.md)")
    def test_cpp_port_matches(self):
        result = subprocess.run(
            [cpp_cli(), "check-helpers", str(self.out)],
            capture_output=True, text=True,
        )
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertNotIn("MISMATCH", result.stdout)
        self.assertIn("may_alias:", result.stdout)


if __name__ == "__main__":
    unittest.main()
