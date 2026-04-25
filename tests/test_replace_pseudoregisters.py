import unittest

import asm_ast
from passes.replace_pseudoregisters import (
    Replacer,
    replace_function,
    replace_program,
)


def _reg_a():
    return asm_ast.Reg(reg=asm_ast.A())


def _fn(*instrs, name="main", params=()):
    return asm_ast.Function(
        name=name, params=list(params), instructions=list(instrs),
    )


class TestLocalLayout(unittest.TestCase):
    """Locals (Pseudos whose name isn't in the function's `params`)
    are assigned Frame offsets 1..M in source-encounter order. Same
    name reused across instructions reuses the same offset."""

    def test_distinct_pseudos_get_sequential_offsets(self):
        out = replace_function(_fn(
            asm_ast.Mov(src=asm_ast.Imm(value=1),
                        dst=asm_ast.Pseudo(name="a")),
            asm_ast.Mov(src=asm_ast.Imm(value=2),
                        dst=asm_ast.Pseudo(name="b")),
            asm_ast.Mov(src=asm_ast.Pseudo(name="a"), dst=_reg_a()),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ))
        # `a` was first → Frame(1); `b` second → Frame(2). Two
        # locals → M=2, so the prologue and Ret carry local_bytes=2.
        self.assertEqual(out, _fn(
            asm_ast.FunctionPrologue(arg_bytes=0, local_bytes=2),
            asm_ast.Mov(src=asm_ast.Imm(value=1),
                        dst=asm_ast.Frame(offset=1)),
            asm_ast.Mov(src=asm_ast.Imm(value=2),
                        dst=asm_ast.Frame(offset=2)),
            asm_ast.Mov(src=asm_ast.Frame(offset=1), dst=_reg_a()),
            asm_ast.Ret(arg_bytes=0, local_bytes=2),
        ))

    def test_same_pseudo_on_both_sides_uses_one_offset(self):
        out = replace_function(_fn(
            asm_ast.Mov(src=asm_ast.Pseudo(name="x"),
                        dst=asm_ast.Pseudo(name="x")),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ))
        # One local, M=1; both sides resolve to Frame(1).
        self.assertEqual(out.instructions[1], asm_ast.Mov(
            src=asm_ast.Frame(offset=1),
            dst=asm_ast.Frame(offset=1),
        ))

    def test_imm_src_only_pseudo_dst(self):
        out = replace_function(_fn(
            asm_ast.Mov(src=asm_ast.Imm(value=7),
                        dst=asm_ast.Pseudo(name="t")),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ))
        self.assertEqual(out.instructions[1], asm_ast.Mov(
            src=asm_ast.Imm(value=7),
            dst=asm_ast.Frame(offset=1),
        ))

    def test_arith_instructions_rewrite_pseudo_operands(self):
        # All operand-bearing ops (Add/Sub/And/Or/Xor) must rewrite
        # Pseudo operands to Frame slots. Each case is its own
        # function so the local counter restarts and the assertion
        # is local to the case.
        op_cases = [
            (
                asm_ast.Add(src=asm_ast.Pseudo(name="t"),
                            dst=_reg_a()),
                asm_ast.Add(src=asm_ast.Frame(offset=1),
                            dst=_reg_a()),
            ),
            (
                asm_ast.Sub(src=asm_ast.Pseudo(name="t"),
                            dst=_reg_a()),
                asm_ast.Sub(src=asm_ast.Frame(offset=1),
                            dst=_reg_a()),
            ),
            (
                asm_ast.And(src=asm_ast.Pseudo(name="t"),
                            dst=_reg_a()),
                asm_ast.And(src=asm_ast.Frame(offset=1),
                            dst=_reg_a()),
            ),
            (
                asm_ast.Or(src=asm_ast.Pseudo(name="t"),
                           dst=_reg_a()),
                asm_ast.Or(src=asm_ast.Frame(offset=1),
                           dst=_reg_a()),
            ),
            (
                asm_ast.Xor(src1=_reg_a(),
                            src2=asm_ast.Pseudo(name="t"),
                            dst=_reg_a()),
                asm_ast.Xor(src1=_reg_a(),
                            src2=asm_ast.Frame(offset=1),
                            dst=_reg_a()),
            ),
        ]
        for src_instr, expected_instr in op_cases:
            with self.subTest(src=src_instr):
                out = replace_function(_fn(
                    src_instr, asm_ast.Ret(arg_bytes=0, local_bytes=0),
                ))
                # Body starts at index 1 (prologue is index 0).
                self.assertEqual(out.instructions[1], expected_instr)


class TestNoPseudosNoLocals(unittest.TestCase):
    """Functions that don't use any Pseudos still pick up the
    prologue and the patched Ret, but with arg_bytes=0 and
    local_bytes=0 — the emitter takes that as a special case and
    elides the prologue boilerplate / collapses the epilogue to
    `RTS`."""

    def test_function_with_no_pseudos(self):
        out = replace_function(_fn(
            asm_ast.Mov(src=asm_ast.Imm(value=42), dst=_reg_a()),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ))
        self.assertEqual(out, _fn(
            asm_ast.FunctionPrologue(arg_bytes=0, local_bytes=0),
            asm_ast.Mov(src=asm_ast.Imm(value=42), dst=_reg_a()),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ))

    def test_empty_function(self):
        # No instructions at all → just the (empty-emitting) prologue
        # gets prepended; nothing else.
        out = replace_function(_fn())
        self.assertEqual(out, _fn(
            asm_ast.FunctionPrologue(arg_bytes=0, local_bytes=0),
        ))


class TestParamLayout(unittest.TestCase):
    """Parameters get Frame offsets at the top of the frame:
    `M+3 .. M+2+N`. The 2-byte gap between locals and params (at
    M+1, M+2) holds the saved caller FP. Param j (1-indexed) lands
    at offset M+2+j."""

    def test_one_param_no_locals_lands_at_offset_3(self):
        # M=0, N=1 → param `a` at offset 0+2+1 = 3.
        out = replace_function(_fn(
            asm_ast.Mov(src=asm_ast.Pseudo(name="a"), dst=_reg_a()),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
            params=("a",),
        ))
        self.assertEqual(out.instructions[1], asm_ast.Mov(
            src=asm_ast.Frame(offset=3), dst=_reg_a(),
        ))
        # Prologue and Ret both report N=1, M=0.
        self.assertEqual(
            out.instructions[0],
            asm_ast.FunctionPrologue(arg_bytes=1, local_bytes=0),
        )
        self.assertEqual(
            out.instructions[-1],
            asm_ast.Ret(arg_bytes=1, local_bytes=0),
        )

    def test_two_params_two_locals(self):
        # Source-order: encounter `t1` (local 1), `a` (param), `t2`
        # (local 2), `b` (param). M=2, N=2.
        # Locals: t1→1, t2→2.
        # Params: a→2+2+1=5, b→2+2+2=6.
        out = replace_function(_fn(
            asm_ast.Mov(src=asm_ast.Imm(value=1),
                        dst=asm_ast.Pseudo(name="t1")),
            asm_ast.Mov(src=asm_ast.Pseudo(name="a"), dst=_reg_a()),
            asm_ast.Mov(src=asm_ast.Imm(value=2),
                        dst=asm_ast.Pseudo(name="t2")),
            asm_ast.Mov(src=asm_ast.Pseudo(name="b"), dst=_reg_a()),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
            params=("a", "b"),
        ))
        # Indices: 0=prologue, 1..4=body, 5=Ret.
        self.assertEqual(
            out.instructions[0],
            asm_ast.FunctionPrologue(arg_bytes=2, local_bytes=2),
        )
        self.assertEqual(
            out.instructions[1].dst, asm_ast.Frame(offset=1),
        )
        self.assertEqual(
            out.instructions[2].src, asm_ast.Frame(offset=5),
        )
        self.assertEqual(
            out.instructions[3].dst, asm_ast.Frame(offset=2),
        )
        self.assertEqual(
            out.instructions[4].src, asm_ast.Frame(offset=6),
        )
        self.assertEqual(
            out.instructions[-1],
            asm_ast.Ret(arg_bytes=2, local_bytes=2),
        )

    def test_param_order_independent_of_use_order(self):
        # `b` is referenced before `a` in the body. Param offsets
        # are determined by the function's `params` list (declaration
        # order), not the encounter order — `a` must still get the
        # smaller offset.
        out = replace_function(_fn(
            asm_ast.Mov(src=asm_ast.Pseudo(name="b"), dst=_reg_a()),
            asm_ast.Mov(src=asm_ast.Pseudo(name="a"), dst=_reg_a()),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
            params=("a", "b"),
        ))
        # M=0, N=2: a→3, b→4.
        self.assertEqual(out.instructions[1].src, asm_ast.Frame(offset=4))
        self.assertEqual(out.instructions[2].src, asm_ast.Frame(offset=3))

    def test_unused_param_does_not_appear_in_body(self):
        # `b` is declared as a param but never referenced. Its
        # offset entry exists in the param map but no asm
        # instruction shows Frame(M+2+2). The function's arg_bytes
        # in prologue/Ret still reflects len(params).
        out = replace_function(_fn(
            asm_ast.Mov(src=asm_ast.Pseudo(name="a"), dst=_reg_a()),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
            params=("a", "b"),
        ))
        self.assertEqual(
            out.instructions[0],
            asm_ast.FunctionPrologue(arg_bytes=2, local_bytes=0),
        )
        self.assertEqual(
            out.instructions[-1],
            asm_ast.Ret(arg_bytes=2, local_bytes=0),
        )

    def test_param_used_as_dst(self):
        # `int foo(int a) { a = 5; ... }` — assigning to a
        # parameter writes Frame(M+3) just like any other Frame
        # store.
        out = replace_function(_fn(
            asm_ast.Mov(src=asm_ast.Imm(value=5),
                        dst=asm_ast.Pseudo(name="a")),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
            params=("a",),
        ))
        self.assertEqual(out.instructions[1], asm_ast.Mov(
            src=asm_ast.Imm(value=5), dst=asm_ast.Frame(offset=3),
        ))


class TestPassThrough(unittest.TestCase):
    """Operands other than Pseudo and instructions without Pseudo-
    typed fields pass through unchanged. Ret is the exception — it
    gets patched with the function's dims."""

    def test_non_pseudo_operands_pass_through(self):
        # Imm/Reg/Stack/Frame all stay as-is. Use Mov(Imm, Reg(A))
        # and Mov(Frame, Reg(A)) as representatives.
        out = replace_function(_fn(
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_reg_a()),
            asm_ast.Mov(src=asm_ast.Frame(offset=7), dst=_reg_a()),
            asm_ast.Mov(src=asm_ast.Stack(offset=3), dst=_reg_a()),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ))
        # Body (indices 1..3) untouched.
        self.assertEqual(
            out.instructions[1].src, asm_ast.Imm(value=1),
        )
        self.assertEqual(
            out.instructions[2].src, asm_ast.Frame(offset=7),
        )
        self.assertEqual(
            out.instructions[3].src, asm_ast.Stack(offset=3),
        )

    def test_call_jump_branch_label_pass_through(self):
        # No operand-typed fields → no replacement, but they keep
        # their place in the instruction stream.
        instrs = [
            asm_ast.Call(name="mul8"),
            asm_ast.Jump(target="end"),
            asm_ast.Branch(cond=asm_ast.EQ(), target="end"),
            asm_ast.Label(name="end"),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ]
        out = replace_function(_fn(*instrs))
        # All four control-flow instructions pass through verbatim;
        # Ret is at the end and gets patched (here with 0/0, which
        # equals what was there).
        self.assertEqual(out.instructions[1:], instrs)


class TestUnknownPseudo(unittest.TestCase):
    """A Pseudo whose name isn't in the function's params list and
    doesn't appear during the local-discovery walk would mean we
    missed an operand-bearing instruction in `_operands_in`.
    Bisection-friendly: raises immediately rather than emitting a
    Pseudo-bearing asm node that the emitter would later reject."""

    def test_unknown_pseudo_raises(self):
        # We exercise the branch by feeding a Replacer directly
        # (skipping the discover walk that would have found "x").
        r = Replacer(params=[])
        with self.assertRaises(ValueError) as ctx:
            r.replace(asm_ast.Pseudo(name="x"))
        self.assertIn("x", str(ctx.exception))


class TestReplaceProgram(unittest.TestCase):
    """`replace_program` walks every function in a multi-function
    Program. Each function gets its own Replacer instance so the
    local counter restarts at 1."""

    def test_two_functions_have_independent_local_counters(self):
        prog = asm_ast.Program(function_definition=[
            asm_ast.Function(
                name="foo", params=[],
                instructions=[
                    asm_ast.Mov(src=asm_ast.Imm(value=1),
                                dst=asm_ast.Pseudo(name="t")),
                    asm_ast.Ret(arg_bytes=0, local_bytes=0),
                ],
            ),
            asm_ast.Function(
                name="bar", params=[],
                instructions=[
                    asm_ast.Mov(src=asm_ast.Imm(value=2),
                                dst=asm_ast.Pseudo(name="t")),
                    asm_ast.Ret(arg_bytes=0, local_bytes=0),
                ],
            ),
        ])
        out = replace_program(prog)
        # Each function's `t` lands at Frame(1), independently.
        self.assertEqual(
            out.function_definition[0].instructions[1].dst,
            asm_ast.Frame(offset=1),
        )
        self.assertEqual(
            out.function_definition[1].instructions[1].dst,
            asm_ast.Frame(offset=1),
        )

    def test_function_with_params_and_locals_via_program(self):
        prog = asm_ast.Program(function_definition=[
            asm_ast.Function(
                name="foo", params=["a"],
                instructions=[
                    asm_ast.Mov(src=asm_ast.Imm(value=5),
                                dst=asm_ast.Pseudo(name="t")),
                    asm_ast.Mov(src=asm_ast.Pseudo(name="a"),
                                dst=_reg_a()),
                    asm_ast.Ret(arg_bytes=0, local_bytes=0),
                ],
            ),
        ])
        out = replace_program(prog)
        fn = out.function_definition[0]
        # M=1 (one local `t`), N=1 (one param `a`). Local at 1,
        # param at 1+2+1=4.
        self.assertEqual(
            fn.instructions[0],
            asm_ast.FunctionPrologue(arg_bytes=1, local_bytes=1),
        )
        self.assertEqual(
            fn.instructions[1].dst, asm_ast.Frame(offset=1),
        )
        self.assertEqual(
            fn.instructions[2].src, asm_ast.Frame(offset=4),
        )


class TestErrors(unittest.TestCase):
    def test_unknown_program_raises(self):
        stub = type("Stub", (asm_ast.Type_program,), {})
        with self.assertRaises(TypeError):
            replace_program(stub())

    def test_unknown_function_raises(self):
        stub = type("Stub", (asm_ast.Type_function_definition,), {})
        with self.assertRaises(TypeError):
            replace_function(stub())


if __name__ == "__main__":
    unittest.main()
