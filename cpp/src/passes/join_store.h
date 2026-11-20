// The port of passes/join_store.py.
#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Moves each join's `LDA t; STA M` into the stores to `t` on the paths
// into the join, in every function, where the rules in the comment at
// the top of join_store.cpp allow. `zp_slot_symbols`, which may be
// null, maps the slot symbols to their zero-page addresses for the
// check that `t` is dead after the load (`mem_dead_after`).
asm_ast::Program apply_join_store(asm_ast::Program prog, const AsmContext& ctx,
                                  const SymbolAddrs* zp_slot_symbols = nullptr);

}  // namespace c6502::passes
