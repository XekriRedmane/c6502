#pragma once

#include <span>

#include "asm_ast.h"
#include "passes/asm_context.h"
#include "passes/optimization_asm/cfg.h"

namespace c6502::passes {

// Returns the CFG of `fn` (`build_cfg`), with a synthetic block of
// `Label(target); Return` after the function's instructions for each
// `Branch` / `Jump` target the function doesn't define, in the order
// the targets first appear. A tail call is such a jump, so it reaches
// EXIT through its synthetic block. The synthetic instructions are in
// the CFG's `extra` list and never in a rewritten function.
Cfg build_cfg_tolerant(const asm_ast::Function& fn);

// Drops each load into A, X, or Y of a byte or constant the register is
// known to hold already, by a dataflow of what each register mirrors
// over the function's CFG, and makes the other rewrites the comment at
// the top of redundant_load.cpp describes. `zp_symbol_addrs`, which may
// be null, maps the slot symbols and zero-page objects to their
// addresses.
asm_ast::Program apply_redundant_load_elimination(asm_ast::Program prog,
                                                  const AsmContext& ctx,
                                                  const SymbolAddrs* zp_symbol_addrs = nullptr);

// Returns whether `a` and `b` are `name+k1,R` and `name+k2,R` with `k1`
// and `k2` different and less than 256 apart, and nothing in `window`
// writes R, so the two stores in it reach different bytes.
bool same_index_apart(const asm_ast::Type_operand& a, const asm_ast::Type_operand& b,
                      std::span<const asm_ast::Type_instruction> window);

}  // namespace c6502::passes
