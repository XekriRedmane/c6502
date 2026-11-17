"""passes/return_in_zero_page.py: a `static` function returning a
pointer returns it in a zero-page object of its own, `F@ret`.

`TestRewrite` checks which functions the TAC rewrite chooses and the
instructions it writes. `TestPrograms` runs programs that use the
returned pointer in each way the pass and the asm passes after it
have to keep right, in the simulator with and without `-O`.
`TestCodegen` checks that a read through the result right after the
call reads through `F@ret`, with no copy."""

from __future__ import annotations

import shutil
import unittest

import c99_ast
import tac_ast
from passes.return_in_zero_page import return_pointers_in_zero_page
from passes.type_checking import FunAttr, StaticAttr, Symbol
from sim.harness import build_sim


def _var(name):
    return tac_ast.Var(name=name)


def _program(is_global=False, ret=None, taken=False):
    """`f` returning `%r`, and `g` calling it into `%d`. With `taken`,
    `g` also takes `f`'s address."""
    f = tac_ast.Function(name="f", is_global=is_global, params=[],
                         instructions=[tac_ast.Ret(val=_var("%r"))])
    g_instrs = [tac_ast.FunctionCall(name="f", args=[], dst=_var("%d")),
                tac_ast.Ret(val=None)]
    if taken:
        g_instrs.insert(0, tac_ast.GetAddress(operand=_var("f"),
                                              dst=_var("%fp")))
    g = tac_ast.Function(name="g", is_global=True, params=[],
                         instructions=g_instrs)
    ret = ret if ret is not None else c99_ast.Pointer(
        referenced_type=c99_ast.UChar())
    symbols = {
        "f": Symbol(type=c99_ast.FunType(params=[], ret=ret),
                    attrs=FunAttr(defined=True, is_global=is_global)),
        "g": Symbol(type=c99_ast.FunType(params=[], ret=c99_ast.Void()),
                    attrs=FunAttr(defined=True, is_global=True)),
    }
    return tac_ast.Program(top_level=[f, g]), symbols


class TestRewrite(unittest.TestCase):

    def test_static_pointer_function_returns_in_its_object(self):
        prog, symbols = _program()
        out = return_pointers_in_zero_page(prog, symbols)
        f, g, obj = out.top_level
        self.assertEqual(f.instructions, [
            tac_ast.Copy(src=_var("%r"), dst=_var("f@ret")),
            tac_ast.Ret(val=None),
        ])
        self.assertEqual(g.instructions[:2], [
            tac_ast.FunctionCall(name="f", args=[], dst=None),
            tac_ast.Copy(src=_var("f@ret"), dst=_var("%d")),
        ])
        self.assertEqual(obj.name, "f@ret")
        attrs = symbols["f@ret"].attrs
        self.assertIsInstance(attrs, StaticAttr)
        self.assertTrue(attrs.zeropage)
        self.assertFalse(attrs.is_global)

    def test_function_with_external_linkage_left_alone(self):
        # Another unit can call it, and its calls expect A and X.
        prog, symbols = _program(is_global=True)
        self.assertIs(return_pointers_in_zero_page(prog, symbols), prog)

    def test_function_whose_address_is_taken_left_alone(self):
        # A call through a pointer expects the result in A and X.
        prog, symbols = _program(taken=True)
        self.assertIs(return_pointers_in_zero_page(prog, symbols), prog)

    def test_integer_result_left_alone(self):
        # A 2-byte integer is usually passed on to another function's
        # slot or stored, where A and X cost less.
        prog, symbols = _program(ret=c99_ast.UInt())
        self.assertIs(return_pointers_in_zero_page(prog, symbols), prog)


# (name, C source, expected int return of main)
_CASES: list[tuple[str, str, int]] = [
    # The result read right after the call, kept across a second
    # call (so it has to be copied out of `F@ret`), stored into a
    # static, compared, and returned on from another function.
    ("uses_of_the_result", """
        unsigned char t[16] = { 1, 2, 3, 4, 5, 6, 7, 8,
                                9, 10, 11, 12, 13, 14, 15, 16 };
        unsigned char *saved;
        static unsigned char *at(unsigned char i) { return t + i; }
        static unsigned char *pick(unsigned char i) {
            if (i & 1) return at(i);
            return t;
        }
        unsigned char *outer(unsigned char i) { return at(i + 1); }
        int main(void) {
            unsigned char *a = at(3);
            unsigned char *b = at(5);
            saved = at(7);
            int r = a[0] * 100 + b[0] * 10 + saved[0];
            if (at(2) == t + 2) r += 1000;
            if (pick(2) == t) r += 2000;
            r += pick(9)[0] + outer(4)[0];
            a[1] = 40;
            return r + t[4];
        }
    """, 3000 + 400 + 60 + 8 + 10 + 6 + 40),
    # A recursive pointer function: each call's result is read
    # before the caller's own return writes the object.
    ("recursion", """
        unsigned char t[8] = { 5, 6, 7, 8, 9, 10, 11, 12 };
        static unsigned char *nth(unsigned char n) {
            if (n == 0) return t;
            return nth(n - 1) + 1;
        }
        int main(void) { return *nth(5) + *nth(0) * 100; }
    """, 10 + 500),
    # Stores through the result, and through a pointer to another
    # element, between reads of it.
    ("stores_between_reads", """
        unsigned char t[4] = { 1, 2, 3, 4 };
        unsigned char *q;
        static unsigned char *first(void) { return t; }
        int main(void) {
            q = t + 3;
            unsigned char *p = first();
            p[1] = 20;
            *q = 30;
            p[2] = p[1] + *q;
            return p[0] + p[1] + p[2] + p[3];
        }
    """, 1 + 20 + 50 + 30),
]


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestPrograms(unittest.TestCase):

    def test_programs(self):
        for name, src, expected in _CASES:
            for optimize in (False, True):
                with self.subTest(case=name, optimize=optimize):
                    result = build_sim(src, optimize=optimize).run()
                    self.assertFalse(result.timed_out)
                    self.assertEqual(result.return_int_signed(), expected)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestCodegen(unittest.TestCase):

    def test_read_right_after_the_call_goes_through_the_object(self):
        from compile import _run_stage
        from preprocessor import preprocess
        src = (
            "#include <stdint.h>\n"
            "uint8_t *base;\n"
            "static uint8_t *addr(uint8_t n) { return base + n * 9; }\n"
            "uint8_t get(uint8_t n) { return addr(n)[4]; }\n"
        )
        asm = _run_stage("codegen", preprocess(src), optimize=True)
        get = asm[asm.index("\nget:\n"):]
        self.assertIn("LDA   (addr@ret),Y", get)
        self.assertNotIn("STA", get[:get.index("RTS")])


if __name__ == "__main__":
    unittest.main()
