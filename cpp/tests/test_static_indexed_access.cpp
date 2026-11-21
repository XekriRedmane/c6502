// The recorded calls of `tac_static_indexed_access`, the C++ port of
// passes/optimization/static_indexed_access.py, with what the Python
// pass returned. They were recorded with the capture harness's wrappers
// while tests/test_static_indexed_access.py ran, through the TAC
// optimizer's driver and through the Python entry points that the tests
// call directly. They are the calls that changed the function or the
// symbols, and the calls of small synthetic functions through those
// entry points.
#include "doctest.h"
#include "stage_case.h"

TEST_CASE("tac_static_indexed_access: test_static_indexed_access.TestStaticIndexedAccess.test_member_reads (call 1)") {
  c6502::testing::check_stage_case(
      "tac_static_indexed_access",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Structure(\\\"rec\\\") 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [{\"tuple\": [1, 2, 3, 4, 5]}, 1000, 7]}, {\"tuple\": [{\"tuple\": [6, 7, 8, 9, 10]}, 2000, 8]}, {\"tuple\": [{\"tuple\": [11, 12, 13, 14, 15]}, 3000, 9]}, {\"tuple\": [{\"tuple\": [16, 17, 18, 19, 20]}, 4000, 10]}]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UInt)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%8.1\", \"%9.1\", \"@1.i\"]}}}",
      "Function(\"w\" true [\"@1.i\"] [\n"
      "  GetAddress(Var(\"r\") Var(\"%8.1\"))\n"
      "  Binary(LeftShift Var(\"@1.i\") Constant(ConstInt(3)) Var(\"%11.1\"))\n"
      "  ZeroExtend(Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Binary(Add Var(\"%8.1\") Var(\"%12.1\") Var(\"%13.1\"))\n"
      "  Binary(Add Var(\"%13.1\") Constant(ConstInt(5)) Var(\"%14.1\"))\n"
      "  Load(Var(\"%14.1\") Var(\"%15.1\") false)\n"
      "  Ret(Var(\"%15.1\"))\n"
      "])\n",
      "Function(\"w\" true [\"@1.i\"] [\n"
      "  GetAddress(Var(\"r\") Var(\"%8.1\"))\n"
      "  Binary(LeftShift Var(\"@1.i\") Constant(ConstInt(3)) Var(\"%11.1\"))\n"
      "  ZeroExtend(Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Binary(Add Var(\"%8.1\") Var(\"%12.1\") Var(\"%13.1\"))\n"
      "  Binary(Add Var(\"%8.1\") Constant(ConstInt(5)) Var(\"%14.1.base\"))\n"
      "  Binary(Add Var(\"%14.1.base\") Var(\"%12.1\") Var(\"%14.1\"))\n"
      "  IndexedLoad(\"r\" Var(\"%11.1\") Var(\"%15.1\") false 5 0)\n"
      "  Ret(Var(\"%15.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Structure(\\\"rec\\\") 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [{\"tuple\": [1, 2, 3, 4, 5]}, 1000, 7]}, {\"tuple\": [{\"tuple\": [6, 7, 8, 9, 10]}, 2000, 8]}, {\"tuple\": [{\"tuple\": [11, 12, 13, 14, 15]}, 3000, 9]}, {\"tuple\": [{\"tuple\": [16, 17, 18, 19, 20]}, 4000, 10]}]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UInt)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14.1.base\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UInt)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%14.1.base\", \"%15.1\", \"%8.1\", \"%9.1\", \"@1.i\"]}}}");
}

TEST_CASE("tac_static_indexed_access: test_static_indexed_access.TestStaticIndexedAccess.test_address_of_member (call 1)") {
  c6502::testing::check_stage_case(
      "tac_static_indexed_access",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Structure(\\\"rec\\\") 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [{\"tuple\": [1, 2, 3, 4, 5]}, 1000, 7]}, {\"tuple\": [{\"tuple\": [6, 7, 8, 9, 10]}, 2000, 8]}, {\"tuple\": [{\"tuple\": [11, 12, 13, 14, 15]}, 3000, 9]}, {\"tuple\": [{\"tuple\": [16, 17, 18, 19, 20]}, 4000, 10]}]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%2.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@1.s.1\", \"@1.s.2\", \"@1.s.3\", \"@2.i.1\", \"@2.i.2\", \"@2.i.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@1.s.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%16.1\"))])\n"
      "  Phi(Var(\"@2.i.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%18.1\"))])\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@2.i.2\") Constant(ConstUChar(4)) \".loop@0_break\")\n"
      "  GetAddress(Var(\"r\") Var(\"%5.1\"))\n"
      "  Binary(LeftShift Var(\"@2.i.2\") Constant(ConstInt(3)) Var(\"%8.1\"))\n"
      "  ZeroExtend(Var(\"%8.1\") Var(\"%9.1\"))\n"
      "  Binary(Add Var(\"%5.1\") Var(\"%9.1\") Var(\"%10.1\"))\n"
      "  Binary(Add Var(\"%10.1\") Constant(ConstInt(7)) Var(\"%11.1\"))\n"
      "  FunctionCall(\"deref\" [Var(\"%11.1\")] Var(\"%12.1\") false false)\n"
      "  Binary(Add Var(\"@1.s.2\") Var(\"%12.1\") Var(\"%16.1\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@2.i.2\") Constant(ConstUChar(1)) Var(\"%18.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  ZeroExtend(Var(\"@1.s.2\") Var(\"%19.1\"))\n"
      "  GetAddress(Var(\"r\") Var(\"%20.1\"))\n"
      "  Binary(Add Constant(ConstInt(19)) Var(\"%20.1\") Var(\"%23.1\"))\n"
      "  FunctionCall(\"deref\" [Var(\"%23.1\")] Var(\"%24.1\") false false)\n"
      "  ZeroExtend(Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  Binary(Add Var(\"%19.1\") Var(\"%25.1\") Var(\"%26.1\"))\n"
      "  Ret(Var(\"%26.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@1.s.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%16.1\"))])\n"
      "  Phi(Var(\"@2.i.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%18.1\"))])\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@2.i.2\") Constant(ConstUChar(4)) \".loop@0_break\")\n"
      "  GetAddress(Var(\"r\") Var(\"%5.1\"))\n"
      "  Binary(LeftShift Var(\"@2.i.2\") Constant(ConstInt(3)) Var(\"%8.1\"))\n"
      "  ZeroExtend(Var(\"%8.1\") Var(\"%9.1\"))\n"
      "  Binary(Add Var(\"%5.1\") Var(\"%9.1\") Var(\"%10.1\"))\n"
      "  Binary(Add Var(\"%5.1\") Constant(ConstInt(7)) Var(\"%11.1.base\"))\n"
      "  Binary(Add Var(\"%11.1.base\") Var(\"%9.1\") Var(\"%11.1\"))\n"
      "  FunctionCall(\"deref\" [Var(\"%11.1\")] Var(\"%12.1\") false false)\n"
      "  Binary(Add Var(\"@1.s.2\") Var(\"%12.1\") Var(\"%16.1\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@2.i.2\") Constant(ConstUChar(1)) Var(\"%18.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  ZeroExtend(Var(\"@1.s.2\") Var(\"%19.1\"))\n"
      "  GetAddress(Var(\"r\") Var(\"%20.1\"))\n"
      "  Binary(Add Constant(ConstInt(19)) Var(\"%20.1\") Var(\"%23.1\"))\n"
      "  FunctionCall(\"deref\" [Var(\"%23.1\")] Var(\"%24.1\") false false)\n"
      "  ZeroExtend(Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  Binary(Add Var(\"%19.1\") Var(\"%25.1\") Var(\"%26.1\"))\n"
      "  Ret(Var(\"%26.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Structure(\\\"rec\\\") 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [{\"tuple\": [1, 2, 3, 4, 5]}, 1000, 7]}, {\"tuple\": [{\"tuple\": [6, 7, 8, 9, 10]}, 2000, 8]}, {\"tuple\": [{\"tuple\": [11, 12, 13, 14, 15]}, 3000, 9]}, {\"tuple\": [{\"tuple\": [16, 17, 18, 19, 20]}, 4000, 10]}]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1.base\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"%10.1\", \"%11.1\", \"%11.1.base\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%2.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@1.s.1\", \"@1.s.2\", \"@1.s.3\", \"@2.i.1\", \"@2.i.2\", \"@2.i.3\"]}}}");
}

TEST_CASE("tac_static_indexed_access: test_static_indexed_access.TestStaticIndexedAccess.test_member_writes (call 1)") {
  c6502::testing::check_stage_case(
      "tac_static_indexed_access",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Structure(\\\"rec\\\") 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [{\"tuple\": [1, 2, 3, 4, 5]}, 1000, 7]}, {\"tuple\": [{\"tuple\": [6, 7, 8, 9, 10]}, 2000, 8]}, {\"tuple\": [{\"tuple\": [11, 12, 13, 14, 15]}, 3000, 9]}, {\"tuple\": [{\"tuple\": [16, 17, 18, 19, 20]}, 4000, 10]}]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UInt)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.i\", \"@1.v\", \"@2.x\"]}}}",
      "Function(\"put\" true [\"@0.i\" \"@1.v\" \"@2.x\"] [\n"
      "  GetAddress(Var(\"r\") Var(\"%0.1\"))\n"
      "  Binary(LeftShift Var(\"@0.i\") Constant(ConstInt(3)) Var(\"%3.1\"))\n"
      "  ZeroExtend(Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Binary(Add Var(\"%0.1\") Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  Binary(Add Var(\"%5.1\") Constant(ConstInt(7)) Var(\"%6.1\"))\n"
      "  Store(Var(\"@1.v\") Var(\"%6.1\") false)\n"
      "  GetAddress(Var(\"r\") Var(\"%7.1\"))\n"
      "  Binary(Add Var(\"%7.1\") Var(\"%4.1\") Var(\"%12.1\"))\n"
      "  Binary(Add Var(\"%12.1\") Constant(ConstInt(5)) Var(\"%13.1\"))\n"
      "  Store(Var(\"@2.x\") Var(\"%13.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"put\" true [\"@0.i\" \"@1.v\" \"@2.x\"] [\n"
      "  GetAddress(Var(\"r\") Var(\"%0.1\"))\n"
      "  Binary(LeftShift Var(\"@0.i\") Constant(ConstInt(3)) Var(\"%3.1\"))\n"
      "  ZeroExtend(Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Binary(Add Var(\"%0.1\") Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  Binary(Add Var(\"%0.1\") Constant(ConstInt(7)) Var(\"%6.1.base\"))\n"
      "  Binary(Add Var(\"%6.1.base\") Var(\"%4.1\") Var(\"%6.1\"))\n"
      "  IndexedSymbolStore(\"r\" Var(\"%3.1\") Var(\"@1.v\") false 7)\n"
      "  GetAddress(Var(\"r\") Var(\"%7.1\"))\n"
      "  Binary(Add Var(\"%7.1\") Var(\"%4.1\") Var(\"%12.1\"))\n"
      "  Binary(Add Var(\"%7.1\") Constant(ConstInt(5)) Var(\"%13.1.base\"))\n"
      "  Binary(Add Var(\"%13.1.base\") Var(\"%4.1\") Var(\"%13.1\"))\n"
      "  IndexedSymbolStore(\"r\" Var(\"%3.1\") Var(\"@2.x\") false 5)\n"
      "  Ret(None)\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Structure(\\\"rec\\\") 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [{\"tuple\": [1, 2, 3, 4, 5]}, 1000, 7]}, {\"tuple\": [{\"tuple\": [6, 7, 8, 9, 10]}, 2000, 8]}, {\"tuple\": [{\"tuple\": [11, 12, 13, 14, 15]}, 3000, 9]}, {\"tuple\": [{\"tuple\": [16, 17, 18, 19, 20]}, 4000, 10]}]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UInt)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1.base\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13.1.base\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UInt)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%13.1.base\", \"%2.1\""
      ", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%6.1.base\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.i\", \"@1.v\", \"@2.x\"]}}}");
}

TEST_CASE("tac_static_indexed_access: test_static_indexed_access.TestStaticIndexedAccess.test_member_reads (call 2)") {
  c6502::testing::check_stage_case(
      "tac_static_indexed_access",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Structure(\\\"rec\\\") 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [{\"tuple\": [1, 2, 3, 4, 5]}, 1000, 7]}, {\"tuple\": [{\"tuple\": [6, 7, 8, 9, 10]}, 2000, 8]}, {\"tuple\": [{\"tuple\": [11, 12, 13, 14, 15]}, 3000, 9]}, {\"tuple\": [{\"tuple\": [16, 17, 18, 19, 20]}, 4000, 10]}]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"@0.i\"]}}}",
      "Function(\"a\" true [\"@0.i\"] [\n"
      "  GetAddress(Var(\"r\") Var(\"%0.1\"))\n"
      "  Binary(LeftShift Var(\"@0.i\") Constant(ConstInt(3)) Var(\"%3.1\"))\n"
      "  ZeroExtend(Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Binary(Add Var(\"%0.1\") Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  Binary(Add Var(\"%5.1\") Constant(ConstInt(7)) Var(\"%6.1\"))\n"
      "  Load(Var(\"%6.1\") Var(\"%7.1\") false)\n"
      "  Ret(Var(\"%7.1\"))\n"
      "])\n",
      "Function(\"a\" true [\"@0.i\"] [\n"
      "  GetAddress(Var(\"r\") Var(\"%0.1\"))\n"
      "  Binary(LeftShift Var(\"@0.i\") Constant(ConstInt(3)) Var(\"%3.1\"))\n"
      "  ZeroExtend(Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Binary(Add Var(\"%0.1\") Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  Binary(Add Var(\"%0.1\") Constant(ConstInt(7)) Var(\"%6.1.base\"))\n"
      "  Binary(Add Var(\"%6.1.base\") Var(\"%4.1\") Var(\"%6.1\"))\n"
      "  IndexedLoad(\"r\" Var(\"%3.1\") Var(\"%7.1\") false 7 0)\n"
      "  Ret(Var(\"%7.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Structure(\\\"rec\\\") 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [{\"tuple\": [1, 2, 3, 4, 5]}, 1000, 7]}, {\"tuple\": [{\"tuple\": [6, 7, 8, 9, 10]}, 2000, 8]}, {\"tuple\": [{\"tuple\": [11, 12, 13, 14, 15]}, 3000, 9]}, {\"tuple\": [{\"tuple\": [16, 17, 18, 19, 20]}, 4000, 10]}]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1.base\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%6.1.base\", \"%7.1\", \"@0.i\"]}}}");
}
