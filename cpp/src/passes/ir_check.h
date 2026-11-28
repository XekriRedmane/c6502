// Checks of the IR between passes. They're off unless `enable()` turns
// them on. A failed check throws `InternalError`, which `c6502cpp
// compile` reports as an AssertionError.
//
// TAC (`check_tac`):
//
//   * Labels are unique, and every jump goes to one of them.
//   * Every variable is in the symbol table.
//   * In SSA form, an SSA-renamed variable is defined once, and every
//     read of it is dominated by its definition (for a Phi argument,
//     the end of the predecessor it comes from). A parameter's first
//     value is defined at entry. Unreachable blocks are ignored.
//   * Outside SSA form, every compiler temporary (`%...`) that's read
//     is defined somewhere in the function, unless it's an object
//     accessed in memory (its address taken, or indexed by name).
//
// Asm (`check_asm`):
//
//   * Labels are unique in each function, and every branch, and every
//     jump to a local label (one starting with `.`), goes to one of
//     them.
//   * No `Phi` outside the asm optimizer's SSA form, and no `Pseudo`
//     once pseudoregisters have been replaced.
//   * Every `DecimalArith` has operands of the forms
//     `decimal_arith::form_problems` accepts.
//   * No `SetDecimal` or `ClearDecimal`.
//   * An `INC`, `DEC`, or shift of an array element (`IndexedData`) is
//     indexed by X, since the 6502 has no abs,Y form of them.
//
// Asm2 (`check_asm2`):
//
//   * Every `SED` is followed by `CLC` and ADCs or by `SEC` and SBCs,
//     each byte as `LDA src; ADC src; STA dst`, and then a `CLD`, with
//     nothing else between them but a soft-stack operand's `LDY #off`.
//     No `CLD` is anywhere else, and no function ends with the flag
//     set.
//   * A runtime helper in `rtlib::decimal_mode_helpers` passes instead
//     with a `CLD` right before each `RTS`. A compiled function of the
//     same name passes by either rule.
#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "asm2_ast.h"
#include "asm_ast.h"
#include "passes/asm_context.h"
#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::passes::ir_check {

// Turns the checks on or off for the whole process.
void enable(bool on = true);

// Returns whether the checks are on.
bool enabled();

// Turns the checks on or off for the life of the object, and back to
// what they were when it ends.
class Enabled {
 public:
  explicit Enabled(bool on);
  ~Enabled();
  Enabled(const Enabled&) = delete;
  Enabled& operator=(const Enabled&) = delete;

 private:
  bool before_;
};

// Returns what's wrong with the TAC function `fn`. `symbols`, when
// given, is the table every name must be in, and `ssa_dsts` the
// SSA-renamed names, given only while the function is in SSA form. The
// checks of the names' definitions run only when the labels and the
// symbol table have no problem.
std::vector<std::string> tac_problems(const tac_ast::Function& fn, const SymbolTable* symbols,
                                      const NameSet* ssa_dsts);

// Throws `InternalError` if the checks are on and `fn` breaks a TAC
// invariant: "after <after>, function '<name>': " and the first five of
// `tac_problems`, joined by "; ".
void check_tac(const tac_ast::Function& fn, std::string_view after,
               const SymbolTable* symbols = nullptr, const NameSet* ssa_dsts = nullptr);

// Returns what's wrong with the asm function `fn`. `pseudos` and `phis`
// say whether `Pseudo` operands and `Phi` nodes may still appear.
std::vector<std::string> asm_problems(const asm_ast::Function& fn, bool pseudos, bool phis);

// Throws `InternalError` if the checks are on and a function of `prog`
// breaks an asm invariant, naming the first such function.
void check_asm(const asm_ast::Program& prog, std::string_view after, bool pseudos = false,
               bool phis = false);

// Returns what's wrong with the decimal-mode regions of a function's
// asm2 instructions.
std::vector<std::string> decimal_region_problems(
    const std::vector<asm2_ast::Type_instruction>& instrs);

// Throws `InternalError` if the checks are on and the decimal flag is
// set anywhere in `prog` but across a packed BCD chain.
void check_asm2(const asm2_ast::Program& prog, std::string_view after);

}  // namespace c6502::passes::ir_check
