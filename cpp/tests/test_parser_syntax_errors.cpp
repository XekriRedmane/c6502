// The syntax errors of the parse, recorded from Python: for each
// distinct message that the Python tests and the corpus get from Lark
// (with parser.py's rewording) or from the lexer's check of a number,
// the shortest source that gives it, with the tokens Python's parser
// was fed and the positions of the identifiers its scopes named as
// typedefs (tools/capture_parser_tokens.py). Then the distinct
// LexErrors of lexer.py's `tokenize`. Recorded by a script from the
// records of `parse_tokens` and `tokenize`.
#include <string>
#include <vector>

#include "doctest.h"
#include "errors.h"
#include "parser_token_checks.h"

namespace {

struct Case {
  std::string source;
  std::vector<int> lex_typedefs;
  std::vector<int> retype_typedefs;
  // The record's result, `[tokens, outcome]` as JSON.
  std::string expected;
};

const Case kCases[] = {
    {std::string(R"~~(int x = 1''0;)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "x", 1, 5, 1], ["ASSIGN", "=", 1, 7, 1]], ["lex", "LexError", "1:9: a digit separator has to be between two digits, and the one after '1' in \"1''0\" isn't followed by a digit"]])~~")},
    {std::string(R"~~(int x = 0x'FF;)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "x", 1, 5, 1], ["ASSIGN", "=", 1, 7, 1]], ["lex", "LexError", "1:9: a digit separator has to be between two digits, and the one in \"0x'FF\" comes right after the prefix 0x"]])~~")},
    {std::string(R"~~(int x = 0x1'0000'0000'0000'0000uwb;)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "x", 1, 5, 1], ["ASSIGN", "=", 1, 7, 1]], ["lex", "LexError", "1:9: integer constant \"0x1'0000'0000'0000'0000uwb\" needs a unsigned _BitInt(65), and BITINT_MAXWIDTH is 64 (C23 \u00a76.4.4.2p6)"]])~~")},
    {std::string(R"~~(int x = 0b10000000000000000000000000000000000000000000000000000000000000000wb;)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "x", 1, 5, 1], ["ASSIGN", "=", 1, 7, 1]], ["lex", "LexError", "1:9: integer constant '0b10000000000000000000000000000000000000000000000000000000000000000wb' needs a _BitInt(66), and BITINT_MAXWIDTH is 64 (C23 \u00a76.4.4.2p6)"]])~~")},
    {std::string(R"~~(int x = 0x8000000000000000wb;)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "x", 1, 5, 1], ["ASSIGN", "=", 1, 7, 1]], ["lex", "LexError", "1:9: integer constant '0x8000000000000000wb' needs a _BitInt(65), and BITINT_MAXWIDTH is 64 (C23 \u00a76.4.4.2p6)"]])~~")},
    {std::string(R"~~(int x = 9223372036854775808wb;)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "x", 1, 5, 1], ["ASSIGN", "=", 1, 7, 1]], ["lex", "LexError", "1:9: integer constant '9223372036854775808wb' needs a _BitInt(65), and BITINT_MAXWIDTH is 64 (C23 \u00a76.4.4.2p6)"]])~~")},
    {std::string(R"~~(int x = 0b12;)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "x", 1, 5, 1], ["ASSIGN", "=", 1, 7, 1]], ["lex", "LexError", "1:9: invalid digit '2' in binary constant '0b12'"]])~~")},
    {std::string(R"~~(int x = 5uwbu;)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "x", 1, 5, 1], ["ASSIGN", "=", 1, 7, 1]], ["lex", "LexError", "1:9: invalid suffix 'uwbu' on integer constant '5uwbu'"]])~~")},
    {std::string(R"~~(char *s = "hello
    world ";

    int
    main(void)
{
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["CHAR", "char", 1, 1, 1], ["STAR", "*", 1, 6, 1], ["IDENTIFIER", "s", 1, 7, 1], ["ASSIGN", "=", 1, 9, 1]], ["syntax", "UnexpectedCharacters", "No terminal matches '\"' in the current parser context, at line 1 col 11\n\nchar *s = \"hello\n          ^\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* CARET\n\t* CARET_ASSIGN\n\t* CHAR_CONSTANT\n\t* COLON\n\t* COLONCOLON\n\t* COMMA\n\t* DOT\n\t* DOUBLE_CONSTANT\n\t* ELLIPSIS\n\t* EQ\n\t* FLOAT_CONSTANT\n\t* GE\n\t* GT\n\t* HASH\n\t* HASHHASH\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* INVALID_NUMBER\n\t* LBRACE\n\t* LBRACKET\n\t* LE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RBRACE\n\t* RBRACKET\n\t* RPAREN\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n\t* STRING_LITERAL\n\t* TILDE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\nPrevious tokens: Token('ASSIGN', '=')\n"]])~~")},
    {std::string(R"~~(int main(void) {
    char *ptr = "foo\";
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["CHAR", "char", 2, 5, 2], ["STAR", "*", 2, 10, 2], ["IDENTIFIER", "ptr", 2, 11, 2], ["ASSIGN", "=", 2, 15, 2]], ["syntax", "UnexpectedCharacters", "No terminal matches '\"' in the current parser context, at line 2 col 17\n\n    char *ptr = \"foo\\\";\n                ^\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* CARET\n\t* CARET_ASSIGN\n\t* CHAR_CONSTANT\n\t* COLON\n\t* COLONCOLON\n\t* COMMA\n\t* DOT\n\t* DOUBLE_CONSTANT\n\t* ELLIPSIS\n\t* EQ\n\t* FLOAT_CONSTANT\n\t* GE\n\t* GT\n\t* HASH\n\t* HASHHASH\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* INVALID_NUMBER\n\t* LBRACE\n\t* LBRACKET\n\t* LE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RBRACE\n\t* RBRACKET\n\t* RPAREN\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n\t* STRING_LITERAL\n\t* TILDE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\nPrevious tokens: Token('ASSIGN', '=')\n"]])~~")},
    {std::string(R"~~(int main(void)
{
    char *str = "foo\ybar";
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["CHAR", "char", 3, 5, 2], ["STAR", "*", 3, 10, 2], ["IDENTIFIER", "str", 3, 11, 2], ["ASSIGN", "=", 3, 15, 2]], ["syntax", "UnexpectedCharacters", "No terminal matches '\"' in the current parser context, at line 3 col 17\n\n    char *str = \"foo\\ybar\";\n                ^\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* CARET\n\t* CARET_ASSIGN\n\t* CHAR_CONSTANT\n\t* COLON\n\t* COLONCOLON\n\t* COMMA\n\t* DOT\n\t* DOUBLE_CONSTANT\n\t* ELLIPSIS\n\t* EQ\n\t* FLOAT_CONSTANT\n\t* GE\n\t* GT\n\t* HASH\n\t* HASHHASH\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* INVALID_NUMBER\n\t* LBRACE\n\t* LBRACKET\n\t* LE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RBRACE\n\t* RBRACKET\n\t* RPAREN\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n\t* STRING_LITERAL\n\t* TILDE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\nPrevious tokens: Token('ASSIGN', '=')\n"]])~~")},
    {std::string(R"~~(int main(void)
{
    return ''';
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["RETURN", "return", 3, 5, 2]], ["syntax", "UnexpectedCharacters", "No terminal matches ''' in the current parser context, at line 3 col 12\n\n    return ''';\n           ^\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* CARET\n\t* CARET_ASSIGN\n\t* CHAR_CONSTANT\n\t* COLON\n\t* COLONCOLON\n\t* COMMA\n\t* DOT\n\t* DOUBLE_CONSTANT\n\t* ELLIPSIS\n\t* EQ\n\t* FLOAT_CONSTANT\n\t* GE\n\t* GT\n\t* HASH\n\t* HASHHASH\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* INVALID_NUMBER\n\t* LBRACE\n\t* LBRACKET\n\t* LE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RBRACE\n\t* RBRACKET\n\t* RPAREN\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n\t* STRING_LITERAL\n\t* TILDE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\nPrevious tokens: Token('RETURN', 'return')\n"]])~~")},
    {std::string(R"~~(int main(void)
{
    return '\';
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["RETURN", "return", 3, 5, 2]], ["syntax", "UnexpectedCharacters", "No terminal matches ''' in the current parser context, at line 3 col 12\n\n    return '\\';\n           ^\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* CARET\n\t* CARET_ASSIGN\n\t* CHAR_CONSTANT\n\t* COLON\n\t* COLONCOLON\n\t* COMMA\n\t* DOT\n\t* DOUBLE_CONSTANT\n\t* ELLIPSIS\n\t* EQ\n\t* FLOAT_CONSTANT\n\t* GE\n\t* GT\n\t* HASH\n\t* HASHHASH\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* INVALID_NUMBER\n\t* LBRACE\n\t* LBRACKET\n\t* LE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RBRACE\n\t* RBRACKET\n\t* RPAREN\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n\t* STRING_LITERAL\n\t* TILDE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\nPrevious tokens: Token('RETURN', 'return')\n"]])~~")},
    {std::string(R"~~(int main(void)
{
    return '\y';
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["RETURN", "return", 3, 5, 2]], ["syntax", "UnexpectedCharacters", "No terminal matches ''' in the current parser context, at line 3 col 12\n\n    return '\\y';\n           ^\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* CARET\n\t* CARET_ASSIGN\n\t* CHAR_CONSTANT\n\t* COLON\n\t* COLONCOLON\n\t* COMMA\n\t* DOT\n\t* DOUBLE_CONSTANT\n\t* ELLIPSIS\n\t* EQ\n\t* FLOAT_CONSTANT\n\t* GE\n\t* GT\n\t* HASH\n\t* HASHHASH\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* INVALID_NUMBER\n\t* LBRACE\n\t* LBRACKET\n\t* LE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RBRACE\n\t* RBRACKET\n\t* RPAREN\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n\t* STRING_LITERAL\n\t* TILDE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\nPrevious tokens: Token('RETURN', 'return')\n"]])~~")},
    {std::string(R"~~(int main(void)
{
    return 'x
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["RETURN", "return", 3, 5, 2]], ["syntax", "UnexpectedCharacters", "No terminal matches ''' in the current parser context, at line 3 col 12\n\n    return 'x\n           ^\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* CARET\n\t* CARET_ASSIGN\n\t* CHAR_CONSTANT\n\t* COLON\n\t* COLONCOLON\n\t* COMMA\n\t* DOT\n\t* DOUBLE_CONSTANT\n\t* ELLIPSIS\n\t* EQ\n\t* FLOAT_CONSTANT\n\t* GE\n\t* GT\n\t* HASH\n\t* HASHHASH\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* INVALID_NUMBER\n\t* LBRACE\n\t* LBRACKET\n\t* LE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RBRACE\n\t* RBRACKET\n\t* RPAREN\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n\t* STRING_LITERAL\n\t* TILDE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\nPrevious tokens: Token('RETURN', 'return')\n"]])~~")},
    {std::string(R"~~(int main(void) { return @; })~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 1, 18, 2]], ["syntax", "UnexpectedCharacters", "No terminal matches '@' in the current parser context, at line 1 col 25\n\nint main(void) { return @; }\n                        ^\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* CARET\n\t* CARET_ASSIGN\n\t* CHAR_CONSTANT\n\t* COLON\n\t* COLONCOLON\n\t* COMMA\n\t* DOT\n\t* DOUBLE_CONSTANT\n\t* ELLIPSIS\n\t* EQ\n\t* FLOAT_CONSTANT\n\t* GE\n\t* GT\n\t* HASH\n\t* HASHHASH\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* INVALID_NUMBER\n\t* LBRACE\n\t* LBRACKET\n\t* LE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RBRACE\n\t* RBRACKET\n\t* RPAREN\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n\t* STRING_LITERAL\n\t* TILDE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\nPrevious tokens: Token('RETURN', 'return')\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return 0@1;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["INTEGER_CONSTANT", "0", 2, 12, 2]], ["syntax", "UnexpectedCharacters", "No terminal matches '@' in the current parser context, at line 2 col 13\n\n    return 0@1;\n            ^\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* CARET\n\t* CARET_ASSIGN\n\t* CHAR_CONSTANT\n\t* COLON\n\t* COLONCOLON\n\t* COMMA\n\t* DOT\n\t* DOUBLE_CONSTANT\n\t* ELLIPSIS\n\t* EQ\n\t* FLOAT_CONSTANT\n\t* GE\n\t* GT\n\t* HASH\n\t* HASHHASH\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* INVALID_NUMBER\n\t* LBRACE\n\t* LBRACKET\n\t* LE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RBRACE\n\t* RBRACKET\n\t* RPAREN\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n\t* STRING_LITERAL\n\t* TILDE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\nPrevious tokens: Token('INTEGER_CONSTANT', '0')\n"]])~~")},
    {std::string(R"~~(int main(void)
{
    return @b;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["RETURN", "return", 3, 5, 2]], ["syntax", "UnexpectedCharacters", "No terminal matches '@' in the current parser context, at line 3 col 12\n\n    return @b;\n           ^\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* CARET\n\t* CARET_ASSIGN\n\t* CHAR_CONSTANT\n\t* COLON\n\t* COLONCOLON\n\t* COMMA\n\t* DOT\n\t* DOUBLE_CONSTANT\n\t* ELLIPSIS\n\t* EQ\n\t* FLOAT_CONSTANT\n\t* GE\n\t* GT\n\t* HASH\n\t* HASHHASH\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* INVALID_NUMBER\n\t* LBRACE\n\t* LBRACKET\n\t* LE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RBRACE\n\t* RBRACKET\n\t* RPAREN\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n\t* STRING_LITERAL\n\t* TILDE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\nPrevious tokens: Token('RETURN', 'return')\n"]])~~")},
    {std::string(R"~~(

int main(void) {
    return 0@1;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 3, 1, 1], ["IDENTIFIER", "main", 3, 5, 1], ["LPAREN", "(", 3, 9, 1], ["VOID", "void", 3, 10, 1], ["RPAREN", ")", 3, 14, 1], ["LBRACE", "{", 3, 16, 2], ["RETURN", "return", 4, 5, 2], ["INTEGER_CONSTANT", "0", 4, 12, 2]], ["syntax", "UnexpectedCharacters", "No terminal matches '@' in the current parser context, at line 4 col 13\n\n    return 0@1;\n            ^\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* CARET\n\t* CARET_ASSIGN\n\t* CHAR_CONSTANT\n\t* COLON\n\t* COLONCOLON\n\t* COMMA\n\t* DOT\n\t* DOUBLE_CONSTANT\n\t* ELLIPSIS\n\t* EQ\n\t* FLOAT_CONSTANT\n\t* GE\n\t* GT\n\t* HASH\n\t* HASHHASH\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* INVALID_NUMBER\n\t* LBRACE\n\t* LBRACKET\n\t* LE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RBRACE\n\t* RBRACKET\n\t* RPAREN\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n\t* STRING_LITERAL\n\t* TILDE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\nPrevious tokens: Token('INTEGER_CONSTANT', '0')\n"]])~~")},
    {std::string(R"~~(\
)~~"),
     {},
     {},
     std::string(R"~~([[], ["syntax", "UnexpectedCharacters", "No terminal matches '\\' in the current parser context, at line 1 col 1\n\n\\\n^\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* CARET\n\t* CARET_ASSIGN\n\t* CHAR_CONSTANT\n\t* COLON\n\t* COLONCOLON\n\t* COMMA\n\t* DOT\n\t* DOUBLE_CONSTANT\n\t* ELLIPSIS\n\t* EQ\n\t* FLOAT_CONSTANT\n\t* GE\n\t* GT\n\t* HASH\n\t* HASHHASH\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* INVALID_NUMBER\n\t* LBRACE\n\t* LBRACKET\n\t* LE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RBRACE\n\t* RBRACKET\n\t* RPAREN\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n\t* STRING_LITERAL\n\t* TILDE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(
\
)~~"),
     {},
     {},
     std::string(R"~~([[], ["syntax", "UnexpectedCharacters", "No terminal matches '\\' in the current parser context, at line 2 col 1\n\n\\\n^\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* CARET\n\t* CARET_ASSIGN\n\t* CHAR_CONSTANT\n\t* COLON\n\t* COLONCOLON\n\t* COMMA\n\t* DOT\n\t* DOUBLE_CONSTANT\n\t* ELLIPSIS\n\t* EQ\n\t* FLOAT_CONSTANT\n\t* GE\n\t* GT\n\t* HASH\n\t* HASHHASH\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* INVALID_NUMBER\n\t* LBRACE\n\t* LBRACKET\n\t* LE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RBRACE\n\t* RBRACKET\n\t* RPAREN\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n\t* STRING_LITERAL\n\t* TILDE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(`
)~~"),
     {},
     {},
     std::string(R"~~([[], ["syntax", "UnexpectedCharacters", "No terminal matches '`' in the current parser context, at line 1 col 1\n\n`\n^\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* CARET\n\t* CARET_ASSIGN\n\t* CHAR_CONSTANT\n\t* COLON\n\t* COLONCOLON\n\t* COMMA\n\t* DOT\n\t* DOUBLE_CONSTANT\n\t* ELLIPSIS\n\t* EQ\n\t* FLOAT_CONSTANT\n\t* GE\n\t* GT\n\t* HASH\n\t* HASHHASH\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* INVALID_NUMBER\n\t* LBRACE\n\t* LBRACKET\n\t* LE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RBRACE\n\t* RBRACKET\n\t* RPAREN\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n\t* STRING_LITERAL\n\t* TILDE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(
`
)~~"),
     {},
     {},
     std::string(R"~~([[], ["syntax", "UnexpectedCharacters", "No terminal matches '`' in the current parser context, at line 2 col 1\n\n`\n^\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* CARET\n\t* CARET_ASSIGN\n\t* CHAR_CONSTANT\n\t* COLON\n\t* COLONCOLON\n\t* COMMA\n\t* DOT\n\t* DOUBLE_CONSTANT\n\t* ELLIPSIS\n\t* EQ\n\t* FLOAT_CONSTANT\n\t* GE\n\t* GT\n\t* HASH\n\t* HASHHASH\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* INVALID_NUMBER\n\t* LBRACE\n\t* LBRACKET\n\t* LE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RBRACE\n\t* RBRACKET\n\t* RPAREN\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n\t* STRING_LITERAL\n\t* TILDE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void) { return 0)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 1, 18, 2], ["INTEGER_CONSTANT", "0", 1, 25, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('$END', '') at line 1, column 25.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return 0;
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["INTEGER_CONSTANT", "0", 2, 12, 2], ["SEMICOLON", ";", 2, 13, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('$END', '') at line 2, column 13.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* AMP\n\t* ATTR_OPEN\n\t* AUTO\n\t* BANG\n\t* BITINT\n\t* BITINT_INTEGER\n\t* BOOL\n\t* BREAK\n\t* CASE\n\t* CHAR\n\t* CHAR_CONSTANT\n\t* CONST\n\t* CONSTEXPR\n\t* CONTINUE\n\t* DEFAULT\n\t* DO\n\t* DOUBLE\n\t* DOUBLE_CONSTANT\n\t* ENUM\n\t* EXTERN\n\t* FALSE\n\t* FLOAT\n\t* FLOAT_CONSTANT\n\t* FOR\n\t* GOTO\n\t* IDENTIFIER\n\t* IF\n\t* INT\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LONG\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* RBRACE\n\t* REGISTER\n\t* RESTRICT\n\t* RETURN\n\t* SEMICOLON\n\t* SHORT\n\t* SIGNED\n\t* SIZEOF\n\t* STAR\n\t* STATIC\n\t* STRING_LITERAL\n\t* STRUCT\n\t* SWITCH\n\t* THREAD_LOCAL\n\t* TILDE\n\t* TRUE\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n\t* WHILE\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('$END', '') at line 2, column 5.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SEMICOLON\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void) {
    if(0){
        return 1;
    return 2;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["IF", "if", 2, 5, 2], ["LPAREN", "(", 2, 7, 2], ["INTEGER_CONSTANT", "0", 2, 8, 2], ["RPAREN", ")", 2, 9, 2], ["LBRACE", "{", 2, 10, 3], ["RETURN", "return", 3, 9, 3], ["INTEGER_CONSTANT", "1", 3, 16, 3], ["SEMICOLON", ";", 3, 17, 3], ["RETURN", "return", 4, 5, 3], ["INTEGER_CONSTANT", "2", 4, 12, 3], ["SEMICOLON", ";", 4, 13, 3], ["RBRACE", "}", 5, 1, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('$END', '') at line 5, column 1.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* AMP\n\t* ATTR_OPEN\n\t* AUTO\n\t* BANG\n\t* BITINT\n\t* BITINT_INTEGER\n\t* BOOL\n\t* BREAK\n\t* CASE\n\t* CHAR\n\t* CHAR_CONSTANT\n\t* CONST\n\t* CONSTEXPR\n\t* CONTINUE\n\t* DEFAULT\n\t* DO\n\t* DOUBLE\n\t* DOUBLE_CONSTANT\n\t* ELSE\n\t* ENUM\n\t* EXTERN\n\t* FALSE\n\t* FLOAT\n\t* FLOAT_CONSTANT\n\t* FOR\n\t* GOTO\n\t* IDENTIFIER\n\t* IF\n\t* INT\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LONG\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* RBRACE\n\t* REGISTER\n\t* RESTRICT\n\t* RETURN\n\t* SEMICOLON\n\t* SHORT\n\t* SIGNED\n\t* SIZEOF\n\t* STAR\n\t* STATIC\n\t* STRING_LITERAL\n\t* STRUCT\n\t* SWITCH\n\t* THREAD_LOCAL\n\t* TILDE\n\t* TRUE\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n\t* WHILE\n"]])~~")},
    {std::string(R"~~(int _BitInt = 1;)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["BITINT", "_BitInt", 1, 5, 1], ["ASSIGN", "=", 1, 13, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('ASSIGN', '=') at line 1, column 13.\nExpected one of: \n\t* LPAREN\n"]])~~")},
    {std::string(R"~~(int bad_params(int a = 3) {
    return 1;
}

int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "bad_params", 1, 5, 1], ["LPAREN", "(", 1, 15, 1], ["INT", "int", 1, 16, 1], ["IDENTIFIER", "a", 1, 20, 1], ["ASSIGN", "=", 1, 22, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('ASSIGN', '=') at line 1, column 22.\nExpected one of: \n\t* COMMA\n\t* RPAREN\n"]])~~")},
    {std::string(R"~~(int main(void) { int x; switch (x) { case x = 1: return 0; } })~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 1, 18, 2], ["IDENTIFIER", "x", 1, 22, 2], ["SEMICOLON", ";", 1, 23, 2], ["SWITCH", "switch", 1, 25, 2], ["LPAREN", "(", 1, 32, 2], ["IDENTIFIER", "x", 1, 33, 2], ["RPAREN", ")", 1, 34, 2], ["LBRACE", "{", 1, 36, 3], ["CASE", "case", 1, 38, 3], ["IDENTIFIER", "x", 1, 43, 3], ["ASSIGN", "=", 1, 45, 3]], ["syntax", "UnexpectedToken", "Unexpected token Token('ASSIGN', '=') at line 1, column 45.\nExpected one of: \n\t* COLON\n"]])~~")},
    {std::string(R"~~(
int bad_params(int a = 3) {
    return 1;
}

int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 2, 1, 1], ["IDENTIFIER", "bad_params", 2, 5, 1], ["LPAREN", "(", 2, 15, 1], ["INT", "int", 2, 16, 1], ["IDENTIFIER", "a", 2, 20, 1], ["ASSIGN", "=", 2, 22, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('ASSIGN', '=') at line 2, column 22.\nExpected one of: \n\t* COMMA\n\t* RPAREN\n"]])~~")},
    {std::string(R"~~(int main(void) {

    int long = 5;
    return long;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 3, 5, 2], ["LONG", "long", 3, 9, 2], ["ASSIGN", "=", 3, 14, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('ASSIGN', '=') at line 3, column 14.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* IDENTIFIER\n\t* INT\n\t* LONG\n\t* LPAREN\n\t* REGISTER\n\t* RESTRICT\n\t* SEMICOLON\n\t* SHORT\n\t* SIGNED\n\t* STAR\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(union a {

    int member = 1;
};
)~~"),
     {},
     {},
     std::string(R"~~([[["UNION", "union", 1, 1, 1], ["IDENTIFIER", "a", 1, 7, 1], ["LBRACE", "{", 1, 9, 1], ["INT", "int", 3, 5, 1], ["IDENTIFIER", "member", 3, 9, 1], ["ASSIGN", "=", 3, 16, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('ASSIGN', '=') at line 3, column 16.\nExpected one of: \n\t* COMMA\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(int main(void) {
    int a = 0;
    a + = 1;
    return a;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 2, 5, 2], ["IDENTIFIER", "a", 2, 9, 2], ["ASSIGN", "=", 2, 11, 2], ["INTEGER_CONSTANT", "0", 2, 13, 2], ["SEMICOLON", ";", 2, 14, 2], ["IDENTIFIER", "a", 3, 5, 2], ["PLUS", "+", 3, 7, 2], ["ASSIGN", "=", 3, 9, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('ASSIGN', '=') at line 3, column 9.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void)
{



    return 1 < = 2;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["RETURN", "return", 6, 5, 2], ["INTEGER_CONSTANT", "1", 6, 12, 2], ["LT", "<", 6, 14, 2], ["ASSIGN", "=", 6, 16, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('ASSIGN', '=') at line 6, column 16.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(


int main(void) {
    int a = 0;
    a + = 1;
    return a;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 4, 1, 1], ["IDENTIFIER", "main", 4, 5, 1], ["LPAREN", "(", 4, 9, 1], ["VOID", "void", 4, 10, 1], ["RPAREN", ")", 4, 14, 1], ["LBRACE", "{", 4, 16, 2], ["INT", "int", 5, 5, 2], ["IDENTIFIER", "a", 5, 9, 2], ["ASSIGN", "=", 5, 11, 2], ["INTEGER_CONSTANT", "0", 5, 13, 2], ["SEMICOLON", ";", 5, 14, 2], ["IDENTIFIER", "a", 6, 5, 2], ["PLUS", "+", 6, 7, 2], ["ASSIGN", "=", 6, 9, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('ASSIGN', '=') at line 6, column 9.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int a[3] [[c6502::zeropage]];)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "a", 1, 5, 1], ["LBRACKET", "[", 1, 6, 1], ["INTEGER_CONSTANT", "3", 1, 7, 1], ["RBRACKET", "]", 1, 8, 1], ["ATTR_OPEN", "[[", 1, 10, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('ATTR_OPEN', '[[') at line 1, column 10.\nExpected one of: \n\t* ASSIGN\n\t* COMMA\n\t* LBRACE\n\t* LBRACKET\n\t* LPAREN\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(enum { A } [[deprecated]];)~~"),
     {},
     {},
     std::string(R"~~([[["ENUM", "enum", 1, 1, 1], ["LBRACE", "{", 1, 6, 1], ["IDENTIFIER", "A", 1, 8, 1], ["RBRACE", "}", 1, 10, 1], ["ATTR_OPEN", "[[", 1, 12, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('ATTR_OPEN', '[[') at line 1, column 12.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* IDENTIFIER\n\t* INT\n\t* LONG\n\t* LPAREN\n\t* REGISTER\n\t* RESTRICT\n\t* SEMICOLON\n\t* SHORT\n\t* SIGNED\n\t* STAR\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(void f(void) [[c6502::zp_abi]];)~~"),
     {},
     {},
     std::string(R"~~([[["VOID", "void", 1, 1, 1], ["IDENTIFIER", "f", 1, 6, 1], ["LPAREN", "(", 1, 7, 1], ["VOID", "void", 1, 8, 1], ["RPAREN", ")", 1, 12, 1], ["ATTR_OPEN", "[[", 1, 14, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('ATTR_OPEN', '[[') at line 1, column 14.\nExpected one of: \n\t* ASSIGN\n\t* COMMA\n\t* LBRACE\n\t* LBRACKET\n\t* LPAREN\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(int [[c6502::zeropage]] x;)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["ATTR_OPEN", "[[", 1, 5, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('ATTR_OPEN', '[[') at line 1, column 5.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* IDENTIFIER\n\t* INT\n\t* LONG\n\t* LPAREN\n\t* REGISTER\n\t* RESTRICT\n\t* SEMICOLON\n\t* SHORT\n\t* SIGNED\n\t* STAR\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int * [[c6502::zeropage]] p;)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["STAR", "*", 1, 5, 1], ["ATTR_OPEN", "[[", 1, 7, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('ATTR_OPEN', '[[') at line 1, column 7.\nExpected one of: \n\t* CONST\n\t* IDENTIFIER\n\t* LPAREN\n\t* RESTRICT\n\t* STAR\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void)
{



    return 1 ! = 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["RETURN", "return", 6, 5, 2], ["INTEGER_CONSTANT", "1", 6, 12, 2], ["BANG", "!", 6, 14, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('BANG', '!') at line 6, column 14.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(int main(void) {
    int a = 3;
    return a'1';
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 2, 5, 2], ["IDENTIFIER", "a", 2, 9, 2], ["ASSIGN", "=", 2, 11, 2], ["INTEGER_CONSTANT", "3", 2, 13, 2], ["SEMICOLON", ";", 2, 14, 2], ["RETURN", "return", 3, 5, 2], ["IDENTIFIER", "a", 3, 12, 2], ["CHAR_CONSTANT", "'1'", 3, 13, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('CHAR_CONSTANT', \"'1'\") at line 3, column 13.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(


int main(void) {
    int a = 3;
    return a'1';
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 4, 1, 1], ["IDENTIFIER", "main", 4, 5, 1], ["LPAREN", "(", 4, 9, 1], ["VOID", "void", 4, 10, 1], ["RPAREN", ")", 4, 14, 1], ["LBRACE", "{", 4, 16, 2], ["INT", "int", 5, 5, 2], ["IDENTIFIER", "a", 5, 9, 2], ["ASSIGN", "=", 5, 11, 2], ["INTEGER_CONSTANT", "3", 5, 13, 2], ["SEMICOLON", ";", 5, 14, 2], ["RETURN", "return", 6, 5, 2], ["IDENTIFIER", "a", 6, 12, 2], ["CHAR_CONSTANT", "'1'", 6, 13, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('CHAR_CONSTANT', \"'1'\") at line 6, column 13.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(int main(void) {
    goto 'x';
    'x';
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["GOTO", "goto", 2, 5, 2], ["CHAR_CONSTANT", "'x'", 2, 10, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('CHAR_CONSTANT', \"'x'\") at line 2, column 10.\nExpected one of: \n\t* IDENTIFIER\n"]])~~")},
    {std::string(R"~~(
int main(void) {
    goto 'x';
    'x';
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 2, 1, 1], ["IDENTIFIER", "main", 2, 5, 1], ["LPAREN", "(", 2, 9, 1], ["VOID", "void", 2, 10, 1], ["RPAREN", ")", 2, 14, 1], ["LBRACE", "{", 2, 16, 2], ["GOTO", "goto", 3, 5, 2], ["CHAR_CONSTANT", "'x'", 3, 10, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('CHAR_CONSTANT', \"'x'\") at line 3, column 10.\nExpected one of: \n\t* IDENTIFIER\n"]])~~")},
    {std::string(R"~~(int main(void) {
    "foo": return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["STRING_LITERAL", "\"foo\"", 2, 5, 2], ["COLON", ":", 2, 10, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('COLON', ':') at line 2, column 10.\nExpected one of: \n\t* COMMA\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return: return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["COLON", ":", 2, 11, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('COLON', ':') at line 2, column 11.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SEMICOLON\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void) {
    extern a:
    return 1;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["EXTERN", "extern", 2, 5, 2], ["IDENTIFIER", "a", 2, 12, 2], ["COLON", ":", 2, 13, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('COLON', ':') at line 2, column 13.\nExpected one of: \n\t* ASSIGN\n\t* ATTR_OPEN\n\t* COMMA\n\t* LBRACE\n\t* LBRACKET\n\t* LPAREN\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(int main(void) {
    1 && label: 2;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INTEGER_CONSTANT", "1", 2, 5, 2], ["ANDAND", "&&", 2, 7, 2], ["IDENTIFIER", "label", 2, 10, 2], ["COLON", ":", 2, 15, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('COLON', ':') at line 2, column 15.\nExpected one of: \n\t* COMMA\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return 1 ? 2 : 3 : 4;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["INTEGER_CONSTANT", "1", 2, 12, 2], ["QUESTION", "?", 2, 14, 2], ["INTEGER_CONSTANT", "2", 2, 16, 2], ["COLON", ":", 2, 18, 2], ["INTEGER_CONSTANT", "3", 2, 20, 2], ["COLON", ":", 2, 22, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('COLON', ':') at line 2, column 22.\nExpected one of: \n\t* COMMA\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(int main(void) {
    for (int i = 0; label: i < 10; i = i + 1) {
        ;
    }
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["FOR", "for", 2, 5, 3], ["LPAREN", "(", 2, 9, 3], ["INT", "int", 2, 10, 3], ["IDENTIFIER", "i", 2, 14, 3], ["ASSIGN", "=", 2, 16, 3], ["INTEGER_CONSTANT", "0", 2, 18, 3], ["SEMICOLON", ";", 2, 19, 3], ["IDENTIFIER", "label", 2, 21, 3], ["COLON", ":", 2, 26, 3]], ["syntax", "UnexpectedToken", "Unexpected token Token('COLON', ':') at line 2, column 26.\nExpected one of: \n\t* COMMA\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(int main(void) {
    'x': return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["CHAR_CONSTANT", "'x'", 2, 5, 2], ["COLON", ":", 2, 8, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('COLON', ':') at line 2, column 8.\nExpected one of: \n\t* COMMA\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(
int main(void) {
    "foo": return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 2, 1, 1], ["IDENTIFIER", "main", 2, 5, 1], ["LPAREN", "(", 2, 9, 1], ["VOID", "void", 2, 10, 1], ["RPAREN", ")", 2, 14, 1], ["LBRACE", "{", 2, 16, 2], ["STRING_LITERAL", "\"foo\"", 3, 5, 2], ["COLON", ":", 3, 10, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('COLON', ':') at line 3, column 10.\nExpected one of: \n\t* COMMA\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(int main(void) {
    switch(0) {
        case: return 0;
    }
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["SWITCH", "switch", 2, 5, 2], ["LPAREN", "(", 2, 11, 2], ["INTEGER_CONSTANT", "0", 2, 12, 2], ["RPAREN", ")", 2, 13, 2], ["LBRACE", "{", 2, 15, 3], ["CASE", "case", 3, 9, 3], ["COLON", ":", 3, 13, 3]], ["syntax", "UnexpectedToken", "Unexpected token Token('COLON', ':') at line 3, column 13.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(
int main(void) {
    'x': return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 2, 1, 1], ["IDENTIFIER", "main", 2, 5, 1], ["LPAREN", "(", 2, 9, 1], ["VOID", "void", 2, 10, 1], ["RPAREN", ")", 2, 14, 1], ["LBRACE", "{", 2, 16, 2], ["CHAR_CONSTANT", "'x'", 3, 5, 2], ["COLON", ":", 3, 8, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('COLON', ':') at line 3, column 8.\nExpected one of: \n\t* COMMA\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(

int main(void) {
    extern a:
    return 1;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 3, 1, 1], ["IDENTIFIER", "main", 3, 5, 1], ["LPAREN", "(", 3, 9, 1], ["VOID", "void", 3, 10, 1], ["RPAREN", ")", 3, 14, 1], ["LBRACE", "{", 3, 16, 2], ["EXTERN", "extern", 4, 5, 2], ["IDENTIFIER", "a", 4, 12, 2], ["COLON", ":", 4, 13, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('COLON', ':') at line 4, column 13.\nExpected one of: \n\t* ASSIGN\n\t* ATTR_OPEN\n\t* COMMA\n\t* LBRACE\n\t* LBRACKET\n\t* LPAREN\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(int x = {,};)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "x", 1, 5, 1], ["ASSIGN", "=", 1, 7, 1], ["LBRACE", "{", 1, 9, 2], ["COMMA", ",", 1, 10, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('COMMA', ',') at line 1, column 10.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LBRACKET\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* RBRACE\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int f(void) { return (int){,}; })~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "f", 1, 5, 1], ["LPAREN", "(", 1, 6, 1], ["VOID", "void", 1, 7, 1], ["RPAREN", ")", 1, 11, 1], ["LBRACE", "{", 1, 13, 2], ["RETURN", "return", 1, 15, 2], ["LPAREN", "(", 1, 22, 2], ["INT", "int", 1, 23, 2], ["RPAREN", ")", 1, 26, 2], ["LBRACE", "{", 1, 27, 3], ["COMMA", ",", 1, 28, 3]], ["syntax", "UnexpectedToken", "Unexpected token Token('COMMA', ',') at line 1, column 28.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LBRACKET\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* RBRACE\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void) { int a[3] = {,}; return 0; })~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 1, 18, 2], ["IDENTIFIER", "a", 1, 22, 2], ["LBRACKET", "[", 1, 23, 2], ["INTEGER_CONSTANT", "3", 1, 24, 2], ["RBRACKET", "]", 1, 25, 2], ["ASSIGN", "=", 1, 27, 2], ["LBRACE", "{", 1, 29, 3], ["COMMA", ",", 1, 30, 3]], ["syntax", "UnexpectedToken", "Unexpected token Token('COMMA', ',') at line 1, column 30.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LBRACKET\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* RBRACE\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(struct s {
    int default;
};

int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["INT", "int", 2, 5, 1], ["DEFAULT", "default", 2, 9, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('DEFAULT', 'default') at line 2, column 9.\nExpected one of: \n\t* ALT_BOOL\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* DOUBLE\n\t* ENUM\n\t* FLOAT\n\t* IDENTIFIER\n\t* INT\n\t* LONG\n\t* LPAREN\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STAR\n\t* STRUCT\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(
struct s {
    int default;
};

int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 2, 1, 1], ["IDENTIFIER", "s", 2, 8, 1], ["LBRACE", "{", 2, 10, 1], ["INT", "int", 3, 5, 1], ["DEFAULT", "default", 3, 9, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('DEFAULT', 'default') at line 3, column 9.\nExpected one of: \n\t* ALT_BOOL\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* DOUBLE\n\t* ENUM\n\t* FLOAT\n\t* IDENTIFIER\n\t* INT\n\t* LONG\n\t* LPAREN\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STAR\n\t* STRUCT\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(struct pair {
    int a;
    int b;
};

int main(void) {



    struct pair x.a = 10;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "pair", 1, 8, 1], ["LBRACE", "{", 1, 13, 1], ["INT", "int", 2, 5, 1], ["IDENTIFIER", "a", 2, 9, 1], ["SEMICOLON", ";", 2, 10, 1], ["INT", "int", 3, 5, 1], ["IDENTIFIER", "b", 3, 9, 1], ["SEMICOLON", ";", 3, 10, 1], ["RBRACE", "}", 4, 1, 1], ["SEMICOLON", ";", 4, 2, 1], ["INT", "int", 6, 1, 1], ["IDENTIFIER", "main", 6, 5, 1], ["LPAREN", "(", 6, 9, 1], ["VOID", "void", 6, 10, 1], ["RPAREN", ")", 6, 14, 1], ["LBRACE", "{", 6, 16, 2], ["STRUCT", "struct", 10, 5, 2], ["IDENTIFIER", "pair", 10, 12, 2], ["IDENTIFIER", "x", 10, 17, 2], ["DOT", ".", 10, 18, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('DOT', '.') at line 10, column 18.\nExpected one of: \n\t* ASSIGN\n\t* ATTR_OPEN\n\t* COMMA\n\t* LBRACE\n\t* LBRACKET\n\t* LPAREN\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return .a;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["DOT", ".", 2, 12, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('DOT', '.') at line 2, column 12.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SEMICOLON\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void)
{





    double d = 1.0e10.0;
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["DOUBLE", "double", 8, 5, 2], ["IDENTIFIER", "d", 8, 12, 2], ["ASSIGN", "=", 8, 14, 2], ["DOUBLE_CONSTANT", "1.0e10", 8, 16, 2], ["DOUBLE_CONSTANT", ".0", 8, 22, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('DOUBLE_CONSTANT', '.0') at line 8, column 22.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(int main(void) {
    if (0) else return 0;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["IF", "if", 2, 5, 2], ["LPAREN", "(", 2, 8, 2], ["INTEGER_CONSTANT", "0", 2, 9, 2], ["RPAREN", ")", 2, 10, 2], ["ELSE", "else", 2, 12, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('ELSE', 'else') at line 2, column 12.\nExpected one of: \n\t* AMP\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* BREAK\n\t* CASE\n\t* CHAR_CONSTANT\n\t* CONTINUE\n\t* DEFAULT\n\t* DO\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* FOR\n\t* GOTO\n\t* IDENTIFIER\n\t* IF\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* RETURN\n\t* SEMICOLON\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* SWITCH\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\t* WHILE\n"]])~~")},
    {std::string(R"~~(int main(void) {
    int a = 0;
    if (1)
        return 1;
    else
        return 2;
    else
        return 3;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 2, 5, 2], ["IDENTIFIER", "a", 2, 9, 2], ["ASSIGN", "=", 2, 11, 2], ["INTEGER_CONSTANT", "0", 2, 13, 2], ["SEMICOLON", ";", 2, 14, 2], ["IF", "if", 3, 5, 2], ["LPAREN", "(", 3, 8, 2], ["INTEGER_CONSTANT", "1", 3, 9, 2], ["RPAREN", ")", 3, 10, 2], ["RETURN", "return", 4, 9, 2], ["INTEGER_CONSTANT", "1", 4, 16, 2], ["SEMICOLON", ";", 4, 17, 2], ["ELSE", "else", 5, 5, 2], ["RETURN", "return", 6, 9, 2], ["INTEGER_CONSTANT", "2", 6, 16, 2], ["SEMICOLON", ";", 6, 17, 2], ["ELSE", "else", 7, 5, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('ELSE', 'else') at line 7, column 5.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* AMP\n\t* ATTR_OPEN\n\t* AUTO\n\t* BANG\n\t* BITINT\n\t* BITINT_INTEGER\n\t* BOOL\n\t* BREAK\n\t* CASE\n\t* CHAR\n\t* CHAR_CONSTANT\n\t* CONST\n\t* CONSTEXPR\n\t* CONTINUE\n\t* DEFAULT\n\t* DO\n\t* DOUBLE\n\t* DOUBLE_CONSTANT\n\t* ENUM\n\t* EXTERN\n\t* FALSE\n\t* FLOAT\n\t* FLOAT_CONSTANT\n\t* FOR\n\t* GOTO\n\t* IDENTIFIER\n\t* IF\n\t* INT\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LONG\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* RBRACE\n\t* REGISTER\n\t* RESTRICT\n\t* RETURN\n\t* SEMICOLON\n\t* SHORT\n\t* SIGNED\n\t* SIZEOF\n\t* STAR\n\t* STATIC\n\t* STRING_LITERAL\n\t* STRUCT\n\t* SWITCH\n\t* THREAD_LOCAL\n\t* TILDE\n\t* TRUE\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n\t* WHILE\n"]])~~")},
    {std::string(R"~~(struct s {
    int a;
};

int main(void) {
    struct s x;

    return x.0foo;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["INT", "int", 2, 5, 1], ["IDENTIFIER", "a", 2, 9, 1], ["SEMICOLON", ";", 2, 10, 1], ["RBRACE", "}", 3, 1, 1], ["SEMICOLON", ";", 3, 2, 1], ["INT", "int", 5, 1, 1], ["IDENTIFIER", "main", 5, 5, 1], ["LPAREN", "(", 5, 9, 1], ["VOID", "void", 5, 10, 1], ["RPAREN", ")", 5, 14, 1], ["LBRACE", "{", 5, 16, 2], ["STRUCT", "struct", 6, 5, 2], ["IDENTIFIER", "s", 6, 12, 2], ["IDENTIFIER", "x", 6, 14, 2], ["SEMICOLON", ";", 6, 15, 2], ["RETURN", "return", 8, 5, 2], ["IDENTIFIER", "x", 8, 12, 2], ["FLOAT_CONSTANT", ".0f", 8, 13, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('FLOAT_CONSTANT', '.0f') at line 8, column 13.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(struct for {
    int a;
};
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["FOR", "for", 1, 8, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('FOR', 'for') at line 1, column 8.\nExpected one of: \n\t* ATTR_OPEN\n\t* IDENTIFIER\n\t* LBRACE\n"]])~~")},
    {std::string(R"~~(
struct for {
    int a;
};
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 2, 1, 1], ["FOR", "for", 2, 8, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('FOR', 'for') at line 2, column 8.\nExpected one of: \n\t* ATTR_OPEN\n\t* IDENTIFIER\n\t* LBRACE\n"]])~~")},
    {std::string(R"~~(struct goto { int a; };
int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["GOTO", "goto", 1, 8, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('GOTO', 'goto') at line 1, column 8.\nExpected one of: \n\t* ATTR_OPEN\n\t* IDENTIFIER\n\t* LBRACE\n"]])~~")},
    {std::string(R"~~(
struct goto { int a; };
int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 2, 1, 1], ["GOTO", "goto", 2, 8, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('GOTO', 'goto') at line 2, column 8.\nExpected one of: \n\t* ATTR_OPEN\n\t* IDENTIFIER\n\t* LBRACE\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return 1 < > 3;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["INTEGER_CONSTANT", "1", 2, 12, 2], ["LT", "<", 2, 14, 2], ["GT", ">", 2, 16, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('GT', '>') at line 2, column 16.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(#ifdef SUPPRESS_WARNINGS
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif
int main(void) {
    int outer = 1;
    int foo = 0;
    if (outer) {
        /* You can declare a variable with linkage
         * multiple times in the same block;
         * these both refer to the 'foo' variable defined below
         */
        extern int foo;
        extern int foo;
        return foo;
    }
    return 0;
}

int foo = 3;)~~"),
     {},
     {},
     std::string(R"~~([[["HASH", "#", 1, 1, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('HASH', '#') at line 1, column 1.\nExpected one of: \n\t* $END\n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void) {





    return 0lL;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 7, 5, 2], ["LONG_INTEGER", "0l", 7, 12, 2], ["IDENTIFIER", "L", 7, 14, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'L') at line 7, column 14.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(int main(void) {





    return 0LLL;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 7, 5, 2], ["LONG_INTEGER", "0LL", 7, 12, 2], ["IDENTIFIER", "L", 7, 15, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'L') at line 7, column 15.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(int x = 5Wb;)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "x", 1, 5, 1], ["ASSIGN", "=", 1, 7, 1], ["INTEGER_CONSTANT", "5", 1, 9, 1], ["IDENTIFIER", "Wb", 1, 10, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'Wb') at line 1, column 10.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(int main(void)
{





    return 2._;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["RETURN", "return", 8, 5, 2], ["DOUBLE_CONSTANT", "2.", 8, 12, 2], ["IDENTIFIER", "_", 8, 14, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', '_') at line 8, column 14.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(__attribute__((zp_abi)) void f(void);)~~"),
     {},
     {},
     std::string(R"~~([[["IDENTIFIER", "__attribute__", 1, 1, 1]], ["syntax", "ParserError", "Unexpected token Token('IDENTIFIER', '__attribute__') at line 1, column 1.\nExpected one of: \n\t* $END\n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n\nc6502 doesn't take GCC's `__attribute__((...))`; it takes C23 attributes, such as `[[c6502::zp_abi]]` before a declaration or `uint8_t n [[c6502::in_x]]` after a name"]])~~")},
    {std::string(R"~~(unsigned char n __attribute__((zeropage));)~~"),
     {},
     {},
     std::string(R"~~([[["UNSIGNED", "unsigned", 1, 1, 1], ["CHAR", "char", 1, 10, 1], ["IDENTIFIER", "n", 1, 15, 1], ["IDENTIFIER", "__attribute__", 1, 17, 1]], ["syntax", "ParserError", "Unexpected token Token('IDENTIFIER', '__attribute__') at line 1, column 17.\nExpected one of: \n\t* ASSIGN\n\t* ATTR_OPEN\n\t* COMMA\n\t* LBRACE\n\t* LBRACKET\n\t* LPAREN\n\t* SEMICOLON\n\nc6502 doesn't take GCC's `__attribute__((...))`; it takes C23 attributes, such as `[[c6502::zp_abi]]` before a declaration or `uint8_t n [[c6502::in_x]]` after a name"]])~~")},
    {std::string(R"~~(int main(void) {
    ints a = 1;
    return a;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["IDENTIFIER", "ints", 2, 5, 2], ["IDENTIFIER", "a", 2, 10, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'a') at line 2, column 10.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COLON\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(union u {
    a;
};
)~~"),
     {},
     {},
     std::string(R"~~([[["UNION", "union", 1, 1, 1], ["IDENTIFIER", "u", 1, 7, 1], ["LBRACE", "{", 1, 9, 1], ["IDENTIFIER", "a", 2, 5, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'a') at line 2, column 5.\nExpected one of: \n\t* ALT_BOOL\n\t* ATTR_OPEN\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* DOUBLE\n\t* ENUM\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STRUCT\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void) {
    int a = 2
    a = a + 4;
    return a;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 2, 5, 2], ["IDENTIFIER", "a", 2, 9, 2], ["ASSIGN", "=", 2, 11, 2], ["INTEGER_CONSTANT", "2", 2, 13, 2], ["IDENTIFIER", "a", 3, 5, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'a') at line 3, column 5.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(int main(void)
{

    return (int **a)(10);
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["RETURN", "return", 4, 5, 2], ["LPAREN", "(", 4, 12, 2], ["INT", "int", 4, 13, 2], ["STAR", "*", 4, 17, 2], ["STAR", "*", 4, 18, 2], ["IDENTIFIER", "a", 4, 19, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'a') at line 4, column 19.\nExpected one of: \n\t* LBRACKET\n\t* LPAREN\n\t* RPAREN\n"]])~~")},
    {std::string(R"~~(int main(void) {
    int a = 0;
    int b = 0;


    do
    do_body:
        a = a + 1;
        b = b - 1;
    while (a < 10)
        ;
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 2, 5, 2], ["IDENTIFIER", "a", 2, 9, 2], ["ASSIGN", "=", 2, 11, 2], ["INTEGER_CONSTANT", "0", 2, 13, 2], ["SEMICOLON", ";", 2, 14, 2], ["INT", "int", 3, 5, 2], ["IDENTIFIER", "b", 3, 9, 2], ["ASSIGN", "=", 3, 11, 2], ["INTEGER_CONSTANT", "0", 3, 13, 2], ["SEMICOLON", ";", 3, 14, 2], ["DO", "do", 6, 5, 2], ["IDENTIFIER", "do_body", 7, 5, 2], ["COLON", ":", 7, 12, 2], ["IDENTIFIER", "a", 8, 9, 2], ["ASSIGN", "=", 8, 11, 2], ["IDENTIFIER", "a", 8, 13, 2], ["PLUS", "+", 8, 15, 2], ["INTEGER_CONSTANT", "1", 8, 17, 2], ["SEMICOLON", ";", 8, 18, 2], ["IDENTIFIER", "b", 9, 9, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'b') at line 9, column 9.\nExpected one of: \n\t* WHILE\n"]])~~")},
    {std::string(R"~~(int main(void) {
    int foo bar = 3;
    return bar;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 2, 5, 2], ["IDENTIFIER", "foo", 2, 9, 2], ["IDENTIFIER", "bar", 2, 13, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'bar') at line 2, column 13.\nExpected one of: \n\t* ASSIGN\n\t* ATTR_OPEN\n\t* COMMA\n\t* LBRACE\n\t* LBRACKET\n\t* LPAREN\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(int main(void)
{
    char *ptr = "foo"bar";
        return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["CHAR", "char", 3, 5, 2], ["STAR", "*", 3, 10, 2], ["IDENTIFIER", "ptr", 3, 11, 2], ["ASSIGN", "=", 3, 15, 2], ["STRING_LITERAL", "\"foo\"", 3, 17, 2], ["IDENTIFIER", "bar", 3, 22, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'bar') at line 3, column 22.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n\t* STRING_LITERAL\n"]])~~")},
    {std::string(R"~~(int main(void)
{



    double foo = 30.e;
    return 4;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["DOUBLE", "double", 6, 5, 2], ["IDENTIFIER", "foo", 6, 12, 2], ["ASSIGN", "=", 6, 16, 2], ["DOUBLE_CONSTANT", "30.", 6, 18, 2], ["IDENTIFIER", "e", 6, 21, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'e') at line 6, column 21.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(int main(void)
{





    return 1.ex;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["RETURN", "return", 8, 5, 2], ["DOUBLE_CONSTANT", "1.", 8, 12, 2], ["IDENTIFIER", "ex", 8, 14, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'ex') at line 8, column 14.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return 1foo;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["INTEGER_CONSTANT", "1", 2, 12, 2], ["IDENTIFIER", "foo", 2, 13, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'foo') at line 2, column 13.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(
int main(void) {
    return 1foo;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 2, 1, 1], ["IDENTIFIER", "main", 2, 5, 1], ["LPAREN", "(", 2, 9, 1], ["VOID", "void", 2, 10, 1], ["RPAREN", ")", 2, 14, 1], ["LBRACE", "{", 2, 16, 2], ["RETURN", "return", 3, 5, 2], ["INTEGER_CONSTANT", "1", 3, 12, 2], ["IDENTIFIER", "foo", 3, 13, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'foo') at line 3, column 13.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(struct s {
    int i;


    foo : int j;
};

int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["INT", "int", 2, 5, 1], ["IDENTIFIER", "i", 2, 9, 1], ["SEMICOLON", ";", 2, 10, 1], ["IDENTIFIER", "foo", 5, 5, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'foo') at line 5, column 5.\nExpected one of: \n\t* ALT_BOOL\n\t* ATTR_OPEN\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* DOUBLE\n\t* ENUM\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* RBRACE\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STRUCT\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void)
{
    return 2;
}

foo
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["RETURN", "return", 3, 5, 2], ["INTEGER_CONSTANT", "2", 3, 12, 2], ["SEMICOLON", ";", 3, 13, 2], ["RBRACE", "}", 4, 1, 1], ["IDENTIFIER", "foo", 6, 1, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'foo') at line 6, column 1.\nExpected one of: \n\t* $END\n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(

struct s {
    int i;


    foo : int j;
};

int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 3, 1, 1], ["IDENTIFIER", "s", 3, 8, 1], ["LBRACE", "{", 3, 10, 1], ["INT", "int", 4, 5, 1], ["IDENTIFIER", "i", 4, 9, 1], ["SEMICOLON", ";", 4, 10, 1], ["IDENTIFIER", "foo", 7, 5, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'foo') at line 7, column 5.\nExpected one of: \n\t* ALT_BOOL\n\t* ATTR_OPEN\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* DOUBLE\n\t* ENUM\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* RBRACE\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STRUCT\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(typedef unsigned int size_t;

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef int int16_t;
typedef unsigned int uint16_t;
typedef long int32_t;
typedef unsigned long uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;

typedef int8_t int_least8_t;
typedef uint8_t uint_least8_t;
typedef int16_t int_least16_t;
typedef uint16_t uint_least16_t;
typedef int32_t int_least32_t;
typedef uint32_t uint_least32_t;
typedef int64_t int_least64_t;
typedef uint64_t uint_least64_t;



unsigned int stdc_leading_zeros_uc(unsigned char __value);
unsigned int stdc_leading_zeros_us(unsigned short __value);
unsigned int stdc_leading_zeros_ui(unsigned int __value);
unsigned int stdc_leading_zeros_ul(unsigned long __value);
unsigned int stdc_leading_zeros_ull(unsigned long long __value);








unsigned int stdc_leading_ones_uc(unsigned char __value);
unsigned int stdc_leading_ones_us(unsigned short __value);
unsigned int stdc_leading_ones_ui(unsigned int __value);
unsigned int stdc_leading_ones_ul(unsigned long __value);
unsigned int stdc_leading_ones_ull(unsigned long long __value);








unsigned int stdc_trailing_zeros_uc(unsigned char __value);
unsigned int stdc_trailing_zeros_us(unsigned short __value);
unsigned int stdc_trailing_zeros_ui(unsigned int __value);
unsigned int stdc_trailing_zeros_ul(unsigned long __value);
unsigned int stdc_trailing_zeros_ull(unsigned long long __value);








unsigned int stdc_trailing_ones_uc(unsigned char __value);
unsigned int stdc_trailing_ones_us(unsigned short __value);
unsigned int stdc_trailing_ones_ui(unsigned int __value);
unsigned int stdc_trailing_ones_ul(unsigned long __value);
unsigned int stdc_trailing_ones_ull(unsigned long long __value);








unsigned int stdc_first_leading_zero_uc(unsigned char __value);
unsigned int stdc_first_leading_zero_us(unsigned short __value);
unsigned int stdc_first_leading_zero_ui(unsigned int __value);
unsigned int stdc_first_leading_zero_ul(unsigned long __value);
unsigned int stdc_first_leading_zero_ull(unsigned long long __value);








unsigned int stdc_first_leading_one_uc(unsigned char __value);
unsigned int stdc_first_leading_one_us(unsigned short __value);
unsigned int stdc_first_leading_one_ui(unsigned int __value);
unsigned int stdc_first_leading_one_ul(unsigned long __value);
unsigned int stdc_first_leading_one_ull(unsigned long long __value);








unsigned int stdc_first_trailing_zero_uc(unsigned char __value);
unsigned int stdc_first_trailing_zero_us(unsigned short __value);
unsigned int stdc_first_trailing_zero_ui(unsigned int __value);
unsigned int stdc_first_trailing_zero_ul(unsigned long __value);
unsigned int stdc_first_trailing_zero_ull(unsigned long long __value);








unsigned int stdc_first_trailing_one_uc(unsigned char __value);
unsigned int stdc_first_trailing_one_us(unsigned short __value);
unsigned int stdc_first_trailing_one_ui(unsigned int __value);
unsigned int stdc_first_trailing_one_ul(unsigned long __value);
unsigned int stdc_first_trailing_one_ull(unsigned long long __value);








unsigned int stdc_count_zeros_uc(unsigned char __value);
unsigned int stdc_count_zeros_us(unsigned short __value);
unsigned int stdc_count_zeros_ui(unsigned int __value);
unsigned int stdc_count_zeros_ul(unsigned long __value);
unsigned int stdc_count_zeros_ull(unsigned long long __value);








unsigned int stdc_count_ones_uc(unsigned char __value);
unsigned int stdc_count_ones_us(unsigned short __value);
unsigned int stdc_count_ones_ui(unsigned int __value);
unsigned int stdc_count_ones_ul(unsigned long __value);
unsigned int stdc_count_ones_ull(unsigned long long __value);








bool stdc_has_single_bit_uc(unsigned char __value);
bool stdc_has_single_bit_us(unsigned short __value);
bool stdc_has_single_bit_ui(unsigned int __value);
bool stdc_has_single_bit_ul(unsigned long __value);
bool stdc_has_single_bit_ull(unsigned long long __value);








unsigned int stdc_bit_width_uc(unsigned char _)~~") + R"~~(_value);
unsigned int stdc_bit_width_us(unsigned short __value);
unsigned int stdc_bit_width_ui(unsigned int __value);
unsigned int stdc_bit_width_ul(unsigned long __value);
unsigned int stdc_bit_width_ull(unsigned long long __value);








unsigned char stdc_bit_floor_uc(unsigned char __value);
unsigned short stdc_bit_floor_us(unsigned short __value);
unsigned int stdc_bit_floor_ui(unsigned int __value);
unsigned long stdc_bit_floor_ul(unsigned long __value);
unsigned long long stdc_bit_floor_ull(unsigned long long __value);








unsigned char stdc_bit_ceil_uc(unsigned char __value);
unsigned short stdc_bit_ceil_us(unsigned short __value);
unsigned int stdc_bit_ceil_ui(unsigned int __value);
unsigned long stdc_bit_ceil_ul(unsigned long __value);
unsigned long long stdc_bit_ceil_ull(unsigned long long __value);
intptr_t p;
)~~",
     {45, 49, 53, 57, 61, 65, 69, 73},
     {},
     std::string(R"~~([[["TYPEDEF", "typedef", 1, 1, 1], ["UNSIGNED", "unsigned", 1, 9, 1], ["INT", "int", 1, 18, 1], ["IDENTIFIER", "size_t", 1, 22, 1], ["SEMICOLON", ";", 1, 28, 1], ["TYPEDEF", "typedef", 3, 1, 1], ["SIGNED", "signed", 3, 9, 1], ["CHAR", "char", 3, 16, 1], ["IDENTIFIER", "int8_t", 3, 21, 1], ["SEMICOLON", ";", 3, 27, 1], ["TYPEDEF", "typedef", 4, 1, 1], ["UNSIGNED", "unsigned", 4, 9, 1], ["CHAR", "char", 4, 18, 1], ["IDENTIFIER", "uint8_t", 4, 23, 1], ["SEMICOLON", ";", 4, 30, 1], ["TYPEDEF", "typedef", 5, 1, 1], ["INT", "int", 5, 9, 1], ["IDENTIFIER", "int16_t", 5, 13, 1], ["SEMICOLON", ";", 5, 20, 1], ["TYPEDEF", "typedef", 6, 1, 1], ["UNSIGNED", "unsigned", 6, 9, 1], ["INT", "int", 6, 18, 1], ["IDENTIFIER", "uint16_t", 6, 22, 1], ["SEMICOLON", ";", 6, 30, 1], ["TYPEDEF", "typedef", 7, 1, 1], ["LONG", "long", 7, 9, 1], ["IDENTIFIER", "int32_t", 7, 14, 1], ["SEMICOLON", ";", 7, 21, 1], ["TYPEDEF", "typedef", 8, 1, 1], ["UNSIGNED", "unsigned", 8, 9, 1], ["LONG", "long", 8, 18, 1], ["IDENTIFIER", "uint32_t", 8, 23, 1], ["SEMICOLON", ";", 8, 31, 1], ["TYPEDEF", "typedef", 9, 1, 1], ["LONG", "long", 9, 9, 1], ["LONG", "long", 9, 14, 1], ["IDENTIFIER", "int64_t", 9, 19, 1], ["SEMICOLON", ";", 9, 26, 1], ["TYPEDEF", "typedef", 10, 1, 1], ["UNSIGNED", "unsigned", 10, 9, 1], ["LONG", "long", 10, 18, 1], ["LONG", "long", 10, 23, 1], ["IDENTIFIER", "uint64_t", 10, 28, 1], ["SEMICOLON", ";", 10, 36, 1], ["TYPEDEF", "typedef", 12, 1, 1], ["TYPEDEF_NAME", "int8_t", 12, 9, 1], ["IDENTIFIER", "int_least8_t", 12, 16, 1], ["SEMICOLON", ";", 12, 28, 1], ["TYPEDEF", "typedef", 13, 1, 1], ["TYPEDEF_NAME", "uint8_t", 13, 9, 1], ["IDENTIFIER", "uint_least8_t", 13, 17, 1], ["SEMICOLON", ";", 13, 30, 1], ["TYPEDEF", "typedef", 14, 1, 1], ["TYPEDEF_NAME", "int16_t", 14, 9, 1], ["IDENTIFIER", "int_least16_t", 14, 17, 1], ["SEMICOLON", ";", 14, 30, 1], ["TYPEDEF", "typedef", 15, 1, 1], ["TYPEDEF_NAME", "uint16_t", 15, 9, 1], ["IDENTIFIER", "uint_least16_t", 15, 18, 1], ["SEMICOLON", ";", 15, 32, 1], ["TYPEDEF", "typedef", 16, 1, 1], ["TYPEDEF_NAME", "int32_t", 16, 9, 1], ["IDENTIFIER", "int_least32_t", 16, 17, 1], ["SEMICOLON", ";", 16, 30, 1], ["TYPEDEF", "typedef", 17, 1, 1], ["TYPEDEF_NAME", "uint32_t", 17, 9, 1], ["IDENTIFIER", "uint_least32_t", 17, 18, 1], ["SEMICOLON", ";", 17, 32, 1], ["TYPEDEF", "typedef", 18, 1, 1], ["TYPEDEF_NAME", "int64_t", 18, 9, 1], ["IDENTIFIER", "int_least64_t", 18, 17, 1], ["SEMICOLON", ";", 18, 30, 1], ["TYPEDEF", "typedef", 19, 1, 1], ["TYPEDEF_NAME", "uint64_t", 19, 9, 1], ["IDENTIFIER", "uint_least64_t", 19, 18, 1], ["SEMICOLON", ";", 19, 32, 1], ["UNSIGNED", "unsigned", 23, 1, 1], ["INT", "int", 23, 10, 1], ["IDENTIFIER", "stdc_leading_zeros_uc", 23, 14, 1], ["LPAREN", "(", 23, 35, 1], ["UNSIGNED", "unsigned", 23, 36, 1], ["CHAR", "char", 23, 45, 1], ["IDENTIFIER", "__value", 23, 50, 1], ["RPAREN", ")", 23, 57, 1], ["SEMICOLON", ";", 23, 58, 1], ["UNSIGNED", "unsigned", 24, 1, 1], ["INT", "int", 24, 10, 1], ["IDENTIFIER", "stdc_leading_zeros_us", 24, 14, 1], ["LPAREN", "(", 24, 35, 1], ["UNSIGNED", "unsigned", 24, 36, 1], ["SHORT", "short", 24, 45, 1], ["IDENTIFIER", "__value", 24, 51, 1], ["RPAREN", ")", 24, 58, 1], ["SEMICOLON", ";", 24, 59, 1], ["UNSIGNED", "unsigned", 25, 1, 1], ["INT", "int", 25, 10, 1], ["IDENTIFIER", "stdc_leading_zeros_ui", 25, 14, 1], ["LPAREN", "(", 25, 35, 1], ["UNSIGNED", "unsigned", 25, 36, 1], ["INT", "int", 25, 45, 1], ["IDENTIFIER", "__value", 25, 49, 1], ["RPAREN", ")", 25, 56, 1], ["SEMICOLON", ";", 25, 57, 1], ["UNSIGNED", "unsigned", 26, 1, 1], ["INT", "int", 26, 10, 1], ["IDENTIFIER", "stdc_leading_zeros_ul", 26, 14, 1], ["LPAREN", "(", 26, 35, 1], ["UNSIGNED", "unsigned", 26, 36, 1], ["LONG", "long", 26, 45, 1], ["IDENTIFIER", "__value", 26, 50, 1], ["RPAREN", ")", 26, 57, 1], ["SEMICOLON", ";", 26, 58, 1], ["UNSIGNED", "unsigned", 27, 1, 1], ["INT", "int", 27, 10, 1], ["IDENTIFIER", "stdc_leading_zeros_ull", 27, 14, 1], ["LPAREN", "(", 27, 36, 1], ["UNSIGNED", "unsigned", 27, 3)~~") + R"~~(7, 1], ["LONG", "long", 27, 46, 1], ["LONG", "long", 27, 51, 1], ["IDENTIFIER", "__value", 27, 56, 1], ["RPAREN", ")", 27, 63, 1], ["SEMICOLON", ";", 27, 64, 1], ["UNSIGNED", "unsigned", 36, 1, 1], ["INT", "int", 36, 10, 1], ["IDENTIFIER", "stdc_leading_ones_uc", 36, 14, 1], ["LPAREN", "(", 36, 34, 1], ["UNSIGNED", "unsigned", 36, 35, 1], ["CHAR", "char", 36, 44, 1], ["IDENTIFIER", "__value", 36, 49, 1], ["RPAREN", ")", 36, 56, 1], ["SEMICOLON", ";", 36, 57, 1], ["UNSIGNED", "unsigned", 37, 1, 1], ["INT", "int", 37, 10, 1], ["IDENTIFIER", "stdc_leading_ones_us", 37, 14, 1], ["LPAREN", "(", 37, 34, 1], ["UNSIGNED", "unsigned", 37, 35, 1], ["SHORT", "short", 37, 44, 1], ["IDENTIFIER", "__value", 37, 50, 1], ["RPAREN", ")", 37, 57, 1], ["SEMICOLON", ";", 37, 58, 1], ["UNSIGNED", "unsigned", 38, 1, 1], ["INT", "int", 38, 10, 1], ["IDENTIFIER", "stdc_leading_ones_ui", 38, 14, 1], ["LPAREN", "(", 38, 34, 1], ["UNSIGNED", "unsigned", 38, 35, 1], ["INT", "int", 38, 44, 1], ["IDENTIFIER", "__value", 38, 48, 1], ["RPAREN", ")", 38, 55, 1], ["SEMICOLON", ";", 38, 56, 1], ["UNSIGNED", "unsigned", 39, 1, 1], ["INT", "int", 39, 10, 1], ["IDENTIFIER", "stdc_leading_ones_ul", 39, 14, 1], ["LPAREN", "(", 39, 34, 1], ["UNSIGNED", "unsigned", 39, 35, 1], ["LONG", "long", 39, 44, 1], ["IDENTIFIER", "__value", 39, 49, 1], ["RPAREN", ")", 39, 56, 1], ["SEMICOLON", ";", 39, 57, 1], ["UNSIGNED", "unsigned", 40, 1, 1], ["INT", "int", 40, 10, 1], ["IDENTIFIER", "stdc_leading_ones_ull", 40, 14, 1], ["LPAREN", "(", 40, 35, 1], ["UNSIGNED", "unsigned", 40, 36, 1], ["LONG", "long", 40, 45, 1], ["LONG", "long", 40, 50, 1], ["IDENTIFIER", "__value", 40, 55, 1], ["RPAREN", ")", 40, 62, 1], ["SEMICOLON", ";", 40, 63, 1], ["UNSIGNED", "unsigned", 49, 1, 1], ["INT", "int", 49, 10, 1], ["IDENTIFIER", "stdc_trailing_zeros_uc", 49, 14, 1], ["LPAREN", "(", 49, 36, 1], ["UNSIGNED", "unsigned", 49, 37, 1], ["CHAR", "char", 49, 46, 1], ["IDENTIFIER", "__value", 49, 51, 1], ["RPAREN", ")", 49, 58, 1], ["SEMICOLON", ";", 49, 59, 1], ["UNSIGNED", "unsigned", 50, 1, 1], ["INT", "int", 50, 10, 1], ["IDENTIFIER", "stdc_trailing_zeros_us", 50, 14, 1], ["LPAREN", "(", 50, 36, 1], ["UNSIGNED", "unsigned", 50, 37, 1], ["SHORT", "short", 50, 46, 1], ["IDENTIFIER", "__value", 50, 52, 1], ["RPAREN", ")", 50, 59, 1], ["SEMICOLON", ";", 50, 60, 1], ["UNSIGNED", "unsigned", 51, 1, 1], ["INT", "int", 51, 10, 1], ["IDENTIFIER", "stdc_trailing_zeros_ui", 51, 14, 1], ["LPAREN", "(", 51, 36, 1], ["UNSIGNED", "unsigned", 51, 37, 1], ["INT", "int", 51, 46, 1], ["IDENTIFIER", "__value", 51, 50, 1], ["RPAREN", ")", 51, 57, 1], ["SEMICOLON", ";", 51, 58, 1], ["UNSIGNED", "unsigned", 52, 1, 1], ["INT", "int", 52, 10, 1], ["IDENTIFIER", "stdc_trailing_zeros_ul", 52, 14, 1], ["LPAREN", "(", 52, 36, 1], ["UNSIGNED", "unsigned", 52, 37, 1], ["LONG", "long", 52, 46, 1], ["IDENTIFIER", "__value", 52, 51, 1], ["RPAREN", ")", 52, 58, 1], ["SEMICOLON", ";", 52, 59, 1], ["UNSIGNED", "unsigned", 53, 1, 1], ["INT", "int", 53, 10, 1], ["IDENTIFIER", "stdc_trailing_zeros_ull", 53, 14, 1], ["LPAREN", "(", 53, 37, 1], ["UNSIGNED", "unsigned", 53, 38, 1], ["LONG", "long", 53, 47, 1], ["LONG", "long", 53, 52, 1], ["IDENTIFIER", "__value", 53, 57, 1], ["RPAREN", ")", 53, 64, 1], ["SEMICOLON", ";", 53, 65, 1], ["UNSIGNED", "unsigned", 62, 1, 1], ["INT", "int", 62, 10, 1], ["IDENTIFIER", "stdc_trailing_ones_uc", 62, 14, 1], ["LPAREN", "(", 62, 35, 1], ["UNSIGNED", "unsigned", 62, 36, 1], ["CHAR", "char", 62, 45, 1], ["IDENTIFIER", "__value", 62, 50, 1], ["RPAREN", ")", 62, 57, 1], ["SEMICOLON", ";", 62, 58, 1], ["UNSIGNED", "unsigned", 63, 1, 1], ["INT", "int", 63, 10, 1], ["IDENTIFIER", "stdc_trailing_ones_us", 63, 14, 1], ["LPAREN", "(", 63, 35, 1], ["UNSIGNED", "unsigned", 63, 36, 1], ["SHORT", "short", 63, 45, 1], ["IDENTIFIER", "__value", 63, 51, 1], ["RPAREN", ")", 63, 58, 1], ["SEMICOLON", ";", 63, 59, 1], ["UNSIGNED", "unsigned", 64, 1, 1], ["INT", "int", 64, 10, 1], ["IDENTIFIER", "stdc_trailing_ones_ui", 6)~~" + R"~~(4, 14, 1], ["LPAREN", "(", 64, 35, 1], ["UNSIGNED", "unsigned", 64, 36, 1], ["INT", "int", 64, 45, 1], ["IDENTIFIER", "__value", 64, 49, 1], ["RPAREN", ")", 64, 56, 1], ["SEMICOLON", ";", 64, 57, 1], ["UNSIGNED", "unsigned", 65, 1, 1], ["INT", "int", 65, 10, 1], ["IDENTIFIER", "stdc_trailing_ones_ul", 65, 14, 1], ["LPAREN", "(", 65, 35, 1], ["UNSIGNED", "unsigned", 65, 36, 1], ["LONG", "long", 65, 45, 1], ["IDENTIFIER", "__value", 65, 50, 1], ["RPAREN", ")", 65, 57, 1], ["SEMICOLON", ";", 65, 58, 1], ["UNSIGNED", "unsigned", 66, 1, 1], ["INT", "int", 66, 10, 1], ["IDENTIFIER", "stdc_trailing_ones_ull", 66, 14, 1], ["LPAREN", "(", 66, 36, 1], ["UNSIGNED", "unsigned", 66, 37, 1], ["LONG", "long", 66, 46, 1], ["LONG", "long", 66, 51, 1], ["IDENTIFIER", "__value", 66, 56, 1], ["RPAREN", ")", 66, 63, 1], ["SEMICOLON", ";", 66, 64, 1], ["UNSIGNED", "unsigned", 75, 1, 1], ["INT", "int", 75, 10, 1], ["IDENTIFIER", "stdc_first_leading_zero_uc", 75, 14, 1], ["LPAREN", "(", 75, 40, 1], ["UNSIGNED", "unsigned", 75, 41, 1], ["CHAR", "char", 75, 50, 1], ["IDENTIFIER", "__value", 75, 55, 1], ["RPAREN", ")", 75, 62, 1], ["SEMICOLON", ";", 75, 63, 1], ["UNSIGNED", "unsigned", 76, 1, 1], ["INT", "int", 76, 10, 1], ["IDENTIFIER", "stdc_first_leading_zero_us", 76, 14, 1], ["LPAREN", "(", 76, 40, 1], ["UNSIGNED", "unsigned", 76, 41, 1], ["SHORT", "short", 76, 50, 1], ["IDENTIFIER", "__value", 76, 56, 1], ["RPAREN", ")", 76, 63, 1], ["SEMICOLON", ";", 76, 64, 1], ["UNSIGNED", "unsigned", 77, 1, 1], ["INT", "int", 77, 10, 1], ["IDENTIFIER", "stdc_first_leading_zero_ui", 77, 14, 1], ["LPAREN", "(", 77, 40, 1], ["UNSIGNED", "unsigned", 77, 41, 1], ["INT", "int", 77, 50, 1], ["IDENTIFIER", "__value", 77, 54, 1], ["RPAREN", ")", 77, 61, 1], ["SEMICOLON", ";", 77, 62, 1], ["UNSIGNED", "unsigned", 78, 1, 1], ["INT", "int", 78, 10, 1], ["IDENTIFIER", "stdc_first_leading_zero_ul", 78, 14, 1], ["LPAREN", "(", 78, 40, 1], ["UNSIGNED", "unsigned", 78, 41, 1], ["LONG", "long", 78, 50, 1], ["IDENTIFIER", "__value", 78, 55, 1], ["RPAREN", ")", 78, 62, 1], ["SEMICOLON", ";", 78, 63, 1], ["UNSIGNED", "unsigned", 79, 1, 1], ["INT", "int", 79, 10, 1], ["IDENTIFIER", "stdc_first_leading_zero_ull", 79, 14, 1], ["LPAREN", "(", 79, 41, 1], ["UNSIGNED", "unsigned", 79, 42, 1], ["LONG", "long", 79, 51, 1], ["LONG", "long", 79, 56, 1], ["IDENTIFIER", "__value", 79, 61, 1], ["RPAREN", ")", 79, 68, 1], ["SEMICOLON", ";", 79, 69, 1], ["UNSIGNED", "unsigned", 88, 1, 1], ["INT", "int", 88, 10, 1], ["IDENTIFIER", "stdc_first_leading_one_uc", 88, 14, 1], ["LPAREN", "(", 88, 39, 1], ["UNSIGNED", "unsigned", 88, 40, 1], ["CHAR", "char", 88, 49, 1], ["IDENTIFIER", "__value", 88, 54, 1], ["RPAREN", ")", 88, 61, 1], ["SEMICOLON", ";", 88, 62, 1], ["UNSIGNED", "unsigned", 89, 1, 1], ["INT", "int", 89, 10, 1], ["IDENTIFIER", "stdc_first_leading_one_us", 89, 14, 1], ["LPAREN", "(", 89, 39, 1], ["UNSIGNED", "unsigned", 89, 40, 1], ["SHORT", "short", 89, 49, 1], ["IDENTIFIER", "__value", 89, 55, 1], ["RPAREN", ")", 89, 62, 1], ["SEMICOLON", ";", 89, 63, 1], ["UNSIGNED", "unsigned", 90, 1, 1], ["INT", "int", 90, 10, 1], ["IDENTIFIER", "stdc_first_leading_one_ui", 90, 14, 1], ["LPAREN", "(", 90, 39, 1], ["UNSIGNED", "unsigned", 90, 40, 1], ["INT", "int", 90, 49, 1], ["IDENTIFIER", "__value", 90, 53, 1], ["RPAREN", ")", 90, 60, 1], ["SEMICOLON", ";", 90, 61, 1], ["UNSIGNED", "unsigned", 91, 1, 1], ["INT", "int", 91, 10, 1], ["IDENTIFIER", "stdc_first_leading_one_ul", 91, 14, 1], ["LPAREN", "(", 91, 39, 1], ["UNSIGNED", "unsigned", 91, 40, 1], ["LONG", "long", 91, 49, 1], ["IDENTIFIER", "__value", 91, 54, 1], ["RPAREN", ")", 91, 61, 1], ["SEMICOLON", ";", 91, 62, 1], ["UNSIGNED", "unsigned", 92, 1, 1], ["INT", "int", 92, 10, 1], ["IDENTIFIER", "stdc_first_leading_one_ull", 92, 14, 1], ["LPAREN", "(", 92, 40, 1], ["UNSIGNED", "unsigned", 92, 41, 1], ["LONG", "long", 92, 50, 1], ["LONG", "long", 92, 55, 1], ["IDENTIFIER", "__value", 92, 60, 1], ["RPAREN", ")", 92, 67, 1], ["SEMICOLON", ";", 92, 6)~~" + R"~~(8, 1], ["UNSIGNED", "unsigned", 101, 1, 1], ["INT", "int", 101, 10, 1], ["IDENTIFIER", "stdc_first_trailing_zero_uc", 101, 14, 1], ["LPAREN", "(", 101, 41, 1], ["UNSIGNED", "unsigned", 101, 42, 1], ["CHAR", "char", 101, 51, 1], ["IDENTIFIER", "__value", 101, 56, 1], ["RPAREN", ")", 101, 63, 1], ["SEMICOLON", ";", 101, 64, 1], ["UNSIGNED", "unsigned", 102, 1, 1], ["INT", "int", 102, 10, 1], ["IDENTIFIER", "stdc_first_trailing_zero_us", 102, 14, 1], ["LPAREN", "(", 102, 41, 1], ["UNSIGNED", "unsigned", 102, 42, 1], ["SHORT", "short", 102, 51, 1], ["IDENTIFIER", "__value", 102, 57, 1], ["RPAREN", ")", 102, 64, 1], ["SEMICOLON", ";", 102, 65, 1], ["UNSIGNED", "unsigned", 103, 1, 1], ["INT", "int", 103, 10, 1], ["IDENTIFIER", "stdc_first_trailing_zero_ui", 103, 14, 1], ["LPAREN", "(", 103, 41, 1], ["UNSIGNED", "unsigned", 103, 42, 1], ["INT", "int", 103, 51, 1], ["IDENTIFIER", "__value", 103, 55, 1], ["RPAREN", ")", 103, 62, 1], ["SEMICOLON", ";", 103, 63, 1], ["UNSIGNED", "unsigned", 104, 1, 1], ["INT", "int", 104, 10, 1], ["IDENTIFIER", "stdc_first_trailing_zero_ul", 104, 14, 1], ["LPAREN", "(", 104, 41, 1], ["UNSIGNED", "unsigned", 104, 42, 1], ["LONG", "long", 104, 51, 1], ["IDENTIFIER", "__value", 104, 56, 1], ["RPAREN", ")", 104, 63, 1], ["SEMICOLON", ";", 104, 64, 1], ["UNSIGNED", "unsigned", 105, 1, 1], ["INT", "int", 105, 10, 1], ["IDENTIFIER", "stdc_first_trailing_zero_ull", 105, 14, 1], ["LPAREN", "(", 105, 42, 1], ["UNSIGNED", "unsigned", 105, 43, 1], ["LONG", "long", 105, 52, 1], ["LONG", "long", 105, 57, 1], ["IDENTIFIER", "__value", 105, 62, 1], ["RPAREN", ")", 105, 69, 1], ["SEMICOLON", ";", 105, 70, 1], ["UNSIGNED", "unsigned", 114, 1, 1], ["INT", "int", 114, 10, 1], ["IDENTIFIER", "stdc_first_trailing_one_uc", 114, 14, 1], ["LPAREN", "(", 114, 40, 1], ["UNSIGNED", "unsigned", 114, 41, 1], ["CHAR", "char", 114, 50, 1], ["IDENTIFIER", "__value", 114, 55, 1], ["RPAREN", ")", 114, 62, 1], ["SEMICOLON", ";", 114, 63, 1], ["UNSIGNED", "unsigned", 115, 1, 1], ["INT", "int", 115, 10, 1], ["IDENTIFIER", "stdc_first_trailing_one_us", 115, 14, 1], ["LPAREN", "(", 115, 40, 1], ["UNSIGNED", "unsigned", 115, 41, 1], ["SHORT", "short", 115, 50, 1], ["IDENTIFIER", "__value", 115, 56, 1], ["RPAREN", ")", 115, 63, 1], ["SEMICOLON", ";", 115, 64, 1], ["UNSIGNED", "unsigned", 116, 1, 1], ["INT", "int", 116, 10, 1], ["IDENTIFIER", "stdc_first_trailing_one_ui", 116, 14, 1], ["LPAREN", "(", 116, 40, 1], ["UNSIGNED", "unsigned", 116, 41, 1], ["INT", "int", 116, 50, 1], ["IDENTIFIER", "__value", 116, 54, 1], ["RPAREN", ")", 116, 61, 1], ["SEMICOLON", ";", 116, 62, 1], ["UNSIGNED", "unsigned", 117, 1, 1], ["INT", "int", 117, 10, 1], ["IDENTIFIER", "stdc_first_trailing_one_ul", 117, 14, 1], ["LPAREN", "(", 117, 40, 1], ["UNSIGNED", "unsigned", 117, 41, 1], ["LONG", "long", 117, 50, 1], ["IDENTIFIER", "__value", 117, 55, 1], ["RPAREN", ")", 117, 62, 1], ["SEMICOLON", ";", 117, 63, 1], ["UNSIGNED", "unsigned", 118, 1, 1], ["INT", "int", 118, 10, 1], ["IDENTIFIER", "stdc_first_trailing_one_ull", 118, 14, 1], ["LPAREN", "(", 118, 41, 1], ["UNSIGNED", "unsigned", 118, 42, 1], ["LONG", "long", 118, 51, 1], ["LONG", "long", 118, 56, 1], ["IDENTIFIER", "__value", 118, 61, 1], ["RPAREN", ")", 118, 68, 1], ["SEMICOLON", ";", 118, 69, 1], ["UNSIGNED", "unsigned", 127, 1, 1], ["INT", "int", 127, 10, 1], ["IDENTIFIER", "stdc_count_zeros_uc", 127, 14, 1], ["LPAREN", "(", 127, 33, 1], ["UNSIGNED", "unsigned", 127, 34, 1], ["CHAR", "char", 127, 43, 1], ["IDENTIFIER", "__value", 127, 48, 1], ["RPAREN", ")", 127, 55, 1], ["SEMICOLON", ";", 127, 56, 1], ["UNSIGNED", "unsigned", 128, 1, 1], ["INT", "int", 128, 10, 1], ["IDENTIFIER", "stdc_count_zeros_us", 128, 14, 1], ["LPAREN", "(", 128, 33, 1], ["UNSIGNED", "unsigned", 128, 34, 1], ["SHORT", "short", 128, 43, 1], ["IDENTIFIER", "__value", 128, 49, 1], ["RPAREN", ")", 128, 56, 1], ["SEMICOLON", ";", 128, 57, 1], ["UNSIGNED", "unsigned", 129, 1, 1], ["INT", "int", 129, 10, 1], ["IDENTIFIER", "stdc_count_zeros_)~~" + R"~~(ui", 129, 14, 1], ["LPAREN", "(", 129, 33, 1], ["UNSIGNED", "unsigned", 129, 34, 1], ["INT", "int", 129, 43, 1], ["IDENTIFIER", "__value", 129, 47, 1], ["RPAREN", ")", 129, 54, 1], ["SEMICOLON", ";", 129, 55, 1], ["UNSIGNED", "unsigned", 130, 1, 1], ["INT", "int", 130, 10, 1], ["IDENTIFIER", "stdc_count_zeros_ul", 130, 14, 1], ["LPAREN", "(", 130, 33, 1], ["UNSIGNED", "unsigned", 130, 34, 1], ["LONG", "long", 130, 43, 1], ["IDENTIFIER", "__value", 130, 48, 1], ["RPAREN", ")", 130, 55, 1], ["SEMICOLON", ";", 130, 56, 1], ["UNSIGNED", "unsigned", 131, 1, 1], ["INT", "int", 131, 10, 1], ["IDENTIFIER", "stdc_count_zeros_ull", 131, 14, 1], ["LPAREN", "(", 131, 34, 1], ["UNSIGNED", "unsigned", 131, 35, 1], ["LONG", "long", 131, 44, 1], ["LONG", "long", 131, 49, 1], ["IDENTIFIER", "__value", 131, 54, 1], ["RPAREN", ")", 131, 61, 1], ["SEMICOLON", ";", 131, 62, 1], ["UNSIGNED", "unsigned", 140, 1, 1], ["INT", "int", 140, 10, 1], ["IDENTIFIER", "stdc_count_ones_uc", 140, 14, 1], ["LPAREN", "(", 140, 32, 1], ["UNSIGNED", "unsigned", 140, 33, 1], ["CHAR", "char", 140, 42, 1], ["IDENTIFIER", "__value", 140, 47, 1], ["RPAREN", ")", 140, 54, 1], ["SEMICOLON", ";", 140, 55, 1], ["UNSIGNED", "unsigned", 141, 1, 1], ["INT", "int", 141, 10, 1], ["IDENTIFIER", "stdc_count_ones_us", 141, 14, 1], ["LPAREN", "(", 141, 32, 1], ["UNSIGNED", "unsigned", 141, 33, 1], ["SHORT", "short", 141, 42, 1], ["IDENTIFIER", "__value", 141, 48, 1], ["RPAREN", ")", 141, 55, 1], ["SEMICOLON", ";", 141, 56, 1], ["UNSIGNED", "unsigned", 142, 1, 1], ["INT", "int", 142, 10, 1], ["IDENTIFIER", "stdc_count_ones_ui", 142, 14, 1], ["LPAREN", "(", 142, 32, 1], ["UNSIGNED", "unsigned", 142, 33, 1], ["INT", "int", 142, 42, 1], ["IDENTIFIER", "__value", 142, 46, 1], ["RPAREN", ")", 142, 53, 1], ["SEMICOLON", ";", 142, 54, 1], ["UNSIGNED", "unsigned", 143, 1, 1], ["INT", "int", 143, 10, 1], ["IDENTIFIER", "stdc_count_ones_ul", 143, 14, 1], ["LPAREN", "(", 143, 32, 1], ["UNSIGNED", "unsigned", 143, 33, 1], ["LONG", "long", 143, 42, 1], ["IDENTIFIER", "__value", 143, 47, 1], ["RPAREN", ")", 143, 54, 1], ["SEMICOLON", ";", 143, 55, 1], ["UNSIGNED", "unsigned", 144, 1, 1], ["INT", "int", 144, 10, 1], ["IDENTIFIER", "stdc_count_ones_ull", 144, 14, 1], ["LPAREN", "(", 144, 33, 1], ["UNSIGNED", "unsigned", 144, 34, 1], ["LONG", "long", 144, 43, 1], ["LONG", "long", 144, 48, 1], ["IDENTIFIER", "__value", 144, 53, 1], ["RPAREN", ")", 144, 60, 1], ["SEMICOLON", ";", 144, 61, 1], ["BOOL", "bool", 153, 1, 1], ["IDENTIFIER", "stdc_has_single_bit_uc", 153, 6, 1], ["LPAREN", "(", 153, 28, 1], ["UNSIGNED", "unsigned", 153, 29, 1], ["CHAR", "char", 153, 38, 1], ["IDENTIFIER", "__value", 153, 43, 1], ["RPAREN", ")", 153, 50, 1], ["SEMICOLON", ";", 153, 51, 1], ["BOOL", "bool", 154, 1, 1], ["IDENTIFIER", "stdc_has_single_bit_us", 154, 6, 1], ["LPAREN", "(", 154, 28, 1], ["UNSIGNED", "unsigned", 154, 29, 1], ["SHORT", "short", 154, 38, 1], ["IDENTIFIER", "__value", 154, 44, 1], ["RPAREN", ")", 154, 51, 1], ["SEMICOLON", ";", 154, 52, 1], ["BOOL", "bool", 155, 1, 1], ["IDENTIFIER", "stdc_has_single_bit_ui", 155, 6, 1], ["LPAREN", "(", 155, 28, 1], ["UNSIGNED", "unsigned", 155, 29, 1], ["INT", "int", 155, 38, 1], ["IDENTIFIER", "__value", 155, 42, 1], ["RPAREN", ")", 155, 49, 1], ["SEMICOLON", ";", 155, 50, 1], ["BOOL", "bool", 156, 1, 1], ["IDENTIFIER", "stdc_has_single_bit_ul", 156, 6, 1], ["LPAREN", "(", 156, 28, 1], ["UNSIGNED", "unsigned", 156, 29, 1], ["LONG", "long", 156, 38, 1], ["IDENTIFIER", "__value", 156, 43, 1], ["RPAREN", ")", 156, 50, 1], ["SEMICOLON", ";", 156, 51, 1], ["BOOL", "bool", 157, 1, 1], ["IDENTIFIER", "stdc_has_single_bit_ull", 157, 6, 1], ["LPAREN", "(", 157, 29, 1], ["UNSIGNED", "unsigned", 157, 30, 1], ["LONG", "long", 157, 39, 1], ["LONG", "long", 157, 44, 1], ["IDENTIFIER", "__value", 157, 49, 1], ["RPAREN", ")", 157, 56, 1], ["SEMICOLON", ";", 157, 57, 1], ["UNSIGNED", "unsigned", 166, 1, 1], ["INT", "int", 166, 10, 1], ["IDENTIFIER", "stdc_bit_width_uc", 166, 14, 1],)~~" + R"~~( ["LPAREN", "(", 166, 31, 1], ["UNSIGNED", "unsigned", 166, 32, 1], ["CHAR", "char", 166, 41, 1], ["IDENTIFIER", "__value", 166, 46, 1], ["RPAREN", ")", 166, 53, 1], ["SEMICOLON", ";", 166, 54, 1], ["UNSIGNED", "unsigned", 167, 1, 1], ["INT", "int", 167, 10, 1], ["IDENTIFIER", "stdc_bit_width_us", 167, 14, 1], ["LPAREN", "(", 167, 31, 1], ["UNSIGNED", "unsigned", 167, 32, 1], ["SHORT", "short", 167, 41, 1], ["IDENTIFIER", "__value", 167, 47, 1], ["RPAREN", ")", 167, 54, 1], ["SEMICOLON", ";", 167, 55, 1], ["UNSIGNED", "unsigned", 168, 1, 1], ["INT", "int", 168, 10, 1], ["IDENTIFIER", "stdc_bit_width_ui", 168, 14, 1], ["LPAREN", "(", 168, 31, 1], ["UNSIGNED", "unsigned", 168, 32, 1], ["INT", "int", 168, 41, 1], ["IDENTIFIER", "__value", 168, 45, 1], ["RPAREN", ")", 168, 52, 1], ["SEMICOLON", ";", 168, 53, 1], ["UNSIGNED", "unsigned", 169, 1, 1], ["INT", "int", 169, 10, 1], ["IDENTIFIER", "stdc_bit_width_ul", 169, 14, 1], ["LPAREN", "(", 169, 31, 1], ["UNSIGNED", "unsigned", 169, 32, 1], ["LONG", "long", 169, 41, 1], ["IDENTIFIER", "__value", 169, 46, 1], ["RPAREN", ")", 169, 53, 1], ["SEMICOLON", ";", 169, 54, 1], ["UNSIGNED", "unsigned", 170, 1, 1], ["INT", "int", 170, 10, 1], ["IDENTIFIER", "stdc_bit_width_ull", 170, 14, 1], ["LPAREN", "(", 170, 32, 1], ["UNSIGNED", "unsigned", 170, 33, 1], ["LONG", "long", 170, 42, 1], ["LONG", "long", 170, 47, 1], ["IDENTIFIER", "__value", 170, 52, 1], ["RPAREN", ")", 170, 59, 1], ["SEMICOLON", ";", 170, 60, 1], ["UNSIGNED", "unsigned", 179, 1, 1], ["CHAR", "char", 179, 10, 1], ["IDENTIFIER", "stdc_bit_floor_uc", 179, 15, 1], ["LPAREN", "(", 179, 32, 1], ["UNSIGNED", "unsigned", 179, 33, 1], ["CHAR", "char", 179, 42, 1], ["IDENTIFIER", "__value", 179, 47, 1], ["RPAREN", ")", 179, 54, 1], ["SEMICOLON", ";", 179, 55, 1], ["UNSIGNED", "unsigned", 180, 1, 1], ["SHORT", "short", 180, 10, 1], ["IDENTIFIER", "stdc_bit_floor_us", 180, 16, 1], ["LPAREN", "(", 180, 33, 1], ["UNSIGNED", "unsigned", 180, 34, 1], ["SHORT", "short", 180, 43, 1], ["IDENTIFIER", "__value", 180, 49, 1], ["RPAREN", ")", 180, 56, 1], ["SEMICOLON", ";", 180, 57, 1], ["UNSIGNED", "unsigned", 181, 1, 1], ["INT", "int", 181, 10, 1], ["IDENTIFIER", "stdc_bit_floor_ui", 181, 14, 1], ["LPAREN", "(", 181, 31, 1], ["UNSIGNED", "unsigned", 181, 32, 1], ["INT", "int", 181, 41, 1], ["IDENTIFIER", "__value", 181, 45, 1], ["RPAREN", ")", 181, 52, 1], ["SEMICOLON", ";", 181, 53, 1], ["UNSIGNED", "unsigned", 182, 1, 1], ["LONG", "long", 182, 10, 1], ["IDENTIFIER", "stdc_bit_floor_ul", 182, 15, 1], ["LPAREN", "(", 182, 32, 1], ["UNSIGNED", "unsigned", 182, 33, 1], ["LONG", "long", 182, 42, 1], ["IDENTIFIER", "__value", 182, 47, 1], ["RPAREN", ")", 182, 54, 1], ["SEMICOLON", ";", 182, 55, 1], ["UNSIGNED", "unsigned", 183, 1, 1], ["LONG", "long", 183, 10, 1], ["LONG", "long", 183, 15, 1], ["IDENTIFIER", "stdc_bit_floor_ull", 183, 20, 1], ["LPAREN", "(", 183, 38, 1], ["UNSIGNED", "unsigned", 183, 39, 1], ["LONG", "long", 183, 48, 1], ["LONG", "long", 183, 53, 1], ["IDENTIFIER", "__value", 183, 58, 1], ["RPAREN", ")", 183, 65, 1], ["SEMICOLON", ";", 183, 66, 1], ["UNSIGNED", "unsigned", 192, 1, 1], ["CHAR", "char", 192, 10, 1], ["IDENTIFIER", "stdc_bit_ceil_uc", 192, 15, 1], ["LPAREN", "(", 192, 31, 1], ["UNSIGNED", "unsigned", 192, 32, 1], ["CHAR", "char", 192, 41, 1], ["IDENTIFIER", "__value", 192, 46, 1], ["RPAREN", ")", 192, 53, 1], ["SEMICOLON", ";", 192, 54, 1], ["UNSIGNED", "unsigned", 193, 1, 1], ["SHORT", "short", 193, 10, 1], ["IDENTIFIER", "stdc_bit_ceil_us", 193, 16, 1], ["LPAREN", "(", 193, 32, 1], ["UNSIGNED", "unsigned", 193, 33, 1], ["SHORT", "short", 193, 42, 1], ["IDENTIFIER", "__value", 193, 48, 1], ["RPAREN", ")", 193, 55, 1], ["SEMICOLON", ";", 193, 56, 1], ["UNSIGNED", "unsigned", 194, 1, 1], ["INT", "int", 194, 10, 1], ["IDENTIFIER", "stdc_bit_ceil_ui", 194, 14, 1], ["LPAREN", "(", 194, 30, 1], ["UNSIGNED", "unsigned", 194, 31, 1], ["INT", "int", 194, 40, 1], ["IDENTIFIER", "__value", 194, 44, 1], ["RPAREN", ")", 194, 51, 1)~~" + R"~~(], ["SEMICOLON", ";", 194, 52, 1], ["UNSIGNED", "unsigned", 195, 1, 1], ["LONG", "long", 195, 10, 1], ["IDENTIFIER", "stdc_bit_ceil_ul", 195, 15, 1], ["LPAREN", "(", 195, 31, 1], ["UNSIGNED", "unsigned", 195, 32, 1], ["LONG", "long", 195, 41, 1], ["IDENTIFIER", "__value", 195, 46, 1], ["RPAREN", ")", 195, 53, 1], ["SEMICOLON", ";", 195, 54, 1], ["UNSIGNED", "unsigned", 196, 1, 1], ["LONG", "long", 196, 10, 1], ["LONG", "long", 196, 15, 1], ["IDENTIFIER", "stdc_bit_ceil_ull", 196, 20, 1], ["LPAREN", "(", 196, 37, 1], ["UNSIGNED", "unsigned", 196, 38, 1], ["LONG", "long", 196, 47, 1], ["LONG", "long", 196, 52, 1], ["IDENTIFIER", "__value", 196, 57, 1], ["RPAREN", ")", 196, 64, 1], ["SEMICOLON", ";", 196, 65, 1], ["IDENTIFIER", "intptr_t", 197, 1, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'intptr_t') at line 197, column 1.\nExpected one of: \n\t* $END\n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~"},
    {std::string(R"~~(int main(void) {
    0invalid_label:
        return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INTEGER_CONSTANT", "0", 2, 5, 2], ["IDENTIFIER", "invalid_label", 2, 6, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'invalid_label') at line 2, column 6.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(int main(void) {

    return 0 l;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 3, 5, 2], ["INTEGER_CONSTANT", "0", 3, 12, 2], ["IDENTIFIER", "l", 3, 14, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'l') at line 3, column 14.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(int main(void) {

    return 0lul;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 3, 5, 2], ["ULONG_INTEGER", "0lu", 3, 12, 2], ["IDENTIFIER", "l", 3, 15, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'l') at line 3, column 15.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(label:
int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["IDENTIFIER", "label", 1, 1, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'label') at line 1, column 1.\nExpected one of: \n\t* $END\n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["IDENTIFIER", "main", 1, 1, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'main') at line 1, column 1.\nExpected one of: \n\t* $END\n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(



main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["IDENTIFIER", "main", 5, 1, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'main') at line 5, column 1.\nExpected one of: \n\t* $END\n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void){
    retur n 0;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 15, 2], ["IDENTIFIER", "retur", 2, 5, 2], ["IDENTIFIER", "n", 2, 11, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'n') at line 2, column 11.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COLON\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(s struct x { int a; };

int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["IDENTIFIER", "s", 1, 1, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 's') at line 1, column 1.\nExpected one of: \n\t* $END\n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(
s struct x { int a; };

int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["IDENTIFIER", "s", 2, 1, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 's') at line 2, column 1.\nExpected one of: \n\t* $END\n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void) {

    return 0uu;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 3, 5, 2], ["UINT_INTEGER", "0u", 3, 12, 2], ["IDENTIFIER", "u", 3, 14, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'u') at line 3, column 14.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(int x = 5wB;)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "x", 1, 5, 1], ["ASSIGN", "=", 1, 7, 1], ["INTEGER_CONSTANT", "5", 1, 9, 1], ["IDENTIFIER", "wB", 1, 10, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'wB') at line 1, column 10.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(x:
int foo = 0;

int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["IDENTIFIER", "x", 1, 1, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'x') at line 1, column 1.\nExpected one of: \n\t* $END\n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void) { { typedef int T; } T x; return 0; })~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["LBRACE", "{", 1, 18, 3], ["TYPEDEF", "typedef", 1, 20, 3], ["INT", "int", 1, 28, 3], ["IDENTIFIER", "T", 1, 32, 3], ["SEMICOLON", ";", 1, 33, 3], ["RBRACE", "}", 1, 35, 2], ["IDENTIFIER", "T", 1, 37, 2], ["IDENTIFIER", "x", 1, 39, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'x') at line 1, column 39.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COLON\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(int main(void) { int x; return ++(int)x; })~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 1, 18, 2], ["IDENTIFIER", "x", 1, 22, 2], ["SEMICOLON", ";", 1, 23, 2], ["RETURN", "return", 1, 25, 2], ["PLUSPLUS", "++", 1, 32, 2], ["LPAREN", "(", 1, 34, 2], ["INT", "int", 1, 35, 2], ["RPAREN", ")", 1, 38, 2], ["IDENTIFIER", "x", 1, 39, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'x') at line 1, column 39.\nExpected one of: \n\t* LBRACE\n"]])~~")},
    {std::string(R"~~(
x:
int foo = 0;

int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["IDENTIFIER", "x", 2, 1, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'x') at line 2, column 1.\nExpected one of: \n\t* $END\n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void)
{
    return 1.e-10x;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["RETURN", "return", 3, 5, 2], ["DOUBLE_CONSTANT", "1.e-10", 3, 12, 2], ["IDENTIFIER", "x", 3, 18, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'x') at line 3, column 18.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(int main(void)
{
    double foo = 1E2x;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["DOUBLE", "double", 3, 5, 2], ["IDENTIFIER", "foo", 3, 12, 2], ["ASSIGN", "=", 3, 16, 2], ["DOUBLE_CONSTANT", "1E2", 3, 18, 2], ["IDENTIFIER", "x", 3, 21, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'x') at line 3, column 21.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(int main(void) {
    struct x;
    x y;

    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["STRUCT", "struct", 2, 5, 2], ["IDENTIFIER", "x", 2, 12, 2], ["SEMICOLON", ";", 2, 13, 2], ["IDENTIFIER", "x", 3, 5, 2], ["IDENTIFIER", "y", 3, 7, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IDENTIFIER', 'y') at line 3, column 7.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COLON\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(int main(void) {
    int flag = 0;
    int a = if (flag)
                2;
            else
                3;
    return a;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 2, 5, 2], ["IDENTIFIER", "flag", 2, 9, 2], ["ASSIGN", "=", 2, 14, 2], ["INTEGER_CONSTANT", "0", 2, 16, 2], ["SEMICOLON", ";", 2, 17, 2], ["INT", "int", 3, 5, 2], ["IDENTIFIER", "a", 3, 9, 2], ["ASSIGN", "=", 3, 11, 2], ["IF", "if", 3, 13, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('IF', 'if') at line 3, column 13.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void) {
    while(int a) {
        2;
    }
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["WHILE", "while", 2, 5, 2], ["LPAREN", "(", 2, 10, 2], ["INT", "int", 2, 11, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INT', 'int') at line 2, column 11.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return int;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["INT", "int", 2, 12, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INT', 'int') at line 2, column 12.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SEMICOLON\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return sizeof int;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["SIZEOF", "sizeof", 2, 12, 2], ["INT", "int", 2, 19, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INT', 'int') at line 2, column 19.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(
int main(void) {
    return sizeof int;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 2, 1, 1], ["IDENTIFIER", "main", 2, 5, 1], ["LPAREN", "(", 2, 9, 1], ["VOID", "void", 2, 10, 1], ["RPAREN", ")", 2, 14, 1], ["LBRACE", "{", 2, 16, 2], ["RETURN", "return", 3, 5, 2], ["SIZEOF", "sizeof", 3, 12, 2], ["INT", "int", 3, 19, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INT', 'int') at line 3, column 19.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void) {
    if (5)
        int i = 0;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["IF", "if", 2, 5, 2], ["LPAREN", "(", 2, 8, 2], ["INTEGER_CONSTANT", "5", 2, 9, 2], ["RPAREN", ")", 2, 10, 2], ["INT", "int", 3, 9, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INT', 'int') at line 3, column 9.\nExpected one of: \n\t* AMP\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* BREAK\n\t* CASE\n\t* CHAR_CONSTANT\n\t* CONTINUE\n\t* DEFAULT\n\t* DO\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* FOR\n\t* GOTO\n\t* IDENTIFIER\n\t* IF\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* RETURN\n\t* SEMICOLON\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* SWITCH\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\t* WHILE\n"]])~~")},
    {std::string(R"~~(int main(void) {

label:
    int a = 0;
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["IDENTIFIER", "label", 3, 1, 2], ["COLON", ":", 3, 6, 2], ["INT", "int", 4, 5, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INT', 'int') at line 4, column 5.\nExpected one of: \n\t* AMP\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* BREAK\n\t* CASE\n\t* CHAR_CONSTANT\n\t* CONTINUE\n\t* DEFAULT\n\t* DO\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* FOR\n\t* GOTO\n\t* IDENTIFIER\n\t* IF\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* RETURN\n\t* SEMICOLON\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* SWITCH\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\t* WHILE\n"]])~~")},
    {std::string(R"~~(int foo(int a) {
    return 0;
}

int main(void) {

    return foo(int a);
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "foo", 1, 5, 1], ["LPAREN", "(", 1, 8, 1], ["INT", "int", 1, 9, 1], ["IDENTIFIER", "a", 1, 13, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["INTEGER_CONSTANT", "0", 2, 12, 2], ["SEMICOLON", ";", 2, 13, 2], ["RBRACE", "}", 3, 1, 1], ["INT", "int", 5, 1, 1], ["IDENTIFIER", "main", 5, 5, 1], ["LPAREN", "(", 5, 9, 1], ["VOID", "void", 5, 10, 1], ["RPAREN", ")", 5, 14, 1], ["LBRACE", "{", 5, 16, 2], ["RETURN", "return", 7, 5, 2], ["IDENTIFIER", "foo", 7, 12, 2], ["LPAREN", "(", 7, 15, 2], ["INT", "int", 7, 16, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INT', 'int') at line 7, column 16.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* RPAREN\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void) {
    switch(3) {
        case 3:




            int i = 0;
            return i;
    }
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["SWITCH", "switch", 2, 5, 2], ["LPAREN", "(", 2, 11, 2], ["INTEGER_CONSTANT", "3", 2, 12, 2], ["RPAREN", ")", 2, 13, 2], ["LBRACE", "{", 2, 15, 3], ["CASE", "case", 3, 9, 3], ["INTEGER_CONSTANT", "3", 3, 14, 3], ["COLON", ":", 3, 15, 3], ["INT", "int", 8, 13, 3]], ["syntax", "UnexpectedToken", "Unexpected token Token('INT', 'int') at line 8, column 13.\nExpected one of: \n\t* AMP\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* BREAK\n\t* CASE\n\t* CHAR_CONSTANT\n\t* CONTINUE\n\t* DEFAULT\n\t* DO\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* FOR\n\t* GOTO\n\t* IDENTIFIER\n\t* IF\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* RETURN\n\t* SEMICOLON\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* SWITCH\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\t* WHILE\n"]])~~")},
    {std::string(R"~~(int main(void) {
    RETURN 0;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["IDENTIFIER", "RETURN", 2, 5, 2], ["INTEGER_CONSTANT", "0", 2, 12, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INTEGER_CONSTANT', '0') at line 2, column 12.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COLON\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(int main(void) {
    returns 0;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["IDENTIFIER", "returns", 2, 5, 2], ["INTEGER_CONSTANT", "0", 2, 13, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INTEGER_CONSTANT', '0') at line 2, column 13.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COLON\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(int main(void) {
    if 0 return 1;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["IF", "if", 2, 5, 2], ["INTEGER_CONSTANT", "0", 2, 8, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INTEGER_CONSTANT', '0') at line 2, column 8.\nExpected one of: \n\t* LPAREN\n"]])~~")},
    {std::string(R"~~(int main(void) { switch 1 { case 0: return 0; } })~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["SWITCH", "switch", 1, 18, 2], ["INTEGER_CONSTANT", "1", 1, 25, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INTEGER_CONSTANT', '1') at line 1, column 25.\nExpected one of: \n\t* LPAREN\n"]])~~")},
    {std::string(R"~~(int main(void) {
    while 1 {
        return 0;
    }
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["WHILE", "while", 2, 5, 2], ["INTEGER_CONSTANT", "1", 2, 11, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INTEGER_CONSTANT', '1') at line 2, column 11.\nExpected one of: \n\t* LPAREN\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return sizeof(char) 1;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["SIZEOF", "sizeof", 2, 12, 2], ["LPAREN", "(", 2, 18, 2], ["CHAR", "char", 2, 19, 2], ["RPAREN", ")", 2, 23, 2], ["INTEGER_CONSTANT", "1", 2, 25, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INTEGER_CONSTANT', '1') at line 2, column 25.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACE\n\t* LE\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(int main(void) {
    int a = 0;
    return a ++ 1;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 2, 5, 2], ["IDENTIFIER", "a", 2, 9, 2], ["ASSIGN", "=", 2, 11, 2], ["INTEGER_CONSTANT", "0", 2, 13, 2], ["SEMICOLON", ";", 2, 14, 2], ["RETURN", "return", 3, 5, 2], ["IDENTIFIER", "a", 3, 12, 2], ["PLUSPLUS", "++", 3, 14, 2], ["INTEGER_CONSTANT", "1", 3, 17, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INTEGER_CONSTANT', '1') at line 3, column 17.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(

int main(void) {
    return sizeof(char) 1;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 3, 1, 1], ["IDENTIFIER", "main", 3, 5, 1], ["LPAREN", "(", 3, 9, 1], ["VOID", "void", 3, 10, 1], ["RPAREN", ")", 3, 14, 1], ["LBRACE", "{", 3, 16, 2], ["RETURN", "return", 4, 5, 2], ["SIZEOF", "sizeof", 4, 12, 2], ["LPAREN", "(", 4, 18, 2], ["CHAR", "char", 4, 19, 2], ["RPAREN", ")", 4, 23, 2], ["INTEGER_CONSTANT", "1", 4, 25, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INTEGER_CONSTANT', '1') at line 4, column 25.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACE\n\t* LE\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(int main(void)
{
    int 10 = return 0;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["INT", "int", 3, 5, 2], ["INTEGER_CONSTANT", "10", 3, 9, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INTEGER_CONSTANT', '10') at line 3, column 9.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* IDENTIFIER\n\t* INT\n\t* LONG\n\t* LPAREN\n\t* REGISTER\n\t* RESTRICT\n\t* SEMICOLON\n\t* SHORT\n\t* SIGNED\n\t* STAR\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void) {



    return (static int) 10;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 5, 5, 2], ["LPAREN", "(", 5, 12, 2], ["STATIC", "static", 5, 13, 2], ["INT", "int", 5, 20, 2], ["RPAREN", ")", 5, 23, 2], ["INTEGER_CONSTANT", "10", 5, 25, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INTEGER_CONSTANT', '10') at line 5, column 25.\nExpected one of: \n\t* LBRACE\n"]])~~")},
    {std::string(R"~~(enum E : 3 { A };)~~"),
     {},
     {},
     std::string(R"~~([[["ENUM", "enum", 1, 1, 1], ["IDENTIFIER", "E", 1, 6, 1], ["COLON", ":", 1, 8, 1], ["INTEGER_CONSTANT", "3", 1, 10, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('INTEGER_CONSTANT', '3') at line 1, column 10.\nExpected one of: \n\t* ALT_BOOL\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* DOUBLE\n\t* ENUM\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STRUCT\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void) { int 3x; })~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 1, 18, 2], ["INTEGER_CONSTANT", "3", 1, 22, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INTEGER_CONSTANT', '3') at line 1, column 22.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* IDENTIFIER\n\t* INT\n\t* LONG\n\t* LPAREN\n\t* REGISTER\n\t* RESTRICT\n\t* SEMICOLON\n\t* SHORT\n\t* SIGNED\n\t* STAR\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int 3 (void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["INTEGER_CONSTANT", "3", 1, 5, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('INTEGER_CONSTANT', '3') at line 1, column 5.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* IDENTIFIER\n\t* INT\n\t* LONG\n\t* LPAREN\n\t* REGISTER\n\t* RESTRICT\n\t* SEMICOLON\n\t* SHORT\n\t* SIGNED\n\t* STAR\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void) {
    goto 3;
    switch (3) {
        case 3: return 0;
    }
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["GOTO", "goto", 2, 5, 2], ["INTEGER_CONSTANT", "3", 2, 10, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INTEGER_CONSTANT', '3') at line 2, column 10.\nExpected one of: \n\t* IDENTIFIER\n"]])~~")},
    {std::string(R"~~(int main(void) {
    switch 3 {
        case 3: return 0;
    }
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["SWITCH", "switch", 2, 5, 2], ["INTEGER_CONSTANT", "3", 2, 12, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INTEGER_CONSTANT', '3') at line 2, column 12.\nExpected one of: \n\t* LPAREN\n"]])~~")},
    {std::string(R"~~(
int 3 (void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 2, 1, 1], ["INTEGER_CONSTANT", "3", 2, 5, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('INTEGER_CONSTANT', '3') at line 2, column 5.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* IDENTIFIER\n\t* INT\n\t* LONG\n\t* LPAREN\n\t* REGISTER\n\t* RESTRICT\n\t* SEMICOLON\n\t* SHORT\n\t* SIGNED\n\t* STAR\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void) {
    union 4 foo;
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["UNION", "union", 2, 5, 2], ["INTEGER_CONSTANT", "4", 2, 11, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INTEGER_CONSTANT', '4') at line 2, column 11.\nExpected one of: \n\t* ATTR_OPEN\n\t* IDENTIFIER\n\t* LBRACE\n"]])~~")},
    {std::string(R"~~(int main(void) {
    struct 4 foo;
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["STRUCT", "struct", 2, 5, 2], ["INTEGER_CONSTANT", "4", 2, 12, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INTEGER_CONSTANT', '4') at line 2, column 12.\nExpected one of: \n\t* ATTR_OPEN\n\t* IDENTIFIER\n\t* LBRACE\n"]])~~")},
    {std::string(R"~~(int main(void) {



    double foo = 24e-;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["DOUBLE", "double", 5, 5, 2], ["IDENTIFIER", "foo", 5, 12, 2], ["ASSIGN", "=", 5, 16, 2], ["INVALID_NUMBER", "24e", 5, 18, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('INVALID_NUMBER', '24e') at line 5, column 18.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(enum E : { A };)~~"),
     {},
     {},
     std::string(R"~~([[["ENUM", "enum", 1, 1, 1], ["IDENTIFIER", "E", 1, 6, 1], ["COLON", ":", 1, 8, 1], ["LBRACE", "{", 1, 10, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('LBRACE', '{') at line 1, column 10.\nExpected one of: \n\t* ALT_BOOL\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* DOUBLE\n\t* ENUM\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STRUCT\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main( {
    return 0;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["LBRACE", "{", 1, 11, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('LBRACE', '{') at line 1, column 11.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* IDENTIFIER\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* RPAREN\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int foo(int a, int b {
    return 0;
}

int main(void) {
    return 0;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "foo", 1, 5, 1], ["LPAREN", "(", 1, 8, 1], ["INT", "int", 1, 9, 1], ["IDENTIFIER", "a", 1, 13, 1], ["COMMA", ",", 1, 14, 1], ["INT", "int", 1, 16, 1], ["IDENTIFIER", "b", 1, 20, 1], ["LBRACE", "{", 1, 22, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('LBRACE', '{') at line 1, column 22.\nExpected one of: \n\t* COMMA\n\t* RPAREN\n"]])~~")},
    {std::string(R"~~(int main(void) {
    switch {
        return 0;
    }
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["SWITCH", "switch", 2, 5, 2], ["LBRACE", "{", 2, 12, 3]], ["syntax", "UnexpectedToken", "Unexpected token Token('LBRACE', '{') at line 2, column 12.\nExpected one of: \n\t* LPAREN\n"]])~~")},
    {std::string(R"~~(int main(void) {
    int a;
    return 1 ? { a = 2 } : a = 4;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 2, 5, 2], ["IDENTIFIER", "a", 2, 9, 2], ["SEMICOLON", ";", 2, 10, 2], ["RETURN", "return", 3, 5, 2], ["INTEGER_CONSTANT", "1", 3, 12, 2], ["QUESTION", "?", 3, 14, 2], ["LBRACE", "{", 3, 16, 3]], ["syntax", "UnexpectedToken", "Unexpected token Token('LBRACE', '{') at line 3, column 16.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return <= 2;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["LE", "<=", 2, 12, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('LE', '<=') at line 2, column 12.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SEMICOLON\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void) {

    return long 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 3, 5, 2], ["LONG", "long", 3, 12, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('LONG', 'long') at line 3, column 12.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SEMICOLON\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(struct s {
    int a;
};

int main(void) {
    struct s x;


    return x.1l;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["INT", "int", 2, 5, 1], ["IDENTIFIER", "a", 2, 9, 1], ["SEMICOLON", ";", 2, 10, 1], ["RBRACE", "}", 3, 1, 1], ["SEMICOLON", ";", 3, 2, 1], ["INT", "int", 5, 1, 1], ["IDENTIFIER", "main", 5, 5, 1], ["LPAREN", "(", 5, 9, 1], ["VOID", "void", 5, 10, 1], ["RPAREN", ")", 5, 14, 1], ["LBRACE", "{", 5, 16, 2], ["STRUCT", "struct", 6, 5, 2], ["IDENTIFIER", "s", 6, 12, 2], ["IDENTIFIER", "x", 6, 14, 2], ["SEMICOLON", ";", 6, 15, 2], ["RETURN", "return", 9, 5, 2], ["IDENTIFIER", "x", 9, 12, 2], ["LONG_DOUBLE_CONSTANT", ".1l", 9, 13, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('LONG_DOUBLE_CONSTANT', '.1l') at line 9, column 13.\nExpected one of: \n\t* AMP\n\t* AMP_ASSIGN\n\t* ANDAND\n\t* ARROW\n\t* ASSIGN\n\t* CARET\n\t* CARET_ASSIGN\n\t* COMMA\n\t* DOT\n\t* EQ\n\t* GE\n\t* GT\n\t* LBRACKET\n\t* LE\n\t* LPAREN\n\t* LSHIFT\n\t* LSHIFT_ASSIGN\n\t* LT\n\t* MINUS\n\t* MINUSMINUS\n\t* MINUS_ASSIGN\n\t* NE\n\t* OROR\n\t* PERCENT\n\t* PERCENT_ASSIGN\n\t* PIPE\n\t* PIPE_ASSIGN\n\t* PLUS\n\t* PLUSPLUS\n\t* PLUS_ASSIGN\n\t* QUESTION\n\t* RSHIFT\n\t* RSHIFT_ASSIGN\n\t* SEMICOLON\n\t* SLASH\n\t* SLASH_ASSIGN\n\t* STAR\n\t* STAR_ASSIGN\n"]])~~")},
    {std::string(R"~~(int main(void) {

    int 10l;
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 3, 5, 2], ["LONG_INTEGER", "10l", 3, 9, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('LONG_INTEGER', '10l') at line 3, column 9.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* IDENTIFIER\n\t* INT\n\t* LONG\n\t* LPAREN\n\t* REGISTER\n\t* RESTRICT\n\t* SEMICOLON\n\t* SHORT\n\t* SIGNED\n\t* STAR\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int foo((void));
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "foo", 1, 5, 1], ["LPAREN", "(", 1, 8, 1], ["LPAREN", "(", 1, 9, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('LPAREN', '(') at line 1, column 9.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* IDENTIFIER\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* RPAREN\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void) {
    goto(a);
a:
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["GOTO", "goto", 2, 5, 2], ["LPAREN", "(", 2, 9, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('LPAREN', '(') at line 2, column 9.\nExpected one of: \n\t* IDENTIFIER\n"]])~~")},
    {std::string(R"~~(

int foo((void));
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 3, 1, 1], ["IDENTIFIER", "foo", 3, 5, 1], ["LPAREN", "(", 3, 8, 1], ["LPAREN", "(", 3, 9, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('LPAREN', '(') at line 3, column 9.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* IDENTIFIER\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* RPAREN\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(struct s {
    int y;
};

int main(void) {

    union(s) var;

    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["INT", "int", 2, 5, 1], ["IDENTIFIER", "y", 2, 9, 1], ["SEMICOLON", ";", 2, 10, 1], ["RBRACE", "}", 3, 1, 1], ["SEMICOLON", ";", 3, 2, 1], ["INT", "int", 5, 1, 1], ["IDENTIFIER", "main", 5, 5, 1], ["LPAREN", "(", 5, 9, 1], ["VOID", "void", 5, 10, 1], ["RPAREN", ")", 5, 14, 1], ["LBRACE", "{", 5, 16, 2], ["UNION", "union", 7, 5, 2], ["LPAREN", "(", 7, 10, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('LPAREN', '(') at line 7, column 10.\nExpected one of: \n\t* ATTR_OPEN\n\t* IDENTIFIER\n\t* LBRACE\n"]])~~")},
    {std::string(R"~~(struct s {
    int y;
};

int main(void) {

    struct(s) var;

    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["INT", "int", 2, 5, 1], ["IDENTIFIER", "y", 2, 9, 1], ["SEMICOLON", ";", 2, 10, 1], ["RBRACE", "}", 3, 1, 1], ["SEMICOLON", ";", 3, 2, 1], ["INT", "int", 5, 1, 1], ["IDENTIFIER", "main", 5, 5, 1], ["LPAREN", "(", 5, 9, 1], ["VOID", "void", 5, 10, 1], ["RPAREN", ")", 5, 14, 1], ["LBRACE", "{", 5, 16, 2], ["STRUCT", "struct", 7, 5, 2], ["LPAREN", "(", 7, 11, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('LPAREN', '(') at line 7, column 11.\nExpected one of: \n\t* ATTR_OPEN\n\t* IDENTIFIER\n\t* LBRACE\n"]])~~")},
    {std::string(R"~~(struct s {
    int y;
};

struct s x;


int main(void) {
    return x.(y);
}
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["INT", "int", 2, 5, 1], ["IDENTIFIER", "y", 2, 9, 1], ["SEMICOLON", ";", 2, 10, 1], ["RBRACE", "}", 3, 1, 1], ["SEMICOLON", ";", 3, 2, 1], ["STRUCT", "struct", 5, 1, 1], ["IDENTIFIER", "s", 5, 8, 1], ["IDENTIFIER", "x", 5, 10, 1], ["SEMICOLON", ";", 5, 11, 1], ["INT", "int", 8, 1, 1], ["IDENTIFIER", "main", 8, 5, 1], ["LPAREN", "(", 8, 9, 1], ["VOID", "void", 8, 10, 1], ["RPAREN", ")", 8, 14, 1], ["LBRACE", "{", 8, 16, 2], ["RETURN", "return", 9, 5, 2], ["IDENTIFIER", "x", 9, 12, 2], ["DOT", ".", 9, 13, 2], ["LPAREN", "(", 9, 14, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('LPAREN', '(') at line 9, column 14.\nExpected one of: \n\t* IDENTIFIER\n"]])~~")},
    {std::string(R"~~(int main(void) {


    return 1 | | 2;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 4, 5, 2], ["INTEGER_CONSTANT", "1", 4, 12, 2], ["PIPE", "|", 4, 14, 2], ["PIPE", "|", 4, 16, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('PIPE', '|') at line 4, column 16.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void) {
    int a++;
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 2, 5, 2], ["IDENTIFIER", "a", 2, 9, 2], ["PLUSPLUS", "++", 2, 10, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('PLUSPLUS', '++') at line 2, column 10.\nExpected one of: \n\t* ASSIGN\n\t* ATTR_OPEN\n\t* COMMA\n\t* LBRACE\n\t* LBRACKET\n\t* LPAREN\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(int main(void) {
    int a += 0;
    return a;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 2, 5, 2], ["IDENTIFIER", "a", 2, 9, 2], ["PLUS_ASSIGN", "+=", 2, 11, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('PLUS_ASSIGN', '+=') at line 2, column 11.\nExpected one of: \n\t* ASSIGN\n\t* ATTR_OPEN\n\t* COMMA\n\t* LBRACE\n\t* LBRACKET\n\t* LPAREN\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(int main(void) {
    for (int i += 1; i < 10; i += 1) {
        return 0;
    }
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["FOR", "for", 2, 5, 3], ["LPAREN", "(", 2, 9, 3], ["INT", "int", 2, 10, 3], ["IDENTIFIER", "i", 2, 14, 3], ["PLUS_ASSIGN", "+=", 2, 16, 3]], ["syntax", "UnexpectedToken", "Unexpected token Token('PLUS_ASSIGN', '+=') at line 2, column 16.\nExpected one of: \n\t* ASSIGN\n\t* ATTR_OPEN\n\t* COMMA\n\t* LBRACKET\n\t* LPAREN\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(union s {};

int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["UNION", "union", 1, 1, 1], ["IDENTIFIER", "s", 1, 7, 1], ["LBRACE", "{", 1, 9, 1], ["RBRACE", "}", 1, 10, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('RBRACE', '}') at line 1, column 10.\nExpected one of: \n\t* ALT_BOOL\n\t* ATTR_OPEN\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* DOUBLE\n\t* ENUM\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STRUCT\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(struct s {};

int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["RBRACE", "}", 1, 11, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('RBRACE', '}') at line 1, column 11.\nExpected one of: \n\t* ALT_BOOL\n\t* ATTR_OPEN\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* DOUBLE\n\t* ENUM\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STRUCT\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int foo(int x, int y} { return x + y; }

int main(void) { return 0;}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "foo", 1, 5, 1], ["LPAREN", "(", 1, 8, 1], ["INT", "int", 1, 9, 1], ["IDENTIFIER", "x", 1, 13, 1], ["COMMA", ",", 1, 14, 1], ["INT", "int", 1, 16, 1], ["IDENTIFIER", "y", 1, 20, 1], ["RBRACE", "}", 1, 21, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('RBRACE', '}') at line 1, column 21.\nExpected one of: \n\t* ATTR_OPEN\n\t* COMMA\n\t* LBRACKET\n\t* LPAREN\n\t* RPAREN\n"]])~~")},
    {std::string(R"~~(int main(void) { switch (1) { default: } })~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["SWITCH", "switch", 1, 18, 2], ["LPAREN", "(", 1, 25, 2], ["INTEGER_CONSTANT", "1", 1, 26, 2], ["RPAREN", ")", 1, 27, 2], ["LBRACE", "{", 1, 29, 3], ["DEFAULT", "default", 1, 31, 3], ["COLON", ":", 1, 38, 3], ["RBRACE", "}", 1, 40, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('RBRACE', '}') at line 1, column 40.\nExpected one of: \n\t* AMP\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* BREAK\n\t* CASE\n\t* CHAR_CONSTANT\n\t* CONTINUE\n\t* DEFAULT\n\t* DO\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* FOR\n\t* GOTO\n\t* IDENTIFIER\n\t* IF\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* RETURN\n\t* SEMICOLON\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* SWITCH\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\t* WHILE\n"]])~~")},
    {std::string(R"~~(struct s {
    int a
};

int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["INT", "int", 2, 5, 1], ["IDENTIFIER", "a", 2, 9, 1], ["RBRACE", "}", 3, 1, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('RBRACE', '}') at line 3, column 1.\nExpected one of: \n\t* ATTR_OPEN\n\t* COMMA\n\t* LBRACKET\n\t* LPAREN\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(int main (void) {
    return 0
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 10, 1], ["VOID", "void", 1, 11, 1], ["RPAREN", ")", 1, 15, 1], ["LBRACE", "{", 1, 17, 2], ["RETURN", "return", 2, 5, 2], ["INTEGER_CONSTANT", "0", 2, 12, 2], ["RBRACE", "}", 3, 1, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('RBRACE', '}') at line 3, column 1.\nExpected one of: \n\t* COMMA\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(

union s {};

int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["UNION", "union", 3, 1, 1], ["IDENTIFIER", "s", 3, 7, 1], ["LBRACE", "{", 3, 9, 1], ["RBRACE", "}", 3, 10, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('RBRACE', '}') at line 3, column 10.\nExpected one of: \n\t* ALT_BOOL\n\t* ATTR_OPEN\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* DOUBLE\n\t* ENUM\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STRUCT\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(

struct s {};

int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 3, 1, 1], ["IDENTIFIER", "s", 3, 8, 1], ["LBRACE", "{", 3, 10, 1], ["RBRACE", "}", 3, 11, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('RBRACE', '}') at line 3, column 11.\nExpected one of: \n\t* ALT_BOOL\n\t* ATTR_OPEN\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* DOUBLE\n\t* ENUM\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STRUCT\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void) {

    foo:
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["IDENTIFIER", "foo", 3, 5, 2], ["COLON", ":", 3, 8, 2], ["RBRACE", "}", 4, 1, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('RBRACE', '}') at line 4, column 1.\nExpected one of: \n\t* AMP\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* BREAK\n\t* CASE\n\t* CHAR_CONSTANT\n\t* CONTINUE\n\t* DEFAULT\n\t* DO\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* FOR\n\t* GOTO\n\t* IDENTIFIER\n\t* IF\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* RETURN\n\t* SEMICOLON\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* SWITCH\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\t* WHILE\n"]])~~")},
    {std::string(R"~~(int main(void)
{
    return !10
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["RETURN", "return", 3, 5, 2], ["BANG", "!", 3, 12, 2], ["INTEGER_CONSTANT", "10", 3, 13, 2], ["RBRACE", "}", 4, 1, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('RBRACE', '}') at line 4, column 1.\nExpected one of: \n\t* COMMA\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(


int foo(int x, int y} { return x + y; }

int main(void) { return 0;}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 4, 1, 1], ["IDENTIFIER", "foo", 4, 5, 1], ["LPAREN", "(", 4, 8, 1], ["INT", "int", 4, 9, 1], ["IDENTIFIER", "x", 4, 13, 1], ["COMMA", ",", 4, 14, 1], ["INT", "int", 4, 16, 1], ["IDENTIFIER", "y", 4, 20, 1], ["RBRACE", "}", 4, 21, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('RBRACE', '}') at line 4, column 21.\nExpected one of: \n\t* ATTR_OPEN\n\t* COMMA\n\t* LBRACKET\n\t* LPAREN\n\t* RPAREN\n"]])~~")},
    {std::string(R"~~(int foo(int x, int y) {
    return x + y;
}

int main(void) { return foo(1, 2};}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "foo", 1, 5, 1], ["LPAREN", "(", 1, 8, 1], ["INT", "int", 1, 9, 1], ["IDENTIFIER", "x", 1, 13, 1], ["COMMA", ",", 1, 14, 1], ["INT", "int", 1, 16, 1], ["IDENTIFIER", "y", 1, 20, 1], ["RPAREN", ")", 1, 21, 1], ["LBRACE", "{", 1, 23, 2], ["RETURN", "return", 2, 5, 2], ["IDENTIFIER", "x", 2, 12, 2], ["PLUS", "+", 2, 14, 2], ["IDENTIFIER", "y", 2, 16, 2], ["SEMICOLON", ";", 2, 17, 2], ["RBRACE", "}", 3, 1, 1], ["INT", "int", 5, 1, 1], ["IDENTIFIER", "main", 5, 5, 1], ["LPAREN", "(", 5, 9, 1], ["VOID", "void", 5, 10, 1], ["RPAREN", ")", 5, 14, 1], ["LBRACE", "{", 5, 16, 2], ["RETURN", "return", 5, 18, 2], ["IDENTIFIER", "foo", 5, 25, 2], ["LPAREN", "(", 5, 28, 2], ["INTEGER_CONSTANT", "1", 5, 29, 2], ["COMMA", ",", 5, 30, 2], ["INTEGER_CONSTANT", "2", 5, 32, 2], ["RBRACE", "}", 5, 33, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('RBRACE', '}') at line 5, column 33.\nExpected one of: \n\t* COMMA\n\t* RPAREN\n"]])~~")},
    {std::string(R"~~(int main(void) {
    int a = 4;
    {
        a = 5;
        return a
    }
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 2, 5, 2], ["IDENTIFIER", "a", 2, 9, 2], ["ASSIGN", "=", 2, 11, 2], ["INTEGER_CONSTANT", "4", 2, 13, 2], ["SEMICOLON", ";", 2, 14, 2], ["LBRACE", "{", 3, 5, 3], ["IDENTIFIER", "a", 4, 9, 3], ["ASSIGN", "=", 4, 11, 3], ["INTEGER_CONSTANT", "5", 4, 13, 3], ["SEMICOLON", ";", 4, 14, 3], ["RETURN", "return", 5, 9, 3], ["IDENTIFIER", "a", 5, 16, 3], ["RBRACE", "}", 6, 5, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('RBRACE', '}') at line 6, column 5.\nExpected one of: \n\t* COMMA\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(


int foo(int x, int y) {
    return x + y;
}

int main(void) { return foo(1, 2};}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 4, 1, 1], ["IDENTIFIER", "foo", 4, 5, 1], ["LPAREN", "(", 4, 8, 1], ["INT", "int", 4, 9, 1], ["IDENTIFIER", "x", 4, 13, 1], ["COMMA", ",", 4, 14, 1], ["INT", "int", 4, 16, 1], ["IDENTIFIER", "y", 4, 20, 1], ["RPAREN", ")", 4, 21, 1], ["LBRACE", "{", 4, 23, 2], ["RETURN", "return", 5, 5, 2], ["IDENTIFIER", "x", 5, 12, 2], ["PLUS", "+", 5, 14, 2], ["IDENTIFIER", "y", 5, 16, 2], ["SEMICOLON", ";", 5, 17, 2], ["RBRACE", "}", 6, 1, 1], ["INT", "int", 8, 1, 1], ["IDENTIFIER", "main", 8, 5, 1], ["LPAREN", "(", 8, 9, 1], ["VOID", "void", 8, 10, 1], ["RPAREN", ")", 8, 14, 1], ["LBRACE", "{", 8, 16, 2], ["RETURN", "return", 8, 18, 2], ["IDENTIFIER", "foo", 8, 25, 2], ["LPAREN", "(", 8, 28, 2], ["INTEGER_CONSTANT", "1", 8, 29, 2], ["COMMA", ",", 8, 30, 2], ["INTEGER_CONSTANT", "2", 8, 32, 2], ["RBRACE", "}", 8, 33, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('RBRACE', '}') at line 8, column 33.\nExpected one of: \n\t* COMMA\n\t* RPAREN\n"]])~~")},
    {std::string(R"~~(int main(void) {
    int return = 4;
    return return + 1;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 2, 5, 2], ["RETURN", "return", 2, 9, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('RETURN', 'return') at line 2, column 9.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* IDENTIFIER\n\t* INT\n\t* LONG\n\t* LPAREN\n\t* REGISTER\n\t* RESTRICT\n\t* SEMICOLON\n\t* SHORT\n\t* SIGNED\n\t* STAR\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(struct s {
    int return;
}

int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["INT", "int", 2, 5, 1], ["RETURN", "return", 2, 9, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('RETURN', 'return') at line 2, column 9.\nExpected one of: \n\t* ALT_BOOL\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* DOUBLE\n\t* ENUM\n\t* FLOAT\n\t* IDENTIFIER\n\t* INT\n\t* LONG\n\t* LPAREN\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STAR\n\t* STRUCT\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(

struct s {
    int return;
}

int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 3, 1, 1], ["IDENTIFIER", "s", 3, 8, 1], ["LBRACE", "{", 3, 10, 1], ["INT", "int", 4, 5, 1], ["RETURN", "return", 4, 9, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('RETURN', 'return') at line 4, column 9.\nExpected one of: \n\t* ALT_BOOL\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* DOUBLE\n\t* ENUM\n\t* FLOAT\n\t* IDENTIFIER\n\t* INT\n\t* LONG\n\t* LPAREN\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STAR\n\t* STRUCT\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void) {
    if(0){
        return 1;
    }}
    return 2;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["IF", "if", 2, 5, 2], ["LPAREN", "(", 2, 7, 2], ["INTEGER_CONSTANT", "0", 2, 8, 2], ["RPAREN", ")", 2, 9, 2], ["LBRACE", "{", 2, 10, 3], ["RETURN", "return", 3, 9, 3], ["INTEGER_CONSTANT", "1", 3, 16, 3], ["SEMICOLON", ";", 3, 17, 3], ["RBRACE", "}", 4, 5, 2], ["RBRACE", "}", 4, 6, 1], ["RETURN", "return", 5, 5, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('RETURN', 'return') at line 5, column 5.\nExpected one of: \n\t* $END\n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void) {
    do {
        4;
    } while(1)
    return 0;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["DO", "do", 2, 5, 2], ["LBRACE", "{", 2, 8, 3], ["INTEGER_CONSTANT", "4", 3, 9, 3], ["SEMICOLON", ";", 3, 10, 3], ["RBRACE", "}", 4, 5, 2], ["WHILE", "while", 4, 7, 2], ["LPAREN", "(", 4, 12, 2], ["INTEGER_CONSTANT", "1", 4, 13, 2], ["RPAREN", ")", 4, 14, 2], ["RETURN", "return", 5, 5, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('RETURN', 'return') at line 5, column 5.\nExpected one of: \n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(int main )( {
    return 0;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["RPAREN", ")", 1, 10, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('RPAREN', ')') at line 1, column 10.\nExpected one of: \n\t* ASSIGN\n\t* COMMA\n\t* LBRACE\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(int foo(int a,) {
    return a + 1;
}

int main(void) {
    return foo(4);
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "foo", 1, 5, 1], ["LPAREN", "(", 1, 8, 1], ["INT", "int", 1, 9, 1], ["IDENTIFIER", "a", 1, 13, 1], ["COMMA", ",", 1, 14, 1], ["RPAREN", ")", 1, 15, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('RPAREN', ')') at line 1, column 15.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ELLIPSIS\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return (-)3;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["LPAREN", "(", 2, 12, 2], ["MINUS", "-", 2, 13, 2], ["RPAREN", ")", 2, 14, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('RPAREN', ')') at line 2, column 14.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(
int foo(int a,) {
    return a + 1;
}

int main(void) {
    return foo(4);
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 2, 1, 1], ["IDENTIFIER", "foo", 2, 5, 1], ["LPAREN", "(", 2, 8, 1], ["INT", "int", 2, 9, 1], ["IDENTIFIER", "a", 2, 13, 1], ["COMMA", ",", 2, 14, 1], ["RPAREN", ")", 2, 15, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('RPAREN', ')') at line 2, column 15.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ELLIPSIS\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return 1 + 2);
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["INTEGER_CONSTANT", "1", 2, 12, 2], ["PLUS", "+", 2, 14, 2], ["INTEGER_CONSTANT", "2", 2, 16, 2], ["RPAREN", ")", 2, 17, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('RPAREN', ')') at line 2, column 17.\nExpected one of: \n\t* COMMA\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(int main(void) {
    for (int i = 0;)
        ;
    return 0;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["FOR", "for", 2, 5, 3], ["LPAREN", "(", 2, 9, 3], ["INT", "int", 2, 10, 3], ["IDENTIFIER", "i", 2, 14, 3], ["ASSIGN", "=", 2, 16, 3], ["INTEGER_CONSTANT", "0", 2, 18, 3], ["SEMICOLON", ";", 2, 19, 3], ["RPAREN", ")", 2, 20, 3]], ["syntax", "UnexpectedToken", "Unexpected token Token('RPAREN', ')') at line 2, column 20.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SEMICOLON\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void) {
    for (2 + 2 == 4)
        ;
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["FOR", "for", 2, 5, 3], ["LPAREN", "(", 2, 9, 3], ["INTEGER_CONSTANT", "2", 2, 10, 3], ["PLUS", "+", 2, 12, 3], ["INTEGER_CONSTANT", "2", 2, 14, 3], ["EQ", "==", 2, 16, 3], ["INTEGER_CONSTANT", "4", 2, 19, 3], ["RPAREN", ")", 2, 20, 3]], ["syntax", "UnexpectedToken", "Unexpected token Token('RPAREN', ')') at line 2, column 20.\nExpected one of: \n\t* COMMA\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(int main(void) {
    for (int i = 2; ))
        int a = 0;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["FOR", "for", 2, 5, 3], ["LPAREN", "(", 2, 9, 3], ["INT", "int", 2, 10, 3], ["IDENTIFIER", "i", 2, 14, 3], ["ASSIGN", "=", 2, 16, 3], ["INTEGER_CONSTANT", "2", 2, 18, 3], ["SEMICOLON", ";", 2, 19, 3], ["RPAREN", ")", 2, 21, 3]], ["syntax", "UnexpectedToken", "Unexpected token Token('RPAREN', ')') at line 2, column 21.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SEMICOLON\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void) {
    for (int i = 0; i < 10)
        ;
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["FOR", "for", 2, 5, 3], ["LPAREN", "(", 2, 9, 3], ["INT", "int", 2, 10, 3], ["IDENTIFIER", "i", 2, 14, 3], ["ASSIGN", "=", 2, 16, 3], ["INTEGER_CONSTANT", "0", 2, 18, 3], ["SEMICOLON", ";", 2, 19, 3], ["IDENTIFIER", "i", 2, 21, 3], ["LT", "<", 2, 23, 3], ["INTEGER_CONSTANT", "10", 2, 25, 3], ["RPAREN", ")", 2, 27, 3]], ["syntax", "UnexpectedToken", "Unexpected token Token('RPAREN', ')') at line 2, column 27.\nExpected one of: \n\t* COMMA\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(int main(void) {

    return () 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 3, 5, 2], ["LPAREN", "(", 3, 12, 2], ["RPAREN", ")", 3, 13, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('RPAREN', ')') at line 3, column 13.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* AMP\n\t* AUTO\n\t* BANG\n\t* BITINT\n\t* BITINT_INTEGER\n\t* BOOL\n\t* CHAR\n\t* CHAR_CONSTANT\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* DOUBLE_CONSTANT\n\t* ENUM\n\t* EXTERN\n\t* FALSE\n\t* FLOAT\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INT\n\t* INTEGER_CONSTANT\n\t* LONG\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* REGISTER\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* SIZEOF\n\t* STAR\n\t* STATIC\n\t* STRING_LITERAL\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TILDE\n\t* TRUE\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void)
{
    return (3));
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["RETURN", "return", 3, 5, 2], ["LPAREN", "(", 3, 12, 2], ["INTEGER_CONSTANT", "3", 3, 13, 2], ["RPAREN", ")", 3, 14, 2], ["RPAREN", ")", 3, 15, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('RPAREN', ')') at line 3, column 15.\nExpected one of: \n\t* COMMA\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(int main(void) {
    do
        1;
    while ();
    return 0;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["DO", "do", 2, 5, 2], ["INTEGER_CONSTANT", "1", 3, 9, 2], ["SEMICOLON", ";", 3, 10, 2], ["WHILE", "while", 4, 5, 2], ["LPAREN", "(", 4, 11, 2], ["RPAREN", ")", 4, 12, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('RPAREN', ')') at line 4, column 12.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void) {




    int (*)* y;
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 6, 5, 2], ["LPAREN", "(", 6, 9, 2], ["STAR", "*", 6, 10, 2], ["RPAREN", ")", 6, 11, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('RPAREN', ')') at line 6, column 11.\nExpected one of: \n\t* IDENTIFIER\n\t* LPAREN\n"]])~~")},
    {std::string(R"~~(int foo(int a, int b, int c) {
    return a + b + c;
}

int main(void) {

    return foo(1, 2, 3,);
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "foo", 1, 5, 1], ["LPAREN", "(", 1, 8, 1], ["INT", "int", 1, 9, 1], ["IDENTIFIER", "a", 1, 13, 1], ["COMMA", ",", 1, 14, 1], ["INT", "int", 1, 16, 1], ["IDENTIFIER", "b", 1, 20, 1], ["COMMA", ",", 1, 21, 1], ["INT", "int", 1, 23, 1], ["IDENTIFIER", "c", 1, 27, 1], ["RPAREN", ")", 1, 28, 1], ["LBRACE", "{", 1, 30, 2], ["RETURN", "return", 2, 5, 2], ["IDENTIFIER", "a", 2, 12, 2], ["PLUS", "+", 2, 14, 2], ["IDENTIFIER", "b", 2, 16, 2], ["PLUS", "+", 2, 18, 2], ["IDENTIFIER", "c", 2, 20, 2], ["SEMICOLON", ";", 2, 21, 2], ["RBRACE", "}", 3, 1, 1], ["INT", "int", 5, 1, 1], ["IDENTIFIER", "main", 5, 5, 1], ["LPAREN", "(", 5, 9, 1], ["VOID", "void", 5, 10, 1], ["RPAREN", ")", 5, 14, 1], ["LBRACE", "{", 5, 16, 2], ["RETURN", "return", 7, 5, 2], ["IDENTIFIER", "foo", 7, 12, 2], ["LPAREN", "(", 7, 15, 2], ["INTEGER_CONSTANT", "1", 7, 16, 2], ["COMMA", ",", 7, 17, 2], ["INTEGER_CONSTANT", "2", 7, 19, 2], ["COMMA", ",", 7, 20, 2], ["INTEGER_CONSTANT", "3", 7, 22, 2], ["COMMA", ",", 7, 23, 2], ["RPAREN", ")", 7, 24, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('RPAREN', ')') at line 7, column 24.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return ~;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["TILDE", "~", 2, 12, 2], ["SEMICOLON", ";", 2, 13, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 2, column 13.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return 4-;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["INTEGER_CONSTANT", "4", 2, 12, 2], ["MINUS", "-", 2, 13, 2], ["SEMICOLON", ";", 2, 14, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 2, column 14.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(union u {
    int struct;
};
)~~"),
     {},
     {},
     std::string(R"~~([[["UNION", "union", 1, 1, 1], ["IDENTIFIER", "u", 1, 7, 1], ["LBRACE", "{", 1, 9, 1], ["INT", "int", 2, 5, 1], ["STRUCT", "struct", 2, 9, 1], ["SEMICOLON", ";", 2, 15, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 2, column 15.\nExpected one of: \n\t* ATTR_OPEN\n\t* IDENTIFIER\n\t* LBRACE\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return 1 + ;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["INTEGER_CONSTANT", "1", 2, 12, 2], ["PLUS", "+", 2, 14, 2], ["SEMICOLON", ";", 2, 16, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 2, column 16.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return 1 ? 2;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["INTEGER_CONSTANT", "1", 2, 12, 2], ["QUESTION", "?", 2, 14, 2], ["INTEGER_CONSTANT", "2", 2, 16, 2], ["SEMICOLON", ";", 2, 17, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 2, column 17.\nExpected one of: \n\t* COLON\n\t* COMMA\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return 2 && ~;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["INTEGER_CONSTANT", "2", 2, 12, 2], ["ANDAND", "&&", 2, 14, 2], ["TILDE", "~", 2, 17, 2], ["SEMICOLON", ";", 2, 18, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 2, column 18.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return 1 + (2;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["INTEGER_CONSTANT", "1", 2, 12, 2], ["PLUS", "+", 2, 14, 2], ["LPAREN", "(", 2, 16, 2], ["INTEGER_CONSTANT", "2", 2, 17, 2], ["SEMICOLON", ";", 2, 18, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 2, column 18.\nExpected one of: \n\t* COMMA\n\t* RPAREN\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return 1 ? 2 ? 3 : 4;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["INTEGER_CONSTANT", "1", 2, 12, 2], ["QUESTION", "?", 2, 14, 2], ["INTEGER_CONSTANT", "2", 2, 16, 2], ["QUESTION", "?", 2, 18, 2], ["INTEGER_CONSTANT", "3", 2, 20, 2], ["COLON", ":", 2, 22, 2], ["INTEGER_CONSTANT", "4", 2, 24, 2], ["SEMICOLON", ";", 2, 25, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 2, column 25.\nExpected one of: \n\t* COLON\n\t* COMMA\n"]])~~")},
    {std::string(R"~~(int main(void) {
    for (int i = 0; i < 10; i = i + 1; )
        ;
    return 0;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["FOR", "for", 2, 5, 3], ["LPAREN", "(", 2, 9, 3], ["INT", "int", 2, 10, 3], ["IDENTIFIER", "i", 2, 14, 3], ["ASSIGN", "=", 2, 16, 3], ["INTEGER_CONSTANT", "0", 2, 18, 3], ["SEMICOLON", ";", 2, 19, 3], ["IDENTIFIER", "i", 2, 21, 3], ["LT", "<", 2, 23, 3], ["INTEGER_CONSTANT", "10", 2, 25, 3], ["SEMICOLON", ";", 2, 27, 3], ["IDENTIFIER", "i", 2, 29, 3], ["ASSIGN", "=", 2, 31, 3], ["IDENTIFIER", "i", 2, 33, 3], ["PLUS", "+", 2, 35, 3], ["INTEGER_CONSTANT", "1", 2, 37, 3], ["SEMICOLON", ";", 2, 38, 3]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 2, column 38.\nExpected one of: \n\t* COMMA\n\t* RPAREN\n"]])~~")},
    {std::string(R"~~(struct s {
    ;
    int a;
};
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["SEMICOLON", ";", 2, 5, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 2, column 5.\nExpected one of: \n\t* ALT_BOOL\n\t* ATTR_OPEN\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* DOUBLE\n\t* ENUM\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STRUCT\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(union u {
    int;





};
)~~"),
     {},
     {},
     std::string(R"~~([[["UNION", "union", 1, 1, 1], ["IDENTIFIER", "u", 1, 7, 1], ["LBRACE", "{", 1, 9, 1], ["INT", "int", 2, 5, 1], ["SEMICOLON", ";", 2, 8, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 2, column 8.\nExpected one of: \n\t* ALT_BOOL\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* DOUBLE\n\t* ENUM\n\t* FLOAT\n\t* IDENTIFIER\n\t* INT\n\t* LONG\n\t* LPAREN\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STAR\n\t* STRUCT\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void) {
    goto;
lbl:
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["GOTO", "goto", 2, 5, 2], ["SEMICOLON", ";", 2, 9, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 2, column 9.\nExpected one of: \n\t* IDENTIFIER\n"]])~~")},
    {std::string(R"~~(int main(void) {
    int a = 0;
    a + +;
    return a;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 2, 5, 2], ["IDENTIFIER", "a", 2, 9, 2], ["ASSIGN", "=", 2, 11, 2], ["INTEGER_CONSTANT", "0", 2, 13, 2], ["SEMICOLON", ";", 2, 14, 2], ["IDENTIFIER", "a", 3, 5, 2], ["PLUS", "+", 3, 7, 2], ["PLUS", "+", 3, 9, 2], ["SEMICOLON", ";", 3, 10, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 3, column 10.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void)
{
    return -~;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["RETURN", "return", 3, 5, 2], ["MINUS", "-", 3, 12, 2], ["TILDE", "~", 3, 13, 2], ["SEMICOLON", ";", 3, 14, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 3, column 14.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void)
{
    return (1;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["RETURN", "return", 3, 5, 2], ["LPAREN", "(", 3, 12, 2], ["INTEGER_CONSTANT", "1", 3, 13, 2], ["SEMICOLON", ";", 3, 14, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 3, column 14.\nExpected one of: \n\t* COMMA\n\t* RPAREN\n"]])~~")},
    {std::string(R"~~(int main(void)
{
    return 10 <= !;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["RETURN", "return", 3, 5, 2], ["INTEGER_CONSTANT", "10", 3, 12, 2], ["LE", "<=", 3, 15, 2], ["BANG", "!", 3, 18, 2], ["SEMICOLON", ";", 3, 19, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 3, column 19.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(struct s {
    int a;
    ;
};
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["INT", "int", 2, 5, 1], ["IDENTIFIER", "a", 2, 9, 1], ["SEMICOLON", ";", 2, 10, 1], ["SEMICOLON", ";", 3, 5, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 3, column 5.\nExpected one of: \n\t* ALT_BOOL\n\t* ATTR_OPEN\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* DOUBLE\n\t* ENUM\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* RBRACE\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STRUCT\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(

union u {
    int struct;
};
)~~"),
     {},
     {},
     std::string(R"~~([[["UNION", "union", 3, 1, 1], ["IDENTIFIER", "u", 3, 7, 1], ["LBRACE", "{", 3, 9, 1], ["INT", "int", 4, 5, 1], ["STRUCT", "struct", 4, 9, 1], ["SEMICOLON", ";", 4, 15, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 4, column 15.\nExpected one of: \n\t* ATTR_OPEN\n\t* IDENTIFIER\n\t* LBRACE\n"]])~~")},
    {std::string(R"~~(
union u {
    int a;
    ;
};

int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["UNION", "union", 2, 1, 1], ["IDENTIFIER", "u", 2, 7, 1], ["LBRACE", "{", 2, 9, 1], ["INT", "int", 3, 5, 1], ["IDENTIFIER", "a", 3, 9, 1], ["SEMICOLON", ";", 3, 10, 1], ["SEMICOLON", ";", 4, 5, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 4, column 5.\nExpected one of: \n\t* ALT_BOOL\n\t* ATTR_OPEN\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* DOUBLE\n\t* ENUM\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* RBRACE\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STRUCT\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void) {
    do {
        int a;
    }; while(1);
    return 0;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["DO", "do", 2, 5, 2], ["LBRACE", "{", 2, 8, 3], ["INT", "int", 3, 9, 3], ["IDENTIFIER", "a", 3, 13, 3], ["SEMICOLON", ";", 3, 14, 3], ["RBRACE", "}", 4, 5, 2], ["SEMICOLON", ";", 4, 6, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 4, column 6.\nExpected one of: \n\t* WHILE\n"]])~~")},
    {std::string(R"~~(int main(void) {

    int x = 10;

    return x ? 1 = 2;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 3, 5, 2], ["IDENTIFIER", "x", 3, 9, 2], ["ASSIGN", "=", 3, 11, 2], ["INTEGER_CONSTANT", "10", 3, 13, 2], ["SEMICOLON", ";", 3, 15, 2], ["RETURN", "return", 5, 5, 2], ["IDENTIFIER", "x", 5, 12, 2], ["QUESTION", "?", 5, 14, 2], ["INTEGER_CONSTANT", "1", 5, 16, 2], ["ASSIGN", "=", 5, 18, 2], ["INTEGER_CONSTANT", "2", 5, 20, 2], ["SEMICOLON", ";", 5, 21, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 5, column 21.\nExpected one of: \n\t* COLON\n\t* COMMA\n"]])~~")},
    {std::string(R"~~(


int main(void) {
    int a = 0;
    a + +;
    return a;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 4, 1, 1], ["IDENTIFIER", "main", 4, 5, 1], ["LPAREN", "(", 4, 9, 1], ["VOID", "void", 4, 10, 1], ["RPAREN", ")", 4, 14, 1], ["LBRACE", "{", 4, 16, 2], ["INT", "int", 5, 5, 2], ["IDENTIFIER", "a", 5, 9, 2], ["ASSIGN", "=", 5, 11, 2], ["INTEGER_CONSTANT", "0", 5, 13, 2], ["SEMICOLON", ";", 5, 14, 2], ["IDENTIFIER", "a", 6, 5, 2], ["PLUS", "+", 6, 7, 2], ["PLUS", "+", 6, 9, 2], ["SEMICOLON", ";", 6, 10, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 6, column 10.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(struct s {
    int y;
};

int main(void) {
    struct s *ptr = 0;
    return ptr->;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["INT", "int", 2, 5, 1], ["IDENTIFIER", "y", 2, 9, 1], ["SEMICOLON", ";", 2, 10, 1], ["RBRACE", "}", 3, 1, 1], ["SEMICOLON", ";", 3, 2, 1], ["INT", "int", 5, 1, 1], ["IDENTIFIER", "main", 5, 5, 1], ["LPAREN", "(", 5, 9, 1], ["VOID", "void", 5, 10, 1], ["RPAREN", ")", 5, 14, 1], ["LBRACE", "{", 5, 16, 2], ["STRUCT", "struct", 6, 5, 2], ["IDENTIFIER", "s", 6, 12, 2], ["STAR", "*", 6, 14, 2], ["IDENTIFIER", "ptr", 6, 15, 2], ["ASSIGN", "=", 6, 19, 2], ["INTEGER_CONSTANT", "0", 6, 21, 2], ["SEMICOLON", ";", 6, 22, 2], ["RETURN", "return", 7, 5, 2], ["IDENTIFIER", "ptr", 7, 12, 2], ["ARROW", "->", 7, 15, 2], ["SEMICOLON", ";", 7, 17, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SEMICOLON', ';') at line 7, column 17.\nExpected one of: \n\t* IDENTIFIER\n"]])~~")},
    {std::string(R"~~(/* Structure tag must be a single identifier */

struct x y {
    int a;
};)~~"),
     {},
     {},
     std::string(R"~~([[["SLASH", "/", 1, 1, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('SLASH', '/') at line 1, column 1.\nExpected one of: \n\t* $END\n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(
// You can't wrap a parameter list in multiple sets of parentheses
int foo((void));
)~~"),
     {},
     {},
     std::string(R"~~([[["SLASH", "/", 2, 1, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('SLASH', '/') at line 2, column 1.\nExpected one of: \n\t* $END\n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return /3;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["SLASH", "/", 2, 12, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SLASH', '/') at line 2, column 12.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SEMICOLON\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(struct s {
    int a  // structure member declaration must end with a semicolon
};

int main(void) {
    return 0;
})~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["INT", "int", 2, 5, 1], ["IDENTIFIER", "a", 2, 9, 1], ["SLASH", "/", 2, 12, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('SLASH', '/') at line 2, column 12.\nExpected one of: \n\t* ATTR_OPEN\n\t* COMMA\n\t* LBRACKET\n\t* LPAREN\n\t* SEMICOLON\n"]])~~")},
    {std::string(R"~~(int main(void) {
    return 1 * / 2;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["RETURN", "return", 2, 5, 2], ["INTEGER_CONSTANT", "1", 2, 12, 2], ["STAR", "*", 2, 14, 2], ["SLASH", "/", 2, 16, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SLASH', '/') at line 2, column 16.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void) {
    /* A cast expression must include at least one type specifier */
    return () 0;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["SLASH", "/", 2, 5, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SLASH', '/') at line 2, column 5.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* AMP\n\t* ATTR_OPEN\n\t* AUTO\n\t* BANG\n\t* BITINT\n\t* BITINT_INTEGER\n\t* BOOL\n\t* BREAK\n\t* CASE\n\t* CHAR\n\t* CHAR_CONSTANT\n\t* CONST\n\t* CONSTEXPR\n\t* CONTINUE\n\t* DEFAULT\n\t* DO\n\t* DOUBLE\n\t* DOUBLE_CONSTANT\n\t* ENUM\n\t* EXTERN\n\t* FALSE\n\t* FLOAT\n\t* FLOAT_CONSTANT\n\t* FOR\n\t* GOTO\n\t* IDENTIFIER\n\t* IF\n\t* INT\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LONG\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* RBRACE\n\t* REGISTER\n\t* RESTRICT\n\t* RETURN\n\t* SEMICOLON\n\t* SHORT\n\t* SIGNED\n\t* SIZEOF\n\t* STAR\n\t* STATIC\n\t* STRING_LITERAL\n\t* STRUCT\n\t* SWITCH\n\t* THREAD_LOCAL\n\t* TILDE\n\t* TRUE\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n\t* WHILE\n"]])~~")},
    {std::string(R"~~(struct y {
    // structure member cannot have storage class
    static int a;
};)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "y", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["SLASH", "/", 2, 5, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('SLASH', '/') at line 2, column 5.\nExpected one of: \n\t* ALT_BOOL\n\t* ATTR_OPEN\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* DOUBLE\n\t* ENUM\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STRUCT\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(struct s;

// cannot combine struct keyword with other type specifier
struct s long a;

int main(void) {
    return 0;
})~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["SEMICOLON", ";", 1, 9, 1], ["SLASH", "/", 3, 1, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('SLASH', '/') at line 3, column 1.\nExpected one of: \n\t* $END\n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(struct s {
    int a;
} // a structure declaration must end with a semicolon

int main(void) {
    return 0;
})~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["INT", "int", 2, 5, 1], ["IDENTIFIER", "a", 2, 9, 1], ["SEMICOLON", ";", 2, 10, 1], ["RBRACE", "}", 3, 1, 1], ["SLASH", "/", 3, 3, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('SLASH', '/') at line 3, column 3.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* IDENTIFIER\n\t* INT\n\t* LONG\n\t* LPAREN\n\t* REGISTER\n\t* RESTRICT\n\t* SEMICOLON\n\t* SHORT\n\t* SIGNED\n\t* STAR\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(int main(void)
{
    // can only cast to declarator types, not abstract declarators
    return (int **a)(10);
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 2, 1, 2], ["SLASH", "/", 3, 5, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SLASH', '/') at line 3, column 5.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* AMP\n\t* ATTR_OPEN\n\t* AUTO\n\t* BANG\n\t* BITINT\n\t* BITINT_INTEGER\n\t* BOOL\n\t* BREAK\n\t* CASE\n\t* CHAR\n\t* CHAR_CONSTANT\n\t* CONST\n\t* CONSTEXPR\n\t* CONTINUE\n\t* DEFAULT\n\t* DO\n\t* DOUBLE\n\t* DOUBLE_CONSTANT\n\t* ENUM\n\t* EXTERN\n\t* FALSE\n\t* FLOAT\n\t* FLOAT_CONSTANT\n\t* FOR\n\t* GOTO\n\t* IDENTIFIER\n\t* IF\n\t* INT\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LONG\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* RBRACE\n\t* REGISTER\n\t* RESTRICT\n\t* RETURN\n\t* SEMICOLON\n\t* SHORT\n\t* SIGNED\n\t* SIZEOF\n\t* STAR\n\t* STATIC\n\t* STRING_LITERAL\n\t* STRUCT\n\t* SWITCH\n\t* THREAD_LOCAL\n\t* TILDE\n\t* TRUE\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n\t* WHILE\n"]])~~")},
    {std::string(R"~~(int main(void) {
    int a = 10;
    a =/ 1;
    return a;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 2, 5, 2], ["IDENTIFIER", "a", 2, 9, 2], ["ASSIGN", "=", 2, 11, 2], ["INTEGER_CONSTANT", "10", 2, 13, 2], ["SEMICOLON", ";", 2, 15, 2], ["IDENTIFIER", "a", 3, 5, 2], ["ASSIGN", "=", 3, 7, 2], ["SLASH", "/", 3, 8, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SLASH', '/') at line 3, column 8.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void) {

    int x = 10;
    // make sure second delimiter in ternary expression is :
    return x ? 1 = 2;
})~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 3, 5, 2], ["IDENTIFIER", "x", 3, 9, 2], ["ASSIGN", "=", 3, 11, 2], ["INTEGER_CONSTANT", "10", 3, 13, 2], ["SEMICOLON", ";", 3, 15, 2], ["SLASH", "/", 4, 5, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SLASH', '/') at line 4, column 5.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* AMP\n\t* ATTR_OPEN\n\t* AUTO\n\t* BANG\n\t* BITINT\n\t* BITINT_INTEGER\n\t* BOOL\n\t* BREAK\n\t* CASE\n\t* CHAR\n\t* CHAR_CONSTANT\n\t* CONST\n\t* CONSTEXPR\n\t* CONTINUE\n\t* DEFAULT\n\t* DO\n\t* DOUBLE\n\t* DOUBLE_CONSTANT\n\t* ENUM\n\t* EXTERN\n\t* FALSE\n\t* FLOAT\n\t* FLOAT_CONSTANT\n\t* FOR\n\t* GOTO\n\t* IDENTIFIER\n\t* IF\n\t* INT\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LONG\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* RBRACE\n\t* REGISTER\n\t* RESTRICT\n\t* RETURN\n\t* SEMICOLON\n\t* SHORT\n\t* SIGNED\n\t* SIZEOF\n\t* STAR\n\t* STATIC\n\t* STRING_LITERAL\n\t* STRUCT\n\t* SWITCH\n\t* THREAD_LOCAL\n\t* TILDE\n\t* TRUE\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n\t* WHILE\n"]])~~")},
    {std::string(R"~~(struct s {
    int a;
};

// storage class specifier can't come between struct keyword and tag
struct static s foo;)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["INT", "int", 2, 5, 1], ["IDENTIFIER", "a", 2, 9, 1], ["SEMICOLON", ";", 2, 10, 1], ["RBRACE", "}", 3, 1, 1], ["SEMICOLON", ";", 3, 2, 1], ["SLASH", "/", 5, 1, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('SLASH', '/') at line 5, column 1.\nExpected one of: \n\t* $END\n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(struct s {
    int y;
};

struct s x;
// dot operator must be immediately followed by member name
// (can't parenthesize it)
int main(void) {
    return x.(y);
}
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["INT", "int", 2, 5, 1], ["IDENTIFIER", "y", 2, 9, 1], ["SEMICOLON", ";", 2, 10, 1], ["RBRACE", "}", 3, 1, 1], ["SEMICOLON", ";", 3, 2, 1], ["STRUCT", "struct", 5, 1, 1], ["IDENTIFIER", "s", 5, 8, 1], ["IDENTIFIER", "x", 5, 10, 1], ["SEMICOLON", ";", 5, 11, 1], ["SLASH", "/", 6, 1, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('SLASH', '/') at line 6, column 1.\nExpected one of: \n\t* $END\n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(struct s {
    int y;
};

int main(void) {
    // can't parenthesize struct tag
    struct(s) var;

    return 0;
})~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["INT", "int", 2, 5, 1], ["IDENTIFIER", "y", 2, 9, 1], ["SEMICOLON", ";", 2, 10, 1], ["RBRACE", "}", 3, 1, 1], ["SEMICOLON", ";", 3, 2, 1], ["INT", "int", 5, 1, 1], ["IDENTIFIER", "main", 5, 5, 1], ["LPAREN", "(", 5, 9, 1], ["VOID", "void", 5, 10, 1], ["RPAREN", ")", 5, 14, 1], ["LBRACE", "{", 5, 16, 2], ["SLASH", "/", 6, 5, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SLASH', '/') at line 6, column 5.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* AMP\n\t* ATTR_OPEN\n\t* AUTO\n\t* BANG\n\t* BITINT\n\t* BITINT_INTEGER\n\t* BOOL\n\t* BREAK\n\t* CASE\n\t* CHAR\n\t* CHAR_CONSTANT\n\t* CONST\n\t* CONSTEXPR\n\t* CONTINUE\n\t* DEFAULT\n\t* DO\n\t* DOUBLE\n\t* DOUBLE_CONSTANT\n\t* ENUM\n\t* EXTERN\n\t* FALSE\n\t* FLOAT\n\t* FLOAT_CONSTANT\n\t* FOR\n\t* GOTO\n\t* IDENTIFIER\n\t* IF\n\t* INT\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LONG\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* RBRACE\n\t* REGISTER\n\t* RESTRICT\n\t* RETURN\n\t* SEMICOLON\n\t* SHORT\n\t* SIGNED\n\t* SIZEOF\n\t* STAR\n\t* STATIC\n\t* STRING_LITERAL\n\t* STRUCT\n\t* SWITCH\n\t* THREAD_LOCAL\n\t* TILDE\n\t* TRUE\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n\t* WHILE\n"]])~~")},
    {std::string(R"~~(static int foo = 3;

int main(void) {
    return foo;
}

/* if you declare a variable with the 'extern' keyword when a declaration
 * with linkage is already in scope, the new declaration takes on the same
 * linkage as the previous declaration
 */
extern int foo;)~~"),
     {},
     {},
     std::string(R"~~([[["STATIC", "static", 1, 1, 1], ["INT", "int", 1, 8, 1], ["IDENTIFIER", "foo", 1, 12, 1], ["ASSIGN", "=", 1, 16, 1], ["INTEGER_CONSTANT", "3", 1, 18, 1], ["SEMICOLON", ";", 1, 19, 1], ["INT", "int", 3, 1, 1], ["IDENTIFIER", "main", 3, 5, 1], ["LPAREN", "(", 3, 9, 1], ["VOID", "void", 3, 10, 1], ["RPAREN", ")", 3, 14, 1], ["LBRACE", "{", 3, 16, 2], ["RETURN", "return", 4, 5, 2], ["IDENTIFIER", "foo", 4, 12, 2], ["SEMICOLON", ";", 4, 15, 2], ["RBRACE", "}", 5, 1, 1], ["SLASH", "/", 7, 1, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('SLASH', '/') at line 7, column 1.\nExpected one of: \n\t* $END\n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* ATTR_OPEN\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* REGISTER\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(struct pair {
    int a;
    int b;
};

int main(void) {
    // you can't use the dot operator to specify one member in a struct
    // declarator; you need a compound initializer to initialize structure
    // members
    struct pair x.a = 10;
})~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "pair", 1, 8, 1], ["LBRACE", "{", 1, 13, 1], ["INT", "int", 2, 5, 1], ["IDENTIFIER", "a", 2, 9, 1], ["SEMICOLON", ";", 2, 10, 1], ["INT", "int", 3, 5, 1], ["IDENTIFIER", "b", 3, 9, 1], ["SEMICOLON", ";", 3, 10, 1], ["RBRACE", "}", 4, 1, 1], ["SEMICOLON", ";", 4, 2, 1], ["INT", "int", 6, 1, 1], ["IDENTIFIER", "main", 6, 5, 1], ["LPAREN", "(", 6, 9, 1], ["VOID", "void", 6, 10, 1], ["RPAREN", ")", 6, 14, 1], ["LBRACE", "{", 6, 16, 2], ["SLASH", "/", 7, 5, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SLASH', '/') at line 7, column 5.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* AMP\n\t* ATTR_OPEN\n\t* AUTO\n\t* BANG\n\t* BITINT\n\t* BITINT_INTEGER\n\t* BOOL\n\t* BREAK\n\t* CASE\n\t* CHAR\n\t* CHAR_CONSTANT\n\t* CONST\n\t* CONSTEXPR\n\t* CONTINUE\n\t* DEFAULT\n\t* DO\n\t* DOUBLE\n\t* DOUBLE_CONSTANT\n\t* ENUM\n\t* EXTERN\n\t* FALSE\n\t* FLOAT\n\t* FLOAT_CONSTANT\n\t* FOR\n\t* GOTO\n\t* IDENTIFIER\n\t* IF\n\t* INT\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LONG\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* RBRACE\n\t* REGISTER\n\t* RESTRICT\n\t* RETURN\n\t* SEMICOLON\n\t* SHORT\n\t* SIGNED\n\t* SIZEOF\n\t* STAR\n\t* STATIC\n\t* STRING_LITERAL\n\t* STRUCT\n\t* SWITCH\n\t* THREAD_LOCAL\n\t* TILDE\n\t* TRUE\n\t* TYPEDEF\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n\t* WHILE\n"]])~~")},
    {std::string(R"~~(



int main(void) {
    int a = 10;
    a =/ 1;
    return a;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 5, 1, 1], ["IDENTIFIER", "main", 5, 5, 1], ["LPAREN", "(", 5, 9, 1], ["VOID", "void", 5, 10, 1], ["RPAREN", ")", 5, 14, 1], ["LBRACE", "{", 5, 16, 2], ["INT", "int", 6, 5, 2], ["IDENTIFIER", "a", 6, 9, 2], ["ASSIGN", "=", 6, 11, 2], ["INTEGER_CONSTANT", "10", 6, 13, 2], ["SEMICOLON", ";", 6, 15, 2], ["IDENTIFIER", "a", 7, 5, 2], ["ASSIGN", "=", 7, 7, 2], ["SLASH", "/", 7, 8, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('SLASH', '/') at line 7, column 8.\nExpected one of: \n\t* AMP\n\t* BANG\n\t* BITINT_INTEGER\n\t* CHAR_CONSTANT\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* IDENTIFIER\n\t* INTEGER_CONSTANT\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n"]])~~")},
    {std::string(R"~~(int main(void) {




    (int (*)*) 10;
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["LPAREN", "(", 6, 5, 2], ["INT", "int", 6, 6, 2], ["LPAREN", "(", 6, 10, 2], ["STAR", "*", 6, 11, 2], ["RPAREN", ")", 6, 12, 2], ["STAR", "*", 6, 13, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('STAR', '*') at line 6, column 13.\nExpected one of: \n\t* LBRACKET\n\t* LPAREN\n\t* RPAREN\n"]])~~")},
    {std::string(R"~~(union y {

    static int a;
};
)~~"),
     {},
     {},
     std::string(R"~~([[["UNION", "union", 1, 1, 1], ["IDENTIFIER", "y", 1, 7, 1], ["LBRACE", "{", 1, 9, 1], ["STATIC", "static", 3, 5, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('STATIC', 'static') at line 3, column 5.\nExpected one of: \n\t* ALT_BOOL\n\t* ATTR_OPEN\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* DOUBLE\n\t* ENUM\n\t* FLOAT\n\t* INT\n\t* LONG\n\t* RESTRICT\n\t* SHORT\n\t* SIGNED\n\t* STRUCT\n\t* TYPEDEF_NAME\n\t* TYPEOF\n\t* TYPEOF_UNQUAL\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(struct s {
    int a;
};


struct static s foo;
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["INT", "int", 2, 5, 1], ["IDENTIFIER", "a", 2, 9, 1], ["SEMICOLON", ";", 2, 10, 1], ["RBRACE", "}", 3, 1, 1], ["SEMICOLON", ";", 3, 2, 1], ["STRUCT", "struct", 6, 1, 1], ["STATIC", "static", 6, 8, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('STATIC', 'static') at line 6, column 8.\nExpected one of: \n\t* ATTR_OPEN\n\t* IDENTIFIER\n\t* LBRACE\n"]])~~")},
    {std::string(R"~~(int main(void) {
    goto "foo";
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["GOTO", "goto", 2, 5, 2], ["STRING_LITERAL", "\"foo\"", 2, 10, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('STRING_LITERAL', '\"foo\"') at line 2, column 10.\nExpected one of: \n\t* IDENTIFIER\n"]])~~")},
    {std::string(R"~~(
int main(void) {
    goto "foo";
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 2, 1, 1], ["IDENTIFIER", "main", 2, 5, 1], ["LPAREN", "(", 2, 9, 1], ["VOID", "void", 2, 10, 1], ["RPAREN", ")", 2, 14, 1], ["LBRACE", "{", 2, 16, 2], ["GOTO", "goto", 3, 5, 2], ["STRING_LITERAL", "\"foo\"", 3, 10, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('STRING_LITERAL', '\"foo\"') at line 3, column 10.\nExpected one of: \n\t* IDENTIFIER\n"]])~~")},
    {std::string(R"~~(int main(void) {
    int "x" = 0;
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["INT", "int", 2, 5, 2], ["STRING_LITERAL", "\"x\"", 2, 9, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('STRING_LITERAL', '\"x\"') at line 2, column 9.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* IDENTIFIER\n\t* INT\n\t* LONG\n\t* LPAREN\n\t* REGISTER\n\t* RESTRICT\n\t* SEMICOLON\n\t* SHORT\n\t* SIGNED\n\t* STAR\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(

int main(void) {
    int "x" = 0;
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 3, 1, 1], ["IDENTIFIER", "main", 3, 5, 1], ["LPAREN", "(", 3, 9, 1], ["VOID", "void", 3, 10, 1], ["RPAREN", ")", 3, 14, 1], ["LBRACE", "{", 3, 16, 2], ["INT", "int", 4, 5, 2], ["STRING_LITERAL", "\"x\"", 4, 9, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('STRING_LITERAL', '\"x\"') at line 4, column 9.\nExpected one of: \n\t* ALT_BOOL\n\t* ALT_THREAD_LOCAL\n\t* AUTO\n\t* BITINT\n\t* BOOL\n\t* CHAR\n\t* CONST\n\t* CONSTEXPR\n\t* DOUBLE\n\t* ENUM\n\t* EXTERN\n\t* FLOAT\n\t* IDENTIFIER\n\t* INT\n\t* LONG\n\t* LPAREN\n\t* REGISTER\n\t* RESTRICT\n\t* SEMICOLON\n\t* SHORT\n\t* SIGNED\n\t* STAR\n\t* STATIC\n\t* STRUCT\n\t* THREAD_LOCAL\n\t* TYPEDEF\n\t* UNION\n\t* UNSIGNED\n\t* VOID\n\t* VOLATILE\n"]])~~")},
    {std::string(R"~~(union struct {
    int a;
};

int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["UNION", "union", 1, 1, 1], ["STRUCT", "struct", 1, 7, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('STRUCT', 'struct') at line 1, column 7.\nExpected one of: \n\t* ATTR_OPEN\n\t* IDENTIFIER\n\t* LBRACE\n"]])~~")},
    {std::string(R"~~(struct struct s;

int main(void) {
    return 1;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["STRUCT", "struct", 1, 8, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('STRUCT', 'struct') at line 1, column 8.\nExpected one of: \n\t* ATTR_OPEN\n\t* IDENTIFIER\n\t* LBRACE\n"]])~~")},
    {std::string(R"~~(
union struct {
    int a;
};

int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["UNION", "union", 2, 1, 1], ["STRUCT", "struct", 2, 7, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('STRUCT', 'struct') at line 2, column 7.\nExpected one of: \n\t* ATTR_OPEN\n\t* IDENTIFIER\n\t* LBRACE\n"]])~~")},
    {std::string(R"~~(int main(void) {
foo:
    struct s {
        int a;
    };
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["IDENTIFIER", "foo", 2, 1, 2], ["COLON", ":", 2, 4, 2], ["STRUCT", "struct", 3, 5, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('STRUCT', 'struct') at line 3, column 5.\nExpected one of: \n\t* AMP\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* BREAK\n\t* CASE\n\t* CHAR_CONSTANT\n\t* CONTINUE\n\t* DEFAULT\n\t* DO\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* FOR\n\t* GOTO\n\t* IDENTIFIER\n\t* IF\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* RETURN\n\t* SEMICOLON\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* SWITCH\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\t* WHILE\n"]])~~")},
    {std::string(R"~~(int main(void) {
    switch (0) {
        case 0:
            struct s {
                int a;
            };
            return 0;
    }
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["SWITCH", "switch", 2, 5, 2], ["LPAREN", "(", 2, 12, 2], ["INTEGER_CONSTANT", "0", 2, 13, 2], ["RPAREN", ")", 2, 14, 2], ["LBRACE", "{", 2, 16, 3], ["CASE", "case", 3, 9, 3], ["INTEGER_CONSTANT", "0", 3, 14, 3], ["COLON", ":", 3, 15, 3], ["STRUCT", "struct", 4, 13, 3]], ["syntax", "UnexpectedToken", "Unexpected token Token('STRUCT', 'struct') at line 4, column 13.\nExpected one of: \n\t* AMP\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* BREAK\n\t* CASE\n\t* CHAR_CONSTANT\n\t* CONTINUE\n\t* DEFAULT\n\t* DO\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* FOR\n\t* GOTO\n\t* IDENTIFIER\n\t* IF\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* RETURN\n\t* SEMICOLON\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* SWITCH\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\t* WHILE\n"]])~~")},
    {std::string(R"~~(

int main(void) {
foo:
    struct s {
        int a;
    };
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 3, 1, 1], ["IDENTIFIER", "main", 3, 5, 1], ["LPAREN", "(", 3, 9, 1], ["VOID", "void", 3, 10, 1], ["RPAREN", ")", 3, 14, 1], ["LBRACE", "{", 3, 16, 2], ["IDENTIFIER", "foo", 4, 1, 2], ["COLON", ":", 4, 4, 2], ["STRUCT", "struct", 5, 5, 2]], ["syntax", "UnexpectedToken", "Unexpected token Token('STRUCT', 'struct') at line 5, column 5.\nExpected one of: \n\t* AMP\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* BREAK\n\t* CASE\n\t* CHAR_CONSTANT\n\t* CONTINUE\n\t* DEFAULT\n\t* DO\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* FOR\n\t* GOTO\n\t* IDENTIFIER\n\t* IF\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* RETURN\n\t* SEMICOLON\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* SWITCH\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\t* WHILE\n"]])~~")},
    {std::string(R"~~(struct s {
    int a;
};

struct struct s x;

int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["INT", "int", 2, 5, 1], ["IDENTIFIER", "a", 2, 9, 1], ["SEMICOLON", ";", 2, 10, 1], ["RBRACE", "}", 3, 1, 1], ["SEMICOLON", ";", 3, 2, 1], ["STRUCT", "struct", 5, 1, 1], ["STRUCT", "struct", 5, 8, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('STRUCT', 'struct') at line 5, column 8.\nExpected one of: \n\t* ATTR_OPEN\n\t* IDENTIFIER\n\t* LBRACE\n"]])~~")},
    {std::string(R"~~(

int main(void) {
    switch (0) {
        case 0:
            struct s {
                int a;
            };
            return 0;
    }
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 3, 1, 1], ["IDENTIFIER", "main", 3, 5, 1], ["LPAREN", "(", 3, 9, 1], ["VOID", "void", 3, 10, 1], ["RPAREN", ")", 3, 14, 1], ["LBRACE", "{", 3, 16, 2], ["SWITCH", "switch", 4, 5, 2], ["LPAREN", "(", 4, 12, 2], ["INTEGER_CONSTANT", "0", 4, 13, 2], ["RPAREN", ")", 4, 14, 2], ["LBRACE", "{", 4, 16, 3], ["CASE", "case", 5, 9, 3], ["INTEGER_CONSTANT", "0", 5, 14, 3], ["COLON", ":", 5, 15, 3], ["STRUCT", "struct", 6, 13, 3]], ["syntax", "UnexpectedToken", "Unexpected token Token('STRUCT', 'struct') at line 6, column 13.\nExpected one of: \n\t* AMP\n\t* ATTR_OPEN\n\t* BANG\n\t* BITINT_INTEGER\n\t* BREAK\n\t* CASE\n\t* CHAR_CONSTANT\n\t* CONTINUE\n\t* DEFAULT\n\t* DO\n\t* DOUBLE_CONSTANT\n\t* FALSE\n\t* FLOAT_CONSTANT\n\t* FOR\n\t* GOTO\n\t* IDENTIFIER\n\t* IF\n\t* INTEGER_CONSTANT\n\t* LBRACE\n\t* LONG_DOUBLE_CONSTANT\n\t* LONG_INTEGER\n\t* LPAREN\n\t* MINUS\n\t* MINUSMINUS\n\t* NULLPTR\n\t* PLUS\n\t* PLUSPLUS\n\t* RETURN\n\t* SEMICOLON\n\t* SIZEOF\n\t* STAR\n\t* STRING_LITERAL\n\t* SWITCH\n\t* TILDE\n\t* TRUE\n\t* UBITINT_INTEGER\n\t* UINT_INTEGER\n\t* ULONG_INTEGER\n\t* WHILE\n"]])~~")},
    {std::string(R"~~(union u {
    int a;
};

union union u x;

int main(void) {
    return 0;
}
)~~"),
     {},
     {},
     std::string(R"~~([[["UNION", "union", 1, 1, 1], ["IDENTIFIER", "u", 1, 7, 1], ["LBRACE", "{", 1, 9, 1], ["INT", "int", 2, 5, 1], ["IDENTIFIER", "a", 2, 9, 1], ["SEMICOLON", ";", 2, 10, 1], ["RBRACE", "}", 3, 1, 1], ["SEMICOLON", ";", 3, 2, 1], ["UNION", "union", 5, 1, 1], ["UNION", "union", 5, 7, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('UNION', 'union') at line 5, column 7.\nExpected one of: \n\t* ATTR_OPEN\n\t* IDENTIFIER\n\t* LBRACE\n"]])~~")},
    {std::string(R"~~(int long(void) {
    return 4;
}

int main(void){
    return long();
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["LONG", "long", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('VOID', 'void') at line 1, column 10.\nExpected one of: \n\t* IDENTIFIER\n\t* LPAREN\n\t* STAR\n"]])~~")},
    {std::string(R"~~(
int long(void) {
    return 4;
}

int main(void){
    return long();
}
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 2, 1, 1], ["LONG", "long", 2, 5, 1], ["LPAREN", "(", 2, 9, 1], ["VOID", "void", 2, 10, 1]], ["syntax", "UnexpectedToken", "Unexpected token Token('VOID', 'void') at line 2, column 10.\nExpected one of: \n\t* IDENTIFIER\n\t* LPAREN\n\t* STAR\n"]])~~")},
    {std::string(R"~~(enum E : int; int n = sizeof(enum E : int);)~~"),
     {},
     {},
     std::string(R"~~([[["ENUM", "enum", 1, 1, 1], ["IDENTIFIER", "E", 1, 6, 1], ["COLON", ":", 1, 8, 1], ["INT", "int", 1, 10, 1], ["SEMICOLON", ";", 1, 13, 1], ["INT", "int", 1, 15, 1], ["IDENTIFIER", "n", 1, 19, 1], ["ASSIGN", "=", 1, 21, 1], ["SIZEOF", "sizeof", 1, 23, 1], ["LPAREN", "(", 1, 29, 1], ["ENUM", "enum", 1, 30, 1], ["IDENTIFIER", "E", 1, 35, 1], ["COLON", ":", 1, 37, 1], ["INT", "int", 1, 39, 1], ["RPAREN", ")", 1, 42, 1]], ["syntax", "ParserError", "`enum e : T` without a list of constants can only be a whole declaration, `enum e : T;`, with no storage class, `typedef`, or declarator, and not in a struct, a `for`, a cast, or `sizeof` (C23 \u00a76.7.3.3). Declare the tag that way, and then name the type as `enum e`. The parser stopped at ')' on line 1"]])~~")},
    {std::string(R"~~(enum E : int *p;)~~"),
     {},
     {},
     std::string(R"~~([[["ENUM", "enum", 1, 1, 1], ["IDENTIFIER", "E", 1, 6, 1], ["COLON", ":", 1, 8, 1], ["INT", "int", 1, 10, 1], ["STAR", "*", 1, 14, 1]], ["syntax", "ParserError", "`enum e : T` without a list of constants can only be a whole declaration, `enum e : T;`, with no storage class, `typedef`, or declarator, and not in a struct, a `for`, a cast, or `sizeof` (C23 \u00a76.7.3.3). Declare the tag that way, and then name the type as `enum e`. The parser stopped at '*' on line 1"]])~~")},
    {std::string(R"~~(enum E : int, x;)~~"),
     {},
     {},
     std::string(R"~~([[["ENUM", "enum", 1, 1, 1], ["IDENTIFIER", "E", 1, 6, 1], ["COLON", ":", 1, 8, 1], ["INT", "int", 1, 10, 1], ["COMMA", ",", 1, 13, 1]], ["syntax", "ParserError", "`enum e : T` without a list of constants can only be a whole declaration, `enum e : T;`, with no storage class, `typedef`, or declarator, and not in a struct, a `for`, a cast, or `sizeof` (C23 \u00a76.7.3.3). Declare the tag that way, and then name the type as `enum e`. The parser stopped at ',' on line 1"]])~~")},
    {std::string(R"~~(static enum E : int;)~~"),
     {},
     {},
     std::string(R"~~([[["STATIC", "static", 1, 1, 1], ["ENUM", "enum", 1, 8, 1], ["IDENTIFIER", "E", 1, 13, 1], ["COLON", ":", 1, 15, 1], ["INT", "int", 1, 17, 1], ["SEMICOLON", ";", 1, 20, 1]], ["syntax", "ParserError", "`enum e : T` without a list of constants can only be a whole declaration, `enum e : T;`, with no storage class, `typedef`, or declarator, and not in a struct, a `for`, a cast, or `sizeof` (C23 \u00a76.7.3.3). Declare the tag that way, and then name the type as `enum e`. The parser stopped at ';' on line 1"]])~~")},
    {std::string(R"~~(enum E : int f(void) { return 0; })~~"),
     {},
     {},
     std::string(R"~~([[["ENUM", "enum", 1, 1, 1], ["IDENTIFIER", "E", 1, 6, 1], ["COLON", ":", 1, 8, 1], ["INT", "int", 1, 10, 1], ["IDENTIFIER", "f", 1, 14, 1]], ["syntax", "ParserError", "`enum e : T` without a list of constants can only be a whole declaration, `enum e : T;`, with no storage class, `typedef`, or declarator, and not in a struct, a `for`, a cast, or `sizeof` (C23 \u00a76.7.3.3). Declare the tag that way, and then name the type as `enum e`. The parser stopped at 'f' on line 1"]])~~")},
    {std::string(R"~~(enum E : int x;)~~"),
     {},
     {},
     std::string(R"~~([[["ENUM", "enum", 1, 1, 1], ["IDENTIFIER", "E", 1, 6, 1], ["COLON", ":", 1, 8, 1], ["INT", "int", 1, 10, 1], ["IDENTIFIER", "x", 1, 14, 1]], ["syntax", "ParserError", "`enum e : T` without a list of constants can only be a whole declaration, `enum e : T;`, with no storage class, `typedef`, or declarator, and not in a struct, a `for`, a cast, or `sizeof` (C23 \u00a76.7.3.3). Declare the tag that way, and then name the type as `enum e`. The parser stopped at 'x' on line 1"]])~~")},
    {std::string(R"~~(int bool = 1;)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["BOOL", "bool", 1, 5, 1], ["ASSIGN", "=", 1, 10, 1]], ["syntax", "ParserError", "line 1, column 10: `bool` is a keyword in C23 (\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label"]])~~")},
    {std::string(R"~~(int f(int nullptr) { return 0; })~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "f", 1, 5, 1], ["LPAREN", "(", 1, 6, 1], ["INT", "int", 1, 7, 1], ["NULLPTR", "nullptr", 1, 11, 1]], ["syntax", "ParserError", "line 1, column 11: `nullptr` is a keyword in C23 (\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label"]])~~")},
    {std::string(R"~~(struct s { int false; };)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["INT", "int", 1, 12, 1], ["FALSE", "false", 1, 16, 1]], ["syntax", "ParserError", "line 1, column 16: `false` is a keyword in C23 (\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label"]])~~")},
    {std::string(R"~~(struct s { int nullptr; };)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["IDENTIFIER", "s", 1, 8, 1], ["LBRACE", "{", 1, 10, 1], ["INT", "int", 1, 12, 1], ["NULLPTR", "nullptr", 1, 16, 1]], ["syntax", "ParserError", "line 1, column 16: `nullptr` is a keyword in C23 (\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label"]])~~")},
    {std::string(R"~~(int f(int x, int true) { return x; })~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "f", 1, 5, 1], ["LPAREN", "(", 1, 6, 1], ["INT", "int", 1, 7, 1], ["IDENTIFIER", "x", 1, 11, 1], ["COMMA", ",", 1, 12, 1], ["INT", "int", 1, 14, 1], ["TRUE", "true", 1, 18, 1]], ["syntax", "ParserError", "line 1, column 18: `true` is a keyword in C23 (\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label"]])~~")},
    {std::string(R"~~(int f(void) { int typeof_unqual; return 0; }
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "f", 1, 5, 1], ["LPAREN", "(", 1, 6, 1], ["VOID", "void", 1, 7, 1], ["RPAREN", ")", 1, 11, 1], ["LBRACE", "{", 1, 13, 2], ["INT", "int", 1, 15, 2], ["TYPEOF_UNQUAL", "typeof_unqual", 1, 19, 2]], ["syntax", "ParserError", "line 1, column 19: `typeof_unqual` is a keyword in C23 (\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label"]])~~")},
    {std::string(R"~~(int main(void) { goto bool; })~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "main", 1, 5, 1], ["LPAREN", "(", 1, 9, 1], ["VOID", "void", 1, 10, 1], ["RPAREN", ")", 1, 14, 1], ["LBRACE", "{", 1, 16, 2], ["GOTO", "goto", 1, 18, 2], ["BOOL", "bool", 1, 23, 2]], ["syntax", "ParserError", "line 1, column 23: `bool` is a keyword in C23 (\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label"]])~~")},
    {std::string(R"~~(int f(void) { int constexpr = 1; return 0; })~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["IDENTIFIER", "f", 1, 5, 1], ["LPAREN", "(", 1, 6, 1], ["VOID", "void", 1, 7, 1], ["RPAREN", ")", 1, 11, 1], ["LBRACE", "{", 1, 13, 2], ["INT", "int", 1, 15, 2], ["CONSTEXPR", "constexpr", 1, 19, 2], ["ASSIGN", "=", 1, 29, 2]], ["syntax", "ParserError", "line 1, column 29: `constexpr` is a keyword in C23 (\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label"]])~~")},
    {std::string(R"~~(int false = 0;)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["FALSE", "false", 1, 5, 1]], ["syntax", "ParserError", "line 1, column 5: `false` is a keyword in C23 (\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label"]])~~")},
    {std::string(R"~~(int nullptr;)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["NULLPTR", "nullptr", 1, 5, 1]], ["syntax", "ParserError", "line 1, column 5: `nullptr` is a keyword in C23 (\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label"]])~~")},
    {std::string(R"~~(int true;)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["TRUE", "true", 1, 5, 1]], ["syntax", "ParserError", "line 1, column 5: `true` is a keyword in C23 (\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label"]])~~")},
    {std::string(R"~~(int typeof = 1;
)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["TYPEOF", "typeof", 1, 5, 1]], ["syntax", "ParserError", "line 1, column 5: `typeof` is a keyword in C23 (\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label"]])~~")},
    {std::string(R"~~(int *nullptr(void);)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["STAR", "*", 1, 5, 1], ["NULLPTR", "nullptr", 1, 6, 1]], ["syntax", "ParserError", "line 1, column 6: `nullptr` is a keyword in C23 (\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label"]])~~")},
    {std::string(R"~~(enum { false };)~~"),
     {},
     {},
     std::string(R"~~([[["ENUM", "enum", 1, 1, 1], ["LBRACE", "{", 1, 6, 1], ["FALSE", "false", 1, 8, 1]], ["syntax", "ParserError", "line 1, column 8: `false` is a keyword in C23 (\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label"]])~~")},
    {std::string(R"~~(enum { nullptr };)~~"),
     {},
     {},
     std::string(R"~~([[["ENUM", "enum", 1, 1, 1], ["LBRACE", "{", 1, 6, 1], ["NULLPTR", "nullptr", 1, 8, 1]], ["syntax", "ParserError", "line 1, column 8: `nullptr` is a keyword in C23 (\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label"]])~~")},
    {std::string(R"~~(struct true { int x; };)~~"),
     {},
     {},
     std::string(R"~~([[["STRUCT", "struct", 1, 1, 1], ["TRUE", "true", 1, 8, 1]], ["syntax", "ParserError", "line 1, column 8: `true` is a keyword in C23 (\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label"]])~~")},
    {std::string(R"~~(int bool, x;)~~"),
     {},
     {},
     std::string(R"~~([[["INT", "int", 1, 1, 1], ["BOOL", "bool", 1, 5, 1], ["COMMA", ",", 1, 9, 1]], ["syntax", "ParserError", "line 1, column 9: `bool` is a keyword in C23 (\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label"]])~~")},
    {std::string(R"~~(_Bool a;
int true;
)~~"),
     {},
     {},
     std::string(R"~~([[["ALT_BOOL", "_Bool", 1, 1, 1], ["IDENTIFIER", "a", 1, 7, 1], ["SEMICOLON", ";", 1, 8, 1], ["INT", "int", 2, 1, 1], ["TRUE", "true", 2, 5, 1]], ["syntax", "ParserError", "line 2, column 5: `true` is a keyword in C23 (\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label"]])~~")},
    {std::string(R"~~(typedef int ptrdiff_t;
typedef unsigned int size_t;
typedef char max_align_t;
typedef int wchar_t;
typedef typeof_unqual(nullptr) nullptr_t;
int thread_local = 1;
)~~"),
     {},
     {},
     std::string(R"~~([[["TYPEDEF", "typedef", 1, 1, 1], ["INT", "int", 1, 9, 1], ["IDENTIFIER", "ptrdiff_t", 1, 13, 1], ["SEMICOLON", ";", 1, 22, 1], ["TYPEDEF", "typedef", 2, 1, 1], ["UNSIGNED", "unsigned", 2, 9, 1], ["INT", "int", 2, 18, 1], ["IDENTIFIER", "size_t", 2, 22, 1], ["SEMICOLON", ";", 2, 28, 1], ["TYPEDEF", "typedef", 3, 1, 1], ["CHAR", "char", 3, 9, 1], ["IDENTIFIER", "max_align_t", 3, 14, 1], ["SEMICOLON", ";", 3, 25, 1], ["TYPEDEF", "typedef", 4, 1, 1], ["INT", "int", 4, 9, 1], ["IDENTIFIER", "wchar_t", 4, 13, 1], ["SEMICOLON", ";", 4, 20, 1], ["TYPEDEF", "typedef", 5, 1, 1], ["TYPEOF_UNQUAL", "typeof_unqual", 5, 9, 1], ["LPAREN", "(", 5, 22, 1], ["NULLPTR", "nullptr", 5, 23, 1], ["RPAREN", ")", 5, 30, 1], ["IDENTIFIER", "nullptr_t", 5, 32, 1], ["SEMICOLON", ";", 5, 41, 1], ["INT", "int", 6, 1, 1], ["THREAD_LOCAL", "thread_local", 6, 5, 1], ["ASSIGN", "=", 6, 18, 1]], ["syntax", "ParserError", "line 6, column 18: `thread_local` is a keyword in C23 (\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label"]])~~")},
};

struct TokenizeCase {
  std::string source;
  // The `{"type": ..., "message": ...}` of the LexError.
  std::string error;
};

const TokenizeCase kTokenizeCases[] = {
    {std::string(R"~~(0b1.0)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: a binary constant is an integer, so '0b1.0' can't have a period"})~~")},
    {std::string(R"~~(0b102)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: invalid digit '2' in binary constant '0b102'"})~~")},
    {std::string(R"~~(0b2)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: invalid digit '2' in binary constant '0b2'"})~~")},
    {std::string(R"~~(1e2e3)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: invalid pp-number: 1e2'e3' (numeric constant may not abut an identifier)"})~~")},
    {std::string(R"~~(3f)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: invalid pp-number: 3'f' (numeric constant may not abut an identifier)"})~~")},
    {std::string(R"~~(3.e)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: invalid pp-number: 3.'e' (numeric constant may not abut an identifier)"})~~")},
    {std::string(R"~~(3.u)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: invalid pp-number: 3.'u' (numeric constant may not abut an identifier)"})~~")},
    {std::string(R"~~(3.14e)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: invalid pp-number: 3.14'e' (numeric constant may not abut an identifier)"})~~")},
    {std::string(R"~~(42Ll)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: invalid pp-number: 42L'l' (numeric constant may not abut an identifier)"})~~")},
    {std::string(R"~~(42lL)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: invalid pp-number: 42l'L' (numeric constant may not abut an identifier)"})~~")},
    {std::string(R"~~(42ua)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: invalid pp-number: 42u'a' (numeric constant may not abut an identifier)"})~~")},
    {std::string(R"~~(0b1f)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: invalid suffix 'f' on integer constant '0b1f'"})~~")},
    {std::string(R"~~(0X)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: malformed numeric token '0X'"})~~")},
    {std::string(R"~~(0x)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: malformed numeric token '0x'"})~~")},
    {std::string(R"~~(0x.p1)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: malformed numeric token '0x.'"})~~")},
    {std::string(R"~~(0x.FF)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: malformed numeric token '0x.FF'"})~~")},
    {std::string(R"~~(0x1.)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: malformed numeric token '0x1.'"})~~")},
    {std::string(R"~~(0x1.8)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: malformed numeric token '0x1.8'"})~~")},
    {std::string(R"~~(0x1p)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: malformed numeric token '0x1p'"})~~")},
    {std::string(R"~~(0xFF.)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: malformed numeric token '0xFF.'"})~~")},
    {std::string(R"~~(3e)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: malformed numeric token '3e'"})~~")},
    {std::string(R"~~(0B)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: no digits after the prefix 0B in '0B'"})~~")},
    {std::string(R"~~(0b)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: no digits after the prefix 0b in '0b'"})~~")},
    {std::string(R"~~($)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: unexpected character '$'"})~~")},
    {std::string(R"~~(@)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: unexpected character '@'"})~~")},
    {std::string(R"~~(")~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: unexpected character '\"'"})~~")},
    {std::string(R"~~(`)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: unexpected character '`'"})~~")},
    {std::string(R"~~(')~~"),
     std::string(R"~~({"type": "LexError", "message": "1:1: unexpected character \"'\""})~~")},
    {std::string(R"~~(L")~~"),
     std::string(R"~~({"type": "LexError", "message": "1:2: unexpected character '\"'"})~~")},
    {std::string(R"~~(L')~~"),
     std::string(R"~~({"type": "LexError", "message": "1:2: unexpected character \"'\""})~~")},
    {std::string(R"~~(x = 1''000;)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:5: a digit separator has to be between two digits, and the one after '1' in \"1''000\" isn't followed by a digit"})~~")},
    {std::string(R"~~(x = 1'.5;)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:5: a digit separator has to be between two digits, and the one after '1' in \"1'.5\" isn't followed by a digit"})~~")},
    {std::string(R"~~(x = 1';)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:5: a digit separator has to be between two digits, and the one after '1' in \"1'\" isn't followed by a digit"})~~")},
    {std::string(R"~~(x = 1'000';)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:5: a digit separator has to be between two digits, and the one after \"1'000\" in \"1'000'\" isn't followed by a digit"})~~")},
    {std::string(R"~~(x = 0X'FF;)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:5: a digit separator has to be between two digits, and the one in \"0X'FF\" comes right after the prefix 0X"})~~")},
    {std::string(R"~~(x = 0b'1;)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:5: a digit separator has to be between two digits, and the one in \"0b'1\" comes right after the prefix 0b"})~~")},
    {std::string(R"~~(x = 0x'FF;)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:5: a digit separator has to be between two digits, and the one in \"0x'FF\" comes right after the prefix 0x"})~~")},
    {std::string(R"~~(x = 0x1'p1;)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:5: a digit separator has to be between two digits, and the one in \"0x1'p1\" comes right before the exponent"})~~")},
    {std::string(R"~~(x = 1'e5;)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:5: a digit separator has to be between two digits, and the one in \"1'e5\" comes right before the exponent"})~~")},
    {std::string(R"~~(x = 1'u;)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:5: a digit separator has to be between two digits, and the one in \"1'u\" comes right before the suffix"})~~")},
    {std::string(R"~~(x = 1.'5;)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:5: a digit separator has to be between two digits, and the one in \"1.'5\" touches the period"})~~")},
    {std::string(R"~~(x = 1.5'f;)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:5: a digit separator has to be between two digits, and the one in \"1.5'f\" comes right before the suffix"})~~")},
    {std::string(R"~~(x = 1e'5;)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:5: a digit separator has to be between two digits, and the one in \"1e'5\" comes right after the exponent's letter or sign"})~~")},
    {std::string(R"~~(x = 1e+'5;)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:5: a digit separator has to be between two digits, and the one in \"1e+'5\" comes right after the exponent's letter or sign"})~~")},
    {std::string(R"~~(x = 0b1'2;)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:5: invalid digit '2' in binary constant \"0b1'2\""})~~")},
    {std::string(R"~~(x = 0'8;)~~"),
     std::string(R"~~({"type": "LexError", "message": "1:5: invalid digit '8' in octal constant \"0'8\""})~~")},
    {std::string(R"~~(int
  @)~~"),
     std::string(R"~~({"type": "LexError", "message": "2:3: unexpected character '@'"})~~")},
};

}  // namespace

TEST_CASE("the recognizer stops where Lark stops, with its message") {
  for (const Case& c : kCases) {
    INFO(c.source);
    CHECK(c6502::parser_token_checks::parse_tokens(c.source, c.lex_typedefs,
                                                   c.retype_typedefs) == c.expected);
  }
}

TEST_CASE("tokenize raises Python's LexErrors") {
  for (const TokenizeCase& c : kTokenizeCases) {
    INFO(c.source);
    std::string got;
    try {
      c6502::parser_token_checks::tokenize_json(c.source);
    } catch (const c6502::CompileError& e) {
      got = std::string("{\"type\": \"") + e.python_type() + "\", \"message\": " +
            c6502::parser_token_checks::json_string(e.what()) + "}";
    }
    CHECK(got == c.error);
  }
}

