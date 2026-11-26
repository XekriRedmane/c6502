// How many low bytes of each local of a function can be nonzero, and
// which locals have only their low byte read, for `tac_to_asm`'s
// shifts and comparisons.
#pragma once

#include <functional>
#include <map>
#include <string>

#include "passes/asm_context.h"
#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::passes {

// `tac_to_asm`'s byte width of a TAC value and its test of whether one
// is unsigned.
using ValSize = std::function<int(const tac_ast::Type_val&)>;
using ValTest = std::function<bool(const tac_ast::Type_val&)>;

// Returns each local of `fn` with fewer significant bytes than its
// size, mapped to that number. `symbols` may be null.
std::map<std::string, int, std::less<>> significant_bytes(const tac_ast::Function& fn,
                                                          const SymbolTable* symbols,
                                                          const ValSize& size_of,
                                                          const ValTest& is_unsigned);

// Returns the locals of `fn` defined by one instruction whose every
// read is a `Truncate` to a 1-byte value, so only their low byte is
// ever read.
NameSet low_byte_only(const tac_ast::Function& fn, const SymbolTable* symbols,
                      const ValSize& size_of);

}  // namespace c6502::passes
