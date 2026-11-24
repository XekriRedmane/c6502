// The Python unit tests of helpers beside the TAC passes that no record
// kind covers, ported by hand in step 8 (group G7), one case per Python
// test with each of its assertions: the inliner's size measure
// (tests/test_inline_functions.py) and the register effects of a call
// to a routine declared `preserves_x` or `preserves_y`
// (test_preserves.py), whose `Call` the recorded `tac_to_asm` calls of
// the same tests give.
#include <string>
#include <variant>

#include "asm_ast.h"
#include "c99_to_tac.h"
#include "doctest.h"
#include "passes/asm_liveness.h"
#include "passes/front_end.h"
#include "passes/optimization/inline_functions.h"

namespace {

namespace tac = c6502::tac_ast;
namespace asm_ir = c6502::asm_ast;

}  // namespace

TEST_CASE("inline_cost: test_inline_functions.py::TestRewrite::test_cost_counts_widths") {
  c6502::passes::FrontEndOptions options;
  options.preprocess = false;
  options.optimize = true;
  std::string err;
  auto front = c6502::passes::front_end(
      "static unsigned long add(unsigned long a, unsigned long b) {\n"
      "    return a + b;\n"
      "}\n"
      "static unsigned char addc(unsigned char a, unsigned char b) {\n"
      "    return a + b;\n"
      "}\n",
      options, err);
  const tac::Program prog =
      c6502::c99_to_tac::translate_program(front.prog, front.symbols, &front.types);
  const auto function = [&](const std::string& name) -> const tac::Function& {
    for (const auto& tl : prog.top_level) {
      if (const auto* fn = std::get_if<tac::Function>(&tl); fn && fn->name == name) {
        return *fn;
      }
    }
    FAIL("no function ", name);
    return std::get<tac::Function>(prog.top_level.at(0));
  };
  CHECK(c6502::passes::inline_cost(function("add"), front.symbols) == 4);
  // The byte add is promoted to `int`: two extends, an add, and a
  // truncate, each with a 2-byte operand.
  CHECK(c6502::passes::inline_cost(function("addc"), front.symbols) == 8);
}

TEST_CASE("preserves: test_preserves.py::TestCalls::test_call_carries_the_set") {
  const asm_ir::Type_instruction call = asm_ir::Call{.name = "f", .preserves = {"X", "Y"}};
  CHECK_FALSE(c6502::passes::kills_x(call));
  CHECK_FALSE(c6502::passes::kills_y(call));
}

TEST_CASE("preserves: test_preserves.py::TestCalls::test_plain_call_clobbers") {
  const asm_ir::Type_instruction call = asm_ir::Call{.name = "f"};
  CHECK(c6502::passes::kills_x(call));
  CHECK_FALSE(c6502::passes::call_preserves(call, "Y"));
}
