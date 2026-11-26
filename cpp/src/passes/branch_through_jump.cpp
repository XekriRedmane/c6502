#include "passes/branch_through_jump.h"

#include <cstddef>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_terminator(const ir::Type_instruction& instr) {
  return isa<ir::Jump, ir::Ret, ir::Return>(instr);
}

bool is_noop_mov(const ir::Type_instruction& instr) {
  const auto* mov = as<ir::Mov>(instr);
  if (mov == nullptr || mov->is_volatile) {
    return false;
  }
  return mov->src == mov->dst;
}

template <class Fn>
void for_each_label_ref(const ir::Type_instruction& instr, Fn&& fn) {
  if (const auto* branch = as<ir::Branch>(instr)) {
    fn(branch->target);
  } else if (const auto* jump = as<ir::Jump>(instr)) {
    fn(jump->target);
  } else if (const auto* phi = as<ir::Phi>(instr)) {
    for (const auto& arg : phi->args) {
      fn(std::get<ir::AsmPhiArg>(arg).pred_label);
    }
  }
}

void rewrite_function(ir::Function& fn) {
  std::vector<ir::Type_instruction>& instrs = fn.instructions;
  // A 6502 branch is relative and can't leave the function, so only
  // the function's own labels can be a `Branch`'s target.
  std::unordered_set<std::string> in_fn_labels;
  for (const auto& instr : instrs) {
    if (const auto* label = as<ir::Label>(instr)) {
      in_fn_labels.insert(label->name);
    }
  }
  std::unordered_map<std::string, std::string> label_to_jump_target;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto* label = as<ir::Label>(instrs[i]);
    if (label == nullptr) {
      continue;
    }
    std::size_t j = i + 1;
    while (j < instrs.size() &&
           (isa<ir::Label>(instrs[j]) || is_noop_mov(instrs[j]))) {
      j += 1;
    }
    if (j >= instrs.size()) {
      continue;
    }
    const auto* next = as<ir::Jump>(instrs[j]);
    if (next != nullptr && in_fn_labels.contains(next->target)) {
      label_to_jump_target[label->name] = next->target;
    }
  }
  if (label_to_jump_target.empty()) {
    return;
  }
  const auto resolve = [&](const std::string& target) -> std::string {
    std::unordered_set<std::string> seen{target};
    std::string cur = target;
    while (true) {
      const auto it = label_to_jump_target.find(cur);
      if (it == label_to_jump_target.end()) {
        return cur;
      }
      if (seen.contains(it->second)) {
        return target;
      }
      seen.insert(it->second);
      cur = it->second;
    }
  };
  std::unordered_map<std::string, int> ref_count;
  for (const auto& instr : instrs) {
    for_each_label_ref(instr, [&](const std::string& ref) { ++ref_count[ref]; });
  }
  std::vector<ir::Type_instruction> new_instrs;
  new_instrs.reserve(instrs.size());
  std::unordered_map<std::string, int> drop_decremented;
  for (auto& instr : instrs) {
    std::string* target = nullptr;
    if (auto* branch = as<ir::Branch>(instr)) {
      target = &branch->target;
    } else if (auto* jump = as<ir::Jump>(instr)) {
      target = &jump->target;
    }
    if (target != nullptr && label_to_jump_target.contains(*target)) {
      std::string new_target = resolve(*target);
      if (new_target != *target) {
        ++drop_decremented[*target];
        *target = std::move(new_target);
      }
    }
    new_instrs.push_back(std::move(instr));
  }
  std::unordered_set<std::string> fully_drop;
  for (const auto& [name, dec] : drop_decremented) {
    const auto it = ref_count.find(name);
    const int refs = it == ref_count.end() ? 0 : it->second;
    if (refs - dec <= 0) {
      fully_drop.insert(name);
    }
  }
  if (fully_drop.empty()) {
    instrs = std::move(new_instrs);
    return;
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(new_instrs.size());
  std::size_t i = 0;
  while (i < new_instrs.size()) {
    const auto* label = as<ir::Label>(new_instrs[i]);
    if (label != nullptr && fully_drop.contains(label->name) && !out.empty() &&
        is_terminator(out.back())) {
      i += 1;
      while (i < new_instrs.size()) {
        const auto& next = new_instrs[i];
        if (isa<ir::Jump>(next)) {
          i += 1;
          break;
        }
        if (const auto* inner = as<ir::Label>(next)) {
          if (fully_drop.contains(inner->name)) {
            i += 1;
            continue;
          }
          break;
        }
        if (is_noop_mov(next)) {
          i += 1;
          continue;
        }
        break;
      }
      continue;
    }
    out.push_back(std::move(new_instrs[i]));
    i += 1;
  }
  instrs = std::move(out);
}

}  // namespace

asm_ast::Program apply_branch_through_jump(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
