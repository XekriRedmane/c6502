// The calls that the tests of tests/test_auto.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_auto.py::TestInference::test_arrays_decay (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_auto.py::TestInference::test_arrays_decay\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\ndouble A[3] = { 0 }; auto pA = A; auto qA = &A; auto s = \\\"abc\\\";\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" InitList([Constant(ConstInt(0) None)] None None) Array(Double 3) None None None None false))\n"
      "  VarDecl(var_decl(\"pA\" Var(\"A\" None) Pointer(Double) None None None None false))\n"
      "  VarDecl(var_decl(\"qA\" AddressOf(Var(\"A\" None) None) Pointer(Array(Double 3)) None None None None false))\n"
      "  VarDecl(var_decl(\"s\" String(\"abc\") Pointer(Char) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_auto.py::TestInference::test_arrays_decay (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_auto.py::TestInference::test_arrays_decay\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" InitList([Constant(ConstInt(0) None)] None None) Array(Double 3) None None None None false))\n"
      "  VarDecl(var_decl(\"pA\" Var(\"A\" None) Pointer(Double) None None None None false))\n"
      "  VarDecl(var_decl(\"qA\" AddressOf(Var(\"A\" None) None) Pointer(Array(Double 3)) None None None None false))\n"
      "  VarDecl(var_decl(\"s\" String(\"abc\") Pointer(Char) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" InitList([Constant(ConstInt(0) None)] None None) Array(Double 3) None None None None false))\n"
      "  VarDecl(var_decl(\"pA\" Var(\"A\" None) Pointer(Double) None None None None false))\n"
      "  VarDecl(var_decl(\"qA\" AddressOf(Var(\"A\" None) None) Pointer(Array(Double 3)) None None None None false))\n"
      "  VarDecl(var_decl(\"s\" String(\"abc\") Pointer(Char) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_auto.py::TestInference::test_arrays_decay (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_auto.py::TestInference::test_arrays_decay\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" InitList([Constant(ConstInt(0) None)] None None) Array(Double 3) None None None None false))\n"
      "  VarDecl(var_decl(\"pA\" Var(\"A\" None) Pointer(Double) None None None None false))\n"
      "  VarDecl(var_decl(\"qA\" AddressOf(Var(\"A\" None) None) Pointer(Array(Double 3)) None None None None false))\n"
      "  VarDecl(var_decl(\"s\" String(\"abc\") Pointer(Char) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"str@0\" String(\"abc\") Array(Char 4) Static None None None false))\n"
      "  VarDecl(var_decl(\"A\" InitList([Constant(ConstInt(0) None)] None None) Array(Double 3) None None None None false))\n"
      "  VarDecl(var_decl(\"pA\" Var(\"A\" None) Pointer(Double) None None None None false))\n"
      "  VarDecl(var_decl(\"qA\" AddressOf(Var(\"A\" None) None) Pointer(Array(Double 3)) None None None None false))\n"
      "  VarDecl(var_decl(\"s\" Var(\"str@0\" None) Pointer(Char) None None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_auto.py::TestInference::test_arrays_decay (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_auto.py::TestInference::test_arrays_decay\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"str@0\" String(\"abc\") Array(Char 4) Static None None None false))\n"
      "  VarDecl(var_decl(\"A\" InitList([Constant(ConstInt(0) None)] None None) Array(Double 3) None None None None false))\n"
      "  VarDecl(var_decl(\"pA\" Var(\"A\" None) Pointer(Double) None None None None false))\n"
      "  VarDecl(var_decl(\"qA\" AddressOf(Var(\"A\" None) None) Pointer(Array(Double 3)) None None None None false))\n"
      "  VarDecl(var_decl(\"s\" Var(\"str@0\" None) Pointer(Char) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"str@0\" String(\"abc\") Array(Char 4) Static None None None false))\n"
      "  VarDecl(var_decl(\"A\" InitList([Constant(ConstInt(0) None)] None None) Array(Double 3) None None None None false))\n"
      "  VarDecl(var_decl(\"pA\" Var(\"A\" None) Pointer(Double) None None None None false))\n"
      "  VarDecl(var_decl(\"qA\" AddressOf(Var(\"A\" None) None) Pointer(Array(Double 3)) None None None None false))\n"
      "  VarDecl(var_decl(\"s\" Var(\"str@0\" None) Pointer(Char) None None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_auto.py::TestInference::test_arrays_decay (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_auto.py::TestInference::test_arrays_decay\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"str@0\" String(\"abc\") Array(Char 4) Static None None None false))\n"
      "  VarDecl(var_decl(\"A\" InitList([Constant(ConstInt(0) None)] None None) Array(Double 3) None None None None false))\n"
      "  VarDecl(var_decl(\"pA\" Var(\"A\" None) Pointer(Double) None None None None false))\n"
      "  VarDecl(var_decl(\"qA\" AddressOf(Var(\"A\" None) None) Pointer(Array(Double 3)) None None None None false))\n"
      "  VarDecl(var_decl(\"s\" Var(\"str@0\" None) Pointer(Char) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"str@0\" String(\"abc\") Array(Char 4) Static None None None false))\n"
      "  VarDecl(var_decl(\"A\" InitList([Constant(ConstInt(0) None)] None None) Array(Double 3) None None None None false))\n"
      "  VarDecl(var_decl(\"pA\" Var(\"A\" None) Pointer(Double) None None None None false))\n"
      "  VarDecl(var_decl(\"qA\" AddressOf(Var(\"A\" None) None) Pointer(Array(Double 3)) None None None None false))\n"
      "  VarDecl(var_decl(\"s\" Var(\"str@0\" None) Pointer(Char) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_auto.py::TestInference::test_arrays_decay (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_auto.py::TestInference::test_arrays_decay\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"str@0\\\" String(\\\"abc\\\") Array(Char 4) Static None None None false))\\n  VarDecl(var_decl(\\\"A\\\" InitList([Cast(Double Constant(ConstInt(0) Int) Double)] Array(Double 3) None) Array(Double 3) None None None None false))\\n  VarDecl(var_decl(\\\"pA\\\" AddressOf(Var(\\\"A\\\" Array(Double 3)) Pointer(Double)) Pointer(Double) None None None None false))\\n  VarDecl(var_decl(\\\"qA\\\" AddressOf(Var(\\\"A\\\" Array(Double 3)) Pointer(Array(Double 3))) Pointer(Array(Double 3)) None None None None false))\\n  VarDecl(var_decl(\\\"s\\\" AddressOf(Var(\\\"str@0\\\" Array(Char 4)) Pointer(Char)) Pointer(Char) None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"str@0\" String(\"abc\") Array(Char 4) Static None None None false))\n"
      "  VarDecl(var_decl(\"A\" InitList([Constant(ConstInt(0) None)] None None) Array(Double 3) None None None None false))\n"
      "  VarDecl(var_decl(\"pA\" Var(\"A\" None) Pointer(Double) None None None None false))\n"
      "  VarDecl(var_decl(\"qA\" AddressOf(Var(\"A\" None) None) Pointer(Array(Double 3)) None None None None false))\n"
      "  VarDecl(var_decl(\"s\" Var(\"str@0\" None) Pointer(Char) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"str@0\\\" String(\\\"abc\\\") Array(Char 4) Static None None None false))\\n  VarDecl(var_decl(\\\"A\\\" InitList([Cast(Double Constant(ConstInt(0) Int) Double)] Array(Double 3) None) Array(Double 3) None None None None false))\\n  VarDecl(var_decl(\\\"pA\\\" AddressOf(Var(\\\"A\\\" Array(Double 3)) Pointer(Double)) Pointer(Double) None None None None false))\\n  VarDecl(var_decl(\\\"qA\\\" AddressOf(Var(\\\"A\\\" Array(Double 3)) Pointer(Array(Double 3))) Pointer(Array(Double 3)) None None None None false))\\n  VarDecl(var_decl(\\\"s\\\" AddressOf(Var(\\\"str@0\\\" Array(Char 4)) Pointer(Char)) Pointer(Char) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"str@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [97, 98, 99, 0]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"A\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Double 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0, 0, 0]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"pA\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Double)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"A\", \"offset\": 0}}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"qA\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(Double 3))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"A\", \"offset\": 0}}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"str@0\", \"offset\": 0}}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_auto.py::TestInference::test_errors (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_auto.py::TestInference::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint f(void) { double a = 7; { auto a = a * a; return 0; } }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"the initializer of `auto` object `a` uses the name `a`, which isn't in scope until the initializer ends and hides any `a` outside (C23 \\u00a76.7.10p5)\"}\n");
}

TEST_CASE("parse: test_auto.py::TestInference::test_errors (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_auto.py::TestInference::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nauto int x = 5;\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`auto` at file scope only marks a type to infer, and this declaration has a type specifier (C23 \\u00a76.7.2p4)\"}\n");
}

TEST_CASE("parse: test_auto.py::TestInference::test_errors (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_auto.py::TestInference::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint f(void) { static auto int x = 5; return x; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`auto` goes with `static` only when it infers the type from the initializer, and this declaration has a type specifier (C23 \\u00a76.7.2p4)\"}\n");
}

TEST_CASE("parse: test_auto.py::TestInference::test_errors (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_auto.py::TestInference::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nauto x;\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`auto` object `x` has no initializer to infer its type from (C23 \\u00a76.7.10p2)\"}\n");
}

TEST_CASE("parse: test_auto.py::TestInference::test_errors (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_auto.py::TestInference::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nauto x = {1};\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"the initializer of `auto` object `x` is a list in braces, and `auto` infers a type only from an expression (C23 \\u00a76.7.10p2)\"}\n");
}

TEST_CASE("parse: test_auto.py::TestInference::test_errors (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_auto.py::TestInference::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nauto *p = 0;\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`auto` infers the type of a plain name, and the declarator of `p` adds to it, which c6502 doesn't take (C23 \\u00a76.7.10p2 leaves it to the implementation)\"}\n");
}

TEST_CASE("parse: test_auto.py::TestInference::test_errors (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_auto.py::TestInference::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nauto a = 1, b = 2;\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"an `auto` declaration that infers its type declares one name, and c6502 doesn't take more (C23 \\u00a76.7.1p12 leaves it to the implementation)\"}\n");
}

TEST_CASE("parse: test_auto.py::TestInference::test_errors (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_auto.py::TestInference::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nauto f(void) { return 1; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`auto` can't go on function definition 'f': c6502 doesn't infer a function's return type (C23 \\u00a76.7.1p12 leaves it to the implementation), and a function has no automatic storage\"}\n");
}

TEST_CASE("parse: test_auto.py::TestInference::test_errors (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_auto.py::TestInference::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint f(void) { auto int g(void); return 0; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`auto` can't go on function declaration 'g', since a function has no automatic storage and c6502 doesn't infer a function's type\"}\n");
}

TEST_CASE("parse: test_auto.py::TestInference::test_errors (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_auto.py::TestInference::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nvoid f(auto int x);\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`auto` isn't permitted on parameter 'x', where the only storage-class specifier is `register`\"}\n");
}

TEST_CASE("parse: test_auto.py::TestInference::test_errors (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_auto.py::TestInference::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\ntypedef auto T = 1;\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`auto` can't go with `typedef` (C23 \\u00a76.7.2p2)\"}\n");
}

TEST_CASE("parse: test_auto.py::TestInference::test_errors (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_auto.py::TestInference::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nauto auto x = 1;\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`auto` appears twice in a declaration\"}\n");
}

TEST_CASE("parse: test_auto.py::TestInference::test_errors (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_auto.py::TestInference::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint f(void) { auto x = undeclared; return x; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"the initializer of `auto` object `x`: undeclared identifier 'undeclared'\"}\n");
}

TEST_CASE("parse: test_auto.py::TestInference::test_file_scope (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_auto.py::TestInference::test_file_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nstatic auto a = 3.5; auto p = &a;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" Constant(ConstDouble(4615063718147915776) None) Double Static None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Double) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_auto.py::TestInference::test_file_scope (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_auto.py::TestInference::test_file_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" Constant(ConstDouble(4615063718147915776) None) Double Static None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Double) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" Constant(ConstDouble(4615063718147915776) None) Double Static None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Double) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_auto.py::TestInference::test_file_scope (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_auto.py::TestInference::test_file_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" Constant(ConstDouble(4615063718147915776) None) Double Static None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Double) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" Constant(ConstDouble(4615063718147915776) None) Double Static None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Double) None None None None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_auto.py::TestInference::test_file_scope (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_auto.py::TestInference::test_file_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" Constant(ConstDouble(4615063718147915776) None) Double Static None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Double) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" Constant(ConstDouble(4615063718147915776) None) Double Static None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Double) None None None None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_auto.py::TestInference::test_file_scope (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_auto.py::TestInference::test_file_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" Constant(ConstDouble(4615063718147915776) None) Double Static None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Double) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" Constant(ConstDouble(4615063718147915776) None) Double Static None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Double) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_auto.py::TestInference::test_file_scope (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_auto.py::TestInference::test_file_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" Constant(ConstDouble(4615063718147915776) Double) Double Static None None None false))\\n  VarDecl(var_decl(\\\"p\\\" AddressOf(Var(\\\"a\\\" Double) Pointer(Double)) Pointer(Double) None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" Constant(ConstDouble(4615063718147915776) None) Double Static None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Double) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" Constant(ConstDouble(4615063718147915776) Double) Double Static None None None false))\\n  VarDecl(var_decl(\\\"p\\\" AddressOf(Var(\\\"a\\\" Double) Pointer(Double)) Pointer(Double) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Double\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 4615063718147915776}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Double)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"a\", \"offset\": 0}}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_auto.py::TestInference::test_lvalue_conversion_and_qualifiers (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_auto.py::TestInference::test_lvalue_conversion_and_qualifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconst unsigned char c = 3;int f(void) { auto d = c; auto e = c + 1; const auto k = 5; volatile auto v = 'x'; return d + e + k + v; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" Constant(ConstInt(3) None) Const(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"d\" Var(\"c\" None) UChar None None None None false)))\n"
      "    D(VarDecl(var_decl(\"e\" Binary(Add Var(\"c\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"k\" Constant(ConstInt(5) None) Const(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"v\" Constant(ConstInt(120) None) Volatile(Int) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Var(\"d\" None) Var(\"e\" None) None) Var(\"k\" None) None) Var(\"v\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_auto.py::TestInference::test_lvalue_conversion_and_qualifiers (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_auto.py::TestInference::test_lvalue_conversion_and_qualifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" Constant(ConstInt(3) None) Const(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"d\" Var(\"c\" None) UChar None None None None false)))\n"
      "    D(VarDecl(var_decl(\"e\" Binary(Add Var(\"c\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"k\" Constant(ConstInt(5) None) Const(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"v\" Constant(ConstInt(120) None) Volatile(Int) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Var(\"d\" None) Var(\"e\" None) None) Var(\"k\" None) None) Var(\"v\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" Constant(ConstInt(3) None) Const(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.d\" Var(\"c\" None) UChar None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.e\" Binary(Add Var(\"c\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.k\" Constant(ConstInt(5) None) Const(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@3.v\" Constant(ConstInt(120) None) Volatile(Int) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Var(\"@0.d\" None) Var(\"@1.e\" None) None) Var(\"@2.k\" None) None) Var(\"@3.v\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_auto.py::TestInference::test_lvalue_conversion_and_qualifiers (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_auto.py::TestInference::test_lvalue_conversion_and_qualifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" Constant(ConstInt(3) None) Const(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.d\" Var(\"c\" None) UChar None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.e\" Binary(Add Var(\"c\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.k\" Constant(ConstInt(5) None) Const(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@3.v\" Constant(ConstInt(120) None) Volatile(Int) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Var(\"@0.d\" None) Var(\"@1.e\" None) None) Var(\"@2.k\" None) None) Var(\"@3.v\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" Constant(ConstInt(3) None) Const(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.d\" Var(\"c\" None) UChar None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.e\" Binary(Add Var(\"c\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.k\" Constant(ConstInt(5) None) Const(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@3.v\" Constant(ConstInt(120) None) Volatile(Int) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Var(\"@0.d\" None) Var(\"@1.e\" None) None) Var(\"@2.k\" None) None) Var(\"@3.v\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_auto.py::TestInference::test_lvalue_conversion_and_qualifiers (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_auto.py::TestInference::test_lvalue_conversion_and_qualifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" Constant(ConstInt(3) None) Const(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.d\" Var(\"c\" None) UChar None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.e\" Binary(Add Var(\"c\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.k\" Constant(ConstInt(5) None) Const(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@3.v\" Constant(ConstInt(120) None) Volatile(Int) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Var(\"@0.d\" None) Var(\"@1.e\" None) None) Var(\"@2.k\" None) None) Var(\"@3.v\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" Constant(ConstInt(3) None) Const(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.d\" Var(\"c\" None) UChar None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.e\" Binary(Add Var(\"c\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.k\" Constant(ConstInt(5) None) Const(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@3.v\" Constant(ConstInt(120) None) Volatile(Int) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Var(\"@0.d\" None) Var(\"@1.e\" None) None) Var(\"@2.k\" None) None) Var(\"@3.v\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_auto.py::TestInference::test_lvalue_conversion_and_qualifiers (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_auto.py::TestInference::test_lvalue_conversion_and_qualifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" Constant(ConstInt(3) None) Const(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.d\" Var(\"c\" None) UChar None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.e\" Binary(Add Var(\"c\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.k\" Constant(ConstInt(5) None) Const(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@3.v\" Constant(ConstInt(120) None) Volatile(Int) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Var(\"@0.d\" None) Var(\"@1.e\" None) None) Var(\"@2.k\" None) None) Var(\"@3.v\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" Constant(ConstInt(3) None) Const(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.d\" Var(\"c\" None) UChar None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.e\" Binary(Add Var(\"c\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.k\" Constant(ConstInt(5) None) Const(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@3.v\" Constant(ConstInt(120) None) Volatile(Int) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Var(\"@0.d\" None) Var(\"@1.e\" None) None) Var(\"@2.k\" None) None) Var(\"@3.v\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_auto.py::TestInference::test_lvalue_conversion_and_qualifiers (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_auto.py::TestInference::test_lvalue_conversion_and_qualifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"c\\\" Cast(Const(UChar) Constant(ConstInt(3) Int) Const(UChar)) Const(UChar) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.d\\\" Var(\\\"c\\\" Const(UChar)) UChar None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.e\\\" Binary(Add Cast(Int Var(\\\"c\\\" Const(UChar)) Int) Constant(ConstInt(1) Int) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.k\\\" Constant(ConstInt(5) Int) Const(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@3.v\\\" Constant(ConstInt(120) Int) Volatile(Int) None None None None false)))\\n    S(Return(Binary(Add Binary(Add Binary(Add Cast(Int Var(\\\"@0.d\\\" UChar) Int) Var(\\\"@1.e\\\" Int) Int) Var(\\\"@2.k\\\" Const(Int)) Int) Var(\\\"@3.v\\\" Volatile(Int)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" Constant(ConstInt(3) None) Const(UChar) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.d\" Var(\"c\" None) UChar None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.e\" Binary(Add Var(\"c\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.k\" Constant(ConstInt(5) None) Const(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@3.v\" Constant(ConstInt(120) None) Volatile(Int) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Var(\"@0.d\" None) Var(\"@1.e\" None) None) Var(\"@2.k\" None) None) Var(\"@3.v\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"c\\\" Cast(Const(UChar) Constant(ConstInt(3) Int) Const(UChar)) Const(UChar) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.d\\\" Var(\\\"c\\\" Const(UChar)) UChar None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.e\\\" Binary(Add Cast(Int Var(\\\"c\\\" Const(UChar)) Int) Constant(ConstInt(1) Int) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.k\\\" Constant(ConstInt(5) Int) Const(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@3.v\\\" Constant(ConstInt(120) Int) Volatile(Int) None None None None false)))\\n    S(Return(Binary(Add Binary(Add Binary(Add Cast(Int Var(\\\"@0.d\\\" UChar) Int) Var(\\\"@1.e\\\" Int) Int) Var(\\\"@2.k\\\" Const(Int)) Int) Var(\\\"@3.v\\\" Volatile(Int)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 3}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.e\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.k\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Volatile(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_auto.py::TestInference::test_other_types (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_auto.py::TestInference::test_other_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint g(int); struct pt { int x, y; };auto fp = g; auto n = nullptr; auto b = true;int f(void) { auto P = (struct pt){1, 2}; auto Y = P.y; return Y; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"g\" [\"\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"fp\" Var(\"g\" None) Pointer(FunType([Int] Int)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstNullptr None) NullptrT None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(1) None) Bool None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"P\" CompoundLiteral(\"\" Structure(\"pt\") InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None false) Structure(\"pt\") None None None None false)))\n"
      "    D(VarDecl(var_decl(\"Y\" Dot(Var(\"P\" None) \"y\" None) Int None None None None false)))\n"
      "    S(Return(Var(\"Y\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_auto.py::TestInference::test_other_types (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_auto.py::TestInference::test_other_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"g\" [\"\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"fp\" Var(\"g\" None) Pointer(FunType([Int] Int)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstNullptr None) NullptrT None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(1) None) Bool None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"P\" CompoundLiteral(\"\" Structure(\"pt\") InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None false) Structure(\"pt\") None None None None false)))\n"
      "    D(VarDecl(var_decl(\"Y\" Dot(Var(\"P\" None) \"y\" None) Int None None None None false)))\n"
      "    S(Return(Var(\"Y\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"g\" [\"\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"fp\" Var(\"g\" None) Pointer(FunType([Int] Int)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstNullptr None) NullptrT None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(1) None) Bool None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.P\" CompoundLiteral(\"@1.compound_literal\" Structure(\"pt\") InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None false) Structure(\"pt\") None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.Y\" Dot(Var(\"@0.P\" None) \"y\" None) Int None None None None false)))\n"
      "    S(Return(Var(\"@2.Y\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_auto.py::TestInference::test_other_types (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_auto.py::TestInference::test_other_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"g\" [\"\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"fp\" Var(\"g\" None) Pointer(FunType([Int] Int)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstNullptr None) NullptrT None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(1) None) Bool None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.P\" CompoundLiteral(\"@1.compound_literal\" Structure(\"pt\") InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None false) Structure(\"pt\") None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.Y\" Dot(Var(\"@0.P\" None) \"y\" None) Int None None None None false)))\n"
      "    S(Return(Var(\"@2.Y\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"g\" [\"\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"fp\" Var(\"g\" None) Pointer(FunType([Int] Int)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstNullptr None) NullptrT None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(1) None) Bool None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.P\" CompoundLiteral(\"@1.compound_literal\" Structure(\"pt\") InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None false) Structure(\"pt\") None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.Y\" Dot(Var(\"@0.P\" None) \"y\" None) Int None None None None false)))\n"
      "    S(Return(Var(\"@2.Y\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_auto.py::TestInference::test_other_types (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_auto.py::TestInference::test_other_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"g\" [\"\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"fp\" Var(\"g\" None) Pointer(FunType([Int] Int)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstNullptr None) NullptrT None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(1) None) Bool None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.P\" CompoundLiteral(\"@1.compound_literal\" Structure(\"pt\") InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None false) Structure(\"pt\") None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.Y\" Dot(Var(\"@0.P\" None) \"y\" None) Int None None None None false)))\n"
      "    S(Return(Var(\"@2.Y\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"g\" [\"\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"fp\" Var(\"g\" None) Pointer(FunType([Int] Int)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstNullptr None) NullptrT None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(1) None) Bool None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.P\" CompoundLiteral(\"@1.compound_literal\" Structure(\"pt\") InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None false) Structure(\"pt\") None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.Y\" Dot(Var(\"@0.P\" None) \"y\" None) Int None None None None false)))\n"
      "    S(Return(Var(\"@2.Y\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_auto.py::TestInference::test_other_types (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_auto.py::TestInference::test_other_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"g\" [\"\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"fp\" Var(\"g\" None) Pointer(FunType([Int] Int)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstNullptr None) NullptrT None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(1) None) Bool None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.P\" CompoundLiteral(\"@1.compound_literal\" Structure(\"pt\") InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None false) Structure(\"pt\") None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.Y\" Dot(Var(\"@0.P\" None) \"y\" None) Int None None None None false)))\n"
      "    S(Return(Var(\"@2.Y\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"g\" [\"\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"fp\" Var(\"g\" None) Pointer(FunType([Int] Int)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstNullptr None) NullptrT None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(1) None) Bool None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.P\" CompoundLiteral(\"@1.compound_literal\" Structure(\"pt\") InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None false) Structure(\"pt\") None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.Y\" Dot(Var(\"@0.P\" None) \"y\" None) Int None None None None false)))\n"
      "    S(Return(Var(\"@2.Y\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_auto.py::TestInference::test_other_types (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_auto.py::TestInference::test_other_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"g\\\" [\\\"\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  StructDecl(struct_decl(\\\"pt\\\" false [member_decl(\\\"x\\\" Int) member_decl(\\\"y\\\" Int)]))\\n  VarDecl(var_decl(\\\"fp\\\" AddressOf(Var(\\\"g\\\" FunType([Int] Int)) Pointer(FunType([Int] Int))) Pointer(FunType([Int] Int)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" Constant(ConstNullptr NullptrT) NullptrT None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Constant(ConstBool(1) Bool) Bool None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.P\\\" CompoundLiteral(\\\"@1.compound_literal\\\" Structure(\\\"pt\\\") InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Structure(\\\"pt\\\") None) Structure(\\\"pt\\\") None false) Structure(\\\"pt\\\") None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.Y\\\" Dot(Var(\\\"@0.P\\\" Structure(\\\"pt\\\")) \\\"y\\\" Int) Int None None None None false)))\\n    S(Return(Var(\\\"@2.Y\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"g\" [\"\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  VarDecl(var_decl(\"fp\" Var(\"g\" None) Pointer(FunType([Int] Int)) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" Constant(ConstNullptr None) NullptrT None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(1) None) Bool None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.P\" CompoundLiteral(\"@1.compound_literal\" Structure(\"pt\") InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None false) Structure(\"pt\") None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.Y\" Dot(Var(\"@0.P\" None) \"y\" None) Int None None None None false)))\n"
      "    S(Return(Var(\"@2.Y\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"g\\\" [\\\"\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  StructDecl(struct_decl(\\\"pt\\\" false [member_decl(\\\"x\\\" Int) member_decl(\\\"y\\\" Int)]))\\n  VarDecl(var_decl(\\\"fp\\\" AddressOf(Var(\\\"g\\\" FunType([Int] Int)) Pointer(FunType([Int] Int))) Pointer(FunType([Int] Int)) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" Constant(ConstNullptr NullptrT) NullptrT None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Constant(ConstBool(1) Bool) Bool None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.P\\\" CompoundLiteral(\\\"@1.compound_literal\\\" Structure(\\\"pt\\\") InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Structure(\\\"pt\\\") None) Structure(\\\"pt\\\") None false) Structure(\\\"pt\\\") None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.Y\\\" Dot(Var(\\\"@0.P\\\" Structure(\\\"pt\\\")) \\\"y\\\" Int) Int None None None None false)))\\n    S(Return(Var(\\\"@2.Y\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"fp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([Int] Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"g\", \"offset\": 0}}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.P\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Structure(\\\"pt\\\")\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.compound_literal\", {\"object\": \"passes.type_checking.Sy"
      "mbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Structure(\\\"pt\\\")\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.Y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"pt\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"pt\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"x\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"y\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 2}}], \"size\": 4, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_auto.py::TestInference::test_storage_class (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_auto.py::TestInference::test_storage_class\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nint f(void) { auto int x = 5; auto long y; return x; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"y\" None Long None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_auto.py::TestInference::test_storage_class (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_auto.py::TestInference::test_storage_class\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"y\" None Long None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" None Long None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_auto.py::TestInference::test_storage_class (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_auto.py::TestInference::test_storage_class\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" None Long None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" None Long None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_auto.py::TestInference::test_storage_class (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_auto.py::TestInference::test_storage_class\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" None Long None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" None Long None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_auto.py::TestInference::test_storage_class (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_auto.py::TestInference::test_storage_class\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" None Long None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" None Long None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_auto.py::TestInference::test_storage_class (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_auto.py::TestInference::test_storage_class\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.y\\\" None Long None None None None false)))\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" None Long None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.y\\\" None Long None None None None false)))\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_auto.py::TestInference::test_with_other_storage_classes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_auto.py::TestInference::test_with_other_storage_classes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int ptrdiff_t;\\ntypedef unsigned int size_t;\\ntypedef char max_align_t;\\ntypedef int wchar_t;\\ntypedef typeof_unqual(nullptr) nullptr_t;\\nconstexpr auto K = 7; int arr[K];int f(void) { static auto s = 5; register auto r = (unsigned char)4; return s + r; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(7) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"arr\" None Array(Int 7) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(5) None) Int Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"r\" Cast(UChar Constant(ConstInt(4) None) None) UChar Register None None None false)))\n"
      "    S(Return(Binary(Add Var(\"s\" None) Var(\"r\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_auto.py::TestInference::test_with_other_storage_classes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_auto.py::TestInference::test_with_other_storage_classes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(7) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"arr\" None Array(Int 7) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(5) None) Int Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"r\" Cast(UChar Constant(ConstInt(4) None) None) UChar Register None None None false)))\n"
      "    S(Return(Binary(Add Var(\"s\" None) Var(\"r\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(7) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"arr\" None Array(Int 7) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.s\" Constant(ConstInt(5) None) Int Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.r\" Cast(UChar Constant(ConstInt(4) None) None) UChar Register None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.s\" None) Var(\"@1.r\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_auto.py::TestInference::test_with_other_storage_classes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_auto.py::TestInference::test_with_other_storage_classes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(7) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"arr\" None Array(Int 7) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.s\" Constant(ConstInt(5) None) Int Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.r\" Cast(UChar Constant(ConstInt(4) None) None) UChar Register None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.s\" None) Var(\"@1.r\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(7) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"arr\" None Array(Int 7) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.s\" Constant(ConstInt(5) None) Int Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.r\" Cast(UChar Constant(ConstInt(4) None) None) UChar Register None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.s\" None) Var(\"@1.r\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_auto.py::TestInference::test_with_other_storage_classes (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_auto.py::TestInference::test_with_other_storage_classes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(7) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"arr\" None Array(Int 7) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.s\" Constant(ConstInt(5) None) Int Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.r\" Cast(UChar Constant(ConstInt(4) None) None) UChar Register None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.s\" None) Var(\"@1.r\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(7) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"arr\" None Array(Int 7) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.s\" Constant(ConstInt(5) None) Int Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.r\" Cast(UChar Constant(ConstInt(4) None) None) UChar Register None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.s\" None) Var(\"@1.r\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_auto.py::TestInference::test_with_other_storage_classes (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_auto.py::TestInference::test_with_other_storage_classes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(7) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"arr\" None Array(Int 7) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.s\" Constant(ConstInt(5) None) Int Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.r\" Cast(UChar Constant(ConstInt(4) None) None) UChar Register None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.s\" None) Var(\"@1.r\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(7) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"arr\" None Array(Int 7) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.s\" Constant(ConstInt(5) None) Int Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.r\" Cast(UChar Constant(ConstInt(4) None) None) UChar Register None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.s\" None) Var(\"@1.r\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_auto.py::TestInference::test_with_other_storage_classes (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_auto.py::TestInference::test_with_other_storage_classes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"K\\\" Constant(ConstInt(7) Int) Const(Int) Static None None None true))\\n  VarDecl(var_decl(\\\"arr\\\" None Array(Int 7) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.s\\\" Constant(ConstInt(5) Int) Int Static None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.r\\\" Cast(UChar Constant(ConstInt(4) Int) UChar) UChar Register None None None false)))\\n    S(Return(Binary(Add Var(\\\"@0.s\\\" Int) Cast(Int Var(\\\"@1.r\\\" UChar) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(7) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"arr\" None Array(Int 7) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.s\" Constant(ConstInt(5) None) Int Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.r\" Cast(UChar Constant(ConstInt(4) None) None) UChar Register None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.s\" None) Var(\"@1.r\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"K\\\" Constant(ConstInt(7) Int) Const(Int) Static None None None true))\\n  VarDecl(var_decl(\\\"arr\\\" None Array(Int 7) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.s\\\" Constant(ConstInt(5) Int) Int Static None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.r\\\" Cast(UChar Constant(ConstInt(4) Int) UChar) UChar Register None None None false)))\\n    S(Return(Binary(Add Var(\\\"@0.s\\\" Int) Cast(Int Var(\\\"@1.r\\\" UChar) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"K\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 7}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 7)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 5}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@1.r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": true, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}
