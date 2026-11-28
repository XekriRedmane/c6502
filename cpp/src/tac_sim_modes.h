#pragma once

#include <optional>
#include <string>

#include "passes/front_end.h"
#include "passes/type_checking.h"
#include "tac_ast.h"
#include "tac_sim.h"

namespace c6502::tac_sim_modes {

// The TAC of a program and the tables it was translated with.
struct Translated {
  tac_ast::Program tac;
  passes::SymbolTable symbols;
  passes::TypeTable types;
};

// Runs the front end on `source` (the preprocessor, the parser, the
// three passes that resolve names, string lifting, and the type
// checker, without `-O`'s unrolling) and translates the program to TAC.
// With `fell_off`, each function definition that returns a value, other
// than `main`, ends with a call of the function that `falls_off_end`
// declares. What the preprocessor
// writes to stderr is appended to `err`. Throws the `CompileError` of
// the pass that rejects the program.
Translated translate(const std::string& source, const passes::FrontEndOptions& options,
                     std::string& err, bool fell_off = false);

// What `main` returns on the TAC of `t`, and on the TAC after the TAC
// optimizer. `optimized` is empty when the first run raised an error.
struct PlainAndOptimized {
  tac_sim::Outcome plain;
  std::optional<tac_sim::Outcome> optimized;
};
PlainAndOptimized run_plain_and_optimized(Translated t);

// The first step of the TAC optimizer after which `main` returns
// something other than on the unoptimized TAC, with the function the
// step changed and both results. A step is named by the Python class of
// its pass (`NamedPass::class_name`), or `to_ssa` or `from_ssa`.
struct ChangingPass {
  std::string function;
  std::string pass;
  tac_sim::Outcome expected;
  tac_sim::Outcome got;
};

std::optional<ChangingPass> first_changing_pass(Translated& t);

// Returns whether a run of `main` on the TAC of `source` reaches the end
// of a function, other than `main`, that returns a value. A program that
// the front end rejects, or whose run raises another of the
// interpreter's errors, gives false, and a compiler bug throws its
// `InternalError`.
bool falls_off_end(const std::string& source, const passes::FrontEndOptions& options,
                   std::string& err);

// Returns an outcome as the JSON of check-golden's `result` and
// `optimized` lines: `{"value": <int or null>}`, or `{"error": {"type":
// ..., "message": ...}}` with the Python class and message of what the
// interpreter raised.
std::string outcome_json(const tac_sim::Outcome& o);

// Returns `found` as the JSON of check-golden's `found` line: `null`
// when no step changes `main`'s result, or `[function, step, expected,
// got]` for the first that does, where a result is an integer, `null`
// for no value, or `"ERR <type>: <message>"`.
std::string changing_pass_json(const std::optional<ChangingPass>& found);

}  // namespace c6502::tac_sim_modes
