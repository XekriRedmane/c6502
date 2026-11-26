#include "passes/index_inc_cmp_y.h"

#include <cstddef>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "passes/asm_dead_store.h"
#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using Instructions = std::vector<ir::Type_instruction>;

struct Match {
  ir::Type_operand src;
  bool is_dec = false;
  ir::Type_operand other;
};

bool cpy_addressable(const ir::Type_operand& op) {
  return isa<ir::Imm, ir::Data, ir::ZP>(op);
}

bool starts_with(const std::string& s, const std::string& prefix) {
  return s.compare(0, prefix.size(), prefix) == 0;
}

// The 6502 has `LDY abs,X` and no `LDY abs,Y`.
bool is_load_indexed_by_x(const ir::Type_instruction& instr) {
  const auto* mov = as<ir::Mov>(instr);
  if (mov == nullptr || mov->is_volatile) {
    return false;
  }
  const auto* src = as<ir::IndexedData>(mov->src);
  return src != nullptr && isa<ir::X>(src->index) && is_reg_a(mov->dst);
}

bool tmp_dead_after(const Instructions& instrs, const LabelMap& labels, std::size_t idx,
                    const ir::Type_operand& tmp, const std::string& fn_name,
                    const AsmContext& ctx) {
  const std::string prefix = "__local_" + fn_name + "__";
  if (const auto* d = as<ir::Data>(tmp)) {
    if (!starts_with(d->name, prefix)) {
      return false;
    }
  } else if (const auto* zp = as<ir::ZP>(tmp)) {
    const asdl::Int addr = zp->address + zp->offset;
    if (!(0x80 <= addr && addr < 0x100) || ctx.is_user_zp_byte(addr)) {
      return false;
    }
  } else {
    return false;
  }
  const NameSet taken = asm_dead_store::collect_address_taken_names(instrs);
  const bool is_private = asm_dead_store::call_cannot_touch(tmp, prefix, taken);
  std::set<std::size_t> visited;
  std::vector<std::size_t> stack = asm_dead_store::successors(instrs, idx, labels);
  while (!stack.empty()) {
    const std::size_t j = stack.back();
    stack.pop_back();
    if (visited.contains(j)) {
      continue;
    }
    visited.insert(j);
    const auto& nxt = instrs[j];
    auto push_successors = [&] {
      for (const std::size_t s : asm_dead_store::successors(instrs, j, labels)) {
        stack.push_back(s);
      }
    };
    if (const auto* call = as<ir::Call>(nxt);
        call != nullptr && call->name != "icall" && is_private) {
      push_successors();
      continue;
    }
    if (asm_dead_store::is_opaque(nxt)) {
      return false;
    }
    if (const auto* jump = as<ir::Jump>(nxt); jump != nullptr && !labels.contains(jump->target)) {
      if (!asm_dead_store::is_dead_at_tail_call(tmp, &taken)) {
        return false;
      }
      continue;
    }
    if (isa<ir::Ret, ir::Return>(nxt)) {
      continue;
    }
    if (asm_dead_store::reads(nxt, tmp, ctx, nullptr, is_private)) {
      return false;
    }
    if (asm_dead_store::writes_same(nxt, tmp)) {
      continue;
    }
    push_successors();
  }
  return true;
}

std::optional<Match> match_pattern(const Instructions& instrs, const LabelMap& labels,
                                   std::size_t i, const std::string& fn_name,
                                   const AsmContext& ctx) {
  if (i + 4 >= instrs.size()) {
    return std::nullopt;
  }
  if (!is_load_indexed_by_x(instrs[i])) {
    return std::nullopt;
  }
  const ir::Type_operand& indexed_src = std::get<ir::Mov>(instrs[i]).src;
  const auto* b = as<ir::Mov>(instrs[i + 1]);
  if (b == nullptr || b->is_volatile) {
    return std::nullopt;
  }
  if (!(is_reg_a(b->src) && isa<ir::Data, ir::ZP>(b->dst))) {
    return std::nullopt;
  }
  const ir::Type_operand& tmp = b->dst;
  bool is_dec = false;
  if (const auto* inc = as<ir::Inc>(instrs[i + 2])) {
    if (inc->dst != tmp) {
      return std::nullopt;
    }
  } else if (const auto* dec = as<ir::Dec>(instrs[i + 2])) {
    if (dec->dst != tmp) {
      return std::nullopt;
    }
    is_dec = true;
  } else {
    return std::nullopt;
  }
  const auto* d = as<ir::Mov>(instrs[i + 3]);
  if (d == nullptr || d->is_volatile) {
    return std::nullopt;
  }
  if (!(d->src == tmp && is_reg_a(d->dst))) {
    return std::nullopt;
  }
  const auto* e = as<ir::Compare>(instrs[i + 4]);
  if (e == nullptr || !is_reg_a(e->left) || !cpy_addressable(e->right)) {
    return std::nullopt;
  }
  if (!a_dead_at(instrs, labels, i + 5)) {
    return std::nullopt;
  }
  if (!y_dead_at(instrs, labels, i)) {
    return std::nullopt;
  }
  if (!tmp_dead_after(instrs, labels, i + 4, tmp, fn_name, ctx)) {
    return std::nullopt;
  }
  return Match{.src = indexed_src, .is_dec = is_dec, .other = e->right};
}

// `CMP` and `CPY` set N, Z, and C alike, and `CPY` doesn't set V.
std::optional<Match> match_in_a(const Instructions& instrs, const LabelMap& labels,
                                std::size_t i) {
  if (i + 3 >= instrs.size()) {
    return std::nullopt;
  }
  if (!is_load_indexed_by_x(instrs[i])) {
    return std::nullopt;
  }
  const auto& b = instrs[i + 1];
  const auto& c = instrs[i + 2];
  const ir::Type_operand* step_src = nullptr;
  const ir::Type_operand* step_dst = nullptr;
  bool is_dec = false;
  if (const auto* add = as<ir::Add>(c); add != nullptr && isa<ir::ClearCarry>(b)) {
    step_src = &add->src;
    step_dst = &add->dst;
  } else if (const auto* sub = as<ir::Sub>(c); sub != nullptr && isa<ir::SetCarry>(b)) {
    step_src = &sub->src;
    step_dst = &sub->dst;
    is_dec = true;
  } else {
    return std::nullopt;
  }
  const auto* imm = as<ir::Imm>(*step_src);
  if (!(imm != nullptr && imm->value == 1 && is_reg_a(*step_dst))) {
    return std::nullopt;
  }
  const auto* e = as<ir::Compare>(instrs[i + 3]);
  if (!(e != nullptr && is_reg_a(e->left) && cpy_addressable(e->right))) {
    return std::nullopt;
  }
  const std::size_t after = i + 4;
  if (!(a_dead_at(instrs, labels, after) && v_dead_at(instrs, labels, after) &&
        y_dead_at(instrs, labels, i))) {
    return std::nullopt;
  }
  return Match{.src = std::get<ir::Mov>(instrs[i]).src, .is_dec = is_dec, .other = e->right};
}

void rewrite_function(ir::Function& fn, const AsmContext& ctx) {
  const auto& instrs = fn.instructions;
  const LabelMap labels(instrs);
  const ir::Type_operand reg_y = ir::Reg{.reg = ir::Y{}};
  Instructions out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    std::optional<Match> match = match_pattern(instrs, labels, i, fn.name, ctx);
    std::size_t width = 5;
    if (!match) {
      match = match_in_a(instrs, labels, i);
      width = 4;
    }
    if (match) {
      out.push_back(ir::Mov{.src = match->src, .dst = reg_y, .is_volatile = false});
      if (match->is_dec) {
        out.push_back(ir::Dec{.dst = reg_y});
      } else {
        out.push_back(ir::Inc{.dst = reg_y});
      }
      out.push_back(ir::Compare{.left = reg_y, .right = match->other});
      i += width;
      changed = true;
      continue;
    }
    out.push_back(instrs[i]);
    ++i;
  }
  if (changed) {
    fn.instructions = std::move(out);
  }
}

}  // namespace

asm_ast::Program apply_index_inc_cmp_y(asm_ast::Program prog, const AsmContext& ctx) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, ctx);
    }
  }
  return prog;
}

}  // namespace c6502::passes
