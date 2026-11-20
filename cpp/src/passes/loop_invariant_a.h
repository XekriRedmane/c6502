// The port of passes/loop_invariant_a.py.
#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Keeps a loop-invariant compare key in A across each loop whose
// header starts `LDA x; CMP k; BEQ/BNE`: the compare becomes `CMP x`,
// `k` is loaded once before the loop, and it is loaded again only on
// the edges inside the loop that reach the header with A overwritten.
// The comment at the top of loop_invariant_a.cpp gives the conditions.
// `zp_slot_symbols`, which may be null, maps the compiler's zero-page
// slot symbols to their addresses for `may_alias`. The statics and
// every function the pass leaves alone come back unchanged.
asm_ast::Program apply_loop_invariant_a(asm_ast::Program prog, const AsmContext& ctx,
                                        const SymbolAddrs* zp_slot_symbols = nullptr);

}  // namespace c6502::passes
