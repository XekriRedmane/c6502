// Checks of recorded calls of ported stage calls (step 4), for the tests
// that port a Python module's unit tests as the calls they make, each
// with what the Python function returned or, for `check_stage_call`, the
// exception it raised.
#pragma once

#include <exception>
#include <initializer_list>
#include <string>
#include <string_view>

#include "capture.h"
#include "doctest.h"
#include "errors.h"
#include "json.h"
#include "passes/stage_registry.h"

namespace c6502::testing {

// Returns the pieces joined, for a recorded text too long for one
// string literal, since MSVC limits a literal to 64 KB.
inline std::string text(std::initializer_list<std::string_view> pieces) {
  std::string out;
  for (const auto p : pieces) {
    out += p;
  }
  return out;
}

// Runs the stage call `name` on `input` (a dump, or the tagged JSON of a
// first argument that isn't an IR value) with the call's arguments
// (`args_json`, the harness's `{"args": [...], "kwargs": {...}}`) in the
// classic layout, and checks that it returns `output` and changes its
// arguments as `mutated` records (empty when it changes none).
inline void check_stage_case(std::string_view name, std::string_view args_json,
                             std::string_view input, std::string_view output,
                             std::string_view mutated) {
  const passes::StageSignature* sig = passes::find_stage(name);
  REQUIRE(sig != nullptr);
  REQUIRE(sig->run != nullptr);
  capture::Call call;
  call.pass = std::string(name);
  call.args_json = std::string(args_json);
  call.input = std::string(input);
  call.input_is_json = sig->input_json != nullptr;
  const json::Value args = json::parse(args_json);
  const passes::StageOutput got =
      sig->run(call, passes::AsmContext{}, *args.get("args"), *args.get("kwargs"));
  CHECK(got.output == output);
  CHECK(got.mutated == mutated);
}

// Runs the stage call `name`'s runner on `input` (a dump, or tagged JSON
// when `input_is_json`) with the call's arguments and context, as
// `check-captures` does, and checks that it returns `output`, a result
// of tagged JSON with the newline the harness ends it with, and changes
// its arguments as `mutated` records (empty for none), or throws the
// exception `output` records, `exception: <Python type>: <message>`.
inline void check_stage_call(std::string_view name, std::string_view args_json,
                             std::string_view context_json, bool input_is_json,
                             std::string_view input, std::string_view output,
                             std::string_view mutated) {
  const passes::StageSignature* sig = passes::find_stage(name);
  REQUIRE(sig != nullptr);
  REQUIRE(sig->run != nullptr);
  capture::Call call;
  call.pass = std::string(name);
  call.args_json = std::string(args_json);
  call.context_json = std::string(context_json);
  call.input = std::string(input);
  call.input_is_json = input_is_json;
  call.output_is_json = true;
  const json::Value args = json::parse(args_json);
  std::string got;
  std::string got_mutated;
  try {
    const passes::StageOutput out =
        sig->run(call, capture::context(call), *args.get("args"), *args.get("kwargs"));
    got = out.output + "\n";
    got_mutated = out.mutated;
  } catch (const CompileError& e) {
    got = "exception: " + e.python_type() + ": " + e.what() + "\n";
  } catch (const InternalError& e) {
    got = std::string("exception: AssertionError: ") + e.what() + "\n";
  } catch (const std::exception& e) {
    got = std::string("exception: ") + e.what() + "\n";
  }
  CHECK(got == output);
  CHECK(got_mutated == mutated);
}

}  // namespace c6502::testing
