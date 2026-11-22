"""Tests that `compile.py` reports a problem with the program being
compiled as a one-line error, `<file>: error: <message>`, with exit
status 1 and no Python traceback (`compile._source_errors`).
"""
from __future__ import annotations

import io
import unittest
from unittest.mock import patch

from compile import main as compile_main


def _compile(src: str, *flags: str) -> tuple[int, str]:
    err = io.StringIO()
    with patch("sys.stdin", io.StringIO(src)), patch("sys.stderr", err), \
            patch("sys.stdout", io.StringIO()):
        rc = compile_main(["compile.py", "-", "-S", *flags])
    return rc, err.getvalue()


class TestCliErrors(unittest.TestCase):

    _CASES = {
        "unknown attribute":
            ("[[c6502::bogus]] void f(void);", "unknown attribute"),
        "attribute twice":
            ("[[c6502::zp_abi, c6502::zp_abi]] void f(void);",
             "`c6502::zp_abi` is given twice"),
        "preserves_x without address":
            ("[[c6502::preserves_x]] void f(void);",
             "has `preserves_x` but no `[[c6502::address(...)]]`"),
        "preserves_x with an int result":
            ("[[c6502::preserves_x, c6502::address(0x1000)]] int f(void);",
             "whose high byte comes back in X"),
        "preserves on a definition":
            ("[[c6502::preserves_y]] void f(void) {}",
             "can't have `preserves_x` or `preserves_y`"),
        "preserves_x with an int carry struct":
            ("struct s { int v; bool end; };\n"
             "[[c6502::preserves_x, c6502::return_bool_in_carry, c6502::"
             "address(0x1000)]] struct s f(void);",
             "whose value member is 2 bytes"),
        "carry struct of the wrong shape":
            ("struct s { long v; bool end; };\n"
             "[[c6502::return_bool_in_carry]] struct s f(void);",
             "exactly two members"),
        "carry on an int":
            ("[[c6502::return_bool_in_carry]] int f(void);",
             "a struct of a value and a `bool`, not `int`"),
        "call save on a call that can't recurse":
            ("int g(int x) { return x; }\nint f(int x) {\n"
             "[[c6502::save_hardware_stack]] return g(x);\n}\n",
             "'g' can't call 'f' again"),
        "call save before an if":
            ("int f(int x) {\n[[c6502::save_hardware_stack]]\n"
             "if (x) return f(x - 1);\nreturn 0;\n}\n",
             "goes before an expression statement"),
        "old pragma after an include":
            ("#include <stdint.h>\nint f(void) {\n"
             "#pragma c6502 loop unroll(enable)\n"
             "for (int i = 0; i < 2; i++) ;\nreturn 0;\n}\n",
             "<stdin>: error: line 3: c6502 no longer takes `#pragma "
             "c6502 loop unroll(enable)`; write the statement attribute "
             "`[[c6502::unroll]]`"),
        "zeropage on a local":
            ("int main(void) { unsigned char a "
             "[[c6502::zeropage]]; return a; }",
             "'a' is automatic"),
        "two addresses":
            ("[[c6502::address(0x1000)]] void f(void);\n"
             "[[c6502::address(0x2000)]] void f(void);",
             "different addresses, $1000 and $2000"),
        "register parameter without zp_abi":
            ("[[c6502::address(0x1000)]] "
             "void f(register unsigned char c);\n"
             "void g(void) { f(1); }",
             "no `[[c6502::zp_abi]]`"),
    }

    def test_frame_too_large(self) -> None:
        # Only without -O: the optimizer folds this array away.
        rc, err = _compile(
            "unsigned char f(void) { unsigned char b[300]; b[0] = 1; "
            "return b[0]; }",
        )
        self.assertEqual(rc, 1)
        self.assertTrue(err.startswith("<stdin>: error: function 'f': "))
        self.assertIn("a frame holds at most 253", err)

    def test_arguments_past_the_frame_reach(self) -> None:
        # The locals fit in 253 bytes, but `d` sits above them, the
        # saved frame pointer, and the other arguments, past Frame(255),
        # and the function reads it. This used to raise a ValueError
        # from `asm_emit` (`fuzz.exprs` seed 43).
        rc, err = _compile(
            "unsigned char f(unsigned long long a, unsigned long long b, "
            "unsigned long long c, unsigned char d) "
            "{ unsigned char t[225]; t[0] = d; return t[0]; }",
        )
        self.assertEqual(rc, 1)
        self.assertTrue(err.startswith("<stdin>: error: function 'f': "), err)
        self.assertIn("frame offset 261 out of range 1..255", err)
        self.assertNotIn("Traceback", err)

    def test_unread_arguments_past_the_frame_reach(self) -> None:
        # `b` sits past Frame(255), but nothing reads it through the
        # frame, so the function compiles.
        rc, err = _compile(
            "unsigned char f(unsigned long long a, unsigned long long b) "
            "{ unsigned char t[230]; t[0] = 1; return t[0]; }",
        )
        self.assertEqual(rc, 0, err)

    def test_one_line_errors(self) -> None:
        for label, (src, needle) in self._CASES.items():
            for flags in ((), ("-O",)):
                with self.subTest(label, flags=flags):
                    rc, err = _compile(src, *flags)
                    self.assertEqual(rc, 1)
                    self.assertTrue(err.startswith("<stdin>: error: "), err)
                    self.assertIn(needle, err)
                    self.assertNotIn("Traceback", err)

    def test_no_internal_names(self) -> None:
        # A block-scope object is renamed `@N.x` inside the compiler.
        rc, err = _compile(
            "int main(void) { static unsigned char x "
            "[[c6502::address(0xC000)]]; return x; }",
        )
        self.assertEqual(rc, 1)
        self.assertIn("'x'", err)
        self.assertNotIn("@", err)


class TestUnreadableInputs(unittest.TestCase):
    """An input or a linker script that isn't there, or a source that
    isn't UTF-8, is a one-line error. Each gave a traceback, of the
    FileNotFoundError or the UnicodeDecodeError."""

    def _run(self, *argv: str) -> tuple[int, str]:
        err = io.StringIO()
        with patch("sys.stderr", err), patch("sys.stdout", io.StringIO()):
            rc = compile_main(["compile.py", *argv])
        return rc, err.getvalue()

    def test_missing_files(self) -> None:
        import tempfile
        from pathlib import Path
        with tempfile.TemporaryDirectory() as tmp:
            out = str(Path(tmp) / "out")
            obj = Path(tmp) / "a.o"
            self.assertEqual(self._run(
                "examples/sfx_tone.c", "-c", "-o", str(obj))[0], 0)
            cases = {
                ("missing.c", "-S", "-o", out + ".s"):
                    "missing.c: error: No such file or directory\n",
                ("missing.c", "-c", "-o", out):
                    "missing.c: error: No such file or directory\n",
                ("missing.asm", "-c", "-o", out):
                    "missing.asm: error: No such file or directory\n",
                ("missing.o", "-o", out):
                    "missing.o: error: No such file or directory\n",
                ("-T", "missing.ld", str(obj), "-o", out):
                    "compile.py: link error: missing.ld: No such file or "
                    "directory\n",
            }
            for argv, expected in cases.items():
                with self.subTest(argv=argv):
                    self.assertEqual(self._run(*argv), (1, expected))

    def test_source_that_isnt_utf8(self) -> None:
        import tempfile
        from pathlib import Path
        with tempfile.TemporaryDirectory() as tmp:
            src = Path(tmp) / "latin1.c"
            src.write_bytes(b"int main(void) { return 0; } /* \xe9 */\n")
            rc, err = self._run(str(src), "-S", "-o", str(Path(tmp) / "x.s"))
        self.assertEqual(rc, 1)
        self.assertEqual(
            err,
            f"{src}: error: 'utf-8' codec can't decode byte 0xe9 in "
            f"position 32: invalid continuation byte\n",
        )


if __name__ == "__main__":
    unittest.main()
