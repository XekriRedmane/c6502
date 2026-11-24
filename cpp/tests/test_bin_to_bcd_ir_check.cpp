// The IR checks of the runtime helper bin16_to_bcd and of the decimal
// flag, ported by hand from tests/test_bin_to_bcd.py's `TestIrCheck`,
// with the messages the Python checks gave. In Python, asm_to_asm2's
// `translate_program` runs `check_asm2` itself when the checks are on.
// In C++ the stage driver runs it after `lower_to_asm2`, so these cases
// make both calls.
#include <string>
#include <variant>
#include <vector>

#include "asm2_ast.h"
#include "asm_ast.h"
#include "doctest.h"
#include "errors.h"
#include "passes/asm_to_asm2.h"
#include "passes/ir_check.h"
#include "rtlib/helpers.h"

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

}  // namespace

TEST_CASE("the runtime helpers lower to asm2 and pass its checks") {
  const ir_check::Enabled on(true);
  c6502::asm_ast::Program prog;
  for (const auto& fn : c6502::rtlib::all_helper_functions()) {
    prog.top_level.push_back(fn);
  }
  CHECK(message_of([&] {
          ir_check::check_asm2(c6502::passes::lower_to_asm2(prog), "asm_to_asm2");
        }) == "");
}

TEST_CASE("bin16_to_bcd without its CLD fails the asm2 check") {
  const ir_check::Enabled on(true);
  c6502::asm_ast::Function fn;
  for (const auto& h : c6502::rtlib::all_helper_functions()) {
    if (h.name == "bin16_to_bcd") {
      fn = h;
    }
  }
  REQUIRE(fn.name == "bin16_to_bcd");
  std::vector<c6502::asm_ast::Type_instruction> kept;
  for (const auto& i : fn.instructions) {
    if (!std::holds_alternative<c6502::asm_ast::ClearDecimal>(i)) {
      kept.push_back(i);
    }
  }
  fn.instructions = kept;
  const c6502::asm_ast::Program prog{.top_level = {fn}};
  CHECK(message_of([&] {
          ir_check::check_asm2(c6502::passes::lower_to_asm2(prog), "asm_to_asm2");
        }) == "after asm_to_asm2, function 'bin16_to_bcd': RTS at 24 without a CLD before it");
}

TEST_CASE("a function other than a helper keeps the rule of the BCD chain") {
  const ir_check::Enabled on(true);
  const c6502::asm2_ast::Program prog{.top_level = {c6502::asm2_ast::Function{
      .name = "f",
      .is_global = true,
      .instructions = {c6502::asm2_ast::SetDecimal{}, c6502::asm2_ast::ClearDecimal{},
                       c6502::asm2_ast::Return{}}}}};
  CHECK(message_of([&] { ir_check::check_asm2(prog, "t"); }) ==
        "after t, function 'f': SED at 0 isn't followed by CLC or SEC; CLD at 1 outside a "
        "BCD chain");
}

TEST_CASE("compiled code may not set the decimal flag") {
  const ir_check::Enabled on(true);
  const c6502::asm_ast::Program prog{.top_level = {c6502::asm_ast::Function{
      .name = "f",
      .is_global = true,
      .instructions = {c6502::asm_ast::SetDecimal{}, c6502::asm_ast::ClearDecimal{}}}}};
  CHECK(message_of([&] { ir_check::check_asm(prog, "t"); }) ==
        "after t, function 'f': SetDecimal outside a runtime helper; ClearDecimal outside a "
        "runtime helper");
}
