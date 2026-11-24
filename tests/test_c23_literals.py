"""Tests for C23's binary constants and digit separators (C23 §6.4.4.2,
§6.4.4.3, §6.4.8; pp_numbers.py).

Coverage:

  * `0b1010` and `0B1010` lex as one constant, with every suffix, and
    take the type list of octal and hexadecimal constants, so `0b` and
    sixteen 1s is an `unsigned int`, where the decimal 65535 is a
    `long`.
  * A `'` between two digits of a digit sequence is a digit separator,
    in decimal, octal, hexadecimal, and binary constants and in the
    whole part, fraction, and exponent of a floating constant. It's
    ignored for the value.
  * A separator anywhere else is a lex error whose message says where
    it is: at the end, doubled, after the prefix, touching the period,
    next to the exponent, or before the suffix. A binary constant with
    a digit other than 0 and 1, or with no digits, is an error, and so
    is a `wb` or `uwb` constant too wide for `_BitInt(64)` (the `wb`
    constants themselves are tested in tests/test_bitint.py).
  * The preprocessor keeps a number with separators one token, so the
    `'` doesn't start a character constant that would hide a macro
    from expansion, and `#if` reads binary constants, separators, and
    the `ll` suffixes. A number there that isn't an integer constant
    is a `PreprocessorError`.
  * An octal constant with a digit other than 0, such as `017`, has
    the right value. The parser used to raise a Python ValueError for
    it.
  * A program using them runs to the same result in the simulator, in
    both pipelines.
"""
from __future__ import annotations

import shutil
import unittest

from preprocessor import PreprocessorError, preprocess
from sim.harness import build_sim

_HAVE_PCPP = shutil.which("pcpp") is not None


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestPreprocessor(unittest.TestCase):
    def test_separator_doesnt_start_a_character_constant(self) -> None:
        out = preprocess(
            "#define FOO 3\nint a = 1'000 + FOO + 'a';\n"
        )
        self.assertIn("int a = 1'000 + 3 + 'a';", out)

    def test_number_with_separators_is_one_token(self) -> None:
        # Stringizing keeps the spelling, and pasting makes one number.
        out = preprocess(
            "#define S(x) #x\n#define CAT(a, b) a ## b\n"
            "const char *s = S(1'000'000);\nlong n = CAT(1'000, '000);\n"
        )
        self.assertIn('const char *s = "1\'000\'000";', out)
        self.assertIn("long n = 1'000'000;", out)

    def test_if_reads_c23_numbers(self) -> None:
        conditions = [
            "0b1010 == 10",
            "0B1111'0000 == 0xF0",
            "1'000'000 == 1000000",
            "0xF'F == 255",
            "1ULL == 1 && 2ll == 2 && 3LLU == 3",
            "0'17 == 15",
            # A hexadecimal constant above INTMAX_MAX is unsigned.
            "0xFFFF'FFFF'FFFF'FFFF > 0",
            "-1 == 0xFFFFFFFFFFFFFFFF",
            "18446744073709551615u == 0xFFFFFFFFFFFFFFFF",
        ]
        for cond in conditions:
            with self.subTest(cond=cond):
                out = preprocess(f"#if {cond}\nint yes;\n#else\nint no;\n#endif\n")
                self.assertIn("int yes;", out)
                self.assertNotIn("int no;", out)

    def test_if_number_from_a_macro(self) -> None:
        out = preprocess(
            "#define MASK 0b1111'0000\n#if MASK == 240\nint yes;\n#endif\n"
        )
        self.assertIn("int yes;", out)

    def test_if_rejects_invalid_numbers(self) -> None:
        cases = {
            "0x'FF": "comes right after the prefix 0x",
            "0b2": "invalid digit '2' in binary constant",
            "1'u": "comes right before the suffix",
            "1.5": "can't contain the floating constant '1.5'",
            "9223372036854775808wb": "needs a _BitInt(65)",
            "18446744073709551615": "doesn't fit in intmax_t",
            "0x1'0000'0000'0000'0000": "doesn't fit in uintmax_t",
        }
        for number, message in cases.items():
            with self.subTest(number=number):
                with self.assertRaises(PreprocessorError) as cm:
                    preprocess(f"int x;\n#if {number}\n#endif\n")
                self.assertIn(message, str(cm.exception))
                self.assertIn("line 2", str(cm.exception))

    def test_if_skipped_group_isnt_evaluated(self) -> None:
        # The `#elif` after a true `#if` isn't evaluated (C23 §6.10.2p17),
        # so its invalid number isn't reported.
        out = preprocess("#if 1\nint yes;\n#elif 0b2\n#endif\n")
        self.assertIn("int yes;", out)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestSimulator(unittest.TestCase):
    def test_program(self) -> None:
        src = """
#define K 1'000
#if 0b1 && K == 1000
static unsigned long table[] = { 0b1, 0b10, 0x1'00, 1'000'000 };
#endif
int main(void) {
    unsigned long sum = 0;
    for (unsigned char i = 0; i < 4; i++)
        sum += table[i];
    double d = 1'000.5;
    if (d != 1000.5)
        return -1;
    return (int)(sum - 1'000'000UL) + 0b1000'0000 + K;
}
"""
        want = 1 + 2 + 256 + 128 + 1000
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(src, optimize=optimize).run()
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int(), want)


if __name__ == "__main__":
    unittest.main()
