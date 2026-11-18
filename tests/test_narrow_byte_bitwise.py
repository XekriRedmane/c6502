"""Tests for `passes.optimization.narrow_byte_bitwise`."""

from __future__ import annotations

import shutil
import unittest

import c99_ast
import tac_ast
from passes.optimization.framework import PassContext
from passes.optimization.narrow_byte_bitwise import NarrowByteBitwise
from passes.type_checking import LocalAttr, Symbol, SymbolTable


def _var(name):
    return tac_ast.Var(name=name)


def _int(v):
    return tac_ast.Constant(const=tac_ast.ConstInt(value=v))


def _run(op, src1, src2, types):
    symbols = SymbolTable()
    for name, t in types.items():
        symbols[name] = Symbol(type=t, attrs=LocalAttr())
    fn = tac_ast.Function(
        name="f", is_global=True, params=[],
        instructions=[
            tac_ast.ZeroExtend(src=_var("b"), dst=_var("ze")),
            tac_ast.SignExtend(src=_var("s"), dst=_var("se")),
            tac_ast.Binary(op=op, src1=src1, src2=src2, dst=_var("r")),
            tac_ast.Ret(val=_var("r")),
        ],
    )
    ctx = PassContext(symbols=symbols, ssa_dsts={"ze", "se", "r", "b", "s",
                                                 "w"})
    return NarrowByteBitwise().run(fn, ctx).instructions


_TYPES = {
    "b": c99_ast.UChar(), "s": c99_ast.SChar(), "ze": c99_ast.Int(),
    "se": c99_ast.Int(), "r": c99_ast.Int(), "w": c99_ast.Int(),
}


def _narrowed(instrs):
    """The byte Binary the pass wrote, or None."""
    for i in instrs:
        if isinstance(i, tac_ast.Binary) and i.dst.name != "r":
            return i
    return None


class TestRule(unittest.TestCase):

    def test_and_of_zero_extend_and_constant(self):
        out = _run(tac_ast.BitwiseAnd(), _var("ze"), _int(0x13F), _TYPES)
        b = _narrowed(out)
        self.assertEqual(b.src1, _var("b"))
        # Only the constant's low byte reaches the byte AND.
        self.assertEqual(b.src2.const, tac_ast.ConstUChar(value=0x3F))
        self.assertIn(tac_ast.ZeroExtend(src=b.dst, dst=_var("r")), out)

    def test_and_of_sign_extend_and_small_constant(self):
        out = _run(tac_ast.BitwiseAnd(), _var("se"), _int(0x7F), _TYPES)
        self.assertEqual(_narrowed(out).src1, _var("s"))

    def test_and_of_zero_extend_and_wide_var_truncates_it(self):
        out = _run(tac_ast.BitwiseAnd(), _var("w"), _var("ze"), _TYPES)
        truncs = [i for i in out if isinstance(i, tac_ast.Truncate)]
        self.assertEqual(len(truncs), 1)
        self.assertEqual(truncs[0].src, _var("w"))
        self.assertEqual(_narrowed(out).src1, truncs[0].dst)

    def test_or_needs_both_high_bytes_zero(self):
        cases = (
            (_var("ze"), _int(0x80), True),
            (_var("ze"), _int(0x100), False),
            (_var("ze"), _int(-1), False),
            (_var("se"), _int(1), False),
            (_var("ze"), _var("w"), False),
        )
        for op in (tac_ast.BitwiseOr(), tac_ast.BitwiseXor()):
            for a, b, narrows in cases:
                with self.subTest(op=op, b=b):
                    out = _run(op, a, b, _TYPES)
                    self.assertEqual(_narrowed(out) is not None, narrows)

    def test_and_with_neither_high_byte_zero_stays(self):
        out = _run(tac_ast.BitwiseAnd(), _var("se"), _int(-2), _TYPES)
        self.assertIsNone(_narrowed(out))

    def test_without_an_extend_stays(self):
        # `w & 0x3F` already writes its high byte as a constant 0.
        out = _run(tac_ast.BitwiseAnd(), _var("w"), _int(0x3F), _TYPES)
        self.assertIsNone(_narrowed(out))


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestEndToEnd(unittest.TestCase):

    _SRC = (
        "#include <stdint.h>\n"
        "uint8_t g = 0xC5;\n"
        "int8_t sg = -3;\n"
        "uint8_t p[8] = {0, 0, 0, 0, 0, 0, 0x2A, 0};\n"
        "int below(uint8_t *q, uint8_t lim) { return (q[6] & 0x3F) < lim; }\n"
        "int main(void) {\n"
        "    int n = 0;\n"
        "    if ((g & 0x3F) < 6) n |= 1;\n"
        "    if ((g | 0x10) == 0xD5) n |= 2;\n"
        "    if ((g ^ 0x0F) > 0xC9) n |= 4;\n"
        "    if ((sg & 0xF0) == 0xF0) n |= 8;\n"
        "    if (below(p, 43)) n |= 16;\n"
        "    if (below(p, 42)) n |= 32;\n"
        "    int8_t d = (int8_t)(g - 0xC6);\n"
        "    if (d > 0) n |= 64;\n"
        "    if (d > -2) n |= 128;\n"
        "    return n;\n"
        "}\n"
    )

    def test_runs(self):
        from sim.harness import build_sim
        # g & 0x3F is 5, g | 0x10 is 0xD5, g ^ 0x0F is 0xCA, sg & 0xF0
        # is 0xF0, p[6] & 0x3F is 42, and d is -1.
        expected = 1 | 2 | 4 | 8 | 16 | 128
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(self._SRC, optimize=optimize).run()
                self.assertEqual(out.return_int(), expected)

    def test_byte_compares(self):
        from compile import _run_stage
        from preprocessor import preprocess
        asm = _run_stage("codegen", preprocess(self._SRC), optimize=True)
        body = asm[asm.index("\nbelow:"):asm.index("\nmain:")]
        # A byte compare has no overflow test.
        self.assertNotIn("BVC", body)
        self.assertNotIn("EOR   #$80", body)


if __name__ == "__main__":
    unittest.main()
