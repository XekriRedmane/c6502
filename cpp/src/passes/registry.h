// The ported asm passes, by the names the capture harness records,
// for the CLI's `check-captures` command.
#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "asm_ast.h"
#include "json.h"
#include "passes/asm_context.h"

namespace c6502::passes {

struct RegisteredPass {
  // The pass's name in tools/capture_passes.py's table, which for a
  // pass of `_peephole_fixedpoint` is its name in the sweep.
  std::string_view name;
  // Runs the pass on a captured call's program, in the compile's
  // context `ctx`, with the call's positional arguments after the program (`args`, a JSON array) and
  // keyword arguments (`kwargs`, a JSON object) in the harness's
  // tagged JSON, and returns the text the capture's output section
  // holds: the dump of the program, or for a pass whose result isn't
  // one, its tagged JSON.
  std::string (*run)(asm_ast::Program input, const AsmContext& ctx,
                     const json::Value& args, const json::Value& kwargs);
  // Whether `run` returns tagged JSON, which the capture records under
  // `output-json`.
  bool output_is_json = false;
  // For a function whose first argument isn't an asm program, such as
  // `emit_program`, which takes an asm2 program, or `format_metadata`,
  // which takes a `LinkMetadata`, `run` is null and this runs it on the
  // capture's input text, which is a dump or, for an input recorded
  // under `input-json`, its tagged JSON.
  std::string (*run_text)(const std::string& input, const AsmContext& ctx,
                          const json::Value& args, const json::Value& kwargs) = nullptr;
  // The IR module of the input's dump and of the output's when they
  // are dumps, for checking that each reads and writes back as the
  // same bytes.
  std::string_view input_module = "asm_ast";
  std::string_view output_module = "asm_ast";
};

// Returns the ported pass named `name`, or null.
const RegisteredPass* find_pass(std::string_view name);

// Returns every ported pass, in the order of compile.py's pipeline.
const std::vector<RegisteredPass>& registered_passes();

}  // namespace c6502::passes
