// The calls that the tests of tests/test_dead_label_drop.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("dead_label_drop: test_dead_label_drop.py::TestDeadLabelDrop::test_drops_multiple_orphans (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass dead_label_drop\n"
      "source tests/test_dead_label_drop.py::TestDeadLabelDrop::test_drops_multiple_orphans\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Label(\".ssa_block@0\")\n"
      "    Label(\".ssa_block@1\")\n"
      "    Label(\".ssa_block@2\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("dead_label_drop: test_dead_label_drop.py::TestDeadLabelDrop::test_drops_unreferenced_label (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass dead_label_drop\n"
      "source tests/test_dead_label_drop.py::TestDeadLabelDrop::test_drops_unreferenced_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Label(\".never_targeted\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("dead_label_drop: test_dead_label_drop.py::TestDeadLabelDrop::test_keeps_branch_target (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass dead_label_drop\n"
      "source tests/test_dead_label_drop.py::TestDeadLabelDrop::test_keeps_branch_target\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Branch(EQ \".tgt\")\n"
      "    Label(\".tgt\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Branch(EQ \".tgt\")\n"
      "    Label(\".tgt\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("dead_label_drop: test_dead_label_drop.py::TestDeadLabelDrop::test_keeps_phi_pred_label (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass dead_label_drop\n"
      "source tests/test_dead_label_drop.py::TestDeadLabelDrop::test_keeps_phi_pred_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Label(\".pred\")\n"
      "    Phi(Pseudo(\"x\" 0) [AsmPhiArg(\".pred\" Imm(0))])\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Label(\".pred\")\n"
      "    Phi(Pseudo(\"x\" 0) [AsmPhiArg(\".pred\" Imm(0))])\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("dead_label_drop: test_dead_label_drop.py::TestDeadLabelDrop::test_keeps_referenced_label (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass dead_label_drop\n"
      "source tests/test_dead_label_drop.py::TestDeadLabelDrop::test_keeps_referenced_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Jump(\".here\" [])\n"
      "    Label(\".here\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Jump(\".here\" [])\n"
      "    Label(\".here\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("dead_label_drop: test_dead_label_drop.py::TestDeadLabelDrop::test_no_change_when_all_labels_referenced (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass dead_label_drop\n"
      "source tests/test_dead_label_drop.py::TestDeadLabelDrop::test_no_change_when_all_labels_referenced\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Jump(\".a\" [])\n"
      "    Label(\".a\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Jump(\".a\" [])\n"
      "    Label(\".a\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}
