// The calls that the tests of tests/test_carry_return.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_carry_return.py::TestFrontEnd::test_parses (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_carry_return.py::TestFrontEnd::test_parses\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::return_bool_in_carry]] bool f(void) { return 1; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Bool) None None [] true None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_carry_return.py::TestFrontEnd::test_rejects_argument (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_carry_return.py::TestFrontEnd::test_rejects_argument\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::return_bool_in_carry(1)]] bool f(void) { return 1; }\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::return_bool_in_carry` doesn't take an argument\"}\n");
}

TEST_CASE("parse: test_carry_return.py::TestFrontEnd::test_rejects_object (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_carry_return.py::TestFrontEnd::test_rejects_object\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::return_bool_in_carry]] bool g;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::return_bool_in_carry]]` is only valid on function declarations; saw it on object declaration `g`\"}\n");
}
