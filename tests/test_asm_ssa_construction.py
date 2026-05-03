"""Behavioral tests for `passes.optimization_asm.ssa_construction.to_ssa`.

Coverage:
  - Single straight-line block: each Pseudo def gets a fresh
    versioned name, uses are rewritten to their current version.
  - Diamond CFG: a Phi appears at the merge block for each promotable
    (name, offset) defined in the diamond.
  - Multi-byte Pseudo: byte 0 and byte 1 of the same name are
    versioned INDEPENDENTLY.
  - Address-taken name: not versioned (`LoadAddress.src` excludes
    the name from the promotable set).
  - Inc/Dec operand: not versioned (read-modify-write target is
    excluded defensively).
  - Loop: a Phi appears at the loop header for each promotable
    (name, offset) modified in the body.
"""
from __future__ import annotations

import unittest

import asm_ast
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


def _label(n: str) -> asm_ast.Label:
    return asm_ast.Label(name=n)


def _jump(t: str) -> asm_ast.Jump:
    return asm_ast.Jump(target=t)


def _branch(t: str) -> asm_ast.Branch:
    return asm_ast.Branch(cond=asm_ast.EQ(), target=t)


def _fn(*instrs, name: str = "main", params=()) -> asm_ast.Function:
    return asm_ast.Function(
        name=name,
        is_global=True,
        params=list(params),
        instructions=list(instrs),
    )


class TestAsmToSsaSingleBlock(unittest.TestCase):
    def test_single_def_renames_to_v1(self) -> None:
        # Mov #$01 -> %x ; Mov %x -> A ; Return
        # %x at offset 0 has one def → renamed once to %x.b0.v1.
        fn = _fn(
            _mov(_imm(1), _ps("%x")),
            _mov(_ps("%x"), _A()),
            _ret_bare(),
        )
        out = to_ssa(fn)
        # Find the renamed Mov (def-site).
        defs = [
            i for i in out.instructions
            if isinstance(i, asm_ast.Mov) and isinstance(i.dst, asm_ast.Pseudo)
        ]
        self.assertEqual(len(defs), 1)
        self.assertEqual(defs[0].dst.name, "%x.b0.v1")
        self.assertEqual(defs[0].dst.offset, 0)
        # The use should refer to the same versioned name.
        uses = [
            i for i in out.instructions
            if isinstance(i, asm_ast.Mov) and isinstance(i.src, asm_ast.Pseudo)
        ]
        self.assertEqual(len(uses), 1)
        self.assertEqual(uses[0].src.name, "%x.b0.v1")

    def test_two_defs_get_distinct_versions(self) -> None:
        fn = _fn(
            _mov(_imm(1), _ps("%x")),
            _mov(_ps("%x"), _A()),
            _mov(_imm(2), _ps("%x")),
            _mov(_ps("%x"), _A()),
            _ret_bare(),
        )
        out = to_ssa(fn)
        defs = [
            i.dst.name for i in out.instructions
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, asm_ast.Pseudo)
        ]
        self.assertEqual(defs, ["%x.b0.v1", "%x.b0.v2"])

    def test_multi_byte_versions_independently(self) -> None:
        # Two bytes of %y are written; byte 0 and byte 1 each get
        # their own version.
        fn = _fn(
            _mov(_imm(0x12), _ps("%y", 0)),
            _mov(_imm(0x34), _ps("%y", 1)),
            _ret_bare(),
        )
        out = to_ssa(fn)
        defs = [
            i.dst.name for i in out.instructions
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, asm_ast.Pseudo)
        ]
        # Each byte gets v1 of its own slot.
        self.assertIn("%y.b0.v1", defs)
        self.assertIn("%y.b1.v1", defs)


class TestAsmToSsaDiamond(unittest.TestCase):
    def test_diamond_inserts_phi_at_merge(self) -> None:
        # B0: Mov 1 -> %x ; Branch L1
        # B1 (fall-through): Mov 2 -> %x ; Jump L3
        # B2 (L1):           Mov 3 -> %x ; Jump L3
        # B3 (L3): Mov %x -> A ; Return
        fn = _fn(
            _mov(_imm(1), _ps("%x")),
            _branch("L1"),
            _mov(_imm(2), _ps("%x")),
            _jump("L3"),
            _label("L1"),
            _mov(_imm(3), _ps("%x")),
            _jump("L3"),
            _label("L3"),
            _mov(_ps("%x"), _A()),
            _ret_bare(),
        )
        out = to_ssa(fn)
        phis = [i for i in out.instructions if isinstance(i, asm_ast.Phi)]
        self.assertEqual(len(phis), 1)
        phi = phis[0]
        # Phi has two args (one per merge predecessor).
        self.assertEqual(len(phi.args), 2)
        # Phi's dst is a fresh version; sources are the predecessor
        # versions of %x.b0.
        self.assertIsInstance(phi.dst, asm_ast.Pseudo)
        self.assertTrue(phi.dst.name.startswith("%x.b0.v"))
        for arg in phi.args:
            self.assertIsInstance(arg.source, asm_ast.Pseudo)
            self.assertTrue(arg.source.name.startswith("%x.b0.v"))


class TestAsmToSsaExclusions(unittest.TestCase):
    def test_address_taken_name_not_renamed(self) -> None:
        # %p has its address taken via LoadAddress; reads/writes of
        # %p stay at the original name and offset.
        fn = _fn(
            _mov(_imm(0xAA), _ps("%p", 0)),
            asm_ast.LoadAddress(src=_ps("%p"), dst=_ps("%addr")),
            _mov(_ps("%p", 0), _A()),
            _ret_bare(),
        )
        out = to_ssa(fn)
        # No def of %p has been versioned.
        for i in out.instructions:
            if isinstance(i, asm_ast.Mov):
                if isinstance(i.dst, asm_ast.Pseudo) and i.dst.name == "%p":
                    self.assertEqual(i.dst.offset, 0)
                if isinstance(i.src, asm_ast.Pseudo) and i.src.name == "%p":
                    self.assertEqual(i.src.offset, 0)
        # %addr (LoadAddress.dst) IS promotable (not address-taken
        # itself), so it gets versioned.
        defs = [
            i for i in out.instructions
            if isinstance(i, asm_ast.LoadAddress)
        ]
        self.assertEqual(len(defs), 1)
        self.assertIsInstance(defs[0].dst, asm_ast.Pseudo)
        self.assertTrue(defs[0].dst.name.startswith("%addr.b0.v"))

    def test_inc_target_not_renamed(self) -> None:
        # Inc on %x — defensive: not promoted even though Inc isn't
        # emitted by tac_to_asm today.
        fn = _fn(
            _mov(_imm(0), _ps("%x")),
            asm_ast.Inc(dst=_ps("%x")),
            _ret_bare(),
        )
        out = to_ssa(fn)
        # Mov's dst still refers to the original name.
        movs = [i for i in out.instructions if isinstance(i, asm_ast.Mov)]
        self.assertEqual(movs[0].dst.name, "%x")


class TestAsmToSsaLoop(unittest.TestCase):
    def test_loop_header_gets_phi(self) -> None:
        # Loop body reads %i (so it's live-in across the back-edge)
        # and rewrites it. Phi appears at the loop header.
        # Pre-header writes %i; loop reads it into A then writes a
        # new value back; loop branches back to itself; on exit we
        # read %i once more.
        fn = _fn(
            _mov(_imm(0), _ps("%i")),
            _label("L"),
            _mov(_ps("%i"), _A()),       # use of %i — keeps it live-in
            _mov(_imm(1), _ps("%i")),    # def of %i
            _branch("L"),
            _mov(_ps("%i"), _A()),       # post-loop use, also keeps live
            _ret_bare(),
        )
        out = to_ssa(fn)
        phis = [i for i in out.instructions if isinstance(i, asm_ast.Phi)]
        self.assertEqual(len(phis), 1)
        phi = phis[0]
        # Two predecessors: the entry (preheader) and the back-edge.
        self.assertEqual(len(phi.args), 2)


if __name__ == "__main__":
    unittest.main()
