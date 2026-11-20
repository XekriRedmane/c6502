// Builds asm programs for the tests from the dump format, which states
// a function's instructions more briefly than the aggregates do.
#pragma once

#include <string>
#include <string_view>

#include "asm_ast.h"

namespace asm_text {

// Returns the program whose top-level items' dumps are `items`, such as
// `Function("f" true [] [Return(false false false)])`.
inline c6502::asm_ast::Program program(std::string_view items) {
  return c6502::asdl::load<c6502::asm_ast::Program>("Program([" +
                                                    std::string(items) + "])");
}

// Returns a program of one global function `name` with the instructions
// whose dumps are `body`.
inline c6502::asm_ast::Program function(std::string_view body,
                                        std::string_view name = "f") {
  return program("Function(\"" + std::string(name) + "\" true [] [" +
                 std::string(body) + "])");
}

// Returns the dump of `prog`, for comparing two programs with a
// readable difference.
inline std::string dump(const c6502::asm_ast::Program& prog) {
  return c6502::asdl::dump(prog);
}

}  // namespace asm_text
