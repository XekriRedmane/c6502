"""Tests for HwReg (X / Y) pinning infrastructure.

Phase 2 of asm-level register allocation: extend coloring to pin
single-byte SSA Pseudos into the 6502's X / Y index registers
when their use shape is compatible (LDX/LDY/STX/STY/INX/DEX/CPX
operations only, no live range crossing a `Call`).

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).
"""
from __future__ import annotations

import shutil
import unittest

import asm_ast
from passes.optimization_asm.apply_coloring import _match_transfer_chain, _rewrite_redundant_transfers


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


class TestApplyColoringHwReg(unittest.TestCase):
    """Unit tests for HwReg substitution + redundant-transfer
    chain elimination."""


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

    def _compile(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage(
            "codegen", preprocess(src), optimize=True,
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
