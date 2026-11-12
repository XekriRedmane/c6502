"""The TAC interpreter on optimized TAC and on the SSA form between
optimizer passes, and `fuzz.tac_passes`, which uses it to name the pass
that changes a program's result."""
from __future__ import annotations

import shutil
import unittest

import tac_ast
from fuzz.tac_passes import _front_end, first_changing_pass
from passes.optimization import optimize_program
from passes.optimization.ssa_construction import to_ssa
from tac_sim import Simulator

# Each program's optimized TAC has the instruction it's named for.
_PROGRAMS = {
    tac_ast.JumpIfCmp: """
        int main(void) {
            int s = 0;
            for (unsigned char i = 0; i < 10; i++) s += i;
            return s;
        }
    """,
    tac_ast.JumpIfMasked: """
        unsigned char c = 0x93;
        int main(void) {
            int r = 0;
            if (c & 0x80) r += 1;
            if (c & 0x04) r += 2;
            if (c & 0x10) r += 4;
            return r;
        }
    """,
    tac_ast.IndexedStore: """
        static unsigned char * const buf = (unsigned char * const)0x2000;
        unsigned char col = 3;
        int main(void) {
            buf[100 + col] = 42;
            buf[101 + col] = 7;
            return buf[100 + col] * 10 + buf[101 + col];
        }
    """,
    tac_ast.IndexedConstLoad: """
        static unsigned char * const buf = (unsigned char * const)0x2000;
        unsigned char col = 3;
        int main(void) {
            buf[100 + col] = 42;
            return buf[100 + col];
        }
    """,
    tac_ast.IndirectIndexedLoad: """
        unsigned char data[8] = {5, 6, 7, 8, 9, 10, 11, 12};
        int sum(unsigned char *p, unsigned char n) {
            int s = 0;
            for (unsigned char i = 0; i < n; i++) s += p[i];
            return s;
        }
        int main(void) { return sum(data, 8); }
    """,
    tac_ast.IndirectIndexedStore: """
        unsigned char data[8];
        void fill(unsigned char *p, unsigned char n) {
            for (unsigned char i = 0; i < n; i++) p[i] = i * 3;
        }
        int main(void) { fill(data, 8); return data[7] + data[2]; }
    """,
}


def _has(prog: tac_ast.Program, kind) -> bool:
    return any(
        isinstance(i, kind)
        for tl in prog.top_level if isinstance(tl, tac_ast.Function)
        for i in tl.instructions
    )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestOptimizedTac(unittest.TestCase):

    def test_optimizer_instructions(self) -> None:
        for kind, src in _PROGRAMS.items():
            with self.subTest(kind.__name__):
                tac, syms, types = _front_end(src)
                expected = Simulator(tac, syms, types).call("main", [])
                opt = optimize_program(tac, syms)
                self.assertTrue(_has(opt, kind), f"no {kind.__name__}")
                got = Simulator(opt, syms, types).call("main", [])
                self.assertEqual(got, expected)

    def test_ssa_form(self) -> None:
        # A loop's SSA form has Phis at its header.
        src = """
            int main(void) {
                int a = 1, b = 0;
                for (int i = 0; i < 6; i++) { int t = a; a = a + b; b = t; }
                return a * 100 + b;
            }
        """
        tac, syms, types = _front_end(src)
        expected = Simulator(tac, syms, types).call("main", [])
        main = tac.top_level[0]
        ssa_fn, _ = to_ssa(main, syms)
        self.assertTrue(any(isinstance(i, tac_ast.Phi)
                            for i in ssa_fn.instructions))
        got = Simulator(
            tac_ast.Program(top_level=[ssa_fn]), syms, types,
        ).call("main", [])
        self.assertEqual(got, expected)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestTacPasses(unittest.TestCase):

    _SRC = """
        unsigned char g = 10;
        unsigned char h(void) { g = 77; return 1; }
        int main(void) {
            int t = g + 1;
            unsigned char k = h();
            unsigned char x = (unsigned char)t;
            return x + k;
        }
    """

    def test_no_pass_changes_a_correct_program(self) -> None:
        self.assertIsNone(first_changing_pass(self._SRC))

    def test_names_the_pass(self) -> None:
        # With narrow_widened_arith's check that a global is unchanged
        # since the extend read it turned off, it reads g after h()
        # changed it.
        import passes.optimization.narrow_widened_arith as n
        saved = n._unchanged
        n._unchanged = lambda *a, **k: True
        try:
            res = first_changing_pass(self._SRC)
        finally:
            n._unchanged = saved
        self.assertEqual(res, ("main", "NarrowWidenedArith", 12, 79))


if __name__ == "__main__":
    unittest.main()
