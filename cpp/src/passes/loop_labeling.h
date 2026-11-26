// The labels of loops, switches, and their cases, and the targets of
// `break` and `continue`.
#pragma once

#include "c99_ast.h"

namespace c6502::passes {

// Labels each iteration statement `.loop@N` and each `switch`
// `.switch@N`, gives each `break` and `continue` the label of the
// statement it leaves, and labels each `case` `.case@N` and each
// `default` `.default@N`, collecting them into their switch's `cases`
// and `default_label`. The four forms share one counter over the
// program, which starts at 0. Throws `CompileError` of type
// `LoopLabelingError` for a `break` outside a loop or switch, a
// `continue` outside a loop, a `case` or `default` outside a switch,
// and a second `default` in one switch.
c99_ast::Program label_program(c99_ast::Program prog);

}  // namespace c6502::passes
