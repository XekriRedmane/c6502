"""Tests for the bundled `<stdint.h>`.

Coverage:
  * Each fixed-width type alias produces the documented byte size
    via `sizeof(<alias>)`.
  * Each `_least` / `_fast` alias matches its exact-width sibling.
  * Each MIN/MAX limit macro has the documented value.
  * Integer constant macros (`INTn_C` / `UINTn_C` / `INTMAX_C` /
    `UINTMAX_C`) produce constants of the right type and value.
  * `intptr_t` / `uintptr_t` are 2 bytes.
  * `intmax_t` / `uintmax_t` are 8 bytes.
  * `ptrdiff_t` limits match c6502's `long`-sized ptrdiff_t.
  * Idempotent double-include.
  * Aliases work in declarations, casts, and arithmetic.
"""
from __future__ import annotations

import unittest

from sim.harness import run_c_program


_HEADER = "#include <stdint.h>\n"


class TestStdintTypeAliases(unittest.TestCase):
    """Each typedef-macro must expand to a c6502 type of the right
    byte width. Test by returning `sizeof(alias)` and reading the
    accumulator."""

    def _sizeof(self, alias: str) -> int:
        src = (
            _HEADER + f"int main(void) {{ return sizeof({alias}); }}"
        )
        return run_c_program(src).return_int_signed()

    def test_exact_width_sizes(self) -> None:
        self.assertEqual(self._sizeof("int8_t"), 1)
        self.assertEqual(self._sizeof("uint8_t"), 1)
        self.assertEqual(self._sizeof("int16_t"), 2)
        self.assertEqual(self._sizeof("uint16_t"), 2)
        self.assertEqual(self._sizeof("int32_t"), 4)
        self.assertEqual(self._sizeof("uint32_t"), 4)
        self.assertEqual(self._sizeof("int64_t"), 8)
        self.assertEqual(self._sizeof("uint64_t"), 8)

    def test_least_width_aliases_exact(self) -> None:
        self.assertEqual(self._sizeof("int_least8_t"), 1)
        self.assertEqual(self._sizeof("uint_least8_t"), 1)
        self.assertEqual(self._sizeof("int_least16_t"), 2)
        self.assertEqual(self._sizeof("uint_least16_t"), 2)
        self.assertEqual(self._sizeof("int_least32_t"), 4)
        self.assertEqual(self._sizeof("uint_least32_t"), 4)
        self.assertEqual(self._sizeof("int_least64_t"), 8)
        self.assertEqual(self._sizeof("uint_least64_t"), 8)

    def test_fast_width_aliases_exact(self) -> None:
        self.assertEqual(self._sizeof("int_fast8_t"), 1)
        self.assertEqual(self._sizeof("uint_fast8_t"), 1)
        self.assertEqual(self._sizeof("int_fast16_t"), 2)
        self.assertEqual(self._sizeof("uint_fast16_t"), 2)
        self.assertEqual(self._sizeof("int_fast32_t"), 4)
        self.assertEqual(self._sizeof("uint_fast32_t"), 4)
        self.assertEqual(self._sizeof("int_fast64_t"), 8)
        self.assertEqual(self._sizeof("uint_fast64_t"), 8)

    def test_intptr_is_two_bytes(self) -> None:
        # 6502 addresses are 16-bit
        self.assertEqual(self._sizeof("intptr_t"), 2)
        self.assertEqual(self._sizeof("uintptr_t"), 2)

    def test_intmax_is_eight_bytes(self) -> None:
        # widest integer modeled = long long
        self.assertEqual(self._sizeof("intmax_t"), 8)
        self.assertEqual(self._sizeof("uintmax_t"), 8)


class TestStdintLimits(unittest.TestCase):
    """Each limit macro must expand to the documented integer
    value."""

    def _run_int_signed(self, body: str) -> int:
        src = _HEADER + f"int main(void) {{ {body} }}"
        return run_c_program(src).return_int_signed()

    def _run_int_unsigned(self, body: str) -> int:
        src = _HEADER + f"unsigned int main(void) {{ {body} }}"
        return run_c_program(src).return_int()

    def _run_long(self, body: str) -> int:
        src = _HEADER + f"long main(void) {{ {body} }}"
        return run_c_program(src).return_long_signed()

    def _run_ulong(self, body: str) -> int:
        src = _HEADER + f"unsigned long main(void) {{ {body} }}"
        return run_c_program(src).return_long()

    def test_int8_limits(self) -> None:
        self.assertEqual(self._run_int_signed("return INT8_MIN;"), -128)
        self.assertEqual(self._run_int_signed("return INT8_MAX;"), 127)
        self.assertEqual(self._run_int_signed("return UINT8_MAX;"), 255)

    def test_int16_limits(self) -> None:
        self.assertEqual(self._run_int_signed("return INT16_MIN;"), -32768)
        self.assertEqual(self._run_int_signed("return INT16_MAX;"), 32767)
        self.assertEqual(self._run_int_unsigned("return UINT16_MAX;"), 65535)

    def test_int32_limits(self) -> None:
        self.assertEqual(self._run_long("return INT32_MIN;"), -0x80000000)
        self.assertEqual(self._run_long("return INT32_MAX;"), 0x7FFFFFFF)
        self.assertEqual(self._run_ulong("return UINT32_MAX;"), 0xFFFFFFFF)

    def test_least_aliases_exact(self) -> None:
        self.assertEqual(self._run_int_signed("return INT_LEAST8_MAX;"), 127)
        self.assertEqual(self._run_int_signed("return INT_LEAST16_MIN;"), -32768)
        self.assertEqual(self._run_long("return INT_LEAST32_MAX;"), 0x7FFFFFFF)
        self.assertEqual(self._run_int_signed("return UINT_LEAST8_MAX;"), 255)

    def test_fast_aliases_exact(self) -> None:
        self.assertEqual(self._run_int_signed("return INT_FAST8_MAX;"), 127)
        self.assertEqual(self._run_int_signed("return INT_FAST16_MIN;"), -32768)
        self.assertEqual(self._run_long("return INT_FAST32_MAX;"), 0x7FFFFFFF)

    def test_intptr_limits(self) -> None:
        self.assertEqual(self._run_int_signed("return INTPTR_MIN;"), -32768)
        self.assertEqual(self._run_int_signed("return INTPTR_MAX;"), 32767)
        self.assertEqual(self._run_int_unsigned("return UINTPTR_MAX;"), 65535)

    def test_ptrdiff_limits(self) -> None:
        # c6502's ptrdiff_t is `long` (4 bytes signed).
        self.assertEqual(self._run_long("return PTRDIFF_MIN;"), -0x80000000)
        self.assertEqual(self._run_long("return PTRDIFF_MAX;"), 0x7FFFFFFF)


class TestStdintIntegerConstantMacros(unittest.TestCase):
    """`INTn_C(value)` / `UINTn_C(value)` produce a constant whose
    value matches the argument and whose type is at least
    `int_leastN_t` / `uint_leastN_t`."""

    def test_int8_c(self) -> None:
        # Result type is at least int_least8_t (= signed char,
        # which integer-promotes to int).
        src = (
            _HEADER
            + "int main(void) { return INT8_C(42); }"
        )
        self.assertEqual(run_c_program(src).return_int_signed(), 42)

    def test_int16_c(self) -> None:
        src = (
            _HEADER
            + "int main(void) { return INT16_C(-12345); }"
        )
        self.assertEqual(run_c_program(src).return_int_signed(), -12345)

    def test_int32_c(self) -> None:
        # 100000 doesn't fit in 16-bit int → require the L suffix.
        src = (
            _HEADER
            + "long main(void) { return INT32_C(100000); }"
        )
        self.assertEqual(run_c_program(src).return_long_signed(), 100000)

    def test_uint32_c(self) -> None:
        src = (
            _HEADER
            + "unsigned long main(void) { return UINT32_C(4000000000); }"
        )
        self.assertEqual(run_c_program(src).return_long(), 4000000000)

    def test_uint16_c(self) -> None:
        src = (
            _HEADER
            + "unsigned int main(void) { return UINT16_C(50000); }"
        )
        self.assertEqual(run_c_program(src).return_int(), 50000)


class TestStdintFunctional(unittest.TestCase):
    """End-to-end uses of the type aliases — declarations, casts,
    arithmetic, function params/returns."""

    def test_int8_arithmetic(self) -> None:
        # int8_t (= signed char) integer-promotes to int for +;
        # result returned as int.
        src = (
            _HEADER
            + "int main(void) {\n"
            + "    int8_t a = 50;\n"
            + "    int8_t b = 70;\n"
            + "    return a + b;\n"
            + "}\n"
        )
        self.assertEqual(run_c_program(src).return_int_signed(), 120)

    def test_uint16_arithmetic(self) -> None:
        src = (
            _HEADER
            + "unsigned int main(void) {\n"
            + "    uint16_t a = 30000;\n"
            + "    uint16_t b = 25000;\n"
            + "    return (unsigned int)(a + b);\n"
            + "}\n"
        )
        self.assertEqual(run_c_program(src).return_int(), 55000)

    def test_int32_arithmetic(self) -> None:
        src = (
            _HEADER
            + "long main(void) {\n"
            + "    int32_t a = 100000L;\n"
            + "    int32_t b = -50000L;\n"
            + "    return a + b;\n"
            + "}\n"
        )
        self.assertEqual(run_c_program(src).return_long_signed(), 50000)

    def test_int8_cast(self) -> None:
        src = (
            _HEADER
            + "int main(void) {\n"
            + "    int x = 300;\n"
            + "    return (int8_t)x;\n"  # 300 & 0xFF = 44, signed
            + "}\n"
        )
        self.assertEqual(run_c_program(src).return_int_signed(), 44)

    def test_function_param_and_return(self) -> None:
        src = (
            _HEADER
            + "int16_t add(int16_t a, int16_t b) { return a + b; }\n"
            + "int main(void) { return add(1234, 4321); }\n"
        )
        self.assertEqual(run_c_program(src).return_int_signed(), 5555)

    def test_idempotent_double_include(self) -> None:
        # Including twice mustn't error — guard + pcpp's
        # auto-pragma-once protect this.
        src = (
            "#include <stdint.h>\n"
            "#include <stdint.h>\n"
            "int main(void) { return INT8_MAX; }\n"
        )
        self.assertEqual(run_c_program(src).return_int_signed(), 127)


if __name__ == "__main__":
    unittest.main()
