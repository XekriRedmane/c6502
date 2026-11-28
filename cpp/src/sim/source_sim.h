#pragma once

#include <cstdint>
#include <string>

#include "sim/harness.h"

namespace c6502::sim {

struct SourceOptions {
  std::string include_dir;
  bool optimize = false;
  bool inline_calls = true;
  bool check_ir = false;
  std::int64_t origin = kDefaultOrigin;
};

// Compiles `source`, with no preprocessor arguments and no file name,
// and loads it, ready to run from the boot stub. What the preprocessor
// writes to stderr is appended to `err`. Throws the `CompileError` of a
// rejected program, the `InternalError` of a failed IR check, and what
// `build_sim` throws.
Simulation build_source_sim(const std::string& source, const SourceOptions& options,
                            std::string& err);

}  // namespace c6502::sim
