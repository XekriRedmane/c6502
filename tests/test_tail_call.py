"""Tests for the tail-call peephole.

`apply_tail_call` rewrites `Call(name); Return(_)` to `Jump(name)`.
The pass refuses to match `Ret(...)` (frame teardown still required).

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).
"""
from __future__ import annotations

import unittest

import asm_ast
from passes.tail_call import apply_tail_call


def _fn(instrs: list[asm_ast.Type_instruction]) -> asm_ast.Function:
    return asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )


class TestTailCall(unittest.TestCase):


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
