// The runner of the step 5 call `c99_to_tac` (group A), which
// src/passes/optimization/tac_stage.cpp gives to its signature.
#pragma once

#include "passes/stage_registry.h"

namespace c6502::c99_to_tac {

// Runs `translate_program` on the recorded c99 program with the
// recorded symbol and type tables, and returns the TAC program's dump
// and, when the call changed the program or a table in place, the
// `mutated` line with the program under `input` and each changed table
// by its position, as tools/capture_passes.py writes it.
passes::StageOutput run_c99_to_tac(const capture::Call& call, const passes::AsmContext& ctx,
                                   const json::Value& args, const json::Value& kwargs);

}  // namespace c6502::c99_to_tac
