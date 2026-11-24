// Tests of the IR checks (src/passes/ir_check.h), the port of
// passes/ir_check.py. Each of test_ir_check.py's 20 tests is a case of
// ir_check_cases.h, and so is each raise site of the Python module,
// with the message Python gives for the same input. The cases of the
// asm2 regions are test_bcd_builtins.py's `TestIrCheck` and the rest of
// `decimal_region_problems`.
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "asm2_ast.h"
#include "asm_ast.h"
#include "c99_ast.h"
#include "doctest.h"
#include "errors.h"
#include "ir_check_cases.h"
#include "passes/decimal_arith.h"
#include "passes/ir_check.h"
#include "tac_ast.h"

namespace asdl = c6502::asdl;
namespace cases = c6502::testing::ir_check_cases;
namespace ir_check = c6502::passes::ir_check;
using c6502::InternalError;

namespace {

// Returns the message of the `InternalError` that `check` throws, or ""
// when it throws nothing.
template <class F>
std::string message_of(F&& check) {
  try {
    check();
  } catch (const InternalError& e) {
    return e.what();
  }
  return "";
}

c6502::passes::SymbolTable symbols_of(const cases::Names& names) {
  c6502::passes::SymbolTable out;
  for (const std::string_view n : names) {
    out.insert_or_assign(std::string(n), c6502::passes::Symbol{.type = c6502::c99_ast::Int{},
                                                                .attrs = c6502::passes::LocalAttr{}});
  }
  return out;
}

c6502::passes::NameSet names_of(const cases::Names& names) {
  c6502::passes::NameSet out;
  for (const std::string_view n : names) {
    out.emplace(n);
  }
  return out;
}

}  // namespace

TEST_CASE("the TAC checks give Python's messages") {
  for (const auto& c : cases::tac_cases()) {
    CAPTURE(c.name);
    for (const auto& check : c.checks) {
      const auto fn = std::get<c6502::tac_ast::Function>(
          asdl::load<c6502::tac_ast::Type_top_level>(check.fn));
      std::optional<c6502::passes::SymbolTable> symbols;
      if (check.symbols) symbols = symbols_of(*check.symbols);
      std::optional<c6502::passes::NameSet> ssa;
      if (check.ssa) ssa = names_of(*check.ssa);
      const ir_check::Enabled on(check.on);
      CHECK(message_of([&] {
              ir_check::check_tac(fn, "test", symbols ? &*symbols : nullptr,
                                  ssa ? &*ssa : nullptr);
            }) == check.message);
    }
  }
}

TEST_CASE("the asm checks give Python's messages") {
  const ir_check::Enabled on(true);
  for (const auto& c : cases::asm_cases()) {
    CAPTURE(c.name);
    for (const auto& check : c.checks) {
      const auto prog = asdl::load<c6502::asm_ast::Program>(check.prog);
      CHECK(message_of([&] { ir_check::check_asm(prog, "test", check.pseudos, check.phis); }) ==
            check.message);
    }
  }
}

TEST_CASE("the decimal regions' problems are Python's") {
  for (const auto& c : cases::region_cases()) {
    CAPTURE(c.name);
    for (const auto& check : c.checks) {
      const auto fn = std::get<c6502::asm2_ast::Function>(
          asdl::load<c6502::asm2_ast::Type_top_level>(check.instrs));
      std::string got;
      for (const auto& p : ir_check::decimal_region_problems(fn.instructions)) {
        if (!got.empty()) got += "; ";
        got += p;
      }
      CHECK(got == check.problems);
    }
  }
}

TEST_CASE("the asm2 checks give Python's messages") {
  const ir_check::Enabled on(true);
  for (const auto& c : cases::asm2_cases()) {
    CAPTURE(c.name);
    for (const auto& check : c.checks) {
      const auto prog = asdl::load<c6502::asm2_ast::Program>(check.prog);
      CHECK(message_of([&] { ir_check::check_asm2(prog, "test"); }) == check.message);
    }
  }
}

TEST_CASE("the checks are off unless enabled, and Enabled restores them") {
  const auto prog = asdl::load<c6502::asm_ast::Program>(
      "Program([Function(\"f\" true [] [Branch(EQ \".x\")])])");
  CHECK_FALSE(ir_check::enabled());
  CHECK(message_of([&] { ir_check::check_asm(prog, "test"); }).empty());
  {
    const ir_check::Enabled on(true);
    CHECK(ir_check::enabled());
    {
      const ir_check::Enabled off(false);
      CHECK(message_of([&] { ir_check::check_asm(prog, "test"); }).empty());
    }
    CHECK(message_of([&] { ir_check::check_asm(prog, "test"); }) ==
          "after test, function 'f': branch to missing label .x");
  }
  CHECK_FALSE(ir_check::enabled());
}

TEST_CASE("form_problems names each operand the chain can't take") {
  const auto instr = std::get<c6502::asm_ast::DecimalArith>(
      asdl::load<c6502::asm_ast::Type_instruction>(
          "DecimalArith(false [Reg(A)] [IndirectY] [ImmLabelLow(\"t\" 0)])"));
  CHECK(c6502::passes::decimal_arith::form_problems(instr) ==
        std::vector<std::string>{"DecimalArith reads Reg", "DecimalArith reads IndirectY",
                                 "DecimalArith writes ImmLabelLow"});
}
