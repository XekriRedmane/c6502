// tests/test_narrow_byte_range.py's `TestValueRange`, the tests of
// `value_range::max_value` (src/passes/optimization/value_range.h),
// ported by hand in step 8 (group G7), one case per Python test. Each
// function starts with the module's prologue, which widens the bytes
// `b` and `c` to `ze` and `zc` and the signed byte `s` to `se`, and
// every name the tests use is an SSA name.
#include <optional>
#include <string>

#include "asdl/runtime.h"
#include "doctest.h"
#include "passes/optimization/framework.h"
#include "passes/optimization/value_range.h"

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using namespace ::c6502::passes;

Symbol local(c99::Type_data_type t) { return Symbol{.type = std::move(t), .attrs = LocalAttr{}}; }

SymbolTable types() {
  return SymbolTable{{"b", local(c99::UChar{})}, {"c", local(c99::UChar{})},
                     {"s", local(c99::SChar{})}, {"ze", local(c99::Int{})},
                     {"zc", local(c99::Int{})},  {"se", local(c99::Int{})},
                     {"t", local(c99::Int{})},   {"u", local(c99::Int{})},
                     {"r", local(c99::Int{})},   {"w", local(c99::Int{})},
                     {"uw", local(c99::UInt{})}};
}

ir::Function fn_of(const std::string& body) {
  return std::get<ir::Function>(c6502::asdl::load<ir::Type_top_level>(
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  ZeroExtend(Var(\"c\") Var(\"zc\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n" +
      body + "  Ret(Var(\"r\"))\n])\n"));
}

// Returns `max_value` of the variable `name` in `fn`, as the Python
// module's `_bound` gives it.
std::optional<c6502::asdl::Int> bound(const ir::Function& fn, const char* name) {
  const SymbolTable symbols = types();
  NameSet ssa;
  for (const auto& [n, _] : symbols) {
    ssa.insert(n);
  }
  const auto def_idx = framework::build_def_idx(fn.instructions);
  const value_range::DefOf def_of = [&](const ir::Var& v) -> const ir::Type_instruction* {
    const auto it = def_idx.find(v.name);
    return it == def_idx.end() ? nullptr : &fn.instructions[it->second];
  };
  return value_range::max_value(ir::Var{.name = name}, def_of, &symbols, &ssa);
}

}  // namespace

TEST_CASE("value_range: TestValueRange::test_zero_extend_of_a_byte") {
  CHECK(bound(fn_of(""), "ze") == c6502::asdl::Int(255));
}

TEST_CASE("value_range: TestValueRange::test_sign_extend_of_a_signed_byte_is_unbounded") {
  CHECK_FALSE(bound(fn_of(""), "se").has_value());
}

TEST_CASE("value_range: TestValueRange::test_and_with_a_constant_bounds_a_signed_value") {
  const auto fn =
      fn_of("  Binary(BitwiseAnd Var(\"se\") Constant(ConstInt(3)) Var(\"t\"))\n");
  CHECK(bound(fn, "t") == c6502::asdl::Int(3));
}

TEST_CASE("value_range: TestValueRange::test_add_and_shift") {
  const auto fn = fn_of(
      "  Binary(RightShift Var(\"ze\") Constant(ConstInt(5)) Var(\"t\"))\n"
      "  Binary(Add Var(\"t\") Constant(ConstInt(1)) Var(\"u\"))\n");
  CHECK(bound(fn, "t") == c6502::asdl::Int(7));
  CHECK(bound(fn, "u") == c6502::asdl::Int(8));
}

TEST_CASE("value_range: TestValueRange::test_add_that_may_wrap_is_unbounded") {
  const auto fn = fn_of("  Binary(Add Var(\"ze\") Constant(ConstInt(32700)) Var(\"t\"))\n");
  CHECK_FALSE(bound(fn, "t").has_value());
}

TEST_CASE("value_range: TestValueRange::test_unsigned_type_bounds_any_value") {
  CHECK(bound(fn_of(""), "uw") == c6502::asdl::Int(0xFFFF));
  CHECK_FALSE(bound(fn_of(""), "w").has_value());
}
