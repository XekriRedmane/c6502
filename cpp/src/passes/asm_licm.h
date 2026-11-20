// The port of passes/asm_licm.py: loop-invariant stores hoisted to the
// loop's preheader.
#pragma once

#include <optional>
#include <string>

#include "asm_ast.h"
#include "passes/asm_context.h"
#include "passes/asm_liveness.h"

namespace c6502::passes {

// Hoists the loop-invariant constant stores and the copies of stable
// bytes of every function's natural loops to the loops' preheaders,
// repeating on each function until nothing more moves, so a store
// hoisted out of an inner loop can leave the outer one too.
// `zp_cells` maps the compiler's zero-page slot symbols and the
// zero-page globals to their addresses, and may be null. The comment
// at the top of asm_licm.cpp gives the shapes and the conditions.
asm_ast::Program apply_licm(asm_ast::Program prog, const AsmContext& ctx,
                            const SymbolAddrs* zp_cells = nullptr);

namespace asm_licm {

// Returns the target of a `Jump` or `Branch`, or nothing for any other
// instruction.
std::optional<std::string> branch_or_jump_target(
    const asm_ast::Type_instruction& inst);

// Returns whether the only label in `instrs[begin..end]` (inclusive)
// that an instruction outside that range jumps or branches to is
// `header_name`. Labels referenced only from inside the range are the
// loop's own control flow.
bool is_single_entry(Instrs instrs, std::size_t begin, std::size_t end,
                     const std::string& header_name);

}  // namespace asm_licm

}  // namespace c6502::passes
