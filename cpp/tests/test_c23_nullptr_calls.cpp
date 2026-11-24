// The calls that the tests of tests/test_c23_nullptr.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 and G7). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_c23_nullptr.py::TestValues::test_sizes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nunsigned long n = sizeof(nullptr);\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Constant(ConstNullptr None) None) ULong None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestValues::test_sizes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Constant(ConstNullptr None) None) ULong None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Constant(ConstNullptr None) None) ULong None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestValues::test_sizes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(ULong SizeOfExp(Constant(ConstNullptr NullptrT) UInt) ULong) ULong None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Constant(ConstNullptr None) None) ULong None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(ULong SizeOfExp(Constant(ConstNullptr NullptrT) UInt) ULong) ULong None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 2}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestValues::test_sizes (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nunsigned long n = sizeof nullptr;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Constant(ConstNullptr None) None) ULong None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestValues::test_sizes (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nunsigned long n = sizeof(nullptr_t);\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfType(NullptrT None) ULong None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestValues::test_sizes (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfType(NullptrT None) ULong None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfType(NullptrT None) ULong None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestValues::test_sizes (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(ULong SizeOfType(NullptrT UInt) ULong) ULong None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfType(NullptrT None) ULong None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(ULong SizeOfType(NullptrT UInt) ULong) ULong None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 2}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestValues::test_sizes (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nunsigned long n = sizeof(nullptr_t[1]);\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfType(Array(NullptrT 1) None) ULong None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestValues::test_sizes (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfType(Array(NullptrT 1) None) ULong None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfType(Array(NullptrT 1) None) ULong None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestValues::test_sizes (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(ULong SizeOfType(Array(NullptrT 1) UInt) ULong) ULong None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfType(Array(NullptrT 1) None) ULong None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(ULong SizeOfType(Array(NullptrT 1) UInt) ULong) ULong None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 2}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestValues::test_static_initializers (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nint *b = nullptr;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstNullptr None) Pointer(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestValues::test_static_initializers (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstNullptr None) Pointer(Int) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstNullptr None) Pointer(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestValues::test_static_initializers (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Cast(Pointer(Int) Constant(ConstNullptr NullptrT) Pointer(Int)) Pointer(Int) None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstNullptr None) Pointer(Int) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Cast(Pointer(Int) Constant(ConstNullptr NullptrT) Pointer(Int)) Pointer(Int) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestValues::test_static_initializers (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nbool b = nullptr;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstNullptr None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestValues::test_static_initializers (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstNullptr None) Bool None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstNullptr None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestValues::test_static_initializers (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Constant(ConstNullptr NullptrT) Bool) Bool None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstNullptr None) Bool None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Constant(ConstNullptr NullptrT) Bool) Bool None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestValues::test_static_initializers (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nnullptr_t b = nullptr;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstNullptr None) NullptrT None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestValues::test_static_initializers (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstNullptr None) NullptrT None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstNullptr None) NullptrT None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestValues::test_static_initializers (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Constant(ConstNullptr NullptrT) NullptrT None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstNullptr None) NullptrT None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Constant(ConstNullptr NullptrT) NullptrT None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestValues::test_static_initializers (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nnullptr_t b = 0;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(0) None) NullptrT None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestValues::test_static_initializers (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(0) None) NullptrT None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(0) None) NullptrT None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestValues::test_static_initializers (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Cast(NullptrT Constant(ConstInt(0) Int) NullptrT) NullptrT None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstInt(0) None) NullptrT None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Cast(NullptrT Constant(ConstInt(0) Int) NullptrT) NullptrT None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestValues::test_static_initializers (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;\\nvoid (*b)(void) = nullptr;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstNullptr None) Pointer(FunType([] Void)) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestValues::test_static_initializers (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstNullptr None) Pointer(FunType([] Void)) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstNullptr None) Pointer(FunType([] Void)) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestValues::test_static_initializers (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Cast(Pointer(FunType([] Void)) Constant(ConstNullptr NullptrT) Pointer(FunType([] Void))) Pointer(FunType([] Void)) None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstNullptr None) Pointer(FunType([] Void)) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Cast(Pointer(FunType([] Void)) Constant(ConstNullptr NullptrT) Pointer(FunType([] Void))) Pointer(FunType([] Void)) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([] Void))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_nullptr.py::TestValues::test_tac_has_the_constant (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_nullptr.py::TestValues::test_tac_has_the_constant\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int *f(void) { return nullptr; }bool g(void) { return nullptr; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] Bool) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_nullptr.py::TestValues::test_tac_has_the_constant (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_nullptr.py::TestValues::test_tac_has_the_constant\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] Bool) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] Bool) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_nullptr.py::TestValues::test_tac_has_the_constant (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_nullptr.py::TestValues::test_tac_has_the_constant\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Cast(Pointer(Int) Constant(ConstNullptr NullptrT) Pointer(Int))))\\n  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(Bool Constant(ConstNullptr NullptrT) Bool)))\\n  ]) FunType([] Bool) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(Constant(ConstNullptr None)))\n"
      "  ]) FunType([] Bool) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Cast(Pointer(Int) Constant(ConstNullptr NullptrT) Pointer(Int))))\\n  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(Bool Constant(ConstNullptr NullptrT) Bool)))\\n  ]) FunType([] Bool) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Pointer(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_c23_nullptr.py::TestValues::test_tac_has_the_constant (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_c23_nullptr.py::TestValues::test_tac_has_the_constant\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Pointer(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Bool)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Cast(Pointer(Int) Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Pointer(Int))))\\n  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Return(Cast(Bool Cast(Pointer(Void) Constant(ConstInt(0) Int) Pointer(Void)) Bool)))\\n  ]) FunType([] UChar) None None [] false None [] None false))\\n])\\n\"}, \"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Pointer(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Cast(Pointer(Int) Constant(ConstNullptr NullptrT) Pointer(Int))))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(Cast(Bool Constant(ConstNullptr NullptrT) Bool)))\n"
      "  ]) FunType([] Bool) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Ret(Constant(ConstUInt(0)))\n"
      "  ])\n"
      "  Function(\"g\" true [] [\n"
      "    Ret(Constant(ConstUChar(0)))\n"
      "  ])\n"
      "])\n");
}
