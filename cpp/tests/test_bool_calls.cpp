// The calls that the tests of tests/test_bool.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 and G7). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_bool.py::TestErasure::test_no_bool_reaches_the_back_end (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestErasure::test_no_bool_reaches_the_back_end\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s { bool flag; bool *p; };bool g[3]; struct s gs;bool f(bool (*fp)(bool), bool b) { bool t = !b; return fp(t); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"flag\" Bool) member_decl(\"p\" Pointer(Bool))]))\n"
      "  VarDecl(var_decl(\"g\" None Array(Bool 3) None None None None false))\n"
      "  VarDecl(var_decl(\"gs\" None Structure(\"s\") None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"fp\" \"b\"] Block([\n"
      "    D(VarDecl(var_decl(\"t\" Unary(LogicalNot Var(\"b\" None) None) Bool None None None None false)))\n"
      "    S(Return(FunctionCall(\"fp\" [Var(\"t\" None)] None false)))\n"
      "  ]) FunType([Pointer(FunType([Bool] Bool)) Bool] Bool) None None [\"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestErasure::test_no_bool_reaches_the_back_end (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestErasure::test_no_bool_reaches_the_back_end\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"flag\" Bool) member_decl(\"p\" Pointer(Bool))]))\n"
      "  VarDecl(var_decl(\"g\" None Array(Bool 3) None None None None false))\n"
      "  VarDecl(var_decl(\"gs\" None Structure(\"s\") None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"fp\" \"b\"] Block([\n"
      "    D(VarDecl(var_decl(\"t\" Unary(LogicalNot Var(\"b\" None) None) Bool None None None None false)))\n"
      "    S(Return(FunctionCall(\"fp\" [Var(\"t\" None)] None false)))\n"
      "  ]) FunType([Pointer(FunType([Bool] Bool)) Bool] Bool) None None [\"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"flag\" Bool) member_decl(\"p\" Pointer(Bool))]))\n"
      "  VarDecl(var_decl(\"g\" None Array(Bool 3) None None None None false))\n"
      "  VarDecl(var_decl(\"gs\" None Structure(\"s\") None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.fp\" \"@1.b\"] Block([\n"
      "    D(VarDecl(var_decl(\"@2.t\" Unary(LogicalNot Var(\"@1.b\" None) None) Bool None None None None false)))\n"
      "    S(Return(FunctionCall(\"@0.fp\" [Var(\"@2.t\" None)] None false)))\n"
      "  ]) FunType([Pointer(FunType([Bool] Bool)) Bool] Bool) None None [\"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestErasure::test_no_bool_reaches_the_back_end (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestErasure::test_no_bool_reaches_the_back_end\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"flag\\\" Bool) member_decl(\\\"p\\\" Pointer(Bool))]))\\n  VarDecl(var_decl(\\\"g\\\" None Array(Bool 3) None None None None false))\\n  VarDecl(var_decl(\\\"gs\\\" None Structure(\\\"s\\\") None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.fp\\\" \\\"@1.b\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@2.t\\\" Cast(Bool Unary(LogicalNot Var(\\\"@1.b\\\" Bool) Int) Bool) Bool None None None None false)))\\n    S(Return(FunctionCall(\\\"@0.fp\\\" [Var(\\\"@2.t\\\" Bool)] Bool false)))\\n  ]) FunType([Pointer(FunType([Bool] Bool)) Bool] Bool) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"flag\" Bool) member_decl(\"p\" Pointer(Bool))]))\n"
      "  VarDecl(var_decl(\"g\" None Array(Bool 3) None None None None false))\n"
      "  VarDecl(var_decl(\"gs\" None Structure(\"s\") None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.fp\" \"@1.b\"] Block([\n"
      "    D(VarDecl(var_decl(\"@2.t\" Unary(LogicalNot Var(\"@1.b\" None) None) Bool None None None None false)))\n"
      "    S(Return(FunctionCall(\"@0.fp\" [Var(\"@2.t\" None)] None false)))\n"
      "  ]) FunType([Pointer(FunType([Bool] Bool)) Bool] Bool) None None [\"\" \"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"flag\\\" Bool) member_decl(\\\"p\\\" Pointer(Bool))]))\\n  VarDecl(var_decl(\\\"g\\\" None Array(Bool 3) None None None None false))\\n  VarDecl(var_decl(\\\"gs\\\" None Structure(\\\"s\\\") None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.fp\\\" \\\"@1.b\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@2.t\\\" Cast(Bool Unary(LogicalNot Var(\\\"@1.b\\\" Bool) Int) Bool) Bool None None None None false)))\\n    S(Return(FunctionCall(\\\"@0.fp\\\" [Var(\\\"@2.t\\\" Bool)] Bool false)))\\n  ]) FunType([Pointer(FunType([Bool] Bool)) Bool] Bool) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Bool 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"gs\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Structure(\\\"s\\\")\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(FunType([Bool] Bool)) Bool] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.fp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([Bool] Bool))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"flag\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"p\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Bool)\\n\"}, \"byte_offset\": 1}}], \"size\": 3, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("c99_to_tac: test_bool.py::TestErasure::test_no_bool_reaches_the_back_end (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_bool.py::TestErasure::test_no_bool_reaches_the_back_end\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Bool 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"gs\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Structure(\\\"s\\\")\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(FunType([Bool] Bool)) Bool] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.fp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([Bool] Bool))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"flag\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"p\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Bool)\\n\"}, \"byte_offset\": 1}}], \"size\": 3, \"complete\": true}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"flag\\\" Bool) member_decl(\\\"p\\\" Pointer(Bool))]))\\n  VarDecl(var_decl(\\\"g\\\" None Array(UChar 3) None None None None false))\\n  VarDecl(var_decl(\\\"gs\\\" None Structure(\\\"s\\\") None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.fp\\\" \\\"@1.b\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@2.t\\\" Cast(Bool Unary(LogicalNot Var(\\\"@1.b\\\" Bool) Int) Bool) Bool None None None None false)))\\n    S(Return(FunctionCall(\\\"@0.fp\\\" [Var(\\\"@2.t\\\" Bool)] Bool false)))\\n  ]) FunType([Pointer(FunType([UChar] UChar)) UChar] UChar) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"gs\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Structure(\\\"s\\\")\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(FunType([UChar] UChar)) UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.fp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([UChar] UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"@2.t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}]]}}}, \"1\": {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\":"
      " [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"flag\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"p\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"byte_offset\": 1}}], \"size\": 3, \"complete\": true}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"flag\" Bool) member_decl(\"p\" Pointer(Bool))]))\n"
      "  VarDecl(var_decl(\"g\" None Array(Bool 3) None None None None false))\n"
      "  VarDecl(var_decl(\"gs\" None Structure(\"s\") None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.fp\" \"@1.b\"] Block([\n"
      "    D(VarDecl(var_decl(\"@2.t\" Cast(Bool Unary(LogicalNot Var(\"@1.b\" Bool) Int) Bool) Bool None None None None false)))\n"
      "    S(Return(FunctionCall(\"@0.fp\" [Var(\"@2.t\" Bool)] Bool false)))\n"
      "  ]) FunType([Pointer(FunType([Bool] Bool)) Bool] Bool) None None [\"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [\"@0.fp\" \"@1.b\"] [\n"
      "    Unary(LogicalNot Var(\"@1.b\") Var(\"%0\"))\n"
      "    Truncate(Var(\"%0\") Var(\"%1\"))\n"
      "    Copy(Var(\"%1\") Var(\"@2.t\"))\n"
      "    IndirectCall(Var(\"@0.fp\") [Var(\"@2.t\")] Var(\"%2\"))\n"
      "    Ret(Var(\"%2\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true Pointer [ZeroInit(3)])\n"
      "  StaticVariable(\"gs\" true Pointer [ZeroInit(3)])\n"
      "])\n");
}

TEST_CASE("parse: test_bool.py::TestLowering::test_other_values_compare_with_zero (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestLowering::test_other_values_compare_with_zero\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"bool f(int a) { return a & 2; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\"] Block([\n"
      "    S(Return(Binary(BitwiseAnd Var(\"a\" None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([Int] Bool) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestLowering::test_other_values_compare_with_zero (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestLowering::test_other_values_compare_with_zero\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\"] Block([\n"
      "    S(Return(Binary(BitwiseAnd Var(\"a\" None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([Int] Bool) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\"] Block([\n"
      "    S(Return(Binary(BitwiseAnd Var(\"@0.a\" None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([Int] Bool) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestLowering::test_other_values_compare_with_zero (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestLowering::test_other_values_compare_with_zero\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\"] Block([\\n    S(Return(Cast(Bool Binary(BitwiseAnd Var(\\\"@0.a\\\" Int) Constant(ConstInt(2) Int) Int) Bool)))\\n  ]) FunType([Int] Bool) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\"] Block([\n"
      "    S(Return(Binary(BitwiseAnd Var(\"@0.a\" None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([Int] Bool) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\"] Block([\\n    S(Return(Cast(Bool Binary(BitwiseAnd Var(\\\"@0.a\\\" Int) Constant(ConstInt(2) Int) Int) Bool)))\\n  ]) FunType([Int] Bool) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_bool.py::TestLowering::test_other_values_compare_with_zero (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_bool.py::TestLowering::test_other_values_compare_with_zero\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\"] Block([\\n    S(Return(Cast(Bool Binary(BitwiseAnd Var(\\\"@0.a\\\" Int) Constant(ConstInt(2) Int) Int) Bool)))\\n  ]) FunType([Int] UChar) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\"] Block([\n"
      "    S(Return(Cast(Bool Binary(BitwiseAnd Var(\"@0.a\" Int) Constant(ConstInt(2) Int) Int) Bool)))\n"
      "  ]) FunType([Int] Bool) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [\"@0.a\"] [\n"
      "    Binary(BitwiseAnd Var(\"@0.a\") Constant(ConstInt(2)) Var(\"%0\"))\n"
      "    Binary(NotEqual Var(\"%0\") Constant(ConstInt(0)) Var(\"%1\"))\n"
      "    Ret(Var(\"%1\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("parse: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"bool f(int a, int b, bool c) { return a < b; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\" \"b\" \"c\"] Block([\n"
      "    S(Return(Binary(LessThan Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\" \"b\" \"c\"] Block([\n"
      "    S(Return(Binary(LessThan Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\" \"@1.b\" \"@2.c\"] Block([\n"
      "    S(Return(Binary(LessThan Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\" \\\"@2.c\\\"] Block([\\n    S(Return(Cast(Bool Binary(LessThan Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int) Bool)))\\n  ]) FunType([Int Int Bool] Bool) None None [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\" \"@1.b\" \"@2.c\"] Block([\n"
      "    S(Return(Binary(LessThan Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\" \\\"@2.c\\\"] Block([\\n    S(Return(Cast(Bool Binary(LessThan Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int) Bool)))\\n  ]) FunType([Int Int Bool] Bool) None None [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int Bool] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int Bool] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\" \\\"@2.c\\\"] Block([\\n    S(Return(Cast(Bool Binary(LessThan Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int) Bool)))\\n  ]) FunType([Int Int UChar] UChar) None None [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\" \"@1.b\" \"@2.c\"] Block([\n"
      "    S(Return(Cast(Bool Binary(LessThan Var(\"@0.a\" Int) Var(\"@1.b\" Int) Int) Bool)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [\"@0.a\" \"@1.b\" \"@2.c\"] [\n"
      "    Binary(LessThan Var(\"@0.a\") Var(\"@1.b\") Var(\"%0\"))\n"
      "    Truncate(Var(\"%0\") Var(\"%1\"))\n"
      "    Ret(Var(\"%1\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("parse: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"bool f(int a, int b, bool c) { return !a; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\" \"b\" \"c\"] Block([\n"
      "    S(Return(Unary(LogicalNot Var(\"a\" None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\" \"b\" \"c\"] Block([\n"
      "    S(Return(Unary(LogicalNot Var(\"a\" None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\" \"@1.b\" \"@2.c\"] Block([\n"
      "    S(Return(Unary(LogicalNot Var(\"@0.a\" None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\" \\\"@2.c\\\"] Block([\\n    S(Return(Cast(Bool Unary(LogicalNot Var(\\\"@0.a\\\" Int) Int) Bool)))\\n  ]) FunType([Int Int Bool] Bool) None None [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\" \"@1.b\" \"@2.c\"] Block([\n"
      "    S(Return(Unary(LogicalNot Var(\"@0.a\" None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\" \\\"@2.c\\\"] Block([\\n    S(Return(Cast(Bool Unary(LogicalNot Var(\\\"@0.a\\\" Int) Int) Bool)))\\n  ]) FunType([Int Int Bool] Bool) None None [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int Bool] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int Bool] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\" \\\"@2.c\\\"] Block([\\n    S(Return(Cast(Bool Unary(LogicalNot Var(\\\"@0.a\\\" Int) Int) Bool)))\\n  ]) FunType([Int Int UChar] UChar) None None [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\" \"@1.b\" \"@2.c\"] Block([\n"
      "    S(Return(Cast(Bool Unary(LogicalNot Var(\"@0.a\" Int) Int) Bool)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [\"@0.a\" \"@1.b\" \"@2.c\"] [\n"
      "    Unary(LogicalNot Var(\"@0.a\") Var(\"%0\"))\n"
      "    Truncate(Var(\"%0\") Var(\"%1\"))\n"
      "    Ret(Var(\"%1\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("parse: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"bool f(int a, int b, bool c) { return a && b; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\" \"b\" \"c\"] Block([\n"
      "    S(Return(Binary(LogicalAnd Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\" \"b\" \"c\"] Block([\n"
      "    S(Return(Binary(LogicalAnd Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\" \"@1.b\" \"@2.c\"] Block([\n"
      "    S(Return(Binary(LogicalAnd Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\" \\\"@2.c\\\"] Block([\\n    S(Return(Cast(Bool Binary(LogicalAnd Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int) Bool)))\\n  ]) FunType([Int Int Bool] Bool) None None [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\" \"@1.b\" \"@2.c\"] Block([\n"
      "    S(Return(Binary(LogicalAnd Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\" \\\"@2.c\\\"] Block([\\n    S(Return(Cast(Bool Binary(LogicalAnd Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int) Bool)))\\n  ]) FunType([Int Int Bool] Bool) None None [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int Bool] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int Bool] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\" \\\"@2.c\\\"] Block([\\n    S(Return(Cast(Bool Binary(LogicalAnd Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int) Bool)))\\n  ]) FunType([Int Int UChar] UChar) None None [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\" \"@1.b\" \"@2.c\"] Block([\n"
      "    S(Return(Cast(Bool Binary(LogicalAnd Var(\"@0.a\" Int) Var(\"@1.b\" Int) Int) Bool)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [\"@0.a\" \"@1.b\" \"@2.c\"] [\n"
      "    JumpIfFalse(Var(\"@0.a\") \".and_false@0\")\n"
      "    JumpIfFalse(Var(\"@1.b\") \".and_false@0\")\n"
      "    Copy(Constant(ConstInt(1)) Var(\"%0\"))\n"
      "    Jump(\".and_end@1\")\n"
      "    Label(\".and_false@0\")\n"
      "    Copy(Constant(ConstInt(0)) Var(\"%0\"))\n"
      "    Label(\".and_end@1\")\n"
      "    Truncate(Var(\"%0\") Var(\"%1\"))\n"
      "    Ret(Var(\"%1\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("parse: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"bool f(int a, int b, bool c) { return c; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\" \"b\" \"c\"] Block([\n"
      "    S(Return(Var(\"c\" None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\" \"b\" \"c\"] Block([\n"
      "    S(Return(Var(\"c\" None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\" \"@1.b\" \"@2.c\"] Block([\n"
      "    S(Return(Var(\"@2.c\" None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\" \\\"@2.c\\\"] Block([\\n    S(Return(Var(\\\"@2.c\\\" Bool)))\\n  ]) FunType([Int Int Bool] Bool) None None [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\" \"@1.b\" \"@2.c\"] Block([\n"
      "    S(Return(Var(\"@2.c\" None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\" \\\"@2.c\\\"] Block([\\n    S(Return(Var(\\\"@2.c\\\" Bool)))\\n  ]) FunType([Int Int Bool] Bool) None None [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int Bool] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int Bool] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\" \\\"@2.c\\\"] Block([\\n    S(Return(Var(\\\"@2.c\\\" Bool)))\\n  ]) FunType([Int Int UChar] UChar) None None [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\" \"@1.b\" \"@2.c\"] Block([\n"
      "    S(Return(Var(\"@2.c\" Bool)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [\"@0.a\" \"@1.b\" \"@2.c\"] [\n"
      "    Ret(Var(\"@2.c\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("parse: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"bool f(int a, int b, bool c) { return (a < b) & a; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\" \"b\" \"c\"] Block([\n"
      "    S(Return(Binary(BitwiseAnd Binary(LessThan Var(\"a\" None) Var(\"b\" None) None) Var(\"a\" None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\" \"b\" \"c\"] Block([\n"
      "    S(Return(Binary(BitwiseAnd Binary(LessThan Var(\"a\" None) Var(\"b\" None) None) Var(\"a\" None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\" \"@1.b\" \"@2.c\"] Block([\n"
      "    S(Return(Binary(BitwiseAnd Binary(LessThan Var(\"@0.a\" None) Var(\"@1.b\" None) None) Var(\"@0.a\" None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 19)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\" \\\"@2.c\\\"] Block([\\n    S(Return(Cast(Bool Binary(BitwiseAnd Binary(LessThan Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int) Var(\\\"@0.a\\\" Int) Int) Bool)))\\n  ]) FunType([Int Int Bool] Bool) None None [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\" \"@1.b\" \"@2.c\"] Block([\n"
      "    S(Return(Binary(BitwiseAnd Binary(LessThan Var(\"@0.a\" None) Var(\"@1.b\" None) None) Var(\"@0.a\" None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\" \\\"@2.c\\\"] Block([\\n    S(Return(Cast(Bool Binary(BitwiseAnd Binary(LessThan Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int) Var(\\\"@0.a\\\" Int) Int) Bool)))\\n  ]) FunType([Int Int Bool] Bool) None None [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int Bool] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 20)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int Bool] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\" \\\"@2.c\\\"] Block([\\n    S(Return(Cast(Bool Binary(BitwiseAnd Binary(LessThan Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int) Var(\\\"@0.a\\\" Int) Int) Bool)))\\n  ]) FunType([Int Int UChar] UChar) None None [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\" \"@1.b\" \"@2.c\"] Block([\n"
      "    S(Return(Cast(Bool Binary(BitwiseAnd Binary(LessThan Var(\"@0.a\" Int) Var(\"@1.b\" Int) Int) Var(\"@0.a\" Int) Int) Bool)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [\"@0.a\" \"@1.b\" \"@2.c\"] [\n"
      "    Binary(LessThan Var(\"@0.a\") Var(\"@1.b\") Var(\"%0\"))\n"
      "    Binary(BitwiseAnd Var(\"%0\") Var(\"@0.a\") Var(\"%1\"))\n"
      "    Truncate(Var(\"%1\") Var(\"%2\"))\n"
      "    Ret(Var(\"%2\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("parse: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 21)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"bool f(int a, int b, bool c) { return a < b \077 1 : 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\" \"b\" \"c\"] Block([\n"
      "    S(Return(Conditional(Binary(LessThan Var(\"a\" None) Var(\"b\" None) None) Constant(ConstInt(1) None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 22)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\" \"b\" \"c\"] Block([\n"
      "    S(Return(Conditional(Binary(LessThan Var(\"a\" None) Var(\"b\" None) None) Constant(ConstInt(1) None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\" \"@1.b\" \"@2.c\"] Block([\n"
      "    S(Return(Conditional(Binary(LessThan Var(\"@0.a\" None) Var(\"@1.b\" None) None) Constant(ConstInt(1) None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 23)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\" \\\"@2.c\\\"] Block([\\n    S(Return(Cast(Bool Conditional(Binary(LessThan Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int) Constant(ConstInt(1) Int) Constant(ConstInt(0) Int) Int) Bool)))\\n  ]) FunType([Int Int Bool] Bool) None None [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\" \"@1.b\" \"@2.c\"] Block([\n"
      "    S(Return(Conditional(Binary(LessThan Var(\"@0.a\" None) Var(\"@1.b\" None) None) Constant(ConstInt(1) None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\" \\\"@2.c\\\"] Block([\\n    S(Return(Cast(Bool Conditional(Binary(LessThan Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int) Constant(ConstInt(1) Int) Constant(ConstInt(0) Int) Int) Bool)))\\n  ]) FunType([Int Int Bool] Bool) None None [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int Bool] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 24)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int Bool] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\" \\\"@2.c\\\"] Block([\\n    S(Return(Cast(Bool Conditional(Binary(LessThan Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int) Constant(ConstInt(1) Int) Constant(ConstInt(0) Int) Int) Bool)))\\n  ]) FunType([Int Int UChar] UChar) None None [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\" \"@1.b\" \"@2.c\"] Block([\n"
      "    S(Return(Cast(Bool Conditional(Binary(LessThan Var(\"@0.a\" Int) Var(\"@1.b\" Int) Int) Constant(ConstInt(1) Int) Constant(ConstInt(0) Int) Int) Bool)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [\"@0.a\" \"@1.b\" \"@2.c\"] [\n"
      "    Binary(LessThan Var(\"@0.a\") Var(\"@1.b\") Var(\"%0\"))\n"
      "    JumpIfFalse(Var(\"%0\") \".cond_else@0\")\n"
      "    Copy(Constant(ConstInt(1)) Var(\"%1\"))\n"
      "    Jump(\".cond_end@1\")\n"
      "    Label(\".cond_else@0\")\n"
      "    Copy(Constant(ConstInt(0)) Var(\"%1\"))\n"
      "    Label(\".cond_end@1\")\n"
      "    Truncate(Var(\"%1\") Var(\"%2\"))\n"
      "    Ret(Var(\"%2\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("parse: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 25)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"bool f(int a, int b, bool c) { return (int)c; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\" \"b\" \"c\"] Block([\n"
      "    S(Return(Cast(Int Var(\"c\" None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 26)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\" \"b\" \"c\"] Block([\n"
      "    S(Return(Cast(Int Var(\"c\" None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\" \"@1.b\" \"@2.c\"] Block([\n"
      "    S(Return(Cast(Int Var(\"@2.c\" None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 27)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\" \\\"@2.c\\\"] Block([\\n    S(Return(Cast(Bool Cast(Int Var(\\\"@2.c\\\" Bool) Int) Bool)))\\n  ]) FunType([Int Int Bool] Bool) None None [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\" \"@1.b\" \"@2.c\"] Block([\n"
      "    S(Return(Cast(Int Var(\"@2.c\" None) None)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\" \\\"@2.c\\\"] Block([\\n    S(Return(Cast(Bool Cast(Int Var(\\\"@2.c\\\" Bool) Int) Bool)))\\n  ]) FunType([Int Int Bool] Bool) None None [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int Bool] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison (call 28)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_bool.py::TestLowering::test_zero_or_one_values_skip_the_comparison\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int Bool] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\" \\\"@2.c\\\"] Block([\\n    S(Return(Cast(Bool Cast(Int Var(\\\"@2.c\\\" Bool) Int) Bool)))\\n  ]) FunType([Int Int UChar] UChar) None None [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.a\" \"@1.b\" \"@2.c\"] Block([\n"
      "    S(Return(Cast(Bool Cast(Int Var(\"@2.c\" Bool) Int) Bool)))\n"
      "  ]) FunType([Int Int Bool] Bool) None None [\"\" \"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [\"@0.a\" \"@1.b\" \"@2.c\"] [\n"
      "    ZeroExtend(Var(\"@2.c\") Var(\"%0\"))\n"
      "    Truncate(Var(\"%0\") Var(\"%1\"))\n"
      "    Ret(Var(\"%1\"))\n"
      "  ])\n"
      "])\n");
}
