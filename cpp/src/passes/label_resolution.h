// The labels of labeled statements and the targets of `goto` (C99
// §6.8.1, §6.8.6).
#pragma once

#include "c99_ast.h"

namespace c6502::passes {

// Renames each label of each function `.<function>@<label>`, and each
// `goto`'s target with it, in the program it takes by value and
// returns. Throws `CompileError` of type `LabelResolutionError` for a
// label declared twice in one function and for a `goto` to a label the
// function doesn't declare.
c99_ast::Program resolve_labels(c99_ast::Program prog);

}  // namespace c6502::passes
