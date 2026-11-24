// The port of passes/ir_check.py, which checks the IR between passes,
// so that a pass that breaks it fails there.
//
// An end-to-end test or a fuzzer finds a miscompile only when it
// changes a program's result for the inputs it runs. A pass that leaves
// the IR inconsistent, for example reading a temporary nothing defines,
// often gives the right result anyway, and when it doesn't, the wrong
// value shows up many passes later. These checks run after every pass
// instead and name the pass that broke the IR.
//
// They're off unless `enable()` turns them on, as Python's are unless
// `C6502_CHECK_IR=1` is set. `c6502cpp backend` turns them on for a
// request whose `check_ir` is set, which cpp_backend.py sets from the
// variable or from Python's `ir_check.enable()`, as the fuzzers call
// it. A failed check throws `InternalError` with the message Python's
// `IRCheckError` has, which cpp_backend.py raises as an AssertionError.
//
// The checks run where Python runs them (cpp/README.md, "The IR
// checks"): the TAC after `c99_to_tac` and after every pass of the TAC
// optimizer's driver, SSA construction and destruction included; the
// asm after `tac_to_asm`, after the asm optimizer, after each sweep of
// the peephole fixed point, and at the end of each pipeline; and the
// asm2 program at the end of `lower_to_asm2`.
//
// TAC (`check_tac`):
//
//   * Labels are unique, and every jump goes to one of them.
//   * Every variable is in the symbol table.
//   * In SSA form, an SSA-renamed variable is defined once, and every
//     read of it is dominated by its definition (for a Phi argument,
//     the end of the predecessor it comes from). A parameter's first
//     value is defined at entry. Unreachable blocks are ignored, since
//     SSA renaming never visits them.
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
//   * Every `DecimalArith` (packed BCD) has lists of one length, at
//     least one byte long, and operands of the forms its expansion
//     takes (`decimal_arith::form_problems`).
//   * No `SetDecimal` or `ClearDecimal`, which only the runtime helpers
//     write.
//   * An `INC`, `DEC`, or shift of an array element (`IndexedData`) is
//     indexed by X, since the 6502 has no abs,Y form of them.
//
// Asm2 (`check_asm2`):
//
//   * The decimal flag is set only across a packed BCD chain. Every
//     `SED` is followed by `CLC` and ADCs or by `SEC` and SBCs, each
//     byte as `LDA src; ADC src; STA dst` with no register operand but
//     the A these name, and then a `CLD`. Nothing else is between them,
//     no `CLD` is anywhere else, and no function ends with the flag set.
//     A soft-stack operand's `LDY #off` is part of its access, so it's
//     inside the chain too.
//   * A runtime helper that sets the flag itself
//     (`rtlib::decimal_mode_helpers`, which never runs through the asm
//     passes) passes with a `CLD` right before each `RTS` instead, so
//     that it returns with the flag clear. A compiled function of the
//     same name passes by either rule.
//
// Python keeps the blocks and dominators of the SSA test for each
// control skeleton it has seen, since it checks about 15,000 times in
// 20 fuzzer programs. The C++ check keeps them the same way
// (`ir_check.cpp`), which changes how long a check takes and nothing
// it reports.
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

// Turns the checks on or off for the whole process, as Python's
// `ir_check.enable(on)` does.
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

// Returns what's wrong with the TAC function `fn`, in the order Python
// finds it. `symbols`, when given, is the table every name must be in,
// and `ssa_dsts` the SSA-renamed names, given only while the function is
// in SSA form. The checks of the names' definitions run only when the
// labels and the symbol table have no problem.
std::vector<std::string> tac_problems(const tac_ast::Function& fn, const SymbolTable* symbols,
                                      const NameSet* ssa_dsts);

// Throws `InternalError` with Python's message if the checks are on and
// `fn` breaks a TAC invariant: "after <after>, function '<name>': " and
// the first five of `tac_problems`, joined by "; ".
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
// asm2 instructions. It's public so that a test can give it a broken
// region, as Python's `decimal_region_problems` is.
std::vector<std::string> decimal_region_problems(
    const std::vector<asm2_ast::Type_instruction>& instrs);

// Throws `InternalError` if the checks are on and the decimal flag is
// set anywhere in `prog` but across a packed BCD chain.
void check_asm2(const asm2_ast::Program& prog, std::string_view after);

}  // namespace c6502::passes::ir_check
