#include "passes/dec_branch_indirect_y_fold.h"

#include <cstddef>

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_dec_y(const ir::Type_instruction& instr) {
  const auto* d = as<ir::Dec>(instr);
  if (d == nullptr) {
    return false;
  }
  const auto* r = as<ir::Reg>(d->dst);
  return r != nullptr && isa<ir::Y>(r->reg);
}

bool is_branch_ne(const ir::Type_instruction& instr) {
  const auto* b = as<ir::Branch>(instr);
  return b != nullptr && isa<ir::NE>(b->cond);
}

bool is_indirect_zero(const ir::Type_operand& op) {
  const auto* ind = as<ir::Indirect>(op);
  return ind != nullptr && ind->offset == 0;
}

void fold(ir::Type_operand& op) {
  if (is_indirect_zero(op)) {
    op = ir::IndirectY{};
  }
}

void rewrite_indirect_zero(ir::Type_instruction& instr) {
  if (auto* m = as<ir::Mov>(instr)) {
    fold(m->src);
    fold(m->dst);
  } else if (auto* c = as<ir::Compare>(instr)) {
    fold(c->left);
    fold(c->right);
  }
}

void rewrite_function(ir::Function& fn) {
  auto& instrs = fn.instructions;
  for (std::size_t i = 0; i + 2 < instrs.size(); ++i) {
    // DEY sets N and Z from the new Y, so on BNE's fall-through Y is 0
    // with Z set and N clear, the flags `LDY #$00` would set.
    if (is_dec_y(instrs[i]) && is_branch_ne(instrs[i + 1])) {
      rewrite_indirect_zero(instrs[i + 2]);
    }
  }
}

}  // namespace

asm_ast::Program apply_dec_branch_indirect_y_fold(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
