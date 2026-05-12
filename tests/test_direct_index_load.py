"""Tests for the direct-into-X/Y peephole.

`apply_direct_index_load` recognizes `Mov(M, Reg(A));
Mov(Reg(A), Reg(X|Y))` where M is `Data` / `ZP` / `Imm` and `A`
is dead immediately after the second Mov. Rewrites to a single
`Mov(M, Reg(X|Y))` — `LDX` / `LDY` directly from memory or
immediate.

Coverage:
  * Asm shape: the canonical IndexedStore + IndexedLoad lowerings
    (which both stage their index through A) collapse to a
    direct LDX.
  * Disqualifications: A live after the pair (read by next
    instruction), Frame / Stack / Indirect operand (LDX/LDY don't
    support indirect-Y), the second Mov targeting A (not X/Y), no
    matching first Mov.
  * End-to-end correctness via the sim — the program writes to
    the right address and produces the right return value.
"""
from __future__ import annotations

import shutil
import unittest

import asm_ast
from passes.direct_index_load import apply_direct_index_load
from sim.harness import build_sim


_REG_A = asm_ast.Reg(reg=asm_ast.A())
_REG_X = asm_ast.Reg(reg=asm_ast.X())
_REG_Y = asm_ast.Reg(reg=asm_ast.Y())


def _fn(instrs: list[asm_ast.Type_instruction]) -> asm_ast.Function:
    return asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )


def _prog(instrs: list[asm_ast.Type_instruction]) -> asm_ast.Program:
    return asm_ast.Program(top_level=[_fn(instrs)])


class TestDirectIndexLoadUnit(unittest.TestCase):
    """Direct calls to `apply_direct_index_load` on synthetic asm."""

    def test_zp_to_x_with_dead_a_folds(self) -> None:
        # LDA $80; TAX; <kills A> — fold to LDX $80; <kills A>.
        instrs = [
            asm_ast.Mov(src=asm_ast.ZP(address=0x80, offset=0), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_REG_X),
            asm_ast.Mov(src=asm_ast.Imm(value=0x42), dst=_REG_A),  # kills A
        ]
        out = apply_direct_index_load(_prog(instrs))
        rebuilt = out.top_level[0].instructions
        self.assertEqual(len(rebuilt), 2)
        self.assertEqual(
            rebuilt[0],
            asm_ast.Mov(src=asm_ast.ZP(address=0x80, offset=0), dst=_REG_X),
        )

    def test_data_to_y_with_dead_a_folds(self) -> None:
        instrs = [
            asm_ast.Mov(src=asm_ast.Data(name="x", offset=0), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_REG_Y),
            asm_ast.Return(save_a=False),  # A dead at end of fn
        ]
        out = apply_direct_index_load(_prog(instrs))
        rebuilt = out.top_level[0].instructions
        self.assertEqual(len(rebuilt), 2)
        self.assertEqual(
            rebuilt[0],
            asm_ast.Mov(src=asm_ast.Data(name="x", offset=0), dst=_REG_Y),
        )

    def test_imm_to_x_with_dead_a_folds(self) -> None:
        # LDA #5; TAX → LDX #5.
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=5), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_REG_X),
            asm_ast.Return(save_a=False),
        ]
        out = apply_direct_index_load(_prog(instrs))
        rebuilt = out.top_level[0].instructions
        self.assertEqual(len(rebuilt), 2)
        self.assertEqual(
            rebuilt[0],
            asm_ast.Mov(src=asm_ast.Imm(value=5), dst=_REG_X),
        )

    def test_a_live_after_does_not_fold(self) -> None:
        # The next instruction after the pair reads A — the LDA
        # was needed; can't drop.
        instrs = [
            asm_ast.Mov(src=asm_ast.ZP(address=0x80, offset=0), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_REG_X),
            # STA somewhere — reads A.
            asm_ast.Mov(
                src=_REG_A, dst=asm_ast.ZP(address=0x90, offset=0),
            ),
            asm_ast.Return(save_a=False),
        ]
        out = apply_direct_index_load(_prog(instrs))
        self.assertEqual(out.top_level[0].instructions, instrs)

    def test_frame_operand_does_not_fold(self) -> None:
        # Frame uses indirect-Y; LDX/LDY have no (ind),Y mode.
        instrs = [
            asm_ast.Mov(src=asm_ast.Frame(offset=4), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_REG_X),
            asm_ast.Return(save_a=False),
        ]
        out = apply_direct_index_load(_prog(instrs))
        self.assertEqual(out.top_level[0].instructions, instrs)

    def test_indirect_operand_does_not_fold(self) -> None:
        # Indirect operands (DPTR-staged) likewise.
        instrs = [
            asm_ast.Mov(src=asm_ast.Indirect(offset=0), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_REG_X),
            asm_ast.Return(save_a=False),
        ]
        out = apply_direct_index_load(_prog(instrs))
        self.assertEqual(out.top_level[0].instructions, instrs)

    def test_second_mov_to_a_does_not_fold(self) -> None:
        # Mov(A, A) — same-register Mov doesn't match X/Y dst.
        # (Self-Mov peephole at emit also drops it, but the
        # peephole here just doesn't apply.)
        instrs = [
            asm_ast.Mov(src=asm_ast.ZP(address=0x80, offset=0), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        out = apply_direct_index_load(_prog(instrs))
        self.assertEqual(out.top_level[0].instructions, instrs)

    def test_first_mov_not_to_a_does_not_fold(self) -> None:
        # The first Mov has to load INTO A. If the dst is already
        # X (e.g., LDX directly), there's nothing to fold.
        instrs = [
            asm_ast.Mov(src=asm_ast.ZP(address=0x80, offset=0), dst=_REG_X),
            asm_ast.Mov(src=_REG_A, dst=_REG_X),  # uses A, not the first
            asm_ast.Return(save_a=False),
        ]
        out = apply_direct_index_load(_prog(instrs))
        self.assertEqual(out.top_level[0].instructions, instrs)

    def test_a_killed_before_pair_fold_still_works(self) -> None:
        # A liveness only matters AFTER the pair. Whatever came
        # before the LDA doesn't matter.
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=99), dst=_REG_A),  # any prior
            asm_ast.Mov(src=asm_ast.ZP(address=0x80, offset=0), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_REG_X),
            asm_ast.Return(save_a=False),
        ]
        out = apply_direct_index_load(_prog(instrs))
        rebuilt = out.top_level[0].instructions
        # The pair starting at index 1 folds. The prior Mov(Imm,A)
        # at index 0 is now dead but this pass doesn't drop it
        # (DCE / backward_copy_propagation handle that).
        self.assertEqual(len(rebuilt), 3)
        self.assertEqual(
            rebuilt[1],
            asm_ast.Mov(src=asm_ast.ZP(address=0x80, offset=0), dst=_REG_X),
        )

    def test_branch_after_pair_with_clean_targets_folds(self) -> None:
        # A Branch after the pair: the CFG-wide A-liveness walk
        # follows both successors (fall-through and target). Here
        # both paths terminate at `Return(save_a=False)` with no
        # intervening read of A, so the fold IS sound.
        instrs = [
            asm_ast.Mov(src=asm_ast.ZP(address=0x80, offset=0), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_REG_X),
            asm_ast.Branch(cond=asm_ast.NE(), target="L"),
            asm_ast.Label(name="L"),
            asm_ast.Return(save_a=False),
        ]
        out = apply_direct_index_load(_prog(instrs))
        rebuilt = out.top_level[0].instructions
        self.assertEqual(
            rebuilt[0],
            asm_ast.Mov(src=asm_ast.ZP(address=0x80, offset=0), dst=_REG_X),
        )

    def test_branch_target_reads_a_blocks_fold(self) -> None:
        # If A is live on at least one path after the Branch, the
        # fold is unsound. Here the Branch target stores A to
        # memory before any kill — A is observably live there.
        instrs = [
            asm_ast.Mov(src=asm_ast.ZP(address=0x80, offset=0), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_REG_X),
            asm_ast.Branch(cond=asm_ast.NE(), target="L"),
            # Fall-through: kill A immediately.
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Return(save_a=False),
            asm_ast.Label(name="L"),
            # Target: read A (STA to memory). A is live here.
            asm_ast.Mov(src=_REG_A, dst=asm_ast.ZP(address=0x90, offset=0)),
            asm_ast.Return(save_a=False),
        ]
        out = apply_direct_index_load(_prog(instrs))
        self.assertEqual(out.top_level[0].instructions, instrs)

    def test_call_kills_a_so_pair_folds(self) -> None:
        # A Call clobbers A, so A is dead just after the call —
        # but here the call comes AFTER the pair. So A is dead at
        # the position right after the pair (before the Call
        # would even start), which is what _a_dead_at checks.
        instrs = [
            asm_ast.Mov(src=asm_ast.ZP(address=0x80, offset=0), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=_REG_X),
            asm_ast.Call(name="foo"),
            asm_ast.Return(save_a=False),
        ]
        out = apply_direct_index_load(_prog(instrs))
        rebuilt = out.top_level[0].instructions
        self.assertEqual(len(rebuilt), 3)
        self.assertEqual(
            rebuilt[0],
            asm_ast.Mov(src=asm_ast.ZP(address=0x80, offset=0), dst=_REG_X),
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestDirectIndexLoadAsmShape(unittest.TestCase):
    """Source-level checks: real C programs lowered through the
    full pipeline produce the expected `LDX` / `LDY` form."""

    def _compile(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_indexed_load_uses_direct_ldx(self) -> None:
        # IndexedLoad's lowering stages the index via A→X. Under
        # --optimize, the loop counter `i` ends up either pinned to
        # the X register directly (HwReg coloring) or living in ZP
        # with a peephole-emitted direct `LDX $XX`. Either way the
        # `arr[i]` access uses absolute,X addressing — `LDA arr,X`
        # — without an `LDA <i>; TAX` pair.
        src = (
            "#include <stdint.h>\n"
            "static const uint8_t arr[10] ="
            " {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};\n"
            "int main(void) {\n"
            "    int s = 0;\n"
            "    for (uint8_t i = 0; i < 10; i++) s += arr[i];\n"
            "    return s;\n"
            "}\n"
        )
        asm = self._compile(src)
        # Direct absolute,X read.
        self.assertIn("LDA   arr,X", asm)
        # No `LDA <i>; TAX` setup pair feeding the indexed load.
        self.assertNotIn("TAX", asm)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestDirectIndexLoadCorrectness(unittest.TestCase):
    """End-to-end correctness."""

    def test_indexed_store_lands_correctly(self) -> None:
        src = (
            "#include <stdint.h>\n"
            "static uint8_t * const buf = (uint8_t * const)0x4100;\n"
            "void put(uint8_t value, uint8_t col) {\n"
            "    buf[col] = value;\n"
            "}\n"
            "int main(void) { put(0x55, 17); return 0; }\n"
        )
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out)
        self.assertEqual(result.memory[0x4100 + 17], 0x55)


if __name__ == "__main__":
    unittest.main()
