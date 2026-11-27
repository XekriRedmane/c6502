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
  out.text = asdl::py_pretty(tac_stage(text, optimize, inline_calls).tac) + "\n";
  return out;
}

TacStage tac_stage(const std::string& text, bool optimize, bool inline_calls) {
  c99_ast::Program prog = resolved(text, optimize);
  type_checking::CheckedProgram tables = type_checking::check_program(prog);
  TacStage out{.symbols = std::move(tables.symbols), .types = std::move(tables.types)};
  out.tac = c99_to_tac::translate_program(prog, out.symbols, &out.types);
  if (optimize) {
    if (inline_calls) {
      out.tac = inline_functions(std::move(out.tac), out.symbols, prog);
    }
    out.tac = optimize_program(std::move(out.tac), &out.symbols);
  }
  return out;
}

}  // namespace c6502::passes
