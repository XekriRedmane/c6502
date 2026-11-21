"""Tests of `C6502_CPP_BACKEND=1`, which runs code generation, from the
TAC to the `-S` text, in the C++ port (cpp_backend.py).
tools/compare_backends.py compares the two compilers on the whole
corpus, and these check the plumbing."""
from __future__ import annotations

import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tests.no_inlining import no_inlining
from tests.test_ir_dump import cpp_cli
from tools.capture_passes import SWEEP_PASSES

_ROOT = Path(__file__).resolve().parent.parent


def _compile(args: list[str], cpp: bool, stage: str = "-S") -> bytes:
    """Returns the `-S` text (or with `stage` "-c", the object) of
    `compile.py <args>` with the C++ back end on or off."""
    env = dict(os.environ)
    env.pop("C6502_CPP_BACKEND", None)
    if cpp:
        env["C6502_CPP_BACKEND"] = "1"
    with tempfile.TemporaryDirectory() as tmp:
        out = os.path.join(tmp, "out.s" if stage == "-S" else "out.o")
        subprocess.run(
            [sys.executable, "compile.py", *args, stage, "-o", out],
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

    def test_the_harness_gets_python_s_program(self):
        # The simulator harness takes the finished asm program back, and
        # the symbol table gets what the Python passes enter in it: the
        # carry-flag return chosen for `small`, and the `.reg` local of
        # its register parameter.
        import ir_dump
        from sim.harness import compile_to_asm
        from tools.capture_passes import encode
        source = (
            "#include <stdint.h>\nuint8_t g;\n"
            "static bool small(uint8_t x) { return x < 10; }\n"
            "int main(void) { if (small(g)) g = 1; return g; }\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = []
                for cpp in (True, False):
                    with mock.patch.dict(os.environ):
                        os.environ.pop("C6502_CPP_BACKEND", None)
                        if cpp:
                            os.environ["C6502_CPP_BACKEND"] = "1"
                        with no_inlining():
                            prog, symbols, _, slots = compile_to_asm(
                                source, optimize=optimize,
                            )
                    got.append((ir_dump.dump(prog), slots, encode(symbols)))
                self.assertEqual(got[0], got[1])

    def test_objects_are_python_s(self):
        # Without -O, only the text of `-c` has the link metadata block, whose
        # items, zero-page records, and HARGS need come from C++. (The
        # `-S -O` text above always has the block.)
        game = _ROOT / "examples" / "lode_runner"
        args = [str(game / "guards.c"), "-I", str(game)]
        self.assertEqual(
            _compile(args, cpp=True, stage="-c"),
            _compile(args, cpp=False, stage="-c"),
        )

    def test_a_frame_error_keeps_its_message(self):
        # The FrameSizeError of `asm_to_asm2` comes back from c6502cpp
        # with its Python type, so compile.py reports it as a one-line
        # error, as it does without the variable.
        source = (
            "unsigned char f(unsigned long long a, unsigned long long b, "
            "unsigned long long c, unsigned char d) "
            "{ unsigned char t[225]; t[0] = d; return t[0]; }"
        )
        results = []
        for cpp in (True, False):
            env = dict(os.environ)
            env.pop("C6502_CPP_BACKEND", None)
            if cpp:
                env["C6502_CPP_BACKEND"] = "1"
            done = subprocess.run(
                [sys.executable, "compile.py", "-", "-S", "-o", "-"],
                cwd=_ROOT, env=env, input=source, capture_output=True,
                text=True,
            )
            results.append((done.returncode, done.stderr))
        self.assertEqual(results[0], results[1])
        self.assertEqual(results[0][0], 1)
        self.assertIn("frame offset 261 out of range", results[0][1])

    def test_python_reads_no_program_back(self):
        # Without the IR checks, the text comes from c6502cpp and Python
        # loads no dump. With them, it loads the finished asm and asm2
        # programs to check them.
        code = (
            "import sys, compile, ir_dump\n"
            "from passes import ir_check\n"
            "loads = []\n"
            "real = ir_dump.load\n"
            "ir_dump.load = lambda *a: loads.append(1) or real(*a)\n"
            "ir_check.enable(sys.argv[2] == '1')\n"
            "for mode in ([], ['-O']):\n"
            "    compile.main(['compile.py', 'examples/sfx_tone.c', '-S',"
            " '-o', sys.argv[1], *mode])\n"
            "print(len(loads))\n"
        )
        env = dict(os.environ)
        env["C6502_CPP_BACKEND"] = "1"
        env.pop("C6502_CHECK_IR", None)
        counts = []
        for check in ("0", "1"):
            with tempfile.TemporaryDirectory() as tmp:
                done = subprocess.run(
                    [sys.executable, "-c", code, os.path.join(tmp, "out.s"),
                     check],
                    cwd=_ROOT, env=env, check=True, capture_output=True,
                    text=True,
                )
            counts.append(int(done.stdout))
        self.assertEqual(counts, [0, 4])


if __name__ == "__main__":
    unittest.main()
