// The recorded calls of `tac_thread_constant_jumps`, the C++ port of
// passes/optimization/jump_thread.py, with what the Python pass
// returned. They were recorded with the capture harness's wrappers
// while tests/test_jump_thread.py ran, through the TAC optimizer's
// driver and through the Python entry points that the tests call
// directly. They are every call of an entry point on the tests'
// synthetic TAC and every call through the driver that changed the
// function.
#include "doctest.h"
#include "stage_case.h"

TEST_CASE("tac_thread_constant_jumps: test_jump_thread.TestThreading.test_name_not_ssa_blocks (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_thread_constant_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": {\"set\": []}}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".entry\")\n"
      "  JumpIfTrue(Var(\"c\") \".found\")\n"
      "  Label(\".loop\")\n"
      "  JumpIfTrue(Var(\"d\") \".loop\")\n"
      "  Label(\".break\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".found\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".end\")\n"
      "  Phi(Var(\"t.1\") [PhiArg(\".break\" Constant(ConstUChar(0))) PhiArg(\".found\" Constant(ConstUChar(1)))])\n"
      "  JumpIfFalse(Var(\"t.1\") \".if_end\")\n"
      "  Ret(Constant(ConstUChar(7)))\n"
      "  Label(\".if_end\")\n"
      "  Ret(Constant(ConstUChar(9)))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".entry\")\n"
      "  JumpIfTrue(Var(\"c\") \".found\")\n"
      "  Label(\".loop\")\n"
      "  JumpIfTrue(Var(\"d\") \".loop\")\n"
      "  Label(\".break\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".found\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".end\")\n"
      "  Phi(Var(\"t.1\") [PhiArg(\".break\" Constant(ConstUChar(0))) PhiArg(\".found\" Constant(ConstUChar(1)))])\n"
      "  JumpIfFalse(Var(\"t.1\") \".if_end\")\n"
      "  Ret(Constant(ConstUChar(7)))\n"
      "  Label(\".if_end\")\n"
      "  Ret(Constant(ConstUChar(9)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_thread_constant_jumps: test_jump_thread.TestThreading.test_entering_loop_body_blocks (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_thread_constant_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": {\"set\": [\"t.1\", \"u.1\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".entry\")\n"
      "  Jump(\".head\")\n"
      "  Label(\".head\")\n"
      "  Phi(Var(\"t.1\") [PhiArg(\".entry\" Constant(ConstUChar(1))) PhiArg(\".body\" Var(\"u.1\"))])\n"
      "  JumpIfFalse(Var(\"t.1\") \".exit\")\n"
      "  Label(\".body\")\n"
      "  Copy(Var(\"c\") Var(\"u.1\"))\n"
      "  Jump(\".head\")\n"
      "  Label(\".exit\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".entry\")\n"
      "  Jump(\".head\")\n"
      "  Label(\".head\")\n"
      "  Phi(Var(\"t.1\") [PhiArg(\".entry\" Constant(ConstUChar(1))) PhiArg(\".body\" Var(\"u.1\"))])\n"
      "  JumpIfFalse(Var(\"t.1\") \".exit\")\n"
      "  Label(\".body\")\n"
      "  Copy(Var(\"c\") Var(\"u.1\"))\n"
      "  Jump(\".head\")\n"
      "  Label(\".exit\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_thread_constant_jumps: test_jump_thread.TestPrograms.test_found_and_not_found (call 1)") {
  c6502::testing::check_stage_case(
      "tac_thread_constant_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%11.1\", \"%11.2\", \"%11.3\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%39.1\", \"%40.1\", \"%41.1\", \"%42.1\", \"%43.1\", \"%44.1\", \"%45.1\", \"%46.1\", \"%47.1\", \"%48.1\", \"%49.1\", \"@2.v\", \"@5.v.1\", \"@6.i.1\", \"@6.i.2\", \"@6.i.3\"]}}}",
      "Function(\"f\" true [\"@2.v\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Copy(Var(\"n\") Var(\"@6.i.1\"))\n"
      "  Label(\".i0_loop@0_start\")\n"
      "  Phi(Var(\"@6.i.2\") [PhiArg(\".f@ssa_block@0\" Var(\"@6.i.1\")) PhiArg(\".i0_loop@0_continue\" Var(\"%47.1\"))])\n"
      "  ZeroExtend(Var(\"@6.i.2\") Var(\"%39.1\"))\n"
      "  Copy(Var(\"@6.i.2\") Var(\"%40.1\"))\n"
      "  IndexedLoad(\"a\" Var(\"%40.1\") Var(\"%41.1\") false 0 0)\n"
      "  JumpIfCmp(NotEqual Var(\"%41.1\") Var(\"@2.v\") \".i0_if_end@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Binary(Add Var(\"hits\") Constant(ConstUChar(1)) Var(\"hits\"))\n"
      "  Jump(\".i0@end\")\n"
      "  Label(\".i0_if_end@0\")\n"
      "  Label(\".i0_loop@0_continue\")\n"
      "  Binary(Subtract Var(\"@6.i.2\") Constant(ConstUChar(1)) Var(\"%47.1\"))\n"
      "  JumpIfTrue(Var(\"%47.1\") \".i0_loop@0_start\")\n"
      "  Label(\".i0_loop@0_break\")\n"
      "  Label(\".i0@end\")\n"
      "  Phi(Var(\"%11.3\") [PhiArg(\".f@ssa_block@1\" Constant(ConstUChar(1))) PhiArg(\".i0_loop@0_break\" Constant(ConstUChar(0)))])\n"
      "  JumpIfFalse(Var(\"%11.3\") \".if_end@1\")\n"
      "  Ret(Constant(ConstUChar(10)))\n"
      "  Label(\".if_end@1\")\n"
      "  Binary(Add Var(\"hits\") Constant(ConstUChar(2)) Var(\"hits\"))\n"
      "  Ret(Constant(ConstUChar(20)))\n"
      "])\n",
      "Function(\"f\" true [\"@2.v\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Copy(Var(\"n\") Var(\"@6.i.1\"))\n"
      "  Label(\".i0_loop@0_start\")\n"
      "  Phi(Var(\"@6.i.2\") [PhiArg(\".f@ssa_block@0\" Var(\"@6.i.1\")) PhiArg(\".i0_loop@0_continue\" Var(\"%47.1\"))])\n"
      "  ZeroExtend(Var(\"@6.i.2\") Var(\"%39.1\"))\n"
      "  Copy(Var(\"@6.i.2\") Var(\"%40.1\"))\n"
      "  IndexedLoad(\"a\" Var(\"%40.1\") Var(\"%41.1\") false 0 0)\n"
      "  JumpIfCmp(NotEqual Var(\"%41.1\") Var(\"@2.v\") \".i0_if_end@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Binary(Add Var(\"hits\") Constant(ConstUChar(1)) Var(\"hits\"))\n"
      "  Jump(\".f@jthread@0\")\n"
      "  Label(\".i0_if_end@0\")\n"
      "  Label(\".i0_loop@0_continue\")\n"
      "  Binary(Subtract Var(\"@6.i.2\") Constant(ConstUChar(1)) Var(\"%47.1\"))\n"
      "  JumpIfTrue(Var(\"%47.1\") \".i0_loop@0_start\")\n"
      "  Label(\".i0_loop@0_break\")\n"
      "  Jump(\".if_end@1\")\n"
      "  Label(\".i0@end\")\n"
      "  Phi(Var(\"%11.3\") [])\n"
      "  JumpIfFalse(Var(\"%11.3\") \".if_end@1\")\n"
      "  Label(\".f@jthread@0\")\n"
      "  Ret(Constant(ConstUChar(10)))\n"
      "  Label(\".if_end@1\")\n"
      "  Binary(Add Var(\"hits\") Constant(ConstUChar(2)) Var(\"hits\"))\n"
      "  Ret(Constant(ConstUChar(20)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_thread_constant_jumps: test_jump_thread.TestPrograms.test_and_or_results (call 1)") {
  c6502::testing::check_stage_case(
      "tac_thread_constant_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%29.2\", \"%29.3\", \"%29.4\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"%39.1\", \"%40.1\", \"%40.2\", \"%40.3\", \"%41.1\", \"%41.2\", \"%41.3\", \"%42.1\", \"%43.1\", \"%44.1\", \"%45.1\", \"%46.1\", \"%47.1\", \"%47.2\", \"%47.3\", \"%48.1\", \"%49.1\", \"@0.r.1\", \"@0.r.2\", \"@0.r.3\", \"@4.k.1\", \"@4.k.2\", \"@4.k.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.r.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstInt(0))) PhiArg(\".loop@0_continue\" Var(\"%31.1\"))])\n"
      "  Phi(Var(\"@4.k.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%33.1\"))])\n"
      "  ZeroExtend(Var(\"@4.k.2\") Var(\"%17.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@4.k.2\") Constant(ConstUChar(5)) \".loop@0_break\")\n"
      "  Copy(Var(\"@4.k.2\") Var(\"%20.1\"))\n"
      "  IndexedLoad(\"@1.xs\" Var(\"%20.1\") Var(\"x\") false 0 0)\n"
      "  Copy(Var(\"@4.k.2\") Var(\"%23.1\"))\n"
      "  IndexedLoad(\"@2.ys\" Var(\"%23.1\") Var(\"y\") false 0 0)\n"
      "  Copy(Var(\"@4.k.2\") Var(\"%26.1\"))\n"
      "  IndexedLoad(\"@3.zs\" Var(\"%26.1\") Var(\"z\") false 0 0)\n"
      "  Binary(LeftShift Var(\"@0.r.2\") Constant(ConstInt(3)) Var(\"%28.1\"))\n"
      "  JumpIfTrue(Var(\"x\") \".i0_and_false@0\")\n"
      "  JumpIfTrue(Var(\"y\") \".i0_or_true@2\")\n"
      "  JumpIfTrue(Var(\"z\") \".i0_or_true@2\")\n"
      "  Label(\".main@ssa_block@4\")\n"
      "  Jump(\".i0_or_end@3\")\n"
      "  Label(\".i0_or_true@2\")\n"
      "  Label(\".i0_or_end@3\")\n"
      "  Phi(Var(\"%40.3\") [PhiArg(\".main@ssa_block@4\" Constant(ConstInt(0))) PhiArg(\".i0_or_true@2\" Constant(ConstInt(1)))])\n"
      "  JumpIfFalse(Var(\"%40.3\") \".i0_and_false@0\")\n"
      "  Label(\".main@ssa_block@5\")\n"
      "  Jump(\".i0_and_end@1\")\n"
      "  Label(\".i0_and_false@0\")\n"
      "  Label(\".i0_and_end@1\")\n"
      "  Phi(Var(\"%41.3\") [PhiArg(\".main@ssa_block@5\" Constant(ConstInt(1))) PhiArg(\".i0_and_false@0\" Constant(ConstInt(0)))])\n"
      "  JumpIfFalse(Var(\"%41.3\") \".i0_if_end@4\")\n"
      "  Label(\".main@ssa_block@6\")\n"
      "  Jump(\".i0@end\")\n"
      "  Label(\".i0_if_end@4\")\n"
      "  JumpIfCmp(Equal Var(\"x\") Constant(ConstUChar(5)) \".i0_or_true@5\")\n"
      "  JumpIfCmp(Equal Var(\"y\") Constant(ConstUChar(6)) \".i0_or_true@5\")\n"
      "  Label(\".main@ssa_block@8\")\n"
      "  Jump(\".i0_or_end@6\")\n"
      "  Label(\".i0_or_true@5\")\n"
      "  Label(\".i0_or_end@6\")\n"
      "  Phi(Var(\"%47.3\") [PhiArg(\".main@ssa_block@8\" Constant(ConstInt(0))) PhiArg(\".i0_or_true@5\" Constant(ConstInt(1)))])\n"
      "  JumpIfFalse(Var(\"%47.3\") \".i0_if_end@7\")\n"
      "  Label(\".main@ssa_block@9\")\n"
      "  Jump(\".i0@end\")\n"
      "  Label(\".i0_if_end@7\")\n"
      "  Label(\".i0@end\")\n"
      "  Phi(Var(\"%29.4\") [PhiArg(\".main@ssa_block@6\" Constant(ConstUChar(3))) PhiArg(\".main@ssa_block@9\" Constant(ConstUChar(4))) PhiArg(\".i0_if_end@7\" Constant(ConstUChar(5)))])\n"
      "  ZeroExtend(Var(\"%29.4\") Var(\"%30.1\"))\n"
      "  Binary(Add Var(\"%28.1\") Var(\"%30.1\") Var(\"%31.1\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@4.k.2\") Constant(ConstUChar(1)) Var(\"%33.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"@0.r.2\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.r.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstInt(0))) PhiArg(\".loop@0_continue\" Var(\"%31.1\"))])\n"
      "  Phi(Var(\"@4.k.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%33.1\"))])\n"
      "  ZeroExtend(Var(\"@4.k.2\") Var(\"%17.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@4.k.2\") Constant(ConstUChar(5)) \".loop@0_break\")\n"
      "  Copy(Var(\"@4.k.2\") Var(\"%20.1\"))\n"
      "  IndexedLoad(\"@1.xs\" Var(\"%20.1\") Var(\"x\") false 0 0)\n"
      "  Copy(Var(\"@4.k.2\") Var(\"%23.1\"))\n"
      "  IndexedLoad(\"@2.ys\" Var(\"%23.1\") Var(\"y\") false 0 0)\n"
      "  Copy(Var(\"@4.k.2\") Var(\"%26.1\"))\n"
      "  IndexedLoad(\"@3.zs\" Var(\"%26.1\") Var(\"z\") false 0 0)\n"
      "  Binary(LeftShift Var(\"@0.r.2\") Constant(ConstInt(3)) Var(\"%28.1\"))\n"
      "  JumpIfTrue(Var(\"x\") \".i0_if_end@4\")\n"
      "  JumpIfTrue(Var(\"y\") \".main@ssa_block@6\")\n"
      "  JumpIfTrue(Var(\"z\") \".main@ssa_block@6\")\n"
      "  Label(\".main@ssa_block@4\")\n"
      "  Jump(\".i0_if_end@4\")\n"
      "  Label(\".i0_or_true@2\")\n"
      "  Label(\".i0_or_end@3\")\n"
      "  Phi(Var(\"%40.3\") [])\n"
      "  JumpIfFalse(Var(\"%40.3\") \".i0_and_false@0\")\n"
      "  Label(\".main@ssa_block@5\")\n"
      "  Jump(\".i0_and_end@1\")\n"
      "  Label(\".i0_and_false@0\")\n"
      "  Label(\".i0_and_end@1\")\n"
      "  Phi(Var(\"%41.3\") [])\n"
      "  JumpIfFalse(Var(\"%41.3\") \".i0_if_end@4\")\n"
      "  Label(\".main@ssa_block@6\")\n"
      "  Jump(\".i0@end\")\n"
      "  Label(\".i0_if_end@4\")\n"
      "  JumpIfCmp(Equal Var(\"x\") Constant(ConstUChar(5)) \".main@ssa_block@9\")\n"
      "  JumpIfCmp(Equal Var(\"y\") Constant(ConstUChar(6)) \".main@ssa_block@9\")\n"
      "  Label(\".main@ssa_block@8\")\n"
      "  Jump(\".i0_if_end@7\")\n"
      "  Label(\".i0_or_true@5\")\n"
      "  Label(\".i0_or_end@6\")\n"
      "  Phi(Var(\"%47.3\") [])\n"
      "  JumpIfFalse(Var(\"%47.3\") \".i0_if_end@7\")\n"
      "  Label(\".main@ssa_block@9\")\n"
      "  Jump(\".i0@end\")\n"
      "  Label(\".i0_if_end@7\")\n"
      "  Label(\".i0@end\")\n"
      "  Phi(Var(\"%29.4\") [PhiArg(\".main@ssa_block@6\" Constant(ConstUChar(3))) PhiArg(\".main@ssa_block@9\" Constant(ConstUChar(4))) PhiArg(\".i0_if_end@7\" Constant(ConstUChar(5)))])\n"
      "  ZeroExtend(Var(\"%29.4\") Var(\"%30.1\"))\n"
      "  Binary(Add Var(\"%28.1\") Var(\"%30.1\") Var(\"%31.1\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@4.k.2\") Constant(ConstUChar(1)) Var(\"%33.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"@0.r.2\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_thread_constant_jumps: test_jump_thread.TestThreading.test_empty_block_reached_by_jumps (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_thread_constant_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": {\"set\": [\"t.1\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".entry\")\n"
      "  JumpIfFalse(Var(\"a\") \".and_false\")\n"
      "  JumpIfFalse(Var(\"b\") \".and_false\")\n"
      "  Label(\".true\")\n"
      "  Jump(\".and_end\")\n"
      "  Label(\".and_false\")\n"
      "  Label(\".and_end\")\n"
      "  Phi(Var(\"t.1\") [PhiArg(\".true\" Constant(ConstUChar(1))) PhiArg(\".and_false\" Constant(ConstUChar(0)))])\n"
      "  JumpIfFalse(Var(\"t.1\") \".if_end\")\n"
      "  Ret(Constant(ConstUChar(1)))\n"
      "  Label(\".if_end\")\n"
      "  Ret(Constant(ConstUChar(2)))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".entry\")\n"
      "  JumpIfFalse(Var(\"a\") \".if_end\")\n"
      "  JumpIfFalse(Var(\"b\") \".if_end\")\n"
      "  Label(\".true\")\n"
      "  Jump(\".f@jthread@0\")\n"
      "  Label(\".and_false\")\n"
      "  Label(\".and_end\")\n"
      "  Phi(Var(\"t.1\") [])\n"
      "  JumpIfFalse(Var(\"t.1\") \".if_end\")\n"
      "  Label(\".f@jthread@0\")\n"
      "  Ret(Constant(ConstUChar(1)))\n"
      "  Label(\".if_end\")\n"
      "  Ret(Constant(ConstUChar(2)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_thread_constant_jumps: test_jump_thread.TestThreading.test_threaded_into_phi_target (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_thread_constant_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": {\"set\": [\"r.1\", \"t.1\", \"v.1\", \"w.1\", \"y.1\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".entry\")\n"
      "  JumpIfTrue(Var(\"c\") \".one\")\n"
      "  Label(\".zero\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".one\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".end\")\n"
      "  Phi(Var(\"t.1\") [PhiArg(\".zero\" Var(\"y.1\")) PhiArg(\".one\" Constant(ConstUChar(5)))])\n"
      "  JumpIfTrue(Var(\"t.1\") \".exit\")\n"
      "  Label(\".mid\")\n"
      "  Jump(\".exit\")\n"
      "  Label(\".exit\")\n"
      "  Phi(Var(\"r.1\") [PhiArg(\".end\" Var(\"v.1\")) PhiArg(\".mid\" Var(\"w.1\"))])\n"
      "  Ret(Var(\"r.1\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".entry\")\n"
      "  JumpIfTrue(Var(\"c\") \".exit\")\n"
      "  Label(\".zero\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".one\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".end\")\n"
      "  Phi(Var(\"t.1\") [PhiArg(\".zero\" Var(\"y.1\"))])\n"
      "  JumpIfTrue(Var(\"t.1\") \".exit\")\n"
      "  Label(\".mid\")\n"
      "  Jump(\".exit\")\n"
      "  Label(\".exit\")\n"
      "  Phi(Var(\"r.1\") [PhiArg(\".end\" Var(\"v.1\")) PhiArg(\".mid\" Var(\"w.1\")) PhiArg(\".entry\" Var(\"v.1\"))])\n"
      "  Ret(Var(\"r.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_thread_constant_jumps: test_jump_thread.TestThreading.test_second_reader_blocks (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_thread_constant_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": {\"set\": [\"t.1\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".entry\")\n"
      "  JumpIfTrue(Var(\"c\") \".found\")\n"
      "  Label(\".loop\")\n"
      "  JumpIfTrue(Var(\"d\") \".loop\")\n"
      "  Label(\".break\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".found\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".end\")\n"
      "  Phi(Var(\"t.1\") [PhiArg(\".break\" Constant(ConstUChar(0))) PhiArg(\".found\" Constant(ConstUChar(1)))])\n"
      "  JumpIfFalse(Var(\"t.1\") \".if_end\")\n"
      "  Ret(Constant(ConstUChar(7)))\n"
      "  Label(\".if_end\")\n"
      "  Copy(Var(\"t.1\") Var(\"g\"))\n"
      "  Ret(Constant(ConstUChar(9)))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".entry\")\n"
      "  JumpIfTrue(Var(\"c\") \".found\")\n"
      "  Label(\".loop\")\n"
      "  JumpIfTrue(Var(\"d\") \".loop\")\n"
      "  Label(\".break\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".found\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".end\")\n"
      "  Phi(Var(\"t.1\") [PhiArg(\".break\" Constant(ConstUChar(0))) PhiArg(\".found\" Constant(ConstUChar(1)))])\n"
      "  JumpIfFalse(Var(\"t.1\") \".if_end\")\n"
      "  Ret(Constant(ConstUChar(7)))\n"
      "  Label(\".if_end\")\n"
      "  Copy(Var(\"t.1\") Var(\"g\"))\n"
      "  Ret(Constant(ConstUChar(9)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_thread_constant_jumps: test_jump_thread.TestThreading.test_variable_argument_stays (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_thread_constant_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": {\"set\": [\"t.1\", \"x.1\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".entry\")\n"
      "  JumpIfTrue(Var(\"c\") \".found\")\n"
      "  Label(\".loop\")\n"
      "  JumpIfTrue(Var(\"d\") \".loop\")\n"
      "  Label(\".break\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".found\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".end\")\n"
      "  Phi(Var(\"t.1\") [PhiArg(\".break\" Var(\"x.1\")) PhiArg(\".found\" Constant(ConstUChar(1)))])\n"
      "  JumpIfFalse(Var(\"t.1\") \".if_end\")\n"
      "  Ret(Constant(ConstUChar(7)))\n"
      "  Label(\".if_end\")\n"
      "  Ret(Constant(ConstUChar(9)))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".entry\")\n"
      "  JumpIfTrue(Var(\"c\") \".f@jthread@0\")\n"
      "  Label(\".loop\")\n"
      "  JumpIfTrue(Var(\"d\") \".loop\")\n"
      "  Label(\".break\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".found\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".end\")\n"
      "  Phi(Var(\"t.1\") [PhiArg(\".break\" Var(\"x.1\"))])\n"
      "  JumpIfFalse(Var(\"t.1\") \".if_end\")\n"
      "  Label(\".f@jthread@0\")\n"
      "  Ret(Constant(ConstUChar(7)))\n"
      "  Label(\".if_end\")\n"
      "  Ret(Constant(ConstUChar(9)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_thread_constant_jumps: test_jump_thread.TestThreading.test_both_constants_thread (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_thread_constant_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": {\"set\": [\"t.1\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".entry\")\n"
      "  JumpIfTrue(Var(\"c\") \".found\")\n"
      "  Label(\".loop\")\n"
      "  JumpIfTrue(Var(\"d\") \".loop\")\n"
      "  Label(\".break\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".found\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".end\")\n"
      "  Phi(Var(\"t.1\") [PhiArg(\".break\" Constant(ConstUChar(0))) PhiArg(\".found\" Constant(ConstUChar(1)))])\n"
      "  JumpIfFalse(Var(\"t.1\") \".if_end\")\n"
      "  Ret(Constant(ConstUChar(7)))\n"
      "  Label(\".if_end\")\n"
      "  Ret(Constant(ConstUChar(9)))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".entry\")\n"
      "  JumpIfTrue(Var(\"c\") \".f@jthread@0\")\n"
      "  Label(\".loop\")\n"
      "  JumpIfTrue(Var(\"d\") \".loop\")\n"
      "  Label(\".break\")\n"
      "  Jump(\".if_end\")\n"
      "  Label(\".found\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".end\")\n"
      "  Phi(Var(\"t.1\") [])\n"
      "  JumpIfFalse(Var(\"t.1\") \".if_end\")\n"
      "  Label(\".f@jthread@0\")\n"
      "  Ret(Constant(ConstUChar(7)))\n"
      "  Label(\".if_end\")\n"
      "  Ret(Constant(ConstUChar(9)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_thread_constant_jumps: test_jump_thread.TestThreading.test_fall_through_successor (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_thread_constant_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": {\"set\": [\"r.1\", \"t.1\", \"v.1\", \"w.1\", \"y.1\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".entry\")\n"
      "  JumpIfTrue(Var(\"c\") \".one\")\n"
      "  Label(\".zero\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".one\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".end\")\n"
      "  Phi(Var(\"t.1\") [PhiArg(\".zero\" Constant(ConstUChar(0))) PhiArg(\".one\" Var(\"y.1\"))])\n"
      "  JumpIfTrue(Var(\"t.1\") \".exit\")\n"
      "  Label(\".mid\")\n"
      "  Jump(\".exit\")\n"
      "  Label(\".exit\")\n"
      "  Phi(Var(\"r.1\") [PhiArg(\".end\" Var(\"v.1\")) PhiArg(\".mid\" Var(\"w.1\"))])\n"
      "  Ret(Var(\"r.1\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".entry\")\n"
      "  JumpIfTrue(Var(\"c\") \".one\")\n"
      "  Label(\".zero\")\n"
      "  Jump(\".mid\")\n"
      "  Label(\".one\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".end\")\n"
      "  Phi(Var(\"t.1\") [PhiArg(\".one\" Var(\"y.1\"))])\n"
      "  JumpIfTrue(Var(\"t.1\") \".exit\")\n"
      "  Label(\".mid\")\n"
      "  Jump(\".exit\")\n"
      "  Label(\".exit\")\n"
      "  Phi(Var(\"r.1\") [PhiArg(\".end\" Var(\"v.1\")) PhiArg(\".mid\" Var(\"w.1\"))])\n"
      "  Ret(Var(\"r.1\"))\n"
      "])\n",
      "");
}
