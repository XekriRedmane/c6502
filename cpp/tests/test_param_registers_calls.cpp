// The calls that the tests of tests/test_param_registers.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_param_registers.py::TestParse::test_markers (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_param_registers.py::TestParse::test_markers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(register unsigned char a [[c6502::in_y]],\\n       unsigned char b,\\n       register unsigned int c [[c6502::in_xa]],\\n       register unsigned char d);\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\" \"b\" \"c\" \"d\"] None FunType([UChar UChar UInt UChar] Void) None None [\"Y\" \"\" \"XA\" \"auto\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_param_registers.py::TestParse::test_register_is_optional (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_param_registers.py::TestParse::test_register_is_optional\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(unsigned char a [[c6502::in_y]],\\n       unsigned char *p [[c6502::in_xa]]);\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\" \"p\"] None FunType([UChar Pointer(UChar)] Void) None None [\"Y\" \"XA\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_param_registers.py::TestParse::test_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_param_registers.py::TestParse::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(register unsigned char a [[c6502::in_z]]);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::in_z` doesn't apply to a parameter; a parameter takes `c6502::in_a`, `c6502::in_x`, or `c6502::in_y`, or for a 2-byte parameter a pair such as `c6502::in_ay` (low byte in A, high byte in Y), or `c6502::zeropage(addr)`\"}\n");
}

TEST_CASE("parse: test_param_registers.py::TestParse::test_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_param_registers.py::TestParse::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(register unsigned char a [[c6502::in_y(1)]]);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::in_y` doesn't take an argument\"}\n");
}

TEST_CASE("parse: test_param_registers.py::TestParse::test_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_param_registers.py::TestParse::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(register unsigned int a [[c6502::in_aa]]);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::in_aa` names A twice; each byte of a parameter needs its own register\"}\n");
}

TEST_CASE("parse: test_param_registers.py::TestParse::test_rejected (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_param_registers.py::TestParse::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(register unsigned char a [[c6502::in_x, c6502::in_y]]);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"a parameter takes one attribute: `c6502::in_a`, `c6502::in_x`, `c6502::in_y`, or a pair such as `c6502::in_ay`, naming the register it arrives in, or `c6502::zeropage(addr)`, naming its zero-page address\"}\n");
}
