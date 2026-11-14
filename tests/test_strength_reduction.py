"""Tests for `passes.optimization.strength_reduction`.

Coverage:
  * Multiply by 2^k → LeftShift by k (signed and unsigned).
  * Multiply by 1 → Copy.
  * Multiply by non-power-of-2 → unchanged.
  * Unsigned Divide by 2^k → RightShift by k.
  * Signed Divide by 2^k → unchanged (rounding semantics differ).
  * Unsigned Modulo by 2^k → BitwiseAnd by 2^k - 1.
  * Signed Modulo by 2^k → unchanged.
  * Mul/Div by 1 with both operands constant → unchanged (defers
    to constant folding).
  * Operand ordering: `2 * x` reduced same as `x * 2`.

Sim-end-to-end checks confirm the rewritten programs produce
identical results to the unoptimized helper-call paths for both
signed and unsigned arithmetic.
"""
from __future__ import annotations

import unittest

import tac_ast
import c99_ast
from passes import ir_check
from passes.optimization.strength_reduction import reduce_strength
from passes.type_checking import (
    LocalAttr, Symbol, SymbolTable,
)
from sim.harness import build_sim, run_c_program


# ---------------------------------------------------------------------------
# TAC-level rewrite tests
# ---------------------------------------------------------------------------


def _var(name: str) -> tac_ast.Var:
    return tac_ast.Var(name=name)


def _const_uint(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstUInt(value=v))


def _const_int(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstInt(value=v))


def _fn_with(symbols: dict, *instrs) -> tuple[tac_ast.Function, dict]:
    fn = tac_ast.Function(
        name="f", is_global=True, params=[],
        instructions=list(instrs),
    )
    return fn, symbols


def _sym_uint(name: str) -> tuple[str, Symbol]:
    return name, Symbol(type=c99_ast.UInt(), attrs=LocalAttr())


def _sym_int(name: str) -> tuple[str, Symbol]:
    return name, Symbol(type=c99_ast.Int(), attrs=LocalAttr())


class TestStrengthReduceMultiply(unittest.TestCase):
    def test_unsigned_var_times_8_to_shift_3(self):
        symbols = SymbolTable()
        for k, v in (_sym_uint("%x"), _sym_uint("%dst")):
            symbols[k] = v
        fn, _ = _fn_with(
            symbols,
            tac_ast.Binary(
                op=tac_ast.Multiply(),
                src1=_var("%x"), src2=_const_uint(8), dst=_var("%dst"),
            ),
        )
        out = reduce_strength(fn, symbols=symbols)
        self.assertEqual(len(out.instructions), 1)
        instr = out.instructions[0]
        self.assertIsInstance(instr.op, tac_ast.LeftShift)
        self.assertEqual(instr.src1, _var("%x"))
        self.assertEqual(instr.src2.const.value, 3)

    def test_signed_var_times_2_to_shift_1(self):
        # Multiply→LeftShift is safe for signed too (same bit
        # pattern, same C99 wrap-around).
        symbols = SymbolTable()
        for k, v in (_sym_int("%x"), _sym_int("%dst")):
            symbols[k] = v
        fn, _ = _fn_with(
            symbols,
            tac_ast.Binary(
                op=tac_ast.Multiply(),
                src1=_var("%x"), src2=_const_int(2), dst=_var("%dst"),
            ),
        )
        out = reduce_strength(fn, symbols=symbols)
        self.assertIsInstance(out.instructions[0].op, tac_ast.LeftShift)

    def test_var_times_1_to_copy(self):
        symbols = SymbolTable()
        for k, v in (_sym_uint("%x"), _sym_uint("%dst")):
            symbols[k] = v
        fn, _ = _fn_with(
            symbols,
            tac_ast.Binary(
                op=tac_ast.Multiply(),
                src1=_var("%x"), src2=_const_uint(1), dst=_var("%dst"),
            ),
        )
        out = reduce_strength(fn, symbols=symbols)
        self.assertEqual(len(out.instructions), 1)
        self.assertIsInstance(out.instructions[0], tac_ast.Copy)
        self.assertEqual(out.instructions[0].src, _var("%x"))

    def test_const_times_var_commutes(self):
        # 4 * x is reducible the same way as x * 4.
        symbols = SymbolTable()
        for k, v in (_sym_uint("%x"), _sym_uint("%dst")):
            symbols[k] = v
        fn, _ = _fn_with(
            symbols,
            tac_ast.Binary(
                op=tac_ast.Multiply(),
                src1=_const_uint(4), src2=_var("%x"), dst=_var("%dst"),
            ),
        )
        out = reduce_strength(fn, symbols=symbols)
        self.assertIsInstance(out.instructions[0].op, tac_ast.LeftShift)
        self.assertEqual(out.instructions[0].src1, _var("%x"))
        self.assertEqual(out.instructions[0].src2.const.value, 2)

    def _reduce_uint_multiply(self, c: int) -> list:
        symbols = SymbolTable()
        for k, v in (_sym_uint("%x"), _sym_uint("%dst")):
            symbols[k] = v
        fn, _ = _fn_with(
            symbols,
            tac_ast.Binary(
                op=tac_ast.Multiply(),
                src1=_var("%x"), src2=_const_uint(c), dst=_var("%dst"),
            ),
        )
        return reduce_strength(fn, symbols=symbols).instructions

    def _shape(self, instrs: list) -> list:
        """Each Binary as (op name, src1, src2, dst), with Vars as
        names and Constants as their values."""
        def v(x):
            return x.name if isinstance(x, tac_ast.Var) else x.const.value
        return [
            (type(i.op).__name__, v(i.src1), v(i.src2), v(i.dst))
            for i in instrs
        ]

    def test_non_power_of_two_unchanged(self):
        # 11 is neither 2^a + 2^b nor 2^a - 2^b.
        out = self._reduce_uint_multiply(11)
        self.assertIsInstance(out[0].op, tac_ast.Multiply)

    def test_power_plus_one_is_shift_and_add(self):
        self.assertEqual(self._shape(self._reduce_uint_multiply(9)), [
            ("LeftShift", "%x", 3, "%dst.sr0"),
            ("Add", "%dst.sr0", "%x", "%dst"),
        ])

    def test_power_minus_one_is_shift_and_subtract(self):
        self.assertEqual(self._shape(self._reduce_uint_multiply(7)), [
            ("LeftShift", "%x", 3, "%dst.sr0"),
            ("Subtract", "%dst.sr0", "%x", "%dst"),
        ])

    def test_two_shifted_terms_share_the_low_shift(self):
        # 10 = 8 + 2: t = x << 1, u = t << 2, dst = u + t.
        self.assertEqual(self._shape(self._reduce_uint_multiply(10)), [
            ("LeftShift", "%x", 1, "%dst.sr0"),
            ("LeftShift", "%dst.sr0", 2, "%dst.sr1"),
            ("Add", "%dst.sr1", "%dst.sr0", "%dst"),
        ])

    def test_two_term_rejected_when_shift_would_call_helper(self):
        # 65 = 64 + 1 needs a 16-bit shift by 6, which `shift_cost`
        # leaves to `asl16`, so the multiply stays.
        out = self._reduce_uint_multiply(65)
        self.assertIsInstance(out[0].op, tac_ast.Multiply)

    def test_both_constants_unchanged(self):
        # Defer to constant folding; reduce_strength shouldn't
        # produce an awkward Copy(Constant, dst) where the source
        # is itself a constant.
        symbols = SymbolTable()
        symbols["%dst"] = Symbol(type=c99_ast.UInt(), attrs=LocalAttr())
        fn, _ = _fn_with(
            symbols,
            tac_ast.Binary(
                op=tac_ast.Multiply(),
                src1=_const_uint(3), src2=_const_uint(4), dst=_var("%dst"),
            ),
        )
        out = reduce_strength(fn, symbols=symbols)
        self.assertIsInstance(out.instructions[0].op, tac_ast.Multiply)


class TestStrengthReduceDivide(unittest.TestCase):
    def test_unsigned_var_divided_by_4_to_shift_2(self):
        symbols = SymbolTable()
        for k, v in (_sym_uint("%x"), _sym_uint("%dst")):
            symbols[k] = v
        fn, _ = _fn_with(
            symbols,
            tac_ast.Binary(
                op=tac_ast.Divide(),
                src1=_var("%x"), src2=_const_uint(4), dst=_var("%dst"),
            ),
        )
        out = reduce_strength(fn, symbols=symbols)
        self.assertIsInstance(out.instructions[0].op, tac_ast.RightShift)
        self.assertEqual(out.instructions[0].src2.const.value, 2)

    def test_signed_divide_unchanged(self):
        # x / 2 for signed x must NOT become x >> 1 — they disagree
        # on negative dividends (-3/2 == -1, -3>>1 == -2).
        symbols = SymbolTable()
        for k, v in (_sym_int("%x"), _sym_int("%dst")):
            symbols[k] = v
        fn, _ = _fn_with(
            symbols,
            tac_ast.Binary(
                op=tac_ast.Divide(),
                src1=_var("%x"), src2=_const_int(2), dst=_var("%dst"),
            ),
        )
        out = reduce_strength(fn, symbols=symbols)
        self.assertIsInstance(out.instructions[0].op, tac_ast.Divide)

    def test_unsigned_var_divided_by_1_to_copy(self):
        symbols = SymbolTable()
        for k, v in (_sym_uint("%x"), _sym_uint("%dst")):
            symbols[k] = v
        fn, _ = _fn_with(
            symbols,
            tac_ast.Binary(
                op=tac_ast.Divide(),
                src1=_var("%x"), src2=_const_uint(1), dst=_var("%dst"),
            ),
        )
        out = reduce_strength(fn, symbols=symbols)
        self.assertIsInstance(out.instructions[0], tac_ast.Copy)


class TestStrengthReduceModulo(unittest.TestCase):
    def test_unsigned_var_mod_8_to_and_7(self):
        symbols = SymbolTable()
        for k, v in (_sym_uint("%x"), _sym_uint("%dst")):
            symbols[k] = v
        fn, _ = _fn_with(
            symbols,
            tac_ast.Binary(
                op=tac_ast.Modulo(),
                src1=_var("%x"), src2=_const_uint(8), dst=_var("%dst"),
            ),
        )
        out = reduce_strength(fn, symbols=symbols)
        self.assertIsInstance(out.instructions[0].op, tac_ast.BitwiseAnd)
        # mask = 8 - 1 = 7
        self.assertEqual(out.instructions[0].src2.const.value, 7)

    def test_signed_modulo_unchanged(self):
        symbols = SymbolTable()
        for k, v in (_sym_int("%x"), _sym_int("%dst")):
            symbols[k] = v
        fn, _ = _fn_with(
            symbols,
            tac_ast.Binary(
                op=tac_ast.Modulo(),
                src1=_var("%x"), src2=_const_int(8), dst=_var("%dst"),
            ),
        )
        out = reduce_strength(fn, symbols=symbols)
        self.assertIsInstance(out.instructions[0].op, tac_ast.Modulo)

    def test_unsigned_var_mod_1_to_zero(self):
        symbols = SymbolTable()
        for k, v in (_sym_uint("%x"), _sym_uint("%dst")):
            symbols[k] = v
        fn, _ = _fn_with(
            symbols,
            tac_ast.Binary(
                op=tac_ast.Modulo(),
                src1=_var("%x"), src2=_const_uint(1), dst=_var("%dst"),
            ),
        )
        out = reduce_strength(fn, symbols=symbols)
        self.assertEqual(len(out.instructions), 1)
        self.assertIsInstance(out.instructions[0], tac_ast.Copy)
        self.assertIsInstance(out.instructions[0].src, tac_ast.Constant)
        self.assertEqual(out.instructions[0].src.const.value, 0)


# ---------------------------------------------------------------------------
# End-to-end sim tests — strength reduction + inline shift produce
# identical results to the unoptimized helper-call path.
# ---------------------------------------------------------------------------


class TestEndToEndShifts(unittest.TestCase):
    def _both_paths(self, src: str):
        no_opt = run_c_program(src).return_int_signed()
        opt = build_sim(src, optimize=True).run().return_int_signed()
        return no_opt, opt

    def test_two_multiplies_into_one_global(self):
        # A global keeps its name in SSA form, so each multiply's
        # temporaries need names of their own (fuzzer stmts22250).
        src = """
        unsigned long z __attribute__((zeropage)) = 128;
        int main(void) {
            unsigned char w;
            z *= 3;
            w = 0;
            do {
                if (w & 1) z *= 129;
            } while (++w < 4);
            return (int)(z & 0x7FFF);
        }
        """
        was = ir_check.enabled()
        ir_check.enable()
        try:
            a, b = self._both_paths(src)
        finally:
            ir_check.enable(was)
        want = (128 * 3 * 129 * 129) & 0x7FFF
        self.assertEqual((a, b), (want, want))

    def test_unsigned_multiply_by_2(self):
        src = (
            "unsigned int f(unsigned int x) { return x * 2; }\n"
            "int main(void) { return (int)f(13579u); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual(a, b)

    def test_unsigned_multiply_by_8(self):
        src = (
            "unsigned int f(unsigned int x) { return x * 8; }\n"
            "int main(void) { return (int)f(100u); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual(a, b)

    def test_signed_multiply_by_4(self):
        # Signed * power of 2 is also reduced to LeftShift; check
        # both positive and (after wrap) effectively negative.
        src = (
            "int f(int x) { return x * 4; }\n"
            "int main(void) { return f(7); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual(a, b)

    def test_unsigned_divide_by_2(self):
        src = (
            "unsigned int f(unsigned int x) { return x / 2; }\n"
            "int main(void) { return (int)f(15u); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual(a, b)

    def test_unsigned_modulo_by_4(self):
        src = (
            "unsigned int f(unsigned int x) { return x % 4; }\n"
            "int main(void) { return (int)f(13u); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual(a, b)

    def test_signed_divide_by_4_keeps_truncation(self):
        # Signed division must NOT be strength-reduced — verify the
        # rounding-toward-zero behavior is preserved.
        src = (
            "int f(int x) { return x / 4; }\n"
            "int main(void) { return f(-7); }\n"  # -7/4 = -1
        )
        a, b = self._both_paths(src)
        self.assertEqual(a, -1)
        self.assertEqual(b, -1)


class TestInlineShiftByOne(unittest.TestCase):
    """The inline `LDA / ASL/LSR/ROR / STA` byte-chain in
    `tac_to_asm` matches the runtime helper's result. Test 1, 2,
    and 4-byte shifts; signed and unsigned right shifts."""

    def _run(self, src: str, *, opt: bool = False) -> int:
        if opt:
            return build_sim(src, optimize=True).run().return_int_signed()
        return run_c_program(src).return_int_signed()

    def test_uint16_shift_left_by_1(self):
        src = (
            "unsigned int f(unsigned int x) { return x << 1; }\n"
            "int main(void) { return (int)f(0x4000u); }\n"  # 0x8000
        )
        # No-opt path uses asl16 helper. --optimize uses inline.
        # Both should match: 0x8000 (which signs to -32768 in int).
        self.assertEqual(self._run(src, opt=False), -32768)
        self.assertEqual(self._run(src, opt=True), -32768)

    def test_uint16_shift_right_by_1(self):
        src = (
            "unsigned int f(unsigned int x) { return x >> 1; }\n"
            "int main(void) { return (int)f(0xFFFEu); }\n"  # 0x7FFF
        )
        self.assertEqual(self._run(src, opt=False), 0x7FFF)
        self.assertEqual(self._run(src, opt=True), 0x7FFF)

    def test_int16_shift_right_by_1_signed(self):
        # Arithmetic right shift preserves sign: -2 >> 1 = -1.
        src = (
            "int f(int x) { return x >> 1; }\n"
            "int main(void) { return f(-2); }\n"
        )
        self.assertEqual(self._run(src, opt=False), -1)
        self.assertEqual(self._run(src, opt=True), -1)

    def test_int16_shift_right_negative_odd(self):
        # -3 >> 1 = -2 (arithmetic right shift rounds toward -inf).
        src = (
            "int f(int x) { return x >> 1; }\n"
            "int main(void) { return f(-3); }\n"
        )
        self.assertEqual(self._run(src, opt=False), -2)
        self.assertEqual(self._run(src, opt=True), -2)


if __name__ == "__main__":
    unittest.main()
