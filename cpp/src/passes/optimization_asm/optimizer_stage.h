// The runners of the stage calls of the asm optimizer's program level,
// which src/passes/stage_registry.cpp registers: `asm_optimizer`
// (`optimize_program`), `compute_local_bytes`,
// `compute_address_taken_bytes`, and the calls inside the optimizer that
// the harness records on their own, `optimize_function`,
// `apply_coloring`, `fold_const_statics`, and `dead_static_elimination`.
#pragma once

#include <string>

#include "passes/stage_registry.h"

namespace c6502::passes::optimizer_stage {

// The codec of `optimize_function`'s result, a function's dump and its
// coloring.
std::string round_trip_optimized_function(const json::Value& v);

StageOutput run_asm_optimizer(const capture::Call& call, const AsmContext& ctx,
                              const json::Value& args, const json::Value& kwargs);
StageOutput run_compute_local_bytes(const capture::Call& call, const AsmContext& ctx,
                                    const json::Value& args, const json::Value& kwargs);
StageOutput run_compute_address_taken_bytes(const capture::Call& call, const AsmContext& ctx,
                                            const json::Value& args, const json::Value& kwargs);
StageOutput run_optimize_function(const capture::Call& call, const AsmContext& ctx,
                                  const json::Value& args, const json::Value& kwargs);
StageOutput run_apply_coloring(const capture::Call& call, const AsmContext& ctx,
                               const json::Value& args, const json::Value& kwargs);
StageOutput run_fold_const_statics(const capture::Call& call, const AsmContext& ctx,
                                   const json::Value& args, const json::Value& kwargs);
StageOutput run_dead_static_elimination(const capture::Call& call, const AsmContext& ctx,
                                        const json::Value& args, const json::Value& kwargs);

}  // namespace c6502::passes::optimizer_stage
