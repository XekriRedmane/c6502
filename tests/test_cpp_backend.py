"""Tests of `C6502_CPP_BACKEND=1`, which runs the asm back end's region
in the C++ port (cpp_backend.py). tools/compare_backends.py compares
the two back ends on the whole corpus; these check the plumbing."""
from __future__ import annotations

import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

from tests.test_ir_dump import cpp_cli
from tools.capture_passes import SWEEP_PASSES

_ROOT = Path(__file__).resolve().parent.parent


def _compile(args: list[str], cpp: bool) -> bytes:
    """Returns the `-S` text of `compile.py <args>` with the C++ back end
    on or off."""
    env = dict(os.environ)
    env.pop("C6502_CPP_BACKEND", None)
    if cpp:
        env["C6502_CPP_BACKEND"] = "1"
    with tempfile.TemporaryDirectory() as tmp:
        out = os.path.join(tmp, "out.s")
        subprocess.run(
            [sys.executable, "compile.py", *args, "-S", "-o", out],
            cwd=_ROOT, env=env, check=True, capture_output=True,
        )
        with open(out, "rb") as f:
            return f.read()


class TestWithoutTheVariable(unittest.TestCase):
    def test_compile_imports_nothing_new(self):
        env = dict(os.environ)
        env.pop("C6502_CPP_BACKEND", None)
        code = (
            "import sys, compile\n"
            "compile.main(['compile.py', 'examples/sfx_tone.c', '-S', '-O',"
            " '-o', sys.argv[1]])\n"
            "print('cpp_backend' in sys.modules,"
            " 'tools.capture_passes' in sys.modules)\n"
        )
        with tempfile.TemporaryDirectory() as tmp:
            done = subprocess.run(
                [sys.executable, "-c", code, os.path.join(tmp, "out.s")],
                cwd=_ROOT, env=env, check=True, capture_output=True,
                text=True,
            )
        self.assertEqual(done.stdout.split(), ["False", "False"])


@unittest.skipUnless(cpp_cli(), "the C++ CLI isn't built (cpp/README.md)")
class TestWithTheVariable(unittest.TestCase):
    def test_the_sweep_is_compile_pys(self):
        # tests/test_capture_passes.py checks SWEEP_PASSES against
        # compile.py's `_peephole_fixedpoint`.
        done = subprocess.run(
            [cpp_cli(), "sweep-passes"], check=True, capture_output=True,
            text=True,
        )
        self.assertEqual(done.stdout.split(), list(SWEEP_PASSES))

    def test_the_output_is_python_s(self):
        game = _ROOT / "examples" / "lode_runner"
        for args in (["examples/sfx_tone.c"],
                     [str(game / "guards.c"), "-I", str(game)]):
            for mode in ([], ["-O"]):
                with self.subTest(args=args[0], mode=mode):
                    self.assertEqual(
                        _compile(args + mode, cpp=True),
                        _compile(args + mode, cpp=False),
                    )


if __name__ == "__main__":
    unittest.main()
