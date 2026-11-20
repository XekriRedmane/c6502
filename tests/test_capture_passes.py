"""Tests for tools/capture_passes.py, the recorder of the asm passes'
calls that the C++ port is checked against."""
from __future__ import annotations

import os
import re
import subprocess
import sys
import tempfile
import unittest
from dataclasses import dataclass
from pathlib import Path

import asm_ast
import compile as compile_module
import ir_dump

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), "tools"))
import capture_passes  # noqa: E402

from tests.test_ir_dump import cpp_cli  # noqa: E402


_ROOT = Path(__file__).resolve().parent.parent


class TestPassTable(unittest.TestCase):
    def test_sweep_names_match_compile(self):
        # The table lists the sweep of `_peephole_fixedpoint` under the
        # sweep's names, in its order, with the function each one calls.
        with open(_ROOT / "compile.py", encoding="utf-8") as f:
            source = f.read()
        body = source[source.index("    sweep = ("):source.index(
            "    for _ in range(_PEEPHOLE_FIXEDPOINT_CAP)")]
        entries = re.findall(
            r'\((?:F|P|_ALWAYS), "(\w+)",\s*lambda p, m: (\w+)\(', body,
        )
        self.assertEqual(entries, list(capture_passes.SWEEP_PASSES.items()))

    def test_every_function_is_in_compile(self):
        for name, attr in capture_passes.PASSES.items():
            with self.subTest(name=name):
                self.assertTrue(callable(getattr(compile_module, attr)))


@dataclass
class _Point:
    x: int
    tags: frozenset


class TestEncode(unittest.TestCase):
    def test_forms(self):
        enc = capture_passes.encode
        self.assertEqual(enc([1, "a", None, True]), [1, "a", None, True])
        self.assertEqual(enc((1, 2)), {"tuple": [1, 2]})
        self.assertEqual(enc({"b", "a"}), {"set": ["a", "b"]})
        self.assertEqual(enc({2: "x"}), {"dict": [[2, "x"]]})
        self.assertEqual(enc(range(0, 0x80)), {"range": [0, 128, 1]})
        self.assertEqual(
            enc(asm_ast.Imm(value=3)), {"asdl": "asm_ast", "dump": "Imm(3)\n"},
        )
        self.assertEqual(
            enc(_Point(1, frozenset({3}))),
            {"object": f"{__name__}._Point",
             "fields": {"x": 1, "tags": {"set": [3]}}},
        )


class TestCapture(unittest.TestCase):
    """Records split_mem_to_mem's calls on one example and checks the
    files."""

    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.TemporaryDirectory()
        out = Path(cls.tmp.name) / "captures"
        saved = compile_module.apply_split_mem_to_mem
        try:
            capture_passes.install(["split_mem_to_mem"], out)
            capture_passes._SOURCE = "examples/sfx_tone.c -S -O"
            src = _ROOT / "examples" / "sfx_tone.c"
            assert compile_module.main([
                "compile.py", str(src), "-S", "-O",
                "-o", os.path.join(cls.tmp.name, "out.s"),
            ]) == 0
        finally:
            compile_module.apply_split_mem_to_mem = saved
        cls.files = sorted((out / "split_mem_to_mem").glob("*.txt"))

    @classmethod
    def tearDownClass(cls):
        cls.tmp.cleanup()

    def test_files_hold_the_call(self):
        self.assertTrue(self.files)
        from passes.split_mem_to_mem import apply_split_mem_to_mem
        for path in self.files:
            text = path.read_bytes().decode("utf-8")
            head, _, rest = text.partition("\ninput\n")
            self.assertEqual(head.split("\n")[:3], [
                "c6502-capture 1", "pass split_mem_to_mem",
                "source examples/sfx_tone.c -S -O",
            ])
            self.assertEqual(head.split("\n")[3],
                             'args {"args": [], "kwargs": {}}')
            input_text, _, output_text = rest.partition("\noutput\n")
            prog = ir_dump.load(input_text, asm_ast)
            self.assertEqual(
                ir_dump.dump(apply_split_mem_to_mem(prog)), output_text,
            )

    @unittest.skipUnless(cpp_cli(), "the C++ CLI isn't built (cpp/README.md)")
    def test_cpp_port_matches(self):
        result = subprocess.run(
            [cpp_cli(), "check-captures", str(self.files[0].parent)],
            capture_output=True, text=True,
        )
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn(
            f"split_mem_to_mem: {len(self.files)} calls, "
            f"{len(self.files)} match, 0 differ", result.stdout,
        )


if __name__ == "__main__":
    unittest.main()
