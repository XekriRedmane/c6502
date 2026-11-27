// Tests of the runtime helpers (src/rtlib/helpers.h) and of the HARGS
// footprints measured on them.
#include <string>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "compile_case.h"
#include "doctest.h"
#include "errors.h"
#include "passes/asm_operands.h"
#include "passes/zp_globals.h"
#include "rtlib/helpers.h"

namespace {

using namespace c6502;

TEST_CASE("the helpers come in the library's order") {
  std::vector<std::string> names;
  for (const auto& fn : rtlib::all_helper_functions()) names.push_back(fn.name);
  const std::vector<std::string> expected{
      "mul8",      "mul16",     "mul32",  "udivmod8", "sdivmod8", "udivmod16", "sdivmod16",
      "udivmod32", "sdivmod32", "asl8",   "asr8",     "lsr8",     "asl16",     "asr16",
      "lsr16",     "asl32",     "asr32",  "lsr32",    "bin16_to_bcd"};
  CHECK(names == expected);
}

// cpp/tests/frozen/rtlib_helpers.txt is the dump of rtlib/helpers.py's
// helpers, which cpp/generated/rtlib_helpers.cpp held, written by
// ir_dump.dump before step 9 of the port removed both.
TEST_CASE("the builders give the dump of rtlib/helpers.py's helpers") {
  CHECK(rtlib::helper_dump() == suite::read_repo_file("cpp/tests/frozen/rtlib_helpers.txt"));
}

TEST_CASE("the dump reads back as the functions") {
  const auto prog = asdl::load<asm_ast::Program>(rtlib::helper_dump());
  REQUIRE(prog.top_level.size() == rtlib::all_helper_functions().size());
  for (std::size_t k = 0; k < prog.top_level.size(); ++k) {
    CHECK(std::get<asm_ast::Function>(prog.top_level[k]) == rtlib::all_helper_functions()[k]);
  }
}

TEST_CASE("bin16_to_bcd is the one decimal-mode helper") {
  CHECK(rtlib::decimal_mode_helpers() == std::vector<std::string>{"bin16_to_bcd"});
}

// The footprints Python's `helper_footprints` (passes/runtime_layout.py)
// gave on rtlib/helpers.py before step 9 of the port, in the library's
// order.
TEST_CASE("every helper's footprint counts the helpers it calls") {
  const std::vector<std::pair<std::string, int>> expected{
      {"mul8", 5},       {"mul16", 10},    {"mul32", 20},     {"udivmod8", 4},
      {"sdivmod8", 6},   {"udivmod16", 9}, {"sdivmod16", 11}, {"udivmod32", 20},
      {"sdivmod32", 22}, {"asl8", 3},      {"asr8", 3},       {"lsr8", 3},
      {"asl16", 5},      {"asr16", 5},     {"lsr16", 5},      {"asl32", 9},
      {"asr32", 9},      {"lsr32", 9},     {"bin16_to_bcd", 7}};
  const passes::HelperFootprints& fp = passes::helper_footprints();
  CHECK(fp.size() == expected.size());
  for (const auto& [name, bytes] : expected) {
    CAPTURE(name);
    CHECK(fp.at(name) == bytes);
  }
}

// `library_text` writes the helpers with the classic runtime layout,
// since the link's layout isn't chosen until the helpers' HARGS need is
// known. The layout changes only how `asm_emit` names a `(zp),Y` base,
// which it finds by address, so the text is the same for every layout
// while no helper has one.
TEST_CASE("no helper reaches memory through a zero-page pointer named by its address") {
  for (const auto& fn : rtlib::all_helper_functions()) {
    for (const auto& instr : fn.instructions) {
      passes::for_each_operand<true>(instr, [&](const asm_ast::Type_operand& op) {
        CAPTURE(fn.name);
        CHECK_FALSE(asdl::isa<asm_ast::IndirectZp, asm_ast::IndirectZpY>(op));
      });
    }
  }
}

asm_ast::Function fn(const std::string& name, std::vector<asm_ast::Type_instruction> instrs) {
  return asm_ast::Function{
      .name = name, .is_global = true, .params = {}, .instructions = std::move(instrs)};
}

TEST_CASE("helpers that call each other in a cycle are refused") {
  const std::vector<asm_ast::Function> fns{
      fn("f", {asm_ast::Call{.name = "g"}}),
      fn("g", {asm_ast::Jump{.target = "f"}}),
  };
  CHECK_THROWS_WITH_AS(rtlib::check_acyclic(fns),
                       "the runtime helpers call each other in a cycle: f -> g -> f",
                       InternalError);
}

TEST_CASE("a helper called from two others is no cycle") {
  const std::vector<asm_ast::Function> fns{
      fn("f", {asm_ast::Call{.name = "h"}}),
      fn("g", {asm_ast::Call{.name = "h"}, asm_ast::Call{.name = "fadd"}}),
      fn("h", {}),
  };
  CHECK_NOTHROW(rtlib::check_acyclic(fns));
}

}  // namespace
