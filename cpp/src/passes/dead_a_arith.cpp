#include "passes/dead_a_arith.h"

#include <utility>
#include <vector>

namespace c6502::passes::dead_a_arith {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

namespace {

bool is_this_reg(const ir::Type_operand& op, const ir::Type_reg& reg) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && r->reg.index() == reg.index();
}

}  // namespace

bool flags_reflect_src_reg(Instrs instrs, std::size_t i) {
  const auto* mov = as<ir::Mov>(instrs[i]);
  if (mov == nullptr || !is_reg_a(mov->dst)) {
    return false;
  }
  const auto* src = as<ir::Reg>(mov->src);
  if (src == nullptr || !isa<ir::X, ir::Y>(src->reg)) {
    return false;
  }
  for (std::size_t j = i; j-- > 0;) {
    const ir::Type_instruction& prev = instrs[j];
    if (isa<ir::Label, ir::Jump, ir::Branch, ir::Ret, ir::Return, ir::Call,
            ir::FunctionPrologue, ir::AllocateStack, ir::LoadAddress>(prev)) {
      return false;
    }
    if (!sets_flags(prev)) {
      continue;
    }
    return writes_reg_setting_n_z(prev, src->reg);
  }
  return false;
}

bool writes_reg_setting_n_z(const ir::Type_instruction& instr,
                            const ir::Type_reg& reg) {
  if (const auto* m = as<ir::Mov>(instr)) {
    return is_this_reg(m->dst, reg);
  }
  if (const auto* i = as<ir::Inc>(instr)) {
    return is_this_reg(i->dst, reg);
  }
  if (const auto* d = as<ir::Dec>(instr)) {
    return is_this_reg(d->dst, reg);
  }
  if (isa<ir::Add, ir::Sub, ir::And, ir::Or, ir::Xor>(instr)) {
    // ADC / SBC / AND / ORA / EOR always write A.
    return isa<ir::A>(reg);
  }
  if (const auto* s = as<ir::ArithmeticShiftLeft>(instr)) {
    return is_this_reg(s->dst, reg);
  }
  if (const auto* s = as<ir::LogicalShiftRight>(instr)) {
    return is_this_reg(s->dst, reg);
  }
  if (const auto* s = as<ir::RotateLeft>(instr)) {
    return is_this_reg(s->dst, reg);
  }
  if (const auto* s = as<ir::RotateRight>(instr)) {
    return is_this_reg(s->dst, reg);
  }
  if (const auto* p = as<ir::Pop>(instr)) {
    return is_this_reg(p->dst, reg);
  }
  return false;
}

}  // namespace c6502::passes::dead_a_arith

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_xy_reg(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::X, ir::Y>(r->reg);
}

bool is_pure_source(const ir::Type_operand& op) {
  return isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh, ir::Data, ir::ZP,
             ir::IndexedData>(op);
}

bool is_dead_index_load(Instrs instrs, const LabelMap& labels, std::size_t i) {
  const auto* ins = as<ir::Mov>(instrs[i]);
  if (ins == nullptr || ins->is_volatile || !is_xy_reg(ins->dst) ||
      !isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh, ir::Data, ir::ZP>(
          ins->src)) {
    return false;
  }
  const bool dead = isa<ir::X>(std::get<ir::Reg>(ins->dst).reg)
                        ? x_dead_at(instrs, labels, i + 1)
                        : y_dead_at(instrs, labels, i + 1);
  return dead && flags_dead_at(instrs, labels, i + 1);
}

bool is_dead_pointer_load(Instrs instrs, const LabelMap& labels, std::size_t i) {
  const auto* ins = as<ir::Mov>(instrs[i]);
  if (ins == nullptr || ins->is_volatile || !is_reg_a(ins->dst) ||
      !isa<ir::IndirectZp, ir::IndirectZpY>(ins->src)) {
    return false;
  }
  if (!(a_dead_at(instrs, labels, i + 1) && flags_dead_at(instrs, labels, i + 1))) {
    return false;
  }
  return isa<ir::IndirectZpY>(ins->src) || y_dead_at(instrs, labels, i + 1);
}

// ASL, LSR, ROL, and ROR each write their operand, N, Z, and C.
bool is_shift(const ir::Type_instruction& instr) {
  return isa<ir::ArithmeticShiftLeft, ir::LogicalShiftRight, ir::RotateLeft,
             ir::RotateRight>(instr);
}

bool writes_only_a_and_flags(const ir::Type_instruction& instr) {
  if (const auto* m = as<ir::Mov>(instr)) {
    if (m->is_volatile || !is_reg_a(m->dst)) {
      return false;
    }
    return is_pure_source(m->src) || is_xy_reg(m->src);
  }
  if (const auto* a = as<ir::Add>(instr)) {
    return is_reg_a(a->dst) && is_pure_source(a->src);
  }
  if (const auto* s = as<ir::Sub>(instr)) {
    return is_reg_a(s->dst) && is_pure_source(s->src);
  }
  if (const auto* a = as<ir::And>(instr)) {
    return is_reg_a(a->dst) && is_pure_source(a->src);
  }
  if (const auto* o = as<ir::Or>(instr)) {
    return is_reg_a(o->dst) && is_pure_source(o->src);
  }
  if (const auto* x = as<ir::Xor>(instr)) {
    if (!is_reg_a(x->dst)) {
      return false;
    }
    for (const ir::Type_operand* s : {&x->src1, &x->src2}) {
      if (!is_reg_a(*s) && !is_pure_source(*s)) {
        return false;
      }
    }
    return true;
  }
  if (const auto* s = as<ir::ArithmeticShiftLeft>(instr)) {
    return is_reg_a(s->dst);
  }
  if (const auto* s = as<ir::LogicalShiftRight>(instr)) {
    return is_reg_a(s->dst);
  }
  if (const auto* s = as<ir::RotateLeft>(instr)) {
    return is_reg_a(s->dst);
  }
  if (const auto* s = as<ir::RotateRight>(instr)) {
    return is_reg_a(s->dst);
  }
  return false;
}

bool droppable(Instrs instrs, const LabelMap& labels, std::size_t i) {
  if (is_dead_index_load(instrs, labels, i) ||
      is_dead_pointer_load(instrs, labels, i)) {
    return true;
  }
  const ir::Type_instruction& instr = instrs[i];
  if (!writes_only_a_and_flags(instr)) {
    return false;
  }
  const std::size_t after = i + 1;
  if (!a_dead_at(instrs, labels, after)) {
    return false;
  }
  // ADC, SBC, and the shifts also write C, and ADC and SBC write V.
  // LDA, TXA, TYA, AND, ORA, and EOR leave C and V as they were.
  const bool add_or_sub = isa<ir::Add, ir::Sub>(instr);
  if (add_or_sub || is_shift(instr)) {
    return flags_dead_at(instrs, labels, after) &&
           c_dead_at(instrs, labels, after) &&
           (!add_or_sub || v_dead_at(instrs, labels, after));
  }
  if (flags_dead_at(instrs, labels, after)) {
    return true;
  }
  return dead_a_arith::flags_reflect_src_reg(instrs, i);
}

void rewrite_function(ir::Function& fn) {
  auto& instrs = fn.instructions;
  while (true) {
    const LabelMap labels(instrs);
    std::vector<bool> drop(instrs.size(), false);
    bool any = false;
    for (std::size_t i = 0; i < instrs.size(); ++i) {
      if (droppable(instrs, labels, i)) {
        drop[i] = true;
        any = true;
      }
    }
    if (!any) {
      return;
    }
    std::vector<ir::Type_instruction> out;
    out.reserve(instrs.size());
    for (std::size_t i = 0; i < instrs.size(); ++i) {
      if (!drop[i]) {
        out.push_back(std::move(instrs[i]));
      }
    }
    instrs = std::move(out);
  }
}

}  // namespace

asm_ast::Program apply_dead_a_arith_elimination(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
