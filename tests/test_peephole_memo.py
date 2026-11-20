"""Tests for the record of the functions each peephole pass left
unchanged (passes/peephole_memo.py), and for the fixed point's use of
it in `compile._peephole_fixedpoint`."""
from __future__ import annotations

import unittest
from unittest.mock import patch

import asm_ast
import compile as compile_module
from compile import _peephole_fixedpoint
from fuzz import stmts
from passes import dec_peephole
from passes.peephole_memo import FunctionMemo, PeepholeMemo, same_function
from preprocessor import preprocess

_REG_A = asm_ast.Reg(reg=asm_ast.A())


def _fn(instrs, name="f") -> asm_ast.Function:
    return asm_ast.Function(
        name=name, is_global=True, params=[], instructions=instrs,
    )


class TestSameFunction(unittest.TestCase):
    def test_same_objects_in_a_new_list(self) -> None:
        ins = [asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
               asm_ast.Return(save_a=True)]
        self.assertTrue(same_function(_fn(ins), _fn(list(ins))))

    def test_equal_copies_are_not_the_same(self) -> None:
        # `memory_value_propagation` keys facts on `id(instr)`, so an
        # equal copy is a different input.
        a = _fn([asm_ast.Return(save_a=False)])
        b = _fn([asm_ast.Return(save_a=False)])
        self.assertEqual(a, b)
        self.assertFalse(same_function(a, b))

    def test_other_name(self) -> None:
        ins = [asm_ast.Return(save_a=False)]
        self.assertFalse(same_function(_fn(ins), _fn(ins, name="g")))


class TestFunctionMemo(unittest.TestCase):
    def test_records_only_an_unchanged_result(self) -> None:
        memo = FunctionMemo()
        fn = _fn([asm_ast.Return(save_a=False)])
        changed = _fn([asm_ast.Return(save_a=True)])
        self.assertIs(memo.record(fn, changed, None), changed)
        self.assertFalse(memo.unchanged(fn, None))
        self.assertIs(memo.record(fn, _fn(list(fn.instructions)), 1), fn)
        self.assertTrue(memo.unchanged(fn, 1))
        # A different context runs the pass again.
        self.assertFalse(memo.unchanged(fn, 2))

    def test_other_arguments_drop_the_records(self) -> None:
        memo = PeepholeMemo().for_arguments(({}, {}, frozenset()))
        fn = _fn([asm_ast.Return(save_a=False)])
        memo.of("p").record(fn, fn)
        self.assertTrue(memo.for_arguments(({}, {}, frozenset()))
                        .of("p").unchanged(fn))
        self.assertFalse(memo.for_arguments(({"s": 0x80}, {}, frozenset()))
                         .of("p").unchanged(fn))


class TestFixedPointMemo(unittest.TestCase):
    def test_converged_program_isnt_rewritten_again(self) -> None:
        # A second fixed point on the first one's result, with the same
        # memo, runs no function-scope rewrite.
        prog = asm_ast.Program(top_level=[_fn([
            asm_ast.Mov(src=asm_ast.ZP(address=0x90, offset=0), dst=_REG_A),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.ZP(address=0x90, offset=0)),
            asm_ast.Return(save_a=False),
        ])])
        memo = PeepholeMemo()
        out = _peephole_fixedpoint(prog, memo=memo)
        calls = []
        original = dec_peephole._rewrite_function

        def counting(fn):
            calls.append(fn.name)
            return original(fn)

        with patch.object(dec_peephole, "_rewrite_function", counting):
            again = _peephole_fixedpoint(out, memo=memo)
            self.assertEqual(calls, [])
            _peephole_fixedpoint(out)
            self.assertEqual(calls, ["f"])
        self.assertEqual(again, out)

    def test_results_match_runs_without_the_memo(self) -> None:
        # With the check on, every pass's result is compared with a run
        # on the whole program without the memo, which raises on a
        # difference.
        with patch.object(compile_module, "_CHECK_PEEPHOLE_MEMO", True):
            for seed in (3, 11):
                src, _ = stmts.make_program(seed)
                for optimize in (False, True):
                    compile_module._run_stage(
                        "codegen", preprocess(src, [], None),
                        optimize=optimize,
                    )


if __name__ == "__main__":
    unittest.main()
