// compile.py's front end, from the source text to the type-checked c99
// program and its tables: `_compile_c` through `type_check_program`,
// which `c6502cpp backend` runs before code generation under
// `C6502_CPP_BACKEND=1`.
#pragma once

#include <optional>
#include <string>
#include <vector>

#include "c99_ast.h"
#include "diagnostics.h"
#include "passes/type_checking.h"

namespace c6502::passes {

// What the front end takes besides the source text: the preprocessor's
// arguments, the file the source came from (none for stdin), the
// directory of c6502's own headers (preprocessor.py's
// `_BUNDLED_INCLUDE_DIR`), whether the text still has to be
// preprocessed (false for `compile._run_stage`, which is given the
// preprocessor's output), and `-O`, which unrolls the marked loops.
struct FrontEndOptions {
  std::vector<std::string> pcpp_args;
  std::optional<std::string> filename;
  std::string include_dir;
  bool preprocess = true;
  bool optimize = false;
};

// The type-checked program and the tables the type checker built.
struct FrontEndResult {
  c99_ast::Program prog;
  SymbolTable symbols;
  TypeTable types;
};

// Returns the text the stages after the preprocessor read: `source`
// preprocessed with the options' arguments and file name, or `source`
// itself when `options.preprocess` is false. What the preprocessor
// writes to stderr is appended to `err`, also when it throws its
// `CompileError`, and its `_Bool` warnings go to `diagnostics::warn`.
std::string preprocess_source(const std::string& source, const FrontEndOptions& options,
                              std::string& err);

// Returns compile.py's `_resolved` of `text`, the preprocessor's output:
// `parse`, under `-O` (`optimize`) `unroll_program`,
// `identifier_resolution`, `string_lifting`, `label_resolution`, and
// `loop_labeling`, in that order. Throws the `CompileError` of the pass
// that rejects the program.
c99_ast::Program resolved(const std::string& text, bool optimize);

// Runs `preprocess`, `parse`, under `-O` `unroll_program`,
// `identifier_resolution`, `string_lifting`, `label_resolution`,
// `loop_labeling`, and `type_checking` on `source`, in compile.py's
// order. The preprocessor's `_Bool` warnings go to `diagnostics::warn`,
// and what it writes to stderr is appended to `err`, which keeps it when
// a pass throws. Throws the `CompileError` of the pass that rejects the
// program.
FrontEndResult front_end(const std::string& source, const FrontEndOptions& options,
                         std::string& err);

}  // namespace c6502::passes
