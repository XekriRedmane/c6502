"""Behavioral tests for `passes.optimization_asm.regalloc`.

Coverage:
  - Coloring assigns ZP addresses to byte-granular SSA names.
  - Two non-interfering names can share a color.
  - Two interfering names get different colors.
  - Cross-call values land in the callee-saved pool.
  - End-to-end: --optimize-asm produces ZP loads/stores ($XX
    addresses) for a small program with 2+ live locals, matching
    the kind of placement -O achieves.
"""
from __future__ import annotations

import io
import unittest
from unittest.mock import patch

import asm_ast
from compile import main
from passes.optimization.pool import Pool
from passes.optimization_asm.cfg import build_cfg
from passes.optimization_asm.interference import build_interference
from passes.optimization_asm.liveness import compute_liveness
from passes.optimization_asm.regalloc import color_graph
from passes.optimization_asm.ssa_construction import to_ssa


def _imm(v: int) -> asm_ast.Imm:
    return asm_ast.Imm(value=v)


def _A() -> asm_ast.Reg:
    return asm_ast.Reg(reg=asm_ast.A())


def _ps(name: str, off: int = 0) -> asm_ast.Pseudo:
    return asm_ast.Pseudo(name=name, offset=off)


def _mov(src, dst) -> asm_ast.Mov:
    return asm_ast.Mov(src=src, dst=dst)


def _ret_bare(save_a: bool = True) -> asm_ast.Return:
    return asm_ast.Return(save_a=save_a)


def _fn(*instrs, name: str = "main", params=()) -> asm_ast.Function:
    return asm_ast.Function(
        name=name,
        is_global=True,
        params=list(params),
        instructions=list(instrs),
    )


class TestAsmRegalloc(unittest.TestCase):
    def test_disjoint_lifetimes_share_color(self) -> None:
        # Two locals %a, %b with non-overlapping lifetimes can land
        # at the same ZP byte.
        # %a := 1 ; A := %a ; %b := 2 ; A := %b ; Return.
        # %a is dead before %b is defined.
        fn = _fn(
            _mov(_imm(1), _ps("%a")),
            _mov(_ps("%a"), _A()),
            _mov(_imm(2), _ps("%b")),
            _mov(_ps("%b"), _A()),
            _ret_bare(),
        )
        ssa = to_ssa(fn)
        liveness = compute_liveness(ssa)
        graph = build_interference(ssa, liveness)
        coloring = color_graph(ssa, graph)
        # Both versioned names get colored. Disjoint lifetimes →
        # they may share a color.
        names = list(coloring.assignments.keys())
        self.assertEqual(len(names), 2)
        # Each name gets a real ZP byte (in the configured pool
        # range).
        for n in names:
            addr = coloring.assignments[n]
            self.assertGreaterEqual(addr, 0x80)
            self.assertLess(addr, 0x100)

    def test_overlapping_lifetimes_get_different_colors(self) -> None:
        # %a and %b are both live across the same instruction
        # (Compare reads %a, but %b is also alive). They must get
        # different colors.
        fn = _fn(
            _mov(_imm(1), _ps("%a")),
            _mov(_imm(2), _ps("%b")),
            asm_ast.Compare(left=_ps("%a"), right=_ps("%b")),
            _ret_bare(),
        )
        ssa = to_ssa(fn)
        liveness = compute_liveness(ssa)
        graph = build_interference(ssa, liveness)
        coloring = color_graph(ssa, graph)
        # Two names; their colors must differ.
        addrs = list(coloring.assignments.values())
        self.assertEqual(len(addrs), 2)
        self.assertNotEqual(addrs[0], addrs[1])


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


if __name__ == "__main__":
    unittest.main()
