"""Tests for naming the register a parameter arrives in:
`register uint8_t lo __attribute__((in_y))`, and for a 2-byte parameter
a pair such as `in_xy` (low byte in X, high byte in Y). A `register`
parameter without the attribute keeps the positional rule (A, then X),
skipping the registers the named ones take.
"""
from __future__ import annotations

import io
import shutil
import unittest
from unittest.mock import patch

import asm_ast
from compile import _run_stage, main as compile_main
from parser import ParserError, parse
from passes.abi_selection import AbiSelectionError
from passes.prologue_synthesis import _split_entry_stubs
from passes.type_checking import TypeCheckError
from preprocessor import preprocess
from sim.harness import build_sim


def _codegen(src: str, optimize: bool = True) -> str:
    return _run_stage("codegen", preprocess(src), optimize=optimize)


def _body(asm: str, fn: str) -> str:
    return asm.split(f"\n{fn}:")[1].split("SUBROUTINE")[1].split("\n\n", 2)[1]


class TestParse(unittest.TestCase):

    def _markers(self, src: str) -> list[str]:
        return parse(src).declaration[0].function_decl.param_registers

    def test_markers(self):
        self.assertEqual(self._markers(
            "void f(register unsigned char a __attribute__((in_y)),\n"
            "       unsigned char b,\n"
            "       register unsigned int c __attribute__((in_xa)),\n"
            "       register unsigned char d);"
        ), ["Y", "", "XA", "auto"])

    def test_rejected(self):
        bad = {
            "not register": ("void f(unsigned char a __attribute__((in_y)));",
                             "needs the parameter declared `register`"),
            "unknown": ("void f(register unsigned char a "
                        "__attribute__((in_z)));", "unknown parameter attribute"),
            "argument": ("void f(register unsigned char a "
                         "__attribute__((in_y(1))));", "unknown parameter attribute"),
            "twice": ("void f(register unsigned int a "
                      "__attribute__((in_aa)));", "names A twice"),
            "two attributes": ("void f(register unsigned char a "
                               "__attribute__((in_x, in_y)));", "one attribute"),
        }
        for label, (src, needle) in bad.items():
            with self.subTest(label):
                with self.assertRaisesRegex(ParserError, needle):
                    parse(src)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestAbiErrors(unittest.TestCase):

    _CASES = {
        "wrong width": (
            "void f(register unsigned int a __attribute__((in_y))) {}\n"
            "int main(void) { f(1); return 0; }\n",
            "is 2 bytes, but `in_y` names 1 register",
        ),
        "shared register": (
            "void f(register unsigned char a __attribute__((in_y)),\n"
            "       register unsigned char b __attribute__((in_y))) {}\n"
            "int main(void) { f(1, 2); return 0; }\n",
            "both arrive in Y",
        ),
        "positional overflow": (
            "void f(register unsigned char a __attribute__((in_a)),\n"
            "       register unsigned int b) {}\n"
            "int main(void) { f(1, 2); return 0; }\n",
            "the free ones here are: X",
        ),
        "declarations disagree": (
            "void f(register unsigned char a __attribute__((in_y)));\n"
            "void f(register unsigned char a) {}\n"
            "int main(void) { f(1); return 0; }\n",
            "`register a in_y` against `register a`",
        ),
    }

    def test_errors(self):
        for label, (src, needle) in self._CASES.items():
            with self.subTest(label):
                with self.assertRaisesRegex(AbiSelectionError, needle):
                    _codegen(src)

    def test_pointer(self):
        with self.assertRaisesRegex(
                TypeCheckError, "register parameter 'p': a pointer"):
            _codegen(
                "void f(register unsigned char *p "
                "__attribute__((in_xy))) {}\n"
                "int main(void) { f(0); return 0; }\n"
            )

    def test_one_line_error(self):
        err = io.StringIO()
        src = self._CASES["shared register"][0]
        with patch("sys.stdin", io.StringIO(src)), \
                patch("sys.stderr", err), patch("sys.stdout", io.StringIO()):
            rc = compile_main(["compile.py", "-", "--codegen", "--optimize"])
        self.assertEqual(rc, 1)
        self.assertTrue(err.getvalue().startswith("<stdin>: error: "))
        self.assertNotIn("Traceback", err.getvalue())


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestSim(unittest.TestCase):
    """Each program runs through both pipelines."""

    def _check(self, src: str, expected: int, *, standin=None) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(src, optimize=optimize)
                if standin is not None:
                    addr, code = standin
                    sim.mpu.memory[addr:addr + len(code)] = code
                self.assertEqual(sim.run().return_int(), expected)

    def test_compiled_function(self):
        src = (
            "unsigned char out[2];\n"
            "unsigned char g = 9;\n"
            "void put(register unsigned char hi __attribute__((in_a)),\n"
            "         register unsigned char lo __attribute__((in_y))) {\n"
            "    out[0] = hi; out[1] = lo;\n"
            "}\n"
            "int main(void) {\n"
            "    unsigned char a = g + 1;\n"
            "    put(a, g);\n"
            "    return out[0] * 100 + out[1];\n"
            "}\n"
        )
        self._check(src, 1009)

    def test_pair_and_positional(self):
        # `w` takes X and Y, so `k`, declared plain `register`, gets A.
        src = (
            "unsigned int sum(register unsigned int w __attribute__((in_xy)),\n"
            "                 register unsigned char k) {\n"
            "    return w + k;\n"
            "}\n"
            "unsigned int swap(register unsigned int w __attribute__((in_ya))) {\n"
            "    return (w << 8) | (w >> 8);\n"
            "}\n"
            "int main(void) {\n"
            "    unsigned int s = sum(0x1234, 3);\n"
            "    return (s == 0x1237) + (swap(0xABCD) == 0xCDAB) * 2;\n"
            "}\n"
        )
        self._check(src, 3)

    # A routine at $D000 that returns A + 2 * Y, as `STY $3100; ASL
    # $3100; CLC; ADC $3100; RTS`.
    _ROM = (
        "__attribute__((zp_abi, address(0xD000)))\n"
        "unsigned char rom(register unsigned char a __attribute__((in_a)),\n"
        "                  register unsigned char y __attribute__((in_y)));\n"
    )
    _ROM_CODE = (0xD000, bytes([0x8C, 0x00, 0x31, 0x0E, 0x00, 0x31, 0x18,
                                0x6D, 0x00, 0x31, 0x60]))

    def test_fixed_address_routine(self):
        src = self._ROM + (
            "unsigned char g = 20;\n"
            "int main(void) { return rom(g, 3) + rom(1, g); }\n"
        )
        self._check(src, (20 + 6) + (1 + 40), standin=self._ROM_CODE)

    def test_a_argument_from_a_frame_slot(self):
        # Without --optimize, `a` and `b` live on the soft stack, and
        # loading `a` into A is `LDY #off; LDA (FP),Y`, which would
        # destroy `b`, already in Y. So `a` is loaded first and kept
        # with PHA / PLA.
        src = self._ROM + (
            "int main(void) {\n"
            "    unsigned char a = 7, b = 5;\n"
            "    for (unsigned char i = 0; i < 3; i++) { a++; b += 2; }\n"
            "    return rom(a, b);\n"
            "}\n"
        )
        self.assertIn("PHA", _codegen(src, optimize=False))
        self._check(src, 10 + 2 * 11, standin=self._ROM_CODE)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestShape(unittest.TestCase):

    def test_callee_stores_y_and_caller_loads_it(self):
        asm = _codegen(
            "unsigned char out[2];\n"
            "unsigned char g;\n"
            "void put(register unsigned char hi __attribute__((in_a)),\n"
            "         register unsigned char lo __attribute__((in_y))) {\n"
            "    out[0] = hi; out[1] = lo;\n"
            "}\n"
            "int main(void) { put(1, g); return 0; }\n"
        )
        self.assertEqual(_body(asm, "put").split(),
                         ["STA", "out", "STY", "out+1", "RTS"])
        self.assertIn("LDY   g", asm.split("\nmain:")[1])

    def test_address_bytes_go_straight_to_registers(self):
        # The shape of Zork's RWTS call: `LDA #>iob; LDY #<iob`.
        asm = _codegen(
            "unsigned char iob[17];\n"
            "__attribute__((zp_abi, return_bool_in_carry, address(0x2900)))\n"
            "_Bool RWTS(register unsigned char hi __attribute__((in_a)),\n"
            "                   register unsigned char lo __attribute__((in_y)));\n"
            "int main(void) {\n"
            "    return RWTS((unsigned char)((unsigned int)iob >> 8),\n"
            "                (unsigned char)(unsigned int)iob);\n"
            "}\n"
        )
        main = asm.split("\nmain:")[1]
        self.assertIn("LDA   #>iob\n   LDY   #<iob\n   JSR   RWTS", main)
        self.assertNotIn("PHA", main)


class TestPrologue(unittest.TestCase):

    def test_y_stub_goes_before_the_prologue(self):
        # The soft-stack prologue writes Y, so the store of a parameter
        # that arrives in Y has to come first.
        stub = asm_ast.Mov(
            src=asm_ast.Reg(reg=asm_ast.Y()),
            dst=asm_ast.Data(name="__zpabi_f__lo", offset=0),
            is_volatile=False,
        )
        rest = [asm_ast.Return(save_a=False, save_x=False, save_c=False)]
        stubs, body = _split_entry_stubs([stub] + rest, "f")
        self.assertEqual(stubs, [stub])
        self.assertEqual(body, rest)


if __name__ == "__main__":
    unittest.main()
