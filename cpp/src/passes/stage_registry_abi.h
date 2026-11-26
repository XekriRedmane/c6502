#pragma once

#include "passes/stage_registry.h"

namespace c6502::passes::stage_abi {

StageOutput run_select_abi(const capture::Call& call, const AsmContext& ctx,
                           const json::Value& args, const json::Value& kwargs);
StageOutput run_fixed_address_layouts(const capture::Call& call, const AsmContext& ctx,
                                      const json::Value& args, const json::Value& kwargs);
StageOutput run_pinned_slot_symbols(const capture::Call& call, const AsmContext& ctx,
                                    const json::Value& args, const json::Value& kwargs);
StageOutput run_choose_global_carry_returns(const capture::Call& call, const AsmContext& ctx,
                                            const json::Value& args, const json::Value& kwargs);
StageOutput run_choose_carry_returns(const capture::Call& call, const AsmContext& ctx,
                                     const json::Value& args, const json::Value& kwargs);
StageOutput run_recursive_pool_functions(const capture::Call& call, const AsmContext& ctx,
                                         const json::Value& args, const json::Value& kwargs);
StageOutput run_pool_eligible(const capture::Call& call, const AsmContext& ctx,
                              const json::Value& args, const json::Value& kwargs);
StageOutput run_cycles(const capture::Call& call, const AsmContext& ctx, const json::Value& args,
                       const json::Value& kwargs);
StageOutput run_hw_stack_cycles(const capture::Call& call, const AsmContext& ctx,
                                const json::Value& args, const json::Value& kwargs);
StageOutput run_check_marked_calls(const capture::Call& call, const AsmContext& ctx,
                                   const json::Value& args, const json::Value& kwargs);
StageOutput run_plan_saves(const capture::Call& call, const AsmContext& ctx,
                           const json::Value& args, const json::Value& kwargs);
StageOutput run_live_out(const capture::Call& call, const AsmContext& ctx, const json::Value& args,
                         const json::Value& kwargs);
StageOutput run_build_cfg(const capture::Call& call, const AsmContext& ctx,
                          const json::Value& args, const json::Value& kwargs);

std::string round_trip_save_plan(const json::Value& v);
std::string round_trip_cfg(const json::Value& v);

}  // namespace c6502::passes::stage_abi
