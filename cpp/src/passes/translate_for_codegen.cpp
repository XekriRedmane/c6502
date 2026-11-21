#include "passes/translate_for_codegen.h"

#include <utility>

#include "c99_to_tac.h"
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
  // The address of a row of a 2-D array whose rows aren't a power of two
  // bytes long comes from a table of row addresses, when that's smaller.
  // This runs before strength reduction turns the multiply into shifts.
  tac = apply_row_address_tables(std::move(tac), &symbols, &types);
  tac = optimize_program(std::move(tac), &symbols);
  // A read `arr[i][j]` through a small `static const T * const[N]`
  // becomes a compare-and-branch dispatch on `i`. It runs after the
  // optimizer has taken each function out of SSA form, so each case can
  // write the same destination without a Phi.
  tac = dispatch_const_pointer_arrays(std::move(tac), &symbols);
  // A table of 2-byte elements read only by a byte index is stored as
  // its low bytes and then its high bytes, so the index needs no shift.
  return apply_split_tables(std::move(tac), &symbols);
}

}  // namespace c6502::passes
