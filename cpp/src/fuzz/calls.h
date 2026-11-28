// The fuzzer of call graphs, aimed at the calling conventions, ported
// from fuzz/calls.py.
//
// Each program has three to six functions f0..fN arranged as a DAG (fi
// calls fj only for j > i), with extras chosen per seed:
//   * a recursive function `rec`, which can't get zp_abi and so stays on
//     the soft stack. In half the seeds that have it, it keeps locals
//     live across its recursive call marked
//     `[[c6502::save_hardware_stack]]`, which gives it a private pool and
//     saves them on the hardware stack;
//   * the last function called through a function pointer, which also
//     puts it, and every function making that indirect call, on the soft
//     stack;
//   * `register` parameters (A and X) and a `register unsigned char`
//     local (Y), on the functions that will get zp_abi;
//   * pointer parameters that receive the address of a caller's local;
//   * calls inside loops, and globals updated across calls;
//   * functions returning `bool`, half of them marked
//     `[[c6502::return_bool_in_carry]]`, called for their value or as an
//     `if` condition.
// The bodies compute random integer expressions over parameters, locals,
// and call results, and keep earlier locals live across the calls.
//
// The generator has no model of the result, so the TAC interpreter is
// the reference. Undefined behavior is avoided by construction: divisors
// are `(e | 1)` and shift counts `(e & 7)`. Signed overflow does occur,
// and every path wraps it the same way.
#pragma once

#include "asdl/runtime.h"
#include "fuzz/program.h"

namespace c6502::fuzz::calls {

// Returns the program for `seed`, with no expected return.
GeneratedProgram make_program(asdl::Int seed);

}  // namespace c6502::fuzz::calls
