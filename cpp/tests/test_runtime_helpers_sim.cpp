// The simulator tests of tests/test_runtime_helpers.py: each runtime
// helper (src/rtlib/helpers.h) run from a small C program that calls it
// with `JSR`, as a compiled program does, or from the inline lowerings
// of a byte, returns the value C gives.
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "batch1_case.h"
#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {
namespace {

std::int64_t signed_byte(std::int64_t v) { return v & 0x80 ? v - 0x100 : v; }

// Returns the 4-byte Long return at HARGS+8..11, with HARGS at $04.
std::uint64_t read_long(const Bytes& memory) {
  std::uint64_t v = 0;
  for (int i = 0; i < 4; ++i) v |= std::uint64_t{memory[0x04 + 8 + i]} << (i * 8);
  return v;
}

namespace mul8 {
// 8-bit multiply via char-typed arithmetic. Note: post the C99
// width refresh, char operands integer-promote to int (now 16-bit)
// before multiplication, so the underlying helper is `mul16`, not
// `mul8`. The 8-bit wrap behavior is still observable because
// storing the int result back into a char-typed return-value
// truncates to 1 byte. Same byte-level semantics for signed and
// unsigned char per §6.5.5.4 modular wrap.
void check_case(const auto& src, const auto& expected) {
  auto res = run_c_program(src);
  auto got = signed_byte(res.a);
  CHECK_MESSAGE(got == expected, src);
}

TEST_CASE("runtime_helpers: mul8: basic") { check_case("char main(void) { return 6 * 7; }", 42); }

TEST_CASE("runtime_helpers: mul8: zero") {
  check_case("char main(void) { return 0 * 99; }", 0);
  check_case("char main(void) { return 99 * 0; }", 0);
}

TEST_CASE("runtime_helpers: mul8: one") {
  check_case("char main(void) { return 1 * 42; }", 42);
  check_case("char main(void) { return 42 * 1; }", 42);
}

TEST_CASE("runtime_helpers: mul8: wraps modular") {
  // 17 * 13 = 221 = 0xDD; signed byte = -35.
  check_case("char main(void) { return 17 * 13; }", -35);
  // 100 * 5 = 500. 500 mod 256 = 244 = 0xF4; signed = -12.
  check_case("char main(void) { return 100 * 5; }", -12);
}

TEST_CASE("runtime_helpers: mul8: neg neg") {
  // (-1) * (-1) = $FF * $FF = $FE01; low byte = 1.
  check_case("char main(void) { return (-1) * (-1); }", 1);
  // (-2) * (-3) = $FE * $FD = $FA06; low byte = 6.
  check_case("char main(void) { return (-2) * (-3); }", 6);
}

TEST_CASE("runtime_helpers: mul8: neg pos") {
  // (-3) * 5 = -15.
  check_case("char main(void) { return (-3) * 5; }", -15);
  // 5 * (-3) = -15.
  check_case("char main(void) { return 5 * (-3); }", -15);
}

}  // namespace mul8

namespace mul16 {
// 16-bit unsigned multiply (low 2 bytes) — exercised by `int *
// int` arithmetic now that Int is 2 bytes.
auto int_of(const auto& src) {
  auto res = run_c_program(src);
  return res.return_int();
}

TEST_CASE("runtime_helpers: mul16: basic") { CHECK(int_of("int main(void) { return 100 * 50; }") == 5000); }

TEST_CASE("runtime_helpers: mul16: zero") { CHECK(int_of("int main(void) { return 0 * 12345; }") == 0); }

TEST_CASE("runtime_helpers: mul16: unsigned range") {
  // 200 * 200 = 40000, fits 16-bit unsigned.
  CHECK(int_of("int main(void) { return 200 * 200; }") == 40000);
}

TEST_CASE("runtime_helpers: mul16: modular wrap") {
  // 1000 * 100 = 100000 → mod 65536 = 34464.
  CHECK(int_of("int main(void) { return 1000 * 100; }") == 34464);
}

TEST_CASE("runtime_helpers: mul16: signed negative inputs") {
  // (-100) * 50 = -5000; bit pattern 0xEC78 = 60536 unsigned.
  CHECK(int_of("int main(void) { return (-100) * 50; }") == 60536);
}

TEST_CASE("runtime_helpers: mul16: exact high byte value") {
  // 256 * 256 = 65536 → mod 65536 = 0 (the wrap case).
  CHECK(int_of("int main(void) { return 256 * 256; }") == 0);
}

}  // namespace mul16

namespace mul32 {
// 32-bit unsigned multiply (low 4 bytes) — exercised by `long *
// long` arithmetic now that Long is 4 bytes. Long returns sit at
// HARGS+8..11 (the 4-byte slot).
auto l(const auto& src) {
  auto res = run_c_program(src);
  return read_long(res.memory);
}

TEST_CASE("runtime_helpers: mul32: basic") { CHECK(l("long main(void) { return 100L * 50L; }") == 5000); }

TEST_CASE("runtime_helpers: mul32: zero") { CHECK(l("long main(void) { return 0L * 0xFFFFFFL; }") == 0); }

TEST_CASE("runtime_helpers: mul32: large value") {
  // 1,000,000 * 1000 = 1,000,000,000 — fits in 32 bits.
  CHECK(l("long main(void) { return 1000000L * 1000L; }") == 1'000'000'000);
}

TEST_CASE("runtime_helpers: mul32: modular wrap") {
  // 65536 * 65536 = 2^32 → mod 2^32 = 0.
  CHECK(l("long main(void) { return 65536L * 65536L; }") == 0);
}

TEST_CASE("runtime_helpers: mul32: max x max") {
  // 0xFFFFFFFF * 0xFFFFFFFF mod 2^32 = 1
  // (since (2^32 - 1)^2 = 2^64 - 2^33 + 1, mod 2^32 = 1).
  // Use unsigned long to avoid the 0xFFFFFFFFL parser promoting
  // to signed.
  CHECK(l("long main(void) { unsigned long x = 0xFFFFFFFFUL; return (long)(x * x); }") == 1);
}

}  // namespace mul32

namespace shifts8 {
// 8-bit asl / asr / lsr exercised via char-typed arithmetic.
// Like TestMul8, the underlying helper is the int-width one
// (asl16/asr16/lsr16) — char operands integer-promote before the
// shift, then the result truncates back to 1 byte at the
// char-typed return.
auto i(const auto& src) {
  auto res = run_c_program(src);
  return signed_byte(res.a);
}

TEST_CASE("runtime_helpers: shifts8: asl count zero") { CHECK(i("char main(void) { return 42 << 0; }") == 42); }

TEST_CASE("runtime_helpers: shifts8: asl basic") { CHECK(i("char main(void) { return 3 << 2; }") == 12); }

TEST_CASE("runtime_helpers: shifts8: asl into sign bit") {
  // 1 << 7 = $80 = -128 (signed view).
  CHECK(i("char main(void) { return 1 << 7; }") == -128);
}

TEST_CASE("runtime_helpers: shifts8: asl count at width zeros out") {
  // The shift happens at int (16-bit) width — 1 << 8 = 256.
  // Truncated to char: 256 mod 256 = 0.
  CHECK(i("char main(void) { return 1 << 8; }") == 0);
}

TEST_CASE("runtime_helpers: shifts8: lsr via unsigned char") {
  // `>>` on `unsigned char` routes through unsigned int.
  CHECK(i("char main(void) { unsigned char x = 100; return (char)(x >> 2); }") == 25);
}

TEST_CASE("runtime_helpers: shifts8: asr negative sign fill") {
  // (-100) >> 1 = -50 (sign-extended).
  CHECK(i("char main(void) { return (signed char)(-100) >> 1; }") == -50);
}

TEST_CASE("runtime_helpers: shifts8: asr negative saturates to minus one") {
  // (-1) >> any → -1 (the sign keeps filling).
  CHECK(i("char main(void) { return (signed char)(-1) >> 4; }") == -1);
  CHECK(i("char main(void) { return (signed char)(-1) >> 7; }") == -1);
}

}  // namespace shifts8

namespace shifts16 {
// 16-bit asl / asr / lsr — multi-byte ROR/ROL chain. Exercised
// via int-typed arithmetic now that Int is 2 bytes.
auto i(const auto& src) {
  auto res = run_c_program(src);
  auto v = res.return_int();
  return v & 0x8000 ? v - 0x10000 : v;
}

TEST_CASE("runtime_helpers: shifts16: asl carry chain") {
  // 1 << 8 = 0x100 — exercises ASL low → ROL high.
  CHECK(i("int main(void) { return 1 << 8; }") == 256);
}

TEST_CASE("runtime_helpers: shifts16: asl into sign bit") {
  // 1 << 15 = 0x8000 = -32768 (signed view).
  CHECK(i("int main(void) { return 1 << 15; }") == -32768);
}

TEST_CASE("runtime_helpers: shifts16: asl count at width") { CHECK(i("int main(void) { return 1 << 16; }") == 0); }

TEST_CASE("runtime_helpers: shifts16: lsr") {
  CHECK(i("int main(void) { unsigned int x = 32000U; return (int)(x >> 4); }") == 2000);
}

TEST_CASE("runtime_helpers: shifts16: asr negative") { CHECK(i("int main(void) { return (-32000) >> 4; }") == -2000); }

TEST_CASE("runtime_helpers: shifts16: asr positive") { CHECK(i("int main(void) { return 0x7FFF >> 14; }") == 1); }

}  // namespace shifts16

namespace shifts32 {
// 32-bit asl / asr / lsr — 4-byte ROR/ROL chain. Exercised via
// long-typed arithmetic now that Long is 4 bytes.
auto l(const auto& src) {
  auto res = run_c_program(src);
  const auto v = static_cast<std::int64_t>(read_long(res.memory));
  return v & 0x80000000LL ? v - 0x100000000LL : v;
}

TEST_CASE("runtime_helpers: shifts32: asl into high byte") {
  CHECK(l("long main(void) { return 1L << 16; }") == 65536);
}

TEST_CASE("runtime_helpers: shifts32: asl into sign bit") {
  // 1 << 31 = 0x80000000 = -2147483648.
  CHECK(l("long main(void) { return 1L << 31; }") == -2'147'483'648LL);
}

TEST_CASE("runtime_helpers: shifts32: lsr extracts high half") {
  // 0x12345678 >> 16 = 0x1234.
  CHECK(l("long main(void) { long x = 0x12345678L; return x >> 16; }") == 0x1234);
}

TEST_CASE("runtime_helpers: shifts32: asr negative sign fill") {
  CHECK(l("long main(void) { return ((long)(-1)) >> 8; }") == -1);
}

TEST_CASE("runtime_helpers: shifts32: asr large count saturates") {
  // (-1) >> 31 still = -1 (sign keeps filling all 32 bits).
  CHECK(l("long main(void) { return ((long)(-1)) >> 31; }") == -1);
}

}  // namespace shifts32

namespace udivmod8 {
// Unsigned 8-bit divmod via char-typed arithmetic. The
// underlying helper is now udivmod16 (char promotes to int before
// division), but the result is truncated to 1 byte at the
// char-typed return — so 8-bit semantics are still observable
// through this test class.
void check_case(const auto& src, const auto& expected) {
  auto res = run_c_program(src);
  auto got = signed_byte(res.a);
  CHECK_MESSAGE(got == expected, src);
}

TEST_CASE("runtime_helpers: udivmod8: zero dividend") {
  check_case("char main(void) { unsigned char x = 0; unsigned char y = 5; return (char)(x / y); }", 0);
}

TEST_CASE("runtime_helpers: udivmod8: dividend smaller than divisor") {
  check_case("char main(void) { unsigned char x = 3; unsigned char y = 5; return (char)(x / y); }", 0);
  check_case("char main(void) { unsigned char x = 3; unsigned char y = 5; return (char)(x % y); }", 3);
}

TEST_CASE("runtime_helpers: udivmod8: quotient one") {
  check_case("char main(void) { unsigned char x = 5; unsigned char y = 5; return (char)(x / y); }", 1);
  check_case("char main(void) { unsigned char x = 5; unsigned char y = 5; return (char)(x % y); }", 0);
}

TEST_CASE("runtime_helpers: udivmod8: high byte dividend") {
  // 200 / 7 = 28 rem 4.
  check_case("char main(void) { unsigned char x = 200; unsigned char y = 7; return (char)(x / y); }", 28);
  check_case("char main(void) { unsigned char x = 200; unsigned char y = 7; return (char)(x % y); }", 4);
}

TEST_CASE("runtime_helpers: udivmod8: max unsigned") {
  // 255 / 1 = 255 rem 0. As signed char that's -1.
  check_case("char main(void) { unsigned char x = 255; unsigned char y = 1; return (char)(x / y); }", -1);
}

}  // namespace udivmod8

namespace sdivmod8 {
// Signed 8-bit divmod via signed-char arithmetic. C99 trunc-
// toward-zero — the underlying helper is now sdivmod16 since
// char operands promote to int before division, but truncating
// the result to a signed char still exhibits 8-bit edge cases
// like the -128 / -1 wrap.
void check_case(const auto& src, const auto& expected) {
  auto res = run_c_program(src);
  auto got = signed_byte(res.a);
  CHECK_MESSAGE(got == expected, src);
}

TEST_CASE("runtime_helpers: sdivmod8: pos pos") {
  check_case("char main(void) { return (signed char)(12 / 5); }", 2);
  check_case("char main(void) { return (signed char)(12 % 5); }", 2);
}

TEST_CASE("runtime_helpers: sdivmod8: neg pos div truncates toward zero") {
  // C99: -12 / 5 = -2 (not -3 as Python's // would give).
  check_case("char main(void) { return (signed char)((-12) / 5); }", -2);
  // Remainder follows dividend sign: -12 % 5 = -2.
  check_case("char main(void) { return (signed char)((-12) % 5); }", -2);
}

TEST_CASE("runtime_helpers: sdivmod8: pos neg") {
  check_case("char main(void) { return (signed char)(12 / (-5)); }", -2);
  // Remainder follows dividend sign: 12 % -5 = 2.
  check_case("char main(void) { return (signed char)(12 % (-5)); }", 2);
}

TEST_CASE("runtime_helpers: sdivmod8: neg neg") {
  check_case("char main(void) { return (signed char)((-12) / (-5)); }", 2);
  check_case("char main(void) { return (signed char)((-12) % (-5)); }", -2);
}

TEST_CASE("runtime_helpers: sdivmod8: zero dividend") {
  check_case("char main(void) { return (signed char)(0 / 5); }", 0);
  check_case("char main(void) { return (signed char)(0 / (-5)); }", 0);
  check_case("char main(void) { return (signed char)(0 % 5); }", 0);
}

TEST_CASE("runtime_helpers: sdivmod8: one divisor") {
  check_case("char main(void) { return (signed char)(42 / 1); }", 42);
  check_case("char main(void) { return (signed char)((-42) / 1); }", -42);
  check_case("char main(void) { return (signed char)(42 / (-1)); }", -42);
  check_case("char main(void) { return (signed char)((-42) / (-1)); }", 42);
}

TEST_CASE("runtime_helpers: sdivmod8: int min neg one") {
  // SCHAR_MIN / -1 overflows in C (UB). With signed char, -128
  // / -1 = 128 which doesn't fit; the result wraps to -128 in
  // two's complement. The 8-bit wrap is observable through the
  // char-typed truncation even though the underlying division
  // happens at int width.
  check_case("char main(void) { return (signed char)((-128) / (-1)); }", -128);
}

TEST_CASE("runtime_helpers: sdivmod8: dividend equals divisor") {
  check_case("char main(void) { return (signed char)(5 / 5); }", 1);
  check_case("char main(void) { return (signed char)((-5) / (-5)); }", 1);
  check_case("char main(void) { return (signed char)((-5) / 5); }", -1);
}

TEST_CASE("runtime_helpers: sdivmod8: remainder round trip") {
  // Spot-check (a / b) * b + (a % b) == a for various inputs
  // (the C99 defining identity for / and %). Bytes fit in
  // signed char (-128..127) so 8-bit truncation doesn't lose
  // information.
  for (const auto& a : {-100, -50, -1, 0, 1, 7, 50, 100}) {
    for (const auto& b : {-7, -3, -1, 1, 3, 7}) {
      auto expected = a;
      // the identity
      auto src = cat("char main(void) { signed char a = ", a, "; signed char b = ", b,
                     "; return (signed char)((a / b) * b + (a % b)); }");
      check_case(src, expected);
    }
  }
}

}  // namespace sdivmod8

namespace udivmod16 {
// 16-bit unsigned divmod via shift-and-subtract — exercised via
// `unsigned int` arithmetic now that Int is 2 bytes.
auto i(const auto& src) {
  auto res = run_c_program(src);
  auto v = res.return_int();
  return v & 0x8000 ? v - 0x10000 : v;
}

TEST_CASE("runtime_helpers: udivmod16: basic") {
  CHECK(i("int main(void) { unsigned int x = 1000U; unsigned int y = 7U; return (int)(x / y); }") == 142);
  CHECK(i("int main(void) { unsigned int x = 1000U; unsigned int y = 7U; return (int)(x % y); }") == 6);
}

TEST_CASE("runtime_helpers: udivmod16: zero dividend") {
  CHECK(i("int main(void) { unsigned int x = 0U; unsigned int y = 12345U; return (int)(x / y); }") == 0);
}

TEST_CASE("runtime_helpers: udivmod16: dividend smaller than divisor") {
  CHECK(i("int main(void) { unsigned int x = 100U; unsigned int y = 1000U; return (int)(x / y); }") == 0);
  CHECK(i("int main(void) { unsigned int x = 100U; unsigned int y = 1000U; return (int)(x % y); }") == 100);
}

}  // namespace udivmod16

namespace sdivmod16 {
// 16-bit signed divmod with C99 trunc-toward-zero — exercised
// via int-typed arithmetic now that Int is 2 bytes.
auto i(const auto& src) {
  auto res = run_c_program(src);
  auto v = res.return_int();
  return v & 0x8000 ? v - 0x10000 : v;
}

TEST_CASE("runtime_helpers: sdivmod16: pos pos") {
  CHECK(i("int main(void) { return 1000 / 7; }") == 142);
  CHECK(i("int main(void) { return 1000 % 7; }") == 6);
}

TEST_CASE("runtime_helpers: sdivmod16: neg pos") {
  CHECK(i("int main(void) { return (-1000) / 7; }") == -142);
  CHECK(i("int main(void) { return (-1000) % 7; }") == -6);
}

TEST_CASE("runtime_helpers: sdivmod16: pos neg") {
  CHECK(i("int main(void) { return 1000 / (-7); }") == -142);
  CHECK(i("int main(void) { return 1000 % (-7); }") == 6);
}

TEST_CASE("runtime_helpers: sdivmod16: neg neg") {
  CHECK(i("int main(void) { return (-1000) / (-7); }") == 142);
  CHECK(i("int main(void) { return (-1000) % (-7); }") == -6);
}

TEST_CASE("runtime_helpers: sdivmod16: int min neg one") {
  // Int INT16_MIN / -1 overflows in C; bit pattern wraps to
  // INT16_MIN as the result of the negate (since -(-32768) =
  // 32768 mod 65536 = -32768 in two's complement).
  CHECK(i("int main(void) { return (-32768) / (-1); }") == -32768);
}

TEST_CASE("runtime_helpers: sdivmod16: round trip identity") {
  // (a / b) * b + (a % b) == a for representative values.
  for (const auto& a : {-30000, -1000, -7, 0, 7, 1000, 30000}) {
    for (const auto& b : {-100, -7, -1, 1, 7, 100}) {
      auto src = cat("int main(void) { int a = ", a, "; int b = ", b, "; return (a / b) * b + (a % b); }");
      CHECK_MESSAGE(i(src) == a, src);
    }
  }
}

}  // namespace sdivmod16

namespace udivmod32 {
// 32-bit unsigned divmod — exercised via `unsigned long`
// arithmetic now that Long is 4 bytes.
auto l(const auto& src) {
  auto res = run_c_program(src);
  const auto v = static_cast<std::int64_t>(read_long(res.memory));
  return v & 0x80000000LL ? v - 0x100000000LL : v;
}

TEST_CASE("runtime_helpers: udivmod32: basic") {
  CHECK(l("long main(void) { unsigned long x = 1000000UL; unsigned long y = 7UL; return (long)(x / y); }") == 142857);
}

TEST_CASE("runtime_helpers: udivmod32: large unsigned") {
  CHECK(l("long main(void) { unsigned long x = 0xFFFFFFFFUL; unsigned long y = 0x10000UL; return (long)(x / y); }") ==
        0xFFFF);
}

}  // namespace udivmod32

namespace sdivmod32 {
// 32-bit signed divmod — exercised via `long` arithmetic now
// that Long is 4 bytes.
auto l(const auto& src) {
  auto res = run_c_program(src);
  const auto v = static_cast<std::int64_t>(read_long(res.memory));
  return v & 0x80000000LL ? v - 0x100000000LL : v;
}

TEST_CASE("runtime_helpers: sdivmod32: pos pos") {
  CHECK(l("long main(void) { return 1000000L / 7L; }") == 142857);
  CHECK(l("long main(void) { return 1000000L % 7L; }") == 1);
}

TEST_CASE("runtime_helpers: sdivmod32: neg pos") {
  CHECK(l("long main(void) { return (-1000000L) / 7L; }") == -142857);
  CHECK(l("long main(void) { return (-1000000L) % 7L; }") == -1);
}

TEST_CASE("runtime_helpers: sdivmod32: pos neg") {
  CHECK(l("long main(void) { return 1000000L / (-7L); }") == -142857);
}

TEST_CASE("runtime_helpers: sdivmod32: neg neg") {
  CHECK(l("long main(void) { return (-1000000L) / (-7L); }") == 142857);
}

TEST_CASE("runtime_helpers: sdivmod32: zero dividend") { CHECK(l("long main(void) { return 0L / 12345L; }") == 0); }

}  // namespace sdivmod32

}  // namespace
}  // namespace c6502::suite
