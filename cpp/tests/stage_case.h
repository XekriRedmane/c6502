// A check of one recorded call of a ported stage call (step 4), for the
// tests that port a Python module's unit tests as the calls they make.
#pragma once

#include <string>
#include <string_view>

#include "capture.h"
#include "doctest.h"
#include "json.h"
#include "passes/stage_registry.h"

namespace c6502::testing {

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

}  // namespace c6502::testing
