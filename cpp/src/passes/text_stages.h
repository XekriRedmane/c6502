// The stages `--parse`, `--resolve`, and `--tac`, which print an IR tree
// and which `c6502cpp backend` runs for compile.py's `_run_stage`.
#pragma once

#include <string>

#include "c99_ast.h"
#include "tac_ast.h"

namespace c6502::passes {

enum class TextStage {
  // `--parse`: the parser alone, without the unroller even under `-O`.
  kParse,
  // `--resolve`: the parser, under `-O` the unroller, the three passes
  // that resolve names, and string lifting, without the type checker.
  kResolve,
  // `--tac`: `--resolve`, the type checker, and `c99_to_tac`, and under
  // `-O` `inline_functions` and the TAC optimizer only. The row-address
  // tables, `return_pointers_in_zero_page`, the dispatch of
  // const-pointer arrays, and the split tables of `-S -O` don't run.
  kTac,
};

// `text` is what compile.py prints, `pretty` of the tree and a newline.
// Under `--tac -O` with the IR checks on, the TAC optimizer's driver
// checks the TAC after each pass (src/passes/ir_check.h).
struct TextStageResult {
  std::string text;
};

// Runs `stage` on `text`, the preprocessor's output, with `-O` when
// `optimize` is set. `inline_calls` is false when `--tac -O` doesn't
// inline the calls of small `static` functions, as for a test that
// sets `compile.inline_small_functions` to False (tests/no_inlining.py).
// Throws the `CompileError` of the pass that rejects the program.
TextStageResult run_text_stage(TextStage stage, const std::string& text, bool optimize,
                               bool inline_calls);

}  // namespace c6502::passes
