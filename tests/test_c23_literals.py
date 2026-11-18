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

import c99_ast
import fp_arith
import pp_numbers
from lexer import LexError, TokenKind, tokenize
from parser import parse
from passes.identifier_resolution import resolve_program
from passes.type_checking import check_program
from preprocessor import PreprocessorError, preprocess
from sim.harness import build_sim

_HAVE_PCPP = shutil.which("pcpp") is not None


def _kinds_vals(source: str) -> list[tuple[TokenKind, str]]:
    return [(t.kind, t.value) for t in tokenize(source)]


def _constant(text: str):
    """The c99 const the parser makes of the constant `text`."""
    prog = parse(f"int main(void) {{ return {text}; }}")
    ret = prog.declaration[0].function_decl.body.block_item[0].statement
    return ret.exp.const


def _static_value(src: str, name: str) -> int:
    _, symbols, _ = check_program(resolve_program(parse(preprocess(src))))
    return symbols[name].attrs.initial_value.value


class TestPpNumber(unittest.TestCase):
    def test_extent(self) -> None:
        cases = {
            "1'000;": "1'000",
            "1''0": "1",
            "1'": "1",
            "0x'FF": "0x'FF",
            "1e+5'0": "1e+5'0",
            ".5'5f": ".5'5f",
            "1'.5": "1",
        }
        for text, want in cases.items():
            with self.subTest(text=text):
                self.assertEqual(pp_numbers.pp_number_at(text, 0), want)


class TestBinaryConstants(unittest.TestCase):
    def test_lexes_as_one_constant(self) -> None:
        for s in ["0b0", "0b1", "0B1010", "0b1010u", "0b1010U", "0b1l",
                  "0b1L", "0b1ul", "0b1LU", "0b1ll", "0b1ULL", "0b1llu"]:
            with self.subTest(s=s):
                self.assertEqual(_kinds_vals(s), [(TokenKind.CONSTANT, s)])

    def test_values_and_types(self) -> None:
        cases = {
            "0b1010": c99_ast.ConstInt(value=10),
            "0B1111": c99_ast.ConstInt(value=15),
            "0b0": c99_ast.ConstInt(value=0),
            # The hexadecimal list gives `unsigned int` after `int`.
            "0b1111111111111111": c99_ast.ConstUInt(value=0xFFFF),
            "0b10000000000000000": c99_ast.ConstLong(value=0x10000),
            "0b1u": c99_ast.ConstUInt(value=1),
            "0b1l": c99_ast.ConstLong(value=1),
            "0b1ul": c99_ast.ConstULong(value=1),
            "0b1ll": c99_ast.ConstLongLong(value=1),
            "0b1ull": c99_ast.ConstULongLong(value=1),
            "0b" + "1" * 64: c99_ast.ConstULongLong(value=(1 << 64) - 1),
        }
        for text, want in cases.items():
            with self.subTest(text=text):
                self.assertEqual(_constant(text), want)

    def test_decimal_65535_is_long(self) -> None:
        # The contrast with the binary constant of the same value.
        self.assertEqual(_constant("65535"), c99_ast.ConstLong(value=65535))

    def test_bad_digits_and_missing_digits(self) -> None:
        cases = {
            "0b2": "invalid digit '2' in binary constant",
            "0b102": "invalid digit '2' in binary constant",
            "0b": "no digits after the prefix 0b",
            "0B": "no digits after the prefix 0B",
            "0b1.0": "a binary constant is an integer",
            "0b1f": "invalid suffix 'f'",
        }
        for text, message in cases.items():
            with self.subTest(text=text):
                with self.assertRaises(LexError) as cm:
                    list(tokenize(text))
                self.assertIn(message, str(cm.exception))

    def test_bit_precise_suffix_too_wide_is_an_error(self) -> None:
        for text in ["0b1" + "0" * 64 + "wb", "0x8000000000000000wb",
                     "0x1'0000'0000'0000'0000uwb"]:
            with self.subTest(text=text):
                with self.assertRaises(Exception) as cm:
                    parse(f"int x = {text};")
                self.assertIn("BITINT_MAXWIDTH is 64", str(cm.exception))


class TestDigitSeparators(unittest.TestCase):
    def test_lex_as_one_constant(self) -> None:
        for s in ["1'000", "1'000'000", "0xFF'FF", "0x1'2'3'4AB'C'D",
                  "0b11'10'11'01", "0'17", "1'000u", "1'000ULL",
                  "1'000.5", "1.0'0'1", "1'0e1'0", "1e+1'0", ".5'5f",
                  "0x1'0.8p1"]:
            with self.subTest(s=s):
                self.assertEqual(_kinds_vals(s), [(TokenKind.CONSTANT, s)])

    def test_ignored_for_the_value(self) -> None:
        cases = {
            "1'000": c99_ast.ConstInt(value=1000),
            "1'000'000": c99_ast.ConstLong(value=1000000),
            "0xFF'FF": c99_ast.ConstUInt(value=0xFFFF),
            "0x1'2'3'4AB'C'D": c99_ast.ConstLong(value=0x1234ABCD),
            "0b11'10'11'01": c99_ast.ConstInt(value=0b11101101),
            "0'17": c99_ast.ConstInt(value=0o17),
            "1'2'3u": c99_ast.ConstUInt(value=123),
        }
        for text, want in cases.items():
            with self.subTest(text=text):
                self.assertEqual(_constant(text), want)

    def test_floating(self) -> None:
        cases = {
            "1'000.5": "1000.5",
            "1.2'5": "1.25",
            "1'0e1'0": "10e10",
            "2'5e-1": "25e-1",
        }
        for text, plain in cases.items():
            with self.subTest(text=text):
                self.assertEqual(
                    _constant(text),
                    c99_ast.ConstDouble(
                        bits=fp_arith.double_string_to_bits(plain),
                    ),
                )
        self.assertEqual(
            _constant("1'000.5f"),
            c99_ast.ConstFloat(bits=fp_arith.single_string_to_bits("1000.5")),
        )

    def test_misplaced_separators_are_errors(self) -> None:
        cases = {
            "1'": "isn't followed by a digit",
            "1''000": "isn't followed by a digit",
            "1'000'": "isn't followed by a digit",
            "1'.5": "isn't followed by a digit",
            "0x'FF": "comes right after the prefix 0x",
            "0X'FF": "comes right after the prefix 0X",
            "0b'1": "comes right after the prefix 0b",
            "1.'5": "touches the period",
            "1'e5": "comes right before the exponent",
            "1e'5": "comes right after the exponent's letter or sign",
            "1e+'5": "comes right after the exponent's letter or sign",
            "0x1'p1": "comes right before the exponent",
            "1'u": "comes right before the suffix",
            "1.5'f": "comes right before the suffix",
            "0'8": "invalid digit '8' in octal constant",
            "0b1'2": "invalid digit '2' in binary constant",
        }
        for text, message in cases.items():
            with self.subTest(text=text):
                with self.assertRaises(LexError) as cm:
                    list(tokenize(f"x = {text};"))
                self.assertIn(message, str(cm.exception))

    def test_parser_reports_the_same_errors(self) -> None:
        # The parser runs its own Lark lexer, which calls the same
        # check as `tokenize`.
        cases = {
            "0x'FF": "comes right after the prefix 0x",
            "1''0": "isn't followed by a digit",
            "0b12": "invalid digit '2' in binary constant",
            "9223372036854775808wb": "needs a _BitInt(65)",
        }
        for text, message in cases.items():
            with self.subTest(text=text):
                with self.assertRaises(LexError) as cm:
                    parse(f"int x = {text};")
                self.assertIn(message, str(cm.exception))

    def test_character_constant_after_a_number_is_unchanged(self) -> None:
        # C23 §6.4.4.2p3: `'1'2` is a character constant and then 2.
        self.assertEqual(_kinds_vals("'1'2"), [
            (TokenKind.CONSTANT, "'1'"), (TokenKind.CONSTANT, "2"),
        ])

    def test_numbers_without_c23_spellings_lex_as_before(self) -> None:
        # The pp-number check covers only separators and `0b`, so
        # these keep the tokens test_lexer.py pins.
        self.assertEqual([v for _, v in _kinds_vals("08")], ["0", "8"])
        self.assertEqual(
            [v for _, v in _kinds_vals("3..5")], ["3.", ".5"],
        )


class TestOctal(unittest.TestCase):
    def test_octal_values(self) -> None:
        cases = {
            "017": c99_ast.ConstInt(value=15),
            "0777": c99_ast.ConstInt(value=511),
            "0177777": c99_ast.ConstUInt(value=0xFFFF),
            "017L": c99_ast.ConstLong(value=15),
        }
        for text, want in cases.items():
            with self.subTest(text=text):
                self.assertEqual(_constant(text), want)


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

    def test_static_initializers(self) -> None:
        src = (
            "unsigned long big = 4'294'967'295UL;\n"
            "unsigned char mask = 0b1010'0101;\n"
        )
        self.assertEqual(_static_value(src, "big"), 4294967295)
        self.assertEqual(_static_value(src, "mask"), 0xA5)


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
