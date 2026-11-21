// A check of the TAC CFG's values on one function, for the tests that
// port tests/test_cfg.py as the functions it builds a CFG of.
#pragma once

#include <string>
#include <string_view>

#include "asdl/runtime.h"
#include "doctest.h"
#include "helper_checks.h"
#include "stage_case.h"
#include "tac_ast.h"

namespace c6502::testing {

// Checks that the CFG of the function whose dump is `fn_dump`, and its
// reverse postorder, dominators, dominator tree, dominance frontiers,
// and natural loops, have the values `expected` gives, one `name value`
// line each, as `_cfg_values` writes them.
inline void check_tac_cfg_case(std::string_view fn_dump, std::string_view expected) {
  const auto fn = asdl::load<tac_ast::Function>(fn_dump);
  std::string got;
  for (const auto& [name, value] : helper_checks::cfg_values({&fn}, {})) {
    if (name != "recursion_saves_defs_in") {
      got += name + " " + value + "\n";
    }
  }
  CHECK(got == expected);
}

}  // namespace c6502::testing
