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
        rc = compile_main(["compile.py", "-", "--codegen", *flags])
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
            ("struct s { int v; _Bool end; };\n"
             "[[c6502::preserves_x, c6502::return_bool_in_carry, c6502::"
             "address(0x1000)]] struct s f(void);",
             "whose value member is 2 bytes"),
        "carry struct of the wrong shape":
            ("struct s { long v; _Bool end; };\n"
             "[[c6502::return_bool_in_carry]] struct s f(void);",
             "exactly two members"),
        "carry on an int":
            ("[[c6502::return_bool_in_carry]] int f(void);",
             "a struct of a value and a `bool`, not `int`"),
        "call save on a call that can't recurse":
            ("int g(int x) { return x; }\nint f(int x) {\n"
             "#pragma c6502 call save(hardware_stack)\nreturn g(x);\n}\n",
             "'g' can't call 'f' again"),
        "call save before an if":
            ("int f(int x) {\n#pragma c6502 call save(hardware_stack)\n"
             "if (x) return f(x - 1);\nreturn 0;\n}\n",
             "must come before an expression statement"),
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
        # Only without --optimize: the optimizer folds this array away.
        rc, err = _compile(
            "unsigned char f(void) { unsigned char b[300]; b[0] = 1; "
            "return b[0]; }",
        )
        self.assertEqual(rc, 1)
        self.assertTrue(err.startswith("<stdin>: error: function 'f': "))
        self.assertIn("a frame holds at most 253", err)

    def test_one_line_errors(self) -> None:
        for label, (src, needle) in self._CASES.items():
            for flags in ((), ("--optimize",)):
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


if __name__ == "__main__":
    unittest.main()
