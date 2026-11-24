// The calls that the tests of tests/test_bcd_builtins.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_constant_operands_make_a_constant_expression (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_constant_operands_make_a_constant_expression\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nstatic uint16_t s = __c6502_bcd_add(0x0999, 1);uint16_t t = __c6502_bcd_sub(0x1000, 0x0001);int a[__c6502_bcd_add(0x09, 0x01)];int f(uint8_t k) { switch (k) { case __c6502_bcd_add(0x19, 0x01): return 1; } return sizeof a; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" FunctionCall(\"__c6502_bcd_add\" [Constant(ConstInt(2457) None) Constant(ConstInt(1) None)] None false) UInt Static None None None false))\n"
      "  VarDecl(var_decl(\"t\" FunctionCall(\"__c6502_bcd_sub\" [Constant(ConstInt(4096) None) Constant(ConstInt(1) None)] None false) UInt None None None None false))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 16) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"k\"] Block([\n"
      "    S(SwitchStmt(Var(\"k\" None) Compound(Block([\n"
      "      S(CaseStmt(FunctionCall(\"__c6502_bcd_add\" [Constant(ConstInt(25) None) Constant(ConstInt(1) None)] None false) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(SizeOfExp(Var(\"a\" None) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_constant_operands_make_a_constant_expression (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_constant_operands_make_a_constant_expression\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" FunctionCall(\"__c6502_bcd_add\" [Constant(ConstInt(2457) None) Constant(ConstInt(1) None)] None false) UInt Static None None None false))\n"
      "  VarDecl(var_decl(\"t\" FunctionCall(\"__c6502_bcd_sub\" [Constant(ConstInt(4096) None) Constant(ConstInt(1) None)] None false) UInt None None None None false))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 16) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"k\"] Block([\n"
      "    S(SwitchStmt(Var(\"k\" None) Compound(Block([\n"
      "      S(CaseStmt(FunctionCall(\"__c6502_bcd_add\" [Constant(ConstInt(25) None) Constant(ConstInt(1) None)] None false) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(SizeOfExp(Var(\"a\" None) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" FunctionCall(\"__c6502_bcd_add\" [Constant(ConstInt(2457) None) Constant(ConstInt(1) None)] None false) UInt Static None None None false))\n"
      "  VarDecl(var_decl(\"t\" FunctionCall(\"__c6502_bcd_sub\" [Constant(ConstInt(4096) None) Constant(ConstInt(1) None)] None false) UInt None None None None false))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 16) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.k\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.k\" None) Compound(Block([\n"
      "      S(CaseStmt(FunctionCall(\"__c6502_bcd_add\" [Constant(ConstInt(25) None) Constant(ConstInt(1) None)] None false) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(SizeOfExp(Var(\"a\" None) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestFrontEnd::test_constant_operands_make_a_constant_expression (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_constant_operands_make_a_constant_expression\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"s\\\" FunctionCall(\\\"__c6502_bcd_add\\\" [Constant(ConstUInt(2457) UInt) Constant(ConstUInt(1) UInt)] UInt false) UInt Static None None None false))\\n  VarDecl(var_decl(\\\"t\\\" FunctionCall(\\\"__c6502_bcd_sub\\\" [Constant(ConstUInt(4096) UInt) Constant(ConstUInt(1) UInt)] UInt false) UInt None None None None false))\\n  VarDecl(var_decl(\\\"a\\\" None Array(Int 16) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.k\\\"] Block([\\n    S(SwitchStmt(Cast(Int Var(\\\"@0.k\\\" UChar) Int) Compound(Block([\\n      S(CaseStmt(FunctionCall(\\\"__c6502_bcd_add\\\" [Constant(ConstInt(25) None) Constant(ConstInt(1) None)] None false) Return(Constant(ConstInt(1) Int)) \\\"\\\"))\\n    ])) \\\"\\\" [] None Int))\\n    S(Return(Cast(Int SizeOfExp(Var(\\\"a\\\" Array(Int 16)) UInt) Int)))\\n  ]) FunType([UChar] Int) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" FunctionCall(\"__c6502_bcd_add\" [Constant(ConstInt(2457) None) Constant(ConstInt(1) None)] None false) UInt Static None None None false))\n"
      "  VarDecl(var_decl(\"t\" FunctionCall(\"__c6502_bcd_sub\" [Constant(ConstInt(4096) None) Constant(ConstInt(1) None)] None false) UInt None None None None false))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 16) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.k\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.k\" None) Compound(Block([\n"
      "      S(CaseStmt(FunctionCall(\"__c6502_bcd_add\" [Constant(ConstInt(25) None) Constant(ConstInt(1) None)] None false) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(SizeOfExp(Var(\"a\" None) None)))\n"
      "  ]) FunType([UChar] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"s\\\" FunctionCall(\\\"__c6502_bcd_add\\\" [Constant(ConstUInt(2457) UInt) Constant(ConstUInt(1) UInt)] UInt false) UInt Static None None None false))\\n  VarDecl(var_decl(\\\"t\\\" FunctionCall(\\\"__c6502_bcd_sub\\\" [Constant(ConstUInt(4096) UInt) Constant(ConstUInt(1) UInt)] UInt false) UInt None None None None false))\\n  VarDecl(var_decl(\\\"a\\\" None Array(Int 16) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.k\\\"] Block([\\n    S(SwitchStmt(Cast(Int Var(\\\"@0.k\\\" UChar) Int) Compound(Block([\\n      S(CaseStmt(FunctionCall(\\\"__c6502_bcd_add\\\" [Constant(ConstInt(25) None) Constant(ConstInt(1) None)] None false) Return(Constant(ConstInt(1) Int)) \\\"\\\"))\\n    ])) \\\"\\\" [] None Int))\\n    S(Return(Cast(Int SizeOfExp(Var(\\\"a\\\" Array(Int 16)) UInt) Int)))\\n  ]) FunType([UChar] Int) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 4096}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 2457}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 16)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.k\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_names_are_reserved (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_names_are_reserved\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int __c6502_bcd_add(int a, int b);\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"__c6502_bcd_add\" [\"a\" \"b\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_names_are_reserved (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_names_are_reserved\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"__c6502_bcd_add\" [\"a\" \"b\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"__c6502_bcd_add is a builtin and can't be declared\"}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_names_are_reserved (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_names_are_reserved\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(void) { int __c6502_bcd_sub = 1; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"__c6502_bcd_sub\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_names_are_reserved (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_names_are_reserved\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"__c6502_bcd_sub\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"__c6502_bcd_sub is a builtin and can't be declared\"}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_names_are_reserved (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_names_are_reserved\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(int __c6502_bcd_add) { }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"__c6502_bcd_add\"] Block([]) FunType([Int] Void) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_names_are_reserved (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_names_are_reserved\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"__c6502_bcd_add\"] Block([]) FunType([Int] Void) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"__c6502_bcd_add is a builtin and can't be declared\"}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_names_are_reserved (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_names_are_reserved\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void *p = __c6502_bcd_add;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" Var(\"__c6502_bcd_add\" None) Pointer(Void) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_names_are_reserved (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_names_are_reserved\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" Var(\"__c6502_bcd_add\" None) Pointer(Void) None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"__c6502_bcd_add is a builtin, which can only be called\"}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_names_are_reserved (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_names_are_reserved\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void) { return __c6502_bcd_add != 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(NotEqual Var(\"__c6502_bcd_add\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_names_are_reserved (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_names_are_reserved\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(NotEqual Var(\"__c6502_bcd_add\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"__c6502_bcd_add is a builtin, which can only be called\"}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nuint8_t a, b; int f(void) { return 0; } unsigned long long g(void) { return __c6502_bcd_add(a, b); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None UChar None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(ULongLong FunctionCall(\\\"__c6502_bcd_add\\\" [Var(\\\"a\\\" UChar) Var(\\\"b\\\" UChar)] UChar false) ULongLong)))\\n  ]) FunType([] ULongLong) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None UChar None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(ULongLong FunctionCall(\\\"__c6502_bcd_add\\\" [Var(\\\"a\\\" UChar) Var(\\\"b\\\" UChar)] UChar false) ULongLong)))\\n  ]) FunType([] ULongLong) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] ULongLong)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nuint8_t a; uint16_t b; int f(void) { return 0; } unsigned long long g(void) { return __c6502_bcd_add(a, b); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None UChar None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" None UInt None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(ULongLong FunctionCall(\\\"__c6502_bcd_add\\\" [Cast(UInt Var(\\\"a\\\" UChar) UInt) Var(\\\"b\\\" UInt)] UInt false) ULongLong)))\\n  ]) FunType([] ULongLong) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None UChar None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" None UInt None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(ULongLong FunctionCall(\\\"__c6502_bcd_add\\\" [Cast(UInt Var(\\\"a\\\" UChar) UInt) Var(\\\"b\\\" UInt)] UInt false) ULongLong)))\\n  ]) FunType([] ULongLong) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] ULongLong)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nunsigned short a; unsigned int b; int f(void) { return 0; } unsigned long long g(void) { return __c6502_bcd_add(a, b); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UShort None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UShort None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UShort None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None UShort None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" None UInt None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(ULongLong FunctionCall(\\\"__c6502_bcd_add\\\" [Cast(UInt Var(\\\"a\\\" UShort) UInt) Var(\\\"b\\\" UInt)] UInt false) ULongLong)))\\n  ]) FunType([] ULongLong) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UShort None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None UShort None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" None UInt None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(ULongLong FunctionCall(\\\"__c6502_bcd_add\\\" [Cast(UInt Var(\\\"a\\\" UShort) UInt) Var(\\\"b\\\" UInt)] UInt false) ULongLong)))\\n  ]) FunType([] ULongLong) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UShort\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] ULongLong)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nuint32_t a; int f(void) { return 0; } unsigned long long g(void) { return __c6502_bcd_add(a, 0x0250); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None ULong None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(592) None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None ULong None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(592) None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None ULong None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(592) None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None ULong None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(ULongLong FunctionCall(\\\"__c6502_bcd_add\\\" [Var(\\\"a\\\" ULong) Constant(ConstULong(592) ULong)] ULong false) ULongLong)))\\n  ]) FunType([] ULongLong) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None ULong None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(592) None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None ULong None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(ULongLong FunctionCall(\\\"__c6502_bcd_add\\\" [Var(\\\"a\\\" ULong) Constant(ConstULong(592) ULong)] ULong false) ULongLong)))\\n  ]) FunType([] ULongLong) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] ULongLong)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nuint8_t a; int f(void) { return 0; } unsigned long long g(void) { return __c6502_bcd_add(0x75, a); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Constant(ConstInt(117) None) Var(\"a\" None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Constant(ConstInt(117) None) Var(\"a\" None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Constant(ConstInt(117) None) Var(\"a\" None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(ULongLong FunctionCall(\\\"__c6502_bcd_add\\\" [Constant(ConstUChar(117) UChar) Var(\\\"a\\\" UChar)] UChar false) ULongLong)))\\n  ]) FunType([] ULongLong) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Constant(ConstInt(117) None) Var(\"a\" None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(ULongLong FunctionCall(\\\"__c6502_bcd_add\\\" [Constant(ConstUChar(117) UChar) Var(\\\"a\\\" UChar)] UChar false) ULongLong)))\\n  ]) FunType([] ULongLong) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] ULongLong)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nuint8_t a; int f(void) { return 0; } unsigned long long g(void) { return __c6502_bcd_add(a, 0x75u); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstUInt(117) None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstUInt(117) None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstUInt(117) None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(ULongLong FunctionCall(\\\"__c6502_bcd_add\\\" [Cast(UInt Var(\\\"a\\\" UChar) UInt) Constant(ConstUInt(117) UInt)] UInt false) ULongLong)))\\n  ]) FunType([] ULongLong) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstUInt(117) None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(ULongLong FunctionCall(\\\"__c6502_bcd_add\\\" [Cast(UInt Var(\\\"a\\\" UChar) UInt) Constant(ConstUInt(117) UInt)] UInt false) ULongLong)))\\n  ]) FunType([] ULongLong) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] ULongLong)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 19)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\n int f(void) { return 0; } unsigned long long g(void) { return __c6502_bcd_add(0x99, 1); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Constant(ConstInt(153) None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 20)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Constant(ConstInt(153) None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Constant(ConstInt(153) None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 21)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(ULongLong FunctionCall(\\\"__c6502_bcd_add\\\" [Constant(ConstUInt(153) UInt) Constant(ConstUInt(1) UInt)] UInt false) ULongLong)))\\n  ]) FunType([] ULongLong) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Constant(ConstInt(153) None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(ULongLong FunctionCall(\\\"__c6502_bcd_add\\\" [Constant(ConstUInt(153) UInt) Constant(ConstUInt(1) UInt)] UInt false) ULongLong)))\\n  ]) FunType([] ULongLong) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] ULongLong)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 22)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\n int f(void) { return 0; } unsigned long long g(void) { return __c6502_bcd_add(0x99, 0x10000); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Constant(ConstInt(153) None) Constant(ConstLong(65536) None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 23)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Constant(ConstInt(153) None) Constant(ConstLong(65536) None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Constant(ConstInt(153) None) Constant(ConstLong(65536) None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 24)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(ULongLong FunctionCall(\\\"__c6502_bcd_add\\\" [Constant(ConstULong(153) ULong) Constant(ConstULong(65536) ULong)] ULong false) ULongLong)))\\n  ]) FunType([] ULongLong) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Constant(ConstInt(153) None) Constant(ConstLong(65536) None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(ULongLong FunctionCall(\\\"__c6502_bcd_add\\\" [Constant(ConstULong(153) ULong) Constant(ConstULong(65536) ULong)] ULong false) ULongLong)))\\n  ]) FunType([] ULongLong) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] ULongLong)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 25)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nuint64_t a; int f(void) { return 0; } unsigned long long g(void) { return __c6502_bcd_add(a, 1); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None ULongLong None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 26)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None ULongLong None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None ULongLong None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestFrontEnd::test_result_types (call 27)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_result_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None ULongLong None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(FunctionCall(\\\"__c6502_bcd_add\\\" [Var(\\\"a\\\" ULongLong) Constant(ConstULongLong(1) ULongLong)] ULongLong false)))\\n  ]) FunType([] ULongLong) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None ULongLong None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None ULongLong None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(FunctionCall(\\\"__c6502_bcd_add\\\" [Var(\\\"a\\\" ULongLong) Constant(ConstULongLong(1) ULongLong)] ULongLong false)))\\n  ]) FunType([] ULongLong) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] ULongLong)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_save_hardware_stack_ignores_the_builtin (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_save_hardware_stack_ignores_the_builtin\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nuint8_t a;void f(void) { [[c6502::save_hardware_stack]] a = __c6502_bcd_add(a, 1); }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::save_hardware_stack]]` must come before a statement or declaration that calls a function\"}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nint8_t a; void f(void) { __c6502_bcd_add(a, a); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None SChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Var(\"a\" None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None SChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Var(\"a\" None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None SChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Var(\"a\" None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None SChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Var(\"a\" None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"argument 1 of __c6502_bcd_add has the signed type signed char; packed BCD operands must have an unsigned integer type (cast it, or give a constant a `u` suffix)\"}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nint a; void f(void) { __c6502_bcd_sub(1, a); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_sub\" [Constant(ConstInt(1) None) Var(\"a\" None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_sub\" [Constant(ConstInt(1) None) Var(\"a\" None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_sub\" [Constant(ConstInt(1) None) Var(\"a\" None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_sub\" [Constant(ConstInt(1) None) Var(\"a\" None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"argument 2 of __c6502_bcd_sub has the signed type int; packed BCD operands must have an unsigned integer type (cast it, or give a constant a `u` suffix)\"}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\ndouble d; void f(void) { __c6502_bcd_add(d, 1); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"d\" None Double None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"d\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"d\" None Double None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"d\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"d\" None Double None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"d\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"d\" None Double None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"d\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"argument 1 of __c6502_bcd_add has floating type double; packed BCD operands must have an unsigned integer type\"}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nuint8_t *p; void f(void) { __c6502_bcd_add(p, 1); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"p\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"p\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"p\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"p\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"argument 1 of __c6502_bcd_add is a pointer (unsigned char *); packed BCD operands must have an unsigned integer type\"}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nuint8_t t[2]; void f(void) { __c6502_bcd_add(t, 1); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" None Array(UChar 2) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"t\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" None Array(UChar 2) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"t\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" None Array(UChar 2) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"t\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" None Array(UChar 2) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"t\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"argument 1 of __c6502_bcd_add is a pointer (unsigned char *); packed BCD operands must have an unsigned integer type\"}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nstruct s { int x; } v; void f(void) { __c6502_bcd_add(v, 1); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"x\" Int)]))\n"
      "  VarDecl(var_decl(\"v\" None Structure(\"s\") None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"v\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"x\" Int)]))\n"
      "  VarDecl(var_decl(\"v\" None Structure(\"s\") None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"v\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"x\" Int)]))\n"
      "  VarDecl(var_decl(\"v\" None Structure(\"s\") None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"v\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"x\" Int)]))\n"
      "  VarDecl(var_decl(\"v\" None Structure(\"s\") None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"v\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"argument 1 of __c6502_bcd_add has type struct s; packed BCD operands must have an unsigned integer type\"}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 19)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nbool b; void f(void) { __c6502_bcd_add(b, 1); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" None Bool None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"b\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 20)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" None Bool None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"b\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" None Bool None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"b\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 21)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" None Bool None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"b\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"argument 1 of __c6502_bcd_add has type bool; packed BCD operands must have an unsigned integer type other than bool and plain char (use unsigned char)\"}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 22)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nchar c; void f(void) { __c6502_bcd_add(c, 1); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None Char None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"c\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 23)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None Char None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"c\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None Char None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"c\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 24)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" None Char None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"c\" None) Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"argument 1 of __c6502_bcd_add has type char; packed BCD operands must have an unsigned integer type other than bool and plain char (use unsigned char)\"}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 25)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nuint8_t a; void f(void) { __c6502_bcd_add(a, -1); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Unary(Negate Constant(ConstInt(1) None) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 26)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Unary(Negate Constant(ConstInt(1) None) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Unary(Negate Constant(ConstInt(1) None) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 27)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Unary(Negate Constant(ConstInt(1) None) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"argument 2 of __c6502_bcd_add is the negative constant -1; packed BCD operands are unsigned\"}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 28)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nuint8_t a; void f(void) { __c6502_bcd_add(a, 0x100); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(256) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 29)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(256) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(256) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 30)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(256) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"argument 2 of __c6502_bcd_add, 0x100, doesn't fit in unsigned char, the type of the result (cast the other operand to widen it)\"}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 31)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nuint8_t a; void f(void) { __c6502_bcd_add(a); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 32)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 33)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"__c6502_bcd_add takes 2 arguments, got 1\"}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 34)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nuint8_t a; void f(void) { __c6502_bcd_sub(a, a, a); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_sub\" [Var(\"a\" None) Var(\"a\" None) Var(\"a\" None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 35)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_sub\" [Var(\"a\" None) Var(\"a\" None) Var(\"a\" None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_sub\" [Var(\"a\" None) Var(\"a\" None) Var(\"a\" None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestFrontEnd::test_type_errors (call 36)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestFrontEnd::test_type_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(FunctionCall(\"__c6502_bcd_sub\" [Var(\"a\" None) Var(\"a\" None) Var(\"a\" None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"__c6502_bcd_sub takes 2 arguments, got 3\"}\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestTac::test_lowers_to_a_binary (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestTac::test_lowers_to_a_binary\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nuint16_t a, b;uint16_t f(void) { return __c6502_bcd_sub(a, b); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UInt None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_sub\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] UInt) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestTac::test_lowers_to_a_binary (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestTac::test_lowers_to_a_binary\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UInt None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_sub\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] UInt) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UInt None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_sub\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] UInt) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_bcd_builtins.py::TestTac::test_lowers_to_a_binary (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_bcd_builtins.py::TestTac::test_lowers_to_a_binary\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UInt None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_sub\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] UInt) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UInt None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_sub\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] UInt) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_bcd_builtins.py::TestTac::test_lowers_to_a_binary (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_bcd_builtins.py::TestTac::test_lowers_to_a_binary\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UInt None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_sub\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] UInt) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UInt None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_sub\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] UInt) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_bcd_builtins.py::TestTac::test_lowers_to_a_binary (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_bcd_builtins.py::TestTac::test_lowers_to_a_binary\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UInt None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_sub\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] UInt) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UInt None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_sub\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] UInt) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestTac::test_lowers_to_a_binary (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestTac::test_lowers_to_a_binary\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None UInt None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" None UInt None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(FunctionCall(\\\"__c6502_bcd_sub\\\" [Var(\\\"a\\\" UInt) Var(\\\"b\\\" UInt)] UInt false)))\\n  ]) FunType([] UInt) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UInt None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_sub\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] UInt) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None UInt None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" None UInt None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(FunctionCall(\\\"__c6502_bcd_sub\\\" [Var(\\\"a\\\" UInt) Var(\\\"b\\\" UInt)] UInt false)))\\n  ]) FunType([] UInt) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] UInt)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_bcd_builtins.py::TestTac::test_lowers_to_a_binary (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_bcd_builtins.py::TestTac::test_lowers_to_a_binary\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] UInt)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] UInt)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UInt None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_sub\" [Var(\"a\" UInt) Var(\"b\" UInt)] UInt false)))\n"
      "  ]) FunType([] UInt) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Binary(BcdSubtract Var(\"a\") Var(\"b\") Var(\"%0\"))\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "  StaticVariable(\"a\" true UInt [ZeroInit(2)])\n"
      "  StaticVariable(\"b\" true UInt [ZeroInit(2)])\n"
      "])\n");
}

TEST_CASE("unroll_program: test_bcd_builtins.py::TestTac::test_lowers_to_a_binary (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_bcd_builtins.py::TestTac::test_lowers_to_a_binary\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UInt None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_sub\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] UInt) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UInt None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None UInt None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(FunctionCall(\"__c6502_bcd_sub\" [Var(\"a\" None) Var(\"b\" None)] None false)))\n"
      "  ]) FunType([] UInt) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("inline_functions: test_bcd_builtins.py::TestTac::test_lowers_to_a_binary (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass inline_functions\n"
      "source tests/test_bcd_builtins.py::TestTac::test_lowers_to_a_binary\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] UInt)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None UInt None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" None UInt None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(FunctionCall(\\\"__c6502_bcd_sub\\\" [Var(\\\"a\\\" UInt) Var(\\\"b\\\" UInt)] UInt false)))\\n  ]) FunType([] UInt) None None [] false None [] None false))\\n])\\n\"}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Binary(BcdSubtract Var(\"a\") Var(\"b\") Var(\"%0\"))\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "  StaticVariable(\"a\" true UInt [ZeroInit(2)])\n"
      "  StaticVariable(\"b\" true UInt [ZeroInit(2)])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Binary(BcdSubtract Var(\"a\") Var(\"b\") Var(\"%0\"))\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "  StaticVariable(\"a\" true UInt [ZeroInit(2)])\n"
      "  StaticVariable(\"b\" true UInt [ZeroInit(2)])\n"
      "])\n");
}

TEST_CASE("return_pointers_in_zero_page: test_bcd_builtins.py::TestTac::test_lowers_to_a_binary (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass return_pointers_in_zero_page\n"
      "source tests/test_bcd_builtins.py::TestTac::test_lowers_to_a_binary\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] UInt)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Binary(BcdSubtract Var(\"a\") Var(\"b\") Var(\"%0\"))\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "  StaticVariable(\"a\" true UInt [ZeroInit(2)])\n"
      "  StaticVariable(\"b\" true UInt [ZeroInit(2)])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Binary(BcdSubtract Var(\"a\") Var(\"b\") Var(\"%0\"))\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "  StaticVariable(\"a\" true UInt [ZeroInit(2)])\n"
      "  StaticVariable(\"b\" true UInt [ZeroInit(2)])\n"
      "])\n");
}

TEST_CASE("optimize_tac: test_bcd_builtins.py::TestTac::test_lowers_to_a_binary (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass optimize_tac\n"
      "source tests/test_bcd_builtins.py::TestTac::test_lowers_to_a_binary\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] UInt)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] UInt)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Binary(BcdSubtract Var(\"a\") Var(\"b\") Var(\"%0\"))\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "  StaticVariable(\"a\" true UInt [ZeroInit(2)])\n"
      "  StaticVariable(\"b\" true UInt [ZeroInit(2)])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Binary(BcdSubtract Var(\"a\") Var(\"b\") Var(\"%0.1\"))\n"
      "    Ret(Var(\"%0.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"a\" true UInt [ZeroInit(2)])\n"
      "  StaticVariable(\"b\" true UInt [ZeroInit(2)])\n"
      "])\n");
}

TEST_CASE("parse: test_bcd_builtins.py::TestTac::test_unused_result_goes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bcd_builtins.py::TestTac::test_unused_result_goes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\nuint8_t a;void f(void) { (void)__c6502_bcd_add(a, 1); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Void FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(1) None)] None false) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_bcd_builtins.py::TestTac::test_unused_result_goes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_bcd_builtins.py::TestTac::test_unused_result_goes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Void FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(1) None)] None false) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Void FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(1) None)] None false) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bcd_builtins.py::TestTac::test_unused_result_goes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bcd_builtins.py::TestTac::test_unused_result_goes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Void FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(1) None)] None false) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Void FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(1) None)] None false) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_bcd_builtins.py::TestTac::test_unused_result_goes (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_bcd_builtins.py::TestTac::test_unused_result_goes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Void FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(1) None)] None false) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Void FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(1) None)] None false) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_bcd_builtins.py::TestTac::test_unused_result_goes (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_bcd_builtins.py::TestTac::test_unused_result_goes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Void FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(1) None)] None false) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Void FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(1) None)] None false) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_bcd_builtins.py::TestTac::test_unused_result_goes (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_bcd_builtins.py::TestTac::test_unused_result_goes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Void FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(1) None)] None false) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Void FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(1) None)] None false) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bcd_builtins.py::TestTac::test_unused_result_goes (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bcd_builtins.py::TestTac::test_unused_result_goes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Expression(Cast(Void FunctionCall(\\\"__c6502_bcd_add\\\" [Var(\\\"a\\\" UChar) Constant(ConstUChar(1) UChar)] UChar false) Void)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Void FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" None) Constant(ConstInt(1) None)] None false) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Expression(Cast(Void FunctionCall(\\\"__c6502_bcd_add\\\" [Var(\\\"a\\\" UChar) Constant(ConstUChar(1) UChar)] UChar false) Void)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_bcd_builtins.py::TestTac::test_unused_result_goes (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_bcd_builtins.py::TestTac::test_unused_result_goes\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None UChar None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Cast(Void FunctionCall(\"__c6502_bcd_add\" [Var(\"a\" UChar) Constant(ConstUChar(1) UChar)] UChar false) Void)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Binary(BcdAdd Var(\"a\") Constant(ConstUChar(1)) Var(\"%0\"))\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"a\" true UInt [ZeroInit(1)])\n"
      "])\n");
}

TEST_CASE("inline_functions: test_bcd_builtins.py::TestTac::test_unused_result_goes (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass inline_functions\n"
      "source tests/test_bcd_builtins.py::TestTac::test_unused_result_goes\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Expression(Cast(Void FunctionCall(\\\"__c6502_bcd_add\\\" [Var(\\\"a\\\" UChar) Constant(ConstUChar(1) UChar)] UChar false) Void)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Binary(BcdAdd Var(\"a\") Constant(ConstUChar(1)) Var(\"%0\"))\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"a\" true UInt [ZeroInit(1)])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Binary(BcdAdd Var(\"a\") Constant(ConstUChar(1)) Var(\"%0\"))\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"a\" true UInt [ZeroInit(1)])\n"
      "])\n");
}

TEST_CASE("return_pointers_in_zero_page: test_bcd_builtins.py::TestTac::test_unused_result_goes (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass return_pointers_in_zero_page\n"
      "source tests/test_bcd_builtins.py::TestTac::test_unused_result_goes\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Binary(BcdAdd Var(\"a\") Constant(ConstUChar(1)) Var(\"%0\"))\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"a\" true UInt [ZeroInit(1)])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Binary(BcdAdd Var(\"a\") Constant(ConstUChar(1)) Var(\"%0\"))\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"a\" true UInt [ZeroInit(1)])\n"
      "])\n");
}

TEST_CASE("optimize_tac: test_bcd_builtins.py::TestTac::test_unused_result_goes (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass optimize_tac\n"
      "source tests/test_bcd_builtins.py::TestTac::test_unused_result_goes\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Binary(BcdAdd Var(\"a\") Constant(ConstUChar(1)) Var(\"%0\"))\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"a\" true UInt [ZeroInit(1)])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"a\" true UInt [ZeroInit(1)])\n"
      "])\n");
}
