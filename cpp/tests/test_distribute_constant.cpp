// A test of `distribute_constant`, which has no Python unit tests of its
// own. The expected function is what the Python pass returns for the
// same input.
#include <string>

#include "asdl/runtime.h"
#include "doctest.h"
#include "passes/optimization/distribute_constant.h"

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using c6502::passes::LocalAttr;
using c6502::passes::NameSet;
using c6502::passes::Symbol;
using c6502::passes::SymbolTable;
using c6502::passes::framework::PassContext;

ir::Function fn_of(const std::string& body) {
  return std::get<ir::Function>(c6502::asdl::load<ir::Type_top_level>(
      "Function(\"f\" true [] [\n" + body + "])\n"));
}

std::string dump(const ir::Function& fn) {
  return c6502::asdl::dump(ir::Type_top_level{fn});
}

Symbol local(c99::Type_data_type t) { return Symbol{.type = std::move(t), .attrs = LocalAttr{}}; }

}  // namespace

TEST_CASE("distribute_constant: a widened byte is multiplied before the constant is added") {
  SymbolTable symbols{{"n", local(c99::UChar{})}, {"x", local(c99::Int{})},
                      {"t", local(c99::Int{})},   {"m", local(c99::Int{})},
                      {"t2", local(c99::Int{})},  {"m2", local(c99::Int{})}};
  NameSet ssa{"n", "x", "t", "m", "t2", "m2"};
  const auto out = c6502::passes::distribute_constant(
      fn_of("  ZeroExtend(Var(\"n\") Var(\"x\"))\n"
            "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"t\"))\n"
            "  Binary(Multiply Var(\"t\") Constant(ConstInt(9)) Var(\"m\"))\n"
            "  Binary(Add Constant(ConstInt(3)) Var(\"x\") Var(\"t2\"))\n"
            "  Binary(Multiply Constant(ConstInt(-2)) Var(\"t2\") Var(\"m2\"))\n"),
      PassContext{.symbols = &symbols, .ssa_dsts = &ssa});
  // A negative product is subtracted, so `(x + 3) * -2` is `x * -2 - 6`.
  CHECK(dump(out) ==
        "Function(\"f\" true [] [\n"
        "  ZeroExtend(Var(\"n\") Var(\"x\"))\n"
        "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"t\"))\n"
        "  Binary(Multiply Var(\"x\") Constant(ConstInt(9)) Var(\"m.dc0\"))\n"
        "  Binary(Subtract Var(\"m.dc0\") Constant(ConstInt(9)) Var(\"m\"))\n"
        "  Binary(Add Constant(ConstInt(3)) Var(\"x\") Var(\"t2\"))\n"
        "  Binary(Multiply Var(\"x\") Constant(ConstInt(-2)) Var(\"m2.dc0\"))\n"
        "  Binary(Subtract Var(\"m2.dc0\") Constant(ConstInt(6)) Var(\"m2\"))\n"
        "])\n");
  CHECK(ssa.contains("m.dc0"));
  CHECK(ssa.contains("m2.dc0"));
}
