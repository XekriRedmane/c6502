// Checks of the parts of bit_precise.py that c99_to_tac reads, against
// the values the Python functions give. tests/test_fp_arith.cpp checks
// fp_arith and builtin_functions, which c99_to_tac reads too.
#include <utility>

#include "bit_precise.h"
#include "doctest.h"

using c6502::asdl::Int;

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
