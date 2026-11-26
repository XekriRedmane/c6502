#include "passes/split_mem_to_mem.h"

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_mem(const ir::Type_operand& op) {
  return isa<ir::Data, ir::ZP, ir::Frame, ir::Stack, ir::Indirect,
             ir::IndirectY, ir::IndirectZp, ir::IndirectZpY,
             ir::IndexedData>(op);
}

enum class Action {
  kKeep,
  kDrop,
  kSplitRegisters,
  kSplitMemory,
};

Action action_for(const ir::Type_instruction& instr) {
  const ir::Mov* mov = as<ir::Mov>(instr);
  if (mov == nullptr) {
    return Action::kKeep;
  }
  const ir::Reg* src_reg = as<ir::Reg>(mov->src);
  const ir::Reg* dst_reg = as<ir::Reg>(mov->dst);
  if (dst_reg != nullptr && mov->src == mov->dst) {
    return Action::kDrop;
  }
  if (src_reg != nullptr && dst_reg != nullptr &&
      isa<ir::X, ir::Y>(src_reg->reg) && isa<ir::X, ir::Y>(dst_reg->reg)) {
    return Action::kSplitRegisters;
  }
  if (!mov->is_volatile && is_mem(mov->src) && is_mem(mov->dst)) {
    if (mov->src == mov->dst) {
      return Action::kDrop;
    }
    return Action::kSplitMemory;
  }
  return Action::kKeep;
}

void split_through_a(const ir::Mov& mov, bool is_volatile,
                     std::vector<ir::Type_instruction>& out) {
  const ir::Reg a{.reg = ir::A{}};
  out.push_back(ir::Mov{.src = mov.src, .dst = a, .is_volatile = is_volatile});
  out.push_back(ir::Mov{.src = a, .dst = mov.dst, .is_volatile = is_volatile});
}

void rewrite_function(ir::Function& fn) {
  auto& instrs = fn.instructions;
  const auto first = std::find_if(
      instrs.begin(), instrs.end(),
      [](const ir::Type_instruction& i) { return action_for(i) != Action::kKeep; });
  if (first == instrs.end()) {
    return;
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size() + 8);
  std::move(instrs.begin(), first, std::back_inserter(out));
  for (auto it = first; it != instrs.end(); ++it) {
    switch (action_for(*it)) {
      case Action::kKeep:
        out.push_back(std::move(*it));
        break;
      case Action::kDrop:
        break;
      case Action::kSplitRegisters:
        split_through_a(std::get<ir::Mov>(*it), false, out);
        break;
      case Action::kSplitMemory: {
        const auto& mov = std::get<ir::Mov>(*it);
        split_through_a(mov, mov.is_volatile, out);
        break;
      }
    }
  }
  instrs = std::move(out);
}

}  // namespace

asm_ast::Program apply_split_mem_to_mem(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
