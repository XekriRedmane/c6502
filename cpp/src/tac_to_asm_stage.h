// The runner of the stage call `tac_to_asm` (group C of step 4), which
// src/passes/stage_registry.cpp gives to its signature.
#pragma once

#include "passes/stage_registry.h"

namespace c6502::tac_to_asm {

// Runs `translate_program` on the recorded TAC program with the
// recorded symbol and type tables and keyword arguments, and returns the
// asm program's dump and, when the call added locals to the symbol
// table, the `mutated` line with the table after the call.
passes::StageOutput run_tac_to_asm(const capture::Call& call, const passes::AsmContext& ctx,
                                   const json::Value& args, const json::Value& kwargs);

}  // namespace c6502::tac_to_asm
