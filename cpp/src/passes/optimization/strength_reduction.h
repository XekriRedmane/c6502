// The port of passes/optimization/strength_reduction.py, which rewrites
// a multiply, divide, or remainder by a constant into shifts, masks, and
// adds. `scalar_int_size`, `zero_for_c99_type`, and `two_power_terms`
// are shared: reassoc_offset sizes its operands with the first,
// narrow_byte_range makes its zeros with the second, and
// row_address_tables splits its row sizes with the third.
#pragma once

#include <optional>

#include "asdl/runtime.h"
#include "c99_ast.h"
#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each multiply, divide, and remainder by a constant
// that the rules of the .cpp's module comment take rewritten (Python's
// `RuleSet(*STRENGTH_RULES, name="reduce_strength")`), or `fn` itself
// when none is. A multiply by two or three powers of two adds the
// temporaries `<dst>.sr<k>` to the symbols, and to the SSA names in SSA
// form.
tac_ast::Function reduce_strength(tac_ast::Function fn, const framework::PassContext& ctx);

namespace strength_reduction {

// Returns the byte width of an integer type, or none for any other type
// and for a qualified one, since the qualifiers aren't looked through
// (Python's `_scalar_int_size`).
std::optional<int> scalar_int_size(const c99_ast::Type_data_type& t);

// Returns the TAC zero constant for the c99 type `t`, or none (Python's
// `_zero_for_c99_type`). `char` and `unsigned char` give a `ConstUInt`
// and `signed char` a `ConstInt`, the promoted types, and a qualified
// type gives none.
std::optional<tac_ast::Type_const> zero_for_c99_type(const c99_ast::Type_data_type& t);

// `value` as `2^a + 2^b` or `2^a - 2^b` with a > b >= 0.
struct TwoPowerTerms {
  int a = 0;
  int b = 0;
  // Whether the terms are subtracted, `2^a - 2^b`.
  bool subtract = false;
};

// Returns the terms of `value` (Python's `_two_power_terms`), or none
// when it has no such form. A sum is preferred when both forms exist,
// which is only for 3 = 2 + 1 = 4 - 1. A power of two `2^b` is `2^(b+1) -
// 2^b`. `value` must be positive.
std::optional<TwoPowerTerms> two_power_terms(asdl::Int value);

}  // namespace strength_reduction

}  // namespace c6502::passes
