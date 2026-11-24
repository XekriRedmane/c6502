// The calls that the tests of tests/test_statement_attributes.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

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
