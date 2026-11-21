// Tests of PyIntSet's order. Each expected order is what CPython 3.11.3
// printed for a set built by adding the keys in the order given.
#include <cstdint>
#include <vector>

#include "doctest.h"
#include "py_int_set.h"

namespace {

std::vector<std::int64_t> order_of(const std::vector<std::int64_t>& keys) {
  c6502::PyIntSet s;
  for (const auto k : keys) {
    s.add(k);
  }
  return s.items();
}

}  // namespace

TEST_CASE("PyIntSet iterates in CPython's order") {
  CHECK(order_of({9, 1, 17, 2}) == std::vector<std::int64_t>{9, 2, 1, 17});
  CHECK(order_of({40, 37, 34, 31, 28, 25, 22, 19, 16, 13, 10, 7, 4, 1}) ==
        std::vector<std::int64_t>{1, 34, 4, 37, 7, 40, 10, 13, 16, 19, 22, 25, 28, 31});
  CHECK(order_of({0, 8, 16, 24, 32, 40}) == std::vector<std::int64_t>{0, 32, 8, 40, 16, 24});
  CHECK(order_of({5, 13, 21, 3, 11, 100, 64, 7, 200, 1000, 12, 6}) ==
        std::vector<std::int64_t>{64, 3, 100, 5, 6, 7, 200, 1000, 11, 12, 13, 21});
  CHECK(order_of({183, 271, 14, 238, 127, 26, 80, 57, 190, 240, 126, 194, 278, 52, 293, 127, 6,
                  110, 208, 143, 93}) ==
        std::vector<std::int64_t>{6,   14,  271, 143, 278, 26,  293, 52,  183, 57,
                                  190, 194, 80,  208, 93,  238, 110, 240, 126, 127});
  CHECK(order_of({23, 92, 100, 60, 125, 236, 176, 262, 181, 268, 128, 236, 55, 188, 151, 18, 221,
                  46, 106, 174, 262, 185, 75, 174, 141, 279, 47, 159, 162, 156, 90, 40, 76}) ==
        std::vector<std::int64_t>{128, 262, 268, 141, 18,  23,  151, 279, 156, 159,
                                  162, 40,  46,  174, 176, 47,  181, 55,  185, 60,
                                  188, 75,  76,  90,  92,  221, 100, 106, 236, 125});
}

TEST_CASE("PyIntSet keeps one copy of a key and compares by its keys") {
  c6502::PyIntSet a;
  a.add(3);
  a.add(3);
  a.add(11);
  c6502::PyIntSet b;
  b.add(11);
  b.add(3);
  CHECK(a.size() == 2);
  CHECK(a.contains(11));
  CHECK_FALSE(a.contains(4));
  CHECK(a == b);
}
