// The stages of compile.py that print an IR tree with pretty.py,
// `--parse`, `--resolve`, and `--tac` (compile.py's `_run_stage`), which
// `c6502cpp backend` runs under `C6502_CPP_BACKEND=1`.
#pragma once

#include <optional>
#include <string>

#include "c99_ast.h"
#include "tac_ast.h"

namespace c6502::passes {

// The stage a request stops at.
enum class TextStage {
  // `--parse`: the parser alone.
  kParse,
  // `--resolve`: compile.py's `_resolved`, which is the parser, under
  // `-O` the unroller, the three passes that resolve names, and string
  // lifting, without the type checker.
  kResolve,
  // `--tac`: `_resolved`, the type checker, and `c99_to_tac`, and under
  // `-O` `inline_functions` and the TAC optimizer only. The row-address
  // tables, `return_pointers_in_zero_page`, the dispatch of
  // const-pointer arrays, and the split tables of `-S -O` don't run.
  kTac,
};

// What a stage gives: the text compile.py prints, `pretty` of the tree
// and a newline, and for `--tac` the TAC itself, which the backend
// sends back for `ir_check` under `C6502_CHECK_IR=1`.
struct TextStageResult {
  std::string text;
  std::optional<tac_ast::Program> tac;
};

// Runs `stage` on `text`, the preprocessor's output, with `-O` when
// `optimize` is set. `inline_calls` is false when `--tac -O` doesn't
// inline the calls of small `static` functions, as for a test that
// patches `compile.inline_functions` (tests/no_inlining.py). Throws the
// `CompileError` of the pass that rejects the program.
TextStageResult run_text_stage(TextStage stage, const std::string& text, bool optimize,
                               bool inline_calls);

}  // namespace c6502::passes
