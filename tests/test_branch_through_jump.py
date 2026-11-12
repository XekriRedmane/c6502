"""Tests for `passes.branch_through_jump`: a branch or `JMP` to a label
that holds only a `JMP` goes straight to that `JMP`'s target."""
from __future__ import annotations

import unittest

import asm_ast
from passes.branch_through_jump import apply_branch_through_jump
from sim.harness import build_sim


def _fn(instrs: list[asm_ast.Type_instruction]) -> asm_ast.Function:
    return asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )


def _rewritten(instrs):
    prog = asm_ast.Program(top_level=[_fn(instrs)])
    return apply_branch_through_jump(prog).top_level[0].instructions


def _label(name: str) -> asm_ast.Label:
    return asm_ast.Label(name=name)


def _jmp(target: str) -> asm_ast.Jump:
    return asm_ast.Jump(target=target)


def _beq(target: str) -> asm_ast.Branch:
    return asm_ast.Branch(cond=asm_ast.EQ(), target=target)


_RET = asm_ast.Ret()


class TestBranch(unittest.TestCase):
    def test_branch_retargeted_and_trampoline_dropped(self) -> None:
        self.assertEqual(_rewritten([
            _label("top"), _beq("tramp"), _RET,
            _label("tramp"), _jmp("top"),
        ]), [_label("top"), _beq("top"), _RET])


class TestJump(unittest.TestCase):
    def test_jump_retargeted_and_trampoline_dropped(self) -> None:
        # The `if (0) label: return 5; goto label;` shape.
        self.assertEqual(_rewritten([
            _jmp("if_end"), _label("label"), _RET,
            _label("if_end"), _jmp("label"),
        ]), [_jmp("label"), _label("label"), _RET])

    def test_chain_followed_to_its_end(self) -> None:
        out = _rewritten([
            _jmp("a"), _label("end"), _RET,
            _label("a"), _jmp("b"),
            _label("b"), _jmp("end"),
        ])
        self.assertEqual(out[0], _jmp("end"))

    def test_cycle_left_alone(self) -> None:
        # `a: JMP b; b: JMP a` loops forever. Retargeting inside the
        # cycle would change it on every sweep.
        instrs = [
            _jmp("a"),
            _label("a"), _jmp("b"),
            _label("b"), _jmp("a"),
        ]
        self.assertEqual(_rewritten(instrs), instrs)

    def test_trampoline_kept_when_fallen_into(self) -> None:
        # The `LDA #1` falls into `mid`, so its `JMP` stays.
        a = asm_ast.Reg(reg=asm_ast.A())
        load = asm_ast.Mov(src=asm_ast.Imm(value=1), dst=a,
                           is_volatile=False)
        self.assertEqual(_rewritten([
            _jmp("mid"), _label("end"), _RET,
            load, _label("mid"), _jmp("end"),
        ]), [
            _jmp("end"), _label("end"), _RET,
            load, _label("mid"), _jmp("end"),
        ])

    def test_tail_call_not_followed(self) -> None:
        # `tick` is another function, not a label in this one.
        instrs = [
            _jmp("t"), _label("u"), _RET,
            _label("t"), _jmp("tick"),
        ]
        self.assertEqual(_rewritten(instrs), instrs)


class TestGotoBackwards(unittest.TestCase):
    _SRC = """
        int main(void) {
            if (0)
            label:
                return 5;
            goto label;
            return 0;
        }
    """

    def test_optimized_is_straight_line(self) -> None:
        from compile import _run_stage
        from preprocessor import preprocess
        asm = _run_stage("codegen", preprocess(self._SRC), optimize=True)
        body = asm[asm.index("main:"):]
        self.assertNotIn("JMP", body)
        self.assertRegex(body, r"LDX\s+#\$00\s+LDA\s+#\$05\s+RTS")

    def test_runs(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(self._SRC, optimize=optimize).run()
                self.assertEqual(result.return_int(), 5)


if __name__ == "__main__":
    unittest.main()
