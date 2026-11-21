// compile.py's `translate_for_codegen` and `_optimized_tac`: the
// translation of the type-checked c99 program to TAC and, under `-O`,
// the TAC passes that run before code generation.
#pragma once

#include "c99_ast.h"
#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::passes {

// Translates the type-checked `prog` to TAC for code generation
// (`c99_to_tac`, which changes `prog`, `symbols`, and `types` in place).
// Under `-O` (`optimize`) it then inlines the calls of small `static`
// functions, unless `inline_calls` is false, and returns each remaining
// `static` function's pointer result in a zero-page object of its own,
// which adds the objects to `symbols` before `plan_zero_page` places
// them.
tac_ast::Program translate_for_codegen(c99_ast::Program& prog, SymbolTable& symbols,
                                       TypeTable& types, bool optimize, bool inline_calls);

// Runs the TAC passes at the start of the `-O` pipeline on the
// unoptimized TAC `tac` and returns the TAC that code generation lowers:
// the tables of row addresses, the TAC optimizer, the dispatch of small
// const-pointer arrays, and the split tables. Each may add symbols to
// `symbols`.
tac_ast::Program optimized_tac(tac_ast::Program tac, SymbolTable& symbols, const TypeTable& types);

}  // namespace c6502::passes
