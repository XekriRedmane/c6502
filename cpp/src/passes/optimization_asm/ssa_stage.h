// The runners of the stage calls of the asm SSA round trip, which
// src/passes/stage_registry.cpp registers: `to_ssa`, `copy_propagate`,
// `backward_copy_propagate`, `byte_dce`, `absorb_zero_load`, `from_ssa`,
// and `order_parallel_copies`.
#pragma once

#include "passes/stage_registry.h"

namespace c6502::passes::ssa_stage {

// The codec of a list of asm instructions, each as its dump, which
// `order_parallel_copies` takes and returns.
std::string round_trip_instructions(const json::Value& v);

StageOutput run_to_ssa(const capture::Call& call, const AsmContext& ctx, const json::Value& args,
                       const json::Value& kwargs);
StageOutput run_copy_propagate(const capture::Call& call, const AsmContext& ctx,
                               const json::Value& args, const json::Value& kwargs);
StageOutput run_backward_copy_propagate(const capture::Call& call, const AsmContext& ctx,
                                        const json::Value& args, const json::Value& kwargs);
StageOutput run_byte_dce(const capture::Call& call, const AsmContext& ctx, const json::Value& args,
                         const json::Value& kwargs);
StageOutput run_absorb_zero_load(const capture::Call& call, const AsmContext& ctx,
                                 const json::Value& args, const json::Value& kwargs);
StageOutput run_from_ssa(const capture::Call& call, const AsmContext& ctx, const json::Value& args,
                         const json::Value& kwargs);
StageOutput run_order_parallel_copies(const capture::Call& call, const AsmContext& ctx,
                                      const json::Value& args, const json::Value& kwargs);

}  // namespace c6502::passes::ssa_stage
