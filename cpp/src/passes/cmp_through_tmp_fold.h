// The port of passes/cmp_through_tmp_fold.py.
#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Folds each `LDA src; STA tmp; LDA other; CMP tmp` into `LDA other;
// CMP src`, and the same with `ADC`, `SBC`, `AND`, `ORA`, or `EOR` for
// `CMP`, in every function, where the comment at the top of
// cmp_through_tmp_fold.cpp says it is sound. `zp_slot_symbols`, which
// may be null, maps the slot symbols to their zero-page addresses for
// the check that `tmp` is dead after the compare (`mem_dead_after`).
asm_ast::Program apply_cmp_through_tmp_fold(asm_ast::Program prog, const AsmContext& ctx,
                                            const SymbolAddrs* zp_slot_symbols = nullptr);

}  // namespace c6502::passes
