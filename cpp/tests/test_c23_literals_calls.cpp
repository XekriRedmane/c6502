// The calls that the tests of tests/test_c23_literals.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"
#include "lexer_case.h"

TEST_CASE("pp_number_at: test_c23_literals.py::TestPpNumber::test_extent (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass pp_number_at\n"
      "source tests/test_c23_literals.py::TestPpNumber::test_extent\n"
      "args {\"args\": [0], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"1'000;\"\n"
      "output-json\n"
      "\"1'000\"\n");
}

TEST_CASE("pp_number_at: test_c23_literals.py::TestPpNumber::test_extent (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass pp_number_at\n"
      "source tests/test_c23_literals.py::TestPpNumber::test_extent\n"
      "args {\"args\": [0], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"1''\"\n"
      "output-json\n"
      "\"1\"\n");
}

TEST_CASE("pp_number_at: test_c23_literals.py::TestPpNumber::test_extent (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass pp_number_at\n"
      "source tests/test_c23_literals.py::TestPpNumber::test_extent\n"
      "args {\"args\": [0], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"1'\"\n"
      "output-json\n"
      "\"1\"\n");
}

TEST_CASE("pp_number_at: test_c23_literals.py::TestPpNumber::test_extent (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass pp_number_at\n"
      "source tests/test_c23_literals.py::TestPpNumber::test_extent\n"
      "args {\"args\": [0], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"0x'FF\"\n"
      "output-json\n"
      "\"0x'FF\"\n");
}

TEST_CASE("pp_number_at: test_c23_literals.py::TestPpNumber::test_extent (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass pp_number_at\n"
      "source tests/test_c23_literals.py::TestPpNumber::test_extent\n"
      "args {\"args\": [0], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"1e+5'0\"\n"
      "output-json\n"
      "\"1e+5'0\"\n");
}

TEST_CASE("pp_number_at: test_c23_literals.py::TestPpNumber::test_extent (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass pp_number_at\n"
      "source tests/test_c23_literals.py::TestPpNumber::test_extent\n"
      "args {\"args\": [0], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\".5'5f\"\n"
      "output-json\n"
      "\".5'5f\"\n");
}

TEST_CASE("pp_number_at: test_c23_literals.py::TestPpNumber::test_extent (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass pp_number_at\n"
      "source tests/test_c23_literals.py::TestPpNumber::test_extent\n"
      "args {\"args\": [0], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"1'.\"\n"
      "output-json\n"
      "\"1\"\n");
}

TEST_CASE("tokens: test_c23_literals.py::TestBinaryConstants::test_bad_digits_and_missing_digits (call 1)") {
  c6502::testing::check_tokens(
      "0b2",
      "{\"message\": \"1:1: invalid digit '2' in binary constant '0b2'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_c23_literals.py::TestBinaryConstants::test_bad_digits_and_missing_digits (call 2)") {
  c6502::testing::check_tokens(
      "0b102",
      "{\"message\": \"1:1: invalid digit '2' in binary constant '0b102'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_c23_literals.py::TestBinaryConstants::test_bad_digits_and_missing_digits (call 3)") {
  c6502::testing::check_tokens(
      "0b",
      "{\"message\": \"1:1: no digits after the prefix 0b in '0b'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_c23_literals.py::TestBinaryConstants::test_bad_digits_and_missing_digits (call 4)") {
  c6502::testing::check_tokens(
      "0B",
      "{\"message\": \"1:1: no digits after the prefix 0B in '0B'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_c23_literals.py::TestBinaryConstants::test_bad_digits_and_missing_digits (call 5)") {
  c6502::testing::check_tokens(
      "0b1.0",
      "{\"message\": \"1:1: a binary constant is an integer, so '0b1.0' can't have a period\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_c23_literals.py::TestBinaryConstants::test_bad_digits_and_missing_digits (call 6)") {
  c6502::testing::check_tokens(
      "0b1f",
      "{\"message\": \"1:1: invalid suffix 'f' on integer constant '0b1f'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("parse: test_c23_literals.py::TestBinaryConstants::test_bit_precise_suffix_too_wide_is_an_error (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestBinaryConstants::test_bit_precise_suffix_too_wide_is_an_error\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int x = 0b10000000000000000000000000000000000000000000000000000000000000000wb;\"\n"
      "output-error\n"
      "{\"type\": \"LexError\", \"message\": \"1:9: integer constant '0b10000000000000000000000000000000000000000000000000000000000000000wb' needs a _BitInt(66), and BITINT_MAXWIDTH is 64 (C23 \\u00a76.4.4.2p6)\"}\n");
}

TEST_CASE("parse: test_c23_literals.py::TestBinaryConstants::test_bit_precise_suffix_too_wide_is_an_error (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestBinaryConstants::test_bit_precise_suffix_too_wide_is_an_error\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int x = 0x8000000000000000wb;\"\n"
      "output-error\n"
      "{\"type\": \"LexError\", \"message\": \"1:9: integer constant '0x8000000000000000wb' needs a _BitInt(65), and BITINT_MAXWIDTH is 64 (C23 \\u00a76.4.4.2p6)\"}\n");
}

TEST_CASE("parse: test_c23_literals.py::TestBinaryConstants::test_bit_precise_suffix_too_wide_is_an_error (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestBinaryConstants::test_bit_precise_suffix_too_wide_is_an_error\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int x = 0x1'0000'0000'0000'0000uwb;\"\n"
      "output-error\n"
      "{\"type\": \"LexError\", \"message\": \"1:9: integer constant \\\"0x1'0000'0000'0000'0000uwb\\\" needs a unsigned _BitInt(65), and BITINT_MAXWIDTH is 64 (C23 \\u00a76.4.4.2p6)\"}\n");
}

TEST_CASE("parse: test_c23_literals.py::TestBinaryConstants::test_decimal_65535_is_long (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestBinaryConstants::test_decimal_65535_is_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 65535; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstLong(65535) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("tokens: test_c23_literals.py::TestBinaryConstants::test_lexes_as_one_constant (call 1)") {
  c6502::testing::check_tokens(
      "0b0",
      "1:1\011constant\011" "0b0\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestBinaryConstants::test_lexes_as_one_constant (call 2)") {
  c6502::testing::check_tokens(
      "0b1",
      "1:1\011constant\011" "0b1\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestBinaryConstants::test_lexes_as_one_constant (call 3)") {
  c6502::testing::check_tokens(
      "0B1010",
      "1:1\011constant\011" "0B1010\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestBinaryConstants::test_lexes_as_one_constant (call 4)") {
  c6502::testing::check_tokens(
      "0b1010u",
      "1:1\011constant\011" "0b1010u\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestBinaryConstants::test_lexes_as_one_constant (call 5)") {
  c6502::testing::check_tokens(
      "0b1010U",
      "1:1\011constant\011" "0b1010U\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestBinaryConstants::test_lexes_as_one_constant (call 6)") {
  c6502::testing::check_tokens(
      "0b1l",
      "1:1\011constant\011" "0b1l\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestBinaryConstants::test_lexes_as_one_constant (call 7)") {
  c6502::testing::check_tokens(
      "0b1L",
      "1:1\011constant\011" "0b1L\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestBinaryConstants::test_lexes_as_one_constant (call 8)") {
  c6502::testing::check_tokens(
      "0b1ul",
      "1:1\011constant\011" "0b1ul\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestBinaryConstants::test_lexes_as_one_constant (call 9)") {
  c6502::testing::check_tokens(
      "0b1LU",
      "1:1\011constant\011" "0b1LU\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestBinaryConstants::test_lexes_as_one_constant (call 10)") {
  c6502::testing::check_tokens(
      "0b1ll",
      "1:1\011constant\011" "0b1ll\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestBinaryConstants::test_lexes_as_one_constant (call 11)") {
  c6502::testing::check_tokens(
      "0b1ULL",
      "1:1\011constant\011" "0b1ULL\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestBinaryConstants::test_lexes_as_one_constant (call 12)") {
  c6502::testing::check_tokens(
      "0b1llu",
      "1:1\011constant\011" "0b1llu\n",
      false);
}

TEST_CASE("parse: test_c23_literals.py::TestBinaryConstants::test_values_and_types (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestBinaryConstants::test_values_and_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 0b1010; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(10) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestBinaryConstants::test_values_and_types (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestBinaryConstants::test_values_and_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 0B1111; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(15) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestBinaryConstants::test_values_and_types (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestBinaryConstants::test_values_and_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 0b0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestBinaryConstants::test_values_and_types (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestBinaryConstants::test_values_and_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 0b1111111111111111; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstUInt(65535) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestBinaryConstants::test_values_and_types (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestBinaryConstants::test_values_and_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 0b10000000000000000; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstLong(65536) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestBinaryConstants::test_values_and_types (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestBinaryConstants::test_values_and_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 0b1u; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstUInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestBinaryConstants::test_values_and_types (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestBinaryConstants::test_values_and_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 0b1l; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstLong(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestBinaryConstants::test_values_and_types (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestBinaryConstants::test_values_and_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 0b1ul; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstULong(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestBinaryConstants::test_values_and_types (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestBinaryConstants::test_values_and_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 0b1ll; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstLongLong(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestBinaryConstants::test_values_and_types (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestBinaryConstants::test_values_and_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 0b1ull; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstULongLong(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestBinaryConstants::test_values_and_types (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestBinaryConstants::test_values_and_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 0b1111111111111111111111111111111111111111111111111111111111111111; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstULongLong(18446744073709551615) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_character_constant_after_a_number_is_unchanged (call 1)") {
  c6502::testing::check_tokens(
      "'1'2",
      "1:1\011constant\011'1'\n"
      "1:4\011constant\011" "2\n",
      false);
}

TEST_CASE("parse: test_c23_literals.py::TestDigitSeparators::test_floating (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestDigitSeparators::test_floating\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 1'000.5; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstDouble(4652011706887700480) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestDigitSeparators::test_floating (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestDigitSeparators::test_floating\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 1.2'5; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstDouble(4608308318706860032) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestDigitSeparators::test_floating (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestDigitSeparators::test_floating\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 1'0e1'0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstDouble(4771362005757984768) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestDigitSeparators::test_floating (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestDigitSeparators::test_floating\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 2'5e-1; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstDouble(4612811918334230528) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestDigitSeparators::test_floating (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestDigitSeparators::test_floating\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 1'000.5f; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstFloat(1148854272) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestDigitSeparators::test_ignored_for_the_value (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestDigitSeparators::test_ignored_for_the_value\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 1'000; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1000) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestDigitSeparators::test_ignored_for_the_value (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestDigitSeparators::test_ignored_for_the_value\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 1'000'000; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstLong(1000000) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestDigitSeparators::test_ignored_for_the_value (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestDigitSeparators::test_ignored_for_the_value\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 0xFF'FF; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstUInt(65535) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestDigitSeparators::test_ignored_for_the_value (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestDigitSeparators::test_ignored_for_the_value\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 0x1'2'3'4AB'C'D; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstLong(305441741) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestDigitSeparators::test_ignored_for_the_value (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestDigitSeparators::test_ignored_for_the_value\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 0b11'10'11'01; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(237) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestDigitSeparators::test_ignored_for_the_value (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestDigitSeparators::test_ignored_for_the_value\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 0'17; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(15) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestDigitSeparators::test_ignored_for_the_value (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestDigitSeparators::test_ignored_for_the_value\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 1'2'3u; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstUInt(123) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_lex_as_one_constant (call 1)") {
  c6502::testing::check_tokens(
      "1'000",
      "1:1\011constant\011" "1'000\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_lex_as_one_constant (call 2)") {
  c6502::testing::check_tokens(
      "1'000'000",
      "1:1\011constant\011" "1'000'000\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_lex_as_one_constant (call 3)") {
  c6502::testing::check_tokens(
      "0xFF'FF",
      "1:1\011constant\011" "0xFF'FF\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_lex_as_one_constant (call 4)") {
  c6502::testing::check_tokens(
      "0x1'2'3'4AB'C'D",
      "1:1\011constant\011" "0x1'2'3'4AB'C'D\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_lex_as_one_constant (call 5)") {
  c6502::testing::check_tokens(
      "0b11'10'11'01",
      "1:1\011constant\011" "0b11'10'11'01\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_lex_as_one_constant (call 6)") {
  c6502::testing::check_tokens(
      "0'17",
      "1:1\011constant\011" "0'17\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_lex_as_one_constant (call 7)") {
  c6502::testing::check_tokens(
      "1'000u",
      "1:1\011constant\011" "1'000u\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_lex_as_one_constant (call 8)") {
  c6502::testing::check_tokens(
      "1'000ULL",
      "1:1\011constant\011" "1'000ULL\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_lex_as_one_constant (call 9)") {
  c6502::testing::check_tokens(
      "1'000.5",
      "1:1\011constant\011" "1'000.5\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_lex_as_one_constant (call 10)") {
  c6502::testing::check_tokens(
      "1.0'0'1",
      "1:1\011constant\011" "1.0'0'1\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_lex_as_one_constant (call 11)") {
  c6502::testing::check_tokens(
      "1'0e1'0",
      "1:1\011constant\011" "1'0e1'0\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_lex_as_one_constant (call 12)") {
  c6502::testing::check_tokens(
      "1e+1'0",
      "1:1\011constant\011" "1e+1'0\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_lex_as_one_constant (call 13)") {
  c6502::testing::check_tokens(
      ".5'5f",
      "1:1\011constant\011.5'5f\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_lex_as_one_constant (call 14)") {
  c6502::testing::check_tokens(
      "0x1'0.8p1",
      "1:1\011constant\011" "0x1'0.8p1\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_misplaced_separators_are_errors (call 1)") {
  c6502::testing::check_tokens(
      "x = 1';",
      "{\"message\": \"1:5: a digit separator has to be between two digits, and the one after '1' in \\\"1'\\\" isn't followed by a digit\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_misplaced_separators_are_errors (call 2)") {
  c6502::testing::check_tokens(
      "x = 1''000;",
      "{\"message\": \"1:5: a digit separator has to be between two digits, and the one after '1' in \\\"1''000\\\" isn't followed by a digit\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_misplaced_separators_are_errors (call 3)") {
  c6502::testing::check_tokens(
      "x = 1'000';",
      "{\"message\": \"1:5: a digit separator has to be between two digits, and the one after \\\"1'000\\\" in \\\"1'000'\\\" isn't followed by a digit\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_misplaced_separators_are_errors (call 4)") {
  c6502::testing::check_tokens(
      "x = 1'.5;",
      "{\"message\": \"1:5: a digit separator has to be between two digits, and the one after '1' in \\\"1'.5\\\" isn't followed by a digit\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_misplaced_separators_are_errors (call 5)") {
  c6502::testing::check_tokens(
      "x = 0x'FF;",
      "{\"message\": \"1:5: a digit separator has to be between two digits, and the one in \\\"0x'FF\\\" comes right after the prefix 0x\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_misplaced_separators_are_errors (call 6)") {
  c6502::testing::check_tokens(
      "x = 0X'FF;",
      "{\"message\": \"1:5: a digit separator has to be between two digits, and the one in \\\"0X'FF\\\" comes right after the prefix 0X\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_misplaced_separators_are_errors (call 7)") {
  c6502::testing::check_tokens(
      "x = 0b'1;",
      "{\"message\": \"1:5: a digit separator has to be between two digits, and the one in \\\"0b'1\\\" comes right after the prefix 0b\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_misplaced_separators_are_errors (call 8)") {
  c6502::testing::check_tokens(
      "x = 1.'5;",
      "{\"message\": \"1:5: a digit separator has to be between two digits, and the one in \\\"1.'5\\\" touches the period\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_misplaced_separators_are_errors (call 9)") {
  c6502::testing::check_tokens(
      "x = 1'e5;",
      "{\"message\": \"1:5: a digit separator has to be between two digits, and the one in \\\"1'e5\\\" comes right before the exponent\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_misplaced_separators_are_errors (call 10)") {
  c6502::testing::check_tokens(
      "x = 1e'5;",
      "{\"message\": \"1:5: a digit separator has to be between two digits, and the one in \\\"1e'5\\\" comes right after the exponent's letter or sign\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_misplaced_separators_are_errors (call 11)") {
  c6502::testing::check_tokens(
      "x = 1e+'5;",
      "{\"message\": \"1:5: a digit separator has to be between two digits, and the one in \\\"1e+'5\\\" comes right after the exponent's letter or sign\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_misplaced_separators_are_errors (call 12)") {
  c6502::testing::check_tokens(
      "x = 0x1'p1;",
      "{\"message\": \"1:5: a digit separator has to be between two digits, and the one in \\\"0x1'p1\\\" comes right before the exponent\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_misplaced_separators_are_errors (call 13)") {
  c6502::testing::check_tokens(
      "x = 1'u;",
      "{\"message\": \"1:5: a digit separator has to be between two digits, and the one in \\\"1'u\\\" comes right before the suffix\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_misplaced_separators_are_errors (call 14)") {
  c6502::testing::check_tokens(
      "x = 1.5'f;",
      "{\"message\": \"1:5: a digit separator has to be between two digits, and the one in \\\"1.5'f\\\" comes right before the suffix\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_misplaced_separators_are_errors (call 15)") {
  c6502::testing::check_tokens(
      "x = 0'8;",
      "{\"message\": \"1:5: invalid digit '8' in octal constant \\\"0'8\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_misplaced_separators_are_errors (call 16)") {
  c6502::testing::check_tokens(
      "x = 0b1'2;",
      "{\"message\": \"1:5: invalid digit '2' in binary constant \\\"0b1'2\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_numbers_without_c23_spellings_lex_as_before (call 1)") {
  c6502::testing::check_tokens(
      "08",
      "1:1\011constant\011" "0\n"
      "1:2\011constant\011" "8\n",
      false);
}

TEST_CASE("tokens: test_c23_literals.py::TestDigitSeparators::test_numbers_without_c23_spellings_lex_as_before (call 2)") {
  c6502::testing::check_tokens(
      "3..5",
      "1:1\011constant\011" "3.\n"
      "1:3\011constant\011.5\n",
      false);
}

TEST_CASE("parse: test_c23_literals.py::TestDigitSeparators::test_parser_reports_the_same_errors (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestDigitSeparators::test_parser_reports_the_same_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int x = 0x'FF;\"\n"
      "output-error\n"
      "{\"type\": \"LexError\", \"message\": \"1:9: a digit separator has to be between two digits, and the one in \\\"0x'FF\\\" comes right after the prefix 0x\"}\n");
}

TEST_CASE("parse: test_c23_literals.py::TestDigitSeparators::test_parser_reports_the_same_errors (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestDigitSeparators::test_parser_reports_the_same_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int x = 1''0;\"\n"
      "output-error\n"
      "{\"type\": \"LexError\", \"message\": \"1:9: a digit separator has to be between two digits, and the one after '1' in \\\"1''0\\\" isn't followed by a digit\"}\n");
}

TEST_CASE("parse: test_c23_literals.py::TestDigitSeparators::test_parser_reports_the_same_errors (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestDigitSeparators::test_parser_reports_the_same_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int x = 0b12;\"\n"
      "output-error\n"
      "{\"type\": \"LexError\", \"message\": \"1:9: invalid digit '2' in binary constant '0b12'\"}\n");
}

TEST_CASE("parse: test_c23_literals.py::TestDigitSeparators::test_parser_reports_the_same_errors (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestDigitSeparators::test_parser_reports_the_same_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int x = 9223372036854775808wb;\"\n"
      "output-error\n"
      "{\"type\": \"LexError\", \"message\": \"1:9: integer constant '9223372036854775808wb' needs a _BitInt(65), and BITINT_MAXWIDTH is 64 (C23 \\u00a76.4.4.2p6)\"}\n");
}

TEST_CASE("parse: test_c23_literals.py::TestOctal::test_octal_values (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestOctal::test_octal_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 017; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(15) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestOctal::test_octal_values (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestOctal::test_octal_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 0777; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(511) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestOctal::test_octal_values (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestOctal::test_octal_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 0177777; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstUInt(65535) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestOctal::test_octal_values (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestOctal::test_octal_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 017L; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstLong(15) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_literals.py::TestPreprocessor::test_static_initializers (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_literals.py::TestPreprocessor::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"unsigned long big = 4'294'967'295UL;\\nunsigned char mask = 0b1010'0101;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"big\" Constant(ConstULong(4294967295) None) ULong None None None None false))\n"
      "  VarDecl(var_decl(\"mask\" Constant(ConstInt(165) None) UChar None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_literals.py::TestPreprocessor::test_static_initializers (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_literals.py::TestPreprocessor::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"big\" Constant(ConstULong(4294967295) None) ULong None None None None false))\n"
      "  VarDecl(var_decl(\"mask\" Constant(ConstInt(165) None) UChar None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"big\" Constant(ConstULong(4294967295) None) ULong None None None None false))\n"
      "  VarDecl(var_decl(\"mask\" Constant(ConstInt(165) None) UChar None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_literals.py::TestPreprocessor::test_static_initializers (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_literals.py::TestPreprocessor::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"big\\\" Constant(ConstULong(4294967295) ULong) ULong None None None None false))\\n  VarDecl(var_decl(\\\"mask\\\" Cast(UChar Constant(ConstInt(165) Int) UChar) UChar None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"big\" Constant(ConstULong(4294967295) None) ULong None None None None false))\n"
      "  VarDecl(var_decl(\"mask\" Constant(ConstInt(165) None) UChar None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"big\\\" Constant(ConstULong(4294967295) ULong) ULong None None None None false))\\n  VarDecl(var_decl(\\\"mask\\\" Cast(UChar Constant(ConstInt(165) Int) UChar) UChar None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"big\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 4294967295}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"mask\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 165}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}
