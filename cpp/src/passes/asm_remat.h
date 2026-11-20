// The port of passes/asm_remat.py.
#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Rewrites each read of a `__local_<fn>__*` staging slot whose value
// can be recomputed at the read (an immediate, or a byte or an element
// nothing changes in between) to read the source itself, and turns a
// store into a slot that nothing reads any more into its load. The
// comment at the top of asm_remat.cpp gives the rules. `zp_slot_symbols`,
// which may be null, maps each slot symbol to its zero-page address.
asm_ast::Program apply_remat(asm_ast::Program prog, const AsmContext& ctx,
                             const SymbolAddrs* zp_slot_symbols = nullptr);

}  // namespace c6502::passes
