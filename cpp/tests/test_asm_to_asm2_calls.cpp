// The calls that the tests of tests/test_asm_to_asm2.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("lower_to_asm2: test_asm_to_asm2.py::TestFunctionPrologueExpansion::test_a_frame_access_past_the_ldy_reach_is_a_frame_size_error (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass lower_to_asm2\n"
      "source tests/test_asm_to_asm2.py::TestFunctionPrologueExpansion::test_a_frame_access_past_the_ldy_reach_is_a_frame_size_error\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    FunctionPrologue(4 250 [])\n"
      "    Mov(Frame(256) Reg(A) false)\n"
      "  ])\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"FrameSizeError\", \"message\": \"function 'f': it reads or writes the byte at frame offset 256, but the `LDY #offset` of each frame access reaches at most 255 (frame offset 256 out of range 1..255); a large array fits better as a `static` or a global\"}\n");
}
