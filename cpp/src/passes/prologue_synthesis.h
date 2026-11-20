// The port of passes/prologue_synthesis.py.
#pragma once

#include <string>

#include "asm_ast.h"
#include "ordered_map.h"
#include "passes/replace_pseudoregisters.h"

namespace c6502::passes {

// Each function's frame, by name, as `replace_program_bare_exit`
// returns it.
using FrameDimsByFunction = OrderedMap<std::string, FrameDims>;

// Returns `prog` with each function's prologue added and each bare
// `Return` made a full `Ret`, from the function's frame in
// `dims_by_fn`, as the comment at the top of prologue_synthesis.cpp
// describes. A function with no arguments, no frame locals, and no
// callee-saved bytes, and every static, comes back unchanged. Throws
// `InternalError` for a function `dims_by_fn` has no entry for, or one
// that already has a `Ret` or a `FunctionPrologue`.
asm_ast::Program synthesize_program(asm_ast::Program prog, const FrameDimsByFunction& dims_by_fn);

}  // namespace c6502::passes
