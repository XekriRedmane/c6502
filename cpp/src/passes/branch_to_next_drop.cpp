// A `Branch` right after a `Branch` on the same condition, with no
// label between, is never taken, since the first one falls through only
// when its condition is false and a 6502 branch changes no flag.
#include "passes/branch_to_next_drop.h"

#include <cstddef>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;

std::unordered_map<std::string, int> label_ref_counts(
    const std::vector<ir::Type_instruction>& instrs) {
  std::unordered_map<std::string, int> out;
  for (const auto& instr : instrs) {
    if (const auto* jump = as<ir::Jump>(instr)) {
      ++out[jump->target];
    } else if (const auto* branch = as<ir::Branch>(instr)) {
      ++out[branch->target];
    } else if (const auto* phi = as<ir::Phi>(instr)) {
      for (const auto& arg : phi->args) {
        ++out[std::get<ir::AsmPhiArg>(arg).pred_label];
      }
    }
  }
  return out;
}

void rewrite_function(ir::Function& fn) {
  std::vector<ir::Type_instruction>& instrs = fn.instructions;
  const auto ref_counts = label_ref_counts(instrs);
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  const std::size_t n = instrs.size();
  std::size_t i = 0;
  while (i < n) {
    const ir::Type_instruction& a = instrs[i];
    const auto* branch = as<ir::Branch>(a);
    if (branch != nullptr && i + 1 < n) {
      const auto* next = as<ir::Label>(instrs[i + 1]);
      if (next != nullptr && branch->target == next->name) {
        const auto it = ref_counts.find(next->name);
        const int refs = it == ref_counts.end() ? 0 : it->second;
        const bool only_reference = refs <= 1;
        i += only_reference ? 2 : 1;
        continue;
      }
    }
    if (branch != nullptr && !out.empty()) {
      const auto* prev = as<ir::Branch>(out.back());
      if (prev != nullptr && prev->cond.index() == branch->cond.index()) {
        i += 1;
        continue;
      }
    }
    if (const auto* jump = as<ir::Jump>(a)) {
      std::size_t j = i + 1;
      bool found = false;
      while (j < n) {
        const auto* label = as<ir::Label>(instrs[j]);
        if (label == nullptr) {
          break;
        }
        if (label->name == jump->target) {
          found = true;
          break;
        }
        j += 1;
      }
      if (found) {
        i += 1;
        continue;
      }
    }
    out.push_back(std::move(instrs[i]));
    i += 1;
  }
  instrs = std::move(out);
}

}  // namespace

asm_ast::Program apply_branch_to_next_drop(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
