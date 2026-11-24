"""Tests for the asm-level redundant-load elimination pass.

`apply_redundant_load_elimination` walks each function linearly,
tracking which operand each of A/X/Y currently mirrors, and drops
any subsequent `Mov(M, Reg(R))` whose target register already
holds memory[M] (or a matching immediate). The pass invalidates
tracking on register-clobbering instructions, basic-block
boundaries, calls, and aliasing memory writes.

Coverage:
  * Repeat loads from the same source collapse.
  * Stores to provably-disjoint memory don't invalidate tracking.
  * Stores to the same / aliasing memory do invalidate.
  * Block boundaries (Label / Jump / Branch / Call / Ret) reset.
  * Arithmetic / shifts / Pop on Reg(A) invalidate A.
  * Branch immediately after the load preserves the load
    (flag liveness).

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).
"""
from __future__ import annotations

import shutil
import unittest

import asm_ast


_REG_A = asm_ast.Reg(reg=asm_ast.A())


class TestIndexRegisterOfIndexedData(unittest.TestCase):
    """`tab,X` and `tab,Y` are different bytes unless X equals Y."""

    def _tab(self, reg):
        return asm_ast.IndexedData(name="tab", offset=0, index=reg)


    def test_simulated_table_chase(self):
        import shutil
        if not shutil.which("pcpp"):
            self.skipTest("pcpp not installed")
        from sim.harness import build_sim
        src = """
unsigned char tab[8] = {3, 0, 0, 7, 0, 0, 0, 0};
unsigned char g;
unsigned char f(unsigned char i) { unsigned char j = tab[i]; if (j == 0) return 0; g = tab[j]; return j; }
int main(void) { unsigned char r = f(0); return g * 100 + r; }
"""
        self.assertEqual(
            build_sim(src, optimize=True).run().return_int(), 703,
        )


def _d(name: str, offset: int = 0) -> asm_ast.Data:
    return asm_ast.Data(name=name, offset=offset)


class TestSwapPairs(unittest.TestCase):
    """`LDA X; STA D1; LDA M; STA D2` with A already holding M becomes
    `STA D2; LDA X; STA D1`, as at the join of `floor_enemy_draw`'s
    dispatch."""

    def _join(self, d1, m, d2, after=None):
        return [
            asm_ast.Mov(src=_d("hi"), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_d("t")),
            asm_ast.Mov(src=_d("lo"), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=d1),
            asm_ast.Mov(src=m, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=d2),
            *(after or [asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A)]),
            asm_ast.Return(save_a=False),
        ]


    def _indexed_join(self, index, m):
        """The join storing a slot and then `m` to two bytes of `z`
        indexed by `index`, as `instr_call` stores a local's two
        bytes. The slots are the function's own, which an indexed
        store can't reach."""
        z = lambda k: asm_ast.IndexedData(name="z", offset=k, index=index)
        t = _d("__local_f__t")
        return [
            asm_ast.Mov(src=_d("hi"), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=t),
            asm_ast.Mov(src=_d("__local_f__lo"), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=z(0)),
            asm_ast.Mov(src=m or t, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=z(1)),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]


    def test_indexed_bytes_through_y_setup(self):
        # The `LDY #off` of a soft-stack load between the stores
        # changes Y, so `z,Y` and `z+1,Y` may be one byte.
        from passes.redundant_load import _same_index_apart
        z = lambda k: asm_ast.IndexedData(name="z", offset=k, index=asm_ast.Y())
        plain = self._indexed_join(asm_ast.Y(), None)[2:6]
        self.assertTrue(_same_index_apart(z(0), z(1), plain))
        setup = self._indexed_join(asm_ast.Y(), asm_ast.Frame(offset=3))[2:6]
        self.assertFalse(_same_index_apart(z(0), z(1), setup))


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestRowsOfTwoTablesSim(unittest.TestCase):
    """Two 2-D arrays whose tables of row addresses share the low
    bytes, read and written in one row, as Lode Runner's LEVEL_MAP and
    LEVEL_BG are."""

    _SRC = (
        "#include <stdint.h>\n"
        "static uint8_t map[16][28];\n"
        "static uint8_t bg[16][28];\n"
        "uint8_t row, col;\n"
        "void fall(void) {\n"
        "    uint8_t r = row + 1;\n"
        "    if (bg[r][col] == 1 && (int8_t)col >= 0) {\n"
        "        uint8_t above = r - 1;\n"
        "        if (bg[above][col] == 0) {\n"
        "            map[above][col] = 7;\n"
        "            bg[above][col] = 7;\n"
        "        }\n"
        "    }\n"
        "}\n"
        "int main(void) {\n"
        "    for (uint8_t i = 0; i < 16; i++)\n"
        "        for (uint8_t j = 0; j < 28; j++)\n"
        "            bg[i][j] = (uint8_t)(i + j) & 1;\n"
        "    row = 4; col = 4;\n"
        "    fall();\n"
        "    return map[4][4] + bg[4][4] * 16 + bg[5][4] * 256;\n"
        "}\n"
    )

    def test_runs(self):
        from sim.harness import build_sim
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(self._SRC, optimize=optimize).run()
                self.assertEqual(out.return_int(), 7 + 7 * 16 + 256)


if __name__ == "__main__":
    unittest.main()
