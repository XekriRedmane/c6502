// The calls that the tests of tests/test_unsized_arrays.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_unsized_arrays.py::TestSizes::test_errors (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int t[];\\nint main(void) { return 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" None Array(Int 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_unsized_arrays.py::TestSizes::test_errors (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" None Array(Int 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" None Array(Int 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_unsized_arrays.py::TestSizes::test_errors (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" None Array(Int 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" None Array(Int 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_unsized_arrays.py::TestSizes::test_errors (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" None Array(Int 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" None Array(Int 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_unsized_arrays.py::TestSizes::test_errors (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" None Array(Int 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" None Array(Int 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_unsized_arrays.py::TestSizes::test_errors (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" None Array(Int 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"file-scope object 't': incomplete array type (`[]`) is allowed only at the outermost type of an `extern` declaration\"}\n");
}

TEST_CASE("parse: test_unsized_arrays.py::TestSizes::test_errors (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int t[]; return 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"t\" None Array(Int 0) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_unsized_arrays.py::TestSizes::test_errors (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"t\" None Array(Int 0) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.t\" None Array(Int 0) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_unsized_arrays.py::TestSizes::test_errors (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.t\" None Array(Int 0) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.t\" None Array(Int 0) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_unsized_arrays.py::TestSizes::test_errors (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.t\" None Array(Int 0) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.t\" None Array(Int 0) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_unsized_arrays.py::TestSizes::test_errors (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.t\" None Array(Int 0) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.t\" None Array(Int 0) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_unsized_arrays.py::TestSizes::test_errors (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.t\" None Array(Int 0) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"object '@0.t': incomplete array type (`[]`) is allowed only at the outermost type of an `extern` declaration\"}\n");
}

TEST_CASE("parse: test_unsized_arrays.py::TestSizes::test_errors (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int t[] = 5;\\nint main(void) { return 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" Constant(ConstInt(5) None) Array(Int 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_unsized_arrays.py::TestSizes::test_errors (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" Constant(ConstInt(5) None) Array(Int 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" Constant(ConstInt(5) None) Array(Int 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_unsized_arrays.py::TestSizes::test_errors (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" Constant(ConstInt(5) None) Array(Int 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" Constant(ConstInt(5) None) Array(Int 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_unsized_arrays.py::TestSizes::test_errors (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" Constant(ConstInt(5) None) Array(Int 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" Constant(ConstInt(5) None) Array(Int 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_unsized_arrays.py::TestSizes::test_errors (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" Constant(ConstInt(5) None) Array(Int 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" Constant(ConstInt(5) None) Array(Int 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_unsized_arrays.py::TestSizes::test_errors (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" Constant(ConstInt(5) None) Array(Int 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"file-scope object 't': incomplete array type (`[]`) is allowed only at the outermost type of an `extern` declaration\"}\n");
}

TEST_CASE("parse: test_unsized_arrays.py::TestSizes::test_errors (call 19)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { static int t[] = {}; return 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"t\" InitList([] None None) Array(Int 0) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_unsized_arrays.py::TestSizes::test_errors (call 20)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"t\" InitList([] None None) Array(Int 0) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.t\" InitList([] None None) Array(Int 0) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_unsized_arrays.py::TestSizes::test_errors (call 21)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.t\" InitList([] None None) Array(Int 0) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.t\" InitList([] None None) Array(Int 0) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_unsized_arrays.py::TestSizes::test_errors (call 22)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.t\" InitList([] None None) Array(Int 0) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.t\" InitList([] None None) Array(Int 0) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_unsized_arrays.py::TestSizes::test_errors (call 23)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.t\" InitList([] None None) Array(Int 0) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.t\" InitList([] None None) Array(Int 0) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_unsized_arrays.py::TestSizes::test_errors (call 24)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.t\" InitList([] None None) Array(Int 0) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"array 't' has unknown size, so it can't be initialized by the empty initializer `{}`, which has no elements to give it a size (C23 \\u00a76.7.11p4)\"}\n");
}

TEST_CASE("parse: test_unsized_arrays.py::TestSizes::test_sizes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n            int t[] = {1, 2, 3};\\n            char s[] = \\\"abc\\\";\\n            const unsigned char k[] = {9, 8};\\n            char b[] = {\\\"xy\\\"};\\n            int m[][2] = {{1, 2}, {3, 4}, {5, 6}};\\n            int main(void) {\\n                static int u[] = {4, 5, 6, 7};\\n                long v[] = {1, 2};\\n                return sizeof t + sizeof s + sizeof k + sizeof b\\n                    + sizeof m + sizeof u + sizeof v;\\n            }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 0) None None None None false))\n"
      "  VarDecl(var_decl(\"s\" String(\"abc\") Array(Char 0) None None None None false))\n"
      "  VarDecl(var_decl(\"k\" InitList([Constant(ConstInt(9) None) Constant(ConstInt(8) None)] None None) Array(Const(UChar) 0) None None None None false))\n"
      "  VarDecl(var_decl(\"b\" InitList([String(\"xy\")] None None) Array(Char 0) None None None None false))\n"
      "  VarDecl(var_decl(\"m\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Int 2) 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"u\" InitList([Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None) Constant(ConstInt(7) None)] None None) Array(Int 0) Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"v\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Long 0) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add SizeOfExp(Var(\"t\" None) None) SizeOfExp(Var(\"s\" None) None) None) SizeOfExp(Var(\"k\" None) None) None) SizeOfExp(Var(\"b\" None) None) None) SizeOfExp(Var(\"m\" None) None) None) SizeOfExp(Var(\"u\" None) None) None) SizeOfExp(Var(\"v\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_unsized_arrays.py::TestSizes::test_sizes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 0) None None None None false))\n"
      "  VarDecl(var_decl(\"s\" String(\"abc\") Array(Char 0) None None None None false))\n"
      "  VarDecl(var_decl(\"k\" InitList([Constant(ConstInt(9) None) Constant(ConstInt(8) None)] None None) Array(Const(UChar) 0) None None None None false))\n"
      "  VarDecl(var_decl(\"b\" InitList([String(\"xy\")] None None) Array(Char 0) None None None None false))\n"
      "  VarDecl(var_decl(\"m\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Int 2) 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"u\" InitList([Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None) Constant(ConstInt(7) None)] None None) Array(Int 0) Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"v\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Long 0) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add SizeOfExp(Var(\"t\" None) None) SizeOfExp(Var(\"s\" None) None) None) SizeOfExp(Var(\"k\" None) None) None) SizeOfExp(Var(\"b\" None) None) None) SizeOfExp(Var(\"m\" None) None) None) SizeOfExp(Var(\"u\" None) None) None) SizeOfExp(Var(\"v\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 0) None None None None false))\n"
      "  VarDecl(var_decl(\"s\" String(\"abc\") Array(Char 0) None None None None false))\n"
      "  VarDecl(var_decl(\"k\" InitList([Constant(ConstInt(9) None) Constant(ConstInt(8) None)] None None) Array(Const(UChar) 0) None None None None false))\n"
      "  VarDecl(var_decl(\"b\" InitList([String(\"xy\")] None None) Array(Char 0) None None None None false))\n"
      "  VarDecl(var_decl(\"m\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Int 2) 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.u\" InitList([Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None) Constant(ConstInt(7) None)] None None) Array(Int 0) Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.v\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Long 0) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add SizeOfExp(Var(\"t\" None) None) SizeOfExp(Var(\"s\" None) None) None) SizeOfExp(Var(\"k\" None) None) None) SizeOfExp(Var(\"b\" None) None) None) SizeOfExp(Var(\"m\" None) None) None) SizeOfExp(Var(\"@0.u\" None) None) None) SizeOfExp(Var(\"@1.v\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_unsized_arrays.py::TestSizes::test_sizes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 0) None None None None false))\n"
      "  VarDecl(var_decl(\"s\" String(\"abc\") Array(Char 0) None None None None false))\n"
      "  VarDecl(var_decl(\"k\" InitList([Constant(ConstInt(9) None) Constant(ConstInt(8) None)] None None) Array(Const(UChar) 0) None None None None false))\n"
      "  VarDecl(var_decl(\"b\" InitList([String(\"xy\")] None None) Array(Char 0) None None None None false))\n"
      "  VarDecl(var_decl(\"m\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Int 2) 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.u\" InitList([Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None) Constant(ConstInt(7) None)] None None) Array(Int 0) Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.v\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Long 0) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add SizeOfExp(Var(\"t\" None) None) SizeOfExp(Var(\"s\" None) None) None) SizeOfExp(Var(\"k\" None) None) None) SizeOfExp(Var(\"b\" None) None) None) SizeOfExp(Var(\"m\" None) None) None) SizeOfExp(Var(\"@0.u\" None) None) None) SizeOfExp(Var(\"@1.v\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 0) None None None None false))\n"
      "  VarDecl(var_decl(\"s\" String(\"abc\") Array(Char 0) None None None None false))\n"
      "  VarDecl(var_decl(\"k\" InitList([Constant(ConstInt(9) None) Constant(ConstInt(8) None)] None None) Array(Const(UChar) 0) None None None None false))\n"
      "  VarDecl(var_decl(\"b\" String(\"xy\") Array(Char 0) None None None None false))\n"
      "  VarDecl(var_decl(\"m\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Int 2) 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.u\" InitList([Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None) Constant(ConstInt(7) None)] None None) Array(Int 0) Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.v\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Long 0) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add SizeOfExp(Var(\"t\" None) None) SizeOfExp(Var(\"s\" None) None) None) SizeOfExp(Var(\"k\" None) None) None) SizeOfExp(Var(\"b\" None) None) None) SizeOfExp(Var(\"m\" None) None) None) SizeOfExp(Var(\"@0.u\" None) None) None) SizeOfExp(Var(\"@1.v\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_unsized_arrays.py::TestSizes::test_sizes (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 0) None None None None false))\n"
      "  VarDecl(var_decl(\"s\" String(\"abc\") Array(Char 0) None None None None false))\n"
      "  VarDecl(var_decl(\"k\" InitList([Constant(ConstInt(9) None) Constant(ConstInt(8) None)] None None) Array(Const(UChar) 0) None None None None false))\n"
      "  VarDecl(var_decl(\"b\" String(\"xy\") Array(Char 0) None None None None false))\n"
      "  VarDecl(var_decl(\"m\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Int 2) 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.u\" InitList([Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None) Constant(ConstInt(7) None)] None None) Array(Int 0) Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.v\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Long 0) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add SizeOfExp(Var(\"t\" None) None) SizeOfExp(Var(\"s\" None) None) None) SizeOfExp(Var(\"k\" None) None) None) SizeOfExp(Var(\"b\" None) None) None) SizeOfExp(Var(\"m\" None) None) None) SizeOfExp(Var(\"@0.u\" None) None) None) SizeOfExp(Var(\"@1.v\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 0) None None None None false))\n"
      "  VarDecl(var_decl(\"s\" String(\"abc\") Array(Char 0) None None None None false))\n"
      "  VarDecl(var_decl(\"k\" InitList([Constant(ConstInt(9) None) Constant(ConstInt(8) None)] None None) Array(Const(UChar) 0) None None None None false))\n"
      "  VarDecl(var_decl(\"b\" String(\"xy\") Array(Char 0) None None None None false))\n"
      "  VarDecl(var_decl(\"m\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Int 2) 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.u\" InitList([Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None) Constant(ConstInt(7) None)] None None) Array(Int 0) Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.v\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Long 0) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add SizeOfExp(Var(\"t\" None) None) SizeOfExp(Var(\"s\" None) None) None) SizeOfExp(Var(\"k\" None) None) None) SizeOfExp(Var(\"b\" None) None) None) SizeOfExp(Var(\"m\" None) None) None) SizeOfExp(Var(\"@0.u\" None) None) None) SizeOfExp(Var(\"@1.v\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_unsized_arrays.py::TestSizes::test_sizes (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 0) None None None None false))\n"
      "  VarDecl(var_decl(\"s\" String(\"abc\") Array(Char 0) None None None None false))\n"
      "  VarDecl(var_decl(\"k\" InitList([Constant(ConstInt(9) None) Constant(ConstInt(8) None)] None None) Array(Const(UChar) 0) None None None None false))\n"
      "  VarDecl(var_decl(\"b\" String(\"xy\") Array(Char 0) None None None None false))\n"
      "  VarDecl(var_decl(\"m\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Int 2) 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.u\" InitList([Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None) Constant(ConstInt(7) None)] None None) Array(Int 0) Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.v\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Long 0) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add SizeOfExp(Var(\"t\" None) None) SizeOfExp(Var(\"s\" None) None) None) SizeOfExp(Var(\"k\" None) None) None) SizeOfExp(Var(\"b\" None) None) None) SizeOfExp(Var(\"m\" None) None) None) SizeOfExp(Var(\"@0.u\" None) None) None) SizeOfExp(Var(\"@1.v\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 0) None None None None false))\n"
      "  VarDecl(var_decl(\"s\" String(\"abc\") Array(Char 0) None None None None false))\n"
      "  VarDecl(var_decl(\"k\" InitList([Constant(ConstInt(9) None) Constant(ConstInt(8) None)] None None) Array(Const(UChar) 0) None None None None false))\n"
      "  VarDecl(var_decl(\"b\" String(\"xy\") Array(Char 0) None None None None false))\n"
      "  VarDecl(var_decl(\"m\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Int 2) 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.u\" InitList([Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None) Constant(ConstInt(7) None)] None None) Array(Int 0) Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.v\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Long 0) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add SizeOfExp(Var(\"t\" None) None) SizeOfExp(Var(\"s\" None) None) None) SizeOfExp(Var(\"k\" None) None) None) SizeOfExp(Var(\"b\" None) None) None) SizeOfExp(Var(\"m\" None) None) None) SizeOfExp(Var(\"@0.u\" None) None) None) SizeOfExp(Var(\"@1.v\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_unsized_arrays.py::TestSizes::test_sizes (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_unsized_arrays.py::TestSizes::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"t\\\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) Array(Int 3) None None None None false))\\n  VarDecl(var_decl(\\\"s\\\" String(\\\"abc\\\") Array(Char 4) None None None None false))\\n  VarDecl(var_decl(\\\"k\\\" InitList([Cast(Const(UChar) Constant(ConstInt(9) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(8) Int) Const(UChar))] Array(Const(UChar) 2) None) Array(Const(UChar) 2) None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" String(\\\"xy\\\") Array(Char 3) None None None None false))\\n  VarDecl(var_decl(\\\"m\\\" InitList([InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Array(Int 2) None) InitList([Constant(ConstInt(3) Int) Constant(ConstInt(4) Int)] Array(Int 2) None) InitList([Constant(ConstInt(5) Int) Constant(ConstInt(6) Int)] Array(Int 2) None)] Array(Array(Int 2) 3) None) Array(Array(Int 2) 3) None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.u\\\" InitList([Constant(ConstInt(4) Int) Constant(ConstInt(5) Int) Constant(ConstInt(6) Int) Constant(ConstInt(7) Int)] Array(Int 4) None) Array(Int 4) Static None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.v\\\" InitList([Cast(Long Constant(ConstInt(1) Int) Long) Cast(Long Constant(ConstInt(2) Int) Long)] Array(Long 2) None) Array(Long 2) None None None None false)))\\n    S(Return(Cast(Int Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add SizeOfExp(Var(\\\"t\\\" Array(Int 3)) UInt) SizeOfExp(Var(\\\"s\\\" Array(Char 4)) UInt) UInt) SizeOfExp(Var(\\\"k\\\" Array(Const(UChar) 2)) UInt) UInt) SizeOfExp(Var(\\\"b\\\" Array(Char 3)) UInt) UInt) SizeOfExp(Var(\\\"m\\\" Array(Array(Int 2) 3)) UInt) UInt) SizeOfExp(Var(\\\"@0.u\\\" Array(Int 4)) UInt) UInt) SizeOfExp(Var(\\\"@1.v\\\" Array(Long 2)) UInt) UInt) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 0) None None None None false))\n"
      "  VarDecl(var_decl(\"s\" String(\"abc\") Array(Char 0) None None None None false))\n"
      "  VarDecl(var_decl(\"k\" InitList([Constant(ConstInt(9) None) Constant(ConstInt(8) None)] None None) Array(Const(UChar) 0) None None None None false))\n"
      "  VarDecl(var_decl(\"b\" String(\"xy\") Array(Char 0) None None None None false))\n"
      "  VarDecl(var_decl(\"m\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Int 2) 0) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.u\" InitList([Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None) Constant(ConstInt(7) None)] None None) Array(Int 0) Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.v\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Long 0) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add SizeOfExp(Var(\"t\" None) None) SizeOfExp(Var(\"s\" None) None) None) SizeOfExp(Var(\"k\" None) None) None) SizeOfExp(Var(\"b\" None) None) None) SizeOfExp(Var(\"m\" None) None) None) SizeOfExp(Var(\"@0.u\" None) None) None) SizeOfExp(Var(\"@1.v\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"t\\\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) Array(Int 3) None None None None false))\\n  VarDecl(var_decl(\\\"s\\\" String(\\\"abc\\\") Array(Char 4) None None None None false))\\n  VarDecl(var_decl(\\\"k\\\" InitList([Cast(Const(UChar) Constant(ConstInt(9) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(8) Int) Const(UChar))] Array(Const(UChar) 2) None) Array(Const(UChar) 2) None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" String(\\\"xy\\\") Array(Char 3) None None None None false))\\n  VarDecl(var_decl(\\\"m\\\" InitList([InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Array(Int 2) None) InitList([Constant(ConstInt(3) Int) Constant(ConstInt(4) Int)] Array(Int 2) None) InitList([Constant(ConstInt(5) Int) Constant(ConstInt(6) Int)] Array(Int 2) None)] Array(Array(Int 2) 3) None) Array(Array(Int 2) 3) None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.u\\\" InitList([Constant(ConstInt(4) Int) Constant(ConstInt(5) Int) Constant(ConstInt(6) Int) Constant(ConstInt(7) Int)] Array(Int 4) None) Array(Int 4) Static None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.v\\\" InitList([Cast(Long Constant(ConstInt(1) Int) Long) Cast(Long Constant(ConstInt(2) Int) Long)] Array(Long 2) None) Array(Long 2) None None None None false)))\\n    S(Return(Cast(Int Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add Binary(Add SizeOfExp(Var(\\\"t\\\" Array(Int 3)) UInt) SizeOfExp(Var(\\\"s\\\" Array(Char 4)) UInt) UInt) SizeOfExp(Var(\\\"k\\\" Array(Const(UChar) 2)) UInt) UInt) SizeOfExp(Var(\\\"b\\\" Array(Char 3)) UInt) UInt) SizeOfExp(Var(\\\"m\\\" Array(Array(Int 2) 3)) UInt) UInt) SizeOfExp(Var(\\\"@0.u\\\" Array(Int 4)) UInt) UInt) SizeOfExp(Var(\\\"@1.v\\\" Array(Long 2)) UInt) UInt) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [1, 2, 3]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [97, 98, 99, 0]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"k\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(UChar) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [9, 8]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [120, 121, 0]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null"
      ", \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"m\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Int 2) 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [1, 2]}, {\"tuple\": [3, 4]}, {\"tuple\": [5, 6]}]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [4, 5, 6, 7]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@1.v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Long 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}
