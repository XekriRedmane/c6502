// The calls that the tests of tests/test_preserves.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_preserves.py::TestParse::test_recorded (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_preserves.py::TestParse::test_recorded\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::preserves_y, c6502::address(0x1000), c6502::preserves_x]]\\nvoid f(void);\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false 4096 [\"X\" \"Y\"] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_preserves.py::TestParse::test_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_preserves.py::TestParse::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::preserves_x(1), c6502::address(1)]] void f(void);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::preserves_x` doesn't take an argument\"}\n");
}

TEST_CASE("parse: test_preserves.py::TestParse::test_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_preserves.py::TestParse::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::preserves_x]] void f(void) {}\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"function 'f' is defined here, so it can't have `preserves_x` or `preserves_y`. Those describe a routine at a fixed address (`[[c6502::address(...)]]`), whose code the compiler doesn't compile and so can't check\"}\n");
}

TEST_CASE("parse: test_preserves.py::TestParse::test_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_preserves.py::TestParse::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"extern unsigned char k [[c6502::preserves_x]];\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::preserves_x]]` is only valid on function declarations; saw it on object declaration `k`\"}\n");
}
