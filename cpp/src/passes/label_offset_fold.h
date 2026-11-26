#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Folds each 2-byte add of a label's halves and a constant, and each
// such add of the low byte alone, into immediates of the label plus the
// constant (`#<(r+k)`, `#>(r+k)`), when C and V are dead after the last
// store.
asm_ast::Program apply_label_offset_fold(asm_ast::Program prog);

}  // namespace c6502::passes
