// A C source compiled and loaded into the simulator, as
// sim/harness.py's `build_sim` builds one: the passes `compile.py -S`
// runs, with `-O` when asked, stopped before `asm_to_asm2`, and the asm
// program assembled with the runtime for its layout.
#pragma once

#include <cstdint>
#include <string>

#include "sim/harness.h"

namespace c6502::sim {

struct SourceOptions {
  // The directory of c6502's own headers.
  std::string include_dir;
  bool optimize = false;
  // false when `-O` doesn't inline the calls of small `static`
  // functions.
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
