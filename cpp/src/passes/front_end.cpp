#include "passes/front_end.h"

#include <utility>

#include "parser.h"
#include "pcpp_preprocessor.h"
#include "passes/identifier_resolution.h"
#include "passes/label_resolution.h"
#include "passes/loop_labeling.h"
#include "passes/optimization_ast/unroll.h"
#include "passes/string_lifting.h"
#include "passes/type_checker.h"
#include "preprocessor.h"

namespace c6502::passes {

std::string preprocess_source(const std::string& source, const FrontEndOptions& options,
                              std::string& err) {
  if (!options.preprocess) return source;
  const pcpp::Host host = pcpp::native_host();
  std::string pp_err;
  std::string text;
  try {
    text = preprocessor::preprocess(source, options.pcpp_args, options.filename, host,
                                    options.include_dir, &pp_err)
               .text;
  } catch (...) {
    err += pp_err;
    throw;
  }
  err += pp_err;
  return text;
}

c99_ast::Program resolved(const std::string& text, bool optimize) {
  // The unroller runs before identifier resolution, so each copy of a
  // body gets names of its own, and string lifting after it, so the
  // names it makes aren't renamed.
  c99_ast::Program prog = parser::parse(text);
  if (optimize) {
    prog = unroll_program(std::move(prog));
  }
  prog = resolve_identifiers(std::move(prog));
  prog = lift_strings(std::move(prog));
  prog = resolve_labels(std::move(prog));
  return label_program(std::move(prog));
}

FrontEndResult front_end(const std::string& source, const FrontEndOptions& options,
                         std::string& err) {
  c99_ast::Program prog = resolved(preprocess_source(source, options, err), options.optimize);
  type_checking::CheckedProgram tables = type_checking::check_program(prog);
  return {std::move(prog), std::move(tables.symbols), std::move(tables.types)};
}

}  // namespace c6502::passes
