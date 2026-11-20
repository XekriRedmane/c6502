// The part of passes/slot_sharing.py that reads the finished asm
// program, `check_shares`. Finding the shares (`find_shares`) reads the
// TAC and stays in Python.
#pragma once

#include <vector>

#include "asm_ast.h"
#include "passes/slot_coalescing.h"

namespace c6502::passes {

// Throws `InternalError` with Python's message when the finished `prog`
// breaks what the shares rest on: a function writes one of its own
// parameter slots that is in a share. `find_shares` admits only owners
// that never assign the parameter, so such a write would be one that a
// pass after it made up.
void check_shares(const asm_ast::Program& prog, const std::vector<SlotShare>& shares);

}  // namespace c6502::passes
