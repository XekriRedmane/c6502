// The port of passes/memory_value_propagation.py.
#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Follows, by a forward dataflow over each function's CFG, the
// recomputable source each zero-page byte holds, and rewrites the reads
// of those bytes: an access through DPTR or a zero-page pointer whose
// bytes copy another pointer or hold a label's address goes through
// that pointer or to the label, and a read of a byte whose value is an
// immediate, a fixed byte, or an element reads that instead. The
// comment at the top of memory_value_propagation.cpp gives the rules.
//
// `zp_symbol_addrs`, which may be null, adds the zero-page addresses of
// `Data` names (the `__zpabi_*` and `__local_*` slots and the zero-page
// objects) to the runtime's. `noreturn` names the functions that never
// return, whose calls add nothing to what a call may write.
asm_ast::Program apply_memory_value_propagation(asm_ast::Program prog,
                                                const AsmContext& ctx,
                                                const SymbolAddrs* zp_symbol_addrs,
                                                const NameSet& noreturn);

}  // namespace c6502::passes
