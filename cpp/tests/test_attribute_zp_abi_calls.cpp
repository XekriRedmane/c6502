// The calls that the tests of tests/test_attribute_zp_abi.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_attribute_zp_abi.py::TestAttributeZpAbiParsing::test_attribute_on_object_decl_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_attribute_zp_abi.py::TestAttributeZpAbiParsing::test_attribute_on_object_decl_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zp_abi]] int x;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::zp_abi]]` is only valid on function declarations; saw it on object declaration `x`\"}\n");
}

TEST_CASE("parse: test_attribute_zp_abi.py::TestAttributeZpAbiParsing::test_attribute_on_tag_only_decl_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_attribute_zp_abi.py::TestAttributeZpAbiParsing::test_attribute_on_tag_only_decl_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zp_abi]] struct foo;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"attributes before a declaration need a declarator to apply to; a struct's, union's, or enum's go after its keyword, as in `struct [[deprecated]] s { ... };`\"}\n");
}

TEST_CASE("parse: test_attribute_zp_abi.py::TestAttributeZpAbiParsing::test_forward_decl_with_annotation (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_attribute_zp_abi.py::TestAttributeZpAbiParsing::test_forward_decl_with_annotation\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zp_abi]] int f(int x);\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] None FunType([Int] Int) None \"zp_abi\" [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_attribute_zp_abi.py::TestAttributeZpAbiParsing::test_function_decl_without_annotation_has_none (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_attribute_zp_abi.py::TestAttributeZpAbiParsing::test_function_decl_without_annotation_has_none\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int h(int x) { return x; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"h\" [\"x\"] Block([\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_attribute_zp_abi.py::TestAttributeZpAbiParsing::test_function_definition_with_annotation (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_attribute_zp_abi.py::TestAttributeZpAbiParsing::test_function_definition_with_annotation\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zp_abi]] int g(int x) { return x + 1; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"g\" [\"x\"] Block([\n"
      "    S(Return(Binary(Add Var(\"x\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([Int] Int) None \"zp_abi\" [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_attribute_zp_abi.py::TestAttributeZpAbiParsing::test_unknown_attribute_name_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_attribute_zp_abi.py::TestAttributeZpAbiParsing::test_unknown_attribute_name_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::wrong]] int f(int x);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"unknown attribute `c6502::wrong`; c6502's attributes are address, keep, preserves_x, preserves_y, return_bool_in_carry, save_hardware_stack, section, unroll, zeropage, zp_abi, and the parameter registers `in_a`, `in_x`, `in_y`, `in_ay`, and so on\"}\n");
}
