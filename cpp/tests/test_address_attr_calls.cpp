// The calls that the tests of tests/test_address_attr.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_address_attr.py::TestParse::test_function_and_object_forms (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_address_attr.py::TestParse::test_function_and_object_forms\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zp_abi, c6502::address(0xFDED)]]\\nextern void COUT(register unsigned char c);\\nextern unsigned char KBD [[c6502::address(0xC000)]];\\n[[c6502::address(0x36)]] extern unsigned int CSW;\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"COUT\" [\"c\"] None FunType([UChar] Void) Extern \"zp_abi\" [\"auto\"] false 65005 [] None false))\n"
      "  VarDecl(var_decl(\"KBD\" None UChar Extern None Absolute(49152) None false))\n"
      "  VarDecl(var_decl(\"CSW\" None UInt Extern None Absolute(54) None false))\n"
      "])\n");
}

TEST_CASE("parse: test_address_attr.py::TestParse::test_plain_function_has_no_address (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_address_attr.py::TestParse::test_plain_function_has_no_address\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(void);\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_address_attr.py::TestParse::test_rejected_forms (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_address_attr.py::TestParse::test_rejected_forms\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::address]] void f(void);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::address(...)]]` takes an integer address; saw none on function declaration 'f'\"}\n");
}

TEST_CASE("parse: test_address_attr.py::TestParse::test_rejected_forms (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_address_attr.py::TestParse::test_rejected_forms\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::address(\\\"x\\\")]] void f(void);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::address(...)]]` takes an integer address; saw a string on function declaration 'f'\"}\n");
}

TEST_CASE("parse: test_address_attr.py::TestParse::test_rejected_forms (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_address_attr.py::TestParse::test_rejected_forms\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::address(0x10000)]] void f(void);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"address $10000 on function declaration 'f' is outside $0000-$FFFF\"}\n");
}

TEST_CASE("parse: test_address_attr.py::TestParse::test_rejected_forms (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_address_attr.py::TestParse::test_rejected_forms\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::address(0x1000)]] void f(void) {}\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"function 'f' has `[[c6502::address(...)]]`, which names code that's already at that address, so it can't have a body\"}\n");
}

TEST_CASE("parse: test_address_attr.py::TestParse::test_rejected_forms (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_address_attr.py::TestParse::test_rejected_forms\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"extern unsigned char x [[c6502::zeropage(0x40), c6502::address(0x40)]];\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::zeropage]]` and `[[c6502::address(...)]]` both given on `x`\"}\n");
}

TEST_CASE("parse: test_address_attr.py::TestParse::test_rejected_forms (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_address_attr.py::TestParse::test_rejected_forms\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::address(0xC000)]] extern unsigned char x [[c6502::address(0xC000)]];\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::address` is given twice on object declaration `x`\"}\n");
}

TEST_CASE("parse: test_address_attr.py::TestParse::test_rejected_forms (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_address_attr.py::TestParse::test_rejected_forms\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::address(1), c6502::address(2)]] void f(void);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::address` is given twice on function declaration 'f'\"}\n");
}

TEST_CASE("parse: test_address_attr.py::TestTypeCheck::test_address_accumulates_across_declarations (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_address_attr.py::TestTypeCheck::test_address_accumulates_across_declarations\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(void);\\n[[c6502::address(0x1000)]] void f(void);\\nvoid f(void);\\nextern unsigned char x;\\nextern unsigned char x [[c6502::address(0xC000)]];\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false 4096 [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None None None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None Absolute(49152) None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_address_attr.py::TestTypeCheck::test_address_accumulates_across_declarations (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_address_attr.py::TestTypeCheck::test_address_accumulates_across_declarations\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false 4096 [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None None None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None Absolute(49152) None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false 4096 [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None None None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None Absolute(49152) None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_address_attr.py::TestTypeCheck::test_address_accumulates_across_declarations (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_address_attr.py::TestTypeCheck::test_address_accumulates_across_declarations\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false 4096 [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None None None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None Absolute(49152) None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false 4096 [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None None None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None Absolute(49152) None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_address_attr.py::TestTypeCheck::test_address_accumulates_across_declarations (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_address_attr.py::TestTypeCheck::test_address_accumulates_across_declarations\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false 4096 [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None None None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None Absolute(49152) None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false 4096 [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None None None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None Absolute(49152) None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_address_attr.py::TestTypeCheck::test_address_accumulates_across_declarations (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_address_attr.py::TestTypeCheck::test_address_accumulates_across_declarations\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false 4096 [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None None None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None Absolute(49152) None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false 4096 [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None None None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None Absolute(49152) None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_address_attr.py::TestTypeCheck::test_address_accumulates_across_declarations (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_address_attr.py::TestTypeCheck::test_address_accumulates_across_declarations\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false 4096 [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None None None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None Absolute(49152) None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false 4096 [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None None None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None Absolute(49152) None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_address_attr.py::TestTypeCheck::test_address_accumulates_across_declarations (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_address_attr.py::TestTypeCheck::test_address_accumulates_across_declarations\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false 4096 [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Void) None None [] false None [] None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None None None false))\n"
      "  VarDecl(var_decl(\"x\" None UChar Extern None Absolute(49152) None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] None FunType([] Void) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] None FunType([] Void) None None [] false 4096 [] None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] None FunType([] Void) None None [] false None [] None false))\\n  VarDecl(var_decl(\\\"x\\\" None UChar Extern None None None false))\\n  VarDecl(var_decl(\\\"x\\\" None UChar Extern None Absolute(49152) None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": 4096, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": 49152, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}
