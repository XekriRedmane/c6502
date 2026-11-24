// The calls that the tests of tests/test_narrow_byte_range.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 and G7). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("tac_reduce_strength: test_narrow_byte_range.py::TestSignedNonnegativeStrength::test_divide_becomes_shift (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_reduce_strength\n"
      "source tests/test_narrow_byte_range.py::TestSignedNonnegativeStrength::test_divide_becomes_shift\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"ze\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"b\", \"c\", \"r\", \"s\", \"se\", \"t\", \"u\", \"uw\", \"w\", \"zc\", \"ze\"]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  ZeroExtend(Var(\"c\") Var(\"zc\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(Add Var(\"ze\") Constant(ConstInt(1)) Var(\"t\"))\n"
      "  Binary(Divide Var(\"t\") Constant(ConstInt(2)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  ZeroExtend(Var(\"c\") Var(\"zc\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(Add Var(\"ze\") Constant(ConstInt(1)) Var(\"t\"))\n"
      "  Binary(RightShift Var(\"t\") Constant(ConstInt(1)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n");
}

TEST_CASE("tac_reduce_strength: test_narrow_byte_range.py::TestSignedNonnegativeStrength::test_modulo_becomes_mask (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_reduce_strength\n"
      "source tests/test_narrow_byte_range.py::TestSignedNonnegativeStrength::test_modulo_becomes_mask\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"ze\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"b\", \"c\", \"r\", \"s\", \"se\", \"t\", \"u\", \"uw\", \"w\", \"zc\", \"ze\"]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  ZeroExtend(Var(\"c\") Var(\"zc\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(Add Var(\"ze\") Constant(ConstInt(1)) Var(\"t\"))\n"
      "  Binary(Modulo Var(\"t\") Constant(ConstInt(8)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  ZeroExtend(Var(\"c\") Var(\"zc\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(Add Var(\"ze\") Constant(ConstInt(1)) Var(\"t\"))\n"
      "  Binary(BitwiseAnd Var(\"t\") Constant(ConstInt(7)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n");
}

TEST_CASE("tac_reduce_strength: test_narrow_byte_range.py::TestSignedNonnegativeStrength::test_possibly_negative_dividend_is_left (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_reduce_strength\n"
      "source tests/test_narrow_byte_range.py::TestSignedNonnegativeStrength::test_possibly_negative_dividend_is_left\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"se\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  ZeroExtend(Var(\"c\") Var(\"zc\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(Divide Var(\"se\") Constant(ConstInt(2)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  ZeroExtend(Var(\"c\") Var(\"zc\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(Divide Var(\"se\") Constant(ConstInt(2)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n");
}
