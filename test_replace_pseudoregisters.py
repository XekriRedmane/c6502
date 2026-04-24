import unittest

import asm_ast
from replace_pseudoregisters import (
    Replacer,
    replace_function,
    replace_program,
)


def _reg_a():
    return asm_ast.Reg(reg=asm_ast.A())


class TestReplaceOperand(unittest.TestCase):
    def test_pseudo_new_assigns_offset_and_increments(self):
        r = Replacer(args_bytes=0)
        # FP points at the next-free byte, so the first usable offset
        # is 1, not 0.
        self.assertEqual(
            r.replace_operand(asm_ast.Pseudo(name="t0")),
            asm_ast.Frame(offset=1),
        )
        self.assertEqual(r.sp, 2)
        self.assertEqual(r.offsets, {"t0": 1})

    def test_pseudo_existing_reuses_offset_without_incrementing_sp(self):
        r = Replacer(args_bytes=0)
        r.replace_operand(asm_ast.Pseudo(name="t0"))   # offset 1, sp=2
        r.replace_operand(asm_ast.Pseudo(name="t1"))   # offset 2, sp=3
        # Repeat of t0 should reuse offset 1 and leave sp alone.
        self.assertEqual(
            r.replace_operand(asm_ast.Pseudo(name="t0")),
            asm_ast.Frame(offset=1),
        )
        self.assertEqual(r.sp, 3)

    def test_non_pseudo_passes_through_unchanged(self):
        # Pseudo is the only operand the pass rewrites; everything
        # else (including pre-existing Stack/Frame from another pass)
        # is left alone.
        r = Replacer()
        for op in [
            asm_ast.Imm(value=1),
            asm_ast.Reg(reg=asm_ast.A()),
            asm_ast.Reg(reg=asm_ast.X()),
            asm_ast.Stack(offset=5),
            asm_ast.Frame(offset=7),
        ]:
            with self.subTest(op=op):
                self.assertEqual(r.replace_operand(op), op)
        self.assertEqual(r.sp, 1)
        self.assertEqual(r.offsets, {})

    def test_args_bytes_seeds_starting_sp(self):
        # With N args, the first local sits at offset N+1 (the +1 is
        # the next-free convention for FP).
        r = Replacer(args_bytes=3)
        self.assertEqual(
            r.replace_operand(asm_ast.Pseudo(name="t0")),
            asm_ast.Frame(offset=4),
        )
        self.assertEqual(r.sp, 5)


class TestReplaceInstruction(unittest.TestCase):
    def test_mov_rewrites_src_then_dst(self):
        # Distinct pseudos: src gets the lower offset because it's
        # walked first.
        r = Replacer()
        out = r.replace_instruction(asm_ast.Mov(
            src=asm_ast.Pseudo(name="a"),
            dst=asm_ast.Pseudo(name="b"),
        ))
        self.assertEqual(out, asm_ast.Mov(
            src=asm_ast.Frame(offset=1),
            dst=asm_ast.Frame(offset=2),
        ))

    def test_mov_same_pseudo_on_both_sides_uses_one_offset(self):
        r = Replacer()
        out = r.replace_instruction(asm_ast.Mov(
            src=asm_ast.Pseudo(name="x"),
            dst=asm_ast.Pseudo(name="x"),
        ))
        self.assertEqual(out, asm_ast.Mov(
            src=asm_ast.Frame(offset=1),
            dst=asm_ast.Frame(offset=1),
        ))
        self.assertEqual(r.sp, 2)

    def test_mov_with_imm_src_only_rewrites_dst(self):
        r = Replacer()
        out = r.replace_instruction(asm_ast.Mov(
            src=asm_ast.Imm(value=7),
            dst=asm_ast.Pseudo(name="t"),
        ))
        self.assertEqual(out, asm_ast.Mov(
            src=asm_ast.Imm(value=7),
            dst=asm_ast.Frame(offset=1),
        ))

    def test_other_instructions_pass_through(self):
        r = Replacer()
        for instr in [
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
            asm_ast.Ret(arg_bytes=0, local_bytes=5),
            asm_ast.FunctionPrologue(arg_bytes=0, local_bytes=3),
        ]:
            with self.subTest(instr=instr):
                self.assertEqual(r.replace_instruction(instr), instr)
        self.assertEqual(r.sp, 1)


class TestReplaceFunction(unittest.TestCase):
    def test_assigns_unique_pseudos_distinct_offsets(self):
        fn = asm_ast.Function(name="main", instructions=[
            asm_ast.Mov(src=asm_ast.Imm(value=1),
                        dst=asm_ast.Pseudo(name="a")),
            asm_ast.Mov(src=asm_ast.Imm(value=2),
                        dst=asm_ast.Pseudo(name="b")),
            asm_ast.Mov(src=asm_ast.Pseudo(name="a"),
                        dst=_reg_a()),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ])
        out = replace_function(fn)
        self.assertEqual(out, asm_ast.Function(name="main", instructions=[
            asm_ast.Mov(src=asm_ast.Imm(value=1),
                        dst=asm_ast.Frame(offset=1)),
            asm_ast.Mov(src=asm_ast.Imm(value=2),
                        dst=asm_ast.Frame(offset=2)),
            asm_ast.Mov(src=asm_ast.Frame(offset=1),
                        dst=_reg_a()),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ]))

    def test_function_with_no_pseudos_is_unchanged(self):
        fn = asm_ast.Function(name="main", instructions=[
            asm_ast.Mov(src=asm_ast.Imm(value=42), dst=_reg_a()),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ])
        self.assertEqual(replace_function(fn), fn)

    def test_empty_function(self):
        fn = asm_ast.Function(name="main", instructions=[])
        self.assertEqual(replace_function(fn),
                         asm_ast.Function(name="main", instructions=[]))

    def test_args_bytes_offsets_all_pseudos(self):
        fn = asm_ast.Function(name="main", instructions=[
            asm_ast.Mov(src=asm_ast.Imm(value=1),
                        dst=asm_ast.Pseudo(name="t")),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ])
        # With args_bytes=2, the first local lands at Frame(3) (2 args
        # + the +1 next-free offset).
        out = replace_function(fn, args_bytes=2)
        self.assertEqual(out, asm_ast.Function(name="main", instructions=[
            asm_ast.Mov(src=asm_ast.Imm(value=1),
                        dst=asm_ast.Frame(offset=3)),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ]))


class TestReplaceProgram(unittest.TestCase):
    def test_full_tree(self):
        prog = asm_ast.Program(
            function_definition=asm_ast.Function(name="main", instructions=[
                asm_ast.Mov(src=asm_ast.Imm(value=5),
                            dst=asm_ast.Pseudo(name="t")),
                asm_ast.Mov(src=asm_ast.Pseudo(name="t"), dst=_reg_a()),
                asm_ast.Ret(arg_bytes=0, local_bytes=0),
            ]),
        )
        expected = asm_ast.Program(
            function_definition=asm_ast.Function(name="main", instructions=[
                asm_ast.Mov(src=asm_ast.Imm(value=5),
                            dst=asm_ast.Frame(offset=1)),
                asm_ast.Mov(src=asm_ast.Frame(offset=1), dst=_reg_a()),
                asm_ast.Ret(arg_bytes=0, local_bytes=0),
            ]),
        )
        self.assertEqual(replace_program(prog), expected)


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
