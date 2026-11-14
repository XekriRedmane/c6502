"""Enumerations (C99 §6.7.2.2): the parser's enumeration constants
and enum tags, and programs run through the TAC simulator and the asm
simulator with and without `--optimize`."""

from __future__ import annotations

import shutil
import unittest

import c99_ast
from parser import ParserError, parse
from sim.harness import build_sim
from tests.test_sim_chapters import _run_program as _run_tac


def _return_exp(src: str):
    prog = parse(src)
    body = prog.declaration[-1].function_decl.body.block_item
    return body[-1].statement.exp


def _int(value: int):
    return c99_ast.Constant(const=c99_ast.ConstInt(value=value & 0xFFFF))


class TestParse(unittest.TestCase):

    def test_values(self) -> None:
        src = ("enum { A, B, C = 10, D, E = C * 2 - 1, F = -2, G };"
               "int main(void) { return %s; }")
        for name, value in [("A", 0), ("B", 1), ("C", 10), ("D", 11),
                            ("E", 19), ("F", -2), ("G", -1)]:
            with self.subTest(name=name):
                self.assertEqual(_return_exp(src % name), _int(value))

    def test_type_is_int(self) -> None:
        prog = parse("enum e { A }; enum e x; typedef enum { B } t; t y;")
        types = [d.var_decl.data_type for d in prog.declaration]
        self.assertEqual(types, [c99_ast.Int(), c99_ast.Int()])

    def test_definition_alone_declares_nothing_in_the_ast(self) -> None:
        self.assertEqual(parse("enum e { A, B, };").declaration, [])

    def test_array_size(self) -> None:
        prog = parse("enum { N = 3 }; int a[N * 2];")
        self.assertEqual(prog.declaration[0].var_decl.data_type,
                         c99_ast.Array(element_type=c99_ast.Int(), size=6))

    def test_scope(self) -> None:
        # An inner declaration hides a constant, and an inner enum's
        # constants end with their block.
        src = """
            enum { A = 1 };
            int main(void) {
                { int A = 5; A; }
                { enum { A = 2 }; A; }
                return A;
            }
        """
        body = parse(src).declaration[-1].function_decl.body.block_item
        self.assertIsInstance(
            body[0].statement.block.block_item[1].statement.exp, c99_ast.Var,
        )
        self.assertEqual(
            body[1].statement.block.block_item[0].statement.exp, _int(2),
        )
        self.assertEqual(body[2].statement.exp, _int(1))

    def test_constant_hides_typedef_name(self) -> None:
        exp = _return_exp(
            "typedef int T; int main(void) { enum { T = 3 }; return T * 2; }"
        )
        self.assertIsInstance(exp, c99_ast.Binary)
        self.assertEqual(exp.left, _int(3))

    def test_errors(self) -> None:
        for src, message in [
            ("enum E { A }; enum E { B };", "defined twice"),
            ("enum E x;", "isn't defined"),
            ("enum { A }; int A;", "declared again"),
            ("enum { A, A };", "declared again"),
            ("typedef int T; enum { T };", "declared again"),
            ("enum { A = 32768 };", "doesn't fit in an int"),
            ("enum { A = 32767, B };", "doesn't fit in an int"),
            ("int x; enum { A = x };", "integer constant expression"),
            ("struct S { int a; }; enum S { A };", "struct or union tag"),
            ("enum S { A }; struct S *p;", "enum tag"),
            ("enum S { A }; struct S { int x; };", "enum tag"),
            ("enum E { A }; unsigned enum E x;", "cannot combine"),
            ("enum { A }; int f(void) { return A(); }", "not a function"),
        ]:
            with self.subTest(src=src):
                with self.assertRaises(ParserError) as ctx:
                    parse(src)
                self.assertIn(message, str(ctx.exception))

    def test_packed_type(self) -> None:
        for src, expected in [
            ("enum [[c6502::packed]] E { A, B = 255 }; enum E x;",
             c99_ast.UChar()),
            ("enum [[c6502::packed]] E { A = -128, B = 127 };"
             " enum E x;", c99_ast.SChar()),
            ("enum [[c6502::packed]] { A = -1, B = 200 } x;",
             c99_ast.Int()),
            ("typedef enum [[c6502::packed]] { A } T; T x;",
             c99_ast.UChar()),
        ]:
            with self.subTest(src=src):
                prog = parse(src)
                self.assertEqual(prog.declaration[-1].var_decl.data_type,
                                 expected)

    def test_packed_constants_are_int(self) -> None:
        self.assertEqual(_return_exp(
            "enum [[c6502::packed]] { A = 7 };"
            "int main(void) { return A; }"
        ), _int(7))

    def test_packed_errors(self) -> None:
        for src, message in [
            ("enum E { A }; enum [[c6502::packed]] E x;",
             "enum's definition"),
            ("enum [[c6502::zeropage]] { A };", "takes `c6502::packed`"),
            ("enum [[c6502::packed, c6502::packed]] { A };", "twice"),
            ("enum [[c6502::packed(1)]] { A };", "argument"),
            ("int x [[c6502::packed]];", "doesn't apply"),
            ("enum [[packed]] { A };", "c6502's prefix"),
        ]:
            with self.subTest(src=src):
                with self.assertRaises(ParserError) as ctx:
                    parse(src)
                self.assertIn(message, str(ctx.exception))

    def test_packed_after_list_is_a_syntax_error(self) -> None:
        # C23 has no place for attributes after an enum's `}`, where GCC
        # also took them.
        from lark.exceptions import UnexpectedInput
        with self.assertRaises(UnexpectedInput):
            parse("enum { A } [[c6502::packed]];")

    def test_inner_tag_hides_outer(self) -> None:
        parse("enum S { A }; void f(void) { struct S { int x; } s; }")
        parse("enum E { A }; void f(void) { enum E { B }; }")


# (name, C source, expected int return of main)
_PROGRAMS: list[tuple[str, str, int]] = [
    ("enum", """
        enum color { RED, GREEN = 5, BLUE, LAST = BLUE + 10 };
        typedef enum { OFF, ON } state;
        enum { N = 4, NEG = -3, BIG = 32767 };
        int table[N];
        struct thing { enum color c; state s; };
        struct thing things[2] = { { GREEN, ON }, { LAST, OFF } };
        enum color pick(enum color c) { return c == RED ? BLUE : RED; }
        int main(void) {
            enum color c = GREEN;
            long l = NEG;
            int r = 0;
            switch (c) {
            case RED: r = 1; break;
            case GREEN: r = 2; break;
            default: r = 3;
            }
            { int GREEN = 100; r += GREEN; }
            { enum { GREEN = 7 }; r += GREEN; }
            for (int i = 0; i < N; i++) table[i] = i * BLUE;
            r += table[3];
            r += things[1].c + things[0].s;
            r += pick(RED) + pick(c);
            r += (int)sizeof(enum color) + (int)sizeof(state)
                 + (int)sizeof RED;
            r += (int)l + (BIG == 32767) + (NEG < 0);
            r += (enum color)3;
            return r;
        }
    """, 158),
    ("enum_negative_static", """
        enum { M = -32768, P = 32767 };
        long lo = M;
        int hi = P;
        int main(void) { return (lo == -32768L) + (hi == 32767) * 2; }
    """, 3),
    ("enum_packed", """
        typedef enum [[c6502::packed]] { IDLE, RUN, STOP = 250 } state;
        enum [[c6502::packed]] dir { LEFT = -1, NONE, RIGHT };
        struct actor { state s; enum dir d; unsigned char x; };
        struct actor actors[3] = {
            { RUN, RIGHT, 10 }, { STOP, LEFT, 20 }, { IDLE, NONE, 30 },
        };
        state next(state s) { return s == IDLE ? RUN : s == RUN ? STOP : IDLE; }
        int main(void) {
            int r = 0;
            for (unsigned char i = 0; i < 3; i++) {
                struct actor *a = &actors[i];
                a->x += a->d;
                a->s = next(a->s);
                switch (a->s) {
                case IDLE: r += 1; break;
                case RUN: r += 2; break;
                case STOP: r += 4; break;
                }
                if (a->d < 0) r += 100;
            }
            r += sizeof(struct actor) * 1000;
            r += actors[0].x + actors[1].x + actors[2].x;
            r += (actors[1].s == IDLE) * 10000;
            return r;
        }
    """, 13167),
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
