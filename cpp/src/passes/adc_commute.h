// The port of passes/adc_commute.py.
#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Drops the `LDA mem` of each `STA temp; ...; LDA mem; [CLC|SEC]; ADC
// temp; [STA mem]` in the program's functions, rewriting the `ADC temp`
// (or `AND temp`, `ORA temp`) to read `mem`, and then runs
// `apply_register_copy_commute` on the result. The comment at the top
// of adc_commute.cpp gives the conditions and the reasons.
asm_ast::Program apply_adc_commute(asm_ast::Program prog, const AsmContext& ctx);

// Rewrites `LDA #k; [CLC | SEC]; ADC m` (or `AND m`, `ORA m`) as `TXA;
// [CLC | SEC]; ADC #k` when X holds the byte `m` (and the same with Y),
// so that the `STX m` that put it there can go if nothing else reads
// `m`. The load may be of a byte `n` in memory instead of a constant,
// when it isn't a volatile access, and `ADC n` then reads it one
// instruction later, with only the `CLC` or `SEC` between. The
// operation is commutative, so A and every flag come out the same, and
// the two forms differ only in A, N, and Z between the load and the
// operation, which nothing there reads.
//
// X holds `m` when the straight-line code before the load last stored X
// there or loaded X from it (not as a volatile access), and no
// instruction since has written X or may have written `m`. A write
// through a pointer counts unless `m` is out of every pointer's reach
// (`unreachable_by_pointer`, installed as the pointer proof), and a
// label, a call, or any other kind of instruction ends the scan. A
// quotient left in X by the byte division loop and then added to a
// constant was `STX t; LDA #$11; CLC; ADC t` and is `TXA; CLC; ADC
// #$11`.
asm_ast::Program apply_register_copy_commute(asm_ast::Program prog,
                                             const AsmContext& ctx);

}  // namespace c6502::passes
