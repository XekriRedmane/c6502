// The calls that the tests of tests/test_tac_to_asm.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("tac_to_asm: test_tac_to_asm.py::TestTranslateProgram::test_full_tree (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_to_asm\n"
      "source tests/test_tac_to_asm.py::TestTranslateProgram::test_full_tree\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstChar(42)))\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(42) Reg(A) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("tac_to_asm: test_tac_to_asm.py::TestStaticVariableTranslation::test_array_init_list_translates_per_element (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_to_asm\n"
      "source tests/test_tac_to_asm.py::TestStaticVariableTranslation::test_array_init_list_translates_per_element\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StaticVariable(\"a\" false Long [IntInit(1) IntInit(2) IntInit(3)])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StaticVariable(\"a\" false [IntInit(1) IntInit(2) IntInit(3)])\n"
      "])\n");
}

TEST_CASE("tac_to_asm: test_tac_to_asm.py::TestStaticVariableTranslation::test_int_init_passes_through (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_to_asm\n"
      "source tests/test_tac_to_asm.py::TestStaticVariableTranslation::test_int_init_passes_through\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StaticVariable(\"g\" true Int [IntInit(42)])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StaticVariable(\"g\" true [IntInit(42)])\n"
      "])\n");
}

TEST_CASE("tac_to_asm: test_tac_to_asm.py::TestStaticVariableTranslation::test_long_init_passes_through_as_long_init (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_to_asm\n"
      "source tests/test_tac_to_asm.py::TestStaticVariableTranslation::test_long_init_passes_through_as_long_init\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StaticVariable(\"g\" false Long [LongInit(200)])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StaticVariable(\"g\" false [LongInit(200)])\n"
      "])\n");
}

TEST_CASE("tac_to_asm: test_tac_to_asm.py::TestStaticVariableTranslation::test_long_long_init_passes_through (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_to_asm\n"
      "source tests/test_tac_to_asm.py::TestStaticVariableTranslation::test_long_long_init_passes_through\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StaticVariable(\"g\" true LongLong [LongLongInit(1234567890)])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StaticVariable(\"g\" true [LongLongInit(1234567890)])\n"
      "])\n");
}

TEST_CASE("tac_to_asm: test_tac_to_asm.py::TestStaticVariableTranslation::test_ulong_long_init_collapses_to_long_long_init (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_to_asm\n"
      "source tests/test_tac_to_asm.py::TestStaticVariableTranslation::test_ulong_long_init_collapses_to_long_long_init\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StaticVariable(\"g\" false ULongLong [ULongLongInit(4000000000)])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StaticVariable(\"g\" false [LongLongInit(4000000000)])\n"
      "])\n");
}

TEST_CASE("tac_to_asm: test_tac_to_asm.py::TestStaticVariableTranslation::test_zero_init_translates_one_to_one (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_to_asm\n"
      "source tests/test_tac_to_asm.py::TestStaticVariableTranslation::test_zero_init_translates_one_to_one\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StaticVariable(\"a\" false Long [ZeroInit(12)])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StaticVariable(\"a\" false [ZeroInit(12)])\n"
      "])\n");
}
