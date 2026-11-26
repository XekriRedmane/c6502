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

}  // namespace

TEST_CASE("tac_rotate_signed_countdown_loops leaves a loop alone without a symbol table") {
  c6502::testing::check_stage_case(
      "tac_rotate_signed_countdown_loops",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}", kCountdownLoop,
      kCountdownLoop, "");
}
