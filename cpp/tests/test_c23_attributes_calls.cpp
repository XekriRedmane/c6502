// The calls that the tests of tests/test_c23_attributes.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_c23_attributes.py::TestPlacement::test_after_a_name_applies_to_that_declarator_only (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestPlacement::test_after_a_name_applies_to_that_declarator_only\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"unsigned char n [[c6502::zeropage]], m;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" None UChar None None ZeroPage(None) None false))\n"
      "  VarDecl(var_decl(\"m\" None UChar None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestPlacement::test_after_an_array_name (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestPlacement::test_after_an_array_name\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"extern unsigned char z [[c6502::zeropage(0x8A)]][3];\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"z\" None Array(UChar 3) Extern None ZeroPage(138) None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestPlacement::test_array_subscripts_still_parse (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestPlacement::test_array_subscripts_still_parse\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int i) { int a[3]; int b[2][2]; a[0] = b[1][i]; return a[b[0][0]]; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"i\"] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 3) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" None Array(Array(Int 2) 2) None None None None false)))\n"
      "    S(Expression(Assignment(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None) Subscript(Subscript(Var(\"b\" None) Constant(ConstInt(1) None) None) Var(\"i\" None) None) None)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Subscript(Subscript(Var(\"b\" None) Constant(ConstInt(0) None) None) Constant(ConstInt(0) None) None) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestPlacement::test_before_and_after_the_name_mean_the_same (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestPlacement::test_before_and_after_the_name_mean_the_same\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zp_abi]] void f(void);\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None \"zp_abi\" [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestPlacement::test_before_and_after_the_name_mean_the_same (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestPlacement::test_before_and_after_the_name_mean_the_same\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f [[c6502::zp_abi]] (void);\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None \"zp_abi\" [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestPlacement::test_before_and_after_the_name_mean_the_same (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestPlacement::test_before_and_after_the_name_mean_the_same\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zp_abi]] void f(void) {}\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([]) FunType([] Void) None \"zp_abi\" [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestPlacement::test_before_and_after_the_name_mean_the_same (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestPlacement::test_before_and_after_the_name_mean_the_same\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f [[c6502::zp_abi]] (void) {}\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([]) FunType([] Void) None \"zp_abi\" [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestPlacement::test_double_underscore_spelling (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestPlacement::test_double_underscore_spelling\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[__c6502__::__zp_abi__]] void f(void);\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None \"zp_abi\" [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestPlacement::test_inside_a_parenthesized_declarator (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestPlacement::test_inside_a_parenthesized_declarator\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void (*fp [[c6502::zeropage]])(void);\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None ZeroPage(None) None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestPlacement::test_leading_applies_to_every_declarator (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestPlacement::test_leading_applies_to_every_declarator\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zeropage]] unsigned char n, m;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" None UChar None None ZeroPage(None) None false))\n"
      "  VarDecl(var_decl(\"m\" None UChar None None ZeroPage(None) None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestPlacement::test_packed_was_removed (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestPlacement::test_packed_was_removed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum [[c6502::packed]] e { A }; enum e v;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::packed]]` was removed. C23's fixed underlying type gives an enum its size: `enum e : uint8_t { ... }` is one byte (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestPlacement::test_parameters (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestPlacement::test_parameters\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void g(unsigned char a [[c6502::in_x]], [[c6502::in_y]] unsigned char b, [[c6502::in_a]] unsigned char);\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"g\" [\"a\" \"b\" \"\"] None FunType([UChar UChar UChar] Void) None None [\"X\" \"Y\" \"A\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestPlacement::test_several_lists_and_several_attributes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestPlacement::test_several_lists_and_several_attributes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zp_abi]] [[c6502::preserves_x, c6502::address(0xFC58)]] void HOME(void);\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"HOME\" [] None FunType([] Void) None \"zp_abi\" [] false 64600 [\"X\"] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestPlacement::test_space_between_the_brackets (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestPlacement::test_space_between_the_brackets\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[ [c6502::zp_abi] ] void f(void);\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None \"zp_abi\" [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestStandardAndForeign::test_a_struct_body_after_attributes_isnt_a_block (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestStandardAndForeign::test_a_struct_body_after_attributes_isnt_a_block\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct [[deprecated]] s { enum { A, B } k; }; int f(void) { return B; }\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"k\" Int)]))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestStandardAndForeign::test_accepted_and_ignored (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestStandardAndForeign::test_accepted_and_ignored\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum e { A [[deprecated]], B = 3 };\"\n"
      "output\n"
      "Program([])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestStandardAndForeign::test_accepted_and_ignored (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestStandardAndForeign::test_accepted_and_ignored\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct [[deprecated(\\\"old\\\")]] s { [[maybe_unused]] int a; int b [[deprecated]]; };\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Int) member_decl(\"b\" Int)]))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestStandardAndForeign::test_accepted_and_ignored (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestStandardAndForeign::test_accepted_and_ignored\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[deprecated]] typedef int T; T x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestStandardAndForeign::test_accepted_and_ignored (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestStandardAndForeign::test_accepted_and_ignored\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[noreturn]] void die(void);\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None true))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestStandardAndForeign::test_accepted_and_ignored (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestStandardAndForeign::test_accepted_and_ignored\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[_Noreturn]] void die(void);\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None true))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestStandardAndForeign::test_accepted_and_ignored (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestStandardAndForeign::test_accepted_and_ignored\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[nodiscard(\\\"check it\\\")]] int get(void);\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"get\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestStandardAndForeign::test_accepted_and_ignored (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestStandardAndForeign::test_accepted_and_ignored\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f([[maybe_unused]] int x) {}\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([]) FunType([Int] Void) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestStandardAndForeign::test_accepted_and_ignored (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestStandardAndForeign::test_accepted_and_ignored\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T; void k(void) { [[maybe_unused]] T * p; for ([[maybe_unused]] int i = 0; i < 2; i++) {} }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"k\" [] Block([\n"
      "    D(VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(2) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([])) \"\" None))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestStandardAndForeign::test_accepted_and_ignored (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestStandardAndForeign::test_accepted_and_ignored\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int sw(int x) { switch (x) { case 1: x++; [[fallthrough]]; case 2: return x; } return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"sw\" [\"x\"] Block([\n"
      "    S(SwitchStmt(Var(\"x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(1) None) Expression(Postfix(Increment Var(\"x\" None) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstInt(2) None) Return(Var(\"x\" None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestStandardAndForeign::test_accepted_and_ignored (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestStandardAndForeign::test_accepted_and_ignored\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[]] int z; [[,,]] int w;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"z\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"w\" None Int None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestStandardAndForeign::test_other_prefixes_and_unknown_names_are_ignored (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestStandardAndForeign::test_other_prefixes_and_unknown_names_are_ignored\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[gnu::aligned(sizeof(int)), gnu::format(printf, 1, 2), clang::x([1], (2))]] [[unknown_thing]] int y;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"y\" None Int None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_errors (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[zp_abi]] void f(void);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`zp_abi` is a c6502 attribute, so it needs c6502's prefix: `[[c6502::zp_abi]]`\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_errors (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::bogus]] void f(void);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"unknown attribute `c6502::bogus`; c6502's attributes are address, keep, preserves_x, preserves_y, return_bool_in_carry, save_hardware_stack, section, unroll, zeropage, zp_abi, and the parameter registers `in_a`, `in_x`, `in_y`, `in_ay`, and so on\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_errors (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zp_abi(1)]] void f(void);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::zp_abi` doesn't take an argument\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_errors (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zp_abi()]] void f(void);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::zp_abi()` has empty parentheses; leave them off\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_errors (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::address(1, 2)]] extern int a;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"the argument of `c6502::address` has to be one integer constant or one string literal\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_errors (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zeropage]] void f(void);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::zeropage]]` places an object, not a function; saw it on function declaration 'f'\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_errors (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::in_x]] void f(void);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::in_x` doesn't apply to function declaration 'f'\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_errors (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zeropage]] unsigned char n [[c6502::zeropage]];\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::zeropage` is given twice on object declaration `n`\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_errors (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[noreturn]] int x;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"the standard attribute `noreturn` can't apply to an object; saw it on object declaration `x`\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_errors (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[unsequenced]] int f(void);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[unsequenced]]` applies to a function's type, after its parameter list, where c6502 doesn't accept attributes; saw it on function declaration 'f'\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_errors (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[deprecated, deprecated]] int d;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`deprecated` appears twice in one attribute list\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_errors (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[deprecated(1)]] int d;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`deprecated` takes a string literal, such as `[[deprecated(\\\"reason\\\")]]`, or nothing\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_errors (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[maybe_unused(\\\"x\\\")]] int d;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`maybe_unused` doesn't take an argument\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_errors (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[fallthrough]];\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[fallthrough]];` belongs in a switch statement, not at file scope\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_errors (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zp_abi]];\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::zp_abi` doesn't apply to an attribute declaration\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_errors (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zp_abi]] struct s;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"attributes before a declaration need a declarator to apply to; a struct's, union's, or enum's go after its keyword, as in `struct [[deprecated]] s { ... };`\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_errors (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct [[c6502::zp_abi]] s { int a; };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::zp_abi` doesn't apply to a struct\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_errors (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum e { A [[c6502::zp_abi]] };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::zp_abi` doesn't apply to enumeration constant 'A'\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_errors (call 19)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s { int a [[c6502::zeropage]]; };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::zeropage` doesn't apply to member 'a'\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_errors (call 20)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zeropage]] typedef int T;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::zeropage` doesn't apply to typedef 'T'\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_errors (call 21)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"__attribute__((zp_abi)) void f(void);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"Unexpected token Token('IDENTIFIER', '__attribute__') at line 1, column 1.\\nExpected one of: \\n\\t* $END\\n\\t* ALT_BOOL\\n\\t* ALT_THREAD_LOCAL\\n\\t* ATTR_OPEN\\n\\t* AUTO\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* CONSTEXPR\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* EXTERN\\n\\t* FLOAT\\n\\t* INT\\n\\t* LONG\\n\\t* REGISTER\\n\\t* RESTRICT\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STATIC\\n\\t* STRUCT\\n\\t* THREAD_LOCAL\\n\\t* TYPEDEF\\n\\t* TYPEDEF_NAME\\n\\t* TYPEOF\\n\\t* TYPEOF_UNQUAL\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\\nc6502 doesn't take GCC's `__attribute__((...))`; it takes C23 attributes, such as `[[c6502::zp_abi]]` before a declaration or `uint8_t n [[c6502::in_x]]` after a name\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_errors (call 22)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"unsigned char n __attribute__((zeropage));\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"Unexpected token Token('IDENTIFIER', '__attribute__') at line 1, column 17.\\nExpected one of: \\n\\t* ASSIGN\\n\\t* ATTR_OPEN\\n\\t* COMMA\\n\\t* LBRACE\\n\\t* LBRACKET\\n\\t* LPAREN\\n\\t* SEMICOLON\\n\\nc6502 doesn't take GCC's `__attribute__((...))`; it takes C23 attributes, such as `[[c6502::zp_abi]]` before a declaration or `uint8_t n [[c6502::in_x]]` after a name\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_type_positions_arent_parsed (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_type_positions_arent_parsed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(void) [[c6502::zp_abi]];\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('ATTR_OPEN', '[[') at line 1, column 14.\\nExpected one of: \\n\\t* ASSIGN\\n\\t* COMMA\\n\\t* LBRACE\\n\\t* LBRACKET\\n\\t* LPAREN\\n\\t* SEMICOLON\\n\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_type_positions_arent_parsed (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_type_positions_arent_parsed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int [[c6502::zeropage]] x;\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('ATTR_OPEN', '[[') at line 1, column 5.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* ALT_THREAD_LOCAL\\n\\t* AUTO\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* CONSTEXPR\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* EXTERN\\n\\t* FLOAT\\n\\t* IDENTIFIER\\n\\t* INT\\n\\t* LONG\\n\\t* LPAREN\\n\\t* REGISTER\\n\\t* RESTRICT\\n\\t* SEMICOLON\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STAR\\n\\t* STATIC\\n\\t* STRUCT\\n\\t* THREAD_LOCAL\\n\\t* TYPEDEF\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_type_positions_arent_parsed (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_type_positions_arent_parsed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int * [[c6502::zeropage]] p;\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('ATTR_OPEN', '[[') at line 1, column 7.\\nExpected one of: \\n\\t* CONST\\n\\t* IDENTIFIER\\n\\t* LPAREN\\n\\t* RESTRICT\\n\\t* STAR\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_c23_attributes.py::TestErrors::test_type_positions_arent_parsed (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_attributes.py::TestErrors::test_type_positions_arent_parsed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[3] [[c6502::zeropage]];\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('ATTR_OPEN', '[[') at line 1, column 10.\\nExpected one of: \\n\\t* ASSIGN\\n\\t* COMMA\\n\\t* LBRACE\\n\\t* LBRACKET\\n\\t* LPAREN\\n\\t* SEMICOLON\\n\"}\n");
}
