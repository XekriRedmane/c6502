// Leaves the unused `__local_<fn>_b<k>` bindings out of the slot
// symbols just before the text is written.
//
// `zp_local_allocation` gives each function a private range of zero-page
// bytes for its body locals before the peephole fixed point runs. The
// passes after it (`asm_dead_store`, `redundant_load_elimination`,
// `adc_commute`, and others) may then drop every read and write of one
// of those bytes, and its `EQU` line would declare an address no
// instruction names.
//
// This pass collects the name of every operand that the emitter writes
// by its symbol, and keeps only the `__local_*` symbols among them. The
// `__zpabi_*` slots and every other entry stay, because callers in other
// units or the runtime may name them.
//
// The link metadata still gives each function's `local_bytes`, which the
// linker sizes the function's range by, so the pruned bytes stay
// reserved. The pruning makes the output clearer and saves no bytes.
#include "passes/prune_unused_slots.h"

#include "passes/asm_context.h"
#include "passes/asm_operands.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;

// Returns the name of every operand of `prog` that the emitter writes by
// its symbol. The halves of an address (`#<sym`, `#>sym`) count, since
// an address-taken local whose only references are its address would
// otherwise lose its `EQU` and leave the output with an undefined
// symbol. Reads and writes count alike, since either makes the symbol
// live.
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
