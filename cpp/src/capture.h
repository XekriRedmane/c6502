// Reads the files of recorded pass calls that tools/capture_passes.py
// writes.
#pragma once

#include <filesystem>
#include <string>

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
  // The dump of the program the pass was given.
  std::string input;
  // The dump of the program the pass returned, or, when
  // `output_is_json`, the tagged JSON of a result that isn't a
  // program, such as a tuple.
  std::string output;
  bool output_is_json = false;
};

// Returns the call recorded in the text of a capture file. Raises
// `std::runtime_error` for text that isn't one.
Call parse(const std::string& text);

// Returns the bytes of a file. Raises `std::runtime_error` when it
// can't be read.
std::string read_file(const std::filesystem::path& path);

}  // namespace c6502::capture
