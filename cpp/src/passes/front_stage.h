// The front end's calls that the frozen capture records name: the
// preprocessor, the parser, the passes on the c99 program through the
// type checker, and the pp_numbers functions, each with a signature
// and a runner for `check-captures`. A record of a call that raised
// holds the exception, which `check-captures` compares with the
// `CompileError` the runner throws.
#pragma once

#include <vector>

#include "passes/stage_registry.h"

namespace c6502::passes::front_stage {

std::vector<StageSignature> signatures();

}  // namespace c6502::passes::front_stage
