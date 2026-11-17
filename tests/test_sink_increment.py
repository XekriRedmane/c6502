"""Tests for `passes.optimization.sink_increment`, which moves `Y = X
+ c` past the last read of `X` in its block."""

from __future__ import annotations

import shutil
import unittest

import tac_ast
from passes.optimization.framework import PassContext
from passes.optimization.sink_increment import SinkIncrements
from sim.harness import build_sim


def _var(name: str) -> tac_ast.Var:
    return tac_ast.Var(name=name)


def _uc(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstUChar(value=v))


def _fn(*instrs) -> tac_ast.Function:
    return tac_ast.Function(
        name="f", is_global=True, params=[], instructions=list(instrs),
    )


class TestSinkGates(unittest.TestCase):
    def test_sinks_ssa_add_past_read(self) -> None:
        add = tac_ast.Binary(
            op=tac_ast.Add(), src1=_var("x.1"), src2=_uc(1), dst=_var("y.1"),
        )
        read = tac_ast.Copy(src=_var("x.1"), dst=_var("z.1"))
        fn = _fn(add, read, tac_ast.Ret(val=_var("y.1")))
        ctx = PassContext(ssa_dsts={"x.1", "y.1", "z.1"})
        out = SinkIncrements().run(fn, ctx)
        self.assertEqual(out.instructions[:2], [read, add])

    def test_non_ssa_source_stays(self) -> None:
        # `s` is a static. The Copy writes it between the add and its
        # last read, so moving the add would make it read 5.
        add = tac_ast.Binary(
            op=tac_ast.Add(), src1=_var("s"), src2=_uc(1), dst=_var("t.1"),
        )
        store = tac_ast.Copy(src=_uc(5), dst=_var("s"))
        read = tac_ast.Copy(src=_var("s"), dst=_var("g"))
        fn = _fn(add, store, read, tac_ast.Copy(src=_var("t.1"), dst=_var("h")))
        ctx = PassContext(ssa_dsts={"t.1"})
        out = SinkIncrements().run(fn, ctx)
        self.assertEqual(out.instructions, fn.instructions)

    def test_write_to_source_ends_scan_without_ssa_info(self) -> None:
        add = tac_ast.Binary(
            op=tac_ast.Add(), src1=_var("s"), src2=_uc(1), dst=_var("t"),
        )
        store = tac_ast.Copy(src=_uc(5), dst=_var("s"))
        read = tac_ast.Copy(src=_var("s"), dst=_var("g"))
        fn = _fn(add, store, read, tac_ast.Copy(src=_var("t"), dst=_var("h")))
        out = SinkIncrements().run(fn, PassContext())
        self.assertEqual(out.instructions, fn.instructions)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestSinkIncrementSim(unittest.TestCase):
    def _both(self, src: str, expected: int) -> None:
        for optimize in (False, True):
            r = build_sim(src, optimize=optimize).run(max_cycles=200_000)
            self.assertFalse(r.timed_out, f"optimize={optimize}")
            self.assertEqual(r.return_int(), expected, f"optimize={optimize}")

    def test_static_written_before_last_read(self) -> None:
        # The add of the static `s` moved past `s = 5`, so h got 6.
        src = (
            "static unsigned s;\n"
            "unsigned g, h;\n"
            "void f(void) { unsigned t = s + 1; s = 5; g = s; h = t; }\n"
            "int main(void) { s = 1; f(); return h * 10 + g; }\n"
        )
        self._both(src, 25)

    def test_src_y_postincrement_chain(self) -> None:
        src = (
            "#include <stdint.h>\n"
            "uint8_t tab[4] = {3, 5, 7, 9};\n"
            "uint8_t out[3];\n"
            "void copy3(uint8_t y) {\n"
            "    out[0] = tab[y++];\n"
            "    out[1] = tab[y++];\n"
            "    out[2] = tab[y];\n"
            "}\n"
            "int main(void) { copy3(1); return out[0] + out[1] * 16; }\n"
        )
        self._both(src, 5 + 7 * 16)

    def test_post_decrement_tests(self) -> None:
        # `x-- != 0` tests the decremented value against -1, for each
        # width and signedness, with the count running out at 0 and
        # at a match.
        src = (
            "#include <stdint.h>\n"
            "uint8_t tab[8] = {9, 8, 7, 6, 5, 4, 3, 2};\n"
            "uint8_t find(uint8_t c) {\n"
            "    uint8_t x = 7;\n"
            "    do { if (tab[x] == c) return x + 10; } while (x-- != 0);\n"
            "    return 0;\n"
            "}\n"
            "int8_t sdown(int8_t n) { int8_t k = 0;\n"
            "    while (n-- != -3) k++; return k; }\n"
            "uint16_t wdown(uint16_t n) { uint16_t k = 0;\n"
            "    while (n--) k += 2; return k; }\n"
            "int main(void) {\n"
            "    return find(5) + find(1) * 100 + sdown(4) * 1000\n"
            "        + (int)wdown(300);\n"
            "}\n"
        )
        self._both(src, (4 + 10) + 0 + 7 * 1000 + 600)

    def test_post_decrement_becomes_dex(self) -> None:
        from compile import _run_stage
        from preprocessor import preprocess
        src = (
            "#include <stdint.h>\n"
            "uint8_t tab[8];\n"
            "uint8_t find(uint8_t c) {\n"
            "    uint8_t x = 7;\n"
            "    do { if (tab[x] == c) return x + 8; } while (x-- != 0);\n"
            "    return 0;\n"
            "}\n"
        )
        asm = _run_stage("codegen", preprocess(src), optimize=True)
        self.assertIn("CPX   #$FF", asm)
        self.assertNotIn("SBC", asm)


if __name__ == "__main__":
    unittest.main()
