#pragma once

#include <vector>

#include "passes/stage_registry.h"

namespace c6502::passes::tac_stage {

// Returns the signatures of the TAC optimizer's calls on one function
// and then those of the program-level TAC calls.
std::vector<StageSignature> signatures();

}  // namespace c6502::passes::tac_stage
