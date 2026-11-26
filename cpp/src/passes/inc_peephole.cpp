#include "passes/inc_peephole.h"

#include <algorithm>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_x_indexed(const ir::Type_operand& op) {
  const auto* d = as<ir::IndexedData>(op);
  return d != nullptr && isa<ir::X>(d->index);
}

namespace inc_peephole {

bool is_inc_eligible_operand(const ir::Type_operand& op) {
  if (isa<ir::Data, ir::ZP>(op) || is_x_indexed(op)) {
    return true;
  }
  if (const auto* r = as<ir::Reg>(op)) {
    return isa<ir::X, ir::Y>(r->reg);
  }
  return false;
}

bool operands_equal(const ir::Type_operand& a, const ir::Type_operand& b) {
  if (const auto* da = as<ir::Data>(a)) {
    const auto* db = as<ir::Data>(b);
    return db != nullptr && da->name == db->name && da->offset == db->offset;
  }
  if (const auto* za = as<ir::ZP>(a)) {
    const auto* zb = as<ir::ZP>(b);
    return zb != nullptr && za->address == zb->address && za->offset == zb->offset;
  }
  if (const auto* ra = as<ir::Reg>(a)) {
    const auto* rb = as<ir::Reg>(b);
    return rb != nullptr && ra->reg.index() == rb->reg.index();
  }
  if (const auto* xa = as<ir::IndexedData>(a)) {
    const auto* xb = as<ir::IndexedData>(b);
    return xb != nullptr && xa->name == xb->name && xa->offset == xb->offset &&
           xa->index.index() == xb->index.index();
  }
  return false;
}

}  // namespace inc_peephole

namespace {

using inc_peephole::is_inc_eligible_operand;
using inc_peephole::operands_equal;

constexpr std::string_view kDonePrefix = ".inc_done@";

bool is_reg_a_op(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

bool is_imm(const ir::Type_operand& op, int value) {
  const auto* imm = as<ir::Imm>(op);
  return imm != nullptr && imm->value == value;
}

std::optional<long long> parse_digits(std::string_view s) {
  if (s.empty()) {
    return std::nullopt;
  }
  long long n = 0;
  for (char c : s) {
    if (c < '0' || c > '9') {
      return std::nullopt;
    }
    n = n * 10 + (c - '0');
  }
  return n;
}

const ir::Type_operand* match_first_byte(Instrs instrs, std::size_t start) {
  if (start + 4 > instrs.size()) {
    return nullptr;
  }
  const auto* i0 = as<ir::Mov>(instrs[start]);
  if (i0 == nullptr || i0->is_volatile || !is_inc_eligible_operand(i0->src) ||
      !is_reg_a_op(i0->dst)) {
    return nullptr;
  }
  if (!isa<ir::ClearCarry>(instrs[start + 1])) {
    return nullptr;
  }
  const auto* i2 = as<ir::Add>(instrs[start + 2]);
  if (i2 == nullptr || !is_imm(i2->src, 1) || !is_reg_a_op(i2->dst)) {
    return nullptr;
  }
  const auto* i3 = as<ir::Mov>(instrs[start + 3]);
  if (i3 == nullptr || i3->is_volatile || !is_reg_a_op(i3->src) ||
      !operands_equal(i3->dst, i0->src)) {
    return nullptr;
  }
  return &i0->src;
}

const ir::Type_operand* match_continuation_byte(Instrs instrs, std::size_t start) {
  if (start + 3 > instrs.size()) {
    return nullptr;
  }
  const auto* i0 = as<ir::Mov>(instrs[start]);
  if (i0 == nullptr || i0->is_volatile || !is_inc_eligible_operand(i0->src) ||
      !is_reg_a_op(i0->dst)) {
    return nullptr;
  }
  const auto* i1 = as<ir::Add>(instrs[start + 1]);
  if (i1 == nullptr || !is_imm(i1->src, 0) || !is_reg_a_op(i1->dst)) {
    return nullptr;
  }
  const auto* i2 = as<ir::Mov>(instrs[start + 2]);
  if (i2 == nullptr || i2->is_volatile || !is_reg_a_op(i2->src) ||
      !operands_equal(i2->dst, i0->src)) {
    return nullptr;
  }
  return &i0->src;
}

// Loads, stores, and transfers leave C alone, and ADC reads it.
bool carry_consumed_after(Instrs instrs, std::size_t idx) {
  for (std::size_t j = idx; j < instrs.size(); ++j) {
    if (isa<ir::Mov>(instrs[j])) {
      continue;
    }
    return isa<ir::Add>(instrs[j]);
  }
  return false;
}

struct Match {
  std::size_t consumed = 0;
  std::vector<ir::Type_operand> mem_ops;
};

std::optional<Match> try_match(Instrs instrs, std::size_t start) {
  if (start + 4 > instrs.size()) {
    return std::nullopt;
  }
  const ir::Type_operand* head = match_first_byte(instrs, start);
  if (head == nullptr) {
    return std::nullopt;
  }
  Match m;
  m.mem_ops.push_back(*head);
  m.consumed = 4;
  while (const ir::Type_operand* next = match_continuation_byte(instrs, start + m.consumed)) {
    m.mem_ops.push_back(*next);
    m.consumed += 3;
  }
  if (carry_consumed_after(instrs, start + m.consumed)) {
    return std::nullopt;
  }
  const bool any_indexed = std::any_of(m.mem_ops.begin(), m.mem_ops.end(),
                                       [](const ir::Type_operand& op) {
                                         return is_x_indexed(op);
                                       });
  if (any_indexed && !(c_dead_at(instrs, start + m.consumed) &&
                       v_dead_at(instrs, start + m.consumed))) {
    return std::nullopt;
  }
  return m;
}

// `INC m` sets Z exactly when the byte wraps to zero. The leading `.`
// makes the label local to dasm's SUBROUTINE.
void build_inc_chain(const std::vector<ir::Type_operand>& mem_ops, long long& counter,
                     std::vector<ir::Type_instruction>& out) {
  if (mem_ops.size() == 1) {
    out.push_back(ir::Inc{.dst = mem_ops[0]});
    return;
  }
  const std::string done = std::string(kDonePrefix) + std::to_string(counter);
  ++counter;
  for (std::size_t k = 0; k < mem_ops.size(); ++k) {
    out.push_back(ir::Inc{.dst = mem_ops[k]});
    if (k + 1 < mem_ops.size()) {
      out.push_back(ir::Branch{.cond = ir::NE{}, .target = done});
    }
  }
  out.push_back(ir::Label{.name = done});
}

void rewrite_function(ir::Function& fn, long long& counter) {
  const auto& instrs = fn.instructions;
  std::vector<ir::Type_instruction> out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    std::optional<Match> match = try_match(instrs, i);
    if (!match) {
      out.push_back(instrs[i]);
      ++i;
      continue;
    }
    changed = true;
    build_inc_chain(match->mem_ops, counter, out);
    if (!a_dead_at(instrs, i + match->consumed)) {
      out.push_back(ir::Mov{.src = match->mem_ops.back(),
                            .dst = ir::Reg{.reg = ir::A{}},
                            .is_volatile = false});
    }
    i += match->consumed;
  }
  if (changed) {
    fn.instructions = std::move(out);
  }
}

}  // namespace

asm_ast::Program apply_inc_peephole(asm_ast::Program prog) {
  // dasm rejects a local label defined twice in one SUBROUTINE.
  long long counter = 0;
  for (const auto& tl : prog.top_level) {
    const auto* fn = as<ir::Function>(tl);
    if (fn == nullptr) {
      continue;
    }
    for (const auto& ins : fn->instructions) {
      const auto* label = as<ir::Label>(ins);
      if (label == nullptr || !label->name.starts_with(kDonePrefix)) {
        continue;
      }
      if (auto n = parse_digits(std::string_view(label->name).substr(kDonePrefix.size()))) {
        counter = std::max(counter, *n + 1);
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
