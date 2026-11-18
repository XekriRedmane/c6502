"""Tests for C23's `auto` (§6.7.2, §6.7.10): with no type specifier it
infers an object's type from its initializer, and with one it's C99's
automatic storage class, which C23 allows only at block scope. Most
cases are the standard's own examples."""

from __future__ import annotations

import shutil
import unittest

import c99_ast
import compile as compile_module
from parser import ParserError
from passes.type_checking import TypeCheckError
from preprocessor import preprocess
from sim.harness import build_sim
from tests.test_sim_chapters import _run_program as _run_tac


def _types(src: str) -> dict:
    """The type of each object `src` declares, by the name the program
    spells it with (the last one, for a name declared twice)."""
    _, symbols, _ = compile_module.type_check_program(
        compile_module._resolved(
            preprocess("#include <stddef.h>\n" + src), optimize=False,
        ),
    )
    out = {}
    for name, sym in symbols.items():
        out[name.split(".", 1)[1] if name.startswith("@") else name] = sym.type
    return out


def _ptr(t):
    return c99_ast.Pointer(referenced_type=t)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestInference(unittest.TestCase):
    def test_file_scope(self) -> None:
        # EXAMPLE 1: `a` is a double and `p` a double *.
        t = _types("static auto a = 3.5; auto p = &a;")
        self.assertEqual(t["a"], c99_ast.Double())
        self.assertEqual(t["p"], _ptr(c99_ast.Double()))

    def test_arrays_decay(self) -> None:
        # EXAMPLE 3: an array converts to a pointer, and `&A` is a
        # pointer to the array.
        t = _types("double A[3] = { 0 }; auto pA = A; auto qA = &A;"
                   " auto s = \"abc\";")
        self.assertEqual(t["pA"], _ptr(c99_ast.Double()))
        self.assertEqual(t["qA"], _ptr(c99_ast.Array(
            element_type=c99_ast.Double(), size=3,
        )))
        self.assertEqual(t["s"], _ptr(c99_ast.Char()))

    def test_lvalue_conversion_and_qualifiers(self) -> None:
        # Lvalue conversion drops the initializer's qualifiers and
        # doesn't promote, and the declaration's own qualifiers apply.
        t = _types(
            "const unsigned char c = 3;"
            "int f(void) { auto d = c; auto e = c + 1;"
            " const auto k = 5; volatile auto v = 'x';"
            " return d + e + k + v; }"
        )
        self.assertEqual(t["d"], c99_ast.UChar())
        self.assertEqual(t["e"], c99_ast.Int())
        self.assertEqual(t["k"], c99_ast.Const(referenced_type=c99_ast.Int()))
        self.assertEqual(
            t["v"], c99_ast.Volatile(referenced_type=c99_ast.Int()),
        )

    def test_other_types(self) -> None:
        t = _types(
            "int g(int); struct pt { int x, y; };"
            "auto fp = g; auto n = nullptr; auto b = true;"
            "int f(void) { auto P = (struct pt){1, 2}; auto Y = P.y;"
            " return Y; }"
        )
        self.assertEqual(t["fp"], _ptr(c99_ast.FunType(
            params=[c99_ast.Int()], ret=c99_ast.Int(),
        )))
        self.assertEqual(t["n"], c99_ast.NullptrT())
        self.assertEqual(t["b"], c99_ast.Bool())
        self.assertEqual(t["P"], c99_ast.Structure(tag="pt"))
        self.assertEqual(t["Y"], c99_ast.Int())

    def test_with_other_storage_classes(self) -> None:
        t = _types(
            "constexpr auto K = 7; int arr[K];"
            "int f(void) { static auto s = 5;"
            " register auto r = (unsigned char)4; return s + r; }"
        )
        self.assertEqual(t["K"], c99_ast.Const(referenced_type=c99_ast.Int()))
        self.assertEqual(t["arr"], c99_ast.Array(
            element_type=c99_ast.Int(), size=7,
        ))
        self.assertEqual(t["s"], c99_ast.Int())
        self.assertEqual(t["r"], c99_ast.UChar())

    def test_storage_class(self) -> None:
        # With a type specifier, `auto` is the storage class an object
        # in a block has anyway.
        t = _types("int f(void) { auto int x = 5; auto long y; return x; }")
        self.assertEqual(t["x"], c99_ast.Int())
        self.assertEqual(t["y"], c99_ast.Long())

    def test_errors(self) -> None:
        for src, message in (
            # EXAMPLE 2: the name isn't in scope in its own
            # initializer, and hides an outer one there.
            ("int f(void) { double a = 7; { auto a = a * a; return 0; } }",
             "uses the name `a`"),
            ("auto int x = 5;", "`auto` at file scope only marks"),
            ("int f(void) { static auto int x = 5; return x; }",
             "`auto` goes with `static` only when it infers"),
            ("auto x;", "has no initializer"),
            ("auto x = {1};", "a list in braces"),
            ("auto *p = 0;", "a plain name"),
            ("auto a = 1, b = 2;", "declares one name"),
            ("auto f(void) { return 1; }", "function definition"),
            ("int f(void) { auto int g(void); return 0; }",
             "since a function has no automatic storage"),
            ("void f(auto int x);", "isn't permitted on parameter"),
            ("typedef auto T = 1;", "can't go with `typedef`"),
            ("auto auto x = 1;", "appears twice"),
            ("int f(void) { auto x = undeclared; return x; }",
             "the initializer of `auto` object `x`"),
        ):
            with self.subTest(src=src):
                with self.assertRaises(
                    (ParserError, TypeCheckError, NotImplementedError),
                ) as cm:
                    _types(src)
                self.assertIn(message, str(cm.exception))


_PROGRAM = """\
static auto a = 3.5;
auto p = &a;

int count(void) {
    static auto n = 0;
    return ++n;
}

int main(void) {
    double x = 7;
    double y = 9;
    {
        auto y = x * x;
        auto x = y;
        if (x != 49.0) return 1;
    }
    if (*p != 3.5) return 2;
    auto u = (unsigned char)250;
    u += 10;
    if (u != 4) return 3;
    auto s = 0L;
    for (auto i = 0; i < 300; i++)
        s += i;
    if (s != 44850L) return 4;
    count();
    count();
    if (count() != 3) return 5;
    auto int z = 8;
    return z - 8;
}
"""


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestPrograms(unittest.TestCase):
    """`u` is an `unsigned char`, so it wraps at 256, and `s` a `long`,
    so the sum of 0 to 299 fits."""

    def test_tac_sim(self) -> None:
        self.assertEqual(_run_tac(preprocess(_PROGRAM)), 0)

    def test_asm_sim(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(_PROGRAM, optimize=optimize).run(
                    max_cycles=4_000_000,
                )
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int_signed(), 0)


if __name__ == "__main__":
    unittest.main()
