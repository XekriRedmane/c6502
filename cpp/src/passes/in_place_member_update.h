// The port of passes/in_place_member_update.py, which finds the members
// of static objects that `tac_to_asm` can compute in their own bytes.
#pragma once

#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "asdl/runtime.h"
#include "passes/significant_bytes.h"
#include "tac_ast.h"

namespace c6502::passes {

// One member computed in place: the index of its store, the value `u`
// stored, the object and first byte of the member, and for an update,
// the index of the load and the loaded local `t`.
struct InPlaceUpdate {
  int store = 0;
  std::string u;
  std::string obj;
  asdl::Int base = 0;
  std::optional<int> load;
  std::optional<std::string> t;
};

// Returns the members of `instrs` that can be computed in place.
// `is_local` says whether a name is a local of the function (not a
// static), and `size_of` is `tac_to_asm`'s.
std::vector<InPlaceUpdate> plan_in_place_updates(
    const std::vector<tac_ast::Type_instruction>& instrs, const std::vector<std::string>& params,
    const std::function<bool(std::string_view)>& is_local, const ValSize& size_of);

}  // namespace c6502::passes
