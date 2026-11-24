// The port of passes/prologue_synthesis.py.
#pragma once

#include <string>
#include <utility>
#include <vector>

#include "asm_ast.h"
#include "ordered_map.h"
#include "passes/replace_pseudoregisters.h"

namespace c6502::passes {

// `FrameDimsByFunction`, each function's frame by name, is in
// passes/replace_pseudoregisters.h with `replace_program_bare_exit`,
// which returns it.

// Returns `prog` with each function's prologue added and each bare
// `Return` made a full `Ret`, from the function's frame in
// `dims_by_fn`, as the comment at the top of prologue_synthesis.cpp
// describes. A function with no arguments, no frame locals, and no
// callee-saved bytes, and every static, comes back unchanged. Throws
// `InternalError` for a function `dims_by_fn` has no entry for, or one
// that already has a `Ret` or a `FunctionPrologue`.
asm_ast::Program synthesize_program(asm_ast::Program prog, const FrameDimsByFunction& dims_by_fn);

// Splits the register parameters' entry stubs off the start of
// `instrs`, and returns the stubs and the rest, as the comment at its
// definition describes. The unit tests call it, as
// tests/test_param_registers.py called Python's `_split_entry_stubs`.
std::pair<std::vector<asm_ast::Type_instruction>, std::vector<asm_ast::Type_instruction>>
split_entry_stubs(std::vector<asm_ast::Type_instruction> instrs);

}  // namespace c6502::passes
