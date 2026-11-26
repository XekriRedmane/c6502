// Calls of the TAC optimizer's passes on inputs that the corpus doesn't
// reach, written by hand, each with the result the pass must give.
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "asdl/runtime.h"
#include "doctest.h"
#include "errors.h"
#include "passes/optimization/narrow_widened_arith.h"
#include "passes/optimization/ssa_construction.h"
#include "passes/optimization/ssa_destruction.h"
#include "stage_case.h"
#include "tac_ast.h"

namespace {

namespace ir = c6502::tac_ast;

ir::Function fn_of(const std::string& body) {
  return std::get<ir::Function>(
      c6502::asdl::load<ir::Type_top_level>("Function(\"f\" true [] [\n" + body + "])\n"));
}

constexpr const char* kCountdownLoop =
    "Function(\"f\" true [] [\n"
    "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
    "  Label(\"L_start\")\n"
    "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
    "  JumpIfFalse(Var(\"%0\") \"L_break\")\n"
    "  Binary(Add Var(\"sum\") Var(\"x\") Var(\"sum\"))\n"
    "  Label(\"L_continue\")\n"
    "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
    "  Copy(Var(\"%1\") Var(\"x\"))\n"
    "  Jump(\"L_start\")\n"
    "  Label(\"L_break\")\n"
    "])\n";

constexpr const char* kIntX =
    "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", "
    "\"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", "
    "\"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": "
    "{\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, "
    "\"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}";

}  // namespace

TEST_CASE("tac_rotate_signed_countdown_loops leaves a loop alone without a symbol table") {
  c6502::testing::check_stage_case(
      "tac_rotate_signed_countdown_loops",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}", kCountdownLoop,
      kCountdownLoop, "");
}

TEST_CASE("tac_rotate_signed_countdown_loops keeps a SignExtend of a counter a load through a pointer writes") {
  c6502::testing::check_stage_case(
      "tac_rotate_signed_countdown_loops", kIntX,
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%cond\"))\n"
      "  JumpIfFalse(Var(\"%cond\") \"L_break\")\n"
      "  SignExtend(Var(\"x\") Var(\"%ext\"))\n"
      "  Copy(Var(\"%ext\") Var(\"scratch\"))\n"
      "  IndirectIndexedLoad(Var(\"p\") Constant(ConstUChar(0)) Var(\"x\") false)\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%new\"))\n"
      "  Copy(Var(\"%new\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  SignExtend(Var(\"x\") Var(\"%ext\"))\n"
      "  Copy(Var(\"%ext\") Var(\"scratch\"))\n"
      "  IndirectIndexedLoad(Var(\"p\") Constant(ConstUChar(0)) Var(\"x\") false)\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%new\"))\n"
      "  Copy(Var(\"%new\") Var(\"x\"))\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%cond\"))\n"
      "  JumpIfTrue(Var(\"%cond\") \"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_rotate_signed_countdown_loops leaves a 24-bit counter alone") {
  const char* loop =
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt24(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt24(0)) Var(\"%0\"))\n"
      "  JumpIfFalse(Var(\"%0\") \"L_break\")\n"
      "  Binary(Add Var(\"sum\") Var(\"x\") Var(\"sum\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt24(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n";
  c6502::testing::check_stage_case(
      "tac_rotate_signed_countdown_loops",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", "
      "\"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", "
      "\"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"BitInt(24 true)\\n\"}, "
      "\"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": "
      "false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      loop, loop, "");
}

TEST_CASE("tac_from_ssa breaks a cycle of copies without a symbol table") {
  c6502::testing::check_stage_case(
      "tac_from_ssa", "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".pre\")\n"
      "  Copy(Constant(ConstInt(7)) Var(\"a\"))\n"
      "  Copy(Constant(ConstInt(11)) Var(\"b\"))\n"
      "  Jump(\".loop_continue\")\n"
      "  Label(\".loop_continue\")\n"
      "  Label(\".join\")\n"
      "  Phi(Var(\"a\") [PhiArg(\".pre\" Constant(ConstInt(0))) PhiArg(\".loop_continue\" Var(\"b\"))])\n"
      "  Phi(Var(\"b\") [PhiArg(\".pre\" Constant(ConstInt(0))) PhiArg(\".loop_continue\" Var(\"a\"))])\n"
      "  Ret(Var(\"a\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".pre\")\n"
      "  Copy(Constant(ConstInt(7)) Var(\"a\"))\n"
      "  Copy(Constant(ConstInt(11)) Var(\"b\"))\n"
      "  Copy(Constant(ConstInt(0)) Var(\"a\"))\n"
      "  Copy(Constant(ConstInt(0)) Var(\"b\"))\n"
      "  Jump(\".loop_continue\")\n"
      "  Label(\".loop_continue\")\n"
      "  Copy(Var(\"a\") Var(\".main@cycle_tmp@1\"))\n"
      "  Copy(Var(\"b\") Var(\"a\"))\n"
      "  Copy(Var(\".main@cycle_tmp@1\") Var(\"b\"))\n"
      "  Label(\".join\")\n"
      "  Ret(Var(\"a\"))\n"
      "])\n",
      "");
}

TEST_CASE("from_ssa rejects a Phi whose destination is a Constant") {
  const ir::Function fn = fn_of(
      "  Label(\".a\")\n"
      "  Jump(\".b\")\n"
      "  Label(\".b\")\n"
      "  Phi(Constant(ConstInt(0)) [PhiArg(\".a\" Var(\"x\"))])\n"
      "  Ret(None)\n");
  CHECK_THROWS_WITH_AS(c6502::passes::from_ssa(fn), "from_ssa: a Phi's destination is a Constant",
                       c6502::InternalError);
}

TEST_CASE("to_ssa renames the operands of a JumpIfCmp and an IndirectIndexedLoad") {
  c6502::passes::SymbolTable symbols;
  for (const char* name : {"x", "y"}) {
    symbols.insert_or_assign(
        name, c6502::passes::Symbol{.type = c6502::c99_ast::Int{},
                                    .attrs = c6502::passes::LocalAttr{}});
  }
  const ir::Function fn = fn_of(
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  IndirectIndexedLoad(Var(\"p\") Constant(ConstUChar(0)) Var(\"y\") false)\n"
      "  JumpIfCmp(LessThan Var(\"x\") Var(\"y\") \".t\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "  Label(\".t\")\n"
      "  Ret(Var(\"y\"))\n");
  const auto [out, ssa] = c6502::passes::to_ssa(fn, symbols);
  const ir::JumpIfCmp* jump = nullptr;
  const ir::IndirectIndexedLoad* load = nullptr;
  for (const auto& instr : out.instructions) {
    if (const auto* j = std::get_if<ir::JumpIfCmp>(&instr)) {
      jump = j;
    }
    if (const auto* l = std::get_if<ir::IndirectIndexedLoad>(&instr)) {
      load = l;
    }
  }
  REQUIRE(jump != nullptr);
  REQUIRE(load != nullptr);
  CHECK(jump->src1 == ir::Type_val{ir::Var{.name = "x.1"}});
  CHECK(jump->src2 == ir::Type_val{ir::Var{.name = "y.1"}});
  CHECK(load->dst == ir::Type_val{ir::Var{.name = "y.1"}});
}

TEST_CASE("to_ssa places a Phi at the frontier of a Phi it placed") {
  c6502::passes::SymbolTable symbols;
  symbols.insert_or_assign("x", c6502::passes::Symbol{.type = c6502::c99_ast::Int{},
                                                      .attrs = c6502::passes::LocalAttr{}});
  const ir::Function fn = fn_of(
      "  Copy(Constant(ConstInt(0)) Var(\"x\"))\n"
      "  Label(\".loop\")\n"
      "  JumpIfTrue(Var(\"c\") \".join\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Label(\".join\")\n"
      "  JumpIfTrue(Var(\"d\") \".loop\")\n"
      "  Ret(Var(\"x\"))\n");
  const auto [out, ssa] = c6502::passes::to_ssa(fn, symbols);
  std::vector<std::string> phi_blocks;
  std::string label;
  for (const auto& instr : out.instructions) {
    if (const auto* l = std::get_if<ir::Label>(&instr)) {
      label = l->name;
    } else if (std::holds_alternative<ir::Phi>(instr)) {
      phi_blocks.push_back(label);
    }
  }
  CHECK(phi_blocks == std::vector<std::string>{".loop", ".join"});
}

TEST_CASE("var_unchanged doesn't look past an instruction a rewrite replaced with a list") {
  c6502::passes::SymbolTable symbols;
  symbols.insert_or_assign("g", c6502::passes::Symbol{.type = c6502::c99_ast::Int{},
                                                      .attrs = c6502::passes::LocalAttr{}});
  c6502::passes::NameSet ssa;
  c6502::passes::framework::PassContext ctx{.symbols = &symbols, .ssa_dsts = &ssa};
  c6502::passes::framework::DefUseEnv env;
  const ir::Type_instruction copy =
      ir::Copy{.src = ir::Var{.name = "g"}, .dst = ir::Var{.name = "t"}};
  env.instructions = {copy, std::nullopt, copy};
  CHECK_FALSE(c6502::passes::var_unchanged(ir::Var{.name = "g"}, 0, 2, env, ctx));
  env.instructions[1] = copy;
  CHECK(c6502::passes::var_unchanged(ir::Var{.name = "g"}, 0, 2, env, ctx));
}

TEST_CASE("tac_move_steps_onto_edges finds a label after the steps it moved") {
  c6502::testing::check_stage_case(
      "tac_move_steps_onto_edges",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": {\"set\": [\"t1\", \"t2\", "
      "\"t3\", \"x1\", \"x2\", \"x3\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"x1\") Constant(ConstInt(1)) Var(\"t1\"))\n"
      "  JumpIfTrue(Var(\"c\") \".lx\")\n"
      "  Copy(Var(\"t1\") Var(\"z1\"))\n"
      "  Binary(Add Var(\"x2\") Constant(ConstInt(1)) Var(\"t2\"))\n"
      "  JumpIfTrue(Var(\"c\") \".ly\")\n"
      "  Copy(Var(\"t2\") Var(\"z2\"))\n"
      "  Binary(Add Var(\"x3\") Constant(ConstInt(1)) Var(\"t3\"))\n"
      "  Jump(\".l\")\n"
      "  Label(\".l\")\n"
      "  Ret(None)\n"
      "  Label(\".n\")\n"
      "  Copy(Var(\"t3\") Var(\"z3\"))\n"
      "  Ret(None)\n"
      "  Label(\".lx\")\n"
      "  Ret(None)\n"
      "  Label(\".ly\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfTrue(Var(\"c\") \".lx\")\n"
      "  Binary(Add Var(\"x1\") Constant(ConstInt(1)) Var(\"z1\"))\n"
      "  JumpIfTrue(Var(\"c\") \".ly\")\n"
      "  Binary(Add Var(\"x2\") Constant(ConstInt(1)) Var(\"z2\"))\n"
      "  Binary(Add Var(\"x3\") Constant(ConstInt(1)) Var(\"t3\"))\n"
      "  Jump(\".l\")\n"
      "  Label(\".l\")\n"
      "  Ret(None)\n"
      "  Label(\".n\")\n"
      "  Copy(Var(\"t3\") Var(\"z3\"))\n"
      "  Ret(None)\n"
      "  Label(\".lx\")\n"
      "  Ret(None)\n"
      "  Label(\".ly\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}
