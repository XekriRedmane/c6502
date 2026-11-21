// The TAC optimizer's driver (src/passes/optimization/driver.h): its
// order of the lists and of SSA construction and destruction, its two
// fixed points and their cap, and the table of the passes in
// src/passes/optimization/optimizer.cpp.
//
// The whole driver can't run until every pass of its table is ported,
// so the cases at the end run a driver with only the passes ported so
// far: the two passes before SSA, SSA construction and destruction, and
// the two folds of a truncate or an extend in the fixed point. Each
// case was recorded by running Python's `PhaseDriver` with the same
// lists on a function of a program, `c99_to_tac`'s TAC of the program
// below or of an example, and the symbols the function names, without
// the statics, which give the same result.
#include <string>
#include <string_view>
#include <vector>

#include "asdl/runtime.h"
#include "doctest.h"
#include "errors.h"
#include "json.h"
#include "passes/optimization/driver.h"
#include "passes/optimization/loop_rotate.h"
#include "passes/optimization/noreturn_call.h"
#include "passes/optimization/optimizer.h"
#include "passes/optimization/tac_stage.h"
#include "passes/optimization/truncate_extend_fold.h"
#include "stage_case.h"
#include "tagged_json.h"

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using c6502::passes::LocalAttr;
using c6502::passes::NameSet;
using c6502::passes::Symbol;
using c6502::passes::SymbolTable;
using c6502::passes::framework::NamedPass;
using c6502::passes::framework::PassContext;
using c6502::passes::framework::PhaseDriver;

ir::Function fn_of(const std::string& body) {
  return std::get<ir::Function>(c6502::asdl::load<ir::Type_top_level>(
      "Function(\"f\" true [] [\n" + body + "])\n"));
}

// What the fake passes below saw, in the order they ran.
std::vector<std::string> g_log;

// Logs its name and whether the SSA names are set, and changes nothing.
template <const char* Name>
ir::Function logging_pass(ir::Function fn, const PassContext& ctx) {
  g_log.push_back(std::string(Name) + (ctx.ssa_dsts != nullptr ? " ssa" : ""));
  return fn;
}

constexpr char kA[] = "a";
constexpr char kB[] = "b";
constexpr char kC[] = "c";
constexpr char kD[] = "d";
constexpr char kE[] = "e";
constexpr char kF[] = "f";

// Appends a Label to a function of fewer than three instructions, so
// that a fixed point of it converges in the sweep after the third.
ir::Function grow_to_three(ir::Function fn, const PassContext&) {
  g_log.push_back("grow");
  if (fn.instructions.size() < 3) {
    fn.instructions.insert(fn.instructions.begin(),
                           ir::Label{.name = std::to_string(fn.instructions.size())});
  }
  return fn;
}

// Changes the function every time, so that a fixed point of it never
// converges.
ir::Function always_grow(ir::Function fn, const PassContext&) {
  g_log.push_back("grow");
  fn.instructions.insert(fn.instructions.begin(),
                         ir::Label{.name = std::to_string(fn.instructions.size())});
  return fn;
}

// Runs a driver of the ported passes on the recorded function and
// symbols, and checks the result and the table after it.
void check_partial_driver(std::string_view input, std::string_view symbols_json,
                          std::string_view output, std::string_view symbols_after) {
  PhaseDriver driver;
  driver.pre_ssa = {
      {"rotate_signed_countdown_loops", &c6502::passes::rotate_signed_countdown_loops},
      {"noreturn_calls", &c6502::passes::noreturn_calls},
  };
  driver.fixedpoint = {
      {"fold_truncate_extend", &c6502::passes::fold_truncate_extend},
      {"fold_extend_extend", &c6502::passes::fold_extend_extend},
  };
  auto fn = std::get<ir::Function>(c6502::asdl::load<ir::Type_top_level>(input));
  SymbolTable symbols =
      c6502::tagged::SymbolTableC::decode(c6502::json::parse(std::string(symbols_json)));
  PassContext ctx{.symbols = &symbols};
  NameSet ssa;
  ir::Function out = driver.apply(std::move(fn), ctx, ssa);
  CHECK(c6502::asdl::dump(ir::Type_top_level{std::move(out)}) == output);
  CHECK(c6502::tagged::encode<c6502::tagged::SymbolTableC>(symbols) == symbols_after);
}

}  // namespace

TEST_CASE("PhaseDriver runs the lists in order, with SSA between them") {
  g_log.clear();
  PhaseDriver d;
  d.pre_ssa = {{"a", &logging_pass<kA>}};
  d.pre_fixedpoint = {{"b", &logging_pass<kB>}};
  d.fixedpoint = {{"c", &logging_pass<kC>}};
  d.post_fixedpoint = {{"d", &logging_pass<kD>}};
  d.post_destruction = {{"e", &logging_pass<kE>}};
  d.post_destruction_fixedpoint = {{"f", &logging_pass<kF>}};
  SymbolTable symbols{{"x", Symbol{.type = c99::Int{}, .attrs = LocalAttr{}}}};
  PassContext ctx{.symbols = &symbols};
  NameSet ssa;
  const ir::Function out =
      d.apply(fn_of("  Copy(Constant(ConstInt(1)) Var(\"x\"))\n  Ret(Var(\"x\"))\n"), ctx, ssa);
  // The SSA names stay set after destruction, as Python's do, so the
  // passes after it see them.
  CHECK(g_log == std::vector<std::string>{"a", "b ssa", "c ssa", "d ssa", "e ssa", "f ssa"});
  CHECK(ssa == NameSet{"x.1"});
  CHECK(symbols.contains("x.1"));
  CHECK(c6502::asdl::dump(ir::Type_top_level{out}) ==
        "Function(\"f\" true [] [\n"
        "  Label(\".f@ssa_block@0\")\n"
        "  Copy(Constant(ConstInt(1)) Var(\"x.1\"))\n"
        "  Ret(Var(\"x.1\"))\n"
        "])\n");
}

TEST_CASE("PhaseDriver without a symbol table skips SSA and the passes in SSA form") {
  g_log.clear();
  PhaseDriver d;
  d.pre_ssa = {{"a", &logging_pass<kA>}};
  d.pre_fixedpoint = {{"b", &logging_pass<kB>}};
  d.fixedpoint = {{"c", &logging_pass<kC>}};
  d.post_fixedpoint = {{"d", &logging_pass<kD>}};
  d.post_destruction = {{"e", &logging_pass<kE>}};
  d.post_destruction_fixedpoint = {{"f", &logging_pass<kF>}};
  PassContext ctx;
  NameSet ssa;
  const auto in = fn_of("  Copy(Constant(ConstInt(1)) Var(\"x\"))\n  Ret(Var(\"x\"))\n");
  const ir::Function out = d.apply(in, ctx, ssa);
  CHECK(g_log == std::vector<std::string>{"a", "c", "e", "f"});
  CHECK(out == in);
}

TEST_CASE("PhaseDriver sweeps a fixed point until a sweep changes nothing") {
  g_log.clear();
  PhaseDriver d;
  d.fixedpoint = {{"grow", &grow_to_three}};
  PassContext ctx;
  NameSet ssa;
  const ir::Function out = d.apply(fn_of("  Ret(None)\n"), ctx, ssa);
  // Two sweeps grow the function to three instructions, and the third
  // leaves it alone.
  CHECK(g_log.size() == 3);
  CHECK(out.instructions.size() == 3);
}

TEST_CASE("PhaseDriver stops a fixed point that doesn't converge at the cap") {
  for (const bool after_destruction : {false, true}) {
    g_log.clear();
    PhaseDriver d;
    d.fixedpoint_cap = 5;
    (after_destruction ? d.post_destruction_fixedpoint : d.fixedpoint) = {
        {"grow", &always_grow}};
    PassContext ctx;
    NameSet ssa;
    const ir::Function out = d.apply(fn_of("  Ret(None)\n"), ctx, ssa);
    CHECK(g_log.size() == 5);
    CHECK(out.instructions.size() == 6);
  }
}

TEST_CASE("PhaseDriver throws for a pass without a port") {
  PhaseDriver d;
  d.fixedpoint = {{"constant_fold", nullptr}};
  CHECK_FALSE(d.complete());
  PassContext ctx;
  NameSet ssa;
  CHECK_THROWS_AS(d.apply(fn_of("  Ret(None)\n"), ctx, ssa), c6502::InternalError);
}

TEST_CASE("the driver's table lists Python's passes in its order") {
  // tests/test_capture_passes.py checks the signatures of tac_stage.cpp
  // against Python's `_DRIVER`, and this checks the table against them.
  std::vector<std::string> table;
  const PhaseDriver& d = c6502::passes::tac_driver();
  for (const auto* list : {&d.pre_ssa, &d.pre_fixedpoint, &d.fixedpoint, &d.post_fixedpoint,
                           &d.post_destruction, &d.post_destruction_fixedpoint}) {
    for (const NamedPass& p : *list) {
      table.push_back("tac_" + std::string(p.name));
    }
  }
  std::vector<std::string> signatures;
  for (const auto& sig : c6502::passes::tac_stage::signatures()) {
    const std::string name(sig.name);
    if (name == "tac_to_ssa" || name == "tac_from_ssa") {
      continue;
    }
    if (name == "c99_to_tac") {
      break;
    }
    signatures.push_back(name);
  }
  CHECK(table == signatures);
  CHECK(d.fixedpoint_cap == 256);
}

TEST_CASE("optimize_program leaves the statics alone and optimizes each function") {
  auto prog = c6502::asdl::load<ir::Program>(
      "Program([\n"
      "  StaticVariable(\"g\" true Int [IntInit(3)])\n"
      "  Function(\"f\" true [] [\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n");
  if (c6502::passes::tac_driver().complete()) {
    SymbolTable symbols;
    const auto out = c6502::passes::optimize_program(prog, &symbols);
    REQUIRE(out.top_level.size() == 2);
    CHECK(out.top_level[0] == prog.top_level[0]);
    CHECK(std::holds_alternative<ir::Function>(out.top_level[1]));
  } else {
    // The table isn't full yet, so the driver reaches a pass without a
    // port.
    SymbolTable symbols;
    CHECK_THROWS_AS(c6502::passes::optimize_program(prog, &symbols), c6502::InternalError);
  }
}

TEST_CASE("the partial driver: custom sum") {
  check_partial_driver(
      "Function(\"sum\" true [\"@0.n\" \"@1.k\"] [\n"
      "  Truncate(Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  Copy(Var(\"%0\") Var(\"@2.s\"))\n"
      "  Truncate(Constant(ConstInt(9)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"@3.i\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  SignExtend(Var(\"@3.i\") Var(\"%2\"))\n"
      "  Binary(GreaterOrEqual Var(\"%2\") Constant(ConstInt(0)) Var(\"%3\"))\n"
      "  JumpIfFalse(Var(\"%3\") \".loop@0_break\")\n"
      "  SignExtend(Var(\"@3.i\") Var(\"%4\"))\n"
      "  Truncate(Var(\"%4\") Var(\"%5\"))\n"
      "  IndexedLoad(\"buf\" Var(\"%5\") Var(\"%6\") false 0 0)\n"
      "  Copy(Var(\"%6\") Var(\"%7\"))\n"
      "  SignExtend(Var(\"%7\") Var(\"%8\"))\n"
      "  SignExtend(Var(\"@2.s\") Var(\"%9\"))\n"
      "  Binary(Add Var(\"%9\") Var(\"%8\") Var(\"%10\"))\n"
      "  Truncate(Var(\"%10\") Var(\"%11\"))\n"
      "  Copy(Var(\"%11\") Var(\"@2.s\"))\n"
      "  SignExtend(Var(\"@2.s\") Var(\"%12\"))\n"
      "  SignExtend(Var(\"@0.n\") Var(\"%13\"))\n"
      "  Binary(GreaterThan Var(\"%12\") Var(\"%13\") Var(\"%14\"))\n"
      "  JumpIfFalse(Var(\"%14\") \".if_end@0\")\n"
      "  FunctionCall(\"die\" [] None false true)\n"
      "  Label(\".if_end@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@3.i\") Var(\"%15\"))\n"
      "  Binary(Subtract Var(\"@3.i\") Constant(ConstChar(1)) Var(\"%16\"))\n"
      "  Copy(Var(\"%16\") Var(\"@3.i\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Label(\".loop@1_continue\")\n"
      "  JumpIfFalse(Var(\"@1.k\") \".loop@1_break\")\n"
      "  Copy(Var(\"@1.k\") Var(\"@4.t\"))\n"
      "  ZeroExtend(Var(\"@1.k\") Var(\"%17\"))\n"
      "  Binary(RightShift Var(\"%17\") Constant(ConstInt(1)) Var(\"%18\"))\n"
      "  Truncate(Var(\"%18\") Var(\"%19\"))\n"
      "  Copy(Var(\"%19\") Var(\"@1.k\"))\n"
      "  SignExtend(Var(\"@2.s\") Var(\"%20\"))\n"
      "  ZeroExtend(Var(\"@4.t\") Var(\"%21\"))\n"
      "  Binary(Add Var(\"%20\") Var(\"%21\") Var(\"%22\"))\n"
      "  Truncate(Var(\"%22\") Var(\"%23\"))\n"
      "  Copy(Var(\"%23\") Var(\"@2.s\"))\n"
      "  Jump(\".loop@1_continue\")\n"
      "  Label(\".loop@1_break\")\n"
      "  Ret(Var(\"@2.s\"))\n"
      "])\n",
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"die\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": true}}}}], [\"@0.n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.k\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%12\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%15\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%18\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%20\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%21\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%23\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      "Function(\"sum\" true [\"@0.n\" \"@1.k\"] [\n"
      "  Label(\".sum@ssa_block@0\")\n"
      "  Truncate(Constant(ConstInt(0)) Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"@2.s.1\"))\n"
      "  Truncate(Constant(ConstInt(9)) Var(\"%1.1\"))\n"
      "  Copy(Var(\"%1.1\") Var(\"@3.i.1\"))\n"
      "  Copy(Var(\"@2.s.1\") Var(\"@2.s.2\"))\n"
      "  Copy(Var(\"@3.i.1\") Var(\"@3.i.2\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  ZeroExtend(Var(\"@3.i.2\") Var(\"%4.1\"))\n"
      "  Copy(Var(\"@3.i.2\") Var(\"%5.1\"))\n"
      "  IndexedLoad(\"buf\" Var(\"%5.1\") Var(\"%6.1\") false 0 0)\n"
      "  Copy(Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  SignExtend(Var(\"%7.1\") Var(\"%8.1\"))\n"
      "  SignExtend(Var(\"@2.s.2\") Var(\"%9.1\"))\n"
      "  Binary(Add Var(\"%9.1\") Var(\"%8.1\") Var(\"%10.1\"))\n"
      "  Truncate(Var(\"%10.1\") Var(\"%11.1\"))\n"
      "  Copy(Var(\"%11.1\") Var(\"@2.s.3\"))\n"
      "  SignExtend(Var(\"@2.s.3\") Var(\"%12.1\"))\n"
      "  SignExtend(Var(\"@0.n\") Var(\"%13.1\"))\n"
      "  Binary(GreaterThan Var(\"%12.1\") Var(\"%13.1\") Var(\"%14.1\"))\n"
      "  JumpIfFalse(Var(\"%14.1\") \".if_end@0\")\n"
      "  Label(\".sum@ssa_block@1\")\n"
      "  FunctionCall(\"die\" [] None false true)\n"
      "  Ret(None)\n"
      "  Label(\".if_end@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@3.i.2\") Var(\"%15.1\"))\n"
      "  Binary(Subtract Var(\"@3.i.2\") Constant(ConstChar(1)) Var(\"%16.1\"))\n"
      "  Copy(Var(\"%16.1\") Var(\"@3.i.3\"))\n"
      "  SignExtend(Var(\"@3.i.3\") Var(\"%2.1\"))\n"
      "  Binary(GreaterOrEqual Var(\"%2.1\") Constant(ConstInt(0)) Var(\"%3.1\"))\n"
      "  Copy(Var(\"@2.s.3\") Var(\"@2.s.2\"))\n"
      "  Copy(Var(\"@3.i.3\") Var(\"@3.i.2\"))\n"
      "  JumpIfTrue(Var(\"%3.1\") \".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Copy(Var(\"@2.s.3\") Var(\"@2.s.4\"))\n"
      "  Copy(Var(\"@1.k\") Var(\"@1.k.1\"))\n"
      "  Label(\".loop@1_continue\")\n"
      "  JumpIfFalse(Var(\"@1.k.1\") \".loop@1_break\")\n"
      "  Label(\".sum@ssa_block@2\")\n"
      "  Copy(Var(\"@1.k.1\") Var(\"@4.t.1\"))\n"
      "  ZeroExtend(Var(\"@1.k.1\") Var(\"%17.1\"))\n"
      "  Binary(RightShift Var(\"%17.1\") Constant(ConstInt(1)) Var(\"%18.1\"))\n"
      "  Truncate(Var(\"%18.1\") Var(\"%19.1\"))\n"
      "  Copy(Var(\"%19.1\") Var(\"@1.k.2\"))\n"
      "  SignExtend(Var(\"@2.s.4\") Var(\"%20.1\"))\n"
      "  ZeroExtend(Var(\"@4.t.1\") Var(\"%21.1\"))\n"
      "  Binary(Add Var(\"%20.1\") Var(\"%21.1\") Var(\"%22.1\"))\n"
      "  Truncate(Var(\"%22.1\") Var(\"%23.1\"))\n"
      "  Copy(Var(\"%23.1\") Var(\"@2.s.5\"))\n"
      "  Copy(Var(\"@2.s.5\") Var(\"@2.s.4\"))\n"
      "  Copy(Var(\"@1.k.2\") Var(\"@1.k.1\"))\n"
      "  Jump(\".loop@1_continue\")\n"
      "  Label(\".loop@1_break\")\n"
      "  Ret(Var(\"@2.s.4\"))\n"
      "])\n",
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"die\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": true}}}}], [\"@0.n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.k\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%12\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%15\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%18\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%20\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%21\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%23\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.s.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.i.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.s.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.i.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.s.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%12.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%15.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.i.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.s.4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.k.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.t.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%18.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.k.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%20.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%21.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%23.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.s.5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("the partial driver: custom widen") {
  check_partial_driver(
      "Function(\"widen\" true [\"@5.c\" \"@6.u\"] [\n"
      "  SignExtend(Var(\"@5.c\") Var(\"%24\"))\n"
      "  Copy(Var(\"%24\") Var(\"@7.a\"))\n"
      "  ZeroExtend(Var(\"@6.u\") Var(\"%25\"))\n"
      "  Copy(Var(\"%25\") Var(\"@8.b\"))\n"
      "  Copy(Constant(ConstInt(0)) Var(\"@9.x\"))\n"
      "  Copy(Constant(ConstInt(3)) Var(\"@10.j\"))\n"
      "  Label(\".loop@2_start\")\n"
      "  Binary(GreaterOrEqual Var(\"@10.j\") Constant(ConstInt(0)) Var(\"%26\"))\n"
      "  JumpIfFalse(Var(\"%26\") \".loop@2_break\")\n"
      "  Binary(Add Var(\"@9.x\") Var(\"@10.j\") Var(\"%27\"))\n"
      "  Copy(Var(\"%27\") Var(\"@9.x\"))\n"
      "  Binary(GreaterThan Var(\"@9.x\") Constant(ConstInt(5)) Var(\"%28\"))\n"
      "  JumpIfFalse(Var(\"%28\") \".if_end@1\")\n"
      "  Jump(\".loop@2_break\")\n"
      "  Label(\".if_end@1\")\n"
      "  Label(\".loop@2_continue\")\n"
      "  Copy(Var(\"@10.j\") Var(\"%29\"))\n"
      "  Binary(Subtract Var(\"@10.j\") Constant(ConstInt(1)) Var(\"%30\"))\n"
      "  Copy(Var(\"%30\") Var(\"@10.j\"))\n"
      "  Jump(\".loop@2_start\")\n"
      "  Label(\".loop@2_break\")\n"
      "  Copy(Var(\"@8.b\") Var(\"%31\"))\n"
      "  Binary(Add Var(\"@7.a\") Var(\"%31\") Var(\"%32\"))\n"
      "  Truncate(Var(\"@9.x\") Var(\"%33\"))\n"
      "  SignExtend(Var(\"%33\") Var(\"%34\"))\n"
      "  Binary(Add Var(\"%32\") Var(\"%34\") Var(\"%35\"))\n"
      "  Ret(Var(\"%35\"))\n"
      "])\n",
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@5.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@6.u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@7.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@8.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@9.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@10.j\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%24\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%25\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%26\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%27\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%28\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%29\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%30\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%31\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%32\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%33\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%34\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%35\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      "Function(\"widen\" true [\"@5.c\" \"@6.u\"] [\n"
      "  Label(\".widen@ssa_block@0\")\n"
      "  SignExtend(Var(\"@5.c\") Var(\"%24.1\"))\n"
      "  Copy(Var(\"%24.1\") Var(\"@7.a.1\"))\n"
      "  ZeroExtend(Var(\"@6.u\") Var(\"%25.1\"))\n"
      "  Copy(Var(\"%25.1\") Var(\"@8.b.1\"))\n"
      "  Copy(Constant(ConstInt(0)) Var(\"@9.x.1\"))\n"
      "  Copy(Constant(ConstInt(3)) Var(\"@10.j.1\"))\n"
      "  Copy(Var(\"@9.x.1\") Var(\"@9.x.2\"))\n"
      "  Copy(Var(\"@10.j.1\") Var(\"@10.j.2\"))\n"
      "  Label(\".loop@2_start\")\n"
      "  Binary(Add Var(\"@9.x.2\") Var(\"@10.j.2\") Var(\"%27.1\"))\n"
      "  Copy(Var(\"%27.1\") Var(\"@9.x.3\"))\n"
      "  Binary(GreaterThan Var(\"@9.x.3\") Constant(ConstInt(5)) Var(\"%28.1\"))\n"
      "  JumpIfFalse(Var(\"%28.1\") \".if_end@1\")\n"
      "  Label(\".widen@ssa_block@1\")\n"
      "  Jump(\".loop@2_break\")\n"
      "  Label(\".if_end@1\")\n"
      "  Label(\".loop@2_continue\")\n"
      "  Copy(Var(\"@10.j.2\") Var(\"%29.1\"))\n"
      "  Binary(Subtract Var(\"@10.j.2\") Constant(ConstInt(1)) Var(\"%30.1\"))\n"
      "  Copy(Var(\"%30.1\") Var(\"@10.j.3\"))\n"
      "  Binary(GreaterOrEqual Var(\"@10.j.3\") Constant(ConstInt(0)) Var(\"%26.1\"))\n"
      "  Copy(Var(\"@9.x.3\") Var(\"@9.x.2\"))\n"
      "  Copy(Var(\"@10.j.3\") Var(\"@10.j.2\"))\n"
      "  JumpIfTrue(Var(\"%26.1\") \".loop@2_start\")\n"
      "  Label(\".loop@2_break\")\n"
      "  Copy(Var(\"@8.b.1\") Var(\"%31.1\"))\n"
      "  Binary(Add Var(\"@7.a.1\") Var(\"%31.1\") Var(\"%32.1\"))\n"
      "  Truncate(Var(\"@9.x.3\") Var(\"%33.1\"))\n"
      "  SignExtend(Var(\"%33.1\") Var(\"%34.1\"))\n"
      "  Binary(Add Var(\"%32.1\") Var(\"%34.1\") Var(\"%35.1\"))\n"
      "  Ret(Var(\"%35.1\"))\n"
      "])\n",
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@5.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@6.u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@7.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@8.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@9.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@10.j\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%24\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%25\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%26\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%27\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%28\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%29\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%30\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%31\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%32\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%33\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%34\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%35\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%24.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@7.a.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%25.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@8.b.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@9.x.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@10.j.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@9.x.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@10.j.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%27.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@9.x.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%28.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%29.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%30.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@10.j.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%26.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%31.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%32.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%33.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%34.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%35.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}

TEST_CASE("the partial driver: snd_delay_up.c snd_delay_up") {
  check_partial_driver(
      "Function(\"snd_delay_up\" true [\"@0.pitch\" \"@1.clicks\"] [\n"
      "  Label(\".loop@0_start\")\n"
      "  Copy(Var(\"@0.pitch\") Var(\"@2.y\"))\n"
      "  ZeroExtend(Var(\"@0.pitch\") Var(\"%0\"))\n"
      "  Binary(Add Var(\"%0\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Truncate(Var(\"%1\") Var(\"%2\"))\n"
      "  Copy(Var(\"%2\") Var(\"@0.pitch\"))\n"
      "  Label(\".loop@1_continue\")\n"
      "  Binary(Subtract Var(\"@2.y\") Constant(ConstUChar(1)) Var(\"%3\"))\n"
      "  Copy(Var(\"%3\") Var(\"@2.y\"))\n"
      "  ZeroExtend(Var(\"%3\") Var(\"%4\"))\n"
      "  Binary(NotEqual Var(\"%4\") Constant(ConstInt(0)) Var(\"%5\"))\n"
      "  JumpIfFalse(Var(\"%5\") \".loop@1_break\")\n"
      "  Jump(\".loop@1_continue\")\n"
      "  Label(\".loop@1_break\")\n"
      "  Load(Var(\"sfx_click_ptr\") Var(\"%6\") true)\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Subtract Var(\"@1.clicks\") Constant(ConstUChar(1)) Var(\"%7\"))\n"
      "  Copy(Var(\"%7\") Var(\"@1.clicks\"))\n"
      "  ZeroExtend(Var(\"%7\") Var(\"%8\"))\n"
      "  Binary(NotEqual Var(\"%8\") Constant(ConstInt(0)) Var(\"%9\"))\n"
      "  JumpIfTrue(Var(\"%9\") \".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(None)\n"
      "])\n",
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.pitch\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": true, \"was_bool\": false}}}}], [\"@1.clicks\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": true, \"was_bool\": false}}}}], [\"@2.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Volatile(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": true, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      "Function(\"snd_delay_up\" true [\"@0.pitch\" \"@1.clicks\"] [\n"
      "  Label(\".snd_delay_up@ssa_preheader@0\")\n"
      "  Copy(Var(\"@0.pitch\") Var(\"@0.pitch.1\"))\n"
      "  Copy(Var(\"@1.clicks\") Var(\"@1.clicks.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Copy(Var(\"@0.pitch.1\") Var(\"@2.y\"))\n"
      "  ZeroExtend(Var(\"@0.pitch.1\") Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"%0.1\") Constant(ConstInt(1)) Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Copy(Var(\"%2.1\") Var(\"@0.pitch.2\"))\n"
      "  Label(\".loop@1_continue\")\n"
      "  Binary(Subtract Var(\"@2.y\") Constant(ConstUChar(1)) Var(\"%3.1\"))\n"
      "  Copy(Var(\"%3.1\") Var(\"@2.y\"))\n"
      "  ZeroExtend(Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Binary(NotEqual Var(\"%4.1\") Constant(ConstInt(0)) Var(\"%5.1\"))\n"
      "  JumpIfFalse(Var(\"%5.1\") \".loop@1_break\")\n"
      "  Label(\".snd_delay_up@ssa_block@0\")\n"
      "  Jump(\".loop@1_continue\")\n"
      "  Label(\".loop@1_break\")\n"
      "  Load(Var(\"sfx_click_ptr\") Var(\"%6.1\") true)\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Subtract Var(\"@1.clicks.1\") Constant(ConstUChar(1)) Var(\"%7.1\"))\n"
      "  Copy(Var(\"%7.1\") Var(\"@1.clicks.2\"))\n"
      "  ZeroExtend(Var(\"%7.1\") Var(\"%8.1\"))\n"
      "  Binary(NotEqual Var(\"%8.1\") Constant(ConstInt(0)) Var(\"%9.1\"))\n"
      "  Copy(Var(\"@0.pitch.2\") Var(\"@0.pitch.1\"))\n"
      "  Copy(Var(\"@1.clicks.2\") Var(\"@1.clicks.1\"))\n"
      "  JumpIfTrue(Var(\"%9.1\") \".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(None)\n"
      "])\n",
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.pitch\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": true, \"was_bool\": false}}}}], [\"@1.clicks\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": true, \"was_bool\": false}}}}], [\"@2.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Volatile(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": true, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.pitch.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.clicks.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.pitch.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.clicks.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}");
}
