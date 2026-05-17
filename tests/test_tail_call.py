"""Tests for the tail-call peephole.

`apply_tail_call` rewrites `Call(name); Return(_)` to `Jump(name)`.
The pass refuses to match `Ret(...)` (frame teardown still required).
"""
from __future__ import annotations

import unittest

import asm_ast
from passes.tail_call import apply_tail_call


_REG_A = asm_ast.Reg(reg=asm_ast.A())


def _fn(instrs: list[asm_ast.Type_instruction]) -> asm_ast.Function:
    return asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )


def _prog(instrs: list[asm_ast.Type_instruction]) -> asm_ast.Program:
    return asm_ast.Program(top_level=[_fn(instrs)])


def _rewrite(instrs):
    return apply_tail_call(_prog(instrs)).top_level[0].instructions


class TestTailCall(unittest.TestCase):
    def test_call_then_return_becomes_jump(self) -> None:
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Call(name="apply_bobble"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewrite(instrs)
        self.assertEqual(
            out,
            [
                asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
                asm_ast.Jump(target="apply_bobble"),
            ],
        )

    def test_call_then_return_save_a_true_also_folds(self) -> None:
        # `save_a` is a tag from earlier passes that's discarded at
        # the bare-RTS lowering anyway; tail-call is sound either way.
        instrs = [
            asm_ast.Call(name="helper"),
            asm_ast.Return(save_a=True),
        ]
        out = _rewrite(instrs)
        self.assertEqual(out, [asm_ast.Jump(target="helper")])

    def test_call_then_ret_does_not_fold(self) -> None:
        # `Ret` carries a non-trivial frame teardown sequence that
        # must run before control leaves the function.
        instrs = [
            asm_ast.Call(name="helper"),
            asm_ast.Ret(
                arg_bytes=2, local_bytes=4, save_a=False,
                callee_saved_addrs=[],
            ),
        ]
        out = _rewrite(instrs)
        self.assertEqual(out, instrs)

    def test_call_followed_by_other_instr_does_not_fold(self) -> None:
        # Only the immediate Call->Return pattern matches. Anything
        # else between the Call and the Return blocks the fold.
        instrs = [
            asm_ast.Call(name="helper"),
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        out = _rewrite(instrs)
        self.assertEqual(out, instrs)

    def test_return_alone_does_not_fold(self) -> None:
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        out = _rewrite(instrs)
        self.assertEqual(out, instrs)

    def test_multiple_call_return_pairs_all_fold(self) -> None:
        # If a function has multiple Call+Return pairs (e.g. after
        # branch_invert collapses an if/else into early exits), each
        # one folds independently.
        instrs = [
            asm_ast.Branch(
                cond=asm_ast.EQ(), target=".else_path",
            ),
            asm_ast.Call(name="path_a"),
            asm_ast.Return(save_a=False),
            asm_ast.Label(name=".else_path"),
            asm_ast.Call(name="path_b"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewrite(instrs)
        self.assertEqual(
            out,
            [
                asm_ast.Branch(
                    cond=asm_ast.EQ(), target=".else_path",
                ),
                asm_ast.Jump(target="path_a"),
                asm_ast.Label(name=".else_path"),
                asm_ast.Jump(target="path_b"),
            ],
        )

    def test_other_top_level_passes_through(self) -> None:
        # StaticVariable / etc. (anything that's not a Function) is
        # left alone — only Function bodies are walked.
        sv = asm_ast.StaticVariable(
            name="g", is_global=True,
            init=asm_ast.IntInit(value=0),
        )
        fn = _fn([
            asm_ast.Call(name="h"),
            asm_ast.Return(save_a=False),
        ])
        prog = asm_ast.Program(top_level=[sv, fn])
        out = apply_tail_call(prog)
        self.assertEqual(out.top_level[0], sv)
        self.assertEqual(
            out.top_level[1].instructions,
            [asm_ast.Jump(target="h")],
        )


if __name__ == "__main__":
    unittest.main()
