#include "passes/dead_pha_pla.h"

#include <cstddef>
#include <map>
#include <optional>
#include <set>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

std::optional<std::size_t> find_matching_pop(Instrs instrs, std::size_t start) {
  for (std::size_t j = start; j < instrs.size(); ++j) {
    const ir::Type_instruction& instr = instrs[j];
    if (const auto* p = as<ir::Pop>(instr)) {
      if (is_reg_a(p->dst)) {
        return j;
      }
      return std::nullopt;
    }
    if (isa<ir::Push>(instr)) {
      return std::nullopt;
    }
    if (isa<ir::Call, ir::Ret, ir::Return, ir::Label, ir::Jump, ir::Branch>(instr)) {
      return std::nullopt;
    }
    if (reads_a(instr) || kills_a(instr)) {
      return std::nullopt;
    }
  }
  return std::nullopt;
}

// LDA M sets N and Z from M as PLA does, and neither touches C or V.
std::optional<std::size_t> find_reload_pop(Instrs instrs, std::size_t i) {
  if (i == 0) {
    return std::nullopt;
  }
  const auto* load = as<ir::Mov>(instrs[i - 1]);
  if (!(load != nullptr && is_reg_a(load->dst) && !load->is_volatile &&
        isa<ir::Data, ir::ZP>(load->src))) {
    return std::nullopt;
  }
  bool a_written = false;
  for (std::size_t j = i + 1; j < instrs.size(); ++j) {
    const ir::Type_instruction& instr = instrs[j];
    if (const auto* p = as<ir::Pop>(instr)) {
      if (is_reg_a(p->dst) && a_written) {
        return j;
      }
      return std::nullopt;
    }
    const auto* mov = as<ir::Mov>(instr);
    if (!(mov != nullptr && isa<ir::Reg>(mov->dst))) {
      return std::nullopt;
    }
    if (reads_a(instr) && !a_written) {
      return std::nullopt;
    }
    if (is_reg_a(mov->dst)) {
      a_written = true;
    }
  }
  return std::nullopt;
}

void rewrite_function(ir::Function& fn) {
  auto& instrs = fn.instructions;
  const LabelMap labels(instrs);
  std::set<std::size_t> drop;
  std::map<std::size_t, ir::Type_instruction> reload;
  std::size_t i = 0;
  while (i < instrs.size()) {
    const auto* push = as<ir::Push>(instrs[i]);
    if (push != nullptr && is_reg_a(push->src)) {
      const auto j = find_matching_pop(instrs, i + 1);
      if (j && flags_dead_at(instrs, labels, *j + 1)) {
        drop.insert(i);
        drop.insert(*j);
        i = *j + 1;
        continue;
      }
      if (const auto k = find_reload_pop(instrs, i)) {
        drop.insert(i - 1);
        drop.insert(i);
        reload.insert_or_assign(*k, instrs[i - 1]);
        i = *k + 1;
        continue;
      }
    }
    ++i;
  }
  if (drop.empty()) {
    return;
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (drop.count(k) != 0) {
      continue;
    }
    const auto it = reload.find(k);
    out.push_back(it != reload.end() ? it->second : std::move(instrs[k]));
  }
  instrs = std::move(out);
}

}  // namespace

asm_ast::Program apply_dead_pha_pla(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
