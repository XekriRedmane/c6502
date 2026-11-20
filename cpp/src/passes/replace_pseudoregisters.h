// The parts of passes/replace_pseudoregisters.py ported so far: the
// frame dimensions it returns for each function.
#pragma once

#include <vector>

#include "asdl/runtime.h"

namespace c6502::passes {

// A function's frame, as `replace_program_bare_exit` computes it for
// `prologue_synthesis` (Python's `FrameDims`): the bytes of its
// arguments and of its locals in the frame, and the callee-saved
// zero-page bytes it saves, which are the payload of
// `FunctionPrologue` and `Ret`.
struct FrameDims {
  asdl::Int arg_bytes = 0;
  asdl::Int local_bytes = 0;
  std::vector<asdl::Int> callee_saved_addrs;
  bool operator==(const FrameDims&) const = default;
};

}  // namespace c6502::passes
