// Builds the asm programs of the pass tests from the dump format
// (ir_dump.py), which is shorter to write and to read than the IR's
// designated initializers, and gives a test's failure as the dumps of
// the instructions.
#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "asm_ast.h"

namespace c6502::asm_text {

// Returns a program of one function, `name`, whose instructions are the
// dump `instrs`, with the instructions separated by whitespace. Any
// statics the test needs go in `before`, as the dumps of top-level
// items followed by a space.
inline asm_ast::Program program(std::string_view instrs, std::string_view name = "f",
                                std::string_view before = "") {
  std::string text = "Program([";
  text += before;
  text += "Function(\"";
  text += name;
  text += "\" false [] [";
  text += instrs;
  text += "])])";
  return asdl::load<asm_ast::Program>(text);
}

// Returns the dumps of `instrs`, one per line.
inline std::string dump(const std::vector<asm_ast::Type_instruction>& instrs) {
  std::string out;
  for (const auto& i : instrs) {
    out += asdl::dump(i);
  }
  return out;
}

// Returns the dumps of the instructions of the first function of
// `prog`, one per line.
inline std::string instructions(const asm_ast::Program& prog) {
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = asdl::as<asm_ast::Function>(tl)) {
      return dump(fn->instructions);
    }
  }
  return "";
}

// Returns the instructions that the dump `instrs` holds, one per line,
// in the form `instructions` returns, so a test can compare a pass's
// result with the text of the instructions it expects.
inline std::string expect(std::string_view instrs) {
  return instructions(program(instrs));
}

// Returns the program whose top-level items' dumps are `items`, such as
// `Function("f" true [] [Return(false false false)])`.
inline asm_ast::Program top_level(std::string_view items) {
  return asdl::load<asm_ast::Program>("Program([" + std::string(items) + "])");
}

// Returns a program of one global function `name` with the instructions
// whose dumps are `body`.
inline asm_ast::Program function(std::string_view body, std::string_view name = "f") {
  return top_level("Function(\"" + std::string(name) + "\" true [] [" + std::string(body) +
                   "])");
}

// Returns the dump of `prog`, for comparing two programs with a
// readable difference.
inline std::string dump(const asm_ast::Program& prog) { return asdl::dump(prog); }

}  // namespace c6502::asm_text
