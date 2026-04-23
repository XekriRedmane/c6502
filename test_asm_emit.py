import io
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import asm_ast
from asm_emit import (
    emit_function,
    emit_instruction,
    emit_operand,
    emit_program,
    main,
)


def _prog(*instrs, name="main") -> asm_ast.Type_program:
    return asm_ast.Program(function_definition=asm_ast.Function(
        name=name, instructions=list(instrs),
    ))


class TestEmitOperand(unittest.TestCase):
    def test_imm_hex(self):
        for v, expected in [(0, "#$00"), (1, "#$01"), (0x2A, "#$2A"),
                            (0xFF, "#$FF"), (10, "#$0A")]:
            with self.subTest(v=v):
                self.assertEqual(emit_operand(asm_ast.Imm(value=v)), expected)

    def test_imm_out_of_range_raises(self):
        for v in [-1, 256, 1000, -100]:
            with self.subTest(v=v):
                with self.assertRaises(ValueError):
                    emit_operand(asm_ast.Imm(value=v))

    def test_register_emits_a(self):
        self.assertEqual(emit_operand(asm_ast.Register()), "A")


class TestEmitInstruction(unittest.TestCase):
    def test_ret_emits_rts(self):
        self.assertEqual(emit_instruction(asm_ast.Ret()), ["   RTS"])

    def test_mov_imm_to_register_emits_lda(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=asm_ast.Imm(value=0x2A), dst=asm_ast.Register())
            ),
            ["   LDA   #$2A"],
        )


class TestEmitFunction(unittest.TestCase):
    def test_label_and_instructions(self):
        fn = asm_ast.Function(name="main", instructions=[
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=asm_ast.Register()),
            asm_ast.Ret(),
        ])
        self.assertEqual(
            emit_function(fn),
            ["main:", "   LDA   #$00", "   RTS"],
        )

    def test_empty_instructions_just_label(self):
        fn = asm_ast.Function(name="main", instructions=[])
        self.assertEqual(emit_function(fn), ["main:"])


class TestEmitProgram(unittest.TestCase):
    def test_full(self):
        prog = _prog(
            asm_ast.Mov(src=asm_ast.Imm(value=42), dst=asm_ast.Register()),
            asm_ast.Ret(),
        )
        self.assertEqual(
            emit_program(prog),
            "main:\n   LDA   #$2A\n   RTS\n",
        )


class TestColumnAlignment(unittest.TestCase):
    """Column 1 labels, column 4 opcodes, column 10 operands."""

    def test_columns(self):
        prog = _prog(
            asm_ast.Mov(src=asm_ast.Imm(value=0x2A), dst=asm_ast.Register()),
            asm_ast.Ret(),
        )
        lines = emit_program(prog).splitlines()
        # Label at column 1 (index 0).
        self.assertTrue(lines[0].startswith("main:"))
        # Opcode at column 4 (index 3), operand at column 10 (index 9).
        self.assertEqual(lines[1][:3], "   ")
        self.assertEqual(lines[1][3:6], "LDA")
        self.assertEqual(lines[1][6:9], "   ")
        self.assertEqual(lines[1][9:], "#$2A")
        # RTS has no operand.
        self.assertEqual(lines[2], "   RTS")


class TestMainCLI(unittest.TestCase):
    def test_stdout_output(self):
        src = "int main(void) { return 42; }"
        with patch("sys.stdin", io.StringIO(src)), \
             patch("sys.stdout", new_callable=io.StringIO) as out:
            rc = main(["asm_emit.py", "-"])
        self.assertEqual(rc, 0)
        self.assertEqual(out.getvalue(), "main:\n   LDA   #$2A\n   RTS\n")

    def test_output_file_must_end_in_asm(self):
        with patch("sys.stdin", io.StringIO("int main(void) { return 0; }")), \
             patch("sys.stderr", new_callable=io.StringIO) as err:
            rc = main(["asm_emit.py", "-", "-o", "out.txt"])
        self.assertNotEqual(rc, 0)
        self.assertIn(".asm suffix", err.getvalue())

    def test_file_output_writes_asm(self):
        with tempfile.TemporaryDirectory() as tmp:
            out_path = Path(tmp) / "hello.asm"
            with patch("sys.stdin", io.StringIO("int main(void) { return 7; }")):
                rc = main(["asm_emit.py", "-", "-o", str(out_path)])
            self.assertEqual(rc, 0)
            self.assertEqual(
                out_path.read_text(),
                "main:\n   LDA   #$07\n   RTS\n",
            )


if __name__ == "__main__":
    unittest.main()
