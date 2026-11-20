#include "passes/optimization_asm/all_operands.h"

namespace c6502::passes::optimizer {

namespace ir = ::c6502::asm_ast;

std::vector<const ir::Type_operand*> all_operands_in(const ir::Type_instruction& instr) {
  using K = ir::Kind_instruction;
  std::vector<const ir::Type_operand*> out;
  switch (ir::kind(instr)) {
    case K::DecimalArith: {
      const auto& d = std::get<ir::DecimalArith>(instr);
      for (const auto& op : d.left) {
        out.push_back(&op);
      }
      for (const auto& op : d.right) {
        out.push_back(&op);
      }
      for (const auto& op : d.dst) {
        out.push_back(&op);
      }
      break;
    }
    case K::Mov: {
      const auto& m = std::get<ir::Mov>(instr);
      out = {&m.src, &m.dst};
      break;
    }
    case K::Add: {
      const auto& m = std::get<ir::Add>(instr);
      out = {&m.src, &m.dst};
      break;
    }
    case K::Sub: {
      const auto& m = std::get<ir::Sub>(instr);
      out = {&m.src, &m.dst};
      break;
    }
    case K::And: {
      const auto& m = std::get<ir::And>(instr);
      out = {&m.src, &m.dst};
      break;
    }
    case K::Or: {
      const auto& m = std::get<ir::Or>(instr);
      out = {&m.src, &m.dst};
      break;
    }
    case K::Xor: {
      const auto& m = std::get<ir::Xor>(instr);
      out = {&m.src1, &m.src2, &m.dst};
      break;
    }
    case K::Inc:
      out = {&std::get<ir::Inc>(instr).dst};
      break;
    case K::Dec:
      out = {&std::get<ir::Dec>(instr).dst};
      break;
    case K::ArithmeticShiftLeft:
      out = {&std::get<ir::ArithmeticShiftLeft>(instr).dst};
      break;
    case K::LogicalShiftRight:
      out = {&std::get<ir::LogicalShiftRight>(instr).dst};
      break;
    case K::RotateLeft:
      out = {&std::get<ir::RotateLeft>(instr).dst};
      break;
    case K::RotateRight:
      out = {&std::get<ir::RotateRight>(instr).dst};
      break;
    case K::Push:
      out = {&std::get<ir::Push>(instr).src};
      break;
    case K::Pop:
      out = {&std::get<ir::Pop>(instr).dst};
      break;
    case K::Compare: {
      const auto& c = std::get<ir::Compare>(instr);
      out = {&c.left, &c.right};
      break;
    }
    case K::LoadAddress: {
      const auto& l = std::get<ir::LoadAddress>(instr);
      out = {&l.src, &l.dst};
      break;
    }
    case K::Phi: {
      const auto& p = std::get<ir::Phi>(instr);
      out.push_back(&p.dst);
      for (const auto& a : p.args) {
        out.push_back(&std::get<ir::AsmPhiArg>(a).source);
      }
      break;
    }
    default:
      break;
  }
  return out;
}

}  // namespace c6502::passes::optimizer
