// The calls that the tests of tests/test_statement_attributes.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"
#include "preprocessor_case.h"

TEST_CASE("parse: test_statement_attributes.py::TestGrammar::test_after_a_label (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestGrammar::test_after_a_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int x) { L: [[c6502::keep]] for (;x;) x--; return x; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    S(LabeledStmt(\"L\" ForStmt(InitExp(None) Var(\"x\" None) None Expression(Postfix(Decrement Var(\"x\" None) None)) \"\" \"keep\")))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestGrammar::test_attribute_declaration_is_not_a_statement (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestGrammar::test_attribute_declaration_is_not_a_statement\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int x) { switch (x) { case 1: x++; [[fallthrough]]; case 2: x++; } return x; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    S(SwitchStmt(Var(\"x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(1) None) Expression(Postfix(Increment Var(\"x\" None) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstInt(2) None) Expression(Postfix(Increment Var(\"x\" None) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestGrammar::test_declarations_in_a_block_keep_their_attributes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestGrammar::test_declarations_in_a_block_keep_their_attributes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T;\\nint f(void) {\\n    [[c6502::zeropage]] static unsigned char z;\\n    [[maybe_unused]] T y = 1;\\n    [[gnu::cold]] T * p = &y;\\n    return z + *p;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"z\" None UChar Static None ZeroPage(None) None false)))\n"
      "    D(VarDecl(var_decl(\"y\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"y\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"z\" None) Dereference(Var(\"p\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestGrammar::test_maybe_unused_on_a_label (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestGrammar::test_maybe_unused_on_a_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int x) { [[maybe_unused]] L: x++; return x; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    S(LabeledStmt(\"L\" Expression(Postfix(Increment Var(\"x\" None) None))))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestGrammar::test_other_prefixes_are_ignored_on_statements (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestGrammar::test_other_prefixes_are_ignored_on_statements\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int x) { [[gnu::hot]] x++; return x; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    S(Expression(Postfix(Increment Var(\"x\" None) None)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestLoopAttributes::test_errors (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestLoopAttributes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(void) { [[c6502::keep]] while (1) ; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::keep` goes before a `for` statement; saw it on a `while` statement\"}\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestLoopAttributes::test_errors (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestLoopAttributes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(void) { [[c6502::unroll]] do ; while (0); }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::unroll` goes before a `for` statement; saw it on a `do` statement\"}\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestLoopAttributes::test_errors (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestLoopAttributes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(void) { [[c6502::unroll]] { } }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::unroll` goes before a `for` statement; saw it on a block\"}\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestLoopAttributes::test_errors (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestLoopAttributes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(void) { [[c6502::keep, c6502::unroll]] for (;;) ; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::unroll` and `c6502::keep` are both given on one `for` statement. An unrolled loop leaves no loop to keep\"}\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestLoopAttributes::test_errors (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestLoopAttributes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(void) { [[c6502::keep]] [[c6502::keep]] for (;;) ; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::keep` is given twice on a `for` statement\"}\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestLoopAttributes::test_errors (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestLoopAttributes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(void) { [[c6502::unroll(4)]] for (;;) ; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::unroll` doesn't take an argument\"}\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestLoopAttributes::test_errors (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestLoopAttributes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(void) { [[c6502::keep]]; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::keep` goes before a `for` statement; it doesn't apply to an attribute declaration\"}\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestLoopAttributes::test_errors (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestLoopAttributes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(void) { [[c6502::keep]] L: for (;;) ; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"attributes before a label apply to the label, and `c6502::keep` doesn't apply to one. Put it after the label, just before the statement, as in `L: [[c6502::keep]] ...`\"}\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestLoopAttributes::test_errors (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestLoopAttributes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(int x) { switch (x) { [[c6502::unroll]] case 1: for (;;) ; } }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"attributes before a label apply to the label, and `c6502::unroll` doesn't apply to one. Put it after the label, just before the statement, as in `L: [[c6502::unroll]] ...`\"}\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestLoopAttributes::test_errors (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestLoopAttributes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::unroll]] void f(void);\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::unroll` goes before a `for` statement; it doesn't apply to function declaration 'f'\"}\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestLoopAttributes::test_errors (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestLoopAttributes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::keep]] void f(void) { }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::keep` goes before a `for` statement; it doesn't apply to function definition 'f'\"}\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestLoopAttributes::test_errors (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestLoopAttributes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::keep]] unsigned char n;\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::keep` goes before a `for` statement; it doesn't apply to object declaration `n`\"}\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestLoopAttributes::test_errors (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestLoopAttributes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(void) { [[c6502::keep]] unsigned char n; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::keep` goes before a `for` statement; it doesn't apply to object declaration `n`\"}\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestLoopAttributes::test_errors (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestLoopAttributes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(unsigned char n [[c6502::unroll]]);\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::unroll` goes before a `for` statement; it doesn't apply to parameter 'n'\"}\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestLoopAttributes::test_errors (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestLoopAttributes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(void) { [[c6502::zp_abi]] for (;;) ; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::zp_abi` doesn't apply to a statement; saw it on a `for` statement\"}\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestLoopAttributes::test_errors (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestLoopAttributes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(void) { [[deprecated]] for (;;) ; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"the standard attribute `deprecated` can't apply to a statement; saw it on a `for` statement\"}\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestLoopAttributes::test_errors (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestLoopAttributes::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(void) { [[unroll]] for (;;) ; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`unroll` is a c6502 attribute, so it needs c6502's prefix: `[[c6502::unroll]]`\"}\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestLoopAttributes::test_keep_sets_loop_attribute (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestLoopAttributes::test_keep_sets_loop_attribute\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(void) { [[c6502::keep]] for (unsigned char y = 0xB4; --y;) { } }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"y\" Constant(ConstInt(180) None) UChar None None None None false)]) Prefix(Decrement Var(\"y\" None) None) None Compound(Block([])) \"\" \"keep\"))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestSaveHardwareStackPlacement::test_errors (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestSaveHardwareStackPlacement::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int g(int);\\n[[c6502::save_hardware_stack]] int r = g(1);\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::save_hardware_stack]]` goes before a statement or a declaration inside a function; saw it on a declaration at file scope\"}\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestSaveHardwareStackPlacement::test_errors (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestSaveHardwareStackPlacement::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::save_hardware_stack]] int g(int);\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::save_hardware_stack]]` applies to the calls in a declaration's initializers, and `g` declares a function\"}\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestSaveHardwareStackPlacement::test_errors (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestSaveHardwareStackPlacement::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::save_hardware_stack]] int g(void) { return 0; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::save_hardware_stack` goes before an expression statement, a `return` statement, or a declaration inside a function; it doesn't apply to function definition 'g'\"}\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestSaveHardwareStackPlacement::test_errors (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestSaveHardwareStackPlacement::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s { [[c6502::save_hardware_stack]] int m; };\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::save_hardware_stack` goes before an expression statement, a `return` statement, or a declaration inside a function; it doesn't apply to member 'm'\"}\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestSaveHardwareStackPlacement::test_errors (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestSaveHardwareStackPlacement::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int x) { [[c6502::save_hardware_stack]] do f(x); while (0); return 0; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::save_hardware_stack]]` goes before an expression statement, a `return` statement, or a declaration, so that it covers only the calls in that one; saw it on a `do` statement\"}\n");
}

TEST_CASE("parse: test_statement_attributes.py::TestSaveHardwareStackPlacement::test_every_declarator_of_a_declaration (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_statement_attributes.py::TestSaveHardwareStackPlacement::test_every_declarator_of_a_declaration\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int g(int);\\nint f(int x) {\\n    [[c6502::save_hardware_stack]] int a = g(1), b = g(2);\\n    return a + b;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"g\" [\"\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    D(VarDecl(var_decl(\"a\" FunctionCall(\"g\" [Constant(ConstInt(1) None)] None true) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" FunctionCall(\"g\" [Constant(ConstInt(2) None)] None true) Int None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("preprocess: test_statement_attributes.py::TestOldPragmas::test_each_names_its_replacement (call 1)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": null, \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": [\"PreprocessorError\", \"line 2: c6502 no longer takes `#pragma c6502 loop unroll(enable)`; write the statement attribute `[[c6502::unroll]]` before the loop instead, as in `[[c6502::unroll]] for (...)`\"], \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": null, \"source\": \"int x;\\n#pragma c6502 loop unroll(enable)\\nint y;\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_statement_attributes.py::TestOldPragmas::test_each_names_its_replacement (call 2)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": null, \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": [\"PreprocessorError\", \"line 2: c6502 no longer takes `#pragma c6502 loop unroll(disable)`; it never did anything, so delete the line\"], \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": null, \"source\": \"int x;\\n#pragma c6502 loop unroll(disable)\\nint y;\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_statement_attributes.py::TestOldPragmas::test_each_names_its_replacement (call 3)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": null, \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": [\"PreprocessorError\", \"line 2: c6502 no longer takes `#pragma c6502 loop keep`; write the statement attribute `[[c6502::keep]]` before the loop instead, as in `[[c6502::keep]] for (...)`\"], \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": null, \"source\": \"int x;\\n#pragma c6502 loop keep\\nint y;\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_statement_attributes.py::TestOldPragmas::test_each_names_its_replacement (call 4)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": null, \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": [\"PreprocessorError\", \"line 2: c6502 no longer takes `#pragma c6502 call save(hardware_stack)`; write the statement attribute `[[c6502::save_hardware_stack]]` before the statement or declaration instead, as in `[[c6502::save_hardware_stack]] f(x);`\"], \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": null, \"source\": \"int x;\\n#pragma c6502 call save(hardware_stack)\\nint y;\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_statement_attributes.py::TestOldPragmas::test_each_names_its_replacement (call 5)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": null, \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": [\"PreprocessorError\", \"line 2: c6502 no longer takes `#pragma c6502   loop  unroll ( enable )`; write the statement attribute `[[c6502::unroll]]` before the loop instead, as in `[[c6502::unroll]] for (...)`\"], \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": null, \"source\": \"int x;\\n#  pragma   c6502   loop  unroll ( enable )\\nint y;\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_statement_attributes.py::TestOldPragmas::test_line_in_a_header_names_the_header (call 1)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": null, \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": [\"PreprocessorError\", \"line 2 of C:\\\\Users\\\\rober\\\\AppData\\\\Local\\\\Temp\\\\tmpr0rajxd2\\\\loops.h: c6502 no longer takes `#pragma c6502 loop keep`; write the statement attribute `[[c6502::keep]]` before the loop instead, as in `[[c6502::keep]] for (...)`\"], \"filename\": \"C:\\\\Users\\\\rober\\\\AppData\\\\Local\\\\Temp\\\\tmpr0rajxd2\\\\main.c\", \"files\": {\"C:\\\\Users\\\\rober\\\\AppData\\\\Local\\\\Temp\\\\tmpr0rajxd2\\\\loops.h\": \"int y;\\r\\n#pragma c6502 loop keep\\r\\n\", \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\\\\stdint.h\": \"/* stdint.h \\u00e2\\u0080\\u0094 fixed-width integer types for c6502 (C99 \\u00c2\\u00a77.18).\\n *\\n * Mapping to c6502's integer model:\\n *   int8_t   = signed char            (1 byte, -128..127)\\n *   uint8_t  = unsigned char          (1 byte, 0..255)\\n *   int16_t  = int                    (2 bytes, -32768..32767)\\n *   uint16_t = unsigned int           (2 bytes, 0..65535)\\n *   int32_t  = long                   (4 bytes, -2^31..2^31-1)\\n *   uint32_t = unsigned long          (4 bytes, 0..2^32-1)\\n *   int64_t  = long long              (8 bytes, -2^63..2^63-1)\\n *   uint64_t = unsigned long long     (8 bytes, 0..2^64-1)\\n *\\n * `_least` and `_fast` types alias the matching exact-width types.\\n * On the 6502 the \\\"fastest\\\" choice for each width is just the\\n * exact-width type \\u00e2\\u0080\\u0094 wider types take more bytes and more cycles\\n * regardless of the underlying ALU.\\n *\\n *   intptr_t  = int                   (6502 addresses are 16 bits)\\n *   uintptr_t = unsigned int\\n *   intmax_t  = long long             (widest integer modeled)\\n *   uintmax_t = unsigned long long\\n *\\n * `PTRDIFF_*`, `SIZE_MAX`, and `WCHAR_*` give the ranges of the types\\n * <stddef.h> declares. `sig_atomic_t` and `wint_t` aren't modeled,\\n * so `SIG_ATOMIC_*` and `WINT_*` aren't defined here.\\n */\\n#ifndef _STDINT_H\\n#define _STDINT_H\\n\\n/* exact-width integer types \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.1.1 */\\ntypedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n/* minimum-width \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.1.2 */\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n/* fastest minimum-width \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.1.3 */\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n/* integer types capable of holding object pointers \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.1.4 */\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n/* greatest-width integer types \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.1.5 */\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\n\\n/* limits of exact-width integer types \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.2.1 */\\n#define INT8_MIN            (-128)\\n#define INT8_MAX            127\\n#define UINT8_MAX           255\\n\\n#define INT16_MAX           32767\\n#define INT16_MIN           (-INT16_MAX - 1)\\n#define UINT16_MAX          65535U\\n\\n#define INT32_MAX           2147483647L\\n#define INT32_MIN           (-INT32_MAX - 1L)\\n#define UINT32_MAX          4294967295UL\\n\\n#define INT64_MAX           9223372036854775807LL\\n#define INT64_MIN           (-INT64_MAX - 1LL)\\n#d"
      "efine UINT64_MAX          18446744073709551615ULL\\n\\n/* limits of minimum-width \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.2.2 */\\n#define INT_LEAST8_MIN      INT8_MIN\\n#define INT_LEAST8_MAX      INT8_MAX\\n#define UINT_LEAST8_MAX     UINT8_MAX\\n#define INT_LEAST16_MIN     INT16_MIN\\n#define INT_LEAST16_MAX     INT16_MAX\\n#define UINT_LEAST16_MAX    UINT16_MAX\\n#define INT_LEAST32_MIN     INT32_MIN\\n#define INT_LEAST32_MAX     INT32_MAX\\n#define UINT_LEAST32_MAX    UINT32_MAX\\n#define INT_LEAST64_MIN     INT64_MIN\\n#define INT_LEAST64_MAX     INT64_MAX\\n#define UINT_LEAST64_MAX    UINT64_MAX\\n\\n/* limits of fastest minimum-width \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.2.3 */\\n#define INT_FAST8_MIN       INT8_MIN\\n#define INT_FAST8_MAX       INT8_MAX\\n#define UINT_FAST8_MAX      UINT8_MAX\\n#define INT_FAST16_MIN      INT16_MIN\\n#define INT_FAST16_MAX      INT16_MAX\\n#define UINT_FAST16_MAX     UINT16_MAX\\n#define INT_FAST32_MIN      INT32_MIN\\n#define INT_FAST32_MAX      INT32_MAX\\n#define UINT_FAST32_MAX     UINT32_MAX\\n#define INT_FAST64_MIN      INT64_MIN\\n#define INT_FAST64_MAX      INT64_MAX\\n#define UINT_FAST64_MAX     UINT64_MAX\\n\\n/* limits of integer types capable of holding object pointers \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.2.4 */\\n#define INTPTR_MIN          INT16_MIN\\n#define INTPTR_MAX          INT16_MAX\\n#define UINTPTR_MAX         UINT16_MAX\\n\\n/* limits of greatest-width \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.2.5 */\\n#define INTMAX_MIN          INT64_MIN\\n#define INTMAX_MAX          INT64_MAX\\n#define UINTMAX_MAX         UINT64_MAX\\n\\n/* limits of other integer types \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.3.\\n * The subtraction of two pointers gives an `int` (2 bytes signed),\\n * `sizeof` gives an `unsigned int`, and <stddef.h> makes `wchar_t`\\n * an `int`. */\\n#define PTRDIFF_MIN         INT16_MIN\\n#define PTRDIFF_MAX         INT16_MAX\\n#define SIZE_MAX            UINT16_MAX\\n#define WCHAR_MIN           INT16_MIN\\n#define WCHAR_MAX           INT16_MAX\\n\\n/* macros for integer constants \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.4 */\\n#define INT8_C(c)           c\\n#define INT16_C(c)          c\\n#define INT32_C(c)          c ## L\\n#define INT64_C(c)          c ## LL\\n\\n#define UINT8_C(c)          c ## U\\n#define UINT16_C(c)         c ## U\\n#define UINT32_C(c)         c ## UL\\n#define UINT64_C(c)         c ## ULL\\n\\n#define INTMAX_C(c)         c ## LL\\n#define UINTMAX_C(c)        c ## ULL\\n\\n#endif /* _STDINT_H */\\n\"}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": null, \"source\": \"#include <stdint.h>\\n#include \\\"loops.h\\\"\\nint x;\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_statement_attributes.py::TestOldPragmas::test_line_is_the_source_line (call 1)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": null, \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": [\"PreprocessorError\", \"line 3: c6502 no longer takes `#pragma c6502 loop keep`; write the statement attribute `[[c6502::keep]]` before the loop instead, as in `[[c6502::keep]] for (...)`\"], \"filename\": null, \"files\": {\"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\\\\stdint.h\": \"/* stdint.h \\u00e2\\u0080\\u0094 fixed-width integer types for c6502 (C99 \\u00c2\\u00a77.18).\\n *\\n * Mapping to c6502's integer model:\\n *   int8_t   = signed char            (1 byte, -128..127)\\n *   uint8_t  = unsigned char          (1 byte, 0..255)\\n *   int16_t  = int                    (2 bytes, -32768..32767)\\n *   uint16_t = unsigned int           (2 bytes, 0..65535)\\n *   int32_t  = long                   (4 bytes, -2^31..2^31-1)\\n *   uint32_t = unsigned long          (4 bytes, 0..2^32-1)\\n *   int64_t  = long long              (8 bytes, -2^63..2^63-1)\\n *   uint64_t = unsigned long long     (8 bytes, 0..2^64-1)\\n *\\n * `_least` and `_fast` types alias the matching exact-width types.\\n * On the 6502 the \\\"fastest\\\" choice for each width is just the\\n * exact-width type \\u00e2\\u0080\\u0094 wider types take more bytes and more cycles\\n * regardless of the underlying ALU.\\n *\\n *   intptr_t  = int                   (6502 addresses are 16 bits)\\n *   uintptr_t = unsigned int\\n *   intmax_t  = long long             (widest integer modeled)\\n *   uintmax_t = unsigned long long\\n *\\n * `PTRDIFF_*`, `SIZE_MAX`, and `WCHAR_*` give the ranges of the types\\n * <stddef.h> declares. `sig_atomic_t` and `wint_t` aren't modeled,\\n * so `SIG_ATOMIC_*` and `WINT_*` aren't defined here.\\n */\\n#ifndef _STDINT_H\\n#define _STDINT_H\\n\\n/* exact-width integer types \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.1.1 */\\ntypedef signed char int8_t;\\ntypedef unsigned char uint8_t;\\ntypedef int int16_t;\\ntypedef unsigned int uint16_t;\\ntypedef long int32_t;\\ntypedef unsigned long uint32_t;\\ntypedef long long int64_t;\\ntypedef unsigned long long uint64_t;\\n\\n/* minimum-width \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.1.2 */\\ntypedef int8_t int_least8_t;\\ntypedef uint8_t uint_least8_t;\\ntypedef int16_t int_least16_t;\\ntypedef uint16_t uint_least16_t;\\ntypedef int32_t int_least32_t;\\ntypedef uint32_t uint_least32_t;\\ntypedef int64_t int_least64_t;\\ntypedef uint64_t uint_least64_t;\\n\\n/* fastest minimum-width \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.1.3 */\\ntypedef int8_t int_fast8_t;\\ntypedef uint8_t uint_fast8_t;\\ntypedef int16_t int_fast16_t;\\ntypedef uint16_t uint_fast16_t;\\ntypedef int32_t int_fast32_t;\\ntypedef uint32_t uint_fast32_t;\\ntypedef int64_t int_fast64_t;\\ntypedef uint64_t uint_fast64_t;\\n\\n/* integer types capable of holding object pointers \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.1.4 */\\ntypedef int intptr_t;\\ntypedef unsigned int uintptr_t;\\n\\n/* greatest-width integer types \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.1.5 */\\ntypedef long long intmax_t;\\ntypedef unsigned long long uintmax_t;\\n\\n/* limits of exact-width integer types \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.2.1 */\\n#define INT8_MIN            (-128)\\n#define INT8_MAX            127\\n#define UINT8_MAX           255\\n\\n#define INT16_MAX           32767\\n#define INT16_MIN           (-INT16_MAX - 1)\\n#define UINT16_MAX          65535U\\n\\n#define INT32_MAX           2147483647L\\n#define INT32_MIN           (-INT32_MAX - 1L)\\n#define UINT32_MAX          4294967295UL\\n\\n#define INT64_MAX           9223372036854775807LL\\n#define INT64_MIN           (-INT64_MAX - 1LL)\\n#define UINT64_MAX          18446744073709551615ULL\\n\\n/* limits of minimum-width \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.2.2 */\\n#define INT_LEAST8_MIN      INT8_MIN\\n#define INT_LEAST8_MAX      INT8_MAX\\n#define UINT_LEAST8_MAX     UINT8_MAX\\n#define INT_LEAST16_MIN     "
      "INT16_MIN\\n#define INT_LEAST16_MAX     INT16_MAX\\n#define UINT_LEAST16_MAX    UINT16_MAX\\n#define INT_LEAST32_MIN     INT32_MIN\\n#define INT_LEAST32_MAX     INT32_MAX\\n#define UINT_LEAST32_MAX    UINT32_MAX\\n#define INT_LEAST64_MIN     INT64_MIN\\n#define INT_LEAST64_MAX     INT64_MAX\\n#define UINT_LEAST64_MAX    UINT64_MAX\\n\\n/* limits of fastest minimum-width \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.2.3 */\\n#define INT_FAST8_MIN       INT8_MIN\\n#define INT_FAST8_MAX       INT8_MAX\\n#define UINT_FAST8_MAX      UINT8_MAX\\n#define INT_FAST16_MIN      INT16_MIN\\n#define INT_FAST16_MAX      INT16_MAX\\n#define UINT_FAST16_MAX     UINT16_MAX\\n#define INT_FAST32_MIN      INT32_MIN\\n#define INT_FAST32_MAX      INT32_MAX\\n#define UINT_FAST32_MAX     UINT32_MAX\\n#define INT_FAST64_MIN      INT64_MIN\\n#define INT_FAST64_MAX      INT64_MAX\\n#define UINT_FAST64_MAX     UINT64_MAX\\n\\n/* limits of integer types capable of holding object pointers \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.2.4 */\\n#define INTPTR_MIN          INT16_MIN\\n#define INTPTR_MAX          INT16_MAX\\n#define UINTPTR_MAX         UINT16_MAX\\n\\n/* limits of greatest-width \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.2.5 */\\n#define INTMAX_MIN          INT64_MIN\\n#define INTMAX_MAX          INT64_MAX\\n#define UINTMAX_MAX         UINT64_MAX\\n\\n/* limits of other integer types \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.3.\\n * The subtraction of two pointers gives an `int` (2 bytes signed),\\n * `sizeof` gives an `unsigned int`, and <stddef.h> makes `wchar_t`\\n * an `int`. */\\n#define PTRDIFF_MIN         INT16_MIN\\n#define PTRDIFF_MAX         INT16_MAX\\n#define SIZE_MAX            UINT16_MAX\\n#define WCHAR_MIN           INT16_MIN\\n#define WCHAR_MAX           INT16_MAX\\n\\n/* macros for integer constants \\u00e2\\u0080\\u0094 \\u00c2\\u00a77.18.4 */\\n#define INT8_C(c)           c\\n#define INT16_C(c)          c\\n#define INT32_C(c)          c ## L\\n#define INT64_C(c)          c ## LL\\n\\n#define UINT8_C(c)          c ## U\\n#define UINT16_C(c)         c ## U\\n#define UINT32_C(c)         c ## UL\\n#define UINT64_C(c)         c ## ULL\\n\\n#define INTMAX_C(c)         c ## LL\\n#define UINTMAX_C(c)        c ## ULL\\n\\n#endif /* _STDINT_H */\\n\"}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": null, \"source\": \"#include <stdint.h>\\n\\n#pragma c6502 loop keep\\nint x;\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_statement_attributes.py::TestOldPragmas::test_other_pragmas_are_dropped (call 1)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": null, \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"\\nint x;\\n\", \"source\": \"#pragma GCC diagnostic ignored \\\"-Wfoo\\\"\\nint x;\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_statement_attributes.py::TestOldPragmas::test_other_pragmas_are_dropped (call 2)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": null, \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"\\nint x;\\n\", \"source\": \"#pragma c6502x\\nint x;\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_statement_attributes.py::TestOldPragmas::test_other_pragmas_are_dropped (call 3)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": null, \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"\\nint x;\\n\", \"source\": \"#pragma once\\nint x;\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_statement_attributes.py::TestOldPragmas::test_pragma_in_a_skipped_group (call 1)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": null, \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"\\n\\n\\nint x;\\n\", \"source\": \"#if 0\\n#pragma c6502 loop keep\\n#endif\\nint x;\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_statement_attributes.py::TestOldPragmas::test_unknown_c6502_pragma (call 1)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": null, \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": [\"PreprocessorError\", \"line 1: c6502 takes no `#pragma c6502` directives, so `#pragma c6502 wat` is an error. Its directives are the statement attributes `[[c6502::unroll]]` and `[[c6502::keep]]` before a `for` loop, and `[[c6502::save_hardware_stack]]` before a statement or declaration\"], \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": null, \"source\": \"#pragma c6502 wat\\nint x;\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}
