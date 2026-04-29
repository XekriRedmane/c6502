import unittest

from lexer import KEYWORDS, SYMBOLS, LexError, TokenKind, tokenize


def vals(source: str) -> list[str]:
    return [t.value for t in tokenize(source)]


def kinds_vals(source: str) -> list[tuple[TokenKind, str]]:
    return [(t.kind, t.value) for t in tokenize(source)]


class TestKeywords(unittest.TestCase):
    def test_each_keyword_tokenizes_as_keyword(self):
        for kw in KEYWORDS:
            with self.subTest(kw=kw):
                self.assertEqual(kinds_vals(kw), [(TokenKind.KEYWORD, kw)])

    def test_keyword_count(self):
        self.assertEqual(len(KEYWORDS), 37)


class TestIdentifiers(unittest.TestCase):
    def test_simple(self):
        for s in ["x", "foo", "_bar", "_", "a1", "x_y_z", "CamelCase"]:
            with self.subTest(s=s):
                self.assertEqual(kinds_vals(s), [(TokenKind.IDENTIFIER, s)])

    def test_keyword_prefix_is_not_keyword(self):
        for s in ["intx", "returning", "while_", "_int", "ifx"]:
            with self.subTest(s=s):
                self.assertEqual(kinds_vals(s), [(TokenKind.IDENTIFIER, s)])


class TestSymbols(unittest.TestCase):
    def test_each_symbol(self):
        for sym in SYMBOLS:
            with self.subTest(sym=sym):
                self.assertEqual(kinds_vals(sym), [(TokenKind.SYMBOL, sym)])

    def test_longest_match(self):
        cases = [
            ("<<=", ["<<="]),
            ("<<", ["<<"]),
            ("<<<", ["<<", "<"]),
            ("<<=<<", ["<<=", "<<"]),
            ("<<==", ["<<=", "="]),
            (">>=", [">>="]),
            ("...", ["..."]),
            ("....", ["...", "."]),
            (".....", ["...", ".", "."]),
            ("......", ["...", "..."]),
            (".......", ["...", "...", "."]),
            ("->", ["->"]),
            ("-->", ["--", ">"]),
            ("++", ["++"]),
            ("+++", ["++", "+"]),
            ("&&&", ["&&", "&"]),
            ("##", ["##"]),
            ("###", ["##", "#"]),
            ("==", ["=="]),
            ("===", ["==", "="]),
        ]
        for src, expected in cases:
            with self.subTest(src=src):
                self.assertEqual(vals(src), expected)


class TestIntegerConstants(unittest.TestCase):
    def test_decimal(self):
        for s in ["1", "42", "123", "999999"]:
            with self.subTest(s=s):
                self.assertEqual(kinds_vals(s), [(TokenKind.CONSTANT, s)])

    def test_octal(self):
        for s in ["0", "00", "01", "0123", "0777"]:
            with self.subTest(s=s):
                self.assertEqual(kinds_vals(s), [(TokenKind.CONSTANT, s)])

    def test_hex(self):
        for s in ["0x0", "0X0", "0x1A", "0X1a", "0xDEADBEEF", "0xffffffff"]:
            with self.subTest(s=s):
                self.assertEqual(kinds_vals(s), [(TokenKind.CONSTANT, s)])

    def test_all_valid_suffixes_on_all_bodies(self):
        suffixes = [
            "", "u", "U",
            "ul", "uL", "Ul", "UL",
            "ull", "uLL", "Ull", "ULL",
            "l", "L",
            "lu", "lU", "Lu", "LU",
            "ll", "LL",
            "llu", "llU", "LLu", "LLU",
        ]
        for body in ["1", "0", "0x1A"]:
            for suf in suffixes:
                s = body + suf
                with self.subTest(s=s):
                    self.assertEqual(kinds_vals(s), [(TokenKind.CONSTANT, s)])

    def test_long_long_is_case_strict(self):
        # long-long-suffix is exactly 'll' or 'LL'; mixed `lL` / `Ll` doesn't
        # form a valid suffix and the adjacency check rejects the whole thing.
        for s in ["42lL", "42Ll"]:
            with self.subTest(s=s):
                with self.assertRaises(LexError):
                    list(tokenize(s))

    def test_suffix_then_identifier_rejected(self):
        # `42ua` isn't a valid int + separate identifier — it's a malformed
        # pp-number. Must be rejected (strict C99).
        with self.assertRaises(LexError):
            list(tokenize("42ua"))

    def test_octal_stops_at_nonoctal_digit(self):
        # '8' and '9' are not octal digits, so they break off.
        self.assertEqual(vals("08"), ["0", "8"])
        self.assertEqual(vals("019"), ["01", "9"])
        self.assertEqual(vals("089"), ["0", "89"])

    def test_hex_requires_at_least_one_digit(self):
        for s in ["0x", "0X", "0xg", "0x+"]:
            with self.subTest(s=s):
                with self.assertRaises(LexError):
                    list(tokenize(s))


class TestFloatingConstants(unittest.TestCase):
    def test_decimal_with_dot(self):
        for s in ["3.14", "3.", ".5", "0.0", "123.456", "00.00"]:
            with self.subTest(s=s):
                self.assertEqual(kinds_vals(s), [(TokenKind.CONSTANT, s)])

    def test_decimal_with_exponent(self):
        for s in ["3e5", "3E5", "3e+5", "3e-5", "3E+10", "3.14e5", "3.e5",
                  ".5e10", ".5E-10", "0e0"]:
            with self.subTest(s=s):
                self.assertEqual(kinds_vals(s), [(TokenKind.CONSTANT, s)])

    def test_decimal_with_suffix(self):
        for s in ["3.14f", "3.14F", "3.14l", "3.14L", "3.f", ".5L",
                  "3e5f", "3e5F", "3e5l", "3e5L"]:
            with self.subTest(s=s):
                self.assertEqual(kinds_vals(s), [(TokenKind.CONSTANT, s)])

    def test_hex_float_basic(self):
        for s in ["0x1p1", "0X1p1", "0x1P1", "0x1p+1", "0x1p-1",
                  "0xFFp10", "0xDEADp-5"]:
            with self.subTest(s=s):
                self.assertEqual(kinds_vals(s), [(TokenKind.CONSTANT, s)])

    def test_hex_float_fractional(self):
        for s in ["0x1.p1", "0x1.8p1", "0x.FFp0", "0x1.FFp-2", "0xA.Bp3"]:
            with self.subTest(s=s):
                self.assertEqual(kinds_vals(s), [(TokenKind.CONSTANT, s)])

    def test_hex_float_with_suffix(self):
        for s in ["0x1p1f", "0x1p1F", "0x1p1l", "0x1p1L",
                  "0x1.8p1f", "0x.FFp0L"]:
            with self.subTest(s=s):
                self.assertEqual(kinds_vals(s), [(TokenKind.CONSTANT, s)])

    def test_decimal_without_exp_requires_dot(self):
        # "3e" with no digits after e is invalid.
        for s in ["3e", "3e+", "3e-", "3.e", "3.14e"]:
            with self.subTest(s=s):
                with self.assertRaises(LexError):
                    list(tokenize(s))

    def test_hex_float_requires_binary_exponent(self):
        for s in ["0x1.", "0x1.8", "0x.FF", "0xFF."]:
            with self.subTest(s=s):
                with self.assertRaises(LexError):
                    list(tokenize(s))

    def test_hex_float_requires_exp_digits(self):
        for s in ["0x1p", "0x1p+", "0x1p-"]:
            with self.subTest(s=s):
                with self.assertRaises(LexError):
                    list(tokenize(s))

    def test_hex_float_needs_hex_digit(self):
        # "0x.p1" has no hex digits at all; "0xp1" same.
        for s in ["0x.p1", "0xp1"]:
            with self.subTest(s=s):
                with self.assertRaises(LexError):
                    list(tokenize(s))

    def test_int_vs_float_suffix_disambiguation(self):
        # 'l' on a dotless integer is int-long.
        self.assertEqual(kinds_vals("3l"), [(TokenKind.CONSTANT, "3l")])
        # 'l' on a float is float-long.
        self.assertEqual(kinds_vals("3.l"), [(TokenKind.CONSTANT, "3.l")])
        # 'f' is not a valid int suffix, so `3f` is an invalid pp-number.
        with self.assertRaises(LexError):
            list(tokenize("3f"))
        # 'u' is not a valid float suffix, so `3.u` is an invalid pp-number.
        with self.assertRaises(LexError):
            list(tokenize("3.u"))

    def test_lone_dot_is_symbol(self):
        # A dot not followed by a digit stays a symbol.
        self.assertEqual(kinds_vals("."), [(TokenKind.SYMBOL, ".")])
        self.assertEqual(kinds_vals(".x"), [
            (TokenKind.SYMBOL, "."),
            (TokenKind.IDENTIFIER, "x"),
        ])

    def test_trailing_dot_breaks_off(self):
        # "3..5" -> "3." then ".5".
        self.assertEqual(kinds_vals("3..5"), [
            (TokenKind.CONSTANT, "3."),
            (TokenKind.CONSTANT, ".5"),
        ])

    def test_exponent_followed_by_more_rejected(self):
        # `1e2e3` is an invalid pp-number (numeric + abutting identifier).
        with self.assertRaises(LexError):
            list(tokenize("1e2e3"))


class TestCharacterConstants(unittest.TestCase):
    def test_basic(self):
        for s in ["'a'", "'Z'", "'0'", "' '", "'~'", "'!'", "'?'", "'{'"]:
            with self.subTest(s=s):
                self.assertEqual(kinds_vals(s), [(TokenKind.CONSTANT, s)])

    def test_wide(self):
        self.assertEqual(kinds_vals("L'a'"), [(TokenKind.CONSTANT, "L'a'")])
        self.assertEqual(kinds_vals("L'abc'"), [(TokenKind.CONSTANT, "L'abc'")])

    def test_multi_char(self):
        for s in ["'ab'", "'abc'", "'hello'"]:
            with self.subTest(s=s):
                self.assertEqual(kinds_vals(s), [(TokenKind.CONSTANT, s)])

    def test_simple_escapes(self):
        for s in [r"'\''", r"'\"'", r"'\?'", r"'\\'",
                  r"'\a'", r"'\b'", r"'\f'", r"'\n'",
                  r"'\r'", r"'\t'", r"'\v'"]:
            with self.subTest(s=s):
                self.assertEqual(kinds_vals(s), [(TokenKind.CONSTANT, s)])

    def test_octal_escapes(self):
        for s in [r"'\0'", r"'\7'", r"'\12'", r"'\77'", r"'\123'", r"'\000'"]:
            with self.subTest(s=s):
                self.assertEqual(kinds_vals(s), [(TokenKind.CONSTANT, s)])

    def test_octal_max_three_digits(self):
        # \1234 consumes 3 octal digits (\123), then '4' is another c-char.
        self.assertEqual(kinds_vals(r"'\1234'"), [(TokenKind.CONSTANT, r"'\1234'")])

    def test_hex_escapes(self):
        for s in [r"'\x0'", r"'\xA'", r"'\x41'", r"'\xFF'", r"'\xDEADBEEF'"]:
            with self.subTest(s=s):
                self.assertEqual(kinds_vals(s), [(TokenKind.CONSTANT, s)])

    def test_hex_escape_stops_at_non_hex(self):
        # \x41 then 'x' literal (x is not a hex digit).
        self.assertEqual(kinds_vals(r"'\x41x'"), [(TokenKind.CONSTANT, r"'\x41x'")])

    def test_universal_escapes(self):
        s = "'\\u0041'"
        self.assertEqual(kinds_vals(s), [(TokenKind.CONSTANT, s)])
        s = "'\\U0001F600'"
        self.assertEqual(kinds_vals(s), [(TokenKind.CONSTANT, s)])

    def test_universal_stops_at_exact_digit_count(self):
        # \u takes exactly 4 hex digits, then '5' is another c-char.
        s = "'\\u12345'"
        self.assertEqual(kinds_vals(s), [(TokenKind.CONSTANT, s)])

    def test_empty_is_error(self):
        with self.assertRaises(LexError):
            list(tokenize("''"))
        with self.assertRaises(LexError):
            list(tokenize("L''"))

    def test_unterminated_is_error(self):
        for s in ["'", "'a", "'abc", "L'", r"'\n"]:
            with self.subTest(s=s):
                with self.assertRaises(LexError):
                    list(tokenize(s))

    def test_real_newline_inside_is_error(self):
        with self.assertRaises(LexError):
            list(tokenize("'a\nb'"))

    def test_invalid_escape_is_error(self):
        for s in [r"'\q'", r"'\9'", r"'\Q'", r"'\e'"]:
            with self.subTest(s=s):
                with self.assertRaises(LexError):
                    list(tokenize(s))

    def test_hex_escape_requires_digit(self):
        for s in [r"'\x'", r"'\xG'"]:
            with self.subTest(s=s):
                with self.assertRaises(LexError):
                    list(tokenize(s))

    def test_universal_requires_full_digit_count(self):
        for s in [r"'\u'", r"'\u1'", r"'\u12'", r"'\u123'", r"'\u12G4'"]:
            with self.subTest(s=s):
                with self.assertRaises(LexError):
                    list(tokenize(s))
        for s in [r"'\U'", r"'\U1234'", r"'\U1234567'"]:
            with self.subTest(s=s):
                with self.assertRaises(LexError):
                    list(tokenize(s))

    def test_non_printable_ascii_is_error(self):
        for s in ["'\t'", "'\x01'", "'\x7f'"]:
            with self.subTest(s=repr(s)):
                with self.assertRaises(LexError):
                    list(tokenize(s))

    def test_non_ascii_is_error(self):
        with self.assertRaises(LexError):
            list(tokenize("'é'"))

    def test_L_alone_is_identifier(self):
        self.assertEqual(kinds_vals("L"), [(TokenKind.IDENTIFIER, "L")])
        self.assertEqual(kinds_vals("Lx"), [(TokenKind.IDENTIFIER, "Lx")])

    def test_L_space_quote_splits(self):
        self.assertEqual(kinds_vals("L 'a'"), [
            (TokenKind.IDENTIFIER, "L"),
            (TokenKind.CONSTANT, "'a'"),
        ])

    def test_adjacent_to_symbols(self):
        self.assertEqual(kinds_vals("c='a';"), [
            (TokenKind.IDENTIFIER, "c"),
            (TokenKind.SYMBOL, "="),
            (TokenKind.CONSTANT, "'a'"),
            (TokenKind.SYMBOL, ";"),
        ])


class TestStringLiterals(unittest.TestCase):
    def test_empty(self):
        self.assertEqual(kinds_vals('""'), [(TokenKind.STRING_LITERAL, '""')])
        self.assertEqual(kinds_vals('L""'), [(TokenKind.STRING_LITERAL, 'L""')])

    def test_basic(self):
        for s in ['"a"', '"hello"', '"Hello, world!"', '"123"', '"{}"', '"  "']:
            with self.subTest(s=s):
                self.assertEqual(kinds_vals(s), [(TokenKind.STRING_LITERAL, s)])

    def test_wide(self):
        self.assertEqual(kinds_vals('L"hello"'),
                         [(TokenKind.STRING_LITERAL, 'L"hello"')])

    def test_single_quote_inside_is_literal(self):
        # Single quote is not excluded from s-char; allowed unescaped.
        s = "\"it's\""
        self.assertEqual(kinds_vals(s), [(TokenKind.STRING_LITERAL, s)])

    def test_escaped_double_quote(self):
        s = r'"\"a\""'
        self.assertEqual(kinds_vals(s), [(TokenKind.STRING_LITERAL, s)])

    def test_escape_sequences(self):
        for s in [r'"\n"', r'"\t"', r'"\\"', r'"\?"', r'"\x41"', r'"\123"',
                  "\"\\u0041\"", "\"\\U0001F600\""]:
            with self.subTest(s=s):
                self.assertEqual(kinds_vals(s), [(TokenKind.STRING_LITERAL, s)])

    def test_unterminated(self):
        for s in ['"', '"abc', 'L"', r'"\n']:
            with self.subTest(s=s):
                with self.assertRaises(LexError):
                    list(tokenize(s))

    def test_real_newline_inside_is_error(self):
        with self.assertRaises(LexError):
            list(tokenize('"a\nb"'))

    def test_invalid_escape_is_error(self):
        for s in [r'"\q"', r'"\9"', r'"\Q"']:
            with self.subTest(s=s):
                with self.assertRaises(LexError):
                    list(tokenize(s))

    def test_non_printable_is_error(self):
        for s in ['"a\tb"', '"a\x01b"', '"a\x7fb"']:
            with self.subTest(s=repr(s)):
                with self.assertRaises(LexError):
                    list(tokenize(s))

    def test_non_ascii_is_error(self):
        with self.assertRaises(LexError):
            list(tokenize('"café"'))

    def test_L_alone_still_identifier(self):
        self.assertEqual(kinds_vals("Lx"), [(TokenKind.IDENTIFIER, "Lx")])

    def test_L_space_quote_splits(self):
        self.assertEqual(kinds_vals('L "a"'), [
            (TokenKind.IDENTIFIER, "L"),
            (TokenKind.STRING_LITERAL, '"a"'),
        ])

    def test_adjacent_literals_are_separate_tokens(self):
        # Concatenation happens at translation-phase-6 / parse time, not here.
        self.assertEqual(kinds_vals('"abc""def"'), [
            (TokenKind.STRING_LITERAL, '"abc"'),
            (TokenKind.STRING_LITERAL, '"def"'),
        ])

    def test_in_function_call(self):
        self.assertEqual(kinds_vals('puts("hi");'), [
            (TokenKind.IDENTIFIER, "puts"),
            (TokenKind.SYMBOL, "("),
            (TokenKind.STRING_LITERAL, '"hi"'),
            (TokenKind.SYMBOL, ")"),
            (TokenKind.SYMBOL, ";"),
        ])

    def test_distinct_from_char_constant(self):
        # Same-looking source gets different token kinds.
        self.assertEqual(next(iter(tokenize("'a'"))).kind, TokenKind.CONSTANT)
        self.assertEqual(next(iter(tokenize('"a"'))).kind, TokenKind.STRING_LITERAL)


class TestNoWhitespaceRequired(unittest.TestCase):
    def test_tight_assignment(self):
        expected = [
            (TokenKind.KEYWORD, "int"),
            (TokenKind.IDENTIFIER, "x"),
            (TokenKind.SYMBOL, "="),
            (TokenKind.CONSTANT, "42"),
            (TokenKind.SYMBOL, ";"),
        ]
        self.assertEqual(kinds_vals("int x=42;"), expected)
        self.assertEqual(kinds_vals("int x = 42 ;"), expected)

    def test_glued_arithmetic(self):
        self.assertEqual(vals("1+2*3"), ["1", "+", "2", "*", "3"])

    def test_keyword_adjacent_to_symbol(self):
        self.assertEqual(vals("return;"), ["return", ";"])

    def test_identifier_adjacent_to_symbol(self):
        self.assertEqual(vals("x->y"), ["x", "->", "y"])


class TestWhitespace(unittest.TestCase):
    def test_all_whitespace_chars_skipped(self):
        self.assertEqual(vals(" \t\n\fint\f\n\t x"), ["int", "x"])


class TestPositions(unittest.TestCase):
    def test_first_token_is_1_1(self):
        t = next(iter(tokenize("int")))
        self.assertEqual((t.line, t.col), (1, 1))

    def test_column_after_spaces(self):
        t = next(iter(tokenize("  int")))
        self.assertEqual((t.line, t.col), (1, 3))

    def test_line_and_col_reset_after_newline(self):
        ts = list(tokenize("int\nx"))
        self.assertEqual((ts[0].line, ts[0].col), (1, 1))
        self.assertEqual((ts[1].line, ts[1].col), (2, 1))

    def test_tab_advances_one_column(self):
        t = next(iter(tokenize("\tx")))
        self.assertEqual((t.line, t.col), (1, 2))

class TestErrors(unittest.TestCase):
    def test_unexpected_char(self):
        for s in ["@", "`", "$"]:
            with self.subTest(s=s):
                with self.assertRaises(LexError):
                    list(tokenize(s))

    def test_error_carries_position(self):
        with self.assertRaises(LexError) as cm:
            list(tokenize("int\n  @"))
        self.assertEqual((cm.exception.line, cm.exception.col), (2, 3))


class TestSampleProgram(unittest.TestCase):
    def test_full_snippet(self):
        src = (
            "int main(void) {\n"
            "    int x = 42;\n"
            "    return x + 0xDEADBEEFull;\n"
            "}\n"
        )
        self.assertEqual(kinds_vals(src), [
            (TokenKind.KEYWORD, "int"),
            (TokenKind.IDENTIFIER, "main"),
            (TokenKind.SYMBOL, "("),
            (TokenKind.KEYWORD, "void"),
            (TokenKind.SYMBOL, ")"),
            (TokenKind.SYMBOL, "{"),
            (TokenKind.KEYWORD, "int"),
            (TokenKind.IDENTIFIER, "x"),
            (TokenKind.SYMBOL, "="),
            (TokenKind.CONSTANT, "42"),
            (TokenKind.SYMBOL, ";"),
            (TokenKind.KEYWORD, "return"),
            (TokenKind.IDENTIFIER, "x"),
            (TokenKind.SYMBOL, "+"),
            (TokenKind.CONSTANT, "0xDEADBEEFull"),
            (TokenKind.SYMBOL, ";"),
            (TokenKind.SYMBOL, "}"),
        ])


if __name__ == "__main__":
    unittest.main()
