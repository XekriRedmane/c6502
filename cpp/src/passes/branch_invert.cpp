#include "passes/branch_invert.h"

#include <cstddef>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "passes/long_branches.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;

void rewrite_function(ir::Function& fn) {
  std::vector<ir::Type_instruction>& instrs = fn.instructions;
  std::unordered_set<std::string> local_labels;
  for (const auto& ins : instrs) {
    if (const auto* label = as<ir::Label>(ins)) {
      local_labels.insert(label->name);
    }
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  std::size_t i = 0;
  while (i + 2 < instrs.size()) {
    const auto* a = as<ir::Branch>(instrs[i]);
    const auto* b = as<ir::Jump>(instrs[i + 1]);
    const auto* c = as<ir::Label>(instrs[i + 2]);
    if (a != nullptr && b != nullptr && c != nullptr && a->target == c->name &&
        local_labels.contains(b->target)) {
      out.push_back(ir::Branch{.cond = long_branches::invert(a->cond),
                               .target = b->target});
      out.push_back(std::move(instrs[i + 2]));
      i += 3;
      continue;
    }
    out.push_back(std::move(instrs[i]));
    i += 1;
  }
  while (i < instrs.size()) {
    out.push_back(std::move(instrs[i]));
    i += 1;
  }
  instrs = std::move(out);
}

}  // namespace

asm_ast::Program apply_branch_invert(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
