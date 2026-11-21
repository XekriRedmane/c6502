// Tests of the descent's own parts (src/parser_descent.h): the start
// rule, the guard of the actions, and the walks over the AST.
#include <string>
#include <vector>

#include "asdl/runtime.h"
#include "c99_ast.h"
#include "doctest.h"
#include "lark_parser.h"
#include "parser.h"
#include "parser_descent.h"

namespace P = c6502::parser;
namespace ir = c6502::c99_ast;

TEST_CASE("an empty source is a program with no declarations") {
  CHECK(c6502::asdl::dump(P::parse("")) == c6502::asdl::dump(ir::Program{}));
  CHECK(c6502::asdl::dump(P::parse("  \n\t")) == c6502::asdl::dump(ir::Program{}));
}

TEST_CASE("a character no terminal matches is Lark's UnexpectedCharacters") {
  CHECK_THROWS_AS(P::parse("@"), c6502::lark::UnexpectedCharacters);
}

TEST_CASE("the guard reports the syntax error Lark finds before the action's rule") {
  // After `int x`, Lark has shifted the declarator's name, and `)` is a
  // syntax error before any reduction, so an action of `var_decl` that
  // throws gives way to the syntax error. A lookahead the parser can
  // take leaves the action's error to propagate.
  P::ParseContext ctx;
  P::Descent d("int x ) ;", ctx);
  d.take();
  d.take();
  CHECK_THROWS_AS(d.throw_syntax_error_before("var_decl"), c6502::lark::UnexpectedToken);

  P::ParseContext ctx2;
  P::Descent d2("int x ;", ctx2);
  d2.take();
  d2.take();
  CHECK_NOTHROW(d2.throw_syntax_error_before("var_decl"));
  CHECK_THROWS_AS(d2.action("var_decl", []() -> int { throw P::ParserError("from the action"); }),
                  P::ParserError);
}

namespace {

// Returns `f(a), (literal, literal)` with the compound literal
// `(int){b}`, built step by step, since MSVC 19.36 failed with an
// internal error on the nested designated initializers.
ir::Type_exp walk_sample() {
  ir::CompoundLiteral lit;
  lit.target_type = ir::Int{};
  lit.init = ir::Var{.name = "b"};
  ir::FunctionCall call;
  call.name = "f";
  call.args.push_back(ir::Var{.name = "a"});
  ir::Comma comma;
  comma.left = lit;
  comma.right = lit;
  ir::Binary sum;
  sum.op = ir::Add{};
  sum.left = call;
  sum.right = comma;
  return sum;
}

}  // namespace

TEST_CASE("the walks visit the expressions in the order of the fields") {
  ir::Type_exp e = walk_sample();
  CHECK(P::names_in(e) == std::vector<std::string>{"f", "a", "b"});
  CHECK(P::has_literal(e));
  P::name_literals(e);
  const auto& comma = std::get<ir::Comma>(*std::get<ir::Binary>(e).right);
  CHECK(std::get<ir::CompoundLiteral>(*comma.left).name == "@fold1.compound_literal");
  CHECK(std::get<ir::CompoundLiteral>(*comma.right).name == "@fold2.compound_literal");

  ir::Type_statement stmt = ir::Expression{.exp = e};
  P::mark_hw_stack_save(stmt);
  const auto& marked = std::get<ir::Binary>(std::get<ir::Expression>(stmt).exp);
  CHECK(std::get<ir::FunctionCall>(*marked.left).hw_stack_save);
  ir::Return ret;
  ret.exp = ir::Var{.name = "a"};
  ir::Type_statement no_call = ret;
  CHECK_THROWS_AS(P::mark_hw_stack_save(no_call), P::ParserError);
}
