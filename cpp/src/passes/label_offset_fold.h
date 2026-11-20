// The port of passes/label_offset_fold.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Folds each add of a label's address and a constant into one
// immediate of the label plus the constant, as the comment at the top
// of label_offset_fold.cpp describes.
asm_ast::Program apply_label_offset_fold(asm_ast::Program prog);

}  // namespace c6502::passes
