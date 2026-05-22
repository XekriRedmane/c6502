"""Unit tests for `passes.optimization.narrow_widened_arith`.

Covers the `Truncate(Binary(safe_op, Extend(a), Extend(b) | Const),
u_n)` → `Binary(safe_op, a, b | narrowed_Const, u_n)` rewrite for
the wraparound-safe ops (Add / Subtract / Multiply / BitwiseAnd /
BitwiseOr / BitwiseXor), the soundness gates that exclude
non-wraparound-safe ops (Divide / Modulo / RightShift / LeftShift)
and non-SSA-renamed names, and the constant-narrowing logic that
masks Constants modulo the target width.
"""

import unittest

import c99_ast
import tac_ast
from passes.optimization.narrow_widened_arith import narrow_widened_arith
from passes.type_checking import LocalAttr, StaticAttr, Symbol


def _var(name):
    return tac_ast.Var(name=name)


def _const_int(value):
    return tac_ast.Constant(const=tac_ast.ConstInt(value=value))


def _const_uint(value):
    return tac_ast.Constant(const=tac_ast.ConstUInt(value=value))


def _const_uchar(value):
    return tac_ast.Constant(const=tac_ast.ConstUChar(value=value))


def _fn(instrs):
    return tac_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )


def _local(c99_type):
    return Symbol(type=c99_type, attrs=LocalAttr())


def _static(c99_type):
    return Symbol(
        type=c99_type,
        attrs=StaticAttr(initial_value=None, is_global=True),
    )


def _zext_add_trunc(op_cls):
    """Build the canonical `ZeroExtend; ZeroExtend; Binary; Truncate`
    chain for two uchars promoted to int, op'd, and truncated back
    to uchar. Returns (instrs, symbols, ssa_dsts)."""
    instrs = [
        tac_ast.ZeroExtend(src=_var("a"), dst=_var("ea")),
        tac_ast.ZeroExtend(src=_var("b"), dst=_var("eb")),
        tac_ast.Binary(
            op=op_cls(), src1=_var("ea"), src2=_var("eb"),
            dst=_var("r"),
        ),
        tac_ast.Truncate(src=_var("r"), dst=_var("d")),
    ]
    symbols = {
        "a": _local(c99_ast.UChar()),
        "b": _local(c99_ast.UChar()),
        "ea": _local(c99_ast.Int()),
        "eb": _local(c99_ast.Int()),
        "r": _local(c99_ast.Int()),
        "d": _local(c99_ast.UChar()),
    }
    ssa_dsts = {"ea", "eb", "r", "d"}
    return instrs, symbols, ssa_dsts


class TestNarrowWidenedArith(unittest.TestCase):

    # ------------------------------------------------------------------
    # Positive cases: wraparound-safe ops narrow to 1-byte.
    # ------------------------------------------------------------------

    def test_subtract_two_zeroextended_uchars(self):
        # The motivating headline: `uint8_t d = (uint8_t)(a - b)`
        # lowers as ZE; ZE; Sub; Truncate and should narrow to a
        # single 1-byte Subtract.
        instrs, symbols, ssa_dsts = _zext_add_trunc(tac_ast.Subtract)
        out = narrow_widened_arith(
            _fn(instrs), symbols=symbols, ssa_dsts=ssa_dsts,
        )
        new = out.instructions[-1]
        self.assertIsInstance(new, tac_ast.Binary)
        self.assertIsInstance(new.op, tac_ast.Subtract)
        self.assertEqual(new.src1, _var("a"))
        self.assertEqual(new.src2, _var("b"))
        self.assertEqual(new.dst, _var("d"))

    def test_add_two_zeroextended_uchars(self):
        instrs, symbols, ssa_dsts = _zext_add_trunc(tac_ast.Add)
        out = narrow_widened_arith(
            _fn(instrs), symbols=symbols, ssa_dsts=ssa_dsts,
        )
        new = out.instructions[-1]
        self.assertIsInstance(new.op, tac_ast.Add)
        self.assertEqual(new.src1, _var("a"))
        self.assertEqual(new.src2, _var("b"))

    def test_multiply_two_zeroextended_uchars(self):
        instrs, symbols, ssa_dsts = _zext_add_trunc(tac_ast.Multiply)
        out = narrow_widened_arith(
            _fn(instrs), symbols=symbols, ssa_dsts=ssa_dsts,
        )
        new = out.instructions[-1]
        self.assertIsInstance(new.op, tac_ast.Multiply)

    def test_bitwise_and_or_xor_narrow(self):
        for op_cls in (
            tac_ast.BitwiseAnd, tac_ast.BitwiseOr, tac_ast.BitwiseXor,
        ):
            instrs, symbols, ssa_dsts = _zext_add_trunc(op_cls)
            out = narrow_widened_arith(
                _fn(instrs), symbols=symbols, ssa_dsts=ssa_dsts,
            )
            new = out.instructions[-1]
            self.assertIsInstance(new, tac_ast.Binary)
            self.assertIsInstance(new.op, op_cls)
            self.assertEqual(new.src1, _var("a"))

    def test_signext_two_schars(self):
        # `int8_t d = (int8_t)(a - b)` with signed chars. The
        # SignExtend version of the same shape.
        instrs = [
            tac_ast.SignExtend(src=_var("a"), dst=_var("ea")),
            tac_ast.SignExtend(src=_var("b"), dst=_var("eb")),
            tac_ast.Binary(
                op=tac_ast.Subtract(),
                src1=_var("ea"), src2=_var("eb"), dst=_var("r"),
            ),
            tac_ast.Truncate(src=_var("r"), dst=_var("d")),
        ]
        symbols = {
            "a": _local(c99_ast.SChar()),
            "b": _local(c99_ast.SChar()),
            "ea": _local(c99_ast.Int()),
            "eb": _local(c99_ast.Int()),
            "r": _local(c99_ast.Int()),
            "d": _local(c99_ast.SChar()),
        }
        ssa_dsts = {"ea", "eb", "r", "d"}
        out = narrow_widened_arith(
            _fn(instrs), symbols=symbols, ssa_dsts=ssa_dsts,
        )
        new = out.instructions[-1]
        self.assertIsInstance(new, tac_ast.Binary)
        self.assertEqual(new.src1, _var("a"))
        self.assertEqual(new.src2, _var("b"))

    # ------------------------------------------------------------------
    # Constant narrowing: the second operand can be a wide Constant
    # that we mask down to the narrow width.
    # ------------------------------------------------------------------

    def test_one_extend_one_small_constant(self):
        # `uint8_t d = (uint8_t)(a + 3)` — `a` zero-extended, the
        # `3` is a ConstInt. Narrow the const to ConstUChar(3).
        instrs = [
            tac_ast.ZeroExtend(src=_var("a"), dst=_var("ea")),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=_var("ea"), src2=_const_int(3), dst=_var("r"),
            ),
            tac_ast.Truncate(src=_var("r"), dst=_var("d")),
        ]
        symbols = {
            "a": _local(c99_ast.UChar()),
            "ea": _local(c99_ast.Int()),
            "r": _local(c99_ast.Int()),
            "d": _local(c99_ast.UChar()),
        }
        ssa_dsts = {"ea", "r", "d"}
        out = narrow_widened_arith(
            _fn(instrs), symbols=symbols, ssa_dsts=ssa_dsts,
        )
        new = out.instructions[-1]
        self.assertIsInstance(new, tac_ast.Binary)
        self.assertEqual(new.src1, _var("a"))
        self.assertIsInstance(new.src2, tac_ast.Constant)
        self.assertIsInstance(new.src2.const, tac_ast.ConstUChar)
        self.assertEqual(new.src2.const.value, 3)

    def test_large_constant_masks_modulo(self):
        # `uint8_t d = (uint8_t)(a - 0x1234)` — narrow the 0x1234
        # to (0x1234 & 0xFF) = 0x34. Modular reduction preserves
        # the low byte.
        instrs = [
            tac_ast.ZeroExtend(src=_var("a"), dst=_var("ea")),
            tac_ast.Binary(
                op=tac_ast.Subtract(),
                src1=_var("ea"), src2=_const_int(0x1234),
                dst=_var("r"),
            ),
            tac_ast.Truncate(src=_var("r"), dst=_var("d")),
        ]
        symbols = {
            "a": _local(c99_ast.UChar()),
            "ea": _local(c99_ast.Int()),
            "r": _local(c99_ast.Int()),
            "d": _local(c99_ast.UChar()),
        }
        ssa_dsts = {"ea", "r", "d"}
        out = narrow_widened_arith(
            _fn(instrs), symbols=symbols, ssa_dsts=ssa_dsts,
        )
        new = out.instructions[-1]
        self.assertIsInstance(new.src2.const, tac_ast.ConstUChar)
        self.assertEqual(new.src2.const.value, 0x34)

    def test_negative_constant_signed_target(self):
        # `int8_t d = (int8_t)(a + (-1))` — narrow ConstInt(-1) to
        # ConstChar(-1).
        instrs = [
            tac_ast.SignExtend(src=_var("a"), dst=_var("ea")),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=_var("ea"), src2=_const_int(-1), dst=_var("r"),
            ),
            tac_ast.Truncate(src=_var("r"), dst=_var("d")),
        ]
        symbols = {
            "a": _local(c99_ast.SChar()),
            "ea": _local(c99_ast.Int()),
            "r": _local(c99_ast.Int()),
            "d": _local(c99_ast.SChar()),
        }
        ssa_dsts = {"ea", "r", "d"}
        out = narrow_widened_arith(
            _fn(instrs), symbols=symbols, ssa_dsts=ssa_dsts,
        )
        new = out.instructions[-1]
        self.assertIsInstance(new.src2.const, tac_ast.ConstChar)
        self.assertEqual(new.src2.const.value, -1)

    # ------------------------------------------------------------------
    # Negative cases: ops that aren't wraparound-safe, mismatched
    # widths, non-SSA names.
    # ------------------------------------------------------------------

    def test_divide_not_narrowed(self):
        # Divide is NOT wraparound-safe: high bytes contribute to
        # the low bytes of the quotient. Leave alone.
        instrs, symbols, ssa_dsts = _zext_add_trunc(tac_ast.Divide)
        out = narrow_widened_arith(
            _fn(instrs), symbols=symbols, ssa_dsts=ssa_dsts,
        )
        self.assertIsInstance(out.instructions[-1], tac_ast.Truncate)

    def test_modulo_not_narrowed(self):
        instrs, symbols, ssa_dsts = _zext_add_trunc(tac_ast.Modulo)
        out = narrow_widened_arith(
            _fn(instrs), symbols=symbols, ssa_dsts=ssa_dsts,
        )
        self.assertIsInstance(out.instructions[-1], tac_ast.Truncate)

    def test_right_shift_not_narrowed(self):
        # RightShift: high bytes shift DOWN into the low bytes of
        # the result. Narrowing would lose them.
        instrs, symbols, ssa_dsts = _zext_add_trunc(tac_ast.RightShift)
        out = narrow_widened_arith(
            _fn(instrs), symbols=symbols, ssa_dsts=ssa_dsts,
        )
        self.assertIsInstance(out.instructions[-1], tac_ast.Truncate)

    def test_left_shift_not_narrowed_today(self):
        # LeftShift COULD be narrowed in principle (low bytes
        # depend only on low bytes), but is deliberately excluded
        # by this pass — the asm shift helpers consume only one
        # byte of the count anyway, so the win is marginal.
        instrs, symbols, ssa_dsts = _zext_add_trunc(tac_ast.LeftShift)
        out = narrow_widened_arith(
            _fn(instrs), symbols=symbols, ssa_dsts=ssa_dsts,
        )
        self.assertIsInstance(out.instructions[-1], tac_ast.Truncate)

    def test_dst_width_equals_extend_width_left_alone(self):
        # `Truncate(int -> int)` is a no-op of zero width gap;
        # there's nothing to narrow. (In practice this wouldn't
        # exist post-fold_truncate_extend; defensively skip.)
        instrs = [
            tac_ast.ZeroExtend(src=_var("a"), dst=_var("ea")),
            tac_ast.ZeroExtend(src=_var("b"), dst=_var("eb")),
            tac_ast.Binary(
                op=tac_ast.Subtract(),
                src1=_var("ea"), src2=_var("eb"), dst=_var("r"),
            ),
            tac_ast.Truncate(src=_var("r"), dst=_var("d")),
        ]
        # Now `d` is Int (2 bytes), same width as `r`. No narrowing.
        symbols = {
            "a": _local(c99_ast.UChar()),
            "b": _local(c99_ast.UChar()),
            "ea": _local(c99_ast.Int()),
            "eb": _local(c99_ast.Int()),
            "r": _local(c99_ast.Int()),
            "d": _local(c99_ast.Int()),     # not narrower than r
        }
        ssa_dsts = {"ea", "eb", "r", "d"}
        out = narrow_widened_arith(
            _fn(instrs), symbols=symbols, ssa_dsts=ssa_dsts,
        )
        self.assertIsInstance(out.instructions[-1], tac_ast.Truncate)

    def test_extend_source_width_mismatch_left_alone(self):
        # `a` is a 2-byte int extended to a 4-byte long, then
        # truncated back to a 1-byte uchar. We don't synthesize a
        # narrower truncate of the original source — leave alone.
        instrs = [
            tac_ast.SignExtend(src=_var("a"), dst=_var("ea")),
            tac_ast.SignExtend(src=_var("b"), dst=_var("eb")),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=_var("ea"), src2=_var("eb"), dst=_var("r"),
            ),
            tac_ast.Truncate(src=_var("r"), dst=_var("d")),
        ]
        symbols = {
            "a": _local(c99_ast.Int()),      # 2 bytes
            "b": _local(c99_ast.Int()),      # 2 bytes
            "ea": _local(c99_ast.Long()),    # 4 bytes
            "eb": _local(c99_ast.Long()),    # 4 bytes
            "r": _local(c99_ast.Long()),
            "d": _local(c99_ast.UChar()),    # 1 byte — mismatch
        }
        ssa_dsts = {"ea", "eb", "r", "d"}
        out = narrow_widened_arith(
            _fn(instrs), symbols=symbols, ssa_dsts=ssa_dsts,
        )
        self.assertIsInstance(out.instructions[-1], tac_ast.Truncate)

    def test_binary_dst_is_global_not_narrowed(self):
        # The Binary's dst is a static global, NOT SSA-renamed.
        # The "def reaches this Truncate" assumption breaks
        # (globals can be re-defined). Truncate stays as is.
        instrs = [
            tac_ast.ZeroExtend(src=_var("a"), dst=_var("ea")),
            tac_ast.ZeroExtend(src=_var("b"), dst=_var("eb")),
            tac_ast.Binary(
                op=tac_ast.Subtract(),
                src1=_var("ea"), src2=_var("eb"), dst=_var("r_global"),
            ),
            tac_ast.Truncate(src=_var("r_global"), dst=_var("d")),
        ]
        symbols = {
            "a": _local(c99_ast.UChar()),
            "b": _local(c99_ast.UChar()),
            "ea": _local(c99_ast.Int()),
            "eb": _local(c99_ast.Int()),
            "r_global": _static(c99_ast.Int()),
            "d": _local(c99_ast.UChar()),
        }
        # `r_global` is NOT in ssa_dsts.
        ssa_dsts = {"ea", "eb", "d"}
        out = narrow_widened_arith(
            _fn(instrs), symbols=symbols, ssa_dsts=ssa_dsts,
        )
        self.assertIsInstance(out.instructions[-1], tac_ast.Truncate)

    def test_extend_source_global_not_narrowed(self):
        # One Binary operand is a Var defined by an Extend whose
        # OWN dst is a non-SSA global. Same soundness issue.
        instrs = [
            tac_ast.ZeroExtend(src=_var("a"), dst=_var("ea_global")),
            tac_ast.ZeroExtend(src=_var("b"), dst=_var("eb")),
            tac_ast.Binary(
                op=tac_ast.Subtract(),
                src1=_var("ea_global"), src2=_var("eb"), dst=_var("r"),
            ),
            tac_ast.Truncate(src=_var("r"), dst=_var("d")),
        ]
        symbols = {
            "a": _local(c99_ast.UChar()),
            "b": _local(c99_ast.UChar()),
            "ea_global": _static(c99_ast.Int()),
            "eb": _local(c99_ast.Int()),
            "r": _local(c99_ast.Int()),
            "d": _local(c99_ast.UChar()),
        }
        ssa_dsts = {"eb", "r", "d"}  # ea_global NOT renamed
        out = narrow_widened_arith(
            _fn(instrs), symbols=symbols, ssa_dsts=ssa_dsts,
        )
        self.assertIsInstance(out.instructions[-1], tac_ast.Truncate)

    def test_no_ssa_dsts_is_noop(self):
        instrs, symbols, _ = _zext_add_trunc(tac_ast.Subtract)
        out = narrow_widened_arith(_fn(instrs), symbols=symbols)
        self.assertEqual(out.instructions, instrs)

    def test_no_symbols_is_noop(self):
        instrs, _, ssa_dsts = _zext_add_trunc(tac_ast.Subtract)
        out = narrow_widened_arith(_fn(instrs), ssa_dsts=ssa_dsts)
        self.assertEqual(out.instructions, instrs)


if __name__ == "__main__":
    unittest.main()
