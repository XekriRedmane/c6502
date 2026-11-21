// Tests of the parts of the TAC CFG (src/passes/optimization/cfg.*) that
// the recorded cases of tests/test_cfg.py don't reach: flattening a
// graph back into a function, and a jump to a label no block has.
#include <string>

#include "asdl/runtime.h"
#include "doctest.h"
#include "errors.h"
#include "passes/optimization/cfg.h"
#include "tac_ast.h"

namespace cfg = c6502::passes::tac_cfg;
namespace tac = c6502::tac_ast;

namespace {

// A function that falls through from `main` into `.end`, after a
// jump over a dead block.
const char* kFunction =
    "Function(\"main\" true [\"@1.p\"] [\n"
    "  Jump(\".end\")\n"
    "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
    "  Label(\".end\")\n"
    "  Ret(Constant(ConstInt(0)))\n"
    "])\n";

}  // namespace

TEST_CASE("cfg_to_function gives back an unchanged function") {
  const auto fn = c6502::asdl::load<tac::Function>(kFunction);
  CHECK(c6502::asdl::dump(cfg::cfg_to_function(fn, cfg::build_cfg(fn))) == kFunction);
}

TEST_CASE("cfg_to_function drops the blocks left out of block_order") {
  const auto fn = c6502::asdl::load<tac::Function>(kFunction);
  cfg::Cfg graph = cfg::build_cfg(fn);
  REQUIRE(graph.block_order.size() == 3);
  // The block of the dead `Copy` goes from the order, and the block of
  // `.end` from the blocks, as unreachable-code elimination drops one.
  graph.block_order.erase(graph.block_order.begin() + 1);
  CHECK(c6502::asdl::dump(cfg::cfg_to_function(fn, graph)) ==
        "Function(\"main\" true [\"@1.p\"] [\n"
        "  Jump(\".end\")\n"
        "  Label(\".end\")\n"
        "  Ret(Constant(ConstInt(0)))\n"
        "])\n");
  graph.blocks.erase(graph.block_order.back());
  CHECK(c6502::asdl::dump(cfg::cfg_to_function(fn, graph)) ==
        "Function(\"main\" true [\"@1.p\"] [\n"
        "  Jump(\".end\")\n"
        "])\n");
}

TEST_CASE("build_cfg rejects a jump to a label no block starts with") {
  const auto fn = c6502::asdl::load<tac::Function>(
      "Function(\"main\" true [] [\n"
      "  Jump(\".nowhere\")\n"
      "])\n");
  CHECK_THROWS_AS(cfg::build_cfg(fn), c6502::InternalError);
}
