// The calls that the tests of tests/test_volatile_named.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("lower_to_asm2: test_volatile_named.py::TestVolatileSelfMov::test_emitted (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass lower_to_asm2\n"
      "source tests/test_volatile_named.py::TestVolatileSelfMov::test_emitted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"g\" 0) Data(\"g\" 0) true)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"g\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"g\" 0))\n"
      "    Return\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("emit_program: test_volatile_named.py::TestVolatileSelfMov::test_emitted (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass emit_program\n"
      "source tests/test_volatile_named.py::TestVolatileSelfMov::test_emitted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"g\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"g\" 0))\n"
      "    Return\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "\"f:\\n   SUBROUTINE\\n\\n   LDA   g\\n   STA   g\\n   RTS\\n\"\n");
}

TEST_CASE("lower_to_asm2: test_volatile_named.py::TestVolatileSelfMov::test_emitted (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass lower_to_asm2\n"
      "source tests/test_volatile_named.py::TestVolatileSelfMov::test_emitted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"g\" 0) Data(\"g\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"g\" 0) Data(\"g\" 0))\n"
      "    Return\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("emit_program: test_volatile_named.py::TestVolatileSelfMov::test_emitted (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass emit_program\n"
      "source tests/test_volatile_named.py::TestVolatileSelfMov::test_emitted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"g\" 0) Data(\"g\" 0))\n"
      "    Return\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "\"f:\\n   SUBROUTINE\\n\\n   RTS\\n\"\n");
}
