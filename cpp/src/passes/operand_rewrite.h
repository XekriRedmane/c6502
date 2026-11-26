// Visits every operand of an asm instruction so that it can be changed
// in place, for the passes that rename operands through the whole
// instruction.
#pragma once

#include <type_traits>
#include <variant>
#include <vector>

#include "asm_ast.h"

namespace c6502::passes {

// Calls `fn(asm_ast::Type_operand&)` on each operand of `instr`: each
// field that holds one, each element of a list of them (a
// `DecimalArith`'s three lists), and the source of each `Phi` argument.
// The order is the order of the fields in asm.asdl.
template <class Fn>
void for_each_operand_mut(asm_ast::Type_instruction& instr, Fn&& fn) {
  std::visit(
      [&](auto& ins) {
        const auto visit_field = [&](auto& field) {
          using Field = std::remove_cvref_t<decltype(field)>;
          if constexpr (std::is_same_v<Field, asm_ast::Type_operand>) {
            fn(field);
          } else if constexpr (std::is_same_v<Field,
                                              std::vector<asm_ast::Type_operand>>) {
            for (auto& op : field) {
              fn(op);
            }
          }
        };
        if constexpr (requires { ins.src; }) {
          visit_field(ins.src);
        }
        if constexpr (requires { ins.src1; }) {
          visit_field(ins.src1);
        }
        if constexpr (requires { ins.src2; }) {
          visit_field(ins.src2);
        }
        if constexpr (requires { ins.left; }) {
          visit_field(ins.left);
        }
        if constexpr (requires { ins.right; }) {
          visit_field(ins.right);
        }
        if constexpr (requires { ins.dst; }) {
          visit_field(ins.dst);
        }
        if constexpr (requires { ins.args; }) {
          for (auto& arg : ins.args) {
            fn(std::get<asm_ast::AsmPhiArg>(arg).source);
          }
        }
      },
      instr);
}

}  // namespace c6502::passes
