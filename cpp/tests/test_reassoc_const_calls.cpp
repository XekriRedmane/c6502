// The calls of `tac_reassoc_constants`
// (passes/optimization/reassoc_const.py) that the Python unit tests
// make, each with the function the test gives the pass, the symbols the
// call read and the SSA names, and what the Python pass returned and
// changed, which the C++ pass must return and change too. They were
// recorded by wrapping the Python passes while
// tests/test_constant_folding.py, test_strength_reduction.py,
// test_static_const_fold.py, test_reassoc_offset_licm.py,
// test_split_tables.py, test_pointer_member_access.py, and
// test_narrow_byte_range.py ran, leaving out each call whose record
// takes more than 15 KB. They are the calls that changed their
// function. A test that calls the pass several times gives a case for
// each distinct call.
#include "doctest.h"
#include "stage_case.h"

using c6502::testing::check_stage_case;

TEST_CASE("tac_reassoc_constants: test_static_const_fold.TestReassocConstantsUnit.test_combines_two_constants (1)") {
  check_stage_case(
      "tac_reassoc_constants",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": {\"set\": [\"%inner\", \"%outer\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Constant(ConstUInt(256)) Var(\"v\") Var(\"%inner\"))\n"
      "  Binary(Add Constant(ConstUInt(512)) Var(\"%inner\") Var(\"%outer\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Constant(ConstUInt(768)) Var(\"v\") Var(\"%outer\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reassoc_constants: test_static_const_fold.TestReassocConstantsUnit.test_commutative_arrangement_combines (1)") {
  check_stage_case(
      "tac_reassoc_constants",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": {\"set\": [\"%inner\", \"%outer\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"v\") Constant(ConstUInt(256)) Var(\"%inner\"))\n"
      "  Binary(Add Var(\"%inner\") Constant(ConstUInt(512)) Var(\"%outer\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Constant(ConstUInt(768)) Var(\"v\") Var(\"%outer\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reassoc_constants: test_static_const_fold.TestReassocConstantsUnit.test_mixed_signedness_same_width_combines (1)") {
  check_stage_case(
      "tac_reassoc_constants",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": {\"set\": [\"%inner\", \"%outer\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Constant(ConstInt(100)) Var(\"v\") Var(\"%inner\"))\n"
      "  Binary(Add Constant(ConstUInt(8192)) Var(\"%inner\") Var(\"%outer\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Constant(ConstUInt(8292)) Var(\"v\") Var(\"%outer\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reassoc_constants: test_static_const_fold.TestReassocConstantsUnit.test_wrap_at_uint_width (1)") {
  check_stage_case(
      "tac_reassoc_constants",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": {\"set\": [\"%inner\", \"%outer\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Constant(ConstUInt(65535)) Var(\"v\") Var(\"%inner\"))\n"
      "  Binary(Add Constant(ConstUInt(2)) Var(\"%inner\") Var(\"%outer\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Constant(ConstUInt(1)) Var(\"v\") Var(\"%outer\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reassoc_constants: test_static_const_fold.TestStaticConstFoldCorrectness.test_pointer_const_static_indexed_write (1)") {
  check_stage_case(
      "tac_reassoc_constants",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%3.1\", \"%4.1\", \"%5.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Add Constant(ConstUInt(512)) Constant(ConstUInt(5)) Var(\"%4.1\"))\n"
      "  Binary(Add Constant(ConstUInt(16384)) Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  Store(Constant(ConstUChar(66)) Var(\"%5.1\") false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Binary(Add Constant(ConstUInt(16896)) Constant(ConstUInt(5)) Var(\"%5.1\"))\n"
      "  Store(Constant(ConstUChar(66)) Var(\"%5.1\") false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}
