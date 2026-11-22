// The port of asm_emit.py, which writes an asm2 program as dasm's 6502
// assembly text.
#pragma once

#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "asm2_ast.h"
#include "passes/asm_context.h"

namespace c6502::asm_emit {

// Symbol names with their addresses, in the order of a Python dict.
using OrderedAddrs = std::vector<std::pair<std::string, asdl::Int>>;

// The keyword arguments of Python's `emit_program`.
struct EmitOptions {
  // `zp_slot_symbols`: each symbol the output binds with an `EQU`, in
  // the dict's order, which decides the name `_pick_symbol` gives a
  // `(zp),Y` pointer among several at one address. Empty stands for
  // `None` too, since the Python code tests the dict's truth.
  OrderedAddrs zp_slot_symbols;
  // `link_metadata_lines`: the link metadata block
  // (`zp_link_metadata.format_metadata`), written after the EQU block.
  std::vector<std::string> link_metadata_lines;
  // `preferred_symbols`: for each function, the slot symbols of other
  // functions that hold its values (`slot_sharing.preferred_symbols`).
  std::map<std::string, passes::NameSet, std::less<>> preferred_symbols;
};

// Returns the text of `prog`: the EQU block, the link metadata block,
// and each function and static in order, one blank line apart, after
// `passes/y_peephole.py`'s rewrite of the `LDY` lines. `layout` is the
// runtime's zero-page layout of the compile (`runtime_layout.current()`).
// Throws `CompileError` with Python's type (`ValueError`, `TypeError`)
// and message for an instruction the 6502 can't encode, as Python
// raises.
std::string emit_program(const asm2_ast::Program& prog, const passes::RuntimeLayout& layout,
                         const EmitOptions& options);

// Returns the line of one instruction as `_instr_line` writes it: the
// opcode in capitals at column 3 and the operand, when there is one, at
// column 9, or one space after an opcode that reaches it. The linker
// writes its own code (`__zp_init`, the startup code, and `__spill`)
// with it.
std::string instr_line(std::string_view opcode, std::string_view operand = {});

// Return the lines of one instruction, one function, and one static
// object, as Python's `emit_instruction`, `emit_function`, and
// `emit_static_variable` give them outside `emit_program`, where no
// symbol names a `(zp),Y` pointer and the pointer is written as its
// address.
std::vector<std::string> emit_instruction(const asm2_ast::Type_instruction& instr);
std::vector<std::string> emit_function(const asm2_ast::Function& fn);
std::vector<std::string> emit_static_variable(const asm2_ast::StaticVariable& sv);

}  // namespace c6502::asm_emit
