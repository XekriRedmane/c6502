"""Tests for `passes.optimization.value_range`,
`passes.optimization.narrow_byte_range`, strength reduction's rule for
a nonnegative signed dividend, and the `tac_to_asm` lowerings they
lead to (`_inline_byte_divide`, `_shift_loop`, `_inline_byte_product`,
and the pairing of a divide with a remainder)."""

from __future__ import annotations

import shutil
import unittest

import asm_ast
import c99_ast
import tac_ast
from passes.optimization.framework import PassContext
from passes.optimization.narrow_byte_range import NarrowByteRange
from passes.optimization.strength_reduction import reduce_strength
from passes.optimization.value_range import max_value
from passes.type_checking import LocalAttr, Symbol, SymbolTable


def _var(name):
    return tac_ast.Var(name=name)


def _int(v):
    return tac_ast.Constant(const=tac_ast.ConstInt(value=v))


def _symbols(types):
    symbols = SymbolTable()
    for name, t in types.items():
        symbols[name] = Symbol(type=t, attrs=LocalAttr())
    return symbols


_TYPES = {
    "b": c99_ast.UChar(), "c": c99_ast.UChar(), "s": c99_ast.SChar(),
    "ze": c99_ast.Int(), "zc": c99_ast.Int(), "se": c99_ast.Int(),
    "t": c99_ast.Int(), "u": c99_ast.Int(), "r": c99_ast.Int(),
    "w": c99_ast.Int(), "uw": c99_ast.UInt(),
}

_PROLOGUE = [
    tac_ast.ZeroExtend(src=_var("b"), dst=_var("ze")),
    tac_ast.ZeroExtend(src=_var("c"), dst=_var("zc")),
    tac_ast.SignExtend(src=_var("s"), dst=_var("se")),
]


def _fn(*body):
    return tac_ast.Function(
        name="f", is_global=True, params=[],
        instructions=_PROLOGUE + list(body) + [tac_ast.Ret(val=_var("r"))],
    )


def _ssa():
    return set(_TYPES)


def _bound(fn, name, symbols=None):
    symbols = symbols or _symbols(_TYPES)
    defs = {}
    for ins in fn.instructions:
        d = getattr(ins, "dst", None)
        if isinstance(d, tac_ast.Var):
            defs[d.name] = ins
    return max_value(_var(name), lambda v: defs.get(v.name), symbols, _ssa())


def _narrow(*body):
    symbols = _symbols(_TYPES)
    ctx = PassContext(symbols=symbols, ssa_dsts=_ssa())
    return NarrowByteRange().run(_fn(*body), ctx).instructions, symbols


class TestValueRange(unittest.TestCase):

    def test_zero_extend_of_a_byte(self):
        self.assertEqual(_bound(_fn(), "ze"), 255)

    def test_sign_extend_of_a_signed_byte_is_unbounded(self):
        self.assertIsNone(_bound(_fn(), "se"))

    def test_and_with_a_constant_bounds_a_signed_value(self):
        fn = _fn(tac_ast.Binary(op=tac_ast.BitwiseAnd(), src1=_var("se"),
                                src2=_int(3), dst=_var("t")))
        self.assertEqual(_bound(fn, "t"), 3)

    def test_add_and_shift(self):
        fn = _fn(
            tac_ast.Binary(op=tac_ast.RightShift(), src1=_var("ze"),
                           src2=_int(5), dst=_var("t")),
            tac_ast.Binary(op=tac_ast.Add(), src1=_var("t"), src2=_int(1),
                           dst=_var("u")),
        )
        self.assertEqual(_bound(fn, "t"), 7)
        self.assertEqual(_bound(fn, "u"), 8)

    def test_add_that_may_wrap_is_unbounded(self):
        fn = _fn(tac_ast.Binary(op=tac_ast.Add(), src1=_var("ze"),
                                src2=_int(32700), dst=_var("t")))
        self.assertIsNone(_bound(fn, "t"))

    def test_unsigned_type_bounds_any_value(self):
        self.assertEqual(_bound(_fn(), "uw"), 0xFFFF)
        self.assertIsNone(_bound(_fn(), "w"))


class TestNarrowByteRange(unittest.TestCase):

    def test_divide_by_constant_becomes_byte_divide(self):
        out, _ = _narrow(tac_ast.Binary(
            op=tac_ast.Divide(), src1=_var("ze"), src2=_int(20),
            dst=_var("r")))
        div = next(i for i in out if isinstance(i, tac_ast.Binary))
        self.assertEqual(div.src1, _var("b"))
        self.assertEqual(div.src2.const, tac_ast.ConstUChar(value=20))
        self.assertIn(tac_ast.ZeroExtend(src=div.dst, dst=_var("r")), out)

    def test_modulo_by_byte_var(self):
        out, _ = _narrow(tac_ast.Binary(
            op=tac_ast.Modulo(), src1=_var("ze"), src2=_var("zc"),
            dst=_var("r")))
        mod = next(i for i in out if isinstance(i, tac_ast.Binary))
        self.assertEqual((mod.src1, mod.src2), (_var("b"), _var("c")))

    def test_divisor_out_of_range_is_left(self):
        for c in (0, 256, -3):
            with self.subTest(c=c):
                body = tac_ast.Binary(op=tac_ast.Divide(), src1=_var("ze"),
                                      src2=_int(c), dst=_var("r"))
                out, _ = _narrow(body)
                self.assertIn(body, out)

    def test_signed_dividend_is_left(self):
        body = tac_ast.Binary(op=tac_ast.Divide(), src1=_var("se"),
                              src2=_int(3), dst=_var("r"))
        out, _ = _narrow(body)
        self.assertIn(body, out)

    def test_right_shift_of_byte_by_variable(self):
        out, _ = _narrow(tac_ast.Binary(
            op=tac_ast.RightShift(), src1=_var("ze"), src2=_var("zc"),
            dst=_var("r")))
        sh = next(i for i in out if isinstance(i, tac_ast.Binary))
        # The count is read as the byte it widens.
        self.assertEqual((sh.src1, sh.src2), (_var("b"), _var("c")))

    def test_right_shift_by_eight_is_zero(self):
        out, _ = _narrow(tac_ast.Binary(
            op=tac_ast.RightShift(), src1=_var("ze"), src2=_int(9),
            dst=_var("r")))
        self.assertIn(tac_ast.Copy(src=_int(0), dst=_var("r")), out)

    def test_left_shift_that_stays_a_byte(self):
        # `(b >> 3) << 3` is at most 248. The pass narrows the right
        # shift on its first run and the left one on its second.
        symbols = _symbols(_TYPES)
        ctx = PassContext(symbols=symbols, ssa_dsts=_ssa())
        fn = _fn(
            tac_ast.Binary(op=tac_ast.RightShift(), src1=_var("ze"),
                           src2=_int(3), dst=_var("t")),
            tac_ast.Binary(op=tac_ast.LeftShift(), src1=_var("t"),
                           src2=_int(3), dst=_var("r")))
        for _ in range(2):
            fn = NarrowByteRange().run(fn, ctx)
        out = fn.instructions
        sh = [i for i in out if isinstance(i, tac_ast.Binary)
              and isinstance(i.op, tac_ast.LeftShift)]
        self.assertEqual(len(sh), 1)
        self.assertEqual(sh[0].src2, tac_ast.Constant(
            const=tac_ast.ConstUChar(value=3)))
        self.assertIn(tac_ast.ZeroExtend(src=sh[0].dst, dst=_var("r")),
                      out)

    def test_left_shift_past_a_byte_is_left(self):
        body = tac_ast.Binary(op=tac_ast.LeftShift(), src1=_var("ze"),
                              src2=_int(1), dst=_var("r"))
        out, _ = _narrow(body)
        self.assertIn(body, out)

    def test_bounded_wide_value_is_truncated(self):
        out, _ = _narrow(
            tac_ast.Binary(op=tac_ast.BitwiseAnd(), src1=_var("w"),
                           src2=_int(0x3F), dst=_var("t")),
            tac_ast.Binary(op=tac_ast.Divide(), src1=_var("t"),
                           src2=_int(5), dst=_var("r")),
        )
        trunc = next(i for i in out if isinstance(i, tac_ast.Truncate))
        self.assertEqual(trunc.src, _var("t"))

    def test_multiply_reads_bounded_operand_as_extended_byte(self):
        out, symbols = _narrow(
            tac_ast.Binary(op=tac_ast.Add(), src1=_var("zc"),
                           src2=_int(1), dst=_var("t")),
            tac_ast.Binary(op=tac_ast.BitwiseAnd(), src1=_var("t"),
                           src2=_int(7), dst=_var("u")),
            tac_ast.Binary(op=tac_ast.Multiply(), src1=_var("ze"),
                           src2=_var("u"), dst=_var("r")),
        )
        mul = next(i for i in out if isinstance(i, tac_ast.Binary)
                   and isinstance(i.op, tac_ast.Multiply))
        self.assertEqual(mul.src1, _var("ze"))
        ext = next(i for i in out if isinstance(i, tac_ast.ZeroExtend)
                   and i.dst == mul.src2)
        trunc = next(i for i in out if isinstance(i, tac_ast.Truncate))
        self.assertEqual((trunc.src, trunc.dst), (_var("u"), ext.src))

    def test_multiply_of_extended_bytes_is_left(self):
        body = tac_ast.Binary(op=tac_ast.Multiply(), src1=_var("ze"),
                              src2=_var("zc"), dst=_var("r"))
        out, _ = _narrow(body)
        self.assertIn(body, out)


class TestSignedNonnegativeStrength(unittest.TestCase):

    def _run(self, op, k):
        symbols = _symbols(_TYPES)
        fn = _fn(
            tac_ast.Binary(op=tac_ast.Add(), src1=_var("ze"), src2=_int(1),
                           dst=_var("t")),
            tac_ast.Binary(op=op, src1=_var("t"), src2=_int(k),
                           dst=_var("r")),
        )
        from passes.optimization.framework import RuleSet
        from passes.optimization.strength_reduction import STRENGTH_RULES
        ctx = PassContext(symbols=symbols, ssa_dsts=_ssa())
        return RuleSet(*STRENGTH_RULES, name="sr").run(fn, ctx).instructions

    def test_divide_becomes_shift(self):
        out = self._run(tac_ast.Divide(), 2)
        self.assertIn(tac_ast.Binary(
            op=tac_ast.RightShift(), src1=_var("t"), src2=_int(1),
            dst=_var("r")), out)

    def test_modulo_becomes_mask(self):
        out = self._run(tac_ast.Modulo(), 8)
        self.assertIn(tac_ast.Binary(
            op=tac_ast.BitwiseAnd(), src1=_var("t"), src2=_int(7),
            dst=_var("r")), out)

    def test_possibly_negative_dividend_is_left(self):
        symbols = _symbols(_TYPES)
        fn = _fn(tac_ast.Binary(op=tac_ast.Divide(), src1=_var("se"),
                                src2=_int(2), dst=_var("r")))
        out = reduce_strength(fn, symbols=symbols).instructions
        self.assertTrue(any(isinstance(i, tac_ast.Binary)
                            and isinstance(i.op, tac_ast.Divide)
                            for i in out))


def _asm(src: str, optimize: bool = True) -> str:
    from compile import _run_stage
    from preprocessor import preprocess
    return _run_stage("codegen", preprocess(src), optimize=optimize)


def _body(asm: str, fn: str) -> str:
    """The lines of function `fn`, up to the next function's label."""
    rest = asm[asm.index(f"\n{fn}:"):]
    nxt = rest.find(":\n   SUBROUTINE", len(fn) + 3)
    return rest if nxt < 0 else rest[:rest.rfind("\n", 0, nxt)]


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestLowerings(unittest.TestCase):

    def test_byte_divide_is_inline(self):
        asm = _asm("#include <stdint.h>\n"
                   "uint8_t f(uint8_t x) { return x / 20; }\n")
        body = _body(asm, "f")
        self.assertIn("SBC   #$14", body)
        self.assertNotIn("JSR", body)
        self.assertNotIn("JMP", body)

    def test_divide_and_remainder_share_one_loop(self):
        asm = _asm("#include <stdint.h>\n"
                   "uint16_t f(uint8_t x) { uint8_t c = x % 20;"
                   " uint8_t r = x / 20; return c | (uint16_t)r << 8; }\n")
        body = _body(asm, "f")
        self.assertEqual(body.count("SBC   #$14"), 1)
        self.assertIn("ADC   #$14", body)

    def test_word_divide_and_remainder_share_one_call(self):
        asm = _asm("#include <stdint.h>\n"
                   "uint16_t q;\n"
                   "uint16_t f(uint16_t v, uint16_t p) "
                   "{ q = v / p; v %= p; return v; }\n")
        self.assertEqual(_body(asm, "f").count("udivmod16"), 1)

    def test_variable_shifts_are_loops(self):
        src = ("#include <stdint.h>\n"
               "unsigned int f(unsigned int x, uint8_t n) { return x << n; }\n"
               "int g(int x, uint8_t n) { return x >> n; }\n"
               "uint8_t h(uint8_t x, uint8_t n) { return x >> n; }\n")
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                asm = _asm(src, optimize)
                self.assertNotIn("JSR   asl", asm)
                self.assertNotIn("JSR   asr", asm)
                self.assertNotIn("JSR   lsr", asm)
                self.assertIn("DEX", asm)

    def test_shift_of_promoted_byte_is_logical(self):
        # `(style >> 5)` inside a product had called `asr16`.
        asm = _asm("#include <stdint.h>\n"
                   "uint16_t f(uint8_t p, uint8_t s) "
                   "{ return p * (((s >> 5) & 3) + 1); }\n")
        body = _body(asm, "f")
        self.assertEqual(body.count("LSR\n"), 5)
        self.assertNotIn("JSR", body)
        self.assertNotIn("mul16", body)

    def test_byte_product_is_inline(self):
        asm = _asm("#include <stdint.h>\n"
                   "uint16_t f(uint8_t a, uint8_t b) { return a * b; }\n")
        body = _body(asm, "f")
        self.assertIn("ROR   HARGS", body)
        self.assertNotIn("mul16", body)

    def test_word_product_calls_mul16(self):
        asm = _asm("#include <stdint.h>\n"
                   "uint16_t f(uint16_t a, uint8_t b) { return a * b; }\n")
        self.assertIn("mul16", _body(asm, "f"))


class TestShiftLoop(unittest.TestCase):
    """`tac_to_asm._shift_loop` reads a count that's also the
    destination before the copy into the destination overwrites it."""

    def test_count_in_destination_is_read_first(self):
        from tac_to_asm import _shift_loop
        x = asm_ast.Pseudo(name="x", offset=0)
        n = asm_ast.Pseudo(name="n", offset=0)
        out = _shift_loop(x, n, None, n, 1, "asl", ".l", ".d", True)
        self.assertEqual(out[:3], [
            asm_ast.Mov(src=n, dst=asm_ast.Reg(reg=asm_ast.A()),
                        is_volatile=False),
            asm_ast.Mov(src=asm_ast.Reg(reg=asm_ast.A()),
                        dst=asm_ast.Reg(reg=asm_ast.X()), is_volatile=False),
            asm_ast.Mov(src=x, dst=n, is_volatile=False),
        ])


if __name__ == "__main__":
    unittest.main()
