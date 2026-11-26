#include "passes/call_edges.h"

namespace c6502::passes {

namespace tac = ::c6502::tac_ast;
using ::c6502::asdl::as;

bool abandons_caller(const tac::Type_instruction& instr) {
  const auto* call = as<tac::FunctionCall>(instr);
  return call != nullptr && call->noreturn && call->args.empty();
}

std::vector<const tac::FunctionCall*> graph_calls(const tac::Function& fn) {
  std::vector<const tac::FunctionCall*> out;
  for (const auto& instr : fn.instructions) {
    if (const auto* call = as<tac::FunctionCall>(instr)) {
      if (!abandons_caller(instr)) {
        out.push_back(call);
      }
    }
  }
  return out;
}

}  // namespace c6502::passes
