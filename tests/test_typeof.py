"""Tests for C23's typeof operators, `typeof` and `typeof_unqual`
(§6.7.3.6), which the parser puts in place as the type they name.
Most cases are the standard's own examples."""

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

_INT = c99_ast.Int()
_CHAR = c99_ast.Char()


def _const(t):
    return c99_ast.Const(referenced_type=t)


def _ptr(t):
    return c99_ast.Pointer(referenced_type=t)


def _types(src: str) -> dict:
    """The type of each object and function `src` declares, by the
    name the program spells it with."""
    _, symbols, _ = compile_module.type_check_program(
        compile_module._resolved(preprocess(src), optimize=False),
    )
    out = {}
    for name, sym in symbols.items():
        out[name.split(".", 1)[1] if name.startswith("@") else name] = sym.type
    return out


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestTypes(unittest.TestCase):
    def test_type_of_an_expression(self) -> None:
        # EXAMPLE 1.
        self.assertEqual(
            _types("typeof(1+1) main(void) { return 0; }")["main"],
            c99_ast.FunType(params=[], ret=_INT),
        )

    def test_qualifiers(self) -> None:
        # EXAMPLE 2: `typeof` keeps the qualifiers, `typeof_unqual`
        # drops them, from an array's elements too, and keeps those of
        # what a pointer points to.
        t = _types(
            "const int meow = 1;"
            "const char *const animals[] = {\"aardvark\", \"bluejay\","
            " \"catte\"};"
            "typeof_unqual(meow) plain;"
            "typeof(meow) still = 2;"
            "typeof(animals) animals_array;"
            "typeof_unqual(animals) animals2_array;"
            "volatile int vi; typeof_unqual(vi) u;"
        )
        self.assertEqual(t["plain"], _INT)
        self.assertEqual(t["still"], _const(_INT))
        self.assertEqual(t["animals_array"], c99_ast.Array(
            element_type=_const(_ptr(_const(_CHAR))), size=3,
        ))
        self.assertEqual(t["animals2_array"], c99_ast.Array(
            element_type=_ptr(_const(_CHAR)), size=3,
        ))
        self.assertEqual(t["u"], _INT)

    def test_sizes(self) -> None:
        # EXAMPLE 3: a character constant is an `int`, a string literal
        # a `char` array, and a parameter declared as an array a
        # pointer.
        t = _types(
            "int main(int argc, char *argv[]) {"
            " typeof('p') a; typeof((char)'p') b; typeof(\"meow\") c;"
            " typeof(argc) d; typeof(argv) e; typeof_unqual(argv) f;"
            " return 0; }"
        )
        self.assertEqual(t["a"], _INT)
        self.assertEqual(t["b"], _CHAR)
        self.assertEqual(t["c"], c99_ast.Array(element_type=_CHAR, size=5))
        self.assertEqual(t["d"], _INT)
        self.assertEqual(t["e"], _ptr(_ptr(_CHAR)))
        self.assertEqual(t["f"], _ptr(_ptr(_CHAR)))

    def test_functions_and_type_names(self) -> None:
        t = _types(
            "int f(int);"
            "typeof(f) g;"
            "typeof(f) *fp = f;"
            "typeof(&f) fp2 = f;"
            "typeof(f(1)) r;"
            "typedef int T; typeof(T *) tp;"
            "typeof(typeof_unqual(typeof(const int))) v = 3;"
            "struct s { int a; char b[3]; } x;"
            "typeof(x.b) y; typeof(struct s) w;"
            "int h(void) { typeof(h) *self = h; return self != 0; }"
        )
        fun = c99_ast.FunType(params=[_INT], ret=_INT)
        self.assertEqual(t["g"], fun)
        self.assertEqual(t["fp"], _ptr(fun))
        self.assertEqual(t["fp2"], _ptr(fun))
        self.assertEqual(t["r"], _INT)
        self.assertEqual(t["tp"], _ptr(_INT))
        self.assertEqual(t["v"], _INT)
        self.assertEqual(t["y"], c99_ast.Array(element_type=_CHAR, size=3))
        self.assertEqual(t["w"], c99_ast.Structure(tag="s"))

    def test_inner_scope(self) -> None:
        t = _types(
            "int g; void f(void) { typeof(g) h = g;"
            " { char g = 1; typeof(g) k = g; } }"
        )
        self.assertEqual(t["h"], _INT)
        self.assertEqual(t["k"], _CHAR)

    def test_nullptr_t(self) -> None:
        t = _types("#include <stddef.h>\nnullptr_t n = nullptr;")
        self.assertEqual(t["n"], c99_ast.NullptrT())

    def test_errors(self) -> None:
        for src, message in (
            ("typeof(undeclared) x;", "undeclared identifier"),
            ("typeof(int) unsigned u;", "cannot combine"),
            ("int typeof = 1;", "`typeof` is a keyword"),
            ("int f(void) { int typeof_unqual; return 0; }",
             "`typeof_unqual` is a keyword"),
            ("struct s { int a; } x; void f(void) { struct s { char c; };"
             " typeof(x) y; }", "hides here"),
        ):
            with self.subTest(src=src):
                with self.assertRaises(
                    (ParserError, TypeCheckError, NotImplementedError),
                ) as cm:
                    _types(src)
                self.assertIn(message, str(cm.exception))


_PROGRAM = """\
int count;
int bump(void) { return ++count; }

int main(void) {
    int x = 0;
    typeof(x++) y = 5;
    typeof(bump()) z = 7;
    if (x != 0 || count != 0) return 1;
    if (y + z != 12) return 2;
    float val = 6.0f;
    int argc = 1;
    if ((typeof(typeof_unqual(typeof(argc))))val != 6) return 3;
    typeof(char[4]) four = { 1, 2, 3, 4 };
    if (sizeof four != 4 || four[3] != 4) return 4;
    return 0;
}
"""


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestPrograms(unittest.TestCase):
    """The operand isn't evaluated (C23 §6.7.3.6p4), so `x++` and the
    call of `bump` don't happen. EXAMPLE 4's nested typeof is a cast
    to `int`."""

    def test_tac_sim(self) -> None:
        self.assertEqual(_run_tac(preprocess(_PROGRAM)), 0)

    def test_asm_sim(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(_PROGRAM, optimize=optimize).run(
                    max_cycles=2_000_000,
                )
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int_signed(), 0)


if __name__ == "__main__":
    unittest.main()
