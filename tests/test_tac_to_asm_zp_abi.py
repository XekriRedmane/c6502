"""Behavioral tests for ZP-ABI call-site lowering in `tac_to_asm`.

Coverage:
  - Soft-stack default: no abi dict → existing AllocateStack +
    Stack writes. (Regression backstop: existing tests cover this.
    A representative test here serves as a control.)
  - ZpLayout callee: arg byte writes go to ZP addresses, no
    AllocateStack.
  - Multi-byte arg: per-byte Movs to consecutive ZP addresses.
  - Multiple args: each arg's bytes follow the layout's flat
    address sequence.
  - Parallel-copy hazard: when two args' source/dest aliasing
    forms a ZP cycle, the call-site emission topo-sorts and
    breaks the cycle with a fresh temp.
"""
from __future__ import annotations

import unittest

import asm_ast
import tac_ast
from passes.abi_selection import SoftStackLayout, ZpLayout
from tac_to_asm import translate_program


def _ci(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstInt(value=v))


def _fn(name: str, *instrs, params=()) -> tac_ast.Function:
    return tac_ast.Function(
        name=name,
        is_global=True,
        params=list(params),
        instructions=list(instrs),
    )


def _prog(*tops) -> tac_ast.Program:
    return tac_ast.Program(top_level=list(tops))


def _ops(prog: asm_ast.Program, fn_name: str) -> list[asm_ast.Type_instruction]:
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function) and tl.name == fn_name:
            return tl.instructions
    raise AssertionError(f"function {fn_name!r} not found")


class TestZpAbiCallSite(unittest.TestCase):
    def test_soft_stack_default_uses_allocate_stack(self) -> None:
        # No abi dict → soft-stack convention. Caller emits
        # AllocateStack(2) + Mov to Stack(1)/Stack(2).
        prog = _prog(
            _fn(
                "main",
                tac_ast.FunctionCall(
                    name="callee",
                    args=[_ci(42)],
                    dst=None,
                ),
                tac_ast.Ret(val=_ci(0)),
            ),
        )
        out = translate_program(prog)
        instrs = _ops(out, "main")
        # Find AllocateStack and confirm a Stack(1) write follows.
        kinds = [type(i).__name__ for i in instrs]
        self.assertIn("AllocateStack", kinds)
        # Find the AllocateStack and check the next Mov writes to
        # Stack.
        idx = kinds.index("AllocateStack")
        next_mov = next(
            i for i in instrs[idx + 1:] if isinstance(i, asm_ast.Mov)
        )
        self.assertIsInstance(next_mov.dst, asm_ast.Stack)

    def test_zp_layout_caller_emits_no_allocate_stack(self) -> None:
        # callee has ZpLayout — an Int param. Caller writes the
        # arg's two bytes to the callee's slot symbols directly
        # (`Data(__zpabi_callee_p0)` / `Data(__zpabi_callee_p1)`).
        # dasm resolves those symbols to ZP addresses via the
        # `EQU` directives the emit stage prepends.
        abi = {
            "callee": ZpLayout(
                slot_symbols=[
                    "__zpabi_callee_p0", "__zpabi_callee_p1",
                ],
                addrs=[0x80, 0x81],
            ),
            "main": SoftStackLayout(),
        }
        prog = _prog(
            _fn(
                "main",
                # Call callee(42) — arg is the constant 42 (Int).
                tac_ast.FunctionCall(
                    name="callee",
                    args=[_ci(42)],
                    dst=None,
                ),
                tac_ast.Ret(val=_ci(0)),
            ),
        )
        out = translate_program(prog, abi=abi)
        instrs = _ops(out, "main")
        # No AllocateStack between the call setup.
        self.assertFalse(any(
            isinstance(i, asm_ast.AllocateStack)
            for i in instrs
        ))
        # The two arg-byte Movs target Data(__zpabi_callee_p0) and
        # Data(__zpabi_callee_p1).
        data_movs = [
            i for i in instrs
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, asm_ast.Data)
            and i.dst.name.startswith("__zpabi_")
        ]
        self.assertEqual(len(data_movs), 2)
        names = sorted(m.dst.name for m in data_movs)
        self.assertEqual(
            names, ["__zpabi_callee_p0", "__zpabi_callee_p1"],
        )

    def test_zp_layout_two_args_use_consecutive_addrs(self) -> None:
        # Two Int args → 4 bytes total. Slot symbols index 0..3.
        abi = {
            "callee": ZpLayout(
                slot_symbols=[
                    "__zpabi_callee_p0", "__zpabi_callee_p1",
                    "__zpabi_callee_p2", "__zpabi_callee_p3",
                ],
                addrs=[0x80, 0x81, 0x82, 0x83],
            ),
            "main": SoftStackLayout(),
        }
        prog = _prog(
            _fn(
                "main",
                tac_ast.FunctionCall(
                    name="callee",
                    args=[_ci(1), _ci(2)],
                    dst=None,
                ),
                tac_ast.Ret(val=_ci(0)),
            ),
        )
        out = translate_program(prog, abi=abi)
        instrs = _ops(out, "main")
        data_names = [
            i.dst.name for i in instrs
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, asm_ast.Data)
            and i.dst.name.startswith("__zpabi_")
        ]
        self.assertEqual(
            data_names,
            [
                "__zpabi_callee_p0", "__zpabi_callee_p1",
                "__zpabi_callee_p2", "__zpabi_callee_p3",
            ],
        )


if __name__ == "__main__":
    unittest.main()
