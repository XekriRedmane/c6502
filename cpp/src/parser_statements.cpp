// Part R3 of the parser's descent (parser_descent.h): the statements,
// blocks, initializers, designators, and compound literals
// (parser.py:3452-3505, 3607-3716, and the statement actions of
// 4170-4395). The expressions and constants are in
// parser_expressions.cpp.
//
// Loop and switch labels are made by loop_labeling, which runs after
// identifier resolution, so the parser leaves them empty, and a
// `switch` statement's cases, its default label, and its promoted type
// are filled in by loop_labeling and the type checker.
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "parser_descent.h"
#include "pyrepr.h"

namespace c6502::parser {

namespace ir = c99_ast;

// --- Blocks ---

c99_ast::Block Descent::parse_block() {
  // `block: LBRACE block_item* RBRACE`. A block item gives one item, or
  // several when a declaration defines a struct body inline.
  expect(t.LBRACE);
  std::vector<ir::Type_block_item> items;
  while (!at(t.RBRACE)) {
    parse_block_item(items);
  }
  expect(t.RBRACE);
  return ir::Block{.block_item = std::move(items)};
}

void Descent::parse_block_item(std::vector<ir::Type_block_item>& items) {
  // Attributes can begin a declaration, an attribute declaration
  // (`[[fallthrough]];`), or a statement, so they're taken first. A
  // typedef name is retyped where a declaration can start, so a
  // statement never starts with one.
  std::optional<Attributes> attrs;
  if (at(t.ATTR_OPEN)) {
    attrs = parse_attribute_specifier_seq();
  }
  if (!at_declaration_specifier() && !(attrs && at(t.SEMICOLON))) {
    ir::Type_statement stmt = parse_statement_after(std::move(attrs));
    items.push_back(ir::S{.statement = std::move(stmt)});
    return;
  }
  DeclList decls = parse_declaration(std::move(attrs));
  // `decl_item`. A function definition is an external declaration, at
  // file scope only (C99 §6.9.1). A block may hold a function's forward
  // declaration, and never its definition.
  action("decl_item", [&] {
    if (decls.hw_stack_save) {
      mark_hw_stack_save(decls.decls);
    }
    for (ir::Type_declaration& d : decls.decls) {
      if (const auto* f = std::get_if<ir::FunctionDecl>(&d); f != nullptr && f->function_decl->body) {
        throw ParserError("function definition for " + pyrepr::string(f->function_decl->name) +
                          " is only legal at file scope; nested function definitions aren't "
                          "part of standard C (C99 §6.9.1)");
      }
    }
  });
  for (ir::Type_declaration& d : decls.decls) {
    items.push_back(ir::D{.declaration = std::move(d)});
  }
}

// --- Statements ---

c99_ast::Type_statement Descent::parse_statement() { return parse_statement_after(std::nullopt); }

c99_ast::Type_statement Descent::parse_statement_after(std::optional<Attributes> attrs) {
  if (!attrs && at(t.ATTR_OPEN)) {
    attrs = parse_attribute_specifier_seq();
  }
  if (at(t.IDENTIFIER)) {
    // `IDENTIFIER COLON statement` is a label. Otherwise the identifier
    // begins an expression statement.
    const lark::Token name = take();
    if (at(t.COLON)) {
      take();
      ir::Type_statement body = parse_statement();
      if (attrs) {
        // Attributes before a label apply to the label.
        action("attributed_labeled_stmt", [&] { check_label_attributes(*attrs); });
      }
      return ir::LabeledStmt{.label = name.value, .statement = std::move(body)};
    }
    pending_primary = identifier_exp(name);
  } else if (at(t.CASE)) {
    // `case constant_exp : statement`. The value's constness is checked
    // by the labeling and type-checking passes.
    take();
    Exp value = parse_constant_exp();
    expect(t.COLON);
    ir::Type_statement body = parse_statement();
    if (attrs) {
      action("attributed_case_stmt", [&] { check_label_attributes(*attrs); });
    }
    return ir::CaseStmt{.value = std::move(value), .body = std::move(body), .label = ""};
  } else if (at(t.DEFAULT)) {
    take();
    expect(t.COLON);
    ir::Type_statement body = parse_statement();
    if (attrs) {
      action("attributed_default_stmt", [&] { check_label_attributes(*attrs); });
    }
    return ir::DefaultStmt{.body = std::move(body), .label = ""};
  } else if (!attrs && at(t.SEMICOLON)) {
    take();
    return ir::Null{};
  }
  ir::Type_statement stmt = parse_unlabeled_statement();
  if (attrs) {
    // Attributes before a statement (C23 §6.8), which a declaration's
    // leading attributes are not.
    action("attributed_stmt", [&] { apply_statement_attributes(*attrs, stmt); });
  }
  return stmt;
}

c99_ast::Type_statement Descent::parse_unlabeled_statement() {
  const int k = pending_primary ? t.IDENTIFIER : kind();
  if (k == t.RETURN) {
    // A bare `return;` is legal only in a function returning void,
    // which the type checker enforces.
    take();
    std::optional<Exp> value;
    if (!at(t.SEMICOLON)) {
      value = parse_exp();
    }
    expect(t.SEMICOLON);
    return ir::Return{.exp = std::move(value)};
  }
  if (k == t.IF) {
    take();
    expect(t.LPAREN);
    Exp condition = parse_exp();
    expect(t.RPAREN);
    ir::Type_statement then_clause = parse_statement();
    // A dangling `else` binds to the nearest `if`, as Lark's shift does.
    std::optional<asdl::Box<ir::Type_statement>> else_clause;
    if (at(t.ELSE)) {
      take();
      else_clause = asdl::Box<ir::Type_statement>(parse_statement());
    }
    return ir::IfStmt{.condition = std::move(condition),
                      .then_clause = std::move(then_clause),
                      .else_clause = std::move(else_clause)};
  }
  if (k == t.GOTO) {
    take();
    const lark::Token label = expect(t.IDENTIFIER);
    expect(t.SEMICOLON);
    return ir::Goto{.label = label.value};
  }
  if (k == t.LBRACE) {
    return ir::Compound{.block = parse_block()};
  }
  if (k == t.BREAK) {
    take();
    expect(t.SEMICOLON);
    return ir::BreakStmt{.label = ""};
  }
  if (k == t.CONTINUE) {
    take();
    expect(t.SEMICOLON);
    return ir::ContinueStmt{.label = ""};
  }
  if (k == t.WHILE) {
    take();
    expect(t.LPAREN);
    Exp condition = parse_exp();
    expect(t.RPAREN);
    ir::Type_statement body = parse_statement();
    return ir::WhileStmt{.condition = std::move(condition), .body = std::move(body), .label = ""};
  }
  if (k == t.DO) {
    take();
    ir::Type_statement body = parse_statement();
    expect(t.WHILE);
    expect(t.LPAREN);
    Exp condition = parse_exp();
    expect(t.RPAREN);
    expect(t.SEMICOLON);
    return ir::DoWhileStmt{.body = std::move(body), .condition = std::move(condition),
                           .label = ""};
  }
  if (k == t.FOR) {
    return parse_for_statement();
  }
  if (k == t.SWITCH) {
    // The controlling expression is a full `exp` (C99 §6.8.4.2p1).
    take();
    expect(t.LPAREN);
    Exp control = parse_exp();
    expect(t.RPAREN);
    ir::Type_statement body = parse_statement();
    return ir::SwitchStmt{.control = std::move(control), .body = std::move(body), .label = ""};
  }
  Exp e = parse_exp();
  expect(t.SEMICOLON);
  return ir::Expression{.exp = std::move(e)};
}

c99_ast::Type_statement Descent::parse_for_statement() {
  // `FOR LPAREN for_init exp? SEMICOLON exp? RPAREN statement`. The
  // lexer pushed the loop's scope at the `for`.
  expect(t.FOR);
  expect(t.LPAREN);
  ir::Type_for_init init;
  if (at(t.ATTR_OPEN) || at_declaration_specifier()) {
    std::optional<Attributes> attrs;
    if (at(t.ATTR_OPEN)) {
      attrs = parse_attribute_specifier_seq();
    }
    DeclList decls = parse_var_decl(std::move(attrs));
    // `for_init: var_decl`. None of the declarations may define a
    // struct body inline, which would bring the tag into the loop's
    // scope, or declare a function, and none may have a storage class
    // other than `register` (C99 §6.8.5.3 permits only `auto` and
    // `register`). Checking keeps a function declaration from being
    // dropped and the loop parsed with an empty first clause.
    init = action("for_init", [&]() -> ir::Type_for_init {
      if (decls.hw_stack_save) {
        throw ParserError(
            "`[[c6502::save_hardware_stack]]` can't go on the declaration in a `for` "
            "statement's first clause. It goes before an expression statement, a `return` "
            "statement, or a declaration in a block");
      }
      for (const ir::Type_declaration& d : decls.decls) {
        if (std::holds_alternative<ir::StructDecl>(d)) {
          throw ParserError("struct/union definition isn't permitted in a for-loop initializer");
        }
      }
      if (decls.decls.empty()) {
        throw ParserError(
            "a for-loop initializer can't declare a typedef (C99 §6.8.5.3 — only auto / "
            "register)");
      }
      ir::InitDecl out;
      for (ir::Type_declaration& d : decls.decls) {
        if (std::holds_alternative<ir::FunctionDecl>(d)) {
          throw ParserError(
              "function declarations aren't permitted in a for-loop initializer (C99 "
              "§6.8.5.3)");
        }
        ir::Type_var_decl& vd = std::get<ir::VarDecl>(d).var_decl;
        if (vd.storage_class && !std::holds_alternative<ir::Register>(*vd.storage_class)) {
          const char* name =
              std::holds_alternative<ir::Static>(*vd.storage_class) ? "static" : "extern";
          throw ParserError("storage class " + pyrepr::string(name) +
                            " isn't permitted on a for-loop initializer (C99 §6.8.5.3 — only "
                            "auto / register)");
        }
        out.var_decls.push_back(std::move(vd));
      }
      return out;
    });
  } else if (at(t.SEMICOLON)) {
    take();
    init = ir::InitExp{};
  } else {
    Exp e = parse_exp();
    expect(t.SEMICOLON);
    init = ir::InitExp{.exp = std::move(e)};
  }
  std::optional<Exp> condition;
  if (!at(t.SEMICOLON)) {
    condition = parse_exp();
  }
  expect(t.SEMICOLON);
  std::optional<Exp> post_clause;
  if (!at(t.RPAREN)) {
    post_clause = parse_exp();
  }
  expect(t.RPAREN);
  ir::Type_statement body = parse_statement();
  // `for_stmt`. The loop is over, so its scope ends, after the lexer has
  // already lexed the token that follows, which is retyped if the end
  // of the scope uncovers a typedef name.
  stream().end_for_scope();
  return ir::ForStmt{.init = std::move(init),
                     .condition = std::move(condition),
                     .post_clause = std::move(post_clause),
                     .body = std::move(body),
                     .label = ""};
}

// --- Initializers ---

Exp Descent::parse_initializer() {
  // `initializer: assignment_exp -> init_exp | LBRACE ... RBRACE ->
  // init_list`. The type checker decides what the braces mean from the
  // declared type, since `{5}` initializes a scalar and `{}` any object
  // (C23 §6.7.11p11-12).
  if (!at(t.LBRACE)) {
    return parse_assignment_exp();
  }
  return ir::InitList{.items = parse_braced_items()};
}

std::vector<Exp> Descent::parse_braced_items() {
  expect(t.LBRACE);
  std::vector<Exp> items;
  if (!at(t.RBRACE)) {
    items.push_back(parse_init_item());
    while (at(t.COMMA)) {
      take();
      if (at(t.RBRACE)) {
        break;
      }
      items.push_back(parse_init_item());
    }
  }
  expect(t.RBRACE);
  return items;
}

Exp Descent::parse_init_item() {
  // `designator+ ASSIGN initializer -> designation`, an item of a braced
  // list with designators (C23 §6.7.11p1). The designators stay in
  // source order, and passes/designators.py works out which subobject
  // they name.
  if (!at(t.LBRACKET) && !at(t.DOT)) {
    return parse_initializer();
  }
  std::vector<ir::Type_designator> designators;
  while (at(t.LBRACKET) || at(t.DOT)) {
    if (at(t.DOT)) {
      // `.identifier`. The struct or union it names a member of is
      // known only once the list's type is, so passes/designators.py
      // checks the name.
      take();
      const lark::Token name = expect(t.IDENTIFIER);
      designators.push_back(ir::MemberDesignator{.member = name.value});
      continue;
    }
    take();
    Exp index_exp = parse_constant_exp();
    expect(t.RBRACKET);
    // `[constant-expression]`. The index is an integer constant
    // expression (C23 §6.7.11p6), folded as an array size is, so an
    // enumeration constant or a `constexpr` object works. Whether it's
    // in range depends on the array's type, which passes/designators.py
    // checks.
    designators.push_back(action("index_designator", [&]() -> ir::Type_designator {
      asdl::Int index = 0;
      try {
        index = fold_integer_constant(index_exp);
      } catch (const ParserError& e) {
        throw ParserError(
            std::string("an array designator's index must be an integer constant expression "
                        "(C23 §6.7.11p6): ") +
            e.what());
      }
      if (index < 0) {
        throw ParserError("an array designator's index is " + pyrepr::integer(index) +
                          ", and it must not be negative (C23 §6.7.11p6)");
      }
      return ir::IndexDesignator{.index = index};
    }));
  }
  expect(t.ASSIGN);
  Exp init = parse_initializer();
  return ir::Designation{.designators = std::move(designators), .init = std::move(init)};
}

Exp Descent::parse_compound_literal_rest(Type target, const std::vector<lark::Token>& storage) {
  // A compound literal's braces hold what a declaration's `= {...}`
  // would, and its initializer takes the shape of a var_decl's, so the
  // passes after the parser treat it as one: an InitList for an array
  // or a struct, the String for a char array initialized by `{"abc"}`
  // (C99 §6.7.8p14 allows the braces), and the one expression for a
  // scalar (§6.7.8p11). A scalar's `{}`, or a list of more than one
  // item, stays an InitList, which the type checker turns into a zero
  // or rejects, as it does for a declaration's.
  std::vector<Exp> inits = parse_braced_items();
  return action("compound_literal", [&]() -> Exp {
    std::optional<ir::Type_storage_class> storage_class;
    bool is_constexpr = false;
    if (!storage.empty()) {
      std::tie(storage_class, is_constexpr) = literal_storage(storage);
    }
    const Type& core = unqualified(target);
    if (std::holds_alternative<ir::FunType>(core) || std::holds_alternative<ir::Void>(core)) {
      throw ParserError("a compound literal's type must be an object type (C99 §6.5.2.5.1)");
    }
    std::optional<Exp> init;
    if (const auto* arr = std::get_if<ir::Array>(&core)) {
      const Type& element = *arr->element_type;
      if (inits.size() == 1 && std::holds_alternative<ir::String>(inits[0]) &&
          (std::holds_alternative<ir::Char>(element) ||
           std::holds_alternative<ir::SChar>(element) ||
           std::holds_alternative<ir::UChar>(element))) {
        init = std::move(inits[0]);
      }
    } else if (!std::holds_alternative<ir::Structure>(core) &&
               !std::holds_alternative<ir::Union>(core) && inits.size() == 1 &&
               !std::holds_alternative<ir::InitList>(inits[0]) &&
               !std::holds_alternative<ir::Designation>(inits[0])) {
      init = std::move(inits[0]);
    }
    if (!init) {
      init = ir::InitList{.items = std::move(inits)};
    }
    if (is_constexpr) {
      target = constexpr_type(target);
    }
    return ir::CompoundLiteral{.target_type = std::move(target),
                               .init = std::move(*init),
                               .storage_class = std::move(storage_class),
                               .is_constexpr = is_constexpr};
  });
}

}  // namespace c6502::parser
