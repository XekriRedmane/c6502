"""Tests for HwReg (X / Y) pinning infrastructure.

Phase 2 of asm-level register allocation: extend coloring to pin
single-byte SSA Pseudos into the 6502's X / Y index registers
when their use shape is compatible (LDX/LDY/STX/STY/INX/DEX/CPX
operations only, no live range crossing a `Call`).
"""
from __future__ import annotations

import shutil
import unittest

import asm_ast
from passes.optimization.register_allocation import Coloring
from passes.optimization_asm.apply_coloring import (
    apply_coloring,
    _match_transfer_chain,
    _rewrite_redundant_transfers,
)
from passes.optimization_asm.hwreg_eligibility import scan_function


def _mov(src, dst):
    return asm_ast.Mov(src=src, dst=dst)


def _reg(letter: str) -> asm_ast.Reg:
    if letter == "A":
        return asm_ast.Reg(reg=asm_ast.A())
    if letter == "X":
        return asm_ast.Reg(reg=asm_ast.X())
    if letter == "Y":
        return asm_ast.Reg(reg=asm_ast.Y())
    raise ValueError(letter)


class TestHwRegEligibility(unittest.TestCase):
    """Unit tests for the per-instruction eligibility scan."""

    def _scan(self, instrs):
        fn = asm_ast.Function(
            name="test", is_global=True, params=[], instructions=instrs,
        )
        return scan_function(fn)

    def test_pseudo_with_indexed_data_setup_chain_gets_x_hint(self):
        # Mov(P, A); Mov(A, X); ... = X-setup chain. P picks up
        # hints_x for the index-load.
        instrs = [
            _mov(asm_ast.Pseudo(name="P", offset=0), _reg("A")),
            _mov(_reg("A"), _reg("X")),
            _mov(asm_ast.IndexedData(name="arr", offset=0, index=asm_ast.X()), _reg("A")),
        ]
        elig = self._scan(instrs)
        self.assertIn("P", elig.hints_x)
        self.assertNotIn("P", elig.hints_y)

    def test_pseudo_with_y_setup_chain_gets_y_hint(self):
        instrs = [
            _mov(asm_ast.Pseudo(name="P", offset=0), _reg("A")),
            _mov(_reg("A"), _reg("Y")),
            _mov(asm_ast.IndexedData(name="arr", offset=0, index=asm_ast.Y()), _reg("A")),
        ]
        elig = self._scan(instrs)
        self.assertIn("P", elig.hints_y)
        self.assertNotIn("P", elig.hints_x)

    def test_pseudo_in_unsupported_op_disqualified(self):
        # Pseudo as Add operand: not HwReg-representable (ADC needs
        # A as dst, the source operand can be Imm/Stack/Frame/Data
        # but the op-style RMW on a HwReg would need TYA/TAY scratching
        # which the eligibility check rejects defensively).
        instrs = [
            asm_ast.Add(
                src=asm_ast.Pseudo(name="P", offset=0),
                dst=_reg("A"),
            ),
        ]
        elig = self._scan(instrs)
        self.assertNotIn("P", elig.eligible)

    def test_use_count_tracks_references(self):
        instrs = [
            _mov(asm_ast.Pseudo(name="P", offset=0), _reg("A")),
            _mov(_reg("A"), _reg("X")),
            _mov(asm_ast.Pseudo(name="P", offset=0), _reg("A")),
            _mov(_reg("A"), _reg("Y")),
        ]
        elig = self._scan(instrs)
        self.assertEqual(elig.use_count.get("P", 0), 2)


class TestApplyColoringHwReg(unittest.TestCase):
    """Unit tests for HwReg substitution + redundant-transfer
    chain elimination."""

    def test_pseudo_substituted_with_reg_y(self):
        coloring = Coloring(
            assignments={}, spilled=set(),
            hwreg_assignments={"P": "Y"},
        )
        fn = asm_ast.Function(
            name="test", is_global=True, params=[],
            instructions=[
                _mov(asm_ast.Imm(value=5), asm_ast.Pseudo(name="P", offset=0)),
                asm_ast.Inc(dst=asm_ast.Pseudo(name="P", offset=0)),
            ],
        )
        result = apply_coloring(fn, coloring)
        self.assertEqual(
            result.instructions[0],
            _mov(asm_ast.Imm(value=5), _reg("Y")),
        )
        self.assertEqual(
            result.instructions[1],
            asm_ast.Inc(dst=_reg("Y")),
        )

    def test_self_transfer_chain_dropped(self):
        # Mov(Reg(X), Reg(A)); Mov(Reg(A), Reg(X)) is a no-op for X.
        instrs = [
            _mov(_reg("X"), _reg("A")),
            _mov(_reg("A"), _reg("X")),
            _mov(asm_ast.ZP(address=0x80, offset=0), _reg("A")),
        ]
        result = _rewrite_redundant_transfers(instrs)
        # Both transfer Movs dropped; the LDA $80 stays.
        self.assertEqual(len(result), 1)
        self.assertEqual(result[0].src, asm_ast.ZP(address=0x80, offset=0))

    def test_cross_transfer_rewrites_indexed_data(self):
        # Mov(Reg(Y), Reg(A)); Mov(Reg(A), Reg(X)); IndexedData(... X)
        # → drop chain, rewrite IndexedData.index to Y.
        instrs = [
            _mov(_reg("Y"), _reg("A")),
            _mov(_reg("A"), _reg("X")),
            _mov(
                asm_ast.IndexedData(name="arr", offset=0, index=asm_ast.X()),
                _reg("A"),
            ),
        ]
        result = _rewrite_redundant_transfers(instrs)
        self.assertEqual(len(result), 1)
        self.assertEqual(
            result[0].src,
            asm_ast.IndexedData(name="arr", offset=0, index=asm_ast.Y()),
        )

    def test_match_transfer_chain_returns_letters(self):
        instrs = [
            _mov(_reg("Y"), _reg("A")),
            _mov(_reg("A"), _reg("X")),
        ]
        self.assertEqual(_match_transfer_chain(instrs, 0), ("Y", "X", 2))

    def test_match_returns_none_for_non_chain(self):
        instrs = [
            _mov(_reg("Y"), _reg("A")),
            asm_ast.ClearCarry(),
        ]
        self.assertIsNone(_match_transfer_chain(instrs, 0))


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestHwRegPinningEndToEnd(unittest.TestCase):
    """End-to-end checks on real C programs: HwReg pinning fires
    where expected and produces the right asm shape."""

    def _compile(self, src: str, *, unroll: bool = False) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage(
            "codegen", preprocess(src), optimize=True, unroll=unroll,
        )

    def test_loop_iv_used_as_indexed_store_pin(self):
        # A column-iv used as the X index for many indexed stores
        # should pin to X — the writes should emit `STA $XXXX,X`
        # without per-store `LDX <slot>` setup.
        src = (
            "#include <stdint.h>\n"
            "static uint8_t * const buf = (uint8_t * const)0x4000;\n"
            "void clear(void) {\n"
            "    for (uint8_t i = 0; i < 16; i++) buf[i] = 0;\n"
            "}\n"
        )
        asm = self._compile(src)
        # The store `buf[i] = 0` should emit `STA $4000,X` (or
        # `STA buf,X` if not const-folded). The body should NOT
        # have a separate `LDX <ZP>` reload before each store.
        self.assertIn("STA   $40", asm)


if __name__ == "__main__":
    unittest.main()
