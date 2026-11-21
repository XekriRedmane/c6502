// The calls of step 6 of the port (docs/cpp_port.md), the front end,
// which tools/capture_passes.py records under the names of its `FRONT`:
// the preprocessor, the parser, and the passes on the c99 program
// through the type checker (`FRONT_PASSES`), and the functions of
// pp_numbers.py where the lexer, the parser, and the preprocessor call
// them (`FRONT_HELPERS`). Each has a signature for `check-captures`, as
// the calls of steps 4 and 5 do, and a runner when it is ported. A
// record of a call that raised holds the exception, which
// `check-captures` compares with the `CompileError` the runner throws.
#pragma once

#include <vector>

#include "passes/stage_registry.h"

namespace c6502::passes::front_stage {

// Returns the signatures of the calls of step 6, in the order of the
// harness's `FRONT`.
std::vector<StageSignature> signatures();

}  // namespace c6502::passes::front_stage
