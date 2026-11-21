// The calls of the asm optimizer's driver that reach the lines of
// optimizer.py, apply_coloring.py, const_static_fold.py, dead_static.py,
// and function_local_sizing.py that no call of the corpus reaches, each
// with the program or function it is given, its arguments, and what the
// Python function returned, which the C++ function must return too. A
// line trace of the five modules over the corpus and over the unit tests
// of the modules that compile through them chose them. The direct calls
// of tests/test_dead_static.py and tests/test_function_local_sizing.py
// are here, with the calls of four tests of tests/test_const_static_fold.py,
// test_byte_arith_sim.py, and test_hwreg_pinning.py. The calls named "f1
// synthetic" were made on inputs written to reach the lines that neither
// the corpus nor a unit test reaches, and recorded from the Python
// functions the same way. The one that raises gives Python's message.
#include "doctest.h"
#include "stage_case.h"

using c6502::testing::check_stage_call;
using c6502::testing::check_stage_case;

TEST_CASE("fold_const_statics: tests.test_const_static_fold.TestConstPointer.test_reads_the_address (1)") {
  check_stage_case(
      "fold_const_statics",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"buf\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 8)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [1, 2, 3, 4, 5, 6, 7, 8]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"B\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Pointer(Const(UChar)))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"buf\", \"offset\": 3}}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"addres"
      "s\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"at\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"ptr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Pointer(Const(UChar)))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"secti"
      "on\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.bad\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\":"
      " \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6\", {"
      "\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking"
      ".LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%12\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%15\", {\"object\": \"passes.type_checking.Symbol\", \""
      "fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"field"
      "s\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.bad.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\":"
      " \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false"
      "}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%12.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\":"
      " \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.bad.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.bad.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%15.1\", {\"obj"
      "ect\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.bad.4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.bad.5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \""
      "passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.i.reg\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}",
      "Program([\n"
      "  Function(\"at\" true [\"@0.i\"] [\n"
      "    Mov(Reg(A) Pseudo(\"@0.i.reg\" 0) false)\n"
      "    Mov(Pseudo(\"B\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Pseudo(\"B\" 1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Pseudo(\"@0.i.reg\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(Y) false)\n"
      "    Mov(IndirectY Reg(A) false)\n"
      "    Mov(Reg(A) Pseudo(\"%2.1\" 0) false)\n"
      "    Mov(Pseudo(\"%2.1\" 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "  Function(\"ptr\" true [] [\n"
      "    Mov(Pseudo(\"B\" 1) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Pseudo(\"B\" 0) Reg(A) false)\n"
      "    Return(true true false)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Call(\"at\" [\"A\"] [])\n"
      "    Mov(Reg(A) Pseudo(\"%5.1\" 0) false)\n"
      "    Mov(Pseudo(\"%5.1\" 0) Reg(A) false)\n"
      "    Compare(Reg(A) Imm(4))\n"
      "    Branch(NE \".main@ssa_block@3\")\n"
      "    Mov(Imm(4) Reg(A) false)\n"
      "    Call(\"at\" [\"A\"] [])\n"
      "    Mov(Reg(A) Pseudo(\"%9.1\" 0) false)\n"
      "    Mov(Pseudo(\"%9.1\" 0) Reg(A) false)\n"
      "    Compare(Reg(A) Imm(8))\n"
      "    Branch(NE \".main@ssa_block@3\")\n"
      "    Label(\".main@ssa_block@2\")\n"
      "    Mov(Imm(0) Pseudo(\"@1.bad.3\" 0) false)\n"
      "    Mov(Imm(0) Pseudo(\"@1.bad.3\" 1) false)\n"
      "    Jump(\".if_end@2\" [])\n"
      "    Label(\".main@ssa_block@3\")\n"
      "    Mov(Imm(1) Pseudo(\"@1.bad.3\" 0) false)\n"
      "    Mov(Imm(0) Pseudo(\"@1.bad.3\" 1) false)\n"
      "    Label(\".if_end@2\")\n"
      "    Call(\"ptr\" [] [])\n"
      "    Mov(Reg(A) Pseudo(\"%13.1\" 0) false)\n"
      "    Mov(Reg(X) Pseudo(\"%13.1\" 1) false)\n"
      "    Mov(ImmLabelLow(\"buf\" 0) Pseudo(\"%14.1\" 0) false)\n"
      "    Mov(ImmLabelHigh(\"buf\" 0) Pseudo(\"%14.1\" 1) false)\n"
      "    Mov(Pseudo(\"%14.1\" 0) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Imm(3) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"%15.1\" 0) false)\n"
      "    Mov(Pseudo(\"%14.1\" 1) Reg(A) false)\n"
      "    Add(Imm(0) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"%15.1\" 1) false)\n"
      "    Mov(Pseudo(\"@1.bad.3\" 0) Pseudo(\"@1.bad.5\" 0) false)\n"
      "    Mov(Pseudo(\"@1.bad.3\" 1) Pseudo(\"@1.bad.5\" 1) false)\n"
      "    Mov(Pseudo(\"%13.1\" 1) Reg(A) false)\n"
      "    Compare(Reg(A) Pseudo(\"%15.1\" 1))\n"
      "    Branch(NE \".jcmp_skip@0\")\n"
      "    Mov(Pseudo(\"%13.1\" 0) Reg(A) false)\n"
      "    Compare(Reg(A) Pseudo(\"%15.1\" 0))\n"
      "    Branch(EQ \".if_end@3\")\n"
      "    Label(\".jcmp_skip@0\")\n"
      "    Label(\".main@ssa_block@4\")\n"
      "    Mov(Pseudo(\"@1.bad.3\" 0) Reg(A) false)\n"
      "    Or(Imm(2) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"@1.bad.5\" 0) false)\n"
      "    Mov(Pseudo(\"@1.bad.3\" 1) Reg(A) false)\n"
      "    Or(Imm(0) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"@1.bad.5\" 1) false)\n"
      "    Label(\".if_end@3\")\n"
      "    Mov(Pseudo(\"@1.bad.5\" 1) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Pseudo(\"@1.bad.5\" 0) Reg(A) false)\n"
      "    Return(true true false)\n"
      "  ])\n"
      "  StaticVariable(\"buf\" true [StringInit(\"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\x08\" 8)])\n"
      "  StaticVariable(\"B\" false [AddressInit(\"buf\" 3)])\n"
      "])\n",
      "Program([\n"
      "  Function(\"at\" true [\"@0.i\"] [\n"
      "    Mov(Reg(A) Pseudo(\"@0.i.reg\" 0) false)\n"
      "    Mov(ImmLabelLow(\"buf\" 3) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ImmLabelHigh(\"buf\" 3) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Pseudo(\"@0.i.reg\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(Y) false)\n"
      "    Mov(IndirectY Reg(A) false)\n"
      "    Mov(Reg(A) Pseudo(\"%2.1\" 0) false)\n"
      "    Mov(Pseudo(\"%2.1\" 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "  Function(\"ptr\" true [] [\n"
      "    Mov(ImmLabelHigh(\"buf\" 3) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(ImmLabelLow(\"buf\" 3) Reg(A) false)\n"
      "    Return(true true false)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Call(\"at\" [\"A\"] [])\n"
      "    Mov(Reg(A) Pseudo(\"%5.1\" 0) false)\n"
      "    Mov(Pseudo(\"%5.1\" 0) Reg(A) false)\n"
      "    Compare(Reg(A) Imm(4))\n"
      "    Branch(NE \".main@ssa_block@3\")\n"
      "    Mov(Imm(4) Reg(A) false)\n"
      "    Call(\"at\" [\"A\"] [])\n"
      "    Mov(Reg(A) Pseudo(\"%9.1\" 0) false)\n"
      "    Mov(Pseudo(\"%9.1\" 0) Reg(A) false)\n"
      "    Compare(Reg(A) Imm(8))\n"
      "    Branch(NE \".main@ssa_block@3\")\n"
      "    Label(\".main@ssa_block@2\")\n"
      "    Mov(Imm(0) Pseudo(\"@1.bad.3\" 0) false)\n"
      "    Mov(Imm(0) Pseudo(\"@1.bad.3\" 1) false)\n"
      "    Jump(\".if_end@2\" [])\n"
      "    Label(\".main@ssa_block@3\")\n"
      "    Mov(Imm(1) Pseudo(\"@1.bad.3\" 0) false)\n"
      "    Mov(Imm(0) Pseudo(\"@1.bad.3\" 1) false)\n"
      "    Label(\".if_end@2\")\n"
      "    Call(\"ptr\" [] [])\n"
      "    Mov(Reg(A) Pseudo(\"%13.1\" 0) false)\n"
      "    Mov(Reg(X) Pseudo(\"%13.1\" 1) false)\n"
      "    Mov(ImmLabelLow(\"buf\" 0) Pseudo(\"%14.1\" 0) false)\n"
      "    Mov(ImmLabelHigh(\"buf\" 0) Pseudo(\"%14.1\" 1) false)\n"
      "    Mov(Pseudo(\"%14.1\" 0) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Imm(3) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"%15.1\" 0) false)\n"
      "    Mov(Pseudo(\"%14.1\" 1) Reg(A) false)\n"
      "    Add(Imm(0) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"%15.1\" 1) false)\n"
      "    Mov(Pseudo(\"@1.bad.3\" 0) Pseudo(\"@1.bad.5\" 0) false)\n"
      "    Mov(Pseudo(\"@1.bad.3\" 1) Pseudo(\"@1.bad.5\" 1) false)\n"
      "    Mov(Pseudo(\"%13.1\" 1) Reg(A) false)\n"
      "    Compare(Reg(A) Pseudo(\"%15.1\" 1))\n"
      "    Branch(NE \".jcmp_skip@0\")\n"
      "    Mov(Pseudo(\"%13.1\" 0) Reg(A) false)\n"
      "    Compare(Reg(A) Pseudo(\"%15.1\" 0))\n"
      "    Branch(EQ \".if_end@3\")\n"
      "    Label(\".jcmp_skip@0\")\n"
      "    Label(\".main@ssa_block@4\")\n"
      "    Mov(Pseudo(\"@1.bad.3\" 0) Reg(A) false)\n"
      "    Or(Imm(2) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"@1.bad.5\" 0) false)\n"
      "    Mov(Pseudo(\"@1.bad.3\" 1) Reg(A) false)\n"
      "    Or(Imm(0) Reg(A))\n"
      "    Mov(Reg(A) Pseudo(\"@1.bad.5\" 1) false)\n"
      "    Label(\".if_end@3\")\n"
      "    Mov(Pseudo(\"@1.bad.5\" 1) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Pseudo(\"@1.bad.5\" 0) Reg(A) false)\n"
      "    Return(true true false)\n"
      "  ])\n"
      "  StaticVariable(\"buf\" true [StringInit(\"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\x08\" 8)])\n"
      "])\n",
      "");
}

TEST_CASE("fold_const_statics: tests.test_const_static_fold.TestConstStaticFoldAsmShape.test_address_taken_keeps_static (1)") {
  check_stage_case(
      "fold_const_statics",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"buf\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Pointer(UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 16384}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"ptr_holder\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Pointer(UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"pass"
      "es.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Pointer(UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Pointer(UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(ImmLabelLow(\"buf\" 0) Pseudo(\"ptr_holder\" 0) false)\n"
      "    Mov(ImmLabelHigh(\"buf\" 0) Pseudo(\"ptr_holder\" 1) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Return(true true false)\n"
      "  ])\n"
      "  StaticVariable(\"buf\" false [IntInit(16384)])\n"
      "  StaticVariable(\"ptr_holder\" true [ZeroInit(2)])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(ImmLabelLow(\"buf\" 0) Pseudo(\"ptr_holder\" 0) false)\n"
      "    Mov(ImmLabelHigh(\"buf\" 0) Pseudo(\"ptr_holder\" 1) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Return(true true false)\n"
      "  ])\n"
      "  StaticVariable(\"buf\" false [IntInit(16384)])\n"
      "  StaticVariable(\"ptr_holder\" true [ZeroInit(2)])\n"
      "])\n",
      "");
}

TEST_CASE("fold_const_statics: tests.test_const_static_fold.TestConstStaticFoldUnit.test_no_symbols_is_noop (1)") {
  check_stage_case(
      "fold_const_statics",
      "{\"args\": [], \"kwargs\": {\"symbols\": null}}",
      "Program([\n"
      "  StaticVariable(\"x\" false [IntInit(4660)])\n"
      "])\n",
      "Program([\n"
      "  StaticVariable(\"x\" false [IntInit(4660)])\n"
      "])\n",
      "");
}

TEST_CASE("dead_static_elimination: tests.test_dead_static (1)") {
  check_stage_case(
      "dead_static_elimination",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  StaticVariable(\"global_x\" true [IntInit(0)])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  StaticVariable(\"global_x\" true [IntInit(0)])\n"
      "])\n",
      "");
}

TEST_CASE("dead_static_elimination: tests.test_dead_static (2)") {
  check_stage_case(
      "dead_static_elimination",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Data(\"b\" 0) Reg(A) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  StaticVariable(\"a\" false [IntInit(42)])\n"
      "  StaticVariable(\"b\" false [AddressInit(\"a\" 0)])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Data(\"b\" 0) Reg(A) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  StaticVariable(\"a\" false [IntInit(42)])\n"
      "  StaticVariable(\"b\" false [AddressInit(\"a\" 0)])\n"
      "])\n",
      "");
}

TEST_CASE("dead_static_elimination: tests.test_dead_static (3)") {
  check_stage_case(
      "dead_static_elimination",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Data(\"kept\" 0) Reg(A) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  StaticVariable(\"kept\" false [IntInit(0)])\n"
      "  StaticVariable(\"dead\" false [IntInit(0)])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Data(\"kept\" 0) Reg(A) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  StaticVariable(\"kept\" false [IntInit(0)])\n"
      "])\n",
      "");
}

TEST_CASE("dead_static_elimination: tests.test_dead_static (4)") {
  check_stage_case(
      "dead_static_elimination",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Pseudo(\"kept\" 0) Reg(A) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  StaticVariable(\"kept\" false [IntInit(0)])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Pseudo(\"kept\" 0) Reg(A) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  StaticVariable(\"kept\" false [IntInit(0)])\n"
      "])\n",
      "");
}

TEST_CASE("dead_static_elimination: tests.test_dead_static (5)") {
  check_stage_case(
      "dead_static_elimination",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(168) Reg(A) false)\n"
      "    Mov(Reg(A) IndexedData(\"\" 8360 X) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  StaticVariable(\"interlace_p1_offsets\" false [LongInit(168)])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(168) Reg(A) false)\n"
      "    Mov(Reg(A) IndexedData(\"\" 8360 X) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "");
}

TEST_CASE("dead_static_elimination: tests.test_dead_static (6)") {
  check_stage_case(
      "dead_static_elimination",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(IndexedData(\"kept\" 0 X) Reg(A) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  StaticVariable(\"kept\" false [IntInit(0)])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(IndexedData(\"kept\" 0 X) Reg(A) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  StaticVariable(\"kept\" false [IntInit(0)])\n"
      "])\n",
      "");
}

TEST_CASE("dead_static_elimination: tests.test_dead_static (7)") {
  check_stage_case(
      "dead_static_elimination",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    LoadAddress(Data(\"kept\" 0) Pseudo(\"ptr\" 0))\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  StaticVariable(\"kept\" false [IntInit(0)])\n"
      "  StaticVariable(\"ptr\" false [IntInit(0)])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    LoadAddress(Data(\"kept\" 0) Pseudo(\"ptr\" 0))\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  StaticVariable(\"kept\" false [IntInit(0)])\n"
      "  StaticVariable(\"ptr\" false [IntInit(0)])\n"
      "])\n",
      "");
}

TEST_CASE("dead_static_elimination: tests.test_dead_static (8)") {
  check_stage_case(
      "dead_static_elimination",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  StaticVariable(\"dead\" false [IntInit(0)])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "");
}

TEST_CASE("dead_static_elimination: tests.test_dead_static (9)") {
  check_stage_case(
      "dead_static_elimination",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"g\" true [] [\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"g\" true [] [\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "");
}

TEST_CASE("dead_static_elimination: tests.test_dead_static (10)") {
  check_stage_case(
      "dead_static_elimination",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(ImmLabelLow(\"kept\" 0) Reg(A) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  StaticVariable(\"kept\" false [IntInit(0)])\n"
      "])\n",
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(ImmLabelLow(\"kept\" 0) Reg(A) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  StaticVariable(\"kept\" false [IntInit(0)])\n"
      "])\n",
      "");
}

TEST_CASE("compute_local_bytes: tests.test_function_local_sizing.TestComputeLocalBytes (1)") {
  check_stage_case(
      "compute_local_bytes",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(85) Data(\"HARGS\" 0) false)\n"
      "    Mov(Data(\"__zpabi_f_p0\" 0) Reg(A) false)\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"f\", 0]]}",
      "");
}

TEST_CASE("compute_local_bytes: tests.test_function_local_sizing.TestComputeLocalBytes (2)") {
  check_stage_case(
      "compute_local_bytes",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"f\" true [] [])\n"
      "])\n",
      "{\"dict\": [[\"f\", 0]]}",
      "");
}

TEST_CASE("compute_local_bytes: tests.test_function_local_sizing.TestComputeLocalBytes (3)") {
  check_stage_case(
      "compute_local_bytes",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(85) Reg(A) false)\n"
      "    Mov(Reg(A) Frame(1) false)\n"
      "    Mov(Pseudo(\"p\" 0) Reg(A) false)\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"f\", 0]]}",
      "");
}

TEST_CASE("compute_local_bytes: tests.test_function_local_sizing.TestComputeLocalBytes (4)") {
  check_stage_case(
      "compute_local_bytes",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(1) ZP(128 0) false)\n"
      "    Mov(Imm(2) ZP(129 0) false)\n"
      "    Mov(Imm(3) ZP(130 0) false)\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"f\", 3]]}",
      "");
}

TEST_CASE("compute_local_bytes: tests.test_function_local_sizing.TestComputeLocalBytes (5)") {
  check_stage_case(
      "compute_local_bytes",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(66) ZP(128 0) false)\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"f\", 1]]}",
      "");
}

TEST_CASE("compute_local_bytes: tests.test_function_local_sizing.TestComputeLocalBytes (6)") {
  check_stage_case(
      "compute_local_bytes",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"f1\" true [] [\n"
      "    Mov(Imm(0) ZP(128 0) false)\n"
      "    Mov(Imm(0) ZP(129 0) false)\n"
      "  ])\n"
      "  Function(\"f2\" true [] [\n"
      "    Mov(Imm(0) ZP(192 0) false)\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"f1\", 2], [\"f2\", 1]]}",
      "");
}

TEST_CASE("compute_local_bytes: tests.test_function_local_sizing.TestComputeLocalBytes (7)") {
  check_stage_case(
      "compute_local_bytes",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(1) ZP(144 0) false)\n"
      "    Mov(Imm(2) ZP(144 1) false)\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"f\", 2]]}",
      "");
}

TEST_CASE("compute_local_bytes: tests.test_function_local_sizing.TestComputeLocalBytes (8)") {
  check_stage_case(
      "compute_local_bytes",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(1) ZP(128 0) false)\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(128 0) false)\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"f\", 1]]}",
      "");
}

TEST_CASE("compute_local_bytes: tests.test_function_local_sizing.TestComputeLocalBytes (9)") {
  check_stage_case(
      "compute_local_bytes",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(0) ZP(128 0) false)\n"
      "  ])\n"
      "  StaticVariable(\"x\" false [IntInit(0)])\n"
      "])\n",
      "{\"dict\": [[\"f\", 1]]}",
      "");
}

TEST_CASE("apply_coloring: tests.test_hwreg_pinning.TestApplyColoringHwReg (1)") {
  check_stage_case(
      "apply_coloring",
      "{\"args\": [{\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": []}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": [[\"P\", \"Y\"]]}}}], \"kwargs\": {}}",
      "Function(\"test\" true [] [\n"
      "  Mov(Imm(5) Pseudo(\"P\" 0) false)\n"
      "  Inc(Pseudo(\"P\" 0))\n"
      "])\n",
      "Function(\"test\" true [] [\n"
      "  Mov(Imm(5) Reg(Y) false)\n"
      "  Inc(Reg(Y))\n"
      "])\n",
      "");
}

TEST_CASE("apply_coloring: f1 synthetic cases (1)") {
  check_stage_case(
      "apply_coloring",
      "{\"args\": [{\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"%z\", 144]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": [[\"%p\", \"Y\"]]}}}], \"kwargs\": {}}",
      "Function(\"f\" true [] [\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(Reg(Y) IndexedData(\"arr\" 0 X) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Mov(Reg(Y) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(Reg(Y) IndexedData(\"arr\" 0 X) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("apply_coloring: f1 synthetic cases (2)") {
  check_stage_case(
      "apply_coloring",
      "{\"args\": [{\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"%x.b0.v3\", 128], [\"%x.b0.v1\", 128], [\"%x.b0.v2\", 128]]}, \"spilled\": {\"set\": [\"%y.b0.v1\"]}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}], \"kwargs\": {\"local_pool\": null}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  Mov(Data(\"in\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Pseudo(\"%y.b0.v1\" 0) false)\n"
      "  Mov(Data(\"in\" 1) Reg(A) false)\n"
      "  Branch(EQ \"L1\")\n"
      "  Label(\".f@asm_ssa_block@1\")\n"
      "  Mov(Data(\"in\" 2) Reg(A) false)\n"
      "  Mov(Reg(A) Pseudo(\"%x.b0.v1\" 0) false)\n"
      "  Jump(\"L2\" [])\n"
      "  Label(\"L1\")\n"
      "  Mov(Data(\"in\" 3) Reg(A) false)\n"
      "  Mov(Reg(A) Pseudo(\"%x.b0.v2\" 0) false)\n"
      "  Label(\"L2\")\n"
      "  Phi(Pseudo(\"%x.b0.v3\" 0) [AsmPhiArg(\".f@asm_ssa_block@1\" Pseudo(\"%x.b0.v1\" 0)) AsmPhiArg(\"L1\" Pseudo(\"%x.b0.v2\" 0))])\n"
      "  Mov(Pseudo(\"%x.b0.v3\" 0) Reg(A) false)\n"
      "  Add(Pseudo(\"%y.b0.v1\" 0) Reg(A))\n"
      "  Mov(Reg(A) Data(\"out\" 0) false)\n"
      "  Mov(Pseudo(\"%x.b0.v3\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"out\" 1) false)\n"
      "  Mov(Pseudo(\"%y.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"out\" 2) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  Mov(Data(\"in\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Pseudo(\"%y.b0.v1\" 0) false)\n"
      "  Mov(Data(\"in\" 1) Reg(A) false)\n"
      "  Branch(EQ \"L1\")\n"
      "  Label(\".f@asm_ssa_block@1\")\n"
      "  Mov(Data(\"in\" 2) Reg(A) false)\n"
      "  Mov(Reg(A) ZP(128 0) false)\n"
      "  Jump(\"L2\" [])\n"
      "  Label(\"L1\")\n"
      "  Mov(Data(\"in\" 3) Reg(A) false)\n"
      "  Mov(Reg(A) ZP(128 0) false)\n"
      "  Label(\"L2\")\n"
      "  Phi(ZP(128 0) [AsmPhiArg(\".f@asm_ssa_block@1\" ZP(128 0)) AsmPhiArg(\"L1\" ZP(128 0))])\n"
      "  Mov(ZP(128 0) Reg(A) false)\n"
      "  Add(Pseudo(\"%y.b0.v1\" 0) Reg(A))\n"
      "  Mov(Reg(A) Data(\"out\" 0) false)\n"
      "  Mov(ZP(128 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"out\" 1) false)\n"
      "  Mov(Pseudo(\"%y.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"out\" 2) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("apply_coloring: f1 synthetic cases (3)") {
  check_stage_case(
      "apply_coloring",
      "{\"args\": [{\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": []}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": [[\"@1.r\", \"X\"]]}}}], \"kwargs\": {\"local_pool\": null}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  Mov(Imm(5) Pseudo(\"@1.r\" 0) false)\n"
      "  Label(\"L1\")\n"
      "  Dec(Pseudo(\"@1.r\" 0))\n"
      "  Mov(Pseudo(\"@1.r\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"out\" 0) false)\n"
      "  Branch(NE \"L1\")\n"
      "  Label(\".f@asm_ssa_block@1\")\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  Mov(Imm(5) Reg(X) false)\n"
      "  Label(\"L1\")\n"
      "  Dec(Reg(X))\n"
      "  Mov(Reg(X) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"out\" 0) false)\n"
      "  Branch(NE \"L1\")\n"
      "  Label(\".f@asm_ssa_block@1\")\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("apply_coloring: f1 synthetic cases (4)") {
  check_stage_case(
      "apply_coloring",
      "{\"args\": [{\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": []}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": [[\"%n\", \"X\"]]}}}], \"kwargs\": {\"local_pool\": null}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  Mov(Pseudo(\"%n\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Inc(Reg(X))\n"
      "  Dec(Reg(X))\n"
      "  Branch(EQ \"L1\")\n"
      "  Label(\".f@asm_ssa_block@1\")\n"
      "  Mov(Imm(1) Reg(A) false)\n"
      "  Label(\"L1\")\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) Data(\"out\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  Inc(Reg(X))\n"
      "  Dec(Reg(X))\n"
      "  Branch(EQ \"L1\")\n"
      "  Label(\".f@asm_ssa_block@1\")\n"
      "  Mov(Imm(1) Reg(A) false)\n"
      "  Label(\"L1\")\n"
      "  Add(Imm(1) Reg(A))\n"
      "  Mov(Reg(A) Data(\"out\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("apply_coloring: f1 synthetic cases (5)") {
  check_stage_case(
      "apply_coloring",
      "{\"args\": [{\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": []}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}], \"kwargs\": {\"local_pool\": null}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  Mov(Data(\"in\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"out\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  Mov(Data(\"in\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"out\" 0) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("apply_coloring: f1 synthetic cases (6)") {
  check_stage_case(
      "apply_coloring",
      "{\"args\": [{\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": []}, \"spilled\": {\"set\": [\"%x.b0.v1\", \"%x.b0.v2\", \"%x.b0.v3\", \"%y.b0.v1\"]}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}], \"kwargs\": {\"local_pool\": null}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  Mov(Data(\"in\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Pseudo(\"%y.b0.v1\" 0) false)\n"
      "  Mov(Data(\"in\" 1) Reg(A) false)\n"
      "  Branch(EQ \"L1\")\n"
      "  Label(\".f@asm_ssa_block@1\")\n"
      "  Mov(Data(\"in\" 2) Reg(A) false)\n"
      "  Mov(Reg(A) Pseudo(\"%x.b0.v1\" 0) false)\n"
      "  Jump(\"L2\" [])\n"
      "  Label(\"L1\")\n"
      "  Mov(Data(\"in\" 3) Reg(A) false)\n"
      "  Mov(Reg(A) Pseudo(\"%x.b0.v2\" 0) false)\n"
      "  Label(\"L2\")\n"
      "  Phi(Pseudo(\"%x.b0.v3\" 0) [AsmPhiArg(\".f@asm_ssa_block@1\" Pseudo(\"%x.b0.v1\" 0)) AsmPhiArg(\"L1\" Pseudo(\"%x.b0.v2\" 0))])\n"
      "  Mov(Pseudo(\"%x.b0.v3\" 0) Reg(A) false)\n"
      "  Add(Pseudo(\"%y.b0.v1\" 0) Reg(A))\n"
      "  Mov(Reg(A) Data(\"out\" 0) false)\n"
      "  Mov(Pseudo(\"%x.b0.v3\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"out\" 1) false)\n"
      "  Mov(Pseudo(\"%y.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"out\" 2) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@asm_ssa_block@0\")\n"
      "  Mov(Data(\"in\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Pseudo(\"%y.b0.v1\" 0) false)\n"
      "  Mov(Data(\"in\" 1) Reg(A) false)\n"
      "  Branch(EQ \"L1\")\n"
      "  Label(\".f@asm_ssa_block@1\")\n"
      "  Mov(Data(\"in\" 2) Reg(A) false)\n"
      "  Mov(Reg(A) Pseudo(\"%x.b0.v1\" 0) false)\n"
      "  Jump(\"L2\" [])\n"
      "  Label(\"L1\")\n"
      "  Mov(Data(\"in\" 3) Reg(A) false)\n"
      "  Mov(Reg(A) Pseudo(\"%x.b0.v2\" 0) false)\n"
      "  Label(\"L2\")\n"
      "  Phi(Pseudo(\"%x.b0.v3\" 0) [AsmPhiArg(\".f@asm_ssa_block@1\" Pseudo(\"%x.b0.v1\" 0)) AsmPhiArg(\"L1\" Pseudo(\"%x.b0.v2\" 0))])\n"
      "  Mov(Pseudo(\"%x.b0.v3\" 0) Reg(A) false)\n"
      "  Add(Pseudo(\"%y.b0.v1\" 0) Reg(A))\n"
      "  Mov(Reg(A) Data(\"out\" 0) false)\n"
      "  Mov(Pseudo(\"%x.b0.v3\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"out\" 1) false)\n"
      "  Mov(Pseudo(\"%y.b0.v1\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Data(\"out\" 2) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("apply_coloring: f1 synthetic cases (7)") {
  check_stage_case(
      "apply_coloring",
      "{\"args\": [{\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"%z\", 144]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": [[\"%p\", \"Y\"]]}}}], \"kwargs\": {\"local_pool\": [144, 145]}}",
      "Function(\"f\" true [] [\n"
      "  Mov(Pseudo(\"%p\" 0) Reg(A) false)\n"
      "  Mov(Reg(A) Reg(X) false)\n"
      "  Mov(IndexedData(\"arr\" 0 X) Reg(A) false)\n"
      "  Xor(Reg(A) Imm(1) Reg(A))\n"
      "  Pop(Reg(A))\n"
      "  LoadAddress(Pseudo(\"@t\" 0) Pseudo(\"%z\" 0))\n"
      "  Mov(IndexedData(\"arr\" 1 X) Reg(A) false)\n"
      "  Label(\"L1\")\n"
      "  Mov(IndexedData(\"arr\" 2 X) Reg(A) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Mov(IndexedData(\"arr\" 0 Y) Reg(A) false)\n"
      "  Xor(Reg(A) Imm(1) Reg(A))\n"
      "  Pop(Reg(A))\n"
      "  LoadAddress(Pseudo(\"@t\" 0) Data(\"__local_f__0\" 0))\n"
      "  Mov(IndexedData(\"arr\" 1 Y) Reg(A) false)\n"
      "  Label(\"L1\")\n"
      "  Mov(IndexedData(\"arr\" 2 X) Reg(A) false)\n"
      "  Return(false false false)\n"
      "])\n",
      "");
}

TEST_CASE("compute_address_taken_bytes: f1 synthetic cases (1)") {
  check_stage_case(
      "compute_address_taken_bytes",
      "{\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@1.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.loc\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, null], \"kwargs\": {}}",
      "Program([\n"
      "  StaticVariable(\"g\" true [CharInit(0)])\n"
      "  Function(\"f\" true [\"@1.a\"] [\n"
      "    LoadAddress(Pseudo(\"@1.a\" 0) Pseudo(\"%p\" 0))\n"
      "    LoadAddress(Pseudo(\"g\" 0) Pseudo(\"%q\" 0))\n"
      "    LoadAddress(Pseudo(\"@2.loc\" 0) Pseudo(\"%r\" 0))\n"
      "    LoadAddress(Pseudo(\"@2.loc\" 0) Pseudo(\"%s\" 0))\n"
      "    Phi(ZP(128 0) [AsmPhiArg(\"L1\" ZP(129 1)) AsmPhiArg(\"L2\" ZP(131 0))])\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"f\", 4]]}",
      "");
}

TEST_CASE("compute_address_taken_bytes: f1 synthetic cases (2)") {
  check_stage_case(
      "compute_address_taken_bytes",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  StaticVariable(\"g\" true [CharInit(0)])\n"
      "  Function(\"f\" true [\"@1.a\"] [\n"
      "    LoadAddress(Pseudo(\"@1.a\" 0) Pseudo(\"%p\" 0))\n"
      "    LoadAddress(Pseudo(\"g\" 0) Pseudo(\"%q\" 0))\n"
      "    LoadAddress(Pseudo(\"@2.loc\" 0) Pseudo(\"%r\" 0))\n"
      "    LoadAddress(Pseudo(\"@2.loc\" 0) Pseudo(\"%s\" 0))\n"
      "    Phi(ZP(128 0) [AsmPhiArg(\"L1\" ZP(129 1)) AsmPhiArg(\"L2\" ZP(131 0))])\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"f\", 0]]}",
      "");
}

TEST_CASE("compute_local_bytes: f1 synthetic cases (1)") {
  check_stage_case(
      "compute_local_bytes",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  StaticVariable(\"g\" true [CharInit(0)])\n"
      "  Function(\"f\" true [\"@1.a\"] [\n"
      "    LoadAddress(Pseudo(\"@1.a\" 0) Pseudo(\"%p\" 0))\n"
      "    LoadAddress(Pseudo(\"g\" 0) Pseudo(\"%q\" 0))\n"
      "    LoadAddress(Pseudo(\"@2.loc\" 0) Pseudo(\"%r\" 0))\n"
      "    LoadAddress(Pseudo(\"@2.loc\" 0) Pseudo(\"%s\" 0))\n"
      "    Phi(ZP(128 0) [AsmPhiArg(\"L1\" ZP(129 1)) AsmPhiArg(\"L2\" ZP(131 0))])\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"f\", 3]]}",
      "");
}

TEST_CASE("dead_static_elimination: f1 synthetic cases (1)") {
  check_stage_case(
      "dead_static_elimination",
      "{\"args\": [], \"kwargs\": {}}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Label(\".f@asm_ssa_block@0\")\n"
      "    Mov(Data(\"in\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"out\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Label(\".f@asm_ssa_block@0\")\n"
      "    Mov(Data(\"in\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"out\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "");
}

TEST_CASE("fold_const_statics: f1 synthetic cases (1)") {
  check_stage_case(
      "fold_const_statics",
      "{\"args\": [], \"kwargs\": {\"symbols\": null}}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"in\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Pseudo(\"%a\" 0) false)\n"
      "    Mov(Pseudo(\"%a\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"out\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"in\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Pseudo(\"%a\" 0) false)\n"
      "    Mov(Pseudo(\"%a\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"out\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "");
}

TEST_CASE("fold_const_statics: f1 synthetic cases (2)") {
  check_stage_case(
      "fold_const_statics",
      "{\"args\": [], \"kwargs\": {\"symbols\": null}}",
      "Program([\n"
      "  StaticVariable(\"@k8\" false [CharInit(300)])\n"
      "  StaticVariable(\"@k32\" false [LongInit(-2)])\n"
      "  StaticVariable(\"@k64\" false [LongLongInit(18446744073709551615)])\n"
      "  StaticVariable(\"@kf\" false [FloatInit(1065353216)])\n"
      "  StaticVariable(\"@kd\" false [DoubleInit(4607182418800017408)])\n"
      "  StaticVariable(\"@kp\" false [AddressInit(\"msg\" 2)])\n"
      "  StaticVariable(\"@ka\" false [CharInit(1)])\n"
      "  StaticVariable(\"@kl\" false [CharInit(2)])\n"
      "  StaticVariable(\"@kw\" false [CharInit(3)])\n"
      "  StaticVariable(\"@kdw\" false [CharInit(4)])\n"
      "  StaticVariable(\"@ki\" false [CharInit(5)])\n"
      "  StaticVariable(\"@kv\" false [CharInit(6)])\n"
      "  StaticVariable(\"@kz\" false [ZeroInit(2)])\n"
      "  StaticVariable(\"@nosym\" false [CharInit(7)])\n"
      "  StaticVariable(\"msg\" true [StringInit(\"hi\" 3) AddressInit(\"@ka\" 0)])\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Pseudo(\"@k8\" 0) Reg(A) false)\n"
      "    Add(Data(\"@k32\" 3) Reg(A))\n"
      "    Compare(Reg(A) Pseudo(\"@k64\" 7))\n"
      "    And(Pseudo(\"@kf\" 3) Reg(A))\n"
      "    DecimalArith(false [Pseudo(\"@kd\" 7)] [Pseudo(\"@k8\" 0)] [Pseudo(\"%t\" 0)])\n"
      "    Mov(Pseudo(\"@kp\" 0) Reg(A) false)\n"
      "    Or(Pseudo(\"@kp\" 1) Reg(A))\n"
      "    Mov(Pseudo(\"@k8\" 5) Reg(A) false)\n"
      "    LoadAddress(Pseudo(\"@kl\" 0) Pseudo(\"%p\" 0))\n"
      "    Mov(Reg(A) Pseudo(\"@kw\" 0) false)\n"
      "    DecimalArith(false [Pseudo(\"%t\" 0)] [Imm(1)] [Pseudo(\"@kdw\" 0)])\n"
      "    Mov(IndexedData(\"@ki\" 0 X) Reg(A) false)\n"
      "    Mov(Pseudo(\"@kv\" 0) Reg(A) false)\n"
      "    Mov(Pseudo(\"@kz\" 0) Reg(A) false)\n"
      "    Mov(Pseudo(\"@nosym\" 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  StaticVariable(\"@k8\" false [CharInit(300)])\n"
      "  StaticVariable(\"@k32\" false [LongInit(-2)])\n"
      "  StaticVariable(\"@k64\" false [LongLongInit(18446744073709551615)])\n"
      "  StaticVariable(\"@kf\" false [FloatInit(1065353216)])\n"
      "  StaticVariable(\"@kd\" false [DoubleInit(4607182418800017408)])\n"
      "  StaticVariable(\"@kp\" false [AddressInit(\"msg\" 2)])\n"
      "  StaticVariable(\"@ka\" false [CharInit(1)])\n"
      "  StaticVariable(\"@kl\" false [CharInit(2)])\n"
      "  StaticVariable(\"@kw\" false [CharInit(3)])\n"
      "  StaticVariable(\"@kdw\" false [CharInit(4)])\n"
      "  StaticVariable(\"@ki\" false [CharInit(5)])\n"
      "  StaticVariable(\"@kv\" false [CharInit(6)])\n"
      "  StaticVariable(\"@kz\" false [ZeroInit(2)])\n"
      "  StaticVariable(\"@nosym\" false [CharInit(7)])\n"
      "  StaticVariable(\"msg\" true [StringInit(\"hi\" 3) AddressInit(\"@ka\" 0)])\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Pseudo(\"@k8\" 0) Reg(A) false)\n"
      "    Add(Data(\"@k32\" 3) Reg(A))\n"
      "    Compare(Reg(A) Pseudo(\"@k64\" 7))\n"
      "    And(Pseudo(\"@kf\" 3) Reg(A))\n"
      "    DecimalArith(false [Pseudo(\"@kd\" 7)] [Pseudo(\"@k8\" 0)] [Pseudo(\"%t\" 0)])\n"
      "    Mov(Pseudo(\"@kp\" 0) Reg(A) false)\n"
      "    Or(Pseudo(\"@kp\" 1) Reg(A))\n"
      "    Mov(Pseudo(\"@k8\" 5) Reg(A) false)\n"
      "    LoadAddress(Pseudo(\"@kl\" 0) Pseudo(\"%p\" 0))\n"
      "    Mov(Reg(A) Pseudo(\"@kw\" 0) false)\n"
      "    DecimalArith(false [Pseudo(\"%t\" 0)] [Imm(1)] [Pseudo(\"@kdw\" 0)])\n"
      "    Mov(IndexedData(\"@ki\" 0 X) Reg(A) false)\n"
      "    Mov(Pseudo(\"@kv\" 0) Reg(A) false)\n"
      "    Mov(Pseudo(\"@kz\" 0) Reg(A) false)\n"
      "    Mov(Pseudo(\"@nosym\" 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n",
      "");
}

TEST_CASE("fold_const_statics: f1 synthetic cases (3)") {
  check_stage_case(
      "fold_const_statics",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@k8\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@k32\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@k64\", {\"object\": \"passes.type_checking.Symbo"
      "l\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(ULongLong)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@kf\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Float)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@kd\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Double)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\""
      "object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@kp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Pointer(UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@ka\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\""
      ": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@kl\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@kw\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@kdw\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"d"
      "ump\": \"Const(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@ki\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@kv\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Volatile(Const(UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\","
      " \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@kz\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"msg\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": fa"
      "lse}}}}]]}}}}}",
      "Program([\n"
      "  StaticVariable(\"@k8\" false [CharInit(300)])\n"
      "  StaticVariable(\"@k32\" false [LongInit(-2)])\n"
      "  StaticVariable(\"@k64\" false [LongLongInit(18446744073709551615)])\n"
      "  StaticVariable(\"@kf\" false [FloatInit(1065353216)])\n"
      "  StaticVariable(\"@kd\" false [DoubleInit(4607182418800017408)])\n"
      "  StaticVariable(\"@kp\" false [AddressInit(\"msg\" 2)])\n"
      "  StaticVariable(\"@ka\" false [CharInit(1)])\n"
      "  StaticVariable(\"@kl\" false [CharInit(2)])\n"
      "  StaticVariable(\"@kw\" false [CharInit(3)])\n"
      "  StaticVariable(\"@kdw\" false [CharInit(4)])\n"
      "  StaticVariable(\"@ki\" false [CharInit(5)])\n"
      "  StaticVariable(\"@kv\" false [CharInit(6)])\n"
      "  StaticVariable(\"@kz\" false [ZeroInit(2)])\n"
      "  StaticVariable(\"@nosym\" false [CharInit(7)])\n"
      "  StaticVariable(\"msg\" true [StringInit(\"hi\" 3) AddressInit(\"@ka\" 0)])\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Pseudo(\"@k8\" 0) Reg(A) false)\n"
      "    Add(Data(\"@k32\" 3) Reg(A))\n"
      "    Compare(Reg(A) Pseudo(\"@k64\" 7))\n"
      "    And(Pseudo(\"@kf\" 3) Reg(A))\n"
      "    DecimalArith(false [Pseudo(\"@kd\" 7)] [Pseudo(\"@k8\" 0)] [Pseudo(\"%t\" 0)])\n"
      "    Mov(Pseudo(\"@kp\" 0) Reg(A) false)\n"
      "    Or(Pseudo(\"@kp\" 1) Reg(A))\n"
      "    Mov(Pseudo(\"@k8\" 5) Reg(A) false)\n"
      "    LoadAddress(Pseudo(\"@kl\" 0) Pseudo(\"%p\" 0))\n"
      "    Mov(Reg(A) Pseudo(\"@kw\" 0) false)\n"
      "    DecimalArith(false [Pseudo(\"%t\" 0)] [Imm(1)] [Pseudo(\"@kdw\" 0)])\n"
      "    Mov(IndexedData(\"@ki\" 0 X) Reg(A) false)\n"
      "    Mov(Pseudo(\"@kv\" 0) Reg(A) false)\n"
      "    Mov(Pseudo(\"@kz\" 0) Reg(A) false)\n"
      "    Mov(Pseudo(\"@nosym\" 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  StaticVariable(\"@k8\" false [CharInit(300)])\n"
      "  StaticVariable(\"@kp\" false [AddressInit(\"msg\" 2)])\n"
      "  StaticVariable(\"@ka\" false [CharInit(1)])\n"
      "  StaticVariable(\"@kl\" false [CharInit(2)])\n"
      "  StaticVariable(\"@kw\" false [CharInit(3)])\n"
      "  StaticVariable(\"@kdw\" false [CharInit(4)])\n"
      "  StaticVariable(\"@ki\" false [CharInit(5)])\n"
      "  StaticVariable(\"@kv\" false [CharInit(6)])\n"
      "  StaticVariable(\"@kz\" false [ZeroInit(2)])\n"
      "  StaticVariable(\"@nosym\" false [CharInit(7)])\n"
      "  StaticVariable(\"msg\" true [StringInit(\"hi\" 3) AddressInit(\"@ka\" 0)])\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(44) Reg(A) false)\n"
      "    Add(Imm(255) Reg(A))\n"
      "    Compare(Reg(A) Imm(255))\n"
      "    And(Imm(63) Reg(A))\n"
      "    DecimalArith(false [Imm(63)] [Imm(44)] [Pseudo(\"%t\" 0)])\n"
      "    Mov(ImmLabelLow(\"msg\" 2) Reg(A) false)\n"
      "    Or(Pseudo(\"@kp\" 1) Reg(A))\n"
      "    Mov(Pseudo(\"@k8\" 5) Reg(A) false)\n"
      "    LoadAddress(Pseudo(\"@kl\" 0) Pseudo(\"%p\" 0))\n"
      "    Mov(Reg(A) Pseudo(\"@kw\" 0) false)\n"
      "    DecimalArith(false [Pseudo(\"%t\" 0)] [Imm(1)] [Pseudo(\"@kdw\" 0)])\n"
      "    Mov(IndexedData(\"@ki\" 0 X) Reg(A) false)\n"
      "    Mov(Pseudo(\"@kv\" 0) Reg(A) false)\n"
      "    Mov(Pseudo(\"@kz\" 0) Reg(A) false)\n"
      "    Mov(Pseudo(\"@nosym\" 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n",
      "");
}

TEST_CASE("apply_coloring: f1 synthetic, a pseudo pinned to Y at a nonzero offset") {
  check_stage_call(
      "apply_coloring",
      "{\"args\": [{\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": [[\"%z\", 144]]}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": [[\"%p\", \"Y\"]]}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Function(\"f\" true [] [\n"
      "  Inc(Pseudo(\"%p\" 1))\n"
      "])\n",
      "exception: AssertionError: HwReg-colored Pseudo %p has nonzero offset 1; eligibility scan should have rejected it\n",
      "");
}
