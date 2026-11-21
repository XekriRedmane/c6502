// The start rule, the tokens, and the guard of the actions of the
// descent (parser_descent.h), and `parse` (parser.h).
#include "parser_descent.h"

#include <memory>
#include <string>

#include "parser.h"

namespace c6502::parser {

namespace {

// The callback Lark calls for a rule: its alias, or the name of its
// origin when it has none.
std::string_view callback_name(const lark_table::Table& table, int rule) {
  const std::string_view alias = table.rule_alias[static_cast<std::size_t>(rule)];
  if (!alias.empty()) {
    return alias;
  }
  return table.name(table.rule_origin[static_cast<std::size_t>(rule)]);
}

}  // namespace

Terminals::Terminals(const lark_table::Table& table) {
  const auto look_up = [&table](std::string_view name) {
    const int s = lark::symbol_of(table, name);
    if (s < 0) {
      throw InternalError("parser: no terminal " + std::string(name));
    }
    return s;
  };
  END = look_up("$END");
#define C6502_PARSER_TERMINAL_LOOKUP(name) name = look_up(#name);
  C6502_PARSER_TERMINALS(C6502_PARSER_TERMINAL_LOOKUP)
#undef C6502_PARSER_TERMINAL_LOOKUP
}

const Terminals& terminals() {
  static const Terminals kTerminals(lark_table::c99_table());
  return kTerminals;
}

Descent::Descent(std::string_view source, ParseContext& ctx, std::string_view start)
    : t(terminals()), ctx_(ctx), table_(lark_table::c99_table()), stream_(source, ctx, start) {}

c99_ast::Program Descent::parse_start() {
  std::vector<DeclList> items;
  while (!stream_.at_end()) {
    std::optional<Attributes> prefix;
    if (at(t.ATTR_OPEN)) {
      prefix = parse_attribute_specifier_seq();
    }
    items.push_back(parse_declaration(std::move(prefix)));
  }
  c99_ast::Program program = action("start", [&] {
    // `start: declaration*`. Each declaration's value is a list of
    // declarations, two for `struct foo { ... } x;`, the StructDecl of
    // the body and the VarDecl of `x`.
    c99_ast::Program p;
    for (DeclList& item : items) {
      if (item.hw_stack_save) {
        throw ParserError(
            "`[[c6502::save_hardware_stack]]` goes before a statement or a declaration inside "
            "a function; saw it on a declaration at file scope");
      }
      for (auto& d : item.decls) {
        p.declaration.push_back(std::move(d));
      }
    }
    return p;
  });
  take();
  return program;
}

lark::Token Descent::expect(int terminal) {
  lark::Token token = take();
  if (token.type != terminal) {
    throw InternalError("parser: the descent expected " + std::string(table_.name(terminal)) +
                        " and the recognizer took " + lark::token_repr(token, table_));
  }
  return token;
}

void Descent::unexpected(std::string_view where) {
  const lark::Token token = peek();
  take();
  throw InternalError("parser: the descent can't take " + lark::token_repr(token, table_) +
                      " in " + std::string(where) + ", and the recognizer took it");
}

bool Descent::at_type_name() const {
  const int k = kind();
  return k == t.CONST || k == t.RESTRICT || k == t.VOLATILE || k == t.INT || k == t.LONG ||
         k == t.SHORT || k == t.SIGNED || k == t.UNSIGNED || k == t.FLOAT || k == t.DOUBLE ||
         k == t.CHAR || k == t.BOOL || k == t.ALT_BOOL || k == t.VOID || k == t.BITINT ||
         k == t.STRUCT || k == t.UNION || k == t.ENUM || k == t.TYPEDEF_NAME || k == t.TYPEOF ||
         k == t.TYPEOF_UNQUAL;
}

bool Descent::at_declaration_specifier() const {
  const int k = kind();
  return at_type_name() || k == t.STATIC || k == t.EXTERN || k == t.REGISTER ||
         k == t.TYPEDEF || k == t.CONSTEXPR || k == t.THREAD_LOCAL ||
         k == t.ALT_THREAD_LOCAL || k == t.AUTO;
}

bool Descent::at_compound_literal_storage() const {
  const int k = kind();
  return k == t.STATIC || k == t.REGISTER || k == t.CONSTEXPR || k == t.EXTERN ||
         k == t.TYPEDEF || k == t.AUTO || k == t.THREAD_LOCAL || k == t.ALT_THREAD_LOCAL;
}

void Descent::throw_syntax_error_before(std::string_view rule) const {
  // Lark makes the lookahead's reductions one at a time, calling each
  // rule's action as it reduces it, until the table has no action for
  // the lookahead, which is the syntax error, or gives a shift.
  lark::Recognizer copy = stream_.recognizer();
  lark::Token token = peek();
  copy.reduce_through(token, [&](int r) { return callback_name(table_, r) == rule; });
}

c99_ast::Program parse(std::string_view source) {
  ParseContext ctx;
  try {
    Descent descent(source, ctx);
    return descent.parse_start();
  } catch (const lark::UnexpectedToken& e) {
    raise_syntax_error(e, source, ctx);
  } catch (const lark::UnexpectedCharacters& e) {
    raise_syntax_error(e, source, ctx);
  }
}

}  // namespace c6502::parser
