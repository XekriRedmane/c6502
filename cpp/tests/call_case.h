// Checks of recorded calls of the functions of step 3 (emission and the
// work before it), for the tests that port a Python module's unit tests
// as the calls they make, each with what the Python function returned or
// the exception it raised.
#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "asdl/runtime.h"
#include "asm2_ast.h"
#include "asm_ast.h"
#include "asm_emit.h"
#include "capture.h"
#include "doctest.h"
#include "errors.h"
#include "json.h"
#include "passes/registry.h"
#include "passes/y_peephole.h"

namespace c6502::testing {

// Returns the text of a recorded exception, `exception: <Python type>:
// <message>` and a newline, for an exception of the C++ port.
inline std::string exception_text(const std::exception& e) {
  if (const auto* c = dynamic_cast<const CompileError*>(&e)) {
    return "exception: " + c->python_type() + ": " + c->what() + "\n";
  }
  if (dynamic_cast<const InternalError*>(&e) != nullptr) {
    return std::string("exception: AssertionError: ") + e.what() + "\n";
  }
  return std::string("exception: ") + e.what() + "\n";
}

// Runs the registered function `name` on `input` (a dump, or tagged JSON
// when `input_is_json`) with the call's arguments and context, as
// `check-captures` does, and checks that it returns `expected`, or
// throws the exception `expected` records.
inline void check_call(std::string_view name, std::string_view args_json,
                       std::string_view context_json, bool input_is_json,
                       std::string_view input, std::string_view expected) {
  const passes::RegisteredPass* pass = passes::find_pass(name);
  REQUIRE(pass != nullptr);
  const json::Value args = json::parse(args_json);
  const json::Value empty_array{.kind = json::Value::Kind::kArray};
  const json::Value empty_object{.kind = json::Value::Kind::kObject};
  const json::Value* positional = args.get("args");
  const json::Value* keywords = args.get("kwargs");
  const passes::AsmContext ctx = capture::context_from_json(json::parse(context_json));
  std::string got;
  try {
    if (pass->run_text != nullptr) {
      got = pass->run_text(std::string(input), ctx, positional ? *positional : empty_array,
                           keywords ? *keywords : empty_object);
    } else {
      REQUIRE_FALSE(input_is_json);
      got = pass->run(asdl::load<asm_ast::Program>(input), ctx,
                      positional ? *positional : empty_array,
                      keywords ? *keywords : empty_object);
    }
  } catch (const std::exception& e) {
    got = exception_text(e);
  }
  CHECK(got == expected);
}

// Checks one recorded call of `emit_instruction`, `emit_function`, or
// `emit_static_variable` on the node whose dump is `input`, or of
// `y_peephole` on `lines`: the lines returned, or one line with the
// exception raised.
inline std::vector<std::string> lines_of(std::string_view kind, std::string_view input) {
  try {
    if (kind == "emit_instruction") {
      return asm_emit::emit_instruction(asdl::load<asm2_ast::Type_instruction>(input));
    }
    const auto tl = asdl::load<asm2_ast::Type_top_level>(input);
    if (kind == "emit_function") {
      return asm_emit::emit_function(std::get<asm2_ast::Function>(tl));
    }
    return asm_emit::emit_static_variable(std::get<asm2_ast::StaticVariable>(tl));
  } catch (const std::exception& e) {
    return {exception_text(e)};
  }
}

inline void check_lines(std::string_view kind, std::string_view input,
                        const std::vector<std::string>& expected) {
  CHECK(lines_of(kind, input) == expected);
}

inline void check_y_peephole(const std::vector<std::string>& lines,
                             const std::vector<std::string>& expected) {
  CHECK(passes::apply_y_peephole(lines) == expected);
}

}  // namespace c6502::testing
