// The port of passes/recursion_saves.py, `[[c6502::save_hardware_stack]]`:
// the cycles of the call graph whose recursive calls are all marked,
// the check of the marked calls, and the plan of the values `tac_to_asm`
// saves on the hardware stack around each recursive call. `live_out`
// and `defs_in`, which `find_shares` reads too, are in namespace
// `recursion_saves`, since Python's `defs_in` differs from
// var_visit.h's.
#pragma once

#include <map>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "ordered_map.h"
#include "passes/asm_context.h"
#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::passes {

// The direct call edges among the functions a program defines, each
// function mapped to its callees. Python builds this dict from a set of
// names, so its order follows the hashes of strings, which change from
// run to run, and nothing reads the order. The C++ map is sorted.
using CallGraph = std::map<std::string, NameSet, std::less<>>;

// Each function on a cycle mapped to its cycle's members, in the order
// `cycles` finds them, as Python's dict holds them.
using Cycles = OrderedMap<std::string, NameSet>;

// Returns the direct call edges among the functions defined in `prog`.
// A call that abandons its caller isn't an edge (call_edges.h), so it
// doesn't make a cycle.
CallGraph call_graph(const tac_ast::Program& prog);

// Returns each function on a cycle of `graph` mapped to its cycle (its
// strongly connected component). A function that calls only itself is a
// cycle of one. Functions on no cycle aren't keys.
Cycles cycles(const CallGraph& graph);

// Throws `CompileError("RecursionSaveError", ...)` for a marked call
// that can't recurse: one to a function that can't call the caller
// back, or one that never returns.
void check_marked_calls(const tac_ast::Program& prog);

// Returns the cycles whose every call from one member to another is
// marked, as `cycles` maps them. Only these can have private pools,
// since a call that recurses without the mark would need its caller's
// bytes saved somewhere other than the hardware stack.
Cycles hw_stack_cycles(const tac_ast::Program& prog);

// One saved value: its name and the TAC value its bytes come from (a
// push) or go to (a pull).
using SavedValue = std::pair<std::string, tac_ast::Type_val>;

// Where `tac_to_asm` saves and restores in one function, keyed by TAC
// instruction index. `push_before[i]` are pushed just before marked call
// `i`, `push_after[i]` just after instruction `i` (their last definition
// before the call), and `pop_after[i]` are pulled after marked call `i`,
// in that order. The instructions in `skip` aren't translated, since a
// pull already stored their value. The maps are in the order Python's
// dicts hold them.
struct SavePlan {
  OrderedMap<int, std::vector<SavedValue>> push_before;
  OrderedMap<int, std::vector<SavedValue>> push_after;
  OrderedMap<int, std::vector<SavedValue>> pop_after;
  std::set<int> skip;
  bool operator==(const SavePlan&) const = default;
};

// Returns the saves around `fn`'s calls to the functions in `members`
// (its cycle). A value is saved when it's live across the call and could
// be in `fn`'s private pool: not a static, not a parameter (those are in
// the frame), and not an object whose address is taken or that's
// indexed by name (also in the frame).
SavePlan plan_saves(const tac_ast::Function& fn, const NameSet& members,
                    const SymbolTable& symbols);

namespace recursion_saves {

// Returns the names `instr` writes, as var_visit.h's `defs_in` does,
// with an `IndirectIndexedLoad`'s destination too, which that one
// leaves out. Without it that value would look live everywhere. The
// views are into `instr`.
std::vector<std::string_view> defs_in(const tac_ast::Type_instruction& instr);

// Returns the names live just after each instruction of `fn`.
std::vector<NameSet> live_out(const tac_ast::Function& fn);

}  // namespace recursion_saves

}  // namespace c6502::passes
