"""Tests for `passes.split_mem_to_mem.apply_split_mem_to_mem`."""

import unittest

import asm_ast
from passes.split_mem_to_mem import apply_split_mem_to_mem


A = asm_ast.Reg(reg=asm_ast.A())


def _fn(instrs):
    return asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )


def _instrs(prog):
    return prog.top_level[0].instructions


def _run(instrs):
    return _instrs(apply_split_mem_to_mem(
        asm_ast.Program(top_level=[_fn(instrs)]),
    ))


class TestSplit(unittest.TestCase):
    """Two-memory-operand Movs split into LDA + STA atoms."""

    def test_data_to_data(self):
        src = asm_ast.Data(name="src", offset=0)
        dst = asm_ast.Data(name="dst", offset=0)
        out = _run([asm_ast.Mov(src=src, dst=dst, is_volatile=False)])
        self.assertEqual(len(out), 2)
        self.assertEqual(out[0].src, src)
        self.assertEqual(out[0].dst, A)
        self.assertEqual(out[1].src, A)
        self.assertEqual(out[1].dst, dst)

    def test_zp_to_data(self):
        src = asm_ast.ZP(address=0x80, offset=0)
        dst = asm_ast.Data(name="dst", offset=0)
        out = _run([asm_ast.Mov(src=src, dst=dst, is_volatile=False)])
        self.assertEqual(len(out), 2)
        self.assertEqual(out[0].src, src)
        self.assertEqual(out[0].dst, A)
        self.assertEqual(out[1].dst, dst)

    def test_indexed_data_to_data(self):
        # asm_emit lowers Mov(IndexedData, mem) as LDA name,X; STA dst
        # — same shape as plain mem-to-mem, just with an indexed src.
        src = asm_ast.IndexedData(name="floor_ceil", offset=0,
                                  index=asm_ast.X())
        dst = asm_ast.Data(name="dst", offset=0)
        out = _run([asm_ast.Mov(src=src, dst=dst, is_volatile=False)])
        self.assertEqual(len(out), 2)
        self.assertEqual(out[0].src, src)
        self.assertEqual(out[0].dst, A)
        self.assertEqual(out[1].dst, dst)

    def test_volatile_mem_to_mem_unchanged(self):
        # Volatile mem-to-mem is NOT split — the conservative
        # is_volatile bit doesn't say which operand is the
        # volatile-typed one, so we can't safely mark only one
        # half. Leaves the atom alone for the existing
        # redundant_load._update_for_mov volatile branch to track.
        src = asm_ast.Data(name="src", offset=0)
        dst = asm_ast.Data(name="dst", offset=0)
        instr = asm_ast.Mov(src=src, dst=dst, is_volatile=True)
        out = _run([instr])
        self.assertEqual(len(out), 1)
        self.assertIs(out[0], instr)


class TestSelfMovDrop(unittest.TestCase):
    """`Mov(M, M)` is a no-op — drop it entirely rather than emit
    a useless `LDA M; STA M` pair."""

    def test_drops_data_self_mov(self):
        m = asm_ast.Data(name="m", offset=0)
        out = _run([asm_ast.Mov(src=m, dst=m, is_volatile=False)])
        self.assertEqual(out, [])

    def test_drops_zp_self_mov(self):
        m = asm_ast.ZP(address=0x85, offset=0)
        out = _run([asm_ast.Mov(src=m, dst=m, is_volatile=False)])
        self.assertEqual(out, [])


class TestNonTargets(unittest.TestCase):
    """Movs that aren't mem-to-mem stay unchanged."""

    def test_imm_to_mem_unchanged(self):
        instr = asm_ast.Mov(
            src=asm_ast.Imm(value=5),
            dst=asm_ast.Data(name="dst", offset=0),
            is_volatile=False,
        )
        out = _run([instr])
        self.assertEqual(len(out), 1)
        self.assertIs(out[0], instr)

    def test_reg_to_mem_unchanged(self):
        instr = asm_ast.Mov(
            src=A,
            dst=asm_ast.Data(name="dst", offset=0),
            is_volatile=False,
        )
        out = _run([instr])
        self.assertEqual(len(out), 1)
        self.assertIs(out[0], instr)

    def test_mem_to_reg_unchanged(self):
        instr = asm_ast.Mov(
            src=asm_ast.Data(name="src", offset=0),
            dst=A,
            is_volatile=False,
        )
        out = _run([instr])
        self.assertEqual(len(out), 1)
        self.assertIs(out[0], instr)

    def test_reg_to_reg_unchanged(self):
        instr = asm_ast.Mov(
            src=asm_ast.Reg(reg=asm_ast.X()),
            dst=A,
            is_volatile=False,
        )
        out = _run([instr])
        self.assertEqual(len(out), 1)
        self.assertIs(out[0], instr)


class TestSequenceFromAscFloor(unittest.TestCase):
    """The user-reported pattern from do_ascend: three consecutive
    mem-to-mem Movs sharing a source. After splitting, the redundant
    inner LDAs are explicit and reachable by `redundant_load`."""

    def test_three_consecutive_same_source(self):
        src = asm_ast.Data(name="src", offset=0)
        d1 = asm_ast.Data(name="d1", offset=0)
        d2 = asm_ast.Data(name="d2", offset=0)
        d3 = asm_ast.Data(name="d3", offset=0)
        out = _run([
            asm_ast.Mov(src=src, dst=d1, is_volatile=False),
            asm_ast.Mov(src=src, dst=d2, is_volatile=False),
            asm_ast.Mov(src=src, dst=d3, is_volatile=False),
        ])
        # Six atoms: LDA src, STA d1, LDA src, STA d2, LDA src, STA d3.
        self.assertEqual(len(out), 6)
        for i in (0, 2, 4):
            self.assertEqual(out[i].src, src)
            self.assertEqual(out[i].dst, A)
        self.assertEqual(out[1].dst, d1)
        self.assertEqual(out[3].dst, d2)
        self.assertEqual(out[5].dst, d3)


if __name__ == "__main__":
    unittest.main()
