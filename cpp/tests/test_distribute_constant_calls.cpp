// The calls of `tac_distribute_constant`
// (passes/optimization/distribute_constant.py) that the Python unit
// tests make, each with the function the test gives the pass, the
// symbols the call read and the SSA names, and what the Python pass
// returned and changed, which the C++ pass must return and change too.
// They were recorded by wrapping the Python passes while
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

TEST_CASE("tac_distribute_constant: test_pointer_member_access.TestRuns.test_choice_of_member (1)") {
  check_stage_case(
      "tac_distribute_constant",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@15.n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%37.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"@14.p\", \"@15.n\"]}}}",
      "Function(\"option\" true [\"@14.p\" \"@15.n\"] [\n"
      "  Label(\".option@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@15.n\") Var(\"%35.1\"))\n"
      "  Binary(Subtract Var(\"%35.1\") Constant(ConstInt(1)) Var(\"%36.1\"))\n"
      "  Binary(Multiply Var(\"%36.1\") Constant(ConstInt(9)) Var(\"%37.1\"))\n"
      "  Binary(Add Var(\"@14.p\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "Function(\"option\" true [\"@14.p\" \"@15.n\"] [\n"
      "  Label(\".option@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@15.n\") Var(\"%35.1\"))\n"
      "  Binary(Subtract Var(\"%35.1\") Constant(ConstInt(1)) Var(\"%36.1\"))\n"
      "  Binary(Multiply Var(\"%35.1\") Constant(ConstInt(9)) Var(\"%37.1.dc0\"))\n"
      "  Binary(Subtract Var(\"%37.1.dc0\") Constant(ConstInt(9)) Var(\"%37.1\"))\n"
      "  Binary(Add Var(\"@14.p\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@15.n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%37.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%37.1.dc0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%35.1\", \"%36.1\", \"%37.1\", \"%37.1.dc0\", \"%38.1\", \"@14.p\", \"@15.n\"]}}}");
}

TEST_CASE("tac_distribute_constant: test_pointer_member_access.TestRuns.test_choice_of_member (2)") {
  check_stage_case(
      "tac_distribute_constant",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@16.n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%42.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%39.1\", \"%40.1\", \"%41.1\", \"%42.1\", \"@16.n\"]}}}",
      "Function(\"scaled\" true [\"@16.n\"] [\n"
      "  ZeroExtend(Var(\"@16.n\") Var(\"%39.1\"))\n"
      "  Binary(Subtract Var(\"%39.1\") Constant(ConstLong(5)) Var(\"%41.1\"))\n"
      "  Binary(Multiply Var(\"%41.1\") Constant(ConstLong(100000)) Var(\"%42.1\"))\n"
      "  Ret(Var(\"%42.1\"))\n"
      "])\n",
      "Function(\"scaled\" true [\"@16.n\"] [\n"
      "  ZeroExtend(Var(\"@16.n\") Var(\"%39.1\"))\n"
      "  Binary(Subtract Var(\"%39.1\") Constant(ConstLong(5)) Var(\"%41.1\"))\n"
      "  Binary(Multiply Var(\"%39.1\") Constant(ConstLong(100000)) Var(\"%42.1.dc0\"))\n"
      "  Binary(Subtract Var(\"%42.1.dc0\") Constant(ConstLong(500000)) Var(\"%42.1\"))\n"
      "  Ret(Var(\"%42.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@16.n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%42.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%42.1.dc0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%39.1\", \"%40.1\", \"%41.1\", \"%42.1\", \"%42.1.dc0\", \"@16.n\"]}}}");
}

TEST_CASE("tac_distribute_constant: test_pointer_member_access.TestRuns.test_choice_of_member (3)") {
  check_stage_case(
      "tac_distribute_constant",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@17.n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%45.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%43.1\", \"%44.1\", \"%45.1\", \"%46.1\", \"@17.n\"]}}}",
      "Function(\"times37\" true [\"@17.n\"] [\n"
      "  Label(\".times37@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@17.n\") Var(\"%43.1\"))\n"
      "  Binary(Add Var(\"%43.1\") Constant(ConstInt(3)) Var(\"%44.1\"))\n"
      "  Binary(Multiply Var(\"%44.1\") Constant(ConstInt(37)) Var(\"%45.1\"))\n"
      "  Copy(Var(\"%45.1\") Var(\"%46.1\"))\n"
      "  Ret(Var(\"%46.1\"))\n"
      "])\n",
      "Function(\"times37\" true [\"@17.n\"] [\n"
      "  Label(\".times37@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@17.n\") Var(\"%43.1\"))\n"
      "  Binary(Add Var(\"%43.1\") Constant(ConstInt(3)) Var(\"%44.1\"))\n"
      "  Binary(Multiply Var(\"%43.1\") Constant(ConstInt(37)) Var(\"%45.1.dc0\"))\n"
      "  Binary(Add Var(\"%45.1.dc0\") Constant(ConstInt(111)) Var(\"%45.1\"))\n"
      "  Copy(Var(\"%45.1\") Var(\"%46.1\"))\n"
      "  Ret(Var(\"%46.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@17.n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%45.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%45.1.dc0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%43.1\", \"%44.1\", \"%45.1\", \"%45.1.dc0\", \"%46.1\", \"@17.n\"]}}}");
}

TEST_CASE("tac_distribute_constant: test_pointer_member_access.TestShapes.test_index_minus_one_multiplies_the_byte (1)") {
  check_stage_case(
      "tac_distribute_constant",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@1.n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"@0.p\", \"@1.n\"]}}}",
      "Function(\"option\" true [\"@0.p\" \"@1.n\"] [\n"
      "  Label(\".option@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@1.n\") Var(\"%0.1\"))\n"
      "  Binary(Subtract Var(\"%0.1\") Constant(ConstInt(1)) Var(\"%1.1\"))\n"
      "  Binary(Multiply Var(\"%1.1\") Constant(ConstInt(9)) Var(\"%2.1\"))\n"
      "  Binary(Add Var(\"@0.p\") Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "Function(\"option\" true [\"@0.p\" \"@1.n\"] [\n"
      "  Label(\".option@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@1.n\") Var(\"%0.1\"))\n"
      "  Binary(Subtract Var(\"%0.1\") Constant(ConstInt(1)) Var(\"%1.1\"))\n"
      "  Binary(Multiply Var(\"%0.1\") Constant(ConstInt(9)) Var(\"%2.1.dc0\"))\n"
      "  Binary(Subtract Var(\"%2.1.dc0\") Constant(ConstInt(9)) Var(\"%2.1\"))\n"
      "  Binary(Add Var(\"@0.p\") Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@1.n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1.dc0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%2.1.dc0\", \"%3.1\", \"@0.p\", \"@1.n\"]}}}");
}
