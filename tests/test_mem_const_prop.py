"""Tests for the basic-block memory-constant forward propagation pass."""

import unittest

import asm_ast
from passes.mem_const_prop import apply_mem_const_prop


_A = asm_ast.Reg(reg=asm_ast.A())
_X = asm_ast.Reg(reg=asm_ast.X())
_Y = asm_ast.Reg(reg=asm_ast.Y())


def _instrs(prog):
    return prog.top_level[0].instructions


def _wrap(instrs):
    return asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])


class TestMemConstProp(unittest.TestCase):

    def test_substitutes_known_data_value_into_ora_source(self):
        # LDA #0; STA M; LDA other; ORA M  → ... ORA #$0
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="M", offset=0)),
            asm_ast.Mov(src=asm_ast.Data(name="other", offset=0), dst=_A),
            asm_ast.Or(src=asm_ast.Data(name="M", offset=0), dst=_A),
        ])
        out = _instrs(apply_mem_const_prop(prog))
        self.assertEqual(out[3], asm_ast.Or(src=asm_ast.Imm(value=0), dst=_A))

    def test_substitutes_known_zp_value(self):
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Imm(value=0xAA), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.ZP(address=0x90, offset=0)),
            asm_ast.Mov(src=asm_ast.Data(name="other", offset=0), dst=_A),
            asm_ast.And(src=asm_ast.ZP(address=0x90, offset=0), dst=_A),
        ])
        out = _instrs(apply_mem_const_prop(prog))
        self.assertEqual(out[3], asm_ast.And(src=asm_ast.Imm(value=0xAA), dst=_A))

    def test_drops_known_value_after_block_boundary_label(self):
        # State resets at a Label — control could enter from elsewhere.
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="M", offset=0)),
            asm_ast.Label(name=".reentry"),
            asm_ast.Or(src=asm_ast.Data(name="M", offset=0), dst=_A),
        ])
        out = _instrs(apply_mem_const_prop(prog))
        self.assertEqual(out[3], asm_ast.Or(src=asm_ast.Data(name="M", offset=0), dst=_A))

    def test_drops_known_value_after_call(self):
        # A Call may write anywhere — invalidate all tracked cells.
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="M", offset=0)),
            asm_ast.Call(name="helper"),
            asm_ast.Or(src=asm_ast.Data(name="M", offset=0), dst=_A),
        ])
        out = _instrs(apply_mem_const_prop(prog))
        self.assertEqual(out[3], asm_ast.Or(src=asm_ast.Data(name="M", offset=0), dst=_A))

    def test_overwrite_invalidates_tracked_cell(self):
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="M", offset=0)),
            asm_ast.Mov(src=asm_ast.Data(name="X", offset=0), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="M", offset=0)),
            asm_ast.Or(src=asm_ast.Data(name="M", offset=0), dst=_A),
        ])
        out = _instrs(apply_mem_const_prop(prog))
        # M was overwritten with an unknown value — no substitution.
        self.assertEqual(out[4], asm_ast.Or(src=asm_ast.Data(name="M", offset=0), dst=_A))

    def test_indexed_write_invalidates_same_name_data(self):
        # IndexedData(M, ...) write can hit any byte of M's region.
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="M", offset=0)),
            asm_ast.Mov(src=_A, dst=asm_ast.IndexedData(name="M", offset=0, index=asm_ast.X())),
            asm_ast.Or(src=asm_ast.Data(name="M", offset=0), dst=_A),
        ])
        out = _instrs(apply_mem_const_prop(prog))
        # No substitution — the indexed write may have clobbered M[0].
        self.assertEqual(out[3], asm_ast.Or(src=asm_ast.Data(name="M", offset=0), dst=_A))

    def test_constant_stored_through_a_is_what_a_holds(self):
        # `Mov(#7, M)` is emitted as `LDA #7; STA M`, so the `STA N` after
        # it stores 7. The 5 loaded before it had been recorded as N's
        # value, and the load of N became `LDA #5`.
        n = asm_ast.Data(name="N", offset=0)
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Imm(value=5), dst=_A),
            asm_ast.Mov(src=asm_ast.Imm(value=7), dst=asm_ast.Data(name="M", offset=0)),
            asm_ast.Mov(src=_A, dst=n),
            asm_ast.Mov(src=asm_ast.Data(name="other", offset=0), dst=_A),
            asm_ast.Or(src=n, dst=_A),
        ])
        out = _instrs(apply_mem_const_prop(prog))
        self.assertEqual(out[4], asm_ast.Or(src=asm_ast.Imm(value=7), dst=_A))

    def test_byte_copied_through_a_leaves_a_unknown(self):
        # `Mov(other, M)` is `LDA other; STA M`, so A no longer holds 5.
        n = asm_ast.Data(name="N", offset=0)
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Imm(value=5), dst=_A),
            asm_ast.Mov(src=asm_ast.Data(name="other", offset=0),
                        dst=asm_ast.Data(name="M", offset=0)),
            asm_ast.Mov(src=_A, dst=n),
            asm_ast.Mov(src=asm_ast.Data(name="other", offset=1), dst=_A),
            asm_ast.Or(src=n, dst=_A),
        ])
        out = _instrs(apply_mem_const_prop(prog))
        self.assertEqual(out[4], asm_ast.Or(src=n, dst=_A))

    def test_move_between_index_registers_leaves_a_unknown(self):
        # `Mov(X, Y)` is `TXA; TAY`.
        n = asm_ast.Data(name="N", offset=0)
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Imm(value=5), dst=_A),
            asm_ast.Mov(src=_X, dst=_Y),
            asm_ast.Mov(src=_A, dst=n),
            asm_ast.Mov(src=asm_ast.Data(name="other", offset=0), dst=_A),
            asm_ast.Or(src=n, dst=_A),
        ])
        out = _instrs(apply_mem_const_prop(prog))
        self.assertEqual(out[4], asm_ast.Or(src=n, dst=_A))

    def test_pull_into_a_leaves_a_unknown(self):
        # `LDA #5; PHA; LDA #3; PLA; STA N` stores 5, not 3.
        n = asm_ast.Data(name="N", offset=0)
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Imm(value=5), dst=_A),
            asm_ast.Push(src=_A),
            asm_ast.Mov(src=asm_ast.Imm(value=3), dst=_A),
            asm_ast.Pop(dst=_A),
            asm_ast.Mov(src=_A, dst=n),
            asm_ast.Mov(src=asm_ast.Data(name="other", offset=0), dst=_A),
            asm_ast.Or(src=n, dst=_A),
        ])
        out = _instrs(apply_mem_const_prop(prog))
        self.assertEqual(out[6], asm_ast.Or(src=n, dst=_A))


if __name__ == "__main__":
    unittest.main()
