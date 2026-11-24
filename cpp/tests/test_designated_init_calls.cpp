// The calls that the tests of tests/test_designated_init.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_designated_init.py::TestParse::test_compound_literal (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestParse::test_compound_literal\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int *p = (int[]){[3] = 1};\\nint q = (int){.x = 1};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" CompoundLiteral(\"\" Array(Int 0) InitList([Designation([IndexDesignator(3)] Constant(ConstInt(1) None))] None None) None None false) Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"q\" CompoundLiteral(\"\" Int InitList([Designation([MemberDesignator(\"x\")] Constant(ConstInt(1) None))] None None) None None false) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_designated_init.py::TestParse::test_designations (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestParse::test_designations\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nstruct s v = {.pt[1].y = 7, [2] = 1, 3};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\") IndexDesignator(1) MemberDesignator(\"y\")] Constant(ConstInt(7) None)) Designation([IndexDesignator(2)] Constant(ConstInt(1) None)) Constant(ConstInt(3) None)] None None) Structure(\"s\") None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_designated_init.py::TestParse::test_index_errors (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestParse::test_index_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[3] = {[-1] = 1};\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"an array designator's index is -1, and it must not be negative (C23 \\u00a76.7.11p6)\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestParse::test_index_errors (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestParse::test_index_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int n; int a[3] = {[n] = 1};\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"an array designator's index must be an integer constant expression (C23 \\u00a76.7.11p6): undeclared identifier 'n'\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestParse::test_index_errors (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestParse::test_index_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[3] = {[1.5] = 1};\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"an array designator's index must be an integer constant expression (C23 \\u00a76.7.11p6): floating constant ConstDouble(bits=4609434218613702656) is not an integer constant\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestParse::test_index_is_folded (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestParse::test_index_is_folded\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum { E = 1 };\\nconstexpr int K = 2;\\nint a[8] = {[E] = 1, [K * 2 + 1] = 2, [sizeof(long)] = 3};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(2) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(1)] Constant(ConstInt(1) None)) Designation([IndexDesignator(5)] Constant(ConstInt(2) None)) Designation([IndexDesignator(4)] Constant(ConstInt(3) None))] None None) Array(Int 8) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_designated_init.py::TestNormalize::test_chain (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestNormalize::test_chain\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nstruct s v = {.pt[1].y = 7, .n = 3};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\") IndexDesignator(1) MemberDesignator(\"y\")] Constant(ConstInt(7) None)) Designation([MemberDesignator(\"n\")] Constant(ConstInt(3) None))] None None) Structure(\"s\") None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestNormalize::test_chain (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestNormalize::test_chain\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\") IndexDesignator(1) MemberDesignator(\"y\")] Constant(ConstInt(7) None)) Designation([MemberDesignator(\"n\")] Constant(ConstInt(3) None))] None None) Structure(\"s\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\") IndexDesignator(1) MemberDesignator(\"y\")] Constant(ConstInt(7) None)) Designation([MemberDesignator(\"n\")] Constant(ConstInt(3) None))] None None) Structure(\"s\") None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestNormalize::test_chain (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestNormalize::test_chain\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\") IndexDesignator(1) MemberDesignator(\"y\")] Constant(ConstInt(7) None)) Designation([MemberDesignator(\"n\")] Constant(ConstInt(3) None))] None None) Structure(\"s\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Constant(ConstInt(3) None) InitList([InitList([] None None) InitList([InitList([] None None) Constant(ConstInt(7) None)] None None)] None None)] None None) Structure(\"s\") None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_designated_init.py::TestNormalize::test_constexpr (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestNormalize::test_constexpr\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nconstexpr int K = 2;\\nint a[4] = {[K] = 5, [K + 1] = 6};\\nconstexpr struct p c = {.y = 4};\\nint b[c.y];\\nconstexpr union u cu = {.i = 3};\\nint d[cu.i];\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(2) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(2)] Constant(ConstInt(5) None)) Designation([IndexDesignator(3)] Constant(ConstInt(6) None))] None None) Array(Int 4) None None None None false))\n"
      "  VarDecl(var_decl(\"c\" InitList([Designation([MemberDesignator(\"y\")] Constant(ConstInt(4) None))] None None) Const(Structure(\"p\")) None None None None true))\n"
      "  VarDecl(var_decl(\"b\" None Array(Int 4) None None None None false))\n"
      "  VarDecl(var_decl(\"cu\" InitList([Designation([MemberDesignator(\"i\")] Constant(ConstInt(3) None))] None None) Const(Union(\"u\")) None None None None true))\n"
      "  VarDecl(var_decl(\"d\" None Array(Int 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestNormalize::test_constexpr (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestNormalize::test_constexpr\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(2) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(2)] Constant(ConstInt(5) None)) Designation([IndexDesignator(3)] Constant(ConstInt(6) None))] None None) Array(Int 4) None None None None false))\n"
      "  VarDecl(var_decl(\"c\" InitList([Designation([MemberDesignator(\"y\")] Constant(ConstInt(4) None))] None None) Const(Structure(\"p\")) None None None None true))\n"
      "  VarDecl(var_decl(\"b\" None Array(Int 4) None None None None false))\n"
      "  VarDecl(var_decl(\"cu\" InitList([Designation([MemberDesignator(\"i\")] Constant(ConstInt(3) None))] None None) Const(Union(\"u\")) None None None None true))\n"
      "  VarDecl(var_decl(\"d\" None Array(Int 3) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(2) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(2)] Constant(ConstInt(5) None)) Designation([IndexDesignator(3)] Constant(ConstInt(6) None))] None None) Array(Int 4) None None None None false))\n"
      "  VarDecl(var_decl(\"c\" InitList([Designation([MemberDesignator(\"y\")] Constant(ConstInt(4) None))] None None) Const(Structure(\"p\")) None None None None true))\n"
      "  VarDecl(var_decl(\"b\" None Array(Int 4) None None None None false))\n"
      "  VarDecl(var_decl(\"cu\" InitList([Designation([MemberDesignator(\"i\")] Constant(ConstInt(3) None))] None None) Const(Union(\"u\")) None None None None true))\n"
      "  VarDecl(var_decl(\"d\" None Array(Int 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestNormalize::test_constexpr (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestNormalize::test_constexpr\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(2) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(2)] Constant(ConstInt(5) None)) Designation([IndexDesignator(3)] Constant(ConstInt(6) None))] None None) Array(Int 4) None None None None false))\n"
      "  VarDecl(var_decl(\"c\" InitList([Designation([MemberDesignator(\"y\")] Constant(ConstInt(4) None))] None None) Const(Structure(\"p\")) None None None None true))\n"
      "  VarDecl(var_decl(\"b\" None Array(Int 4) None None None None false))\n"
      "  VarDecl(var_decl(\"cu\" InitList([Designation([MemberDesignator(\"i\")] Constant(ConstInt(3) None))] None None) Const(Union(\"u\")) None None None None true))\n"
      "  VarDecl(var_decl(\"d\" None Array(Int 3) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(2) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" InitList([InitList([] None None) InitList([] None None) Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None) Array(Int 4) None None None None false))\n"
      "  VarDecl(var_decl(\"c\" InitList([InitList([] None None) Constant(ConstInt(4) None)] None None) Const(Structure(\"p\")) None None None None true))\n"
      "  VarDecl(var_decl(\"b\" None Array(Int 4) None None None None false))\n"
      "  VarDecl(var_decl(\"cu\" InitList([Constant(ConstInt(3) None)] None \"i\") Const(Union(\"u\")) None None None None true))\n"
      "  VarDecl(var_decl(\"d\" None Array(Int 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_designated_init.py::TestNormalize::test_constexpr (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_designated_init.py::TestNormalize::test_constexpr\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"p\\\" false [member_decl(\\\"x\\\" Int) member_decl(\\\"y\\\" Int)]))\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"n\\\" Char) member_decl(\\\"pt\\\" Array(Structure(\\\"p\\\") 2)) member_decl(\\\"name\\\" Array(Char 4))]))\\n  StructDecl(struct_decl(\\\"u\\\" true [member_decl(\\\"c\\\" Char) member_decl(\\\"l\\\" Long) member_decl(\\\"i\\\" Int)]))\\n  StructDecl(struct_decl(\\\"w\\\" false [member_decl(\\\"tag\\\" Int) member_decl(\\\"v\\\" Union(\\\"u\\\"))]))\\n  VarDecl(var_decl(\\\"K\\\" Constant(ConstInt(2) Int) Const(Int) Static None None None true))\\n  VarDecl(var_decl(\\\"a\\\" InitList([Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(5) Int) Constant(ConstInt(6) Int)] Array(Int 4) None) Array(Int 4) None None None None false))\\n  VarDecl(var_decl(\\\"c\\\" InitList([Constant(ConstInt(0) Int) Constant(ConstInt(4) Int)] Structure(\\\"p\\\") None) Const(Structure(\\\"p\\\")) Static None None None true))\\n  VarDecl(var_decl(\\\"b\\\" None Array(Int 4) None None None None false))\\n  VarDecl(var_decl(\\\"cu\\\" InitList([Constant(ConstInt(3) Int)] Union(\\\"u\\\") \\\"i\\\") Const(Union(\\\"u\\\")) Static None None None true))\\n  VarDecl(var_decl(\\\"d\\\" None Array(Int 3) None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"K\" Constant(ConstInt(2) None) Const(Int) None None None None true))\n"
      "  VarDecl(var_decl(\"a\" InitList([InitList([] None None) InitList([] None None) Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None) Array(Int 4) None None None None false))\n"
      "  VarDecl(var_decl(\"c\" InitList([InitList([] None None) Constant(ConstInt(4) None)] None None) Const(Structure(\"p\")) None None None None true))\n"
      "  VarDecl(var_decl(\"b\" None Array(Int 4) None None None None false))\n"
      "  VarDecl(var_decl(\"cu\" InitList([Constant(ConstInt(3) None)] None \"i\") Const(Union(\"u\")) None None None None true))\n"
      "  VarDecl(var_decl(\"d\" None Array(Int 3) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"p\\\" false [member_decl(\\\"x\\\" Int) member_decl(\\\"y\\\" Int)]))\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"n\\\" Char) member_decl(\\\"pt\\\" Array(Structure(\\\"p\\\") 2)) member_decl(\\\"name\\\" Array(Char 4))]))\\n  StructDecl(struct_decl(\\\"u\\\" true [member_decl(\\\"c\\\" Char) member_decl(\\\"l\\\" Long) member_decl(\\\"i\\\" Int)]))\\n  StructDecl(struct_decl(\\\"w\\\" false [member_decl(\\\"tag\\\" Int) member_decl(\\\"v\\\" Union(\\\"u\\\"))]))\\n  VarDecl(var_decl(\\\"K\\\" Constant(ConstInt(2) Int) Const(Int) Static None None None true))\\n  VarDecl(var_decl(\\\"a\\\" InitList([Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(5) Int) Constant(ConstInt(6) Int)] Array(Int 4) None) Array(Int 4) None None None None false))\\n  VarDecl(var_decl(\\\"c\\\" InitList([Constant(ConstInt(0) Int) Constant(ConstInt(4) Int)] Structure(\\\"p\\\") None) Const(Structure(\\\"p\\\")) Static None None None true))\\n  VarDecl(var_decl(\\\"b\\\" None Array(Int 4) None None None None false))\\n  VarDecl(var_decl(\\\"cu\\\" InitList([Constant(ConstInt(3) Int)] Union(\\\"u\\\") \\\"i\\\") Const(Union(\\\"u\\\")) Static None None None true))\\n  VarDecl(var_decl(\\\"d\\\" None Array(Int 3) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"K\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 2}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0, 0, 5, 6]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Structure(\\\"p\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0, 4]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"cu\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Union(\\\"u\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"object\": \"passes.type_checking.UnionValue\", \"fields\": {\"member\": 2, \"value\": 3}}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_check"
      "ing.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"p\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"x\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"y\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 2}}], \"size\": 4, \"complete\": true}}], [\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"n\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"pt\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Structure(\\\"p\\\") 2)\\n\"}, \"byte_offset\": 1}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"name\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 4)\\n\"}, \"byte_offset\": 9}}], \"size\": 13, \"complete\": true}}], [\"u\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"u\", \"is_union\": true, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"c\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"l\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"i\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 0}}], \"size\": 4, \"complete\": true}}], [\"w\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"w\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"tag\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"v\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Union(\\\"u\\\")\\n\"}, \"byte_offset\": 2}}], \"size\": 6, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_designated_init.py::TestNormalize::test_constexpr (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestNormalize::test_constexpr\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nconstexpr union u cu = {.i = 3};\\nint d[cu.c];\\n\"\n"
      "output-error\n"
      "{\"type\": \"NotImplementedError\", \"message\": \"array size must be an integer constant expression: member 'c' of a union constant isn't the member its initializer sets, and only that one is a constant (C23 \\u00a76.6p16)\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestNormalize::test_list_without_designators_is_unchanged (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestNormalize::test_list_without_designators_is_unchanged\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[2][2] = {{1}, {2, 3}};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([InitList([Constant(ConstInt(1) None)] None None) InitList([Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None)] None None) Array(Array(Int 2) 2) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestNormalize::test_list_without_designators_is_unchanged (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestNormalize::test_list_without_designators_is_unchanged\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([InitList([Constant(ConstInt(1) None)] None None) InitList([Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None)] None None) Array(Array(Int 2) 2) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([InitList([Constant(ConstInt(1) None)] None None) InitList([Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None)] None None) Array(Array(Int 2) 2) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestNormalize::test_list_without_designators_is_unchanged (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestNormalize::test_list_without_designators_is_unchanged\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([InitList([Constant(ConstInt(1) None)] None None) InitList([Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None)] None None) Array(Array(Int 2) 2) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([InitList([Constant(ConstInt(1) None)] None None) InitList([Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None)] None None) Array(Array(Int 2) 2) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_designated_init.py::TestNormalize::test_overrides (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestNormalize::test_overrides\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nstruct s v = {.pt = {{1, 2}, {3, 4}}, .pt[0].y = 9, .n = 1, .n = 2};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\")] InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None)] None None)) Designation([MemberDesignator(\"pt\") IndexDesignator(0) MemberDesignator(\"y\")] Constant(ConstInt(9) None)) Designation([MemberDesignator(\"n\")] Constant(ConstInt(1) None)) Designation([MemberDesignator(\"n\")] Constant(ConstInt(2) None))] None None) Structure(\"s\") None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestNormalize::test_overrides (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestNormalize::test_overrides\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\")] InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None)] None None)) Designation([MemberDesignator(\"pt\") IndexDesignator(0) MemberDesignator(\"y\")] Constant(ConstInt(9) None)) Designation([MemberDesignator(\"n\")] Constant(ConstInt(1) None)) Designation([MemberDesignator(\"n\")] Constant(ConstInt(2) None))] None None) Structure(\"s\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\")] InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None)] None None)) Designation([MemberDesignator(\"pt\") IndexDesignator(0) MemberDesignator(\"y\")] Constant(ConstInt(9) None)) Designation([MemberDesignator(\"n\")] Constant(ConstInt(1) None)) Designation([MemberDesignator(\"n\")] Constant(ConstInt(2) None))] None None) Structure(\"s\") None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestNormalize::test_overrides (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestNormalize::test_overrides\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\")] InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None)] None None)) Designation([MemberDesignator(\"pt\") IndexDesignator(0) MemberDesignator(\"y\")] Constant(ConstInt(9) None)) Designation([MemberDesignator(\"n\")] Constant(ConstInt(1) None)) Designation([MemberDesignator(\"n\")] Constant(ConstInt(2) None))] None None) Structure(\"s\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Constant(ConstInt(2) None) InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(9) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None)] None None)] None None) Structure(\"s\") None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_designated_init.py::TestNormalize::test_overrides (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestNormalize::test_overrides\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nstruct s v = {.pt[1].y = 6, .pt[1] = {5}};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\") IndexDesignator(1) MemberDesignator(\"y\")] Constant(ConstInt(6) None)) Designation([MemberDesignator(\"pt\") IndexDesignator(1)] InitList([Constant(ConstInt(5) None)] None None))] None None) Structure(\"s\") None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestNormalize::test_overrides (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestNormalize::test_overrides\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\") IndexDesignator(1) MemberDesignator(\"y\")] Constant(ConstInt(6) None)) Designation([MemberDesignator(\"pt\") IndexDesignator(1)] InitList([Constant(ConstInt(5) None)] None None))] None None) Structure(\"s\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\") IndexDesignator(1) MemberDesignator(\"y\")] Constant(ConstInt(6) None)) Designation([MemberDesignator(\"pt\") IndexDesignator(1)] InitList([Constant(ConstInt(5) None)] None None))] None None) Structure(\"s\") None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestNormalize::test_overrides (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestNormalize::test_overrides\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\") IndexDesignator(1) MemberDesignator(\"y\")] Constant(ConstInt(6) None)) Designation([MemberDesignator(\"pt\") IndexDesignator(1)] InitList([Constant(ConstInt(5) None)] None None))] None None) Structure(\"s\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([InitList([] None None) InitList([InitList([] None None) InitList([Constant(ConstInt(5) None)] None None)] None None)] None None) Structure(\"s\") None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_designated_init.py::TestNormalize::test_overrides (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestNormalize::test_overrides\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[3] = {[0] = 1, 2, [0] = 3};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0)] Constant(ConstInt(1) None)) Constant(ConstInt(2) None) Designation([IndexDesignator(0)] Constant(ConstInt(3) None))] None None) Array(Int 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestNormalize::test_overrides (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestNormalize::test_overrides\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0)] Constant(ConstInt(1) None)) Constant(ConstInt(2) None) Designation([IndexDesignator(0)] Constant(ConstInt(3) None))] None None) Array(Int 3) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0)] Constant(ConstInt(1) None)) Constant(ConstInt(2) None) Designation([IndexDesignator(0)] Constant(ConstInt(3) None))] None None) Array(Int 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestNormalize::test_overrides (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestNormalize::test_overrides\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0)] Constant(ConstInt(1) None)) Constant(ConstInt(2) None) Designation([IndexDesignator(0)] Constant(ConstInt(3) None))] None None) Array(Int 3) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(3) None) Constant(ConstInt(2) None)] None None) Array(Int 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_designated_init.py::TestNormalize::test_positional_continues_after_designation (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestNormalize::test_positional_continues_after_designation\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[6] = {1, [3] = 4, 5, [1] = 2};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Designation([IndexDesignator(3)] Constant(ConstInt(4) None)) Constant(ConstInt(5) None) Designation([IndexDesignator(1)] Constant(ConstInt(2) None))] None None) Array(Int 6) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestNormalize::test_positional_continues_after_designation (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestNormalize::test_positional_continues_after_designation\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Designation([IndexDesignator(3)] Constant(ConstInt(4) None)) Constant(ConstInt(5) None) Designation([IndexDesignator(1)] Constant(ConstInt(2) None))] None None) Array(Int 6) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Designation([IndexDesignator(3)] Constant(ConstInt(4) None)) Constant(ConstInt(5) None) Designation([IndexDesignator(1)] Constant(ConstInt(2) None))] None None) Array(Int 6) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestNormalize::test_positional_continues_after_designation (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestNormalize::test_positional_continues_after_designation\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Designation([IndexDesignator(3)] Constant(ConstInt(4) None)) Constant(ConstInt(5) None) Designation([IndexDesignator(1)] Constant(ConstInt(2) None))] None None) Array(Int 6) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) InitList([] None None) Constant(ConstInt(4) None) Constant(ConstInt(5) None)] None None) Array(Int 6) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_designated_init.py::TestNormalize::test_positional_continues_after_designation (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestNormalize::test_positional_continues_after_designation\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nstruct p q = {.y = 1, .x = 2};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"q\" InitList([Designation([MemberDesignator(\"y\")] Constant(ConstInt(1) None)) Designation([MemberDesignator(\"x\")] Constant(ConstInt(2) None))] None None) Structure(\"p\") None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestNormalize::test_positional_continues_after_designation (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestNormalize::test_positional_continues_after_designation\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"q\" InitList([Designation([MemberDesignator(\"y\")] Constant(ConstInt(1) None)) Designation([MemberDesignator(\"x\")] Constant(ConstInt(2) None))] None None) Structure(\"p\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"q\" InitList([Designation([MemberDesignator(\"y\")] Constant(ConstInt(1) None)) Designation([MemberDesignator(\"x\")] Constant(ConstInt(2) None))] None None) Structure(\"p\") None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestNormalize::test_positional_continues_after_designation (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestNormalize::test_positional_continues_after_designation\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"q\" InitList([Designation([MemberDesignator(\"y\")] Constant(ConstInt(1) None)) Designation([MemberDesignator(\"x\")] Constant(ConstInt(2) None))] None None) Structure(\"p\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"q\" InitList([Constant(ConstInt(2) None) Constant(ConstInt(1) None)] None None) Structure(\"p\") None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_designated_init.py::TestNormalize::test_side_effects_kept_when_not_overridden (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestNormalize::test_side_effects_kept_when_not_overridden\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void);\\nint g(void) { int a[3] = {[2] = f()}; return a[2]; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(2)] FunctionCall(\"f\" [] None false))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestNormalize::test_side_effects_kept_when_not_overridden (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestNormalize::test_side_effects_kept_when_not_overridden\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(2)] FunctionCall(\"f\" [] None false))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Designation([IndexDesignator(2)] FunctionCall(\"f\" [] None false))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestNormalize::test_side_effects_kept_when_not_overridden (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestNormalize::test_side_effects_kept_when_not_overridden\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Designation([IndexDesignator(2)] FunctionCall(\"f\" [] None false))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([InitList([] None None) InitList([] None None) FunctionCall(\"f\" [] None false)] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_designated_init.py::TestNormalize::test_side_effects_kept_when_not_overridden (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_designated_init.py::TestNormalize::test_side_effects_kept_when_not_overridden\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] None FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) FunctionCall(\\\"f\\\" [] Int false)] Array(Int 3) None) Array(Int 3) None None None None false)))\\n    S(Return(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 3)) Pointer(Int)) Constant(ConstInt(2) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([InitList([] None None) InitList([] None None) FunctionCall(\"f\" [] None false)] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] None FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) FunctionCall(\\\"f\\\" [] Int false)] Array(Int 3) None) Array(Int 3) None None None None false)))\\n    S(Return(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 3)) Pointer(Int)) Constant(ConstInt(2) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_designated_init.py::TestNormalize::test_type_checker_normalizes_too (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestNormalize::test_type_checker_normalizes_too\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[4] = {[2] = 5, 6};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(2)] Constant(ConstInt(5) None)) Constant(ConstInt(6) None)] None None) Array(Int 4) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestNormalize::test_type_checker_normalizes_too (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestNormalize::test_type_checker_normalizes_too\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(2)] Constant(ConstInt(5) None)) Constant(ConstInt(6) None)] None None) Array(Int 4) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(2)] Constant(ConstInt(5) None)) Constant(ConstInt(6) None)] None None) Array(Int 4) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_designated_init.py::TestNormalize::test_type_checker_normalizes_too (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_designated_init.py::TestNormalize::test_type_checker_normalizes_too\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" InitList([Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(5) Int) Constant(ConstInt(6) Int)] Array(Int 4) None) Array(Int 4) None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(2)] Constant(ConstInt(5) None)) Constant(ConstInt(6) None)] None None) Array(Int 4) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" InitList([Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(5) Int) Constant(ConstInt(6) Int)] Array(Int 4) None) Array(Int 4) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0, 0, 5, 6]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_designated_init.py::TestNormalize::test_union (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestNormalize::test_union\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nunion u v = {.i = 5};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"i\")] Constant(ConstInt(5) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestNormalize::test_union (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestNormalize::test_union\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"i\")] Constant(ConstInt(5) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"i\")] Constant(ConstInt(5) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestNormalize::test_union (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestNormalize::test_union\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"i\")] Constant(ConstInt(5) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Constant(ConstInt(5) None)] None \"i\") Union(\"u\") None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_designated_init.py::TestNormalize::test_union (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestNormalize::test_union\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nunion u v = {.c = 1, .l = 2};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"c\")] Constant(ConstInt(1) None)) Designation([MemberDesignator(\"l\")] Constant(ConstInt(2) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestNormalize::test_union (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestNormalize::test_union\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"c\")] Constant(ConstInt(1) None)) Designation([MemberDesignator(\"l\")] Constant(ConstInt(2) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"c\")] Constant(ConstInt(1) None)) Designation([MemberDesignator(\"l\")] Constant(ConstInt(2) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestNormalize::test_union (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestNormalize::test_union\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"c\")] Constant(ConstInt(1) None)) Designation([MemberDesignator(\"l\")] Constant(ConstInt(2) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Constant(ConstInt(2) None)] None \"l\") Union(\"u\") None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_designated_init.py::TestNormalize::test_union (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestNormalize::test_union\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nunion u v = {1, .i = 2};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Constant(ConstInt(1) None) Designation([MemberDesignator(\"i\")] Constant(ConstInt(2) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestNormalize::test_union (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestNormalize::test_union\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Constant(ConstInt(1) None) Designation([MemberDesignator(\"i\")] Constant(ConstInt(2) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Constant(ConstInt(1) None) Designation([MemberDesignator(\"i\")] Constant(ConstInt(2) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestNormalize::test_union (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestNormalize::test_union\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Constant(ConstInt(1) None) Designation([MemberDesignator(\"i\")] Constant(ConstInt(2) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Constant(ConstInt(2) None)] None \"i\") Union(\"u\") None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_designated_init.py::TestNormalize::test_union (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestNormalize::test_union\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nstruct w v = {.v.l = 4, .tag = 1};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"v\") MemberDesignator(\"l\")] Constant(ConstInt(4) None)) Designation([MemberDesignator(\"tag\")] Constant(ConstInt(1) None))] None None) Structure(\"w\") None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestNormalize::test_union (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestNormalize::test_union\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"v\") MemberDesignator(\"l\")] Constant(ConstInt(4) None)) Designation([MemberDesignator(\"tag\")] Constant(ConstInt(1) None))] None None) Structure(\"w\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"v\") MemberDesignator(\"l\")] Constant(ConstInt(4) None)) Designation([MemberDesignator(\"tag\")] Constant(ConstInt(1) None))] None None) Structure(\"w\") None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestNormalize::test_union (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestNormalize::test_union\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"v\") MemberDesignator(\"l\")] Constant(ConstInt(4) None)) Designation([MemberDesignator(\"tag\")] Constant(ConstInt(1) None))] None None) Structure(\"w\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Constant(ConstInt(1) None) InitList([Constant(ConstInt(4) None)] None \"l\")] None None) Structure(\"w\") None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_designated_init.py::TestNormalize::test_union_value_trees (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestNormalize::test_union_value_trees\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nunion u v = {.i = 5};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"i\")] Constant(ConstInt(5) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestNormalize::test_union_value_trees (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestNormalize::test_union_value_trees\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"i\")] Constant(ConstInt(5) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"i\")] Constant(ConstInt(5) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestNormalize::test_union_value_trees (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestNormalize::test_union_value_trees\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"i\")] Constant(ConstInt(5) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Constant(ConstInt(5) None)] None \"i\") Union(\"u\") None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_designated_init.py::TestNormalize::test_union_value_trees (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_designated_init.py::TestNormalize::test_union_value_trees\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"p\\\" false [member_decl(\\\"x\\\" Int) member_decl(\\\"y\\\" Int)]))\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"n\\\" Char) member_decl(\\\"pt\\\" Array(Structure(\\\"p\\\") 2)) member_decl(\\\"name\\\" Array(Char 4))]))\\n  StructDecl(struct_decl(\\\"u\\\" true [member_decl(\\\"c\\\" Char) member_decl(\\\"l\\\" Long) member_decl(\\\"i\\\" Int)]))\\n  StructDecl(struct_decl(\\\"w\\\" false [member_decl(\\\"tag\\\" Int) member_decl(\\\"v\\\" Union(\\\"u\\\"))]))\\n  VarDecl(var_decl(\\\"v\\\" InitList([Constant(ConstInt(5) Int)] Union(\\\"u\\\") \\\"i\\\") Union(\\\"u\\\") None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Constant(ConstInt(5) None)] None \"i\") Union(\"u\") None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"p\\\" false [member_decl(\\\"x\\\" Int) member_decl(\\\"y\\\" Int)]))\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"n\\\" Char) member_decl(\\\"pt\\\" Array(Structure(\\\"p\\\") 2)) member_decl(\\\"name\\\" Array(Char 4))]))\\n  StructDecl(struct_decl(\\\"u\\\" true [member_decl(\\\"c\\\" Char) member_decl(\\\"l\\\" Long) member_decl(\\\"i\\\" Int)]))\\n  StructDecl(struct_decl(\\\"w\\\" false [member_decl(\\\"tag\\\" Int) member_decl(\\\"v\\\" Union(\\\"u\\\"))]))\\n  VarDecl(var_decl(\\\"v\\\" InitList([Constant(ConstInt(5) Int)] Union(\\\"u\\\") \\\"i\\\") Union(\\\"u\\\") None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Union(\\\"u\\\")\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"object\": \"passes.type_checking.UnionValue\", \"fields\": {\"member\": 2, \"value\": 5}}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"p\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"x\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"y\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 2}}], \"size\": 4, \"complete\": true}}], [\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"n\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"pt\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Structure(\\\"p\\\") 2)\\n\"}, \"byte_offset\": 1}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"name\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 4)\\n\"}, \"byte_offset\": 9}}], \"size\": 13, \"complete\": true}}], [\"u\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"u\", \"is_union\": true, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"c\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"l\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"i\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 0}}], \"size\": 4, \"complete\": true}}], [\"w\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"w\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"tag\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"v\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Union(\\\"u\\\")\\n\"}, \"byte_offset\": 2}}], \"size\": 6, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_designated_init.py::TestNormalize::test_union_value_trees (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestNormalize::test_union_value_trees\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nunion u v = {.c = 5};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"c\")] Constant(ConstInt(5) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestNormalize::test_union_value_trees (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestNormalize::test_union_value_trees\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"c\")] Constant(ConstInt(5) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"c\")] Constant(ConstInt(5) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestNormalize::test_union_value_trees (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestNormalize::test_union_value_trees\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"c\")] Constant(ConstInt(5) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Constant(ConstInt(5) None)] None \"c\") Union(\"u\") None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_designated_init.py::TestNormalize::test_union_value_trees (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_designated_init.py::TestNormalize::test_union_value_trees\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"p\\\" false [member_decl(\\\"x\\\" Int) member_decl(\\\"y\\\" Int)]))\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"n\\\" Char) member_decl(\\\"pt\\\" Array(Structure(\\\"p\\\") 2)) member_decl(\\\"name\\\" Array(Char 4))]))\\n  StructDecl(struct_decl(\\\"u\\\" true [member_decl(\\\"c\\\" Char) member_decl(\\\"l\\\" Long) member_decl(\\\"i\\\" Int)]))\\n  StructDecl(struct_decl(\\\"w\\\" false [member_decl(\\\"tag\\\" Int) member_decl(\\\"v\\\" Union(\\\"u\\\"))]))\\n  VarDecl(var_decl(\\\"v\\\" InitList([Cast(Char Constant(ConstInt(5) Int) Char)] Union(\\\"u\\\") \\\"c\\\") Union(\\\"u\\\") None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Constant(ConstInt(5) None)] None \"c\") Union(\"u\") None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"p\\\" false [member_decl(\\\"x\\\" Int) member_decl(\\\"y\\\" Int)]))\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"n\\\" Char) member_decl(\\\"pt\\\" Array(Structure(\\\"p\\\") 2)) member_decl(\\\"name\\\" Array(Char 4))]))\\n  StructDecl(struct_decl(\\\"u\\\" true [member_decl(\\\"c\\\" Char) member_decl(\\\"l\\\" Long) member_decl(\\\"i\\\" Int)]))\\n  StructDecl(struct_decl(\\\"w\\\" false [member_decl(\\\"tag\\\" Int) member_decl(\\\"v\\\" Union(\\\"u\\\"))]))\\n  VarDecl(var_decl(\\\"v\\\" InitList([Cast(Char Constant(ConstInt(5) Int) Char)] Union(\\\"u\\\") \\\"c\\\") Union(\\\"u\\\") None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Union(\\\"u\\\")\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [5]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"p\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"x\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"y\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 2}}], \"size\": 4, \"complete\": true}}], [\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"n\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"pt\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Structure(\\\"p\\\") 2)\\n\"}, \"byte_offset\": 1}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"name\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 4)\\n\"}, \"byte_offset\": 9}}], \"size\": 13, \"complete\": true}}], [\"u\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"u\", \"is_union\": true, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"c\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"l\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"i\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 0}}], \"size\": 4, \"complete\": true}}], [\"w\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"w\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"tag\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"v\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Union(\\\"u\\\")\\n\"}, \"byte_offset\": 2}}], \"size\": 6, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_designated_init.py::TestNormalize::test_union_value_trees (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestNormalize::test_union_value_trees\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nstruct w v = {.v.l = 4, .tag = 1};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"v\") MemberDesignator(\"l\")] Constant(ConstInt(4) None)) Designation([MemberDesignator(\"tag\")] Constant(ConstInt(1) None))] None None) Structure(\"w\") None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestNormalize::test_union_value_trees (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestNormalize::test_union_value_trees\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"v\") MemberDesignator(\"l\")] Constant(ConstInt(4) None)) Designation([MemberDesignator(\"tag\")] Constant(ConstInt(1) None))] None None) Structure(\"w\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"v\") MemberDesignator(\"l\")] Constant(ConstInt(4) None)) Designation([MemberDesignator(\"tag\")] Constant(ConstInt(1) None))] None None) Structure(\"w\") None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestNormalize::test_union_value_trees (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestNormalize::test_union_value_trees\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"v\") MemberDesignator(\"l\")] Constant(ConstInt(4) None)) Designation([MemberDesignator(\"tag\")] Constant(ConstInt(1) None))] None None) Structure(\"w\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Constant(ConstInt(1) None) InitList([Constant(ConstInt(4) None)] None \"l\")] None None) Structure(\"w\") None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_designated_init.py::TestNormalize::test_union_value_trees (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_designated_init.py::TestNormalize::test_union_value_trees\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"p\\\" false [member_decl(\\\"x\\\" Int) member_decl(\\\"y\\\" Int)]))\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"n\\\" Char) member_decl(\\\"pt\\\" Array(Structure(\\\"p\\\") 2)) member_decl(\\\"name\\\" Array(Char 4))]))\\n  StructDecl(struct_decl(\\\"u\\\" true [member_decl(\\\"c\\\" Char) member_decl(\\\"l\\\" Long) member_decl(\\\"i\\\" Int)]))\\n  StructDecl(struct_decl(\\\"w\\\" false [member_decl(\\\"tag\\\" Int) member_decl(\\\"v\\\" Union(\\\"u\\\"))]))\\n  VarDecl(var_decl(\\\"v\\\" InitList([Constant(ConstInt(1) Int) InitList([Cast(Long Constant(ConstInt(4) Int) Long)] Union(\\\"u\\\") \\\"l\\\")] Structure(\\\"w\\\") None) Structure(\\\"w\\\") None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Constant(ConstInt(1) None) InitList([Constant(ConstInt(4) None)] None \"l\")] None None) Structure(\"w\") None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"p\\\" false [member_decl(\\\"x\\\" Int) member_decl(\\\"y\\\" Int)]))\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"n\\\" Char) member_decl(\\\"pt\\\" Array(Structure(\\\"p\\\") 2)) member_decl(\\\"name\\\" Array(Char 4))]))\\n  StructDecl(struct_decl(\\\"u\\\" true [member_decl(\\\"c\\\" Char) member_decl(\\\"l\\\" Long) member_decl(\\\"i\\\" Int)]))\\n  StructDecl(struct_decl(\\\"w\\\" false [member_decl(\\\"tag\\\" Int) member_decl(\\\"v\\\" Union(\\\"u\\\"))]))\\n  VarDecl(var_decl(\\\"v\\\" InitList([Constant(ConstInt(1) Int) InitList([Cast(Long Constant(ConstInt(4) Int) Long)] Union(\\\"u\\\") \\\"l\\\")] Structure(\\\"w\\\") None) Structure(\\\"w\\\") None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Structure(\\\"w\\\")\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [1, {\"object\": \"passes.type_checking.UnionValue\", \"fields\": {\"member\": 1, \"value\": 4}}]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"p\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"x\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"y\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 2}}], \"size\": 4, \"complete\": true}}], [\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"n\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"pt\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Structure(\\\"p\\\") 2)\\n\"}, \"byte_offset\": 1}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"name\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 4)\\n\"}, \"byte_offset\": 9}}], \"size\": 13, \"complete\": true}}], [\"u\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"u\", \"is_union\": true, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"c\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"l\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"i\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 0}}], \"size\": 4, \"complete\": true}}], [\"w\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"w\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"tag\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"v\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Union(\\\"u\\\")\\n\"}, \"byte_offset\": 2}}], \"size\": 6, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_designated_init.py::TestNormalize::test_unknown_size (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestNormalize::test_unknown_size\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[] = {[4] = 7, [1] = 3};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(4)] Constant(ConstInt(7) None)) Designation([IndexDesignator(1)] Constant(ConstInt(3) None))] None None) Array(Int 0) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestNormalize::test_unknown_size (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestNormalize::test_unknown_size\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(4)] Constant(ConstInt(7) None)) Designation([IndexDesignator(1)] Constant(ConstInt(3) None))] None None) Array(Int 0) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(4)] Constant(ConstInt(7) None)) Designation([IndexDesignator(1)] Constant(ConstInt(3) None))] None None) Array(Int 0) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestNormalize::test_unknown_size (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestNormalize::test_unknown_size\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(4)] Constant(ConstInt(7) None)) Designation([IndexDesignator(1)] Constant(ConstInt(3) None))] None None) Array(Int 0) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([InitList([] None None) Constant(ConstInt(3) None) InitList([] None None) InitList([] None None) Constant(ConstInt(7) None)] None None) Array(Int 0) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_designated_init.py::TestNormalize::test_unknown_size (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestNormalize::test_unknown_size\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[] = {[4] = 7, [1] = 3, 8};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(4)] Constant(ConstInt(7) None)) Designation([IndexDesignator(1)] Constant(ConstInt(3) None)) Constant(ConstInt(8) None)] None None) Array(Int 0) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestNormalize::test_unknown_size (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestNormalize::test_unknown_size\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(4)] Constant(ConstInt(7) None)) Designation([IndexDesignator(1)] Constant(ConstInt(3) None)) Constant(ConstInt(8) None)] None None) Array(Int 0) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(4)] Constant(ConstInt(7) None)) Designation([IndexDesignator(1)] Constant(ConstInt(3) None)) Constant(ConstInt(8) None)] None None) Array(Int 0) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestNormalize::test_unknown_size (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestNormalize::test_unknown_size\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(4)] Constant(ConstInt(7) None)) Designation([IndexDesignator(1)] Constant(ConstInt(3) None)) Constant(ConstInt(8) None)] None None) Array(Int 0) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([InitList([] None None) Constant(ConstInt(3) None) Constant(ConstInt(8) None) InitList([] None None) Constant(ConstInt(7) None)] None None) Array(Int 0) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_designated_init.py::TestNormalize::test_unknown_size (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_designated_init.py::TestNormalize::test_unknown_size\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" InitList([Constant(ConstInt(0) Int) Constant(ConstInt(3) Int) Constant(ConstInt(8) Int) Constant(ConstInt(0) Int) Constant(ConstInt(7) Int)] Array(Int 5) None) Array(Int 5) None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([InitList([] None None) Constant(ConstInt(3) None) Constant(ConstInt(8) None) InitList([] None None) Constant(ConstInt(7) None)] None None) Array(Int 0) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" InitList([Constant(ConstInt(0) Int) Constant(ConstInt(3) Int) Constant(ConstInt(8) Int) Constant(ConstInt(0) Int) Constant(ConstInt(7) Int)] Array(Int 5) None) Array(Int 5) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0, 3, 8, 0, 7]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_designated_init.py::TestNormalize::test_unknown_size (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestNormalize::test_unknown_size\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[] = {[9] = 1, [2] = 3};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(9)] Constant(ConstInt(1) None)) Designation([IndexDesignator(2)] Constant(ConstInt(3) None))] None None) Array(Int 0) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestNormalize::test_unknown_size (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestNormalize::test_unknown_size\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(9)] Constant(ConstInt(1) None)) Designation([IndexDesignator(2)] Constant(ConstInt(3) None))] None None) Array(Int 0) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(9)] Constant(ConstInt(1) None)) Designation([IndexDesignator(2)] Constant(ConstInt(3) None))] None None) Array(Int 0) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestNormalize::test_unknown_size (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestNormalize::test_unknown_size\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(9)] Constant(ConstInt(1) None)) Designation([IndexDesignator(2)] Constant(ConstInt(3) None))] None None) Array(Int 0) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([InitList([] None None) InitList([] None None) Constant(ConstInt(3) None) InitList([] None None) InitList([] None None) InitList([] None None) InitList([] None None) InitList([] None None) InitList([] None None) Constant(ConstInt(1) None)] None None) Array(Int 0) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_designated_init.py::TestNormalize::test_unknown_size (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_designated_init.py::TestNormalize::test_unknown_size\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" InitList([Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(3) Int) Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(1) Int)] Array(Int 10) None) Array(Int 10) None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([InitList([] None None) InitList([] None None) Constant(ConstInt(3) None) InitList([] None None) InitList([] None None) InitList([] None None) InitList([] None None) InitList([] None None) InitList([] None None) Constant(ConstInt(1) None)] None None) Array(Int 0) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" InitList([Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(3) Int) Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(0) Int) Constant(ConstInt(1) Int)] Array(Int 10) None) Array(Int 10) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0, 0, 3, 0, 0, 0, 0, 0, 0, 1]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_brace_elision (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_brace_elision\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nstruct s v = {.pt[0].x = 1, 2};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\") IndexDesignator(0) MemberDesignator(\"x\")] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Structure(\"s\") None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_brace_elision (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_brace_elision\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\") IndexDesignator(0) MemberDesignator(\"x\")] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Structure(\"s\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\") IndexDesignator(0) MemberDesignator(\"x\")] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Structure(\"s\") None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_brace_elision (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_brace_elision\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\") IndexDesignator(0) MemberDesignator(\"x\")] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Structure(\"s\") None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"in the initializer of `v`, the item after `.pt[0].x = ...` would initialize the subobject after `v.pt[0].x` inside `v.pt`, which takes brace elision, and c6502 doesn't elide braces. Put the initializers of `v.pt` in braces of their own, as in `.pt = {...}`\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_brace_elision (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_brace_elision\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[2][2] = {[0][1] = 1, 2};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0) IndexDesignator(1)] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Array(Array(Int 2) 2) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_brace_elision (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_brace_elision\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0) IndexDesignator(1)] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Array(Array(Int 2) 2) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0) IndexDesignator(1)] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Array(Array(Int 2) 2) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_brace_elision (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_brace_elision\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0) IndexDesignator(1)] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Array(Array(Int 2) 2) None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"in the initializer of `a`, the item after `[0][1] = ...` would initialize the subobject after `a[0][1]` inside `a[0]`, which takes brace elision, and c6502 doesn't elide braces. Put the initializers of `a[0]` in braces of their own, as in `[0] = {...}`\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_designation_outside_list (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_designation_outside_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int g(void) { return 1; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_designation_outside_list (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_designation_outside_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_designated_init.py::TestErrors::test_designation_outside_list (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_designated_init.py::TestErrors::test_designation_outside_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    S(Return(Designation([IndexDesignator(0)] Constant(ConstInt(1) None))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"a designator such as `[2] =` or `.x =` goes only in front of an item of a braced initializer\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_designator_in_scalar (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_designator_in_scalar\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int x = {.x = 1};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" InitList([Designation([MemberDesignator(\"x\")] Constant(ConstInt(1) None))] None None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_designator_in_scalar (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_designator_in_scalar\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" InitList([Designation([MemberDesignator(\"x\")] Constant(ConstInt(1) None))] None None) Int None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" InitList([Designation([MemberDesignator(\"x\")] Constant(ConstInt(1) None))] None None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_designator_in_scalar (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_designator_in_scalar\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" InitList([Designation([MemberDesignator(\"x\")] Constant(ConstInt(1) None))] None None) Int None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"`x` is a scalar of type `int`, so its initializer can't have the designator `.x`, which names an element of an array or a member of a struct or union (C23 \\u00a76.7.11p6-7)\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_designator_in_scalar (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_designator_in_scalar\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int x = {[0] = 1};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" InitList([Designation([IndexDesignator(0)] Constant(ConstInt(1) None))] None None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_designator_in_scalar (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_designator_in_scalar\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" InitList([Designation([IndexDesignator(0)] Constant(ConstInt(1) None))] None None) Int None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" InitList([Designation([IndexDesignator(0)] Constant(ConstInt(1) None))] None None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_designator_in_scalar (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_designator_in_scalar\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" InitList([Designation([IndexDesignator(0)] Constant(ConstInt(1) None))] None None) Int None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"`x` is a scalar of type `int`, so its initializer can't have the designator `[0]`, which names an element of an array or a member of a struct or union (C23 \\u00a76.7.11p6-7)\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_designator_in_scalar (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_designator_in_scalar\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[2] = {[0] = {[0] = 1}};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0)] InitList([Designation([IndexDesignator(0)] Constant(ConstInt(1) None))] None None))] None None) Array(Int 2) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_designator_in_scalar (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_designator_in_scalar\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0)] InitList([Designation([IndexDesignator(0)] Constant(ConstInt(1) None))] None None))] None None) Array(Int 2) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0)] InitList([Designation([IndexDesignator(0)] Constant(ConstInt(1) None))] None None))] None None) Array(Int 2) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_designator_in_scalar (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_designator_in_scalar\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0)] InitList([Designation([IndexDesignator(0)] Constant(ConstInt(1) None))] None None))] None None) Array(Int 2) None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"`a[0]` is a scalar of type `int`, so its initializer can't have the designator `[0]`, which names an element of an array or a member of a struct or union (C23 \\u00a76.7.11p6-7)\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_designator_in_scalar (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_designator_in_scalar\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void) { return (int){.x = 1}; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"\" Int InitList([Designation([MemberDesignator(\"x\")] Constant(ConstInt(1) None))] None None) None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_designator_in_scalar (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_designator_in_scalar\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"\" Int InitList([Designation([MemberDesignator(\"x\")] Constant(ConstInt(1) None))] None None) None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Int InitList([Designation([MemberDesignator(\"x\")] Constant(ConstInt(1) None))] None None) None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_designator_in_scalar (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_designator_in_scalar\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Int InitList([Designation([MemberDesignator(\"x\")] Constant(ConstInt(1) None))] None None) None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"`(int){...}` is a scalar of type `int`, so its initializer can't have the designator `.x`, which names an element of an array or a member of a struct or union (C23 \\u00a76.7.11p6-7)\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_index_on_non_array (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_index_on_non_array\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nstruct p q = {[0] = 1};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"q\" InitList([Designation([IndexDesignator(0)] Constant(ConstInt(1) None))] None None) Structure(\"p\") None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_index_on_non_array (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_index_on_non_array\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"q\" InitList([Designation([IndexDesignator(0)] Constant(ConstInt(1) None))] None None) Structure(\"p\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"q\" InitList([Designation([IndexDesignator(0)] Constant(ConstInt(1) None))] None None) Structure(\"p\") None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_index_on_non_array (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_index_on_non_array\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"q\" InitList([Designation([IndexDesignator(0)] Constant(ConstInt(1) None))] None None) Structure(\"p\") None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the designator `[0]` names an element of an array, and `q` has type `struct p` (C23 \\u00a76.7.11p6)\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_index_on_non_array (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_index_on_non_array\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nstruct s v = {.n[0] = 1};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"n\") IndexDesignator(0)] Constant(ConstInt(1) None))] None None) Structure(\"s\") None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_index_on_non_array (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_index_on_non_array\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"n\") IndexDesignator(0)] Constant(ConstInt(1) None))] None None) Structure(\"s\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"n\") IndexDesignator(0)] Constant(ConstInt(1) None))] None None) Structure(\"s\") None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_index_on_non_array (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_index_on_non_array\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"n\") IndexDesignator(0)] Constant(ConstInt(1) None))] None None) Structure(\"s\") None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the designator `[0]` names an element of an array, and `v.n` has type `char` (C23 \\u00a76.7.11p6)\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_index_past_the_end (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_index_past_the_end\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[3] = {[3] = 1};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(3)] Constant(ConstInt(1) None))] None None) Array(Int 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_index_past_the_end (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_index_past_the_end\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(3)] Constant(ConstInt(1) None))] None None) Array(Int 3) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(3)] Constant(ConstInt(1) None))] None None) Array(Int 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_index_past_the_end (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_index_past_the_end\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(3)] Constant(ConstInt(1) None))] None None) Array(Int 3) None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the designator `[3]` is past the end of `a`, an array of 3 elements (C23 \\u00a76.7.11p6)\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_index_past_the_end (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_index_past_the_end\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nstruct s v = {.pt[2].x = 1};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\") IndexDesignator(2) MemberDesignator(\"x\")] Constant(ConstInt(1) None))] None None) Structure(\"s\") None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_index_past_the_end (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_index_past_the_end\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\") IndexDesignator(2) MemberDesignator(\"x\")] Constant(ConstInt(1) None))] None None) Structure(\"s\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\") IndexDesignator(2) MemberDesignator(\"x\")] Constant(ConstInt(1) None))] None None) Structure(\"s\") None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_index_past_the_end (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_index_past_the_end\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\") IndexDesignator(2) MemberDesignator(\"x\")] Constant(ConstInt(1) None))] None None) Structure(\"s\") None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the designator `[2]` is past the end of `v.pt`, an array of 2 elements (C23 \\u00a76.7.11p6)\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_member_on_non_struct (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_member_on_non_struct\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[3] = {.x = 1};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([MemberDesignator(\"x\")] Constant(ConstInt(1) None))] None None) Array(Int 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_member_on_non_struct (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_member_on_non_struct\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([MemberDesignator(\"x\")] Constant(ConstInt(1) None))] None None) Array(Int 3) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([MemberDesignator(\"x\")] Constant(ConstInt(1) None))] None None) Array(Int 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_member_on_non_struct (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_member_on_non_struct\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([MemberDesignator(\"x\")] Constant(ConstInt(1) None))] None None) Array(Int 3) None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the designator `.x` names a member of a struct or union, and `a` has type `int[3]` (C23 \\u00a76.7.11p7)\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_member_on_non_struct (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_member_on_non_struct\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nstruct p q = {.x.y = 1};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"q\" InitList([Designation([MemberDesignator(\"x\") MemberDesignator(\"y\")] Constant(ConstInt(1) None))] None None) Structure(\"p\") None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_member_on_non_struct (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_member_on_non_struct\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"q\" InitList([Designation([MemberDesignator(\"x\") MemberDesignator(\"y\")] Constant(ConstInt(1) None))] None None) Structure(\"p\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"q\" InitList([Designation([MemberDesignator(\"x\") MemberDesignator(\"y\")] Constant(ConstInt(1) None))] None None) Structure(\"p\") None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_member_on_non_struct (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_member_on_non_struct\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"q\" InitList([Designation([MemberDesignator(\"x\") MemberDesignator(\"y\")] Constant(ConstInt(1) None))] None None) Structure(\"p\") None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the designator `.y` names a member of a struct or union, and `q.x` has type `int` (C23 \\u00a76.7.11p7)\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_overridden_side_effects (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void);\\nint g(void) { int n = 0; int a[3] = {[0] = f(), [0] = 2}; return a[0]; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"n\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0)] FunctionCall(\"f\" [] None false)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_overridden_side_effects (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"n\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0)] FunctionCall(\"f\" [] None false)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.n\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.a\" InitList([Designation([IndexDesignator(0)] FunctionCall(\"f\" [] None false)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@1.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_overridden_side_effects (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.n\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.a\" InitList([Designation([IndexDesignator(0)] FunctionCall(\"f\" [] None false)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@1.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"a later designator overrides the initializer of `a[0]`, which has side effects. C leaves it unspecified whether they happen (C23 \\u00a76.7.11p19), and c6502 doesn't evaluate an overridden initializer, so it rejects one with side effects\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_overridden_side_effects (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void);\\nint g(void) { int n = 0; int a[3] = {[0] = n++, [0] = 2}; return a[0]; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"n\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0)] Postfix(Increment Var(\"n\" None) None)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_overridden_side_effects (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"n\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0)] Postfix(Increment Var(\"n\" None) None)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.n\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.a\" InitList([Designation([IndexDesignator(0)] Postfix(Increment Var(\"@0.n\" None) None)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@1.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_overridden_side_effects (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.n\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.a\" InitList([Designation([IndexDesignator(0)] Postfix(Increment Var(\"@0.n\" None) None)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@1.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"a later designator overrides the initializer of `a[0]`, which has side effects. C leaves it unspecified whether they happen (C23 \\u00a76.7.11p19), and c6502 doesn't evaluate an overridden initializer, so it rejects one with side effects\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_overridden_side_effects (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void);\\nint g(void) { int n = 0; int a[3] = {[0] = n = 1, [0] = 2}; return a[0]; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"n\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0)] Assignment(Var(\"n\" None) Constant(ConstInt(1) None) None)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_overridden_side_effects (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"n\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0)] Assignment(Var(\"n\" None) Constant(ConstInt(1) None) None)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.n\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.a\" InitList([Designation([IndexDesignator(0)] Assignment(Var(\"@0.n\" None) Constant(ConstInt(1) None) None)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@1.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_overridden_side_effects (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.n\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.a\" InitList([Designation([IndexDesignator(0)] Assignment(Var(\"@0.n\" None) Constant(ConstInt(1) None) None)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@1.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"a later designator overrides the initializer of `a[0]`, which has side effects. C leaves it unspecified whether they happen (C23 \\u00a76.7.11p19), and c6502 doesn't evaluate an overridden initializer, so it rejects one with side effects\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_overridden_side_effects (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void);\\nint g(void) { int n = 0; int a[3] = {[0] = --n, [0] = 2}; return a[0]; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"n\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0)] Prefix(Decrement Var(\"n\" None) None)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_overridden_side_effects (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"n\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0)] Prefix(Decrement Var(\"n\" None) None)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.n\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.a\" InitList([Designation([IndexDesignator(0)] Prefix(Decrement Var(\"@0.n\" None) None)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@1.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_overridden_side_effects (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.n\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.a\" InitList([Designation([IndexDesignator(0)] Prefix(Decrement Var(\"@0.n\" None) None)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@1.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"a later designator overrides the initializer of `a[0]`, which has side effects. C leaves it unspecified whether they happen (C23 \\u00a76.7.11p19), and c6502 doesn't evaluate an overridden initializer, so it rejects one with side effects\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_overridden_side_effects (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void);\\nint g(void) { int n = 0; int a[3] = {[0] = n += 2, [0] = 2}; return a[0]; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"n\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0)] CompoundAssignment(Add Var(\"n\" None) Constant(ConstInt(2) None) None None)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_overridden_side_effects (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"n\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0)] CompoundAssignment(Add Var(\"n\" None) Constant(ConstInt(2) None) None None)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.n\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.a\" InitList([Designation([IndexDesignator(0)] CompoundAssignment(Add Var(\"@0.n\" None) Constant(ConstInt(2) None) None None)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@1.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_overridden_side_effects (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.n\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.a\" InitList([Designation([IndexDesignator(0)] CompoundAssignment(Add Var(\"@0.n\" None) Constant(ConstInt(2) None) None None)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@1.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"a later designator overrides the initializer of `a[0]`, which has side effects. C leaves it unspecified whether they happen (C23 \\u00a76.7.11p19), and c6502 doesn't evaluate an overridden initializer, so it rejects one with side effects\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_overridden_side_effects (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nint f(void);\\nint g(void) { struct s v = {.pt[0] = {f(), 1}, .pt[0].x = 2}; return v.n; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\") IndexDesignator(0)] InitList([FunctionCall(\"f\" [] None false) Constant(ConstInt(1) None)] None None)) Designation([MemberDesignator(\"pt\") IndexDesignator(0) MemberDesignator(\"x\")] Constant(ConstInt(2) None))] None None) Structure(\"s\") None None None None false)))\n"
      "    S(Return(Dot(Var(\"v\" None) \"n\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_overridden_side_effects (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"pt\") IndexDesignator(0)] InitList([FunctionCall(\"f\" [] None false) Constant(ConstInt(1) None)] None None)) Designation([MemberDesignator(\"pt\") IndexDesignator(0) MemberDesignator(\"x\")] Constant(ConstInt(2) None))] None None) Structure(\"s\") None None None None false)))\n"
      "    S(Return(Dot(Var(\"v\" None) \"n\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.v\" InitList([Designation([MemberDesignator(\"pt\") IndexDesignator(0)] InitList([FunctionCall(\"f\" [] None false) Constant(ConstInt(1) None)] None None)) Designation([MemberDesignator(\"pt\") IndexDesignator(0) MemberDesignator(\"x\")] Constant(ConstInt(2) None))] None None) Structure(\"s\") None None None None false)))\n"
      "    S(Return(Dot(Var(\"@0.v\" None) \"n\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_overridden_side_effects (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.v\" InitList([Designation([MemberDesignator(\"pt\") IndexDesignator(0)] InitList([FunctionCall(\"f\" [] None false) Constant(ConstInt(1) None)] None None)) Designation([MemberDesignator(\"pt\") IndexDesignator(0) MemberDesignator(\"x\")] Constant(ConstInt(2) None))] None None) Structure(\"s\") None None None None false)))\n"
      "    S(Return(Dot(Var(\"@0.v\" None) \"n\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"a later designator overrides the initializer of `v.pt[0].x`, which has side effects. C leaves it unspecified whether they happen (C23 \\u00a76.7.11p19), and c6502 doesn't evaluate an overridden initializer, so it rejects one with side effects\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_overridden_side_effects (call 19)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nint f(void);\\nint g(void) { union u v = {.c = f(), .l = 2}; return v.c; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"c\")] FunctionCall(\"f\" [] None false)) Designation([MemberDesignator(\"l\")] Constant(ConstInt(2) None))] None None) Union(\"u\") None None None None false)))\n"
      "    S(Return(Dot(Var(\"v\" None) \"c\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_overridden_side_effects (call 20)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"c\")] FunctionCall(\"f\" [] None false)) Designation([MemberDesignator(\"l\")] Constant(ConstInt(2) None))] None None) Union(\"u\") None None None None false)))\n"
      "    S(Return(Dot(Var(\"v\" None) \"c\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.v\" InitList([Designation([MemberDesignator(\"c\")] FunctionCall(\"f\" [] None false)) Designation([MemberDesignator(\"l\")] Constant(ConstInt(2) None))] None None) Union(\"u\") None None None None false)))\n"
      "    S(Return(Dot(Var(\"@0.v\" None) \"c\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_overridden_side_effects (call 21)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.v\" InitList([Designation([MemberDesignator(\"c\")] FunctionCall(\"f\" [] None false)) Designation([MemberDesignator(\"l\")] Constant(ConstInt(2) None))] None None) Union(\"u\") None None None None false)))\n"
      "    S(Return(Dot(Var(\"@0.v\" None) \"c\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"a later designator overrides the initializer of `v.c`, which has side effects. C leaves it unspecified whether they happen (C23 \\u00a76.7.11p19), and c6502 doesn't evaluate an overridden initializer, so it rejects one with side effects\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_overridden_side_effects (call 22)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void);\\nint g(void) { int a[3] = {[0] = sizeof(f()), [0] = 2}; return a[0]; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0)] SizeOfExp(FunctionCall(\"f\" [] None false) None)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_overridden_side_effects (call 23)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(0)] SizeOfExp(FunctionCall(\"f\" [] None false) None)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Designation([IndexDesignator(0)] SizeOfExp(FunctionCall(\"f\" [] None false) None)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_overridden_side_effects (call 24)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Designation([IndexDesignator(0)] SizeOfExp(FunctionCall(\"f\" [] None false) None)) Designation([IndexDesignator(0)] Constant(ConstInt(2) None))] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(2) None)] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_designated_init.py::TestErrors::test_overridden_side_effects (call 25)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_designated_init.py::TestErrors::test_overridden_side_effects\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] None FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Constant(ConstInt(2) Int)] Array(Int 3) None) Array(Int 3) None None None None false)))\\n    S(Return(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 3)) Pointer(Int)) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] None FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(2) None)] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] None FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Constant(ConstInt(2) Int)] Array(Int 3) None) Array(Int 3) None None None None false)))\\n    S(Return(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 3)) Pointer(Int)) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_partial_override_of_expression (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_partial_override_of_expression\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nstruct p p0;\\nstruct q { struct p a; };\\nint g(void) { struct q v = {.a = p0, .a.y = 2}; return 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"p0\" None Structure(\"p\") None None None None false))\n"
      "  StructDecl(struct_decl(\"q\" false [member_decl(\"a\" Structure(\"p\"))]))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"a\")] Var(\"p0\" None)) Designation([MemberDesignator(\"a\") MemberDesignator(\"y\")] Constant(ConstInt(2) None))] None None) Structure(\"q\") None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_partial_override_of_expression (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_partial_override_of_expression\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"p0\" None Structure(\"p\") None None None None false))\n"
      "  StructDecl(struct_decl(\"q\" false [member_decl(\"a\" Structure(\"p\"))]))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"a\")] Var(\"p0\" None)) Designation([MemberDesignator(\"a\") MemberDesignator(\"y\")] Constant(ConstInt(2) None))] None None) Structure(\"q\") None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"p0\" None Structure(\"p\") None None None None false))\n"
      "  StructDecl(struct_decl(\"q\" false [member_decl(\"a\" Structure(\"p\"))]))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.v\" InitList([Designation([MemberDesignator(\"a\")] Var(\"p0\" None)) Designation([MemberDesignator(\"a\") MemberDesignator(\"y\")] Constant(ConstInt(2) None))] None None) Structure(\"q\") None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_partial_override_of_expression (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_partial_override_of_expression\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"p0\" None Structure(\"p\") None None None None false))\n"
      "  StructDecl(struct_decl(\"q\" false [member_decl(\"a\" Structure(\"p\"))]))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.v\" InitList([Designation([MemberDesignator(\"a\")] Var(\"p0\" None)) Designation([MemberDesignator(\"a\") MemberDesignator(\"y\")] Constant(ConstInt(2) None))] None None) Structure(\"q\") None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"a designator overrides part of `v.a`, which an expression initializes, and c6502 can only override part of a subobject that a braced list initializes. Initialize `v.a` with a braced list instead\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_positional_after_union_member (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_positional_after_union_member\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nunion u v = {.c = 1, 2};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"c\")] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Union(\"u\") None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_positional_after_union_member (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_positional_after_union_member\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"c\")] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Union(\"u\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"c\")] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Union(\"u\") None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_positional_after_union_member (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_positional_after_union_member\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"c\")] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Union(\"u\") None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"too many initializers for union `v`: a union's list initializes one member, and an item without a designator can't follow another item (C23 \\u00a76.7.11p17)\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_positional_past_the_end (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_positional_past_the_end\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[3] = {[2] = 1, 2};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(2)] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Array(Int 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_positional_past_the_end (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_positional_past_the_end\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(2)] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Array(Int 3) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(2)] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Array(Int 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_positional_past_the_end (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_positional_past_the_end\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Designation([IndexDesignator(2)] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Array(Int 3) None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"too many initializers for array `a`: the item for index 3 is past the end of an array of 3 elements\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_positional_past_the_end (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_positional_past_the_end\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nstruct p q = {.y = 1, 2};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"q\" InitList([Designation([MemberDesignator(\"y\")] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Structure(\"p\") None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_positional_past_the_end (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_positional_past_the_end\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"q\" InitList([Designation([MemberDesignator(\"y\")] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Structure(\"p\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"q\" InitList([Designation([MemberDesignator(\"y\")] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Structure(\"p\") None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_positional_past_the_end (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_positional_past_the_end\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"q\" InitList([Designation([MemberDesignator(\"y\")] Constant(ConstInt(1) None)) Constant(ConstInt(2) None)] None None) Structure(\"p\") None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"too many initializers for struct `q`: the struct has 2 members, and the item after the last one has no member to initialize\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_unknown_member (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_unknown_member\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nstruct p q = {.z = 1};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"q\" InitList([Designation([MemberDesignator(\"z\")] Constant(ConstInt(1) None))] None None) Structure(\"p\") None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_unknown_member (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_unknown_member\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"q\" InitList([Designation([MemberDesignator(\"z\")] Constant(ConstInt(1) None))] None None) Structure(\"p\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"q\" InitList([Designation([MemberDesignator(\"z\")] Constant(ConstInt(1) None))] None None) Structure(\"p\") None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_unknown_member (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_unknown_member\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"q\" InitList([Designation([MemberDesignator(\"z\")] Constant(ConstInt(1) None))] None None) Structure(\"p\") None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the designator `.z` names no member of `q`, which has type `struct p` (C23 \\u00a76.7.11p7)\"}\n");
}

TEST_CASE("parse: test_designated_init.py::TestErrors::test_unknown_member (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_designated_init.py::TestErrors::test_unknown_member\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int x; int y; };\\nstruct s { char n; struct p pt[2]; char name[4]; };\\nunion u { char c; long l; int i; };\\nstruct w { int tag; union u v; };\\nunion u v = {.x = 1};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"x\")] Constant(ConstInt(1) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_designated_init.py::TestErrors::test_unknown_member (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_designated_init.py::TestErrors::test_unknown_member\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"x\")] Constant(ConstInt(1) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"x\")] Constant(ConstInt(1) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_designated_init.py::TestErrors::test_unknown_member (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_designated_init.py::TestErrors::test_unknown_member\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"n\" Char) member_decl(\"pt\" Array(Structure(\"p\") 2)) member_decl(\"name\" Array(Char 4))]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long) member_decl(\"i\" Int)]))\n"
      "  StructDecl(struct_decl(\"w\" false [member_decl(\"tag\" Int) member_decl(\"v\" Union(\"u\"))]))\n"
      "  VarDecl(var_decl(\"v\" InitList([Designation([MemberDesignator(\"x\")] Constant(ConstInt(1) None))] None None) Union(\"u\") None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the designator `.x` names no member of `v`, which has type `union u` (C23 \\u00a76.7.11p7)\"}\n");
}
