// The scopes, the typedef-aware lexer, and the rewording of syntax
// errors of parser.py (parser_context.h).
#include "parser_context.h"

#include <algorithm>

#include "lexer.h"
#include "pyrepr.h"

namespace c6502::parser {

namespace {

int terminal(const lark_table::Table& table, std::string_view name) {
  const int s = lark::symbol_of(table, name);
  if (s < 0) {
    throw InternalError("parser_context: no terminal " + std::string(name));
  }
  return s;
}

}  // namespace

// --- TagHead ---

TagHead::TagHead(const lark_table::Table& table)
    : struct_(terminal(table, "STRUCT")),
      union_(terminal(table, "UNION")),
      enum_kw_(terminal(table, "ENUM")),
      lparen_(terminal(table, "LPAREN")),
      rparen_(terminal(table, "RPAREN")),
      colon_(terminal(table, "COLON")),
      attr_open_(terminal(table, "ATTR_OPEN")),
      lbracket_(terminal(table, "LBRACKET")),
      rbracket_(terminal(table, "RBRACKET")),
      identifier_(terminal(table, "IDENTIFIER")),
      lbrace_(terminal(table, "LBRACE")) {
  for (std::string_view name :
       {"INT", "LONG", "SHORT", "SIGNED", "UNSIGNED", "FLOAT", "DOUBLE", "CHAR", "BOOL",
        "ALT_BOOL", "VOID", "BITINT", "TYPEDEF_NAME", "TYPEOF", "TYPEOF_UNQUAL", "CONST",
        "VOLATILE", "RESTRICT"}) {
    type_tokens_.push_back(terminal(table, name));
  }
}

bool TagHead::is_type_token(int kind) const {
  return std::find(type_tokens_.begin(), type_tokens_.end(), kind) != type_tokens_.end();
}

bool TagHead::step(int kind) {
  type_ended_ = false;
  if (kind == struct_ || kind == union_ || kind == enum_kw_) {
    active_ = true, depth_ = 0, tagged_ = false;
    enum_ = kind == enum_kw_;
    parens_.reset();
    return false;
  }
  if (!active_) {
    return false;
  }
  if (parens_) {
    if (*parens_ != 0) {
      *parens_ += kind == lparen_ ? 1 : kind == rparen_ ? -1 : 0;
      return false;
    }
    if (kind == lparen_) {
      parens_ = 1;
      return false;
    }
    if (is_type_token(kind)) {
      return false;
    }
    active_ = false, type_ended_ = true;
    return kind == lbrace_;
  }
  if (kind == colon_ && enum_ && depth_ == 0) {
    parens_ = 0;
    return false;
  }
  if (depth_ != 0 || kind == attr_open_) {
    if (kind == attr_open_) {
      depth_ += 2;
    } else if (kind == lbracket_) {
      depth_ += 1;
    } else if (kind == rbracket_) {
      depth_ -= 1;
    }
    return false;
  }
  if (kind == identifier_ && !tagged_) {
    tagged_ = true;
    return false;
  }
  active_ = false;
  return kind == lbrace_;
}

// --- ParseContext ---

ParseContext::ParseContext(const lark_table::Table& table) : tag_head(table) {
  scopes.push_back(std::make_shared<Scope>());
}

ScopePtr ParseContext::push_scope() {
  auto scope = std::make_shared<Scope>();
  scopes.push_back(scope);
  return scope;
}

void ParseContext::remove_scope(const ScopePtr& scope) {
  for (std::size_t i = scopes.size() - 1; i > 0; --i) {
    if (scopes[i] == scope) {
      scopes.erase(scopes.begin() + static_cast<std::ptrdiff_t>(i));
      return;
    }
  }
}

const Binding* ParseContext::lookup(std::string_view name) const {
  for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
    if (const Binding* b = (*it)->names.find(name)) {
      return b;
    }
  }
  return nullptr;
}

ScopePtr ParseContext::tag_scope(std::string_view tag) const {
  for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
    if ((*it)->tags.contains(std::string(tag))) {
      return *it;
    }
  }
  return nullptr;
}

std::optional<bool> ParseContext::tag_is_enum(std::string_view tag) const {
  for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
    if ((*it)->enum_tags.contains(tag)) {
      return true;
    }
    if ((*it)->tags.contains(std::string(tag))) {
      return false;
    }
  }
  return std::nullopt;
}

const c99_ast::Type_data_type& ParseContext::enum_tag_type(std::string_view tag) const {
  for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
    if (const EnumTag* e = (*it)->enum_tags.find(tag)) {
      return e->data_type;
    }
  }
  throw InternalError("no enum " + pyrepr::string(tag) + " in scope");
}

void ParseContext::declare(const std::string& name, Binding binding, ScopePtr scope) {
  if (scope == nullptr) {
    scope = scopes.back();
  }
  if (const Binding* old = scope->names.find(name)) {
    if (old->is_enumerator() || binding.is_enumerator()) {
      throw ParserError(pyrepr::string(name) +
                        " is declared again in the same scope, and one of the declarations "
                        "is an enumeration constant");
    }
    if (old->is_typedef() != binding.is_typedef()) {
      throw ParserError(pyrepr::string(name) +
                        " is declared both as a typedef name and as an object or function in "
                        "the same scope");
    }
    if (binding.is_typedef() && !(*old->data_type == *binding.data_type)) {
      throw ParserError("typedef " + pyrepr::string(name) +
                        " is declared again with a different type");
    }
  }
  scope->names.insert_or_assign(name, std::move(binding));
}

void ParseContext::introduce_tag(const std::string& tag) {
  if (tag_scope(tag) == nullptr) {
    scopes.back()->tags.insert(tag);
  }
}

// --- TokenStream ---

TokenStream::TokenStream(std::string_view source, ParseContext& ctx, std::string_view start,
                         StreamHooks hooks)
    : table_(lark_table::c99_table()),
      source_(source),
      text_(lark::decode_utf8(source)),
      ctx_(ctx),
      lexer_(text_, table_),
      recognizer_(table_, start),
      hooks_(std::move(hooks)),
      identifier_(terminal(table_, "IDENTIFIER")),
      typedef_name_(terminal(table_, "TYPEDEF_NAME")),
      lbrace_(terminal(table_, "LBRACE")),
      rbrace_(terminal(table_, "RBRACE")),
      for_(terminal(table_, "FOR")) {
  if (!hooks_.typedef_query) {
    hooks_.typedef_query = [this](const TokenStream&, std::string_view name, bool) {
      const Binding* b = ctx_.lookup(name);
      return b != nullptr && b->is_typedef();
    };
  }
  lex_next();
}

void TokenStream::lex_next() {
  std::optional<lark::Token> lt = lexer_.next_token();
  if (!lt) {
    // Lark's lexer ends, and the parser feeds `$END` at the last token's
    // position. Nothing in the context changes.
    lookahead_ = lark::end_token(last_real_ ? &*last_real_ : nullptr);
    at_end_ = true;
    return;
  }
  if (lexer::is_number_terminal(lt->type, table_)) {
    lexer::check_number(text_, *lt, table_);
  }
  ctx_.body_scope = nullptr;
  retyped_at_lex_ = false;
  if (lt->type == identifier_ && hooks_.typedef_query(*this, lt->value, false) &&
      recognizer_.can_shift(typedef_name_)) {
    // Lark's `Token.new_borrow_pos` makes a new token, so the lexer's
    // `last_token` stays the IDENTIFIER.
    lt->type = typedef_name_;
    retyped_at_lex_ = true;
  }
  const int kind = lt->type;
  // Retyped first, so `TagHead` can tell a typedef name in an enum's
  // underlying type from the declarator after it.
  const bool tag_body = ctx_.tag_head.step(kind);
  if (kind == lbrace_) {
    if (tag_body) {
      ctx_.brace_scopes.push_back(nullptr);
    } else {
      ScopePtr scope = ctx_.push_scope();
      ctx_.brace_scopes.push_back(scope);
      ctx_.body_scope = scope;
    }
  } else if (kind == rbrace_) {
    if (!ctx_.brace_scopes.empty()) {
      ScopePtr scope = ctx_.brace_scopes.back();
      ctx_.brace_scopes.pop_back();
      if (scope != nullptr) {
        ctx_.remove_scope(scope);
      }
    }
  } else if (kind == for_) {
    ctx_.for_scopes.push_back(ctx_.push_scope());
  }
  ctx_.recent = {ctx_.recent.second, kind};
  lookahead_ = std::move(*lt);
  last_real_ = lookahead_;
  ++lexed_;
}

lark::Token TokenStream::take() {
  lark::Token t = lookahead_;
  if (at_end_) {
    recognizer_.feed_end(t, hooks_.reduce_hook);
    return t;
  }
  recognizer_.feed(lookahead_, hooks_.reduce_hook);
  t = lookahead_;
  lex_next();
  return t;
}

void TokenStream::retype_lookahead(int rule_origin, const std::vector<std::uint16_t>& stack) {
  // At the end of the text Python's latest token is the last one lexed,
  // which the parser has already shifted, so retyping it changes nothing.
  if (at_end_ || lookahead_.type != identifier_) {
    return;
  }
  if (!hooks_.typedef_query(*this, lookahead_.value, true)) {
    return;
  }
  // The reduction in progress has popped its children's states but not
  // yet pushed the state for `rule_origin`.
  std::vector<std::uint16_t> s = stack;
  s.push_back(static_cast<std::uint16_t>(recognizer_.action_for(s.back(), rule_origin)));
  if (lark::Recognizer::can_shift_from(std::move(s), typedef_name_, table_)) {
    lookahead_.type = typedef_name_;
    // The token changes in place, and it is the lexer's `last_token`,
    // since it wasn't retyped when it was lexed.
    lexer_.set_last_token_type(typedef_name_);
  }
}

void TokenStream::end_for_scope() {
  ctx_.remove_scope(ctx_.for_scopes.back());
  ctx_.for_scopes.pop_back();
  if (at_end_) {
    return;
  }
  // Lark makes one rule of each form of the loop, with and without its
  // condition and its step, and gives each the alias `for_stmt`.
  int for_rule = -1;
  const bool reduced = recognizer_.reduce_through(lookahead_, [&](int rule) {
    if (std::string_view(table_.rule_alias[static_cast<std::size_t>(rule)]) == "for_stmt") {
      for_rule = rule;
      return true;
    }
    return false;
  });
  if (!reduced) {
    throw InternalError("end_for_scope: the lookahead doesn't end a for statement");
  }
  // The full reduction left the goto on `unlabeled_stmt` on the stack,
  // which is the stack parser.py's `retype_lookahead` computes.
  std::vector<std::uint16_t> s = recognizer_.stack();
  s.pop_back();
  retype_lookahead(table_.rule_origin[static_cast<std::size_t>(for_rule)], s);
}

// --- The syntax errors ---

namespace {

// The keywords C23 added (§6.4.1) that a C99 program could have used as
// names, by their terminals in c99.lark.
constexpr std::string_view kC23KeywordTerminals[] = {
    "BOOL", "TRUE", "FALSE", "NULLPTR", "CONSTEXPR", "THREAD_LOCAL", "TYPEOF", "TYPEOF_UNQUAL",
};

}  // namespace

std::optional<std::string> c23_keyword_as_name(const lark::UnexpectedToken& e,
                                               const ParseContext& ctx) {
  const lark_table::Table& table = lark_table::c99_table();
  const std::string_view type = table.name(e.token().type);
  const int before = ctx.recent.first;
  const std::string_view before_name = before < 0 ? std::string_view{} : table.name(before);
  std::string keyword;
  if (std::find(std::begin(kC23KeywordTerminals), std::end(kC23KeywordTerminals), type) !=
          std::end(kC23KeywordTerminals) &&
      e.expects("IDENTIFIER")) {
    keyword = e.token().value;
  } else if ((before_name == "BOOL" || before_name == "CONSTEXPR" ||
              before_name == "THREAD_LOCAL") &&
             (type == "ASSIGN" || type == "LBRACKET" || type == "COMMA")) {
    keyword = std::string(before_name);
    for (char& c : keyword) {
      c = static_cast<char>(c >= 'A' && c <= 'Z' ? c - 'A' + 'a' : c);
    }
  } else {
    return std::nullopt;
  }
  return "line " + std::to_string(e.line()) + ", column " + std::to_string(e.column()) + ": `" +
         keyword +
         "` is a keyword in C23 (§6.4.1), so it can't be the name of a variable, function, "
         "parameter, type, tag, member, or label";
}

std::optional<std::string> enum_type_without_list(const lark::UnexpectedToken& e,
                                                  const ParseContext& ctx) {
  if (!ctx.tag_head.type_ended() || !e.expects("LBRACE")) {
    return std::nullopt;
  }
  return "`enum e : T` without a list of constants can only be a whole declaration, `enum e : "
         "T;`, with no storage class, `typedef`, or declarator, and not in a struct, a `for`, a "
         "cast, or `sizeof` (C23 §6.7.3.3). Declare the tag that way, and then name the type as "
         "`enum e`. The parser stopped at " +
         pyrepr::string(e.token().value) + " on line " + std::to_string(e.line());
}

void raise_syntax_error(const CompileError& e, std::string_view source,
                        const ParseContext& ctx) {
  if (const auto* token = dynamic_cast<const lark::UnexpectedToken*>(&e)) {
    std::optional<std::string> message = c23_keyword_as_name(*token, ctx);
    if (!message) {
      message = enum_type_without_list(*token, ctx);
    }
    if (message) {
      throw ParserError(*message);
    }
  }
  if (source.find("__attribute__") != std::string_view::npos) {
    throw ParserError(std::string(e.what()) +
                      "\nc6502 doesn't take GCC's `__attribute__((...))`; it takes C23 "
                      "attributes, such as `[[c6502::zp_abi]]` before a declaration or `uint8_t "
                      "n [[c6502::in_x]]` after a name");
  }
  if (const auto* token = dynamic_cast<const lark::UnexpectedToken*>(&e)) {
    throw *token;
  }
  if (const auto* chars = dynamic_cast<const lark::UnexpectedCharacters*>(&e)) {
    throw *chars;
  }
  throw InternalError("raise_syntax_error: not a syntax error");
}

}  // namespace c6502::parser
