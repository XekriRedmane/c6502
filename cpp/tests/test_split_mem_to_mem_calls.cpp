// The calls that the tests of tests/test_split_mem_to_mem.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("split_mem_to_mem: test_split_mem_to_mem.py::TestSplit::test_data_to_data (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass split_mem_to_mem\n"
      "source tests/test_split_mem_to_mem.py::TestSplit::test_data_to_data\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"src\" 0) Data(\"dst\" 0) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"src\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"dst\" 0) false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("split_mem_to_mem: test_split_mem_to_mem.py::TestSplit::test_indexed_data_to_data (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass split_mem_to_mem\n"
      "source tests/test_split_mem_to_mem.py::TestSplit::test_indexed_data_to_data\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(IndexedData(\"floor_ceil\" 0 X) Data(\"dst\" 0) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(IndexedData(\"floor_ceil\" 0 X) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"dst\" 0) false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("split_mem_to_mem: test_split_mem_to_mem.py::TestSplit::test_volatile_mem_to_mem_unchanged (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass split_mem_to_mem\n"
      "source tests/test_split_mem_to_mem.py::TestSplit::test_volatile_mem_to_mem_unchanged\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"src\" 0) Data(\"dst\" 0) true)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"src\" 0) Data(\"dst\" 0) true)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("split_mem_to_mem: test_split_mem_to_mem.py::TestSplit::test_zp_to_data (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass split_mem_to_mem\n"
      "source tests/test_split_mem_to_mem.py::TestSplit::test_zp_to_data\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Data(\"dst\" 0) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"dst\" 0) false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("split_mem_to_mem: test_split_mem_to_mem.py::TestSelfMovDrop::test_drops_data_self_mov (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass split_mem_to_mem\n"
      "source tests/test_split_mem_to_mem.py::TestSelfMovDrop::test_drops_data_self_mov\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"m\" 0) Data(\"m\" 0) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [])\n"
      "])\n");
}

TEST_CASE("split_mem_to_mem: test_split_mem_to_mem.py::TestSelfMovDrop::test_drops_zp_self_mov (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass split_mem_to_mem\n"
      "source tests/test_split_mem_to_mem.py::TestSelfMovDrop::test_drops_zp_self_mov\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(133 0) ZP(133 0) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [])\n"
      "])\n");
}

TEST_CASE("split_mem_to_mem: test_split_mem_to_mem.py::TestNonTargets::test_imm_to_mem_unchanged (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass split_mem_to_mem\n"
      "source tests/test_split_mem_to_mem.py::TestNonTargets::test_imm_to_mem_unchanged\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(5) Data(\"dst\" 0) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(5) Data(\"dst\" 0) false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("split_mem_to_mem: test_split_mem_to_mem.py::TestNonTargets::test_mem_to_reg_unchanged (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass split_mem_to_mem\n"
      "source tests/test_split_mem_to_mem.py::TestNonTargets::test_mem_to_reg_unchanged\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"src\" 0) Reg(A) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"src\" 0) Reg(A) false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("split_mem_to_mem: test_split_mem_to_mem.py::TestNonTargets::test_reg_to_mem_unchanged (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass split_mem_to_mem\n"
      "source tests/test_split_mem_to_mem.py::TestNonTargets::test_reg_to_mem_unchanged\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(A) Data(\"dst\" 0) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(A) Data(\"dst\" 0) false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("split_mem_to_mem: test_split_mem_to_mem.py::TestNonTargets::test_reg_to_reg_unchanged (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass split_mem_to_mem\n"
      "source tests/test_split_mem_to_mem.py::TestNonTargets::test_reg_to_reg_unchanged\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("split_mem_to_mem: test_split_mem_to_mem.py::TestSequenceFromAscFloor::test_three_consecutive_same_source (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass split_mem_to_mem\n"
      "source tests/test_split_mem_to_mem.py::TestSequenceFromAscFloor::test_three_consecutive_same_source\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"src\" 0) Data(\"d1\" 0) false)\n"
      "    Mov(Data(\"src\" 0) Data(\"d2\" 0) false)\n"
      "    Mov(Data(\"src\" 0) Data(\"d3\" 0) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"src\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"d1\" 0) false)\n"
      "    Mov(Data(\"src\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"d2\" 0) false)\n"
      "    Mov(Data(\"src\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"d3\" 0) false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("split_mem_to_mem: test_split_mem_to_mem.py::TestIndexRegisterMove::test_transfer_with_a_left_alone (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass split_mem_to_mem\n"
      "source tests/test_split_mem_to_mem.py::TestIndexRegisterMove::test_transfer_with_a_left_alone\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("split_mem_to_mem: test_split_mem_to_mem.py::TestIndexRegisterMove::test_x_to_y (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass split_mem_to_mem\n"
      "source tests/test_split_mem_to_mem.py::TestIndexRegisterMove::test_x_to_y\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(X) Reg(Y) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(Y) false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("split_mem_to_mem: test_split_mem_to_mem.py::TestIndexRegisterMove::test_y_to_x (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass split_mem_to_mem\n"
      "source tests/test_split_mem_to_mem.py::TestIndexRegisterMove::test_y_to_x\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(Y) Reg(X) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(Y) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "  ])\n"
      "])\n");
}
