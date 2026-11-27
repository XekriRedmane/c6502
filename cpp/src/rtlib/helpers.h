// The runtime helpers, the 6502 routines that compiled code calls to
// multiply, divide, shift by a variable count, and convert to packed
// BCD. helpers.cpp builds each one as an `asm_ast::Function`. The linker
// adds the ones a program calls (passes/runtime_library.h), the HARGS
// footprints are measured on them (passes::helper_footprints), and the
// simulator assembles all of them at $E000, reading them through
// `c6502cpp helper-dump`.
//
// Each helper reads its inputs at the low offsets of HARGS and writes
// its outputs at higher offsets, as `tac_to_asm` passes them, and the
// inputs survive the call. docs/architecture.md gives each layout. A
// helper may use more of HARGS as scratch, since HARGS is caller-saved.
//
// A shift by a count of the width or more gives 0 (`asl`, `lsr`) or the
// sign in every bit (`asr`), as the simulator's Python hooks do. A
// division by zero gives the dividend as the remainder, a quotient of
// all ones from `udivmod`, and from `sdivmod` a quotient of -1 for a
// dividend of 0 or more and 1 for a negative one. On 18 Nov 2026 each
// helper gave the same bytes as its Python hook on edge cases and
// random operands, and on 19 Nov 2026 `bin16_to_bcd` on all 65536
// inputs.
#pragma once

#include <string>
#include <vector>

#include "asm_ast.h"

namespace c6502::rtlib {

// Returns every helper that has 6502 code, in the order the simulator
// assembles them and the runtime library lays them out. The helpers
// are built on the first call, which checks them with `check_acyclic`.
const std::vector<asm_ast::Function>& all_helper_functions();

// Returns the dump of the program of every helper, in the dump format
// (ir_dump.py), which `c6502cpp helper-dump` writes.
std::string helper_dump();

// Returns the names of the helpers that set the decimal flag and must
// clear it before their `RTS`, in code-point order. `ir_check` checks
// them in place of the rule for compiled code, which allows `SED` only
// at the start of a packed BCD chain.
const std::vector<std::string>& decimal_mode_helpers();

// Throws `InternalError` when a helper reaches itself through the
// helpers of `fns` that it calls or jumps to. A footprint
// (passes::helper_footprints) counts the helpers a helper calls, which
// ends only without a cycle.
void check_acyclic(const std::vector<asm_ast::Function>& fns);

}  // namespace c6502::rtlib
