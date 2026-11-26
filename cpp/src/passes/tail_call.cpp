#include "passes/tail_call.h"

#include <cstddef>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

const ir::Return* return_at(const std::vector<ir::Type_instruction>& instrs,
                            const LabelMap& labels, const std::string& target) {
  const long at = labels.find(target);
  if (at < 0) {
    return nullptr;
  }
  std::size_t k = static_cast<std::size_t>(at);
  while (k < instrs.size() && isa<ir::Label>(instrs[k])) {
    k += 1;
  }
  if (k < instrs.size()) {
    return as<ir::Return>(instrs[k]);
  }
  return nullptr;
}

void rewrite_function(ir::Function& fn) {
  const std::vector<ir::Type_instruction>& original = fn.instructions;
  const LabelMap labels(original);
  std::vector<ir::Type_instruction> instrs;
  instrs.reserve(original.size());
  for (const auto& ins : original) {
    if (const auto* jump = as<ir::Jump>(ins)) {
      if (const ir::Return* ret = return_at(original, labels, jump->target)) {
        instrs.push_back(*ret);
        continue;
      }
    }
    instrs.push_back(ins);
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  std::size_t i = 0;
  while (i < instrs.size()) {
    if (i + 1 < instrs.size() && isa<ir::Return>(instrs[i + 1])) {
      if (auto* call = as<ir::Call>(instrs[i])) {
        out.push_back(ir::Jump{.target = std::move(call->name),
                               .reg_args = std::move(call->reg_args)});
        i += 2;
        continue;
      }
    }
    out.push_back(std::move(instrs[i]));
    i += 1;
  }
  fn.instructions = std::move(out);
}

}  // namespace

asm_ast::Program apply_tail_call(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
