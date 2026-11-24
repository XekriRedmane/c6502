// The calls that the tests of tests/test_register_keyword.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_register_keyword.py::TestRegisterKeywordParsing::test_no_register_params (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_register_keyword.py::TestRegisterKeywordParsing::test_no_register_params\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"char f(char a, char b);\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\" \"b\"] None FunType([Char Char] Char) None None [\"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_register_keyword.py::TestRegisterKeywordParsing::test_old_reg_attribute_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_register_keyword.py::TestRegisterKeywordParsing::test_old_reg_attribute_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"char f(char x [[c6502::reg(\\\"X\\\")]]);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"unknown attribute `c6502::reg`; c6502's attributes are address, keep, preserves_x, preserves_y, return_bool_in_carry, save_hardware_stack, section, unroll, zeropage, zp_abi, and the parameter registers `in_a`, `in_x`, `in_y`, `in_ay`, and so on\"}\n");
}

TEST_CASE("parse: test_register_keyword.py::TestRegisterKeywordParsing::test_register_local_storage_class (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_register_keyword.py::TestRegisterKeywordParsing::test_register_local_storage_class\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { register char i; i = 0; return i; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"i\" None Char Register None None None false)))\n"
      "    S(Expression(Assignment(Var(\"i\" None) Constant(ConstInt(0) None) None)))\n"
      "    S(Return(Var(\"i\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_register_keyword.py::TestRegisterKeywordParsing::test_register_on_function_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_register_keyword.py::TestRegisterKeywordParsing::test_register_on_function_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"register int foo(int x);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"storage class 'register' isn't permitted on function declaration 'foo'\"}\n");
}

TEST_CASE("parse: test_register_keyword.py::TestRegisterKeywordParsing::test_register_param_definition (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_register_keyword.py::TestRegisterKeywordParsing::test_register_param_definition\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"char f(register char a, register char b) { return (char)(a + b); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\" \"b\"] Block([\n"
      "    S(Return(Cast(Char Binary(Add Var(\"a\" None) Var(\"b\" None) None) None)))\n"
      "  ]) FunType([Char Char] Char) None None [\"auto\" \"auto\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_register_keyword.py::TestRegisterKeywordParsing::test_register_param_marker (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_register_keyword.py::TestRegisterKeywordParsing::test_register_param_marker\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"char f(register char a, char b, register char c);\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\" \"b\" \"c\"] None FunType([Char Char Char] Char) None None [\"auto\" \"\" \"auto\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_register_keyword.py::TestRegisterKeywordParsing::test_static_on_parameter_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_register_keyword.py::TestRegisterKeywordParsing::test_static_on_parameter_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"char f(static char x);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"storage class 'static' isn't permitted on a function parameter (C99 \\u00a76.7.5.3.2 \\u2014 only register)\"}\n");
}

TEST_CASE("parse: test_register_keyword.py::TestRegisterKeywordParsing::test_unknown_attribute_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_register_keyword.py::TestRegisterKeywordParsing::test_unknown_attribute_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::funky]] char f(void);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"unknown attribute `c6502::funky`; c6502's attributes are address, keep, preserves_x, preserves_y, return_bool_in_carry, save_hardware_stack, section, unroll, zeropage, zp_abi, and the parameter registers `in_a`, `in_x`, `in_y`, `in_ay`, and so on\"}\n");
}

TEST_CASE("parse: test_register_keyword.py::TestRegisterKeywordParsing::test_zp_abi_still_parses (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_register_keyword.py::TestRegisterKeywordParsing::test_zp_abi_still_parses\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zp_abi]] char f(register char a);\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\"] None FunType([Char] Char) None \"zp_abi\" [\"auto\"] false None [] None false))\n"
      "])\n");
}
