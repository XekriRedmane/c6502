// The calls of step 5 of the port (docs/cpp_port.md) that
// tools/capture_passes.py records: the program-level calls of its
// `TAC_STAGE_PASSES` (`c99_to_tac`, the inliner, and the TAC passes of
// `translate_for_codegen` and `_optimized_tac`), and the TAC
// optimizer's calls on one function (`tac_passes`), each pass of its
// driver and SSA construction and destruction. Each has a signature for
// `check-captures`, as the calls of step 4 do in
// src/passes/stage_registry.cpp, and a runner when it is ported.
#pragma once

#include <vector>

#include "passes/stage_registry.h"

namespace c6502::passes::tac_stage {

// Returns the signatures of the calls of step 5, the TAC optimizer's
// calls on one function first and then the program-level ones, each in
// the harness's order.
std::vector<StageSignature> signatures();

}  // namespace c6502::passes::tac_stage
