// The calls that the unit tests of tests/test_redundant_store.py make of
// `redundant_store`, each with the program the test gives the pass, the
// call's arguments, the compile's context, and the program the
// Python pass returned, which the C++ pass must return too. They were
// recorded by wrapping the Python pass while its unit tests ran.
#include "doctest.h"
#include "pass_case.h"

using c6502::testing::check_pass_case;

TEST_CASE("redundant_store: TestDptrStagePattern.test_dptr_double_stage_collapses") {
  check_pass_case(
      "redundant_store",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(129 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(IndirectY Reg(A) false)\n"
      "    Mov(Reg(A) ZP(132 0) false)\n"
      "    Mov(Reg(A) IndexedData(\"\" 8192 X) false)\n"
      "    Inc(Reg(Y))\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(129 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(IndirectY Reg(A) false)\n"
      "    Mov(Reg(A) ZP(132 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(129 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(IndirectY Reg(A) false)\n"
      "    Mov(Reg(A) ZP(132 0) false)\n"
      "    Mov(Reg(A) IndexedData(\"\" 8192 X) false)\n"
      "    Inc(Reg(Y))\n"
      "    Mov(IndirectY Reg(A) false)\n"
      "    Mov(Reg(A) ZP(132 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("redundant_store: TestRedundantStoreBasic.test_different_dst_keeps_both") {
  check_pass_case(
      "redundant_store",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("redundant_store: TestRedundantStoreBasic.test_different_src_keeps_both") {
  check_pass_case(
      "redundant_store",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(129 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(129 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("redundant_store: TestRedundantStoreBasic.test_immediate_repeat_pair_dropped") {
  check_pass_case(
      "redundant_store",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("redundant_store: TestRedundantStoreBoundaries.test_branch_in_middle_resets_state") {
  check_pass_case(
      "redundant_store",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Branch(NE \".somewhere\")\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Branch(NE \".somewhere\")\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("redundant_store: TestRedundantStoreBoundaries.test_call_resets_state") {
  check_pass_case(
      "redundant_store",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Call(\"foo\" [] [])\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Call(\"foo\" [] [])\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("redundant_store: TestRedundantStoreBoundaries.test_label_resets_state") {
  check_pass_case(
      "redundant_store",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Label(\".mid\")\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Label(\".mid\")\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("redundant_store: TestRedundantStoreFlagLiveness.test_arithmetic_resets_flags_so_store_drops") {
  check_pass_case(
      "redundant_store",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    ClearCarry\n"
      "    Add(Imm(1) Reg(A))\n"
      "    Branch(NE \".x\")\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Imm(1) Reg(A))\n"
      "    Branch(NE \".x\")\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("redundant_store: TestRedundantStoreFlagLiveness.test_branch_immediately_after_keeps_pair") {
  check_pass_case(
      "redundant_store",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Branch(NE \".x\")\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Branch(NE \".x\")\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("redundant_store: TestRedundantStoreHardwareStack.test_push_pop_does_not_invalidate") {
  check_pass_case(
      "redundant_store",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Push(Reg(A))\n"
      "    Pop(Reg(A))\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Push(Reg(A))\n"
      "    Pop(Reg(A))\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("redundant_store: TestRedundantStoreInvalidation.test_disjoint_zp_store_preserves_tracking") {
  check_pass_case(
      "redundant_store",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Reg(A) ZP(132 0) false)\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Reg(A) ZP(132 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("redundant_store: TestRedundantStoreInvalidation.test_indexed_data_disjoint_preserves") {
  check_pass_case(
      "redundant_store",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Reg(A) IndexedData(\"\" 8360 X) false)\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Reg(A) IndexedData(\"\" 8360 X) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("redundant_store: TestRedundantStoreInvalidation.test_indexed_data_overlap_invalidates") {
  check_pass_case(
      "redundant_store",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Reg(A) IndexedData(\"\" 32 X) false)\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Reg(A) IndexedData(\"\" 32 X) false)\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("redundant_store: TestRedundantStoreInvalidation.test_write_to_dst_invalidates") {
  check_pass_case(
      "redundant_store",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Imm(187) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Imm(187) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("redundant_store: TestRedundantStoreInvalidation.test_write_to_src_invalidates") {
  check_pass_case(
      "redundant_store",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Imm(170) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(128 0) false)\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Imm(170) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(128 0) false)\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("redundant_store: TestRedundantStoreKeepsLiveLoad.test_load_kept_when_a_is_read_after_the_pair") {
  check_pass_case(
      "redundant_store",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"v3\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(128 0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Data(\"v3\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(128 0) false)\n"
      "    SetCarry\n"
      "    Sub(ZP(130 0) Reg(A))\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"v3\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(128 0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Data(\"v3\" 0) Reg(A) false)\n"
      "    SetCarry\n"
      "    Sub(ZP(130 0) Reg(A))\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n");
}
