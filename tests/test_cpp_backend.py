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


def _run_files(args: list[str], cpp: bool, files: dict[str, str | bytes],
               *, stdin: str | None = None) -> tuple:
    """Runs `compile.py <args>` as a script in a new directory holding
    `files` (text or bytes), with the C++ port on or off, and returns
    its exit status, stdout, stderr, and every file the directory holds
    afterward, by name. A traceback's lines name compile.py's source
    lines, so of a traceback only its last line is kept."""
    env = dict(os.environ)
    env.pop("C6502_CPP_BACKEND", None)
    if cpp:
        env["C6502_CPP_BACKEND"] = "1"
    with tempfile.TemporaryDirectory() as tmp:
        for name, data in files.items():
            path = Path(tmp) / name
            path.parent.mkdir(parents=True, exist_ok=True)
            if isinstance(data, bytes):
                path.write_bytes(data)
            else:
                path.write_text(data, encoding="utf-8", newline="\n")
        done = subprocess.run(
            [sys.executable, str(_ROOT / "compile.py"), *args],
            cwd=tmp, env=env, input=stdin, capture_output=True, text=True,
        )
        left = {
            p.relative_to(tmp).as_posix(): p.read_bytes()
            for p in sorted(Path(tmp).rglob("*")) if p.is_file()
        }
    err = done.stderr
    if "Traceback (most recent call last)" in err:
        err = err.rstrip("\n").rsplit("\n", 1)[-1]
    return done.returncode, done.stdout, err, left


# A program in three units, one of each kind of input to a link: `main`
# in C, `twice` in an assembly file written by `-S -O`, and `add` in an
# object written by `-c -O`. `port` is a name no unit defines, which
# `--defsym` gives.
_MAIN = (
    "extern volatile unsigned char port;\n"
    "unsigned char twice(unsigned char x);\n"
    "unsigned char add(unsigned char a, unsigned char b);\n"
    "_Bool flag;\n"
    "int main(void) { port = add(twice(3), 4); return flag; }\n"
)
_TWICE = "unsigned char twice(unsigned char x) { return x + x; }\n"
_ADD = "unsigned char add(unsigned char a, unsigned char b) { return a * b; }\n"
# The inputs of a link of the three units.
_LINKED = ["main.c", "twice.asm", "add.o", "--defsym", "port=0xC000"]
_ROM_SCRIPT = (
    "MEMORY { ROM : ORIGIN = 0x8000, LENGTH = 0x1000\n"
    "         RAM : ORIGIN = 0x0300, LENGTH = 0x0400 }\n"
    "SECTIONS { .text : { *(.text) } > ROM\n"
    "           .rodata : { *(.rodata) } > ROM\n"
    "           .data : { *(.data) } > RAM AT> ROM\n"
    "           .bss (NOLOAD) : { *(.bss) } > RAM }\n"
)


def _slot_metadata_asm(kind: str) -> str:
    """An assembly file whose link metadata gives one function 66,000
    parameter bytes (`kind` "slot") or local bytes ("local"), more than
    the spill region from $0200 holds, so that the link's allocator
    fails. No compile writes such a block, so it is written by hand."""
    n = 66000
    if kind == "slot":
        names = ",".join(f"__zpabi_f__p{k}" for k in range(n))
        defs = f"; def f params={names} locals= indirect=false in_cycle=false\n"
        body = " RTS\n"
    else:
        names = ",".join(f"__local_f__{k}" for k in range(n))
        defs = f"; def f params= locals={names} indirect=false in_cycle=false\n"
        # A function that takes a local's address keeps its whole pool.
        body = " LDA #<__local_f__0\n RTS\n"
    return (
        "; @zp-link-meta-begin\n" + defs
        + "; def main params= locals= indirect=false in_cycle=false\n"
        "; item main section=.text local=false zero=false\n"
        "; item f section=.text local=false zero=false\n"
        "; call main -> f\n"
        "; runtime hargs_size=32 hargs_need=0\n"
        "; @zp-link-meta-end\n"
        "main:\n JSR f\n RTS\nf:\n" + body
    )


@unittest.skipUnless(cpp_cli(), "the C++ CLI isn't built (cpp/README.md)")
class TestObjectsAndLinks(unittest.TestCase):
    """`-c` and a link hand every input to C++ in one request
    (`cpp_backend.build`), and give what the Python path gives: the
    messages, the exit status, and the files written and not written."""

    @classmethod
    def setUpClass(cls) -> None:
        cls.files: dict[str, str | bytes] = {
            "main.c": _MAIN, "twice.c": _TWICE, "add.c": _ADD,
            "rom.ld": _ROM_SCRIPT,
        }
        with tempfile.TemporaryDirectory() as tmp:
            for name in ("twice.c", "add.c"):
                Path(tmp, name).write_text(cls.files[name], encoding="utf-8")
            env = dict(os.environ)
            env.pop("C6502_CPP_BACKEND", None)
            for args in (["twice.c", "-S", "-O", "-o", "twice.asm"],
                         ["add.c", "-c", "-O", "-o", "add.o"]):
                subprocess.run(
                    [sys.executable, str(_ROOT / "compile.py"), *args],
                    cwd=tmp, env=env, check=True, capture_output=True,
                )
            cls.files["twice.asm"] = Path(tmp, "twice.asm").read_text(encoding="utf-8")
            cls.files["add.o"] = Path(tmp, "add.o").read_bytes()

    def _same(self, args: list[str], extra: dict | None = None, **kw) -> tuple:
        files = {**self.files, **(extra or {})}
        cpp = _run_files(args, True, files, **kw)
        self.assertEqual(cpp, _run_files(args, False, files, **kw))
        return cpp

    def test_objects_of_c_and_assembly(self):
        for opt in ([], ["-O"]):
            with self.subTest(opt=opt):
                status, _, err, left = self._same(
                    ["-c", *opt, "main.c", "twice.asm"])
                self.assertEqual(status, 0)
                self.assertIn("main.c: warning: line 4: ", err)
                self.assertIn("main.o", left)
                self.assertIn("twice.o", left)

    def test_object_of_stdin(self):
        status, _, err, left = self._same(
            ["-c", "-", "-O", "-o", "s.o"], stdin=_MAIN)
        self.assertEqual(status, 0)
        self.assertIn("<stdin>: warning: ", err)
        self.assertIn("s.o", left)

    def test_link_of_each_kind_of_input(self):
        for extra in (["-T", "rom.ld"], ["-Ttext=0x1000"], ["--origin", "$2000"]):
            with self.subTest(extra=extra):
                status, _, _, left = self._same(
                    ["-O", "main.c", "twice.asm", "add.o", *extra,
                     "--defsym", "port=0xC000", "-o", "prog"])
                self.assertEqual(status, 0)
                self.assertIn("prog", left)
        # Without -o the executable is a.out, and without -O the C
        # input is compiled without the optimizer.
        status, _, _, left = self._same(
            ["main.c", "twice.asm", "add.o", "--defsym=port=49152"])
        self.assertEqual(status, 0)
        self.assertIn("a.out", left)

    def test_error_in_the_second_of_several_c_inputs(self):
        bad = {"bad.c": "int f(void) { return y; }\n"}
        status, _, err, left = self._same(
            ["-c", "main.c", "bad.c", "twice.c"], bad)
        self.assertEqual(status, 1)
        self.assertRegex(err, r"(?s)main\.c: warning: .*bad\.c: error: ")
        self.assertIn("main.o", left)
        self.assertNotIn("bad.o", left)
        self.assertNotIn("twice.o", left)
        status, _, err, left = self._same(
            ["main.c", "bad.c", "twice.c", "-o", "prog"], bad)
        self.assertEqual(status, 1)
        self.assertRegex(err, r"(?s)main\.c: warning: .*bad\.c: error: ")
        self.assertNotIn("prog", left)

    def test_an_error_of_each_class(self):
        cases = {
            "AssemblerError": (["-c", "bad.asm"], {"bad.asm": "f:\n BNE 5\n"}),
            "AssemblerError in a link": (
                ["main.c", "bad.asm"], {"bad.asm": "f:\n BNE 5\n"}),
            "ElfError": (["main.c", "bad.o"], {"bad.o": b"not an ELF file"}),
            "LinkError": (["main.c", "twice.asm", "add.o"], {}),
            "ScriptError": ([*_LINKED, "-T", "bad.ld"], {"bad.ld": "SECTIONS {\n"}),
            "LayoutError": (
                [*_LINKED, "-T", "nodata.ld"],
                {"nodata.ld": "SECTIONS { .data : { *(.data) } }\n"}),
            "ZpSlotAllocationError": (
                ["slot.asm", "-o", "prog"], {"slot.asm": _slot_metadata_asm("slot")}),
            "ZpLocalAllocationError": (
                ["local.asm", "-o", "prog"],
                {"local.asm": _slot_metadata_asm("local")}),
            "a missing input": (["main.c", "missing.o"], {}),
            "a missing C input after a good one": (["-c", "main.c", "missing.c"], {}),
            "an input that isn't UTF-8": (["-c", "x.asm"], {"x.asm": b"; \xff\n"}),
            "an object given to -c": (["-c", "main.c", "add.o"], {}),
            "a missing script": ([*_LINKED, "-T", "missing.ld"], {}),
        }
        for name, (args, extra) in cases.items():
            with self.subTest(error=name):
                status, _, err, _ = self._same(args, extra)
                self.assertNotEqual(status, 0, err)
                self.assertTrue(
                    err.startswith(("compile.py: ", "main.c: warning")) or ": error: " in err,
                    err)

    def test_struct_error_leaves_an_empty_file(self):
        # `elf.write` packs an addend in 32 bits, and raises struct.error
        # after compile.py has opened the object's file.
        big = {"big.asm": "f:\n LDA g+$100000000\n RTS\n"}
        status, _, err, left = self._same(["-c", "big.asm"], big)
        self.assertEqual(status, 1)
        self.assertEqual(err, "struct.error: argument out of range")
        self.assertEqual(left["big.o"], b"")

    def test_an_executable_that_cant_be_written(self):
        status, _, err, _ = self._same([*_LINKED, "-o", "nodir/prog"])
        self.assertEqual(status, 1)
        self.assertEqual(
            err, "FileNotFoundError: [Errno 2] No such file or directory: 'nodir/prog'")

    def test_compile_imports_no_binutils_or_linker(self):
        # Run as a script with the variable set, `-c` and a link import
        # none of the modules a Python link would. The modules are
        # found by importing them in another process and keeping the
        # ones from this repository, and numpy.
        code = (
            "import sys\n"
            "base = set(sys.modules)\n"
            "import passes.linker, binutils.assembler, binutils.elf\n"
            "import passes.runtime_library, rtlib.helpers\n"
            "print('\\n'.join(sorted(set(sys.modules) - base)))\n"
        )
        env = dict(os.environ)
        env.pop("C6502_CHECK_IR", None)
        env.pop("C6502_CPP_BACKEND", None)
        done = subprocess.run(
            [sys.executable, "-c", code], cwd=_ROOT, env=env, check=True,
            capture_output=True, text=True,
        )
        linker = set()
        for name in done.stdout.split():
            top = name.split(".")[0]
            if top == "numpy" or (_ROOT / top).exists() or (_ROOT / f"{top}.py").exists():
                linker.add(name)
        self.assertIn("passes.type_checking", linker)
        self.assertIn("numpy", linker)
        run = (
            "import runpy, sys\n"
            "sys.argv = sys.argv[1:]\n"
            "try:\n"
            "    runpy.run_path(sys.argv[0], run_name='__main__')\n"
            "except SystemExit as e:\n"
            "    assert not e.code, e.code\n"
            "print('\\n'.join(sorted(sys.modules)))\n"
        )
        env["C6502_CPP_BACKEND"] = "1"
        with tempfile.TemporaryDirectory() as tmp:
            for name in ("main.c", "twice.asm", "add.o", "rom.ld"):
                data = self.files[name]
                path = Path(tmp) / name
                if isinstance(data, bytes):
                    path.write_bytes(data)
                else:
                    path.write_text(data, encoding="utf-8", newline="\n")
            for args in (["-c", "-O", "main.c", "twice.asm"],
                         ["-O", "main.c", "twice.asm", "add.o", "-T", "rom.ld",
                          "--defsym", "port=0xC000", "-o", "prog"]):
                with self.subTest(args=args):
                    done = subprocess.run(
                        [sys.executable, "-c", run,
                         str(_ROOT / "compile.py"), *args],
                        cwd=tmp, env={**env, "PYTHONPATH": str(_ROOT)},
                        check=True, capture_output=True, text=True,
                    )
                    self.assertEqual(sorted(linker & set(done.stdout.split())), [])


if __name__ == "__main__":
    unittest.main()
