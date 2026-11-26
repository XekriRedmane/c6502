#pragma once

#include <string>
#include <utility>
#include <vector>

#include "asm_ast.h"
#include "ordered_map.h"
#include "passes/replace_pseudoregisters.h"

namespace c6502::passes {

// Returns `prog` with each function's prologue added and each bare
// `Return` made a full `Ret`, from the function's frame in
// `dims_by_fn`, as the comment at the top of prologue_synthesis.cpp
// describes. A function with no arguments, no frame locals, and no
// callee-saved bytes, and every static, comes back unchanged. Throws
// `InternalError` for a function `dims_by_fn` has no entry for, or one
// that already has a `Ret` or a `FunctionPrologue`.
asm_ast::Program synthesize_program(asm_ast::Program prog, const FrameDimsByFunction& dims_by_fn);

// Splits the register parameters' entry stubs off the start of
// `instrs`, skipping labels: the leading moves from A, X, or Y into
// memory (the local each parameter is copied into), into X, or into A.
// Returns the stubs and the rest, with the labels left in the rest.
std::pair<std::vector<asm_ast::Type_instruction>, std::vector<asm_ast::Type_instruction>>
split_entry_stubs(std::vector<asm_ast::Type_instruction> instrs);

}  // namespace c6502::passes
