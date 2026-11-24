// The calls that the tests of tests/test_volatile_void_read_cmp.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("volatile_void_read_cmp: test_volatile_void_read_cmp.py::TestVolatileVoidReadCmp::test_discarded_read_becomes_compare (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass volatile_void_read_cmp\n"
      "source tests/test_volatile_void_read_cmp.py::TestVolatileVoidReadCmp::test_discarded_read_becomes_compare\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Indirect(0) Reg(A) true)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Compare(Reg(A) Indirect(0))\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("volatile_void_read_cmp: test_volatile_void_read_cmp.py::TestVolatileVoidReadCmp::test_flags_read_is_kept (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass volatile_void_read_cmp\n"
      "source tests/test_volatile_void_read_cmp.py::TestVolatileVoidReadCmp::test_flags_read_is_kept\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Label(\".top\")\n"
      "    Mov(Indirect(0) Reg(A) true)\n"
      "    Branch(PL \".top\")\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Label(\".top\")\n"
      "    Mov(Indirect(0) Reg(A) true)\n"
      "    Branch(PL \".top\")\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("volatile_void_read_cmp: test_volatile_void_read_cmp.py::TestVolatileVoidReadCmp::test_read_feeding_and_is_kept (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass volatile_void_read_cmp\n"
      "source tests/test_volatile_void_read_cmp.py::TestVolatileVoidReadCmp::test_read_feeding_and_is_kept\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Label(\".top\")\n"
      "    Mov(Indirect(0) Reg(A) true)\n"
      "    And(Imm(128) Reg(A))\n"
      "    Branch(EQ \".top\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Label(\".top\")\n"
      "    Mov(Indirect(0) Reg(A) true)\n"
      "    And(Imm(128) Reg(A))\n"
      "    Branch(EQ \".top\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("volatile_void_read_cmp: test_volatile_void_read_cmp.py::TestVolatileVoidReadCmp::test_read_feeding_compare_or_add_is_kept (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass volatile_void_read_cmp\n"
      "source tests/test_volatile_void_read_cmp.py::TestVolatileVoidReadCmp::test_read_feeding_compare_or_add_is_kept\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    ClearCarry\n"
      "    Mov(Indirect(0) Reg(A) true)\n"
      "    Compare(Reg(A) Imm(5))\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    ClearCarry\n"
      "    Mov(Indirect(0) Reg(A) true)\n"
      "    Compare(Reg(A) Imm(5))\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("volatile_void_read_cmp: test_volatile_void_read_cmp.py::TestVolatileVoidReadCmp::test_read_feeding_compare_or_add_is_kept (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass volatile_void_read_cmp\n"
      "source tests/test_volatile_void_read_cmp.py::TestVolatileVoidReadCmp::test_read_feeding_compare_or_add_is_kept\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    ClearCarry\n"
      "    Mov(Indirect(0) Reg(A) true)\n"
      "    Add(Imm(1) Reg(A))\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    ClearCarry\n"
      "    Mov(Indirect(0) Reg(A) true)\n"
      "    Add(Imm(1) Reg(A))\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("volatile_void_read_cmp: test_volatile_void_read_cmp.py::TestVolatileVoidReadCmp::test_read_feeding_compare_or_add_is_kept (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass volatile_void_read_cmp\n"
      "source tests/test_volatile_void_read_cmp.py::TestVolatileVoidReadCmp::test_read_feeding_compare_or_add_is_kept\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    ClearCarry\n"
      "    Mov(Indirect(0) Reg(A) true)\n"
      "    Or(Imm(1) Reg(A))\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    ClearCarry\n"
      "    Mov(Indirect(0) Reg(A) true)\n"
      "    Or(Imm(1) Reg(A))\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n");
}
