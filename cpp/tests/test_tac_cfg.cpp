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

// tests/test_cfg.py's two tests of `cfg_to_function`, ported by hand in
// step 8 (group G7), since the recorded cases of tac_cfg_case.h check
// only the graph that `build_cfg` gives for their functions.
TEST_CASE("cfg_to_function: TestCFGToFunction::test_round_trip") {
  // Building a CFG and flattening it at once gives the same function,
  // with its name, parameters, and linkage.
  const char* text =
      "Function(\"foo\" true [\"p\"] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  JumpIfTrue(Var(\"c\") \"end\")\n"
      "  Copy(Constant(ConstInt(2)) Var(\"y\"))\n"
      "  Label(\"end\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n";
  const auto fn = c6502::asdl::load<tac::Function>(text);
  CHECK(c6502::asdl::dump(cfg::cfg_to_function(fn, cfg::build_cfg(fn))) == text);
}

TEST_CASE("cfg_to_function: TestCFGToFunction::test_dropping_block_removes_its_instructions") {
  const auto fn = c6502::asdl::load<tac::Function>(
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Jump(\"L1\")\n"
      "  Copy(Constant(ConstInt(2)) Var(\"y\"))\n"
      "  Label(\"L1\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n");
  cfg::Cfg graph = cfg::build_cfg(fn);
  // The second block is the dead one, the lone Copy between the Jump
  // and the labeled block.
  const int dead = graph.block_order.at(1);
  graph.block_order.erase(graph.block_order.begin() + 1);
  graph.blocks.erase(dead);
  CHECK(c6502::asdl::dump(cfg::cfg_to_function(fn, graph)) ==
        "Function(\"main\" true [] [\n"
        "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
        "  Jump(\"L1\")\n"
        "  Label(\"L1\")\n"
        "  Ret(Constant(ConstInt(0)))\n"
        "])\n");
}
