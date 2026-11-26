// The labels it adds are numbered across the program, since the sim's
// assembler rejects one name in two functions.
#include "passes/branch_thread.h"

#include <cstddef>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"
#include "passes/label_numbers.h"
#include "passes/long_branches.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

void rewrite_function(ir::Function& fn, asdl::Int& counter) {
  std::vector<ir::Type_instruction>& instrs = fn.instructions;
  const LabelMap labels(instrs);
  std::map<std::size_t, std::string> retarget;
  std::map<std::size_t, std::string> added;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    const auto* ins = as<ir::Branch>(instrs[k]);
    if (ins == nullptr) {
      continue;
    }
    const long at = labels.find(ins->target);
    if (at < 0) {
      continue;
    }
    std::size_t j = static_cast<std::size_t>(at);
    while (j < instrs.size() && isa<ir::Label>(instrs[j])) {
      j += 1;
    }
    if (j >= instrs.size() || j == k) {
      continue;
    }
    const auto* second = as<ir::Branch>(instrs[j]);
    if (second == nullptr) {
      continue;
    }
    if (second->cond.index() == ins->cond.index()) {
      if (second->target != ins->target) {
        retarget[k] = second->target;
      }
    } else if (second->cond == long_branches::invert(ins->cond)) {
      if (j + 1 < instrs.size() && isa<ir::Label>(instrs[j + 1])) {
        retarget[k] = std::get<ir::Label>(instrs[j + 1]).name;
      } else {
        auto it = added.find(j);
        if (it == added.end()) {
          it = added.emplace(j, ".thread@" + asdl::int_to_string(counter)).first;
          counter += 1;
        }
        retarget[k] = it->second;
      }
    }
  }
  if (retarget.empty()) {
    return;
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size() + added.size());
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (const auto it = retarget.find(k); it != retarget.end()) {
      std::get<ir::Branch>(instrs[k]).target = it->second;
    }
    out.push_back(std::move(instrs[k]));
    if (const auto it = added.find(k); it != added.end()) {
      out.push_back(ir::Label{.name = it->second});
    }
  }
  instrs = std::move(out);
}

}  // namespace

asm_ast::Program apply_branch_thread(asm_ast::Program prog) {
  asdl::Int counter = next_label_number(prog, {".thread@"});
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, counter);
    }
  }
  return prog;
}

}  // namespace c6502::passes
