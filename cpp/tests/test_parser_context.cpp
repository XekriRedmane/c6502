// Tests of the parser's scopes and its typedef-aware token stream
// (src/parser_context.h), with the actions a parser would take written
// out by hand.
#include <string>
#include <vector>

#include "c99_ast.h"
#include "doctest.h"
#include "lark_parser.h"
#include "parser_context.h"

namespace P = c6502::parser;
namespace L = c6502::lark;

namespace {

P::Binding typedef_of_int() {
  P::Binding b;
  b.data_type = c6502::c99_ast::Type_data_type{c6502::c99_ast::Int{}};
  return b;
}

std::string name_of(const L::Token& t) {
  return std::string(c6502::lark_table::c99_table().name(t.type));
}

// Takes every token of `stream` and returns their terminals' names.
std::vector<std::string> take_all(P::TokenStream& stream) {
  std::vector<std::string> out;
  while (!stream.at_end()) {
    out.push_back(name_of(stream.take()));
  }
  stream.take();
  return out;
}

}  // namespace

TEST_CASE("a typedef name is retyped only where the parser can shift one") {
  P::ParseContext ctx;
  ctx.declare("T", typedef_of_int());
  {
    P::TokenStream s("T x;", ctx);
    CHECK(take_all(s) == std::vector<std::string>{"TYPEDEF_NAME", "IDENTIFIER", "SEMICOLON"});
  }
  {
    // After a type specifier, the name is the declarator.
    P::TokenStream s("int T;", ctx);
    CHECK(take_all(s) == std::vector<std::string>{"INT", "IDENTIFIER", "SEMICOLON"});
  }
}

TEST_CASE("braces open and close scopes, and a struct body isn't one") {
  P::ParseContext ctx;
  P::TokenStream s("struct s { int a; }; int f(void) { return 0; }", ctx);
  while (s.peek().value != "{") {
    s.take();
  }
  CHECK(ctx.scopes.size() == 1);
  CHECK(ctx.brace_scopes.size() == 1);
  CHECK((ctx.brace_scopes.back() == nullptr));
  CHECK((ctx.body_scope == nullptr));
  s.take();
  while (s.peek().value != "{") {
    s.take();
  }
  CHECK(ctx.scopes.size() == 2);
  CHECK((ctx.body_scope == ctx.scopes.back()));
  s.take();
  CHECK((ctx.body_scope == nullptr));
  while (!s.at_end()) {
    s.take();
  }
  CHECK(ctx.scopes.size() == 1);
  CHECK(ctx.brace_scopes.empty());
  s.take();
}

TEST_CASE("the end of a for statement retypes the token after it") {
  // `T` is a typedef at file scope, and the loop's scope declares an
  // object `T`, so the `T` after the loop is lexed as an identifier
  // and becomes a typedef name again when the loop's scope ends.
  P::ParseContext ctx;
  ctx.declare("T", typedef_of_int());
  P::TokenStream s("int f(void) { for (;;) ; T y; }", ctx);
  while (s.peek().value != "for") {
    s.take();
  }
  REQUIRE(ctx.for_scopes.size() == 1);
  ctx.declare("T", P::Binding{}, ctx.for_scopes.back());
  // Takes the rest of the loop, through its null statement.
  while (s.peek().value != "T") {
    s.take();
  }
  CHECK(name_of(s.peek()) == "IDENTIFIER");
  s.end_for_scope();
  CHECK(name_of(s.peek()) == "TYPEDEF_NAME");
  CHECK(ctx.for_scopes.empty());
  CHECK(ctx.scopes.size() == 2);
  while (!s.at_end()) {
    s.take();
  }
  s.take();
}

TEST_CASE("a syntax error is Lark's, with the terminals sorted") {
  P::ParseContext ctx;
  P::TokenStream s("int f(void) { return 1 +; }", ctx);
  std::string message;
  try {
    while (true) {
      s.take();
    }
  } catch (const L::UnexpectedToken& e) {
    message = e.what();
    CHECK(e.expects("IDENTIFIER"));
  }
  CHECK(message.rfind("Unexpected token Token('SEMICOLON', ';') at line 1, column 25.\n"
                      "Expected one of: \n\t* AMP\n",
                      0) == 0);
}

TEST_CASE("declaring a name twice in one scope") {
  P::ParseContext ctx;
  ctx.declare("T", typedef_of_int());
  ctx.declare("T", typedef_of_int());
  CHECK_THROWS_WITH_AS(ctx.declare("T", P::Binding{}),
                       "'T' is declared both as a typedef name and as an object or function in "
                       "the same scope",
                       P::ParserError);
  P::Binding e;
  e.enum_value = 1;
  CHECK_THROWS_WITH_AS(ctx.declare("T", e),
                       "'T' is declared again in the same scope, and one of the declarations "
                       "is an enumeration constant",
                       P::ParserError);
  P::Binding other;
  other.data_type = c6502::c99_ast::Type_data_type{c6502::c99_ast::Long{}};
  CHECK_THROWS_WITH_AS(ctx.declare("T", other),
                       "typedef 'T' is declared again with a different type", P::ParserError);
}
