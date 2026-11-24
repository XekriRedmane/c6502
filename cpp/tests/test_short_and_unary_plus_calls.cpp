// The calls that the tests of tests/test_short_and_unary_plus.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_short_and_unary_plus.py::TestParse::test_bad_short_combinations (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestParse::test_bad_short_combinations\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"short short x;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'short' specified more than once\"}\n");
}

TEST_CASE("parse: test_short_and_unary_plus.py::TestParse::test_bad_short_combinations (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestParse::test_bad_short_combinations\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"short long x;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'short' cannot combine with 'long'\"}\n");
}

TEST_CASE("parse: test_short_and_unary_plus.py::TestParse::test_bad_short_combinations (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestParse::test_bad_short_combinations\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long short int x;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'short' cannot combine with 'long'\"}\n");
}

TEST_CASE("parse: test_short_and_unary_plus.py::TestParse::test_bad_short_combinations (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestParse::test_bad_short_combinations\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"short char x;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'char' cannot combine with 'int' / 'long' / 'short'\"}\n");
}

TEST_CASE("parse: test_short_and_unary_plus.py::TestParse::test_bad_short_combinations (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestParse::test_bad_short_combinations\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"short double x;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"floating type cannot combine with 'int' / 'signed' / 'unsigned' / 'char'\"}\n");
}

TEST_CASE("parse: test_short_and_unary_plus.py::TestParse::test_bad_short_combinations (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestParse::test_bad_short_combinations\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"bool short x;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'bool' cannot combine with another type specifier; `bool` is a keyword in C23 (\\u00a76.4.1), so it can't be a name either\"}\n");
}

TEST_CASE("parse: test_short_and_unary_plus.py::TestParse::test_binary_plus_still_parses (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestParse::test_binary_plus_still_parses\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 1 + +2; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Binary(Add Constant(ConstInt(1) None) Unary(Plus Constant(ConstInt(2) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_short_and_unary_plus.py::TestParse::test_short_specifiers (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestParse::test_short_specifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"short x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Short None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_short_and_unary_plus.py::TestParse::test_short_specifiers (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestParse::test_short_specifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"short int x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Short None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_short_and_unary_plus.py::TestParse::test_short_specifiers (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestParse::test_short_specifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"signed short x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Short None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_short_and_unary_plus.py::TestParse::test_short_specifiers (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestParse::test_short_specifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int short signed x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Short None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_short_and_unary_plus.py::TestParse::test_short_specifiers (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestParse::test_short_specifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"unsigned short x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None UShort None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_short_and_unary_plus.py::TestParse::test_short_specifiers (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestParse::test_short_specifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"short unsigned int x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None UShort None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_short_and_unary_plus.py::TestParse::test_unary_plus (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestParse::test_unary_plus\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return +42; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Unary(Plus Constant(ConstInt(42) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_short_and_unary_plus.py::TestTypeCheck::test_plus_on_pointer_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_plus_on_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int *p; int main(void) { +p; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"p\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_short_and_unary_plus.py::TestTypeCheck::test_plus_on_pointer_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_plus_on_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"p\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"p\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_short_and_unary_plus.py::TestTypeCheck::test_plus_on_pointer_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_plus_on_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"p\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"unary '+' is not defined on pointer type Pointer(referenced_type=Int())\"}\n");
}

TEST_CASE("parse: test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"unsigned char c; int main(void) { +c; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"c\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"c\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"c\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"c\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Unary(Plus Cast(Int Var(\\\"c\\\" UChar) Int) Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"c\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"c\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Unary(Plus Cast(Int Var(\\\"c\\\" UChar) Int) Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"short c; int main(void) { +c; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None Short None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"c\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None Short None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"c\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None Short None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"c\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"c\\\" None Short None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Unary(Plus Cast(Int Var(\\\"c\\\" Short) Int) Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None Short None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"c\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"c\\\" None Short None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Unary(Plus Cast(Int Var(\\\"c\\\" Short) Int) Int)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Short\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"unsigned short c; int main(void) { +c; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None UShort None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"c\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None UShort None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"c\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None UShort None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"c\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"c\\\" None UShort None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Unary(Plus Cast(UInt Var(\\\"c\\\" UShort) UInt) UInt)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None UShort None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"c\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"c\\\" None UShort None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Unary(Plus Cast(UInt Var(\\\"c\\\" UShort) UInt) UInt)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UShort\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long c; int main(void) { +c; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None Long None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"c\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None Long None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"c\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None Long None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"c\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"c\\\" None Long None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Unary(Plus Var(\\\"c\\\" Long) Long)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None Long None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"c\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"c\\\" None Long None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Unary(Plus Var(\\\"c\\\" Long) Long)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"double c; int main(void) { +c; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None Double None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"c\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None Double None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"c\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None Double None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"c\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_plus_promotes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"c\\\" None Double None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Unary(Plus Var(\\\"c\\\" Double) Double)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None Double None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Unary(Plus Var(\"c\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"c\\\" None Double None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Unary(Plus Var(\\\"c\\\" Double) Double)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Double\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_short_and_unary_plus.py::TestTypeCheck::test_short_is_not_int (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_short_is_not_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"short *p; int *q; int main(void) { q = p; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Short) None None None None false))\n"
      "  VarDecl(var_decl(\"q\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"q\" None) Var(\"p\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_short_and_unary_plus.py::TestTypeCheck::test_short_is_not_int (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_short_is_not_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Short) None None None None false))\n"
      "  VarDecl(var_decl(\"q\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"q\" None) Var(\"p\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Short) None None None None false))\n"
      "  VarDecl(var_decl(\"q\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"q\" None) Var(\"p\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_short_and_unary_plus.py::TestTypeCheck::test_short_is_not_int (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_short_is_not_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Short) None None None None false))\n"
      "  VarDecl(var_decl(\"q\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"q\" None) Var(\"p\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot implicitly convert Pointer(referenced_type=Short()) to Pointer(referenced_type=Int()); pointer-to-pointer assignment requires matching pointee types per C99 \\u00a76.5.16.1.1\"}\n");
}

TEST_CASE("parse: test_short_and_unary_plus.py::TestTypeCheck::test_short_is_not_int (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_short_is_not_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(short a); int f(int a) { return a; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\"] None FunType([Short] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\"] Block([\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_short_and_unary_plus.py::TestTypeCheck::test_short_is_not_int (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_short_is_not_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\"] None FunType([Short] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\"] Block([\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\"] None FunType([Short] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@1.a\"] Block([\n"
      "    S(Return(Var(\"@1.a\" None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_short_and_unary_plus.py::TestTypeCheck::test_short_is_not_int (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_short_is_not_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\"] None FunType([Short] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@1.a\"] Block([\n"
      "    S(Return(Var(\"@1.a\" None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"incompatible redeclaration of 'f': previous FunType(params=[Short()], ret=Int()), new FunType(params=[Int()], ret=Int())\"}\n");
}

TEST_CASE("parse: test_short_and_unary_plus.py::TestTypeCheck::test_unsigned_short_promotes_to_unsigned_int (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_unsigned_short_promotes_to_unsigned_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"unsigned short us; int main(void) { return us + 1 == 0; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"us\" None UShort None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Binary(Equal Binary(Add Var(\"us\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_short_and_unary_plus.py::TestTypeCheck::test_unsigned_short_promotes_to_unsigned_int (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_unsigned_short_promotes_to_unsigned_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"us\" None UShort None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Binary(Equal Binary(Add Var(\"us\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"us\" None UShort None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Binary(Equal Binary(Add Var(\"us\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_short_and_unary_plus.py::TestTypeCheck::test_unsigned_short_promotes_to_unsigned_int (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_short_and_unary_plus.py::TestTypeCheck::test_unsigned_short_promotes_to_unsigned_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"us\\\" None UShort None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Binary(Equal Binary(Add Cast(UInt Var(\\\"us\\\" UShort) UInt) Cast(UInt Constant(ConstInt(1) Int) UInt) UInt) Cast(UInt Constant(ConstInt(0) Int) UInt) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"us\" None UShort None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Binary(Equal Binary(Add Var(\"us\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"us\\\" None UShort None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Binary(Equal Binary(Add Cast(UInt Var(\\\"us\\\" UShort) UInt) Cast(UInt Constant(ConstInt(1) Int) UInt) UInt) Cast(UInt Constant(ConstInt(0) Int) UInt) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"us\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UShort\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}
