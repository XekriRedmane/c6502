// The C++ side of tools/capture_helpers.py: the values of the shared
// asm helpers on a recorded function, program, or TAC function, written
// in the encoding the Python side writes, so the two can be compared
// line by line (`c6502cpp check-helpers`).
#pragma once

#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "asm_ast.h"
#include "json.h"
#include "passes/asm_context.h"
#include "tac_ast.h"

namespace c6502::helper_checks {

// The context line of a function record, as `make_context` writes it.
struct HelperContext {
  // The layout and the user zero-page globals, with no pointer proof.
  passes::AsmContext ctx;
  std::optional<passes::SymbolAddrs> zp_slot_symbols;
  std::optional<passes::NameSet> zp_names;
  std::set<asdl::Int> slot_addrs;
  passes::NameSet pointer_proof;
};

// Returns the context that the JSON object `v` holds.
HelperContext parse_context(const json::Value& v);

// One helper's value, by its name in the record.
using Values = std::vector<std::pair<std::string, std::string>>;

// Returns the values `function_values` in tools/capture_helpers.py
// gives for `fn` in `ctx`, in the same order.
Values function_values(const asm_ast::Function& fn, const HelperContext& ctx);

// Returns `unreachable_by_pointer` of `prog`.
Values program_values(const asm_ast::Program& prog);

// Returns `call_edges`'s values on the TAC function `fn`.
Values tac_values(const tac_ast::Function& fn);

// Returns the values `stage_values` in tools/capture_helpers.py gives
// for the code generation helpers on a `tables` record, whose JSON
// object `v` holds the TAC program, the symbol and type tables, the
// shares, and the runtime layout that `find_shares` was given and
// returned (src/stage_helper_checks.cpp).
Values stage_values(const json::Value& v);

// Returns the values of the TAC CFG and its analyses on each function
// of `fns`, and of `recursion_saves::defs_in` on each instruction of
// `instrs`, as `_cfg_values` in tools/capture_helpers.py gives them.
Values cfg_values(const std::vector<const tac_ast::Function*>& fns,
                  const std::vector<const tac_ast::Type_instruction*>& instrs);

// Returns `parse_pseudo_name` of each name of a `pseudos` record, whose
// JSON object `v` holds the names.
Values pseudo_values(const json::Value& v);

// Returns the values `tac_opt_values` in tools/capture_helpers.py gives
// for the TAC optimizer's helpers on a `tacopt` record, whose JSON
// object `v` holds the function a pass was given, the symbols of its
// operands, and the SSA names (src/tac_helper_checks.cpp).
Values tac_opt_values(const json::Value& v);

// Returns how many values the recorded value of helper `name` holds:
// the number of fields separated by spaces for a helper whose value is
// a list, one for a CFG and for the count of operands, and otherwise
// the number of bits.
std::size_t value_count(const std::string& name, const std::string& value);

}  // namespace c6502::helper_checks
