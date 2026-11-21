// The runners of the stage calls at the end of code generation (part F2
// of step 4): `address_taken_assignments`, `address_taken_slot_symbols`,
// and `local_slot_names`.
// src/passes/stage_registry.cpp gives them to the signatures of their
// calls.
#pragma once

#include "passes/stage_registry.h"

namespace c6502::passes::stage_frames {

StageOutput run_address_taken_assignments(const capture::Call& call, const AsmContext& ctx,
                                          const json::Value& args, const json::Value& kwargs);
StageOutput run_address_taken_slot_symbols(const capture::Call& call, const AsmContext& ctx,
                                           const json::Value& args, const json::Value& kwargs);
StageOutput run_local_slot_names(const capture::Call& call, const AsmContext& ctx,
                                 const json::Value& args, const json::Value& kwargs);

}  // namespace c6502::passes::stage_frames
