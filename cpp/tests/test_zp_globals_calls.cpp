// The calls that the tests of tests/test_zp_globals.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_zp_globals.py::TestParse::test_prefix_and_postfix_forms (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_zp_globals.py::TestParse::test_prefix_and_postfix_forms\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zeropage]] unsigned char a;\\nunsigned char b [[c6502::zeropage(0xBA)]];\\nunsigned char c;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None ZeroPage(None) None false))\n"
      "  VarDecl(var_decl(\"b\" None UChar None None ZeroPage(186) None false))\n"
      "  VarDecl(var_decl(\"c\" None UChar None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_zp_globals.py::TestParse::test_prefix_applies_to_every_declarator (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_zp_globals.py::TestParse::test_prefix_applies_to_every_declarator\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zeropage]] unsigned char a, b;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None ZeroPage(None) None false))\n"
      "  VarDecl(var_decl(\"b\" None UChar None None ZeroPage(None) None false))\n"
      "])\n");
}

TEST_CASE("parse: test_zp_globals.py::TestParse::test_rejected_forms (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_zp_globals.py::TestParse::test_rejected_forms\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zeropage]] int f(void) { return 0; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::zeropage]]` places an object, not a function; saw it on function definition 'f'\"}\n");
}

TEST_CASE("parse: test_zp_globals.py::TestParse::test_rejected_forms (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_zp_globals.py::TestParse::test_rejected_forms\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zp_abi]] unsigned char a;\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::zp_abi]]` is only valid on function declarations; saw it on object declaration `a`\"}\n");
}

TEST_CASE("parse: test_zp_globals.py::TestParse::test_rejected_forms (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_zp_globals.py::TestParse::test_rejected_forms\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"unsigned char a [[c6502::zeropage(\\\"x\\\")]];\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::zeropage(...)]]` takes an integer address; got a string on `a`\"}\n");
}

TEST_CASE("parse: test_zp_globals.py::TestParse::test_rejected_forms (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_zp_globals.py::TestParse::test_rejected_forms\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"unsigned char a [[c6502::zeropage(0x100)]];\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"zero-page address $100 for `a` is outside $00-$FF\"}\n");
}

TEST_CASE("parse: test_zp_globals.py::TestParse::test_rejected_forms (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_zp_globals.py::TestParse::test_rejected_forms\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zeropage]] unsigned char a [[c6502::zeropage]];\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::zeropage` is given twice on object declaration `a`\"}\n");
}

TEST_CASE("parse: test_zp_globals.py::TestParse::test_rejected_forms (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_zp_globals.py::TestParse::test_rejected_forms\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"unsigned char a [[c6502::nonsense]];\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"unknown attribute `c6502::nonsense`; c6502's attributes are address, keep, preserves_x, preserves_y, return_bool_in_carry, save_hardware_stack, section, unroll, zeropage, zp_abi, and the parameter registers `in_a`, `in_x`, `in_y`, `in_ay`, and so on\"}\n");
}
