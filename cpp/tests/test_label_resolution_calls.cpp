// The calls that the tests of tests/test_label_resolution.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("label_resolution: test_label_resolution.py::TestLabelRewriting::test_function_name_used_in_prefix (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestLabelRewriting::test_function_name_used_in_prefix\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"other\" [] Block([\n"
      "    S(LabeledStmt(\"foo\" Null))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"other\" [] Block([\n"
      "    S(LabeledStmt(\".other@foo\" Null))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestLabelRewriting::test_goto_is_rewritten_to_match_label (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestLabelRewriting::test_goto_is_rewritten_to_match_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Goto(\"foo\"))\n"
      "    S(LabeledStmt(\"foo\" Null))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Goto(\".main@foo\"))\n"
      "    S(LabeledStmt(\".main@foo\" Null))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestLabelRewriting::test_label_can_be_used_before_declaration (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestLabelRewriting::test_label_can_be_used_before_declaration\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Goto(\"end\"))\n"
      "    S(LabeledStmt(\"end\" Return(Constant(ConstInt(0) None))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Goto(\".main@end\"))\n"
      "    S(LabeledStmt(\".main@end\" Return(Constant(ConstInt(0) None))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestLabelRewriting::test_label_is_rewritten_with_function_prefix (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestLabelRewriting::test_label_is_rewritten_with_function_prefix\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(LabeledStmt(\"foo\" Return(Constant(ConstInt(0) None))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(LabeledStmt(\".main@foo\" Return(Constant(ConstInt(0) None))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestDuplicateLabels::test_duplicate_inside_if_branch_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestDuplicateLabels::test_duplicate_inside_if_branch_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(IfStmt(Constant(ConstInt(1) None) LabeledStmt(\"x\" Null) LabeledStmt(\"x\" Null)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LabelResolutionError\", \"message\": \"duplicate label 'x'\"}\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestDuplicateLabels::test_nested_labels_with_same_name_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestDuplicateLabels::test_nested_labels_with_same_name_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(LabeledStmt(\"a\" LabeledStmt(\"a\" Null)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LabelResolutionError\", \"message\": \"duplicate label 'a'\"}\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestDuplicateLabels::test_two_labels_with_same_name_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestDuplicateLabels::test_two_labels_with_same_name_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(LabeledStmt(\"foo\" Null))\n"
      "    S(LabeledStmt(\"foo\" Null))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LabelResolutionError\", \"message\": \"duplicate label 'foo'\"}\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestUndefinedGoto::test_goto_inside_if_branch_to_nonexistent_label_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestUndefinedGoto::test_goto_inside_if_branch_to_nonexistent_label_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(IfStmt(Constant(ConstInt(1) None) Goto(\"missing\") None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LabelResolutionError\", \"message\": \"goto to undefined label 'missing'\"}\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestUndefinedGoto::test_goto_to_nonexistent_label_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestUndefinedGoto::test_goto_to_nonexistent_label_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Goto(\"foo\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LabelResolutionError\", \"message\": \"goto to undefined label 'foo'\"}\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestLabelsInIfStatements::test_goto_inside_if_branch_resolves_to_outer_label (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestLabelsInIfStatements::test_goto_inside_if_branch_resolves_to_outer_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(LabeledStmt(\"foo\" Null))\n"
      "    S(IfStmt(Constant(ConstInt(1) None) Goto(\"foo\") None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(LabeledStmt(\".main@foo\" Null))\n"
      "    S(IfStmt(Constant(ConstInt(1) None) Goto(\".main@foo\") None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestLabelsInIfStatements::test_label_inside_if_then_is_visible (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestLabelsInIfStatements::test_label_inside_if_then_is_visible\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(IfStmt(Constant(ConstInt(1) None) LabeledStmt(\"foo\" Null) None))\n"
      "    S(Goto(\"foo\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(IfStmt(Constant(ConstInt(1) None) LabeledStmt(\".main@foo\" Null) None))\n"
      "    S(Goto(\".main@foo\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestLabelsInLoops::test_break_continue_pass_through_unchanged (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestLabelsInLoops::test_break_continue_pass_through_unchanged\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(BreakStmt(\"\"))\n"
      "      S(ContinueStmt(\"\"))\n"
      "    ])) \"\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(BreakStmt(\"\"))\n"
      "      S(ContinueStmt(\"\"))\n"
      "    ])) \"\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_label_resolution.py::TestLabelsInLoops::test_duplicate_label_across_loop_boundary_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_label_resolution.py::TestLabelsInLoops::test_duplicate_label_across_loop_boundary_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { while (1) foo: ; foo: ; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) LabeledStmt(\"foo\" Null) \"\"))\n"
      "    S(LabeledStmt(\"foo\" Null))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestLabelsInLoops::test_duplicate_label_across_loop_boundary_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestLabelsInLoops::test_duplicate_label_across_loop_boundary_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) LabeledStmt(\"foo\" Null) \"\"))\n"
      "    S(LabeledStmt(\"foo\" Null))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LabelResolutionError\", \"message\": \"duplicate label 'foo'\"}\n");
}

TEST_CASE("parse: test_label_resolution.py::TestLabelsInLoops::test_goto_inside_loop_body_resolves_outer_label (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_label_resolution.py::TestLabelsInLoops::test_goto_inside_loop_body_resolves_outer_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { foo: ; while (1) goto foo; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(LabeledStmt(\"foo\" Null))\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Goto(\"foo\") \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestLabelsInLoops::test_goto_inside_loop_body_resolves_outer_label (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestLabelsInLoops::test_goto_inside_loop_body_resolves_outer_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(LabeledStmt(\"foo\" Null))\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Goto(\"foo\") \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(LabeledStmt(\".main@foo\" Null))\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Goto(\".main@foo\") \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_label_resolution.py::TestLabelsInLoops::test_label_inside_do_while_body_is_visible (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_label_resolution.py::TestLabelsInLoops::test_label_inside_do_while_body_is_visible\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { do foo: ; while (1); goto foo; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(DoWhileStmt(LabeledStmt(\"foo\" Null) Constant(ConstInt(1) None) \"\"))\n"
      "    S(Goto(\"foo\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestLabelsInLoops::test_label_inside_do_while_body_is_visible (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestLabelsInLoops::test_label_inside_do_while_body_is_visible\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(DoWhileStmt(LabeledStmt(\"foo\" Null) Constant(ConstInt(1) None) \"\"))\n"
      "    S(Goto(\"foo\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(DoWhileStmt(LabeledStmt(\".main@foo\" Null) Constant(ConstInt(1) None) \"\"))\n"
      "    S(Goto(\".main@foo\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_label_resolution.py::TestLabelsInLoops::test_label_inside_for_body_is_visible (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_label_resolution.py::TestLabelsInLoops::test_label_inside_for_body_is_visible\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { for (;;) foo: ; goto foo; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None LabeledStmt(\"foo\" Null) \"\" None))\n"
      "    S(Goto(\"foo\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestLabelsInLoops::test_label_inside_for_body_is_visible (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestLabelsInLoops::test_label_inside_for_body_is_visible\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None LabeledStmt(\"foo\" Null) \"\" None))\n"
      "    S(Goto(\"foo\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None LabeledStmt(\".main@foo\" Null) \"\" None))\n"
      "    S(Goto(\".main@foo\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_label_resolution.py::TestLabelsInLoops::test_label_inside_while_body_is_visible (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_label_resolution.py::TestLabelsInLoops::test_label_inside_while_body_is_visible\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { while (1) foo: ; goto foo; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) LabeledStmt(\"foo\" Null) \"\"))\n"
      "    S(Goto(\"foo\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestLabelsInLoops::test_label_inside_while_body_is_visible (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestLabelsInLoops::test_label_inside_while_body_is_visible\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) LabeledStmt(\"foo\" Null) \"\"))\n"
      "    S(Goto(\"foo\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) LabeledStmt(\".main@foo\" Null) \"\"))\n"
      "    S(Goto(\".main@foo\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestLabelsInLoops::test_loop_label_field_is_preserved (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestLabelsInLoops::test_loop_label_field_is_preserved\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Null \".main@loop_3\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Null \".main@loop_3\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_label_resolution.py::TestLabelsInLoops::test_undefined_goto_inside_for_body_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_label_resolution.py::TestLabelsInLoops::test_undefined_goto_inside_for_body_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { for (;;) goto missing; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None Goto(\"missing\") \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestLabelsInLoops::test_undefined_goto_inside_for_body_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestLabelsInLoops::test_undefined_goto_inside_for_body_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None Goto(\"missing\") \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LabelResolutionError\", \"message\": \"goto to undefined label 'missing'\"}\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestPassthrough::test_declarations_pass_through (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestPassthrough::test_declarations_pass_through\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestPassthrough::test_function_without_labels_or_gotos (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestPassthrough::test_function_without_labels_or_gotos\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(42) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(42) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestPassthrough::test_nested_if_without_labels (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestPassthrough::test_nested_if_without_labels\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(IfStmt(Constant(ConstInt(1) None) Return(Constant(ConstInt(2) None)) Return(Constant(ConstInt(3) None))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(IfStmt(Constant(ConstInt(1) None) Return(Constant(ConstInt(2) None)) Return(Constant(ConstInt(3) None))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestResolveProgram::test_wraps_function_in_program (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestResolveProgram::test_wraps_function_in_program\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(LabeledStmt(\"foo\" Null))\n"
      "    S(Goto(\"foo\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(LabeledStmt(\".main@foo\" Null))\n"
      "    S(Goto(\".main@foo\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_label_resolution.py::TestIntegrationWithParser::test_basic_program (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_label_resolution.py::TestIntegrationWithParser::test_basic_program\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { foo: goto foo; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(LabeledStmt(\"foo\" Goto(\"foo\")))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestIntegrationWithParser::test_basic_program (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestIntegrationWithParser::test_basic_program\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(LabeledStmt(\"foo\" Goto(\"foo\")))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(LabeledStmt(\".main@foo\" Goto(\".main@foo\")))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_label_resolution.py::TestIntegrationWithParser::test_duplicate_label_from_source (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_label_resolution.py::TestIntegrationWithParser::test_duplicate_label_from_source\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { foo: ; foo: ; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(LabeledStmt(\"foo\" Null))\n"
      "    S(LabeledStmt(\"foo\" Null))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestIntegrationWithParser::test_duplicate_label_from_source (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestIntegrationWithParser::test_duplicate_label_from_source\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(LabeledStmt(\"foo\" Null))\n"
      "    S(LabeledStmt(\"foo\" Null))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LabelResolutionError\", \"message\": \"duplicate label 'foo'\"}\n");
}

TEST_CASE("parse: test_label_resolution.py::TestIntegrationWithParser::test_undefined_goto_target_from_source (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_label_resolution.py::TestIntegrationWithParser::test_undefined_goto_target_from_source\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { goto nowhere; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Goto(\"nowhere\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_label_resolution.py::TestIntegrationWithParser::test_undefined_goto_target_from_source (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_label_resolution.py::TestIntegrationWithParser::test_undefined_goto_target_from_source\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Goto(\"nowhere\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LabelResolutionError\", \"message\": \"goto to undefined label 'nowhere'\"}\n");
}
