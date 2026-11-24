// The calls that the tests of tests/test_lexer.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"
#include "lexer_case.h"

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_attribute_tokens (call 1)") {
  c6502::testing::check_tokens(
      "[[c6502::zp_abi]] [ [x]]",
      "1:1\011symbol\011[[\n"
      "1:3\011identifier\011c6502\n"
      "1:8\011symbol\011::\n"
      "1:10\011identifier\011zp_abi\n"
      "1:16\011symbol\011]\n"
      "1:17\011symbol\011]\n"
      "1:19\011symbol\011[ [\n"
      "1:22\011identifier\011x\n"
      "1:23\011symbol\011]\n"
      "1:24\011symbol\011]\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_bit_precise_keyword_and_constants (call 1)") {
  c6502::testing::check_tokens(
      "_BitInt _BitInts 5wb 0x1Fuwb 1'000WBU",
      "1:1\011keyword\011_BitInt\n"
      "1:9\011identifier\011_BitInts\n"
      "1:18\011constant\011" "5wb\n"
      "1:22\011constant\011" "0x1Fuwb\n"
      "1:30\011constant\011" "1'000WBU\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_c23_boolean_keywords (call 1)") {
  c6502::testing::check_tokens(
      "bool true false _Bool booly true_",
      "1:1\011keyword\011bool\n"
      "1:6\011keyword\011true\n"
      "1:11\011keyword\011false\n"
      "1:17\011keyword\011_Bool\n"
      "1:23\011identifier\011booly\n"
      "1:29\011identifier\011true_\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 1)") {
  c6502::testing::check_tokens(
      "char",
      "1:1\011keyword\011char\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 2)") {
  c6502::testing::check_tokens(
      "typeof",
      "1:1\011keyword\011typeof\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 3)") {
  c6502::testing::check_tokens(
      "void",
      "1:1\011keyword\011void\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 4)") {
  c6502::testing::check_tokens(
      "typeof_unqual",
      "1:1\011keyword\011typeof_unqual\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 5)") {
  c6502::testing::check_tokens(
      "switch",
      "1:1\011keyword\011switch\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 6)") {
  c6502::testing::check_tokens(
      "bool",
      "1:1\011keyword\011bool\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 7)") {
  c6502::testing::check_tokens(
      "auto",
      "1:1\011keyword\011auto\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 8)") {
  c6502::testing::check_tokens(
      "int",
      "1:1\011keyword\011int\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 9)") {
  c6502::testing::check_tokens(
      "false",
      "1:1\011keyword\011false\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 10)") {
  c6502::testing::check_tokens(
      "register",
      "1:1\011keyword\011register\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 11)") {
  c6502::testing::check_tokens(
      "do",
      "1:1\011keyword\011do\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 12)") {
  c6502::testing::check_tokens(
      "long",
      "1:1\011keyword\011long\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 13)") {
  c6502::testing::check_tokens(
      "continue",
      "1:1\011keyword\011continue\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 14)") {
  c6502::testing::check_tokens(
      "volatile",
      "1:1\011keyword\011volatile\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 15)") {
  c6502::testing::check_tokens(
      "thread_local",
      "1:1\011keyword\011thread_local\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 16)") {
  c6502::testing::check_tokens(
      "sizeof",
      "1:1\011keyword\011sizeof\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 17)") {
  c6502::testing::check_tokens(
      "case",
      "1:1\011keyword\011case\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 18)") {
  c6502::testing::check_tokens(
      "short",
      "1:1\011keyword\011short\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 19)") {
  c6502::testing::check_tokens(
      "signed",
      "1:1\011keyword\011signed\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 20)") {
  c6502::testing::check_tokens(
      "if",
      "1:1\011keyword\011if\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 21)") {
  c6502::testing::check_tokens(
      "true",
      "1:1\011keyword\011true\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 22)") {
  c6502::testing::check_tokens(
      "_Bool",
      "1:1\011keyword\011_Bool\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 23)") {
  c6502::testing::check_tokens(
      "_Complex",
      "1:1\011keyword\011_Complex\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 24)") {
  c6502::testing::check_tokens(
      "const",
      "1:1\011keyword\011const\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 25)") {
  c6502::testing::check_tokens(
      "typedef",
      "1:1\011keyword\011typedef\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 26)") {
  c6502::testing::check_tokens(
      "static",
      "1:1\011keyword\011static\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 27)") {
  c6502::testing::check_tokens(
      "unsigned",
      "1:1\011keyword\011unsigned\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 28)") {
  c6502::testing::check_tokens(
      "for",
      "1:1\011keyword\011for\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 29)") {
  c6502::testing::check_tokens(
      "return",
      "1:1\011keyword\011return\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 30)") {
  c6502::testing::check_tokens(
      "enum",
      "1:1\011keyword\011enum\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 31)") {
  c6502::testing::check_tokens(
      "nullptr",
      "1:1\011keyword\011nullptr\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 32)") {
  c6502::testing::check_tokens(
      "_Imaginary",
      "1:1\011keyword\011_Imaginary\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 33)") {
  c6502::testing::check_tokens(
      "_BitInt",
      "1:1\011keyword\011_BitInt\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 34)") {
  c6502::testing::check_tokens(
      "restrict",
      "1:1\011keyword\011restrict\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 35)") {
  c6502::testing::check_tokens(
      "constexpr",
      "1:1\011keyword\011constexpr\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 36)") {
  c6502::testing::check_tokens(
      "inline",
      "1:1\011keyword\011inline\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 37)") {
  c6502::testing::check_tokens(
      "break",
      "1:1\011keyword\011break\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 38)") {
  c6502::testing::check_tokens(
      "else",
      "1:1\011keyword\011else\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 39)") {
  c6502::testing::check_tokens(
      "float",
      "1:1\011keyword\011float\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 40)") {
  c6502::testing::check_tokens(
      "_Thread_local",
      "1:1\011keyword\011_Thread_local\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 41)") {
  c6502::testing::check_tokens(
      "goto",
      "1:1\011keyword\011goto\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 42)") {
  c6502::testing::check_tokens(
      "union",
      "1:1\011keyword\011union\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 43)") {
  c6502::testing::check_tokens(
      "default",
      "1:1\011keyword\011default\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 44)") {
  c6502::testing::check_tokens(
      "struct",
      "1:1\011keyword\011struct\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 45)") {
  c6502::testing::check_tokens(
      "extern",
      "1:1\011keyword\011extern\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 46)") {
  c6502::testing::check_tokens(
      "while",
      "1:1\011keyword\011while\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_each_keyword_tokenizes_as_keyword (call 47)") {
  c6502::testing::check_tokens(
      "double",
      "1:1\011keyword\011double\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestKeywords::test_nullptr_keywords (call 1)") {
  c6502::testing::check_tokens(
      "nullptr typeof_unqual nullptr_t nullptrx",
      "1:1\011keyword\011nullptr\n"
      "1:9\011keyword\011typeof_unqual\n"
      "1:23\011identifier\011nullptr_t\n"
      "1:33\011identifier\011nullptrx\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIdentifiers::test_keyword_prefix_is_not_keyword (call 1)") {
  c6502::testing::check_tokens(
      "intx",
      "1:1\011identifier\011intx\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIdentifiers::test_keyword_prefix_is_not_keyword (call 2)") {
  c6502::testing::check_tokens(
      "returning",
      "1:1\011identifier\011returning\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIdentifiers::test_keyword_prefix_is_not_keyword (call 3)") {
  c6502::testing::check_tokens(
      "while_",
      "1:1\011identifier\011while_\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIdentifiers::test_keyword_prefix_is_not_keyword (call 4)") {
  c6502::testing::check_tokens(
      "_int",
      "1:1\011identifier\011_int\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIdentifiers::test_keyword_prefix_is_not_keyword (call 5)") {
  c6502::testing::check_tokens(
      "ifx",
      "1:1\011identifier\011ifx\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIdentifiers::test_simple (call 1)") {
  c6502::testing::check_tokens(
      "x",
      "1:1\011identifier\011x\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIdentifiers::test_simple (call 2)") {
  c6502::testing::check_tokens(
      "foo",
      "1:1\011identifier\011foo\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIdentifiers::test_simple (call 3)") {
  c6502::testing::check_tokens(
      "_bar",
      "1:1\011identifier\011_bar\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIdentifiers::test_simple (call 4)") {
  c6502::testing::check_tokens(
      "_",
      "1:1\011identifier\011_\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIdentifiers::test_simple (call 5)") {
  c6502::testing::check_tokens(
      "a1",
      "1:1\011identifier\011a1\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIdentifiers::test_simple (call 6)") {
  c6502::testing::check_tokens(
      "x_y_z",
      "1:1\011identifier\011x_y_z\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIdentifiers::test_simple (call 7)") {
  c6502::testing::check_tokens(
      "CamelCase",
      "1:1\011identifier\011CamelCase\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 1)") {
  c6502::testing::check_tokens(
      "...",
      "1:1\011symbol\011...\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 2)") {
  c6502::testing::check_tokens(
      "::",
      "1:1\011symbol\011::\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 3)") {
  c6502::testing::check_tokens(
      "<<=",
      "1:1\011symbol\011<<=\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 4)") {
  c6502::testing::check_tokens(
      ">>=",
      "1:1\011symbol\011>>=\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 5)") {
  c6502::testing::check_tokens(
      "->",
      "1:1\011symbol\011->\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 6)") {
  c6502::testing::check_tokens(
      "++",
      "1:1\011symbol\011++\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 7)") {
  c6502::testing::check_tokens(
      "--",
      "1:1\011symbol\011--\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 8)") {
  c6502::testing::check_tokens(
      "<<",
      "1:1\011symbol\011<<\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 9)") {
  c6502::testing::check_tokens(
      ">>",
      "1:1\011symbol\011>>\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 10)") {
  c6502::testing::check_tokens(
      "<=",
      "1:1\011symbol\011<=\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 11)") {
  c6502::testing::check_tokens(
      ">=",
      "1:1\011symbol\011>=\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 12)") {
  c6502::testing::check_tokens(
      "==",
      "1:1\011symbol\011==\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 13)") {
  c6502::testing::check_tokens(
      "!=",
      "1:1\011symbol\011!=\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 14)") {
  c6502::testing::check_tokens(
      "&&",
      "1:1\011symbol\011&&\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 15)") {
  c6502::testing::check_tokens(
      "||",
      "1:1\011symbol\011||\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 16)") {
  c6502::testing::check_tokens(
      "*=",
      "1:1\011symbol\011*=\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 17)") {
  c6502::testing::check_tokens(
      "/=",
      "1:1\011symbol\011/=\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 18)") {
  c6502::testing::check_tokens(
      "%=",
      "1:1\011symbol\011%=\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 19)") {
  c6502::testing::check_tokens(
      "+=",
      "1:1\011symbol\011+=\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 20)") {
  c6502::testing::check_tokens(
      "-=",
      "1:1\011symbol\011-=\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 21)") {
  c6502::testing::check_tokens(
      "&=",
      "1:1\011symbol\011&=\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 22)") {
  c6502::testing::check_tokens(
      "^=",
      "1:1\011symbol\011^=\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 23)") {
  c6502::testing::check_tokens(
      "|=",
      "1:1\011symbol\011|=\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 24)") {
  c6502::testing::check_tokens(
      "##",
      "1:1\011symbol\011##\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 25)") {
  c6502::testing::check_tokens(
      "[",
      "1:1\011symbol\011[\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 26)") {
  c6502::testing::check_tokens(
      "]",
      "1:1\011symbol\011]\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 27)") {
  c6502::testing::check_tokens(
      "(",
      "1:1\011symbol\011(\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 28)") {
  c6502::testing::check_tokens(
      ")",
      "1:1\011symbol\011)\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 29)") {
  c6502::testing::check_tokens(
      "{",
      "1:1\011symbol\011{\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 30)") {
  c6502::testing::check_tokens(
      "}",
      "1:1\011symbol\011}\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 31)") {
  c6502::testing::check_tokens(
      ".",
      "1:1\011symbol\011.\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 32)") {
  c6502::testing::check_tokens(
      "&",
      "1:1\011symbol\011&\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 33)") {
  c6502::testing::check_tokens(
      "*",
      "1:1\011symbol\011*\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 34)") {
  c6502::testing::check_tokens(
      "+",
      "1:1\011symbol\011+\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 35)") {
  c6502::testing::check_tokens(
      "-",
      "1:1\011symbol\011-\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 36)") {
  c6502::testing::check_tokens(
      "~",
      "1:1\011symbol\011~\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 37)") {
  c6502::testing::check_tokens(
      "!",
      "1:1\011symbol\011!\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 38)") {
  c6502::testing::check_tokens(
      "/",
      "1:1\011symbol\011/\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 39)") {
  c6502::testing::check_tokens(
      "%",
      "1:1\011symbol\011%\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 40)") {
  c6502::testing::check_tokens(
      "<",
      "1:1\011symbol\011<\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 41)") {
  c6502::testing::check_tokens(
      ">",
      "1:1\011symbol\011>\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 42)") {
  c6502::testing::check_tokens(
      "^",
      "1:1\011symbol\011^\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 43)") {
  c6502::testing::check_tokens(
      "|",
      "1:1\011symbol\011|\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 44)") {
  c6502::testing::check_tokens(
      "\077",
      "1:1\011symbol\011\077\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 45)") {
  c6502::testing::check_tokens(
      ":",
      "1:1\011symbol\011:\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 46)") {
  c6502::testing::check_tokens(
      ";",
      "1:1\011symbol\011;\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 47)") {
  c6502::testing::check_tokens(
      "=",
      "1:1\011symbol\011=\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 48)") {
  c6502::testing::check_tokens(
      ",",
      "1:1\011symbol\011,\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_each_symbol (call 49)") {
  c6502::testing::check_tokens(
      "#",
      "1:1\011symbol\011#\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_longest_match (call 3)") {
  c6502::testing::check_tokens(
      "<<<",
      "1:1\011symbol\011<<\n"
      "1:3\011symbol\011<\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_longest_match (call 4)") {
  c6502::testing::check_tokens(
      "<<=<<",
      "1:1\011symbol\011<<=\n"
      "1:4\011symbol\011<<\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_longest_match (call 5)") {
  c6502::testing::check_tokens(
      "<<==",
      "1:1\011symbol\011<<=\n"
      "1:4\011symbol\011=\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_longest_match (call 8)") {
  c6502::testing::check_tokens(
      "....",
      "1:1\011symbol\011...\n"
      "1:4\011symbol\011.\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_longest_match (call 9)") {
  c6502::testing::check_tokens(
      ".....",
      "1:1\011symbol\011...\n"
      "1:4\011symbol\011.\n"
      "1:5\011symbol\011.\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_longest_match (call 10)") {
  c6502::testing::check_tokens(
      "......",
      "1:1\011symbol\011...\n"
      "1:4\011symbol\011...\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_longest_match (call 11)") {
  c6502::testing::check_tokens(
      ".......",
      "1:1\011symbol\011...\n"
      "1:4\011symbol\011...\n"
      "1:7\011symbol\011.\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_longest_match (call 13)") {
  c6502::testing::check_tokens(
      "-->",
      "1:1\011symbol\011--\n"
      "1:3\011symbol\011>\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_longest_match (call 15)") {
  c6502::testing::check_tokens(
      "+++",
      "1:1\011symbol\011++\n"
      "1:3\011symbol\011+\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_longest_match (call 16)") {
  c6502::testing::check_tokens(
      "&&&",
      "1:1\011symbol\011&&\n"
      "1:3\011symbol\011&\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_longest_match (call 18)") {
  c6502::testing::check_tokens(
      "###",
      "1:1\011symbol\011##\n"
      "1:3\011symbol\011#\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestSymbols::test_longest_match (call 20)") {
  c6502::testing::check_tokens(
      "===",
      "1:1\011symbol\011==\n"
      "1:3\011symbol\011=\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 1)") {
  c6502::testing::check_tokens(
      "1",
      "1:1\011constant\011" "1\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 2)") {
  c6502::testing::check_tokens(
      "1u",
      "1:1\011constant\011" "1u\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 3)") {
  c6502::testing::check_tokens(
      "1U",
      "1:1\011constant\011" "1U\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 4)") {
  c6502::testing::check_tokens(
      "1ul",
      "1:1\011constant\011" "1ul\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 5)") {
  c6502::testing::check_tokens(
      "1uL",
      "1:1\011constant\011" "1uL\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 6)") {
  c6502::testing::check_tokens(
      "1Ul",
      "1:1\011constant\011" "1Ul\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 7)") {
  c6502::testing::check_tokens(
      "1UL",
      "1:1\011constant\011" "1UL\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 8)") {
  c6502::testing::check_tokens(
      "1ull",
      "1:1\011constant\011" "1ull\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 9)") {
  c6502::testing::check_tokens(
      "1uLL",
      "1:1\011constant\011" "1uLL\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 10)") {
  c6502::testing::check_tokens(
      "1Ull",
      "1:1\011constant\011" "1Ull\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 11)") {
  c6502::testing::check_tokens(
      "1ULL",
      "1:1\011constant\011" "1ULL\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 12)") {
  c6502::testing::check_tokens(
      "1l",
      "1:1\011constant\011" "1l\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 13)") {
  c6502::testing::check_tokens(
      "1L",
      "1:1\011constant\011" "1L\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 14)") {
  c6502::testing::check_tokens(
      "1lu",
      "1:1\011constant\011" "1lu\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 15)") {
  c6502::testing::check_tokens(
      "1lU",
      "1:1\011constant\011" "1lU\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 16)") {
  c6502::testing::check_tokens(
      "1Lu",
      "1:1\011constant\011" "1Lu\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 17)") {
  c6502::testing::check_tokens(
      "1LU",
      "1:1\011constant\011" "1LU\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 18)") {
  c6502::testing::check_tokens(
      "1ll",
      "1:1\011constant\011" "1ll\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 19)") {
  c6502::testing::check_tokens(
      "1LL",
      "1:1\011constant\011" "1LL\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 20)") {
  c6502::testing::check_tokens(
      "1llu",
      "1:1\011constant\011" "1llu\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 21)") {
  c6502::testing::check_tokens(
      "1llU",
      "1:1\011constant\011" "1llU\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 22)") {
  c6502::testing::check_tokens(
      "1LLu",
      "1:1\011constant\011" "1LLu\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 23)") {
  c6502::testing::check_tokens(
      "1LLU",
      "1:1\011constant\011" "1LLU\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 24)") {
  c6502::testing::check_tokens(
      "0",
      "1:1\011constant\011" "0\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 25)") {
  c6502::testing::check_tokens(
      "0u",
      "1:1\011constant\011" "0u\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 26)") {
  c6502::testing::check_tokens(
      "0U",
      "1:1\011constant\011" "0U\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 27)") {
  c6502::testing::check_tokens(
      "0ul",
      "1:1\011constant\011" "0ul\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 28)") {
  c6502::testing::check_tokens(
      "0uL",
      "1:1\011constant\011" "0uL\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 29)") {
  c6502::testing::check_tokens(
      "0Ul",
      "1:1\011constant\011" "0Ul\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 30)") {
  c6502::testing::check_tokens(
      "0UL",
      "1:1\011constant\011" "0UL\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 31)") {
  c6502::testing::check_tokens(
      "0ull",
      "1:1\011constant\011" "0ull\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 32)") {
  c6502::testing::check_tokens(
      "0uLL",
      "1:1\011constant\011" "0uLL\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 33)") {
  c6502::testing::check_tokens(
      "0Ull",
      "1:1\011constant\011" "0Ull\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 34)") {
  c6502::testing::check_tokens(
      "0ULL",
      "1:1\011constant\011" "0ULL\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 35)") {
  c6502::testing::check_tokens(
      "0l",
      "1:1\011constant\011" "0l\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 36)") {
  c6502::testing::check_tokens(
      "0L",
      "1:1\011constant\011" "0L\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 37)") {
  c6502::testing::check_tokens(
      "0lu",
      "1:1\011constant\011" "0lu\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 38)") {
  c6502::testing::check_tokens(
      "0lU",
      "1:1\011constant\011" "0lU\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 39)") {
  c6502::testing::check_tokens(
      "0Lu",
      "1:1\011constant\011" "0Lu\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 40)") {
  c6502::testing::check_tokens(
      "0LU",
      "1:1\011constant\011" "0LU\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 41)") {
  c6502::testing::check_tokens(
      "0ll",
      "1:1\011constant\011" "0ll\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 42)") {
  c6502::testing::check_tokens(
      "0LL",
      "1:1\011constant\011" "0LL\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 43)") {
  c6502::testing::check_tokens(
      "0llu",
      "1:1\011constant\011" "0llu\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 44)") {
  c6502::testing::check_tokens(
      "0llU",
      "1:1\011constant\011" "0llU\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 45)") {
  c6502::testing::check_tokens(
      "0LLu",
      "1:1\011constant\011" "0LLu\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 46)") {
  c6502::testing::check_tokens(
      "0LLU",
      "1:1\011constant\011" "0LLU\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 47)") {
  c6502::testing::check_tokens(
      "0x1A",
      "1:1\011constant\011" "0x1A\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 48)") {
  c6502::testing::check_tokens(
      "0x1Au",
      "1:1\011constant\011" "0x1Au\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 49)") {
  c6502::testing::check_tokens(
      "0x1AU",
      "1:1\011constant\011" "0x1AU\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 50)") {
  c6502::testing::check_tokens(
      "0x1Aul",
      "1:1\011constant\011" "0x1Aul\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 51)") {
  c6502::testing::check_tokens(
      "0x1AuL",
      "1:1\011constant\011" "0x1AuL\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 52)") {
  c6502::testing::check_tokens(
      "0x1AUl",
      "1:1\011constant\011" "0x1AUl\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 53)") {
  c6502::testing::check_tokens(
      "0x1AUL",
      "1:1\011constant\011" "0x1AUL\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 54)") {
  c6502::testing::check_tokens(
      "0x1Aull",
      "1:1\011constant\011" "0x1Aull\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 55)") {
  c6502::testing::check_tokens(
      "0x1AuLL",
      "1:1\011constant\011" "0x1AuLL\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 56)") {
  c6502::testing::check_tokens(
      "0x1AUll",
      "1:1\011constant\011" "0x1AUll\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 57)") {
  c6502::testing::check_tokens(
      "0x1AULL",
      "1:1\011constant\011" "0x1AULL\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 58)") {
  c6502::testing::check_tokens(
      "0x1Al",
      "1:1\011constant\011" "0x1Al\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 59)") {
  c6502::testing::check_tokens(
      "0x1AL",
      "1:1\011constant\011" "0x1AL\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 60)") {
  c6502::testing::check_tokens(
      "0x1Alu",
      "1:1\011constant\011" "0x1Alu\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 61)") {
  c6502::testing::check_tokens(
      "0x1AlU",
      "1:1\011constant\011" "0x1AlU\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 62)") {
  c6502::testing::check_tokens(
      "0x1ALu",
      "1:1\011constant\011" "0x1ALu\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 63)") {
  c6502::testing::check_tokens(
      "0x1ALU",
      "1:1\011constant\011" "0x1ALU\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 64)") {
  c6502::testing::check_tokens(
      "0x1All",
      "1:1\011constant\011" "0x1All\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 65)") {
  c6502::testing::check_tokens(
      "0x1ALL",
      "1:1\011constant\011" "0x1ALL\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 66)") {
  c6502::testing::check_tokens(
      "0x1Allu",
      "1:1\011constant\011" "0x1Allu\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 67)") {
  c6502::testing::check_tokens(
      "0x1AllU",
      "1:1\011constant\011" "0x1AllU\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 68)") {
  c6502::testing::check_tokens(
      "0x1ALLu",
      "1:1\011constant\011" "0x1ALLu\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_all_valid_suffixes_on_all_bodies (call 69)") {
  c6502::testing::check_tokens(
      "0x1ALLU",
      "1:1\011constant\011" "0x1ALLU\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_decimal (call 2)") {
  c6502::testing::check_tokens(
      "42",
      "1:1\011constant\011" "42\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_decimal (call 3)") {
  c6502::testing::check_tokens(
      "123",
      "1:1\011constant\011" "123\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_decimal (call 4)") {
  c6502::testing::check_tokens(
      "999999",
      "1:1\011constant\011" "999999\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_hex (call 1)") {
  c6502::testing::check_tokens(
      "0x0",
      "1:1\011constant\011" "0x0\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_hex (call 2)") {
  c6502::testing::check_tokens(
      "0X0",
      "1:1\011constant\011" "0X0\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_hex (call 4)") {
  c6502::testing::check_tokens(
      "0X1a",
      "1:1\011constant\011" "0X1a\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_hex (call 5)") {
  c6502::testing::check_tokens(
      "0xDEADBEEF",
      "1:1\011constant\011" "0xDEADBEEF\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_hex (call 6)") {
  c6502::testing::check_tokens(
      "0xffffffff",
      "1:1\011constant\011" "0xffffffff\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_hex_requires_at_least_one_digit (call 1)") {
  c6502::testing::check_tokens(
      "0x",
      "{\"message\": \"1:1: malformed numeric token '0x'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_hex_requires_at_least_one_digit (call 2)") {
  c6502::testing::check_tokens(
      "0X",
      "{\"message\": \"1:1: malformed numeric token '0X'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_hex_requires_at_least_one_digit (call 3)") {
  c6502::testing::check_tokens(
      "0xg",
      "{\"message\": \"1:1: malformed numeric token '0x'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_hex_requires_at_least_one_digit (call 4)") {
  c6502::testing::check_tokens(
      "0x+",
      "{\"message\": \"1:1: malformed numeric token '0x'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_long_long_is_case_strict (call 1)") {
  c6502::testing::check_tokens(
      "42lL",
      "{\"message\": \"1:1: invalid pp-number: 42l'L' (numeric constant may not abut an identifier)\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_long_long_is_case_strict (call 2)") {
  c6502::testing::check_tokens(
      "42Ll",
      "{\"message\": \"1:1: invalid pp-number: 42L'l' (numeric constant may not abut an identifier)\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_octal (call 2)") {
  c6502::testing::check_tokens(
      "00",
      "1:1\011constant\011" "00\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_octal (call 3)") {
  c6502::testing::check_tokens(
      "01",
      "1:1\011constant\011" "01\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_octal (call 4)") {
  c6502::testing::check_tokens(
      "0123",
      "1:1\011constant\011" "0123\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_octal (call 5)") {
  c6502::testing::check_tokens(
      "0777",
      "1:1\011constant\011" "0777\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_octal_stops_at_nonoctal_digit (call 1)") {
  c6502::testing::check_tokens(
      "08",
      "1:1\011constant\011" "0\n"
      "1:2\011constant\011" "8\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_octal_stops_at_nonoctal_digit (call 2)") {
  c6502::testing::check_tokens(
      "019",
      "1:1\011constant\011" "01\n"
      "1:3\011constant\011" "9\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_octal_stops_at_nonoctal_digit (call 3)") {
  c6502::testing::check_tokens(
      "089",
      "1:1\011constant\011" "0\n"
      "1:2\011constant\011" "89\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestIntegerConstants::test_suffix_then_identifier_rejected (call 1)") {
  c6502::testing::check_tokens(
      "42ua",
      "{\"message\": \"1:1: invalid pp-number: 42u'a' (numeric constant may not abut an identifier)\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_dot (call 1)") {
  c6502::testing::check_tokens(
      "3.14",
      "1:1\011constant\011" "3.14\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_dot (call 2)") {
  c6502::testing::check_tokens(
      "3.",
      "1:1\011constant\011" "3.\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_dot (call 3)") {
  c6502::testing::check_tokens(
      ".5",
      "1:1\011constant\011.5\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_dot (call 4)") {
  c6502::testing::check_tokens(
      "0.0",
      "1:1\011constant\011" "0.0\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_dot (call 5)") {
  c6502::testing::check_tokens(
      "123.456",
      "1:1\011constant\011" "123.456\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_dot (call 6)") {
  c6502::testing::check_tokens(
      "00.00",
      "1:1\011constant\011" "00.00\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_exponent (call 1)") {
  c6502::testing::check_tokens(
      "3e5",
      "1:1\011constant\011" "3e5\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_exponent (call 2)") {
  c6502::testing::check_tokens(
      "3E5",
      "1:1\011constant\011" "3E5\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_exponent (call 3)") {
  c6502::testing::check_tokens(
      "3e+5",
      "1:1\011constant\011" "3e+5\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_exponent (call 4)") {
  c6502::testing::check_tokens(
      "3e-5",
      "1:1\011constant\011" "3e-5\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_exponent (call 5)") {
  c6502::testing::check_tokens(
      "3E+10",
      "1:1\011constant\011" "3E+10\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_exponent (call 6)") {
  c6502::testing::check_tokens(
      "3.14e5",
      "1:1\011constant\011" "3.14e5\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_exponent (call 7)") {
  c6502::testing::check_tokens(
      "3.e5",
      "1:1\011constant\011" "3.e5\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_exponent (call 8)") {
  c6502::testing::check_tokens(
      ".5e10",
      "1:1\011constant\011.5e10\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_exponent (call 9)") {
  c6502::testing::check_tokens(
      ".5E-10",
      "1:1\011constant\011.5E-10\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_exponent (call 10)") {
  c6502::testing::check_tokens(
      "0e0",
      "1:1\011constant\011" "0e0\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_suffix (call 1)") {
  c6502::testing::check_tokens(
      "3.14f",
      "1:1\011constant\011" "3.14f\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_suffix (call 2)") {
  c6502::testing::check_tokens(
      "3.14F",
      "1:1\011constant\011" "3.14F\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_suffix (call 3)") {
  c6502::testing::check_tokens(
      "3.14l",
      "1:1\011constant\011" "3.14l\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_suffix (call 4)") {
  c6502::testing::check_tokens(
      "3.14L",
      "1:1\011constant\011" "3.14L\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_suffix (call 5)") {
  c6502::testing::check_tokens(
      "3.f",
      "1:1\011constant\011" "3.f\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_suffix (call 6)") {
  c6502::testing::check_tokens(
      ".5L",
      "1:1\011constant\011.5L\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_suffix (call 7)") {
  c6502::testing::check_tokens(
      "3e5f",
      "1:1\011constant\011" "3e5f\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_suffix (call 8)") {
  c6502::testing::check_tokens(
      "3e5F",
      "1:1\011constant\011" "3e5F\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_suffix (call 9)") {
  c6502::testing::check_tokens(
      "3e5l",
      "1:1\011constant\011" "3e5l\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_with_suffix (call 10)") {
  c6502::testing::check_tokens(
      "3e5L",
      "1:1\011constant\011" "3e5L\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_without_exp_requires_dot (call 1)") {
  c6502::testing::check_tokens(
      "3e",
      "{\"message\": \"1:1: malformed numeric token '3e'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_without_exp_requires_dot (call 2)") {
  c6502::testing::check_tokens(
      "3e+",
      "{\"message\": \"1:1: malformed numeric token '3e'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_without_exp_requires_dot (call 3)") {
  c6502::testing::check_tokens(
      "3e-",
      "{\"message\": \"1:1: malformed numeric token '3e'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_without_exp_requires_dot (call 4)") {
  c6502::testing::check_tokens(
      "3.e",
      "{\"message\": \"1:1: invalid pp-number: 3.'e' (numeric constant may not abut an identifier)\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_decimal_without_exp_requires_dot (call 5)") {
  c6502::testing::check_tokens(
      "3.14e",
      "{\"message\": \"1:1: invalid pp-number: 3.14'e' (numeric constant may not abut an identifier)\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_exponent_followed_by_more_rejected (call 1)") {
  c6502::testing::check_tokens(
      "1e2e3",
      "{\"message\": \"1:1: invalid pp-number: 1e2'e3' (numeric constant may not abut an identifier)\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_basic (call 1)") {
  c6502::testing::check_tokens(
      "0x1p1",
      "1:1\011constant\011" "0x1p1\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_basic (call 2)") {
  c6502::testing::check_tokens(
      "0X1p1",
      "1:1\011constant\011" "0X1p1\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_basic (call 3)") {
  c6502::testing::check_tokens(
      "0x1P1",
      "1:1\011constant\011" "0x1P1\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_basic (call 4)") {
  c6502::testing::check_tokens(
      "0x1p+1",
      "1:1\011constant\011" "0x1p+1\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_basic (call 5)") {
  c6502::testing::check_tokens(
      "0x1p-1",
      "1:1\011constant\011" "0x1p-1\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_basic (call 6)") {
  c6502::testing::check_tokens(
      "0xFFp10",
      "1:1\011constant\011" "0xFFp10\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_basic (call 7)") {
  c6502::testing::check_tokens(
      "0xDEADp-5",
      "1:1\011constant\011" "0xDEADp-5\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_fractional (call 1)") {
  c6502::testing::check_tokens(
      "0x1.p1",
      "1:1\011constant\011" "0x1.p1\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_fractional (call 2)") {
  c6502::testing::check_tokens(
      "0x1.8p1",
      "1:1\011constant\011" "0x1.8p1\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_fractional (call 3)") {
  c6502::testing::check_tokens(
      "0x.FFp0",
      "1:1\011constant\011" "0x.FFp0\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_fractional (call 4)") {
  c6502::testing::check_tokens(
      "0x1.FFp-2",
      "1:1\011constant\011" "0x1.FFp-2\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_fractional (call 5)") {
  c6502::testing::check_tokens(
      "0xA.Bp3",
      "1:1\011constant\011" "0xA.Bp3\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_needs_hex_digit (call 1)") {
  c6502::testing::check_tokens(
      "0x.p1",
      "{\"message\": \"1:1: malformed numeric token '0x.'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_needs_hex_digit (call 2)") {
  c6502::testing::check_tokens(
      "0xp1",
      "{\"message\": \"1:1: malformed numeric token '0x'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_requires_binary_exponent (call 1)") {
  c6502::testing::check_tokens(
      "0x1.",
      "{\"message\": \"1:1: malformed numeric token '0x1.'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_requires_binary_exponent (call 2)") {
  c6502::testing::check_tokens(
      "0x1.8",
      "{\"message\": \"1:1: malformed numeric token '0x1.8'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_requires_binary_exponent (call 3)") {
  c6502::testing::check_tokens(
      "0x.FF",
      "{\"message\": \"1:1: malformed numeric token '0x.FF'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_requires_binary_exponent (call 4)") {
  c6502::testing::check_tokens(
      "0xFF.",
      "{\"message\": \"1:1: malformed numeric token '0xFF.'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_requires_exp_digits (call 1)") {
  c6502::testing::check_tokens(
      "0x1p",
      "{\"message\": \"1:1: malformed numeric token '0x1p'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_requires_exp_digits (call 2)") {
  c6502::testing::check_tokens(
      "0x1p+",
      "{\"message\": \"1:1: malformed numeric token '0x1p'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_requires_exp_digits (call 3)") {
  c6502::testing::check_tokens(
      "0x1p-",
      "{\"message\": \"1:1: malformed numeric token '0x1p'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_with_suffix (call 1)") {
  c6502::testing::check_tokens(
      "0x1p1f",
      "1:1\011constant\011" "0x1p1f\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_with_suffix (call 2)") {
  c6502::testing::check_tokens(
      "0x1p1F",
      "1:1\011constant\011" "0x1p1F\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_with_suffix (call 3)") {
  c6502::testing::check_tokens(
      "0x1p1l",
      "1:1\011constant\011" "0x1p1l\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_with_suffix (call 4)") {
  c6502::testing::check_tokens(
      "0x1p1L",
      "1:1\011constant\011" "0x1p1L\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_with_suffix (call 5)") {
  c6502::testing::check_tokens(
      "0x1.8p1f",
      "1:1\011constant\011" "0x1.8p1f\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_hex_float_with_suffix (call 6)") {
  c6502::testing::check_tokens(
      "0x.FFp0L",
      "1:1\011constant\011" "0x.FFp0L\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_int_vs_float_suffix_disambiguation (call 1)") {
  c6502::testing::check_tokens(
      "3l",
      "1:1\011constant\011" "3l\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_int_vs_float_suffix_disambiguation (call 2)") {
  c6502::testing::check_tokens(
      "3.l",
      "1:1\011constant\011" "3.l\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_int_vs_float_suffix_disambiguation (call 3)") {
  c6502::testing::check_tokens(
      "3f",
      "{\"message\": \"1:1: invalid pp-number: 3'f' (numeric constant may not abut an identifier)\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_int_vs_float_suffix_disambiguation (call 4)") {
  c6502::testing::check_tokens(
      "3.u",
      "{\"message\": \"1:1: invalid pp-number: 3.'u' (numeric constant may not abut an identifier)\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_lone_dot_is_symbol (call 2)") {
  c6502::testing::check_tokens(
      ".x",
      "1:1\011symbol\011.\n"
      "1:2\011identifier\011x\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestFloatingConstants::test_trailing_dot_breaks_off (call 1)") {
  c6502::testing::check_tokens(
      "3..5",
      "1:1\011constant\011" "3.\n"
      "1:3\011constant\011.5\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_L_alone_is_identifier (call 1)") {
  c6502::testing::check_tokens(
      "L",
      "1:1\011identifier\011L\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_L_alone_is_identifier (call 2)") {
  c6502::testing::check_tokens(
      "Lx",
      "1:1\011identifier\011Lx\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_L_space_quote_splits (call 1)") {
  c6502::testing::check_tokens(
      "L 'a'",
      "1:1\011identifier\011L\n"
      "1:3\011constant\011'a'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_adjacent_to_symbols (call 1)") {
  c6502::testing::check_tokens(
      "c='a';",
      "1:1\011identifier\011c\n"
      "1:2\011symbol\011=\n"
      "1:3\011constant\011'a'\n"
      "1:6\011symbol\011;\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_basic (call 1)") {
  c6502::testing::check_tokens(
      "'a'",
      "1:1\011constant\011'a'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_basic (call 2)") {
  c6502::testing::check_tokens(
      "'Z'",
      "1:1\011constant\011'Z'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_basic (call 3)") {
  c6502::testing::check_tokens(
      "'0'",
      "1:1\011constant\011'0'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_basic (call 4)") {
  c6502::testing::check_tokens(
      "' '",
      "1:1\011constant\011' '\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_basic (call 5)") {
  c6502::testing::check_tokens(
      "'~'",
      "1:1\011constant\011'~'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_basic (call 6)") {
  c6502::testing::check_tokens(
      "'!'",
      "1:1\011constant\011'!'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_basic (call 7)") {
  c6502::testing::check_tokens(
      "'\077'",
      "1:1\011constant\011'\077'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_basic (call 8)") {
  c6502::testing::check_tokens(
      "'{'",
      "1:1\011constant\011'{'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_empty_is_error (call 1)") {
  c6502::testing::check_tokens(
      "''",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_empty_is_error (call 2)") {
  c6502::testing::check_tokens(
      "L''",
      "{\"message\": \"1:2: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_hex_escape_requires_digit (call 1)") {
  c6502::testing::check_tokens(
      "'\\x'",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_hex_escape_requires_digit (call 2)") {
  c6502::testing::check_tokens(
      "'\\xG'",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_hex_escape_stops_at_non_hex (call 1)") {
  c6502::testing::check_tokens(
      "'\\x41x'",
      "1:1\011constant\011'\\x41x'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_hex_escapes (call 1)") {
  c6502::testing::check_tokens(
      "'\\x0'",
      "1:1\011constant\011'\\x0'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_hex_escapes (call 2)") {
  c6502::testing::check_tokens(
      "'\\xA'",
      "1:1\011constant\011'\\xA'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_hex_escapes (call 3)") {
  c6502::testing::check_tokens(
      "'\\x41'",
      "1:1\011constant\011'\\x41'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_hex_escapes (call 4)") {
  c6502::testing::check_tokens(
      "'\\xFF'",
      "1:1\011constant\011'\\xFF'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_hex_escapes (call 5)") {
  c6502::testing::check_tokens(
      "'\\xDEADBEEF'",
      "1:1\011constant\011'\\xDEADBEEF'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_invalid_escape_is_error (call 1)") {
  c6502::testing::check_tokens(
      "'\\q'",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_invalid_escape_is_error (call 2)") {
  c6502::testing::check_tokens(
      "'\\9'",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_invalid_escape_is_error (call 3)") {
  c6502::testing::check_tokens(
      "'\\Q'",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_invalid_escape_is_error (call 4)") {
  c6502::testing::check_tokens(
      "'\\e'",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_multi_char (call 1)") {
  c6502::testing::check_tokens(
      "'ab'",
      "1:1\011constant\011'ab'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_multi_char (call 2)") {
  c6502::testing::check_tokens(
      "'abc'",
      "1:1\011constant\011'abc'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_multi_char (call 3)") {
  c6502::testing::check_tokens(
      "'hello'",
      "1:1\011constant\011'hello'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_non_ascii_is_error (call 1)") {
  c6502::testing::check_tokens(
      "'\303\251'",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_non_printable_ascii_is_error (call 1)") {
  c6502::testing::check_tokens(
      "'\001'",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_non_printable_ascii_is_error (call 2)") {
  c6502::testing::check_tokens(
      "'\177'",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_octal_escapes (call 1)") {
  c6502::testing::check_tokens(
      "'\\0'",
      "1:1\011constant\011'\\0'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_octal_escapes (call 2)") {
  c6502::testing::check_tokens(
      "'\\7'",
      "1:1\011constant\011'\\7'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_octal_escapes (call 3)") {
  c6502::testing::check_tokens(
      "'\\12'",
      "1:1\011constant\011'\\12'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_octal_escapes (call 4)") {
  c6502::testing::check_tokens(
      "'\\77'",
      "1:1\011constant\011'\\77'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_octal_escapes (call 5)") {
  c6502::testing::check_tokens(
      "'\\123'",
      "1:1\011constant\011'\\123'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_octal_escapes (call 6)") {
  c6502::testing::check_tokens(
      "'\\000'",
      "1:1\011constant\011'\\000'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_octal_max_three_digits (call 1)") {
  c6502::testing::check_tokens(
      "'\\1234'",
      "1:1\011constant\011'\\1234'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_real_newline_inside_is_error (call 1)") {
  c6502::testing::check_tokens(
      "'a\n"
      "b'",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_simple_escapes (call 1)") {
  c6502::testing::check_tokens(
      "'\\''",
      "1:1\011constant\011'\\''\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_simple_escapes (call 2)") {
  c6502::testing::check_tokens(
      "'\\\"'",
      "1:1\011constant\011'\\\"'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_simple_escapes (call 3)") {
  c6502::testing::check_tokens(
      "'\\\077'",
      "1:1\011constant\011'\\\077'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_simple_escapes (call 4)") {
  c6502::testing::check_tokens(
      "'\\\\'",
      "1:1\011constant\011'\\\\'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_simple_escapes (call 5)") {
  c6502::testing::check_tokens(
      "'\\a'",
      "1:1\011constant\011'\\a'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_simple_escapes (call 6)") {
  c6502::testing::check_tokens(
      "'\\b'",
      "1:1\011constant\011'\\b'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_simple_escapes (call 7)") {
  c6502::testing::check_tokens(
      "'\\f'",
      "1:1\011constant\011'\\f'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_simple_escapes (call 8)") {
  c6502::testing::check_tokens(
      "'\\n'",
      "1:1\011constant\011'\\n'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_simple_escapes (call 9)") {
  c6502::testing::check_tokens(
      "'\\r'",
      "1:1\011constant\011'\\r'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_simple_escapes (call 10)") {
  c6502::testing::check_tokens(
      "'\\t'",
      "1:1\011constant\011'\\t'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_simple_escapes (call 11)") {
  c6502::testing::check_tokens(
      "'\\v'",
      "1:1\011constant\011'\\v'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_universal_escapes (call 1)") {
  c6502::testing::check_tokens(
      "'\\u0041'",
      "1:1\011constant\011'\\u0041'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_universal_escapes (call 2)") {
  c6502::testing::check_tokens(
      "'\\U0001F600'",
      "1:1\011constant\011'\\U0001F600'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_universal_requires_full_digit_count (call 1)") {
  c6502::testing::check_tokens(
      "'\\u'",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_universal_requires_full_digit_count (call 2)") {
  c6502::testing::check_tokens(
      "'\\u1'",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_universal_requires_full_digit_count (call 3)") {
  c6502::testing::check_tokens(
      "'\\u12'",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_universal_requires_full_digit_count (call 4)") {
  c6502::testing::check_tokens(
      "'\\u123'",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_universal_requires_full_digit_count (call 5)") {
  c6502::testing::check_tokens(
      "'\\u12G4'",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_universal_requires_full_digit_count (call 6)") {
  c6502::testing::check_tokens(
      "'\\U'",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_universal_requires_full_digit_count (call 7)") {
  c6502::testing::check_tokens(
      "'\\U1234'",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_universal_requires_full_digit_count (call 8)") {
  c6502::testing::check_tokens(
      "'\\U1234567'",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_universal_stops_at_exact_digit_count (call 1)") {
  c6502::testing::check_tokens(
      "'\\u12345'",
      "1:1\011constant\011'\\u12345'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_unterminated_is_error (call 1)") {
  c6502::testing::check_tokens(
      "'",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_unterminated_is_error (call 2)") {
  c6502::testing::check_tokens(
      "'a",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_unterminated_is_error (call 3)") {
  c6502::testing::check_tokens(
      "'abc",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_unterminated_is_error (call 4)") {
  c6502::testing::check_tokens(
      "L'",
      "{\"message\": \"1:2: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_unterminated_is_error (call 5)") {
  c6502::testing::check_tokens(
      "'\\n",
      "{\"message\": \"1:1: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_whitespace_chars_in_literal_accepted (call 1)") {
  c6502::testing::check_tokens(
      "'\011'",
      "1:1\011constant\011'\011'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_whitespace_chars_in_literal_accepted (call 2)") {
  c6502::testing::check_tokens(
      "'\013'",
      "1:1\011constant\011'\013'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_whitespace_chars_in_literal_accepted (call 3)") {
  c6502::testing::check_tokens(
      "'\014'",
      "1:1\011constant\011'\014'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_wide (call 1)") {
  c6502::testing::check_tokens(
      "L'a'",
      "1:1\011constant\011L'a'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestCharacterConstants::test_wide (call 2)") {
  c6502::testing::check_tokens(
      "L'abc'",
      "1:1\011constant\011L'abc'\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_L_space_quote_splits (call 1)") {
  c6502::testing::check_tokens(
      "L \"a\"",
      "1:1\011identifier\011L\n"
      "1:3\011string-literal\011\"a\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_adjacent_literals_are_separate_tokens (call 1)") {
  c6502::testing::check_tokens(
      "\"abc\"\"def\"",
      "1:1\011string-literal\011\"abc\"\n"
      "1:6\011string-literal\011\"def\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_basic (call 1)") {
  c6502::testing::check_tokens(
      "\"a\"",
      "1:1\011string-literal\011\"a\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_basic (call 2)") {
  c6502::testing::check_tokens(
      "\"hello\"",
      "1:1\011string-literal\011\"hello\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_basic (call 3)") {
  c6502::testing::check_tokens(
      "\"Hello, world!\"",
      "1:1\011string-literal\011\"Hello, world!\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_basic (call 4)") {
  c6502::testing::check_tokens(
      "\"123\"",
      "1:1\011string-literal\011\"123\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_basic (call 5)") {
  c6502::testing::check_tokens(
      "\"{}\"",
      "1:1\011string-literal\011\"{}\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_basic (call 6)") {
  c6502::testing::check_tokens(
      "\"  \"",
      "1:1\011string-literal\011\"  \"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_empty (call 1)") {
  c6502::testing::check_tokens(
      "\"\"",
      "1:1\011string-literal\011\"\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_empty (call 2)") {
  c6502::testing::check_tokens(
      "L\"\"",
      "1:1\011string-literal\011L\"\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_escape_sequences (call 1)") {
  c6502::testing::check_tokens(
      "\"\\n\"",
      "1:1\011string-literal\011\"\\n\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_escape_sequences (call 2)") {
  c6502::testing::check_tokens(
      "\"\\t\"",
      "1:1\011string-literal\011\"\\t\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_escape_sequences (call 3)") {
  c6502::testing::check_tokens(
      "\"\\\\\"",
      "1:1\011string-literal\011\"\\\\\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_escape_sequences (call 4)") {
  c6502::testing::check_tokens(
      "\"\\\077\"",
      "1:1\011string-literal\011\"\\\077\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_escape_sequences (call 5)") {
  c6502::testing::check_tokens(
      "\"\\x41\"",
      "1:1\011string-literal\011\"\\x41\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_escape_sequences (call 6)") {
  c6502::testing::check_tokens(
      "\"\\123\"",
      "1:1\011string-literal\011\"\\123\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_escape_sequences (call 7)") {
  c6502::testing::check_tokens(
      "\"\\u0041\"",
      "1:1\011string-literal\011\"\\u0041\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_escape_sequences (call 8)") {
  c6502::testing::check_tokens(
      "\"\\U0001F600\"",
      "1:1\011string-literal\011\"\\U0001F600\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_escaped_double_quote (call 1)") {
  c6502::testing::check_tokens(
      "\"\\\"a\\\"\"",
      "1:1\011string-literal\011\"\\\"a\\\"\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_in_function_call (call 1)") {
  c6502::testing::check_tokens(
      "puts(\"hi\");",
      "1:1\011identifier\011puts\n"
      "1:5\011symbol\011(\n"
      "1:6\011string-literal\011\"hi\"\n"
      "1:10\011symbol\011)\n"
      "1:11\011symbol\011;\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_invalid_escape_is_error (call 1)") {
  c6502::testing::check_tokens(
      "\"\\q\"",
      "{\"message\": \"1:1: unexpected character '\\\"'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_invalid_escape_is_error (call 2)") {
  c6502::testing::check_tokens(
      "\"\\9\"",
      "{\"message\": \"1:1: unexpected character '\\\"'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_invalid_escape_is_error (call 3)") {
  c6502::testing::check_tokens(
      "\"\\Q\"",
      "{\"message\": \"1:1: unexpected character '\\\"'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_non_ascii_is_error (call 1)") {
  c6502::testing::check_tokens(
      "\"caf\303\251\"",
      "{\"message\": \"1:1: unexpected character '\\\"'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_non_printable_is_error (call 1)") {
  c6502::testing::check_tokens(
      "\"a\001b\"",
      "{\"message\": \"1:1: unexpected character '\\\"'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_non_printable_is_error (call 2)") {
  c6502::testing::check_tokens(
      "\"a\177b\"",
      "{\"message\": \"1:1: unexpected character '\\\"'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_real_newline_inside_is_error (call 1)") {
  c6502::testing::check_tokens(
      "\"a\n"
      "b\"",
      "{\"message\": \"1:1: unexpected character '\\\"'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_single_quote_inside_is_literal (call 1)") {
  c6502::testing::check_tokens(
      "\"it's\"",
      "1:1\011string-literal\011\"it's\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_unterminated (call 1)") {
  c6502::testing::check_tokens(
      "\"",
      "{\"message\": \"1:1: unexpected character '\\\"'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_unterminated (call 2)") {
  c6502::testing::check_tokens(
      "\"abc",
      "{\"message\": \"1:1: unexpected character '\\\"'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_unterminated (call 3)") {
  c6502::testing::check_tokens(
      "L\"",
      "{\"message\": \"1:2: unexpected character '\\\"'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_unterminated (call 4)") {
  c6502::testing::check_tokens(
      "\"\\n",
      "{\"message\": \"1:1: unexpected character '\\\"'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_whitespace_chars_in_string_accepted (call 1)") {
  c6502::testing::check_tokens(
      "\"a\011b\"",
      "1:1\011string-literal\011\"a\011b\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_whitespace_chars_in_string_accepted (call 2)") {
  c6502::testing::check_tokens(
      "\"a\013b\"",
      "1:1\011string-literal\011\"a\013b\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_whitespace_chars_in_string_accepted (call 3)") {
  c6502::testing::check_tokens(
      "\"a\014b\"",
      "1:1\011string-literal\011\"a\014b\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestStringLiterals::test_wide (call 1)") {
  c6502::testing::check_tokens(
      "L\"hello\"",
      "1:1\011string-literal\011L\"hello\"\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestNoWhitespaceRequired::test_glued_arithmetic (call 1)") {
  c6502::testing::check_tokens(
      "1+2*3",
      "1:1\011constant\011" "1\n"
      "1:2\011symbol\011+\n"
      "1:3\011constant\011" "2\n"
      "1:4\011symbol\011*\n"
      "1:5\011constant\011" "3\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestNoWhitespaceRequired::test_identifier_adjacent_to_symbol (call 1)") {
  c6502::testing::check_tokens(
      "x->y",
      "1:1\011identifier\011x\n"
      "1:2\011symbol\011->\n"
      "1:4\011identifier\011y\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestNoWhitespaceRequired::test_keyword_adjacent_to_symbol (call 1)") {
  c6502::testing::check_tokens(
      "return;",
      "1:1\011keyword\011return\n"
      "1:7\011symbol\011;\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestNoWhitespaceRequired::test_tight_assignment (call 1)") {
  c6502::testing::check_tokens(
      "int x=42;",
      "1:1\011keyword\011int\n"
      "1:5\011identifier\011x\n"
      "1:6\011symbol\011=\n"
      "1:7\011constant\011" "42\n"
      "1:9\011symbol\011;\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestNoWhitespaceRequired::test_tight_assignment (call 2)") {
  c6502::testing::check_tokens(
      "int x = 42 ;",
      "1:1\011keyword\011int\n"
      "1:5\011identifier\011x\n"
      "1:7\011symbol\011=\n"
      "1:9\011constant\011" "42\n"
      "1:12\011symbol\011;\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestWhitespace::test_all_whitespace_chars_skipped (call 1)") {
  c6502::testing::check_tokens(
      " \011\n"
      "\014int\014\n"
      "\011 x",
      "2:2\011keyword\011int\n"
      "3:3\011identifier\011x\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestPositions::test_column_after_spaces (call 1)") {
  c6502::testing::check_tokens(
      "  int",
      "1:3\011keyword\011int\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestPositions::test_line_and_col_reset_after_newline (call 1)") {
  c6502::testing::check_tokens(
      "int\n"
      "x",
      "1:1\011keyword\011int\n"
      "2:1\011identifier\011x\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestPositions::test_tab_advances_one_column (call 1)") {
  c6502::testing::check_tokens(
      "\011x",
      "1:2\011identifier\011x\n",
      false);
}

TEST_CASE("tokens: test_lexer.py::TestErrors::test_error_carries_position (call 1)") {
  c6502::testing::check_tokens(
      "int\n"
      "  @",
      "{\"message\": \"2:3: unexpected character '@'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestErrors::test_unexpected_char (call 1)") {
  c6502::testing::check_tokens(
      "@",
      "{\"message\": \"1:1: unexpected character '@'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestErrors::test_unexpected_char (call 2)") {
  c6502::testing::check_tokens(
      "`",
      "{\"message\": \"1:1: unexpected character '`'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestErrors::test_unexpected_char (call 3)") {
  c6502::testing::check_tokens(
      "$",
      "{\"message\": \"1:1: unexpected character '$'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_lexer.py::TestSampleProgram::test_full_snippet (call 1)") {
  c6502::testing::check_tokens(
      "int main(void) {\n"
      "    int x = 42;\n"
      "    return x + 0xDEADBEEFull;\n"
      "}\n",
      "1:1\011keyword\011int\n"
      "1:5\011identifier\011main\n"
      "1:9\011symbol\011(\n"
      "1:10\011keyword\011void\n"
      "1:14\011symbol\011)\n"
      "1:16\011symbol\011{\n"
      "2:5\011keyword\011int\n"
      "2:9\011identifier\011x\n"
      "2:11\011symbol\011=\n"
      "2:13\011constant\011" "42\n"
      "2:15\011symbol\011;\n"
      "3:5\011keyword\011return\n"
      "3:12\011identifier\011x\n"
      "3:14\011symbol\011+\n"
      "3:16\011constant\011" "0xDEADBEEFull\n"
      "3:29\011symbol\011;\n"
      "4:1\011symbol\011}\n",
      false);
}
