#pragma once

#include <string>

#include "passes/stage_registry.h"
#include "passes/zp_link_metadata.h"

namespace c6502::passes::stage_sharing {

StageOutput run_plan_zero_page(const capture::Call& call, const AsmContext& ctx,
                               const json::Value& args, const json::Value& kwargs);
StageOutput run_access_weights(const capture::Call& call, const AsmContext& ctx,
                               const json::Value& args, const json::Value& kwargs);
StageOutput run_find_shares(const capture::Call& call, const AsmContext& ctx,
                            const json::Value& args, const json::Value& kwargs);
StageOutput run_allocate_zp_slots(const capture::Call& call, const AsmContext& ctx,
                                  const json::Value& args, const json::Value& kwargs);
StageOutput run_pinned_param_copies(const capture::Call& call, const AsmContext& ctx,
                                    const json::Value& args, const json::Value& kwargs);
StageOutput run_slot_aliases(const capture::Call& call, const AsmContext& ctx,
                             const json::Value& args, const json::Value& kwargs);
StageOutput run_allocate_function_locals(const capture::Call& call, const AsmContext& ctx,
                                         const json::Value& args, const json::Value& kwargs);
StageOutput run_local_slot_symbols(const capture::Call& call, const AsmContext& ctx,
                                   const json::Value& args, const json::Value& kwargs);
StageOutput run_changed_param_slots(const capture::Call& call, const AsmContext& ctx,
                                    const json::Value& args, const json::Value& kwargs);
StageOutput run_preferred_symbols(const capture::Call& call, const AsmContext& ctx,
                                  const json::Value& args, const json::Value& kwargs);
StageOutput run_build_metadata(const capture::Call& call, const AsmContext& ctx,
                               const json::Value& args, const json::Value& kwargs);

std::string round_trip_link_metadata(const json::Value& v);

// Returns the tagged JSON of `meta`, in the form the frozen captures
// hold a `LinkMetadata`.
std::string link_metadata_json(const LinkMetadata& meta);

}  // namespace c6502::passes::stage_sharing
