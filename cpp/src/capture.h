// Reads the files of recorded pass calls that tools/capture_passes.py
// writes.
#pragma once

#include <filesystem>
#include <string>

#include "json.h"
#include "passes/asm_context.h"

namespace c6502::capture {

// One recorded call of a pass.
struct Call {
  // The pass's name, as the harness records it.
  std::string pass;
  // The compile that first made this call, such as
  // `examples/sfx_tone.c -S -O`, for finding it again.
  std::string source;
  // The call's arguments after the program, as the tagged JSON object
  // `{"args": [...], "kwargs": {...}}`.
  std::string args_json;
  // The context of the compile, the JSON object of the `context` line,
  // or empty for a capture of version 1, which has none.
  std::string context_json;
  // The dump of the program the pass was given, or, when
  // `input_is_json`, the tagged JSON of a first argument that isn't an
  // IR value, such as `format_metadata`'s `LinkMetadata` or the pools
  // that `build_local_slot_symbols` takes first.
  std::string input;
  bool input_is_json = false;
  // For a call of `STAGE_PASSES` that changed arguments in place, the
  // tagged JSON object `{"args": {"<index>": ...}, "kwargs": {...}}`
  // of their encodings after the call, by position in `args_json`'s
  // list or by keyword, and empty otherwise.
  std::string mutated_json;
  // The dump of the program the pass returned, or, when
  // `output_is_json`, the tagged JSON of a result that isn't a
  // program, such as a tuple.
  std::string output;
  bool output_is_json = false;
  // For a call of the front end (step 6) that raised, the record's
  // `output-error` section, whose output is the JSON object
  // `{"type": ..., "message": ...}` of the exception's class and
  // message, on one line.
  bool output_is_error = false;
  // The JSON list of `[where, message]` pairs on the `warnings` line,
  // the warnings a call of the front end issued, or empty when it
  // issued none.
  std::string warnings_json;
};

// Returns the call recorded in the text of a capture file. Raises
// `std::runtime_error` for text that isn't one.
Call parse(const std::string& text);

// Returns the compile's context that `call` records, or the default
// context (the classic layout and no user zero-page globals) for a
// capture without one.
passes::AsmContext context(const Call& call);

// Returns the context that the JSON object `v` holds, with its
// `layout`, `user_zp_globals`, and `user_zp_bytes` members as
// tools/capture_passes.py's `context_json` writes them.
passes::AsmContext context_from_json(const json::Value& v);

// Returns the bytes of a file. Raises `std::runtime_error` when it
// can't be read.
std::string read_file(const std::filesystem::path& path);

}  // namespace c6502::capture
