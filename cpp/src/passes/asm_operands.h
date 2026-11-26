#pragma once

#include <type_traits>
#include <variant>
#include <vector>

#include "asm_ast.h"

namespace c6502::passes {

namespace detail {

template <class T>
inline constexpr bool is_operand_v =
    std::is_same_v<std::remove_cvref_t<T>, asm_ast::Type_operand>;

template <class T>
inline constexpr bool is_operand_list_v =
    std::is_same_v<std::remove_cvref_t<T>, std::vector<asm_ast::Type_operand>>;

template <bool lists, class Field, class Fn>
void visit_field(const Field& field, Fn& fn) {
  if constexpr (is_operand_v<Field>) {
    fn(field);
  } else if constexpr (lists && is_operand_list_v<Field>) {
    for (const auto& op : field) {
      fn(op);
    }
  }
}

}  // namespace detail

// Calls `fn(const asm_ast::Type_operand&)` on each operand field of
// `instr`, in field order. With `lists` set it also visits the operands
// in a list field (a `DecimalArith`'s three lists) and the sources of a
// `Phi`'s arguments. Without it, only a field that holds one operand is
// visited.
template <bool lists, class Fn>
void for_each_operand(const asm_ast::Type_instruction& instr, Fn&& fn) {
  std::visit(
      [&](const auto& ins) {
        if constexpr (requires { ins.src; }) {
          detail::visit_field<lists>(ins.src, fn);
        }
        if constexpr (requires { ins.src1; }) {
          detail::visit_field<lists>(ins.src1, fn);
        }
        if constexpr (requires { ins.src2; }) {
          detail::visit_field<lists>(ins.src2, fn);
        }
        if constexpr (requires { ins.left; }) {
          detail::visit_field<lists>(ins.left, fn);
        }
        if constexpr (requires { ins.right; }) {
          detail::visit_field<lists>(ins.right, fn);
        }
        if constexpr (requires { ins.dst; }) {
          detail::visit_field<lists>(ins.dst, fn);
        }
        if constexpr (lists && requires { ins.args; }) {
          for (const auto& arg : ins.args) {
            fn(std::get<asm_ast::AsmPhiArg>(arg).source);
          }
        }
      },
      instr);
}

}  // namespace c6502::passes
