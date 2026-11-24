// The calls that the tests of tests/test_round_trip_load.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("round_trip_load_drop: test_round_trip_load.py::TestExplicitLdaDrop::test_drops_lda_after_sub_sta_pair (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass round_trip_load_drop\n"
      "source tests/test_round_trip_load.py::TestExplicitLdaDrop::test_drops_lda_after_sub_sta_pair\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    SetCarry\n"
      "    Sub(Imm(4) Reg(A))\n"
      "    Mov(Reg(A) ZP(133 0) false)\n"
      "    Mov(ZP(133 0) Reg(A) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    SetCarry\n"
      "    Sub(Imm(4) Reg(A))\n"
      "    Mov(Reg(A) ZP(133 0) false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("round_trip_load_drop: test_round_trip_load.py::TestMemToMemRewrite::test_rewrites_mem_to_mem_after_sub_sta (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass round_trip_load_drop\n"
      "source tests/test_round_trip_load.py::TestMemToMemRewrite::test_rewrites_mem_to_mem_after_sub_sta\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    SetCarry\n"
      "    Sub(Imm(4) Reg(A))\n"
      "    Mov(Reg(A) Data(\"__local_col\" 0) false)\n"
      "    Mov(Data(\"__local_col\" 0) Data(\"player_col\" 0) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    SetCarry\n"
      "    Sub(Imm(4) Reg(A))\n"
      "    Mov(Reg(A) Data(\"__local_col\" 0) false)\n"
      "    Mov(Reg(A) Data(\"player_col\" 0) false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("round_trip_load_drop: test_round_trip_load.py::TestMemToMemRewrite::test_rewrites_mem_to_mem_zp_source (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass round_trip_load_drop\n"
      "source tests/test_round_trip_load.py::TestMemToMemRewrite::test_rewrites_mem_to_mem_zp_source\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(7) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(133 0) false)\n"
      "    Mov(ZP(133 0) Data(\"player_col\" 0) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(7) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(133 0) false)\n"
      "    Mov(Reg(A) Data(\"player_col\" 0) false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("round_trip_load_drop: test_round_trip_load.py::TestMemToMemRewrite::test_skips_volatile_mem_to_mem (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass round_trip_load_drop\n"
      "source tests/test_round_trip_load.py::TestMemToMemRewrite::test_skips_volatile_mem_to_mem\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"m\" 0) false)\n"
      "    Mov(Data(\"m\" 0) Data(\"dst\" 0) true)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"m\" 0) false)\n"
      "    Mov(Data(\"m\" 0) Data(\"dst\" 0) true)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("round_trip_load_drop: test_round_trip_load.py::TestMemToMemRewrite::test_skips_when_addresses_differ (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass round_trip_load_drop\n"
      "source tests/test_round_trip_load.py::TestMemToMemRewrite::test_skips_when_addresses_differ\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"m1\" 0) false)\n"
      "    Mov(Data(\"m2\" 0) Data(\"dst\" 0) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"m1\" 0) false)\n"
      "    Mov(Data(\"m2\" 0) Data(\"dst\" 0) false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("round_trip_load_drop: test_round_trip_load.py::TestMemToMemRewrite::test_skips_when_writer_lacks_flag_effect (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass round_trip_load_drop\n"
      "source tests/test_round_trip_load.py::TestMemToMemRewrite::test_skips_when_writer_lacks_flag_effect\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    SetCarry\n"
      "    Mov(Reg(A) Data(\"m\" 0) false)\n"
      "    Mov(Data(\"m\" 0) Data(\"dst\" 0) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    SetCarry\n"
      "    Mov(Reg(A) Data(\"m\" 0) false)\n"
      "    Mov(Data(\"m\" 0) Data(\"dst\" 0) false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("round_trip_load_drop: test_round_trip_load.py::TestFlagsFromA::test_dropped_when_flags_are_dead (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass round_trip_load_drop\n"
      "source tests/test_round_trip_load.py::TestFlagsFromA::test_dropped_when_flags_are_dead\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Call(\"g\" [] [])\n"
      "    Mov(Reg(A) Data(\"t\" 0) false)\n"
      "    Mov(Data(\"t\" 0) Reg(A) false)\n"
      "    ArithmeticShiftLeft(Reg(A))\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Call(\"g\" [] [])\n"
      "    Mov(Reg(A) Data(\"t\" 0) false)\n"
      "    ArithmeticShiftLeft(Reg(A))\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("round_trip_load_drop: test_round_trip_load.py::TestFlagsFromA::test_ora_when_flags_are_read (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass round_trip_load_drop\n"
      "source tests/test_round_trip_load.py::TestFlagsFromA::test_ora_when_flags_are_read\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Call(\"g\" [] [])\n"
      "    Mov(Reg(A) Data(\"t\" 0) false)\n"
      "    Mov(Data(\"t\" 0) Reg(A) false)\n"
      "    Branch(NE \".l\")\n"
      "    Label(\".l\")\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Call(\"g\" [] [])\n"
      "    Mov(Reg(A) Data(\"t\" 0) false)\n"
      "    Or(Imm(0) Reg(A))\n"
      "    Branch(NE \".l\")\n"
      "    Label(\".l\")\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n");
}
