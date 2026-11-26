#include "passes/text_stages.h"

#include <utility>

#include "asdl/runtime.h"
#include "c99_to_tac.h"
#include "parser.h"
#include "passes/front_end.h"
#include "passes/optimization/inline_functions.h"
#include "passes/optimization/optimizer.h"
#include "passes/type_checker.h"

namespace c6502::passes {

TextStageResult run_text_stage(TextStage stage, const std::string& text, bool optimize,
                               bool inline_calls) {
  TextStageResult out;
  switch (stage) {
    case TextStage::kParse:
      out.text = asdl::py_pretty(parser::parse(text)) + "\n";
      return out;
    case TextStage::kResolve:
      out.text = asdl::py_pretty(resolved(text, optimize)) + "\n";
      return out;
    case TextStage::kTac:
      break;
  }
  c99_ast::Program prog = resolved(text, optimize);
  type_checking::CheckedProgram tables = type_checking::check_program(prog);
  tac_ast::Program tac = c99_to_tac::translate_program(prog, tables.symbols, &tables.types);
  if (optimize) {
    if (inline_calls) {
      tac = inline_functions(std::move(tac), tables.symbols, prog);
    }
    tac = optimize_program(std::move(tac), &tables.symbols);
  }
  out.text = asdl::py_pretty(tac) + "\n";
  return out;
}

}  // namespace c6502::passes
