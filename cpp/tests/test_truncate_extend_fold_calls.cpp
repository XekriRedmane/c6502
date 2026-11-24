// The calls that the tests of tests/test_truncate_extend_fold.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 and G7). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("tac_fold_truncate_extend: test_truncate_extend_fold.py::TestFoldTruncateExtend::test_extend_then_wider_truncate_is_a_narrower_extend (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_fold_truncate_extend\n"
      "source tests/test_truncate_extend_fold.py::TestFoldTruncateExtend::test_extend_then_wider_truncate_is_a_narrower_extend\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"i\", \"t\", \"u\"]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"i\") Var(\"t\"))\n"
      "  Truncate(Var(\"t\") Var(\"u\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"i\") Var(\"t\"))\n"
      "  SignExtend(Var(\"i\") Var(\"u\"))\n"
      "])\n");
}

TEST_CASE("tac_fold_truncate_extend: test_truncate_extend_fold.py::TestFoldTruncateExtend::test_extend_then_wider_truncate_of_non_ssa_source_left_alone (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_fold_truncate_extend\n"
      "source tests/test_truncate_extend_fold.py::TestFoldTruncateExtend::test_extend_then_wider_truncate_of_non_ssa_source_left_alone\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"t\", \"u\"]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"i\") Var(\"t\"))\n"
      "  Truncate(Var(\"t\") Var(\"u\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"i\") Var(\"t\"))\n"
      "  Truncate(Var(\"t\") Var(\"u\"))\n"
      "])\n");
}

TEST_CASE("tac_fold_truncate_extend: test_truncate_extend_fold.py::TestFoldTruncateExtend::test_global_dst_not_folded (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_fold_truncate_extend\n"
      "source tests/test_truncate_extend_fold.py::TestFoldTruncateExtend::test_global_dst_not_folded\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"t\", \"u\"]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"t\") Var(\"ui\"))\n"
      "  Truncate(Var(\"ui\") Var(\"u\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"t\") Var(\"ui\"))\n"
      "  Truncate(Var(\"ui\") Var(\"u\"))\n"
      "])\n");
}

TEST_CASE("tac_fold_truncate_extend: test_truncate_extend_fold.py::TestFoldTruncateExtend::test_no_ssa_dsts_is_noop (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_fold_truncate_extend\n"
      "source tests/test_truncate_extend_fold.py::TestFoldTruncateExtend::test_no_ssa_dsts_is_noop\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"i\") Var(\"t\"))\n"
      "  Truncate(Var(\"t\") Var(\"u\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"i\") Var(\"t\"))\n"
      "  Truncate(Var(\"t\") Var(\"u\"))\n"
      "])\n");
}

TEST_CASE("tac_fold_truncate_extend: test_truncate_extend_fold.py::TestFoldTruncateExtend::test_non_ssa_source_not_folded (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_fold_truncate_extend\n"
      "source tests/test_truncate_extend_fold.py::TestFoldTruncateExtend::test_non_ssa_source_not_folded\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"t\", \"u\"]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"g\") Var(\"t\"))\n"
      "  Binary(Add Var(\"g\") Constant(ConstChar(1)) Var(\"g\"))\n"
      "  Truncate(Var(\"t\") Var(\"u\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"g\") Var(\"t\"))\n"
      "  Binary(Add Var(\"g\") Constant(ConstChar(1)) Var(\"g\"))\n"
      "  Truncate(Var(\"t\") Var(\"u\"))\n"
      "])\n");
}

TEST_CASE("tac_fold_truncate_extend: test_truncate_extend_fold.py::TestFoldTruncateExtend::test_signext_then_truncate_same_width_becomes_copy (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_fold_truncate_extend\n"
      "source tests/test_truncate_extend_fold.py::TestFoldTruncateExtend::test_signext_then_truncate_same_width_becomes_copy\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"i\", \"t\", \"u\"]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"i\") Var(\"t\"))\n"
      "  Truncate(Var(\"t\") Var(\"u\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"i\") Var(\"t\"))\n"
      "  Copy(Var(\"i\") Var(\"u\"))\n"
      "])\n");
}

TEST_CASE("tac_fold_truncate_extend: test_truncate_extend_fold.py::TestFoldTruncateExtend::test_truncate_of_truncate (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_fold_truncate_extend\n"
      "source tests/test_truncate_extend_fold.py::TestFoldTruncateExtend::test_truncate_of_truncate\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"t\", \"u\", \"x\"]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Truncate(Var(\"x\") Var(\"t\"))\n"
      "  Truncate(Var(\"t\") Var(\"u\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  Truncate(Var(\"x\") Var(\"t\"))\n"
      "  Truncate(Var(\"x\") Var(\"u\"))\n"
      "])\n");
}

TEST_CASE("tac_fold_truncate_extend: test_truncate_extend_fold.py::TestFoldTruncateExtend::test_zero_extend_then_wider_truncate (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_fold_truncate_extend\n"
      "source tests/test_truncate_extend_fold.py::TestFoldTruncateExtend::test_zero_extend_then_wider_truncate\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"i\", \"t\", \"u\"]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"i\") Var(\"t\"))\n"
      "  Truncate(Var(\"t\") Var(\"u\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"i\") Var(\"t\"))\n"
      "  ZeroExtend(Var(\"i\") Var(\"u\"))\n"
      "])\n");
}

TEST_CASE("tac_fold_truncate_extend: test_truncate_extend_fold.py::TestFoldTruncateExtend::test_zeroext_then_truncate_same_width_becomes_copy (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_fold_truncate_extend\n"
      "source tests/test_truncate_extend_fold.py::TestFoldTruncateExtend::test_zeroext_then_truncate_same_width_becomes_copy\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"i\", \"t\", \"u\"]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"i\") Var(\"t\"))\n"
      "  Truncate(Var(\"t\") Var(\"u\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"i\") Var(\"t\"))\n"
      "  Copy(Var(\"i\") Var(\"u\"))\n"
      "])\n");
}

TEST_CASE("tac_fold_extend_extend: test_truncate_extend_fold.py::TestFoldExtendExtend::test_non_ssa_source_left_alone (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_fold_extend_extend\n"
      "source tests/test_truncate_extend_fold.py::TestFoldExtendExtend::test_non_ssa_source_left_alone\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"t\", \"u\"]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"i\") Var(\"t\"))\n"
      "  SignExtend(Var(\"t\") Var(\"u\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"i\") Var(\"t\"))\n"
      "  SignExtend(Var(\"t\") Var(\"u\"))\n"
      "])\n");
}

TEST_CASE("tac_fold_extend_extend: test_truncate_extend_fold.py::TestFoldExtendExtend::test_sign_extend_of_sign_extend (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_fold_extend_extend\n"
      "source tests/test_truncate_extend_fold.py::TestFoldExtendExtend::test_sign_extend_of_sign_extend\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"i\", \"t\", \"u\"]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"i\") Var(\"t\"))\n"
      "  SignExtend(Var(\"t\") Var(\"u\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"i\") Var(\"t\"))\n"
      "  SignExtend(Var(\"i\") Var(\"u\"))\n"
      "])\n");
}

TEST_CASE("tac_fold_extend_extend: test_truncate_extend_fold.py::TestFoldExtendExtend::test_sign_extend_of_zero_extend_is_zero_extend (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_fold_extend_extend\n"
      "source tests/test_truncate_extend_fold.py::TestFoldExtendExtend::test_sign_extend_of_zero_extend_is_zero_extend\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"i\", \"t\", \"u\"]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"i\") Var(\"t\"))\n"
      "  SignExtend(Var(\"t\") Var(\"u\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"i\") Var(\"t\"))\n"
      "  ZeroExtend(Var(\"i\") Var(\"u\"))\n"
      "])\n");
}

TEST_CASE("tac_fold_extend_extend: test_truncate_extend_fold.py::TestFoldExtendExtend::test_zero_extend_of_sign_extend_left_alone (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_fold_extend_extend\n"
      "source tests/test_truncate_extend_fold.py::TestFoldExtendExtend::test_zero_extend_of_sign_extend_left_alone\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"i\", \"t\", \"u\"]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"i\") Var(\"t\"))\n"
      "  ZeroExtend(Var(\"t\") Var(\"u\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"i\") Var(\"t\"))\n"
      "  ZeroExtend(Var(\"t\") Var(\"u\"))\n"
      "])\n");
}

TEST_CASE("tac_fold_extend_extend: test_truncate_extend_fold.py::TestFoldExtendExtend::test_zero_extend_of_zero_extend (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_fold_extend_extend\n"
      "source tests/test_truncate_extend_fold.py::TestFoldExtendExtend::test_zero_extend_of_zero_extend\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"i\", \"t\", \"u\"]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"i\") Var(\"t\"))\n"
      "  ZeroExtend(Var(\"t\") Var(\"u\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"i\") Var(\"t\"))\n"
      "  ZeroExtend(Var(\"i\") Var(\"u\"))\n"
      "])\n");
}
