// The calls that the tests of tests/test_loop_labeling.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("loop_labeling: test_loop_labeling.py::TestSimpleLoops::test_consecutive_loops_get_distinct_labels (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestSimpleLoops::test_consecutive_loops_get_distinct_labels\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\".loop@0\") \".loop@0\"))\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\".loop@1\") \".loop@1\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestSimpleLoops::test_do_while_loop_gets_label (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestSimpleLoops::test_do_while_loop_gets_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(DoWhileStmt(BreakStmt(\"\") Constant(ConstInt(1) None) \"\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(DoWhileStmt(BreakStmt(\".loop@0\") Constant(ConstInt(1) None) \".loop@0\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestSimpleLoops::test_for_loop_gets_label (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestSimpleLoops::test_for_loop_gets_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None BreakStmt(\"\") \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None BreakStmt(\".loop@0\") \".loop@0\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestSimpleLoops::test_while_loop_continue_inside_body_picks_label_up (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestSimpleLoops::test_while_loop_continue_inside_body_picks_label_up\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) ContinueStmt(\"\") \"\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) ContinueStmt(\".loop@0\") \".loop@0\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestSimpleLoops::test_while_loop_gets_label_and_break_inside_picks_it_up (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestSimpleLoops::test_while_loop_gets_label_and_break_inside_picks_it_up\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\".loop@0\") \".loop@0\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestNestedLoops::test_break_after_inner_loop_targets_outer (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestNestedLoops::test_break_after_inner_loop_targets_outer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(WhileStmt(Constant(ConstInt(1) None) Null \"\"))\n"
      "      S(BreakStmt(\"\"))\n"
      "    ])) \"\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(WhileStmt(Constant(ConstInt(1) None) Null \".loop@1\"))\n"
      "      S(BreakStmt(\".loop@0\"))\n"
      "    ])) \".loop@0\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestNestedLoops::test_break_in_inner_loop_targets_inner (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestNestedLoops::test_break_in_inner_loop_targets_inner\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "    ])) \"\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\".loop@1\") \".loop@1\"))\n"
      "    ])) \".loop@0\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestNestedLoops::test_continue_in_nested_loop_targets_innermost (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestNestedLoops::test_continue_in_nested_loop_targets_innermost\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) ForStmt(InitExp(None) None None ContinueStmt(\"\") \"\" None) \"\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) ForStmt(InitExp(None) None None ContinueStmt(\".loop@1\") \".loop@1\" None) \".loop@0\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestNestedLoops::test_three_deep_nesting (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestNestedLoops::test_three_deep_nesting\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None Compound(Block([\n"
      "      S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "        S(DoWhileStmt(Compound(Block([\n"
      "          S(BreakStmt(\"\"))\n"
      "        ])) Constant(ConstInt(1) None) \"\"))\n"
      "        S(BreakStmt(\"\"))\n"
      "      ])) \"\"))\n"
      "      S(BreakStmt(\"\"))\n"
      "    ])) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None Compound(Block([\n"
      "      S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "        S(DoWhileStmt(Compound(Block([\n"
      "          S(BreakStmt(\".loop@2\"))\n"
      "        ])) Constant(ConstInt(1) None) \".loop@2\"))\n"
      "        S(BreakStmt(\".loop@1\"))\n"
      "      ])) \".loop@1\"))\n"
      "      S(BreakStmt(\".loop@0\"))\n"
      "    ])) \".loop@0\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestBreakContinueOutsideLoop::test_break_after_inner_loop_ends_at_top_level_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestBreakContinueOutsideLoop::test_break_after_inner_loop_ends_at_top_level_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Null \"\"))\n"
      "    S(BreakStmt(\"\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LoopLabelingError\", \"message\": \"'break' statement not inside a loop or switch\"}\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestBreakContinueOutsideLoop::test_break_at_top_level_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestBreakContinueOutsideLoop::test_break_at_top_level_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(BreakStmt(\"\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LoopLabelingError\", \"message\": \"'break' statement not inside a loop or switch\"}\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestBreakContinueOutsideLoop::test_break_inside_compound_outside_loop_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestBreakContinueOutsideLoop::test_break_inside_compound_outside_loop_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      S(BreakStmt(\"\"))\n"
      "    ])))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LoopLabelingError\", \"message\": \"'break' statement not inside a loop or switch\"}\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestBreakContinueOutsideLoop::test_break_inside_if_outside_loop_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestBreakContinueOutsideLoop::test_break_inside_if_outside_loop_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(IfStmt(Constant(ConstInt(1) None) BreakStmt(\"\") None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LoopLabelingError\", \"message\": \"'break' statement not inside a loop or switch\"}\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestBreakContinueOutsideLoop::test_break_inside_labeled_stmt_outside_loop_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestBreakContinueOutsideLoop::test_break_inside_labeled_stmt_outside_loop_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(LabeledStmt(\"foo\" BreakStmt(\"\")))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LoopLabelingError\", \"message\": \"'break' statement not inside a loop or switch\"}\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestBreakContinueOutsideLoop::test_continue_at_top_level_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestBreakContinueOutsideLoop::test_continue_at_top_level_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ContinueStmt(\"\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LoopLabelingError\", \"message\": \"'continue' statement not inside a loop\"}\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestBreakInsideIfBranchInsideLoop::test_break_in_if_else_inside_while (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestBreakInsideIfBranchInsideLoop::test_break_in_if_else_inside_while\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) IfStmt(Constant(ConstInt(1) None) Null ContinueStmt(\"\")) \"\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) IfStmt(Constant(ConstInt(1) None) Null ContinueStmt(\".loop@0\")) \".loop@0\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestBreakInsideIfBranchInsideLoop::test_break_in_if_then_inside_while (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestBreakInsideIfBranchInsideLoop::test_break_in_if_then_inside_while\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) IfStmt(Constant(ConstInt(1) None) BreakStmt(\"\") None) \"\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) IfStmt(Constant(ConstInt(1) None) BreakStmt(\".loop@0\") None) \".loop@0\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestBreakInsideIfBranchInsideLoop::test_break_inside_labeled_stmt_inside_loop (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestBreakInsideIfBranchInsideLoop::test_break_inside_labeled_stmt_inside_loop\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) LabeledStmt(\"foo\" BreakStmt(\"\")) \"\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) LabeledStmt(\"foo\" BreakStmt(\".loop@0\")) \".loop@0\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestPassthrough::test_function_without_loops (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestPassthrough::test_function_without_loops\n"
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

TEST_CASE("loop_labeling: test_loop_labeling.py::TestPassthrough::test_goto_and_labeled_stmt_pass_through (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestPassthrough::test_goto_and_labeled_stmt_pass_through\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Goto(\".main@end\"))\n"
      "    S(LabeledStmt(\".main@end\" Return(Constant(ConstInt(0) None))))\n"
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

TEST_CASE("loop_labeling: test_loop_labeling.py::TestPassthrough::test_if_with_no_loops_or_breaks (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestPassthrough::test_if_with_no_loops_or_breaks\n"
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

TEST_CASE("loop_labeling: test_loop_labeling.py::TestLabelProgram::test_wraps_function_in_program (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestLabelProgram::test_wraps_function_in_program\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\"\") \"\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) BreakStmt(\".loop@0\") \".loop@0\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_loop_labeling.py::TestIntegrationWithParser::test_break_in_if_outside_loop_from_source_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_loop_labeling.py::TestIntegrationWithParser::test_break_in_if_outside_loop_from_source_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { if (1) break; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(IfStmt(Constant(ConstInt(1) None) BreakStmt(\"\") None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestIntegrationWithParser::test_break_in_if_outside_loop_from_source_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestIntegrationWithParser::test_break_in_if_outside_loop_from_source_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(IfStmt(Constant(ConstInt(1) None) BreakStmt(\"\") None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LoopLabelingError\", \"message\": \"'break' statement not inside a loop or switch\"}\n");
}

TEST_CASE("parse: test_loop_labeling.py::TestIntegrationWithParser::test_break_outside_loop_from_source_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_loop_labeling.py::TestIntegrationWithParser::test_break_outside_loop_from_source_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { break; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(BreakStmt(\"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestIntegrationWithParser::test_break_outside_loop_from_source_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestIntegrationWithParser::test_break_outside_loop_from_source_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(BreakStmt(\"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LoopLabelingError\", \"message\": \"'break' statement not inside a loop or switch\"}\n");
}

TEST_CASE("parse: test_loop_labeling.py::TestIntegrationWithParser::test_continue_outside_loop_from_source_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_loop_labeling.py::TestIntegrationWithParser::test_continue_outside_loop_from_source_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { continue; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ContinueStmt(\"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestIntegrationWithParser::test_continue_outside_loop_from_source_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestIntegrationWithParser::test_continue_outside_loop_from_source_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ContinueStmt(\"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LoopLabelingError\", \"message\": \"'continue' statement not inside a loop\"}\n");
}

TEST_CASE("parse: test_loop_labeling.py::TestIntegrationWithParser::test_nested_loops_from_source (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_loop_labeling.py::TestIntegrationWithParser::test_nested_loops_from_source\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { while (1) { for (;;) { break; continue; } break; } return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(ForStmt(InitExp(None) None None Compound(Block([\n"
      "        S(BreakStmt(\"\"))\n"
      "        S(ContinueStmt(\"\"))\n"
      "      ])) \"\" None))\n"
      "      S(BreakStmt(\"\"))\n"
      "    ])) \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestIntegrationWithParser::test_nested_loops_from_source (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestIntegrationWithParser::test_nested_loops_from_source\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(ForStmt(InitExp(None) None None Compound(Block([\n"
      "        S(BreakStmt(\"\"))\n"
      "        S(ContinueStmt(\"\"))\n"
      "      ])) \"\" None))\n"
      "      S(BreakStmt(\"\"))\n"
      "    ])) \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(ForStmt(InitExp(None) None None Compound(Block([\n"
      "        S(BreakStmt(\".loop@1\"))\n"
      "        S(ContinueStmt(\".loop@1\"))\n"
      "      ])) \".loop@1\" None))\n"
      "      S(BreakStmt(\".loop@0\"))\n"
      "    ])) \".loop@0\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_loop_labeling.py::TestSwitchLabeling::test_break_in_inner_loop_in_switch_targets_loop (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_loop_labeling.py::TestSwitchLabeling::test_break_in_inner_loop_in_switch_targets_loop\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { switch (1) { case 0: for (;;) { break; } } }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) ForStmt(InitExp(None) None None Compound(Block([\n"
      "        S(BreakStmt(\"\"))\n"
      "      ])) \"\" None) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestSwitchLabeling::test_break_in_inner_loop_in_switch_targets_loop (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestSwitchLabeling::test_break_in_inner_loop_in_switch_targets_loop\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) ForStmt(InitExp(None) None None Compound(Block([\n"
      "        S(BreakStmt(\"\"))\n"
      "      ])) \"\" None) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) ForStmt(InitExp(None) None None Compound(Block([\n"
      "        S(BreakStmt(\".loop@2\"))\n"
      "      ])) \".loop@2\" None) \".case@1\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) None) \".case@1\")] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_loop_labeling.py::TestSwitchLabeling::test_break_in_switch_no_loop_compiles (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_loop_labeling.py::TestSwitchLabeling::test_break_in_switch_no_loop_compiles\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { switch (0) { case 0: break; } return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(0) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) BreakStmt(\"\") \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestSwitchLabeling::test_break_in_switch_no_loop_compiles (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestSwitchLabeling::test_break_in_switch_no_loop_compiles\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(0) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) BreakStmt(\"\") \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(0) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) BreakStmt(\".switch@0\") \".case@1\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) None) \".case@1\")] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_loop_labeling.py::TestSwitchLabeling::test_case_and_default_labels_collected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_loop_labeling.py::TestSwitchLabeling::test_case_and_default_labels_collected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { switch (3) { case 0: ; case 5: ; default: ; } }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Null \"\"))\n"
      "      S(CaseStmt(Constant(ConstInt(5) None) Null \"\"))\n"
      "      S(DefaultStmt(Null \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestSwitchLabeling::test_case_and_default_labels_collected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestSwitchLabeling::test_case_and_default_labels_collected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Null \"\"))\n"
      "      S(CaseStmt(Constant(ConstInt(5) None) Null \"\"))\n"
      "      S(DefaultStmt(Null \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Null \".case@1\"))\n"
      "      S(CaseStmt(Constant(ConstInt(5) None) Null \".case@2\"))\n"
      "      S(DefaultStmt(Null \".default@3\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) None) \".case@1\") switch_case(Constant(ConstInt(5) None) \".case@2\")] \".default@3\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_loop_labeling.py::TestSwitchLabeling::test_case_outside_switch_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_loop_labeling.py::TestSwitchLabeling::test_case_outside_switch_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { for (;;) { case 0: return 0; } return 9; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestSwitchLabeling::test_case_outside_switch_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestSwitchLabeling::test_case_outside_switch_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LoopLabelingError\", \"message\": \"'case' label not within a switch statement\"}\n");
}

TEST_CASE("parse: test_loop_labeling.py::TestSwitchLabeling::test_continue_in_switch_in_loop_targets_loop (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_loop_labeling.py::TestSwitchLabeling::test_continue_in_switch_in_loop_targets_loop\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { for (;;) switch (1) { case 0: continue; } }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) ContinueStmt(\"\") \"\"))\n"
      "    ])) \"\" [] None None) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestSwitchLabeling::test_continue_in_switch_in_loop_targets_loop (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestSwitchLabeling::test_continue_in_switch_in_loop_targets_loop\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) ContinueStmt(\"\") \"\"))\n"
      "    ])) \"\" [] None None) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None None SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) ContinueStmt(\".loop@0\") \".case@2\"))\n"
      "    ])) \".switch@1\" [switch_case(Constant(ConstInt(0) None) \".case@2\")] None None) \".loop@0\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_loop_labeling.py::TestSwitchLabeling::test_continue_in_switch_no_loop_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_loop_labeling.py::TestSwitchLabeling::test_continue_in_switch_no_loop_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { switch (0) { case 0: continue; } return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(0) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) ContinueStmt(\"\") \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestSwitchLabeling::test_continue_in_switch_no_loop_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestSwitchLabeling::test_continue_in_switch_no_loop_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(0) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) ContinueStmt(\"\") \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LoopLabelingError\", \"message\": \"'continue' statement not inside a loop\"}\n");
}

TEST_CASE("parse: test_loop_labeling.py::TestSwitchLabeling::test_default_outside_switch_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_loop_labeling.py::TestSwitchLabeling::test_default_outside_switch_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { { default: return 0; } }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestSwitchLabeling::test_default_outside_switch_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestSwitchLabeling::test_default_outside_switch_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LoopLabelingError\", \"message\": \"'default' label not within a switch statement\"}\n");
}

TEST_CASE("parse: test_loop_labeling.py::TestSwitchLabeling::test_duplicate_default_in_nested_block_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_loop_labeling.py::TestSwitchLabeling::test_duplicate_default_in_nested_block_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { switch (0) { case 0: while (1) default: ; default: ; } return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(0) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) WhileStmt(Constant(ConstInt(1) None) DefaultStmt(Null \"\") \"\") \"\"))\n"
      "      S(DefaultStmt(Null \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestSwitchLabeling::test_duplicate_default_in_nested_block_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestSwitchLabeling::test_duplicate_default_in_nested_block_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(0) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) WhileStmt(Constant(ConstInt(1) None) DefaultStmt(Null \"\") \"\") \"\"))\n"
      "      S(DefaultStmt(Null \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LoopLabelingError\", \"message\": \"multiple default labels in one switch\"}\n");
}

TEST_CASE("parse: test_loop_labeling.py::TestSwitchLabeling::test_duplicate_default_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_loop_labeling.py::TestSwitchLabeling::test_duplicate_default_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { switch (0) { default: ; default: ; } }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(0) None) Compound(Block([\n"
      "      S(DefaultStmt(Null \"\"))\n"
      "      S(DefaultStmt(Null \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestSwitchLabeling::test_duplicate_default_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestSwitchLabeling::test_duplicate_default_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(0) None) Compound(Block([\n"
      "      S(DefaultStmt(Null \"\"))\n"
      "      S(DefaultStmt(Null \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"LoopLabelingError\", \"message\": \"multiple default labels in one switch\"}\n");
}

TEST_CASE("parse: test_loop_labeling.py::TestSwitchLabeling::test_nested_switch_owns_its_own_cases (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_loop_labeling.py::TestSwitchLabeling::test_nested_switch_owns_its_own_cases\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { switch (1) { case 0: switch (2) { case 1: ; default: ; } case 9: ; } }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) SwitchStmt(Constant(ConstInt(2) None) Compound(Block([\n"
      "        S(CaseStmt(Constant(ConstInt(1) None) Null \"\"))\n"
      "        S(DefaultStmt(Null \"\"))\n"
      "      ])) \"\" [] None None) \"\"))\n"
      "      S(CaseStmt(Constant(ConstInt(9) None) Null \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestSwitchLabeling::test_nested_switch_owns_its_own_cases (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestSwitchLabeling::test_nested_switch_owns_its_own_cases\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) SwitchStmt(Constant(ConstInt(2) None) Compound(Block([\n"
      "        S(CaseStmt(Constant(ConstInt(1) None) Null \"\"))\n"
      "        S(DefaultStmt(Null \"\"))\n"
      "      ])) \"\" [] None None) \"\"))\n"
      "      S(CaseStmt(Constant(ConstInt(9) None) Null \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) SwitchStmt(Constant(ConstInt(2) None) Compound(Block([\n"
      "        S(CaseStmt(Constant(ConstInt(1) None) Null \".case@3\"))\n"
      "        S(DefaultStmt(Null \".default@4\"))\n"
      "      ])) \".switch@2\" [switch_case(Constant(ConstInt(1) None) \".case@3\")] \".default@4\" None) \".case@1\"))\n"
      "      S(CaseStmt(Constant(ConstInt(9) None) Null \".case@5\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) None) \".case@1\") switch_case(Constant(ConstInt(9) None) \".case@5\")] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_loop_labeling.py::TestSwitchLabeling::test_switch_break_targets_switch_label (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_loop_labeling.py::TestSwitchLabeling::test_switch_break_targets_switch_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { switch (1) { case 0: break; default: ; } }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) BreakStmt(\"\") \"\"))\n"
      "      S(DefaultStmt(Null \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_loop_labeling.py::TestSwitchLabeling::test_switch_break_targets_switch_label (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_loop_labeling.py::TestSwitchLabeling::test_switch_break_targets_switch_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) BreakStmt(\"\") \"\"))\n"
      "      S(DefaultStmt(Null \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) BreakStmt(\".switch@0\") \".case@1\"))\n"
      "      S(DefaultStmt(Null \".default@2\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) None) \".case@1\")] \".default@2\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}
