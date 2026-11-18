"""Tests for passes/loop_constant_a.py, which loads a loop's one
constant into A in front of the loop."""

import unittest

import asm_ast
from passes.loop_constant_a import apply_loop_constant_a

_A = asm_ast.Reg(reg=asm_ast.A())
_X = asm_ast.Reg(reg=asm_ast.X())
_Y = asm_ast.Reg(reg=asm_ast.Y())


def _run(instrs):
    prog = asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])
    return apply_loop_constant_a(prog).top_level[0].instructions


def _clear_loop(after=None):
    """The loop `c99_to_tac._emit_zero_bytes` leaves under -O:

        LDY #20
    .clear:
        DEY
        LDA #0
        STA v,Y
        TYA
        BNE .clear
        <after>
        RTS
    """
    return [
        asm_ast.Mov(src=asm_ast.Imm(value=20), dst=_Y),
        asm_ast.Label(name=".clear"),
        asm_ast.Dec(dst=_Y),
        asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A),
        asm_ast.Mov(src=_A, dst=asm_ast.IndexedData(
            name="v", offset=0, index=asm_ast.Y())),
        asm_ast.Mov(src=_Y, dst=_A),
        asm_ast.Branch(cond=asm_ast.NE(), target=".clear"),
        *(after or []),
        asm_ast.Return(save_a=False),
    ]


class TestLoopConstantA(unittest.TestCase):

    def test_clear_loop(self):
        # The load goes in front of the header, and the TYA goes,
        # since the BNE reads the flags DEY set.
        out = _run(_clear_loop())
        self.assertEqual(out, [
            asm_ast.Mov(src=asm_ast.Imm(value=20), dst=_Y),
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A),
            asm_ast.Label(name=".clear"),
            asm_ast.Dec(dst=_Y),
            asm_ast.Mov(src=_A, dst=asm_ast.IndexedData(
                name="v", offset=0, index=asm_ast.Y())),
            asm_ast.Branch(cond=asm_ast.NE(), target=".clear"),
            asm_ast.Return(save_a=False),
        ])

    def test_a_read_after_the_loop(self):
        # A read of the value the TYA puts in A keeps the loop as
        # it is.
        after = [asm_ast.Mov(src=_A, dst=asm_ast.Data(name="out"))]
        loop = _clear_loop()
        # Store A between the TYA and the branch: the TYA's A is read.
        loop.insert(6, asm_ast.Mov(src=_A, dst=asm_ast.Data(name="out")))
        self.assertEqual(_run(loop), loop)
        # With the store after the loop, the TYA's A is read there.
        loop = _clear_loop(after)
        self.assertEqual(_run(loop), loop)

    def test_two_constants(self):
        # Loads of 0 and of $FF in one loop leave A changing.
        loop = _clear_loop()
        loop[4:4] = [
            asm_ast.Mov(src=asm_ast.Imm(value=0xFF), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="w")),
        ]
        self.assertEqual(_run(loop), loop)

    def test_other_write_of_a(self):
        # An LDA of a byte in the loop is another value in A.
        loop = _clear_loop()
        loop[2:2] = [
            asm_ast.Mov(src=asm_ast.Data(name="m"), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="w")),
        ]
        self.assertEqual(_run(loop), loop)

    def test_call_in_loop(self):
        loop = _clear_loop()
        loop.insert(2, asm_ast.Call(name="g"))
        self.assertEqual(_run(loop), loop)

    def test_a_live_at_header(self):
        # A read before the load in the loop reads the value A had
        # before the loop on the first pass.
        loop = _clear_loop()
        loop.insert(2, asm_ast.Mov(src=_A, dst=asm_ast.Data(name="w")))
        self.assertEqual(_run(loop), loop)

    def test_flags_of_load_read(self):
        # A branch on the load's Z keeps the load.
        loop = _clear_loop()
        loop.insert(4, asm_ast.Branch(cond=asm_ast.EQ(), target=".clear"))
        self.assertEqual(_run(loop), loop)

    def test_entered_by_a_jump(self):
        # A jump to the header from outside the loop would skip the
        # new load.
        loop = [
            asm_ast.Jump(target=".clear"),
            asm_ast.Label(name=".other"),
        ] + _clear_loop()
        self.assertEqual(_run(loop), loop)

    def test_header_after_terminator(self):
        # Code before the header that ends in RTS doesn't fall into
        # the loop, so nothing would run the new load.
        loop = _clear_loop()
        loop.insert(1, asm_ast.Return(save_a=False))
        self.assertEqual(_run(loop), loop)

    def test_inc_indexed_by_y(self):
        # There's no INC abs,Y, so `asm_emit` goes through A for it.
        loop = _clear_loop()
        loop.insert(2, asm_ast.Inc(dst=asm_ast.IndexedData(
            name="w", offset=0, index=asm_ast.Y())))
        self.assertEqual(_run(loop), loop)

    def test_count_up_without_transfer(self):
        # LDX #0; .l: LDA #0; STA v,X; INX; BNE .l moves the load
        # only.
        loop = [
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_X),
            asm_ast.Label(name=".l"),
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.IndexedData(
                name="v", offset=0, index=asm_ast.X())),
            asm_ast.Inc(dst=_X),
            asm_ast.Branch(cond=asm_ast.NE(), target=".l"),
            asm_ast.Return(save_a=False),
        ]
        self.assertEqual(_run(loop), [
            loop[0], loop[2], loop[1], loop[3], loop[4], loop[5],
            loop[6],
        ])


if __name__ == "__main__":
    unittest.main()
