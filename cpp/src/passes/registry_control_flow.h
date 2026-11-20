// The functions of the registry (registry.cpp) for the control-flow and
// whole-program passes: the branch and jump peepholes, the tail and
// run sharing, jump tables, long branches and short jumps, the slot
// coalescing, and the copies through an index register. Each decodes a
// captured call's arguments, runs the pass, and returns what the
// capture's output section holds, as `RegisteredPass::run` describes.
#pragma once

#include <string>
#include <string_view>

#include "asm_ast.h"
#include "json.h"
#include "passes/asm_context.h"

namespace c6502::passes::registry {

// The type of each run function. A declaration through it, as in
// `RunFn run_x;`, declares a function of that type.
using RunFn = std::string(asm_ast::Program input, const AsmContext& ctx,
                          const json::Value& args, const json::Value& kwargs);

RunFn run_dead_label_drop;
RunFn run_unreachable_drop;
RunFn run_branch_invert;
RunFn run_branch_to_next_drop;
RunFn run_branch_through_jump;
RunFn run_branch_thread;
RunFn run_branch_value_thread;
RunFn run_tail_call;
RunFn run_tail_merge;
RunFn run_cross_jump;
RunFn run_slot_coalescing;
RunFn run_ret_slot_coalescing;
RunFn run_jump_tables;
RunFn run_copy_through_index;
RunFn run_long_branches;
RunFn run_short_jumps;
RunFn run_shared_tails;
RunFn run_outline_runs;

// Returns the text of `s` as Python's `json.dumps` writes a `str`, in
// double quotes, with `"`, `\`, and every character outside printable
// ASCII escaped.
std::string json_quote(std::string_view s);

}  // namespace c6502::passes::registry
