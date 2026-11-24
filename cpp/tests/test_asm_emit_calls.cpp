// The calls that the tests of tests/test_asm_emit.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("emit_program: test_asm_emit.py::TestEmitProgram::test_static_long_init_range_check (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass emit_program\n"
      "source tests/test_asm_emit.py::TestEmitProgram::test_static_long_init_range_check\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StaticVariable(\"bad\" false [LongInit(4294967296)])\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"ValueError\", \"message\": \"init for 'bad' 4294967296 out of range for 32-bit (-2147483648..4294967295)\"}\n");
}

TEST_CASE("emit_program: test_asm_emit.py::TestEmitProgram::test_static_string_init_bytes_lt_len_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass emit_program\n"
      "source tests/test_asm_emit.py::TestEmitProgram::test_static_string_init_bytes_lt_len_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StaticVariable(\"s\" false [StringInit(\"abcdef\" 3)])\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"ValueError\", \"message\": \"StringInit for 's': bytes=3 less than string length 6\"}\n");
}

TEST_CASE("emit_program: test_asm_emit.py::TestEmitProgram::test_static_variable_init_byte_range_check (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass emit_program\n"
      "source tests/test_asm_emit.py::TestEmitProgram::test_static_variable_init_byte_range_check\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StaticVariable(\"bad\" false [CharInit(256)])\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"ValueError\", \"message\": \"init for 'bad' 256 out of range for 1-byte (-128..255)\"}\n");
}

TEST_CASE("emit_program: test_asm_emit.py::TestEmitProgram::test_static_zero_init_byte_count_must_be_positive (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass emit_program\n"
      "source tests/test_asm_emit.py::TestEmitProgram::test_static_zero_init_byte_count_must_be_positive\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StaticVariable(\"a\" false [ZeroInit(0)])\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"ValueError\", \"message\": \"ZeroInit byte count must be positive, got 0\"}\n");
}
