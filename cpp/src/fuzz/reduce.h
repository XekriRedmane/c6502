// The line-based reducer of a failing fuzz program.
//
// It deletes single lines, and then replaces binary subexpressions
// `((a) op (b))` with one operand, keeping every change under which the
// program still fails the same way, until neither changes anything. The
// generators put one statement on a line, so deleting a line usually
// leaves valid C.
//
// "Fails the same way" means the same set of paths gives a wrong value
// as in the original (`disagreements`), and each path still compiles or
// still errors as it did, so a reduction can't wander off to another bug
// or delete `main`. With `tac_opt`, the failure is that the optimized
// TAC gives `main` another result than the unoptimized TAC, which
// narrows a bug of the TAC optimizer without compiling to asm at every
// step.
//
// Deleting a `return` can make a function fall off its end. C99
// 6.9.1p12 makes the use of such a function's value undefined, and the
// -O pipeline may return something else there, so a candidate whose run
// on the TAC interpreter reaches the end of a function that returns a
// value, other than `main`, is never kept (`falls_off_end`). That is
// stricter than C, which allows the fall-off when the caller ignores the
// value, and the generators always end such a function with a `return`.
//
// The reducer doesn't rule out the other kinds of undefined behavior an
// operand replacement can bring in, such as a shift by too many places
// or signed overflow, so a reduced program has to be checked for them by
// hand.
#pragma once

#include <functional>
#include <optional>
#include <string>

#include "asdl/runtime.h"
#include "fuzz/runner.h"

namespace c6502::fuzz {

// Tells whether a candidate program still shows the failure.
using Predicate = std::function<bool(const std::string& candidate)>;

enum class Failure {
  // The same paths are wrong as in the original.
  kPaths,
  // The optimized TAC gives main another result than the unoptimized.
  kTacOptimizer,
};

// Returns whether a run of `src` on the TAC interpreter reaches the end
// of a function, other than `main`, whose return type isn't `void`. A
// program the front end rejects, or that the interpreter can't run,
// gives false.
bool falls_off_end(const Runner& runner, const std::string& src);

// Returns main's result on the unoptimized TAC and on the TAC after the
// TAC optimizer, or none when either run raised or the program is
// rejected.
struct TacResults {
  std::optional<asdl::Int> plain;
  std::optional<asdl::Int> optimized;
};
std::optional<TacResults> tac_results(const Runner& runner, const std::string& src);

// Returns the predicate of the failure `src` shows. A candidate that
// falls off the end of a function returning a value never shows it.
Predicate make_predicate(const Runner& runner, const std::string& src, Failure failure);

// Returns the smallest variant of `src` this simple search finds.
std::string reduce(const std::string& src, const Predicate& still_fails);

}  // namespace c6502::fuzz
