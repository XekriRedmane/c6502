// The checks `c6502cpp check-captures` makes of a recorded call of
// tools/capture_passes.py's `STAGE_PASSES` (src/passes/stage_registry.h).
#pragma once

#include <cstddef>
#include <string>

#include "capture.h"
#include "passes/stage_registry.h"

namespace c6502::stage_checks {

struct Result {
  // Why a value of the call didn't make the trip from Python's tagged
  // JSON (or dump) to C++ and back to the same text, or empty when
  // every one did: the input, each argument, each argument the call
  // changed, and the result.
  std::string tables_error;
  // Whether the call is ported, and then whether the C++ result and
  // the arguments it changed match the record, and if not the first
  // line of the output where they differ.
  bool ported = false;
  bool matched = false;
  std::string mismatch;
};

// Checks `call` against the signature `sig`, and runs it when it is
// ported.
Result check(const capture::Call& call, const passes::StageSignature& sig);

}  // namespace c6502::stage_checks
