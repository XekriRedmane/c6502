#include "passes/translate_for_codegen.h"

#include <utility>

#include "c99_to_tac.h"
#include "pass_order.h"
#include "passes/optimization/dispatch_pointer_array.h"
#include "passes/optimization/inline_functions.h"
#include "passes/optimization/optimizer.h"
#include "passes/optimization/row_address_tables.h"
#include "passes/optimization/split_tables.h"
#include "passes/return_in_zero_page.h"

namespace c6502::passes {

tac_ast::Program translate_for_codegen(c99_ast::Program& prog, SymbolTable& symbols,
                                       TypeTable& types, bool optimize, bool inline_calls) {
  tac_ast::Program tac = c99_to_tac::translate_program(prog, symbols, &types);
  if (optimize) {
    if (inline_calls) {
      tac = inline_functions(std::move(tac), symbols, prog);
    }
    tac = return_pointers_in_zero_page(std::move(tac), symbols);
  }
  return tac;
}

tac_ast::Program optimized_tac(tac_ast::Program tac, SymbolTable& symbols, const TypeTable& types) {
  const pass_order::Run pass_run("optimized_tac");
  pass_order::ran("row_address_tables");
  tac = apply_row_address_tables(std::move(tac), &symbols, &types);
  pass_order::ran("optimize_tac");
  tac = optimize_program(std::move(tac), &symbols);
  pass_order::ran("dispatch_const_pointer_arrays");
  tac = dispatch_const_pointer_arrays(std::move(tac), &symbols);
  pass_order::ran("split_tables");
  return apply_split_tables(std::move(tac), &symbols);
}

}  // namespace c6502::passes
