// The asm CFG with blocks that own their instructions.
//
// The edges are those of the function as it was partitioned, so a pass
// that changes control flow builds a new CFG after the change.
#include "passes/optimization_asm/owned_cfg.h"

namespace c6502::passes {

namespace ir = ::c6502::asm_ast;

OwnedCfg build_owned_cfg(const ir::Function& fn) {
  OwnedCfg out;
  out.graph = build_cfg(fn);
  out.code.resize(out.graph.blocks.size());
  for (const int id : out.graph.block_order) {
    const auto span = out.graph.instructions(id);
    out.code[id].assign(span.begin(), span.end());
  }
  out.graph.function_instructions = {};
  for (auto& b : out.graph.blocks) {
    b.begin = 0;
    b.end = 0;
  }
  return out;
}

ir::Function owned_cfg_to_function(const ir::Function& fn, const OwnedCfg& cfg) {
  ir::Function out{.name = fn.name, .is_global = fn.is_global, .params = fn.params};
  std::size_t n = 0;
  for (const int id : cfg.graph.block_order) {
    n += cfg.code[id].size();
  }
  out.instructions.reserve(n);
  for (const int id : cfg.graph.block_order) {
    out.instructions.insert(out.instructions.end(), cfg.code[id].begin(), cfg.code[id].end());
  }
  return out;
}

}  // namespace c6502::passes
