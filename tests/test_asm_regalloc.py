"""Behavioral tests for cpp/src/passes/optimization_asm/regalloc.h.

Coverage:
  - Coloring assigns ZP addresses to byte-granular SSA names.
  - Two non-interfering names can share a color.
  - Two interfering names get different colors.
  - Cross-call values land in the callee-saved pool.
  - End-to-end: --optimize-asm produces ZP loads/stores ($XX
    addresses) for a small program with 2+ live locals, matching
    the kind of placement -O achieves.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).

The `-S -O` text is checked in cpp/tests/test_asm_regalloc.cpp.
"""
from __future__ import annotations

import unittest


class TestFrameAccessWritesY(unittest.TestCase):
    """A `Pseudo` that `replace_pseudoregisters` puts in the frame is
    read with `LDY #off; LDA (FP),Y`, so no value may stay in Y across
    it (`fuzz.calls` seed 335)."""

    def test_shift_count_survives_parameter_read(self) -> None:
        # `f0` calls through a pointer, so it's a soft-stack function
        # and reads `p0` from its frame. The count `l1 & 7` of the
        # second shift was kept in Y across the read of `p0` for the
        # first shift's count, whose `LDY #4` replaced it, so the
        # result was shifted by 4 more places and came out 0.
        src = (
            "unsigned _BitInt(58) f3(long p0) { return 1; }\n"
            "signed char f0(unsigned _BitInt(58) p0) {\n"
            "    signed char l1 = p0;\n"
            "    l1 ^= 5;\n"
            "    unsigned _BitInt(58) (*fp)(long) = f3;\n"
            "    fp(~p0);\n"
            "    return ((100 << (p0 & 7)) << (l1 & 7));\n"
            "}\n"
            "int main(void) { return f0(236); }\n"
        )
        from sim.harness import build_sim
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(src, optimize=optimize).run()
                self.assertEqual(got.return_int_signed(), -128)

    def test_index_survives_spilled_reads(self) -> None:
        # Eighteen 8-byte values live at once are more than the 128
        # bytes of the default pool, so some are spilled to `f`'s
        # frame. The index `c2` was kept in Y across the reads of the
        # spilled values for `gout`, whose `LDY #off` replaced it, and
        # `f` returned 4.
        loads = "".join(
            f"    unsigned long long v{k} = g[{k}];\n" for k in range(18)
        )
        mix = " ^ ".join(f"v{k}" for k in range(18))
        src = (
            "volatile unsigned long long g[18];\n"
            "volatile unsigned char gc[2];\n"
            "unsigned long long gout;\n"
            "unsigned char t[8] = {1, 2, 3, 4, 5, 6, 7, 8};\n"
            "unsigned char h(unsigned char x) { return x; }\n"
            "int f(void) {\n"
            "    unsigned char (*fp)(unsigned char) = h;\n"
            "    fp(0);\n"
            f"{loads}"
            "    unsigned char c1 = gc[0] & 7;\n"
            "    unsigned char c2 = (gc[1] ^ 5) & 7;\n"
            f"    gout = {mix};\n"
            "    return t[c1] + t[c2] * 3;\n"
            "}\n"
            "int main(void) {\n"
            "    for (int k = 0; k < 18; k++)\n"
            "        g[k] = 0x0101010101010101ULL * (k + 1);\n"
            "    gc[0] = 3;\n"
            "    gc[1] = 7;\n"
            "    return f();\n"
            "}\n"
        )
        # Without -O the frame is over the 253-byte limit, so the TAC
        # interpreter gives the reference.
        from sim.harness import build_sim
        from tests.test_sim_chapters import _run_program
        self.assertEqual(_run_program(src), 13)
        got = build_sim(src, optimize=True).run()
        self.assertEqual(got.return_int_signed(), 13)


if __name__ == "__main__":
    unittest.main()
