#include "passes/optimization/pool.h"

#include <cstdio>
#include <stdexcept>
#include <string>

namespace c6502::passes {

bool Range::contains(asdl::Int x) const {
  if (step > 0) {
    return x >= start && x < stop && asdl::floor_mod(x - start, step) == 0;
  }
  return x <= start && x > stop && asdl::floor_mod(start - x, -step) == 0;
}

namespace {

std::string hex2(asdl::Int v) {
  char buf[40];
  std::snprintf(buf, sizeof buf, "0x%02llX", static_cast<unsigned long long>(v));
  return buf;
}

}  // namespace

void Pool::validate() const {
  if (start < 0 || start > 0xFF) {
    throw std::invalid_argument("Pool.start out of range [0, 0xFF]: " + hex2(start));
  }
  if (asdl::floor_mod(start, 2) != 0) {
    throw std::invalid_argument("Pool.start must be even: " + hex2(start));
  }
}

asdl::Int Pool::mid() const { return 0x80 + asdl::floor_div(start, 2); }

Range Pool::caller_saved() const { return Range{.start = start, .stop = mid()}; }

Range Pool::callee_saved() const { return Range{.start = mid(), .stop = 0x100}; }

}  // namespace c6502::passes
