#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Moves each join's `LDA t; STA M`, with `t` one of the function's own
// `__local_*` slots and `M` a non-volatile byte with no index, into the
// last store to `t` on each of the at most three `JMP`s and
// fall-throughs into the join, in every function. Nothing between those
// stores and the copy may touch `t` or `M`, call, or branch, and `t`,
// A, and N/Z must be dead after it. `zp_slot_symbols`, which may be
// null, maps the slot symbols to their zero-page addresses for the
// check that `t` is dead after the load (`mem_dead_after`).
asm_ast::Program apply_join_store(asm_ast::Program prog, const AsmContext& ctx,
                                  const SymbolAddrs* zp_slot_symbols = nullptr);

}  // namespace c6502::passes
