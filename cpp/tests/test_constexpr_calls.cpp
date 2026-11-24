// The calls that the tests of tests/test_constexpr.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_constexpr.py::TestParse::test_array_elements_are_const (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestParse::test_array_elements_are_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"constexpr int A[2] = {1, 2};\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Const(Int) 2) None None None None true))\n"
      "])\n");
}

TEST_CASE("parse: test_constexpr.py::TestParse::test_errors (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"constexpr int x;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`constexpr` object `x` has no initializer, and a constexpr declaration must have one (C23 \\u00a76.7.2p5)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestParse::test_errors (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"extern constexpr int x = 1;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`constexpr` can't go with `extern`; of the other storage-class specifiers, only `static` and `register` may come with it (C23 \\u00a76.7.2p2)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestParse::test_errors (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"constexpr typedef int T;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`constexpr` can't go with `typedef`; of the other storage-class specifiers, only `static` and `register` may come with it (C23 \\u00a76.7.2p2)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestParse::test_errors (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"constexpr constexpr int x = 1;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`constexpr` appears twice in a declaration\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestParse::test_errors (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"constexpr int f(void);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`constexpr` declares a named constant, which is an object, and function declaration 'f' declares a function\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestParse::test_errors (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"constexpr int f(void) { return 0; }\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`constexpr` declares a named constant, which is an object, and function definition 'f' declares a function\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestParse::test_errors (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(constexpr int x);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`constexpr` isn't permitted on parameter 'x', where the only storage-class specifier is `register`\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestParse::test_errors (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"constexpr struct t { int a; };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"a `constexpr` declaration must declare an object\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestParse::test_errors (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int constexpr;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"a `constexpr` declaration must declare an object\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestParse::test_errors (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void) { int constexpr = 1; return 0; }\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"line 1, column 29: `constexpr` is a keyword in C23 (\\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestParse::test_marks_the_declaration_and_adds_const (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestParse::test_marks_the_declaration_and_adds_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"constexpr int K = 47;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(47) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("parse: test_constexpr.py::TestParse::test_the_pointer_itself_is_const (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestParse::test_the_pointer_itself_is_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"static constexpr int *p = 0;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" Constant(ConstInt(0) None) Const(Pointer(Int)) Static None None None true))\n"
      "])\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_accepted (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr unsigned int uint_max = -1U;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"uint_max\" Unary(Negate Constant(ConstUInt(1) None) None) Const(UInt) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_accepted (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"uint_max\" Unary(Negate Constant(ConstUInt(1) None) None) Const(UInt) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"uint_max\" Unary(Negate Constant(ConstUInt(1) None) None) Const(UInt) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_accepted (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"uint_max\" Unary(Negate Constant(ConstUInt(1) None) None) Const(UInt) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"uint_max\" Unary(Negate Constant(ConstUInt(1) None) None) Const(UInt) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_accepted (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"uint_max\" Unary(Negate Constant(ConstUInt(1) None) None) Const(UInt) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"uint_max\" Unary(Negate Constant(ConstUInt(1) None) None) Const(UInt) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_accepted (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"uint_max\" Unary(Negate Constant(ConstUInt(1) None) None) Const(UInt) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"uint_max\" Unary(Negate Constant(ConstUInt(1) None) None) Const(UInt) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_accepted (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"uint_max\\\" Unary(Negate Constant(ConstUInt(1) UInt) UInt) Const(UInt) Static None None None true))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"uint_max\" Unary(Negate Constant(ConstUInt(1) None) None) Const(UInt) None None None None true))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"uint_max\\\" Unary(Negate Constant(ConstUInt(1) UInt) UInt) Const(UInt) Static None None None true))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"uint_max\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(UInt)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 65535}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_accepted (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr int A = 42LL;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" Constant(ConstLongLong(42) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_accepted (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" Constant(ConstLongLong(42) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" Constant(ConstLongLong(42) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_accepted (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" Constant(ConstLongLong(42) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" Constant(ConstLongLong(42) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_accepted (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" Constant(ConstLongLong(42) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" Constant(ConstLongLong(42) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_accepted (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" Constant(ConstLongLong(42) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" Constant(ConstLongLong(42) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_accepted (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"A\\\" Cast(Const(Int) Constant(ConstLongLong(42) LongLong) Const(Int)) Const(Int) Static None None None true))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" Constant(ConstLongLong(42) None) Const(Int) None None None None true))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"A\\\" Cast(Const(Int) Constant(ConstLongLong(42) LongLong) Const(Int)) Const(Int) Static None None None true))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"A\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 42}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_accepted (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr float C = 47u;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"C\" Constant(ConstUInt(47) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_accepted (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"C\" Constant(ConstUInt(47) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"C\" Constant(ConstUInt(47) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_accepted (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"C\" Constant(ConstUInt(47) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"C\" Constant(ConstUInt(47) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_accepted (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"C\" Constant(ConstUInt(47) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"C\" Constant(ConstUInt(47) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_accepted (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"C\" Constant(ConstUInt(47) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"C\" Constant(ConstUInt(47) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_accepted (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"C\\\" Cast(Const(Float) Constant(ConstUInt(47) UInt) Const(Float)) Const(Float) Static None None None true))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"C\" Constant(ConstUInt(47) None) Const(Float) None None None None true))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"C\\\" Cast(Const(Float) Constant(ConstUInt(47) UInt) Const(Float)) Const(Float) Static None None None true))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"C\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Float)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1111228416}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_accepted (call 19)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr double onethird = 1.0 / 3.0;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"onethird\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Double) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_accepted (call 20)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"onethird\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Double) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"onethird\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Double) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_accepted (call 21)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"onethird\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Double) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"onethird\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Double) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_accepted (call 22)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"onethird\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Double) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"onethird\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Double) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_accepted (call 23)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"onethird\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Double) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"onethird\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Double) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_accepted (call 24)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"onethird\\\" Binary(Divide Constant(ConstDouble(4607182418800017408) Double) Constant(ConstDouble(4613937818241073152) Double) Double) Const(Double) Static None None None true))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"onethird\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Double) None None None None true))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"onethird\\\" Binary(Divide Constant(ConstDouble(4607182418800017408) Double) Constant(ConstDouble(4613937818241073152) Double) Double) Const(Double) Static None None None true))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"onethird\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Double)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 4599676419421066581}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_accepted (call 25)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr float F = 1.0f / 3.0f;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"F\" Binary(Divide Constant(ConstFloat(1065353216) None) Constant(ConstFloat(1077936128) None) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_accepted (call 26)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"F\" Binary(Divide Constant(ConstFloat(1065353216) None) Constant(ConstFloat(1077936128) None) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"F\" Binary(Divide Constant(ConstFloat(1065353216) None) Constant(ConstFloat(1077936128) None) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_accepted (call 27)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"F\" Binary(Divide Constant(ConstFloat(1065353216) None) Constant(ConstFloat(1077936128) None) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"F\" Binary(Divide Constant(ConstFloat(1065353216) None) Constant(ConstFloat(1077936128) None) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_accepted (call 28)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"F\" Binary(Divide Constant(ConstFloat(1065353216) None) Constant(ConstFloat(1077936128) None) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"F\" Binary(Divide Constant(ConstFloat(1065353216) None) Constant(ConstFloat(1077936128) None) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_accepted (call 29)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"F\" Binary(Divide Constant(ConstFloat(1065353216) None) Constant(ConstFloat(1077936128) None) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"F\" Binary(Divide Constant(ConstFloat(1065353216) None) Constant(ConstFloat(1077936128) None) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_accepted (call 30)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"F\\\" Binary(Divide Constant(ConstFloat(1065353216) Float) Constant(ConstFloat(1077936128) Float) Float) Const(Float) Static None None None true))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"F\" Binary(Divide Constant(ConstFloat(1065353216) None) Constant(ConstFloat(1077936128) None) None) Const(Float) None None None None true))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"F\\\" Binary(Divide Constant(ConstFloat(1065353216) Float) Constant(ConstFloat(1077936128) Float) Float) Const(Float) Static None None None true))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"F\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Float)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1051372203}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_accepted (call 31)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr double d = 0.1f;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"d\" Constant(ConstFloat(1036831949) None) Const(Double) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_accepted (call 32)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"d\" Constant(ConstFloat(1036831949) None) Const(Double) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"d\" Constant(ConstFloat(1036831949) None) Const(Double) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_accepted (call 33)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"d\" Constant(ConstFloat(1036831949) None) Const(Double) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"d\" Constant(ConstFloat(1036831949) None) Const(Double) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_accepted (call 34)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"d\" Constant(ConstFloat(1036831949) None) Const(Double) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"d\" Constant(ConstFloat(1036831949) None) Const(Double) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_accepted (call 35)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"d\" Constant(ConstFloat(1036831949) None) Const(Double) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"d\" Constant(ConstFloat(1036831949) None) Const(Double) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_accepted (call 36)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"d\\\" Cast(Const(Double) Constant(ConstFloat(1036831949) Float) Const(Double)) Const(Double) Static None None None true))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"d\" Constant(ConstFloat(1036831949) None) Const(Double) None None None None true))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"d\\\" Cast(Const(Double) Constant(ConstFloat(1036831949) Float) Const(Double)) Const(Double) Static None None None true))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Double)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 4591870180174331904}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_accepted (call 37)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr int x = (int)3.0;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Cast(Int Constant(ConstDouble(4613937818241073152) None) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_accepted (call 38)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Cast(Int Constant(ConstDouble(4613937818241073152) None) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Cast(Int Constant(ConstDouble(4613937818241073152) None) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_accepted (call 39)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Cast(Int Constant(ConstDouble(4613937818241073152) None) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Cast(Int Constant(ConstDouble(4613937818241073152) None) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_accepted (call 40)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Cast(Int Constant(ConstDouble(4613937818241073152) None) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Cast(Int Constant(ConstDouble(4613937818241073152) None) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_accepted (call 41)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Cast(Int Constant(ConstDouble(4613937818241073152) None) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Cast(Int Constant(ConstDouble(4613937818241073152) None) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_accepted (call 42)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" Cast(Int Constant(ConstDouble(4613937818241073152) Double) Int) Const(Int) Static None None None true))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Cast(Int Constant(ConstDouble(4613937818241073152) None) None) Const(Int) None None None None true))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" Cast(Int Constant(ConstDouble(4613937818241073152) Double) Int) Const(Int) Static None None None true))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 3}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_accepted (call 43)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr double D = 2.5; constexpr int x = (int)D;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"x\" Cast(Int Var(\"D\" None) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_accepted (call 44)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"x\" Cast(Int Var(\"D\" None) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"x\" Cast(Int Var(\"D\" None) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_accepted (call 45)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"x\" Cast(Int Var(\"D\" None) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"x\" Cast(Int Var(\"D\" None) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_accepted (call 46)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"x\" Cast(Int Var(\"D\" None) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"x\" Cast(Int Var(\"D\" None) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_accepted (call 47)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"x\" Cast(Int Var(\"D\" None) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"x\" Cast(Int Var(\"D\" None) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_accepted (call 48)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"D\\\" Constant(ConstDouble(4612811918334230528) Double) Const(Double) Static None None None true))\\n  VarDecl(var_decl(\\\"x\\\" Cast(Int Var(\\\"D\\\" Const(Double)) Int) Const(Int) Static None None None true))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"x\" Cast(Int Var(\"D\" None) None) Const(Int) None None None None true))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"D\\\" Constant(ConstDouble(4612811918334230528) Double) Const(Double) Static None None None true))\\n  VarDecl(var_decl(\\\"x\\\" Cast(Int Var(\\\"D\\\" Const(Double)) Int) Const(Int) Static None None None true))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"D\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Double)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 4612811918334230528}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 2}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_accepted (call 49)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr int *p = {};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" InitList([] None None) Const(Pointer(Int)) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_accepted (call 50)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" InitList([] None None) Const(Pointer(Int)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" InitList([] None None) Const(Pointer(Int)) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_accepted (call 51)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" InitList([] None None) Const(Pointer(Int)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" InitList([] None None) Const(Pointer(Int)) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_accepted (call 52)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" InitList([] None None) Const(Pointer(Int)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" InitList([] None None) Const(Pointer(Int)) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_accepted (call 53)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" InitList([] None None) Const(Pointer(Int)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" InitList([] None None) Const(Pointer(Int)) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_accepted (call 54)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" Cast(Const(Pointer(Int)) Constant(ConstInt(0) Int) Const(Pointer(Int))) Const(Pointer(Int)) Static None None None true))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" InitList([] None None) Const(Pointer(Int)) None None None None true))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" Cast(Const(Pointer(Int)) Constant(ConstInt(0) Int) Const(Pointer(Int))) Const(Pointer(Int)) Static None None None true))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Pointer(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_accepted (call 55)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr int *p = nullptr;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" Constant(ConstNullptr None) Const(Pointer(Int)) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_accepted (call 56)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" Constant(ConstNullptr None) Const(Pointer(Int)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" Constant(ConstNullptr None) Const(Pointer(Int)) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_accepted (call 57)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" Constant(ConstNullptr None) Const(Pointer(Int)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" Constant(ConstNullptr None) Const(Pointer(Int)) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_accepted (call 58)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" Constant(ConstNullptr None) Const(Pointer(Int)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" Constant(ConstNullptr None) Const(Pointer(Int)) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_accepted (call 59)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" Constant(ConstNullptr None) Const(Pointer(Int)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" Constant(ConstNullptr None) Const(Pointer(Int)) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_accepted (call 60)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" Cast(Const(Pointer(Int)) Constant(ConstNullptr NullptrT) Const(Pointer(Int))) Const(Pointer(Int)) Static None None None true))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" Constant(ConstNullptr None) Const(Pointer(Int)) None None None None true))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" Cast(Const(Pointer(Int)) Constant(ConstNullptr NullptrT) Const(Pointer(Int))) Const(Pointer(Int)) Static None None None true))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Pointer(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_accepted (call 61)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr nullptr_t N = nullptr;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"N\" Constant(ConstNullptr None) Const(NullptrT) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_accepted (call 62)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"N\" Constant(ConstNullptr None) Const(NullptrT) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"N\" Constant(ConstNullptr None) Const(NullptrT) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_accepted (call 63)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"N\" Constant(ConstNullptr None) Const(NullptrT) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"N\" Constant(ConstNullptr None) Const(NullptrT) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_accepted (call 64)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"N\" Constant(ConstNullptr None) Const(NullptrT) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"N\" Constant(ConstNullptr None) Const(NullptrT) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_accepted (call 65)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"N\" Constant(ConstNullptr None) Const(NullptrT) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"N\" Constant(ConstNullptr None) Const(NullptrT) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_accepted (call 66)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"N\\\" Constant(ConstNullptr NullptrT) Const(NullptrT) Static None None None true))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"N\" Constant(ConstNullptr None) Const(NullptrT) None None None None true))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"N\\\" Constant(ConstNullptr NullptrT) Const(NullptrT) Static None None None true))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"N\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(NullptrT)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_accepted (call 67)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nstruct s { void *p; }; constexpr struct s A = { nullptr }; constexpr struct s B = A;\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"p\" Pointer(Void))]))\n"
      "  VarDecl(var_decl(\"A\" InitList([Constant(ConstNullptr None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"B\" Var(\"A\" None) Const(Structure(\"s\")) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_accepted (call 68)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"p\" Pointer(Void))]))\n"
      "  VarDecl(var_decl(\"A\" InitList([Constant(ConstNullptr None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"B\" Var(\"A\" None) Const(Structure(\"s\")) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"p\" Pointer(Void))]))\n"
      "  VarDecl(var_decl(\"A\" InitList([Constant(ConstNullptr None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"B\" Var(\"A\" None) Const(Structure(\"s\")) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_accepted (call 69)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"p\" Pointer(Void))]))\n"
      "  VarDecl(var_decl(\"A\" InitList([Constant(ConstNullptr None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"B\" Var(\"A\" None) Const(Structure(\"s\")) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"p\" Pointer(Void))]))\n"
      "  VarDecl(var_decl(\"A\" InitList([Constant(ConstNullptr None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"B\" Var(\"A\" None) Const(Structure(\"s\")) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_accepted (call 70)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"p\" Pointer(Void))]))\n"
      "  VarDecl(var_decl(\"A\" InitList([Constant(ConstNullptr None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"B\" Var(\"A\" None) Const(Structure(\"s\")) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"p\" Pointer(Void))]))\n"
      "  VarDecl(var_decl(\"A\" InitList([Constant(ConstNullptr None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"B\" Var(\"A\" None) Const(Structure(\"s\")) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_accepted (call 71)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"p\" Pointer(Void))]))\n"
      "  VarDecl(var_decl(\"A\" InitList([Constant(ConstNullptr None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"B\" Var(\"A\" None) Const(Structure(\"s\")) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"p\" Pointer(Void))]))\n"
      "  VarDecl(var_decl(\"A\" InitList([Constant(ConstNullptr None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"B\" Var(\"A\" None) Const(Structure(\"s\")) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_accepted (call 72)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"p\\\" Pointer(Void))]))\\n  VarDecl(var_decl(\\\"A\\\" InitList([Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void))] Structure(\\\"s\\\") None) Const(Structure(\\\"s\\\")) Static None None None true))\\n  VarDecl(var_decl(\\\"B\\\" Var(\\\"A\\\" Const(Structure(\\\"s\\\"))) Const(Structure(\\\"s\\\")) Static None None None true))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"p\" Pointer(Void))]))\n"
      "  VarDecl(var_decl(\"A\" InitList([Constant(ConstNullptr None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"B\" Var(\"A\" None) Const(Structure(\"s\")) None None None None true))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"p\\\" Pointer(Void))]))\\n  VarDecl(var_decl(\\\"A\\\" InitList([Cast(Pointer(Void) Constant(ConstNullptr NullptrT) Pointer(Void))] Structure(\\\"s\\\") None) Const(Structure(\\\"s\\\")) Static None None None true))\\n  VarDecl(var_decl(\\\"B\\\" Var(\\\"A\\\" Const(Structure(\\\"s\\\"))) Const(Structure(\\\"s\\\")) Static None None None true))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"A\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Structure(\\\"s\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"B\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Structure(\\\"s\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"p\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Void)\\n\"}, \"byte_offset\": 0}}], \"size\": 2, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_accepted (call 73)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr char s[] = \\\"\\\\xFF\\\";\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" String(\"\\xff\") Array(Const(Char) 0) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_accepted (call 74)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" String(\"\\xff\") Array(Const(Char) 0) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" String(\"\\xff\") Array(Const(Char) 0) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_accepted (call 75)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" String(\"\\xff\") Array(Const(Char) 0) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" String(\"\\xff\") Array(Const(Char) 0) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_accepted (call 76)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" String(\"\\xff\") Array(Const(Char) 0) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" String(\"\\xff\") Array(Const(Char) 0) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_accepted (call 77)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" String(\"\\xff\") Array(Const(Char) 0) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" String(\"\\xff\") Array(Const(Char) 0) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_accepted (call 78)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"s\\\" String(\\\"\\\\xff\\\") Array(Const(Char) 2) Static None None None true))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" String(\"\\xff\") Array(Const(Char) 0) None None None None true))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"s\\\" String(\\\"\\\\xff\\\") Array(Const(Char) 2) Static None None None true))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(Char) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [255, 0]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_accepted (call 79)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr unsigned char u[] = { 255, 0 };\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" InitList([Constant(ConstInt(255) None) Constant(ConstInt(0) None)] None None) Array(Const(UChar) 0) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_accepted (call 80)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" InitList([Constant(ConstInt(255) None) Constant(ConstInt(0) None)] None None) Array(Const(UChar) 0) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" InitList([Constant(ConstInt(255) None) Constant(ConstInt(0) None)] None None) Array(Const(UChar) 0) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_accepted (call 81)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" InitList([Constant(ConstInt(255) None) Constant(ConstInt(0) None)] None None) Array(Const(UChar) 0) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" InitList([Constant(ConstInt(255) None) Constant(ConstInt(0) None)] None None) Array(Const(UChar) 0) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_accepted (call 82)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" InitList([Constant(ConstInt(255) None) Constant(ConstInt(0) None)] None None) Array(Const(UChar) 0) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" InitList([Constant(ConstInt(255) None) Constant(ConstInt(0) None)] None None) Array(Const(UChar) 0) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_accepted (call 83)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" InitList([Constant(ConstInt(255) None) Constant(ConstInt(0) None)] None None) Array(Const(UChar) 0) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" InitList([Constant(ConstInt(255) None) Constant(ConstInt(0) None)] None None) Array(Const(UChar) 0) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_accepted (call 84)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"u\\\" InitList([Cast(Const(UChar) Constant(ConstInt(255) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(0) Int) Const(UChar))] Array(Const(UChar) 2) None) Array(Const(UChar) 2) Static None None None true))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" InitList([Constant(ConstInt(255) None) Constant(ConstInt(0) None)] None None) Array(Const(UChar) 0) None None None None true))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"u\\\" InitList([Cast(Const(UChar) Constant(ConstInt(255) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(0) Int) Const(UChar))] Array(Const(UChar) 2) None) Array(Const(UChar) 2) Static None None None true))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(UChar) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [255, 0]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_accepted (call 85)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr bool b = 1;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(1) None) Const(Bool) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_accepted (call 86)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(1) None) Const(Bool) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(1) None) Const(Bool) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_accepted (call 87)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(1) None) Const(Bool) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(1) None) Const(Bool) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_accepted (call 88)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(1) None) Const(Bool) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(1) None) Const(Bool) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_accepted (call 89)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(1) None) Const(Bool) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(1) None) Const(Bool) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_accepted (call 90)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Cast(Const(Bool) Constant(ConstInt(1) Int) Const(Bool)) Const(Bool) Static None None None true))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(1) None) Const(Bool) None None None None true))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Cast(Const(Bool) Constant(ConstInt(1) Int) Const(Bool)) Const(Bool) Static None None None true))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_accepted (call 91)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr _BitInt(5) b5 = 15;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b5\" Constant(ConstInt(15) None) Const(BitInt(5 true)) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_accepted (call 92)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b5\" Constant(ConstInt(15) None) Const(BitInt(5 true)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b5\" Constant(ConstInt(15) None) Const(BitInt(5 true)) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_accepted (call 93)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b5\" Constant(ConstInt(15) None) Const(BitInt(5 true)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b5\" Constant(ConstInt(15) None) Const(BitInt(5 true)) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_accepted (call 94)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b5\" Constant(ConstInt(15) None) Const(BitInt(5 true)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b5\" Constant(ConstInt(15) None) Const(BitInt(5 true)) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_accepted (call 95)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b5\" Constant(ConstInt(15) None) Const(BitInt(5 true)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b5\" Constant(ConstInt(15) None) Const(BitInt(5 true)) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_accepted (call 96)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b5\\\" Cast(Const(BitInt(5 true)) Constant(ConstInt(15) Int) Const(BitInt(5 true))) Const(BitInt(5 true)) Static None None None true))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b5\" Constant(ConstInt(15) None) Const(BitInt(5 true)) None None None None true))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b5\\\" Cast(Const(BitInt(5 true)) Constant(ConstInt(15) Int) Const(BitInt(5 true))) Const(BitInt(5 true)) Static None None None true))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(BitInt(5 true))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 15}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_accepted (call 97)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint f(void) { static constexpr int S = 3; register constexpr unsigned char R = 4; return S + R; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"S\" Constant(ConstInt(3) None) Const(Int) Static None None None true)))\n"
      "    D(VarDecl(var_decl(\"R\" Constant(ConstInt(4) None) Const(UChar) Register None None None true)))\n"
      "    S(Return(Binary(Add Var(\"S\" None) Var(\"R\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_accepted (call 98)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"S\" Constant(ConstInt(3) None) Const(Int) Static None None None true)))\n"
      "    D(VarDecl(var_decl(\"R\" Constant(ConstInt(4) None) Const(UChar) Register None None None true)))\n"
      "    S(Return(Binary(Add Var(\"S\" None) Var(\"R\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.S\" Constant(ConstInt(3) None) Const(Int) Static None None None true)))\n"
      "    D(VarDecl(var_decl(\"@1.R\" Constant(ConstInt(4) None) Const(UChar) Register None None None true)))\n"
      "    S(Return(Binary(Add Var(\"@0.S\" None) Var(\"@1.R\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_accepted (call 99)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.S\" Constant(ConstInt(3) None) Const(Int) Static None None None true)))\n"
      "    D(VarDecl(var_decl(\"@1.R\" Constant(ConstInt(4) None) Const(UChar) Register None None None true)))\n"
      "    S(Return(Binary(Add Var(\"@0.S\" None) Var(\"@1.R\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.S\" Constant(ConstInt(3) None) Const(Int) Static None None None true)))\n"
      "    D(VarDecl(var_decl(\"@1.R\" Constant(ConstInt(4) None) Const(UChar) Register None None None true)))\n"
      "    S(Return(Binary(Add Var(\"@0.S\" None) Var(\"@1.R\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_accepted (call 100)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.S\" Constant(ConstInt(3) None) Const(Int) Static None None None true)))\n"
      "    D(VarDecl(var_decl(\"@1.R\" Constant(ConstInt(4) None) Const(UChar) Register None None None true)))\n"
      "    S(Return(Binary(Add Var(\"@0.S\" None) Var(\"@1.R\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.S\" Constant(ConstInt(3) None) Const(Int) Static None None None true)))\n"
      "    D(VarDecl(var_decl(\"@1.R\" Constant(ConstInt(4) None) Const(UChar) Register None None None true)))\n"
      "    S(Return(Binary(Add Var(\"@0.S\" None) Var(\"@1.R\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_accepted (call 101)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.S\" Constant(ConstInt(3) None) Const(Int) Static None None None true)))\n"
      "    D(VarDecl(var_decl(\"@1.R\" Constant(ConstInt(4) None) Const(UChar) Register None None None true)))\n"
      "    S(Return(Binary(Add Var(\"@0.S\" None) Var(\"@1.R\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.S\" Constant(ConstInt(3) None) Const(Int) Static None None None true)))\n"
      "    D(VarDecl(var_decl(\"@1.R\" Constant(ConstInt(4) None) Const(UChar) Register None None None true)))\n"
      "    S(Return(Binary(Add Var(\"@0.S\" None) Var(\"@1.R\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_accepted (call 102)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.S\\\" Constant(ConstInt(3) Int) Const(Int) Static None None None true)))\\n    D(VarDecl(var_decl(\\\"@1.R\\\" Cast(Const(UChar) Constant(ConstInt(4) Int) Const(UChar)) Const(UChar) Register None None None true)))\\n    S(Return(Binary(Add Var(\\\"@0.S\\\" Const(Int)) Cast(Int Var(\\\"@1.R\\\" Const(UChar)) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.S\" Constant(ConstInt(3) None) Const(Int) Static None None None true)))\n"
      "    D(VarDecl(var_decl(\"@1.R\" Constant(ConstInt(4) None) Const(UChar) Register None None None true)))\n"
      "    S(Return(Binary(Add Var(\"@0.S\" None) Var(\"@1.R\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.S\\\" Constant(ConstInt(3) Int) Const(Int) Static None None None true)))\\n    D(VarDecl(var_decl(\\\"@1.R\\\" Cast(Const(UChar) Constant(ConstInt(4) Int) Const(UChar)) Const(UChar) Register None None None true)))\\n    S(Return(Binary(Add Var(\\\"@0.S\\\" Const(Int)) Cast(Int Var(\\\"@1.R\\\" Const(UChar)) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.S\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 3}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@1.R\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": true, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_accepted (call 103)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr float f = 1.0f; constexpr float g = 3.0f; constexpr float h = f / g;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstFloat(1065353216) None) Const(Float) None None None None true))\n"
      "  VarDecl(var_decl(\"g\" Constant(ConstFloat(1077936128) None) Const(Float) None None None None true))\n"
      "  VarDecl(var_decl(\"h\" Binary(Divide Var(\"f\" None) Var(\"g\" None) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_accepted (call 104)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstFloat(1065353216) None) Const(Float) None None None None true))\n"
      "  VarDecl(var_decl(\"g\" Constant(ConstFloat(1077936128) None) Const(Float) None None None None true))\n"
      "  VarDecl(var_decl(\"h\" Binary(Divide Var(\"f\" None) Var(\"g\" None) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstFloat(1065353216) None) Const(Float) None None None None true))\n"
      "  VarDecl(var_decl(\"g\" Constant(ConstFloat(1077936128) None) Const(Float) None None None None true))\n"
      "  VarDecl(var_decl(\"h\" Binary(Divide Var(\"f\" None) Var(\"g\" None) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_accepted (call 105)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstFloat(1065353216) None) Const(Float) None None None None true))\n"
      "  VarDecl(var_decl(\"g\" Constant(ConstFloat(1077936128) None) Const(Float) None None None None true))\n"
      "  VarDecl(var_decl(\"h\" Binary(Divide Var(\"f\" None) Var(\"g\" None) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstFloat(1065353216) None) Const(Float) None None None None true))\n"
      "  VarDecl(var_decl(\"g\" Constant(ConstFloat(1077936128) None) Const(Float) None None None None true))\n"
      "  VarDecl(var_decl(\"h\" Binary(Divide Var(\"f\" None) Var(\"g\" None) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_accepted (call 106)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstFloat(1065353216) None) Const(Float) None None None None true))\n"
      "  VarDecl(var_decl(\"g\" Constant(ConstFloat(1077936128) None) Const(Float) None None None None true))\n"
      "  VarDecl(var_decl(\"h\" Binary(Divide Var(\"f\" None) Var(\"g\" None) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstFloat(1065353216) None) Const(Float) None None None None true))\n"
      "  VarDecl(var_decl(\"g\" Constant(ConstFloat(1077936128) None) Const(Float) None None None None true))\n"
      "  VarDecl(var_decl(\"h\" Binary(Divide Var(\"f\" None) Var(\"g\" None) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_accepted (call 107)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstFloat(1065353216) None) Const(Float) None None None None true))\n"
      "  VarDecl(var_decl(\"g\" Constant(ConstFloat(1077936128) None) Const(Float) None None None None true))\n"
      "  VarDecl(var_decl(\"h\" Binary(Divide Var(\"f\" None) Var(\"g\" None) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstFloat(1065353216) None) Const(Float) None None None None true))\n"
      "  VarDecl(var_decl(\"g\" Constant(ConstFloat(1077936128) None) Const(Float) None None None None true))\n"
      "  VarDecl(var_decl(\"h\" Binary(Divide Var(\"f\" None) Var(\"g\" None) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_accepted (call 108)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"f\\\" Constant(ConstFloat(1065353216) Float) Const(Float) Static None None None true))\\n  VarDecl(var_decl(\\\"g\\\" Constant(ConstFloat(1077936128) Float) Const(Float) Static None None None true))\\n  VarDecl(var_decl(\\\"h\\\" Binary(Divide Var(\\\"f\\\" Const(Float)) Var(\\\"g\\\" Const(Float)) Float) Const(Float) Static None None None true))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstFloat(1065353216) None) Const(Float) None None None None true))\n"
      "  VarDecl(var_decl(\"g\" Constant(ConstFloat(1077936128) None) Const(Float) None None None None true))\n"
      "  VarDecl(var_decl(\"h\" Binary(Divide Var(\"f\" None) Var(\"g\" None) None) Const(Float) None None None None true))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"f\\\" Constant(ConstFloat(1065353216) Float) Const(Float) Static None None None true))\\n  VarDecl(var_decl(\\\"g\\\" Constant(ConstFloat(1077936128) Float) Const(Float) Static None None None true))\\n  VarDecl(var_decl(\\\"h\\\" Binary(Divide Var(\\\"f\\\" Const(Float)) Var(\\\"g\\\" Const(Float)) Float) Const(Float) Static None None None true))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Float)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1065353216}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Float)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1077936128}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"h\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Float)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1051372203}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr unsigned int minusOne = -1;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"minusOne\" Unary(Negate Constant(ConstInt(1) None) None) Const(UInt) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"minusOne\" Unary(Negate Constant(ConstInt(1) None) None) Const(UInt) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"minusOne\" Unary(Negate Constant(ConstInt(1) None) None) Const(UInt) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"minusOne\" Unary(Negate Constant(ConstInt(1) None) None) Const(UInt) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"minusOne\" Unary(Negate Constant(ConstInt(1) None) None) Const(UInt) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_rejected (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"minusOne\" Unary(Negate Constant(ConstInt(1) None) None) Const(UInt) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"minusOne\" Unary(Negate Constant(ConstInt(1) None) None) Const(UInt) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_rejected (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"minusOne\" Unary(Negate Constant(ConstInt(1) None) None) Const(UInt) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"minusOne\" Unary(Negate Constant(ConstInt(1) None) None) Const(UInt) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_rejected (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"minusOne\" Unary(Negate Constant(ConstInt(1) None) None) Const(UInt) None None None None true))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the initializer of `constexpr` object 'minusOne' has the value -1, which unsigned int can't hold exactly, and a constexpr initializer can't change a value (C23 \\u00a76.7.2p5)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_rejected (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr signed short B = 18446744073709551615ULL;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"B\" Constant(ConstULongLong(18446744073709551615) None) Const(Short) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_rejected (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"B\" Constant(ConstULongLong(18446744073709551615) None) Const(Short) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"B\" Constant(ConstULongLong(18446744073709551615) None) Const(Short) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_rejected (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"B\" Constant(ConstULongLong(18446744073709551615) None) Const(Short) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"B\" Constant(ConstULongLong(18446744073709551615) None) Const(Short) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_rejected (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"B\" Constant(ConstULongLong(18446744073709551615) None) Const(Short) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"B\" Constant(ConstULongLong(18446744073709551615) None) Const(Short) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_rejected (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"B\" Constant(ConstULongLong(18446744073709551615) None) Const(Short) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"B\" Constant(ConstULongLong(18446744073709551615) None) Const(Short) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_rejected (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"B\" Constant(ConstULongLong(18446744073709551615) None) Const(Short) None None None None true))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the initializer of `constexpr` object 'B' has the value 18446744073709551615, which short can't hold exactly, and a constexpr initializer can't change a value (C23 \\u00a76.7.2p5)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_rejected (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr float D = 16777217;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstLong(16777217) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_rejected (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstLong(16777217) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstLong(16777217) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_rejected (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstLong(16777217) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstLong(16777217) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_rejected (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstLong(16777217) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstLong(16777217) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_rejected (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstLong(16777217) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstLong(16777217) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_rejected (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstLong(16777217) None) Const(Float) None None None None true))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the initializer of `constexpr` object 'D' has the value 16777217, which float can't hold exactly, and a constexpr initializer can't change a value (C23 \\u00a76.7.2p5)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_rejected (call 19)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr float E = 1.0 / 3.0;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"E\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_rejected (call 20)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"E\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"E\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_rejected (call 21)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"E\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"E\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_rejected (call 22)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"E\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"E\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_rejected (call 23)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"E\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"E\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_rejected (call 24)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"E\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Float) None None None None true))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the initializer of `constexpr` object 'E' has a double value that float can't hold exactly, and a constexpr initializer can't change a value (C23 \\u00a76.7.2p5)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_rejected (call 25)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr float f = 0.1;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstDouble(4591870180066957722) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_rejected (call 26)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstDouble(4591870180066957722) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstDouble(4591870180066957722) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_rejected (call 27)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstDouble(4591870180066957722) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstDouble(4591870180066957722) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_rejected (call 28)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstDouble(4591870180066957722) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstDouble(4591870180066957722) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_rejected (call 29)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstDouble(4591870180066957722) None) Const(Float) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstDouble(4591870180066957722) None) Const(Float) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_rejected (call 30)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstDouble(4591870180066957722) None) Const(Float) None None None None true))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the initializer of `constexpr` object 'f' has a double value that float can't hold exactly, and a constexpr initializer can't change a value (C23 \\u00a76.7.2p5)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_rejected (call 31)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr int x = 3.0;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstDouble(4613937818241073152) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_rejected (call 32)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstDouble(4613937818241073152) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstDouble(4613937818241073152) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_rejected (call 33)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstDouble(4613937818241073152) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstDouble(4613937818241073152) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_rejected (call 34)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstDouble(4613937818241073152) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstDouble(4613937818241073152) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_rejected (call 35)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstDouble(4613937818241073152) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstDouble(4613937818241073152) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_rejected (call 36)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstDouble(4613937818241073152) None) Const(Int) None None None None true))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the initializer of `constexpr` object 'x' has the floating type double, and an object of integer type takes an integer constant expression (C23 \\u00a76.7.2p6)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_rejected (call 37)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr bool b = 2;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(2) None) Const(Bool) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_rejected (call 38)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(2) None) Const(Bool) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(2) None) Const(Bool) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_rejected (call 39)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(2) None) Const(Bool) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(2) None) Const(Bool) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_rejected (call 40)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(2) None) Const(Bool) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(2) None) Const(Bool) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_rejected (call 41)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(2) None) Const(Bool) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(2) None) Const(Bool) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_rejected (call 42)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(2) None) Const(Bool) None None None None true))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the initializer of `constexpr` object 'b' has the value 2, which bool can't hold exactly, and a constexpr initializer can't change a value (C23 \\u00a76.7.2p5)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_rejected (call 43)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr _BitInt(5) b5 = 16;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b5\" Constant(ConstInt(16) None) Const(BitInt(5 true)) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_rejected (call 44)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b5\" Constant(ConstInt(16) None) Const(BitInt(5 true)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b5\" Constant(ConstInt(16) None) Const(BitInt(5 true)) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_rejected (call 45)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b5\" Constant(ConstInt(16) None) Const(BitInt(5 true)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b5\" Constant(ConstInt(16) None) Const(BitInt(5 true)) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_rejected (call 46)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b5\" Constant(ConstInt(16) None) Const(BitInt(5 true)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b5\" Constant(ConstInt(16) None) Const(BitInt(5 true)) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_rejected (call 47)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b5\" Constant(ConstInt(16) None) Const(BitInt(5 true)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b5\" Constant(ConstInt(16) None) Const(BitInt(5 true)) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_rejected (call 48)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b5\" Constant(ConstInt(16) None) Const(BitInt(5 true)) None None None None true))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the initializer of `constexpr` object 'b5' has the value 16, which _BitInt(5) can't hold exactly, and a constexpr initializer can't change a value (C23 \\u00a76.7.2p5)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_rejected (call 49)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr unsigned char u[] = { -1, 0 };\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" InitList([Unary(Negate Constant(ConstInt(1) None) None) Constant(ConstInt(0) None)] None None) Array(Const(UChar) 0) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_rejected (call 50)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" InitList([Unary(Negate Constant(ConstInt(1) None) None) Constant(ConstInt(0) None)] None None) Array(Const(UChar) 0) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" InitList([Unary(Negate Constant(ConstInt(1) None) None) Constant(ConstInt(0) None)] None None) Array(Const(UChar) 0) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_rejected (call 51)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" InitList([Unary(Negate Constant(ConstInt(1) None) None) Constant(ConstInt(0) None)] None None) Array(Const(UChar) 0) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" InitList([Unary(Negate Constant(ConstInt(1) None) None) Constant(ConstInt(0) None)] None None) Array(Const(UChar) 0) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_rejected (call 52)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" InitList([Unary(Negate Constant(ConstInt(1) None) None) Constant(ConstInt(0) None)] None None) Array(Const(UChar) 0) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" InitList([Unary(Negate Constant(ConstInt(1) None) None) Constant(ConstInt(0) None)] None None) Array(Const(UChar) 0) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_rejected (call 53)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" InitList([Unary(Negate Constant(ConstInt(1) None) None) Constant(ConstInt(0) None)] None None) Array(Const(UChar) 0) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" InitList([Unary(Negate Constant(ConstInt(1) None) None) Constant(ConstInt(0) None)] None None) Array(Const(UChar) 0) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_rejected (call 54)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"u\" InitList([Unary(Negate Constant(ConstInt(1) None) None) Constant(ConstInt(0) None)] None None) Array(Const(UChar) 0) None None None None true))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the initializer of `constexpr` object 'u' has the value -1, which unsigned char can't hold exactly, and a constexpr initializer can't change a value (C23 \\u00a76.7.2p5)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_rejected (call 55)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr signed char s[] = \\\"\\\\xFF\\\";\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" String(\"\\xff\") Array(Const(SChar) 0) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_rejected (call 56)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" String(\"\\xff\") Array(Const(SChar) 0) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" String(\"\\xff\") Array(Const(SChar) 0) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_rejected (call 57)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" String(\"\\xff\") Array(Const(SChar) 0) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" String(\"\\xff\") Array(Const(SChar) 0) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_rejected (call 58)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" String(\"\\xff\") Array(Const(SChar) 0) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" String(\"\\xff\") Array(Const(SChar) 0) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_rejected (call 59)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" String(\"\\xff\") Array(Const(SChar) 0) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" String(\"\\xff\") Array(Const(SChar) 0) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_rejected (call 60)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" String(\"\\xff\") Array(Const(SChar) 0) None None None None true))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the string that initializes `constexpr` array 's' has the character 255, which signed char can't hold, and a constexpr initializer can't change a value (C23 \\u00a76.7.2p5)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_rejected (call 61)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint g; constexpr int *p = &g;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"g\" None) None) Const(Pointer(Int)) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_rejected (call 62)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"g\" None) None) Const(Pointer(Int)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"g\" None) None) Const(Pointer(Int)) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_rejected (call 63)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"g\" None) None) Const(Pointer(Int)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"g\" None) None) Const(Pointer(Int)) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_rejected (call 64)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"g\" None) None) Const(Pointer(Int)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"g\" None) None) Const(Pointer(Int)) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_rejected (call 65)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"g\" None) None) Const(Pointer(Int)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"g\" None) None) Const(Pointer(Int)) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_rejected (call 66)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"g\" None) None) Const(Pointer(Int)) None None None None true))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"`constexpr` object 'p' holds a pointer that isn't null, and a constexpr pointer must be null (C23 \\u00a76.7.2p6)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_rejected (call 67)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr volatile int v = 1;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Constant(ConstInt(1) None) Const(Volatile(Int)) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_rejected (call 68)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Constant(ConstInt(1) None) Const(Volatile(Int)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Constant(ConstInt(1) None) Const(Volatile(Int)) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_rejected (call 69)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Constant(ConstInt(1) None) Const(Volatile(Int)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Constant(ConstInt(1) None) Const(Volatile(Int)) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_rejected (call 70)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Constant(ConstInt(1) None) Const(Volatile(Int)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Constant(ConstInt(1) None) Const(Volatile(Int)) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_rejected (call 71)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Constant(ConstInt(1) None) Const(Volatile(Int)) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Constant(ConstInt(1) None) Const(Volatile(Int)) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_rejected (call 72)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"v\" Constant(ConstInt(1) None) Const(Volatile(Int)) None None None None true))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"`constexpr` object 'v' has the volatile type volatile int, and a constexpr object can't have a volatile type, or a member of one (C23 \\u00a76.7.2p5)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_rejected (call 73)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nstruct s { volatile int a; }; constexpr struct s S = {1};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Volatile(Int))]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(1) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_rejected (call 74)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Volatile(Int))]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(1) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Volatile(Int))]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(1) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_rejected (call 75)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Volatile(Int))]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(1) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Volatile(Int))]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(1) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_rejected (call 76)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Volatile(Int))]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(1) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Volatile(Int))]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(1) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_rejected (call 77)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Volatile(Int))]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(1) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Volatile(Int))]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(1) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_rejected (call 78)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Volatile(Int))]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(1) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"`constexpr` object 'S' has a volatile member 'a', and a constexpr object can't have a volatile type, or a member of one (C23 \\u00a76.7.2p5)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_rejected (call 79)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint f(void) { int y = 5; constexpr int z = y; return z; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"y\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"z\" Var(\"y\" None) Const(Int) None None None None true)))\n"
      "    S(Return(Var(\"z\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_rejected (call 80)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"y\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"z\" Var(\"y\" None) Const(Int) None None None None true)))\n"
      "    S(Return(Var(\"z\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.y\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.z\" Var(\"@0.y\" None) Const(Int) None None None None true)))\n"
      "    S(Return(Var(\"@1.z\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_rejected (call 81)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.y\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.z\" Var(\"@0.y\" None) Const(Int) None None None None true)))\n"
      "    S(Return(Var(\"@1.z\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.y\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.z\" Var(\"@0.y\" None) Const(Int) None None None None true)))\n"
      "    S(Return(Var(\"@1.z\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_rejected (call 82)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.y\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.z\" Var(\"@0.y\" None) Const(Int) None None None None true)))\n"
      "    S(Return(Var(\"@1.z\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.y\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.z\" Var(\"@0.y\" None) Const(Int) None None None None true)))\n"
      "    S(Return(Var(\"@1.z\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_rejected (call 83)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.y\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.z\" Var(\"@0.y\" None) Const(Int) None None None None true)))\n"
      "    S(Return(Var(\"@1.z\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.y\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.z\" Var(\"@0.y\" None) Const(Int) None None None None true)))\n"
      "    S(Return(Var(\"@1.z\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_rejected (call 84)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.y\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.z\" Var(\"@0.y\" None) Const(Int) None None None None true)))\n"
      "    S(Return(Var(\"@1.z\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the initializer of `constexpr` object 'z' isn't an integer constant expression (C23 \\u00a76.7.2p6): expression is not an integer constant expression: Var(name='@0.y', data_type=Int())\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_rejected (call 85)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint f(void) { constexpr int K = (int){5}; return K; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"K\" CompoundLiteral(\"\" Int Constant(ConstInt(5) None) None None false) Const(Int) None None None None true)))\n"
      "    S(Return(Var(\"K\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_rejected (call 86)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"K\" CompoundLiteral(\"\" Int Constant(ConstInt(5) None) None None false) Const(Int) None None None None true)))\n"
      "    S(Return(Var(\"K\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.K\" CompoundLiteral(\"@1.compound_literal\" Int Constant(ConstInt(5) None) None None false) Const(Int) None None None None true)))\n"
      "    S(Return(Var(\"@0.K\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_rejected (call 87)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.K\" CompoundLiteral(\"@1.compound_literal\" Int Constant(ConstInt(5) None) None None false) Const(Int) None None None None true)))\n"
      "    S(Return(Var(\"@0.K\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.K\" CompoundLiteral(\"@1.compound_literal\" Int Constant(ConstInt(5) None) None None false) Const(Int) None None None None true)))\n"
      "    S(Return(Var(\"@0.K\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_rejected (call 88)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.K\" CompoundLiteral(\"@1.compound_literal\" Int Constant(ConstInt(5) None) None None false) Const(Int) None None None None true)))\n"
      "    S(Return(Var(\"@0.K\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.K\" CompoundLiteral(\"@1.compound_literal\" Int Constant(ConstInt(5) None) None None false) Const(Int) None None None None true)))\n"
      "    S(Return(Var(\"@0.K\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_rejected (call 89)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.K\" CompoundLiteral(\"@1.compound_literal\" Int Constant(ConstInt(5) None) None None false) Const(Int) None None None None true)))\n"
      "    S(Return(Var(\"@0.K\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.K\" CompoundLiteral(\"@1.compound_literal\" Int Constant(ConstInt(5) None) None None false) Const(Int) None None None None true)))\n"
      "    S(Return(Var(\"@0.K\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_rejected (call 90)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.K\" CompoundLiteral(\"@1.compound_literal\" Int Constant(ConstInt(5) None) None None false) Const(Int) None None None None true)))\n"
      "    S(Return(Var(\"@0.K\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the initializer of `constexpr` object 'K' isn't an integer constant expression (C23 \\u00a76.7.2p6): expression is not an integer constant expression: CompoundLiteral(name='@1.compound_literal', target_type=Int(), init=Constant(const=ConstInt(value=5), data_type=Int()), data_type=Int(), storage_class=None, is_constexpr=False)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_rejected (call 91)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr int K = 1; void f(void) { K = 3; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"K\" None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_rejected (call 92)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"K\" None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"K\" None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_rejected (call 93)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"K\" None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"K\" None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_rejected (call 94)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"K\" None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"K\" None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_rejected (call 95)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"K\" None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"K\" None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_rejected (call 96)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"K\" None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot assign to const-qualified lvalue: Var(name='K', data_type=Const(referenced_type=Int())) has type Const(referenced_type=Int())\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_rejected (call 97)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr int K = 1; extern int K;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"K\" None Int Extern None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_rejected (call 98)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"K\" None Int Extern None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"K\" None Int Extern None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_rejected (call 99)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"K\" None Int Extern None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"K\" None Int Extern None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_rejected (call 100)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"K\" None Int Extern None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"K\" None Int Extern None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_rejected (call 101)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"K\" None Int Extern None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"K\" None Int Extern None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_rejected (call 102)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"K\" None Int Extern None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"'K' is declared again after its `constexpr` declaration, which c6502 takes only as the one declaration of a name (C23 \\u00a76.7.1p12 leaves any other to the implementation)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_rejected (call 103)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint K; constexpr int K = 1;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_rejected (call 104)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_rejected (call 105)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_rejected (call 106)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_rejected (call 107)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_rejected (call 108)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(1) None) Const(Int) None None None None true))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"'K' is declared `constexpr` after another declaration of it, and c6502 takes a `constexpr` declaration only as the one declaration of a name (C23 \\u00a76.7.1p12 leaves any other to the implementation)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_rejected (call 109)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr int A[3] = {1, 2, 3}; int arr[A[1]];\\n\"\n"
      "output-error\n"
      "{\"type\": \"NotImplementedError\", \"message\": \"array size must be an integer constant expression: undeclared identifier 'A'\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestRules::test_rejected (call 110)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nunion u { int a; char c; }; constexpr union u U = {1}; int f(int x) { switch (x) { case U.c: return 1; } return 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"a\" Int) member_decl(\"c\" Char)]))\n"
      "  VarDecl(var_decl(\"U\" InitList([Constant(ConstInt(1) None)] None None) Const(Union(\"u\")) None None None None true))\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    S(SwitchStmt(Var(\"x\" None) Compound(Block([\n"
      "      S(CaseStmt(Dot(Var(\"U\" None) \"c\" None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestRules::test_rejected (call 111)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"a\" Int) member_decl(\"c\" Char)]))\n"
      "  VarDecl(var_decl(\"U\" InitList([Constant(ConstInt(1) None)] None None) Const(Union(\"u\")) None None None None true))\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    S(SwitchStmt(Var(\"x\" None) Compound(Block([\n"
      "      S(CaseStmt(Dot(Var(\"U\" None) \"c\" None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"a\" Int) member_decl(\"c\" Char)]))\n"
      "  VarDecl(var_decl(\"U\" InitList([Constant(ConstInt(1) None)] None None) Const(Union(\"u\")) None None None None true))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Dot(Var(\"U\" None) \"c\" None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestRules::test_rejected (call 112)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"a\" Int) member_decl(\"c\" Char)]))\n"
      "  VarDecl(var_decl(\"U\" InitList([Constant(ConstInt(1) None)] None None) Const(Union(\"u\")) None None None None true))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Dot(Var(\"U\" None) \"c\" None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"a\" Int) member_decl(\"c\" Char)]))\n"
      "  VarDecl(var_decl(\"U\" InitList([Constant(ConstInt(1) None)] None None) Const(Union(\"u\")) None None None None true))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Dot(Var(\"U\" None) \"c\" None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestRules::test_rejected (call 113)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"a\" Int) member_decl(\"c\" Char)]))\n"
      "  VarDecl(var_decl(\"U\" InitList([Constant(ConstInt(1) None)] None None) Const(Union(\"u\")) None None None None true))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Dot(Var(\"U\" None) \"c\" None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"a\" Int) member_decl(\"c\" Char)]))\n"
      "  VarDecl(var_decl(\"U\" InitList([Constant(ConstInt(1) None)] None None) Const(Union(\"u\")) None None None None true))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Dot(Var(\"U\" None) \"c\" None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestRules::test_rejected (call 114)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"a\" Int) member_decl(\"c\" Char)]))\n"
      "  VarDecl(var_decl(\"U\" InitList([Constant(ConstInt(1) None)] None None) Const(Union(\"u\")) None None None None true))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Dot(Var(\"U\" None) \"c\" None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"a\" Int) member_decl(\"c\" Char)]))\n"
      "  VarDecl(var_decl(\"U\" InitList([Constant(ConstInt(1) None)] None None) Const(Union(\"u\")) None None None None true))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Dot(Var(\"U\" None) \"c\" None) Return(Constant(ConstInt(1) None)) \".case@1\"))\n"
      "    ])) \".switch@0\" [switch_case(Dot(Var(\"U\" None) \"c\" None) \".case@1\")] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestRules::test_rejected (call 115)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestRules::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"a\" Int) member_decl(\"c\" Char)]))\n"
      "  VarDecl(var_decl(\"U\" InitList([Constant(ConstInt(1) None)] None None) Const(Union(\"u\")) None None None None true))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Dot(Var(\"U\" None) \"c\" None) Return(Constant(ConstInt(1) None)) \".case@1\"))\n"
      "    ])) \".switch@0\" [switch_case(Dot(Var(\"U\" None) \"c\" None) \".case@1\")] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"case label is not an integer constant expression: member 'c' of a union constant isn't the member its initializer sets, and only that one is a constant (C23 \\u00a76.6p16)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestNamedConstants::test_a_named_zero_is_a_null_pointer_constant (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_a_named_zero_is_a_null_pointer_constant\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr int Z = 0; int *p = Z;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"Z\" Constant(ConstInt(0) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"p\" Var(\"Z\" None) Pointer(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestNamedConstants::test_a_named_zero_is_a_null_pointer_constant (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_a_named_zero_is_a_null_pointer_constant\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"Z\" Constant(ConstInt(0) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"p\" Var(\"Z\" None) Pointer(Int) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"Z\" Constant(ConstInt(0) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"p\" Var(\"Z\" None) Pointer(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestNamedConstants::test_a_named_zero_is_a_null_pointer_constant (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_a_named_zero_is_a_null_pointer_constant\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"Z\" Constant(ConstInt(0) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"p\" Var(\"Z\" None) Pointer(Int) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"Z\" Constant(ConstInt(0) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"p\" Var(\"Z\" None) Pointer(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestNamedConstants::test_a_named_zero_is_a_null_pointer_constant (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_a_named_zero_is_a_null_pointer_constant\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"Z\" Constant(ConstInt(0) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"p\" Var(\"Z\" None) Pointer(Int) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"Z\" Constant(ConstInt(0) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"p\" Var(\"Z\" None) Pointer(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestNamedConstants::test_a_named_zero_is_a_null_pointer_constant (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_a_named_zero_is_a_null_pointer_constant\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"Z\" Constant(ConstInt(0) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"p\" Var(\"Z\" None) Pointer(Int) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"Z\" Constant(ConstInt(0) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"p\" Var(\"Z\" None) Pointer(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestNamedConstants::test_a_named_zero_is_a_null_pointer_constant (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_a_named_zero_is_a_null_pointer_constant\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"Z\\\" Constant(ConstInt(0) Int) Const(Int) Static None None None true))\\n  VarDecl(var_decl(\\\"p\\\" Cast(Pointer(Int) Var(\\\"Z\\\" Const(Int)) Pointer(Int)) Pointer(Int) None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"Z\" Constant(ConstInt(0) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"p\" Var(\"Z\" None) Pointer(Int) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"Z\\\" Constant(ConstInt(0) Int) Const(Int) Static None None None true))\\n  VarDecl(var_decl(\\\"p\\\" Cast(Pointer(Int) Var(\\\"Z\\\" Const(Int)) Pointer(Int)) Pointer(Int) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"Z\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestNamedConstants::test_file_scope_has_internal_linkage (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_file_scope_has_internal_linkage\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr int K = 47;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(47) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestNamedConstants::test_file_scope_has_internal_linkage (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_file_scope_has_internal_linkage\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(47) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(47) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestNamedConstants::test_file_scope_has_internal_linkage (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_file_scope_has_internal_linkage\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(47) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(47) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestNamedConstants::test_file_scope_has_internal_linkage (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_file_scope_has_internal_linkage\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(47) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(47) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestNamedConstants::test_file_scope_has_internal_linkage (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_file_scope_has_internal_linkage\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(47) None) Const(Int) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(47) None) Const(Int) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestNamedConstants::test_file_scope_has_internal_linkage (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_file_scope_has_internal_linkage\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"K\\\" Constant(ConstInt(47) Int) Const(Int) Static None None None true))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(47) None) Const(Int) None None None None true))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"K\\\" Constant(ConstInt(47) Int) Const(Int) Static None None None true))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"K\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 47}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestNamedConstants::test_in_static_initializers (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_in_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nstruct in { char a; int b; };struct s { char x; struct in in; long y; };constexpr struct s S = { 3, { 4, -300 }, 70000 };constexpr int K = 47;static int b = K + 1;static long c = S.y + S.in.b;static struct s S3 = S;constexpr double D = 2.5;static int e = (int)D;constexpr double third = 1.0 / 3.0;static int i = (int)(third * 6.0);union u { unsigned w; unsigned char b[2]; };constexpr union u U = { 0x1234 };static unsigned d = U.w;\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"in\" false [member_decl(\"a\" Char) member_decl(\"b\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"x\" Char) member_decl(\"in\" Structure(\"in\")) member_decl(\"y\" Long)]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(3) None) InitList([Constant(ConstInt(4) None) Unary(Negate Constant(ConstInt(300) None) None)] None None) Constant(ConstLong(70000) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(47) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"b\" Binary(Add Var(\"K\" None) Constant(ConstInt(1) None) None) Int Static None None None false))\n"
      "  VarDecl(var_decl(\"c\" Binary(Add Dot(Var(\"S\" None) \"y\" None) Dot(Dot(Var(\"S\" None) \"in\" None) \"b\" None) None) Long Static None None None false))\n"
      "  VarDecl(var_decl(\"S3\" Var(\"S\" None) Structure(\"s\") Static None None None false))\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"e\" Cast(Int Var(\"D\" None) None) Int Static None None None false))\n"
      "  VarDecl(var_decl(\"third\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"i\" Cast(Int Binary(Multiply Var(\"third\" None) Constant(ConstDouble(4618441417868443648) None) None) None) Int Static None None None false))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"w\" UInt) member_decl(\"b\" Array(UChar 2))]))\n"
      "  VarDecl(var_decl(\"U\" InitList([Constant(ConstInt(4660) None)] None None) Const(Union(\"u\")) None None None None true))\n"
      "  VarDecl(var_decl(\"d\" Dot(Var(\"U\" None) \"w\" None) UInt Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestNamedConstants::test_in_static_initializers (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_in_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"in\" false [member_decl(\"a\" Char) member_decl(\"b\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"x\" Char) member_decl(\"in\" Structure(\"in\")) member_decl(\"y\" Long)]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(3) None) InitList([Constant(ConstInt(4) None) Unary(Negate Constant(ConstInt(300) None) None)] None None) Constant(ConstLong(70000) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(47) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"b\" Binary(Add Var(\"K\" None) Constant(ConstInt(1) None) None) Int Static None None None false))\n"
      "  VarDecl(var_decl(\"c\" Binary(Add Dot(Var(\"S\" None) \"y\" None) Dot(Dot(Var(\"S\" None) \"in\" None) \"b\" None) None) Long Static None None None false))\n"
      "  VarDecl(var_decl(\"S3\" Var(\"S\" None) Structure(\"s\") Static None None None false))\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"e\" Cast(Int Var(\"D\" None) None) Int Static None None None false))\n"
      "  VarDecl(var_decl(\"third\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"i\" Cast(Int Binary(Multiply Var(\"third\" None) Constant(ConstDouble(4618441417868443648) None) None) None) Int Static None None None false))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"w\" UInt) member_decl(\"b\" Array(UChar 2))]))\n"
      "  VarDecl(var_decl(\"U\" InitList([Constant(ConstInt(4660) None)] None None) Const(Union(\"u\")) None None None None true))\n"
      "  VarDecl(var_decl(\"d\" Dot(Var(\"U\" None) \"w\" None) UInt Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"in\" false [member_decl(\"a\" Char) member_decl(\"b\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"x\" Char) member_decl(\"in\" Structure(\"in\")) member_decl(\"y\" Long)]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(3) None) InitList([Constant(ConstInt(4) None) Unary(Negate Constant(ConstInt(300) None) None)] None None) Constant(ConstLong(70000) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(47) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"b\" Binary(Add Var(\"K\" None) Constant(ConstInt(1) None) None) Int Static None None None false))\n"
      "  VarDecl(var_decl(\"c\" Binary(Add Dot(Var(\"S\" None) \"y\" None) Dot(Dot(Var(\"S\" None) \"in\" None) \"b\" None) None) Long Static None None None false))\n"
      "  VarDecl(var_decl(\"S3\" Var(\"S\" None) Structure(\"s\") Static None None None false))\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"e\" Cast(Int Var(\"D\" None) None) Int Static None None None false))\n"
      "  VarDecl(var_decl(\"third\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"i\" Cast(Int Binary(Multiply Var(\"third\" None) Constant(ConstDouble(4618441417868443648) None) None) None) Int Static None None None false))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"w\" UInt) member_decl(\"b\" Array(UChar 2))]))\n"
      "  VarDecl(var_decl(\"U\" InitList([Constant(ConstInt(4660) None)] None None) Const(Union(\"u\")) None None None None true))\n"
      "  VarDecl(var_decl(\"d\" Dot(Var(\"U\" None) \"w\" None) UInt Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestNamedConstants::test_in_static_initializers (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_in_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"in\" false [member_decl(\"a\" Char) member_decl(\"b\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"x\" Char) member_decl(\"in\" Structure(\"in\")) member_decl(\"y\" Long)]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(3) None) InitList([Constant(ConstInt(4) None) Unary(Negate Constant(ConstInt(300) None) None)] None None) Constant(ConstLong(70000) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(47) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"b\" Binary(Add Var(\"K\" None) Constant(ConstInt(1) None) None) Int Static None None None false))\n"
      "  VarDecl(var_decl(\"c\" Binary(Add Dot(Var(\"S\" None) \"y\" None) Dot(Dot(Var(\"S\" None) \"in\" None) \"b\" None) None) Long Static None None None false))\n"
      "  VarDecl(var_decl(\"S3\" Var(\"S\" None) Structure(\"s\") Static None None None false))\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"e\" Cast(Int Var(\"D\" None) None) Int Static None None None false))\n"
      "  VarDecl(var_decl(\"third\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"i\" Cast(Int Binary(Multiply Var(\"third\" None) Constant(ConstDouble(4618441417868443648) None) None) None) Int Static None None None false))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"w\" UInt) member_decl(\"b\" Array(UChar 2))]))\n"
      "  VarDecl(var_decl(\"U\" InitList([Constant(ConstInt(4660) None)] None None) Const(Union(\"u\")) None None None None true))\n"
      "  VarDecl(var_decl(\"d\" Dot(Var(\"U\" None) \"w\" None) UInt Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"in\" false [member_decl(\"a\" Char) member_decl(\"b\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"x\" Char) member_decl(\"in\" Structure(\"in\")) member_decl(\"y\" Long)]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(3) None) InitList([Constant(ConstInt(4) None) Unary(Negate Constant(ConstInt(300) None) None)] None None) Constant(ConstLong(70000) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(47) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"b\" Binary(Add Var(\"K\" None) Constant(ConstInt(1) None) None) Int Static None None None false))\n"
      "  VarDecl(var_decl(\"c\" Binary(Add Dot(Var(\"S\" None) \"y\" None) Dot(Dot(Var(\"S\" None) \"in\" None) \"b\" None) None) Long Static None None None false))\n"
      "  VarDecl(var_decl(\"S3\" Var(\"S\" None) Structure(\"s\") Static None None None false))\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"e\" Cast(Int Var(\"D\" None) None) Int Static None None None false))\n"
      "  VarDecl(var_decl(\"third\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"i\" Cast(Int Binary(Multiply Var(\"third\" None) Constant(ConstDouble(4618441417868443648) None) None) None) Int Static None None None false))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"w\" UInt) member_decl(\"b\" Array(UChar 2))]))\n"
      "  VarDecl(var_decl(\"U\" InitList([Constant(ConstInt(4660) None)] None None) Const(Union(\"u\")) None None None None true))\n"
      "  VarDecl(var_decl(\"d\" Dot(Var(\"U\" None) \"w\" None) UInt Static None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestNamedConstants::test_in_static_initializers (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_in_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"in\" false [member_decl(\"a\" Char) member_decl(\"b\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"x\" Char) member_decl(\"in\" Structure(\"in\")) member_decl(\"y\" Long)]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(3) None) InitList([Constant(ConstInt(4) None) Unary(Negate Constant(ConstInt(300) None) None)] None None) Constant(ConstLong(70000) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(47) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"b\" Binary(Add Var(\"K\" None) Constant(ConstInt(1) None) None) Int Static None None None false))\n"
      "  VarDecl(var_decl(\"c\" Binary(Add Dot(Var(\"S\" None) \"y\" None) Dot(Dot(Var(\"S\" None) \"in\" None) \"b\" None) None) Long Static None None None false))\n"
      "  VarDecl(var_decl(\"S3\" Var(\"S\" None) Structure(\"s\") Static None None None false))\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"e\" Cast(Int Var(\"D\" None) None) Int Static None None None false))\n"
      "  VarDecl(var_decl(\"third\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"i\" Cast(Int Binary(Multiply Var(\"third\" None) Constant(ConstDouble(4618441417868443648) None) None) None) Int Static None None None false))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"w\" UInt) member_decl(\"b\" Array(UChar 2))]))\n"
      "  VarDecl(var_decl(\"U\" InitList([Constant(ConstInt(4660) None)] None None) Const(Union(\"u\")) None None None None true))\n"
      "  VarDecl(var_decl(\"d\" Dot(Var(\"U\" None) \"w\" None) UInt Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"in\" false [member_decl(\"a\" Char) member_decl(\"b\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"x\" Char) member_decl(\"in\" Structure(\"in\")) member_decl(\"y\" Long)]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(3) None) InitList([Constant(ConstInt(4) None) Unary(Negate Constant(ConstInt(300) None) None)] None None) Constant(ConstLong(70000) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(47) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"b\" Binary(Add Var(\"K\" None) Constant(ConstInt(1) None) None) Int Static None None None false))\n"
      "  VarDecl(var_decl(\"c\" Binary(Add Dot(Var(\"S\" None) \"y\" None) Dot(Dot(Var(\"S\" None) \"in\" None) \"b\" None) None) Long Static None None None false))\n"
      "  VarDecl(var_decl(\"S3\" Var(\"S\" None) Structure(\"s\") Static None None None false))\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"e\" Cast(Int Var(\"D\" None) None) Int Static None None None false))\n"
      "  VarDecl(var_decl(\"third\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"i\" Cast(Int Binary(Multiply Var(\"third\" None) Constant(ConstDouble(4618441417868443648) None) None) None) Int Static None None None false))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"w\" UInt) member_decl(\"b\" Array(UChar 2))]))\n"
      "  VarDecl(var_decl(\"U\" InitList([Constant(ConstInt(4660) None)] None None) Const(Union(\"u\")) None None None None true))\n"
      "  VarDecl(var_decl(\"d\" Dot(Var(\"U\" None) \"w\" None) UInt Static None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestNamedConstants::test_in_static_initializers (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_in_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"in\" false [member_decl(\"a\" Char) member_decl(\"b\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"x\" Char) member_decl(\"in\" Structure(\"in\")) member_decl(\"y\" Long)]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(3) None) InitList([Constant(ConstInt(4) None) Unary(Negate Constant(ConstInt(300) None) None)] None None) Constant(ConstLong(70000) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(47) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"b\" Binary(Add Var(\"K\" None) Constant(ConstInt(1) None) None) Int Static None None None false))\n"
      "  VarDecl(var_decl(\"c\" Binary(Add Dot(Var(\"S\" None) \"y\" None) Dot(Dot(Var(\"S\" None) \"in\" None) \"b\" None) None) Long Static None None None false))\n"
      "  VarDecl(var_decl(\"S3\" Var(\"S\" None) Structure(\"s\") Static None None None false))\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"e\" Cast(Int Var(\"D\" None) None) Int Static None None None false))\n"
      "  VarDecl(var_decl(\"third\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"i\" Cast(Int Binary(Multiply Var(\"third\" None) Constant(ConstDouble(4618441417868443648) None) None) None) Int Static None None None false))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"w\" UInt) member_decl(\"b\" Array(UChar 2))]))\n"
      "  VarDecl(var_decl(\"U\" InitList([Constant(ConstInt(4660) None)] None None) Const(Union(\"u\")) None None None None true))\n"
      "  VarDecl(var_decl(\"d\" Dot(Var(\"U\" None) \"w\" None) UInt Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"in\" false [member_decl(\"a\" Char) member_decl(\"b\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"x\" Char) member_decl(\"in\" Structure(\"in\")) member_decl(\"y\" Long)]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(3) None) InitList([Constant(ConstInt(4) None) Unary(Negate Constant(ConstInt(300) None) None)] None None) Constant(ConstLong(70000) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(47) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"b\" Binary(Add Var(\"K\" None) Constant(ConstInt(1) None) None) Int Static None None None false))\n"
      "  VarDecl(var_decl(\"c\" Binary(Add Dot(Var(\"S\" None) \"y\" None) Dot(Dot(Var(\"S\" None) \"in\" None) \"b\" None) None) Long Static None None None false))\n"
      "  VarDecl(var_decl(\"S3\" Var(\"S\" None) Structure(\"s\") Static None None None false))\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"e\" Cast(Int Var(\"D\" None) None) Int Static None None None false))\n"
      "  VarDecl(var_decl(\"third\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"i\" Cast(Int Binary(Multiply Var(\"third\" None) Constant(ConstDouble(4618441417868443648) None) None) None) Int Static None None None false))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"w\" UInt) member_decl(\"b\" Array(UChar 2))]))\n"
      "  VarDecl(var_decl(\"U\" InitList([Constant(ConstInt(4660) None)] None None) Const(Union(\"u\")) None None None None true))\n"
      "  VarDecl(var_decl(\"d\" Dot(Var(\"U\" None) \"w\" None) UInt Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestNamedConstants::test_in_static_initializers (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_in_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"in\\\" false [member_decl(\\\"a\\\" Char) member_decl(\\\"b\\\" Int)]))\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"x\\\" Char) member_decl(\\\"in\\\" Structure(\\\"in\\\")) member_decl(\\\"y\\\" Long)]))\\n  VarDecl(var_decl(\\\"S\\\" InitList([Cast(Char Constant(ConstInt(3) Int) Char) InitList([Cast(Char Constant(ConstInt(4) Int) Char) Unary(Negate Constant(ConstInt(300) Int) Int)] Structure(\\\"in\\\") None) Constant(ConstLong(70000) Long)] Structure(\\\"s\\\") None) Const(Structure(\\\"s\\\")) Static None None None true))\\n  VarDecl(var_decl(\\\"K\\\" Constant(ConstInt(47) Int) Const(Int) Static None None None true))\\n  VarDecl(var_decl(\\\"b\\\" Binary(Add Var(\\\"K\\\" Const(Int)) Constant(ConstInt(1) Int) Int) Int Static None None None false))\\n  VarDecl(var_decl(\\\"c\\\" Binary(Add Dot(Var(\\\"S\\\" Const(Structure(\\\"s\\\"))) \\\"y\\\" Const(Long)) Cast(Long Dot(Dot(Var(\\\"S\\\" Const(Structure(\\\"s\\\"))) \\\"in\\\" Const(Structure(\\\"in\\\"))) \\\"b\\\" Const(Int)) Long) Long) Long Static None None None false))\\n  VarDecl(var_decl(\\\"S3\\\" Var(\\\"S\\\" Const(Structure(\\\"s\\\"))) Structure(\\\"s\\\") Static None None None false))\\n  VarDecl(var_decl(\\\"D\\\" Constant(ConstDouble(4612811918334230528) Double) Const(Double) Static None None None true))\\n  VarDecl(var_decl(\\\"e\\\" Cast(Int Var(\\\"D\\\" Const(Double)) Int) Int Static None None None false))\\n  VarDecl(var_decl(\\\"third\\\" Binary(Divide Constant(ConstDouble(4607182418800017408) Double) Constant(ConstDouble(4613937818241073152) Double) Double) Const(Double) Static None None None true))\\n  VarDecl(var_decl(\\\"i\\\" Cast(Int Binary(Multiply Var(\\\"third\\\" Const(Double)) Constant(ConstDouble(4618441417868443648) Double) Double) Int) Int Static None None None false))\\n  StructDecl(struct_decl(\\\"u\\\" true [member_decl(\\\"w\\\" UInt) member_decl(\\\"b\\\" Array(UChar 2))]))\\n  VarDecl(var_decl(\\\"U\\\" InitList([Cast(UInt Constant(ConstInt(4660) Int) UInt)] Union(\\\"u\\\") None) Const(Union(\\\"u\\\")) Static None None None true))\\n  VarDecl(var_decl(\\\"d\\\" Dot(Var(\\\"U\\\" Const(Union(\\\"u\\\"))) \\\"w\\\" Const(UInt)) UInt Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"in\" false [member_decl(\"a\" Char) member_decl(\"b\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"x\" Char) member_decl(\"in\" Structure(\"in\")) member_decl(\"y\" Long)]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(3) None) InitList([Constant(ConstInt(4) None) Unary(Negate Constant(ConstInt(300) None) None)] None None) Constant(ConstLong(70000) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(47) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"b\" Binary(Add Var(\"K\" None) Constant(ConstInt(1) None) None) Int Static None None None false))\n"
      "  VarDecl(var_decl(\"c\" Binary(Add Dot(Var(\"S\" None) \"y\" None) Dot(Dot(Var(\"S\" None) \"in\" None) \"b\" None) None) Long Static None None None false))\n"
      "  VarDecl(var_decl(\"S3\" Var(\"S\" None) Structure(\"s\") Static None None None false))\n"
      "  VarDecl(var_decl(\"D\" Constant(ConstDouble(4612811918334230528) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"e\" Cast(Int Var(\"D\" None) None) Int Static None None None false))\n"
      "  VarDecl(var_decl(\"third\" Binary(Divide Constant(ConstDouble(4607182418800017408) None) Constant(ConstDouble(4613937818241073152) None) None) Const(Double) None None None None true))\n"
      "  VarDecl(var_decl(\"i\" Cast(Int Binary(Multiply Var(\"third\" None) Constant(ConstDouble(4618441417868443648) None) None) None) Int Static None None None false))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"w\" UInt) member_decl(\"b\" Array(UChar 2))]))\n"
      "  VarDecl(var_decl(\"U\" InitList([Constant(ConstInt(4660) None)] None None) Const(Union(\"u\")) None None None None true))\n"
      "  VarDecl(var_decl(\"d\" Dot(Var(\"U\" None) \"w\" None) UInt Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"in\\\" false [member_decl(\\\"a\\\" Char) member_decl(\\\"b\\\" Int)]))\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"x\\\" Char) member_decl(\\\"in\\\" Structure(\\\"in\\\")) member_decl(\\\"y\\\" Long)]))\\n  VarDecl(var_decl(\\\"S\\\" InitList([Cast(Char Constant(ConstInt(3) Int) Char) InitList([Cast(Char Constant(ConstInt(4) Int) Char) Unary(Negate Constant(ConstInt(300) Int) Int)] Structure(\\\"in\\\") None) Constant(ConstLong(70000) Long)] Structure(\\\"s\\\") None) Const(Structure(\\\"s\\\")) Static None None None true))\\n  VarDecl(var_decl(\\\"K\\\" Constant(ConstInt(47) Int) Const(Int) Static None None None true))\\n  VarDecl(var_decl(\\\"b\\\" Binary(Add Var(\\\"K\\\" Const(Int)) Constant(ConstInt(1) Int) Int) Int Static None None None false))\\n  VarDecl(var_decl(\\\"c\\\" Binary(Add Dot(Var(\\\"S\\\" Const(Structure(\\\"s\\\"))) \\\"y\\\" Const(Long)) Cast(Long Dot(Dot(Var(\\\"S\\\" Const(Structure(\\\"s\\\"))) \\\"in\\\" Const(Structure(\\\"in\\\"))) \\\"b\\\" Const(Int)) Long) Long) Long Static None None None false))\\n  VarDecl(var_decl(\\\"S3\\\" Var(\\\"S\\\" Const(Structure(\\\"s\\\"))) Structure(\\\"s\\\") Static None None None false))\\n  VarDecl(var_decl(\\\"D\\\" Constant(ConstDouble(4612811918334230528) Double) Const(Double) Static None None None true))\\n  VarDecl(var_decl(\\\"e\\\" Cast(Int Var(\\\"D\\\" Const(Double)) Int) Int Static None None None false))\\n  VarDecl(var_decl(\\\"third\\\" Binary(Divide Constant(ConstDouble(4607182418800017408) Double) Constant(ConstDouble(4613937818241073152) Double) Double) Const(Double) Static None None None true))\\n  VarDecl(var_decl(\\\"i\\\" Cast(Int Binary(Multiply Var(\\\"third\\\" Const(Double)) Constant(ConstDouble(4618441417868443648) Double) Double) Int) Int Static None None None false))\\n  StructDecl(struct_decl(\\\"u\\\" true [member_decl(\\\"w\\\" UInt) member_decl(\\\"b\\\" Array(UChar 2))]))\\n  VarDecl(var_decl(\\\"U\\\" InitList([Cast(UInt Constant(ConstInt(4660) Int) UInt)] Union(\\\"u\\\") None) Const(Union(\\\"u\\\")) Static None None None true))\\n  VarDecl(var_decl(\\\"d\\\" Dot(Var(\\\"U\\\" Const(Union(\\\"u\\\"))) \\\"w\\\" Const(UInt)) UInt Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"S\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Structure(\\\"s\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [3, {\"tuple\": [4, -300]}, 70000]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"K\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 47}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 48}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs"
      "\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 69700}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"S3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Structure(\\\"s\\\")\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [3, {\"tuple\": [4, -300]}, 70000]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"D\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Double)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 4612811918334230528}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"e\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 2}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"third\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Double)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 4599676419421066581}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 2}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"U\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Union(\\\"u\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [4660]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 4660}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"in\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"in\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\","
      " \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 1}}], \"size\": 3, \"complete\": true}}], [\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"x\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"in\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Structure(\\\"in\\\")\\n\"}, \"byte_offset\": 1}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"y\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"byte_offset\": 4}}], \"size\": 8, \"complete\": true}}], [\"u\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"u\", \"is_union\": true, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"w\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 2)\\n\"}, \"byte_offset\": 0}}], \"size\": 2, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestNamedConstants::test_where_the_parser_folds (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_where_the_parser_folds\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr int N = 3;struct s { int n; }; constexpr struct s S = {4};enum e { A = N + 1 };int a[A][S.n];_BitInt(N + 2) x;void f(void) { int N = 5; int b[N]; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"N\" Constant(ConstInt(3) None) Const(Int) None None None None true))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Int)]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(4) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" None Array(Array(Int 4) 4) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None BitInt(5 true) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"N\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" None Array(Int 3) None None None None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestNamedConstants::test_where_the_parser_folds (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_where_the_parser_folds\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"N\" Constant(ConstInt(3) None) Const(Int) None None None None true))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Int)]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(4) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" None Array(Array(Int 4) 4) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None BitInt(5 true) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"N\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" None Array(Int 3) None None None None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"N\" Constant(ConstInt(3) None) Const(Int) None None None None true))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Int)]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(4) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" None Array(Array(Int 4) 4) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None BitInt(5 true) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.N\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" None Array(Int 3) None None None None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestNamedConstants::test_where_the_parser_folds (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_where_the_parser_folds\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"N\" Constant(ConstInt(3) None) Const(Int) None None None None true))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Int)]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(4) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" None Array(Array(Int 4) 4) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None BitInt(5 true) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.N\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" None Array(Int 3) None None None None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"N\" Constant(ConstInt(3) None) Const(Int) None None None None true))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Int)]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(4) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" None Array(Array(Int 4) 4) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None BitInt(5 true) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.N\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" None Array(Int 3) None None None None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestNamedConstants::test_where_the_parser_folds (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_where_the_parser_folds\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"N\" Constant(ConstInt(3) None) Const(Int) None None None None true))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Int)]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(4) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" None Array(Array(Int 4) 4) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None BitInt(5 true) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.N\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" None Array(Int 3) None None None None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"N\" Constant(ConstInt(3) None) Const(Int) None None None None true))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Int)]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(4) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" None Array(Array(Int 4) 4) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None BitInt(5 true) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.N\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" None Array(Int 3) None None None None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestNamedConstants::test_where_the_parser_folds (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_where_the_parser_folds\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"N\" Constant(ConstInt(3) None) Const(Int) None None None None true))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Int)]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(4) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" None Array(Array(Int 4) 4) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None BitInt(5 true) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.N\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" None Array(Int 3) None None None None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"N\" Constant(ConstInt(3) None) Const(Int) None None None None true))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Int)]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(4) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" None Array(Array(Int 4) 4) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None BitInt(5 true) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.N\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" None Array(Int 3) None None None None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestNamedConstants::test_where_the_parser_folds (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestNamedConstants::test_where_the_parser_folds\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"N\\\" Constant(ConstInt(3) Int) Const(Int) Static None None None true))\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"n\\\" Int)]))\\n  VarDecl(var_decl(\\\"S\\\" InitList([Constant(ConstInt(4) Int)] Structure(\\\"s\\\") None) Const(Structure(\\\"s\\\")) Static None None None true))\\n  VarDecl(var_decl(\\\"a\\\" None Array(Array(Int 4) 4) None None None None false))\\n  VarDecl(var_decl(\\\"x\\\" None BitInt(5 true) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.N\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" None Array(Int 3) None None None None false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"N\" Constant(ConstInt(3) None) Const(Int) None None None None true))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Int)]))\n"
      "  VarDecl(var_decl(\"S\" InitList([Constant(ConstInt(4) None)] None None) Const(Structure(\"s\")) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" None Array(Array(Int 4) 4) None None None None false))\n"
      "  VarDecl(var_decl(\"x\" None BitInt(5 true) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.N\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" None Array(Int 3) None None None None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"N\\\" Constant(ConstInt(3) Int) Const(Int) Static None None None true))\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"n\\\" Int)]))\\n  VarDecl(var_decl(\\\"S\\\" InitList([Constant(ConstInt(4) Int)] Structure(\\\"s\\\") None) Const(Structure(\\\"s\\\")) Static None None None true))\\n  VarDecl(var_decl(\\\"a\\\" None Array(Array(Int 4) 4) None None None None false))\\n  VarDecl(var_decl(\\\"x\\\" None BitInt(5 true) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.N\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" None Array(Int 3) None None None None false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"N\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 3}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"S\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Structure(\\\"s\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [4]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Int 4) 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"BitInt(5 true)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.N\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"objec"
      "t\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"n\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 0}}], \"size\": 2, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint f(void) { return (constexpr int){5}; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"\" Const(Int) Constant(ConstInt(5) None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"\" Const(Int) Constant(ConstInt(5) None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(5) None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(5) None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(5) None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(5) None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(5) None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(5) None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(5) None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(CompoundLiteral(\\\"@0.compound_literal\\\" Const(Int) Constant(ConstInt(5) Int) Const(Int) None true)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(5) None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(CompoundLiteral(\\\"@0.compound_literal\\\" Const(Int) Constant(ConstInt(5) Int) Const(Int) None true)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.compound_literal\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint a[(constexpr int){3}];\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 3) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 3) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 3) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 3) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 3) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None Array(Int 3) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nstruct pt { int x, y; }; constexpr struct pt P = (constexpr struct pt){1, 2};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"P\" CompoundLiteral(\"\" Const(Structure(\"pt\")) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None true) Const(Structure(\"pt\")) None None None None true))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"P\" CompoundLiteral(\"\" Const(Structure(\"pt\")) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None true) Const(Structure(\"pt\")) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"P\" CompoundLiteral(\"@0.compound_literal\" Const(Structure(\"pt\")) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None true) Const(Structure(\"pt\")) None None None None true))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"P\" CompoundLiteral(\"@0.compound_literal\" Const(Structure(\"pt\")) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None true) Const(Structure(\"pt\")) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"P\" CompoundLiteral(\"@0.compound_literal\" Const(Structure(\"pt\")) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None true) Const(Structure(\"pt\")) None None None None true))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"P\" CompoundLiteral(\"@0.compound_literal\" Const(Structure(\"pt\")) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None true) Const(Structure(\"pt\")) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"P\" CompoundLiteral(\"@0.compound_literal\" Const(Structure(\"pt\")) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None true) Const(Structure(\"pt\")) None None None None true))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"P\" CompoundLiteral(\"@0.compound_literal\" Const(Structure(\"pt\")) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None true) Const(Structure(\"pt\")) None None None None true))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"P\" CompoundLiteral(\"@0.compound_literal\" Const(Structure(\"pt\")) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None true) Const(Structure(\"pt\")) None None None None true))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"pt\\\" false [member_decl(\\\"x\\\" Int) member_decl(\\\"y\\\" Int)]))\\n  VarDecl(var_decl(\\\"P\\\" CompoundLiteral(\\\"@0.compound_literal\\\" Const(Structure(\\\"pt\\\")) InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Structure(\\\"pt\\\") None) Const(Structure(\\\"pt\\\")) None true) Const(Structure(\\\"pt\\\")) Static None None None true))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"P\" CompoundLiteral(\"@0.compound_literal\" Const(Structure(\"pt\")) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None true) Const(Structure(\"pt\")) None None None None true))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"pt\\\" false [member_decl(\\\"x\\\" Int) member_decl(\\\"y\\\" Int)]))\\n  VarDecl(var_decl(\\\"P\\\" CompoundLiteral(\\\"@0.compound_literal\\\" Const(Structure(\\\"pt\\\")) InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Structure(\\\"pt\\\") None) Const(Structure(\\\"pt\\\")) None true) Const(Structure(\\\"pt\\\")) Static None None None true))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.compound_literal\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Structure(\\\"pt\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [1, 2]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"P\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Structure(\\\"pt\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [1, 2]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"pt\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"pt\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"x\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"y\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 2}}], \"size\": 4, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 19)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nstruct pt { int x, y; }; int a[(constexpr struct pt){1, 2}.y];\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 2) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 20)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 2) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 2) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 21)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 2) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 2) None None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 22)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 2) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 2) None None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 23)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 2) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 2) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 24)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 2) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"pt\\\" false [member_decl(\\\"x\\\" Int) member_decl(\\\"y\\\" Int)]))\\n  VarDecl(var_decl(\\\"a\\\" None Array(Int 2) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"pt\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"pt\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"x\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"y\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 2}}], \"size\": 4, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 25)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint *f(void) { return (static int[]){1, 2}; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"\" Array(Int 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None Static false)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 26)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"\" Array(Int 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None Static false)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Array(Int 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None Static false)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 27)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Array(Int 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None Static false)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Array(Int 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None Static false)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 28)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Array(Int 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None Static false)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Array(Int 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None Static false)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 29)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Array(Int 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None Static false)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Array(Int 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None Static false)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 30)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(AddressOf(CompoundLiteral(\\\"@0.compound_literal\\\" Array(Int 2) InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Array(Int 2) None) Array(Int 2) Static false) Pointer(Int))))\\n  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Array(Int 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None Static false)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(AddressOf(CompoundLiteral(\\\"@0.compound_literal\\\" Array(Int 2) InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Array(Int 2) None) Array(Int 2) Static false) Pointer(Int))))\\n  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Pointer(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.compound_literal\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [1, 2]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 31)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint *p = (static int[]){1, 2};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" CompoundLiteral(\"\" Array(Int 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None Static false) Pointer(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 32)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" CompoundLiteral(\"\" Array(Int 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None Static false) Pointer(Int) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" CompoundLiteral(\"@0.compound_literal\" Array(Int 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None Static false) Pointer(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 33)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" CompoundLiteral(\"@0.compound_literal\" Array(Int 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None Static false) Pointer(Int) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" CompoundLiteral(\"@0.compound_literal\" Array(Int 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None Static false) Pointer(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 34)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" CompoundLiteral(\"@0.compound_literal\" Array(Int 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None Static false) Pointer(Int) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" CompoundLiteral(\"@0.compound_literal\" Array(Int 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None Static false) Pointer(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 35)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" CompoundLiteral(\"@0.compound_literal\" Array(Int 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None Static false) Pointer(Int) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" CompoundLiteral(\"@0.compound_literal\" Array(Int 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None Static false) Pointer(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 36)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" AddressOf(CompoundLiteral(\\\"@0.compound_literal\\\" Array(Int 2) InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Array(Int 2) None) Array(Int 2) Static false) Pointer(Int)) Pointer(Int) None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" CompoundLiteral(\"@0.compound_literal\" Array(Int 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None Static false) Pointer(Int) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"p\\\" AddressOf(CompoundLiteral(\\\"@0.compound_literal\\\" Array(Int 2) InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Array(Int 2) None) Array(Int 2) Static false) Pointer(Int)) Pointer(Int) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.compound_literal\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [1, 2]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"@0.compound_literal\", \"offset\": 0}}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 37)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nstatic int s = (constexpr int){4} * 2;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" Binary(Multiply CompoundLiteral(\"\" Const(Int) Constant(ConstInt(4) None) None None true) Constant(ConstInt(2) None) None) Int Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 38)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" Binary(Multiply CompoundLiteral(\"\" Const(Int) Constant(ConstInt(4) None) None None true) Constant(ConstInt(2) None) None) Int Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" Binary(Multiply CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(4) None) None None true) Constant(ConstInt(2) None) None) Int Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 39)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" Binary(Multiply CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(4) None) None None true) Constant(ConstInt(2) None) None) Int Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" Binary(Multiply CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(4) None) None None true) Constant(ConstInt(2) None) None) Int Static None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 40)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" Binary(Multiply CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(4) None) None None true) Constant(ConstInt(2) None) None) Int Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" Binary(Multiply CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(4) None) None None true) Constant(ConstInt(2) None) None) Int Static None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 41)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" Binary(Multiply CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(4) None) None None true) Constant(ConstInt(2) None) None) Int Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" Binary(Multiply CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(4) None) None None true) Constant(ConstInt(2) None) None) Int Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 42)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"s\\\" Binary(Multiply CompoundLiteral(\\\"@0.compound_literal\\\" Const(Int) Constant(ConstInt(4) Int) Const(Int) None true) Constant(ConstInt(2) Int) Int) Int Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"s\" Binary(Multiply CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(4) None) None None true) Constant(ConstInt(2) None) None) Int Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"s\\\" Binary(Multiply CompoundLiteral(\\\"@0.compound_literal\\\" Const(Int) Constant(ConstInt(4) Int) Const(Int) None true) Constant(ConstInt(2) Int) Int) Int Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.compound_literal\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 4}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 8}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 43)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint f(void) { return (static constexpr int){4} + (register constexpr unsigned char){5}; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Add CompoundLiteral(\"\" Const(Int) Constant(ConstInt(4) None) None Static true) CompoundLiteral(\"\" Const(UChar) Constant(ConstInt(5) None) None Register true) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 44)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Add CompoundLiteral(\"\" Const(Int) Constant(ConstInt(4) None) None Static true) CompoundLiteral(\"\" Const(UChar) Constant(ConstInt(5) None) None Register true) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Add CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(4) None) None Static true) CompoundLiteral(\"@1.compound_literal\" Const(UChar) Constant(ConstInt(5) None) None Register true) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 45)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Add CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(4) None) None Static true) CompoundLiteral(\"@1.compound_literal\" Const(UChar) Constant(ConstInt(5) None) None Register true) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Add CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(4) None) None Static true) CompoundLiteral(\"@1.compound_literal\" Const(UChar) Constant(ConstInt(5) None) None Register true) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 46)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Add CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(4) None) None Static true) CompoundLiteral(\"@1.compound_literal\" Const(UChar) Constant(ConstInt(5) None) None Register true) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Add CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(4) None) None Static true) CompoundLiteral(\"@1.compound_literal\" Const(UChar) Constant(ConstInt(5) None) None Register true) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 47)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Add CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(4) None) None Static true) CompoundLiteral(\"@1.compound_literal\" Const(UChar) Constant(ConstInt(5) None) None Register true) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Add CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(4) None) None Static true) CompoundLiteral(\"@1.compound_literal\" Const(UChar) Constant(ConstInt(5) None) None Register true) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestCompoundLiteralStorage::test_accepted (call 48)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Add CompoundLiteral(\\\"@0.compound_literal\\\" Const(Int) Constant(ConstInt(4) Int) Const(Int) Static true) Cast(Int CompoundLiteral(\\\"@1.compound_literal\\\" Const(UChar) Cast(Const(UChar) Constant(ConstInt(5) Int) Const(UChar)) Const(UChar) Register true) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Add CompoundLiteral(\"@0.compound_literal\" Const(Int) Constant(ConstInt(4) None) None Static true) CompoundLiteral(\"@1.compound_literal\" Const(UChar) Constant(ConstInt(5) None) None Register true) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Binary(Add CompoundLiteral(\\\"@0.compound_literal\\\" Const(Int) Constant(ConstInt(4) Int) Const(Int) Static true) Cast(Int CompoundLiteral(\\\"@1.compound_literal\\\" Const(UChar) Cast(Const(UChar) Constant(ConstInt(5) Int) Const(UChar)) Const(UChar) Register true) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.compound_literal\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 4}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@1.compound_literal\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": true, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_parse (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_parse\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int *f(void) { return (static constexpr int[]){1}; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"\" Array(Const(Int) 0) InitList([Constant(ConstInt(1) None)] None None) None Static true)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nthread_local int x;\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`thread_local` gives each thread its own object, and c6502 has no threads, so it doesn't take `thread_local`\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\n_Thread_local int x;\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`_Thread_local` gives each thread its own object, and c6502 has no threads, so it doesn't take `_Thread_local`\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nstatic thread_local int x;\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`thread_local` gives each thread its own object, and c6502 has no threads, so it doesn't take `thread_local`\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint f(void) { return (thread_local int){1}; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`thread_local` gives each thread its own object, and c6502 has no threads, so it doesn't take `thread_local`\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint f(void) { return (extern int){1}; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`extern` can't go on a compound literal, which takes only `constexpr`, `static`, `register`, and `thread_local` (C23 \\u00a76.5.3.6p7)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint f(void) { return (typedef int){1}; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`typedef` can't go on a compound literal, which takes only `constexpr`, `static`, `register`, and `thread_local` (C23 \\u00a76.5.3.6p7)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint f(void) { return (auto int){1}; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`auto` can't go on a compound literal, which takes only `constexpr`, `static`, `register`, and `thread_local` (C23 \\u00a76.5.3.6p7)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint f(void) { return (static register int){1}; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"a compound literal can't be both `static` and `register`\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint f(void) { return (static static int){1}; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`static` appears twice in a compound literal\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nunsigned char *f(void) { return &(register unsigned char){1}; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(AddressOf(CompoundLiteral(\"\" UChar Constant(ConstInt(1) None) None Register false) None)))\n"
      "  ]) FunType([] Pointer(UChar)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(AddressOf(CompoundLiteral(\"\" UChar Constant(ConstInt(1) None) None Register false) None)))\n"
      "  ]) FunType([] Pointer(UChar)) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(AddressOf(CompoundLiteral(\"@0.compound_literal\" UChar Constant(ConstInt(1) None) None Register false) None)))\n"
      "  ]) FunType([] Pointer(UChar)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(AddressOf(CompoundLiteral(\"@0.compound_literal\" UChar Constant(ConstInt(1) None) None Register false) None)))\n"
      "  ]) FunType([] Pointer(UChar)) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(AddressOf(CompoundLiteral(\"@0.compound_literal\" UChar Constant(ConstInt(1) None) None Register false) None)))\n"
      "  ]) FunType([] Pointer(UChar)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(AddressOf(CompoundLiteral(\"@0.compound_literal\" UChar Constant(ConstInt(1) None) None Register false) None)))\n"
      "  ]) FunType([] Pointer(UChar)) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(AddressOf(CompoundLiteral(\"@0.compound_literal\" UChar Constant(ConstInt(1) None) None Register false) None)))\n"
      "  ]) FunType([] Pointer(UChar)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(AddressOf(CompoundLiteral(\"@0.compound_literal\" UChar Constant(ConstInt(1) None) None Register false) None)))\n"
      "  ]) FunType([] Pointer(UChar)) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(AddressOf(CompoundLiteral(\"@0.compound_literal\" UChar Constant(ConstInt(1) None) None Register false) None)))\n"
      "  ]) FunType([] Pointer(UChar)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(AddressOf(CompoundLiteral(\"@0.compound_literal\" UChar Constant(ConstInt(1) None) None Register false) None)))\n"
      "  ]) FunType([] Pointer(UChar)) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"can't take the address of a `register` compound literal (C99 \\u00a76.5.3.2.1)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint x = (register int){1};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" CompoundLiteral(\"\" Int Constant(ConstInt(1) None) None Register false) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" CompoundLiteral(\"\" Int Constant(ConstInt(1) None) None Register false) Int None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" CompoundLiteral(\"@0.compound_literal\" Int Constant(ConstInt(1) None) None Register false) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" CompoundLiteral(\"@0.compound_literal\" Int Constant(ConstInt(1) None) None Register false) Int None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" CompoundLiteral(\"@0.compound_literal\" Int Constant(ConstInt(1) None) None Register false) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 19)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" CompoundLiteral(\"@0.compound_literal\" Int Constant(ConstInt(1) None) None Register false) Int None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" CompoundLiteral(\"@0.compound_literal\" Int Constant(ConstInt(1) None) None Register false) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 20)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" CompoundLiteral(\"@0.compound_literal\" Int Constant(ConstInt(1) None) None Register false) Int None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" CompoundLiteral(\"@0.compound_literal\" Int Constant(ConstInt(1) None) None Register false) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 21)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" CompoundLiteral(\"@0.compound_literal\" Int Constant(ConstInt(1) None) None Register false) Int None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"a compound literal at file scope can't be `register` (C23 \\u00a76.5.3.6p4)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 22)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint a[(static int){3}];\\n\"\n"
      "output-error\n"
      "{\"type\": \"NotImplementedError\", \"message\": \"array size must be an integer constant expression: expression is not an integer constant expression: CompoundLiteral(name='@fold1.compound_literal', target_type=Int(), init=Constant(const=ConstInt(value=3), data_type=Int()), data_type=Int(), storage_class=Static(), is_constexpr=False)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 23)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint f(void) { return (constexpr unsigned){-1}; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"\" Const(UInt) Unary(Negate Constant(ConstInt(1) None) None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 24)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"\" Const(UInt) Unary(Negate Constant(ConstInt(1) None) None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Const(UInt) Unary(Negate Constant(ConstInt(1) None) None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 25)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Const(UInt) Unary(Negate Constant(ConstInt(1) None) None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Const(UInt) Unary(Negate Constant(ConstInt(1) None) None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 26)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Const(UInt) Unary(Negate Constant(ConstInt(1) None) None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Const(UInt) Unary(Negate Constant(ConstInt(1) None) None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 27)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Const(UInt) Unary(Negate Constant(ConstInt(1) None) None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Const(UInt) Unary(Negate Constant(ConstInt(1) None) None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 28)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Const(UInt) Unary(Negate Constant(ConstInt(1) None) None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the initializer of `constexpr` object '(compound literal)' has the value -1, which unsigned int can't hold exactly, and a constexpr initializer can't change a value (C23 \\u00a76.7.2p5)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 29)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint g; int f(void) { return *(constexpr int *){&g}; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Dereference(CompoundLiteral(\"\" Const(Pointer(Int)) AddressOf(Var(\"g\" None) None) None None true) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 30)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Dereference(CompoundLiteral(\"\" Const(Pointer(Int)) AddressOf(Var(\"g\" None) None) None None true) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Dereference(CompoundLiteral(\"@0.compound_literal\" Const(Pointer(Int)) AddressOf(Var(\"g\" None) None) None None true) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 31)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Dereference(CompoundLiteral(\"@0.compound_literal\" Const(Pointer(Int)) AddressOf(Var(\"g\" None) None) None None true) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Dereference(CompoundLiteral(\"@0.compound_literal\" Const(Pointer(Int)) AddressOf(Var(\"g\" None) None) None None true) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 32)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Dereference(CompoundLiteral(\"@0.compound_literal\" Const(Pointer(Int)) AddressOf(Var(\"g\" None) None) None None true) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Dereference(CompoundLiteral(\"@0.compound_literal\" Const(Pointer(Int)) AddressOf(Var(\"g\" None) None) None None true) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 33)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Dereference(CompoundLiteral(\"@0.compound_literal\" Const(Pointer(Int)) AddressOf(Var(\"g\" None) None) None None true) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Dereference(CompoundLiteral(\"@0.compound_literal\" Const(Pointer(Int)) AddressOf(Var(\"g\" None) None) None None true) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 34)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"g\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Dereference(CompoundLiteral(\"@0.compound_literal\" Const(Pointer(Int)) AddressOf(Var(\"g\" None) None) None None true) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"`constexpr` object '(compound literal)' holds a pointer that isn't null, and a constexpr pointer must be null (C23 \\u00a76.7.2p6)\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 35)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint f(void) { int x = 5; return (constexpr int){x}; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Return(CompoundLiteral(\"\" Const(Int) Var(\"x\" None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 36)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Return(CompoundLiteral(\"\" Const(Int) Var(\"x\" None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Return(CompoundLiteral(\"@1.compound_literal\" Const(Int) Var(\"@0.x\" None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 37)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Return(CompoundLiteral(\"@1.compound_literal\" Const(Int) Var(\"@0.x\" None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Return(CompoundLiteral(\"@1.compound_literal\" Const(Int) Var(\"@0.x\" None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 38)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Return(CompoundLiteral(\"@1.compound_literal\" Const(Int) Var(\"@0.x\" None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Return(CompoundLiteral(\"@1.compound_literal\" Const(Int) Var(\"@0.x\" None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 39)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Return(CompoundLiteral(\"@1.compound_literal\" Const(Int) Var(\"@0.x\" None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Return(CompoundLiteral(\"@1.compound_literal\" Const(Int) Var(\"@0.x\" None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 40)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Return(CompoundLiteral(\"@1.compound_literal\" Const(Int) Var(\"@0.x\" None) None None true)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the initializer of `constexpr` object '(compound literal)' isn't an integer constant expression (C23 \\u00a76.7.2p6): expression is not an integer constant expression: Var(name='@0.x', data_type=Int())\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 41)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint f(int x) { return *(static int[]){x}; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    S(Return(Dereference(CompoundLiteral(\"\" Array(Int 0) InitList([Var(\"x\" None)] None None) None Static false) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 42)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    S(Return(Dereference(CompoundLiteral(\"\" Array(Int 0) InitList([Var(\"x\" None)] None None) None Static false) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(Return(Dereference(CompoundLiteral(\"@1.compound_literal\" Array(Int 0) InitList([Var(\"@0.x\" None)] None None) None Static false) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 43)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(Return(Dereference(CompoundLiteral(\"@1.compound_literal\" Array(Int 0) InitList([Var(\"@0.x\" None)] None None) None Static false) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(Return(Dereference(CompoundLiteral(\"@1.compound_literal\" Array(Int 0) InitList([Var(\"@0.x\" None)] None None) None Static false) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 44)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(Return(Dereference(CompoundLiteral(\"@1.compound_literal\" Array(Int 0) InitList([Var(\"@0.x\" None)] None None) None Static false) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(Return(Dereference(CompoundLiteral(\"@1.compound_literal\" Array(Int 0) InitList([Var(\"@0.x\" None)] None None) None Static false) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 45)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(Return(Dereference(CompoundLiteral(\"@1.compound_literal\" Array(Int 0) InitList([Var(\"@0.x\" None)] None None) None Static false) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(Return(Dereference(CompoundLiteral(\"@1.compound_literal\" Array(Int 0) InitList([Var(\"@0.x\" None)] None None) None Static false) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 46)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(Return(Dereference(CompoundLiteral(\"@1.compound_literal\" Array(Int 0) InitList([Var(\"@0.x\" None)] None None) None Static false) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"initializer for static-storage object '(compound literal)' is not a constant expression\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestCompoundLiteralStorage::test_rejected (call 47)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestCompoundLiteralStorage::test_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint thread_local = 1;\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"line 6, column 18: `thread_local` is a keyword in C23 (\\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label\"}\n");
}

TEST_CASE("parse: test_constexpr.py::TestStaticConsts::test_bits_of_a_const_double (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_constexpr.py::TestStaticConsts::test_bits_of_a_const_double\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nstatic const double X = 1.5;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"X\" Constant(ConstDouble(4609434218613702656) None) Const(Double) Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_constexpr.py::TestStaticConsts::test_bits_of_a_const_double (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_constexpr.py::TestStaticConsts::test_bits_of_a_const_double\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"X\" Constant(ConstDouble(4609434218613702656) None) Const(Double) Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"X\" Constant(ConstDouble(4609434218613702656) None) Const(Double) Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_constexpr.py::TestStaticConsts::test_bits_of_a_const_double (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_constexpr.py::TestStaticConsts::test_bits_of_a_const_double\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"X\" Constant(ConstDouble(4609434218613702656) None) Const(Double) Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"X\" Constant(ConstDouble(4609434218613702656) None) Const(Double) Static None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_constexpr.py::TestStaticConsts::test_bits_of_a_const_double (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_constexpr.py::TestStaticConsts::test_bits_of_a_const_double\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"X\" Constant(ConstDouble(4609434218613702656) None) Const(Double) Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"X\" Constant(ConstDouble(4609434218613702656) None) Const(Double) Static None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_constexpr.py::TestStaticConsts::test_bits_of_a_const_double (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_constexpr.py::TestStaticConsts::test_bits_of_a_const_double\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"X\" Constant(ConstDouble(4609434218613702656) None) Const(Double) Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"X\" Constant(ConstDouble(4609434218613702656) None) Const(Double) Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_constexpr.py::TestStaticConsts::test_bits_of_a_const_double (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_constexpr.py::TestStaticConsts::test_bits_of_a_const_double\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"X\\\" Constant(ConstDouble(4609434218613702656) Double) Const(Double) Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"X\" Constant(ConstDouble(4609434218613702656) None) Const(Double) Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"X\\\" Constant(ConstDouble(4609434218613702656) Double) Const(Double) Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"X\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Double)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 4609434218613702656}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}
