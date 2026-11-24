// The calls that the tests of tests/test_slot_sharing.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_slot_sharing.py::TestPinnedParams::test_marker (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_slot_sharing.py::TestPinnedParams::test_marker\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(const char *s [[c6502::zeropage(0xE6)]], register unsigned char n [[c6502::in_x]]);\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"s\" \"n\"] None FunType([Pointer(Const(Char)) UChar] Void) None None [\"@230\" \"X\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_slot_sharing.py::TestPinnedParams::test_parse_errors (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_slot_sharing.py::TestPinnedParams::test_parse_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(register char *s [[c6502::zeropage(0xE6)]]);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"a `register` parameter arrives in a register, so it can't also have `[[c6502::zeropage(...)]]`\"}\n");
}

TEST_CASE("parse: test_slot_sharing.py::TestPinnedParams::test_parse_errors (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_slot_sharing.py::TestPinnedParams::test_parse_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(char *s [[c6502::zeropage]]);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::zeropage(...)]]` on a parameter takes the integer zero-page address it arrives at\"}\n");
}

TEST_CASE("parse: test_slot_sharing.py::TestPinnedParams::test_parse_errors (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_slot_sharing.py::TestPinnedParams::test_parse_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(char *s [[c6502::zeropage(0x100)]]);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"zero-page address $100 for a parameter is outside $00-$FF\"}\n");
}
