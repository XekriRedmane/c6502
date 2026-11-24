// The calls that the tests of tests/test_unroll.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"
#include "preprocessor_case.h"

TEST_CASE("parse: test_unroll.py::TestUnrollAttributeParse::test_attaches_only_to_its_own_loop (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollAttributeParse::test_attaches_only_to_its_own_loop\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n        int main(void) {\\n            [[c6502::unroll]]\\n            for (int i = 0; i < 4; i++) { }\\n            for (int j = 0; j < 4; j++) { }\\n            return 0;\\n        }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(ForStmt(InitDecl([var_decl(\"j\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"j\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"j\" None) None) Compound(Block([])) \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollAttributeParse::test_before_another_statement_is_an_error (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollAttributeParse::test_before_another_statement_is_an_error\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n        int main(void) {\\n            [[c6502::unroll]] return 0;\\n        }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::unroll` goes before a `for` statement; saw it on a `return` statement\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollAttributeParse::test_no_attribute_leaves_it_none (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollAttributeParse::test_no_attribute_leaves_it_none\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\nint main(void) {\\n    for (int i = 0; i < 4; i++) { }\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([])) \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("preprocess: test_unroll.py::TestUnrollAttributeParse::test_old_pragma_names_the_attribute (call 1)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": null, \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": [\"PreprocessorError\", \"line 3: c6502 no longer takes `#pragma c6502 loop unroll(enable)`; write the statement attribute `[[c6502::unroll]]` before the loop instead, as in `[[c6502::unroll]] for (...)`\"], \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": null, \"source\": \"\\nint main(void) {\\n    #pragma c6502 loop unroll(enable)\\nfor (int i = 0; i < 4; i++) { }\\n    return 0;\\n}\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_unroll.py::TestUnrollAttributeParse::test_old_pragma_names_the_attribute (call 2)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": null, \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": [\"PreprocessorError\", \"line 3: c6502 no longer takes `#pragma c6502 loop unroll(disable)`; it never did anything, so delete the line\"], \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": null, \"source\": \"\\nint main(void) {\\n    #pragma c6502 loop unroll(disable)\\nfor (int i = 0; i < 4; i++) { }\\n    return 0;\\n}\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("parse: test_unroll.py::TestUnrollAttributeParse::test_sets_loop_attribute (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollAttributeParse::test_sets_loop_attribute\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\nint main(void) {\\n    [[c6502::unroll]] for (int i = 0; i < 4; i++) { }\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollAttributePipelinePassthrough::test_identifier_resolution_preserves_annotation (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollAttributePipelinePassthrough::test_identifier_resolution_preserves_annotation\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\nint main(void) {\\n    [[c6502::unroll]] for (int i = 0; i < 4; i++) { }\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_unroll.py::TestUnrollAttributePipelinePassthrough::test_identifier_resolution_preserves_annotation (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_unroll.py::TestUnrollAttributePipelinePassthrough::test_identifier_resolution_preserves_annotation\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_unroll.py::TestUnrollAttributePipelinePassthrough::test_identifier_resolution_preserves_annotation (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_unroll.py::TestUnrollAttributePipelinePassthrough::test_identifier_resolution_preserves_annotation\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"@0.i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"@0.i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"@0.i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollAttributePipelinePassthrough::test_label_resolution_preserves_annotation (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollAttributePipelinePassthrough::test_label_resolution_preserves_annotation\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\nint main(void) {\\n    [[c6502::unroll]] for (int i = 0; i < 4; i++) { }\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_unroll.py::TestUnrollAttributePipelinePassthrough::test_label_resolution_preserves_annotation (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_unroll.py::TestUnrollAttributePipelinePassthrough::test_label_resolution_preserves_annotation\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_unroll.py::TestUnrollAttributePipelinePassthrough::test_label_resolution_preserves_annotation (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_unroll.py::TestUnrollAttributePipelinePassthrough::test_label_resolution_preserves_annotation\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"@0.i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"@0.i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"@0.i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_unroll.py::TestUnrollAttributePipelinePassthrough::test_label_resolution_preserves_annotation (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_unroll.py::TestUnrollAttributePipelinePassthrough::test_label_resolution_preserves_annotation\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"@0.i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"@0.i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"@0.i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"@0.i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"@0.i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"@0.i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollAttributePipelinePassthrough::test_loop_labeling_preserves_annotation (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollAttributePipelinePassthrough::test_loop_labeling_preserves_annotation\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\nint main(void) {\\n    [[c6502::unroll]] for (int i = 0; i < 4; i++) { }\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_unroll.py::TestUnrollAttributePipelinePassthrough::test_loop_labeling_preserves_annotation (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_unroll.py::TestUnrollAttributePipelinePassthrough::test_loop_labeling_preserves_annotation\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_unroll.py::TestUnrollAttributePipelinePassthrough::test_loop_labeling_preserves_annotation (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_unroll.py::TestUnrollAttributePipelinePassthrough::test_loop_labeling_preserves_annotation\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"@0.i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"@0.i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"@0.i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_unroll.py::TestUnrollAttributePipelinePassthrough::test_loop_labeling_preserves_annotation (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_unroll.py::TestUnrollAttributePipelinePassthrough::test_loop_labeling_preserves_annotation\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"@0.i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"@0.i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"@0.i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"@0.i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"@0.i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"@0.i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_unroll.py::TestUnrollAttributePipelinePassthrough::test_loop_labeling_preserves_annotation (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_unroll.py::TestUnrollAttributePipelinePassthrough::test_loop_labeling_preserves_annotation\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"@0.i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"@0.i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"@0.i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"@0.i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"@0.i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"@0.i\" None) None) Compound(Block([])) \".loop@0\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollAttributePipelinePassthrough::test_string_lifting_preserves_annotation (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollAttributePipelinePassthrough::test_string_lifting_preserves_annotation\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\nint main(void) {\\n    [[c6502::unroll]] for (int i = 0; i < 4; i++) { }\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_unroll.py::TestUnrollAttributePipelinePassthrough::test_string_lifting_preserves_annotation (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_unroll.py::TestUnrollAttributePipelinePassthrough::test_string_lifting_preserves_annotation\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollAscending::test_compound_assign_step (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollAscending::test_compound_assign_step\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n        int s = 0;\\n        [[c6502::unroll]] for (int i = 0; i < 10; i += 2) s += i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(10) None) None) CompoundAssignment(Add Var(\"i\" None) Constant(ConstInt(2) None) None None) Expression(CompoundAssignment(Add Var(\"s\" None) Var(\"i\" None) None None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollAscending::test_compound_assign_step (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollAscending::test_compound_assign_step\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(10) None) None) CompoundAssignment(Add Var(\"i\" None) Constant(ConstInt(2) None) None None) Expression(CompoundAssignment(Add Var(\"s\" None) Var(\"i\" None) None None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(0) None) None None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(2) None) None None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(4) None) None None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(6) None) None None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(8) None) None None)))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollAscending::test_lt_eq_includes_endpoint (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollAscending::test_lt_eq_includes_endpoint\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n        [[c6502::unroll]] for (int i = 0; i <= 3; i++) i + 1;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessOrEqual Var(\"i\" None) Constant(ConstInt(3) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Binary(Add Var(\"i\" None) Constant(ConstInt(1) None) None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollAscending::test_lt_eq_includes_endpoint (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollAscending::test_lt_eq_includes_endpoint\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessOrEqual Var(\"i\" None) Constant(ConstInt(3) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Binary(Add Var(\"i\" None) Constant(ConstInt(1) None) None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Binary(Add Constant(ConstInt(0) None) Constant(ConstInt(1) None) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Binary(Add Constant(ConstInt(1) None) Constant(ConstInt(1) None) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Binary(Add Constant(ConstInt(2) None) Constant(ConstInt(1) None) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Binary(Add Constant(ConstInt(3) None) Constant(ConstInt(1) None) None)))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollAscending::test_prefix_increment (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollAscending::test_prefix_increment\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n        [[c6502::unroll]] for (int i = 0; i < 3; ++i) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(3) None) None) Prefix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollAscending::test_prefix_increment (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollAscending::test_prefix_increment\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(3) None) None) Prefix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(0) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(1) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(2) None)))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollAscending::test_simple_lt_increment (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollAscending::test_simple_lt_increment\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n        int s = 0;\\n        [[c6502::unroll]] for (int i = 0; i < 4; i++) s += i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Expression(CompoundAssignment(Add Var(\"s\" None) Var(\"i\" None) None None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollAscending::test_simple_lt_increment (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollAscending::test_simple_lt_increment\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Expression(CompoundAssignment(Add Var(\"s\" None) Var(\"i\" None) None None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(0) None) None None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(1) None) None None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(2) None) None None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(3) None) None None)))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollDescending::test_ge_decrement_to_zero (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollDescending::test_ge_decrement_to_zero\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n        [[c6502::unroll]] for (int i = 3; i >= 0; --i) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(3) None) Int None None None None false)]) Binary(GreaterOrEqual Var(\"i\" None) Constant(ConstInt(0) None) None) Prefix(Decrement Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollDescending::test_ge_decrement_to_zero (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollDescending::test_ge_decrement_to_zero\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(3) None) Int None None None None false)]) Binary(GreaterOrEqual Var(\"i\" None) Constant(ConstInt(0) None) None) Prefix(Decrement Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(3) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(2) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(1) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(0) None)))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollDescending::test_gt_decrement (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollDescending::test_gt_decrement\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n        int s = 0;\\n        [[c6502::unroll]] for (int i = 5; i > 0; i--) s += i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(5) None) Int None None None None false)]) Binary(GreaterThan Var(\"i\" None) Constant(ConstInt(0) None) None) Postfix(Decrement Var(\"i\" None) None) Expression(CompoundAssignment(Add Var(\"s\" None) Var(\"i\" None) None None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollDescending::test_gt_decrement (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollDescending::test_gt_decrement\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(5) None) Int None None None None false)]) Binary(GreaterThan Var(\"i\" None) Constant(ConstInt(0) None) None) Postfix(Decrement Var(\"i\" None) None) Expression(CompoundAssignment(Add Var(\"s\" None) Var(\"i\" None) None None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(5) None) None None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(4) None) None None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(3) None) None None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(2) None) None None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(1) None) None None)))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollDescending::test_minus_eq_step (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollDescending::test_minus_eq_step\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n        [[c6502::unroll]] for (int i = 10; i > 0; i -= 2) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(10) None) Int None None None None false)]) Binary(GreaterThan Var(\"i\" None) Constant(ConstInt(0) None) None) CompoundAssignment(Subtract Var(\"i\" None) Constant(ConstInt(2) None) None None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollDescending::test_minus_eq_step (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollDescending::test_minus_eq_step\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(10) None) Int None None None None false)]) Binary(GreaterThan Var(\"i\" None) Constant(ConstInt(0) None) None) CompoundAssignment(Subtract Var(\"i\" None) Constant(ConstInt(2) None) None None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(10) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(8) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(6) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(4) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(2) None)))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollIvTypes::test_char_iv (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollIvTypes::test_char_iv\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n        [[c6502::unroll]] for (char i = 0; i < 3; i++) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Char None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(3) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollIvTypes::test_char_iv (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollIvTypes::test_char_iv\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Char None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(3) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstUChar(0) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstUChar(1) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstUChar(2) None)))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollIvTypes::test_long_iv (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollIvTypes::test_long_iv\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n        [[c6502::unroll]] for (long i = 0; i < 3; i++) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Long None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(3) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollIvTypes::test_long_iv (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollIvTypes::test_long_iv\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Long None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(3) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstLong(0) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstLong(1) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstLong(2) None)))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollIvTypes::test_signed_char_iv (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollIvTypes::test_signed_char_iv\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n        [[c6502::unroll]] for (signed char i = 0; i < 3; i++) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) SChar None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(3) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollIvTypes::test_signed_char_iv (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollIvTypes::test_signed_char_iv\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) SChar None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(3) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstChar(0) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstChar(1) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstChar(2) None)))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollIvTypes::test_unsigned_char_iv (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollIvTypes::test_unsigned_char_iv\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n        [[c6502::unroll]] for (unsigned char i = 0; i < 3; i++) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) UChar None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(3) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollIvTypes::test_unsigned_char_iv (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollIvTypes::test_unsigned_char_iv\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) UChar None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(3) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstUChar(0) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstUChar(1) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstUChar(2) None)))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollIvTypes::test_unsigned_int_iv (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollIvTypes::test_unsigned_int_iv\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n        [[c6502::unroll]] for (unsigned int i = 0; i < 2; i++) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) UInt None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(2) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollIvTypes::test_unsigned_int_iv (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollIvTypes::test_unsigned_int_iv\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) UInt None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(2) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstUInt(0) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstUInt(1) None)))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollEdgeCases::test_body_locals_get_per_iteration_scope (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollEdgeCases::test_body_locals_get_per_iteration_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n            int main(void) {\\n                int s = 0;\\n                [[c6502::unroll]] for (int i = 0; i < 3; i++) {\\n                    int t = i + 1;\\n                    s += t;\\n                }\\n                return s;\\n            }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(3) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      D(VarDecl(var_decl(\"t\" Binary(Add Var(\"i\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "      S(Expression(CompoundAssignment(Add Var(\"s\" None) Var(\"t\" None) None None)))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Var(\"s\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollEdgeCases::test_body_locals_get_per_iteration_scope (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollEdgeCases::test_body_locals_get_per_iteration_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(3) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      D(VarDecl(var_decl(\"t\" Binary(Add Var(\"i\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "      S(Expression(CompoundAssignment(Add Var(\"s\" None) Var(\"t\" None) None None)))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Var(\"s\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        D(VarDecl(var_decl(\"t\" Binary(Add Constant(ConstInt(0) None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "        S(Expression(CompoundAssignment(Add Var(\"s\" None) Var(\"t\" None) None None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        D(VarDecl(var_decl(\"t\" Binary(Add Constant(ConstInt(1) None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "        S(Expression(CompoundAssignment(Add Var(\"s\" None) Var(\"t\" None) None None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        D(VarDecl(var_decl(\"t\" Binary(Add Constant(ConstInt(2) None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "        S(Expression(CompoundAssignment(Add Var(\"s\" None) Var(\"t\" None) None None)))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(Return(Var(\"s\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_unroll.py::TestUnrollEdgeCases::test_body_locals_get_per_iteration_scope (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_unroll.py::TestUnrollEdgeCases::test_body_locals_get_per_iteration_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        D(VarDecl(var_decl(\"t\" Binary(Add Constant(ConstInt(0) None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "        S(Expression(CompoundAssignment(Add Var(\"s\" None) Var(\"t\" None) None None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        D(VarDecl(var_decl(\"t\" Binary(Add Constant(ConstInt(1) None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "        S(Expression(CompoundAssignment(Add Var(\"s\" None) Var(\"t\" None) None None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        D(VarDecl(var_decl(\"t\" Binary(Add Constant(ConstInt(2) None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "        S(Expression(CompoundAssignment(Add Var(\"s\" None) Var(\"t\" None) None None)))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(Return(Var(\"s\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.s\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        D(VarDecl(var_decl(\"@1.t\" Binary(Add Constant(ConstInt(0) None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "        S(Expression(CompoundAssignment(Add Var(\"@0.s\" None) Var(\"@1.t\" None) None None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        D(VarDecl(var_decl(\"@2.t\" Binary(Add Constant(ConstInt(1) None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "        S(Expression(CompoundAssignment(Add Var(\"@0.s\" None) Var(\"@2.t\" None) None None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        D(VarDecl(var_decl(\"@3.t\" Binary(Add Constant(ConstInt(2) None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "        S(Expression(CompoundAssignment(Add Var(\"@0.s\" None) Var(\"@3.t\" None) None None)))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(Return(Var(\"@0.s\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollEdgeCases::test_mixed_annotated_unannotated (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollEdgeCases::test_mixed_annotated_unannotated\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n        [[c6502::unroll]] for (int i = 0; i < 2; i++) i;\\n        for (int j = 0; j < 2; j++) j;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(2) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(ForStmt(InitDecl([var_decl(\"j\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"j\" None) Constant(ConstInt(2) None) None) Postfix(Increment Var(\"j\" None) None) Expression(Var(\"j\" None)) \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollEdgeCases::test_mixed_annotated_unannotated (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollEdgeCases::test_mixed_annotated_unannotated\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(2) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(ForStmt(InitDecl([var_decl(\"j\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"j\" None) Constant(ConstInt(2) None) None) Postfix(Increment Var(\"j\" None) None) Expression(Var(\"j\" None)) \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(0) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(1) None)))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(ForStmt(InitDecl([var_decl(\"j\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"j\" None) Constant(ConstInt(2) None) None) Postfix(Increment Var(\"j\" None) None) Expression(Var(\"j\" None)) \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollEdgeCases::test_nested_annotated_loops (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollEdgeCases::test_nested_annotated_loops\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n        int s = 0;\\n        [[c6502::unroll]] for (int i = 0; i < 4; i++) {\\n            [[c6502::unroll]] for (int j = 0; j < 3; j++) {\\n                s += 1;\\n            }\\n        }\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      S(ForStmt(InitDecl([var_decl(\"j\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"j\" None) Constant(ConstInt(3) None) None) Postfix(Increment Var(\"j\" None) None) Compound(Block([\n"
      "        S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(1) None) None None)))\n"
      "      ])) \"\" \"unroll\"))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollEdgeCases::test_nested_annotated_loops (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollEdgeCases::test_nested_annotated_loops\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      S(ForStmt(InitDecl([var_decl(\"j\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"j\" None) Constant(ConstInt(3) None) None) Postfix(Increment Var(\"j\" None) None) Compound(Block([\n"
      "        S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(1) None) None None)))\n"
      "      ])) \"\" \"unroll\"))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        S(Compound(Block([\n"
      "          S(Compound(Block([\n"
      "            S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(1) None) None None)))\n"
      "          ])))\n"
      "          S(Compound(Block([\n"
      "            S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(1) None) None None)))\n"
      "          ])))\n"
      "          S(Compound(Block([\n"
      "            S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(1) None) None None)))\n"
      "          ])))\n"
      "        ])))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Compound(Block([\n"
      "          S(Compound(Block([\n"
      "            S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(1) None) None None)))\n"
      "          ])))\n"
      "          S(Compound(Block([\n"
      "            S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(1) None) None None)))\n"
      "          ])))\n"
      "          S(Compound(Block([\n"
      "            S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(1) None) None None)))\n"
      "          ])))\n"
      "        ])))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Compound(Block([\n"
      "          S(Compound(Block([\n"
      "            S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(1) None) None None)))\n"
      "          ])))\n"
      "          S(Compound(Block([\n"
      "            S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(1) None) None None)))\n"
      "          ])))\n"
      "          S(Compound(Block([\n"
      "            S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(1) None) None None)))\n"
      "          ])))\n"
      "        ])))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Compound(Block([\n"
      "          S(Compound(Block([\n"
      "            S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(1) None) None None)))\n"
      "          ])))\n"
      "          S(Compound(Block([\n"
      "            S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(1) None) None None)))\n"
      "          ])))\n"
      "          S(Compound(Block([\n"
      "            S(Expression(CompoundAssignment(Add Var(\"s\" None) Constant(ConstInt(1) None) None None)))\n"
      "          ])))\n"
      "        ])))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollEdgeCases::test_unannotated_loop_left_alone (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollEdgeCases::test_unannotated_loop_left_alone\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n        for (int i = 0; i < 4; i++) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollEdgeCases::test_unannotated_loop_left_alone (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollEdgeCases::test_unannotated_loop_left_alone\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollEdgeCases::test_zero_iterations (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollEdgeCases::test_zero_iterations\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n        [[c6502::unroll]] for (int i = 5; i < 0; i++) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(5) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(0) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollEdgeCases::test_zero_iterations (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollEdgeCases::test_zero_iterations\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(5) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(0) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([])))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_break_in_body_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_break_in_body_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            [[c6502::unroll]] for (int i = 0; i < 4; i++) { if (i == 2) break; }\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      S(IfStmt(Binary(Equal Var(\"i\" None) Constant(ConstInt(2) None) None) BreakStmt(\"\") None))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_break_in_body_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_break_in_body_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      S(IfStmt(Binary(Equal Var(\"i\" None) Constant(ConstInt(2) None) None) BreakStmt(\"\") None))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: `break` inside an unrolled loop body is not supported\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_condition_not_against_iv_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_condition_not_against_iv_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            int x;\\n            [[c6502::unroll]] for (int i = 0; x < 4; i++) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" None Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"x\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_condition_not_against_iv_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_condition_not_against_iv_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" None Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"x\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: condition's left operand must be the induction variable `i`\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_continue_in_body_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_continue_in_body_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            [[c6502::unroll]] for (int i = 0; i < 4; i++) { if (i == 2) continue; }\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      S(IfStmt(Binary(Equal Var(\"i\" None) Constant(ConstInt(2) None) None) ContinueStmt(\"\") None))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_continue_in_body_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_continue_in_body_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      S(IfStmt(Binary(Equal Var(\"i\" None) Constant(ConstInt(2) None) None) ContinueStmt(\"\") None))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: `continue` inside an unrolled loop body is not supported\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_equality_op_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_equality_op_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            [[c6502::unroll]] for (int i = 0; i == 0; i++) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(Equal Var(\"i\" None) Constant(ConstInt(0) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_equality_op_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_equality_op_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(Equal Var(\"i\" None) Constant(ConstInt(0) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: comparison op Equal not supported (use <, <=, >, or >=)\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_goto_in_body_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_goto_in_body_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            [[c6502::unroll]] for (int i = 0; i < 4; i++) { goto out; }\\n        out: ;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      S(Goto(\"out\"))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(LabeledStmt(\"out\" Null))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_goto_in_body_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_goto_in_body_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      S(Goto(\"out\"))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(LabeledStmt(\"out\" Null))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: `goto` inside an unrolled loop body is not supported\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_init_must_declare (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_init_must_declare\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            int i;\\n            [[c6502::unroll]] for (i = 0; i < 4; i++) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"i\" None Int None None None None false)))\n"
      "    S(ForStmt(InitExp(Assignment(Var(\"i\" None) Constant(ConstInt(0) None) None)) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_init_must_declare (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_init_must_declare\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"i\" None Int None None None None false)))\n"
      "    S(ForStmt(InitExp(Assignment(Var(\"i\" None) Constant(ConstInt(0) None) None)) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: for-init must declare the induction variable (`for (T i = <const>; ...; ...)`)\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_inner_for_reusing_iv_name_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_inner_for_reusing_iv_name_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            [[c6502::unroll]] for (int i = 0; i < 4; i++) {\\n                for (int i = 0; i < 2; i++) i;\\n            }\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(2) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" None))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_inner_for_reusing_iv_name_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_inner_for_reusing_iv_name_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(2) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" None))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: induction variable `i` is shadowed by an inner for-loop init\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_iteration_cap_exceeded_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_iteration_cap_exceeded_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            [[c6502::unroll]] for (int i = 0; i < 257; i++) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(257) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_iteration_cap_exceeded_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_iteration_cap_exceeded_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(257) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: iteration count exceeds cap of 256\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_iv_address_taken_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_iv_address_taken_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            int *p;\\n            [[c6502::unroll]] for (int i = 0; i < 4; i++) { p = &i; }\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      S(Expression(Assignment(Var(\"p\" None) AddressOf(Var(\"i\" None) None) None)))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_iv_address_taken_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_iv_address_taken_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      S(Expression(Assignment(Var(\"p\" None) AddressOf(Var(\"i\" None) None) None)))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: address of induction variable `i` is taken in body\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_iv_assignment_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_iv_assignment_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            [[c6502::unroll]] for (int i = 0; i < 4; i++) { i = 5; }\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      S(Expression(Assignment(Var(\"i\" None) Constant(ConstInt(5) None) None)))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_iv_assignment_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_iv_assignment_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      S(Expression(Assignment(Var(\"i\" None) Constant(ConstInt(5) None) None)))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: induction variable `i` is reassigned in body\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_iv_compound_assign_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_iv_compound_assign_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            [[c6502::unroll]] for (int i = 0; i < 4; i++) { i += 5; }\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      S(Expression(CompoundAssignment(Add Var(\"i\" None) Constant(ConstInt(5) None) None None)))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_iv_compound_assign_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_iv_compound_assign_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      S(Expression(CompoundAssignment(Add Var(\"i\" None) Constant(ConstInt(5) None) None None)))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: induction variable `i` is modified in body\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_iv_inner_increment_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_iv_inner_increment_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            [[c6502::unroll]] for (int i = 0; i < 4; i++) { i++; }\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      S(Expression(Postfix(Increment Var(\"i\" None) None)))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_iv_inner_increment_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_iv_inner_increment_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      S(Expression(Postfix(Increment Var(\"i\" None) None)))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: induction variable `i` is modified in body\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_iv_inner_shadow_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_iv_inner_shadow_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            [[c6502::unroll]] for (int i = 0; i < 4; i++) { int i = 99; }\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      D(VarDecl(var_decl(\"i\" Constant(ConstInt(99) None) Int None None None None false)))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_iv_inner_shadow_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_iv_inner_shadow_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      D(VarDecl(var_decl(\"i\" Constant(ConstInt(99) None) Int None None None None false)))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: induction variable `i` is shadowed by an inner declaration\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_labeled_stmt_in_body_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_labeled_stmt_in_body_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            [[c6502::unroll]] for (int i = 0; i < 4; i++) { lbl: i; }\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      S(LabeledStmt(\"lbl\" Expression(Var(\"i\" None))))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_labeled_stmt_in_body_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_labeled_stmt_in_body_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      S(LabeledStmt(\"lbl\" Expression(Var(\"i\" None))))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: labeled statements inside an unrolled loop body are not supported\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_missing_condition_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_missing_condition_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            [[c6502::unroll]] for (int i = 0; ; i++) { if (i > 4) break; }\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) None Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      S(IfStmt(Binary(GreaterThan Var(\"i\" None) Constant(ConstInt(4) None) None) BreakStmt(\"\") None))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_missing_condition_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_missing_condition_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) None Postfix(Increment Var(\"i\" None) None) Compound(Block([\n"
      "      S(IfStmt(Binary(GreaterThan Var(\"i\" None) Constant(ConstInt(4) None) None) BreakStmt(\"\") None))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: for-loop condition is required\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_missing_post_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_missing_post_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            [[c6502::unroll]] for (int i = 0; i < 4; ) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) None Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_missing_post_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_missing_post_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) None Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: for-loop post-clause is required\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_non_constant_bound_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_non_constant_bound_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            int x = 4;\\n            [[c6502::unroll]] for (int i = 0; i < x; i++) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(4) None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Var(\"x\" None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_non_constant_bound_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_non_constant_bound_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(4) None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Var(\"x\" None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: condition's right operand must be an integer constant\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_non_constant_init_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_non_constant_init_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            int x = 0;\\n            [[c6502::unroll]] for (int i = x; i < 4; i++) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Var(\"x\" None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_non_constant_init_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_non_constant_init_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Var(\"x\" None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: induction variable must be initialized to an integer constant\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_non_constant_step_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_non_constant_step_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            int x = 1;\\n            [[c6502::unroll]] for (int i = 0; i < 4; i += x) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) CompoundAssignment(Add Var(\"i\" None) Var(\"x\" None) None None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_non_constant_step_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_non_constant_step_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) CompoundAssignment(Add Var(\"i\" None) Var(\"x\" None) None None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: post-clause step must be a positive integer constant\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_not_equal_op_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_not_equal_op_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            [[c6502::unroll]] for (int i = 0; i != 4; i++) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(NotEqual Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_not_equal_op_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_not_equal_op_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(NotEqual Var(\"i\" None) Constant(ConstInt(4) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: comparison op NotEqual not supported (use <, <=, >, or >=)\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_schar_lt_200_never_terminates (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_schar_lt_200_never_terminates\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            [[c6502::unroll]] for (signed char i = 0; i < 200; i++) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) SChar None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(200) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_schar_lt_200_never_terminates (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_schar_lt_200_never_terminates\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) SChar None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(200) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: loop never terminates (`i` wraps at the width of `signed char` and the condition stays true)\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_short_cycle_never_terminates (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_short_cycle_never_terminates\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            [[c6502::unroll]] for (short i = 0; i <= 32767; i += 256) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Short None None None None false)]) Binary(LessOrEqual Var(\"i\" None) Constant(ConstInt(32767) None) None) CompoundAssignment(Add Var(\"i\" None) Constant(ConstInt(256) None) None None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_short_cycle_never_terminates (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_short_cycle_never_terminates\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Short None None None None false)]) Binary(LessOrEqual Var(\"i\" None) Constant(ConstInt(32767) None) None) CompoundAssignment(Add Var(\"i\" None) Constant(ConstInt(256) None) None None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: loop never terminates (`i` wraps at the width of `short` and the condition stays true)\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_swapped_condition_form_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_swapped_condition_form_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            [[c6502::unroll]] for (int i = 0; 4 > i; i++) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(GreaterThan Constant(ConstInt(4) None) Var(\"i\" None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_swapped_condition_form_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_swapped_condition_form_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(GreaterThan Constant(ConstInt(4) None) Var(\"i\" None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: condition's left operand must be the induction variable `i`\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_uchar_le_255_never_terminates (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_uchar_le_255_never_terminates\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            [[c6502::unroll]] for (unsigned char i = 0; i <= 255; i++) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) UChar None None None None false)]) Binary(LessOrEqual Var(\"i\" None) Constant(ConstInt(255) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_uchar_le_255_never_terminates (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_uchar_le_255_never_terminates\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) UChar None None None None false)]) Binary(LessOrEqual Var(\"i\" None) Constant(ConstInt(255) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: loop never terminates (`i` wraps at the width of `unsigned char` and the condition stays true)\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_ushort_cycle_never_terminates (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_ushort_cycle_never_terminates\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            [[c6502::unroll]] for (unsigned short i = 0; i < 65535u; i += 256) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) UShort None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstUInt(65535) None) None) CompoundAssignment(Add Var(\"i\" None) Constant(ConstInt(256) None) None None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_ushort_cycle_never_terminates (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_ushort_cycle_never_terminates\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) UShort None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstUInt(65535) None) None) CompoundAssignment(Add Var(\"i\" None) Constant(ConstInt(256) None) None None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: loop never terminates (`i` wraps at the width of `unsigned short` and the condition stays true)\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollRecognizerRejections::test_zero_step_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_zero_step_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n            [[c6502::unroll]] for (int i = 0; i < 4; i += 0) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) CompoundAssignment(Add Var(\"i\" None) Constant(ConstInt(0) None) None None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollRecognizerRejections::test_zero_step_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollRecognizerRejections::test_zero_step_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(4) None) None) CompoundAssignment(Add Var(\"i\" None) Constant(ConstInt(0) None) None None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: post-clause step must be a positive integer constant\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollTripCountWidth::test_cast_bound_is_converted (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollTripCountWidth::test_cast_bound_is_converted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n        [[c6502::unroll]] for (int i = 0; i < (unsigned char)260; i++) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Cast(UChar Constant(ConstInt(260) None) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollTripCountWidth::test_cast_bound_is_converted (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollTripCountWidth::test_cast_bound_is_converted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Cast(UChar Constant(ConstInt(260) None) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(0) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(1) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(2) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(3) None)))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollTripCountWidth::test_step_wraps_past_bound (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollTripCountWidth::test_step_wraps_past_bound\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n        [[c6502::unroll]] for (unsigned char i = 250; i > 3; i += 4) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(250) None) UChar None None None None false)]) Binary(GreaterThan Var(\"i\" None) Constant(ConstInt(3) None) None) CompoundAssignment(Add Var(\"i\" None) Constant(ConstInt(4) None) None None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollTripCountWidth::test_step_wraps_past_bound (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollTripCountWidth::test_step_wraps_past_bound\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(250) None) UChar None None None None false)]) Binary(GreaterThan Var(\"i\" None) Constant(ConstInt(3) None) None) CompoundAssignment(Add Var(\"i\" None) Constant(ConstInt(4) None) None None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstUChar(250) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstUChar(254) None)))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollTripCountWidth::test_unsigned_bound_compares_unsigned (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollTripCountWidth::test_unsigned_bound_compares_unsigned\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n        [[c6502::unroll]] for (int i = (int)65533u; i > 65530u; i++) i;\\n\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Cast(Int Constant(ConstUInt(65533) None) None) Int None None None None false)]) Binary(GreaterThan Var(\"i\" None) Constant(ConstUInt(65530) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollTripCountWidth::test_unsigned_bound_compares_unsigned (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollTripCountWidth::test_unsigned_bound_compares_unsigned\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Cast(Int Constant(ConstUInt(65533) None) None) Int None None None None false)]) Binary(GreaterThan Var(\"i\" None) Constant(ConstUInt(65530) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(-3) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(-2) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(-1) None)))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollConstArraySubscriptBound::test_1d_const_array_bound (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollConstArraySubscriptBound::test_1d_const_array_bound\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n        static const int BOUNDS[3] = {3, 4, 5};\\n        int main(void) {\\n            [[c6502::unroll]] for (int i = 0; i < BOUNDS[2]; i++) i;\\n            return 0;\\n        }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"BOUNDS\" InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None) Constant(ConstInt(5) None)] None None) Array(Const(Int) 3) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Subscript(Var(\"BOUNDS\" None) Constant(ConstInt(2) None) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollConstArraySubscriptBound::test_1d_const_array_bound (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollConstArraySubscriptBound::test_1d_const_array_bound\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"BOUNDS\" InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None) Constant(ConstInt(5) None)] None None) Array(Const(Int) 3) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Subscript(Var(\"BOUNDS\" None) Constant(ConstInt(2) None) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"BOUNDS\" InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None) Constant(ConstInt(5) None)] None None) Array(Const(Int) 3) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(0) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(1) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(2) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(3) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(4) None)))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollConstArraySubscriptBound::test_2d_const_array_bound (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollConstArraySubscriptBound::test_2d_const_array_bound\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n        static const int M[2][3] = { {1, 2, 3}, {4, 5, 6} };\\n        int main(void) {\\n            [[c6502::unroll]] for (int i = 0; i < M[1][2]; i++) i;\\n            return 0;\\n        }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"M\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) InitList([Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Const(Int) 3) 2) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Subscript(Subscript(Var(\"M\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(2) None) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollConstArraySubscriptBound::test_2d_const_array_bound (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollConstArraySubscriptBound::test_2d_const_array_bound\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"M\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) InitList([Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Const(Int) 3) 2) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Subscript(Subscript(Var(\"M\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(2) None) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"M\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) InitList([Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Const(Int) 3) 2) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(0) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(1) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(2) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(3) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(4) None)))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Expression(Constant(ConstInt(5) None)))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollConstArraySubscriptBound::test_const_array_zero_pads_missing_inner (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollConstArraySubscriptBound::test_const_array_zero_pads_missing_inner\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n        static const int M[2][3] = { {1, 2, 3}, {7} };\\n        int main(void) {\\n            [[c6502::unroll]] for (int i = 0; i < M[1][2]; i++) i;\\n            return 0;\\n        }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"M\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) InitList([Constant(ConstInt(7) None)] None None)] None None) Array(Array(Const(Int) 3) 2) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Subscript(Subscript(Var(\"M\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(2) None) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollConstArraySubscriptBound::test_const_array_zero_pads_missing_inner (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollConstArraySubscriptBound::test_const_array_zero_pads_missing_inner\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"M\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) InitList([Constant(ConstInt(7) None)] None None)] None None) Array(Array(Const(Int) 3) 2) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Subscript(Subscript(Var(\"M\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(2) None) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"M\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) InitList([Constant(ConstInt(7) None)] None None)] None None) Array(Array(Const(Int) 3) 2) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([])))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollConstArraySubscriptBound::test_non_const_array_not_folded (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollConstArraySubscriptBound::test_non_const_array_not_folded\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n        static int BOUNDS[3] = {3, 4, 5};\\n        int main(void) {\\n            [[c6502::unroll]] for (int i = 0; i < BOUNDS[2]; i++) i;\\n            return 0;\\n        }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"BOUNDS\" InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None) Constant(ConstInt(5) None)] None None) Array(Int 3) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Subscript(Var(\"BOUNDS\" None) Constant(ConstInt(2) None) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollConstArraySubscriptBound::test_non_const_array_not_folded (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollConstArraySubscriptBound::test_non_const_array_not_folded\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"BOUNDS\" InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None) Constant(ConstInt(5) None)] None None) Array(Int 3) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Subscript(Var(\"BOUNDS\" None) Constant(ConstInt(2) None) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: condition's right operand must be an integer constant\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollConstArraySubscriptBound::test_non_static_array_not_folded (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollConstArraySubscriptBound::test_non_static_array_not_folded\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n        int main(void) {\\n            const int BOUNDS[3] = {3, 4, 5};\\n            [[c6502::unroll]] for (int i = 0; i < BOUNDS[2]; i++) i;\\n            return 0;\\n        }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"BOUNDS\" InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None) Constant(ConstInt(5) None)] None None) Array(Const(Int) 3) None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Subscript(Var(\"BOUNDS\" None) Constant(ConstInt(2) None) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollConstArraySubscriptBound::test_non_static_array_not_folded (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollConstArraySubscriptBound::test_non_static_array_not_folded\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"BOUNDS\" InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None) Constant(ConstInt(5) None)] None None) Array(Const(Int) 3) None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Subscript(Var(\"BOUNDS\" None) Constant(ConstInt(2) None) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: condition's right operand must be an integer constant\"}\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollConstArraySubscriptBound::test_outer_iv_substituted_into_inner_bound (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollConstArraySubscriptBound::test_outer_iv_substituted_into_inner_bound\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n        static const int COUNT[3] = {2, 4, 1};\\n        int main(void) {\\n            int s = 0;\\n            [[c6502::unroll]] for (int b = 0; b < 3; b++) {\\n                [[c6502::unroll]] for (int i = 0; i < COUNT[b]; i++) s++;\\n            }\\n            return s;\\n        }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"COUNT\" InitList([Constant(ConstInt(2) None) Constant(ConstInt(4) None) Constant(ConstInt(1) None)] None None) Array(Const(Int) 3) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"b\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"b\" None) Constant(ConstInt(3) None) None) Postfix(Increment Var(\"b\" None) None) Compound(Block([\n"
      "      S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Subscript(Var(\"COUNT\" None) Var(\"b\" None) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Postfix(Increment Var(\"s\" None) None)) \"\" \"unroll\"))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Var(\"s\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollConstArraySubscriptBound::test_outer_iv_substituted_into_inner_bound (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollConstArraySubscriptBound::test_outer_iv_substituted_into_inner_bound\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"COUNT\" InitList([Constant(ConstInt(2) None) Constant(ConstInt(4) None) Constant(ConstInt(1) None)] None None) Array(Const(Int) 3) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"b\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"b\" None) Constant(ConstInt(3) None) None) Postfix(Increment Var(\"b\" None) None) Compound(Block([\n"
      "      S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Subscript(Var(\"COUNT\" None) Var(\"b\" None) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Postfix(Increment Var(\"s\" None) None)) \"\" \"unroll\"))\n"
      "    ])) \"\" \"unroll\"))\n"
      "    S(Return(Var(\"s\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"COUNT\" InitList([Constant(ConstInt(2) None) Constant(ConstInt(4) None) Constant(ConstInt(1) None)] None None) Array(Const(Int) 3) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        S(Compound(Block([\n"
      "          S(Compound(Block([\n"
      "            S(Expression(Postfix(Increment Var(\"s\" None) None)))\n"
      "          ])))\n"
      "          S(Compound(Block([\n"
      "            S(Expression(Postfix(Increment Var(\"s\" None) None)))\n"
      "          ])))\n"
      "        ])))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Compound(Block([\n"
      "          S(Compound(Block([\n"
      "            S(Expression(Postfix(Increment Var(\"s\" None) None)))\n"
      "          ])))\n"
      "          S(Compound(Block([\n"
      "            S(Expression(Postfix(Increment Var(\"s\" None) None)))\n"
      "          ])))\n"
      "          S(Compound(Block([\n"
      "            S(Expression(Postfix(Increment Var(\"s\" None) None)))\n"
      "          ])))\n"
      "          S(Compound(Block([\n"
      "            S(Expression(Postfix(Increment Var(\"s\" None) None)))\n"
      "          ])))\n"
      "        ])))\n"
      "      ])))\n"
      "      S(Compound(Block([\n"
      "        S(Compound(Block([\n"
      "          S(Compound(Block([\n"
      "            S(Expression(Postfix(Increment Var(\"s\" None) None)))\n"
      "          ])))\n"
      "        ])))\n"
      "      ])))\n"
      "    ])))\n"
      "    S(Return(Var(\"s\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_unroll.py::TestUnrollConstArraySubscriptBound::test_subscript_with_non_constant_index_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_unroll.py::TestUnrollConstArraySubscriptBound::test_subscript_with_non_constant_index_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n        static const int BOUNDS[3] = {3, 4, 5};\\n        int main(void) {\\n            int j = 1;\\n            [[c6502::unroll]] for (int i = 0; i < BOUNDS[j]; i++) i;\\n            return 0;\\n        }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"BOUNDS\" InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None) Constant(ConstInt(5) None)] None None) Array(Const(Int) 3) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"j\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Subscript(Var(\"BOUNDS\" None) Var(\"j\" None) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("unroll_program: test_unroll.py::TestUnrollConstArraySubscriptBound::test_subscript_with_non_constant_index_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass unroll_program\n"
      "source tests/test_unroll.py::TestUnrollConstArraySubscriptBound::test_subscript_with_non_constant_index_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"BOUNDS\" InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None) Constant(ConstInt(5) None)] None None) Array(Const(Int) 3) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"j\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Subscript(Var(\"BOUNDS\" None) Var(\"j\" None) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" \"unroll\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"UnrollError\", \"message\": \"unroll: condition's right operand must be an integer constant\"}\n");
}
