"""Tests for C23's `constexpr` storage-class specifier (§6.7.2), the
named constants it declares (§6.6p7), and the folding of floating
arithmetic in static initializers that its examples rely on.

The program in `_PROGRAM` returns 0 when every use of a named
constant gives the right value, and otherwise the number of the first
check that fails. It runs in the TAC simulator and in both 6502
pipelines. `TestStaticConsts` covers three bugs in the reads of
`static const` objects that the program found."""

from __future__ import annotations

import shutil
import unittest

import c99_ast
import compile as compile_module
from parser import ParserError, parse
from passes.type_checking import TypeCheckError
from preprocessor import preprocess
from sim.harness import build_sim
from tests.test_sim_chapters import _run_program as _run_tac


def _check(src: str):
    """The type-checked program, symbols, and types of `src`, through
    the compiler's own front end."""
    return compile_module.type_check_program(compile_module._resolved(
        preprocess("#include <stddef.h>\n" + src), optimize=False,
    ))


def _initial(src: str, name: str):
    """The value the type checker records for the static object
    `name` (a block-scope one is found by its spelling)."""
    _, symbols, _ = _check(src)
    for key, sym in symbols.items():
        if key == name or key.endswith("." + name):
            return sym.attrs.initial_value.value
    raise KeyError(name)


def _run(src: str, optimize: bool) -> int:
    result = build_sim(src, optimize=optimize).run(max_cycles=4_000_000)
    assert not result.timed_out
    return result.return_int_signed()


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestParse(unittest.TestCase):
    def test_marks_the_declaration_and_adds_const(self) -> None:
        vd = parse("constexpr int K = 47;").declaration[0].var_decl
        self.assertTrue(vd.is_constexpr)
        self.assertIsNone(vd.storage_class)
        self.assertEqual(vd.data_type, c99_ast.Const(referenced_type=c99_ast.Int()))

    def test_the_pointer_itself_is_const(self) -> None:
        # `constexpr int *p` makes p const, and leaves what it points
        # to alone.
        vd = parse("static constexpr int *p = 0;").declaration[0].var_decl
        self.assertEqual(vd.storage_class, c99_ast.Static())
        self.assertEqual(vd.data_type, c99_ast.Const(
            referenced_type=c99_ast.Pointer(referenced_type=c99_ast.Int()),
        ))

    def test_array_elements_are_const(self) -> None:
        vd = parse("constexpr int A[2] = {1, 2};").declaration[0].var_decl
        self.assertEqual(vd.data_type, c99_ast.Array(
            element_type=c99_ast.Const(referenced_type=c99_ast.Int()),
            size=2,
        ))

    def test_errors(self) -> None:
        for src, message in (
            ("constexpr int x;", "has no initializer"),
            ("extern constexpr int x = 1;", "can't go with `extern`"),
            ("constexpr typedef int T;", "can't go with `typedef`"),
            ("constexpr constexpr int x = 1;", "appears twice"),
            ("constexpr int f(void);", "declares a function"),
            ("constexpr int f(void) { return 0; }", "declares a function"),
            ("void f(constexpr int x);", "isn't permitted on parameter"),
            ("constexpr struct t { int a; };", "must declare an object"),
            ("int constexpr;", "must declare an object"),
            ("int f(void) { int constexpr = 1; return 0; }", "keyword"),
        ):
            with self.subTest(src=src):
                with self.assertRaises(ParserError) as cm:
                    parse(src)
                self.assertIn(message, str(cm.exception))


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestRules(unittest.TestCase):
    """C23 §6.7.2p5-6. Most examples are the standard's own."""

    def test_accepted(self) -> None:
        for src in (
            "constexpr unsigned int uint_max = -1U;",
            "constexpr int A = 42LL;",
            "constexpr float C = 47u;",
            "constexpr double onethird = 1.0 / 3.0;",
            "constexpr float F = 1.0f / 3.0f;",
            "constexpr double d = 0.1f;",
            "constexpr int x = (int)3.0;",
            "constexpr double D = 2.5; constexpr int x = (int)D;",
            "constexpr int *p = {};",
            "constexpr int *p = nullptr;",
            "constexpr nullptr_t N = nullptr;",
            "struct s { void *p; }; constexpr struct s A = { nullptr };"
            " constexpr struct s B = A;",
            "constexpr char s[] = \"\\xFF\";",
            "constexpr unsigned char u[] = { 255, 0 };",
            "constexpr bool b = 1;",
            "constexpr _BitInt(5) b5 = 15;",
            "int f(void) { static constexpr int S = 3;"
            " register constexpr unsigned char R = 4; return S + R; }",
            "constexpr float f = 1.0f; constexpr float g = 3.0f;"
            " constexpr float h = f / g;",
        ):
            with self.subTest(src=src):
                _check(src)

    def test_rejected(self) -> None:
        for src, message in (
            ("constexpr unsigned int minusOne = -1;",
             "has the value -1, which unsigned int can't hold exactly"),
            ("constexpr signed short B = 18446744073709551615ULL;",
             "which short can't hold exactly"),
            ("constexpr float D = 16777217;",
             "has the value 16777217, which float can't hold exactly"),
            ("constexpr float E = 1.0 / 3.0;",
             "has a double value that float can't hold exactly"),
            ("constexpr float f = 0.1;",
             "has a double value that float can't hold exactly"),
            ("constexpr int x = 3.0;",
             "an object of integer type takes an integer constant "
             "expression"),
            ("constexpr bool b = 2;", "which bool can't hold exactly"),
            ("constexpr _BitInt(5) b5 = 16;",
             "which _BitInt(5) can't hold exactly"),
            ("constexpr unsigned char u[] = { -1, 0 };",
             "which unsigned char can't hold exactly"),
            ("constexpr signed char s[] = \"\\xFF\";",
             "has the character 255, which signed char can't hold"),
            ("int g; constexpr int *p = &g;",
             "holds a pointer that isn't null"),
            ("constexpr volatile int v = 1;",
             "can't have a volatile type"),
            ("struct s { volatile int a; }; constexpr struct s S = {1};",
             "has a volatile member 'a'"),
            ("int f(void) { int y = 5; constexpr int z = y; return z; }",
             "isn't an integer constant expression"),
            ("int f(void) { constexpr int K = (int){5}; return K; }",
             "must be a constant expression"),
            ("constexpr int K = 1; void f(void) { K = 3; }",
             "const-qualified"),
            ("constexpr int K = 1; extern int K;",
             "declared again after its `constexpr` declaration"),
            ("int K; constexpr int K = 1;",
             "declared `constexpr` after another declaration"),
            # Subscripting an array isn't a constant expression (C23
            # §6.6p7 names only `.`).
            ("constexpr int A[3] = {1, 2, 3}; int arr[A[1]];",
             "integer constant expression"),
            # Only the member a union constant's initializer sets is a
            # constant (§6.6p16).
            ("union u { int a; char c; }; constexpr union u U = {1};"
             " int f(int x) { switch (x) { case U.c: return 1; }"
             " return 0; }",
             "isn't the member its initializer sets"),
        ):
            with self.subTest(src=src):
                with self.assertRaises((TypeCheckError, ParserError,
                                        NotImplementedError)) as cm:
                    _check(src)
                self.assertIn(message, str(cm.exception))


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestNamedConstants(unittest.TestCase):
    def test_in_static_initializers(self) -> None:
        src = (
            "struct in { char a; int b; };"
            "struct s { char x; struct in in; long y; };"
            "constexpr struct s S = { 3, { 4, -300 }, 70000 };"
            "constexpr int K = 47;"
            "static int b = K + 1;"
            "static long c = S.y + S.in.b;"
            "static struct s S3 = S;"
            "constexpr double D = 2.5;"
            "static int e = (int)D;"
            "constexpr double third = 1.0 / 3.0;"
            "static int i = (int)(third * 6.0);"
            "union u { unsigned w; unsigned char b[2]; };"
            "constexpr union u U = { 0x1234 };"
            "static unsigned d = U.w;"
        )
        self.assertEqual(_initial(src, "b"), 48)
        self.assertEqual(_initial(src, "c"), 70000 - 300)
        self.assertEqual(_initial(src, "S3"), (3, (4, -300), 70000))
        self.assertEqual(_initial(src, "e"), 2)
        self.assertEqual(_initial(src, "i"), 2)
        self.assertEqual(_initial(src, "d"), 0x1234)

    def test_where_the_parser_folds(self) -> None:
        # An enumeration value, an array size, and a `_BitInt` width,
        # each folded by the parser, and a member of a structure
        # constant among them.
        prog, symbols, _ = _check(
            "constexpr int N = 3;"
            "struct s { int n; }; constexpr struct s S = {4};"
            "enum e { A = N + 1 };"
            "int a[A][S.n];"
            "_BitInt(N + 2) x;"
            "void f(void) { int N = 5; int b[N]; }"
        )
        self.assertEqual(symbols.get("a").type, c99_ast.Array(
            element_type=c99_ast.Array(element_type=c99_ast.Int(), size=4),
            size=4,
        ))
        self.assertEqual(symbols.get("x").type, c99_ast.BitInt(
            width=5, is_signed=True,
        ))

    def test_a_named_zero_is_a_null_pointer_constant(self) -> None:
        # C23 §6.6p11, footnote 118.
        self.assertEqual(_initial("constexpr int Z = 0; int *p = Z;", "p"), 0)

    def test_file_scope_has_internal_linkage(self) -> None:
        # C23 §6.2.2p3.
        _, symbols, _ = _check("constexpr int K = 47;")
        self.assertFalse(symbols.get("K").attrs.is_global)


_PROGRAM = """\
#include <stdint.h>
#include <stddef.h>
constexpr int K = 47;
enum { E = K + 1 };
static int b = K + 1;
int array[K];
constexpr unsigned char UC = 200;
constexpr signed char SC = -5;
constexpr long BIG = 100000L;
constexpr bool T = true;
constexpr double D = 2.5;
constexpr float F = 47u;
constexpr int *P = nullptr;
constexpr nullptr_t N = nullptr;
struct inner { uint8_t a; int16_t b; };
struct s { uint8_t x; struct inner in; long y; };
constexpr struct s S = { 3, { 4, -300 }, 70000 };
constexpr struct s S2 = S;
static struct s S3 = S;
static long c = S.y + S.in.b;
static int e = (int)D;
static int *q = P;
constexpr char STR[] = "hi";
constexpr int ARR[3] = { 1, 2, 3 };
union u { uint16_t w; uint8_t b[2]; };
constexpr union u U = { 0x1234 };
static unsigned d = U.w;
constexpr int W = 12;
_BitInt(W) bi;
int sized[S.in.a];

int depth(int n) {
    constexpr int STEP = 2;
    const int *p = &STEP;
    if (n == 0) return *p;
    return depth(n - 1) + STEP;
}

int main(void) {
    constexpr uint8_t M = 5;
    static constexpr int SM = -7;
    uint8_t buf[M];
    for (uint8_t i = 0; i < M; i++) buf[i] = i;
    if (sizeof array != 2 * K) return 1;
    if (E != 48) return 2;
    if (b != 48) return 3;
    if (UC + SC != 195) return 4;
    if (BIG / 4 != 25000) return 5;
    if (!T) return 6;
    if ((int)(D * 2) != 5) return 7;
    if (F != 47.0f) return 8;
    if (P != 0 || q != 0) return 9;
    if (S2.in.b != -300 || S3.y != 70000) return 10;
    if (c != 70000 - 300) return 11;
    if (e != 2) return 12;
    if (STR[1] != 'i' || sizeof STR != 3) return 13;
    if (ARR[2] != 3) return 14;
    if (d != 0x1234) return 15;
    if (sizeof bi != 2 || sizeof sized != 8) return 16;
    switch (buf[4]) { case M - 1: break; default: return 17; }
    switch (SM) { case SM: break; default: return 18; }
    if (depth(3) != 8) return 19;
    const int *pk = &K;
    if (*pk != 47) return 20;
    int x = 3;
    switch (x) { case S.x: break; default: return 21; }
    if (N != nullptr) return 22;
    return 0;
}
"""


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestPrograms(unittest.TestCase):
    def test_tac_sim(self) -> None:
        self.assertEqual(_run_tac(preprocess(_PROGRAM)), 0)

    def test_asm_sim(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(_run(_PROGRAM, optimize), 0)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestStaticConsts(unittest.TestCase):
    """A `const` float or double static stored its value's integer
    part as its bits, and `-O` read a const float, double, or element
    of a const array of them as the number its bits make. Taking the
    address of a `static const` scalar left `-O` reading the storage
    that the asm const-static fold had dropped."""

    def test_float_and_double_values(self) -> None:
        for src, want in (
            ("static const double D = 2.5;"
             " int main(void) { return (int)(D * 2); }", 5),
            ("static const float D = 2.5f;"
             " int main(void) { return (int)(D * 2); }", 5),
            ("static const double D = -0.75;"
             " int main(void) { return D < 0; }", 1),
            ("static const double A[2] = {1.5, 2.5};"
             " int main(void) { return (int)(A[1] * 2); }", 5),
            ("static const float A[2] = {1.5f, 2.5f};"
             " int main(void) { return (int)(A[1] * 2); }", 5),
        ):
            for optimize in (False, True):
                with self.subTest(src=src, optimize=optimize):
                    self.assertEqual(_run(src, optimize), want)

    def test_bits_of_a_const_double(self) -> None:
        self.assertEqual(
            _initial("static const double X = 1.5;", "X"),
            0x3FF8000000000000,
        )

    def test_address_of_a_const_scalar(self) -> None:
        src = ("static const int K = 47;"
               " int main(void) { const int *pk = &K; return *pk + K; }")
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(_run(src, optimize), 94)


if __name__ == "__main__":
    unittest.main()
