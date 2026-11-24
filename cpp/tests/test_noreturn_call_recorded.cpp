// The calls that the tests of tests/test_noreturn_call.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_noreturn_call.py::TestDeclarations::test_any_declaration_makes_it_noreturn (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_noreturn_call.py::TestDeclarations::test_any_declaration_makes_it_noreturn\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void die(void);\\n[[noreturn]] void die(void);\\nvoid die(void);\\nvoid stop(void);\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None true))\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"stop\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_noreturn_call.py::TestDeclarations::test_any_declaration_makes_it_noreturn (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_noreturn_call.py::TestDeclarations::test_any_declaration_makes_it_noreturn\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None true))\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"stop\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None true))\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"stop\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_noreturn_call.py::TestDeclarations::test_any_declaration_makes_it_noreturn (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_noreturn_call.py::TestDeclarations::test_any_declaration_makes_it_noreturn\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None true))\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"stop\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None true))\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"stop\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_noreturn_call.py::TestDeclarations::test_any_declaration_makes_it_noreturn (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_noreturn_call.py::TestDeclarations::test_any_declaration_makes_it_noreturn\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None true))\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"stop\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None true))\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"stop\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_noreturn_call.py::TestDeclarations::test_any_declaration_makes_it_noreturn (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_noreturn_call.py::TestDeclarations::test_any_declaration_makes_it_noreturn\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None true))\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"stop\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None true))\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"stop\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_noreturn_call.py::TestDeclarations::test_any_declaration_makes_it_noreturn (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_noreturn_call.py::TestDeclarations::test_any_declaration_makes_it_noreturn\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None true))\n"
      "  FunctionDecl(function_decl(\"die\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"stop\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"die\\\" [] None FunType([] Void) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"die\\\" [] None FunType([] Void) None None [] false None [] None true))\\n  FunctionDecl(function_decl(\\\"die\\\" [] None FunType([] Void) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"stop\\\" [] None FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"die\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": true}}}}], [\"stop\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_noreturn_call.py::TestDeclarations::test_parsed_on_declarations_and_definitions (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_noreturn_call.py::TestDeclarations::test_parsed_on_declarations_and_definitions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[noreturn]] void a(void);\\nvoid b [[_Noreturn]] (void);\\nvoid c(void);\\n[[noreturn]] void d(void) { for (;;) {} }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"a\" [] None FunType([] Void) None None [] false None [] None true))\n"
      "  FunctionDecl(function_decl(\"b\" [] None FunType([] Void) None None [] false None [] None true))\n"
      "  FunctionDecl(function_decl(\"c\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"d\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None Compound(Block([])) \"\" None))\n"
      "  ]) FunType([] Void) None None [] false None [] None true))\n"
      "])\n");
}
