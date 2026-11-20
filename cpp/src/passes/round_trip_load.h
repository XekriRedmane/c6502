// The port of passes/round_trip_load.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Drops the `LDA M` of each `STA M; LDA M` round trip in the program's
// functions when A and the flags already hold what the load would
// give, rewrites a move from M to memory after `STA M` as a store of A,
// and turns a round trip whose load is there only for its flags into
// `STA M; ORA #$00`. The comment at the top of round_trip_load.cpp
// gives the three patterns and the reasons.
asm_ast::Program apply_round_trip_load_drop(asm_ast::Program prog);

}  // namespace c6502::passes
