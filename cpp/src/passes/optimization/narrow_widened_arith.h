// The helpers of passes/optimization/narrow_widened_arith.py that other
// passes read: whether a Var keeps its value between two instructions,
// the registration of a new temporary, the byte width and signedness of
// a Var's type, and the narrowing of a constant to a width.
// `recognize_indirect_indexed` and `sink_increment` read them. The pass
// itself, `narrow_widened_arith`, is group D's of step 5, which adds it
// to these files.
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
// instruction that names it as a destination. An entry of
// `env.instructions` that an earlier rewrite made a bare list stands
// for nothing here, as Python's tests of a list's type and defs find
// nothing in it. `ctx.ssa_dsts` and `ctx.symbols` must not be null.
bool var_unchanged(const tac_ast::Var& var, std::size_t start, std::size_t end,
                   const framework::DefUseEnv& env, const framework::PassContext& ctx);

}  // namespace c6502::passes

namespace c6502::passes::narrow_widened_arith {

// Registers `name` as a local with the type of `like`, unless `name` is
// in the symbol table already, and adds it to the SSA names in either
// case (Python's `_register_temp`). Throws `InternalError` when `like`
// has no symbol, where Python raises `AttributeError`.
void register_temp(const framework::PassContext& ctx, std::string_view like,
                   const std::string& name);

// Returns a constant of `width` bytes and the given signedness holding
// the value of `c` modulo 2^(8 * width) (Python's `_narrow_constant`),
// or none for a floating constant or a width other than 1, 2, 4, and 8.
std::optional<tac_ast::Constant> narrow_constant(const tac_ast::Constant& c, int width,
                                                 bool is_unsigned);

// Returns the byte width of `v`'s declared type without its qualifiers
// (Python's `_byte_width`), or none when `v` has no symbol or its type
// is an aggregate, a bit-precise type, or another type of no width here.
// `symbols` may be null, which gives none.
std::optional<int> byte_width(const tac_ast::Var& v, const SymbolTable* symbols);

// Returns whether `v`'s declared type is unsigned (Python's
// `_is_unsigned`): true for `char`, `unsigned char`, the unsigned
// integer types, and a pointer, false for the signed integer types, and
// none for any other type or a Var with no symbol. `symbols` may be
// null, which gives none.
std::optional<bool> is_unsigned(const tac_ast::Var& v, const SymbolTable* symbols);

}  // namespace c6502::passes::narrow_widened_arith
