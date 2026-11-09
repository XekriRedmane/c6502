"""Tests for `passes.loop_invariant_a`."""

from __future__ import annotations

import unittest

import asm_ast
from passes.loop_invariant_a import apply_loop_invariant_a

_A = asm_ast.Reg(reg=asm_ast.A())
_X = asm_ast.Reg(reg=asm_ast.X())
_Y = asm_ast.Reg(reg=asm_ast.Y())
_KEY = asm_ast.Data(name="key", offset=0)
_KEY_HI = asm_ast.Data(name="key", offset=1)
_COUNT = asm_ast.Data(name="count", offset=0)


def _tab(base: int) -> asm_ast.IndexedData:
    return asm_ast.IndexedData(name="", offset=base, index=asm_ast.Y())


def _lda(src):
    return asm_ast.Mov(src=src, dst=_A, is_volatile=False)


def _run(instrs):
    prog = asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )])
    return apply_loop_invariant_a(prog).top_level[0].instructions


def _search_loop(extra_in_loop=()):
    """The shape `find_index_of_page_table` compiles to before the
    pass: compare table bytes against a two-byte key."""
    return [
        asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_Y, is_volatile=False),
        asm_ast.Label(name=".loop"),
        _lda(_tab(0x2200)),
        asm_ast.Compare(left=_A, right=_KEY),
        asm_ast.Branch(cond=asm_ast.NE(), target=".next"),
        _lda(_tab(0x2280)),
        asm_ast.Compare(left=_A, right=_KEY_HI),
        asm_ast.Branch(cond=asm_ast.NE(), target=".next"),
        asm_ast.Mov(src=_Y, dst=_A, is_volatile=False),
        asm_ast.Return(save_a=True),
        asm_ast.Label(name=".next"),
        *extra_in_loop,
        asm_ast.Inc(dst=_Y),
        asm_ast.Dec(dst=_COUNT),
        asm_ast.Branch(cond=asm_ast.NE(), target=".loop"),
        asm_ast.Mov(src=asm_ast.Imm(value=0xFF), dst=_A, is_volatile=False),
        asm_ast.Return(save_a=True),
    ]


class TestLoopInvariantA(unittest.TestCase):

    def test_key_hoisted_with_reload_on_clobbering_path(self):
        out = _run(_search_loop())
        loop = out.index(asm_ast.Label(name=".loop"))
        # Loaded once before the loop; the loop compares the table
        # against A.
        self.assertEqual(out[loop - 1], _lda(_KEY))
        self.assertEqual(out[loop + 1], asm_ast.Compare(left=_A, right=_tab(0x2200)))
        # The high-byte mismatch branch goes through a reload stub
        # placed after the RTS, which falls into `.next`.
        nxt = out.index(asm_ast.Label(name=".next"))
        self.assertEqual(out[nxt - 1], _lda(_KEY))
        stub = out[nxt - 2]
        self.assertIsInstance(stub, asm_ast.Label)
        self.assertEqual(out[loop + 5], asm_ast.Branch(cond=asm_ast.NE(), target=stub.name))
        # The fast path (low-byte mismatch) has no load.
        self.assertEqual(out[loop + 2], asm_ast.Branch(cond=asm_ast.NE(), target=".next"))

    def test_key_written_in_loop_blocks_rewrite(self):
        instrs = _search_loop([asm_ast.Inc(dst=_KEY)])
        self.assertEqual(_run(instrs), instrs)

    def test_call_in_loop_blocks_rewrite(self):
        instrs = _search_loop([asm_ast.Call(name="g", reg_args=[])])
        self.assertEqual(_run(instrs), instrs)

    def test_a_read_after_compare_blocks_rewrite(self):
        # `.next` stores A, so A must still hold the table byte there.
        instrs = _search_loop([asm_ast.Mov(src=_A, dst=asm_ast.Data(name="t", offset=0), is_volatile=False)])
        self.assertEqual(_run(instrs), instrs)

    def test_carry_read_after_compare_blocks_rewrite(self):
        # Swapping the CMP changes C, so a BCC after it must keep the
        # original operand order.
        instrs = _search_loop([asm_ast.Branch(cond=asm_ast.CC(), target=".loop")])
        self.assertEqual(_run(instrs), instrs)


if __name__ == "__main__":
    unittest.main()
