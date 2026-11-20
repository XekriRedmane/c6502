#include "passes/branch_condition.h"

namespace c6502::passes {

namespace ir = ::c6502::asm_ast;

ir::Type_condition invert_condition(const ir::Type_condition& cond) {
  switch (ir::kind(cond)) {
    case ir::Kind_condition::CC:
      return ir::CS{};
    case ir::Kind_condition::CS:
      return ir::CC{};
    case ir::Kind_condition::EQ:
      return ir::NE{};
    case ir::Kind_condition::NE:
      return ir::EQ{};
    case ir::Kind_condition::MI:
      return ir::PL{};
    case ir::Kind_condition::PL:
      return ir::MI{};
    case ir::Kind_condition::VC:
      return ir::VS{};
    case ir::Kind_condition::VS:
      return ir::VC{};
  }
  return cond;
}

}  // namespace c6502::passes
