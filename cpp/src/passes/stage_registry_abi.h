// The runners of the stage calls of the ABI and the call graph (group A
// of step 4): `select_abi`, `fixed_address_layouts`,
// `pinned_slot_symbols`, the carry-return choices, the pool eligibility,
// the recursion saves, and the TAC CFG. src/passes/stage_registry.cpp
// gives them to the signatures of their calls, and the codecs of the
// results that only these calls return are here too.
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

// The round trips of the results only these calls return: a `SavePlan`
// and a TAC `CFG`.
std::string round_trip_save_plan(const json::Value& v);
std::string round_trip_cfg(const json::Value& v);

}  // namespace c6502::passes::stage_abi
