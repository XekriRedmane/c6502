"""Tests for `passes.long_branches.expand_program`."""

from __future__ import annotations

import unittest

import asm_ast
from passes.long_branches import expand_program


def _short(name: str = "f") -> asm_ast.Function:
    """A function with no over-long branches — passes through
    unchanged."""
    return asm_ast.Function(
        name=name, is_global=True, params=[],
        instructions=[
            asm_ast.Branch(cond=asm_ast.EQ(), target=".end@0"),
            asm_ast.Mov(
                src=asm_ast.Imm(value=1),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
            asm_ast.Label(name=".end@0"),
            asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True),
        ],
    )


def _long_function(filler_count: int) -> asm_ast.Function:
    """Build a function with a forward `Branch(EQ, .end)` whose target
    sits past the 127-byte window because we pad with `filler_count`
    `Mov(Imm, Stack)` instructions (5 bytes each: LDA #imm + LDY # +
    STA (SSP),Y). With `filler_count=30`, the gap between the branch
    and `.end` is 5 * 30 = 150 bytes — out of range."""
    filler = [
        asm_ast.Mov(
            src=asm_ast.Imm(value=i & 0xFF),
            dst=asm_ast.Stack(offset=1),
        )
        for i in range(filler_count)
    ]
    return asm_ast.Function(
        name="f", is_global=True, params=[],
        instructions=[
            asm_ast.Branch(cond=asm_ast.EQ(), target=".end@0"),
            *filler,
            asm_ast.Label(name=".end@0"),
            asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True),
        ],
    )


class TestExpandProgram(unittest.TestCase):
    def test_short_branch_passes_through_unchanged(self) -> None:
        prog = asm_ast.Program(top_level=[_short()])
        out = expand_program(prog)
        self.assertEqual(out, prog)

    def test_static_variables_pass_through_unchanged(self) -> None:
        # Statics carry no branches; the pass should leave them
        # alone.
        prog = asm_ast.Program(top_level=[
            asm_ast.StaticVariable(
                name="g", is_global=True,
                init=[asm_ast.IntInit(value=42)],
            ),
            _short(),
        ])
        out = expand_program(prog)
        self.assertEqual(out.top_level[0], prog.top_level[0])

    def test_over_long_forward_branch_is_expanded(self) -> None:
        # 30 filler Movs at 5 bytes each = 150 bytes between the
        # branch and `.end@0`. The branch is 2 bytes, so the
        # displacement is 150 bytes — out of range.
        prog = asm_ast.Program(top_level=[_long_function(30)])
        out = expand_program(prog)
        instrs = out.top_level[0].instructions
        # The original BEQ should be gone, replaced by an inverted
        # branch (BNE), a JMP, and a fresh skip-label.
        self.assertIsInstance(instrs[0], asm_ast.Branch)
        self.assertIsInstance(instrs[0].cond, asm_ast.NE)
        self.assertEqual(instrs[0].target, ".lb_skip@0")
        self.assertIsInstance(instrs[1], asm_ast.Jump)
        self.assertEqual(instrs[1].target, ".end@0")
        self.assertIsInstance(instrs[2], asm_ast.Label)
        self.assertEqual(instrs[2].name, ".lb_skip@0")
        # Filler still 30 entries; .end label and Ret still at the
        # tail.
        self.assertEqual(len(instrs), 3 + 30 + 1 + 1)

    def test_over_long_backward_branch_is_expanded(self) -> None:
        # Backward-branch case: the target is at the start, the
        # branch at the end.
        instrs = [
            asm_ast.Label(name=".start@0"),
            *[
                asm_ast.Mov(
                    src=asm_ast.Imm(value=i & 0xFF),
                    dst=asm_ast.Stack(offset=1),
                )
                for i in range(30)
            ],
            asm_ast.Branch(cond=asm_ast.NE(), target=".start@0"),
            asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True),
        ]
        fn = asm_ast.Function(
            name="f", is_global=True, params=[], instructions=instrs,
        )
        out = expand_program(asm_ast.Program(top_level=[fn]))
        out_instrs = out.top_level[0].instructions
        # Original BNE was at index -2 (before Ret); it should have
        # been replaced by an inverted BEQ + JMP + Label triple.
        # Tail: ..., BEQ skip, JMP target, Label skip, RTS
        self.assertIsInstance(out_instrs[-4], asm_ast.Branch)
        self.assertIsInstance(out_instrs[-4].cond, asm_ast.EQ)
        self.assertIsInstance(out_instrs[-3], asm_ast.Jump)
        self.assertEqual(out_instrs[-3].target, ".start@0")
        self.assertIsInstance(out_instrs[-2], asm_ast.Label)

    def test_iteration_to_fixed_point(self) -> None:
        # Two over-long branches close together. After expanding one,
        # the second's window may shift but should still be reachable
        # by the iteration. Just check the program is well-formed and
        # has no remaining `Branch` whose target is out of range.
        from sim.assembler import instruction_size
        prog = asm_ast.Program(top_level=[_long_function(60)])
        out = expand_program(prog)
        instrs = out.top_level[0].instructions
        # Compute label positions and verify every Branch fits.
        addr = 0
        labels: dict[str, int] = {}
        for instr in instrs:
            if isinstance(instr, asm_ast.Label):
                labels[instr.name] = addr
            else:
                addr += instruction_size(instr)
        addr = 0
        for instr in instrs:
            if isinstance(instr, asm_ast.Branch):
                disp = labels[instr.target] - (addr + 2)
                self.assertGreaterEqual(
                    disp, -128, f"branch to {instr.target} disp={disp}",
                )
                self.assertLessEqual(
                    disp, 127, f"branch to {instr.target} disp={disp}",
                )
            if not isinstance(instr, asm_ast.Label):
                addr += instruction_size(instr)

    def test_unknown_target_raises(self) -> None:
        # A branch whose target isn't a Label in this function is a
        # codegen bug — the pass surfaces it loudly.
        instrs = [asm_ast.Branch(cond=asm_ast.EQ(), target=".missing@0")]
        fn = asm_ast.Function(
            name="f", is_global=True, params=[], instructions=instrs,
        )
        with self.assertRaises(ValueError):
            expand_program(asm_ast.Program(top_level=[fn]))


if __name__ == "__main__":
    unittest.main()
