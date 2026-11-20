// The calls that the unit tests of tests/test_mem_const_prop.py make of
// `mem_const_prop`, each with the program the test gives the pass, the
// call's arguments, the compile's context, and the program the
// Python pass returned, which the C++ pass must return too. They were
// recorded by wrapping the Python pass while its unit tests ran.
#include "doctest.h"
#include "pass_case.h"

using c6502::testing::check_pass_case;

TEST_CASE("mem_const_prop: TestMemConstProp.test_byte_copied_through_a_leaves_a_unknown") {
  check_pass_case(
      "mem_const_prop",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Imm(5) Reg(A) false)\n"
      "    Mov(Data(\"other\" 0) Data(\"M\" 0) false)\n"
      "    Mov(Reg(A) Data(\"N\" 0) false)\n"
      "    Mov(Data(\"other\" 1) Reg(A) false)\n"
      "    Or(Data(\"N\" 0) Reg(A))\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Imm(5) Reg(A) false)\n"
      "    Mov(Data(\"other\" 0) Data(\"M\" 0) false)\n"
      "    Mov(Reg(A) Data(\"N\" 0) false)\n"
      "    Mov(Data(\"other\" 1) Reg(A) false)\n"
      "    Or(Data(\"N\" 0) Reg(A))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("mem_const_prop: TestMemConstProp.test_constant_stored_through_a_is_what_a_holds") {
  check_pass_case(
      "mem_const_prop",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Imm(5) Reg(A) false)\n"
      "    Mov(Imm(7) Data(\"M\" 0) false)\n"
      "    Mov(Reg(A) Data(\"N\" 0) false)\n"
      "    Mov(Data(\"other\" 0) Reg(A) false)\n"
      "    Or(Data(\"N\" 0) Reg(A))\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Imm(5) Reg(A) false)\n"
      "    Mov(Imm(7) Data(\"M\" 0) false)\n"
      "    Mov(Reg(A) Data(\"N\" 0) false)\n"
      "    Mov(Data(\"other\" 0) Reg(A) false)\n"
      "    Or(Imm(7) Reg(A))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("mem_const_prop: TestMemConstProp.test_drops_known_value_after_block_boundary_label") {
  check_pass_case(
      "mem_const_prop",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"M\" 0) false)\n"
      "    Label(\".reentry\")\n"
      "    Or(Data(\"M\" 0) Reg(A))\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"M\" 0) false)\n"
      "    Label(\".reentry\")\n"
      "    Or(Data(\"M\" 0) Reg(A))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("mem_const_prop: TestMemConstProp.test_drops_known_value_after_call") {
  check_pass_case(
      "mem_const_prop",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"M\" 0) false)\n"
      "    Call(\"helper\" [] [])\n"
      "    Or(Data(\"M\" 0) Reg(A))\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"M\" 0) false)\n"
      "    Call(\"helper\" [] [])\n"
      "    Or(Data(\"M\" 0) Reg(A))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("mem_const_prop: TestMemConstProp.test_indexed_write_invalidates_same_name_data") {
  check_pass_case(
      "mem_const_prop",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"M\" 0) false)\n"
      "    Mov(Reg(A) IndexedData(\"M\" 0 X) false)\n"
      "    Or(Data(\"M\" 0) Reg(A))\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"M\" 0) false)\n"
      "    Mov(Reg(A) IndexedData(\"M\" 0 X) false)\n"
      "    Or(Data(\"M\" 0) Reg(A))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("mem_const_prop: TestMemConstProp.test_move_between_index_registers_leaves_a_unknown") {
  check_pass_case(
      "mem_const_prop",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Imm(5) Reg(A) false)\n"
      "    Mov(Reg(X) Reg(Y) false)\n"
      "    Mov(Reg(A) Data(\"N\" 0) false)\n"
      "    Mov(Data(\"other\" 0) Reg(A) false)\n"
      "    Or(Data(\"N\" 0) Reg(A))\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Imm(5) Reg(A) false)\n"
      "    Mov(Reg(X) Reg(Y) false)\n"
      "    Mov(Reg(A) Data(\"N\" 0) false)\n"
      "    Mov(Data(\"other\" 0) Reg(A) false)\n"
      "    Or(Data(\"N\" 0) Reg(A))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("mem_const_prop: TestMemConstProp.test_overwrite_invalidates_tracked_cell") {
  check_pass_case(
      "mem_const_prop",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"M\" 0) false)\n"
      "    Mov(Data(\"X\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"M\" 0) false)\n"
      "    Or(Data(\"M\" 0) Reg(A))\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"M\" 0) false)\n"
      "    Mov(Data(\"X\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"M\" 0) false)\n"
      "    Or(Data(\"M\" 0) Reg(A))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("mem_const_prop: TestMemConstProp.test_pull_into_a_leaves_a_unknown") {
  check_pass_case(
      "mem_const_prop",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Imm(5) Reg(A) false)\n"
      "    Push(Reg(A))\n"
      "    Mov(Imm(3) Reg(A) false)\n"
      "    Pop(Reg(A))\n"
      "    Mov(Reg(A) Data(\"N\" 0) false)\n"
      "    Mov(Data(\"other\" 0) Reg(A) false)\n"
      "    Or(Data(\"N\" 0) Reg(A))\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Imm(5) Reg(A) false)\n"
      "    Push(Reg(A))\n"
      "    Mov(Imm(3) Reg(A) false)\n"
      "    Pop(Reg(A))\n"
      "    Mov(Reg(A) Data(\"N\" 0) false)\n"
      "    Mov(Data(\"other\" 0) Reg(A) false)\n"
      "    Or(Data(\"N\" 0) Reg(A))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("mem_const_prop: TestMemConstProp.test_substitutes_known_data_value_into_ora_source") {
  check_pass_case(
      "mem_const_prop",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"M\" 0) false)\n"
      "    Mov(Data(\"other\" 0) Reg(A) false)\n"
      "    Or(Data(\"M\" 0) Reg(A))\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"M\" 0) false)\n"
      "    Mov(Data(\"other\" 0) Reg(A) false)\n"
      "    Or(Imm(0) Reg(A))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("mem_const_prop: TestMemConstProp.test_substitutes_known_zp_value") {
  check_pass_case(
      "mem_const_prop",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Imm(170) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(144 0) false)\n"
      "    Mov(Data(\"other\" 0) Reg(A) false)\n"
      "    And(ZP(144 0) Reg(A))\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Imm(170) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(144 0) false)\n"
      "    Mov(Data(\"other\" 0) Reg(A) false)\n"
      "    And(Imm(170) Reg(A))\n"
      "  ])\n"
      "])\n");
}
