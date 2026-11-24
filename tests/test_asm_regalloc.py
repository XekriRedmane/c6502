"""Behavioral tests for `passes.optimization_asm.regalloc`.

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
"""
from __future__ import annotations

import io
import unittest
from unittest.mock import patch

import asm_ast
from compile import main


def _A() -> asm_ast.Reg:
    return asm_ast.Reg(reg=asm_ast.A())


def _ps(name: str, off: int = 0) -> asm_ast.Pseudo:
    return asm_ast.Pseudo(name=name, offset=off)


def _mov(src, dst) -> asm_ast.Mov:
    return asm_ast.Mov(src=src, dst=dst)


class TestAsmRegallocEndToEnd(unittest.TestCase):
    """Compile real programs through `-S -O` and
    verify the byte-granular regalloc is at least exercised end-
    to-end."""

    def _run(self, argv: list[str], stdin: str = "") -> tuple[int, str, str]:
        with patch("sys.stdin", io.StringIO(stdin)), \
             patch("sys.stdout", new_callable=io.StringIO) as out, \
             patch("sys.stderr", new_callable=io.StringIO) as err:
            rc = main(argv)
        return rc, out.getvalue(), err.getvalue()

    def test_two_locals_land_in_zp(self) -> None:
        # A program that benefits from ZP coloring: enough live
        # locals to motivate caller-saved ZP allocation. Verify
        # the optimized output has at least one ZP-mode load at a
        # caller-saved-pool address.
        src = (
            "int f(int p, int q) { int a = p + 1; int b = a + q;"
            " return b; }\n"
            "int main(void) { return f(3, 4); }\n"
        )
        rc, out, _ = self._run(
            ["compile.py", "-", "-S", "-O"], stdin=src,
        )
        self.assertEqual(rc, 0)
        # At least one body-local symbolic load with an EQU
        # binding in the caller-saved ZP range.
        import re
        self.assertRegex(out, r"LDA\s+__local_\w+__\w+")
        self.assertRegex(out, r"__local_\w+__\w+\s+EQU\s+\$[89AB][0-9A-F]")


class TestFrameAccessWritesY(unittest.TestCase):
    """A `Pseudo` that `replace_pseudoregisters` puts in the frame is
    read with `LDY #off; LDA (FP),Y`, so no value may stay in Y across
    it (`fuzz.calls` seed 335)."""

    def test_frame_name_counts_as_y_write(self) -> None:
        from passes.optimization_asm.regalloc import _instr_writes_hwregs
        read = _mov(_ps("@1.p0"), _A())
        self.assertEqual(_instr_writes_hwregs(read), ())
        self.assertEqual(
            _instr_writes_hwregs(read, frozenset({"@1.p0"})), ("Y",),
        )

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
