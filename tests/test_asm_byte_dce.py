"""Behavioral tests for `passes.optimization_asm.byte_dce`.

Coverage:
  - Mov whose Pseudo dst is unused: dropped.
  - Mov whose Pseudo dst IS used: kept.
  - Mov to Reg(A): kept (we don't track register liveness).
  - Phi with unused dst: dropped.
  - Cascading drop: removing a Mov frees its src's def for
    removal next iteration. The fixed-point loop catches this.
  - LoadAddress is preserved even when its dst Pseudo is unused
    (today's conservative policy — see the module docstring).
"""
from __future__ import annotations

import unittest

import asm_ast
from passes.optimization_asm.byte_dce import byte_dce


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


class TestByteDce(unittest.TestCase):
    def test_drops_mov_with_unused_pseudo_dst(self) -> None:
        # Mov #$01 -> %dead ; Return — %dead is never read.
        fn = _fn(
            _mov(_imm(1), _ps("%dead")),
            _ret_bare(),
        )
        out = byte_dce(fn)
        # The Mov is dropped; only the Return remains.
        self.assertEqual(
            [type(i).__name__ for i in out.instructions],
            ["Return"],
        )

    def test_keeps_mov_with_live_pseudo_dst(self) -> None:
        # Mov #$01 -> %x ; Mov %x -> A ; Return — %x is used.
        fn = _fn(
            _mov(_imm(1), _ps("%x")),
            _mov(_ps("%x"), _A()),
            _ret_bare(),
        )
        out = byte_dce(fn)
        self.assertEqual(len(out.instructions), 3)

    def test_keeps_mov_to_register(self) -> None:
        # Mov %x -> A — dst is Reg(A), not a Pseudo, so we
        # conservatively keep it (Reg(A) flow-sensitivity is
        # opaque to this pass).
        fn = _fn(
            _mov(_ps("%x"), _A()),
            _ret_bare(),
        )
        out = byte_dce(fn)
        self.assertEqual(len(out.instructions), 2)

    def test_drops_phi_with_unused_dst(self) -> None:
        # Phi(dst=%dead, args=[(L, %src)]) followed by Return.
        # %dead is never read.
        fn = _fn(
            asm_ast.Label(name="L"),
            asm_ast.Phi(
                dst=_ps("%dead"),
                args=[asm_ast.AsmPhiArg(
                    pred_label="L", source=_ps("%src"),
                )],
            ),
            _ret_bare(),
        )
        out = byte_dce(fn)
        # Phi gone; Label and Return remain.
        kinds = [type(i).__name__ for i in out.instructions]
        self.assertNotIn("Phi", kinds)

    def test_cascading_dead_chain(self) -> None:
        # Mov #1 -> %a ; Mov %a -> %b ; Return.
        # %b is unused → drop the Mov %a -> %b.
        # Now %a is also unused → drop the Mov #1 -> %a.
        # Fixed-point loop should catch both.
        fn = _fn(
            _mov(_imm(1), _ps("%a")),
            _mov(_ps("%a"), _ps("%b")),
            _ret_bare(),
        )
        out = byte_dce(fn)
        # Both Movs dropped.
        self.assertEqual(
            [type(i).__name__ for i in out.instructions],
            ["Return"],
        )

    def test_byte_granular_dead_high_byte(self) -> None:
        # Two Movs producing bytes 0 and 1 of %y; only byte 0 is
        # consumed by a downstream Mov. Byte 1 is dead.
        fn = _fn(
            _mov(_imm(0x12), _ps("%y", 0)),
            _mov(_imm(0x34), _ps("%y", 1)),
            _mov(_ps("%y", 0), _A()),
            _ret_bare(),
        )
        out = byte_dce(fn)
        # The Mov writing byte 1 is dropped; byte 0's write and
        # read remain, plus the Return.
        movs = [i for i in out.instructions if isinstance(i, asm_ast.Mov)]
        self.assertEqual(len(movs), 2)
        # Byte 0's write to %y still present.
        write_present = any(
            isinstance(m.dst, asm_ast.Pseudo)
            and m.dst.name == "%y" and m.dst.offset == 0
            for m in movs
        )
        self.assertTrue(write_present)
        # Byte 1's write GONE.
        byte1_present = any(
            isinstance(m.dst, asm_ast.Pseudo)
            and m.dst.name == "%y" and m.dst.offset == 1
            for m in movs
        )
        self.assertFalse(byte1_present)

    def test_loadaddress_preserved_even_when_unused(self) -> None:
        # LoadAddress's dst Pseudo is unused, but we keep the
        # instruction so its src Pseudo stays in the operand-walk
        # for replace_pseudoregisters' frame allocation.
        fn = _fn(
            asm_ast.LoadAddress(
                src=_ps("%target"),
                dst=_ps("%dead"),
            ),
            _ret_bare(),
        )
        out = byte_dce(fn)
        self.assertEqual(len(out.instructions), 2)
        self.assertIsInstance(out.instructions[0], asm_ast.LoadAddress)

    def test_static_write_preserved_even_when_unread_locally(self) -> None:
        # Mov #$05 -> Pseudo("g") where "g" is a static-storage
        # name (file-scope global). The function never reads g
        # itself, but other functions might — DCE must not drop.
        fn = _fn(
            _mov(_imm(5), _ps("g")),
            _ret_bare(),
        )
        out = byte_dce(fn, statics=frozenset({"g"}))
        # Mov to g preserved.
        self.assertEqual(len(out.instructions), 2)
        self.assertIsInstance(out.instructions[0], asm_ast.Mov)


if __name__ == "__main__":
    unittest.main()
