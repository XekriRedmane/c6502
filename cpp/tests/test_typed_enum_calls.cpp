// The calls that the tests of tests/test_typed_enum.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_typed_enum.py::TestParse::test_constant_of_each_type (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_constant_of_each_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef unsigned char uint8_t; enum E : int { A = -2 };int main(void) { return A; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(65534) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_constant_of_each_type (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_constant_of_each_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef unsigned char uint8_t; enum E : unsigned int { A = 40000 };int main(void) { return A; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstUInt(40000) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_constant_of_each_type (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_constant_of_each_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef unsigned char uint8_t; enum E : long { A = -1 };int main(void) { return A; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstLong(4294967295) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_constant_of_each_type (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_constant_of_each_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef unsigned char uint8_t; enum E : unsigned long { A = 70000 };int main(void) { return A; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstULong(70000) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_constant_of_each_type (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_constant_of_each_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef unsigned char uint8_t; enum E : long long { A = -1 };int main(void) { return A; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstLongLong(18446744073709551615) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_constant_of_each_type (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_constant_of_each_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef unsigned char uint8_t; enum E : unsigned long long { A = 5 };int main(void) { return A; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstULongLong(5) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_constant_of_each_type (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_constant_of_each_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef unsigned char uint8_t; enum E : unsigned char { A = 200 };int main(void) { return A; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstUChar(200) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_constant_of_each_type (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_constant_of_each_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef unsigned char uint8_t; enum E : uint8_t { A = 200 };int main(void) { return A; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstUChar(200) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_constant_of_each_type (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_constant_of_each_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef unsigned char uint8_t; enum E : bool { A = 1 };int main(void) { return A; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstBool(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_constant_of_each_type (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_constant_of_each_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef unsigned char uint8_t; enum E : char { A = 200 };int main(void) { return A; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Cast(Char Constant(ConstInt(200) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_constant_of_each_type (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_constant_of_each_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef unsigned char uint8_t; enum E : signed char { A = -3 };int main(void) { return A; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Cast(SChar Constant(ConstInt(65533) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_constant_of_each_type (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_constant_of_each_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef unsigned char uint8_t; enum E : short { A = -5 };int main(void) { return A; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Cast(Short Constant(ConstInt(65531) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_constant_of_each_type (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_constant_of_each_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef unsigned char uint8_t; enum E : unsigned short { A = 65535 };int main(void) { return A; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Cast(UShort Constant(ConstUInt(65535) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_constants_have_the_type_inside_the_list (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_constants_have_the_type_inside_the_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : long { A, B = sizeof A, C = sizeof(enum E) * 2 };int main(void) { return C; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstLong(8) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_declaration_again (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_declaration_again\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : int; enum E : int; enum E x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_declaration_again (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_declaration_again\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : int { A }; enum E : int; enum E x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_declaration_again (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_declaration_again\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : const int; enum E : int { A }; enum E x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_declaration_again (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_declaration_again\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : signed; enum E : int; enum E x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_definition_alone_declares_nothing_in_the_ast (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_definition_alone_declares_nothing_in_the_ast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : int; enum E : int { A, B, };\"\n"
      "output\n"
      "Program([])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef unsigned char uint8_t; enum E : uint8_t { A = 255, B };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enumeration constant 'B' would be 256, one more than the constant before it, which doesn't fit in the enum's underlying type `unsigned char` (0..255, C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : unsigned char { A = -1 };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enumeration constant 'A' is -1, which doesn't fit in the enum's underlying type `unsigned char` (0..255, C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : signed char { A = 128 };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enumeration constant 'A' is 128, which doesn't fit in the enum's underlying type `signed char` (-128..127, C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : char { A = -1 };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enumeration constant 'A' is -1, which doesn't fit in the enum's underlying type `char` (0..255, C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : short { A = 32767, B };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enumeration constant 'B' would be 32768, one more than the constant before it, which doesn't fit in the enum's underlying type `short` (-32768..32767, C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : unsigned { A = 65536 };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enumeration constant 'A' is 65536, which doesn't fit in the enum's underlying type `unsigned int` (0..65535, C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : long { A = 2147483648 };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enumeration constant 'A' is 2147483648, which doesn't fit in the enum's underlying type `long` (-2147483648..2147483647, C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : unsigned long long { A = 18446744073709551615ULL, B };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enumeration constant 'B' would be 18446744073709551616, one more than the constant before it, which doesn't fit in the enum's underlying type `unsigned long long` (0..18446744073709551615, C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : bool { F, T, X };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enumeration constant 'X' would be 2, one more than the constant before it, which doesn't fit in the enum's underlying type `bool` (0..1, C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : int { A = 1.5 };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"the value of enumeration constant 'A' must be an integer constant expression: floating constant ConstDouble(bits=4609434218613702656) is not an integer constant\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : float { A };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"the underlying type of an enum must be an integer type, and `float` isn't one (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : void { A };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"the underlying type of an enum must be an integer type, and `void` isn't one (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int *P; enum E : P { A };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"the underlying type of an enum must be an integer type, and `int *` isn't one (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : _BitInt(8) { A };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"the underlying type of an enum can't be a bit-precise integer type such as `_BitInt(8)` (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : unsigned _BitInt(8) { A };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"the underlying type of an enum can't be a bit-precise integer type such as `unsigned _BitInt(8)` (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef _BitInt(12) B; enum E : B { A };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"the underlying type of an enum can't be a bit-precise integer type such as `_BitInt(12)` (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum F { Q }; enum E : enum F { A };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"the underlying type of an enum can't be an enumerated type such as `enum F`. Give its integer type instead (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef enum F { Q } FT; enum E : FT { A };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"the underlying type of an enum can't be an enumerated type, and 'FT' is one. Give its integer type instead (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 19)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct S { int x; }; enum E : struct S { A };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"the underlying type of an enum must be an integer type, and `struct S` isn't one (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 20)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"union U { int x; }; enum E : union U;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"the underlying type of an enum must be an integer type, and `union U` isn't one (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 21)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : int int { A };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"multiple type specifiers in a declaration (at most one 'int' is permitted)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 22)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : long; enum E : int;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enum 'E' was declared with the underlying type `long`, and here with `int`, which isn't compatible with it (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 23)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : char; enum E : unsigned char { A };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enum 'E' was declared with the underlying type `char`, and here with `unsigned char`, which isn't compatible with it (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 24)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : int { A }; enum E : long;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enum 'E' was declared with the underlying type `int`, and here with `long`, which isn't compatible with it (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 25)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E { A }; enum E : int;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enum 'E' was defined without a fixed underlying type, so it can't be declared again with one (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 26)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : int; enum E { A };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enum 'E' was declared with the fixed underlying type `int`, so its definition must give that type too, as in `enum E : int { ... }` (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 27)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : int { A }; enum E : int { B };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enum 'E' is defined twice\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 28)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : int { A }; enum E { B };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enum 'E' is defined twice\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 29)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : int; enum E : int; enum E { B };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enum 'E' was declared with the fixed underlying type `int`, so its definition must give that type too, as in `enum E : int { ... }` (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 30)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E { A }; enum E : int { B };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enum 'E' is defined twice\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 31)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct S; enum S : int;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'S' is a struct or union tag in this scope, so it can't be an enum tag too\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 32)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum S : int; struct S *p;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'S' is an enum tag, not a struct tag\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 33)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum S : int; struct S { int x; };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'S' is an enum tag in this scope, so it can't be a struct tag too\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 35)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : int { A, A };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'A' is declared again in the same scope, and one of the declarations is an enumeration constant\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 36)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int A; enum E : int { A };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'A' is declared again in the same scope, and one of the declarations is an enumeration constant\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 37)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum [[c6502::packed]] E : int { A };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::packed]]` was removed. C23's fixed underlying type gives an enum its size: `enum e : uint8_t { ... }` is one byte (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 38)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum [[c6502::packed]] E : int;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::packed]]` was removed. C23's fixed underlying type gives an enum its size: `enum e : uint8_t { ... }` is one byte (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_errors (call 39)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum [[c6502::zeropage]] E : int;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::zeropage` doesn't apply to an enum\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_forward_declaration (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_forward_declaration\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : long; enum E x; int n = sizeof(enum E);enum E : long { A = 7 }; enum E y = A;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Long None None None None false))\n"
      "  VarDecl(var_decl(\"n\" SizeOfType(Long None) Int None None None None false))\n"
      "  VarDecl(var_decl(\"y\" Constant(ConstLong(7) None) Long None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_largest_and_least_values (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_largest_and_least_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : unsigned char { A = 255 };\"\n"
      "output\n"
      "Program([])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_largest_and_least_values (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_largest_and_least_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : signed char { A = -128, B = 127 };\"\n"
      "output\n"
      "Program([])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_largest_and_least_values (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_largest_and_least_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : char { A = 255 };\"\n"
      "output\n"
      "Program([])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_largest_and_least_values (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_largest_and_least_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : short { A = -32768 };\"\n"
      "output\n"
      "Program([])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_largest_and_least_values (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_largest_and_least_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : unsigned short { A = 65535 };\"\n"
      "output\n"
      "Program([])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_largest_and_least_values (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_largest_and_least_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : unsigned { A = 65534, B };\"\n"
      "output\n"
      "Program([])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_largest_and_least_values (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_largest_and_least_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : long { A = -2147483647 - 1, B = 2147483647 };\"\n"
      "output\n"
      "Program([])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_largest_and_least_values (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_largest_and_least_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : unsigned long { A = 4294967295 };\"\n"
      "output\n"
      "Program([])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_largest_and_least_values (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_largest_and_least_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : long long { A = -9223372036854775807LL - 1 };\"\n"
      "output\n"
      "Program([])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_largest_and_least_values (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_largest_and_least_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : unsigned long long { A = 18446744073709551615ULL };\"\n"
      "output\n"
      "Program([])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_largest_and_least_values (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_largest_and_least_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : bool { A, B };\"\n"
      "output\n"
      "Program([])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_nested_enum_in_a_value (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_nested_enum_in_a_value\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : char { A = sizeof(enum F : long { X = 100000 }), B = 250 }; int main(void) { return A; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Cast(Char Constant(ConstInt(4) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_nested_enum_in_a_value (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_nested_enum_in_a_value\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : unsigned char { A = sizeof(enum { Y = -1000 }), B = 255 }; int main(void) { return A; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstUChar(2) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_nested_enum_in_a_value (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_nested_enum_in_a_value\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum { P = sizeof(enum G : unsigned char { Z = 255 }), Q = -1000 }; int main(void) { return P; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_nested_enum_in_a_value (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_nested_enum_in_a_value\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : char { A = sizeof(enum F : long { X }), B = 300 };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enumeration constant 'B' is 300, which doesn't fit in the enum's underlying type `char` (0..255, C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_other_syntax_errors_keep_lark_messages (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_other_syntax_errors_keep_lark_messages\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : 3 { A };\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('INTEGER_CONSTANT', '3') at line 1, column 10.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* FLOAT\\n\\t* INT\\n\\t* LONG\\n\\t* RESTRICT\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STRUCT\\n\\t* TYPEDEF_NAME\\n\\t* TYPEOF\\n\\t* TYPEOF_UNQUAL\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_other_syntax_errors_keep_lark_messages (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_other_syntax_errors_keep_lark_messages\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : { A };\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('LBRACE', '{') at line 1, column 10.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* FLOAT\\n\\t* INT\\n\\t* LONG\\n\\t* RESTRICT\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STRUCT\\n\\t* TYPEDEF_NAME\\n\\t* TYPEOF\\n\\t* TYPEOF_UNQUAL\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_scopes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_scopes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n            enum E { A = 1 };\\n            int main(void) {\\n                { enum E : long; enum E x; }\\n                { enum E : unsigned char { A = 2 }; enum E y; A; }\\n                enum E z;\\n                return A;\\n            }\\n        \"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"x\" None Long None None None None false)))\n"
      "    ])))\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"y\" None UChar None None None None false)))\n"
      "      S(Expression(Constant(ConstUChar(2) None)))\n"
      "    ])))\n"
      "    D(VarDecl(var_decl(\"z\" None Int None None None None false)))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_type_is_the_unqualified_underlying_type (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_type_is_the_unqualified_underlying_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : unsigned char { A }; enum E x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None UChar None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_type_is_the_unqualified_underlying_type (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_type_is_the_unqualified_underlying_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef unsigned char uint8_t; enum E : const uint8_t { A }; enum E x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None UChar None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_type_is_the_unqualified_underlying_type (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_type_is_the_unqualified_underlying_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef const volatile long CL; enum E : CL { A }; enum E x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Long None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_type_is_the_unqualified_underlying_type (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_type_is_the_unqualified_underlying_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : unsigned long long int { A }; enum E x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None ULongLong None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_type_is_the_unqualified_underlying_type (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_type_is_the_unqualified_underlying_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : signed short { A }; enum E x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Short None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_type_is_the_unqualified_underlying_type (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_type_is_the_unqualified_underlying_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : _Bool { A }; enum E x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_type_is_the_unqualified_underlying_type (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_type_is_the_unqualified_underlying_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int v; enum E : typeof(v) { A }; enum E x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_type_is_the_unqualified_underlying_type (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_type_is_the_unqualified_underlying_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef enum : unsigned { A } T; T x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None UInt None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_type_is_the_unqualified_underlying_type (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_type_is_the_unqualified_underlying_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum : unsigned char { A } x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None UChar None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_type_is_the_unqualified_underlying_type (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_type_is_the_unqualified_underlying_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"const enum E : char { A } x = A;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Cast(Char Constant(ConstInt(0) None) None) Const(Char) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_typed_enum_without_its_list (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_typed_enum_without_its_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : int x;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`enum e : T` without a list of constants can only be a whole declaration, `enum e : T;`, with no storage class, `typedef`, or declarator, and not in a struct, a `for`, a cast, or `sizeof` (C23 \\u00a76.7.3.3). Declare the tag that way, and then name the type as `enum e`. The parser stopped at 'x' on line 1\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_typed_enum_without_its_list (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_typed_enum_without_its_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : int *p;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`enum e : T` without a list of constants can only be a whole declaration, `enum e : T;`, with no storage class, `typedef`, or declarator, and not in a struct, a `for`, a cast, or `sizeof` (C23 \\u00a76.7.3.3). Declare the tag that way, and then name the type as `enum e`. The parser stopped at '*' on line 1\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_typed_enum_without_its_list (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_typed_enum_without_its_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : int, x;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`enum e : T` without a list of constants can only be a whole declaration, `enum e : T;`, with no storage class, `typedef`, or declarator, and not in a struct, a `for`, a cast, or `sizeof` (C23 \\u00a76.7.3.3). Declare the tag that way, and then name the type as `enum e`. The parser stopped at ',' on line 1\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_typed_enum_without_its_list (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_typed_enum_without_its_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : int; int f(void) { return (enum E : int)3; }\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`enum e : T` without a list of constants can only be a whole declaration, `enum e : T;`, with no storage class, `typedef`, or declarator, and not in a struct, a `for`, a cast, or `sizeof` (C23 \\u00a76.7.3.3). Declare the tag that way, and then name the type as `enum e`. The parser stopped at ')' on line 1\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_typed_enum_without_its_list (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_typed_enum_without_its_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : int; int n = sizeof(enum E : int);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`enum e : T` without a list of constants can only be a whole declaration, `enum e : T;`, with no storage class, `typedef`, or declarator, and not in a struct, a `for`, a cast, or `sizeof` (C23 \\u00a76.7.3.3). Declare the tag that way, and then name the type as `enum e`. The parser stopped at ')' on line 1\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_typed_enum_without_its_list (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_typed_enum_without_its_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct S { enum E : int; };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`enum e : T` without a list of constants can only be a whole declaration, `enum e : T;`, with no storage class, `typedef`, or declarator, and not in a struct, a `for`, a cast, or `sizeof` (C23 \\u00a76.7.3.3). Declare the tag that way, and then name the type as `enum e`. The parser stopped at ';' on line 1\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_typed_enum_without_its_list (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_typed_enum_without_its_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(void) { for (enum E : int; ;) {} }\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`enum e : T` without a list of constants can only be a whole declaration, `enum e : T;`, with no storage class, `typedef`, or declarator, and not in a struct, a `for`, a cast, or `sizeof` (C23 \\u00a76.7.3.3). Declare the tag that way, and then name the type as `enum e`. The parser stopped at ';' on line 1\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_typed_enum_without_its_list (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_typed_enum_without_its_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"static enum E : int;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`enum e : T` without a list of constants can only be a whole declaration, `enum e : T;`, with no storage class, `typedef`, or declarator, and not in a struct, a `for`, a cast, or `sizeof` (C23 \\u00a76.7.3.3). Declare the tag that way, and then name the type as `enum e`. The parser stopped at ';' on line 1\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_typed_enum_without_its_list (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_typed_enum_without_its_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef enum E : int;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`enum e : T` without a list of constants can only be a whole declaration, `enum e : T;`, with no storage class, `typedef`, or declarator, and not in a struct, a `for`, a cast, or `sizeof` (C23 \\u00a76.7.3.3). Declare the tag that way, and then name the type as `enum e`. The parser stopped at ';' on line 1\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_typed_enum_without_its_list (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_typed_enum_without_its_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : int f(void) { return 0; }\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`enum e : T` without a list of constants can only be a whole declaration, `enum e : T;`, with no storage class, `typedef`, or declarator, and not in a struct, a `for`, a cast, or `sizeof` (C23 \\u00a76.7.3.3). Declare the tag that way, and then name the type as `enum e`. The parser stopped at 'f' on line 1\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_typedef_of_a_typedef (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_typedef_of_a_typedef\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef enum : int { A } T; typedef const T U;enum F : U { B };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"the underlying type of an enum can't be an enumerated type, and 'U' is one. Give its integer type instead (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_typedef_of_a_typedef (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_typedef_of_a_typedef\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef enum : int { A } T; typedef T *P; P p;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_values (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef unsigned char uint8_t; enum E : uint8_t { A, B, C = 250, D, E1 = C - 200 };int main(void) { return A; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstUChar(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_values (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef unsigned char uint8_t; enum E : uint8_t { A, B, C = 250, D, E1 = C - 200 };int main(void) { return B; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstUChar(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_values (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef unsigned char uint8_t; enum E : uint8_t { A, B, C = 250, D, E1 = C - 200 };int main(void) { return C; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstUChar(250) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_values (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef unsigned char uint8_t; enum E : uint8_t { A, B, C = 250, D, E1 = C - 200 };int main(void) { return D; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstUChar(251) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestParse::test_values (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestParse::test_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef unsigned char uint8_t; enum E : uint8_t { A, B, C = 250, D, E1 = C - 200 };int main(void) { return E1; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstUChar(50) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_compatible_with_the_underlying_type (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_compatible_with_the_underlying_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nenum E : uint8_t { A }; uint8_t u; enum E *p = &u;uint8_t *q; enum E *r = 0;void f(void) { q = r; p = q; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" None UChar None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"u\" None) None) Pointer(UChar) None None None None false))\n"
      "  VarDecl(var_decl(\"q\" None Pointer(UChar) None None None None false))\n"
      "  VarDecl(var_decl(\"r\" Constant(ConstInt(0) None) Pointer(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"q\" None) Var(\"r\" None) None)))\n"
      "    S(Expression(Assignment(Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_typed_enum.py::TestTypes::test_compatible_with_the_underlying_type (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_compatible_with_the_underlying_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" None UChar None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"u\" None) None) Pointer(UChar) None None None None false))\n"
      "  VarDecl(var_decl(\"q\" None Pointer(UChar) None None None None false))\n"
      "  VarDecl(var_decl(\"r\" Constant(ConstInt(0) None) Pointer(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"q\" None) Var(\"r\" None) None)))\n"
      "    S(Expression(Assignment(Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" None UChar None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"u\" None) None) Pointer(UChar) None None None None false))\n"
      "  VarDecl(var_decl(\"q\" None Pointer(UChar) None None None None false))\n"
      "  VarDecl(var_decl(\"r\" Constant(ConstInt(0) None) Pointer(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"q\" None) Var(\"r\" None) None)))\n"
      "    S(Expression(Assignment(Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_typed_enum.py::TestTypes::test_compatible_with_the_underlying_type (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_typed_enum.py::TestTypes::test_compatible_with_the_underlying_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" None UChar None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"u\" None) None) Pointer(UChar) None None None None false))\n"
      "  VarDecl(var_decl(\"q\" None Pointer(UChar) None None None None false))\n"
      "  VarDecl(var_decl(\"r\" Constant(ConstInt(0) None) Pointer(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"q\" None) Var(\"r\" None) None)))\n"
      "    S(Expression(Assignment(Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" None UChar None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"u\" None) None) Pointer(UChar) None None None None false))\n"
      "  VarDecl(var_decl(\"q\" None Pointer(UChar) None None None None false))\n"
      "  VarDecl(var_decl(\"r\" Constant(ConstInt(0) None) Pointer(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"q\" None) Var(\"r\" None) None)))\n"
      "    S(Expression(Assignment(Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_typed_enum.py::TestTypes::test_compatible_with_the_underlying_type (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_compatible_with_the_underlying_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" None UChar None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"u\" None) None) Pointer(UChar) None None None None false))\n"
      "  VarDecl(var_decl(\"q\" None Pointer(UChar) None None None None false))\n"
      "  VarDecl(var_decl(\"r\" Constant(ConstInt(0) None) Pointer(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"q\" None) Var(\"r\" None) None)))\n"
      "    S(Expression(Assignment(Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" None UChar None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"u\" None) None) Pointer(UChar) None None None None false))\n"
      "  VarDecl(var_decl(\"q\" None Pointer(UChar) None None None None false))\n"
      "  VarDecl(var_decl(\"r\" Constant(ConstInt(0) None) Pointer(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"q\" None) Var(\"r\" None) None)))\n"
      "    S(Expression(Assignment(Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_typed_enum.py::TestTypes::test_compatible_with_the_underlying_type (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_typed_enum.py::TestTypes::test_compatible_with_the_underlying_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" None UChar None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"u\" None) None) Pointer(UChar) None None None None false))\n"
      "  VarDecl(var_decl(\"q\" None Pointer(UChar) None None None None false))\n"
      "  VarDecl(var_decl(\"r\" Constant(ConstInt(0) None) Pointer(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"q\" None) Var(\"r\" None) None)))\n"
      "    S(Expression(Assignment(Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" None UChar None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"u\" None) None) Pointer(UChar) None None None None false))\n"
      "  VarDecl(var_decl(\"q\" None Pointer(UChar) None None None None false))\n"
      "  VarDecl(var_decl(\"r\" Constant(ConstInt(0) None) Pointer(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"q\" None) Var(\"r\" None) None)))\n"
      "    S(Expression(Assignment(Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_typed_enum.py::TestTypes::test_compatible_with_the_underlying_type (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_typed_enum.py::TestTypes::test_compatible_with_the_underlying_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"u\\\" None UChar None None None None false))\\n  VarDecl(var_decl(\\\"p\\\" AddressOf(Var(\\\"u\\\" UChar) Pointer(UChar)) Pointer(UChar) None None None None false))\\n  VarDecl(var_decl(\\\"q\\\" None Pointer(UChar) None None None None false))\\n  VarDecl(var_decl(\\\"r\\\" Cast(Pointer(UChar) Constant(ConstInt(0) Int) Pointer(UChar)) Pointer(UChar) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Expression(Assignment(Var(\\\"q\\\" Pointer(UChar)) Var(\\\"r\\\" Pointer(UChar)) Pointer(UChar))))\\n    S(Expression(Assignment(Var(\\\"p\\\" Pointer(UChar)) Var(\\\"q\\\" Pointer(UChar)) Pointer(UChar))))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" None UChar None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"u\" None) None) Pointer(UChar) None None None None false))\n"
      "  VarDecl(var_decl(\"q\" None Pointer(UChar) None None None None false))\n"
      "  VarDecl(var_decl(\"r\" Constant(ConstInt(0) None) Pointer(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"q\" None) Var(\"r\" None) None)))\n"
      "    S(Expression(Assignment(Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"u\\\" None UChar None None None None false))\\n  VarDecl(var_decl(\\\"p\\\" AddressOf(Var(\\\"u\\\" UChar) Pointer(UChar)) Pointer(UChar) None None None None false))\\n  VarDecl(var_decl(\\\"q\\\" None Pointer(UChar) None None None None false))\\n  VarDecl(var_decl(\\\"r\\\" Cast(Pointer(UChar) Constant(ConstInt(0) Int) Pointer(UChar)) Pointer(UChar) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Expression(Assignment(Var(\\\"q\\\" Pointer(UChar)) Var(\\\"r\\\" Pointer(UChar)) Pointer(UChar))))\\n    S(Expression(Assignment(Var(\\\"p\\\" Pointer(UChar)) Var(\\\"q\\\" Pointer(UChar)) Pointer(UChar))))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"u\", \"offset\": 0}}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_constexpr_and_case_labels (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_constexpr_and_case_labels\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nenum E : uint8_t { A = 200, B };constexpr uint8_t k = B; int a[A - 190];int f(uint8_t x) { switch (x) { case A: return 1; case k: return 2; } return 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"k\" Constant(ConstUChar(201) None) Const(UChar) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    S(SwitchStmt(Var(\"x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstUChar(200) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "      S(CaseStmt(Var(\"k\" None) Return(Constant(ConstInt(2) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_typed_enum.py::TestTypes::test_constexpr_and_case_labels (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_constexpr_and_case_labels\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"k\" Constant(ConstUChar(201) None) Const(UChar) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    S(SwitchStmt(Var(\"x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstUChar(200) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "      S(CaseStmt(Var(\"k\" None) Return(Constant(ConstInt(2) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"k\" Constant(ConstUChar(201) None) Const(UChar) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstUChar(200) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "      S(CaseStmt(Var(\"k\" None) Return(Constant(ConstInt(2) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_typed_enum.py::TestTypes::test_constexpr_and_case_labels (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_typed_enum.py::TestTypes::test_constexpr_and_case_labels\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"k\" Constant(ConstUChar(201) None) Const(UChar) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstUChar(200) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "      S(CaseStmt(Var(\"k\" None) Return(Constant(ConstInt(2) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"k\" Constant(ConstUChar(201) None) Const(UChar) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstUChar(200) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "      S(CaseStmt(Var(\"k\" None) Return(Constant(ConstInt(2) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_typed_enum.py::TestTypes::test_constexpr_and_case_labels (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_constexpr_and_case_labels\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"k\" Constant(ConstUChar(201) None) Const(UChar) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstUChar(200) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "      S(CaseStmt(Var(\"k\" None) Return(Constant(ConstInt(2) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"k\" Constant(ConstUChar(201) None) Const(UChar) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstUChar(200) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "      S(CaseStmt(Var(\"k\" None) Return(Constant(ConstInt(2) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_typed_enum.py::TestTypes::test_constexpr_and_case_labels (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_typed_enum.py::TestTypes::test_constexpr_and_case_labels\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"k\" Constant(ConstUChar(201) None) Const(UChar) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstUChar(200) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "      S(CaseStmt(Var(\"k\" None) Return(Constant(ConstInt(2) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"k\" Constant(ConstUChar(201) None) Const(UChar) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstUChar(200) None) Return(Constant(ConstInt(1) None)) \".case@1\"))\n"
      "      S(CaseStmt(Var(\"k\" None) Return(Constant(ConstInt(2) None)) \".case@2\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstUChar(200) None) \".case@1\") switch_case(Var(\"k\" None) \".case@2\")] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_typed_enum.py::TestTypes::test_constexpr_and_case_labels (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_typed_enum.py::TestTypes::test_constexpr_and_case_labels\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"k\\\" Constant(ConstUChar(201) UChar) Const(UChar) Static None None None true))\\n  VarDecl(var_decl(\\\"a\\\" None Array(Int 10) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.x\\\"] Block([\\n    S(SwitchStmt(Cast(Int Var(\\\"@0.x\\\" UChar) Int) Compound(Block([\\n      S(CaseStmt(Constant(ConstUChar(200) UChar) Return(Constant(ConstInt(1) Int)) \\\".case@1\\\"))\\n      S(CaseStmt(Var(\\\"k\\\" Const(UChar)) Return(Constant(ConstInt(2) Int)) \\\".case@2\\\"))\\n    ])) \\\".switch@0\\\" [switch_case(Constant(ConstInt(200) Int) \\\".case@1\\\") switch_case(Constant(ConstInt(201) Int) \\\".case@2\\\")] None Int))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([UChar] Int) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"k\" Constant(ConstUChar(201) None) Const(UChar) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstUChar(200) None) Return(Constant(ConstInt(1) None)) \".case@1\"))\n"
      "      S(CaseStmt(Var(\"k\" None) Return(Constant(ConstInt(2) None)) \".case@2\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstUChar(200) None) \".case@1\") switch_case(Var(\"k\" None) \".case@2\")] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"k\\\" Constant(ConstUChar(201) UChar) Const(UChar) Static None None None true))\\n  VarDecl(var_decl(\\\"a\\\" None Array(Int 10) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.x\\\"] Block([\\n    S(SwitchStmt(Cast(Int Var(\\\"@0.x\\\" UChar) Int) Compound(Block([\\n      S(CaseStmt(Constant(ConstUChar(200) UChar) Return(Constant(ConstInt(1) Int)) \\\".case@1\\\"))\\n      S(CaseStmt(Var(\\\"k\\\" Const(UChar)) Return(Constant(ConstInt(2) Int)) \\\".case@2\\\"))\\n    ])) \\\".switch@0\\\" [switch_case(Constant(ConstInt(200) Int) \\\".case@1\\\") switch_case(Constant(ConstInt(201) Int) \\\".case@2\\\")] None Int))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([UChar] Int) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"k\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 201}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_constexpr_and_case_labels (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_constexpr_and_case_labels\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nenum E : uint8_t { A = 200, B };int f(uint8_t x) { switch (x) { case A: case 200: return 1; } return 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    S(SwitchStmt(Var(\"x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstUChar(200) None) CaseStmt(Constant(ConstInt(200) None) Return(Constant(ConstInt(1) None)) \"\") \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_typed_enum.py::TestTypes::test_constexpr_and_case_labels (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_constexpr_and_case_labels\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    S(SwitchStmt(Var(\"x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstUChar(200) None) CaseStmt(Constant(ConstInt(200) None) Return(Constant(ConstInt(1) None)) \"\") \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstUChar(200) None) CaseStmt(Constant(ConstInt(200) None) Return(Constant(ConstInt(1) None)) \"\") \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_typed_enum.py::TestTypes::test_constexpr_and_case_labels (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_typed_enum.py::TestTypes::test_constexpr_and_case_labels\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstUChar(200) None) CaseStmt(Constant(ConstInt(200) None) Return(Constant(ConstInt(1) None)) \"\") \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstUChar(200) None) CaseStmt(Constant(ConstInt(200) None) Return(Constant(ConstInt(1) None)) \"\") \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_typed_enum.py::TestTypes::test_constexpr_and_case_labels (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_constexpr_and_case_labels\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstUChar(200) None) CaseStmt(Constant(ConstInt(200) None) Return(Constant(ConstInt(1) None)) \"\") \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstUChar(200) None) CaseStmt(Constant(ConstInt(200) None) Return(Constant(ConstInt(1) None)) \"\") \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_typed_enum.py::TestTypes::test_constexpr_and_case_labels (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_typed_enum.py::TestTypes::test_constexpr_and_case_labels\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstUChar(200) None) CaseStmt(Constant(ConstInt(200) None) Return(Constant(ConstInt(1) None)) \"\") \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstUChar(200) None) CaseStmt(Constant(ConstInt(200) None) Return(Constant(ConstInt(1) None)) \".case@2\") \".case@1\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstUChar(200) None) \".case@1\") switch_case(Constant(ConstInt(200) None) \".case@2\")] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_typed_enum.py::TestTypes::test_constexpr_and_case_labels (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_typed_enum.py::TestTypes::test_constexpr_and_case_labels\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstUChar(200) None) CaseStmt(Constant(ConstInt(200) None) Return(Constant(ConstInt(1) None)) \".case@2\") \".case@1\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstUChar(200) None) \".case@1\") switch_case(Constant(ConstInt(200) None) \".case@2\")] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"duplicate case value 200 in switch statement (C99 \\u00a76.8.4.2.3)\"}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nenum U8 : uint8_t { M = 200 };enum Big : unsigned int { BIG = 40000 };enum Wide : long { WIDE = 1000000 };enum S8 : int8_t { NEG = -3 };enum Flag : bool { OFF, ON };static long v = sizeof(enum U8);\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(UChar None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(UChar None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(UChar None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(UChar None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(UChar None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(UChar None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(UChar None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(UChar None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(UChar None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long SizeOfType(UChar UInt) Long) Long Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(UChar None) Long Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long SizeOfType(UChar UInt) Long) Long Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nenum U8 : uint8_t { M = 200 };enum Big : unsigned int { BIG = 40000 };enum Wide : long { WIDE = 1000000 };enum S8 : int8_t { NEG = -3 };enum Flag : bool { OFF, ON };static long v = sizeof M;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstUChar(200) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstUChar(200) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstUChar(200) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstUChar(200) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstUChar(200) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstUChar(200) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstUChar(200) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstUChar(200) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstUChar(200) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long SizeOfExp(Constant(ConstUChar(200) UChar) UInt) Long) Long Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstUChar(200) None) None) Long Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long SizeOfExp(Constant(ConstUChar(200) UChar) UInt) Long) Long Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nenum U8 : uint8_t { M = 200 };enum Big : unsigned int { BIG = 40000 };enum Wide : long { WIDE = 1000000 };enum S8 : int8_t { NEG = -3 };enum Flag : bool { OFF, ON };static long v = M + M;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long Binary(Add Cast(Int Constant(ConstUChar(200) UChar) Int) Cast(Int Constant(ConstUChar(200) UChar) Int) Int) Long) Long Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) Long Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long Binary(Add Cast(Int Constant(ConstUChar(200) UChar) Int) Cast(Int Constant(ConstUChar(200) UChar) Int) Int) Long) Long Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 400}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 19)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nenum U8 : uint8_t { M = 200 };enum Big : unsigned int { BIG = 40000 };enum Wide : long { WIDE = 1000000 };enum S8 : int8_t { NEG = -3 };enum Flag : bool { OFF, ON };static long v = sizeof(M + M);\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 20)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 21)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 22)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 23)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 24)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long SizeOfExp(Binary(Add Cast(Int Constant(ConstUChar(200) UChar) Int) Cast(Int Constant(ConstUChar(200) UChar) Int) Int) UInt) Long) Long Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long SizeOfExp(Binary(Add Cast(Int Constant(ConstUChar(200) UChar) Int) Cast(Int Constant(ConstUChar(200) UChar) Int) Int) UInt) Long) Long Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 2}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 25)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nenum U8 : uint8_t { M = 200 };enum Big : unsigned int { BIG = 40000 };enum Wide : long { WIDE = 1000000 };enum S8 : int8_t { NEG = -3 };enum Flag : bool { OFF, ON };static long v = sizeof(enum Big);\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(UInt None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 26)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(UInt None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(UInt None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 27)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(UInt None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(UInt None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 28)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(UInt None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(UInt None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 29)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(UInt None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(UInt None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 30)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long SizeOfType(UInt UInt) Long) Long Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(UInt None) Long Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long SizeOfType(UInt UInt) Long) Long Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 2}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 31)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nenum U8 : uint8_t { M = 200 };enum Big : unsigned int { BIG = 40000 };enum Wide : long { WIDE = 1000000 };enum S8 : int8_t { NEG = -3 };enum Flag : bool { OFF, ON };static long v = BIG > 32767;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(GreaterThan Constant(ConstUInt(40000) None) Constant(ConstInt(32767) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 32)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(GreaterThan Constant(ConstUInt(40000) None) Constant(ConstInt(32767) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(GreaterThan Constant(ConstUInt(40000) None) Constant(ConstInt(32767) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 33)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(GreaterThan Constant(ConstUInt(40000) None) Constant(ConstInt(32767) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(GreaterThan Constant(ConstUInt(40000) None) Constant(ConstInt(32767) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 34)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(GreaterThan Constant(ConstUInt(40000) None) Constant(ConstInt(32767) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(GreaterThan Constant(ConstUInt(40000) None) Constant(ConstInt(32767) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 35)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(GreaterThan Constant(ConstUInt(40000) None) Constant(ConstInt(32767) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(GreaterThan Constant(ConstUInt(40000) None) Constant(ConstInt(32767) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 36)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long Binary(GreaterThan Constant(ConstUInt(40000) UInt) Cast(UInt Constant(ConstInt(32767) Int) UInt) Int) Long) Long Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(GreaterThan Constant(ConstUInt(40000) None) Constant(ConstInt(32767) None) None) Long Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long Binary(GreaterThan Constant(ConstUInt(40000) UInt) Cast(UInt Constant(ConstInt(32767) Int) UInt) Int) Long) Long Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 37)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nenum U8 : uint8_t { M = 200 };enum Big : unsigned int { BIG = 40000 };enum Wide : long { WIDE = 1000000 };enum S8 : int8_t { NEG = -3 };enum Flag : bool { OFF, ON };static long v = BIG / 2 == 20000u;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Equal Binary(Divide Constant(ConstUInt(40000) None) Constant(ConstInt(2) None) None) Constant(ConstUInt(20000) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 38)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Equal Binary(Divide Constant(ConstUInt(40000) None) Constant(ConstInt(2) None) None) Constant(ConstUInt(20000) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Equal Binary(Divide Constant(ConstUInt(40000) None) Constant(ConstInt(2) None) None) Constant(ConstUInt(20000) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 39)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Equal Binary(Divide Constant(ConstUInt(40000) None) Constant(ConstInt(2) None) None) Constant(ConstUInt(20000) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Equal Binary(Divide Constant(ConstUInt(40000) None) Constant(ConstInt(2) None) None) Constant(ConstUInt(20000) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 40)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Equal Binary(Divide Constant(ConstUInt(40000) None) Constant(ConstInt(2) None) None) Constant(ConstUInt(20000) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Equal Binary(Divide Constant(ConstUInt(40000) None) Constant(ConstInt(2) None) None) Constant(ConstUInt(20000) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 41)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Equal Binary(Divide Constant(ConstUInt(40000) None) Constant(ConstInt(2) None) None) Constant(ConstUInt(20000) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Equal Binary(Divide Constant(ConstUInt(40000) None) Constant(ConstInt(2) None) None) Constant(ConstUInt(20000) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 42)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long Binary(Equal Binary(Divide Constant(ConstUInt(40000) UInt) Cast(UInt Constant(ConstInt(2) Int) UInt) UInt) Constant(ConstUInt(20000) UInt) Int) Long) Long Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Equal Binary(Divide Constant(ConstUInt(40000) None) Constant(ConstInt(2) None) None) Constant(ConstUInt(20000) None) None) Long Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long Binary(Equal Binary(Divide Constant(ConstUInt(40000) UInt) Cast(UInt Constant(ConstInt(2) Int) UInt) UInt) Constant(ConstUInt(20000) UInt) Int) Long) Long Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 43)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nenum U8 : uint8_t { M = 200 };enum Big : unsigned int { BIG = 40000 };enum Wide : long { WIDE = 1000000 };enum S8 : int8_t { NEG = -3 };enum Flag : bool { OFF, ON };static long v = sizeof(BIG + 1);\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstUInt(40000) None) Constant(ConstInt(1) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 44)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstUInt(40000) None) Constant(ConstInt(1) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstUInt(40000) None) Constant(ConstInt(1) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 45)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstUInt(40000) None) Constant(ConstInt(1) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstUInt(40000) None) Constant(ConstInt(1) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 46)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstUInt(40000) None) Constant(ConstInt(1) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstUInt(40000) None) Constant(ConstInt(1) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 47)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstUInt(40000) None) Constant(ConstInt(1) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstUInt(40000) None) Constant(ConstInt(1) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 48)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long SizeOfExp(Binary(Add Constant(ConstUInt(40000) UInt) Cast(UInt Constant(ConstInt(1) Int) UInt) UInt) UInt) Long) Long Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstUInt(40000) None) Constant(ConstInt(1) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long SizeOfExp(Binary(Add Constant(ConstUInt(40000) UInt) Cast(UInt Constant(ConstInt(1) Int) UInt) UInt) UInt) Long) Long Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 2}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 49)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nenum U8 : uint8_t { M = 200 };enum Big : unsigned int { BIG = 40000 };enum Wide : long { WIDE = 1000000 };enum S8 : int8_t { NEG = -3 };enum Flag : bool { OFF, ON };static long v = (BIG + 0) > 0;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(GreaterThan Binary(Add Constant(ConstUInt(40000) None) Constant(ConstInt(0) None) None) Constant(ConstInt(0) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 50)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(GreaterThan Binary(Add Constant(ConstUInt(40000) None) Constant(ConstInt(0) None) None) Constant(ConstInt(0) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(GreaterThan Binary(Add Constant(ConstUInt(40000) None) Constant(ConstInt(0) None) None) Constant(ConstInt(0) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 51)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(GreaterThan Binary(Add Constant(ConstUInt(40000) None) Constant(ConstInt(0) None) None) Constant(ConstInt(0) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(GreaterThan Binary(Add Constant(ConstUInt(40000) None) Constant(ConstInt(0) None) None) Constant(ConstInt(0) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 52)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(GreaterThan Binary(Add Constant(ConstUInt(40000) None) Constant(ConstInt(0) None) None) Constant(ConstInt(0) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(GreaterThan Binary(Add Constant(ConstUInt(40000) None) Constant(ConstInt(0) None) None) Constant(ConstInt(0) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 53)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(GreaterThan Binary(Add Constant(ConstUInt(40000) None) Constant(ConstInt(0) None) None) Constant(ConstInt(0) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(GreaterThan Binary(Add Constant(ConstUInt(40000) None) Constant(ConstInt(0) None) None) Constant(ConstInt(0) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 54)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long Binary(GreaterThan Binary(Add Constant(ConstUInt(40000) UInt) Cast(UInt Constant(ConstInt(0) Int) UInt) UInt) Cast(UInt Constant(ConstInt(0) Int) UInt) Int) Long) Long Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(GreaterThan Binary(Add Constant(ConstUInt(40000) None) Constant(ConstInt(0) None) None) Constant(ConstInt(0) None) None) Long Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long Binary(GreaterThan Binary(Add Constant(ConstUInt(40000) UInt) Cast(UInt Constant(ConstInt(0) Int) UInt) UInt) Cast(UInt Constant(ConstInt(0) Int) UInt) Int) Long) Long Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 55)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nenum U8 : uint8_t { M = 200 };enum Big : unsigned int { BIG = 40000 };enum Wide : long { WIDE = 1000000 };enum S8 : int8_t { NEG = -3 };enum Flag : bool { OFF, ON };static long v = sizeof(enum Wide);\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(Long None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 56)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(Long None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(Long None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 57)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(Long None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(Long None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 58)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(Long None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(Long None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 59)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(Long None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(Long None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 60)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long SizeOfType(Long UInt) Long) Long Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfType(Long None) Long Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long SizeOfType(Long UInt) Long) Long Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 4}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 61)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nenum U8 : uint8_t { M = 200 };enum Big : unsigned int { BIG = 40000 };enum Wide : long { WIDE = 1000000 };enum S8 : int8_t { NEG = -3 };enum Flag : bool { OFF, ON };static long v = sizeof WIDE;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstLong(1000000) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 62)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstLong(1000000) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstLong(1000000) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 63)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstLong(1000000) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstLong(1000000) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 64)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstLong(1000000) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstLong(1000000) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 65)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstLong(1000000) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstLong(1000000) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 66)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long SizeOfExp(Constant(ConstLong(1000000) Long) UInt) Long) Long Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstLong(1000000) None) None) Long Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long SizeOfExp(Constant(ConstLong(1000000) Long) UInt) Long) Long Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 4}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 67)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nenum U8 : uint8_t { M = 200 };enum Big : unsigned int { BIG = 40000 };enum Wide : long { WIDE = 1000000 };enum S8 : int8_t { NEG = -3 };enum Flag : bool { OFF, ON };static long v = WIDE / 1000;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Divide Constant(ConstLong(1000000) None) Constant(ConstInt(1000) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 68)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Divide Constant(ConstLong(1000000) None) Constant(ConstInt(1000) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Divide Constant(ConstLong(1000000) None) Constant(ConstInt(1000) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 69)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Divide Constant(ConstLong(1000000) None) Constant(ConstInt(1000) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Divide Constant(ConstLong(1000000) None) Constant(ConstInt(1000) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 70)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Divide Constant(ConstLong(1000000) None) Constant(ConstInt(1000) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Divide Constant(ConstLong(1000000) None) Constant(ConstInt(1000) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 71)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Divide Constant(ConstLong(1000000) None) Constant(ConstInt(1000) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Divide Constant(ConstLong(1000000) None) Constant(ConstInt(1000) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 72)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Binary(Divide Constant(ConstLong(1000000) Long) Cast(Long Constant(ConstInt(1000) Int) Long) Long) Long Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Divide Constant(ConstLong(1000000) None) Constant(ConstInt(1000) None) None) Long Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Binary(Divide Constant(ConstLong(1000000) Long) Cast(Long Constant(ConstInt(1000) Int) Long) Long) Long Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1000}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 73)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nenum U8 : uint8_t { M = 200 };enum Big : unsigned int { BIG = 40000 };enum Wide : long { WIDE = 1000000 };enum S8 : int8_t { NEG = -3 };enum Flag : bool { OFF, ON };static long v = sizeof(WIDE + 1);\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstLong(1000000) None) Constant(ConstInt(1) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 74)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstLong(1000000) None) Constant(ConstInt(1) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstLong(1000000) None) Constant(ConstInt(1) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 75)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstLong(1000000) None) Constant(ConstInt(1) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstLong(1000000) None) Constant(ConstInt(1) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 76)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstLong(1000000) None) Constant(ConstInt(1) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstLong(1000000) None) Constant(ConstInt(1) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 77)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstLong(1000000) None) Constant(ConstInt(1) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstLong(1000000) None) Constant(ConstInt(1) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 78)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long SizeOfExp(Binary(Add Constant(ConstLong(1000000) Long) Cast(Long Constant(ConstInt(1) Int) Long) Long) UInt) Long) Long Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Binary(Add Constant(ConstLong(1000000) None) Constant(ConstInt(1) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long SizeOfExp(Binary(Add Constant(ConstLong(1000000) Long) Cast(Long Constant(ConstInt(1) Int) Long) Long) UInt) Long) Long Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 4}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 79)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nenum U8 : uint8_t { M = 200 };enum Big : unsigned int { BIG = 40000 };enum Wide : long { WIDE = 1000000 };enum S8 : int8_t { NEG = -3 };enum Flag : bool { OFF, ON };static long v = NEG < 0;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(LessThan Cast(SChar Constant(ConstInt(65533) None) None) Constant(ConstInt(0) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 80)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(LessThan Cast(SChar Constant(ConstInt(65533) None) None) Constant(ConstInt(0) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(LessThan Cast(SChar Constant(ConstInt(65533) None) None) Constant(ConstInt(0) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 81)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(LessThan Cast(SChar Constant(ConstInt(65533) None) None) Constant(ConstInt(0) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(LessThan Cast(SChar Constant(ConstInt(65533) None) None) Constant(ConstInt(0) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 82)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(LessThan Cast(SChar Constant(ConstInt(65533) None) None) Constant(ConstInt(0) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(LessThan Cast(SChar Constant(ConstInt(65533) None) None) Constant(ConstInt(0) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 83)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(LessThan Cast(SChar Constant(ConstInt(65533) None) None) Constant(ConstInt(0) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(LessThan Cast(SChar Constant(ConstInt(65533) None) None) Constant(ConstInt(0) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 84)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long Binary(LessThan Cast(Int Cast(SChar Constant(ConstInt(65533) Int) SChar) Int) Constant(ConstInt(0) Int) Int) Long) Long Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(LessThan Cast(SChar Constant(ConstInt(65533) None) None) Constant(ConstInt(0) None) None) Long Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long Binary(LessThan Cast(Int Cast(SChar Constant(ConstInt(65533) Int) SChar) Int) Constant(ConstInt(0) Int) Int) Long) Long Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 85)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nenum U8 : uint8_t { M = 200 };enum Big : unsigned int { BIG = 40000 };enum Wide : long { WIDE = 1000000 };enum S8 : int8_t { NEG = -3 };enum Flag : bool { OFF, ON };static long v = NEG + 1;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Cast(SChar Constant(ConstInt(65533) None) None) Constant(ConstInt(1) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 86)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Cast(SChar Constant(ConstInt(65533) None) None) Constant(ConstInt(1) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Cast(SChar Constant(ConstInt(65533) None) None) Constant(ConstInt(1) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 87)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Cast(SChar Constant(ConstInt(65533) None) None) Constant(ConstInt(1) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Cast(SChar Constant(ConstInt(65533) None) None) Constant(ConstInt(1) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 88)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Cast(SChar Constant(ConstInt(65533) None) None) Constant(ConstInt(1) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Cast(SChar Constant(ConstInt(65533) None) None) Constant(ConstInt(1) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 89)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Cast(SChar Constant(ConstInt(65533) None) None) Constant(ConstInt(1) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Cast(SChar Constant(ConstInt(65533) None) None) Constant(ConstInt(1) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 90)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long Binary(Add Cast(Int Cast(SChar Constant(ConstInt(65533) Int) SChar) Int) Constant(ConstInt(1) Int) Int) Long) Long Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Cast(SChar Constant(ConstInt(65533) None) None) Constant(ConstInt(1) None) None) Long Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long Binary(Add Cast(Int Cast(SChar Constant(ConstInt(65533) Int) SChar) Int) Constant(ConstInt(1) Int) Int) Long) Long Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": -2}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 91)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nenum U8 : uint8_t { M = 200 };enum Big : unsigned int { BIG = 40000 };enum Wide : long { WIDE = 1000000 };enum S8 : int8_t { NEG = -3 };enum Flag : bool { OFF, ON };static long v = sizeof NEG;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Cast(SChar Constant(ConstInt(65533) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 92)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Cast(SChar Constant(ConstInt(65533) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Cast(SChar Constant(ConstInt(65533) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 93)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Cast(SChar Constant(ConstInt(65533) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Cast(SChar Constant(ConstInt(65533) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 94)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Cast(SChar Constant(ConstInt(65533) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Cast(SChar Constant(ConstInt(65533) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 95)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Cast(SChar Constant(ConstInt(65533) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Cast(SChar Constant(ConstInt(65533) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 96)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long SizeOfExp(Cast(SChar Constant(ConstInt(65533) Int) SChar) UInt) Long) Long Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Cast(SChar Constant(ConstInt(65533) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long SizeOfExp(Cast(SChar Constant(ConstInt(65533) Int) SChar) UInt) Long) Long Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 97)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nenum U8 : uint8_t { M = 200 };enum Big : unsigned int { BIG = 40000 };enum Wide : long { WIDE = 1000000 };enum S8 : int8_t { NEG = -3 };enum Flag : bool { OFF, ON };static long v = ON + ON;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Constant(ConstBool(1) None) Constant(ConstBool(1) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 98)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Constant(ConstBool(1) None) Constant(ConstBool(1) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Constant(ConstBool(1) None) Constant(ConstBool(1) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 99)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Constant(ConstBool(1) None) Constant(ConstBool(1) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Constant(ConstBool(1) None) Constant(ConstBool(1) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 100)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Constant(ConstBool(1) None) Constant(ConstBool(1) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Constant(ConstBool(1) None) Constant(ConstBool(1) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 101)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Constant(ConstBool(1) None) Constant(ConstBool(1) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Constant(ConstBool(1) None) Constant(ConstBool(1) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 102)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long Binary(Add Cast(Int Constant(ConstBool(1) Bool) Int) Cast(Int Constant(ConstBool(1) Bool) Int) Int) Long) Long Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Binary(Add Constant(ConstBool(1) None) Constant(ConstBool(1) None) None) Long Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long Binary(Add Cast(Int Constant(ConstBool(1) Bool) Int) Cast(Int Constant(ConstBool(1) Bool) Int) Int) Long) Long Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 2}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 103)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nenum U8 : uint8_t { M = 200 };enum Big : unsigned int { BIG = 40000 };enum Wide : long { WIDE = 1000000 };enum S8 : int8_t { NEG = -3 };enum Flag : bool { OFF, ON };static long v = sizeof ON;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstBool(1) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 104)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstBool(1) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstBool(1) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 105)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstBool(1) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstBool(1) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 106)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstBool(1) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstBool(1) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 107)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstBool(1) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstBool(1) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 108)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long SizeOfExp(Constant(ConstBool(1) Bool) UInt) Long) Long Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" SizeOfExp(Constant(ConstBool(1) None) None) Long Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long SizeOfExp(Constant(ConstBool(1) Bool) UInt) Long) Long Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 109)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nenum U8 : uint8_t { M = 200 };enum Big : unsigned int { BIG = 40000 };enum Wide : long { WIDE = 1000000 };enum S8 : int8_t { NEG = -3 };enum Flag : bool { OFF, ON };static long v = (unsigned char)(M + M);\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Cast(UChar Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 110)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Cast(UChar Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Cast(UChar Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 111)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Cast(UChar Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Cast(UChar Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 112)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Cast(UChar Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Cast(UChar Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 113)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Cast(UChar Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Cast(UChar Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) None) Long Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_typed_enum.py::TestTypes::test_sizeof_and_promotions (call 114)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_typed_enum.py::TestTypes::test_sizeof_and_promotions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long Cast(UChar Binary(Add Cast(Int Constant(ConstUChar(200) UChar) Int) Cast(Int Constant(ConstUChar(200) UChar) Int) Int) UChar) Long) Long Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Cast(UChar Binary(Add Constant(ConstUChar(200) None) Constant(ConstUChar(200) None) None) None) Long Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Long Cast(UChar Binary(Add Cast(Int Constant(ConstUChar(200) UChar) Int) Cast(Int Constant(ConstUChar(200) UChar) Int) Int) UChar) Long) Long Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 144}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_typed_enum.py::TestTypes::test_typeof_and_auto (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typed_enum.py::TestTypes::test_typeof_and_auto\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E : unsigned long { A = 5 };typeof(A) x; void f(void) { auto y = A + 1; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None ULong None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"y\" Binary(Add Constant(ConstULong(5) None) Constant(ConstInt(1) None) None) ULong None None None None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}
