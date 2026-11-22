"""Tests of `C6502_CPP_BACKEND=1`, which compiles a C source in the C++
port (cpp_backend.py), from the preprocessor to the `-S` text.
tools/compare_backends.py compares the two compilers on the whole
corpus, and these check the plumbing: the warnings, what the
preprocessor writes to stderr, the errors of each class, stdin, the
preprocessor's flags, and the modules Python imports."""
from __future__ import annotations

import contextlib
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
        # The simulator harness takes the finished asm program and its
        # slot symbols back. Without inlining, `small` gets the
        # carry-flag return and a `.reg` local for its register
        # parameter, and with it, `small` is copied into `main`, so the
        # harness's patch of the inliner reaches the C++ side.
        import ir_dump
        from sim.harness import compile_to_asm
        source = (
            "#include <stdint.h>\nuint8_t g;\n"
            "static bool small(uint8_t x) { return x < 10; }\n"
            "int main(void) { if (small(g)) g = 1; return g; }\n"
        )
        for optimize in (False, True):
            for inlining in (False, True):
                with self.subTest(optimize=optimize, inlining=inlining):
                    got = []
                    for cpp in (True, False):
                        with contextlib.ExitStack() as stack:
                            stack.enter_context(mock.patch.dict(os.environ))
                            os.environ.pop("C6502_CPP_BACKEND", None)
                            if cpp:
                                os.environ["C6502_CPP_BACKEND"] = "1"
                            if not inlining:
                                stack.enter_context(no_inlining())
                            prog, slots = compile_to_asm(
                                source, optimize=optimize,
                            )
                        got.append((ir_dump.dump(prog), slots))
                    self.assertEqual(got[0], got[1])
                    inlined = "Function(\"small\"" not in got[0][0]
                    self.assertEqual(inlined, optimize and inlining)

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

    def test_a_translation_error_keeps_its_message(self):
        # `c99_to_tac` runs in C++ too, and the RecursionSaveError of a
        # call marked `[[c6502::save_hardware_stack]]` that can't
        # recurse comes back with its Python type and message.
        source = (
            "void g(void) {}\n"
            "void f(void) { [[c6502::save_hardware_stack]] g(); }\n"
        )
        results = []
        for cpp in (True, False):
            env = dict(os.environ)
            env.pop("C6502_CPP_BACKEND", None)
            if cpp:
                env["C6502_CPP_BACKEND"] = "1"
            done = subprocess.run(
                [sys.executable, "compile.py", "-", "-S", "-O", "-o", "-"],
                cwd=_ROOT, env=env, input=source, capture_output=True,
                text=True,
            )
            results.append((done.returncode, done.stderr))
        self.assertEqual(results[0], results[1])
        self.assertEqual(results[0][0], 1)
        self.assertIn("can't recurse", results[0][1])

    def test_python_reads_no_program_back(self):
        # Without the IR checks, the text comes from c6502cpp and Python
        # loads no dump. With them, it loads the TAC that code
        # generation lowered and the finished asm and asm2 programs to
        # check them.
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
        self.assertEqual(counts, [0, 6])


def _run(args: list[str], cpp: bool, *, stdin: str | None = None,
         files: dict[str, str] | None = None) -> tuple:
    """Runs `compile.py <args>` in a new directory holding `files`, with
    the C++ port on or off, and returns its exit status, stdout, stderr,
    and the bytes of `out.s` there (None when it wasn't written)."""
    env = dict(os.environ)
    env.pop("C6502_CPP_BACKEND", None)
    if cpp:
        env["C6502_CPP_BACKEND"] = "1"
    with tempfile.TemporaryDirectory() as tmp:
        for name, text in (files or {}).items():
            path = Path(tmp) / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(text, encoding="utf-8", newline="\n")
        done = subprocess.run(
            [sys.executable, str(_ROOT / "compile.py"), *args],
            cwd=tmp, env=env, input=stdin, capture_output=True, text=True,
        )
        out = Path(tmp) / "out.s"
        text = out.read_bytes() if out.exists() else None
    return done.returncode, done.stdout, done.stderr, text


# A program for each class of error that `compile._source_errors` lists
# and a compile can raise, with the flags that reach it. ConstantExpressionError
# isn't among them, since every caller of the evaluator catches it and
# raises an error of its own, and the zero-page allocators' errors are
# checks of what `select_abi` already ensures.
_ERRORS = {
    "LexError": ("int x = 1'';\n", []),
    "ParserError": ("long double x;\n", []),
    "PreprocessorError": ("#pragma c6502 zp_abi\nint x;\n", []),
    "UnexpectedToken": ("int f( {\n", []),
    "UnexpectedCharacters": ("int x = 1 ` 2;\n", []),
    "IdentifierResolutionError": ("int main(void) { return y; }\n", []),
    "LabelResolutionError": ("int main(void) { goto l; }\n", []),
    "LoopLabelingError": ("int main(void) { break; }\n", []),
    "UnrollError": (
        "int main(void) { int s = 0; int n = 3;\n"
        "[[c6502::unroll]] for (int i = 0; i < n; i++) s++; return s; }\n",
        ["-O"],
    ),
    "TypeCheckError": ("int main(void) { int x; return x(); }\n", []),
    "AbiSelectionError": (
        "[[c6502::zp_abi]] int f(int n) { return n ? f(n - 1) : 0; }\n"
        "int main(void) { return f(3); }\n",
        ["-O"],
    ),
    "RegallocError": (
        "[[c6502::zp_abi, c6502::address(0xD000)]]\n"
        "void put(register unsigned char c);\n"
        "unsigned char buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};\n"
        "unsigned char n = 8;\n"
        "void send(void) {\n"
        "    register unsigned char i;\n"
        "    for (i = 0; i < n; i++) put(buf[i]);\n"
        "}\n"
        "int main(void) { send(); return 0; }\n",
        ["-O"],
    ),
    "RecursionSaveError": (
        "int g(void) { return 1; }\n"
        "int main(void) { [[c6502::save_hardware_stack]] return g(); }\n",
        [],
    ),
    "NotImplementedError": ("void f(int n) { int a[n]; }\n", []),
}


@unittest.skipUnless(cpp_cli(), "the C++ CLI isn't built (cpp/README.md)")
class TestTheFrontEnd(unittest.TestCase):
    """compile.py hands C++ the source text, and gives what Python's
    front end gives: the text, the messages, and the exit status."""

    def _same(self, args, **kw) -> tuple:
        cpp = _run(args, True, **kw)
        self.assertEqual(cpp, _run(args, False, **kw))
        return cpp

    def test_a_warning(self):
        status, _, err, text = self._same(
            ["a.c", "-S", "-o", "out.s"],
            files={"a.c": "_Bool b;\nint main(void) { return b; }\n"},
        )
        self.assertEqual(status, 0)
        self.assertIn("a.c: warning: line 1: ", err)
        self.assertIsNotNone(text)

    def test_a_warning_comes_before_the_error(self):
        status, _, err, _ = self._same(
            ["a.c", "-S", "-o", "out.s"],
            files={"a.c": "_Bool b;\nint main(void) { return c; }\n"},
        )
        self.assertEqual(status, 1)
        self.assertRegex(err, r"(?s)a\.c: warning: .*a\.c: error: ")

    def test_what_pcpp_writes_to_stderr(self):
        # pcpp reports an `#if` it can't evaluate on stderr and goes on,
        # and it leaves an include it can't find in its output, where the
        # parser rejects it after pcpp's message.
        status, _, err, text = self._same(
            ["a.c", "-S", "-o", "out.s"],
            files={"a.c": "#if 1 +\n#endif\nint main(void) { return 0; }\n"},
        )
        self.assertIn("error: Could not evaluate expression", err)
        self.assertEqual(status, 0)
        self.assertIsNotNone(text)
        status, _, err, _ = self._same(
            ["a.c", "-S", "-o", "out.s"],
            files={"a.c": '#include "missing.h"\nint main(void) { return 0; }\n'},
        )
        self.assertEqual(status, 1)
        self.assertRegex(err, r"(?s)'missing\.h' not found.*a\.c: error: ")

    def test_an_error_of_each_class(self):
        for name, (source, flags) in _ERRORS.items():
            with self.subTest(error=name):
                status, out, err, text = self._same(
                    ["a.c", "-S", "-o", "out.s", *flags],
                    files={"a.c": source},
                )
                self.assertEqual(status, 1)
                self.assertTrue(err.startswith("a.c: error: "), err)
                self.assertIsNone(text)

    def test_each_error_is_python_s_class(self):
        # A test that calls `_run_stage` checks the class of what the
        # front end raises, as the chapter harnesses do.
        import compile
        from preprocessor import preprocess
        for name, (source, flags) in _ERRORS.items():
            with self.subTest(error=name):
                raised = []
                for cpp in (False, True):
                    with mock.patch.dict(os.environ):
                        os.environ.pop("C6502_CPP_BACKEND", None)
                        if cpp:
                            os.environ["C6502_CPP_BACKEND"] = "1"
                        with self.assertRaises(Exception) as cm:
                            compile._run_stage(
                                "codegen", preprocess(source, []),
                                optimize="-O" in flags,
                            )
                    raised.append(cm.exception)
                python, cpp = raised
                self.assertEqual(type(python).__name__, name)
                self.assertIsInstance(cpp, type(python))
                self.assertEqual(str(cpp), str(python))

    def test_stdin(self):
        status, out, err, _ = self._same(
            ["-", "-S", "-O", "-o", "-"],
            stdin="_Bool b;\nint main(void) { return b + 1; }\n",
        )
        self.assertEqual(status, 0)
        self.assertIn("<stdin>: warning: ", err)
        self.assertIn("RTS", out)

    def test_include_path_and_define(self):
        files = {
            "src/a.c": '#include "h.h"\nint main(void) { return V + W; }\n',
            "inc/h.h": "#define V 7\n",
        }
        status, _, _, text = self._same(
            ["src/a.c", "-S", "-O", "-o", "out.s", "-I", "inc", "-DW=2"],
            files=files,
        )
        self.assertEqual(status, 0)
        self.assertIn(b"#$09", text)

    def test_compile_imports_no_front_end(self):
        # Run as a script with the variable set, compile.py stops before
        # it imports the Python pipeline, which the front end's modules
        # are part of. The IR checks are off, since passes/ir_check.py
        # reads helpers of the asm optimizer, which import the type
        # checker.
        code = (
            "import runpy, sys\n"
            "sys.argv = ['compile.py', 'examples/sfx_tone.c', '-S', '-O',"
            " '-o', sys.argv[1]]\n"
            "try:\n"
            "    runpy.run_path('compile.py', run_name='__main__')\n"
            "except SystemExit as e:\n"
            "    assert not e.code, e.code\n"
            "front = ('parser', 'lexer', 'lark', 'pcpp', 'preprocessor',\n"
            "         'passes.type_checking', 'passes.identifier_resolution',\n"
            "         'passes.string_lifting', 'passes.label_resolution',\n"
            "         'passes.loop_labeling', 'passes.optimization_ast',\n"
            "         'tac_to_asm', 'c99_to_tac')\n"
            "print(sorted(m for m in sys.modules\n"
            "             if m.split('.')[0] in front or m in front\n"
            "             or m.startswith(front)))\n"
        )
        env = dict(os.environ)
        env["C6502_CPP_BACKEND"] = "1"
        env.pop("C6502_CHECK_IR", None)
        with tempfile.TemporaryDirectory() as tmp:
            done = subprocess.run(
                [sys.executable, "-c", code, os.path.join(tmp, "out.s")],
                cwd=_ROOT, env=env, check=True, capture_output=True,
                text=True,
            )
        self.assertEqual(done.stdout.strip(), "[]")


if __name__ == "__main__":
    unittest.main()
