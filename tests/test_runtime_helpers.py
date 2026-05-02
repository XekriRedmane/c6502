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


def _read_longlong(memory: bytearray) -> int:
    """4-byte LongLong return at HARGS+8..11."""
    v = 0
    for i in range(4):
        v |= memory[0x04 + 8 + i] << (i * 8)
    return v


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestMul8(unittest.TestCase):
    """8-bit unsigned multiply (low byte). Same hook for signed and
    unsigned because C's int*int wraps to int under §6.5.5.4 modular
    semantics, and the bit pattern of the low byte is identical for
    both interpretations."""

    def _assert(self, src: str, expected: int) -> None:
        res = run_c_program(src)
        got = _signed_byte(res.a)
        self.assertEqual(got, expected, msg=src)

    def test_basic(self) -> None:
        self._assert("int main(void) { return 6 * 7; }", 42)

    def test_zero(self) -> None:
        self._assert("int main(void) { return 0 * 99; }", 0)
        self._assert("int main(void) { return 99 * 0; }", 0)

    def test_one(self) -> None:
        self._assert("int main(void) { return 1 * 42; }", 42)
        self._assert("int main(void) { return 42 * 1; }", 42)

    def test_wraps_modular(self) -> None:
        # 17 * 13 = 221 = 0xDD; signed byte = -35.
        self._assert("int main(void) { return 17 * 13; }", -35)
        # 100 * 5 = 500. 500 mod 256 = 244 = 0xF4; signed = -12.
        self._assert("int main(void) { return 100 * 5; }", -12)

    def test_neg_neg(self) -> None:
        # (-1) * (-1) = $FF * $FF = $FE01; low byte = 1.
        self._assert("int main(void) { return (-1) * (-1); }", 1)
        # (-2) * (-3) = $FE * $FD = $FA06; low byte = 6.
        self._assert("int main(void) { return (-2) * (-3); }", 6)

    def test_neg_pos(self) -> None:
        # (-3) * 5 = -15.
        self._assert("int main(void) { return (-3) * 5; }", -15)
        # 5 * (-3) = -15.
        self._assert("int main(void) { return 5 * (-3); }", -15)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestMul16(unittest.TestCase):
    """16-bit unsigned multiply (low 2 bytes)."""

    def _long(self, src: str) -> int:
        res = run_c_program(src)
        return res.return_long()

    def test_basic(self) -> None:
        self.assertEqual(
            self._long("long main(void) { return 100L * 50L; }"),
            5000,
        )

    def test_zero(self) -> None:
        self.assertEqual(
            self._long("long main(void) { return 0L * 12345L; }"),
            0,
        )

    def test_unsigned_range(self) -> None:
        # 200 * 200 = 40000, fits 16-bit unsigned.
        self.assertEqual(
            self._long("long main(void) { return 200L * 200L; }"),
            40000,
        )

    def test_modular_wrap(self) -> None:
        # 1000 * 100 = 100000 → mod 65536 = 34464.
        self.assertEqual(
            self._long("long main(void) { return 1000L * 100L; }"),
            34464,
        )

    def test_signed_negative_inputs(self) -> None:
        # (-100) * 50 = -5000; bit pattern 0xEC78 = 60536 unsigned.
        self.assertEqual(
            self._long("long main(void) { return (-100L) * 50L; }"),
            60536,
        )

    def test_exact_high_byte_value(self) -> None:
        # 256 * 256 = 65536 → mod 65536 = 0 (the wrap case).
        self.assertEqual(
            self._long("long main(void) { return 256L * 256L; }"),
            0,
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestMul32(unittest.TestCase):
    """32-bit unsigned multiply (low 4 bytes). LongLong returns sit
    at HARGS+8..11 (the FP / LongLong shared slot)."""

    def _ll(self, src: str) -> int:
        res = run_c_program(src)
        return _read_longlong(res.memory)

    def test_basic(self) -> None:
        self.assertEqual(
            self._ll("long long main(void) { return 100LL * 50LL; }"),
            5000,
        )

    def test_zero(self) -> None:
        self.assertEqual(
            self._ll("long long main(void) { return 0LL * 0xFFFFFFFFLL; }"),
            0,
        )

    def test_large_value(self) -> None:
        # 1,000,000 * 1000 = 1,000,000,000 — fits in 32 bits.
        self.assertEqual(
            self._ll(
                "long long main(void) { return 1000000LL * 1000LL; }"
            ),
            1_000_000_000,
        )

    def test_modular_wrap(self) -> None:
        # 65536 * 65536 = 2^32 → mod 2^32 = 0.
        self.assertEqual(
            self._ll(
                "long long main(void) { return 65536LL * 65536LL; }"
            ),
            0,
        )

    def test_max_x_max(self) -> None:
        # 0xFFFFFFFF * 0xFFFFFFFF mod 2^32 = 1
        # (since (2^32 - 1)^2 = 2^64 - 2^33 + 1, mod 2^32 = 1).
        # Use unsigned long long to avoid the 0xFFFFFFFFL parser
        # promoting to signed.
        self.assertEqual(
            self._ll(
                "long long main(void) { unsigned long long x = "
                "0xFFFFFFFFULL; return (long long)(x * x); }"
            ),
            1,
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestUdivmod8(unittest.TestCase):
    """Unsigned 8-bit divmod. The chapter test corpus only exercises
    `int` divisions (signed), so the unsigned helper picks up
    coverage from `unsigned int` arithmetic — we hand-build cases
    here to cover the full 0..255 range and edge values."""

    def _assert(self, src: str, expected: int) -> None:
        res = run_c_program(src)
        got = _signed_byte(res.a)
        self.assertEqual(got, expected, msg=src)

    def test_zero_dividend(self) -> None:
        self._assert(
            "int main(void) { unsigned int x = 0; "
            "unsigned int y = 5; return (int)(x / y); }",
            0,
        )

    def test_dividend_smaller_than_divisor(self) -> None:
        self._assert(
            "int main(void) { unsigned int x = 3; "
            "unsigned int y = 5; return (int)(x / y); }",
            0,
        )
        self._assert(
            "int main(void) { unsigned int x = 3; "
            "unsigned int y = 5; return (int)(x % y); }",
            3,
        )

    def test_quotient_one(self) -> None:
        self._assert(
            "int main(void) { unsigned int x = 5; "
            "unsigned int y = 5; return (int)(x / y); }",
            1,
        )
        self._assert(
            "int main(void) { unsigned int x = 5; "
            "unsigned int y = 5; return (int)(x % y); }",
            0,
        )

    def test_high_byte_dividend(self) -> None:
        # 200 / 7 = 28 rem 4. 28 fits Int (truncated to int range
        # via the cast). The cast (int) on an unsigned int truncates
        # bits at the storage layer, so the int sees the same bit
        # pattern as the unsigned int — for 28 = 0x1C, that's still
        # 28 as a signed Int.
        self._assert(
            "int main(void) { unsigned int x = 200; "
            "unsigned int y = 7; return (int)(x / y); }",
            28,
        )
        self._assert(
            "int main(void) { unsigned int x = 200; "
            "unsigned int y = 7; return (int)(x % y); }",
            4,
        )

    def test_max_unsigned(self) -> None:
        # 255 / 1 = 255 rem 0. (int)255 = -1 (signed-byte view).
        self._assert(
            "int main(void) { unsigned int x = 255; "
            "unsigned int y = 1; return (int)(x / y); }",
            -1,
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestSdivmod8(unittest.TestCase):
    """Signed 8-bit divmod with C99 trunc-toward-zero. Covers the
    sign-correction logic on every (sign(n), sign(d)) combination."""

    def _assert(self, src: str, expected: int) -> None:
        res = run_c_program(src)
        got = _signed_byte(res.a)
        self.assertEqual(got, expected, msg=src)

    def test_pos_pos(self) -> None:
        self._assert("int main(void) { return 12 / 5; }", 2)
        self._assert("int main(void) { return 12 % 5; }", 2)

    def test_neg_pos_div_truncates_toward_zero(self) -> None:
        # C99: -12 / 5 = -2 (not -3 as Python's // would give).
        self._assert("int main(void) { return (-12) / 5; }", -2)
        # Remainder follows dividend sign: -12 % 5 = -2.
        self._assert("int main(void) { return (-12) % 5; }", -2)

    def test_pos_neg(self) -> None:
        self._assert("int main(void) { return 12 / (-5); }", -2)
        # Remainder follows dividend sign: 12 % -5 = 2.
        self._assert("int main(void) { return 12 % (-5); }", 2)

    def test_neg_neg(self) -> None:
        self._assert("int main(void) { return (-12) / (-5); }", 2)
        self._assert("int main(void) { return (-12) % (-5); }", -2)

    def test_zero_dividend(self) -> None:
        self._assert("int main(void) { return 0 / 5; }", 0)
        self._assert("int main(void) { return 0 / (-5); }", 0)
        self._assert("int main(void) { return 0 % 5; }", 0)

    def test_one_divisor(self) -> None:
        self._assert("int main(void) { return 42 / 1; }", 42)
        self._assert("int main(void) { return (-42) / 1; }", -42)
        self._assert("int main(void) { return 42 / (-1); }", -42)
        self._assert("int main(void) { return (-42) / (-1); }", 42)

    def test_int_min_neg_one(self) -> None:
        # INT_MIN / -1 overflows in C (UB). With c6502's 1-byte int,
        # -128 / -1 = 128 which doesn't fit; the result wraps to -128
        # in two's complement. This exercises the
        # `-(-128) → -128` overflow path in sdivmod8.
        self._assert("int main(void) { return (-128) / (-1); }", -128)

    def test_dividend_equals_divisor(self) -> None:
        self._assert("int main(void) { return 5 / 5; }", 1)
        self._assert("int main(void) { return (-5) / (-5); }", 1)
        self._assert("int main(void) { return (-5) / 5; }", -1)

    def test_remainder_round_trip(self) -> None:
        # Spot-check (a / b) * b + (a % b) == a for various inputs
        # (the C99 defining identity for / and %).
        for a in (-100, -50, -1, 0, 1, 7, 50, 100):
            for b in (-7, -3, -1, 1, 3, 7):
                expected = a   # the identity
                src = (
                    f"int main(void) {{ int a = {a}; int b = {b}; "
                    "return (a / b) * b + (a % b); }"
                )
                self._assert(src, expected)


if __name__ == "__main__":
    unittest.main()
