#include "passes/carry_inc.h"

#include <algorithm>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"
#include "passes/inc_peephole.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;
using inc_peephole::is_inc_eligible_operand;
using inc_peephole::operands_equal;

constexpr std::string_view kLabelPrefix = ".carry_inc@";

bool is_reg_a_op(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

bool is_imm_zero(const ir::Type_operand& op) {
  const auto* imm = as<ir::Imm>(op);
  return imm != nullptr && imm->value == 0;
}

std::optional<long long> label_number(std::string_view name) {
  if (!name.starts_with(kLabelPrefix)) {
    return std::nullopt;
  }
  const std::string_view digits = name.substr(kLabelPrefix.size());
  if (digits.empty()) {
    return std::nullopt;
  }
  long long n = 0;
  for (char c : digits) {
    if (c < '0' || c > '9') {
      return std::nullopt;
    }
    n = n * 10 + (c - '0');
  }
  return n;
}

struct CarryByte {
  const ir::Type_operand* mem = nullptr;
  bool is_add = false;
};

std::optional<CarryByte> carry_byte(Instrs instrs, std::size_t i) {
  if (i + 3 > instrs.size()) {
    return std::nullopt;
  }
  const auto* load = as<ir::Mov>(instrs[i]);
  if (load == nullptr || load->is_volatile || !is_reg_a_op(load->dst) ||
      !is_inc_eligible_operand(load->src)) {
    return std::nullopt;
  }
  bool is_add = false;
  if (const auto* add = as<ir::Add>(instrs[i + 1])) {
    if (!is_imm_zero(add->src) || !is_reg_a_op(add->dst)) {
      return std::nullopt;
    }
    is_add = true;
  } else if (const auto* sub = as<ir::Sub>(instrs[i + 1])) {
    if (!is_imm_zero(sub->src) || !is_reg_a_op(sub->dst)) {
      return std::nullopt;
    }
  } else {
    return std::nullopt;
  }
  const auto* store = as<ir::Mov>(instrs[i + 2]);
  if (store == nullptr || store->is_volatile || !is_reg_a_op(store->src) ||
      !operands_equal(store->dst, load->src)) {
    return std::nullopt;
  }
  return CarryByte{&load->src, is_add};
}

struct Match {
  std::vector<ir::Type_operand> mems;
  bool is_add = false;
  bool keep_c = false;
};

std::optional<Match> match_run(Instrs instrs, const LabelMap& labels, std::size_t i) {
  const std::optional<CarryByte> first = carry_byte(instrs, i);
  if (!first) {
    return std::nullopt;
  }
  Match m;
  m.mems.push_back(*first->mem);
  m.is_add = first->is_add;
  if (m.is_add) {
    while (true) {
      const std::optional<CarryByte> next = carry_byte(instrs, i + 3 * m.mems.size());
      if (!next || !next->is_add) {
        break;
      }
      m.mems.push_back(*next->mem);
    }
  }
  const std::size_t end = i + 3 * m.mems.size();
  if (!a_dead_at(instrs, labels, end) || !flags_dead_at(instrs, labels, end) ||
      !v_dead_at(instrs, labels, end)) {
    return std::nullopt;
  }
  if (c_dead_at(instrs, labels, end)) {
    m.keep_c = false;
    return m;
  }
  if (m.mems.size() == 1 && isa<ir::Reg>(m.mems[0])) {
    m.keep_c = true;
    return m;
  }
  return std::nullopt;
}

void rewrite_function(ir::Function& fn, long long& counter) {
  const auto& instrs = fn.instructions;
  const LabelMap labels(instrs);
  std::vector<ir::Type_instruction> out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    std::optional<Match> match = match_run(instrs, labels, i);
    if (!match) {
      out.push_back(instrs[i]);
      ++i;
      continue;
    }
    const std::string skip = std::string(kLabelPrefix) + std::to_string(counter);
    ++counter;
    const ir::Type_operand& m0 = match->mems[0];
    if (match->keep_c && match->is_add) {
      out.push_back(ir::Branch{.cond = ir::CC{}, .target = skip});
      out.push_back(ir::Inc{.dst = m0});
      out.push_back(ir::Branch{.cond = ir::EQ{}, .target = skip});
      out.push_back(ir::ClearCarry{});
    } else if (match->keep_c) {
      out.push_back(ir::Branch{.cond = ir::CS{}, .target = skip});
      out.push_back(ir::Compare{.left = m0, .right = ir::Imm{.value = 1}});
      out.push_back(ir::Dec{.dst = m0});
    } else if (match->is_add) {
      out.push_back(ir::Branch{.cond = ir::CC{}, .target = skip});
      for (std::size_t k = 0; k < match->mems.size(); ++k) {
        if (k != 0) {
          out.push_back(ir::Branch{.cond = ir::NE{}, .target = skip});
        }
        out.push_back(ir::Inc{.dst = match->mems[k]});
      }
    } else {
      out.push_back(ir::Branch{.cond = ir::CS{}, .target = skip});
      out.push_back(ir::Dec{.dst = m0});
    }
    out.push_back(ir::Label{.name = skip});
    changed = true;
    i += 3 * match->mems.size();
  }
  if (changed) {
    fn.instructions = std::move(out);
  }
}

}  // namespace

asm_ast::Program apply_carry_inc_peephole(asm_ast::Program prog) {
  // The labels are numbered across the whole program, past any from an
  // earlier run of the fixed point. dasm scopes a `.` label to its
  // SUBROUTINE, but the simulator's assembler (`sim/assembler.py`)
  // rejects the same label in two functions.
  long long counter = 0;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = as<ir::Function>(tl)) {
      for (const auto& ins : fn->instructions) {
        if (const auto* label = as<ir::Label>(ins)) {
          if (auto n = label_number(label->name)) {
            counter = std::max(counter, *n + 1);
          }
        }
      }
    }
  }
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, counter);
    }
  }
  return prog;
}

}  // namespace c6502::passes
