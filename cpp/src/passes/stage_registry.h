// The signatures of the stage calls of code generation that the frozen
// captures record. A signature gives the codec (src/tagged_json.h) of
// each of a call's arguments and of its result, so that `check-captures`
// can check that every value of a recorded call round-trips, and the
// runner that `check-captures` runs on the call.
#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "capture.h"
#include "json.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Returns the text the codec of a value writes for the value it reads
// from `v`, which `tagged::round_trip<C>` gives.
using RoundTrip = std::string (*)(const json::Value& v);

// What a runner returns: the text of the record's output section
// (a dump, or the tagged JSON of a result that isn't an IR value) and
// the `mutated` line's JSON, or empty when the call changes none of its
// arguments.
struct StageOutput {
  std::string output;
  std::string mutated;
  // The `warnings` line's JSON for a call of the front end that issued
  // warnings, or empty.
  std::string warnings;
  // The `stderr` line's JSON string for a call of `preprocess` that
  // wrote to stderr, or empty.
  std::string stderr_json;
};

// Runs a call on its recorded first argument (`call.input`, a
// program's dump or, when `call.input_is_json`, tagged JSON), its
// positional arguments `args` (a JSON array) and
// keyword arguments `kwargs` (a JSON object), in the compile's context.
using StageRun = StageOutput (*)(const capture::Call& call, const AsmContext& ctx,
                                 const json::Value& args, const json::Value& kwargs);

struct StageSignature {
  std::string_view name;
  // The IR module of the input program, "tac_ast" or "asm_ast", or
  // empty for a call whose first argument isn't a program.
  std::string_view input;
  // The codec of a first argument that isn't a program, which the
  // record holds on its `input-json` line, or null for a program.
  RoundTrip input_json = nullptr;
  // The codecs of the positional arguments after the input, in order.
  std::vector<RoundTrip> args;
  // The codecs of the keyword arguments the call may be given.
  std::vector<std::pair<std::string_view, RoundTrip>> kwargs;
  // The codec of the result, or null for a result that is an IR value,
  // which the record holds as its dump.
  RoundTrip output = nullptr;
  // The IR module of a result that the record holds as its dump: an asm
  // program for the calls here, a TAC program or function for those of
  // src/passes/optimization/tac_stage.h, and a c99 program for those of
  // src/passes/front_stage.h.
  std::string_view output_module = "asm_ast";
  // The runner, or null for a call that `check-captures` only
  // round-trips.
  StageRun run = nullptr;
};

// Returns the signature of the stage call named `name`, or null.
const StageSignature* find_stage(std::string_view name);

// Returns every stage call's signature, in pipeline order.
const std::vector<StageSignature>& stage_signatures();

}  // namespace c6502::passes
