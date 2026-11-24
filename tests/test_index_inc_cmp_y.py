"""Tests for `passes.index_inc_cmp_y.apply_index_inc_cmp_y`.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6)."""

import unittest

import asm_ast


A = asm_ast.Reg(reg=asm_ast.A())


def _indexed_x(name):
    return asm_ast.IndexedData(name=name, index=asm_ast.X())


class TestTmpLiveness(unittest.TestCase):
    """The temporary keeps its old value after the rewrite, so it must
    be compiler-owned scratch that no path reads afterward."""

    def _pattern(self, tmp):
        return [
            asm_ast.Mov(src=_indexed_x("tbl"), dst=A, is_volatile=False),
            asm_ast.Mov(src=A, dst=tmp, is_volatile=False),
            asm_ast.Inc(dst=tmp),
            asm_ast.Mov(src=tmp, dst=A, is_volatile=False),
            asm_ast.Compare(left=A, right=asm_ast.Imm(value=9)),
        ]


    def test_simulated_loop_and_global(self):
        import shutil
        if not shutil.which("pcpp"):
            self.skipTest("pcpp not installed")
        from sim.harness import build_sim
        src = """
unsigned char ceiling[8] = {1,2,3,4,5,6,7,8};
unsigned char out[8];
unsigned char beam_y, hit, top;
void check(unsigned char i) { top = ceiling[i]; top++; if (top == beam_y) hit = 1; }
void scan(void) {
    unsigned char prev = 0;
    for (unsigned char i = 0; i < 8; i++) {
        out[i] = prev;
        prev = ceiling[i];
        prev++;
        if (prev == beam_y) hit = 1;
    }
}
int main(void) { beam_y = 99; check(2); scan(); return top * 100 + out[3]; }
"""
        self.assertEqual(
            build_sim(src, optimize=True).run().return_int(), 404,
        )


if __name__ == "__main__":
    unittest.main()
