// The byte-granular register allocation of the asm optimizer.
#pragma once

#include <optional>
#include <set>
#include <string>
#include <vector>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "ordered_map.h"
#include "passes/optimization/interference.h"
#include "passes/optimization/pool.h"
#include "passes/optimization/register_allocation.h"
#include "passes/optimization_asm/hwreg_eligibility.h"
#include "passes/optimization_asm/liveness.h"

namespace c6502::passes {

// What the pinning to X and Y reads. `liveness` is the function's
// before coalescing, which the check of the writes of X and Y in a
// pinned value's live range reads.
struct HwRegPinning {
  const HwRegEligibility& eligibility;
  const Liveness& liveness;
};

// Colors `graph`'s nodes onto zero-page bytes, first pinning nodes
// eligible for X and Y to those registers when `pinning` is given. Every node of the graph ends up in the result's `assignments`,
// `hwreg_assignments`, or `spilled`. A null or absent argument is left
// out.
//
// `blocked_addrs` holds zero-page bytes that no node may take. The zp_abi
// path reserves the slots of the incoming parameters with it: a zp_abi
// function has its parameters at fixed zero-page addresses on entry, and
// its body's locals must avoid them so as not to overwrite a parameter
// in mid-computation. It blocks them for the whole function, and not
// only while the parameters are live.
//
// `pinning.eligibility` drives the pinning to X and Y, which tries the
// nodes of `hints_x` and `hints_y` against X and Y first and succeeds
// where the graph allows. It holds representatives, so the caller
// projects the names before coalescing through the coalescing's
// `rep_map` first.
//
// `allowed_range`, when given, replaces the pool's split into
// caller-saved and callee-saved bytes. Every color comes from it, and
// `lives_across_call` no longer decides the color, since every byte of
// the range is safe to hold a value across any call the caller knows of
// (the caller is the optimizer's driver, which passes the function's
// private range from zp_local_allocation.h). The range may run above
// $FF, where the addresses assemble in absolute mode, with 3-byte
// instructions in place of 2, and mean the same. Pinning to X or Y still
// depends on the calls in the live range, since a callee writes X and Y
// unless it is declared to preserve them, which has nothing to do with
// the zero-page bytes.
//
// `frame_names` are the names of the pseudos that
// `replace_pseudoregisters` may put in the soft-stack frame, such as a
// soft-stack function's parameters. Each access to one is emitted with
// an `LDY #off` setup, so it counts as a write of Y in a pinned value's
// live range. A spilled node gets a frame slot too, so when a coloring
// spills while it keeps a value in Y, the pinning and the coloring run
// again with the spilled names added, until a run spills nothing new.
//
// Throws `CompileError("RegallocError", ...)` when a `register` local
// can't be pinned to the register it requires.
Coloring color_graph(const asm_ast::Function& fn, const InterferenceGraph& graph,
                     const std::optional<Pool>& pool = std::nullopt,
                     const std::set<asdl::Int>* blocked_addrs = nullptr,
                     const HwRegPinning* pinning = nullptr,
                     const std::optional<Range>& allowed_range = std::nullopt,
                     const OrderedMap<std::string, std::string>* rep_map = nullptr,
                     const NameSet* frame_names = nullptr);

// Returns the registers X and Y that `instr` writes, as the letters 'X'
// and 'Y', for the check of clobbers in a pinned value's live range. An
// access to a pseudo in `frame_names`, which may be null, counts as a
// write of Y. The comment at its definition lists what writes X or Y.
std::vector<char> instr_writes_hwregs(const asm_ast::Type_instruction& instr,
                                      const NameSet* frame_names);

}  // namespace c6502::passes
