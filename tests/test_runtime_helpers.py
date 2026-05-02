"""Tests for the 6502 runtime helpers in `sim.runtime_helpers`.

Each helper has its own correctness obligations; this file drives
each in isolation by handcrafting a tiny C program that exercises the
helper and checking the simulator's return value. The setup uses the
full `sim.harness.run_c_program` machinery so the helper is reached
the same way it would be in production — via a `JSR <name>` from
user code.
"""

from __future__ import annotations

import shutil
import unittest

from sim.harness import run_c_program


def _signed_byte(v: int) -> int:
    return v - 0x100 if v & 0x80 else v


def _read_long(memory: bytearray) -> int:
    """4-byte Long return at HARGS+8..11."""
    v = 0
    for i in range(4):
        v |= memory[0x04 + 8 + i] << (i * 8)
    return v


def _read_longlong(memory: bytearray) -> int:
    """8-byte LongLong return at HARGS+16..23."""
    v = 0
    for i in range(8):
        v |= memory[0x04 + 16 + i] << (i * 8)
    return v


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestMul8(unittest.TestCase):
    """8-bit multiply via char-typed arithmetic. Note: post the C99
    width refresh, char operands integer-promote to int (now 16-bit)
    before multiplication, so the underlying helper is `mul16`, not
    `mul8`. The 8-bit wrap behavior is still observable because
    storing the int result back into a char-typed return-value
    truncates to 1 byte. Same byte-level semantics for signed and
    unsigned char per §6.5.5.4 modular wrap."""

    def _assert(self, src: str, expected: int) -> None:
        res = run_c_program(src)
        got = _signed_byte(res.a)
        self.assertEqual(got, expected, msg=src)

    def test_basic(self) -> None:
        self._assert("char main(void) { return 6 * 7; }", 42)

    def test_zero(self) -> None:
        self._assert("char main(void) { return 0 * 99; }", 0)
        self._assert("char main(void) { return 99 * 0; }", 0)

    def test_one(self) -> None:
        self._assert("char main(void) { return 1 * 42; }", 42)
        self._assert("char main(void) { return 42 * 1; }", 42)

    def test_wraps_modular(self) -> None:
        # 17 * 13 = 221 = 0xDD; signed byte = -35.
        self._assert("char main(void) { return 17 * 13; }", -35)
        # 100 * 5 = 500. 500 mod 256 = 244 = 0xF4; signed = -12.
        self._assert("char main(void) { return 100 * 5; }", -12)

    def test_neg_neg(self) -> None:
        # (-1) * (-1) = $FF * $FF = $FE01; low byte = 1.
        self._assert("char main(void) { return (-1) * (-1); }", 1)
        # (-2) * (-3) = $FE * $FD = $FA06; low byte = 6.
        self._assert("char main(void) { return (-2) * (-3); }", 6)

    def test_neg_pos(self) -> None:
        # (-3) * 5 = -15.
        self._assert("char main(void) { return (-3) * 5; }", -15)
        # 5 * (-3) = -15.
        self._assert("char main(void) { return 5 * (-3); }", -15)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestMul16(unittest.TestCase):
    """16-bit unsigned multiply (low 2 bytes) — exercised by `int *
    int` arithmetic now that Int is 2 bytes."""

    def _int(self, src: str) -> int:
        res = run_c_program(src)
        return res.return_int()

    def test_basic(self) -> None:
        self.assertEqual(
            self._int("int main(void) { return 100 * 50; }"),
            5000,
        )

    def test_zero(self) -> None:
        self.assertEqual(
            self._int("int main(void) { return 0 * 12345; }"),
            0,
        )

    def test_unsigned_range(self) -> None:
        # 200 * 200 = 40000, fits 16-bit unsigned.
        self.assertEqual(
            self._int("int main(void) { return 200 * 200; }"),
            40000,
        )

    def test_modular_wrap(self) -> None:
        # 1000 * 100 = 100000 → mod 65536 = 34464.
        self.assertEqual(
            self._int("int main(void) { return 1000 * 100; }"),
            34464,
        )

    def test_signed_negative_inputs(self) -> None:
        # (-100) * 50 = -5000; bit pattern 0xEC78 = 60536 unsigned.
        self.assertEqual(
            self._int("int main(void) { return (-100) * 50; }"),
            60536,
        )

    def test_exact_high_byte_value(self) -> None:
        # 256 * 256 = 65536 → mod 65536 = 0 (the wrap case).
        self.assertEqual(
            self._int("int main(void) { return 256 * 256; }"),
            0,
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestMul32(unittest.TestCase):
    """32-bit unsigned multiply (low 4 bytes) — exercised by `long *
    long` arithmetic now that Long is 4 bytes. Long returns sit at
    HARGS+8..11 (the 4-byte slot)."""

    def _l(self, src: str) -> int:
        res = run_c_program(src)
        return _read_long(res.memory)

    def test_basic(self) -> None:
        self.assertEqual(
            self._l("long main(void) { return 100L * 50L; }"),
            5000,
        )

    def test_zero(self) -> None:
        self.assertEqual(
            self._l("long main(void) { return 0L * 0xFFFFFFL; }"),
            0,
        )

    def test_large_value(self) -> None:
        # 1,000,000 * 1000 = 1,000,000,000 — fits in 32 bits.
        self.assertEqual(
            self._l(
                "long main(void) { return 1000000L * 1000L; }"
            ),
            1_000_000_000,
        )

    def test_modular_wrap(self) -> None:
        # 65536 * 65536 = 2^32 → mod 2^32 = 0.
        self.assertEqual(
            self._l(
                "long main(void) { return 65536L * 65536L; }"
            ),
            0,
        )

    def test_max_x_max(self) -> None:
        # 0xFFFFFFFF * 0xFFFFFFFF mod 2^32 = 1
        # (since (2^32 - 1)^2 = 2^64 - 2^33 + 1, mod 2^32 = 1).
        # Use unsigned long to avoid the 0xFFFFFFFFL parser promoting
        # to signed.
        self.assertEqual(
            self._l(
                "long main(void) { unsigned long x = "
                "0xFFFFFFFFUL; return (long)(x * x); }"
            ),
            1,
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestShifts8(unittest.TestCase):
    """8-bit asl / asr / lsr exercised via char-typed arithmetic.
    Like TestMul8, the underlying helper is the int-width one
    (asl16/asr16/lsr16) — char operands integer-promote before the
    shift, then the result truncates back to 1 byte at the
    char-typed return."""

    def _i(self, src: str) -> int:
        res = run_c_program(src)
        return _signed_byte(res.a)

    def test_asl_count_zero(self) -> None:
        self.assertEqual(self._i("char main(void) { return 42 << 0; }"), 42)

    def test_asl_basic(self) -> None:
        self.assertEqual(self._i("char main(void) { return 3 << 2; }"), 12)

    def test_asl_into_sign_bit(self) -> None:
        # 1 << 7 = $80 = -128 (signed view).
        self.assertEqual(self._i("char main(void) { return 1 << 7; }"), -128)

    def test_asl_count_at_width_zeros_out(self) -> None:
        # The shift happens at int (16-bit) width — 1 << 8 = 256.
        # Truncated to char: 256 mod 256 = 0.
        self.assertEqual(self._i("char main(void) { return 1 << 8; }"), 0)

    def test_lsr_via_unsigned_char(self) -> None:
        # `>>` on `unsigned char` routes through unsigned int.
        self.assertEqual(
            self._i(
                "char main(void) { unsigned char x = 100; "
                "return (char)(x >> 2); }"
            ),
            25,
        )

    def test_asr_negative_sign_fill(self) -> None:
        # (-100) >> 1 = -50 (sign-extended).
        self.assertEqual(
            self._i("char main(void) { return (signed char)(-100) >> 1; }"),
            -50,
        )

    def test_asr_negative_saturates_to_minus_one(self) -> None:
        # (-1) >> any → -1 (the sign keeps filling).
        self.assertEqual(
            self._i("char main(void) { return (signed char)(-1) >> 4; }"),
            -1,
        )
        self.assertEqual(
            self._i("char main(void) { return (signed char)(-1) >> 7; }"),
            -1,
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestShifts16(unittest.TestCase):
    """16-bit asl / asr / lsr — multi-byte ROR/ROL chain. Exercised
    via int-typed arithmetic now that Int is 2 bytes."""

    def _i(self, src: str) -> int:
        res = run_c_program(src)
        v = res.return_int()
        return v - 0x10000 if v & 0x8000 else v

    def test_asl_carry_chain(self) -> None:
        # 1 << 8 = 0x100 — exercises ASL low → ROL high.
        self.assertEqual(self._i("int main(void) { return 1 << 8; }"), 256)

    def test_asl_into_sign_bit(self) -> None:
        # 1 << 15 = 0x8000 = -32768 (signed view).
        self.assertEqual(
            self._i("int main(void) { return 1 << 15; }"), -32768,
        )

    def test_asl_count_at_width(self) -> None:
        self.assertEqual(self._i("int main(void) { return 1 << 16; }"), 0)

    def test_lsr(self) -> None:
        self.assertEqual(
            self._i(
                "int main(void) { unsigned int x = 32000U; "
                "return (int)(x >> 4); }"
            ),
            2000,
        )

    def test_asr_negative(self) -> None:
        self.assertEqual(
            self._i("int main(void) { return (-32000) >> 4; }"), -2000,
        )

    def test_asr_positive(self) -> None:
        self.assertEqual(
            self._i("int main(void) { return 0x7FFF >> 14; }"), 1,
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestShifts32(unittest.TestCase):
    """32-bit asl / asr / lsr — 4-byte ROR/ROL chain. Exercised via
    long-typed arithmetic now that Long is 4 bytes."""

    def _l(self, src: str) -> int:
        res = run_c_program(src)
        v = _read_long(res.memory)
        return v - 0x100000000 if v & 0x80000000 else v

    def test_asl_into_high_byte(self) -> None:
        self.assertEqual(
            self._l("long main(void) { return 1L << 16; }"),
            65536,
        )

    def test_asl_into_sign_bit(self) -> None:
        # 1 << 31 = 0x80000000 = -2147483648.
        self.assertEqual(
            self._l("long main(void) { return 1L << 31; }"),
            -2_147_483_648,
        )

    def test_lsr_extracts_high_half(self) -> None:
        # 0x12345678 >> 16 = 0x1234.
        self.assertEqual(
            self._l(
                "long main(void) { long x = 0x12345678L; "
                "return x >> 16; }"
            ),
            0x1234,
        )

    def test_asr_negative_sign_fill(self) -> None:
        self.assertEqual(
            self._l(
                "long main(void) { return ((long)(-1)) >> 8; }"
            ),
            -1,
        )

    def test_asr_large_count_saturates(self) -> None:
        # (-1) >> 31 still = -1 (sign keeps filling all 32 bits).
        self.assertEqual(
            self._l(
                "long main(void) { return ((long)(-1)) >> 31; }"
            ),
            -1,
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestUdivmod8(unittest.TestCase):
    """Unsigned 8-bit divmod via char-typed arithmetic. The
    underlying helper is now udivmod16 (char promotes to int before
    division), but the result is truncated to 1 byte at the
    char-typed return — so 8-bit semantics are still observable
    through this test class."""

    def _assert(self, src: str, expected: int) -> None:
        res = run_c_program(src)
        got = _signed_byte(res.a)
        self.assertEqual(got, expected, msg=src)

    def test_zero_dividend(self) -> None:
        self._assert(
            "char main(void) { unsigned char x = 0; "
            "unsigned char y = 5; return (char)(x / y); }",
            0,
        )

    def test_dividend_smaller_than_divisor(self) -> None:
        self._assert(
            "char main(void) { unsigned char x = 3; "
            "unsigned char y = 5; return (char)(x / y); }",
            0,
        )
        self._assert(
            "char main(void) { unsigned char x = 3; "
            "unsigned char y = 5; return (char)(x % y); }",
            3,
        )

    def test_quotient_one(self) -> None:
        self._assert(
            "char main(void) { unsigned char x = 5; "
            "unsigned char y = 5; return (char)(x / y); }",
            1,
        )
        self._assert(
            "char main(void) { unsigned char x = 5; "
            "unsigned char y = 5; return (char)(x % y); }",
            0,
        )

    def test_high_byte_dividend(self) -> None:
        # 200 / 7 = 28 rem 4.
        self._assert(
            "char main(void) { unsigned char x = 200; "
            "unsigned char y = 7; return (char)(x / y); }",
            28,
        )
        self._assert(
            "char main(void) { unsigned char x = 200; "
            "unsigned char y = 7; return (char)(x % y); }",
            4,
        )

    def test_max_unsigned(self) -> None:
        # 255 / 1 = 255 rem 0. As signed char that's -1.
        self._assert(
            "char main(void) { unsigned char x = 255; "
            "unsigned char y = 1; return (char)(x / y); }",
            -1,
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestSdivmod8(unittest.TestCase):
    """Signed 8-bit divmod via signed-char arithmetic. C99 trunc-
    toward-zero — the underlying helper is now sdivmod16 since
    char operands promote to int before division, but truncating
    the result to a signed char still exhibits 8-bit edge cases
    like the -128 / -1 wrap."""

    def _assert(self, src: str, expected: int) -> None:
        res = run_c_program(src)
        got = _signed_byte(res.a)
        self.assertEqual(got, expected, msg=src)

    def test_pos_pos(self) -> None:
        self._assert("char main(void) { return (signed char)(12 / 5); }", 2)
        self._assert("char main(void) { return (signed char)(12 % 5); }", 2)

    def test_neg_pos_div_truncates_toward_zero(self) -> None:
        # C99: -12 / 5 = -2 (not -3 as Python's // would give).
        self._assert("char main(void) { return (signed char)((-12) / 5); }", -2)
        # Remainder follows dividend sign: -12 % 5 = -2.
        self._assert("char main(void) { return (signed char)((-12) % 5); }", -2)

    def test_pos_neg(self) -> None:
        self._assert("char main(void) { return (signed char)(12 / (-5)); }", -2)
        # Remainder follows dividend sign: 12 % -5 = 2.
        self._assert("char main(void) { return (signed char)(12 % (-5)); }", 2)

    def test_neg_neg(self) -> None:
        self._assert("char main(void) { return (signed char)((-12) / (-5)); }", 2)
        self._assert("char main(void) { return (signed char)((-12) % (-5)); }", -2)

    def test_zero_dividend(self) -> None:
        self._assert("char main(void) { return (signed char)(0 / 5); }", 0)
        self._assert("char main(void) { return (signed char)(0 / (-5)); }", 0)
        self._assert("char main(void) { return (signed char)(0 % 5); }", 0)

    def test_one_divisor(self) -> None:
        self._assert("char main(void) { return (signed char)(42 / 1); }", 42)
        self._assert("char main(void) { return (signed char)((-42) / 1); }", -42)
        self._assert("char main(void) { return (signed char)(42 / (-1)); }", -42)
        self._assert("char main(void) { return (signed char)((-42) / (-1)); }", 42)

    def test_int_min_neg_one(self) -> None:
        # SCHAR_MIN / -1 overflows in C (UB). With signed char, -128
        # / -1 = 128 which doesn't fit; the result wraps to -128 in
        # two's complement. The 8-bit wrap is observable through the
        # char-typed truncation even though the underlying division
        # happens at int width.
        self._assert(
            "char main(void) { return (signed char)((-128) / (-1)); }", -128,
        )

    def test_dividend_equals_divisor(self) -> None:
        self._assert("char main(void) { return (signed char)(5 / 5); }", 1)
        self._assert("char main(void) { return (signed char)((-5) / (-5)); }", 1)
        self._assert("char main(void) { return (signed char)((-5) / 5); }", -1)

    def test_remainder_round_trip(self) -> None:
        # Spot-check (a / b) * b + (a % b) == a for various inputs
        # (the C99 defining identity for / and %). Bytes fit in
        # signed char (-128..127) so 8-bit truncation doesn't lose
        # information.
        for a in (-100, -50, -1, 0, 1, 7, 50, 100):
            for b in (-7, -3, -1, 1, 3, 7):
                expected = a   # the identity
                src = (
                    f"char main(void) {{ signed char a = {a}; "
                    f"signed char b = {b}; "
                    "return (signed char)((a / b) * b + (a % b)); }"
                )
                self._assert(src, expected)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestUdivmod16(unittest.TestCase):
    """16-bit unsigned divmod via shift-and-subtract — exercised via
    `unsigned int` arithmetic now that Int is 2 bytes."""

    def _i(self, src: str) -> int:
        res = run_c_program(src)
        v = res.return_int()
        return v - 0x10000 if v & 0x8000 else v

    def test_basic(self) -> None:
        self.assertEqual(
            self._i(
                "int main(void) { unsigned int x = 1000U; "
                "unsigned int y = 7U; return (int)(x / y); }"
            ),
            142,
        )
        self.assertEqual(
            self._i(
                "int main(void) { unsigned int x = 1000U; "
                "unsigned int y = 7U; return (int)(x % y); }"
            ),
            6,
        )

    def test_zero_dividend(self) -> None:
        self.assertEqual(
            self._i(
                "int main(void) { unsigned int x = 0U; "
                "unsigned int y = 12345U; return (int)(x / y); }"
            ),
            0,
        )

    def test_dividend_smaller_than_divisor(self) -> None:
        self.assertEqual(
            self._i(
                "int main(void) { unsigned int x = 100U; "
                "unsigned int y = 1000U; return (int)(x / y); }"
            ),
            0,
        )
        self.assertEqual(
            self._i(
                "int main(void) { unsigned int x = 100U; "
                "unsigned int y = 1000U; return (int)(x % y); }"
            ),
            100,
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestSdivmod16(unittest.TestCase):
    """16-bit signed divmod with C99 trunc-toward-zero — exercised
    via int-typed arithmetic now that Int is 2 bytes."""

    def _i(self, src: str) -> int:
        res = run_c_program(src)
        v = res.return_int()
        return v - 0x10000 if v & 0x8000 else v

    def test_pos_pos(self) -> None:
        self.assertEqual(
            self._i("int main(void) { return 1000 / 7; }"), 142,
        )
        self.assertEqual(
            self._i("int main(void) { return 1000 % 7; }"), 6,
        )

    def test_neg_pos(self) -> None:
        self.assertEqual(
            self._i("int main(void) { return (-1000) / 7; }"), -142,
        )
        self.assertEqual(
            self._i("int main(void) { return (-1000) % 7; }"), -6,
        )

    def test_pos_neg(self) -> None:
        self.assertEqual(
            self._i("int main(void) { return 1000 / (-7); }"), -142,
        )
        self.assertEqual(
            self._i("int main(void) { return 1000 % (-7); }"), 6,
        )

    def test_neg_neg(self) -> None:
        self.assertEqual(
            self._i("int main(void) { return (-1000) / (-7); }"), 142,
        )
        self.assertEqual(
            self._i("int main(void) { return (-1000) % (-7); }"), -6,
        )

    def test_int_min_neg_one(self) -> None:
        # Int INT16_MIN / -1 overflows in C; bit pattern wraps to
        # INT16_MIN as the result of the negate (since -(-32768) =
        # 32768 mod 65536 = -32768 in two's complement).
        self.assertEqual(
            self._i("int main(void) { return (-32768) / (-1); }"),
            -32768,
        )

    def test_round_trip_identity(self) -> None:
        # (a / b) * b + (a % b) == a for representative values.
        for a in (-30000, -1000, -7, 0, 7, 1000, 30000):
            for b in (-100, -7, -1, 1, 7, 100):
                src = (
                    f"int main(void) {{ int a = {a}; int b = {b}; "
                    "return (a / b) * b + (a % b); }"
                )
                self.assertEqual(self._i(src), a, msg=src)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestUdivmod32(unittest.TestCase):
    """32-bit unsigned divmod — exercised via `unsigned long`
    arithmetic now that Long is 4 bytes."""

    def _l(self, src: str) -> int:
        res = run_c_program(src)
        v = _read_long(res.memory)
        return v - 0x100000000 if v & 0x80000000 else v

    def test_basic(self) -> None:
        self.assertEqual(
            self._l(
                "long main(void) { unsigned long x = 1000000UL; "
                "unsigned long y = 7UL; return (long)(x / y); }"
            ),
            142857,
        )

    def test_large_unsigned(self) -> None:
        self.assertEqual(
            self._l(
                "long main(void) { unsigned long x = 0xFFFFFFFFUL; "
                "unsigned long y = 0x10000UL; "
                "return (long)(x / y); }"
            ),
            0xFFFF,
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestSdivmod32(unittest.TestCase):
    """32-bit signed divmod — exercised via `long` arithmetic now
    that Long is 4 bytes."""

    def _l(self, src: str) -> int:
        res = run_c_program(src)
        v = _read_long(res.memory)
        return v - 0x100000000 if v & 0x80000000 else v

    def test_pos_pos(self) -> None:
        self.assertEqual(
            self._l(
                "long main(void) { return 1000000L / 7L; }"
            ),
            142857,
        )
        self.assertEqual(
            self._l(
                "long main(void) { return 1000000L % 7L; }"
            ),
            1,
        )

    def test_neg_pos(self) -> None:
        self.assertEqual(
            self._l(
                "long main(void) { return (-1000000L) / 7L; }"
            ),
            -142857,
        )
        self.assertEqual(
            self._l(
                "long main(void) { return (-1000000L) % 7L; }"
            ),
            -1,
        )

    def test_pos_neg(self) -> None:
        self.assertEqual(
            self._l(
                "long main(void) { return 1000000L / (-7L); }"
            ),
            -142857,
        )

    def test_neg_neg(self) -> None:
        self.assertEqual(
            self._l(
                "long main(void) { return (-1000000L) / (-7L); }"
            ),
            142857,
        )

    def test_zero_dividend(self) -> None:
        self.assertEqual(
            self._l(
                "long main(void) { return 0L / 12345L; }"
            ),
            0,
        )


if __name__ == "__main__":
    unittest.main()
