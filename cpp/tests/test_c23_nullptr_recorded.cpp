// The calls that the tests of tests/test_c23_nullptr.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_c23_nullptr.py::TestKeywords::test_names_are_errors (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestKeywords::test_names_are_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int nullptr;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"line 1, column 5: `nullptr` is a keyword in C23 (\\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestKeywords::test_names_are_errors (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestKeywords::test_names_are_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int nullptr = 0;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"line 1, column 5: `nullptr` is a keyword in C23 (\\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestKeywords::test_names_are_errors (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestKeywords::test_names_are_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int *nullptr(void);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"line 1, column 6: `nullptr` is a keyword in C23 (\\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestKeywords::test_names_are_errors (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestKeywords::test_names_are_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int nullptr) { return 0; }\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"line 1, column 11: `nullptr` is a keyword in C23 (\\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestKeywords::test_names_are_errors (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestKeywords::test_names_are_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s { int nullptr; };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"line 1, column 16: `nullptr` is a keyword in C23 (\\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestKeywords::test_names_are_errors (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestKeywords::test_names_are_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct nullptr { int x; };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"line 1, column 8: `nullptr` is a keyword in C23 (\\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestKeywords::test_names_are_errors (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestKeywords::test_names_are_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum { nullptr };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"line 1, column 8: `nullptr` is a keyword in C23 (\\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestKeywords::test_nullptr_is_a_constant (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestKeywords::test_nullptr_is_a_constant\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int *p = nullptr;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" Constant(ConstNullptr None) Pointer(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestKeywords::test_nullptr_t_is_an_ordinary_name_without_stddef (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestKeywords::test_nullptr_t_is_an_ordinary_name_without_stddef\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int nullptr_t = 3;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"nullptr_t\" Constant(ConstInt(3) None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestKeywords::test_nullptr_t_is_an_ordinary_name_without_stddef (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestKeywords::test_nullptr_t_is_an_ordinary_name_without_stddef\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"nullptr_t\" Constant(ConstInt(3) None) Int None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"nullptr_t\" Constant(ConstInt(3) None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestKeywords::test_nullptr_t_is_an_ordinary_name_without_stddef (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestKeywords::test_nullptr_t_is_an_ordinary_name_without_stddef\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"nullptr_t\\\" Constant(ConstInt(3) Int) Int None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"nullptr_t\" Constant(ConstInt(3) None) Int None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"nullptr_t\\\" Constant(ConstInt(3) Int) Int None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"nullptr_t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 3}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestKeywords::test_nullptr_t_takes_no_other_specifier (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestKeywords::test_nullptr_t_takes_no_other_specifier\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typeof(nullptr) unsigned n;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`typeof` cannot combine with another type specifier\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestKeywords::test_type_of_nullptr (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestKeywords::test_type_of_nullptr\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(void) { nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestKeywords::test_type_of_nullptr (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestKeywords::test_type_of_nullptr\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestKeywords::test_type_of_nullptr (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestKeywords::test_type_of_nullptr\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Expression(Constant(ConstNullptr NullptrT)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Expression(Constant(ConstNullptr NullptrT)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestKeywords::test_typeof_names_nullptr_t (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestKeywords::test_typeof_names_nullptr_t\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typeof(nullptr) n; const typeof_unqual(nullptr) m;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  VarDecl(var_decl(\"m\" None Const(NullptrT) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p = nullptr;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" Constant(ConstNullptr None) Pointer(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" Constant(ConstNullptr None) Pointer(Int) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" Constant(ConstNullptr None) Pointer(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" Cast(Pointer(Int) Constant(ConstNullptr NullptrT) Pointer(Int)) Pointer(Int) None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" Constant(ConstNullptr None) Pointer(Int) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" Cast(Pointer(Int) Constant(ConstNullptr NullptrT) Pointer(Int)) Pointer(Int) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nvoid (*fp)(void) = nullptr;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"fp\" Constant(ConstNullptr None) Pointer(FunType([] Void)) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"fp\" Constant(ConstNullptr None) Pointer(FunType([] Void)) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"fp\" Constant(ConstNullptr None) Pointer(FunType([] Void)) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"fp\\\" Cast(Pointer(FunType([] Void)) Constant(ConstNullptr NullptrT) Pointer(FunType([] Void))) Pointer(FunType([] Void)) None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"fp\" Constant(ConstNullptr None) Pointer(FunType([] Void)) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"fp\\\" Cast(Pointer(FunType([] Void)) Constant(ConstNullptr NullptrT) Pointer(FunType([] Void))) Pointer(FunType([] Void)) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"fp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([] Void))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nvoid *v = nullptr;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Constant(ConstNullptr None) Pointer(Void) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Constant(ConstNullptr None) Pointer(Void) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Constant(ConstNullptr None) Pointer(Void) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Pointer(Void) None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Constant(ConstNullptr None) Pointer(Void) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"v\\\" Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Pointer(Void) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nconst char *s = nullptr;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" Constant(ConstNullptr None) Pointer(Const(Char)) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" Constant(ConstNullptr None) Pointer(Const(Char)) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" Constant(ConstNullptr None) Pointer(Const(Char)) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"s\\\" Cast(Pointer(Const(Char)) Constant(ConstNullptr NullptrT) Pointer(Const(Char))) Pointer(Const(Char)) None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" Constant(ConstNullptr None) Pointer(Const(Char)) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"s\\\" Cast(Pointer(Const(Char)) Constant(ConstNullptr NullptrT) Pointer(Const(Char))) Pointer(Const(Char)) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(Char))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nnullptr_t n = nullptr;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstNullptr None) NullptrT None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstNullptr None) NullptrT None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstNullptr None) NullptrT None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Constant(ConstNullptr NullptrT) NullptrT None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstNullptr None) NullptrT None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Constant(ConstNullptr NullptrT) NullptrT None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 19)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nnullptr_t n = 0;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstInt(0) None) NullptrT None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 20)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstInt(0) None) NullptrT None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstInt(0) None) NullptrT None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 21)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(NullptrT Constant(ConstInt(0) Int) NullptrT) NullptrT None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstInt(0) None) NullptrT None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(NullptrT Constant(ConstInt(0) Int) NullptrT) NullptrT None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 22)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nnullptr_t n = 1 - 1;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Binary(Subtract Constant(ConstInt(1) None) Constant(ConstInt(1) None) None) NullptrT None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 23)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Binary(Subtract Constant(ConstInt(1) None) Constant(ConstInt(1) None) None) NullptrT None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Binary(Subtract Constant(ConstInt(1) None) Constant(ConstInt(1) None) None) NullptrT None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 24)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(NullptrT Binary(Subtract Constant(ConstInt(1) Int) Constant(ConstInt(1) Int) Int) NullptrT) NullptrT None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Binary(Subtract Constant(ConstInt(1) None) Constant(ConstInt(1) None) None) NullptrT None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(NullptrT Binary(Subtract Constant(ConstInt(1) Int) Constant(ConstInt(1) Int) Int) NullptrT) NullptrT None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 25)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nnullptr_t n = false;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstBool(0) None) NullptrT None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 26)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstBool(0) None) NullptrT None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstBool(0) None) NullptrT None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 27)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(NullptrT Constant(ConstBool(0) Bool) NullptrT) NullptrT None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstBool(0) None) NullptrT None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(NullptrT Constant(ConstBool(0) Bool) NullptrT) NullptrT None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 28)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nnullptr_t n = (void *)0;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Cast(Pointer(Void) Constant(ConstInt(0) None) None) NullptrT None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 29)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Cast(Pointer(Void) Constant(ConstInt(0) None) None) NullptrT None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Cast(Pointer(Void) Constant(ConstInt(0) None) None) NullptrT None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 30)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(NullptrT Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) NullptrT) NullptrT None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Cast(Pointer(Void) Constant(ConstInt(0) None) None) NullptrT None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(NullptrT Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) NullptrT) NullptrT None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 31)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nvoid f(int *p); void g(void) { f(nullptr); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\"] None FunType([Pointer(Int)] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Expression(FunctionCall(\"f\" [Constant(ConstNullptr None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 32)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\"] None FunType([Pointer(Int)] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Expression(FunctionCall(\"f\" [Constant(ConstNullptr None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] None FunType([Pointer(Int)] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Expression(FunctionCall(\"f\" [Constant(ConstNullptr None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 33)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\"] None FunType([Pointer(Int)] Void) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [Cast(Pointer(Int) Constant(ConstNullptr NullptrT) Pointer(Int))] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] None FunType([Pointer(Int)] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Expression(FunctionCall(\"f\" [Constant(ConstNullptr None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\"] None FunType([Pointer(Int)] Void) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [Cast(Pointer(Int) Constant(ConstNullptr NullptrT) Pointer(Int))] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Int)] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 34)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nvoid f(bool b); void g(void) { f(nullptr); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"b\"] None FunType([Bool] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Expression(FunctionCall(\"f\" [Constant(ConstNullptr None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 35)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"b\"] None FunType([Bool] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Expression(FunctionCall(\"f\" [Constant(ConstNullptr None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.b\"] None FunType([Bool] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Expression(FunctionCall(\"f\" [Constant(ConstNullptr None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 36)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.b\\\"] None FunType([Bool] Void) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [Cast(Bool Constant(ConstNullptr NullptrT) Bool)] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.b\"] None FunType([Bool] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Expression(FunctionCall(\"f\" [Constant(ConstNullptr None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.b\\\"] None FunType([Bool] Void) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [Cast(Bool Constant(ConstNullptr NullptrT) Bool)] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Bool] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 37)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nvoid f(nullptr_t n); void g(void) { f(nullptr); f(0); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"n\"] None FunType([NullptrT] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Expression(FunctionCall(\"f\" [Constant(ConstNullptr None)] None false)))\n"
      "    S(Expression(FunctionCall(\"f\" [Constant(ConstInt(0) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 38)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"n\"] None FunType([NullptrT] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Expression(FunctionCall(\"f\" [Constant(ConstNullptr None)] None false)))\n"
      "    S(Expression(FunctionCall(\"f\" [Constant(ConstInt(0) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.n\"] None FunType([NullptrT] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Expression(FunctionCall(\"f\" [Constant(ConstNullptr None)] None false)))\n"
      "    S(Expression(FunctionCall(\"f\" [Constant(ConstInt(0) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 39)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.n\\\"] None FunType([NullptrT] Void) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [Constant(ConstNullptr NullptrT)] Void false)))\\n    S(Expression(FunctionCall(\\\"f\\\" [Cast(NullptrT Constant(ConstInt(0) Int) NullptrT)] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.n\"] None FunType([NullptrT] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Expression(FunctionCall(\"f\" [Constant(ConstNullptr None)] None false)))\n"
      "    S(Expression(FunctionCall(\"f\" [Constant(ConstInt(0) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.n\\\"] None FunType([NullptrT] Void) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [Constant(ConstNullptr NullptrT)] Void false)))\\n    S(Expression(FunctionCall(\\\"f\\\" [Cast(NullptrT Constant(ConstInt(0) Int) NullptrT)] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([NullptrT] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 40)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *f(void) { return nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 41)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 42)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Cast(Pointer(Int) Constant(ConstNullptr NullptrT) Pointer(Int))))\\n  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Cast(Pointer(Int) Constant(ConstNullptr NullptrT) Pointer(Int))))\\n  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Pointer(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 43)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nnullptr_t f(void) { return 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] NullptrT) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 44)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] NullptrT) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] NullptrT) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 45)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Cast(NullptrT Constant(ConstInt(0) Int) NullptrT)))\\n  ]) FunType([] NullptrT) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] NullptrT) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Cast(NullptrT Constant(ConstInt(0) Int) NullptrT)))\\n  ]) FunType([] NullptrT) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] NullptrT)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 46)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nnullptr_t f(void) { return nullptr; } int *g(void) { return f(); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] NullptrT) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"f\" [] None false)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 47)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] NullptrT) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"f\" [] None false)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] NullptrT) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"f\" [] None false)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 48)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Constant(ConstNullptr NullptrT)))\\n  ]) FunType([] NullptrT) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(Pointer(Int) FunctionCall(\\\"f\\\" [] NullptrT false) Pointer(Int))))\\n  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] NullptrT) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"f\" [] None false)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Constant(ConstNullptr NullptrT)))\\n  ]) FunType([] NullptrT) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(Pointer(Int) FunctionCall(\\\"f\\\" [] NullptrT false) Pointer(Int))))\\n  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] NullptrT)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Pointer(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 49)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nvoid f(void) { nullptr_t n; int *p; p = n; n = nullptr; n = 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"n\" None NullptrT None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"p\" None) Var(\"n\" None) None)))\n"
      "    S(Expression(Assignment(Var(\"n\" None) Constant(ConstNullptr None) None)))\n"
      "    S(Expression(Assignment(Var(\"n\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 50)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"n\" None NullptrT None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"p\" None) Var(\"n\" None) None)))\n"
      "    S(Expression(Assignment(Var(\"n\" None) Constant(ConstNullptr None) None)))\n"
      "    S(Expression(Assignment(Var(\"n\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.n\" None NullptrT None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" None Pointer(Int) None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"@1.p\" None) Var(\"@0.n\" None) None)))\n"
      "    S(Expression(Assignment(Var(\"@0.n\" None) Constant(ConstNullptr None) None)))\n"
      "    S(Expression(Assignment(Var(\"@0.n\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 51)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.n\\\" None NullptrT None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" None Pointer(Int) None None None None false)))\\n    S(Expression(Assignment(Var(\\\"@1.p\\\" Pointer(Int)) Cast(Pointer(Int) Var(\\\"@0.n\\\" NullptrT) Pointer(Int)) Pointer(Int))))\\n    S(Expression(Assignment(Var(\\\"@0.n\\\" NullptrT) Constant(ConstNullptr NullptrT) NullptrT)))\\n    S(Expression(Assignment(Var(\\\"@0.n\\\" NullptrT) Cast(NullptrT Constant(ConstInt(0) Int) NullptrT) NullptrT)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.n\" None NullptrT None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" None Pointer(Int) None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"@1.p\" None) Var(\"@0.n\" None) None)))\n"
      "    S(Expression(Assignment(Var(\"@0.n\" None) Constant(ConstNullptr None) None)))\n"
      "    S(Expression(Assignment(Var(\"@0.n\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.n\\\" None NullptrT None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" None Pointer(Int) None None None None false)))\\n    S(Expression(Assignment(Var(\\\"@1.p\\\" Pointer(Int)) Cast(Pointer(Int) Var(\\\"@0.n\\\" NullptrT) Pointer(Int)) Pointer(Int))))\\n    S(Expression(Assignment(Var(\\\"@0.n\\\" NullptrT) Constant(ConstNullptr NullptrT) NullptrT)))\\n    S(Expression(Assignment(Var(\\\"@0.n\\\" NullptrT) Cast(NullptrT Constant(ConstInt(0) Int) NullptrT) NullptrT)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 52)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nvoid f(void) { (void)nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Void Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 53)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Void Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Void Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 54)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Expression(Cast(Void Constant(ConstNullptr NullptrT) Void)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Void Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Expression(Cast(Void Constant(ConstNullptr NullptrT) Void)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 55)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nbool f(void) { return (bool)nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Cast(Bool Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Bool) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 56)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Cast(Bool Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Bool) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Cast(Bool Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Bool) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 57)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Cast(Bool Constant(ConstNullptr NullptrT) Bool)))\\n  ]) FunType([] Bool) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Cast(Bool Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Bool) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Cast(Bool Constant(ConstNullptr NullptrT) Bool)))\\n  ]) FunType([] Bool) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 58)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nchar *f(void) { return (char *)nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Cast(Pointer(Char) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Pointer(Char)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 59)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Cast(Pointer(Char) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Pointer(Char)) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Cast(Pointer(Char) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Pointer(Char)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 60)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Cast(Pointer(Char) Constant(ConstNullptr NullptrT) Pointer(Char))))\\n  ]) FunType([] Pointer(Char)) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Cast(Pointer(Char) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Pointer(Char)) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Cast(Pointer(Char) Constant(ConstNullptr NullptrT) Pointer(Char))))\\n  ]) FunType([] Pointer(Char)) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Pointer(Char))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 61)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nnullptr_t f(nullptr_t n) { return (nullptr_t)n; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"n\"] Block([\n"
      "    S(Return(Cast(NullptrT Var(\"n\" None) None)))\n"
      "  ]) FunType([NullptrT] NullptrT) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 62)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"n\"] Block([\n"
      "    S(Return(Cast(NullptrT Var(\"n\" None) None)))\n"
      "  ]) FunType([NullptrT] NullptrT) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.n\"] Block([\n"
      "    S(Return(Cast(NullptrT Var(\"@0.n\" None) None)))\n"
      "  ]) FunType([NullptrT] NullptrT) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 63)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.n\\\"] Block([\\n    S(Return(Cast(NullptrT Var(\\\"@0.n\\\" NullptrT) NullptrT)))\\n  ]) FunType([NullptrT] NullptrT) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.n\"] Block([\n"
      "    S(Return(Cast(NullptrT Var(\"@0.n\" None) None)))\n"
      "  ]) FunType([NullptrT] NullptrT) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.n\\\"] Block([\\n    S(Return(Cast(NullptrT Var(\\\"@0.n\\\" NullptrT) NullptrT)))\\n  ]) FunType([NullptrT] NullptrT) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([NullptrT] NullptrT)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 64)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nnullptr_t f(void) { return (nullptr_t)0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Cast(NullptrT Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] NullptrT) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 65)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Cast(NullptrT Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] NullptrT) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Cast(NullptrT Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] NullptrT) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 66)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Cast(NullptrT Constant(ConstInt(0) Int) NullptrT)))\\n  ]) FunType([] NullptrT) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Cast(NullptrT Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] NullptrT) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Cast(NullptrT Constant(ConstInt(0) Int) NullptrT)))\\n  ]) FunType([] NullptrT) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] NullptrT)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 67)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nnullptr_t f(void) { return (nullptr_t)(void *)0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Cast(NullptrT Cast(Pointer(Void) Constant(ConstInt(0) None) None) None)))\n"
      "  ]) FunType([] NullptrT) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 68)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Cast(NullptrT Cast(Pointer(Void) Constant(ConstInt(0) None) None) None)))\n"
      "  ]) FunType([] NullptrT) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Cast(NullptrT Cast(Pointer(Void) Constant(ConstInt(0) None) None) None)))\n"
      "  ]) FunType([] NullptrT) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 69)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Cast(NullptrT Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) NullptrT)))\\n  ]) FunType([] NullptrT) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Cast(NullptrT Cast(Pointer(Void) Constant(ConstInt(0) None) None) None)))\n"
      "  ]) FunType([] NullptrT) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Cast(NullptrT Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) NullptrT)))\\n  ]) FunType([] NullptrT) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] NullptrT)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 70)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nstruct s { nullptr_t n; int *p; }; struct s v = { nullptr, nullptr };\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" NullptrT) member_decl(\"p\" Pointer(Int))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Constant(ConstNullptr None) Constant(ConstNullptr None)] None None) Structure(\"s\") None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 71)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" NullptrT) member_decl(\"p\" Pointer(Int))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Constant(ConstNullptr None) Constant(ConstNullptr None)] None None) Structure(\"s\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" NullptrT) member_decl(\"p\" Pointer(Int))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Constant(ConstNullptr None) Constant(ConstNullptr None)] None None) Structure(\"s\") None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 72)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"n\\\" NullptrT) member_decl(\\\"p\\\" Pointer(Int))]))\\n  VarDecl(var_decl(\\\"v\\\" InitList([Constant(ConstNullptr NullptrT) Cast(Pointer(Int) Constant(ConstNullptr NullptrT) Pointer(Int))] Structure(\\\"s\\\") None) Structure(\\\"s\\\") None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" NullptrT) member_decl(\"p\" Pointer(Int))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Constant(ConstNullptr None) Constant(ConstNullptr None)] None None) Structure(\"s\") None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"n\\\" NullptrT) member_decl(\\\"p\\\" Pointer(Int))]))\\n  VarDecl(var_decl(\\\"v\\\" InitList([Constant(ConstNullptr NullptrT) Cast(Pointer(Int) Constant(ConstNullptr NullptrT) Pointer(Int))] Structure(\\\"s\\\") None) Structure(\\\"s\\\") None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Structure(\\\"s\\\")\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0, 0]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"n\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"p\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"byte_offset\": 2}}], \"size\": 4, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_allowed (call 73)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nnullptr_t a[3];\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(NullptrT 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_allowed (call 74)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(NullptrT 3) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(NullptrT 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_allowed (call 75)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(NullptrT 3) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None Array(NullptrT 3) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(NullptrT 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nvoid f(void) { (int)nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Int Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Int Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Int Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Int Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot cast nullptr_t to int: nullptr_t converts only to void, bool, or a pointer type (C23 \\u00a76.5.5p4)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nvoid f(void) { (unsigned long)nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(ULong Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(ULong Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(ULong Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(ULong Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot cast nullptr_t to unsigned long: nullptr_t converts only to void, bool, or a pointer type (C23 \\u00a76.5.5p4)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nvoid f(void) { (float)nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Float Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Float Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Float Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Float Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot cast nullptr_t to float: nullptr_t converts only to void, bool, or a pointer type (C23 \\u00a76.5.5p4)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nvoid f(void) { (char)nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Char Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Char Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Char Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Char Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot cast nullptr_t to char: nullptr_t converts only to void, bool, or a pointer type (C23 \\u00a76.5.5p4)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; int x; void f(void) { (nullptr_t)1; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(NullptrT Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(NullptrT Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(NullptrT Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(NullptrT Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot cast int to nullptr_t: the operand must be a null pointer constant or have type nullptr_t (C23 \\u00a76.5.5p4)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; int x; void f(void) { (nullptr_t)p; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(NullptrT Var(\"p\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(NullptrT Var(\"p\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(NullptrT Var(\"p\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(NullptrT Var(\"p\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot cast int * to nullptr_t: the operand must be a null pointer constant or have type nullptr_t (C23 \\u00a76.5.5p4)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 19)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; int x; void f(void) { (nullptr_t)(int *)0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(NullptrT Cast(Pointer(Int) Constant(ConstInt(0) None) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 20)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(NullptrT Cast(Pointer(Int) Constant(ConstInt(0) None) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(NullptrT Cast(Pointer(Int) Constant(ConstInt(0) None) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 21)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(NullptrT Cast(Pointer(Int) Constant(ConstInt(0) None) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot cast int * to nullptr_t: the operand must be a null pointer constant or have type nullptr_t (C23 \\u00a76.5.5p4)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 22)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; int x; void f(void) { (nullptr_t)x; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(NullptrT Var(\"x\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 23)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(NullptrT Var(\"x\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(NullptrT Var(\"x\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_casts_rejected (call 24)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_casts_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(NullptrT Var(\"x\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot cast int to nullptr_t: the operand must be a null pointer constant or have type nullptr_t (C23 \\u00a76.5.5p4)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint x = nullptr;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstNullptr None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstNullptr None) Int None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstNullptr None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstNullptr None) Int None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot convert nullptr_t to int: a nullptr_t value converts implicitly only to a pointer type or bool (C23 \\u00a76.3.2.4, \\u00a76.5.17.2)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nlong f(void) { return nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot convert nullptr_t to long: a nullptr_t value converts implicitly only to a pointer type or bool (C23 \\u00a76.3.2.4, \\u00a76.5.17.2)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nvoid f(unsigned char c); void g(void) { f(nullptr); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"c\"] None FunType([UChar] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Expression(FunctionCall(\"f\" [Constant(ConstNullptr None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"c\"] None FunType([UChar] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Expression(FunctionCall(\"f\" [Constant(ConstNullptr None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.c\"] None FunType([UChar] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Expression(FunctionCall(\"f\" [Constant(ConstNullptr None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.c\"] None FunType([UChar] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Expression(FunctionCall(\"f\" [Constant(ConstNullptr None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot convert nullptr_t to unsigned char: a nullptr_t value converts implicitly only to a pointer type or bool (C23 \\u00a76.3.2.4, \\u00a76.5.17.2)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\ndouble d = nullptr;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"d\" Constant(ConstNullptr None) Double None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"d\" Constant(ConstNullptr None) Double None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"d\" Constant(ConstNullptr None) Double None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"d\" Constant(ConstNullptr None) Double None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot convert nullptr_t to double: a nullptr_t value converts implicitly only to a pointer type or bool (C23 \\u00a76.3.2.4, \\u00a76.5.17.2)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nvoid f(void) { int i; i = nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"i\" None Int None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"i\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"i\" None Int None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"i\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.i\" None Int None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"@0.i\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.i\" None Int None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"@0.i\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot convert nullptr_t to int: a nullptr_t value converts implicitly only to a pointer type or bool (C23 \\u00a76.3.2.4, \\u00a76.5.17.2)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nnullptr_t n = 1;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstInt(1) None) NullptrT None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstInt(1) None) NullptrT None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstInt(1) None) NullptrT None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstInt(1) None) NullptrT None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot convert int to nullptr_t: only nullptr, another nullptr_t value, or a null pointer constant converts to nullptr_t (C23 \\u00a76.3.2.4, \\u00a76.5.17.2)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 19)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nnullptr_t n = (int *)0;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Cast(Pointer(Int) Constant(ConstInt(0) None) None) NullptrT None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 20)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Cast(Pointer(Int) Constant(ConstInt(0) None) None) NullptrT None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Cast(Pointer(Int) Constant(ConstInt(0) None) None) NullptrT None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 21)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" Cast(Pointer(Int) Constant(ConstInt(0) None) None) NullptrT None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot convert int * to nullptr_t: only nullptr, another nullptr_t value, or a null pointer constant converts to nullptr_t (C23 \\u00a76.3.2.4, \\u00a76.5.17.2)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 22)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nvoid *v; void f(void) { nullptr_t n = v; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"n\" Var(\"v\" None) NullptrT None None None None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 23)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"n\" Var(\"v\" None) NullptrT None None None None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.n\" Var(\"v\" None) NullptrT None None None None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 24)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.n\" Var(\"v\" None) NullptrT None None None None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot convert void * to nullptr_t: only nullptr, another nullptr_t value, or a null pointer constant converts to nullptr_t (C23 \\u00a76.3.2.4, \\u00a76.5.17.2)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 25)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nnullptr_t f(int x) { return x; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([Int] NullptrT) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 26)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([Int] NullptrT) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([Int] NullptrT) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected (call 27)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestConversions::test_implicit_conversions_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([Int] NullptrT) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot convert int to nullptr_t: only nullptr, another nullptr_t value, or a null pointer constant converts to nullptr_t (C23 \\u00a76.3.2.4, \\u00a76.5.17.2)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; int f(void) { return nullptr + 1; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Add Constant(ConstNullptr None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Add Constant(ConstNullptr None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Add Constant(ConstNullptr None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Add Constant(ConstNullptr None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '+' is not defined on nullptr_t (nullptr_t + int)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; int f(void) { return p - nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Subtract Var(\"p\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Subtract Var(\"p\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Subtract Var(\"p\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Subtract Var(\"p\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '-' is not defined on nullptr_t (int * - nullptr_t)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; int f(void) { return nullptr * 2; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Multiply Constant(ConstNullptr None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Multiply Constant(ConstNullptr None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Multiply Constant(ConstNullptr None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Multiply Constant(ConstNullptr None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '*' is not defined on nullptr_t (nullptr_t * int)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; int f(void) { return nullptr & 1; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(BitwiseAnd Constant(ConstNullptr None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(BitwiseAnd Constant(ConstNullptr None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(BitwiseAnd Constant(ConstNullptr None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(BitwiseAnd Constant(ConstNullptr None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '&' is not defined on nullptr_t (nullptr_t & int)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; int f(void) { return 1 << nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(LeftShift Constant(ConstInt(1) None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(LeftShift Constant(ConstInt(1) None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(LeftShift Constant(ConstInt(1) None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(LeftShift Constant(ConstInt(1) None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '<<' is not defined on nullptr_t (int << nullptr_t)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint f(void) { return -nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Unary(Negate Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Unary(Negate Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Unary(Negate Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Unary(Negate Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"unary '-' is not defined on nullptr_t (C23 \\u00a76.5.4.3)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 19)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint f(void) { return +nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Unary(Plus Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 20)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Unary(Plus Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Unary(Plus Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 21)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Unary(Plus Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"unary '+' is not defined on nullptr_t (C23 \\u00a76.5.4.3)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 22)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint f(void) { return ~nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Unary(Complement Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 23)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Unary(Complement Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Unary(Complement Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 24)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Unary(Complement Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"unary '~' is not defined on nullptr_t (C23 \\u00a76.5.4.3)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 25)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nvoid f(void) { nullptr_t n; n++; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"n\" None NullptrT None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"n\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 26)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"n\" None NullptrT None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"n\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.n\" None NullptrT None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"@0.n\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 27)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.n\" None NullptrT None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"@0.n\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the operand of ++ or -- must have a real or pointer type, and nullptr_t is neither (C23 \\u00a76.5.3.5, \\u00a76.5.4.1)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 28)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nvoid f(void) { nullptr_t n; --n; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"n\" None NullptrT None None None None false)))\n"
      "    S(Expression(Prefix(Decrement Var(\"n\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 29)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"n\" None NullptrT None None None None false)))\n"
      "    S(Expression(Prefix(Decrement Var(\"n\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.n\" None NullptrT None None None None false)))\n"
      "    S(Expression(Prefix(Decrement Var(\"@0.n\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 30)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.n\" None NullptrT None None None None false)))\n"
      "    S(Expression(Prefix(Decrement Var(\"@0.n\" None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the operand of ++ or -- must have a real or pointer type, and nullptr_t is neither (C23 \\u00a76.5.3.5, \\u00a76.5.4.1)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 31)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nvoid f(void) { nullptr_t n; n += 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"n\" None NullptrT None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Var(\"n\" None) Constant(ConstInt(0) None) None None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 32)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"n\" None NullptrT None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Var(\"n\" None) Constant(ConstInt(0) None) None None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.n\" None NullptrT None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Var(\"@0.n\" None) Constant(ConstInt(0) None) None None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_arithmetic_rejected (call 33)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_arithmetic_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.n\" None NullptrT None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Var(\"@0.n\" None) Constant(ConstInt(0) None) None None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"compound assignment is not defined on nullptr_t (nullptr_t, int; C23 \\u00a76.5.17.3)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_conditional (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_conditional\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; nullptr_t n; void f(int c) { c \077 p : nullptr; c \077 n : p; c \077 nullptr : n; c \077 (void *)0 : nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"c\"] Block([\n"
      "    S(Expression(Conditional(Var(\"c\" None) Var(\"p\" None) Constant(ConstNullptr None) None)))\n"
      "    S(Expression(Conditional(Var(\"c\" None) Var(\"n\" None) Var(\"p\" None) None)))\n"
      "    S(Expression(Conditional(Var(\"c\" None) Constant(ConstNullptr None) Var(\"n\" None) None)))\n"
      "    S(Expression(Conditional(Var(\"c\" None) Cast(Pointer(Void) Constant(ConstInt(0) None) None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([Int] Void) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_conditional (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_conditional\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"c\"] Block([\n"
      "    S(Expression(Conditional(Var(\"c\" None) Var(\"p\" None) Constant(ConstNullptr None) None)))\n"
      "    S(Expression(Conditional(Var(\"c\" None) Var(\"n\" None) Var(\"p\" None) None)))\n"
      "    S(Expression(Conditional(Var(\"c\" None) Constant(ConstNullptr None) Var(\"n\" None) None)))\n"
      "    S(Expression(Conditional(Var(\"c\" None) Cast(Pointer(Void) Constant(ConstInt(0) None) None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([Int] Void) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.c\"] Block([\n"
      "    S(Expression(Conditional(Var(\"@0.c\" None) Var(\"p\" None) Constant(ConstNullptr None) None)))\n"
      "    S(Expression(Conditional(Var(\"@0.c\" None) Var(\"n\" None) Var(\"p\" None) None)))\n"
      "    S(Expression(Conditional(Var(\"@0.c\" None) Constant(ConstNullptr None) Var(\"n\" None) None)))\n"
      "    S(Expression(Conditional(Var(\"@0.c\" None) Cast(Pointer(Void) Constant(ConstInt(0) None) None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([Int] Void) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_conditional (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_conditional\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.c\\\"] Block([\\n    S(Expression(Conditional(Var(\\\"@0.c\\\" Int) Var(\\\"p\\\" Pointer(Int)) Cast(Pointer(Int) Constant(ConstNullptr NullptrT) Pointer(Int)) Pointer(Int))))\\n    S(Expression(Conditional(Var(\\\"@0.c\\\" Int) Cast(Pointer(Int) Var(\\\"n\\\" NullptrT) Pointer(Int)) Var(\\\"p\\\" Pointer(Int)) Pointer(Int))))\\n    S(Expression(Conditional(Var(\\\"@0.c\\\" Int) Constant(ConstNullptr NullptrT) Var(\\\"n\\\" NullptrT) NullptrT)))\\n    S(Expression(Conditional(Var(\\\"@0.c\\\" Int) Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Pointer(Void))))\\n  ]) FunType([Int] Void) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.c\"] Block([\n"
      "    S(Expression(Conditional(Var(\"@0.c\" None) Var(\"p\" None) Constant(ConstNullptr None) None)))\n"
      "    S(Expression(Conditional(Var(\"@0.c\" None) Var(\"n\" None) Var(\"p\" None) None)))\n"
      "    S(Expression(Conditional(Var(\"@0.c\" None) Constant(ConstNullptr None) Var(\"n\" None) None)))\n"
      "    S(Expression(Conditional(Var(\"@0.c\" None) Cast(Pointer(Void) Constant(ConstInt(0) None) None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([Int] Void) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.c\\\"] Block([\\n    S(Expression(Conditional(Var(\\\"@0.c\\\" Int) Var(\\\"p\\\" Pointer(Int)) Cast(Pointer(Int) Constant(ConstNullptr NullptrT) Pointer(Int)) Pointer(Int))))\\n    S(Expression(Conditional(Var(\\\"@0.c\\\" Int) Cast(Pointer(Int) Var(\\\"n\\\" NullptrT) Pointer(Int)) Var(\\\"p\\\" Pointer(Int)) Pointer(Int))))\\n    S(Expression(Conditional(Var(\\\"@0.c\\\" Int) Constant(ConstNullptr NullptrT) Var(\\\"n\\\" NullptrT) NullptrT)))\\n    S(Expression(Conditional(Var(\\\"@0.c\\\" Int) Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Pointer(Void))))\\n  ]) FunType([Int] Void) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_conditional_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_conditional_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nvoid f(int c) { c \077 nullptr : 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"c\"] Block([\n"
      "    S(Expression(Conditional(Var(\"c\" None) Constant(ConstNullptr None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Int] Void) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_conditional_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_conditional_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"c\"] Block([\n"
      "    S(Expression(Conditional(Var(\"c\" None) Constant(ConstNullptr None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Int] Void) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.c\"] Block([\n"
      "    S(Expression(Conditional(Var(\"@0.c\" None) Constant(ConstNullptr None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Int] Void) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_conditional_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_conditional_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.c\"] Block([\n"
      "    S(Expression(Conditional(Var(\"@0.c\" None) Constant(ConstNullptr None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Int] Void) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"conditional branches of type nullptr_t and int: the other branch of a nullptr_t branch must be a pointer or another nullptr_t (C23 \\u00a76.5.16p3)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_conditional_rejected (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_conditional_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nvoid f(int c) { c \077 1L : nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"c\"] Block([\n"
      "    S(Expression(Conditional(Var(\"c\" None) Constant(ConstLong(1) None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([Int] Void) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_conditional_rejected (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_conditional_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"c\"] Block([\n"
      "    S(Expression(Conditional(Var(\"c\" None) Constant(ConstLong(1) None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([Int] Void) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.c\"] Block([\n"
      "    S(Expression(Conditional(Var(\"@0.c\" None) Constant(ConstLong(1) None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([Int] Void) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_conditional_rejected (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_conditional_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.c\"] Block([\n"
      "    S(Expression(Conditional(Var(\"@0.c\" None) Constant(ConstLong(1) None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([Int] Void) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"conditional branches of type long and nullptr_t: the other branch of a nullptr_t branch must be a pointer or another nullptr_t (C23 \\u00a76.5.16p3)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; void *v; void (*fp)(void); nullptr_t n; int f(void) { return nullptr == nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"fp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([] Void))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; void *v; void (*fp)(void); nullptr_t n; int f(void) { return nullptr != nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(NotEqual Constant(ConstNullptr None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(NotEqual Constant(ConstNullptr None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(NotEqual Constant(ConstNullptr None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(NotEqual Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(NotEqual Constant(ConstNullptr None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(NotEqual Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"fp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([] Void))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; void *v; void (*fp)(void); nullptr_t n; int f(void) { return nullptr == 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"fp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([] Void))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; void *v; void (*fp)(void); nullptr_t n; int f(void) { return 0 == nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstInt(0) None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstInt(0) None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstInt(0) None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstInt(0) None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"fp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([] Void))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; void *v; void (*fp)(void); nullptr_t n; int f(void) { return nullptr == 0L; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Constant(ConstLong(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Constant(ConstLong(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Constant(ConstLong(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Cast(Pointer(Void) Constant(ConstLong(0) Long) Pointer(Void)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Constant(ConstLong(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Cast(Pointer(Void) Constant(ConstLong(0) Long) Pointer(Void)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"fp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([] Void))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; void *v; void (*fp)(void); nullptr_t n; int f(void) { return nullptr == false; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Constant(ConstBool(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Constant(ConstBool(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Constant(ConstBool(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Cast(Pointer(Void) Constant(ConstBool(0) Bool) Pointer(Void)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Constant(ConstBool(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Cast(Pointer(Void) Constant(ConstBool(0) Bool) Pointer(Void)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"fp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([] Void))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 19)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; void *v; void (*fp)(void); nullptr_t n; int f(void) { return nullptr == (void *)0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Cast(Pointer(Void) Constant(ConstInt(0) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 20)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Cast(Pointer(Void) Constant(ConstInt(0) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Cast(Pointer(Void) Constant(ConstInt(0) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 21)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Cast(Pointer(Void) Constant(ConstInt(0) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"fp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([] Void))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 22)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; void *v; void (*fp)(void); nullptr_t n; int f(void) { return p == nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"p\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 23)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"p\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"p\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 24)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Var(\\\"p\\\" Pointer(Int)) Cast(Pointer(Int) Constant(ConstNullptr NullptrT) Pointer(Int)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"p\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Var(\\\"p\\\" Pointer(Int)) Cast(Pointer(Int) Constant(ConstNullptr NullptrT) Pointer(Int)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"fp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([] Void))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 25)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; void *v; void (*fp)(void); nullptr_t n; int f(void) { return nullptr != p; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(NotEqual Constant(ConstNullptr None) Var(\"p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 26)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(NotEqual Constant(ConstNullptr None) Var(\"p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(NotEqual Constant(ConstNullptr None) Var(\"p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 27)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(NotEqual Cast(Pointer(Int) Constant(ConstNullptr NullptrT) Pointer(Int)) Var(\\\"p\\\" Pointer(Int)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(NotEqual Constant(ConstNullptr None) Var(\"p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(NotEqual Cast(Pointer(Int) Constant(ConstNullptr NullptrT) Pointer(Int)) Var(\\\"p\\\" Pointer(Int)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"fp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([] Void))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 28)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; void *v; void (*fp)(void); nullptr_t n; int f(void) { return fp == nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"fp\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 29)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"fp\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"fp\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 30)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Var(\\\"fp\\\" Pointer(FunType([] Void))) Cast(Pointer(FunType([] Void)) Constant(ConstNullptr NullptrT) Pointer(FunType([] Void))) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"fp\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Var(\\\"fp\\\" Pointer(FunType([] Void))) Cast(Pointer(FunType([] Void)) Constant(ConstNullptr NullptrT) Pointer(FunType([] Void))) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"fp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([] Void))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 31)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; void *v; void (*fp)(void); nullptr_t n; int f(void) { return n == nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"n\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 32)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"n\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"n\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 33)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Cast(Pointer(Void) Var(\\\"n\\\" NullptrT) Pointer(Void)) Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"n\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Cast(Pointer(Void) Var(\\\"n\\\" NullptrT) Pointer(Void)) Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"fp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([] Void))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 34)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; void *v; void (*fp)(void); nullptr_t n; int f(void) { return n == 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"n\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 35)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"n\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"n\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 36)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Cast(Pointer(Void) Var(\\\"n\\\" NullptrT) Pointer(Void)) Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"n\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Cast(Pointer(Void) Var(\\\"n\\\" NullptrT) Pointer(Void)) Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"fp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([] Void))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 37)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; void *v; void (*fp)(void); nullptr_t n; int f(void) { return n == p; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"n\" None) Var(\"p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 38)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"n\" None) Var(\"p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"n\" None) Var(\"p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 39)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Cast(Pointer(Int) Var(\\\"n\\\" NullptrT) Pointer(Int)) Var(\\\"p\\\" Pointer(Int)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"n\" None) Var(\"p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Cast(Pointer(Int) Var(\\\"n\\\" NullptrT) Pointer(Int)) Var(\\\"p\\\" Pointer(Int)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"fp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([] Void))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 40)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; void *v; void (*fp)(void); nullptr_t n; int f(void) { return v == n; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"v\" None) Var(\"n\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 41)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"v\" None) Var(\"n\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"v\" None) Var(\"n\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_equality_allowed (call 42)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_allowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Var(\\\"v\\\" Pointer(Void)) Cast(Pointer(Void) Var(\\\"n\\\" NullptrT) Pointer(Void)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" None Pointer(Void) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"v\" None) Var(\"n\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" None Pointer(Void) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" None Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Equal Var(\\\"v\\\" Pointer(Void)) Cast(Pointer(Void) Var(\\\"n\\\" NullptrT) Pointer(Void)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"fp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([] Void))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_equality_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint x; nullptr_t n; int f(void) { return nullptr == 1; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_equality_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_equality_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Constant(ConstNullptr None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '==' between nullptr_t and int: a nullptr_t compares only with another nullptr_t, a null pointer constant, or a pointer (C23 \\u00a76.5.10p2)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_equality_rejected (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint x; nullptr_t n; int f(void) { return x == nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"x\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_equality_rejected (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"x\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"x\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_equality_rejected (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"x\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '==' between int and nullptr_t: a nullptr_t compares only with another nullptr_t, a null pointer constant, or a pointer (C23 \\u00a76.5.10p2)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_equality_rejected (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint x; nullptr_t n; int f(void) { return nullptr != 0.0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(NotEqual Constant(ConstNullptr None) Constant(ConstDouble(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_equality_rejected (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(NotEqual Constant(ConstNullptr None) Constant(ConstDouble(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(NotEqual Constant(ConstNullptr None) Constant(ConstDouble(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_equality_rejected (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(NotEqual Constant(ConstNullptr None) Constant(ConstDouble(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '!=' between nullptr_t and double: a nullptr_t compares only with another nullptr_t, a null pointer constant, or a pointer (C23 \\u00a76.5.10p2)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_equality_rejected (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint x; nullptr_t n; int f(void) { return n == x; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"n\" None) Var(\"x\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_equality_rejected (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"n\" None) Var(\"x\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"n\" None) Var(\"x\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_equality_rejected (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_equality_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Equal Var(\"n\" None) Var(\"x\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '==' between nullptr_t and int: a nullptr_t compares only with another nullptr_t, a null pointer constant, or a pointer (C23 \\u00a76.5.10p2)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_not_an_integer_constant (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_not_an_integer_constant\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[nullptr];\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"array size must have integer type, and `nullptr` has type nullptr_t (C23 \\u00a76.7.7.3)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_not_an_integer_constant (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_not_an_integer_constant\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint x = !nullptr;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Unary(LogicalNot Constant(ConstNullptr None) None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_not_an_integer_constant (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_not_an_integer_constant\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Unary(LogicalNot Constant(ConstNullptr None) None) Int None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Unary(LogicalNot Constant(ConstNullptr None) None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_not_an_integer_constant (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_not_an_integer_constant\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Unary(LogicalNot Constant(ConstNullptr None) None) Int None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"initializer for static-storage object 'x' is not a constant expression: `nullptr` has type nullptr_t, which isn't an integer type, so it can't be an operand of an integer constant expression (C23 \\u00a76.6p8)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_relational_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_relational_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; nullptr_t n; int f(void) { return nullptr < nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(LessThan Constant(ConstNullptr None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_relational_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_relational_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(LessThan Constant(ConstNullptr None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(LessThan Constant(ConstNullptr None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_relational_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_relational_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(LessThan Constant(ConstNullptr None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '<' is not defined on nullptr_t: the relational operators compare real types or pointers to object types (C23 \\u00a76.5.9p2)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_relational_rejected (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_relational_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; nullptr_t n; int f(void) { return p > nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(GreaterThan Var(\"p\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_relational_rejected (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_relational_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(GreaterThan Var(\"p\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(GreaterThan Var(\"p\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_relational_rejected (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_relational_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(GreaterThan Var(\"p\" None) Constant(ConstNullptr None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '>' is not defined on nullptr_t: the relational operators compare real types or pointers to object types (C23 \\u00a76.5.9p2)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_relational_rejected (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_relational_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; nullptr_t n; int f(void) { return nullptr <= 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(LessOrEqual Constant(ConstNullptr None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_relational_rejected (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_relational_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(LessOrEqual Constant(ConstNullptr None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(LessOrEqual Constant(ConstNullptr None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_relational_rejected (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_relational_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(LessOrEqual Constant(ConstNullptr None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '<=' is not defined on nullptr_t: the relational operators compare real types or pointers to object types (C23 \\u00a76.5.9p2)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_relational_rejected (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_relational_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *p; nullptr_t n; int f(void) { return n >= p; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(GreaterOrEqual Var(\"n\" None) Var(\"p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_relational_rejected (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_relational_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(GreaterOrEqual Var(\"n\" None) Var(\"p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(GreaterOrEqual Var(\"n\" None) Var(\"p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_relational_rejected (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_relational_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(GreaterOrEqual Var(\"n\" None) Var(\"p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '>=' is not defined on nullptr_t: the relational operators compare real types or pointers to object types (C23 \\u00a76.5.9p2)\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestOperators::test_scalar_contexts (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_scalar_contexts\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint f(nullptr_t n) { if (nullptr) return 1; while (n) {} for (; nullptr;) {} do {} while (nullptr); return !nullptr + (nullptr && 1) + (0 || n) + (nullptr \077 1 : 2); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"n\"] Block([\n"
      "    S(IfStmt(Constant(ConstNullptr None) Return(Constant(ConstInt(1) None)) None))\n"
      "    S(WhileStmt(Var(\"n\" None) Compound(Block([])) \"\"))\n"
      "    S(ForStmt(InitExp(None) Constant(ConstNullptr None) None Compound(Block([])) \"\" None))\n"
      "    S(DoWhileStmt(Compound(Block([])) Constant(ConstNullptr None) \"\"))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Unary(LogicalNot Constant(ConstNullptr None) None) Binary(LogicalAnd Constant(ConstNullptr None) Constant(ConstInt(1) None) None) None) Binary(LogicalOr Constant(ConstInt(0) None) Var(\"n\" None) None) None) Conditional(Constant(ConstNullptr None) Constant(ConstInt(1) None) Constant(ConstInt(2) None) None) None)))\n"
      "  ]) FunType([NullptrT] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestOperators::test_scalar_contexts (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_scalar_contexts\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"n\"] Block([\n"
      "    S(IfStmt(Constant(ConstNullptr None) Return(Constant(ConstInt(1) None)) None))\n"
      "    S(WhileStmt(Var(\"n\" None) Compound(Block([])) \"\"))\n"
      "    S(ForStmt(InitExp(None) Constant(ConstNullptr None) None Compound(Block([])) \"\" None))\n"
      "    S(DoWhileStmt(Compound(Block([])) Constant(ConstNullptr None) \"\"))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Unary(LogicalNot Constant(ConstNullptr None) None) Binary(LogicalAnd Constant(ConstNullptr None) Constant(ConstInt(1) None) None) None) Binary(LogicalOr Constant(ConstInt(0) None) Var(\"n\" None) None) None) Conditional(Constant(ConstNullptr None) Constant(ConstInt(1) None) Constant(ConstInt(2) None) None) None)))\n"
      "  ]) FunType([NullptrT] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.n\"] Block([\n"
      "    S(IfStmt(Constant(ConstNullptr None) Return(Constant(ConstInt(1) None)) None))\n"
      "    S(WhileStmt(Var(\"@0.n\" None) Compound(Block([])) \"\"))\n"
      "    S(ForStmt(InitExp(None) Constant(ConstNullptr None) None Compound(Block([])) \"\" None))\n"
      "    S(DoWhileStmt(Compound(Block([])) Constant(ConstNullptr None) \"\"))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Unary(LogicalNot Constant(ConstNullptr None) None) Binary(LogicalAnd Constant(ConstNullptr None) Constant(ConstInt(1) None) None) None) Binary(LogicalOr Constant(ConstInt(0) None) Var(\"@0.n\" None) None) None) Conditional(Constant(ConstNullptr None) Constant(ConstInt(1) None) Constant(ConstInt(2) None) None) None)))\n"
      "  ]) FunType([NullptrT] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestOperators::test_scalar_contexts (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestOperators::test_scalar_contexts\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.n\\\"] Block([\\n    S(IfStmt(Constant(ConstNullptr NullptrT) Return(Constant(ConstInt(1) Int)) None))\\n    S(WhileStmt(Var(\\\"@0.n\\\" NullptrT) Compound(Block([])) \\\"\\\"))\\n    S(ForStmt(InitExp(None) Constant(ConstNullptr NullptrT) None Compound(Block([])) \\\"\\\" None))\\n    S(DoWhileStmt(Compound(Block([])) Constant(ConstNullptr NullptrT) \\\"\\\"))\\n    S(Return(Binary(Add Binary(Add Binary(Add Unary(LogicalNot Constant(ConstNullptr NullptrT) Int) Binary(LogicalAnd Constant(ConstNullptr NullptrT) Constant(ConstInt(1) Int) Int) Int) Binary(LogicalOr Constant(ConstInt(0) Int) Var(\\\"@0.n\\\" NullptrT) Int) Int) Conditional(Constant(ConstNullptr NullptrT) Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Int) Int)))\\n  ]) FunType([NullptrT] Int) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.n\"] Block([\n"
      "    S(IfStmt(Constant(ConstNullptr None) Return(Constant(ConstInt(1) None)) None))\n"
      "    S(WhileStmt(Var(\"@0.n\" None) Compound(Block([])) \"\"))\n"
      "    S(ForStmt(InitExp(None) Constant(ConstNullptr None) None Compound(Block([])) \"\" None))\n"
      "    S(DoWhileStmt(Compound(Block([])) Constant(ConstNullptr None) \"\"))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Unary(LogicalNot Constant(ConstNullptr None) None) Binary(LogicalAnd Constant(ConstNullptr None) Constant(ConstInt(1) None) None) None) Binary(LogicalOr Constant(ConstInt(0) None) Var(\"@0.n\" None) None) None) Conditional(Constant(ConstNullptr None) Constant(ConstInt(1) None) Constant(ConstInt(2) None) None) None)))\n"
      "  ]) FunType([NullptrT] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.n\\\"] Block([\\n    S(IfStmt(Constant(ConstNullptr NullptrT) Return(Constant(ConstInt(1) Int)) None))\\n    S(WhileStmt(Var(\\\"@0.n\\\" NullptrT) Compound(Block([])) \\\"\\\"))\\n    S(ForStmt(InitExp(None) Constant(ConstNullptr NullptrT) None Compound(Block([])) \\\"\\\" None))\\n    S(DoWhileStmt(Compound(Block([])) Constant(ConstNullptr NullptrT) \\\"\\\"))\\n    S(Return(Binary(Add Binary(Add Binary(Add Unary(LogicalNot Constant(ConstNullptr NullptrT) Int) Binary(LogicalAnd Constant(ConstNullptr NullptrT) Constant(ConstInt(1) Int) Int) Int) Binary(LogicalOr Constant(ConstInt(0) Int) Var(\\\"@0.n\\\" NullptrT) Int) Int) Conditional(Constant(ConstNullptr NullptrT) Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Int) Int)))\\n  ]) FunType([NullptrT] Int) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([NullptrT] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestStddef::test_includes_cleanly (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestStddef::test_includes_cleanly\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nstruct s { char a; int b; long c[3]; };\\nnullptr_t n = nullptr;\\nsize_t z = sizeof(int);\\nptrdiff_t d;\\nmax_align_t m;\\nwchar_t w;\\nint *p = ((void *)0);\\nsize_t o = ((size_t)&((struct s *)0)->c[1]);\\nint main(void) { ((void)0); return ((void *)0) == 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Char) member_decl(\"b\" Int) member_decl(\"c\" Array(Long 3))]))\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstNullptr None) NullptrT None None None None false))\n"
      "  VarDecl(var_decl(\"z\" SizeOfType(Int None) UInt None None None None false))\n"
      "  VarDecl(var_decl(\"d\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"m\" None Char None None None None false))\n"
      "  VarDecl(var_decl(\"w\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" Cast(Pointer(Void) Constant(ConstInt(0) None) None) Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"o\" Cast(UInt AddressOf(Subscript(Arrow(Cast(Pointer(Structure(\"s\")) Constant(ConstInt(0) None) None) \"c\" None) Constant(ConstInt(1) None) None) None) None) UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Cast(Void Constant(ConstInt(0) None) None)))\n"
      "    S(Return(Binary(Equal Cast(Pointer(Void) Constant(ConstInt(0) None) None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestStddef::test_includes_cleanly (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestStddef::test_includes_cleanly\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Char) member_decl(\"b\" Int) member_decl(\"c\" Array(Long 3))]))\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstNullptr None) NullptrT None None None None false))\n"
      "  VarDecl(var_decl(\"z\" SizeOfType(Int None) UInt None None None None false))\n"
      "  VarDecl(var_decl(\"d\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"m\" None Char None None None None false))\n"
      "  VarDecl(var_decl(\"w\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" Cast(Pointer(Void) Constant(ConstInt(0) None) None) Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"o\" Cast(UInt AddressOf(Subscript(Arrow(Cast(Pointer(Structure(\"s\")) Constant(ConstInt(0) None) None) \"c\" None) Constant(ConstInt(1) None) None) None) None) UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Cast(Void Constant(ConstInt(0) None) None)))\n"
      "    S(Return(Binary(Equal Cast(Pointer(Void) Constant(ConstInt(0) None) None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Char) member_decl(\"b\" Int) member_decl(\"c\" Array(Long 3))]))\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstNullptr None) NullptrT None None None None false))\n"
      "  VarDecl(var_decl(\"z\" SizeOfType(Int None) UInt None None None None false))\n"
      "  VarDecl(var_decl(\"d\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"m\" None Char None None None None false))\n"
      "  VarDecl(var_decl(\"w\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" Cast(Pointer(Void) Constant(ConstInt(0) None) None) Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"o\" Cast(UInt AddressOf(Subscript(Arrow(Cast(Pointer(Structure(\"s\")) Constant(ConstInt(0) None) None) \"c\" None) Constant(ConstInt(1) None) None) None) None) UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Cast(Void Constant(ConstInt(0) None) None)))\n"
      "    S(Return(Binary(Equal Cast(Pointer(Void) Constant(ConstInt(0) None) None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestStddef::test_includes_cleanly (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestStddef::test_includes_cleanly\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" Char) member_decl(\\\"b\\\" Int) member_decl(\\\"c\\\" Array(Long 3))]))\\n  VarDecl(var_decl(\\\"n\\\" Constant(ConstNullptr NullptrT) NullptrT None None None None false))\\n  VarDecl(var_decl(\\\"z\\\" SizeOfType(Int UInt) UInt None None None None false))\\n  VarDecl(var_decl(\\\"d\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"m\\\" None Char None None None None false))\\n  VarDecl(var_decl(\\\"w\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"p\\\" Cast(Pointer(Int) Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Pointer(Int)) Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"o\\\" Cast(UInt Constant(ConstUInt(7) UInt) UInt) UInt None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Cast(Void Constant(ConstInt(0) Int) Void)))\\n    S(Return(Binary(Equal Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Char) member_decl(\"b\" Int) member_decl(\"c\" Array(Long 3))]))\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstNullptr None) NullptrT None None None None false))\n"
      "  VarDecl(var_decl(\"z\" SizeOfType(Int None) UInt None None None None false))\n"
      "  VarDecl(var_decl(\"d\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"m\" None Char None None None None false))\n"
      "  VarDecl(var_decl(\"w\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" Cast(Pointer(Void) Constant(ConstInt(0) None) None) Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"o\" Cast(UInt AddressOf(Subscript(Arrow(Cast(Pointer(Structure(\"s\")) Constant(ConstInt(0) None) None) \"c\" None) Constant(ConstInt(1) None) None) None) None) UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Cast(Void Constant(ConstInt(0) None) None)))\n"
      "    S(Return(Binary(Equal Cast(Pointer(Void) Constant(ConstInt(0) None) None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" Char) member_decl(\\\"b\\\" Int) member_decl(\\\"c\\\" Array(Long 3))]))\\n  VarDecl(var_decl(\\\"n\\\" Constant(ConstNullptr NullptrT) NullptrT None None None None false))\\n  VarDecl(var_decl(\\\"z\\\" SizeOfType(Int UInt) UInt None None None None false))\\n  VarDecl(var_decl(\\\"d\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"m\\\" None Char None None None None false))\\n  VarDecl(var_decl(\\\"w\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"p\\\" Cast(Pointer(Int) Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Pointer(Int)) Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"o\\\" Cast(UInt Constant(ConstUInt(7) UInt) UInt) UInt None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Cast(Void Constant(ConstInt(0) Int) Void)))\\n    S(Return(Binary(Equal Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"z\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 2}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"m\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"w\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, "
      "\"mergeable\": false, \"was_bool\": false}}}}], [\"o\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 7}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 1}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"c\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Long 3)\\n\"}, \"byte_offset\": 3}}], \"size\": 15, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestStddef::test_null_compares_equal_to_zero_and_nullptr (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestStddef::test_null_compares_equal_to_zero_and_nullptr\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint f(void) { return ((void *)0) == 0 && ((void *)0) == nullptr; }\\nnullptr_t n = ((void *)0);\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(LogicalAnd Binary(Equal Cast(Pointer(Void) Constant(ConstInt(0) None) None) Constant(ConstInt(0) None) None) Binary(Equal Cast(Pointer(Void) Constant(ConstInt(0) None) None) Constant(ConstNullptr None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  VarDecl(var_decl(\"n\" Cast(Pointer(Void) Constant(ConstInt(0) None) None) NullptrT None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestStddef::test_null_compares_equal_to_zero_and_nullptr (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestStddef::test_null_compares_equal_to_zero_and_nullptr\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(LogicalAnd Binary(Equal Cast(Pointer(Void) Constant(ConstInt(0) None) None) Constant(ConstInt(0) None) None) Binary(Equal Cast(Pointer(Void) Constant(ConstInt(0) None) None) Constant(ConstNullptr None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  VarDecl(var_decl(\"n\" Cast(Pointer(Void) Constant(ConstInt(0) None) None) NullptrT None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(LogicalAnd Binary(Equal Cast(Pointer(Void) Constant(ConstInt(0) None) None) Constant(ConstInt(0) None) None) Binary(Equal Cast(Pointer(Void) Constant(ConstInt(0) None) None) Constant(ConstNullptr None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  VarDecl(var_decl(\"n\" Cast(Pointer(Void) Constant(ConstInt(0) None) None) NullptrT None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestStddef::test_null_compares_equal_to_zero_and_nullptr (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestStddef::test_null_compares_equal_to_zero_and_nullptr\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(LogicalAnd Binary(Equal Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Int) Binary(Equal Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  VarDecl(var_decl(\\\"n\\\" Cast(NullptrT Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) NullptrT) NullptrT None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(LogicalAnd Binary(Equal Cast(Pointer(Void) Constant(ConstInt(0) None) None) Constant(ConstInt(0) None) None) Binary(Equal Cast(Pointer(Void) Constant(ConstInt(0) None) None) Constant(ConstNullptr None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  VarDecl(var_decl(\"n\" Cast(Pointer(Void) Constant(ConstInt(0) None) None) NullptrT None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(LogicalAnd Binary(Equal Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Int) Binary(Equal Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void)) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  VarDecl(var_decl(\\\"n\\\" Cast(NullptrT Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) NullptrT) NullptrT None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestStddef::test_null_is_not_an_integer (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestStddef::test_null_is_not_an_integer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint x = ((void *)0);\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Cast(Pointer(Void) Constant(ConstInt(0) None) None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestStddef::test_null_is_not_an_integer (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestStddef::test_null_is_not_an_integer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Cast(Pointer(Void) Constant(ConstInt(0) None) None) Int None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Cast(Pointer(Void) Constant(ConstInt(0) None) None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestStddef::test_null_is_not_an_integer (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestStddef::test_null_is_not_an_integer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Cast(Pointer(Void) Constant(ConstInt(0) None) None) Int None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot implicitly convert pointer type Pointer(referenced_type=Void()) to arithmetic type Int(); use an explicit cast\"}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestStddef::test_offsetof_is_a_constant (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestStddef::test_offsetof_is_a_constant\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nstruct t { char x; int y; };\\nstruct s { char a; int b; struct t in; struct t arr[2]; };\\nunsigned char o1 = ((size_t)&((struct s *)0)->b);\\nint o2 = ((size_t)&((struct s *)0)->in.y);\\nlong o3 = ((size_t)&((struct s *)0)->arr[1].y);\\nint f(int x) { switch (x) { case ((size_t)&((struct s *)0)->in): return 1; } return 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"t\" false [member_decl(\"x\" Char) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Char) member_decl(\"b\" Int) member_decl(\"in\" Structure(\"t\")) member_decl(\"arr\" Array(Structure(\"t\") 2))]))\n"
      "  VarDecl(var_decl(\"o1\" Cast(UInt AddressOf(Arrow(Cast(Pointer(Structure(\"s\")) Constant(ConstInt(0) None) None) \"b\" None) None) None) UChar None None None None false))\n"
      "  VarDecl(var_decl(\"o2\" Cast(UInt AddressOf(Dot(Arrow(Cast(Pointer(Structure(\"s\")) Constant(ConstInt(0) None) None) \"in\" None) \"y\" None) None) None) Int None None None None false))\n"
      "  VarDecl(var_decl(\"o3\" Cast(UInt AddressOf(Dot(Subscript(Arrow(Cast(Pointer(Structure(\"s\")) Constant(ConstInt(0) None) None) \"arr\" None) Constant(ConstInt(1) None) None) \"y\" None) None) None) Long None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    S(SwitchStmt(Var(\"x\" None) Compound(Block([\n"
      "      S(CaseStmt(Cast(UInt AddressOf(Arrow(Cast(Pointer(Structure(\"s\")) Constant(ConstInt(0) None) None) \"in\" None) None) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestStddef::test_offsetof_is_a_constant (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestStddef::test_offsetof_is_a_constant\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"t\" false [member_decl(\"x\" Char) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Char) member_decl(\"b\" Int) member_decl(\"in\" Structure(\"t\")) member_decl(\"arr\" Array(Structure(\"t\") 2))]))\n"
      "  VarDecl(var_decl(\"o1\" Cast(UInt AddressOf(Arrow(Cast(Pointer(Structure(\"s\")) Constant(ConstInt(0) None) None) \"b\" None) None) None) UChar None None None None false))\n"
      "  VarDecl(var_decl(\"o2\" Cast(UInt AddressOf(Dot(Arrow(Cast(Pointer(Structure(\"s\")) Constant(ConstInt(0) None) None) \"in\" None) \"y\" None) None) None) Int None None None None false))\n"
      "  VarDecl(var_decl(\"o3\" Cast(UInt AddressOf(Dot(Subscript(Arrow(Cast(Pointer(Structure(\"s\")) Constant(ConstInt(0) None) None) \"arr\" None) Constant(ConstInt(1) None) None) \"y\" None) None) None) Long None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    S(SwitchStmt(Var(\"x\" None) Compound(Block([\n"
      "      S(CaseStmt(Cast(UInt AddressOf(Arrow(Cast(Pointer(Structure(\"s\")) Constant(ConstInt(0) None) None) \"in\" None) None) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"t\" false [member_decl(\"x\" Char) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Char) member_decl(\"b\" Int) member_decl(\"in\" Structure(\"t\")) member_decl(\"arr\" Array(Structure(\"t\") 2))]))\n"
      "  VarDecl(var_decl(\"o1\" Cast(UInt AddressOf(Arrow(Cast(Pointer(Structure(\"s\")) Constant(ConstInt(0) None) None) \"b\" None) None) None) UChar None None None None false))\n"
      "  VarDecl(var_decl(\"o2\" Cast(UInt AddressOf(Dot(Arrow(Cast(Pointer(Structure(\"s\")) Constant(ConstInt(0) None) None) \"in\" None) \"y\" None) None) None) Int None None None None false))\n"
      "  VarDecl(var_decl(\"o3\" Cast(UInt AddressOf(Dot(Subscript(Arrow(Cast(Pointer(Structure(\"s\")) Constant(ConstInt(0) None) None) \"arr\" None) Constant(ConstInt(1) None) None) \"y\" None) None) None) Long None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Cast(UInt AddressOf(Arrow(Cast(Pointer(Structure(\"s\")) Constant(ConstInt(0) None) None) \"in\" None) None) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestStddef::test_offsetof_is_a_constant (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestStddef::test_offsetof_is_a_constant\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"t\\\" false [member_decl(\\\"x\\\" Char) member_decl(\\\"y\\\" Int)]))\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" Char) member_decl(\\\"b\\\" Int) member_decl(\\\"in\\\" Structure(\\\"t\\\")) member_decl(\\\"arr\\\" Array(Structure(\\\"t\\\") 2))]))\\n  VarDecl(var_decl(\\\"o1\\\" Cast(UChar Cast(UInt Constant(ConstUInt(1) UInt) UInt) UChar) UChar None None None None false))\\n  VarDecl(var_decl(\\\"o2\\\" Cast(Int Cast(UInt Constant(ConstUInt(4) UInt) UInt) Int) Int None None None None false))\\n  VarDecl(var_decl(\\\"o3\\\" Cast(Long Cast(UInt Constant(ConstUInt(10) UInt) UInt) Long) Long None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.x\\\"] Block([\\n    S(SwitchStmt(Var(\\\"@0.x\\\" Int) Compound(Block([\\n      S(CaseStmt(Cast(UInt AddressOf(Arrow(Cast(Pointer(Structure(\\\"s\\\")) Constant(ConstInt(0) None) None) \\\"in\\\" None) None) None) Return(Constant(ConstInt(1) Int)) \\\"\\\"))\\n    ])) \\\"\\\" [] None Int))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"t\" false [member_decl(\"x\" Char) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Char) member_decl(\"b\" Int) member_decl(\"in\" Structure(\"t\")) member_decl(\"arr\" Array(Structure(\"t\") 2))]))\n"
      "  VarDecl(var_decl(\"o1\" Cast(UInt AddressOf(Arrow(Cast(Pointer(Structure(\"s\")) Constant(ConstInt(0) None) None) \"b\" None) None) None) UChar None None None None false))\n"
      "  VarDecl(var_decl(\"o2\" Cast(UInt AddressOf(Dot(Arrow(Cast(Pointer(Structure(\"s\")) Constant(ConstInt(0) None) None) \"in\" None) \"y\" None) None) None) Int None None None None false))\n"
      "  VarDecl(var_decl(\"o3\" Cast(UInt AddressOf(Dot(Subscript(Arrow(Cast(Pointer(Structure(\"s\")) Constant(ConstInt(0) None) None) \"arr\" None) Constant(ConstInt(1) None) None) \"y\" None) None) None) Long None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Cast(UInt AddressOf(Arrow(Cast(Pointer(Structure(\"s\")) Constant(ConstInt(0) None) None) \"in\" None) None) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"t\\\" false [member_decl(\\\"x\\\" Char) member_decl(\\\"y\\\" Int)]))\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" Char) member_decl(\\\"b\\\" Int) member_decl(\\\"in\\\" Structure(\\\"t\\\")) member_decl(\\\"arr\\\" Array(Structure(\\\"t\\\") 2))]))\\n  VarDecl(var_decl(\\\"o1\\\" Cast(UChar Cast(UInt Constant(ConstUInt(1) UInt) UInt) UChar) UChar None None None None false))\\n  VarDecl(var_decl(\\\"o2\\\" Cast(Int Cast(UInt Constant(ConstUInt(4) UInt) UInt) Int) Int None None None None false))\\n  VarDecl(var_decl(\\\"o3\\\" Cast(Long Cast(UInt Constant(ConstUInt(10) UInt) UInt) Long) Long None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.x\\\"] Block([\\n    S(SwitchStmt(Var(\\\"@0.x\\\" Int) Compound(Block([\\n      S(CaseStmt(Cast(UInt AddressOf(Arrow(Cast(Pointer(Structure(\\\"s\\\")) Constant(ConstInt(0) None) None) \\\"in\\\" None) None) None) Return(Constant(ConstInt(1) Int)) \\\"\\\"))\\n    ])) \\\"\\\" [] None Int))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"o1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"o2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 4}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"o3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 10}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"t\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"t\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"x\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"y\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 1}}], \"size\": 3, \"complete\": true}}], [\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fi"
      "elds\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 1}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"in\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Structure(\\\"t\\\")\\n\"}, \"byte_offset\": 3}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"arr\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Structure(\\\"t\\\") 2)\\n\"}, \"byte_offset\": 6}}], \"size\": 12, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestStddef::test_types_match_the_operators (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestStddef::test_types_match_the_operators\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint a[2];\\nvoid f(void) { sizeof(int); &a[1] - &a[0]; }\\nsize_t z; ptrdiff_t d; nullptr_t n;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 2) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(SizeOfType(Int None)))\n"
      "    S(Expression(Binary(Subtract AddressOf(Subscript(Var(\"a\" None) Constant(ConstInt(1) None) None) None) AddressOf(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "  VarDecl(var_decl(\"z\" None UInt None None None None false))\n"
      "  VarDecl(var_decl(\"d\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestStddef::test_types_match_the_operators (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestStddef::test_types_match_the_operators\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 2) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(SizeOfType(Int None)))\n"
      "    S(Expression(Binary(Subtract AddressOf(Subscript(Var(\"a\" None) Constant(ConstInt(1) None) None) None) AddressOf(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "  VarDecl(var_decl(\"z\" None UInt None None None None false))\n"
      "  VarDecl(var_decl(\"d\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 2) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(SizeOfType(Int None)))\n"
      "    S(Expression(Binary(Subtract AddressOf(Subscript(Var(\"a\" None) Constant(ConstInt(1) None) None) None) AddressOf(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "  VarDecl(var_decl(\"z\" None UInt None None None None false))\n"
      "  VarDecl(var_decl(\"d\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestStddef::test_types_match_the_operators (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestStddef::test_types_match_the_operators\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None Array(Int 2) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Expression(SizeOfType(Int UInt)))\\n    S(Expression(Binary(Subtract AddressOf(Subscript(AddressOf(Var(\\\"a\\\" Array(Int 2)) Pointer(Int)) Constant(ConstInt(1) Int) Int) Pointer(Int)) AddressOf(Subscript(AddressOf(Var(\\\"a\\\" Array(Int 2)) Pointer(Int)) Constant(ConstInt(0) Int) Int) Pointer(Int)) Int)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n  VarDecl(var_decl(\\\"z\\\" None UInt None None None None false))\\n  VarDecl(var_decl(\\\"d\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 2) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(SizeOfType(Int None)))\n"
      "    S(Expression(Binary(Subtract AddressOf(Subscript(Var(\"a\" None) Constant(ConstInt(1) None) None) None) AddressOf(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "  VarDecl(var_decl(\"z\" None UInt None None None None false))\n"
      "  VarDecl(var_decl(\"d\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"n\" None NullptrT None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None Array(Int 2) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Expression(SizeOfType(Int UInt)))\\n    S(Expression(Binary(Subtract AddressOf(Subscript(AddressOf(Var(\\\"a\\\" Array(Int 2)) Pointer(Int)) Constant(ConstInt(1) Int) Int) Pointer(Int)) AddressOf(Subscript(AddressOf(Var(\\\"a\\\" Array(Int 2)) Pointer(Int)) Constant(ConstInt(0) Int) Int) Pointer(Int)) Int)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n  VarDecl(var_decl(\\\"z\\\" None UInt None None None None false))\\n  VarDecl(var_decl(\\\"d\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None NullptrT None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"z\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}
