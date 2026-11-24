// The calls that the tests of tests/test_replace_pseudoregisters.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.py::TestStaticVariableLowering::test_pseudo_for_static_becomes_data (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass replace_pseudoregisters\n"
      "source tests/test_replace_pseudoregisters.py::TestStaticVariableLowering::test_pseudo_for_static_becomes_data\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StaticVariable(\"g\" true [IntInit(5)])\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Pseudo(\"g\" 0) Reg(A) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StaticVariable(\"g\" true [IntInit(5)])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 0 [])\n"
      "    Mov(Data(\"g\" 0) Reg(A) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("replace_pseudoregisters: test_replace_pseudoregisters.py::TestStaticVariableLowering::test_static_does_not_consume_a_local_slot (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass replace_pseudoregisters\n"
      "source tests/test_replace_pseudoregisters.py::TestStaticVariableLowering::test_static_does_not_consume_a_local_slot\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StaticVariable(\"g\" false [IntInit(0)])\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(1) Pseudo(\"t\" 0) false)\n"
      "    Mov(Pseudo(\"g\" 0) Pseudo(\"t\" 0) false)\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StaticVariable(\"g\" false [IntInit(0)])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionPrologue(0 1 [])\n"
      "    Mov(Imm(1) Frame(1) false)\n"
      "    Mov(Data(\"g\" 0) Frame(1) false)\n"
      "    Ret(0 1 true false [] false)\n"
      "  ])\n"
      "])\n");
}
