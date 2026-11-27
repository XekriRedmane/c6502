// The bundled <stdint.h> (include/stdint.h), from tests/test_stdint_h.py:
// the size of each type, the value of each limit and of each integer
// constant macro, and uses of the types, read from what `main` returns
// in the simulator.
#include <cstdint>
#include <string>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

const std::string kHeader = R"c(#include <stdint.h>
)c";

std::int64_t size_of(const std::string& alias) {
  return run_c_program(kHeader + "int main(void) { return sizeof(" + alias + "); }").return_int_signed();
}

std::int64_t run_int_signed(const std::string& body) {
  return run_c_program(kHeader + "int main(void) { " + body + " }").return_int_signed();
}

std::int64_t run_int_unsigned(const std::string& body) {
  return run_c_program(kHeader + "unsigned int main(void) { " + body + " }").return_int();
}

std::int64_t run_long(const std::string& body) {
  return run_c_program(kHeader + "long main(void) { " + body + " }").return_long_signed();
}

std::uint64_t run_ulong(const std::string& body) {
  return run_c_program(kHeader + "unsigned long main(void) { " + body + " }").return_long();
}

}  // namespace

TEST_CASE("stdint_h sim: the exact-width types' sizes") {
  CHECK(size_of("int8_t") == 1);
  CHECK(size_of("uint8_t") == 1);
  CHECK(size_of("int16_t") == 2);
  CHECK(size_of("uint16_t") == 2);
  CHECK(size_of("int32_t") == 4);
  CHECK(size_of("uint32_t") == 4);
  CHECK(size_of("int64_t") == 8);
  CHECK(size_of("uint64_t") == 8);
}

TEST_CASE("stdint_h sim: the least-width types are the exact-width ones' sizes") {
  CHECK(size_of("int_least8_t") == 1);
  CHECK(size_of("uint_least8_t") == 1);
  CHECK(size_of("int_least16_t") == 2);
  CHECK(size_of("uint_least16_t") == 2);
  CHECK(size_of("int_least32_t") == 4);
  CHECK(size_of("uint_least32_t") == 4);
  CHECK(size_of("int_least64_t") == 8);
  CHECK(size_of("uint_least64_t") == 8);
}

TEST_CASE("stdint_h sim: the fast types are the exact-width ones' sizes") {
  CHECK(size_of("int_fast8_t") == 1);
  CHECK(size_of("uint_fast8_t") == 1);
  CHECK(size_of("int_fast16_t") == 2);
  CHECK(size_of("uint_fast16_t") == 2);
  CHECK(size_of("int_fast32_t") == 4);
  CHECK(size_of("uint_fast32_t") == 4);
  CHECK(size_of("int_fast64_t") == 8);
  CHECK(size_of("uint_fast64_t") == 8);
}

TEST_CASE("stdint_h sim: intptr_t and uintptr_t are 2 bytes") {
  // The 6502's addresses are 16 bits.
  CHECK(size_of("intptr_t") == 2);
  CHECK(size_of("uintptr_t") == 2);
}

TEST_CASE("stdint_h sim: intmax_t and uintmax_t are 8 bytes") {
  // The widest integer type c6502 has is `long long`.
  CHECK(size_of("intmax_t") == 8);
  CHECK(size_of("uintmax_t") == 8);
}

TEST_CASE("stdint_h sim: the 8-bit limits") {
  CHECK(run_int_signed("return INT8_MIN;") == -128);
  CHECK(run_int_signed("return INT8_MAX;") == 127);
  CHECK(run_int_signed("return UINT8_MAX;") == 255);
}

TEST_CASE("stdint_h sim: the 16-bit limits") {
  CHECK(run_int_signed("return INT16_MIN;") == -32768);
  CHECK(run_int_signed("return INT16_MAX;") == 32767);
  CHECK(run_int_unsigned("return UINT16_MAX;") == 65535);
}

TEST_CASE("stdint_h sim: the 32-bit limits") {
  CHECK(run_long("return INT32_MIN;") == -0x80000000LL);
  CHECK(run_long("return INT32_MAX;") == 0x7FFFFFFF);
  CHECK(run_ulong("return UINT32_MAX;") == 0xFFFFFFFFu);
}

TEST_CASE("stdint_h sim: the least-width limits are the exact-width ones") {
  CHECK(run_int_signed("return INT_LEAST8_MAX;") == 127);
  CHECK(run_int_signed("return INT_LEAST16_MIN;") == -32768);
  CHECK(run_long("return INT_LEAST32_MAX;") == 0x7FFFFFFF);
  CHECK(run_int_signed("return UINT_LEAST8_MAX;") == 255);
}

TEST_CASE("stdint_h sim: the fast limits are the exact-width ones") {
  CHECK(run_int_signed("return INT_FAST8_MAX;") == 127);
  CHECK(run_int_signed("return INT_FAST16_MIN;") == -32768);
  CHECK(run_long("return INT_FAST32_MAX;") == 0x7FFFFFFF);
}

TEST_CASE("stdint_h sim: the intptr_t limits") {
  CHECK(run_int_signed("return INTPTR_MIN;") == -32768);
  CHECK(run_int_signed("return INTPTR_MAX;") == 32767);
  CHECK(run_int_unsigned("return UINTPTR_MAX;") == 65535);
}

TEST_CASE("stdint_h sim: the ptrdiff_t limits") {
  // The subtraction of two pointers gives an `int` (2 bytes signed),
  // which <stddef.h> names `ptrdiff_t`.
  CHECK(run_int_signed("return PTRDIFF_MIN;") == -32768);
  CHECK(run_int_signed("return PTRDIFF_MAX;") == 32767);
}

TEST_CASE("stdint_h sim: the size_t and wchar_t limits") {
  // `sizeof` gives an `unsigned int`, and <stddef.h> makes `wchar_t` an
  // `int`.
  CHECK(run_int_unsigned("return SIZE_MAX;") == 0xFFFF);
  CHECK(run_int_signed("return WCHAR_MIN;") == -32768);
  CHECK(run_int_signed("return WCHAR_MAX;") == 32767);
}

TEST_CASE("stdint_h sim: INT8_C") {
  // The constant's type is at least int_least8_t, which is `signed
  // char` and promotes to `int`.
  CHECK(run_c_program(R"c(#include <stdint.h>
int main(void) { return INT8_C(42); })c").return_int_signed() == 42);
}

TEST_CASE("stdint_h sim: INT16_C") {
  CHECK(run_c_program(R"c(#include <stdint.h>
int main(void) { return INT16_C(-12345); })c").return_int_signed() == -12345);
}

TEST_CASE("stdint_h sim: INT32_C") {
  // 100000 doesn't fit in a 16-bit `int`, so the macro adds the L
  // suffix.
  CHECK(run_c_program(R"c(#include <stdint.h>
long main(void) { return INT32_C(100000); })c").return_long_signed() == 100000);
}

TEST_CASE("stdint_h sim: UINT32_C") {
  CHECK(run_c_program(R"c(#include <stdint.h>
unsigned long main(void) { return UINT32_C(4000000000); })c").return_long() == 4000000000u);
}

TEST_CASE("stdint_h sim: UINT16_C") {
  CHECK(run_c_program(R"c(#include <stdint.h>
unsigned int main(void) { return UINT16_C(50000); })c").return_int() == 50000);
}

TEST_CASE("stdint_h sim: int8_t arithmetic") {
  // int8_t, which is `signed char`, promotes to `int` for `+`.
  CHECK(run_c_program(R"c(#include <stdint.h>
int main(void) {
    int8_t a = 50;
    int8_t b = 70;
    return a + b;
}
)c").return_int_signed() == 120);
}

TEST_CASE("stdint_h sim: uint16_t arithmetic") {
  CHECK(run_c_program(R"c(#include <stdint.h>
unsigned int main(void) {
    uint16_t a = 30000;
    uint16_t b = 25000;
    return (unsigned int)(a + b);
}
)c").return_int() == 55000);
}

TEST_CASE("stdint_h sim: int32_t arithmetic") {
  CHECK(run_c_program(R"c(#include <stdint.h>
long main(void) {
    int32_t a = 100000L;
    int32_t b = -50000L;
    return a + b;
}
)c").return_long_signed() == 50000);
}

TEST_CASE("stdint_h sim: a cast to int8_t") {
  // 300 & 0xFF is 44, read as signed.
  CHECK(run_c_program(R"c(#include <stdint.h>
int main(void) {
    int x = 300;
    return (int8_t)x;
}
)c").return_int_signed() == 44);
}

TEST_CASE("stdint_h sim: a parameter and a return of int16_t") {
  CHECK(run_c_program(R"c(#include <stdint.h>
int16_t add(int16_t a, int16_t b) { return a + b; }
int main(void) { return add(1234, 4321); }
)c").return_int_signed() == 5555);
}

TEST_CASE("stdint_h sim: the header included twice") {
  // The header's guard and pcpp's automatic `#pragma once` keep the
  // second inclusion from redefining anything.
  CHECK(run_c_program(R"c(#include <stdint.h>
#include <stdint.h>
int main(void) { return INT8_MAX; }
)c").return_int_signed() == 127);
}

}  // namespace c6502::suite
