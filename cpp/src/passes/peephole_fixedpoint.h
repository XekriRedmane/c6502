// The sweep of peephole passes that the asm back end repeats until it
// changes nothing.
#pragma once

#include <functional>
#include <string_view>
#include <vector>

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// The most sweeps the fixed point runs before it reports that it didn't
// converge.
inline constexpr int kPeepholeFixedpointCap = 16;

// One pass of the sweep, under its name in the sweep, with its
// arguments bound.
struct SweepPass {
  std::string_view name;
  std::function<asm_ast::Program(asm_ast::Program)> run;
};

// Returns the passes of the sweep, in its order. `zp_slot_symbols` are
// the compiler's own zero-page slot symbols, which several passes treat
// as scratch that's dead at return, and may be null.
// `zp_global_symbols` are the user zero-page globals. `noreturn` names the functions that never return. The
// pointers and `noreturn` must outlive the sweep.
std::vector<SweepPass> peephole_sweep(const AsmContext& ctx,
                                      const SymbolAddrs* zp_slot_symbols,
                                      const SymbolAddrs* zp_global_symbols,
                                      const NameSet& noreturn);

// Runs the passes of `sweep` in order on `prog`, repeating until a full
// sweep leaves the program equal to what it started with, and returns
// that program. With the IR checks on, the program is checked after
// each sweep (`ir_check::check_asm`, "after a peephole sweep"). Throws
// `InternalError` when `kPeepholeFixedpointCap` sweeps all change it,
// and for a failed check.
asm_ast::Program run_to_fixed_point(asm_ast::Program prog,
                                    const std::vector<SweepPass>& sweep);

// Returns `prog` after the peephole fixed point with the sweep that
// `peephole_sweep` gives.
asm_ast::Program peephole_fixedpoint(asm_ast::Program prog, const AsmContext& ctx,
                                     const SymbolAddrs* zp_slot_symbols = nullptr,
                                     const SymbolAddrs* zp_global_symbols = nullptr,
                                     const NameSet& noreturn = {});

}  // namespace c6502::passes
