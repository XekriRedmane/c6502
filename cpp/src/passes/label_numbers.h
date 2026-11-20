// The numbering of the labels that the control-flow passes add, which
// is the same across the whole program, since the simulator's assembler
// rejects one label name in two functions.
#pragma once

#include <initializer_list>
#include <string_view>

#include "asm_ast.h"

namespace c6502::passes {

// Returns one more than the largest N of a label named `<prefix>N`,
// for any of `prefixes`, in the program's functions, or 0 when there is
// none. N is a run of decimal digits that ends the name. It is what the
// Python passes compute with `1 + max(int(m.group(1)) ...)` over a
// regular expression such as `\.thread@(\d+)$`.
asdl::Int next_label_number(const asm_ast::Program& prog,
                            std::initializer_list<std::string_view> prefixes);

}  // namespace c6502::passes
