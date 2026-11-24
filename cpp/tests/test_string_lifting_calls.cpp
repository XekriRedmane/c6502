// The calls that the tests of tests/test_string_lifting.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_string_lifting.py::TestStringLiftingDirect::test_string_in_char_array_init_stays_inline (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_string_lifting.py::TestStringLiftingDirect::test_string_in_char_array_init_stays_inline\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"char arr[4] = \\\"abc\\\";\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(Char 4) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_string_lifting.py::TestStringLiftingDirect::test_string_in_char_array_init_stays_inline (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_string_lifting.py::TestStringLiftingDirect::test_string_in_char_array_init_stays_inline\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(Char 4) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(Char 4) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_string_lifting.py::TestStringLiftingDirect::test_string_in_char_array_init_stays_inline (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_string_lifting.py::TestStringLiftingDirect::test_string_in_char_array_init_stays_inline\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(Char 4) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(Char 4) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_string_lifting.py::TestStringLiftingDirect::test_string_in_int_array_init_is_hoisted (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_string_lifting.py::TestStringLiftingDirect::test_string_in_int_array_init_is_hoisted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int arr[4] = \\\"abc\\\";\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(Int 4) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_string_lifting.py::TestStringLiftingDirect::test_string_in_int_array_init_is_hoisted (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_string_lifting.py::TestStringLiftingDirect::test_string_in_int_array_init_is_hoisted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(Int 4) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(Int 4) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_string_lifting.py::TestStringLiftingDirect::test_string_in_int_array_init_is_hoisted (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_string_lifting.py::TestStringLiftingDirect::test_string_in_int_array_init_is_hoisted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(Int 4) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"str@0\" String(\"abc\") Array(Char 4) Static None None None false))\n"
      "  VarDecl(var_decl(\"arr\" Var(\"str@0\" None) Array(Int 4) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_string_lifting.py::TestStringLiftingDirect::test_string_in_pointer_init_is_hoisted (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_string_lifting.py::TestStringLiftingDirect::test_string_in_pointer_init_is_hoisted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"static char *p = \\\"abc\\\";\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" String(\"abc\") Pointer(Char) Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_string_lifting.py::TestStringLiftingDirect::test_string_in_pointer_init_is_hoisted (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_string_lifting.py::TestStringLiftingDirect::test_string_in_pointer_init_is_hoisted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" String(\"abc\") Pointer(Char) Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" String(\"abc\") Pointer(Char) Static None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_string_lifting.py::TestStringLiftingDirect::test_string_in_pointer_init_is_hoisted (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_string_lifting.py::TestStringLiftingDirect::test_string_in_pointer_init_is_hoisted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" String(\"abc\") Pointer(Char) Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"str@0\" String(\"abc\") Array(Char 4) Static None None None false))\n"
      "  VarDecl(var_decl(\"p\" Var(\"str@0\" None) Pointer(Char) Static None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_string_lifting.py::TestStringLiftingDirect::test_string_in_signed_char_array_stays_inline (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_string_lifting.py::TestStringLiftingDirect::test_string_in_signed_char_array_stays_inline\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"signed char arr[4] = \\\"abc\\\";\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(SChar 4) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_string_lifting.py::TestStringLiftingDirect::test_string_in_signed_char_array_stays_inline (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_string_lifting.py::TestStringLiftingDirect::test_string_in_signed_char_array_stays_inline\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(SChar 4) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(SChar 4) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_string_lifting.py::TestStringLiftingDirect::test_string_in_signed_char_array_stays_inline (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_string_lifting.py::TestStringLiftingDirect::test_string_in_signed_char_array_stays_inline\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(SChar 4) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(SChar 4) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_string_lifting.py::TestStringLiftingDirect::test_string_in_unsigned_char_array_stays_inline (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_string_lifting.py::TestStringLiftingDirect::test_string_in_unsigned_char_array_stays_inline\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"unsigned char arr[4] = \\\"abc\\\";\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(UChar 4) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_string_lifting.py::TestStringLiftingDirect::test_string_in_unsigned_char_array_stays_inline (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_string_lifting.py::TestStringLiftingDirect::test_string_in_unsigned_char_array_stays_inline\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(UChar 4) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(UChar 4) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_string_lifting.py::TestStringLiftingDirect::test_string_in_unsigned_char_array_stays_inline (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_string_lifting.py::TestStringLiftingDirect::test_string_in_unsigned_char_array_stays_inline\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(UChar 4) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(UChar 4) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_string_lifting.py::TestStringLiftingInExpressions::test_address_of_string_lifts (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_string_lifting.py::TestStringLiftingInExpressions::test_address_of_string_lifts\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"char (*p)[4] = &\\\"abc\\\";\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" AddressOf(String(\"abc\") None) Pointer(Array(Char 4)) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_string_lifting.py::TestStringLiftingInExpressions::test_address_of_string_lifts (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_string_lifting.py::TestStringLiftingInExpressions::test_address_of_string_lifts\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" AddressOf(String(\"abc\") None) Pointer(Array(Char 4)) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" AddressOf(String(\"abc\") None) Pointer(Array(Char 4)) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_string_lifting.py::TestStringLiftingInExpressions::test_address_of_string_lifts (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_string_lifting.py::TestStringLiftingInExpressions::test_address_of_string_lifts\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" AddressOf(String(\"abc\") None) Pointer(Array(Char 4)) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"str@0\" String(\"abc\") Array(Char 4) Static None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"str@0\" None) None) Pointer(Array(Char 4)) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_string_lifting.py::TestStringLiftingInExpressions::test_string_in_return_lifts (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_string_lifting.py::TestStringLiftingInExpressions::test_string_in_return_lifts\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"char *get(void) { return \\\"hi\\\"; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"get\" [] Block([\n"
      "    S(Return(String(\"hi\")))\n"
      "  ]) FunType([] Pointer(Char)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_string_lifting.py::TestStringLiftingInExpressions::test_string_in_return_lifts (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_string_lifting.py::TestStringLiftingInExpressions::test_string_in_return_lifts\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"get\" [] Block([\n"
      "    S(Return(String(\"hi\")))\n"
      "  ]) FunType([] Pointer(Char)) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"get\" [] Block([\n"
      "    S(Return(String(\"hi\")))\n"
      "  ]) FunType([] Pointer(Char)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_string_lifting.py::TestStringLiftingInExpressions::test_string_in_return_lifts (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_string_lifting.py::TestStringLiftingInExpressions::test_string_in_return_lifts\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"get\" [] Block([\n"
      "    S(Return(String(\"hi\")))\n"
      "  ]) FunType([] Pointer(Char)) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"str@0\" String(\"hi\") Array(Char 3) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"get\" [] Block([\n"
      "    S(Return(Var(\"str@0\" None)))\n"
      "  ]) FunType([] Pointer(Char)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_string_lifting.py::TestStringLiftingInExpressions::test_string_in_subscript_lifts (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_string_lifting.py::TestStringLiftingInExpressions::test_string_in_subscript_lifts\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(String(\"abc\") Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(String(\"abc\") Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_string_lifting.py::TestStringLiftingInExpressions::test_string_in_subscript_lifts (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_string_lifting.py::TestStringLiftingInExpressions::test_string_in_subscript_lifts\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(String(\"abc\") Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"str@0\" String(\"abc\") Array(Char 4) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Var(\"str@0\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_string_lifting.py::TestStringLiftingMultiple::test_multiple_strings_get_unique_names (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_string_lifting.py::TestStringLiftingMultiple::test_multiple_strings_get_unique_names\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"char *a = \\\"first\\\"; char *b = \\\"second\\\";\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" String(\"first\") Pointer(Char) None None None None false))\n"
      "  VarDecl(var_decl(\"b\" String(\"second\") Pointer(Char) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_string_lifting.py::TestStringLiftingMultiple::test_multiple_strings_get_unique_names (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_string_lifting.py::TestStringLiftingMultiple::test_multiple_strings_get_unique_names\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" String(\"first\") Pointer(Char) None None None None false))\n"
      "  VarDecl(var_decl(\"b\" String(\"second\") Pointer(Char) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" String(\"first\") Pointer(Char) None None None None false))\n"
      "  VarDecl(var_decl(\"b\" String(\"second\") Pointer(Char) None None None None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_string_lifting.py::TestStringLiftingMultiple::test_multiple_strings_get_unique_names (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_string_lifting.py::TestStringLiftingMultiple::test_multiple_strings_get_unique_names\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" String(\"first\") Pointer(Char) None None None None false))\n"
      "  VarDecl(var_decl(\"b\" String(\"second\") Pointer(Char) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"str@0\" String(\"first\") Array(Char 6) Static None None None false))\n"
      "  VarDecl(var_decl(\"str@1\" String(\"second\") Array(Char 7) Static None None None false))\n"
      "  VarDecl(var_decl(\"a\" Var(\"str@0\" None) Pointer(Char) None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Var(\"str@1\" None) Pointer(Char) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_string_lifting.py::TestStringLiftingMultiple::test_string_inside_function_body (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_string_lifting.py::TestStringLiftingMultiple::test_string_inside_function_body\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {   char *p = \\\"hello\\\";   return p[0]; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"p\" String(\"hello\") Pointer(Char) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"p\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_string_lifting.py::TestStringLiftingMultiple::test_string_inside_function_body (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_string_lifting.py::TestStringLiftingMultiple::test_string_inside_function_body\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"p\" String(\"hello\") Pointer(Char) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"p\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.p\" String(\"hello\") Pointer(Char) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.p\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_string_lifting.py::TestStringLiftingMultiple::test_string_inside_function_body (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_string_lifting.py::TestStringLiftingMultiple::test_string_inside_function_body\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.p\" String(\"hello\") Pointer(Char) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.p\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"str@0\" String(\"hello\") Array(Char 6) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.p\" Var(\"str@0\" None) Pointer(Char) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.p\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}
