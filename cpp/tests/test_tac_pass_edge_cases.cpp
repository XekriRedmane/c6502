// Calls of the TAC optimizer's passes on inputs that the corpus doesn't
// reach, written by hand, each with the result the pass must give.
#include <initializer_list>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "asdl/runtime.h"
#include "doctest.h"
#include "errors.h"
#include "passes/optimization/narrow_widened_arith.h"
#include "passes/optimization/sink_and_past_branch.h"
#include "passes/optimization/static_const_fold.h"
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

// Returns a symbol table of locals, the names in `bytes` unsigned chars
// and those in `ints` ints.
c6502::passes::SymbolTable locals(std::initializer_list<const char*> bytes,
                                  std::initializer_list<const char*> ints) {
  c6502::passes::SymbolTable symbols;
  for (const char* name : bytes) {
    symbols.insert_or_assign(name, c6502::passes::Symbol{.type = c6502::c99_ast::UChar{},
                                                         .attrs = c6502::passes::LocalAttr{}});
  }
  for (const char* name : ints) {
    symbols.insert_or_assign(name, c6502::passes::Symbol{.type = c6502::c99_ast::Int{},
                                                         .attrs = c6502::passes::LocalAttr{}});
  }
  return symbols;
}

// Returns the instructions of `fn` of kind T.
template <class T>
std::vector<T> instrs_of(const ir::Function& fn) {
  std::vector<T> out;
  for (const auto& instr : fn.instructions) {
    if (const auto* i = std::get_if<T>(&instr)) {
      out.push_back(*i);
    }
  }
  return out;
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

TEST_CASE("sink_and_past_branch renames the sunk value in a JumpIfCmp and a Ret") {
  auto symbols = locals({"x", "%t"}, {"%x_ext", "%x_and"});
  const c6502::passes::framework::PassContext ctx{.symbols = &symbols};
  const ir::Function out = c6502::passes::sink_and_past_branch(
      fn_of("  ZeroExtend(Var(\"x\") Var(\"%x_ext\"))\n"
            "  Binary(BitwiseAnd Var(\"%x_ext\") Constant(ConstInt(127)) Var(\"%x_and\"))\n"
            "  Truncate(Var(\"%x_and\") Var(\"%t\"))\n"
            "  JumpIfMasked(Var(\"x\") 128 false \".else\")\n"
            "  Label(\".then\")\n"
            "  JumpIfCmp(Equal Var(\"%t\") Constant(ConstUChar(3)) \".out\")\n"
            "  Ret(None)\n"
            "  Label(\".else\")\n"
            "  Ret(Var(\"%t\"))\n"
            "  Label(\".out\")\n"
            "  Ret(None)\n"),
      ctx);
  const auto jumps = instrs_of<ir::JumpIfCmp>(out);
  REQUIRE(jumps.size() == 1);
  CHECK(jumps[0].src1 == ir::Type_val{ir::Var{.name = "%t.snk_then@0"}});
  const auto rets = instrs_of<ir::Ret>(out);
  REQUIRE(rets.size() == 3);
  CHECK(rets[1].val == std::optional<ir::Type_val>{ir::Var{.name = "%t.snk_else@0"}});
}

TEST_CASE("sink_and_past_branch keeps a value an earlier sink made other blocks read") {
  auto symbols = locals({"t2", "y", "u1"}, {"e1", "a1", "e2", "a2"});
  const c6502::passes::framework::PassContext ctx{.symbols = &symbols};
  // The trio of .b1 goes first and puts reads of t2 into .f1 and .t1,
  // so t2's Truncate in .b2 can't sink into .b1 and .f2 alone.
  const ir::Function out = c6502::passes::sink_and_past_branch(
      fn_of("  Jump(\".b2\")\n"
            "  Label(\".b1\")\n"
            "  ZeroExtend(Var(\"t2\") Var(\"e1\"))\n"
            "  Binary(BitwiseAnd Var(\"e1\") Constant(ConstInt(1)) Var(\"a1\"))\n"
            "  Truncate(Var(\"a1\") Var(\"u1\"))\n"
            "  JumpIfMasked(Var(\"t2\") 2 false \".t1\")\n"
            "  Label(\".f1\")\n"
            "  Ret(Var(\"u1\"))\n"
            "  Label(\".t1\")\n"
            "  Ret(Var(\"u1\"))\n"
            "  Label(\".b2\")\n"
            "  ZeroExtend(Var(\"y\") Var(\"e2\"))\n"
            "  Binary(BitwiseAnd Var(\"e2\") Constant(ConstInt(4)) Var(\"a2\"))\n"
            "  Truncate(Var(\"a2\") Var(\"t2\"))\n"
            "  JumpIfMasked(Var(\"y\") 8 false \".b1\")\n"
            "  Label(\".f2\")\n"
            "  Ret(Var(\"t2\"))\n"),
      ctx);
  const auto truncates = instrs_of<ir::Truncate>(out);
  bool defines_t2 = false;
  for (const auto& t : truncates) {
    defines_t2 = defines_t2 || t.dst == ir::Type_val{ir::Var{.name = "t2"}};
  }
  CHECK(defines_t2);
  CHECK(truncates.size() == 3);
}

TEST_CASE("fold_static_const_reads folds a double's bits and leaves a value tree of a float") {
  namespace c99 = c6502::c99_ast;
  using c6502::passes::ValueTree;
  const auto static_double = [](ValueTree value) {
    return c6502::passes::Symbol{
        .type = c99::Const{.referenced_type = c99::Double{}},
        .attrs = c6502::passes::StaticAttr{.initial_value =
                                               c6502::passes::Initial{.value = std::move(value)}}};
  };
  c6502::passes::SymbolTable symbols;
  symbols.insert_or_assign(
      "bits", static_double(ValueTree{.kind = ValueTree::Kind::kInt, .integer = 0x4000000000000000}));
  symbols.insert_or_assign("two", static_double(ValueTree{.kind = ValueTree::Kind::kFloat,
                                                          .float_bits = 0x4000000000000000}));
  const c6502::passes::framework::PassContext ctx{.symbols = &symbols};
  const ir::Function out = c6502::passes::fold_static_const_reads(
      fn_of("  Copy(Var(\"bits\") Var(\"a\"))\n"
            "  Copy(Var(\"two\") Var(\"b\"))\n"
            "  Ret(None)\n"),
      ctx);
  const auto copies = instrs_of<ir::Copy>(out);
  REQUIRE(copies.size() == 2);
  CHECK(copies[0].src ==
        ir::Type_val{ir::Constant{.const_ = ir::ConstDouble{.bits = 0x4000000000000000}}});
  CHECK(copies[1].src == ir::Type_val{ir::Var{.name = "two"}});
}
