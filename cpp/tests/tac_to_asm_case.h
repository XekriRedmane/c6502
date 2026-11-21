// The check of a recorded call of `tac_to_asm` from the Python unit
// tests, whose result is an asm program's dump.
#pragma once

#include <exception>
#include <string>
#include <string_view>

#include "capture.h"
#include "doctest.h"
#include "json.h"
#include "tac_to_asm_stage.h"

namespace c6502::testing {

// Runs `tac_to_asm` on the TAC program `input` with the call's arguments
// (`args_json`) in the context `context_json`, and checks that it
// returns the dump `output` (or the exception it records, `exception:
// ...`) and changes the symbol table as `mutated` records (empty when
// it adds no local).
inline void check_tac_to_asm_call(std::string_view args_json, std::string_view context_json,
                                  std::string_view input, std::string_view output,
                                  std::string_view mutated) {
  capture::Call call;
  call.pass = "tac_to_asm";
  call.args_json = std::string(args_json);
  call.context_json = std::string(context_json);
  call.input = std::string(input);
  const json::Value args = json::parse(args_json);
  std::string got;
  std::string got_mutated;
  try {
    const passes::StageOutput out = tac_to_asm::run_tac_to_asm(
        call, capture::context(call), *args.get("args"), *args.get("kwargs"));
    got = out.output;
    got_mutated = out.mutated;
  } catch (const std::exception& e) {
    got = std::string("exception: ") + e.what() + "\n";
  }
  CHECK(got == output);
  CHECK(got_mutated == mutated);
}

}  // namespace c6502::testing
