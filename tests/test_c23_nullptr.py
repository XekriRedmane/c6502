"""Tests for C23's `nullptr` and `nullptr_t` (C23 §6.2.5, §6.3.2.4,
§6.4.4.6, §6.5.10, §6.5.16, §6.5.17.2, §7.21.2) and for <stddef.h>.

Coverage:

  * `nullptr` is a keyword and a constant of type `nullptr_t`, which
    `typeof_unqual(nullptr)` names, and a name spelled `nullptr` is an
    error.
  * Each conversion the standard allows and each one it rejects, by
    assignment, initialization, argument, return, and cast, with the
    messages.
  * `==` and `!=` with a `nullptr_t` operand, the relational operators
    and arithmetic that reject one, and `?:`.
  * `sizeof(nullptr)` and `sizeof(nullptr_t)` are 2, and `nullptr`
    works in static initializers but isn't an integer constant.
  * `nullptr` reaches the TAC as the constant 0.
  * <stddef.h> includes without a diagnostic and gives `NULL`,
    `nullptr_t`, `size_t`, `ptrdiff_t`, and an `offsetof` that is a
    constant.
  * A program using all of these runs to the same result on the TAC
    interpreter and in the simulator, in both pipelines.

c6502 has no `_Generic`, so the type of `nullptr` is checked through
the type checker's `data_type` instead.
"""
from __future__ import annotations

import io
import shutil
import unittest
from unittest.mock import patch

import c99_ast
import diagnostics
import tac_ast
from c99_to_tac import translate_program
from compile import main as compile_main
from parser import ParserError, parse
from passes.identifier_resolution import resolve_program
from passes.type_checking import TypeCheckError, check_program
from preprocessor import preprocess
from sim.harness import build_sim
from tests.test_sim_chapters import _run_program as _run_tac

# What <stddef.h> declares `nullptr_t` as, for the tests that don't
# include it.
_NULLPTR_T = "typedef typeof_unqual(nullptr) nullptr_t;\n"


def _check(src: str):
    return check_program(resolve_program(parse(preprocess(src))))


def _static_value(src: str, name: str) -> int:
    _, symbols, _ = _check(src)
    return symbols[name].attrs.initial_value.value


def _compile(src: str, *flags: str) -> tuple[int, str]:
    """compile.py's exit status and stderr for `src` on stdin."""
    err = io.StringIO()
    with patch("sys.stdin", io.StringIO(src)), patch("sys.stderr", err), \
            patch("sys.stdout", io.StringIO()):
        rc = compile_main(["compile.py", "-", "-S", *flags])
    return rc, err.getvalue()


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestKeywords(unittest.TestCase):
    def test_nullptr_is_a_constant(self) -> None:
        prog = parse("int *p = nullptr;")
        self.assertEqual(
            prog.declaration[0].var_decl.init,
            c99_ast.Constant(const=c99_ast.ConstNullptr()),
        )

    def test_typeof_names_nullptr_t(self) -> None:
        prog = parse(
            "typeof(nullptr) n; const typeof_unqual(nullptr) m;"
        )
        self.assertEqual(
            [d.var_decl.data_type for d in prog.declaration],
            [c99_ast.NullptrT(),
             c99_ast.Const(referenced_type=c99_ast.NullptrT())],
        )

    def test_type_of_nullptr(self) -> None:
        prog, _, _ = _check("void f(void) { nullptr; }")
        exp = prog.declaration[0].function_decl.body.block_item[0] \
            .statement.exp
        self.assertEqual(exp.data_type, c99_ast.NullptrT())

    def test_nullptr_t_takes_no_other_specifier(self) -> None:
        with self.assertRaises(ParserError) as cm:
            parse("typeof(nullptr) unsigned n;")
        self.assertEqual(
            str(cm.exception),
            "`typeof` cannot combine with another type specifier",
        )

    def test_names_are_errors(self) -> None:
        for src in (
            "int nullptr;",
            "int nullptr = 0;",
            "int *nullptr(void);",
            "int f(int nullptr) { return 0; }",
            "struct s { int nullptr; };",
            "struct nullptr { int x; };",
            "enum { nullptr };",
        ):
            with self.subTest(src=src):
                with self.assertRaises(ParserError) as cm:
                    parse(src)
                self.assertIn("`nullptr` is a keyword in C23 (§6.4.1)",
                              str(cm.exception))

    def test_nullptr_t_is_an_ordinary_name_without_stddef(self) -> None:
        # Only <stddef.h> declares `nullptr_t` (C23 §7.21.2).
        _check("int nullptr_t = 3;")

    def test_if_treats_nullptr_as_an_identifier(self) -> None:
        # C23 §6.10.2p13 replaces every identifier left in an `#if`
        # expression, keywords included, with 0, except `true`.
        text = preprocess("#if nullptr\nint a;\n#else\nint b;\n#endif\n")
        self.assertNotIn("int a;", text)
        self.assertIn("int b;", text)


def _rejects(test: unittest.TestCase, src: str, message: str) -> None:
    with test.subTest(src=src):
        with test.assertRaises(TypeCheckError) as cm:
            _check(_NULLPTR_T + src)
        test.assertEqual(str(cm.exception), message)


def _to_int(what: str) -> str:
    return (
        f"cannot convert nullptr_t to {what}: a nullptr_t value converts "
        f"implicitly only to a pointer type or bool (C23 §6.3.2.4, "
        f"§6.5.17.2)"
    )


def _to_nullptr_t(what: str) -> str:
    return (
        f"cannot convert {what} to nullptr_t: only nullptr, another "
        f"nullptr_t value, or a null pointer constant converts to "
        f"nullptr_t (C23 §6.3.2.4, §6.5.17.2)"
    )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestConversions(unittest.TestCase):
    """C23 §6.3.2.4: "The type nullptr_t may be converted to void, bool
    or to a pointer type; the result is a void expression, false, or a
    null pointer value, respectively. A null pointer constant or value
    of type nullptr_t may be converted to nullptr_t." §6.5.17.2p1
    allows the same conversions by assignment, and §6.5.5p4 by
    cast."""

    def test_allowed(self) -> None:
        for src in (
            "int *p = nullptr;",
            "void (*fp)(void) = nullptr;",
            "void *v = nullptr;",
            "const char *s = nullptr;",
            "bool b = nullptr;",
            "nullptr_t n = nullptr;",
            "nullptr_t n = 0;",
            "nullptr_t n = 1 - 1;",
            "nullptr_t n = false;",
            "nullptr_t n = (void *)0;",
            "void f(int *p); void g(void) { f(nullptr); }",
            "void f(bool b); void g(void) { f(nullptr); }",
            "void f(nullptr_t n); void g(void) { f(nullptr); f(0); }",
            "int *f(void) { return nullptr; }",
            "nullptr_t f(void) { return 0; }",
            "nullptr_t f(void) { return nullptr; }"
            " int *g(void) { return f(); }",
            "void f(void) { nullptr_t n; int *p; p = n; n = nullptr;"
            " n = 0; }",
            "void f(void) { (void)nullptr; }",
            "bool f(void) { return (bool)nullptr; }",
            "char *f(void) { return (char *)nullptr; }",
            "nullptr_t f(nullptr_t n) { return (nullptr_t)n; }",
            "nullptr_t f(void) { return (nullptr_t)0; }",
            "nullptr_t f(void) { return (nullptr_t)(void *)0; }",
            "struct s { nullptr_t n; int *p; };"
            " struct s v = { nullptr, nullptr };",
            "nullptr_t a[3];",
        ):
            with self.subTest(src=src):
                _check(_NULLPTR_T + src)

    def test_implicit_conversions_rejected(self) -> None:
        _rejects(self, "int x = nullptr;", _to_int("int"))
        _rejects(self, "long f(void) { return nullptr; }", _to_int("long"))
        _rejects(self, "void f(unsigned char c);"
                 " void g(void) { f(nullptr); }", _to_int("unsigned char"))
        _rejects(self, "double d = nullptr;", _to_int("double"))
        _rejects(self, "void f(void) { int i; i = nullptr; }",
                 _to_int("int"))
        _rejects(self, "nullptr_t n = 1;", _to_nullptr_t("int"))
        _rejects(self, "nullptr_t n = (int *)0;", _to_nullptr_t("int *"))
        _rejects(self, "void *v; void f(void) { nullptr_t n = v; }",
                 _to_nullptr_t("void *"))
        _rejects(self, "nullptr_t f(int x) { return x; }",
                 _to_nullptr_t("int"))

    def test_casts_rejected(self) -> None:
        for target in ("int", "unsigned long", "float", "char"):
            _rejects(
                self, f"void f(void) {{ ({target})nullptr; }}",
                f"cannot cast nullptr_t to {target}: nullptr_t converts "
                f"only to void, bool, or a pointer type (C23 §6.5.5p4)",
            )
        for operand, what in (("1", "int"), ("p", "int *"),
                              ("(int *)0", "int *"), ("x", "int")):
            _rejects(
                self,
                f"int *p; int x; void f(void) {{ (nullptr_t){operand}; }}",
                f"cannot cast {what} to nullptr_t: the operand must be a "
                f"null pointer constant or have type nullptr_t (C23 "
                f"§6.5.5p4)",
            )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestOperators(unittest.TestCase):
    def test_equality_allowed(self) -> None:
        # C23 §6.5.10p2: "both operands have type nullptr_t; one
        # operand has type nullptr_t and the other is a null pointer
        # constant; or, one operand is a pointer and the other is a
        # null pointer constant or has type nullptr_t."
        for exp in (
            "nullptr == nullptr", "nullptr != nullptr", "nullptr == 0",
            "0 == nullptr", "nullptr == 0L", "nullptr == false",
            "nullptr == (void *)0", "p == nullptr", "nullptr != p",
            "fp == nullptr", "n == nullptr", "n == 0", "n == p",
            "v == n",
        ):
            with self.subTest(exp=exp):
                _check(_NULLPTR_T + "int *p; void *v; void (*fp)(void);"
                       " nullptr_t n;"
                       f" int f(void) {{ return {exp}; }}")

    def test_equality_rejected(self) -> None:
        for exp, left, right in (
            ("nullptr == 1", "nullptr_t", "int"),
            ("x == nullptr", "int", "nullptr_t"),
            ("nullptr != 0.0", "nullptr_t", "double"),
            ("n == x", "nullptr_t", "int"),
        ):
            op = "!=" if "!=" in exp else "=="
            _rejects(
                self, f"int x; nullptr_t n; int f(void) {{ return {exp}; }}",
                f"binary '{op}' between {left} and {right}: a nullptr_t "
                f"compares only with another nullptr_t, a null pointer "
                f"constant, or a pointer (C23 §6.5.10p2)",
            )

    def test_relational_rejected(self) -> None:
        # §6.5.9p2: "both operands have real type; or both operands are
        # pointers to qualified or unqualified versions of compatible
        # object types."
        for exp, op in (("nullptr < nullptr", "<"), ("p > nullptr", ">"),
                        ("nullptr <= 0", "<="), ("n >= p", ">=")):
            _rejects(
                self, f"int *p; nullptr_t n; int f(void) {{ return {exp}; }}",
                f"binary '{op}' is not defined on nullptr_t: the "
                f"relational operators compare real types or pointers to "
                f"object types (C23 §6.5.9p2)",
            )

    def test_arithmetic_rejected(self) -> None:
        for exp, op, left, right in (
            ("nullptr + 1", "+", "nullptr_t", "int"),
            ("p - nullptr", "-", "int *", "nullptr_t"),
            ("nullptr * 2", "*", "nullptr_t", "int"),
            ("nullptr & 1", "&", "nullptr_t", "int"),
            ("1 << nullptr", "<<", "int", "nullptr_t"),
        ):
            _rejects(
                self, f"int *p; int f(void) {{ return {exp}; }}",
                f"binary '{op}' is not defined on nullptr_t "
                f"({left} {op} {right})",
            )
        for op in ("-", "+", "~"):
            _rejects(
                self, f"int f(void) {{ return {op}nullptr; }}",
                f"unary '{op}' is not defined on nullptr_t (C23 §6.5.4.3)",
            )
        for exp in ("n++", "--n"):
            _rejects(
                self, f"void f(void) {{ nullptr_t n; {exp}; }}",
                "the operand of ++ or -- must have a real or pointer "
                "type, and nullptr_t is neither (C23 §6.5.3.5, §6.5.4.1)",
            )
        _rejects(
            self, "void f(void) { nullptr_t n; n += 0; }",
            "compound assignment is not defined on nullptr_t (nullptr_t, "
            "int; C23 §6.5.17.3)",
        )

    def test_scalar_contexts(self) -> None:
        # nullptr_t is a scalar type (C23 §6.2.5p26), so it can be the
        # operand of `!`, `&&`, and `||`, and the controlling expression
        # of `if`, a loop, and `?:` (§7.21.2p4).
        _check(_NULLPTR_T +
               "int f(nullptr_t n) {"
               " if (nullptr) return 1; while (n) {} for (; nullptr;) {}"
               " do {} while (nullptr);"
               " return !nullptr + (nullptr && 1) + (0 || n)"
               " + (nullptr ? 1 : 2); }")

    def test_conditional(self) -> None:
        # §6.5.16p3 and p7: a nullptr_t with a pointer gives the
        # pointer type, and two nullptr_t operands give nullptr_t.
        prog, _, _ = _check(
            _NULLPTR_T +
            "int *p; nullptr_t n;"
            " void f(int c) { c ? p : nullptr; c ? n : p;"
            " c ? nullptr : n; c ? (void *)0 : nullptr; }"
        )
        stmts = prog.declaration[-1].function_decl.body.block_item
        int_p = c99_ast.Pointer(referenced_type=c99_ast.Int())
        void_p = c99_ast.Pointer(referenced_type=c99_ast.Void())
        self.assertEqual(
            [s.statement.exp.data_type for s in stmts],
            [int_p, int_p, c99_ast.NullptrT(), void_p],
        )

    def test_conditional_rejected(self) -> None:
        # Footnote 109: "If a second or third operand of type nullptr_t
        # is used and the other operand is not a pointer and does not
        # have type nullptr_t itself, a constraint is violated even if
        # that other operand is a null pointer constant such as 0."
        for exp, t, f in (("c ? nullptr : 0", "nullptr_t", "int"),
                          ("c ? 1L : nullptr", "long", "nullptr_t")):
            _rejects(
                self, f"void f(int c) {{ {exp}; }}",
                f"conditional branches of type {t} and {f}: the other "
                f"branch of a nullptr_t branch must be a pointer or another "
                f"nullptr_t (C23 §6.5.16p3)",
            )

    def test_not_an_integer_constant(self) -> None:
        # §6.6p8 admits only operands of integer type to an integer
        # constant expression.
        message = (
            "`nullptr` has type nullptr_t, which isn't an integer type, so "
            "it can't be an operand of an integer constant expression "
            "(C23 §6.6p8)"
        )
        rc, err = _compile(
            "int f(int x) { switch (x) { case nullptr: return 0; } "
            "return 1; }\n",
        )
        self.assertEqual(rc, 1)
        self.assertIn(
            f"case label is not an integer constant expression: {message}",
            err,
        )
        with self.assertRaises(ParserError) as cm:
            parse("int a[nullptr];")
        self.assertEqual(
            str(cm.exception),
            "array size must have integer type, and `nullptr` has type "
            "nullptr_t (C23 §6.7.7.3)",
        )
        _rejects(
            self, "int x = !nullptr;",
            f"initializer for static-storage object 'x' is not a "
            f"constant expression: {message}",
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestValues(unittest.TestCase):
    def test_sizes(self) -> None:
        # C23 §7.21.2p3: "The size and alignment of nullptr_t is the
        # same as for a pointer to character type."
        for exp in ("sizeof(nullptr)", "sizeof nullptr",
                    "sizeof(nullptr_t)", "sizeof(nullptr_t[1])"):
            with self.subTest(exp=exp):
                self.assertEqual(
                    _static_value(
                        _NULLPTR_T + f"unsigned long n = {exp};", "n",
                    ),
                    2,
                )

    def test_static_initializers(self) -> None:
        # `nullptr` is a predefined constant, so it's a named constant
        # (§6.6p7), which an initializer may be (§6.6p9).
        for decl in ("int *b = nullptr;", "bool b = nullptr;",
                     "nullptr_t b = nullptr;", "nullptr_t b = 0;",
                     "void (*b)(void) = nullptr;"):
            with self.subTest(decl=decl):
                self.assertEqual(_static_value(_NULLPTR_T + decl, "b"), 0)

    def test_tac_has_the_constant(self) -> None:
        # c99_to_tac makes `nullptr` the constant 0, so the optimizer
        # folds what follows from it.
        prog, symbols, types = _check(
            "int *f(void) { return nullptr; }"
            "bool g(void) { return nullptr; }",
        )
        tac = translate_program(prog, symbols, types)
        for fn in tac.top_level:
            with self.subTest(fn=fn.name):
                ret = fn.instructions[-1]
                self.assertIsInstance(ret, tac_ast.Ret)
                self.assertIsInstance(ret.val, tac_ast.Constant)
                self.assertEqual(ret.val.const.value, 0)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestStddef(unittest.TestCase):
    def test_includes_cleanly(self) -> None:
        src = (
            "#include <stddef.h>\n"
            "#if __STDC_VERSION_STDDEF_H__ != 202311L\n"
            "#error\n"
            "#endif\n"
            "struct s { char a; int b; long c[3]; };\n"
            "nullptr_t n = nullptr;\n"
            "size_t z = sizeof(int);\n"
            "ptrdiff_t d;\n"
            "max_align_t m;\n"
            "wchar_t w;\n"
            "int *p = NULL;\n"
            "size_t o = offsetof(struct s, c[1]);\n"
            "int main(void) { unreachable(); return NULL == 0; }\n"
        )
        with diagnostics.collecting() as found:
            rc, err = _compile(src)
        self.assertEqual((rc, err, found), (0, "", []))
        _, symbols, _ = _check(src)
        self.assertEqual(symbols["o"].attrs.initial_value.value, 7)
        self.assertEqual(symbols["z"].attrs.initial_value.value, 2)

    def test_types_match_the_operators(self) -> None:
        # `size_t` is what `sizeof` gives, and `ptrdiff_t` what the
        # subtraction of two pointers gives.
        prog, symbols, _ = _check(
            "#include <stddef.h>\n"
            "int a[2];\n"
            "void f(void) { sizeof(int); &a[1] - &a[0]; }\n"
            "size_t z; ptrdiff_t d; nullptr_t n;\n"
        )
        stmts = prog.declaration[1].function_decl.body.block_item
        self.assertEqual(stmts[0].statement.exp.data_type,
                         symbols["z"].type)
        self.assertEqual(stmts[1].statement.exp.data_type,
                         symbols["d"].type)
        self.assertEqual(symbols["n"].type, c99_ast.NullptrT())

    def test_null_compares_equal_to_zero_and_nullptr(self) -> None:
        self.assertEqual(
            _static_value(
                "#include <stddef.h>\n"
                "int f(void) { return NULL == 0 && NULL == nullptr; }\n"
                "nullptr_t n = NULL;\n",
                "n",
            ),
            0,
        )

    def test_null_is_not_an_integer(self) -> None:
        with self.assertRaises(TypeCheckError):
            _check("#include <stddef.h>\nint x = NULL;\n")

    def test_offsetof_is_a_constant(self) -> None:
        prog, symbols, _ = _check(
            "#include <stddef.h>\n"
            "struct t { char x; int y; };\n"
            "struct s { char a; int b; struct t in; struct t arr[2]; };\n"
            "unsigned char o1 = offsetof(struct s, b);\n"
            "int o2 = offsetof(struct s, in.y);\n"
            "long o3 = offsetof(struct s, arr[1].y);\n"
            "int f(int x) { switch (x) {"
            " case offsetof(struct s, in): return 1; } return 0; }\n"
        )
        self.assertEqual(
            [symbols[n].attrs.initial_value.value
             for n in ("o1", "o2", "o3")],
            [1, 4, 10],
        )


_PROGRAM = """\
#include <stddef.h>

struct node { int value; struct node *next; };

static struct node c = { 3, nullptr };
static struct node b = { 2, &c };
static struct node a = { 1, &b };
static int *gp = nullptr;
static nullptr_t gn;
static bool gb = nullptr;
int g;

int length(struct node *n) {
    int k = 0;
    for (; n != nullptr; n = n->next)
        k++;
    return k;
}

int *pick(int which) { return which ? &g : nullptr; }

nullptr_t none(void) { return nullptr; }

int is_null(void *p) { return p == nullptr; }

int main(void) {
    int *p = nullptr;
    nullptr_t n = 0;
    int r = 0;
    if (nullptr == 0 && 0 == nullptr) r += 1;
    if (nullptr == NULL && NULL == 0 && nullptr == (void *)0) r += 2;
    if (p == nullptr && !(nullptr != p)) r += 4;
    if (!nullptr && !(bool)nullptr && !n) r += 8;
    if (is_null(nullptr) && is_null(NULL) && !is_null(&g)) r += 16;
    if (pick(0) == nullptr && pick(1) == &g) r += 32;
    if (gp == nullptr && !gb && gn == nullptr && n == nullptr) r += 64;
    if (none() == nullptr && !none()) r += 128;
    if (sizeof(nullptr) == 2 && sizeof(nullptr_t) == 2) r += 256;
    p = r & 1 ? &g : nullptr;
    if (p == &g) r += 512;
    r += 1000 * length(&a) + 10000 * (int)offsetof(struct node, next);
    return r;
}
"""

# 1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512, three nodes, and
# `next` 2 bytes into a node.
_EXPECTED = 1023 + 3000 + 20000


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestPrograms(unittest.TestCase):
    def test_tac_sim(self) -> None:
        self.assertEqual(_run_tac(preprocess(_PROGRAM)), _EXPECTED)

    def test_asm_sim(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(_PROGRAM, optimize=optimize).run(
                    max_cycles=2_000_000,
                )
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int_signed(), _EXPECTED)


if __name__ == "__main__":
    unittest.main()
