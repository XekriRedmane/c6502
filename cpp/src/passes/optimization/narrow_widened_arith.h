#pragma once

#include <optional>
#include <string>
#include <string_view>

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns whether `var` holds at `env.instructions[end]` the value it
// held at `env.instructions[start]`. An SSA-renamed Var has one
// definition, so it always does. Any other Var must be non-volatile,
// with `start` before `end` in one block and nothing between them that
// may write it: no label, return, or jump, no call or store, and no
// instruction that names it as a destination. `ctx.ssa_dsts` and
// `ctx.symbols` must not be null.
bool var_unchanged(const tac_ast::Var& var, std::size_t start, std::size_t end,
                   const framework::DefUseEnv& env, const framework::PassContext& ctx);

// Returns `fn` with each `Truncate` of a wide Binary on promoted
// operands replaced by the Binary at the Truncate's width, or `fn`
// itself when none is. Outside SSA form, or without a symbol table, it
// returns `fn`. A wide operand read through a new Truncate adds the
// temporary `<src>.lo1` or `<src>.lo2` to the symbols and the SSA names.
tac_ast::Function apply_narrow_widened_arith(tac_ast::Function fn,
                                             const framework::PassContext& ctx);

}  // namespace c6502::passes

namespace c6502::passes::narrow_widened_arith {

// Registers `name` as a local with the type of `like`, unless `name` is
// in the symbol table already, and adds it to the SSA names in either
// case. Throws `InternalError` when `like` has no symbol.
void register_temp(const framework::PassContext& ctx, std::string_view like,
                   const std::string& name);

// Returns a constant of `width` bytes and the given signedness holding
// the value of `c` modulo 2^(8 * width), or none for a floating
// constant or a width other than 1, 2, 4, and 8.
std::optional<tac_ast::Constant> narrow_constant(const tac_ast::Constant& c, int width,
                                                 bool is_unsigned);

// Returns the byte width of `v`'s declared type without its
// qualifiers, or none when `v` has no symbol or its type is an
// aggregate, a bit-precise type, or another type of no width here.
// `symbols` may be null, which gives none.
std::optional<int> byte_width(const tac_ast::Var& v, const SymbolTable* symbols);

// Returns whether `v`'s declared type is unsigned. It is true for
// `char`, `unsigned char`, the unsigned integer types, and a pointer,
// false for the signed integer types, and none for any other type or a
// Var with no symbol. `symbols` may be null, which gives none.
std::optional<bool> is_unsigned(const tac_ast::Var& v, const SymbolTable* symbols);

}  // namespace c6502::passes::narrow_widened_arith
