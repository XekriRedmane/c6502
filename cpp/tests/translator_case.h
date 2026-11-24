// The check of a recorded call of one of tac_to_asm.py's module-level
// functions, `translate_instruction`, `translate_function`,
// `translate_binary`, and `translate_val`, which the Python unit tests
// make on a new `Translator` with no symbol table and which
// tools/record_test_calls.py records as the dumps of the arguments and
// of the result.
#pragma once

#include <exception>
#include <string>
#include <string_view>
#include <vector>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "doctest.h"
#include "passes/asm_context.h"
#include "tac_ast.h"
#include "tac_to_asm.h"
#include "tac_to_asm_translator.h"

namespace c6502::testing {

// Runs `name` on the arguments whose dumps `inputs` holds, with a new
// `Translator` that has no symbol table or type table and the options'
// defaults, as Python's module-level function does, and checks that it
// gives the dumps `expected`: one per instruction, or the one operand or
// function. An `expected` of `exception: <Python type>: <message>`
// checks that the call throws an exception whose message is `<message>`.
inline void check_translator(std::string_view name, const std::vector<std::string>& inputs,
                             const std::vector<std::string>& expected) {
  namespace tac = c6502::tac_ast;
  const passes::AsmContext ctx;
  const tac_to_asm::Options options;
  tac_to_asm::Translator t(nullptr, nullptr, ctx, options);
  std::vector<std::string> got;
  try {
    if (name == "translate_instruction") {
      REQUIRE(inputs.size() == 1);
      for (const auto& i : t.translate_instruction(asdl::load<tac::Type_instruction>(inputs[0]))) {
        got.push_back(asdl::dump(i));
      }
    } else if (name == "translate_binary") {
      REQUIRE(inputs.size() == 4);
      for (const auto& i : t.translate_binary(asdl::load<tac::Type_binary_operator>(inputs[0]),
                                              asdl::load<tac::Type_val>(inputs[1]),
                                              asdl::load<tac::Type_val>(inputs[2]),
                                              asdl::load<tac::Type_val>(inputs[3]))) {
        got.push_back(asdl::dump(i));
      }
    } else if (name == "translate_function") {
      REQUIRE(inputs.size() == 1);
      const auto top = asdl::load<tac::Type_top_level>(inputs[0]);
      const auto* fn = std::get_if<tac::Function>(&top);
      REQUIRE(fn != nullptr);
      got.push_back(asdl::dump(t.translate_function(*fn)));
    } else if (name == "translate_val") {
      REQUIRE(inputs.size() == 1);
      got.push_back(asdl::dump(tac_to_asm::translate_val(asdl::load<tac::Type_val>(inputs[0]))));
    } else {
      FAIL("no translator function ", name);
    }
  } catch (const std::exception& e) {
    REQUIRE(expected.size() == 1);
    const std::string_view want = expected[0];
    constexpr std::string_view kException = "exception: ";
    REQUIRE_MESSAGE(want.substr(0, kException.size()) == kException, e.what());
    const std::string_view rest = want.substr(kException.size());
    CHECK(std::string(e.what()) + "\n" == rest.substr(rest.find(": ") + 2));
    return;
  }
  CHECK(got == expected);
}

}  // namespace c6502::testing
