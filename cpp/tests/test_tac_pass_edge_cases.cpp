// Calls of the TAC optimizer's passes on inputs that the corpus doesn't
// reach, written by hand, each with the result the pass must give.
#include "doctest.h"
#include "stage_case.h"

namespace {

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
