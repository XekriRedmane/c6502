"""C23's enumerations with a fixed underlying type (§6.7.3.3), `enum
e : T { ... }` and `enum e : T;`: the types of the enum and of its
constants, the checks of each value against the type, declarations of
the tag apart from its list, and programs run through the TAC
simulator and the asm simulator with and without `-O`.

`_PROGRAM` returns 0 when every check passes, and otherwise the number
of the first check that fails."""

from __future__ import annotations

import shutil
import unittest

import c99_ast
import compile as compile_module
from parser import ParserError, parse
from preprocessor import preprocess
from sim.harness import build_sim
from tests.test_sim_chapters import _run_program as _run_tac

_U8 = "typedef unsigned char uint8_t;"


def _return_exp(src: str):
    prog = parse(src)
    body = prog.declaration[-1].function_decl.body.block_item
    return body[-1].statement.exp


def _const(cls, value: int):
    return c99_ast.Constant(const=cls(value=value))


def _cast(t, inner):
    return c99_ast.Cast(target_type=t, exp=inner)


def _last_type(src: str):
    return parse(src).declaration[-1].var_decl.data_type


def _check(src: str):
    """The type-checked program, symbols, and types of `src`, through
    the compiler's own front end."""
    return compile_module.type_check_program(compile_module._resolved(
        preprocess("#include <stdint.h>\n" + src), optimize=False,
    ))


class TestParse(unittest.TestCase):

    def test_constant_of_each_type(self) -> None:
        # A type with a constant of its own gets that constant, and the
        # others an int or unsigned int constant cast to the type.
        for t, value, expected in [
            ("int", -2, _const(c99_ast.ConstInt, 0xFFFE)),
            ("unsigned int", 40000, _const(c99_ast.ConstUInt, 40000)),
            ("long", -1, _const(c99_ast.ConstLong, 0xFFFFFFFF)),
            ("unsigned long", 70000, _const(c99_ast.ConstULong, 70000)),
            ("long long", -1,
             _const(c99_ast.ConstLongLong, 0xFFFFFFFFFFFFFFFF)),
            ("unsigned long long", 5, _const(c99_ast.ConstULongLong, 5)),
            ("unsigned char", 200, _const(c99_ast.ConstUChar, 200)),
            ("uint8_t", 200, _const(c99_ast.ConstUChar, 200)),
            ("bool", 1, _const(c99_ast.ConstBool, 1)),
            ("char", 200,
             _cast(c99_ast.Char(), _const(c99_ast.ConstInt, 200))),
            ("signed char", -3,
             _cast(c99_ast.SChar(), _const(c99_ast.ConstInt, 0xFFFD))),
            ("short", -5,
             _cast(c99_ast.Short(), _const(c99_ast.ConstInt, 0xFFFB))),
            ("unsigned short", 65535,
             _cast(c99_ast.UShort(), _const(c99_ast.ConstUInt, 65535))),
        ]:
            with self.subTest(t=t):
                self.assertEqual(_return_exp(
                    f"{_U8} enum E : {t} {{ A = {value} }};"
                    f"int main(void) {{ return A; }}"
                ), expected)

    def test_type_is_the_unqualified_underlying_type(self) -> None:
        for src, expected in [
            ("enum E : unsigned char { A }; enum E x;", c99_ast.UChar()),
            (f"{_U8} enum E : const uint8_t {{ A }}; enum E x;",
             c99_ast.UChar()),
            ("typedef const volatile long CL; enum E : CL { A }; enum E x;",
             c99_ast.Long()),
            ("enum E : unsigned long long int { A }; enum E x;",
             c99_ast.ULongLong()),
            ("enum E : signed short { A }; enum E x;", c99_ast.Short()),
            ("enum E : _Bool { A }; enum E x;", c99_ast.Bool()),
            ("int v; enum E : typeof(v) { A }; enum E x;", c99_ast.Int()),
            ("typedef enum : unsigned { A } T; T x;", c99_ast.UInt()),
            ("enum : unsigned char { A } x;", c99_ast.UChar()),
            ("const enum E : char { A } x = A;",
             c99_ast.Const(referenced_type=c99_ast.Char())),
        ]:
            with self.subTest(src=src):
                self.assertEqual(_last_type(src), expected)

    def test_values(self) -> None:
        src = (f"{_U8} enum E : uint8_t {{ A, B, C = 250, D, E1 = C - 200 }};"
               "int main(void) { return %s; }")
        for name, value in [("A", 0), ("B", 1), ("C", 250), ("D", 251),
                            ("E1", 50)]:
            with self.subTest(name=name):
                self.assertEqual(_return_exp(src % name),
                                 _const(c99_ast.ConstUChar, value))

    def test_constants_have_the_type_inside_the_list(self) -> None:
        # Each constant has the enum's type from its declaration on, so
        # `sizeof A` inside the list is the size of the type.
        self.assertEqual(_return_exp(
            "enum E : long { A, B = sizeof A, C = sizeof(enum E) * 2 };"
            "int main(void) { return C; }"
        ), _const(c99_ast.ConstLong, 8))

    def test_largest_and_least_values(self) -> None:
        for src in [
            "enum E : unsigned char { A = 255 };",
            "enum E : signed char { A = -128, B = 127 };",
            "enum E : char { A = 255 };",
            "enum E : short { A = -32768 };",
            "enum E : unsigned short { A = 65535 };",
            "enum E : unsigned { A = 65534, B };",
            "enum E : long { A = -2147483647 - 1, B = 2147483647 };",
            "enum E : unsigned long { A = 4294967295 };",
            "enum E : long long { A = -9223372036854775807LL - 1 };",
            "enum E : unsigned long long { A = 18446744073709551615ULL };",
            "enum E : bool { A, B };",
        ]:
            with self.subTest(src=src):
                parse(src)

    def test_forward_declaration(self) -> None:
        # `enum E : T;` declares a complete type, which a later
        # definition with the same type defines.
        prog = parse(
            "enum E : long; enum E x; int n = sizeof(enum E);"
            "enum E : long { A = 7 }; enum E y = A;"
        )
        decls = [d.var_decl for d in prog.declaration]
        self.assertEqual([d.data_type for d in decls],
                         [c99_ast.Long(), c99_ast.Int(), c99_ast.Long()])
        self.assertEqual(decls[2].init, _const(c99_ast.ConstLong, 7))

    def test_declaration_again(self) -> None:
        # The tag may be declared again with a compatible type, before
        # or after its definition, and `enum E` refers to it.
        for src in [
            "enum E : int; enum E : int; enum E x;",
            "enum E : int { A }; enum E : int; enum E x;",
            "enum E : const int; enum E : int { A }; enum E x;",
            "enum E : signed; enum E : int; enum E x;",
        ]:
            with self.subTest(src=src):
                self.assertEqual(_last_type(src), c99_ast.Int())

    def test_nested_enum_in_a_value(self) -> None:
        # The enum defined inside an enumerator's value is checked
        # against its own type, and the outer enum's later constants
        # against the outer type.
        for src, value in [
            ("enum E : char { A = sizeof(enum F : long { X = 100000 }),"
             " B = 250 }; int main(void) { return A; }", 4),
            ("enum E : unsigned char { A = sizeof(enum { Y = -1000 }),"
             " B = 255 }; int main(void) { return A; }", 2),
            ("enum { P = sizeof(enum G : unsigned char { Z = 255 }),"
             " Q = -1000 }; int main(void) { return P; }", 1),
        ]:
            with self.subTest(src=src):
                exp = _return_exp(src)
                inner = exp.exp if isinstance(exp, c99_ast.Cast) else exp
                self.assertEqual(inner.const.value, value)
        with self.assertRaises(ParserError) as ctx:
            parse("enum E : char { A = sizeof(enum F : long { X }), B = 300 };")
        self.assertIn("'B' is 300", str(ctx.exception))

    def test_scopes(self) -> None:
        # An inner declaration of the tag hides the outer one, whether
        # or not either has a fixed type, and an inner constant hides
        # an outer one.
        src = """
            enum E { A = 1 };
            int main(void) {
                { enum E : long; enum E x; }
                { enum E : unsigned char { A = 2 }; enum E y; A; }
                enum E z;
                return A;
            }
        """
        body = parse(src).declaration[-1].function_decl.body.block_item
        # `enum E : long;` leaves nothing in the block.
        self.assertEqual(
            body[0].statement.block.block_item[0].declaration
            .var_decl.data_type, c99_ast.Long(),
        )
        inner = body[1].statement.block.block_item
        self.assertEqual(inner[0].declaration.var_decl.data_type,
                         c99_ast.UChar())
        self.assertEqual(inner[1].statement.exp,
                         _const(c99_ast.ConstUChar, 2))
        self.assertEqual(body[2].declaration.var_decl.data_type,
                         c99_ast.Int())
        self.assertEqual(body[3].statement.exp, _const(c99_ast.ConstInt, 1))

    def test_definition_alone_declares_nothing_in_the_ast(self) -> None:
        self.assertEqual(
            parse("enum E : int; enum E : int { A, B, };").declaration, [],
        )

    def test_typedef_of_a_typedef(self) -> None:
        # A typedef name for the enum names an enumerated type, and so
        # does a typedef name for that one, but a pointer to it doesn't.
        with self.assertRaises(ParserError) as ctx:
            parse("typedef enum : int { A } T; typedef const T U;"
                  "enum F : U { B };")
        self.assertIn("'U' is one", str(ctx.exception))
        parse("typedef enum : int { A } T; typedef T *P; P p;")

    def test_errors(self) -> None:
        for src, message in [
            # Values that don't fit the type.
            (f"{_U8} enum E : uint8_t {{ A = 255, B }};",
             "'B' would be 256, one more than the constant before it, "
             "which doesn't fit in the enum's underlying type "
             "`unsigned char` (0..255"),
            ("enum E : unsigned char { A = -1 };",
             "'A' is -1, which doesn't fit in the enum's underlying "
             "type `unsigned char` (0..255"),
            ("enum E : signed char { A = 128 };",
             "`signed char` (-128..127"),
            ("enum E : char { A = -1 };", "`char` (0..255"),
            ("enum E : short { A = 32767, B };", "'B' would be 32768"),
            ("enum E : unsigned { A = 65536 };", "(0..65535"),
            ("enum E : long { A = 2147483648 };", "`long`"),
            ("enum E : unsigned long long { A = 18446744073709551615ULL, B };",
             "'B' would be 18446744073709551616"),
            ("enum E : bool { F, T, X };",
             "'X' would be 2, one more than the constant before it, "
             "which doesn't fit in the enum's underlying type `bool` "
             "(0..1"),
            ("enum E : int { A = 1.5 };", "integer constant expression"),
            # Types that can't be the underlying type.
            ("enum E : float { A };",
             "must be an integer type, and `float` isn't one"),
            ("enum E : void { A };", "`void` isn't one"),
            ("typedef int *P; enum E : P { A };", "`int *` isn't one"),
            ("enum E : _BitInt(8) { A };",
             "can't be a bit-precise integer type such as `_BitInt(8)`"),
            ("enum E : unsigned _BitInt(8) { A };", "bit-precise"),
            ("typedef _BitInt(12) B; enum E : B { A };", "bit-precise"),
            ("enum F { Q }; enum E : enum F { A };",
             "can't be an enumerated type such as `enum F`"),
            ("typedef enum F { Q } FT; enum E : FT { A };",
             "can't be an enumerated type, and 'FT' is one"),
            ("struct S { int x; }; enum E : struct S { A };",
             "`struct S` isn't one"),
            ("union U { int x; }; enum E : union U;", "`union U` isn't one"),
            ("enum E : int int { A };", "at most one 'int'"),
            # Declarations of the tag that don't agree.
            ("enum E : long; enum E : int;",
             "enum 'E' was declared with the underlying type `long`, and "
             "here with `int`, which isn't compatible with it"),
            ("enum E : char; enum E : unsigned char { A };",
             "isn't compatible"),
            ("enum E : int { A }; enum E : long;", "isn't compatible"),
            ("enum E { A }; enum E : int;",
             "enum 'E' was defined without a fixed underlying type, so "
             "it can't be declared again with one"),
            ("enum E : int; enum E { A };",
             "enum 'E' was declared with the fixed underlying type "
             "`int`, so its definition must give that type too"),
            ("enum E : int { A }; enum E : int { B };", "defined twice"),
            ("enum E : int { A }; enum E { B };", "defined twice"),
            ("enum E : int; enum E : int; enum E { B };",
             "must give that type"),
            ("enum E { A }; enum E : int { B };", "defined twice"),
            ("struct S; enum S : int;", "struct or union tag"),
            ("enum S : int; struct S *p;", "enum tag"),
            ("enum S : int; struct S { int x; };", "enum tag"),
            ("enum E x;", "isn't defined. Only an enum with a fixed "
             "underlying type can be declared before its constants"),
            ("enum E : int { A, A };", "declared again"),
            ("int A; enum E : int { A };", "declared again"),
            # `packed` belongs to enums without a fixed type.
            ("enum [[c6502::packed]] E : int { A };",
             "`[[c6502::packed]]` can't go on an enum with a fixed "
             "underlying type"),
            ("enum [[c6502::packed]] E : int;", "`enum e : uint8_t"),
            ("enum [[c6502::packed]] : unsigned char { A };",
             "can't go on an enum with a fixed underlying type"),
            ("enum [[c6502::zeropage]] E : int;",
             "takes `c6502::packed`"),
        ]:
            with self.subTest(src=src):
                with self.assertRaises(ParserError) as ctx:
                    parse(src)
                self.assertIn(message, str(ctx.exception))

    def test_typed_enum_without_its_list(self) -> None:
        # C23 allows `enum E : T` without a list only as the whole
        # declaration `enum E : T;`.
        message = ("`enum e : T` without a list of constants can only "
                   "be a whole declaration, `enum e : T;`")
        for src, token in [
            ("enum E : int x;", "'x'"),
            ("enum E : int *p;", "'*'"),
            ("enum E : int, x;", "','"),
            ("enum E : int; int f(void) { return (enum E : int)3; }",
             "')'"),
            ("enum E : int; int n = sizeof(enum E : int);", "')'"),
            ("struct S { enum E : int; };", "';'"),
            ("void f(void) { for (enum E : int; ;) {} }", "';'"),
            ("static enum E : int;", "';'"),
            ("typedef enum E : int;", "';'"),
            ("enum E : int f(void) { return 0; }", "'f'"),
        ]:
            with self.subTest(src=src):
                with self.assertRaises(ParserError) as ctx:
                    parse(src)
                self.assertIn(message, str(ctx.exception))
                self.assertIn(f"stopped at {token}", str(ctx.exception))

    def test_other_syntax_errors_keep_lark_messages(self) -> None:
        from lark.exceptions import UnexpectedInput
        for src in ["enum E : 3 { A };", "enum E : { A };"]:
            with self.subTest(src=src):
                with self.assertRaises(UnexpectedInput):
                    parse(src)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestTypes(unittest.TestCase):

    def test_sizeof_and_promotions(self) -> None:
        # `_initial` of a static int gives the value the type checker
        # folded, in the type the constants have.
        for expr, value in [
            ("sizeof(enum U8)", 1), ("sizeof M", 1), ("M + M", 400),
            ("sizeof(M + M)", 2), ("sizeof(enum Big)", 2),
            ("BIG > 32767", 1), ("BIG / 2 == 20000u", 1),
            ("sizeof(BIG + 1)", 2), ("(BIG + 0) > 0", 1),
            ("sizeof(enum Wide)", 4), ("sizeof WIDE", 4),
            ("WIDE / 1000", 1000), ("sizeof(WIDE + 1)", 4),
            ("NEG < 0", 1), ("NEG + 1", -2), ("sizeof NEG", 1),
            ("ON + ON", 2), ("sizeof ON", 1),
            ("(unsigned char)(M + M)", 144),
        ]:
            with self.subTest(expr=expr):
                _, symbols, _ = _check(
                    "enum U8 : uint8_t { M = 200 };"
                    "enum Big : unsigned int { BIG = 40000 };"
                    "enum Wide : long { WIDE = 1000000 };"
                    "enum S8 : int8_t { NEG = -3 };"
                    "enum Flag : bool { OFF, ON };"
                    f"static long v = {expr};"
                )
                self.assertEqual(symbols["v"].attrs.initial_value.value,
                                 value)

    def test_compatible_with_the_underlying_type(self) -> None:
        # The enumerated type is its underlying type, so pointers to the
        # two are the same type.
        _check("enum E : uint8_t { A }; uint8_t u; enum E *p = &u;"
               "uint8_t *q; enum E *r = 0;"
               "void f(void) { q = r; p = q; }")

    def test_typeof_and_auto(self) -> None:
        prog = parse("enum E : unsigned long { A = 5 };"
                     "typeof(A) x; void f(void) { auto y = A + 1; }")
        self.assertEqual(prog.declaration[0].var_decl.data_type,
                         c99_ast.ULong())
        y = prog.declaration[1].function_decl.body.block_item[0]
        self.assertEqual(y.declaration.var_decl.data_type, c99_ast.ULong())

    def test_constexpr_and_case_labels(self) -> None:
        _check("enum E : uint8_t { A = 200, B };"
               "constexpr uint8_t k = B; int a[A - 190];"
               "int f(uint8_t x) { switch (x) { case A: return 1;"
               " case k: return 2; } return 0; }")
        with self.assertRaises(Exception) as ctx:
            _check("enum E : uint8_t { A = 200, B };"
                   "int f(uint8_t x) { switch (x) { case A: case 200:"
                   " return 1; } return 0; }")
        self.assertIn("duplicate", str(ctx.exception).lower())


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestNarrowingCastInStaticInit(unittest.TestCase):
    """A cast to a narrower integer type in a static initializer kept
    the value it was given, so `static long v = (unsigned char)400;`
    stored 400. `test_sizeof_and_promotions` found it with `(unsigned
    char)(M + M)`."""

    def test_values(self) -> None:
        for src, want in [
            ("static long v = (unsigned char)400;", 144),
            ("static int v = (signed char)200;", -56),
            ("static const int v = (signed char)200;", -56),
            ("static unsigned v = (unsigned char)-1;", 255),
            ("static long v = (short)40000;", -25536),
            ("static int v = (bool)256;", 1),
        ]:
            program = src + " int main(void) { return v; }"
            for optimize in (False, True):
                with self.subTest(src=src, optimize=optimize):
                    result = build_sim(program, optimize=optimize).run()
                    self.assertEqual(result.return_int_signed(), want)


_PROGRAM = """
#include <stdint.h>

enum kind : uint8_t;
struct node { enum kind k; uint8_t v; };
int score(enum kind k);

enum kind : uint8_t { LEAF = 1, PAIR, LIST = 200, LAST };
enum big : unsigned int { BIG = 40000 };
enum F : uint8_t { M = 200 };
enum sc : signed char { NEG = -3, POS = 3 };
enum wide : long { MEGA = 1000000 };
enum flag : bool { OFF, ON };
enum hw : unsigned short { TOP = 65535 };

static const uint8_t weights[PAIR + 1] = { [LEAF] = 5, [PAIR] = 7 };
struct node nodes[3] = { { LEAF, 10 }, { PAIR, 20 }, { LIST, 30 } };
static enum kind last = LAST;
static int sum = M + M;

int score(enum kind k) {
    switch (k) {
    case LEAF: return 1;
    case PAIR: return 2;
    case LIST: return 4;
    default: return 8;
    }
}

enum kind next(enum kind k) { return k + 1; }

// Each part is a function of its own, so that the unoptimized
// pipeline's frames stay under 253 bytes.
int uses(void) {
    int r = 0;
    int below = 0;
    for (uint8_t i = 0; i < 3; i++) {
        r += score(nodes[i].k);
        if (nodes[i].k < LIST) below++;
    }
    if (r != 7) return 3;
    if (below != 2) return 4;
    if (weights[PAIR] != 7 || weights[LEAF] != 5) return 5;
    return 0;
}

int promotions(void) {
    if (M + M != 400 || sum != 400) return 6;
    if (!(BIG > 32767)) return 7;
    if (BIG / 3 != 13333) return 8;
    if (!(NEG < 0) || NEG + POS != 0 || sizeof NEG != 1) return 9;
    if (MEGA / 1000 != 1000 || sizeof(MEGA) != 4) return 10;
    if (ON + ON != 2 || sizeof(ON) != 1) return 11;
    if (LAST != 201 || last != 201) return 12;
    // unsigned short promotes to unsigned int, which wraps.
    if (TOP != 65535u || TOP + 1 != 0) return 21;
    if (-M != -200) return 22;
    return 0;
}

int objects(void) {
    enum kind k = LIST;
    k++;
    if (k != LAST) return 13;
    enum kind w = LAST;
    w += 60;
    if (w != 5) return 14;
    uint8_t t[4] = {0};
    t[nodes[1].k] = 9;
    if (t[PAIR] != 9) return 15;
    return 0;
}

int inferred(void) {
    auto a = M;
    typeof(BIG) b = BIG;
    if (sizeof a != 1 || b != 40000u || sizeof b != 2) return 16;
    constexpr uint8_t c = M;
    if (c != 200) return 17;
    uint8_t x = nodes[2].k;
    switch (x) {
    case LIST: break;
    default: return 18;
    }
    return 0;
}

int pointers(void) {
    uint8_t *p = &nodes[0].k;
    *p = PAIR;
    if (score(nodes[0].k) != 2) return 19;
    if (next(LIST) != LAST || next(LAST) != 202) return 20;
    return 0;
}

int main(void) {
    if (sizeof(struct node) != 2) return 1;
    if (sizeof(LEAF) != 1 || sizeof(enum kind) != 1) return 2;
    int r = uses();
    if (r == 0) r = promotions();
    if (r == 0) r = objects();
    if (r == 0) r = inferred();
    if (r == 0) r = pointers();
    return r;
}
"""


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestPrograms(unittest.TestCase):

    def test_tac_sim(self) -> None:
        self.assertEqual(_run_tac(_PROGRAM), 0)

    def test_asm_sim(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(_PROGRAM, optimize=optimize).run()
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int_signed(), 0)


if __name__ == "__main__":
    unittest.main()
