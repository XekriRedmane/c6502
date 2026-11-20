"""Unit tests for `passes.ir_check`, the between-pass IR checks."""
from __future__ import annotations

import unittest

import asm_ast
import c99_ast
import tac_ast
from passes import ir_check
from passes.type_checking import LocalAttr, Symbol


def _var(n):
    return tac_ast.Var(name=n)


def _one(v):
    return tac_ast.Constant(const=tac_ast.ConstInt(value=v))


def _fn(*instrs, params=()):
    return tac_ast.Function(
        name="f", is_global=True, params=list(params),
        instructions=list(instrs),
    )


def _symbols(*names):
    return {n: Symbol(type=c99_ast.Int(), attrs=LocalAttr()) for n in names}


_RET = tac_ast.Ret(val=None)


class TestTacChecks(unittest.TestCase):

    def setUp(self) -> None:
        ir_check.enable()

    def tearDown(self) -> None:
        ir_check.enable(False)

    def _check(self, fn, symbols, ssa=None):
        ir_check.check_tac(fn, after="test", symbols=symbols, ssa_dsts=ssa)

    def test_good_ssa(self) -> None:
        fn = _fn(
            tac_ast.Copy(src=_one(1), dst=_var("a.1")),
            tac_ast.Binary(op=tac_ast.Add(), src1=_var("a.1"),
                           src2=_var("p"), dst=_var("b.1")),
            tac_ast.Ret(val=_var("b.1")),
            params=["p"],
        )
        self._check(fn, _symbols("a.1", "b.1", "p"), {"a.1", "b.1", "p"})

    def test_read_of_undefined_ssa_name(self) -> None:
        # The shape of the bug where a second narrowing dropped the
        # first one's Truncate: the narrow Binary reads a temporary
        # nothing defines any more.
        fn = _fn(
            tac_ast.Binary(op=tac_ast.Multiply(), src1=_var("t.lo1"),
                           src2=_one(3), dst=_var("d")),
            tac_ast.Ret(val=_var("d")),
        )
        with self.assertRaisesRegex(ir_check.IRCheckError,
                                    "after test.*t.lo1.*nothing defines"):
            self._check(fn, _symbols("t.lo1", "d"), {"t.lo1", "d"})

    def test_read_before_definition(self) -> None:
        fn = _fn(
            tac_ast.Copy(src=_var("a.1"), dst=_var("b.1")),
            tac_ast.Copy(src=_one(1), dst=_var("a.1")),
            _RET,
        )
        with self.assertRaisesRegex(ir_check.IRCheckError, "before its"):
            self._check(fn, _symbols("a.1", "b.1"), {"a.1", "b.1"})

    def test_definition_doesnt_dominate(self) -> None:
        fn = _fn(
            tac_ast.JumpIfTrue(condition=_var("p"), target="L"),
            tac_ast.Copy(src=_one(1), dst=_var("a.1")),
            tac_ast.Label(name="L"),
            tac_ast.Ret(val=_var("a.1")),
            params=["p"],
        )
        with self.assertRaisesRegex(ir_check.IRCheckError, "dominate"):
            self._check(fn, _symbols("a.1", "p"), {"a.1", "p"})

    def test_ssa_name_defined_twice(self) -> None:
        fn = _fn(
            tac_ast.Copy(src=_one(1), dst=_var("a.1")),
            tac_ast.Copy(src=_one(2), dst=_var("a.1")),
            _RET,
        )
        with self.assertRaisesRegex(ir_check.IRCheckError, "twice"):
            self._check(fn, _symbols("a.1"), {"a.1"})

    def test_unreachable_block_ignored(self) -> None:
        # SSA renaming never visits a block nothing reaches.
        fn = _fn(
            tac_ast.Ret(val=_var("p")),
            tac_ast.Copy(src=_one(1), dst=_var("p")),
            _RET,
            params=["p"],
        )
        self._check(fn, _symbols("p"), {"p"})

    def test_labels(self) -> None:
        with self.assertRaisesRegex(ir_check.IRCheckError, "missing label"):
            self._check(_fn(tac_ast.Jump(target="L"), _RET), _symbols())
        with self.assertRaisesRegex(ir_check.IRCheckError, "twice"):
            self._check(_fn(
                tac_ast.Label(name="L"), tac_ast.Label(name="L"), _RET,
            ), _symbols())

    def test_name_missing_from_symbols(self) -> None:
        fn = _fn(tac_ast.Copy(src=_one(1), dst=_var("x")), _RET)
        with self.assertRaisesRegex(ir_check.IRCheckError, "symbol table"):
            self._check(fn, _symbols())

    def test_undefined_temporary_outside_ssa(self) -> None:
        fn = _fn(tac_ast.Ret(val=_var("%3")))
        with self.assertRaisesRegex(ir_check.IRCheckError, "%3"):
            self._check(fn, _symbols("%3"))

    def _loop(self, phi_pred="B", phi_src="b.2"):
        # a.1 = 0; L: b.1 = Phi(a.1 from entry, b.2 from B); B: b.2 =
        # b.1 + 1; jump L. The block before L is labelled E.
        return _fn(
            tac_ast.Label(name="E"),
            tac_ast.Copy(src=_one(0), dst=_var("a.1")),
            tac_ast.Label(name="L"),
            tac_ast.Phi(dst=_var("b.1"), args=[
                tac_ast.PhiArg(source=_var("a.1"), pred_label="E"),
                tac_ast.PhiArg(source=_var(phi_src), pred_label=phi_pred),
            ]),
            tac_ast.JumpIfTrue(condition=_var("b.1"), target="X"),
            tac_ast.Label(name="B"),
            tac_ast.Binary(op=tac_ast.Add(), src1=_var("b.1"),
                           src2=_one(1), dst=_var("b.2")),
            tac_ast.Jump(target="L"),
            tac_ast.Label(name="X"),
            tac_ast.Ret(val=_var("b.1")),
        )

    def _loop_names(self):
        return _symbols("a.1", "b.1", "b.2"), {"a.1", "b.1", "b.2"}

    def test_good_loop(self) -> None:
        self._check(self._loop(), *self._loop_names())

    def test_phi_argument_from_missing_label(self) -> None:
        with self.assertRaisesRegex(ir_check.IRCheckError,
                                    "Phi argument from missing label Q"):
            self._check(self._loop(phi_pred="Q"), *self._loop_names())

    def test_phi_argument_not_dominated_at_its_predecessor(self) -> None:
        # b.2 is defined in B, which doesn't dominate the end of E.
        fn = self._loop()
        fn.instructions[3] = tac_ast.Phi(dst=_var("b.1"), args=[
            tac_ast.PhiArg(source=_var("b.2"), pred_label="E"),
            tac_ast.PhiArg(source=_var("b.2"), pred_label="B"),
        ])
        with self.assertRaisesRegex(ir_check.IRCheckError,
                                    "Phi reads b.2, whose definition doesn't"):
            self._check(fn, *self._loop_names())

    def test_same_control_flow_other_body(self) -> None:
        # The second function has the first one's labels and jumps, so
        # its blocks come from the first check, and its own reads are
        # still checked.
        self._check(self._loop(), *self._loop_names())
        fn = self._loop()
        fn.instructions[6] = tac_ast.Binary(
            op=tac_ast.Add(), src1=_var("c.1"), src2=_one(1),
            dst=_var("b.2"),
        )
        symbols, ssa = self._loop_names()
        symbols.update(_symbols("c.1"))
        with self.assertRaisesRegex(ir_check.IRCheckError,
                                    "Binary reads c.1, which nothing"):
            self._check(fn, symbols, ssa | {"c.1"})

    def test_jump_changed_in_place(self) -> None:
        # A pass may change an instruction in place, so the blocks of
        # the first check mustn't be used for the second. Once the jump
        # goes to L, a.1's definition no longer dominates L.
        fn = _fn(
            tac_ast.Label(name="E"),
            tac_ast.JumpIfTrue(condition=_var("p"), target="M"),
            tac_ast.Copy(src=_one(1), dst=_var("a.1")),
            tac_ast.Label(name="L"),
            tac_ast.Ret(val=_var("a.1")),
            tac_ast.Label(name="M"),
            tac_ast.Ret(val=_var("p")),
            params=["p"],
        )
        names = _symbols("a.1", "p"), {"a.1", "p"}
        self._check(fn, *names)
        fn.instructions[1].target = "L"
        with self.assertRaisesRegex(ir_check.IRCheckError,
                                    "Ret reads a.1, whose definition"):
            self._check(fn, *names)

    def test_every_problem_is_reported(self) -> None:
        fn = _fn(
            tac_ast.Copy(src=_var("%1"), dst=_var("x")),
            tac_ast.Copy(src=_var("%2"), dst=_var("y")),
            tac_ast.Ret(val=_var("%2")),
        )
        with self.assertRaisesRegex(
                ir_check.IRCheckError,
                "Copy reads %1, which nothing defines; Copy reads %2, "
                "which nothing defines; Ret reads %2, which nothing "
                "defines$"):
            self._check(fn, _symbols("%1", "%2", "x", "y"))
        with self.assertRaisesRegex(
                ir_check.IRCheckError,
                ": %2 isn't in the symbol table; x isn't in the symbol "
                "table$"):
            self._check(fn, _symbols("%1", "y"))

    def test_temporary_in_memory_needs_no_definition(self) -> None:
        fn = _fn(
            tac_ast.GetAddress(operand=_var("%1"), dst=_var("p")),
            tac_ast.Ret(val=_var("%1")),
        )
        self._check(fn, _symbols("%1", "p"))

    def test_off_by_default(self) -> None:
        ir_check.enable(False)
        self._check(_fn(tac_ast.Jump(target="L")), _symbols())


class TestAsmChecks(unittest.TestCase):

    def setUp(self) -> None:
        ir_check.enable()

    def tearDown(self) -> None:
        ir_check.enable(False)

    def _prog(self, *instrs):
        return asm_ast.Program(top_level=[asm_ast.Function(
            name="f", is_global=True, params=[], instructions=list(instrs),
        )])

    def test_pseudo_left(self) -> None:
        prog = self._prog(asm_ast.Mov(
            src=asm_ast.Pseudo(name="x", offset=0),
            dst=asm_ast.Reg(reg=asm_ast.A()), is_volatile=False,
        ))
        ir_check.check_asm(prog, after="tac_to_asm", pseudos=True)
        with self.assertRaisesRegex(ir_check.IRCheckError, "Pseudo"):
            ir_check.check_asm(prog, after="test")

    def test_branch_to_missing_label(self) -> None:
        prog = self._prog(asm_ast.Branch(cond=asm_ast.EQ(), target=".x"))
        with self.assertRaisesRegex(ir_check.IRCheckError, "missing label"):
            ir_check.check_asm(prog, after="test")

    def test_tail_call_jump_is_fine(self) -> None:
        prog = self._prog(asm_ast.Jump(target="g", reg_args=[]))
        ir_check.check_asm(prog, after="test")


if __name__ == "__main__":
    unittest.main()
