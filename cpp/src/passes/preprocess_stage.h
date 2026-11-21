// The runner of the step 6 call `preprocess` (preprocessor.py), which
// check-captures runs on a recorded call with this machine's files.
#pragma once

#include "passes/stage_registry.h"

namespace c6502::passes::preprocess_stage {

// Runs `preprocess(source, pcpp_args, filename)` on the record's source
// text and arguments, reading the files it includes from their paths
// and c6502's headers from include/ under the current directory, which
// is where Python finds them when check-captures runs from the
// repository's root. Returns the text, or the error, with the warnings
// and what the call wrote to stderr.
StageOutput run(const capture::Call& call, const AsmContext& ctx, const json::Value& args,
                const json::Value& kwargs);

}  // namespace c6502::passes::preprocess_stage
