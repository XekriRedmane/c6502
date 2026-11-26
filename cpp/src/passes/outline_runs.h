#pragma once

#include <set>
#include <vector>

#include "asm_ast.h"
#include "passes/asm_context.h"
#include "passes/shared_tails.h"

namespace c6502::passes {

// Returns `prog` with each run of instructions that several places in
// the unit repeat in a function of its own, `<first user>@run<n>`,
// which each place calls, as the comment at the top of outline_runs.cpp
// describes. `zp_names` names the zero-page bytes, which take one-byte
// operands, for the runs' sizes (`sim::instruction_size`), and may be
// null. `slot_addrs` holds the addresses of the unit's `__zpabi_*` and
// `__local_*` slots, whose bytes no `(zp),Y` pointer in a run may have.
// Each function made is appended to `new_functions` when it isn't null.
asm_ast::Program apply_outline_runs(asm_ast::Program prog,
                                    const NameSet* zp_names = nullptr,
                                    const std::set<asdl::Int>& slot_addrs = {},
                                    std::vector<NewFunction>* new_functions = nullptr);

}  // namespace c6502::passes
