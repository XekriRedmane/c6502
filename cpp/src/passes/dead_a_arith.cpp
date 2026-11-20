// Dead A arithmetic elimination. The pass isn't ported yet, only the
// flag tests that `loop_constant_a` and `transfer_past_branch` share
// with it.
#include "passes/dead_a_arith.h"

namespace c6502::passes::dead_a_arith {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

namespace {

// Returns whether `op` is the register `reg`.
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
