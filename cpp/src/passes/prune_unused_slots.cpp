#include "passes/prune_unused_slots.h"

#include "passes/asm_context.h"
#include "passes/asm_operands.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;

NameSet referenced_names(const asm_ast::Program& prog) {
  NameSet names;
  for (const auto& tl : prog.top_level) {
    const auto* fn = asdl::as<ir::Function>(tl);
    if (fn == nullptr) {
      continue;
    }
    for (const auto& instr : fn->instructions) {
      for_each_operand<true>(instr, [&](const ir::Type_operand& op) {
        switch (ir::kind(op)) {
          case ir::Kind_operand::Data:
            names.insert(std::get<ir::Data>(op).name);
            break;
          case ir::Kind_operand::IndexedData:
            names.insert(std::get<ir::IndexedData>(op).name);
            break;
          case ir::Kind_operand::ImmLabelLow:
            names.insert(std::get<ir::ImmLabelLow>(op).name);
            break;
          case ir::Kind_operand::ImmLabelHigh:
            names.insert(std::get<ir::ImmLabelHigh>(op).name);
            break;
          default:
            break;
        }
      });
    }
  }
  return names;
}

}  // namespace

OrderedSymbolAddrs prune_unused_locals(const asm_ast::Program& prog,
                                       const OrderedSymbolAddrs& slot_symbols) {
  const NameSet referenced = referenced_names(prog);
  OrderedSymbolAddrs out;
  for (const auto& [name, addr] : slot_symbols) {
    if (name.starts_with("__local_") && !referenced.contains(name)) {
      continue;
    }
    out.emplace_back(name, addr);
  }
  return out;
}

}  // namespace c6502::passes
