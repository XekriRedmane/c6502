// The fuzzer of statements, for control flow, the assignment operators,
// and every storage class, ported from fuzz/stmts.py.
//
// Each program has a few helper functions and `main`, whose bodies are
// random statements:
//   * `if` / `else`, `for`, `while`, and `do` loops (nested up to three
//     deep, each run 1 to 4 times), `switch` with fall-through and
//     `default`, `break`, `continue`, and early `return` in helpers;
//   * assignments, and `++` / `--` (before and after) and `+=` `-=` `*=`
//     `&=` `|=` `^=` `<<=` `>>=`, as statements and with their value used
//     in an assignment, a `mix()` call, an `if` or `switch` condition, a
//     `return`, or an argument;
//   * objects that are locals (some passed to a helper by pointer, so
//     they're address-taken), block-scope statics, globals, zero-page
//     globals, `*gp` for a global pointer to a global, elements of a
//     global array (with a constant or loop-counter index), and members
//     of a global struct.
// The helpers form a DAG (a helper calls only earlier ones). `mix(v)`
// folds a value into the global checksum `acc`, and `main` ends by
// mixing every global and returns `acc & 0x7FFF`.
//
// The generator builds the program as a tree, renders it to C, and runs
// the same tree in its own interpreter under C99's rules with c6502's
// widths (char 8 bits, int 16, long 32, long long 64), so the program
// carries `// expect: N` and every path is checked against it.
//
// Undefined behavior is avoided by construction where it's structural
// and by rejection where it depends on values:
//   * An expression contains at most one modification (an operator or a
//     call), and the object it modifies appears nowhere else in the
//     statement. A global pointer always points to the same global, so
//     `*gp` and that global count as one object. A pointer parameter
//     points to a caller's local, which the callee can't name.
//   * A call appears only as a whole right-hand side, a `mix()`
//     argument, or an expression statement, with pure arguments.
//   * Loop counters are never written by the loop body, so every loop
//     ends. A `while` increments its counter in the condition and a `do`
//     in the bottom test, so `continue` can't skip the increment.
//   * Signed overflow (including `++` of a signed maximum), a shift
//     count outside the promoted width, and a left shift of a negative
//     value stop the interpreter, and the program is replaced by another
//     drawn from the same seed. Conversion to a narrower signed type
//     wraps, as c6502 implements it.
#pragma once

#include "asdl/runtime.h"
#include "fuzz/program.h"

namespace c6502::fuzz::stmts {

// Returns the program for `seed` and main's correct return. A program
// that has undefined behavior is replaced by the next one drawn from the
// same seed. Throws `std::runtime_error` when 200 draws all do.
GeneratedProgram make_program(asdl::Int seed);

}  // namespace c6502::fuzz::stmts
