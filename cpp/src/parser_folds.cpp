// The parser's folds of constant expressions and its typing of the
// operands of `typeof` and `auto` (parser.py:2422-2663), through a type
// checker of the parser's own for each, and the walks over the AST that
// they and `[[c6502::save_hardware_stack]]` make (parser_descent.h).
#include <algorithm>
#include <string>
#include <utility>
#include <variant>

#include "builtin_functions.h"
#include "parser_descent.h"
#include "passes/constant_expression.h"
#include "passes/type_checker.h"
#include "pyrepr.h"

namespace c6502::parser {

namespace ir = c99_ast;
namespace tc = passes::type_checking;
namespace ce = passes::constant_expression;

namespace {

// Returns whether `e` is one of the errors the folds report: the type
// checker's, the constant evaluator's, and, where `not_implemented` is
// set, a NotImplementedError.
bool fold_error(const CompileError& e, bool not_implemented) {
  const std::string& type = e.python_type();
  return type == "TypeCheckError" || type == "ConstantExpressionError" ||
         (not_implemented && type == "NotImplementedError");
}

// Returns whether `t` is a structure or a union, under its qualifiers.
bool is_aggregate_tag(const Type& t) {
  const Type& u = unqualified(t);
  return std::holds_alternative<ir::Structure>(u) || std::holds_alternative<ir::Union>(u);
}

}  // namespace

const Type& unqualified(const Type& t) {
  const Type* p = &t;
  while (true) {
    if (const auto* c = std::get_if<ir::Const>(p)) {
      p = &*c->referenced_type;
    } else if (const auto* v = std::get_if<ir::Volatile>(p)) {
      p = &*v->referenced_type;
    } else {
      return *p;
    }
  }
}

std::vector<std::string> names_in(const Exp& exp) {
  std::vector<std::string> out;
  const auto add = [&out](const std::string& name) {
    if (std::find(out.begin(), out.end(), name) == out.end()) {
      out.push_back(name);
    }
  };
  walk_exp(exp, [&](const Exp& e) {
    if (const auto* v = std::get_if<ir::Var>(&e)) {
      add(v->name);
    } else if (const auto* c = std::get_if<ir::FunctionCall>(&e)) {
      add(c->name);
    }
  });
  return out;
}

void name_literals(Exp& exp) {
  int counter = 0;
  walk_exp(exp, [&](Exp& e) {
    if (auto* c = std::get_if<ir::CompoundLiteral>(&e); c != nullptr && c->name.empty()) {
      ++counter;
      c->name = "@fold" + std::to_string(counter) + ".compound_literal";
    }
  });
}

bool has_literal(const Exp& exp) {
  bool found = false;
  walk_exp(exp, [&](const Exp& e) {
    found = found || std::holds_alternative<ir::CompoundLiteral>(e);
  });
  return found;
}

namespace {

// Marks the calls `walk` visits, as parser.py's `_mark_hw_stack_save`
// does, and throws ParserError when there is none.
template <class Walk>
void mark_calls(Walk&& walk) {
  int count = 0;
  walk([&](Exp& e) {
    if (auto* c = std::get_if<ir::IndirectCall>(&e)) {
      c->hw_stack_save = true;
      ++count;
    } else if (auto* f = std::get_if<ir::FunctionCall>(&e);
               f != nullptr && !builtin_functions::is_builtin(f->name)) {
      f->hw_stack_save = true;
      ++count;
    }
  });
  if (count == 0) {
    throw ParserError(
        "`[[c6502::save_hardware_stack]]` must come before a statement or declaration that "
        "calls a function");
  }
}

}  // namespace

void mark_hw_stack_save(ir::Type_statement& stmt) {
  mark_calls([&](auto&& f) { walk_statement_exps(stmt, f); });
}

void mark_hw_stack_save(std::vector<ir::Type_declaration>& decls) {
  mark_calls([&](auto&& f) {
    for (auto& d : decls) {
      walk_detail::walk_declaration_exps(d, f);
    }
  });
}

tc::TypeChecker Descent::checker_with_named_constants(const Exp& exp, bool layouts) {
  tc::TypeChecker checker;
  std::vector<std::pair<std::string, std::pair<Type, passes::ValueTree>>> constants;
  for (const std::string& name : names_in(exp)) {
    const Binding* binding = ctx_.lookup(name);
    if (binding != nullptr && binding->constant) {
      constants.emplace_back(name, *binding->constant);
    }
  }
  const bool any_aggregate = std::any_of(constants.begin(), constants.end(), [](const auto& c) {
    return is_aggregate_tag(c.second.first);
  });
  if (layouts || any_aggregate) {
    // Each tag's latest body, in the order of the latest bodies.
    OrderedMap<std::string, const ir::Type_struct_decl*> latest;
    for (const ir::Type_struct_decl& sd : ctx_.struct_bodies) {
      latest.erase(sd.tag);
      latest.insert_or_assign(sd.tag, &sd);
    }
    for (const auto& [tag, sd] : latest) {
      ir::Type_struct_decl copy = *sd;
      try {
        checker.check_struct_decl(copy);
      } catch (const CompileError& e) {
        if (e.python_type() != "TypeCheckError" && e.python_type() != "NotImplementedError") {
          throw;
        }
      }
    }
  }
  for (auto& [name, constant] : constants) {
    checker.symbols.insert_or_assign(name,
                                     passes::Symbol{.type = constant.first, .attrs = passes::LocalAttr{}});
    checker.constexpr_values.insert_or_assign(
        name, ce::NamedConstant{.type = constant.first, .value = constant.second});
  }
  return checker;
}

asdl::Int Descent::fold_integer_constant(const Exp& exp) {
  Exp copy = exp;
  name_literals(copy);
  tc::TypeChecker checker = checker_with_named_constants(copy, has_literal(copy));
  try {
    const ce::NamedConstants resolver(
        [&checker](const Exp& e) { return checker.named_constant(e); });
    checker.check_exp(copy);
    return ce::evaluate_integer_constant_expression(copy).first;
  } catch (const CompileError& e) {
    if (!fold_error(e, false)) {
      throw;
    }
    throw ParserError(e.what());
  }
}

void Descent::check_typeof_tags(const Binding& binding, std::string_view keyword,
                                std::string_view name) const {
  for (const auto& [tag, scope] : binding.tag_scopes) {
    if (ctx_.tag_scope(tag) != scope) {
      throw ParserError("`" + std::string(keyword) + "` of " + pyrepr::string(name) +
                        " would name the struct or union " + pyrepr::string(tag) +
                        " declared in an outer scope, which a declaration of " +
                        pyrepr::string(tag) +
                        " in an inner scope hides here; c6502 can't express that type here, so "
                        "give the inner struct or union another tag");
    }
  }
}

Type Descent::expression_type(const Exp& exp, std::string_view keyword,
                              const std::optional<std::string>& what) {
  if (const auto* v = std::get_if<ir::Var>(&exp)) {
    const Binding* binding = ctx_.lookup(v->name);
    if (binding != nullptr && binding->object_type &&
        std::holds_alternative<ir::FunType>(*binding->object_type)) {
      // A function designator has its function type here, since it
      // doesn't decay as an operand of typeof.
      check_typeof_tags(*binding, keyword, v->name);
      return *binding->object_type;
    }
  }
  Exp copy = exp;
  name_literals(copy);
  tc::TypeChecker checker = checker_with_named_constants(copy, true);
  for (const std::string& name : names_in(copy)) {
    const Binding* binding = ctx_.lookup(name);
    if (binding == nullptr || !binding->object_type) {
      continue;
    }
    check_typeof_tags(*binding, keyword, name);
    if (checker.constexpr_values.contains(name)) {
      continue;
    }
    const Type& object_type = *binding->object_type;
    passes::IdAttr attrs = passes::LocalAttr{};
    if (std::holds_alternative<ir::FunType>(object_type)) {
      attrs = passes::FunAttr{.defined = false, .is_global = true};
    }
    checker.symbols.insert_or_assign(
        name, passes::Symbol{.type = object_type, .attrs = std::move(attrs)});
  }
  try {
    const ce::NamedConstants resolver(
        [&checker](const Exp& e) { return checker.named_constant(e); });
    return checker.check_exp(copy);
  } catch (const CompileError& e) {
    if (!fold_error(e, false)) {
      throw;
    }
    const std::string named =
        what ? *what : "the operand of `" + std::string(keyword) + "`";
    throw ParserError(named + ": " + e.what());
  }
}

std::optional<std::pair<Type, passes::ValueTree>> Descent::fold_named_constant(
    const Type& data_type, const Exp& init) {
  const bool aggregate = is_aggregate_tag(data_type);
  if (std::holds_alternative<ir::Array>(data_type)) {
    return std::nullopt;
  }
  Exp copy = init;
  name_literals(copy);
  tc::TypeChecker checker = checker_with_named_constants(copy, aggregate || has_literal(copy));
  ir::Type_var_decl vd{
      .name = "@constexpr", .init = std::move(copy), .data_type = data_type,
      .storage_class = ir::Static{}};
  try {
    const ce::NamedConstants resolver(
        [&checker](const Exp& e) { return checker.named_constant(e); });
    checker.check_block_var(vd);
  } catch (const CompileError& e) {
    if (!fold_error(e, true)) {
      throw;
    }
    return std::nullopt;
  }
  const passes::Symbol* symbol = checker.symbols.find("@constexpr");
  if (symbol == nullptr) {
    return std::nullopt;
  }
  const auto* attrs = std::get_if<passes::StaticAttr>(&symbol->attrs);
  if (attrs == nullptr) {
    throw InternalError("fold_named_constant: the constant isn't static");
  }
  const auto* initial = std::get_if<passes::Initial>(&attrs->initial_value);
  if (initial == nullptr) {
    return std::nullopt;
  }
  return std::pair<Type, passes::ValueTree>(data_type, initial->value);
}

}  // namespace c6502::parser
