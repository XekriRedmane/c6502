import unittest

import asm_ast
from allocate_stack import (
    allocate_function,
    allocate_program,
)


def _reg_a():
    return asm_ast.Reg(reg=asm_ast.A())


class TestAllocateFunction(unittest.TestCase):
    def test_no_frame_ops_inserts_zero_prologue(self):
        fn = asm_ast.Function(name="main", instructions=[
            asm_ast.Mov(src=asm_ast.Imm(value=42), dst=_reg_a()),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ])
        out = allocate_function(fn)
        self.assertEqual(out, asm_ast.Function(name="main", instructions=[
            asm_ast.FunctionPrologue(arg_bytes=0, local_bytes=0),
            asm_ast.Mov(src=asm_ast.Imm(value=42), dst=_reg_a()),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ]))

    def test_single_frame_op_size_equals_offset(self):
        fn = asm_ast.Function(name="main", instructions=[
            asm_ast.Mov(src=asm_ast.Imm(value=1),
                        dst=asm_ast.Frame(offset=1)),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ])
        out = allocate_function(fn)
        self.assertEqual(out.instructions[0],
                         asm_ast.FunctionPrologue(arg_bytes=0, local_bytes=1))

    def test_multiple_frames_uses_max_offset(self):
        # Highest Frame offset is 3, even though offsets 1 and 3 are
        # referenced and 2 isn't (gap-tolerant).
        fn = asm_ast.Function(name="main", instructions=[
            asm_ast.Mov(src=asm_ast.Imm(value=1),
                        dst=asm_ast.Frame(offset=1)),
            asm_ast.Mov(src=asm_ast.Imm(value=2),
                        dst=asm_ast.Frame(offset=3)),
            asm_ast.Mov(src=asm_ast.Frame(offset=3), dst=_reg_a()),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ])
        out = allocate_function(fn)
        self.assertEqual(out.instructions[0],
                         asm_ast.FunctionPrologue(arg_bytes=0, local_bytes=3))

    def test_frame_in_unary_src_dst_is_counted(self):
        fn = asm_ast.Function(name="main", instructions=[
            asm_ast.Unary(op=asm_ast.Neg(),
                          src_dst=asm_ast.Frame(offset=5)),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ])
        out = allocate_function(fn)
        self.assertEqual(out.instructions[0],
                         asm_ast.FunctionPrologue(arg_bytes=0, local_bytes=5))

    def test_non_frame_operands_dont_inflate_size(self):
        # Imm, Reg, Stack all present but no Frame -> M = 0.
        fn = asm_ast.Function(name="main", instructions=[
            asm_ast.Mov(src=asm_ast.Imm(value=9), dst=_reg_a()),
            asm_ast.Mov(src=asm_ast.Stack(offset=200), dst=_reg_a()),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ])
        out = allocate_function(fn)
        self.assertEqual(out.instructions[0],
                         asm_ast.FunctionPrologue(arg_bytes=0, local_bytes=0))

    def test_existing_instructions_preserved_with_ret_rewritten(self):
        # Non-Ret instructions are passed through verbatim; each Ret
        # has its amt updated to M.
        fn = asm_ast.Function(name="main", instructions=[
            asm_ast.Mov(src=asm_ast.Imm(value=1),
                        dst=asm_ast.Frame(offset=1)),
            asm_ast.Unary(op=asm_ast.Not(),
                          src_dst=asm_ast.Frame(offset=1)),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ])
        out = allocate_function(fn)
        self.assertEqual(out, asm_ast.Function(name="main", instructions=[
            asm_ast.FunctionPrologue(arg_bytes=0, local_bytes=1),
            asm_ast.Mov(src=asm_ast.Imm(value=1),
                        dst=asm_ast.Frame(offset=1)),
            asm_ast.Unary(op=asm_ast.Not(),
                          src_dst=asm_ast.Frame(offset=1)),
            asm_ast.Ret(arg_bytes=0, local_bytes=1),
        ]))

    def test_ret_amt_set_to_zero_when_no_locals(self):
        # M=0 means Ret(amt=0) — emit will collapse to plain RTS.
        fn = asm_ast.Function(name="main", instructions=[
            asm_ast.Mov(src=asm_ast.Imm(value=42), dst=_reg_a()),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ])
        out = allocate_function(fn)
        self.assertEqual(out.instructions[-1], asm_ast.Ret(arg_bytes=0, local_bytes=0))

    def test_multiple_rets_all_get_M(self):
        fn = asm_ast.Function(name="main", instructions=[
            asm_ast.Mov(src=asm_ast.Imm(value=1),
                        dst=asm_ast.Frame(offset=2)),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
            # Hypothetical second Ret (early return); pass should
            # update both.
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ])
        out = allocate_function(fn)
        self.assertEqual(out.instructions[-2], asm_ast.Ret(arg_bytes=0, local_bytes=2))
        self.assertEqual(out.instructions[-1], asm_ast.Ret(arg_bytes=0, local_bytes=2))

    def test_empty_function(self):
        fn = asm_ast.Function(name="main", instructions=[])
        out = allocate_function(fn)
        self.assertEqual(out, asm_ast.Function(name="main", instructions=[
            asm_ast.FunctionPrologue(arg_bytes=0, local_bytes=0),
        ]))


class TestAllocateProgram(unittest.TestCase):
    def test_full_tree(self):
        prog = asm_ast.Program(
            function_definition=asm_ast.Function(name="main", instructions=[
                asm_ast.Mov(src=asm_ast.Imm(value=5),
                            dst=asm_ast.Frame(offset=2)),
                asm_ast.Ret(arg_bytes=0, local_bytes=0),
            ]),
        )
        expected = asm_ast.Program(
            function_definition=asm_ast.Function(name="main", instructions=[
                asm_ast.FunctionPrologue(arg_bytes=0, local_bytes=2),
                asm_ast.Mov(src=asm_ast.Imm(value=5),
                            dst=asm_ast.Frame(offset=2)),
                asm_ast.Ret(arg_bytes=0, local_bytes=2),
            ]),
        )
        self.assertEqual(allocate_program(prog), expected)


class TestErrors(unittest.TestCase):
    def test_unknown_program_raises(self):
        stub = type("Stub", (asm_ast.Type_program,), {})
        with self.assertRaises(TypeError):
            allocate_program(stub())

    def test_unknown_function_raises(self):
        stub = type("Stub", (asm_ast.Type_function_definition,), {})
        with self.assertRaises(TypeError):
            allocate_function(stub())


if __name__ == "__main__":
    unittest.main()
