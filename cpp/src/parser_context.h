// The parts of parser.py that run beside Lark's parse: the parser's
// view of the program's scopes (`_Scope`, `_Binding`, `_ParseContext`),
// the tracking of struct, union, and enum heads (`_TagHead`), the lexer
// that retypes a typedef name and opens and closes scopes
// (`_TypedefLexer`), and the rewording of syntax errors that `parse`
// applies (`_c23_keyword_as_name`, `_enum_type_without_list`, and the
// note about GCC's `__attribute__`).
//
// The C++ parser is a recursive descent written by hand (docs/cpp_port.md,
// "The plan for the parser"). `TokenStream` gives it the tokens as
// parser.py's Lark parser receives them, and feeds each one it takes to
// a `lark::Recognizer`, which runs Lark's table, so that a syntax error
// is reported at Lark's token, with Lark's message.
//
// The order of the actions matters, because the lexer reads one token
// ahead of the reductions. In Lark, a token is lexed when the token
// before it has been shifted, and the action of a construct that ends
// at a token runs when the token after it is fed. `TokenStream::take`
// feeds the lookahead and lexes the token after it at once, so a
// descent that runs a construct's action when the construct returns,
// after taking its last token and before taking the next, keeps that
// order. The lexer decides at each identifier whether it is a typedef
// name from the context's scopes as they are then, so a declaration's
// action has to declare its name before the token after its declarator
// is taken, as parser.py's `init_declarator` does.
#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "c99_ast.h"
#include "errors.h"
#include "lark_lexer.h"
#include "lark_parser.h"
#include "lark_table.h"
#include "ordered_map.h"
#include "passes/type_checking.h"

namespace c6502::parser {

// parser.py's `ParserError`: a construct the grammar accepts and C, or
// c6502, rejects, and the syntax errors `parse` rewords.
class ParserError : public CompileError {
 public:
  explicit ParserError(const std::string& message) : CompileError("ParserError", message) {}
};

struct Scope;
// A scope is compared by identity (`remove_scope`, a binding's
// `tag_scopes`), so it is held by a shared pointer, and a scope that
// is still named somewhere keeps its address.
using ScopePtr = std::shared_ptr<Scope>;

// An enum tag in one scope: the enum's type, whether the type was fixed
// by a `:` (C23 §6.7.3.3), and whether its list of constants has been
// parsed. Only an enum with a fixed underlying type can be declared
// without its list, as `enum e : int;`.
struct EnumTag {
  c99_ast::Type_data_type data_type;
  bool fixed = false;
  bool defined = false;
};

// What an ordinary identifier (C99 §6.2.3) means in one scope, as far as
// the parser needs to know: a typedef name, with the type it names, an
// enumeration constant, with its value, or anything else (an object, a
// function, a parameter). `tag_scopes` maps each struct/union tag in a
// typedef's type to the scope whose declaration of the tag the type
// refers to, which is null where no declaration of the tag was visible.
//
// `constant` is `(type, value)` for a scalar object declared `constexpr`
// (C23 §6.7.2) whose initializer the parser could fold, with the value
// in the form a static initializer's value has, so the constant
// expressions the parser folds can use the name. The object is still an
// object, which keeps its uses in the AST.
//
// `object_type` is the type of an object, a function, or a parameter
// (adjusted to a pointer for an array or a function), for a `typeof` of
// an expression that names it. `tag_scopes` maps the struct/union tags
// in it to their scopes too.
//
// `enum_type` is the type of an enumeration constant whose enum has a
// fixed underlying type (C23 §6.7.3.3), which is that type, and nothing
// for the `int` constants of any other enum. `enumerated` is true for a
// typedef name whose type is an enumerated type.
struct Binding {
  std::optional<c99_ast::Type_data_type> data_type;
  OrderedMap<std::string, ScopePtr> tag_scopes;
  std::optional<asdl::Int> enum_value;
  std::optional<std::pair<c99_ast::Type_data_type, passes::ValueTree>> constant;
  std::optional<c99_ast::Type_data_type> object_type;
  std::optional<c99_ast::Type_data_type> enum_type;
  bool enumerated = false;

  bool is_typedef() const { return data_type.has_value(); }
  bool is_enumerator() const { return enum_value.has_value(); }
};

// One scope of the parser's view of the program: the ordinary
// identifiers declared in it, the struct/union tags, and the enum tags.
// The three kinds of tag share one name space (C99 §6.2.3).
struct Scope {
  OrderedMap<std::string, Binding> names;
  std::set<std::string> tags;
  // Each enum tag's type (Int, or a fixed underlying type) and how it
  // was declared.
  OrderedMap<std::string, EnumTag> enum_tags;
};

// Follows the tokens from `struct`, `union`, or `enum` to the `{` of its
// body, if it has one, so the lexer knows that `{` doesn't start a
// block. In between there may be attributes and a tag, as in `struct
// [[deprecated]] s {`, and after an enum's tag, a `:` and its
// underlying type (C23 §6.7.3.3), as in `enum e : const uint8_t {` or
// `enum : typeof(x) {`.
class TagHead {
 public:
  explicit TagHead(const lark_table::Table& table);

  // Takes the next token's terminal, and returns true if it's the `{` of
  // a struct's, union's, or enum's body.
  bool step(int kind);

  // True when the latest token ended an enum's underlying type, for
  // `enum_type_without_list`.
  bool type_ended() const { return type_ended_; }

 private:
  bool is_type_token(int kind) const;

  bool active_ = false;
  int depth_ = 0;  // of the attributes' square brackets
  bool tagged_ = false;
  bool enum_ = false;
  // Inside an enum's underlying type: the depth of its parentheses, or
  // nothing before the `:`.
  std::optional<int> parens_;
  bool type_ended_ = false;
  int struct_, union_, enum_kw_, lparen_, rparen_, colon_, attr_open_, lbracket_, rbracket_,
      identifier_, lbrace_;
  // The tokens of an enum's underlying type outside parentheses, with a
  // typedef name already retyped. Any other token ends the head.
  std::vector<int> type_tokens_;
};

// Scope tracking while the parser runs, for typedef names.
//
// C's grammar depends on which identifiers name types: `T * x;` is a
// declaration when T is a typedef name and a multiplication when it
// isn't. So the parser follows the scopes as it reads (the "lexer
// hack"). `TokenStream` pushes a scope at each `{` that starts a block
// and at each `for`, pops the block's scope at its `}`, and retypes a
// typedef name to TYPEDEF_NAME. The parser's actions declare names: a
// typedef name, or an object, function, or parameter that hides one.
// The end of a `for` statement pops the scope of its `for`
// (`TokenStream::end_for_scope`).
//
// Enumeration constants never reach the AST. The parser declares each
// one in its scope with its value, and puts a constant in place of each
// use, so an array size or a case label can use one. The constant is an
// `int`, or for an enum with a fixed underlying type, of that type. Enum
// tags stay in the parser too, since the type is an integer type.
//
// The parser also tracks struct/union tags, the way identifier
// resolution does, for two reasons. A typedef that refers to a tag no
// declaration has introduced yet introduces it, and that has to survive
// into the AST, where the typedef declaration itself doesn't appear.
// And a typedef whose struct tag an inner declaration hides can't be
// expressed by putting its type in place of its name, which the parser
// reports.
//
// The fields of parser.py's `_ParseContext` that only the actions read
// and write (the stack of declaration specifiers, the counter of
// anonymous tags, the stack of enums' fixed types, and the struct
// bodies) are here for the parts of the parser that port them to fill
// in. The latest token and the parser state it was lexed in are
// `TokenStream`'s.
class ParseContext {
 public:
  explicit ParseContext(const lark_table::Table& table = lark_table::c99_table());

  std::vector<ScopePtr> scopes;
  // One entry per open `{`: the scope it pushed, or null for a
  // struct/union/enum body, which isn't a scope.
  std::vector<ScopePtr> brace_scopes;
  std::vector<ScopePtr> for_scopes;
  // The terminals of the last two tokens lexed, most recent last, or -1
  // before there were two.
  std::pair<int, int> recent{-1, -1};
  // The scope pushed by a `{` that is the latest token lexed. A function
  // definition's declarator ends when the parser sees the `{` of its
  // body, and the parameters go in this scope.
  ScopePtr body_scope;
  int anonymous_tags = 0;
  TagHead tag_head;
  // One entry per enum whose list of constants is being parsed: its
  // fixed underlying type, or nothing. An enum defined inside an
  // enumerator's value pushes its own entry, so each enumerator is
  // checked against its own enum's type.
  std::vector<std::optional<c99_ast::Type_data_type>> enum_types;
  // Each struct/union body parsed so far, so the constant expressions
  // the parser folds can use a member of a `constexpr` structure.
  std::vector<c99_ast::Type_struct_decl> struct_bodies;

  ScopePtr push_scope();
  // Removes `scope` from the stack, searching from the innermost scope
  // and never removing the file scope.
  void remove_scope(const ScopePtr& scope);
  // Returns the innermost binding of `name`, or null.
  const Binding* lookup(std::string_view name) const;
  // Returns the innermost scope that declares the struct/union tag
  // `tag`, or null.
  ScopePtr tag_scope(std::string_view tag) const;
  // Returns true when the innermost declaration of `tag` is an enum's,
  // false when it is a struct's or a union's, and nothing when none is
  // visible.
  std::optional<bool> tag_is_enum(std::string_view tag) const;
  // The type of the innermost enum `tag`, which must be visible.
  const c99_ast::Type_data_type& enum_tag_type(std::string_view tag) const;
  // Declares `name` in `scope` (the current one when null). C99 §6.7.3:
  // an identifier declared twice in one scope must be the same kind of
  // thing both times. A typedef name may be declared again with the
  // same type, which C11 §6.7.3 allows and headers rely on. Throws
  // ParserError otherwise.
  void declare(const std::string& name, Binding binding, ScopePtr scope = nullptr);
  // A struct/union specifier names `tag`. A tag that no declaration has
  // introduced yet comes into being in the current scope (C99
  // §6.7.2.3.8).
  void introduce_tag(const std::string& tag);
};

class TokenStream;

// What the checks of recorded token streams replace in a TokenStream.
struct StreamHooks {
  // Returns whether `name` names a typedef in scope, which the lexer
  // asks at each identifier (`retyping` false) and `retype_lookahead` at
  // the end of a `for` (`retyping` true), with the stream it asks for.
  // Empty, the stream looks the name up in the context.
  std::function<bool(const TokenStream& stream, std::string_view name, bool retyping)>
      typedef_query;
  // Called at each reduction of a token `take` feeds.
  lark::Recognizer::ReduceHook* reduce_hook = nullptr;
};

// The tokens of one parse, as parser.py's `_TypedefLexer` gives them to
// Lark's parser, and the recognizer they are fed to.
//
// The lexer is Lark's basic lexer, plus the scope tracking
// `ParseContext` describes. An IDENTIFIER that names a typedef in the
// current scope becomes a TYPEDEF_NAME, but only where the parser can
// shift one. Elsewhere, as the declarator in `int T;`, after `.` or
// `->`, or after `struct`, the name is an ordinary identifier.
//
// Lexing and feeding throw what Python's parse raises from them:
// `lexer::LexError` for a number the lexer checks, and
// `lark::UnexpectedCharacters` and `lark::UnexpectedToken`, which the
// parse passes to `raise_syntax_error`.
class TokenStream {
 public:
  // Starts the parse of `source`, which is UTF-8, from the start rule
  // `start`, and lexes the first token.
  TokenStream(std::string_view source, ParseContext& ctx, std::string_view start = "start",
              StreamHooks hooks = {});

  // The lookahead, the token the parser sees next, with the type it was
  // lexed as or retyped to. At the end of the text it is `$END`.
  lark::Token& peek() { return lookahead_; }
  bool at_end() const { return at_end_; }

  // Feeds the lookahead to the recognizer and returns it, and lexes the
  // token after it. At the end of the text, feeds `$END`, which finishes
  // the parse.
  lark::Token take();

  // The end of a `for` statement, which parser.py's `for_stmt` gives.
  // The loop is over, so the scope the lexer pushed at its `for` ends,
  // after the lexer has already lexed the token that follows. That
  // token was lexed while the loop's scope was still open, and the
  // name it spells may be a typedef name again, so the recognizer makes
  // the lookahead's reductions through the loop's (the reduction to
  // `unlabeled_stmt` by a `for_stmt` rule), and the lookahead is
  // retyped there (`retype_lookahead`). The rest of its reductions
  // happen when it is taken.
  void end_for_scope();

  // Lexes the lookahead again as a rule that reduces on seeing it, and
  // makes the nonterminal `rule_origin`, ends a scope. The recognizer's
  // `stack` is the one the reduction leaves after popping the rule's
  // states, before the goto on `rule_origin`. Only an identifier that
  // becomes a typedef name can need it: a scope that ends can hide a
  // typedef name but can't declare one the token could be.
  void retype_lookahead(int rule_origin, const std::vector<std::uint16_t>& stack);

  // The number of tokens lexed, the lookahead among them and `$END`
  // not.
  std::size_t lexed() const { return lexed_; }

  lark::Recognizer& recognizer() { return recognizer_; }
  const lark::Recognizer& recognizer() const { return recognizer_; }
  ParseContext& context() { return ctx_; }
  const ParseContext& context() const { return ctx_; }
  const std::u32string& text() const { return text_; }
  const lark_table::Table& table() const { return table_; }
  const std::string& source() const { return source_; }

  // The terminals the parser's lexer names.
  int typedef_name_symbol() const { return typedef_name_; }
  int identifier_symbol() const { return identifier_; }

 private:
  void lex_next();

  const lark_table::Table& table_;
  std::string source_;
  std::u32string text_;
  ParseContext& ctx_;
  lark::BasicLexer lexer_;
  lark::Recognizer recognizer_;
  StreamHooks hooks_;
  std::size_t lexed_ = 0;
  lark::Token lookahead_;
  bool at_end_ = false;
  // Whether the lookahead was retyped when it was lexed, which leaves
  // Lark's `last_token` an IDENTIFIER.
  bool retyped_at_lex_ = false;
  std::optional<lark::Token> last_real_;
  int identifier_, typedef_name_, lbrace_, rbrace_, for_;
};

// Returns parser.py's message for the syntax error `e` when it's `bool`,
// `true`, `false`, `nullptr`, `constexpr`, `thread_local`, `typeof`, or
// `typeof_unqual` where the parser could take a name, as in `int true;`,
// and nothing for any other error. C99 let a program use those as
// names, and C23 makes them keywords. `int bool = 1;` and `int constexpr
// = 1;` fail on the `=` after the keyword, since `int bool` reads as two
// type specifiers and `int constexpr` as a type and a storage-class
// specifier, and so does `int thread_local = 1;`.
std::optional<std::string> c23_keyword_as_name(const lark::UnexpectedToken& e,
                                               const ParseContext& ctx);

// Returns parser.py's message for the syntax error `e` when it's on the
// token after `enum e : T`, as in `enum e : int x;`, `(enum e : int)x`,
// or `enum e : int;` inside a struct, and nothing for any other error.
// C23 §6.7.3.3 allows `enum e : T` without a list of constants only as
// the whole declaration `enum e : T;`, which `enum_tag_decl` takes.
// `TagHead` marks the token that ends an enum's underlying type, and a
// `{` among the tokens the parser could take there shows that the type
// before it was complete.
std::optional<std::string> enum_type_without_list(const lark::UnexpectedToken& e,
                                                  const ParseContext& ctx);

// Throws what parser.py's `parse` raises for the syntax error `e`, an
// UnexpectedToken or an UnexpectedCharacters: a ParserError with the
// message of `c23_keyword_as_name` or `enum_type_without_list` when one
// applies, else `e` itself, whose terminals are already sorted, or,
// when the source holds `__attribute__` anywhere, a ParserError with
// `e`'s message and a note that c6502 takes C23 attributes. `source` is
// the text of the parse, and `ctx` its context.
[[noreturn]] void raise_syntax_error(const CompileError& e, std::string_view source,
                                     const ParseContext& ctx);

}  // namespace c6502::parser
