// pcpp's lexer (preprocessor.py's `_PP_LEXER`, PLY's lexer with pcpp's
// rules and C23's pp-number) on texts that reach each of its rules and
// the consequences of their order, each with the tokens Python gives:
// their types, values, and line numbers.
#include <string>

#include "doctest.h"
#include "json.h"
#include "pcpp_lexer.h"

namespace {

using namespace c6502;

const char* const kCases[] = {
    "[\"identifier x_1 _a A9\", [[\"CPP_ID\", \"identifier\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_ID\", \"x_1\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_ID\", \"_a\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_ID\", \"A9\", 1]]]",
    "[\"12345 0x1F 1'000 0b1010 1.5e+3 .5 1.e5 08 1e 0x1p-3 1wb\", [[\"CPP_INTEGER\", \"12345\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_INTEGER\", \"0x1F\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_INTEGER\", \"1'000\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_INTEGER\", \"0b1010\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_INTEGER\", \"1.5e+3\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_INTEGER\", \".5\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_INTEGER\", \"1.e5\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_INTEGER\", \"08\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_INTEGER\", \"1e\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_INTEGER\", \"0x1p-3\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_INTEGER\", \"1wb\", 1]]]",
    "[\"\\\"str\\\" \\\"a\\\\\\\"b\\\" \\\"a\\\\\\\\\\\" \\\"unterminated\\n\", [[\"CPP_STRING\", \"\\\"str\\\"\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_STRING\", \"\\\"a\\\\\\\"b\\\"\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_STRING\", \"\\\"a\\\\\\\\\\\"\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_DQUOTE\", \"\\\"\", 1], [\"CPP_ID\", \"unterminated\", 1], [\"CPP_WS\", \"\\n\", 1]]]",
    "[\"\\\"cont\\\\\\nnued\\\" \\\"sp\\\\  \\nx\\\"\", [[\"CPP_STRING\", \"\\\"contnued\\\"\", 1], [\"CPP_WS\", \" \", 2], [\"CPP_DQUOTE\", \"\\\"\", 2], [\"CPP_ID\", \"sp\", 2], [\"CPP_LINECONT\", \"  \", 2], [\"CPP_ID\", \"x\", 3], [\"CPP_DQUOTE\", \"\\\"\", 3]]]",
    "[\"'c' L'x' '\\\\'' '' 'unterminated\\n\", [[\"CPP_CHAR\", \"'c'\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_CHAR\", \"L'x'\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_CHAR\", \"'\\\\''\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_CHAR\", \"''\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_SQUOTE\", \"'\", 1], [\"CPP_ID\", \"unterminated\", 1], [\"CPP_WS\", \"\\n\", 1]]]",
    "[\"L\\\"wide\\\" Lx\", [[\"CPP_ID\", \"L\", 1], [\"CPP_STRING\", \"\\\"wide\\\"\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_ID\", \"Lx\", 1]]]",
    "[\"/* c */ /* multi\\nline */ /*/ x */ // line\\nnext\", [[\"CPP_COMMENT1\", \"/* c */\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_COMMENT1\", \"/* multi\\nline */\", 1], [\"CPP_WS\", \" \", 2], [\"CPP_COMMENT1\", \"/*/ x */\", 2], [\"CPP_WS\", \" \", 2], [\"CPP_COMMENT2\", \"// line\", 2], [\"CPP_WS\", \"\\n\", 2], [\"CPP_ID\", \"next\", 3]]]",
    "[\"/* unterminated\", [[\"CPP_FSLASH\", \"/\", 1], [\"CPP_STAR\", \"*\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_ID\", \"unterminated\", 1]]]",
    "[\"## || ++ <<= *= |= += >>= &= | \\\\ -> /= . == >= ^ != [ <= && ( << -= -- %= + # ? ] ) >> * ^= & : , \\\" = ! / > { < - % } ; ' ~\", [[\"CPP_DPOUND\", \"##\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_LOGICALOR\", \"||\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_PLUSPLUS\", \"++\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_LSHIFTEQUAL\", \"<<=\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_MULTIPLYEQUAL\", \"*=\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_OREQUAL\", \"|=\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_PLUSEQUAL\", \"+=\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_RSHIFTEQUAL\", \">>=\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_ANDEQUAL\", \"&=\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_BAR\", \"|\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_BSLASH\", \"\\\\\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_DEREFERENCE\", \"->\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_DIVIDEEQUAL\", \"/=\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_DOT\", \".\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_EQUALITY\", \"==\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_GREATEREQUAL\", \">=\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_HAT\", \"^\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_INEQUALITY\", \"!=\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_LBRACKET\", \"[\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_LESSEQUAL\", \"<=\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_LOGICALAND\", \"&&\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_LPAREN\", \"(\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_LSHIFT\", \"<<\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_MINUSEQUAL\", \"-=\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_MINUSMINUS\", \"--\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_PERCENTEQUAL\", \"%=\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_PLUS\", \"+\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_POUND\", \"#\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_QUESTION\", \"?\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_RBRACKET\", \"]\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_RPAREN\", \")\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_RSHIFT\", \">>\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_STAR\", \"*\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_HAT\", \"^\", 1], [\"CPP_EQUAL\", \"=\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_AMPERSAND\", \"&\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_COLON\", \":\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_COMMA\", \",\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_DQUOTE\", \"\\\"\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_EQUAL\", \"=\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_EXCLAMATION\", \"!\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_FSLASH\", \"/\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_GREATER\", \">\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_LCURLY\", \"{\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_LESS\", \"<\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_MINUS\", \"-\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_PERCENT\", \"%\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_RCURLY\", \"}\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_SEMICOLON\", \";\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_SQUOTE\", \"'\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_TILDE\", \"~\", 1]]]",
    "[\"= x\", [[\"CPP_XOREQUAL\", \"=\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_ID\", \"x\", 1]]]",
    "[\"== y\", [[\"CPP_EQUALITY\", \"==\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_ID\", \"y\", 1]]]",
    "[\"=\", [[\"CPP_XOREQUAL\", \"=\", 1]]]",
    "[\"a ^= b\", [[\"CPP_ID\", \"a\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_HAT\", \"^\", 1], [\"CPP_EQUAL\", \"=\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_ID\", \"b\", 1]]]",
    "[\"...\", [[\"CPP_DOT\", \".\", 1], [\"CPP_DOT\", \".\", 1], [\"CPP_DOT\", \".\", 1]]]",
    "[\"->*\", [[\"CPP_DEREFERENCE\", \"->\", 1], [\"CPP_STAR\", \"*\", 1]]]",
    "[\"<<<\", [[\"CPP_LSHIFT\", \"<<\", 1], [\"CPP_LESS\", \"<\", 1]]]",
    "[\">>>=\", [[\"CPP_RSHIFT\", \">>\", 1], [\"CPP_GREATEREQUAL\", \">=\", 1]]]",
    "[\"a\\\\  \\nb\", [[\"CPP_ID\", \"a\", 1], [\"CPP_LINECONT\", \"  \", 1], [\"CPP_ID\", \"b\", 2]]]",
    "[\"a\\\\\\n\", [[\"CPP_ID\", \"a\", 1], [\"CPP_LINECONT\", \"\", 1]]]",
    "[\"\\\\\", [[\"CPP_BSLASH\", \"\\\\\", 1]]]",
    "[\"x\\fy\\u000bz\\rw\", [[\"CPP_ID\", \"x\", 1], [\"\\f\", \"\\f\", 1], [\"CPP_ID\", \"y\", 1], [\"\\u000b\", \"\\u000b\", 1], [\"CPP_ID\", \"z\", 1], [\"\\r\", \"\\r\", 1], [\"CPP_ID\", \"w\", 1]]]",
    "[\"@ $ ` \\u0000\", [[\"@\", \"@\", 1], [\"CPP_WS\", \" \", 1], [\"$\", \"$\", 1], [\"CPP_WS\", \" \", 1], [\"`\", \"`\", 1], [\"CPP_WS\", \" \", 1], [\"\\u0000\", \"\\u0000\", 1]]]",
    "[\"\\u00e9 \\u03b1\\u03b2\\u03b3 x\\u0663 \\u0663.\\u0664 \\u0663e\\u0664 \\u0663\\u0664L\", [[\"\\u00e9\", \"\\u00e9\", 1], [\"CPP_WS\", \" \", 1], [\"\\u03b1\", \"\\u03b1\", 1], [\"\\u03b2\", \"\\u03b2\", 1], [\"\\u03b3\", \"\\u03b3\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_ID\", \"x\\u0663\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_FLOAT\", \"\\u0663.\\u0664\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_FLOAT\", \"\\u0663e\\u0664\", 1], [\"CPP_WS\", \" \", 1], [\"\\u0663\", \"\\u0663\", 1], [\"\\u0664\", \"\\u0664\", 1], [\"CPP_ID\", \"L\", 1]]]",
    "[\"a\\u2028b\", [[\"CPP_ID\", \"a\", 1], [\"\\u2028\", \"\\u2028\", 1], [\"CPP_ID\", \"b\", 1]]]",
    "[\"\\t \\t\\n\\n  \", [[\"CPP_WS\", \"\\t \\t\", 1], [\"CPP_WS\", \"\\n\", 1], [\"CPP_WS\", \"\\n\", 2], [\"CPP_WS\", \"  \", 3]]]",
    "[\"#define X(a,b) a##b\", [[\"CPP_POUND\", \"#\", 1], [\"CPP_ID\", \"define\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_ID\", \"X\", 1], [\"CPP_LPAREN\", \"(\", 1], [\"CPP_ID\", \"a\", 1], [\"CPP_COMMA\", \",\", 1], [\"CPP_ID\", \"b\", 1], [\"CPP_RPAREN\", \")\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_ID\", \"a\", 1], [\"CPP_DPOUND\", \"##\", 1], [\"CPP_ID\", \"b\", 1]]]",
    "[\"#if defined(X) && X > 0x10\", [[\"CPP_POUND\", \"#\", 1], [\"CPP_ID\", \"if\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_ID\", \"defined\", 1], [\"CPP_LPAREN\", \"(\", 1], [\"CPP_ID\", \"X\", 1], [\"CPP_RPAREN\", \")\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_LOGICALAND\", \"&&\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_ID\", \"X\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_GREATER\", \">\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_INTEGER\", \"0x10\", 1]]]",
    "[\"a/b/=c\", [[\"CPP_ID\", \"a\", 1], [\"CPP_FSLASH\", \"/\", 1], [\"CPP_ID\", \"b\", 1], [\"CPP_DIVIDEEQUAL\", \"/=\", 1], [\"CPP_ID\", \"c\", 1]]]",
    "[\"1.2.3 1..2 1e+e+5\", [[\"CPP_INTEGER\", \"1.2.3\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_INTEGER\", \"1..2\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_INTEGER\", \"1e+e+5\", 1]]]",
    "[\"'\\\\x41' '\\\\101' '\\\\n'\", [[\"CPP_CHAR\", \"'\\\\x41'\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_CHAR\", \"'\\\\101'\", 1], [\"CPP_WS\", \" \", 1], [\"CPP_CHAR\", \"'\\\\n'\", 1]]]",
    "[\"x+++++y\", [[\"CPP_ID\", \"x\", 1], [\"CPP_PLUSPLUS\", \"++\", 1], [\"CPP_PLUSPLUS\", \"++\", 1], [\"CPP_PLUS\", \"+\", 1], [\"CPP_ID\", \"y\", 1]]]",
    "[\"a<:b:>\", [[\"CPP_ID\", \"a\", 1], [\"CPP_LESS\", \"<\", 1], [\"CPP_COLON\", \":\", 1], [\"CPP_ID\", \"b\", 1], [\"CPP_COLON\", \":\", 1], [\"CPP_GREATER\", \">\", 1]]]",
    "[\"%:%:\", [[\"CPP_PERCENT\", \"%\", 1], [\"CPP_COLON\", \":\", 1], [\"CPP_PERCENT\", \"%\", 1], [\"CPP_COLON\", \":\", 1]]]",
    "[\"??=\", [[\"CPP_QUESTION\", \"?\", 1], [\"CPP_QUESTION\", \"?\", 1], [\"CPP_EQUAL\", \"=\", 1]]]",
};

}  // namespace

TEST_CASE("pcpp's lexer gives Python's tokens") {
  for (const char* text : kCases) {
    const json::Value c = json::parse(text);
    pcpp::Lexer lexer;
    lexer.input(c.items[0].string);
    std::string got;
    std::string want;
    while (auto tok = lexer.token()) {
      got += pcpp::type_name(*tok) + " " + tok->value + " " + std::to_string(tok->lineno) + "\n";
    }
    for (const auto& t : c.items[1].items) {
      want += t.items[0].string + " " + t.items[1].string + " " +
              asdl::int_to_string(t.items[2].integer) + "\n";
    }
    INFO(c.items[0].string);
    CHECK(got == want);
  }
}
