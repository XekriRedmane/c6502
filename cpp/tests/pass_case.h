// A check of one recorded call of a ported pass, for the tests that
// port a Python pass's unit tests as the calls they make of it.
#pragma once

#include <string>
#include <string_view>
#include <utility>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "capture.h"
#include "doctest.h"
#include "json.h"
#include "passes/registry.h"

namespace c6502::testing {

// Runs the registered pass `name` on the program whose dump is `input`,
// with the call's arguments (`args_json`, the harness's
// `{"args": [...], "kwargs": {...}}`) and the compile's context
// (`context_json`, the harness's `context` line), as `check-captures`
// does, and checks that the dump of the result is `output`.
inline void check_pass_case(std::string_view name, std::string_view args_json,
                            std::string_view context_json, std::string_view input,
                            std::string_view output) {
  const passes::RegisteredPass* pass = passes::find_pass(name);
  REQUIRE(pass != nullptr);
  const json::Value args = json::parse(args_json);
  const json::Value empty_array{.kind = json::Value::Kind::kArray};
  const json::Value empty_object{.kind = json::Value::Kind::kObject};
  const json::Value* positional = args.get("args");
  const json::Value* keywords = args.get("kwargs");
  const std::string got = pass->run(
      asdl::load<asm_ast::Program>(input),
      capture::context_from_json(json::parse(context_json)),
      positional != nullptr ? *positional : empty_array,
      keywords != nullptr ? *keywords : empty_object);
  CHECK(got == output);
}

}  // namespace c6502::testing
