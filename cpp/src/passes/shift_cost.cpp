#include "passes/shift_cost.h"

namespace c6502::passes {

bool inline_shift_ok(int size, int count, bool signed_right) {
  if (!(0 < count && count < size * 8)) {
    return false;
  }
  const int q = count / 8;
  const int r = count % 8;
  if (r == 7) {
    return true;
  }
  if (signed_right && q != 0) {
    return false;
  }
  if (count == 1 || r == 0) {
    return true;
  }
  const int per_step = size - q + (signed_right ? 2 : 0);
  return r * per_step <= kMaxInlineShiftSteps;
}

}  // namespace c6502::passes
