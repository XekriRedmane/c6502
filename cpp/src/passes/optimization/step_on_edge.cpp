#include "passes/optimization/step_on_edge.h"

#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <string_view>

#include "asdl/runtime.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using framework::Instrs;
using Counts = std::map<std::string, std::size_t, std::less<>>;

std::size_t count_of(const Counts& counts, std::string_view name) {
  const auto it = counts.find(name);
  return it == counts.end() ? 0 : it->second;
}

bool writes(const ir::Type_instruction& instr, std::string_view name) {
  for (const auto d : defs_in(instr)) {
    if (d == name) {
      return true;
    }
  }
  return false;
}

std::string_view variable_of(std::string_view ssa_name) {
  const auto dot = ssa_name.rfind('.');
  if (dot == std::string_view::npos || dot + 1 == ssa_name.size()) {
    return ssa_name;
  }
  for (const char c : ssa_name.substr(dot + 1)) {
    if (c < '0' || c > '9') {
      return ssa_name;
    }
  }
  return ssa_name.substr(0, dot);
}

bool same_variable(const ir::Type_val& v, std::string_view ssa_name) {
  const auto* var = std::get_if<ir::Var>(&v);
  return var != nullptr && variable_of(var->name) == variable_of(ssa_name);
}

bool conditional_jump(const ir::Type_instruction& ins) {
  return asdl::isa<ir::JumpIfTrue, ir::JumpIfFalse, ir::JumpIfCmp, ir::JumpIfMasked>(ins);
}

bool ends_run(const ir::Type_instruction& ins) {
  return asdl::isa<ir::Label, ir::Jump, ir::Ret>(ins) || conditional_jump(ins);
}

std::optional<std::string> jump_target(const ir::Type_instruction& ins) {
  switch (ir::kind(ins)) {
    case ir::Kind_instruction::Jump:
      return std::get<ir::Jump>(ins).target;
    case ir::Kind_instruction::JumpIfTrue:
      return std::get<ir::JumpIfTrue>(ins).target;
    case ir::Kind_instruction::JumpIfFalse:
      return std::get<ir::JumpIfFalse>(ins).target;
    case ir::Kind_instruction::JumpIfCmp:
      return std::get<ir::JumpIfCmp>(ins).target;
    case ir::Kind_instruction::JumpIfMasked:
      return std::get<ir::JumpIfMasked>(ins).target;
    default:
      return std::nullopt;
  }
}

std::optional<std::size_t> copy_before_write(const Instrs& instrs, std::size_t start,
                                             std::string_view x, std::string_view t) {
  for (std::size_t k = start; k < instrs.size(); ++k) {
    const auto& ins = instrs[k];
    if (const auto* c = std::get_if<ir::Copy>(&ins)) {
      if (const auto* v = std::get_if<ir::Var>(&c->src); v != nullptr && v->name == t) {
        return k;
      }
    }
    if (ends_run(ins) || writes(ins, x)) {
      return std::nullopt;
    }
  }
  return std::nullopt;
}

}  // namespace

ir::Function move_steps_onto_edges(ir::Function fn, const NameSet& ssa_names) {
  Instrs instrs = fn.instructions;
  Counts uses;
  Counts defs;
  for (const auto& ins : instrs) {
    for (const auto u : uses_in(ins)) {
      ++uses[std::string(u)];
    }
    for (const auto d : defs_in(ins)) {
      ++defs[std::string(d)];
    }
  }
  Counts label_refs;
  std::map<std::string, std::size_t, std::less<>> label_at;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (const auto* l = std::get_if<ir::Label>(&instrs[k])) {
      label_at.insert_or_assign(l->name, k);
    }
    if (const auto target = jump_target(instrs[k])) {
      ++label_refs[*target];
    }
  }
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    const auto* step = std::get_if<ir::Binary>(&instrs[i]);
    if (step == nullptr ||
        !asdl::isa<ir::Add, ir::Subtract, ir::BitwiseAnd, ir::BitwiseOr, ir::BitwiseXor>(
            step->op) ||
        !std::holds_alternative<ir::Var>(step->src1) ||
        !std::holds_alternative<ir::Constant>(step->src2) ||
        !std::holds_alternative<ir::Var>(step->dst)) {
      ++i;
      continue;
    }
    const std::string x = std::get<ir::Var>(step->src1).name;
    const std::string t = std::get<ir::Var>(step->dst).name;
    if (x == t || !ssa_names.contains(x) || !ssa_names.contains(t) || count_of(defs, t) != 1 ||
        count_of(uses, t) != 1) {
      ++i;
      continue;
    }
    std::size_t j = i + 1;
    while (j < instrs.size() && !ends_run(instrs[j])) {
      if (writes(instrs[j], x)) {
        break;
      }
      ++j;
    }
    if (j >= instrs.size() || asdl::isa<ir::Label, ir::Ret>(instrs[j]) || writes(instrs[j], x) ||
        !(std::holds_alternative<ir::Jump>(instrs[j]) || conditional_jump(instrs[j]))) {
      ++i;
      continue;
    }
    const auto& jump = instrs[j];
    std::optional<std::size_t> found;
    if (conditional_jump(jump)) {
      found = copy_before_write(instrs, j + 1, x, t);
    }
    if (!found) {
      const std::string target = *jump_target(jump);
      const auto at = label_at.find(target);
      if (at != label_at.end() && count_of(label_refs, target) == 1 && at->second > 0 &&
          at->second <= instrs.size() &&
          asdl::isa<ir::Jump, ir::Ret>(instrs[at->second - 1])) {
        found = copy_before_write(instrs, at->second + 1, x, t);
      }
    }
    if (!found || !same_variable(std::get<ir::Copy>(instrs[*found]).dst, x)) {
      ++i;
      continue;
    }
    const ir::Type_val copy_dst = std::get<ir::Copy>(instrs[*found]).dst;
    ir::Binary moved{.op = step->op, .src1 = step->src1, .src2 = step->src2, .dst = copy_dst};
    instrs[*found] = std::move(moved);
    instrs.erase(instrs.begin() + static_cast<std::ptrdiff_t>(i));
    for (auto& [label, at] : label_at) {
      if (at > i) {
        --at;
      }
    }
    changed = true;
  }
  if (!changed) {
    return fn;
  }
  return framework::with_instructions(fn, std::move(instrs));
}

ir::Function move_steps_onto_edges(ir::Function fn, const framework::PassContext& ctx) {
  if (ctx.ssa_dsts == nullptr) {
    return fn;
  }
  return move_steps_onto_edges(std::move(fn), *ctx.ssa_dsts);
}

}  // namespace c6502::passes
