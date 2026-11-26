"""Tests for cpp/src/passes/carry_return_choice.h, which returns a `static`
function's `bool` in the carry flag, without the annotation, when
every call to it suits the carry."""

import unittest

from sim.harness import build_sim
from tests.no_inlining import no_inlining
from tests.cpp_build import needs_c6502cpp


_HEAD = "#include <stdint.h>\n#include <stdbool.h>\nuint8_t g;\n"


@needs_c6502cpp
@no_inlining()
class TestChoice(unittest.TestCase):

    def test_values(self):
        src = _HEAD + (
            "uint8_t s[6] = {'a', 'x', 'b', 'y', 'b', 'a'};\n"
            "static bool is_a(uint8_t c) { return c == 'a'; }\n"
            "static bool is_b(uint8_t c) { return c == 'b'; }\n"
            "static bool either(uint8_t c) {\n"
            "    if (is_a(c)) return true;\n"
            "    return is_b(c);\n"
            "}\n"
            "int main(void) {\n"
            "    int n = 0;\n"
            "    for (uint8_t i = 0; i != 6; i++) {\n"
            "        if (either(s[i])) n += 10;\n"
            "        if (!is_a(s[i])) n += 1;\n"
            "    }\n"
            "    return n;\n"
            "}\n"
        )
        # Four of the six are 'a' or 'b', and four aren't 'a'.
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(src, optimize=optimize).run(
                    max_cycles=5_000_000,
                )
                self.assertFalse(out.timed_out)
                self.assertEqual(out.return_int(), 40 + 4)


@needs_c6502cpp
class TestGlobalChoice(unittest.TestCase):
    """A `bool` function with external linkage returns in C whenever
    it's zp_abi, whatever its callers do with the result, since the
    other units decide from the declaration too."""

    def test_values(self):
        # Each way a caller can use the result: branched on, kept as a
        # value, negated, and returned by another such function.
        src = _HEAD + (
            "uint8_t s[6] = {'a', 'x', 'b', 'y', 'b', 'a'};\n"
            "bool is_a(uint8_t c) { return c == 'a'; }\n"
            "bool is_b(uint8_t c) { return c == 'b'; }\n"
            "bool either(uint8_t c) {\n"
            "    if (is_a(c)) return true;\n"
            "    return is_b(c);\n"
            "}\n"
            "int main(void) {\n"
            "    int n = 0;\n"
            "    for (uint8_t i = 0; i != 6; i++) {\n"
            "        if (either(s[i])) n += 10;\n"
            "        if (!is_a(s[i])) n += 1;\n"
            "        n += 100 * is_b(s[i]);\n"
            "    }\n"
            "    return n;\n"
            "}\n"
        )
        # Four are 'a' or 'b', four aren't 'a', and two are 'b'.
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(src, optimize=optimize).run(
                    max_cycles=5_000_000,
                )
                self.assertFalse(out.timed_out)
                self.assertEqual(out.return_int(), 40 + 4 + 200)


_LIB = (
    "#include <stdint.h>\n#include <stdbool.h>\n"
    "bool is_digit(uint8_t c) { return c >= '0' && c <= '9'; }\n"
)
_MAIN = (
    "#include <stdint.h>\n#include <stdbool.h>\n"
    "bool is_digit(uint8_t c);\n"
    "uint8_t s[5] = {'1', 'x', '7', ' ', '0'};\n"
    "int main(void) {\n"
    "    int n = 0;\n"
    "    for (uint8_t i = 0; i != 5; i++) {\n"
    "        if (is_digit(s[i])) n += 10;\n"
    "        n += is_digit(s[i]);\n"
    "    }\n"
    "    return n;\n"
    "}\n"
)


@needs_c6502cpp
class TestLinked(unittest.TestCase):

    def _compile(self, src: str, path) -> str:
        import io
        from unittest.mock import patch
        from compile import main as compile_main
        with patch("sys.stdin", io.StringIO(src)):
            self.assertEqual(compile_main([
                "compile.py", "-", "-S", "-O",
                "-o", str(path),
            ]), 0)
        return path.read_text()

    def test_units_agree_and_run(self):
        import tempfile
        from pathlib import Path
        from compile import main as compile_main
        with tempfile.TemporaryDirectory() as tmp:
            lib = self._compile(_LIB, Path(tmp) / "lib.asm")
            main = self._compile(_MAIN, Path(tmp) / "main.asm")
            # The defining unit and the calling unit both record it.
            self.assertRegex(lib, r"; def is_digit .*carry=true")
            self.assertRegex(main, r"; ext is_digit .*carry=true")
            out = Path(tmp) / "out"
            self.assertEqual(compile_main([
                "compile.py", str(Path(tmp) / "lib.asm"),
                str(Path(tmp) / "main.asm"), "-o", str(out),
            ]), 0)
            from tests.link_helpers import linked_text
            text = linked_text(out)
        from sim.linked import build_linked_sim
        # Three of the five are digits.
        self.assertEqual(build_linked_sim(text.exe).run().return_int(), 33)


if __name__ == "__main__":
    unittest.main()
