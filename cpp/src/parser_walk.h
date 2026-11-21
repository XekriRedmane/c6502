// The walks over the c99 AST that parser.py makes with
// `dataclasses.fields`, declared in parser_descent.h. A type holds no
// expression, so the walks skip the types and visit the fields that
// hold expressions, statements, and declarations, in the order of the
// `.asdl`'s fields.
#pragma once

#include <type_traits>
#include <variant>

#include "c99_ast.h"

namespace c6502::parser {

namespace walk_detail {

// `T` with the constness of `Like`, for the alternatives of a variant
// that `std::get` returns.
template <class Like, class T>
using same_const = std::conditional_t<std::is_const_v<Like>, const T, T>;

}  // namespace walk_detail

template <class E, class F>
void walk_exp(E& exp, F&& f) {
  namespace ir = c99_ast;
  f(exp);
  const auto get = [&exp]<class T>() -> walk_detail::same_const<E, T>& { return std::get<T>(exp); };
  switch (ir::kind(exp)) {
    case ir::Kind_exp::Constant:
    case ir::Kind_exp::String:
    case ir::Kind_exp::Var:
    case ir::Kind_exp::SizeOfType:
      break;
    case ir::Kind_exp::Cast:
      walk_exp(*get.template operator()<ir::Cast>().exp, f);
      break;
    case ir::Kind_exp::Unary:
      walk_exp(*get.template operator()<ir::Unary>().exp, f);
      break;
    case ir::Kind_exp::Binary: {
      auto& b = get.template operator()<ir::Binary>();
      walk_exp(*b.left, f);
      walk_exp(*b.right, f);
      break;
    }
    case ir::Kind_exp::Assignment: {
      auto& a = get.template operator()<ir::Assignment>();
      walk_exp(*a.lval, f);
      walk_exp(*a.rval, f);
      break;
    }
    case ir::Kind_exp::CompoundAssignment: {
      auto& a = get.template operator()<ir::CompoundAssignment>();
      walk_exp(*a.lval, f);
      walk_exp(*a.rval, f);
      break;
    }
    case ir::Kind_exp::Postfix:
      walk_exp(*get.template operator()<ir::Postfix>().operand, f);
      break;
    case ir::Kind_exp::Prefix:
      walk_exp(*get.template operator()<ir::Prefix>().operand, f);
      break;
    case ir::Kind_exp::Conditional: {
      auto& c = get.template operator()<ir::Conditional>();
      walk_exp(*c.condition, f);
      walk_exp(*c.true_clause, f);
      walk_exp(*c.false_clause, f);
      break;
    }
    case ir::Kind_exp::FunctionCall:
      for (auto& arg : get.template operator()<ir::FunctionCall>().args) {
        walk_exp(arg, f);
      }
      break;
    case ir::Kind_exp::IndirectCall: {
      auto& c = get.template operator()<ir::IndirectCall>();
      walk_exp(*c.callee, f);
      for (auto& arg : c.args) {
        walk_exp(arg, f);
      }
      break;
    }
    case ir::Kind_exp::Dereference:
      walk_exp(*get.template operator()<ir::Dereference>().exp, f);
      break;
    case ir::Kind_exp::AddressOf:
      walk_exp(*get.template operator()<ir::AddressOf>().exp, f);
      break;
    case ir::Kind_exp::Subscript: {
      auto& s = get.template operator()<ir::Subscript>();
      walk_exp(*s.array, f);
      walk_exp(*s.index, f);
      break;
    }
    case ir::Kind_exp::InitList:
      for (auto& item : get.template operator()<ir::InitList>().items) {
        walk_exp(item, f);
      }
      break;
    case ir::Kind_exp::Designation:
      walk_exp(*get.template operator()<ir::Designation>().init, f);
      break;
    case ir::Kind_exp::SizeOfExp:
      walk_exp(*get.template operator()<ir::SizeOfExp>().exp, f);
      break;
    case ir::Kind_exp::Dot:
      walk_exp(*get.template operator()<ir::Dot>().operand, f);
      break;
    case ir::Kind_exp::Arrow:
      walk_exp(*get.template operator()<ir::Arrow>().operand, f);
      break;
    case ir::Kind_exp::Comma: {
      auto& c = get.template operator()<ir::Comma>();
      walk_exp(*c.left, f);
      walk_exp(*c.right, f);
      break;
    }
    case ir::Kind_exp::CompoundLiteral:
      walk_exp(*get.template operator()<ir::CompoundLiteral>().init, f);
      break;
  }
}

namespace walk_detail {

template <class D, class F>
void walk_declaration_exps(D& decl, F& f);

template <class B, class F>
void walk_block_exps(B& block, F& f) {
  namespace ir = c99_ast;
  using Block = same_const<B, ir::Block>;
  Block& b = std::get<ir::Block>(block);
  for (auto& item : b.block_item) {
    if (ir::kind(item) == ir::Kind_block_item::S) {
      walk_statement_exps(*std::get<ir::S>(item).statement, f);
    } else {
      walk_declaration_exps(*std::get<ir::D>(item).declaration, f);
    }
  }
}

template <class D, class F>
void walk_declaration_exps(D& decl, F& f) {
  namespace ir = c99_ast;
  switch (ir::kind(decl)) {
    case ir::Kind_declaration::FunctionDecl: {
      auto& fd = *std::get<ir::FunctionDecl>(decl).function_decl;
      if (fd.body) {
        walk_block_exps(**fd.body, f);
      }
      break;
    }
    case ir::Kind_declaration::VarDecl: {
      auto& vd = std::get<ir::VarDecl>(decl).var_decl;
      if (vd.init) {
        walk_exp(*vd.init, f);
      }
      break;
    }
    case ir::Kind_declaration::StructDecl:
      break;
  }
}

}  // namespace walk_detail

template <class S, class F>
void walk_statement_exps(S& stmt, F&& f) {
  namespace ir = c99_ast;
  const auto get = [&stmt]<class T>() -> walk_detail::same_const<S, T>& {
    return std::get<T>(stmt);
  };
  switch (ir::kind(stmt)) {
    case ir::Kind_statement::Return: {
      auto& r = get.template operator()<ir::Return>();
      if (r.exp) {
        walk_exp(*r.exp, f);
      }
      break;
    }
    case ir::Kind_statement::Expression:
      walk_exp(get.template operator()<ir::Expression>().exp, f);
      break;
    case ir::Kind_statement::IfStmt: {
      auto& s = get.template operator()<ir::IfStmt>();
      walk_exp(s.condition, f);
      walk_statement_exps(*s.then_clause, f);
      if (s.else_clause) {
        walk_statement_exps(**s.else_clause, f);
      }
      break;
    }
    case ir::Kind_statement::Compound:
      walk_detail::walk_block_exps(*get.template operator()<ir::Compound>().block, f);
      break;
    case ir::Kind_statement::WhileStmt: {
      auto& s = get.template operator()<ir::WhileStmt>();
      walk_exp(s.condition, f);
      walk_statement_exps(*s.body, f);
      break;
    }
    case ir::Kind_statement::DoWhileStmt: {
      auto& s = get.template operator()<ir::DoWhileStmt>();
      walk_statement_exps(*s.body, f);
      walk_exp(s.condition, f);
      break;
    }
    case ir::Kind_statement::ForStmt: {
      auto& s = get.template operator()<ir::ForStmt>();
      if (ir::kind(s.init) == ir::Kind_for_init::InitDecl) {
        for (auto& vd : std::get<ir::InitDecl>(s.init).var_decls) {
          if (vd.init) {
            walk_exp(*vd.init, f);
          }
        }
      } else if (auto& e = std::get<ir::InitExp>(s.init).exp) {
        walk_exp(*e, f);
      }
      if (s.condition) {
        walk_exp(*s.condition, f);
      }
      if (s.post_clause) {
        walk_exp(*s.post_clause, f);
      }
      walk_statement_exps(*s.body, f);
      break;
    }
    case ir::Kind_statement::SwitchStmt: {
      auto& s = get.template operator()<ir::SwitchStmt>();
      walk_exp(s.control, f);
      walk_statement_exps(*s.body, f);
      for (auto& c : s.cases) {
        walk_exp(c.value, f);
      }
      break;
    }
    case ir::Kind_statement::CaseStmt: {
      auto& s = get.template operator()<ir::CaseStmt>();
      walk_exp(s.value, f);
      walk_statement_exps(*s.body, f);
      break;
    }
    case ir::Kind_statement::DefaultStmt:
      walk_statement_exps(*get.template operator()<ir::DefaultStmt>().body, f);
      break;
    case ir::Kind_statement::LabeledStmt:
      walk_statement_exps(*get.template operator()<ir::LabeledStmt>().statement, f);
      break;
    case ir::Kind_statement::BreakStmt:
    case ir::Kind_statement::ContinueStmt:
    case ir::Kind_statement::Goto:
    case ir::Kind_statement::Null:
      break;
  }
}

}  // namespace c6502::parser
