// The walks over an asm instruction's operands that the passes of the
// asm SSA round trip share: every operand, read or written, and every
// operand read.
#pragma once

#include <variant>

#include "asm_ast.h"
#include "passes/decimal_arith.h"

namespace c6502::passes::ssa_operands {

// Calls `f` on each operand of `instr`, read or written.
template <class F>
void for_each_operand(const asm_ast::Type_instruction& instr, F&& f) {
  namespace ir = ::c6502::asm_ast;
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::DecimalArith:
      for (const auto& op : decimal_arith::operands(std::get<ir::DecimalArith>(instr))) {
        f(op);
      }
      break;
    case ir::Kind_instruction::Mov: {
      const auto& i = std::get<ir::Mov>(instr);
      f(i.src);
      f(i.dst);
      break;
    }
    case ir::Kind_instruction::Add: {
      const auto& i = std::get<ir::Add>(instr);
      f(i.src);
      f(i.dst);
      break;
    }
    case ir::Kind_instruction::Sub: {
      const auto& i = std::get<ir::Sub>(instr);
      f(i.src);
      f(i.dst);
      break;
    }
    case ir::Kind_instruction::And: {
      const auto& i = std::get<ir::And>(instr);
      f(i.src);
      f(i.dst);
      break;
    }
    case ir::Kind_instruction::Or: {
      const auto& i = std::get<ir::Or>(instr);
      f(i.src);
      f(i.dst);
      break;
    }
    case ir::Kind_instruction::Xor: {
      const auto& i = std::get<ir::Xor>(instr);
      f(i.src1);
      f(i.src2);
      f(i.dst);
      break;
    }
    case ir::Kind_instruction::Inc:
      f(std::get<ir::Inc>(instr).dst);
      break;
    case ir::Kind_instruction::Dec:
      f(std::get<ir::Dec>(instr).dst);
      break;
    case ir::Kind_instruction::ArithmeticShiftLeft:
      f(std::get<ir::ArithmeticShiftLeft>(instr).dst);
      break;
    case ir::Kind_instruction::LogicalShiftRight:
      f(std::get<ir::LogicalShiftRight>(instr).dst);
      break;
    case ir::Kind_instruction::RotateLeft:
      f(std::get<ir::RotateLeft>(instr).dst);
      break;
    case ir::Kind_instruction::RotateRight:
      f(std::get<ir::RotateRight>(instr).dst);
      break;
    case ir::Kind_instruction::Push:
      f(std::get<ir::Push>(instr).src);
      break;
    case ir::Kind_instruction::Pop:
      f(std::get<ir::Pop>(instr).dst);
      break;
    case ir::Kind_instruction::Compare: {
      const auto& i = std::get<ir::Compare>(instr);
      f(i.left);
      f(i.right);
      break;
    }
    case ir::Kind_instruction::LoadAddress: {
      const auto& i = std::get<ir::LoadAddress>(instr);
      f(i.src);
      f(i.dst);
      break;
    }
    case ir::Kind_instruction::Phi: {
      const auto& i = std::get<ir::Phi>(instr);
      f(i.dst);
      for (const auto& a : i.args) {
        f(std::get<ir::AsmPhiArg>(a).source);
      }
      break;
    }
    default:
      break;
  }
}

// Calls `f` on each operand `instr` reads. The target of a
// read-modify-write is read, and so is the destination of an ALU
// instruction, which is A in what `tac_to_asm` writes. `LoadAddress.src`
// counts as a read of the addressed Pseudo, so that byte DCE keeps the
// definitions of its bytes and `replace_pseudoregisters` its frame slot.
template <class F>
void for_each_use_operand(const asm_ast::Type_instruction& instr, F&& f) {
  namespace ir = ::c6502::asm_ast;
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::DecimalArith:
      for (const auto& op : decimal_arith::reads(std::get<ir::DecimalArith>(instr))) {
        f(op);
      }
      break;
    case ir::Kind_instruction::Mov:
      f(std::get<ir::Mov>(instr).src);
      break;
    case ir::Kind_instruction::Add: {
      const auto& i = std::get<ir::Add>(instr);
      f(i.src);
      f(i.dst);
      break;
    }
    case ir::Kind_instruction::Sub: {
      const auto& i = std::get<ir::Sub>(instr);
      f(i.src);
      f(i.dst);
      break;
    }
    case ir::Kind_instruction::And: {
      const auto& i = std::get<ir::And>(instr);
      f(i.src);
      f(i.dst);
      break;
    }
    case ir::Kind_instruction::Or: {
      const auto& i = std::get<ir::Or>(instr);
      f(i.src);
      f(i.dst);
      break;
    }
    case ir::Kind_instruction::Xor: {
      const auto& i = std::get<ir::Xor>(instr);
      f(i.src1);
      f(i.src2);
      break;
    }
    case ir::Kind_instruction::Inc:
      f(std::get<ir::Inc>(instr).dst);
      break;
    case ir::Kind_instruction::Dec:
      f(std::get<ir::Dec>(instr).dst);
      break;
    case ir::Kind_instruction::ArithmeticShiftLeft:
      f(std::get<ir::ArithmeticShiftLeft>(instr).dst);
      break;
    case ir::Kind_instruction::LogicalShiftRight:
      f(std::get<ir::LogicalShiftRight>(instr).dst);
      break;
    case ir::Kind_instruction::RotateLeft:
      f(std::get<ir::RotateLeft>(instr).dst);
      break;
    case ir::Kind_instruction::RotateRight:
      f(std::get<ir::RotateRight>(instr).dst);
      break;
    case ir::Kind_instruction::Push:
      f(std::get<ir::Push>(instr).src);
      break;
    case ir::Kind_instruction::Compare: {
      const auto& i = std::get<ir::Compare>(instr);
      f(i.left);
      f(i.right);
      break;
    }
    case ir::Kind_instruction::LoadAddress:
      f(std::get<ir::LoadAddress>(instr).src);
      break;
    case ir::Kind_instruction::Phi:
      for (const auto& a : std::get<ir::Phi>(instr).args) {
        f(std::get<ir::AsmPhiArg>(a).source);
      }
      break;
    default:
      break;
  }
}

}  // namespace c6502::passes::ssa_operands
