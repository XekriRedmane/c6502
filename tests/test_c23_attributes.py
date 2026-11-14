"""C23 attribute syntax (§6.7.13): where `[[...]]` may go, what c6502
does with its own `c6502::` attributes, the standard ones, and other
prefixes' (ignored), and the errors for the rest."""

from __future__ import annotations

import unittest

from lark.exceptions import UnexpectedInput

import c99_ast
from parser import ParserError, parse


def _decl(src: str, index: int = -1):
    d = parse(src).declaration[index]
    return d.function_decl if isinstance(d, c99_ast.FunctionDecl) else d.var_decl


class TestPlacement(unittest.TestCase):

    def test_before_and_after_the_name_mean_the_same(self) -> None:
        for src in (
            "[[c6502::zp_abi]] void f(void);",
            "void f [[c6502::zp_abi]] (void);",
            "[[c6502::zp_abi]] void f(void) {}",
            "void f [[c6502::zp_abi]] (void) {}",
        ):
            with self.subTest(src=src):
                self.assertEqual(_decl(src).abi_annotation, "zp_abi")

    def test_several_lists_and_several_attributes(self) -> None:
        d = _decl(
            "[[c6502::zp_abi]] [[c6502::preserves_x, c6502::address(0xFC58)]]"
            " void HOME(void);"
        )
        self.assertEqual(
            (d.abi_annotation, d.address, d.preserves),
            ("zp_abi", 0xFC58, ["X"]),
        )

    def test_after_an_array_name(self) -> None:
        d = _decl("extern unsigned char z [[c6502::zeropage(0x8A)]][3];")
        self.assertEqual(d.placement, c99_ast.ZeroPage(address=0x8A))
        self.assertEqual(
            d.data_type, c99_ast.Array(element_type=c99_ast.UChar(), size=3),
        )

    def test_after_a_name_applies_to_that_declarator_only(self) -> None:
        prog = parse("unsigned char n [[c6502::zeropage]], m;")
        n, m = (d.var_decl for d in prog.declaration)
        self.assertEqual(n.placement, c99_ast.ZeroPage(address=None))
        self.assertIsNone(m.placement)

    def test_leading_applies_to_every_declarator(self) -> None:
        prog = parse("[[c6502::zeropage]] unsigned char n, m;")
        self.assertEqual(
            [d.var_decl.placement for d in prog.declaration],
            [c99_ast.ZeroPage(address=None)] * 2,
        )

    def test_inside_a_parenthesized_declarator(self) -> None:
        d = _decl("void (*fp [[c6502::zeropage]])(void);")
        self.assertEqual(d.placement, c99_ast.ZeroPage(address=None))

    def test_parameters(self) -> None:
        d = _decl(
            "void g(unsigned char a [[c6502::in_x]],"
            " [[c6502::in_y]] unsigned char b, [[c6502::in_a]] unsigned char);"
        )
        self.assertEqual(d.param_registers, ["X", "Y", "A"])

    def test_packed_enum(self) -> None:
        d = _decl("enum [[c6502::packed]] e { A }; enum e v;")
        self.assertEqual(d.data_type, c99_ast.UChar())

    def test_double_underscore_spelling(self) -> None:
        self.assertEqual(
            _decl("[[__c6502__::__zp_abi__]] void f(void);").abi_annotation,
            "zp_abi",
        )

    def test_space_between_the_brackets(self) -> None:
        self.assertEqual(
            _decl("[ [c6502::zp_abi] ] void f(void);").abi_annotation,
            "zp_abi",
        )

    def test_array_subscripts_still_parse(self) -> None:
        parse(
            "int f(int i) { int a[3]; int b[2][2];"
            " a[0] = b[1][i]; return a[b[0][0]]; }"
        )


class TestStandardAndForeign(unittest.TestCase):

    def test_accepted_and_ignored(self) -> None:
        for src in (
            "enum e { A [[deprecated]], B = 3 };",
            "struct [[deprecated(\"old\")]] s"
            " { [[maybe_unused]] int a; int b [[deprecated]]; };",
            "[[deprecated]] typedef int T; T x;",
            "[[noreturn]] void die(void);",
            "[[_Noreturn]] void die(void);",
            "[[nodiscard(\"check it\")]] int get(void);",
            "void f([[maybe_unused]] int x) {}",
            "typedef int T; void k(void) { [[maybe_unused]] T * p;"
            " for ([[maybe_unused]] int i = 0; i < 2; i++) {} }",
            "int sw(int x) { switch (x) { case 1: x++; [[fallthrough]];"
            " case 2: return x; } return 0; }",
            "[[]] int z; [[,,]] int w;",
        ):
            with self.subTest(src=src):
                parse(src)

    def test_other_prefixes_and_unknown_names_are_ignored(self) -> None:
        d = _decl(
            "[[gnu::aligned(sizeof(int)), gnu::format(printf, 1, 2),"
            " clang::x([1], (2))]] [[unknown_thing]] int y;"
        )
        self.assertIsNone(d.placement)

    def test_a_struct_body_after_attributes_isnt_a_block(self) -> None:
        # The lexer opens a scope at a block's `{`, not a struct's, so
        # an enumeration constant declared in the body outlives it.
        f = _decl(
            "struct [[deprecated]] s { enum { A, B } k; };"
            " int f(void) { return B; }"
        )
        self.assertEqual(
            f.body.block_item[0].statement.exp,
            c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
        )


class TestErrors(unittest.TestCase):

    def test_errors(self) -> None:
        for src, message in [
            ("[[zp_abi]] void f(void);", "`[[c6502::zp_abi]]`"),
            ("[[c6502::bogus]] void f(void);", "unknown attribute"),
            ("[[c6502::zp_abi(1)]] void f(void);", "doesn't take an argument"),
            ("[[c6502::zp_abi()]] void f(void);", "empty parentheses"),
            ("[[c6502::address(1, 2)]] extern int a;", "one integer constant"),
            ("[[c6502::zeropage]] void f(void);", "places an object"),
            ("[[c6502::in_x]] void f(void);", "doesn't apply to function"),
            ("[[c6502::zeropage]] unsigned char n [[c6502::zeropage]];",
             "given twice"),
            ("[[noreturn]] int x;", "can't apply to an object"),
            ("[[unsequenced]] int f(void);", "function's type"),
            ("[[deprecated, deprecated]] int d;", "appears twice"),
            ("[[deprecated(1)]] int d;", "takes a string literal"),
            ("[[maybe_unused(\"x\")]] int d;", "doesn't take an argument"),
            ("[[fallthrough]];", "switch statement"),
            ("[[c6502::zp_abi]];", "attribute declaration"),
            ("[[c6502::zp_abi]] struct s;", "need a declarator"),
            ("struct [[c6502::packed]] s { int a; };", "doesn't apply to a struct"),
            ("enum e { A [[c6502::packed]] };", "enumeration constant"),
            ("struct s { int a [[c6502::zeropage]]; };", "member 'a'"),
            ("[[c6502::zeropage]] typedef int T;", "typedef 'T'"),
            ("__attribute__((zp_abi)) void f(void);", "C23 attributes"),
            ("unsigned char n __attribute__((zeropage));", "C23 attributes"),
        ]:
            with self.subTest(src=src):
                with self.assertRaises(ParserError) as ctx:
                    parse(src)
                self.assertIn(message, str(ctx.exception))

    def test_type_positions_arent_parsed(self) -> None:
        # C23 applies these to a type, where c6502 takes no attributes.
        for src in (
            "void f(void) [[c6502::zp_abi]];",
            "int [[c6502::zeropage]] x;",
            "int * [[c6502::zeropage]] p;",
            "int a[3] [[c6502::zeropage]];",
        ):
            with self.subTest(src=src):
                with self.assertRaises(UnexpectedInput):
                    parse(src)


if __name__ == "__main__":
    unittest.main()
