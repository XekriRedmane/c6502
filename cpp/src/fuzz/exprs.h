// The fuzzer of integer expressions, ported from fuzz/exprs.py.
//
// Each program declares six variables of random integer types, as
// initialized globals or as parameters of `check`, chosen per seed, and
// then evaluates eight random expressions over them. The generator
// computes each expression's value as well, under C99's rules with
// c6502's widths (char 8 bits, int 16, long 32, plain char unsigned),
// and the program compares each result with that constant. `main`
// returns a mask with bit i set when expression i came out wrong, so the
// expected return is 0 on every path.
//
// An expression that would be undefined is drawn again: signed
// overflow, division by zero, a shift count that is negative or not
// less than the width, and a left shift of a negative value. A right
// shift of a negative value is implementation-defined, and c6502 shifts
// arithmetically, so the model does too.
//
// Half the seeds add three bit-precise types with N from 1 to 64 to the
// six standard ones (bit_precise_types.h).
#pragma once

#include "asdl/runtime.h"
#include "fuzz/program.h"

namespace c6502::fuzz::exprs {

// Returns the program for `seed`, with main's correct return, 0.
GeneratedProgram make_program(asdl::Int seed, int n_exprs = 8, int depth = 3);

}  // namespace c6502::fuzz::exprs
