// The port of compile.py's `_peephole_fixedpoint`, the sweep of
// peephole passes that the asm back end repeats until it changes
// nothing.
#pragma once

#include <functional>
#include <string_view>
#include <vector>

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Python's `_PEEPHOLE_FIXEDPOINT_CAP`, the most sweeps the fixed point
// runs before it reports that it didn't converge.
inline constexpr int kPeepholeFixedpointCap = 16;

// One pass of the sweep, under its name in the sweep (the name that
// tools/capture_passes.py records it under), with its keyword arguments
// bound.
struct SweepPass {
  std::string_view name;
  std::function<asm_ast::Program(asm_ast::Program)> run;
};

// Returns the passes of `_peephole_fixedpoint`'s sweep, in its order,
// with the keyword arguments Python gives each one. `zp_slot_symbols`
// are the compiler's own zero-page slot symbols, which several passes
// treat as scratch that's dead at return, and may be null, as Python's
// default of `None` is. `zp_global_symbols` are the user zero-page
// globals. `noreturn` names the functions that never return. The
// pointers and `noreturn` must outlive the sweep.
std::vector<SweepPass> peephole_sweep(const AsmContext& ctx,
                                      const SymbolAddrs* zp_slot_symbols,
                                      const SymbolAddrs* zp_global_symbols,
                                      const NameSet& noreturn);

// Runs the passes of `sweep` in order on `prog`, repeating until a full
// sweep leaves the program equal to what it started with, and returns
// that program. Throws `InternalError` with Python's message when
// `kPeepholeFixedpointCap` sweeps all change it.
asm_ast::Program run_to_fixed_point(asm_ast::Program prog,
                                    const std::vector<SweepPass>& sweep);

// Returns `prog` after the peephole fixed point with the sweep that
// `peephole_sweep` gives, as Python's `_peephole_fixedpoint(prog,
// zp_slot_symbols=..., zp_global_symbols=..., noreturn=...)` does.
asm_ast::Program peephole_fixedpoint(asm_ast::Program prog, const AsmContext& ctx,
                                     const SymbolAddrs* zp_slot_symbols = nullptr,
                                     const SymbolAddrs* zp_global_symbols = nullptr,
                                     const NameSet& noreturn = {});

}  // namespace c6502::passes
