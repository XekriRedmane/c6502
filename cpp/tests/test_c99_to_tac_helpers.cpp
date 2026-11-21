// Checks of the parts of bit_precise.py that c99_to_tac reads, and of
// c99_to_tac's fold of a cast of a floating constant, against the values
// the Python functions give. tests/test_fp_arith.cpp checks fp_arith and
// builtin_functions, which c99_to_tac reads too.
#include <utility>

#include "bit_precise.h"
#include "c99_to_tac.h"
#include "c99_to_tac_translator.h"
#include "doctest.h"

using c6502::asdl::Int;

TEST_CASE("c99_to_tac: casts of floating constants fold as Python's do") {
  namespace c99 = c6502::c99_ast;
  namespace tac = c6502::tac_ast;
  using c6502::c99_to_tac::fold_fp_cast_constant;
  const auto value = [](const tac::Type_const& c) { return *c6502::bit_precise::raw_value(c); };
  const auto dbl = [](long long bits) { return tac::Type_const{tac::ConstDouble{.bits = Int(bits)}}; };
  const c99::Type_data_type ulong = c99::ULong{};
  const c99::Type_data_type uchar = c99::UChar{};
  const c99::Type_data_type int_t = c99::Int{};
  const c99::Type_data_type ptr = c99::Pointer{.referenced_type = c99::Int{}};
  // (unsigned long)200e300 once gave a ConstULong of 1000 bits. The
  // value is a multiple of 2^32, so it's 0 in the type's range.
  CHECK(value(fold_fp_cast_constant(ulong, dbl(0x7EB2AA4F4A405BE2ll))) == 0);
  // -1.5 truncates to -1, which wraps in an unsigned type.
  CHECK(value(fold_fp_cast_constant(ulong, dbl(static_cast<long long>(0xBFF8000000000000ull)))) ==
        4294967295);
  CHECK(value(fold_fp_cast_constant(uchar, dbl(static_cast<long long>(0xBFF8000000000000ull)))) == 255);
  CHECK(value(fold_fp_cast_constant(int_t, dbl(0x4202A05F20000000ll))) == -7168);
  CHECK(value(fold_fp_cast_constant(ptr, dbl(static_cast<long long>(0xC202A05F20000000ull)))) == 7168);
  CHECK(value(fold_fp_cast_constant(int_t, dbl(0x7FF8000000000000ll))) == 0);
  CHECK(std::get<tac::ConstFloat>(fold_fp_cast_constant(c99::Float{}, dbl(0x4202A05F20000000ll))).bits ==
        1343554297);
  CHECK(value(fold_fp_cast_constant(int_t, tac::ConstFloat{.bits = 0x4079999A})) == 3);
  // An unsigned source is masked to the low byte of its class, as
  // Python's fold does, and a bit-precise source is read at its width.
  CHECK(std::get<tac::ConstFloat>(fold_fp_cast_constant(c99::Float{}, tac::ConstUInt{.value = -1})).bits ==
        1132396544);
  CHECK(std::get<tac::ConstFloat>(fold_fp_cast_constant(c99::Float{}, tac::ConstInt24{.value = 0x800000}))
            .bits == Int(3405774848u));
}

TEST_CASE("bit_precise: the storage type and range of a bit-precise type") {
  namespace c99 = c6502::c99_ast;
  namespace bp = c6502::bit_precise;
  CHECK(bp::storage_type(c99::BitInt{.width = 12, .is_signed = true}) ==
        c99::Type_data_type{c99::Int{}});
  CHECK(bp::storage_type(c99::BitInt{.width = 20, .is_signed = false}) ==
        c99::Type_data_type{c99::BitInt{.width = 24, .is_signed = false}});
  CHECK(bp::has_padding(c99::BitInt{.width = 12}));
  CHECK_FALSE(bp::has_padding(c99::BitInt{.width = 16}));
  CHECK(bp::value_range(5, true) == std::pair<Int, Int>{-16, 15});
  CHECK(bp::value_range(5, false) == std::pair<Int, Int>{0, 31});
}
