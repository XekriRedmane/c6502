// The calls that the tests of tests/test_recursion_saves.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"
#include "preprocessor_case.h"

TEST_CASE("parse: test_recursion_saves.py::TestSyntax::test_marks_the_calls_after_it (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_recursion_saves.py::TestSyntax::test_marks_the_calls_after_it\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int); int g(void);\\nint h(void) {\\nint x = 0;\\n[[c6502::save_hardware_stack]] x = f(1) + g();\\nreturn f(2);\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"h\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"x\" None) Binary(Add FunctionCall(\"f\" [Constant(ConstInt(1) None)] None true) FunctionCall(\"g\" [] None true) None) None)))\n"
      "    S(Return(FunctionCall(\"f\" [Constant(ConstInt(2) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_recursion_saves.py::TestSyntax::test_marks_the_calls_after_it (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_recursion_saves.py::TestSyntax::test_marks_the_calls_after_it\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int); int g(void);\\nint h(void) {\\nint x = 0;\\n[[c6502::save_hardware_stack]] return f(1) + g();\\nreturn f(2);\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"h\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(Return(Binary(Add FunctionCall(\"f\" [Constant(ConstInt(1) None)] None true) FunctionCall(\"g\" [] None true) None)))\n"
      "    S(Return(FunctionCall(\"f\" [Constant(ConstInt(2) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_recursion_saves.py::TestSyntax::test_marks_the_calls_after_it (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_recursion_saves.py::TestSyntax::test_marks_the_calls_after_it\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int); int g(void);\\nint h(void) {\\nint x = 0;\\n[[c6502::save_hardware_stack]] int x = f(1) + g();\\nreturn f(2);\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"h\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"x\" Binary(Add FunctionCall(\"f\" [Constant(ConstInt(1) None)] None true) FunctionCall(\"g\" [] None true) None) Int None None None None false)))\n"
      "    S(Return(FunctionCall(\"f\" [Constant(ConstInt(2) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("preprocess: test_recursion_saves.py::TestSyntax::test_old_pragma_names_the_attribute (call 1)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": null, \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": [\"PreprocessorError\", \"line 2: c6502 no longer takes `#pragma c6502 call save(hardware_stack)`; write the statement attribute `[[c6502::save_hardware_stack]]` before the statement or declaration instead, as in `[[c6502::save_hardware_stack]] f(x);`\"], \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": null, \"source\": \"void f(void) {\\n#pragma c6502 call save(hardware_stack)\\nf();\\n}\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("parse: test_recursion_saves.py::TestSyntax::test_rejected_positions (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_recursion_saves.py::TestSyntax::test_rejected_positions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int);\\nvoid h(void) {\\nint x;\\n[[c6502::save_hardware_stack]] if (f(1)) x = 1;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::save_hardware_stack]]` goes before an expression statement, a `return` statement, or a declaration, so that it covers only the calls in that one; saw it on an `if` statement\"}\n");
}

TEST_CASE("parse: test_recursion_saves.py::TestSyntax::test_rejected_positions (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_recursion_saves.py::TestSyntax::test_rejected_positions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int);\\nvoid h(void) {\\nint x;\\n[[c6502::save_hardware_stack]] { f(1); }\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::save_hardware_stack]]` goes before an expression statement, a `return` statement, or a declaration, so that it covers only the calls in that one; saw it on a block\"}\n");
}

TEST_CASE("parse: test_recursion_saves.py::TestSyntax::test_rejected_positions (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_recursion_saves.py::TestSyntax::test_rejected_positions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int);\\nvoid h(void) {\\nint x;\\n[[c6502::save_hardware_stack]] while (f(1)) x = 1;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::save_hardware_stack]]` goes before an expression statement, a `return` statement, or a declaration, so that it covers only the calls in that one; saw it on a `while` statement\"}\n");
}

TEST_CASE("parse: test_recursion_saves.py::TestSyntax::test_rejected_positions (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_recursion_saves.py::TestSyntax::test_rejected_positions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int);\\nvoid h(void) {\\nint x;\\n[[c6502::save_hardware_stack]] for (;;) f(1);\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::save_hardware_stack]]` goes before an expression statement, a `return` statement, or a declaration, so that it covers only the calls in that one; saw it on a `for` statement\"}\n");
}

TEST_CASE("parse: test_recursion_saves.py::TestSyntax::test_rejected_positions (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_recursion_saves.py::TestSyntax::test_rejected_positions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int);\\nvoid h(void) {\\nint x;\\n[[c6502::save_hardware_stack]] x = 1;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::save_hardware_stack]]` must come before a statement or declaration that calls a function\"}\n");
}

TEST_CASE("parse: test_recursion_saves.py::TestSyntax::test_rejected_positions (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_recursion_saves.py::TestSyntax::test_rejected_positions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int);\\nvoid h(void) {\\nint x;\\n[[c6502::save_hardware_stack]] int y = 2;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::save_hardware_stack]]` must come before a statement or declaration that calls a function\"}\n");
}

TEST_CASE("parse: test_recursion_saves.py::TestSyntax::test_rejected_positions (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_recursion_saves.py::TestSyntax::test_rejected_positions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int);\\nvoid h(void) {\\nint x;\\n[[c6502::save_hardware_stack]] int g(int);\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::save_hardware_stack]]` applies to the calls in a declaration's initializers, and `g` declares a function\"}\n");
}

TEST_CASE("parse: test_recursion_saves.py::TestSyntax::test_rejected_positions (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_recursion_saves.py::TestSyntax::test_rejected_positions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int);\\nvoid h(void) {\\nint x;\\n[[c6502::save_hardware_stack]] typedef int T;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::save_hardware_stack]]` applies to the calls in a declaration's initializers, and a typedef declaration has none\"}\n");
}

TEST_CASE("parse: test_recursion_saves.py::TestSyntax::test_rejected_positions (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_recursion_saves.py::TestSyntax::test_rejected_positions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int);\\nvoid h(void) {\\nint x;\\nint y [[c6502::save_hardware_stack]] = f(1);\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::save_hardware_stack]]` goes at the front of the declaration of `y`, where it applies to the calls in its initializers, and not after the name\"}\n");
}

TEST_CASE("parse: test_recursion_saves.py::TestSyntax::test_rejected_positions (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_recursion_saves.py::TestSyntax::test_rejected_positions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int);\\nvoid h(void) {\\nint x;\\nfor ([[c6502::save_hardware_stack]] int i = f(1); i;) x = 1;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::save_hardware_stack]]` can't go on the declaration in a `for` statement's first clause. It goes before an expression statement, a `return` statement, or a declaration in a block\"}\n");
}

TEST_CASE("parse: test_recursion_saves.py::TestSyntax::test_rejected_positions (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_recursion_saves.py::TestSyntax::test_rejected_positions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int);\\nvoid h(void) {\\nint x;\\n[[c6502::save_hardware_stack]] L: f(1);\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"attributes before a label apply to the label, and `c6502::save_hardware_stack` doesn't apply to one. Put it after the label, just before the statement, as in `L: [[c6502::save_hardware_stack]] ...`\"}\n");
}

TEST_CASE("parse: test_recursion_saves.py::TestSyntax::test_rejected_positions (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_recursion_saves.py::TestSyntax::test_rejected_positions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int);\\nvoid h(void) {\\nint x;\\n[[c6502::save_hardware_stack]] [[c6502::save_hardware_stack]] x = f(1);\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::save_hardware_stack` is given twice on an expression statement\"}\n");
}

TEST_CASE("parse: test_recursion_saves.py::TestSyntax::test_rejected_positions (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_recursion_saves.py::TestSyntax::test_rejected_positions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int);\\nvoid h(void) {\\nint x;\\n[[c6502::save_hardware_stack]] [[c6502::save_hardware_stack]] int y = f(1);\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::save_hardware_stack` is given twice on one declaration\"}\n");
}

TEST_CASE("parse: test_recursion_saves.py::TestSyntax::test_rejected_positions (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_recursion_saves.py::TestSyntax::test_rejected_positions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int);\\nvoid h(void) {\\nint x;\\n[[c6502::save_hardware_stack(1)]] x = f(1);\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::save_hardware_stack` doesn't take an argument\"}\n");
}
