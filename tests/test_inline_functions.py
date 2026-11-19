"""passes/optimization/inline_functions.py: the calls of small `static`
functions are replaced by copies of their bodies under `-O`.

`TestRewrite` translates small programs to TAC, inlines them, and
checks the instructions written, the names given to the copies, and
which functions are left alone. `TestPrograms` runs programs whose
calls are inlined, in the simulator with and without `-O`, and checks
the values they compute, including the order of an argument's side
effects and volatile accesses."""

from __future__ import annotations

import shutil
import unittest

import tac_ast
from passes.optimization import inline_functions as inl
from passes.optimization.inline_functions import inline_functions
from passes.type_checking import LocalAttr
from sim.harness import build_sim


def _tac(src: str):
    """The unoptimized TAC of `src`, with its symbols and c99 program."""
    from compile import _resolved
    from c99_to_tac import translate_program
    from passes.type_checking import check_program as type_check_program
    from preprocessor import preprocess
    prog, symbols, types = type_check_program(
        _resolved(preprocess(src), optimize=True),
    )
    return translate_program(prog, symbols, types), symbols, prog


def _inlined(src: str):
    tac, symbols, prog = _tac(src)
    out = inline_functions(tac, symbols, prog)
    return tac, out, symbols


def _fn(prog, name):
    for tl in prog.top_level:
        if isinstance(tl, tac_ast.Function) and tl.name == name:
            return tl
    return None


def _calls(fn):
    return [i.name for i in fn.instructions
            if isinstance(i, tac_ast.FunctionCall)]


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestRewrite(unittest.TestCase):

    def test_single_call_is_inlined_and_the_function_dropped(self):
        before, out, symbols = _inlined("""
            static int twice(int x) { int y = x + x; return y; }
            int f(int a) { return twice(a) + 1; }
        """)
        self.assertIsNone(_fn(out, "twice"))
        f = _fn(out, "f")
        self.assertEqual(_calls(f), [])
        # The parameter's copy comes first, and reads the argument.
        first = f.instructions[0]
        self.assertIsInstance(first, tac_ast.Copy)
        self.assertEqual(first.src, tac_ast.Var(name=f.params[0]))
        # Every local of the copy is new, with the type of the one it
        # copies.
        old = _fn(before, "twice")
        old_names = {v.name for i in old.instructions
                     for v in inl._vals(i) if isinstance(v, tac_ast.Var)}
        new_names = {v.name for i in f.instructions
                     for v in inl._vals(i) if isinstance(v, tac_ast.Var)}
        self.assertFalse(old_names & new_names)
        self.assertIn(f.params[0], new_names)
        x_copy = first.dst.name
        self.assertRegex(x_copy, r"^@\d+\.x$")
        self.assertEqual(symbols[x_copy].type, symbols[old.params[0]].type)
        self.assertIsInstance(symbols[x_copy].attrs, LocalAttr)

    def test_return_becomes_a_copy_into_the_result(self):
        _, out, _ = _inlined("""
            static int one(void) { return 1; }
            int f(void) { int r = one(); return r + r; }
        """)
        f = _fn(out, "f")
        call_dst = None
        for i in f.instructions:
            if isinstance(i, tac_ast.Copy) and i.src == tac_ast.Constant(
                    const=tac_ast.ConstInt(value=1)):
                call_dst = i.dst
        self.assertIsNotNone(call_dst)
        # The only return is the last instruction, so no jump to an end
        # label is needed.
        self.assertFalse(any(isinstance(i, tac_ast.Jump)
                             for i in f.instructions))

    def test_early_returns_jump_to_the_end_label(self):
        _, out, _ = _inlined("""
            static int sign(int x) {
                if (x < 0) return -1;
                if (x == 0) return 0;
                return 1;
            }
            int f(int a) { int s = sign(a); return s * 3; }
        """)
        f = _fn(out, "f")
        jumps = [i.target for i in f.instructions
                 if isinstance(i, tac_ast.Jump)]
        ends = [j for j in jumps if j.endswith("@end")]
        self.assertEqual(len(ends), 2)
        self.assertEqual(len(set(ends)), 1)
        labels = [i.name for i in f.instructions
                  if isinstance(i, tac_ast.Label)]
        self.assertIn(ends[0], labels)
        # The copy's own labels carry its prefix.
        self.assertTrue(all(l.startswith(".i") for l in labels))
        self.assertEqual(len(labels), len(set(labels)))

    def test_returns_stay_returns_in_tail_position(self):
        _, out, _ = _inlined("""
            static int sign(int x) {
                if (x < 0) return -1;
                return 1;
            }
            int f(int a) { return sign(a); }
        """)
        f = _fn(out, "f")
        rets = [i for i in f.instructions if isinstance(i, tac_ast.Ret)]
        self.assertEqual(len(rets), 2)
        self.assertFalse(any(isinstance(i, tac_ast.Jump)
                             for i in f.instructions))

    def test_void_function(self):
        _, out, _ = _inlined("""
            unsigned char g;
            static void set(unsigned char v) { if (v) { g = v; return; } g = 1; }
            void f(unsigned char a) { set(a); g++; }
        """)
        f = _fn(out, "f")
        self.assertEqual(_calls(f), [])
        self.assertIsNone(_fn(out, "set"))
        self.assertTrue(any(isinstance(i, tac_ast.Jump)
                            and i.target.endswith("@end")
                            for i in f.instructions))

    def test_two_copies_get_different_names(self):
        _, out, _ = _inlined("""
            static unsigned char inc(unsigned char x) { return x + 1; }
            unsigned char f(unsigned char a) { return inc(a) + inc(a); }
        """)
        f = _fn(out, "f")
        dsts = [i.dst.name for i in f.instructions
                if isinstance(i, tac_ast.Copy)
                and i.src == tac_ast.Var(name=f.params[0])]
        self.assertEqual(len(dsts), 2)
        self.assertNotEqual(dsts[0], dsts[1])

    def test_nested_inlining(self):
        # `outer` calls `inner`, which is inlined into it first, and
        # then `outer`, with that copy, into `f`.
        _, out, _ = _inlined("""
            unsigned char g;
            static unsigned char inner(unsigned char x) { return x ^ g; }
            static unsigned char outer(unsigned char x) { return inner(x) + 1; }
            unsigned char f(unsigned char a) { return outer(a); }
        """)
        self.assertIsNone(_fn(out, "inner"))
        self.assertIsNone(_fn(out, "outer"))
        f = _fn(out, "f")
        self.assertEqual(_calls(f), [])
        ops = [type(i.op).__name__ for i in f.instructions
               if isinstance(i, tac_ast.Binary)]
        self.assertIn("BitwiseXor", ops)
        self.assertIn("Add", ops)

    def test_large_function_with_several_calls_stays(self):
        body = " ".join(f"g = g * {k} + x;" for k in range(3, 9))
        _, out, _ = _inlined(f"""
            unsigned int g;
            static void big(unsigned int x) {{ {body} }}
            void f(void) {{ big(1); big(2); }}
        """)
        self.assertIsNotNone(_fn(out, "big"))
        self.assertEqual(_calls(_fn(out, "f")), ["big", "big"])

    def test_small_function_with_several_calls_is_inlined(self):
        _, out, _ = _inlined("""
            unsigned char t[8];
            static unsigned char at(unsigned char i) { return t[i]; }
            unsigned char f(void) { return at(1) + at(2) + at(3); }
        """)
        self.assertIsNone(_fn(out, "at"))
        self.assertEqual(_calls(_fn(out, "f")), [])

    def test_left_alone(self):
        cases = {
            "external linkage": """
                int one(void) { return 1; }
                int f(void) { return one(); }
            """,
            "recursion": """
                static int down(int n) { return n ? down(n - 1) : 0; }
                int f(void) { return down(3); }
            """,
            "recursion through another function": """
                static int b(int n);
                static int a(int n) { return n ? b(n - 1) : 0; }
                static int b(int n) { return a(n); }
                int f(void) { return a(3); }
            """,
            "address taken": """
                static int one(void) { return 1; }
                int (*p)(void) = one;
                int f(void) { return one(); }
            """,
            "a static local": """
                static int count(void) { static int n; return ++n; }
                int f(void) { return count(); }
            """,
            "an address-taken local": """
                void use(int *p);
                static void g(void) { int x = 1; use(&x); }
                void f(void) { g(); }
            """,
            "an indirect call": """
                void (*hook)(void);
                static void g(void) { hook(); }
                void f(void) { g(); }
            """,
            "a register local in the caller": """
                static unsigned char one(void) { return 1; }
                unsigned char f(void) {
                    register unsigned char i = one();
                    return i;
                }
            """,
            "a register parameter": """
                static unsigned char id(register unsigned char x) { return x; }
                unsigned char f(void) { return id(3); }
            """,
            "noreturn": """
                [[noreturn]] void halt(void);
                [[noreturn]] static void stop(void) { halt(); }
                void f(void) { stop(); }
            """,
            "a section": """
                [[c6502::section("other")]] static int one(void) { return 1; }
                int f(void) { return one(); }
            """,
            "return_bool_in_carry": """
                [[c6502::return_bool_in_carry]] static bool t(void) { return true; }
                bool f(void) { return t(); }
            """,
            "a struct result": """
                struct s { int a, b; };
                static struct s make(void) { struct s v = {1, 2}; return v; }
                int f(void) { return make().b; }
            """,
        }
        for name, src in cases.items():
            with self.subTest(case=name):
                tac, out, _ = _inlined(src)
                self.assertIs(out, tac)

    def test_label_prefix_keeps_loop_suffixes(self):
        _, out, _ = _inlined("""
            static unsigned char sum(unsigned char n) {
                unsigned char s = 0;
                for (unsigned char i = 0; i < n; i++) s += i;
                return s;
            }
            unsigned char f(void) { return sum(4); }
        """)
        labels = [i.name for i in _fn(out, "f").instructions
                  if isinstance(i, tac_ast.Label)]
        self.assertTrue(any(l.startswith(".i0_loop@")
                            and l.endswith("_continue") for l in labels))

    def test_cost_counts_widths(self):
        tac, symbols, _ = _tac("""
            static unsigned long add(unsigned long a, unsigned long b) {
                return a + b;
            }
            static unsigned char addc(unsigned char a, unsigned char b) {
                return a + b;
            }
        """)
        self.assertEqual(inl.inline_cost(_fn(tac, "add"), symbols), 4)
        # The byte add is promoted to `int`: two extends, an add, and a
        # truncate, each with a 2-byte operand.
        self.assertEqual(inl.inline_cost(_fn(tac, "addc"), symbols), 8)


# (name, C source, expected int return of main)
_CASES: list[tuple[str, str, int]] = [
    # Each argument's side effects happen once, in order, before the
    # body, which reads its parameter twice.
    ("argument_side_effects", """
        unsigned char log[4];
        unsigned char n;
        static unsigned char note(unsigned char v) { log[n++] = v; return v; }
        static int pair(int a, int b) { return a * 10 + b + a; }
        int main(void) {
            int r = pair(note(3), note(5));
            return r * 100 + log[0] * 10 + log[1] + n * 1000;
        }
    """, 2000 + 3800 + 30 + 5),
    # Several returns, used in an expression and in tail position.
    ("returns", """
        static int sign(int x) {
            if (x < 0) return -1;
            if (x == 0) return 0;
            return 1;
        }
        static int tail(int x) { return sign(x - 5); }
        int main(void) {
            return (sign(-7) + 2) * 100 + (sign(0) + 2) * 10
                + sign(9) + tail(2) * 1000;
        }
    """, -1000 + 100 + 20 + 1),
    # A void function with an early return, and a pointer result read
    # through, as Ali Baba's next_string is.
    ("void_and_pointer", """
        unsigned char text[] = { 2, 'a', 'b', 3, 'c', 'd', 'e', 1, 'f' };
        unsigned char g;
        static void bump(unsigned char by) { if (by == 0) return; g += by; }
        static const unsigned char *next(const unsigned char *s) {
            return s + s[0] + 1;
        }
        int main(void) {
            const unsigned char *m = text;
            for (unsigned char i = 0; i < 2; i++) m = next(m);
            bump(0);
            bump(4);
            bump(5);
            return m[1] + g * 1000;
        }
    """, ord('f') + 9000),
    # A copy inside a loop: each pass gets the parameter's new value,
    # and a local of the copy starts from its initializer each time.
    ("in_a_loop", """
        static unsigned int tri(unsigned char k) {
            unsigned int s = 0;
            while (k) s += k--;
            return s;
        }
        int main(void) {
            unsigned int t = 0;
            for (unsigned char i = 1; i <= 4; i++) t += tri(i);
            return t;
        }
    """, 1 + 3 + 6 + 10),
    # Volatile reads in the body happen once per call.
    ("volatile", """
        volatile unsigned char port;
        unsigned char reads;
        static unsigned char get(void) { reads++; return port; }
        int main(void) {
            port = 7;
            unsigned char a = get();
            unsigned char b = get();
            return a + b + reads * 100;
        }
    """, 214),
    # Nested candidates, with a wide and a floating parameter.
    ("nested", """
        static long sq(long x) { return x * x; }
        static long sum_sq(long a, long b) { return sq(a) + sq(b); }
        static int trunc(double d) { return (int)d; }
        int main(void) {
            return (int)(sum_sq(3, 4) + sum_sq(100, 200) / 1000) + trunc(2.75);
        }
    """, 25 + 50 + 2),
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


if __name__ == "__main__":
    unittest.main()
