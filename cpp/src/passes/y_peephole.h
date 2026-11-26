#pragma once

#include <string>
#include <vector>

namespace c6502::passes {

// Returns `lines`, the text `asm_emit` writes, with each `LDY #imm`
// dropped when Y already holds `imm` and written as `INY` or `DEY`
// when Y holds one less or one more. What Y holds is followed from the
// last label, and a `JSR` or `TAY` forgets it.
std::vector<std::string> apply_y_peephole(const std::vector<std::string>& lines);

}  // namespace c6502::passes
