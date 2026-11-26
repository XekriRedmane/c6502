#include "passes/decimal_arith.h"

namespace c6502::passes::decimal_arith {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::isa;

std::vector<ir::Type_operand> reads(const ir::DecimalArith& instr) {
  std::vector<ir::Type_operand> out(instr.left);
  out.insert(out.end(), instr.right.begin(), instr.right.end());
  return out;
}

std::vector<ir::Type_operand> writes(const ir::DecimalArith& instr) {
  return instr.dst;
}

std::vector<ir::Type_operand> operands(const ir::DecimalArith& instr) {
  std::vector<ir::Type_operand> out = reads(instr);
  out.insert(out.end(), instr.dst.begin(), instr.dst.end());
  return out;
}

bool writes_y(const ir::DecimalArith& instr) {
  for (const auto* list : {&instr.left, &instr.right, &instr.dst}) {
    for (const auto& op : *list) {
      if (isa<ir::Frame, ir::Stack>(op)) {
        return true;
      }
    }
  }
  return false;
}

std::vector<ir::Pseudo> pseudo_reads(const ir::DecimalArith& instr) {
  std::vector<ir::Pseudo> out;
  for (const auto* list : {&instr.left, &instr.right}) {
    for (const auto& op : *list) {
      if (const auto* p = asdl::as<ir::Pseudo>(op)) {
        out.push_back(*p);
      }
    }
  }
  return out;
}

std::vector<ir::Pseudo> pseudo_writes(const ir::DecimalArith& instr) {
  std::vector<ir::Pseudo> out;
  for (const auto& op : instr.dst) {
    if (const auto* p = asdl::as<ir::Pseudo>(op)) {
      out.push_back(*p);
    }
  }
  return out;
}

std::vector<const ir::Pseudo*> pseudo_read_ptrs(const ir::DecimalArith& instr) {
  std::vector<const ir::Pseudo*> out;
  for (const auto* list : {&instr.left, &instr.right}) {
    for (const auto& op : *list) {
      if (const auto* p = asdl::as<ir::Pseudo>(op)) {
        out.push_back(p);
      }
    }
  }
  return out;
}

std::vector<const ir::Pseudo*> pseudo_write_ptrs(const ir::DecimalArith& instr) {
  std::vector<const ir::Pseudo*> out;
  for (const auto& op : instr.dst) {
    if (const auto* p = asdl::as<ir::Pseudo>(op)) {
      out.push_back(p);
    }
  }
  return out;
}

std::vector<ir::Type_operand> later_sources(const ir::DecimalArith& instr, std::size_t k) {
  std::vector<ir::Type_operand> out;
  for (std::size_t j = k + 1; j < instr.left.size(); ++j) {
    out.push_back(instr.left[j]);
  }
  for (std::size_t j = k + 1; j < instr.right.size(); ++j) {
    out.push_back(instr.right[j]);
  }
  return out;
}

std::vector<std::pair<std::string, std::string>> early_write_pairs(
    const ir::DecimalArith& instr) {
  std::vector<std::pair<std::string, std::string>> out;
  for (std::size_t k = 0; k < instr.dst.size(); ++k) {
    const auto* d = asdl::as<ir::Pseudo>(instr.dst[k]);
    if (d == nullptr) {
      continue;
    }
    for (const auto& s : later_sources(instr, k)) {
      const auto* sp = asdl::as<ir::Pseudo>(s);
      if (sp != nullptr && sp->name != d->name) {
        out.emplace_back(d->name, sp->name);
      }
    }
  }
  return out;
}

std::vector<std::string> form_problems(const ir::DecimalArith& instr) {
  std::vector<std::string> problems;
  const std::size_t n = instr.dst.size();
  if (n == 0 || instr.left.size() != n || instr.right.size() != n) {
    problems.push_back("DecimalArith has " + std::to_string(instr.left.size()) + " left, " +
                       std::to_string(instr.right.size()) + " right, and " + std::to_string(n) +
                       " dst bytes");
  }
  for (const auto& op : reads(instr)) {
    if (!isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh, ir::Pseudo, ir::Data, ir::ZP, ir::Frame,
             ir::Stack>(op)) {
      problems.push_back("DecimalArith reads " + asdl::constructor_name(op));
    }
  }
  for (const auto& op : writes(instr)) {
    if (!isa<ir::Pseudo, ir::Data, ir::ZP, ir::Frame, ir::Stack>(op)) {
      problems.push_back("DecimalArith writes " + asdl::constructor_name(op));
    }
  }
  return problems;
}

}  // namespace c6502::passes::decimal_arith
