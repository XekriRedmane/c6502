// The check of a recorded session of c99_to_tac.py's `Translator`: the
// calls that a Python unit test made of one `Translator`'s methods on
// c99 nodes it built, `translate_exp`, `translate_statement`, and the
// rest, in order, which tools/record_test_calls.py records as the
// dumps of each call's argument, of the instructions in the list before
// and after the call, and of its result (docs/cpp_port.md, step 8,
// group G7). The steps share the translator, so its temporaries and
// labels are numbered on from one step to the next, as in Python.
#pragma once

#include <exception>
#include <string>
#include <string_view>
#include <vector>

#include "asdl/runtime.h"
#include "c99_ast.h"
#include "c99_to_tac_translator.h"
#include "doctest.h"
#include "errors.h"
#include "json.h"
#include "tac_ast.h"
#include "tagged_json.h"

namespace c6502::testing {

// One call of a session. `arg` is the dump of the call's one argument,
// or empty for `make_temporary_variable_name` called without a type.
// `before` and `after` are the dumps of the list's instructions, for a
// method that takes the list (`takes_list`). `result` is the dump of
// what the call returned, the name for `make_temporary_variable_name`,
// empty for a call that returns nothing, or `exception: <Python type>:
// <message>` for one that raised.
struct C99Step {
  std::string_view method;
  std::string arg;
  bool takes_list = false;
  std::vector<std::string> before;
  std::string result;
  std::vector<std::string> after;
};

// Runs the steps on one C++ `Translator` made with the symbol table
// `symbols` (its tagged JSON) and no type table, and checks each step's
// result and list, and the symbol table after the last step against
// `symbols_after`.
inline void check_c99_session(std::string_view symbols, const std::vector<C99Step>& steps,
                              std::string_view symbols_after) {
  namespace c99 = ::c6502::c99_ast;
  namespace tac = ::c6502::tac_ast;
  namespace ct = ::c6502::c99_to_tac;
  passes::SymbolTable table =
      tagged::SymbolTableC::decode(json::parse(std::string(symbols)));
  ct::Translator t(table, nullptr);
  for (const C99Step& step : steps) {
    INFO("step ", step.method, " on ", step.arg);
    ct::Instrs instrs;
    for (const auto& i : step.before) {
      instrs.push_back(asdl::load<tac::Type_instruction>(i));
    }
    std::string got;
    try {
      if (step.method == "translate_exp") {
        const auto val = t.translate_exp(asdl::load<c99::Type_exp>(step.arg), instrs);
        if (val) {
          got = asdl::dump(*val);
        }
      } else if (step.method == "translate_statement") {
        t.translate_statement(asdl::load<c99::Type_statement>(step.arg), instrs);
      } else if (step.method == "translate_declaration") {
        t.translate_declaration(asdl::load<c99::Type_declaration>(step.arg), instrs);
      } else if (step.method == "translate_block_item") {
        t.translate_block_item(asdl::load<c99::Type_block_item>(step.arg), instrs);
      } else if (step.method == "translate_function") {
        const auto def = asdl::load<c99::Type_function_definition>(step.arg);
        got = asdl::dump(tac::Type_top_level{t.translate_function(std::get<c99::Function>(def))});
      } else if (step.method == "translate_program") {
        auto prog = asdl::load<c99::Program>(step.arg);
        got = asdl::dump(t.translate_program(prog));
      } else if (step.method == "make_temporary_variable_name") {
        if (step.arg.empty()) {
          got = t.make_temporary_variable_name(nullptr);
        } else {
          const auto type = asdl::load<c99::Type_data_type>(step.arg);
          got = t.make_temporary_variable_name(&type);
        }
      } else {
        FAIL("no Translator method ", step.method);
      }
    } catch (const std::exception& e) {
      const std::string_view want = step.result;
      constexpr std::string_view kException = "exception: ";
      REQUIRE_MESSAGE(want.substr(0, kException.size()) == kException, e.what());
      const std::string_view rest = want.substr(kException.size());
      std::string message(rest.substr(rest.find(": ") + 2));
      // Python's message for a node that an earlier pass should have
      // rejected ends with the node's repr, `; got Constant(...)`, which
      // the C++ port's `InternalError` leaves out.
      if (dynamic_cast<const InternalError*>(&e) != nullptr) {
        if (const auto got = message.find("; got "); got != std::string::npos) {
          message = message.substr(0, got) + "\n";
        }
      }
      CHECK(std::string(e.what()) + "\n" == message);
      return;
    }
    CHECK(got == step.result);
    if (step.takes_list) {
      std::vector<std::string> dumps;
      for (const auto& i : instrs) {
        dumps.push_back(asdl::dump(i));
      }
      CHECK(dumps == step.after);
    }
  }
  CHECK(tagged::encode<tagged::SymbolTableC>(table) == symbols_after);
}

}  // namespace c6502::testing
