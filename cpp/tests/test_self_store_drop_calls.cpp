// The calls that the tests of tests/test_self_store_drop.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("self_store_drop: test_self_store_drop.py::TestSelfStoreDrop::test_different_offsets_dont_drop (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass self_store_drop\n"
      "source tests/test_self_store_drop.py::TestSelfStoreDrop::test_different_offsets_dont_drop\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Data(\"M\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"M\" 1) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Data(\"M\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"M\" 1) false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("self_store_drop: test_self_store_drop.py::TestSelfStoreDrop::test_different_targets_dont_drop (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass self_store_drop\n"
      "source tests/test_self_store_drop.py::TestSelfStoreDrop::test_different_targets_dont_drop\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Data(\"M\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"N\" 0) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Data(\"M\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"N\" 0) false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("self_store_drop: test_self_store_drop.py::TestSelfStoreDrop::test_lda_sta_same_data_drops_sta (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass self_store_drop\n"
      "source tests/test_self_store_drop.py::TestSelfStoreDrop::test_lda_sta_same_data_drops_sta\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Data(\"M\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"M\" 0) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Data(\"M\" 0) Reg(A) false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("self_store_drop: test_self_store_drop.py::TestSelfStoreDrop::test_lda_sta_same_zp_drops_sta (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass self_store_drop\n"
      "source tests/test_self_store_drop.py::TestSelfStoreDrop::test_lda_sta_same_zp_drops_sta\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(ZP(144 0) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(144 0) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(ZP(144 0) Reg(A) false)\n"
      "  ])\n"
      "])\n");
}
