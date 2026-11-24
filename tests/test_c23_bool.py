"""Tests for C23's boolean type (C23 §6.2.5, §6.4.1, §6.4.4.6, §7.19).

Coverage:

  * `bool`, `true`, and `false` are keywords. `bool` is the type that
    `_Bool` names, and `true` and `false` are constants of type `bool`
    with the values 1 and 0, so `sizeof(true)` is 1 and `true + true`
    is the `int` 2. They work in constant expressions: array sizes,
    enumeration values, case labels, and static initializers.
  * A name spelled `bool`, `true`, or `false` is an error, since C23
    made them keywords.
  * `_Bool` still compiles, with a warning for each use that names
    `bool`, on the line of the use. compile.py prints the warnings to
    stderr and exits 0, and `diagnostics.collecting()` gives them to a
    caller of the library.
  * `<stdbool.h>` is C23's. It includes without a warning, leaves
    `bool`, `true`, and `false` alone, and defines
    `__bool_true_false_are_defined` to 1.
  * In `#if`, `true` is 1 and `false` is 0 (C23 §6.10.2p13).
  * A program using all of these runs to the same result on the TAC
    interpreter and in the simulator, in both pipelines.

c6502 has no `_Generic`, so the type of `true` is checked through
`sizeof` and the type checker's `data_type` instead.
"""
from __future__ import annotations

import io
import shutil
import unittest
from unittest.mock import patch

import diagnostics
from compile import main as compile_main
from preprocessor import ALT_BOOL_WARNING, preprocess
from sim.harness import build_sim
from tests.test_sim_chapters import _run_program as _run_tac


def _compile(src: str, *flags: str) -> tuple[int, str]:
    """compile.py's exit status and stderr for `src` on stdin."""
    err = io.StringIO()
    with patch("sys.stdin", io.StringIO(src)), patch("sys.stderr", err), \
            patch("sys.stdout", io.StringIO()):
        rc = compile_main(["compile.py", "-", "-S", *flags])
    return rc, err.getvalue()


class TestAltBoolWarning(unittest.TestCase):
    def test_compile_warns_and_succeeds(self) -> None:
        for flags in ((), ("-O",)):
            with self.subTest(flags=flags):
                rc, err = _compile(
                    "int x;\n_Bool f(int v) { return v; }\n", *flags,
                )
                self.assertEqual(rc, 0)
                self.assertEqual(
                    err, f"<stdin>: warning: line 2: {ALT_BOOL_WARNING}\n",
                )

    def test_warning_text(self) -> None:
        self.assertEqual(
            ALT_BOOL_WARNING,
            "`_Bool` is deprecated: C23 makes it an obsolescent spelling "
            "of the keyword `bool` (C23 §6.4.1). Use `bool` instead.",
        )

    def test_warning_comes_before_an_error(self) -> None:
        rc, err = _compile("_Bool a;\nint true;\n")
        self.assertEqual(rc, 1)
        lines = err.splitlines()
        self.assertEqual(lines[0],
                         f"<stdin>: warning: line 1: {ALT_BOOL_WARNING}")
        self.assertTrue(lines[1].startswith("<stdin>: error: "), err)


class TestStdbool(unittest.TestCase):
    def test_includes_without_a_warning(self) -> None:
        with diagnostics.collecting() as found:
            text = preprocess(
                "#include <stdbool.h>\n"
                "#if __bool_true_false_are_defined != 1\n"
                "#error\n"
                "#endif\n"
                "#if defined(bool) || defined(true) || defined(false)\n"
                "#error\n"
                "#endif\n"
                "bool b = true; int n = __bool_true_false_are_defined;\n"
            )
        self.assertEqual(found, [])
        self.assertIn("bool b = true; int n = 1;", text)

    def test_compiles_without_a_warning(self) -> None:
        rc, err = _compile(
            "#include <stdbool.h>\n"
            "bool odd(int x) { return x & 1; }\n"
            "int main(void) { return odd(3) == true; }\n",
        )
        self.assertEqual((rc, err), (0, ""))


_PROGRAM = """\
#include <stdbool.h>

enum { TWO = true + true };
static bool flags[TWO + 1] = { true, false, true };
static int seen = false;
bool global = 7;

[[c6502::return_bool_in_carry]] bool positive(int x) { return x > 0; }

bool odd(unsigned char x) { return x & 1; }

int classify(bool b) {
    switch (b) {
    case true:
        return 10;
    case false:
        return 20;
    }
    return 0;
}

int main(void) {
    bool b = 256;
    int r = 0;
    for (unsigned char i = 0; i < sizeof flags; i++)
        if (flags[i] == true)
            r += 1;
    if (positive(5) && !positive(-5))
        r += 100;
    r += classify(odd(3)) + classify(odd(4));
    r += b + global + seen + TWO + sizeof(true) + (true << 9);
    return r;
}
"""

# 2 flags + 100 + 10 + 20 + 1 + 1 + 0 + 2 + 1 + 512
_EXPECTED = 649


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestPrograms(unittest.TestCase):
    def test_tac_sim(self) -> None:
        self.assertEqual(_run_tac(preprocess(_PROGRAM)), _EXPECTED)

    def test_asm_sim(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(_PROGRAM, optimize=optimize).run(
                    max_cycles=2_000_000,
                )
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int_signed(), _EXPECTED)


if __name__ == "__main__":
    unittest.main()
