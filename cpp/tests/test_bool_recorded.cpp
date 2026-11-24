// The calls that the tests of tests/test_bool.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_bool.py::TestParse::test_alternative_spelling_is_the_same_type (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestParse::test_alternative_spelling_is_the_same_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"_Bool a; bool b;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Bool None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_bool.py::TestParse::test_bool_type (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestParse::test_bool_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"bool f(bool b) { return b; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"b\"] Block([\n"
      "    S(Return(Var(\"b\" None)))\n"
      "  ]) FunType([Bool] Bool) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_bool.py::TestParse::test_rejects_other_specifiers (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestParse::test_rejects_other_specifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"unsigned bool b;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'bool' cannot combine with another type specifier; `bool` is a keyword in C23 (\\u00a76.4.1), so it can't be a name either\"}\n");
}

TEST_CASE("parse: test_bool.py::TestParse::test_rejects_other_specifiers (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestParse::test_rejects_other_specifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"bool int b;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'bool' cannot combine with another type specifier; `bool` is a keyword in C23 (\\u00a76.4.1), so it can't be a name either\"}\n");
}

TEST_CASE("parse: test_bool.py::TestParse::test_rejects_other_specifiers (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestParse::test_rejects_other_specifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long bool b;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'bool' cannot combine with another type specifier; `bool` is a keyword in C23 (\\u00a76.4.1), so it can't be a name either\"}\n");
}

TEST_CASE("parse: test_bool.py::TestParse::test_rejects_other_specifiers (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestParse::test_rejects_other_specifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"bool bool b;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'bool' cannot combine with another type specifier; `bool` is a keyword in C23 (\\u00a76.4.1), so it can't be a name either\"}\n");
}

TEST_CASE("parse: test_bool.py::TestParse::test_rejects_other_specifiers (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestParse::test_rejects_other_specifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"_Bool int b;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'_Bool' cannot combine with another type specifier\"}\n");
}

TEST_CASE("parse: test_bool.py::TestParse::test_rejects_other_specifiers (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestParse::test_rejects_other_specifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"bool _Bool b;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'bool' cannot combine with another type specifier; `bool` is a keyword in C23 (\\u00a76.4.1), so it can't be a name either\"}\n");
}

TEST_CASE("parse: test_bool.py::TestTypeCheck::test_carry_return_accepts_bool (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestTypeCheck::test_carry_return_accepts_bool\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::return_bool_in_carry]] bool f(int x) { return x; }struct r { unsigned char v; bool ok; };[[c6502::return_bool_in_carry]] struct r g(void) { return (struct r){ 1, 1 }; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([Int] Bool) None None [\"\"] true None [] None false))\n"
      "  StructDecl(struct_decl(\"r\" false [member_decl(\"v\" UChar) member_decl(\"ok\" Bool)]))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"\" Structure(\"r\") InitList([Constant(ConstInt(1) None) Constant(ConstInt(1) None)] None None) None None false)))\n"
      "  ]) FunType([] Structure(\"r\")) None None [] true None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestTypeCheck::test_carry_return_accepts_bool (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestTypeCheck::test_carry_return_accepts_bool\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([Int] Bool) None None [\"\"] true None [] None false))\n"
      "  StructDecl(struct_decl(\"r\" false [member_decl(\"v\" UChar) member_decl(\"ok\" Bool)]))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"\" Structure(\"r\") InitList([Constant(ConstInt(1) None) Constant(ConstInt(1) None)] None None) None None false)))\n"
      "  ]) FunType([] Structure(\"r\")) None None [] true None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([Int] Bool) None None [\"\"] true None [] None false))\n"
      "  StructDecl(struct_decl(\"r\" false [member_decl(\"v\" UChar) member_decl(\"ok\" Bool)]))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@1.compound_literal\" Structure(\"r\") InitList([Constant(ConstInt(1) None) Constant(ConstInt(1) None)] None None) None None false)))\n"
      "  ]) FunType([] Structure(\"r\")) None None [] true None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestTypeCheck::test_carry_return_accepts_bool (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestTypeCheck::test_carry_return_accepts_bool\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Cast(Bool Binary(NotEqual Var(\\\"@0.x\\\" Int) Constant(ConstInt(0) Int) Int) Bool)))\\n  ]) FunType([Int] Bool) None None [\\\"\\\"] true None [] None false))\\n  StructDecl(struct_decl(\\\"r\\\" false [member_decl(\\\"v\\\" UChar) member_decl(\\\"ok\\\" Bool)]))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(CompoundLiteral(\\\"@1.compound_literal\\\" Structure(\\\"r\\\") InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(Bool Constant(ConstInt(1) Int) Bool)] Structure(\\\"r\\\") None) Structure(\\\"r\\\") None false)))\\n  ]) FunType([] Structure(\\\"r\\\")) None None [] true None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([Int] Bool) None None [\"\"] true None [] None false))\n"
      "  StructDecl(struct_decl(\"r\" false [member_decl(\"v\" UChar) member_decl(\"ok\" Bool)]))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@1.compound_literal\" Structure(\"r\") InitList([Constant(ConstInt(1) None) Constant(ConstInt(1) None)] None None) None None false)))\n"
      "  ]) FunType([] Structure(\"r\")) None None [] true None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Cast(Bool Binary(NotEqual Var(\\\"@0.x\\\" Int) Constant(ConstInt(0) Int) Int) Bool)))\\n  ]) FunType([Int] Bool) None None [\\\"\\\"] true None [] None false))\\n  StructDecl(struct_decl(\\\"r\\\" false [member_decl(\\\"v\\\" UChar) member_decl(\\\"ok\\\" Bool)]))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(CompoundLiteral(\\\"@1.compound_literal\\\" Structure(\\\"r\\\") InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(Bool Constant(ConstInt(1) Int) Bool)] Structure(\\\"r\\\") None) Structure(\\\"r\\\") None false)))\\n  ]) FunType([] Structure(\\\"r\\\")) None None [] true None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Structure(\\\"r\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.compound_literal\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Structure(\\\"r\\\")\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"r\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"r\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"v\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"ok\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"byte_offset\": 1}}], \"size\": 2, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_bool.py::TestTypeCheck::test_pointer_converts_implicitly_only_to_bool (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestTypeCheck::test_pointer_converts_implicitly_only_to_bool\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int g; bool f(int *p) { bool b = p; return p; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\"] Block([\n"
      "    D(VarDecl(var_decl(\"b\" Var(\"p\" None) Bool None None None None false)))\n"
      "    S(Return(Var(\"p\" None)))\n"
      "  ]) FunType([Pointer(Int)] Bool) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestTypeCheck::test_pointer_converts_implicitly_only_to_bool (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestTypeCheck::test_pointer_converts_implicitly_only_to_bool\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\"] Block([\n"
      "    D(VarDecl(var_decl(\"b\" Var(\"p\" None) Bool None None None None false)))\n"
      "    S(Return(Var(\"p\" None)))\n"
      "  ]) FunType([Pointer(Int)] Bool) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    D(VarDecl(var_decl(\"@1.b\" Var(\"@0.p\" None) Bool None None None None false)))\n"
      "    S(Return(Var(\"@0.p\" None)))\n"
      "  ]) FunType([Pointer(Int)] Bool) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestTypeCheck::test_pointer_converts_implicitly_only_to_bool (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestTypeCheck::test_pointer_converts_implicitly_only_to_bool\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Cast(Bool Var(\\\"@0.p\\\" Pointer(Int)) Bool) Bool None None None None false)))\\n    S(Return(Cast(Bool Var(\\\"@0.p\\\" Pointer(Int)) Bool)))\\n  ]) FunType([Pointer(Int)] Bool) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    D(VarDecl(var_decl(\"@1.b\" Var(\"@0.p\" None) Bool None None None None false)))\n"
      "    S(Return(Var(\"@0.p\" None)))\n"
      "  ]) FunType([Pointer(Int)] Bool) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Cast(Bool Var(\\\"@0.p\\\" Pointer(Int)) Bool) Bool None None None None false)))\\n    S(Return(Cast(Bool Var(\\\"@0.p\\\" Pointer(Int)) Bool)))\\n  ]) FunType([Pointer(Int)] Bool) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Int)] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bool.py::TestTypeCheck::test_pointer_converts_implicitly_only_to_bool (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestTypeCheck::test_pointer_converts_implicitly_only_to_bool\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int *p) { unsigned char c = p; return c; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\"] Block([\n"
      "    D(VarDecl(var_decl(\"c\" Var(\"p\" None) UChar None None None None false)))\n"
      "    S(Return(Var(\"c\" None)))\n"
      "  ]) FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestTypeCheck::test_pointer_converts_implicitly_only_to_bool (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestTypeCheck::test_pointer_converts_implicitly_only_to_bool\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\"] Block([\n"
      "    D(VarDecl(var_decl(\"c\" Var(\"p\" None) UChar None None None None false)))\n"
      "    S(Return(Var(\"c\" None)))\n"
      "  ]) FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    D(VarDecl(var_decl(\"@1.c\" Var(\"@0.p\" None) UChar None None None None false)))\n"
      "    S(Return(Var(\"@1.c\" None)))\n"
      "  ]) FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestTypeCheck::test_pointer_converts_implicitly_only_to_bool (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestTypeCheck::test_pointer_converts_implicitly_only_to_bool\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    D(VarDecl(var_decl(\"@1.c\" Var(\"@0.p\" None) UChar None None None None false)))\n"
      "    S(Return(Var(\"@1.c\" None)))\n"
      "  ]) FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot implicitly convert pointer type Pointer(referenced_type=Int()) to arithmetic type UChar(); use an explicit cast\"}\n");
}

TEST_CASE("parse: test_bool.py::TestTypeCheck::test_sizeof_and_promotion (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestTypeCheck::test_sizeof_and_promotion\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"bool b; unsigned long n = sizeof(bool) + sizeof(b + b);\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" None Bool None None None None false))\n"
      "  VarDecl(var_decl(\"n\" Binary(Add SizeOfType(Bool None) SizeOfExp(Binary(Add Var(\"b\" None) Var(\"b\" None) None) None) None) ULong None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestTypeCheck::test_sizeof_and_promotion (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestTypeCheck::test_sizeof_and_promotion\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" None Bool None None None None false))\n"
      "  VarDecl(var_decl(\"n\" Binary(Add SizeOfType(Bool None) SizeOfExp(Binary(Add Var(\"b\" None) Var(\"b\" None) None) None) None) ULong None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" None Bool None None None None false))\n"
      "  VarDecl(var_decl(\"n\" Binary(Add SizeOfType(Bool None) SizeOfExp(Binary(Add Var(\"b\" None) Var(\"b\" None) None) None) None) ULong None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestTypeCheck::test_sizeof_and_promotion (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestTypeCheck::test_sizeof_and_promotion\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" None Bool None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" Cast(ULong Binary(Add SizeOfType(Bool UInt) SizeOfExp(Binary(Add Cast(Int Var(\\\"b\\\" Bool) Int) Cast(Int Var(\\\"b\\\" Bool) Int) Int) UInt) UInt) ULong) ULong None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" None Bool None None None None false))\n"
      "  VarDecl(var_decl(\"n\" Binary(Add SizeOfType(Bool None) SizeOfExp(Binary(Add Var(\"b\" None) Var(\"b\" None) None) None) None) ULong None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" None Bool None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" Cast(ULong Binary(Add SizeOfType(Bool UInt) SizeOfExp(Binary(Add Cast(Int Var(\\\"b\\\" Bool) Int) Cast(Int Var(\\\"b\\\" Bool) Int) Int) UInt) UInt) ULong) ULong None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 3}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int g; bool b = 0;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(0) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(0) None) Bool None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(0) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Constant(ConstInt(0) Int) Bool) Bool None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(0) None) Bool None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Constant(ConstInt(0) Int) Bool) Bool None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int g; bool b = 1;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(1) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(1) None) Bool None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(1) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Constant(ConstInt(1) Int) Bool) Bool None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(1) None) Bool None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Constant(ConstInt(1) Int) Bool) Bool None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int g; bool b = 256;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(256) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(256) None) Bool None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(256) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Constant(ConstInt(256) Int) Bool) Bool None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(256) None) Bool None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Constant(ConstInt(256) Int) Bool) Bool None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int g; bool b = -1;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Unary(Negate Constant(ConstInt(1) None) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Unary(Negate Constant(ConstInt(1) None) None) Bool None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Unary(Negate Constant(ConstInt(1) None) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Unary(Negate Constant(ConstInt(1) Int) Int) Bool) Bool None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Unary(Negate Constant(ConstInt(1) None) None) Bool None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Unary(Negate Constant(ConstInt(1) Int) Int) Bool) Bool None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int g; bool b = 0x10000L;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstLong(65536) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstLong(65536) None) Bool None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstLong(65536) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Constant(ConstLong(65536) Long) Bool) Bool None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstLong(65536) None) Bool None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Constant(ConstLong(65536) Long) Bool) Bool None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int g; bool b = 0.5;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstDouble(4602678819172646912) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstDouble(4602678819172646912) None) Bool None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstDouble(4602678819172646912) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Constant(ConstDouble(4602678819172646912) Double) Bool) Bool None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstDouble(4602678819172646912) None) Bool None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Constant(ConstDouble(4602678819172646912) Double) Bool) Bool None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 19)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int g; bool b = 0.0;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstDouble(0) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 20)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstDouble(0) None) Bool None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstDouble(0) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 21)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Constant(ConstDouble(0) Double) Bool) Bool None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstDouble(0) None) Bool None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Constant(ConstDouble(0) Double) Bool) Bool None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 22)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int g; bool b = -0.0;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Unary(Negate Constant(ConstDouble(0) None) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 23)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Unary(Negate Constant(ConstDouble(0) None) None) Bool None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Unary(Negate Constant(ConstDouble(0) None) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 24)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Unary(Negate Constant(ConstDouble(0) Double) Double) Bool) Bool None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Unary(Negate Constant(ConstDouble(0) None) None) Bool None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Unary(Negate Constant(ConstDouble(0) Double) Double) Bool) Bool None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 25)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int g; bool b = 0.25f;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstFloat(1048576000) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 26)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstFloat(1048576000) None) Bool None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstFloat(1048576000) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 27)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Constant(ConstFloat(1048576000) Float) Bool) Bool None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstFloat(1048576000) None) Bool None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Constant(ConstFloat(1048576000) Float) Bool) Bool None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 28)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int g; bool b = &g;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" AddressOf(Var(\"g\" None) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 29)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" AddressOf(Var(\"g\" None) None) Bool None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" AddressOf(Var(\"g\" None) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 30)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool AddressOf(Var(\\\"g\\\" Int) Pointer(Int)) Bool) Bool None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" AddressOf(Var(\"g\" None) None) Bool None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool AddressOf(Var(\\\"g\\\" Int) Pointer(Int)) Bool) Bool None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 31)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int g; bool b = (unsigned char)256;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Cast(UChar Constant(ConstInt(256) None) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 32)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Cast(UChar Constant(ConstInt(256) None) None) Bool None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Cast(UChar Constant(ConstInt(256) None) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 33)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Cast(UChar Constant(ConstInt(256) Int) UChar) Bool) Bool None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Cast(UChar Constant(ConstInt(256) None) None) Bool None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Cast(UChar Constant(ConstInt(256) Int) UChar) Bool) Bool None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 34)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int g; bool b = 2 > 1;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Binary(GreaterThan Constant(ConstInt(2) None) Constant(ConstInt(1) None) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 35)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Binary(GreaterThan Constant(ConstInt(2) None) Constant(ConstInt(1) None) None) Bool None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Binary(GreaterThan Constant(ConstInt(2) None) Constant(ConstInt(1) None) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 36)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Binary(GreaterThan Constant(ConstInt(2) Int) Constant(ConstInt(1) Int) Int) Bool) Bool None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Binary(GreaterThan Constant(ConstInt(2) None) Constant(ConstInt(1) None) None) Bool None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Binary(GreaterThan Constant(ConstInt(2) Int) Constant(ConstInt(1) Int) Int) Bool) Bool None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 37)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int g; bool b = (bool)2 + 1;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Binary(Add Cast(Bool Constant(ConstInt(2) None) None) Constant(ConstInt(1) None) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 38)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Binary(Add Cast(Bool Constant(ConstInt(2) None) None) Constant(ConstInt(1) None) None) Bool None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Binary(Add Cast(Bool Constant(ConstInt(2) None) None) Constant(ConstInt(1) None) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestTypeCheck::test_static_initializers_convert (call 39)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestTypeCheck::test_static_initializers_convert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Binary(Add Cast(Int Cast(Bool Constant(ConstInt(2) Int) Bool) Int) Constant(ConstInt(1) Int) Int) Bool) Bool None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Binary(Add Cast(Bool Constant(ConstInt(2) None) None) Constant(ConstInt(1) None) None) Bool None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Int None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Binary(Add Cast(Int Cast(Bool Constant(ConstInt(2) Int) Bool) Int) Constant(ConstInt(1) Int) Int) Bool) Bool None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}
