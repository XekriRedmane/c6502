"""Tests of sim/assembler.py that the C++ port's doctest cases
(cpp/tests/test_sim_assembler.cpp) also check. It goes with
sim/assembler.py."""
from __future__ import annotations

import unittest

import asm_ast
from sim.assembler import AssemblerError, assemble


class TestImage(unittest.TestCase):

    def test_program_past_ffff(self) -> None:
        # The image stays 64K, where a slice assignment would grow it.
        prog = asm_ast.Program(top_level=[asm_ast.StaticVariable(
            name="big", is_global=False,
            init=[asm_ast.ZeroInit(bytes=0xF800), asm_ast.CharInit(value=1)],
        )])
        with self.assertRaises(AssemblerError) as cm:
            assemble(prog, origin=0x0800)
        self.assertEqual(str(cm.exception),
                         "the program's bytes end at $10001, past $FFFF")

    def test_program_below_0000(self) -> None:
        prog = asm_ast.Program(top_level=[asm_ast.StaticVariable(
            name="v", is_global=False, init=[asm_ast.CharInit(value=1)],
        )])
        with self.assertRaises(AssemblerError) as cm:
            assemble(prog, origin=-2)
        self.assertEqual(str(cm.exception),
                         "the program's bytes start at -$0002, below $0000")


if __name__ == "__main__":
    unittest.main()
