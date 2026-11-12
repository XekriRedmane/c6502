"""Tests for calling through any expression that gives a function or a
function pointer (C99 §6.5.2.2): `pick()(3)`, `t[i](3)`, `s.g(3)`,
`p->g(3)`, `(*pp)(3)`. The parser makes a c99 `IndirectCall` for a
callee that isn't a name, the type checker strips the `*` and `&` that
don't change which function it is, and c99_to_tac evaluates what's
left and emits a TAC `IndirectCall`."""
from __future__ import annotations

import unittest

import c99_ast
from parser import ParserError, parse
from passes.recursion_saves import RecursionSaveError
from passes.type_checking import TypeCheckError
from sim.harness import build_sim


def _codegen(src: str, optimize: bool = True) -> str:
    from compile import _run_stage
    from preprocessor import preprocess
    return _run_stage("codegen", preprocess(src), optimize=optimize)


def _run(src: str, optimize: bool) -> int:
    return build_sim(src, optimize=optimize).run().return_int()


def _calls(node) -> list:
    """Every FunctionCall and IndirectCall under `node`, outermost
    first."""
    import dataclasses
    out = []

    def walk(n):
        if isinstance(n, list):
            for x in n:
                walk(x)
            return
        if not dataclasses.is_dataclass(n):
            return
        if isinstance(n, (c99_ast.FunctionCall, c99_ast.IndirectCall)):
            out.append(n)
        for f in dataclasses.fields(n):
            walk(getattr(n, f.name))

    walk(node)
    return out


class TestParse(unittest.TestCase):
    def _kinds(self, decls: str, call: str) -> list[str]:
        prog = parse(decls + " int main(void) { return " + call + "; }")
        return [type(c).__name__ for c in _calls(prog.declaration[-1])]

    def test_a_name_is_a_function_call(self) -> None:
        self.assertEqual(self._kinds("int f(int);", "f(1)"),
                         ["FunctionCall"])
        self.assertEqual(self._kinds("int f(int);", "(f)(1)"),
                         ["FunctionCall"])

    def test_other_callees_are_indirect_calls(self) -> None:
        for decls, call in [
            ("int (*t[2])(int);", "t[1](3)"),
            ("struct S { int (*g)(int); } s;", "s.g(3)"),
            ("struct S { int (*g)(int); } *p;", "p->g(3)"),
            ("int (*fp)(int);", "(*fp)(3)"),
        ]:
            with self.subTest(call=call):
                self.assertEqual(self._kinds(decls, call), ["IndirectCall"])

    def test_returned_pointer_called(self) -> None:
        self.assertEqual(
            self._kinds("int (*pick(void))(int);", "pick()(3)"),
            ["IndirectCall", "FunctionCall"],
        )

    def test_constant_callee_rejected(self) -> None:
        for src in ["int main(void) { return 3(); }",
                    "enum { A }; int main(void) { return A(); }",
                    'int main(void) { return "f"(); }']:
            with self.subTest(src=src):
                with self.assertRaisesRegex(ParserError, "not a function"):
                    parse(src)


class TestTypeCheck(unittest.TestCase):
    def test_not_a_function(self) -> None:
        for decls, call in [
            ("int x;", "(*&x)()"),
            # `&fp` is a pointer to the pointer, not to a function.
            ("int (*fp)(void);", "(&fp)()"),
            ("int a[2];", "a[0]()"),
        ]:
            with self.subTest(call=call):
                with self.assertRaisesRegex(TypeCheckError, "not a function"):
                    _codegen(decls + " int main(void) { return "
                             + call + "; }")

    def test_argument_count_checked(self) -> None:
        with self.assertRaisesRegex(TypeCheckError, "called with 2"):
            _codegen("int (*t[1])(int);"
                     " int main(void) { return t[0](1, 2); }")

    def test_arguments_converted(self) -> None:
        # The `long` argument converts to the `unsigned char` parameter.
        src = """
            unsigned char low(unsigned char x) { return x; }
            unsigned char (*t[1])(unsigned char) = { low };
            int main(void) { long v = 0x1234; return t[0](v); }
        """
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(_run(src, optimize), 0x34)


class TestRuns(unittest.TestCase):
    _SHAPES = """
        struct S { int (*g)(int); };
        int dbl(int x) { return 2 * x; }
        int inc(int x) { return x + 1; }
        int (*t[2])(int) = { dbl, inc };
        int (*pick(int i))(int) { return t[i]; }
        struct S s = { inc };
        int (*fp)(int) = dbl;
        int main(void) {
            struct S *p = &s;
            int (**pp)(int) = &fp;
            int r = 0;
            r += pick(0)(3);                          /* 6 */
            r += (pick(1))(3);                        /* 4 */
            r += t[0](5);                             /* 10 */
            r += t[1](5) * 100;                       /* 600 */
            r += s.g(7);                              /* 8 */
            r += p->g(9);                             /* 10 */
            r += (*pp)(1);                            /* 2 */
            r += (**fp)(2) + (*dbl)(3) + (&inc)(4);   /* 4 + 6 + 5 */
            return r;
        }
    """

    def test_every_shape(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(_run(self._SHAPES, optimize), 655)

    def test_dispatch_table_in_a_loop(self) -> None:
        # A const table indexed by a variable, the usual dispatch shape.
        src = """
            int add1(int x) { return x + 1; }
            int add2(int x) { return x + 2; }
            int add3(int x) { return x + 3; }
            static int (*const ops[3])(int) = { add1, add2, add3 };
            int main(void) {
                int v = 0;
                for (unsigned char i = 0; i < 3; i++) v = ops[i](v) * 2;
                return v;
            }
        """
        # ((0 + 1) * 2 + 2) * 2 = 8; (8 + 3) * 2 = 22
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(_run(src, optimize), 22)

    def test_struct_return_and_void(self) -> None:
        src = """
            struct P { int a, b; };
            struct P mk(int x) { struct P p = { x, x + 1 }; return p; }
            int g;
            void set(int x) { g = x; }
            struct V {
                struct P (*make)(int);
                void (*store)(int);
            } v = { mk, set };
            int main(void) {
                struct V *pv = &v;
                pv->store(40);
                return pv->make(1).b + g;
            }
        """
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(_run(src, optimize), 42)

    def test_callee_and_arguments_each_evaluated_once(self) -> None:
        src = """
            int n;
            int id(int x) { return x; }
            int neg(int x) { return -x; }
            int (*t[2])(int) = { id, neg };
            int (*next(void))(int) { return t[n++]; }
            int main(void) {
                int a = next()(n * 10);    /* id or neg of 0 or 10 */
                int b = next()(5);         /* neg(5) */
                return a * 100 + b + n;    /* n is 2 */
            }
        """
        # The callee and the argument are unsequenced, so `a` is id(0),
        # id(10), or the same through `neg`; c6502 evaluates the callee
        # first, so `n` is 1 when `n * 10` runs: a = id(10) = 10.
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(_run(src, optimize), 10 * 100 - 5 + 2)


class TestHardwareStackPragma(unittest.TestCase):
    def test_rejected_through_a_table(self) -> None:
        src = """
            int (*t[1])(int);
            int main(void) {
                #pragma c6502 call save(hardware_stack)
                return t[0](1);
            }
        """
        with self.assertRaisesRegex(RecursionSaveError, "function pointer"):
            _codegen(src)


if __name__ == "__main__":
    unittest.main()
