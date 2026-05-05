"""Tests for the asm-peephole fixed-point loop.

`_peephole_fixedpoint` runs `apply_inc_peephole`,
`apply_direct_index_load`, and `apply_redundant_load_elimination`
in sequence, repeating until a full sweep is a no-op. The
property tested here is convergence — the loop terminates with a
program that no further single-pass run can improve.
"""
from __future__ import annotations

import unittest
from unittest.mock import patch

import asm_ast
from compile import _peephole_fixedpoint, _PEEPHOLE_FIXEDPOINT_CAP
from passes.direct_index_load import apply_direct_index_load
from passes.inc_peephole import apply_inc_peephole
from passes.redundant_load import apply_redundant_load_elimination


_REG_A = asm_ast.Reg(reg=asm_ast.A())
_REG_X = asm_ast.Reg(reg=asm_ast.X())


def _fn(instrs: list[asm_ast.Type_instruction]) -> asm_ast.Function:
    return asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )


def _prog(instrs: list[asm_ast.Type_instruction]) -> asm_ast.Program:
    return asm_ast.Program(top_level=[_fn(instrs)])


class TestFixedpoint(unittest.TestCase):
    def test_empty_function_unchanged(self) -> None:
        prog = _prog([asm_ast.Return(save_a=False)])
        out = _peephole_fixedpoint(prog)
        self.assertEqual(out, prog)

    def test_already_converged_returns_unchanged(self) -> None:
        # A program with no peephole opportunities (no INC chain,
        # no LDA-TAX pair, no redundant load): `_peephole_fixedpoint`
        # returns it unchanged — and the equality check confirms
        # that the loop terminated after one no-op sweep.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        zpC0 = asm_ast.ZP(address=0xC0, offset=0)
        prog = _prog([
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=zpC0),
            asm_ast.Return(save_a=False),
        ])
        out = _peephole_fixedpoint(prog)
        self.assertEqual(out, prog)

    def test_inc_chain_collapses(self) -> None:
        # Verify that a one-byte ADC #1 chain collapses to INC and
        # the loop terminates.
        zp90 = asm_ast.ZP(address=0x90, offset=0)
        prog = _prog([
            asm_ast.Mov(src=zp90, dst=_REG_A),
            asm_ast.ClearCarry(),
            asm_ast.Add(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=zp90),
            asm_ast.Return(save_a=False),
        ])
        out = _peephole_fixedpoint(prog)
        # The 4-instruction chain collapses to a single Inc; the
        # remaining instructions are Inc + Return.
        instrs = out.top_level[0].instructions
        self.assertEqual(len(instrs), 2)
        self.assertIsInstance(instrs[0], asm_ast.Inc)

    def test_inc_chain_then_redundant_load_fires_in_one_iter(self) -> None:
        # The INC peephole replaces an LDA/CLC/ADC/STA chain on
        # $90 with a single INC. The LDA $80 immediately after the
        # chain is then redundant (A still holds $80 — INC $90
        # writes to a disjoint ZP cell). Both rewrites happen in
        # one iteration of the fixed-point loop.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        zp90 = asm_ast.ZP(address=0x90, offset=0)
        zpC0 = asm_ast.ZP(address=0xC0, offset=0)
        prog = _prog([
            asm_ast.Mov(src=zp80, dst=_REG_A),
            # 4-instruction inc chain on $90 (eligible for inc_peephole).
            asm_ast.Mov(src=zp90, dst=_REG_A),
            asm_ast.ClearCarry(),
            asm_ast.Add(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=zp90),
            # Re-load $80; after inc_peephole reduces the chain
            # above to a single INC $90, this becomes redundant
            # because A still mirrors $80 from instruction 0
            # (INC $90 writes to a disjoint ZP cell, so A's
            # tracking survives).
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=zpC0),
            asm_ast.Return(save_a=False),
        ])
        out = _peephole_fixedpoint(prog)
        instrs = out.top_level[0].instructions
        # Expected after one iteration: LDA $80; INC $90; STA $C0; Ret
        # (the redundant LDA $80 is gone).
        self.assertEqual(len(instrs), 4)
        self.assertIsInstance(instrs[0], asm_ast.Mov)
        self.assertIsInstance(instrs[1], asm_ast.Inc)
        self.assertIsInstance(instrs[2], asm_ast.Mov)

    def test_cap_raises_on_pathological_pass(self) -> None:
        # Pathological pass that always reports "changed" by
        # mutating a single byte's name. Use unittest.mock to
        # patch one of the three passes; the cap should surface
        # the failure rather than loop forever.
        zp80 = asm_ast.ZP(address=0x80, offset=0)
        prog = _prog([
            asm_ast.Mov(src=zp80, dst=_REG_A),
            asm_ast.Return(save_a=False),
        ])

        # Each call to the patched pass returns a new Program
        # whose top_level Function is freshly constructed, so
        # the equality check sees "changed" every iteration.
        rebuild_count = [0]

        def always_change(p):
            rebuild_count[0] += 1
            fn = p.top_level[0]
            new_fn = asm_ast.Function(
                name=fn.name + "_x" * rebuild_count[0],
                is_global=fn.is_global,
                params=list(fn.params),
                instructions=list(fn.instructions),
            )
            return asm_ast.Program(top_level=[new_fn])

        with patch(
            "compile.apply_redundant_load_elimination", always_change,
        ):
            with self.assertRaises(AssertionError) as cm:
                _peephole_fixedpoint(prog)
            self.assertIn("didn't converge", str(cm.exception))
            # Should have run exactly _PEEPHOLE_FIXEDPOINT_CAP iterations.
            self.assertEqual(rebuild_count[0], _PEEPHOLE_FIXEDPOINT_CAP)


if __name__ == "__main__":
    unittest.main()
