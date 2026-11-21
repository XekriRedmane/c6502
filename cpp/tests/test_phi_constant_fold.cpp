// Tests of `fold_into_constant_phis`, which has no Python unit tests of
// its own. The expected functions are what the Python pass returns for
// the same inputs.
#include <string>

#include "asdl/runtime.h"
#include "doctest.h"
#include "passes/optimization/phi_constant_fold.h"

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using c6502::passes::LocalAttr;
using c6502::passes::NameSet;
using c6502::passes::NoInitializer;
using c6502::passes::StaticAttr;
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

Symbol global(c99::Type_data_type t) {
  return Symbol{.type = std::move(t),
                .attrs = StaticAttr{.initial_value = NoInitializer{}, .is_global = true}};
}

}  // namespace

TEST_CASE("fold_into_constant_phi: a Truncate into an SSA name takes the Phi's place") {
  SymbolTable symbols{{"c", local(c99::Int{})}, {"c.lo", local(c99::UChar{})},
                      {"B", local(c99::UChar{})}};
  NameSet ssa{"c", "c.lo", "B"};
  const auto out = c6502::passes::fold_into_constant_phis(
      fn_of("  Phi(Var(\"c\") [PhiArg(\".then\" Constant(ConstInt(3))) "
            "PhiArg(\".else\" Constant(ConstInt(2)))])\n"
            "  Truncate(Var(\"c\") Var(\"c.lo\"))\n"
            "  Binary(Add Var(\"c.lo\") Constant(ConstUChar(1)) Var(\"B\"))\n"),
      PassContext{.symbols = &symbols, .ssa_dsts = &ssa});
  CHECK(dump(out) ==
        "Function(\"f\" true [] [\n"
        "  Phi(Var(\"c.lo\") [PhiArg(\".then\" Constant(ConstUChar(3))) "
        "PhiArg(\".else\" Constant(ConstUChar(2)))])\n"
        "  Binary(Add Var(\"c.lo\") Constant(ConstUChar(1)) Var(\"B\"))\n"
        "])\n");
  CHECK(symbols.size() == 3);
}

TEST_CASE("fold_into_constant_phi: an add into a global joins the sums in a new temporary") {
  SymbolTable symbols{{"c.lo", local(c99::UChar{})}, {"gB", global(c99::UChar{})}};
  NameSet ssa{"c.lo"};
  const auto out = c6502::passes::fold_into_constant_phis(
      fn_of("  Phi(Var(\"c.lo\") [PhiArg(\".then\" Constant(ConstUChar(3))) "
            "PhiArg(\".else\" Constant(ConstUChar(2)))])\n"
            "  Binary(Add Var(\"c.lo\") Constant(ConstUChar(1)) Var(\"gB\"))\n"),
      PassContext{.symbols = &symbols, .ssa_dsts = &ssa});
  CHECK(dump(out) ==
        "Function(\"f\" true [] [\n"
        "  Phi(Var(\"gB.p0\") [PhiArg(\".then\" Constant(ConstUChar(4))) "
        "PhiArg(\".else\" Constant(ConstUChar(3)))])\n"
        "  Copy(Var(\"gB.p0\") Var(\"gB\"))\n"
        "])\n");
  REQUIRE(symbols.find("gB.p0") != nullptr);
  CHECK(symbols.find("gB.p0")->type == c99::Type_data_type{c99::UChar{}});
  CHECK(ssa.contains("gB.p0"));
}

TEST_CASE("fold_into_constant_phi: a divide by zero on one path leaves the Phi") {
  SymbolTable symbols{{"c", local(c99::Int{})}, {"d", local(c99::Int{})}};
  NameSet ssa{"c", "d"};
  const std::string body =
      "  Phi(Var(\"c\") [PhiArg(\".a\" Constant(ConstInt(0))) "
      "PhiArg(\".b\" Constant(ConstInt(2)))])\n"
      "  Binary(Divide Constant(ConstInt(6)) Var(\"c\") Var(\"d\"))\n";
  const auto out = c6502::passes::fold_into_constant_phis(
      fn_of(body), PassContext{.symbols = &symbols, .ssa_dsts = &ssa});
  CHECK(dump(out) == dump(fn_of(body)));
}

TEST_CASE("fold_into_constant_phi: an extend isn't taken") {
  SymbolTable symbols{{"c", local(c99::UChar{})}, {"d", local(c99::UInt{})}};
  NameSet ssa{"c", "d"};
  const std::string body =
      "  Phi(Var(\"c\") [PhiArg(\".a\" Constant(ConstUChar(0))) "
      "PhiArg(\".b\" Constant(ConstUChar(1)))])\n"
      "  ZeroExtend(Var(\"c\") Var(\"d\"))\n";
  const auto out = c6502::passes::fold_into_constant_phis(
      fn_of(body), PassContext{.symbols = &symbols, .ssa_dsts = &ssa});
  CHECK(dump(out) == dump(fn_of(body)));
}
