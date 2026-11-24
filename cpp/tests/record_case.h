// Checks of recorded calls in the capture format, for the tests that
// port a Python test module's direct calls of the asm passes and the
// stage calls, which tools/record_test_calls.py records while the
// Python tests run (docs/cpp_port.md, step 8, group G6).
#pragma once

#include <exception>
#include <string>
#include <string_view>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "call_case.h"
#include "capture.h"
#include "doctest.h"
#include "errors.h"
#include "json.h"
#include "passes/registry.h"
#include "passes/stage_registry.h"
#include "stage_case.h"
#include "stage_checks.h"
#include "tagged_json.h"

namespace c6502::testing {

// Returns the text of a recorded `output-error` section's JSON for an
// exception of the C++ port, `{"type": <Python class>, "message": ...}`.
inline std::string error_record(const std::exception& e) {
  std::string type = "Exception";
  if (const auto* c = dynamic_cast<const CompileError*>(&e)) {
    type = c->python_type();
  } else if (dynamic_cast<const InternalError*>(&e) != nullptr) {
    type = "AssertionError";
  }
  std::string out = "{\"type\": ";
  tagged::write_string(out, type);
  out += ", \"message\": ";
  tagged::write_string(out, e.what());
  out += '}';
  return out;
}

// Runs the call that `record` holds, a capture file's text, as
// `check-captures` does: a stage call through `stage_checks::check`,
// and a pass of `passes::find_pass` on the recorded input with the
// recorded arguments and context. Checks that the C++ function returns
// the recorded output, changes its arguments as the record's `mutated`
// line says, or raises the exception of its `output-error` section.
inline void check_record(const std::string& record) {
  const capture::Call call = capture::parse(record);
  INFO("call of ", call.pass, " from ", call.source);
  if (const passes::StageSignature* sig = passes::find_stage(call.pass)) {
    const stage_checks::Result r = stage_checks::check(call, *sig);
    CHECK_MESSAGE(r.tables_error.empty(), r.tables_error);
    REQUIRE(r.ported);
    CHECK_MESSAGE(r.matched, r.mismatch, "\n", r.got);
    return;
  }
  const passes::RegisteredPass* pass = passes::find_pass(call.pass);
  REQUIRE(pass != nullptr);
  const json::Value args = json::parse(call.args_json);
  const json::Value empty_array{.kind = json::Value::Kind::kArray};
  const json::Value empty_object{.kind = json::Value::Kind::kObject};
  const json::Value* positional = args.get("args");
  const json::Value* keywords = args.get("kwargs");
  std::string got;
  try {
    if (pass->run_text != nullptr) {
      got = pass->run_text(call.input, capture::context(call),
                           positional ? *positional : empty_array,
                           keywords ? *keywords : empty_object);
    } else {
      REQUIRE_FALSE(call.input_is_json);
      got = pass->run(asdl::load<asm_ast::Program>(call.input), capture::context(call),
                      positional ? *positional : empty_array,
                      keywords ? *keywords : empty_object);
    }
    CHECK_FALSE(call.output_is_error);
    CHECK(pass->output_is_json == call.output_is_json);
  } catch (const std::exception& e) {
    REQUIRE_MESSAGE(call.output_is_error, exception_text(e));
    got = error_record(e) + "\n";
  }
  CHECK(got == call.output);
}

}  // namespace c6502::testing
