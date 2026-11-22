// The runtime helpers of rtlib/helpers.py, the 6502 routines that
// compiled code calls to multiply, divide, shift by a variable count,
// and convert to packed BCD. The helpers' code stays in Python, where
// the simulator reads it, until step 9 of the port, and
// tools/rtlib_table.py copies it into cpp/generated/rtlib_helpers.cpp in
// the dump format, which this header's functions read. The table is
// compiled into the program, so a compile needs no file beside the
// executable to find it. tests/test_rtlib_table.py writes the table
// again and fails when it differs from the one checked in.
#pragma once

#include <string>
#include <vector>

#include "asm_ast.h"

namespace c6502::rtlib {

// Returns every helper that has 6502 code, in the order of Python's
// `all_helper_functions`, which is the order the simulator assembles
// them and the runtime library lays them out. The table is read on the
// first call.
const std::vector<asm_ast::Function>& all_helper_functions();

// Returns the dump of the program of every helper, as `ir_dump.dump`
// gives it.
std::string helper_dump();

// Returns the names of `DECIMAL_MODE_HELPERS`, the helpers that set the
// decimal flag and must clear it before their `RTS`, in code-point
// order.
const std::vector<std::string>& decimal_mode_helpers();

}  // namespace c6502::rtlib
