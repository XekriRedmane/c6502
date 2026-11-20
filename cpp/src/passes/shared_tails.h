// The port of passes/shared_tails.py.
#pragma once

#include <set>
#include <string>
#include <utility>
#include <vector>

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// A function that `apply_shared_tails` or `apply_outline_runs` made,
// and the function whose symbol it copies. The Python passes add the
// copy, with internal linkage, to the symbol table they are given, and
// the C++ passes report the pairs for the caller to do that.
struct NewFunction {
  std::string name;
  std::string owner;
  bool operator==(const NewFunction&) const = default;
};

// Returns `prog` with each segment that several functions end with in
// a function of its own, `<owner>@tail<n>`, which each of them jumps
// to, as the comment at the top of shared_tails.cpp describes.
// `zp_names` names the zero-page bytes, which take one-byte operands,
// for the segments' sizes (`sim::instruction_size`), and may be null.
// `slot_addrs` holds the addresses of the unit's `__zpabi_*` and
// `__local_*` slots, whose bytes no `(zp),Y` pointer in a segment may
// have. Each function made is appended to `new_functions` when it isn't
// null.
asm_ast::Program apply_shared_tails(asm_ast::Program prog,
                                    const NameSet* zp_names = nullptr,
                                    const std::set<asdl::Int>& slot_addrs = {},
                                    std::vector<NewFunction>* new_functions = nullptr);

}  // namespace c6502::passes
