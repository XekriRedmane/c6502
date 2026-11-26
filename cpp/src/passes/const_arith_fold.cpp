#include "passes/const_arith_fold.h"

#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using InstrList = std::vector<ir::Type_instruction>;

std::optional<asdl::Int> imm_byte(const ir::Type_operand& op) {
  if (const auto* imm = as<ir::Imm>(op)) {
    return imm->value & 0xFF;
  }
  return std::nullopt;
}

ir::Mov load_a(ir::Type_operand src, const ir::Type_operand& dst) {
  return ir::Mov{.src = std::move(src), .dst = dst, .is_volatile = false};
}

std::optional<ir::Type_instruction> constant_result(const ir::Type_instruction& a) {
  if (const auto* x = as<ir::And>(a)) {
    if (is_reg_a(x->dst) && imm_byte(x->src) == asdl::Int(0)) {
      return load_a(ir::Imm{.value = 0}, x->dst);
    }
  } else if (const auto* y = as<ir::Or>(a)) {
    if (is_reg_a(y->dst) && imm_byte(y->src) == asdl::Int(0xFF)) {
      return load_a(ir::Imm{.value = 0xFF}, y->dst);
    }
  }
  return std::nullopt;
}

std::optional<ir::Type_operand> label_high_sum(const ir::Type_instruction& a,
                                               const ir::Type_instruction& flag,
                                               const ir::Type_instruction& op) {
  const auto* load = as<ir::Mov>(a);
  if (load == nullptr || !is_reg_a(load->dst)) {
    return std::nullopt;
  }
  const ir::Type_operand* op_src = nullptr;
  int sign = 0;
  if (const auto* add = as<ir::Add>(op); add && is_reg_a(add->dst)) {
    if (isa<ir::ClearCarry>(flag)) {
      sign = 1;
    }
    op_src = &add->src;
  } else if (const auto* sub = as<ir::Sub>(op); sub && is_reg_a(sub->dst)) {
    if (isa<ir::SetCarry>(flag)) {
      sign = -1;
    }
    op_src = &sub->src;
  }
  if (op_src == nullptr || sign == 0) {
    return std::nullopt;
  }
  const ir::ImmLabelHigh* label = nullptr;
  asdl::Int k = 0;
  if (const auto* hi = as<ir::ImmLabelHigh>(load->src); hi && isa<ir::Imm>(*op_src)) {
    label = hi;
    k = *imm_byte(*op_src);
  } else if (sign == 1 && isa<ir::Imm>(load->src) && isa<ir::ImmLabelHigh>(*op_src)) {
    label = &std::get<ir::ImmLabelHigh>(*op_src);
    k = *imm_byte(load->src);
  } else {
    return std::nullopt;
  }
  const asdl::Int offset = label->offset + asdl::Int(sign) * 256 * k;
  if (offset < 0) {
    return std::nullopt;
  }
  return ir::ImmLabelHigh{.name = label->name, .offset = offset};
}

std::optional<ir::Type_instruction> try_fold_add(const InstrList& instrs, const LabelMap& labels,
                                                 std::size_t i) {
  if (i + 2 >= instrs.size()) {
    return std::nullopt;
  }
  const auto& a = instrs[i];
  const auto& flag = instrs[i + 1];
  const auto& op = instrs[i + 2];
  if (const auto high = label_high_sum(a, flag, op)) {
    if (!(c_dead_at(instrs, labels, i + 3) && v_dead_at(instrs, labels, i + 3))) {
      return std::nullopt;
    }
    return load_a(*high, std::get<ir::Mov>(a).dst);
  }
  const auto* load = as<ir::Mov>(a);
  if (load == nullptr || !isa<ir::Imm>(load->src) || !is_reg_a(load->dst)) {
    return std::nullopt;
  }
  const auto* add = as<ir::Add>(op);
  const auto* sub = as<ir::Sub>(op);
  const ir::Type_operand* src = add ? &add->src : sub ? &sub->src : nullptr;
  const ir::Type_operand* dst = add ? &add->dst : sub ? &sub->dst : nullptr;
  if (src == nullptr || !isa<ir::Imm>(*src) || !is_reg_a(*dst)) {
    return std::nullopt;
  }
  const asdl::Int k1 = *imm_byte(load->src);
  const asdl::Int k2 = *imm_byte(*src);
  asdl::Int value = 0;
  if (add != nullptr && isa<ir::ClearCarry>(flag)) {
    value = (k1 + k2) & 0xFF;
  } else if (sub != nullptr && isa<ir::SetCarry>(flag)) {
    value = (k1 - k2) & 0xFF;
  } else {
    return std::nullopt;
  }
  if (!(c_dead_at(instrs, labels, i + 3) && v_dead_at(instrs, labels, i + 3))) {
    return std::nullopt;
  }
  return load_a(ir::Imm{.value = value}, load->dst);
}

// The 6502's SBC subtracts the borrow 1 - C, so `SBC #k1; CLC; ADC #k2`
// is `A - k1 - (1 - C) + k2`, which is `SBC #(k1 - k2)`.
std::optional<ir::Type_instruction> try_fold_add_add(const InstrList& instrs,
                                                     const LabelMap& labels, std::size_t i) {
  if (i + 2 >= instrs.size()) {
    return std::nullopt;
  }
  const auto& first = instrs[i];
  const auto& flag = instrs[i + 1];
  const auto& second = instrs[i + 2];
  auto operands = [](const ir::Type_instruction& ins)
      -> std::pair<const ir::Type_operand*, const ir::Type_operand*> {
    if (const auto* x = as<ir::Add>(ins)) return {&x->src, &x->dst};
    if (const auto* x = as<ir::Sub>(ins)) return {&x->src, &x->dst};
    return {nullptr, nullptr};
  };
  const auto [src1, dst1] = operands(first);
  const auto [src2, dst2] = operands(second);
  if (src1 == nullptr || !isa<ir::Imm>(*src1) || !is_reg_a(*dst1) || src2 == nullptr ||
      !isa<ir::Imm>(*src2) || !is_reg_a(*dst2)) {
    return std::nullopt;
  }
  if (isa<ir::Add>(second) != isa<ir::ClearCarry>(flag)) {
    return std::nullopt;
  }
  if (!isa<ir::ClearCarry, ir::SetCarry>(flag)) {
    return std::nullopt;
  }
  const asdl::Int k1 = *imm_byte(*src1);
  const asdl::Int k2 = *imm_byte(*src2);
  const bool same = isa<ir::Add>(first) == isa<ir::Add>(second);
  const asdl::Int value = (same ? k1 + k2 : k1 - k2) & 0xFF;
  if (!(c_dead_at(instrs, labels, i + 3) && v_dead_at(instrs, labels, i + 3))) {
    return std::nullopt;
  }
  if (isa<ir::Add>(first)) {
    return ir::Add{.src = ir::Imm{.value = value}, .dst = *dst1};
  }
  return ir::Sub{.src = ir::Imm{.value = value}, .dst = *dst1};
}

std::optional<std::vector<ir::Type_instruction>> shift_in_one(const InstrList& instrs,
                                                              const LabelMap& labels,
                                                              std::size_t i) {
  if (i + 2 >= instrs.size()) {
    return std::nullopt;
  }
  const auto* shift = as<ir::ArithmeticShiftLeft>(instrs[i]);
  const auto* op = as<ir::Add>(instrs[i + 2]);
  if (!(shift && is_reg_a(shift->dst) && isa<ir::ClearCarry>(instrs[i + 1]) && op &&
        is_reg_a(op->dst) && imm_byte(op->src) == asdl::Int(1))) {
    return std::nullopt;
  }
  if (!(c_dead_at(instrs, labels, i + 3) && v_dead_at(instrs, labels, i + 3))) {
    return std::nullopt;
  }
  return std::vector<ir::Type_instruction>{ir::SetCarry{}, ir::RotateLeft{.dst = shift->dst}};
}

bool is_carry_free_zero_add(const InstrList& instrs, const LabelMap& labels, std::size_t i) {
  if (i + 1 >= instrs.size()) {
    return false;
  }
  const auto& flag = instrs[i];
  const auto& op = instrs[i + 1];
  const ir::Type_operand* src = nullptr;
  const ir::Type_operand* dst = nullptr;
  if (const auto* x = as<ir::Add>(op)) {
    src = &x->src;
    dst = &x->dst;
  } else if (const auto* y = as<ir::Sub>(op)) {
    src = &y->src;
    dst = &y->dst;
  }
  if (src == nullptr || imm_byte(*src) != asdl::Int(0) || !is_reg_a(*dst)) {
    return false;
  }
  if (isa<ir::Add>(op) != isa<ir::ClearCarry>(flag)) {
    return false;
  }
  if (isa<ir::Sub>(op) != isa<ir::SetCarry>(flag)) {
    return false;
  }
  if (!v_dead_at(instrs, labels, i + 2)) {
    return false;
  }
  if (flags_dead_at(instrs, labels, i + 2)) {
    return true;
  }
  if (i == 0) {
    return false;
  }
  const auto& prev = instrs[i - 1];
  if (const auto* x = as<ir::Mov>(prev)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::And>(prev)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::Or>(prev)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::Add>(prev)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::Sub>(prev)) return is_reg_a(x->dst);
  return false;
}

std::optional<ir::Type_instruction> try_fold(const ir::Type_instruction& a,
                                             const ir::Type_instruction& b) {
  const auto* load = as<ir::Mov>(a);
  if (load == nullptr || !isa<ir::Imm>(load->src) || !is_reg_a(load->dst)) {
    return std::nullopt;
  }
  const asdl::Int c1 = *imm_byte(load->src);
  if (const auto* x = as<ir::And>(b); x && isa<ir::Imm>(x->src) && is_reg_a(x->dst)) {
    return load_a(ir::Imm{.value = c1 & *imm_byte(x->src)}, load->dst);
  }
  if (const auto* x = as<ir::Or>(b); x && isa<ir::Imm>(x->src) && is_reg_a(x->dst)) {
    return load_a(ir::Imm{.value = c1 | *imm_byte(x->src)}, load->dst);
  }
  return std::nullopt;
}

bool is_identity_after_a_write(const ir::Type_instruction& a, const ir::Type_instruction& b) {
  if (const auto* x = as<ir::Or>(b)) {
    if (!(isa<ir::Imm>(x->src) && is_reg_a(x->dst) && *imm_byte(x->src) == 0)) {
      return false;
    }
  } else if (const auto* y = as<ir::And>(b)) {
    if (!(isa<ir::Imm>(y->src) && is_reg_a(y->dst) && *imm_byte(y->src) == 0xFF)) {
      return false;
    }
  } else {
    return false;
  }
  if (const auto* x = as<ir::Mov>(a)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::And>(a)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::Or>(a)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::Add>(a)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::Sub>(a)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::Xor>(a)) return is_reg_a(x->dst);
  return false;
}

std::optional<ir::Type_instruction> try_absorb_zero_load(const ir::Type_instruction& a,
                                                         const ir::Type_instruction& b) {
  const auto* load = as<ir::Mov>(a);
  if (load == nullptr || !isa<ir::Imm>(load->src) || !is_reg_a(load->dst)) {
    return std::nullopt;
  }
  const asdl::Int c1 = *imm_byte(load->src);
  if (const auto* x = as<ir::Or>(b); x && is_reg_a(x->dst) && c1 == 0) {
    return load_a(x->src, load->dst);
  }
  if (const auto* x = as<ir::And>(b); x && is_reg_a(x->dst) && c1 == 0xFF) {
    return load_a(x->src, load->dst);
  }
  return std::nullopt;
}

void rewrite_function(ir::Function& fn) {
  const InstrList& instrs = fn.instructions;
  const LabelMap labels(instrs);
  InstrList out;
  out.reserve(instrs.size());
  std::size_t i = 0;
  while (i + 1 < instrs.size()) {
    const auto& a = instrs[i];
    const auto& b = instrs[i + 1];
    if (auto c = constant_result(a)) {
      out.push_back(std::move(*c));
      i += 1;
      continue;
    }
    if (auto shifted = shift_in_one(instrs, labels, i)) {
      for (auto& x : *shifted) {
        out.push_back(std::move(x));
      }
      i += 3;
      continue;
    }
    if (is_carry_free_zero_add(instrs, labels, i)) {
      out.push_back(a);
      i += 2;
      continue;
    }
    auto summed = try_fold_add(instrs, labels, i);
    if (!summed) {
      summed = try_fold_add_add(instrs, labels, i);
    }
    if (summed) {
      out.push_back(std::move(*summed));
      i += 3;
      continue;
    }
    if (auto folded = try_fold(a, b)) {
      out.push_back(std::move(*folded));
      i += 2;
      continue;
    }
    if (is_identity_after_a_write(a, b)) {
      out.push_back(a);
      i += 2;
      continue;
    }
    if (auto absorbed = try_absorb_zero_load(a, b)) {
      out.push_back(std::move(*absorbed));
      i += 2;
      continue;
    }
    out.push_back(a);
    i += 1;
  }
  while (i < instrs.size()) {
    out.push_back(instrs[i]);
    ++i;
  }
  fn.instructions = std::move(out);
}

}  // namespace

asm_ast::Program apply_const_arith_fold(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
