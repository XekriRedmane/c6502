#include "passes/optimization/compare_subtract.h"

#include "asdl/runtime.h"

namespace c6502::passes::compare_subtract {

namespace tac = ::c6502::tac_ast;

std::pair<const tac::Type_val*, const tac::Type_val*> compared_difference(
    const tac::Type_binary_operator& op, const tac::Type_val& src1, const tac::Type_val& src2) {
  if (asdl::isa<tac::LessThan, tac::GreaterOrEqual>(op)) {
    return {&src1, &src2};
  }
  return {&src2, &src1};
}

}  // namespace c6502::passes::compare_subtract
