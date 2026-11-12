"""`short`, `unsigned short`, and unary `+`: parsing, type checking,
and programs run through the TAC simulator and the asm simulator
with and without `--optimize`."""

from __future__ import annotations

import shutil
import unittest

import c99_ast
from parser import ParserError, parse
from passes.identifier_resolution import resolve_program
from passes.type_checking import TypeCheckError, check_program
from sim.harness import build_sim
from tests.test_sim_chapters import _run_program as _run_tac


def _decl_type(src: str):
    return parse(src).declaration[0].var_decl.data_type


def _check(src: str):
    prog, symbols, _types = check_program(resolve_program(parse(src)))
    return prog, symbols


def _return_exp(prog):
    body = prog.declaration[-1].function_decl.body.block_item
    return body[-1].statement.exp


class TestParse(unittest.TestCase):

    def test_short_specifiers(self) -> None:
        for src, expected in [
            ("short x;", c99_ast.Short()),
            ("short int x;", c99_ast.Short()),
            ("signed short x;", c99_ast.Short()),
            ("int short signed x;", c99_ast.Short()),
            ("unsigned short x;", c99_ast.UShort()),
            ("short unsigned int x;", c99_ast.UShort()),
        ]:
            with self.subTest(src=src):
                self.assertEqual(_decl_type(src), expected)

    def test_bad_short_combinations(self) -> None:
        for src, message in [
            ("short short x;", "more than once"),
            ("short long x;", "'short' cannot combine with 'long'"),
            ("long short int x;", "'short' cannot combine with 'long'"),
            ("short char x;", "'char' cannot combine"),
            ("short double x;", "floating type cannot combine"),
            ("_Bool short x;", "_Bool"),
        ]:
            with self.subTest(src=src):
                with self.assertRaises(ParserError) as ctx:
                    parse(src)
                self.assertIn(message, str(ctx.exception))

    def test_unary_plus(self) -> None:
        ast = parse("int main(void) { return +42; }")
        ret = ast.declaration[0].function_decl.body.block_item[0].statement
        self.assertEqual(ret.exp, c99_ast.Unary(
            op=c99_ast.Plus(),
            exp=c99_ast.Constant(const=c99_ast.ConstInt(value=42)),
        ))

    def test_binary_plus_still_parses(self) -> None:
        ast = parse("int main(void) { return 1 + +2; }")
        exp = ast.declaration[0].function_decl.body.block_item[0].statement.exp
        self.assertIsInstance(exp, c99_ast.Binary)
        self.assertEqual(exp.right.op, c99_ast.Plus())


class TestTypeCheck(unittest.TestCase):

    def test_plus_promotes(self) -> None:
        # `+c` on an unsigned char is an int, so sizeof(+c) is 2.
        for decl, expected in [
            ("unsigned char c;", c99_ast.Int()),
            ("short c;", c99_ast.Int()),
            ("unsigned short c;", c99_ast.UInt()),
            ("long c;", c99_ast.Long()),
            ("double c;", c99_ast.Double()),
        ]:
            with self.subTest(decl=decl):
                prog, _ = _check(f"{decl} int main(void) {{ +c; return 0; }}")
                stmt = prog.declaration[-1].function_decl.body.block_item[0]
                self.assertEqual(stmt.statement.exp.data_type, expected)

    def test_unsigned_short_promotes_to_unsigned_int(self) -> None:
        # int can't hold 65535, so `us + 1` is unsigned int arithmetic.
        prog, _ = _check(
            "unsigned short us; int main(void) { return us + 1 == 0; }"
        )
        cmp = _return_exp(prog)
        self.assertEqual(cmp.left.data_type, c99_ast.UInt())

    def test_short_is_not_int(self) -> None:
        # They have the same width but are different types.
        with self.assertRaises(TypeCheckError):
            _check("short *p; int *q; int main(void) { q = p; return 0; }")
        with self.assertRaises(TypeCheckError):
            _check("int f(short a); int f(int a) { return a; }")

    def test_plus_on_pointer_rejected(self) -> None:
        with self.assertRaises(TypeCheckError) as ctx:
            _check("int *p; int main(void) { +p; return 0; }")
        self.assertIn("unary '+'", str(ctx.exception))


# (name, C source, expected int return of main)
_PROGRAMS: list[tuple[str, str, int]] = [
    ("unary_plus", """
        unsigned char c = 200;
        signed char s = -5;
        int n = +7;
        double d = +1.5;
        int arr[+4];
        int main(void) {
            int a = +c;
            int b = +s;
            long l = +(long)s;
            switch (a) { case +200: a = a + 1; }
            return a + b + (int)l + n + 3 + (int)sizeof(arr)
                   + (d > 1.0) + sizeof(+c);
        }
    """, 212),
    ("short", """
        short s = -3;
        unsigned short us = 65535;
        short arr[3] = {1, -2, 30000};
        struct P { short a; unsigned short b; };
        struct P p = {-7, 40000};
        short twice(short x) { return x * 2; }
        unsigned short umax(void) { return us; }
        int main(void) {
            short int a = 100;
            signed short b = -50;
            unsigned short int c = 60000;
            long r = 0;
            r += a + b;
            r += (us + 1 == 0);
            r += (c > 0);
            r += sizeof(short) + sizeof(unsigned short) + sizeof s;
            r += twice(arr[2]) == -5536;
            r += p.a + (p.b == 40000);
            r += (unsigned short)70000;
            r += umax() == 65535;
            r += s;
            for (short i = 0; i < 3; i++) r += arr[i];
            return (int)(r & 0x7FFF);
        }
    """, 1746),
    ("unsigned_short_compare", """
        unsigned short big = 40000;
        int main(void) { return (big > 30000) + (big > -1) * 2; }
    """, 1),
]


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestPrograms(unittest.TestCase):

    def test_tac_sim(self) -> None:
        for name, src, expected in _PROGRAMS:
            with self.subTest(case=name):
                v = _run_tac(src) & 0xFFFF
                self.assertEqual(v - 0x10000 if v & 0x8000 else v, expected)

    def test_asm_sim(self) -> None:
        for name, src, expected in _PROGRAMS:
            for optimize in (False, True):
                with self.subTest(case=name, optimize=optimize):
                    result = build_sim(src, optimize=optimize).run()
                    self.assertFalse(result.timed_out)
                    self.assertEqual(result.return_int_signed(), expected)


if __name__ == "__main__":
    unittest.main()
