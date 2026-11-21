// The recursive descent of parser.py's grammar, c99.lark, with the
// actions of its `_ASTBuilder`.
//
// Python builds the AST inside Lark's LALR parse, calling the action of
// each rule as the rule reduces. The C++ parser is a recursive descent
// written by hand over R1's `TokenStream` (parser_context.h), with one
// function per construct, which runs the construct's action when it has
// taken the construct's last token and before it takes the next one.
// The stream feeds each token it takes to a `lark::Recognizer` running
// Lark's table, and lexes the token after it at once, so three things
// come out as they do in Python.
//
// - A syntax error is found at the token where Lark finds it, with
//   Lark's terminals, because the recognizer is fed every token before
//   the descent reads it. The descent never reports a syntax error of
//   its own. Where it meets a token it has no use for, it takes the
//   token (`unexpected`), and the recognizer throws.
// - An action runs after the token that follows its construct has been
//   lexed and before that token is shifted, which is when Lark calls it,
//   since Lark reduces a rule when it is fed the token after the rule.
//   The scopes the lexer opens and closes, the names the actions
//   declare, and the typedef names the lexer retypes therefore follow
//   each other in Python's order.
// - An action that throws is checked against the recognizer
//   (`action`). Lark reduces a rule when it is fed the next token, and
//   when that token is a syntax error, Lark may report it before making
//   the reduction, so the action never runs. The descent ran the action
//   first, and `action` then reports the syntax error in its place.
//
// The work is in three files. This one's own functions, the start rule,
// the tokens, the guard of the actions, and the parser's folds of
// constant expressions through the type checker, are in
// parser_descent.cpp and parser_folds.cpp. Part R2 of the port, the
// declarations (parser.py:525-1678, 1991-2847, and the methods of
// 2849-3350, 3506-3606, and 3717-4169), is in parser_declarations.cpp,
// and part R3, the statements and expressions (1679-1990, 3352-3505,
// 3607-3716, and 4170-4673), is in parser_statements.cpp. Each part
// calls the other only through the functions declared here.
#pragma once

#include <cstddef>
#include <optional>
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
#include "lexer.h"
#include "parser_context.h"
#include "passes/type_checker.h"
#include "pp_numbers.h"

namespace c6502::parser {

using Exp = c99_ast::Type_exp;
using Type = c99_ast::Type_data_type;

// Python's `NotImplementedError`, which the parser raises for a
// variable-length array, a K&R parameter list, and `[static N]`, and
// which compile.py reports as a one-line error.
class NotImplemented : public CompileError {
 public:
  explicit NotImplemented(const std::string& message)
      : CompileError("NotImplementedError", message) {}
};

// The terminals of c99.lark, which the descent compares token types
// with, as `t.LPAREN`. `END` is Lark's `$END`, the end of the input.
#define C6502_PARSER_TERMINALS(X)                                                              \
  X(ALT_BOOL) X(ALT_THREAD_LOCAL) X(AMP) X(AMP_ASSIGN) X(ANDAND) X(ARROW) X(ASSIGN)            \
  X(ATTR_OPEN) X(AUTO) X(BANG) X(BITINT) X(BITINT_INTEGER) X(BOOL) X(BREAK) X(CARET)           \
  X(CARET_ASSIGN) X(CASE) X(CHAR) X(CHAR_CONSTANT) X(COLON) X(COLONCOLON) X(COMMA) X(COMPLEX)  \
  X(CONST) X(CONSTEXPR) X(CONTINUE) X(DEFAULT) X(DO) X(DOT) X(DOUBLE) X(DOUBLE_CONSTANT)       \
  X(ELLIPSIS) X(ELSE) X(ENUM) X(EQ) X(EXTERN) X(FALSE) X(FLOAT) X(FLOAT_CONSTANT) X(FOR) X(GE) \
  X(GOTO) X(GT) X(HASH) X(HASHHASH) X(IDENTIFIER) X(IF) X(IMAGINARY) X(INLINE) X(INT)          \
  X(INTEGER_CONSTANT) X(INVALID_NUMBER) X(LBRACE) X(LBRACKET) X(LE) X(LONG)                    \
  X(LONG_DOUBLE_CONSTANT) X(LONG_INTEGER) X(LPAREN) X(LSHIFT) X(LSHIFT_ASSIGN) X(LT) X(MINUS)  \
  X(MINUSMINUS) X(MINUS_ASSIGN) X(NE) X(NULLPTR) X(OROR) X(PERCENT) X(PERCENT_ASSIGN) X(PIPE)  \
  X(PIPE_ASSIGN) X(PLUS) X(PLUSPLUS) X(PLUS_ASSIGN) X(QUESTION) X(RBRACE) X(RBRACKET)          \
  X(REGISTER) X(RESTRICT) X(RETURN) X(RPAREN) X(RSHIFT) X(RSHIFT_ASSIGN) X(SEMICOLON)          \
  X(SHORT) X(SIGNED) X(SIZEOF) X(SLASH) X(SLASH_ASSIGN) X(STAR) X(STAR_ASSIGN) X(STATIC)       \
  X(STRING_LITERAL) X(STRUCT) X(SWITCH) X(THREAD_LOCAL) X(TILDE) X(TRUE) X(TYPEDEF)            \
  X(TYPEDEF_NAME) X(TYPEOF) X(TYPEOF_UNQUAL) X(UBITINT_INTEGER) X(UINT_INTEGER)                \
  X(ULONG_INTEGER) X(UNION) X(UNSIGNED) X(VOID) X(VOLATILE) X(WHILE)

struct Terminals {
  int END = 0;
#define C6502_PARSER_TERMINAL_FIELD(name) int name = -1;
  C6502_PARSER_TERMINALS(C6502_PARSER_TERMINAL_FIELD)
#undef C6502_PARSER_TERMINAL_FIELD

  // Looks each terminal up in `table` by name, and throws InternalError
  // for a name the table doesn't have.
  explicit Terminals(const lark_table::Table& table);
};

// Returns the terminals of `lark_table::c99_table()`.
const Terminals& terminals();

// The argument of an attribute that means something to c6502
// (parser.py's `_attribute_argument`): none, one integer constant, whose
// value may be any size, or the text of one string literal between its
// quotes.
struct AttributeArg {
  enum class Kind { kNone, kInt, kString };
  Kind kind = Kind::kNone;
  pp_numbers::IntegerConstant integer;
  std::string string;

  bool is_none() const { return kind == Kind::kNone; }
  bool operator==(const AttributeArg&) const = default;
};

// parser.py's `_Attributes`: the attributes of an attribute-specifier
// sequence that mean something to c6502, as `(name, arg)` pairs, its
// own (written `c6502::name`) and the standard ones. Attributes with any
// other prefix, and unprefixed names C23 doesn't define, are dropped as
// they are parsed, since C23 says an implementation ignores attributes
// it doesn't know.
struct Attributes {
  std::vector<std::pair<std::string, AttributeArg>> c6502;
  std::vector<std::pair<std::string, AttributeArg>> standard;
};

// The value of a `declaration` or a `var_decl`: the c99 declarations it
// gives, an inline struct or union body's `StructDecl` first, and
// whether `[[c6502::save_hardware_stack]]` came before it, which makes
// it parser.py's `_HwStackSaveDecls`. A block marks the calls in such a
// declaration, and the file scope and a `for` statement's first clause
// reject it.
struct DeclList {
  std::vector<c99_ast::Type_declaration> decls;
  bool hw_stack_save = false;
};

// What parser.py's `_parse_integer_token` returns for an integer
// constant's spelling: the constant, whether its base is hexadecimal,
// binary, or octal (Python's "hex_oct", which shares the type list of
// C99 §6.4.4.1p5), and whether its suffix holds `LL` or `ll`.
struct IntegerToken {
  pp_numbers::IntegerConstant constant;
  bool hex_oct = false;
  bool has_ll = false;
};

class Descent {
 public:
  // Starts the parse of `source` from the start rule `start`, with the
  // parse's context `ctx`, which the caller keeps for the rewording of a
  // syntax error. Lexes the first token, which can throw what lexing
  // throws.
  Descent(std::string_view source, ParseContext& ctx, std::string_view start = "start");
  Descent(const Descent&) = delete;
  Descent& operator=(const Descent&) = delete;

  // --- The start rule and the tokens (parser_descent.cpp) ---

  // `start: declaration*`, with its action, which flattens the
  // declarations' lists and rejects a declaration that
  // `[[c6502::save_hardware_stack]]` comes before at file scope. Takes
  // the `$END` after the last one.
  c99_ast::Program parse_start();

  // The lookahead's terminal, and the lookahead.
  int kind() const { return stream_.peek().type; }
  const lark::Token& peek() const { return stream_.peek(); }
  bool at(int terminal) const { return kind() == terminal; }

  // Takes the lookahead: feeds it to the recognizer, which throws a
  // syntax error when the table has no action for it, and lexes the
  // next token. Returns the token taken, with the type the recognizer
  // was fed, which a reduction can have changed.
  lark::Token take() { return stream_.take(); }

  // Takes the lookahead, which the grammar requires to be `terminal`
  // here. The recognizer throws for any token that can't come next, so
  // a token of another terminal that it accepts shows that the descent
  // doesn't follow the grammar, and this throws InternalError for it.
  lark::Token expect(int terminal);

  // Takes the lookahead, which the descent has no use for. The
  // recognizer throws its syntax error, and when it doesn't, the descent
  // doesn't follow the grammar, and this throws InternalError.
  [[noreturn]] void unexpected(std::string_view where);

  // Returns whether the lookahead can begin `decl_specs`: a storage
  // class, a qualifier, a type specifier, a typedef name, or `typeof`.
  // An `[[` before a declaration is the caller's to take first.
  bool at_declaration_specifier() const;
  // Returns whether the lookahead can begin a specifier-qualifier list,
  // and so a `type_name`: a qualifier, a type specifier, a typedef name,
  // or `typeof`.
  bool at_type_name() const;
  // Returns whether the lookahead is one of the storage-class
  // specifiers a compound literal may have after its `(` (the grammar's
  // `compound_literal_storage`).
  bool at_compound_literal_storage() const;

  // Runs `body`, the action of a construct of the rule named `rule` (an
  // alternative's alias, or the rule's name, as Lark names the
  // callback), and returns what it returns. When it throws a
  // CompileError, Lark may have found a syntax error in the lookahead
  // before it reduced the rule and called the action, and then the
  // syntax error is thrown in its place (`throw_syntax_error_before`).
  // An action takes no token.
  template <class F>
  decltype(auto) action(std::string_view rule, F&& body) {
    try {
      return body();
    } catch (const lark::UnexpectedToken&) {
      throw;
    } catch (const lark::UnexpectedCharacters&) {
      throw;
    } catch (const CompileError&) {
      throw_syntax_error_before(rule);
      throw;
    }
  }

  // Throws the UnexpectedToken that Lark raises for the lookahead when
  // it finds the lookahead to be a syntax error before it reduces a rule
  // named `rule`, and returns otherwise. Runs the lookahead's reductions
  // on a copy of the recognizer.
  void throw_syntax_error_before(std::string_view rule) const;

  TokenStream& stream() { return stream_; }
  ParseContext& ctx() { return ctx_; }
  const ParseContext& ctx() const { return ctx_; }
  const lark_table::Table& table() const { return table_; }

  const Terminals& t;

  // --- The parser's folds (parser_folds.cpp) ---

  // parser.py's `_fold_integer_constant`: the value of the integer
  // constant expression `exp`. The type checker stamps types and
  // implicit conversions on a copy of the expression, with the named
  // constants in scope, and the constant evaluator folds it. Throws
  // ParserError with the checker's or the evaluator's message when it
  // isn't one.
  asdl::Int fold_integer_constant(const Exp& exp);

  // parser.py's `_checker_with_named_constants`: a type checker holding
  // the named constants in scope that `exp` names (`Binding::constant`),
  // as objects of their types with their values. When one is a
  // structure or a union, or `layouts` is set, it has the layouts of the
  // struct and union bodies parsed so far too, a tag declared again in
  // an inner scope with its latest body.
  passes::type_checking::TypeChecker checker_with_named_constants(const Exp& exp, bool layouts);

  // parser.py's `_expression_type`: the type of the expression `exp` for
  // `keyword` (`typeof`, `typeof_unqual`, or `auto`), which the type
  // checker works out on a copy that has the types of the names in
  // scope. `what` names `exp` in an error, by default as the keyword's
  // operand.
  Type expression_type(const Exp& exp, std::string_view keyword,
                       const std::optional<std::string>& what = std::nullopt);

  // parser.py's `_check_typeof_tags`: throws ParserError when a
  // struct/union tag in the type of `name`, which `binding` gives, is
  // hidden here by an inner declaration of the tag.
  void check_typeof_tags(const Binding& binding, std::string_view keyword,
                         std::string_view name) const;

  // parser.py's `_fold_named_constant`: `(type, value)` of an object
  // declared `constexpr` with the type `data_type` and the initializer
  // `init`, or nothing when the parser can't fold it. The type checker
  // checks the declaration again later and reports what's wrong with
  // it, so a failure here isn't an error.
  std::optional<std::pair<Type, passes::ValueTree>> fold_named_constant(const Type& data_type,
                                                                        const Exp& init);

  // --- Part R2, the declarations (parser_declarations.cpp) ---

  // `attribute_specifier_seq` at an `[[`, with the actions of its
  // specifiers, attributes, and arguments: one `Attributes` for them all.
  Attributes parse_attribute_specifier_seq();

  // `declaration` (`function_decl`, `var_decl` with its alternative
  // `tag_only_decl`, `attribute_declaration`, or `enum_tag_decl`) after
  // its leading attributes, `prefix`, which the caller has taken, since
  // a block item that starts with attributes can be a statement too.
  // Returns the value of the alternative's action, which `declaration`
  // passes on.
  DeclList parse_declaration(std::optional<Attributes> prefix);

  // `var_decl`, or its alternative `tag_only_decl`, after its leading
  // attributes, for the first clause of a `for` statement.
  DeclList parse_var_decl(std::optional<Attributes> prefix);

  // `type_name`, with its action: the type a cast, `sizeof`, `typeof`,
  // or a compound literal names.
  Type parse_type_name();

  // parser.py's `_literal_storage`: the storage class and whether the
  // literal is `constexpr`, for a compound literal with the
  // storage-class specifiers `tokens` (C23 §6.5.3.6).
  std::pair<std::optional<c99_ast::Type_storage_class>, bool> literal_storage(
      const std::vector<lark::Token>& tokens);

  // parser.py's `_constexpr_type`: the type of an object declared
  // `constexpr`, which is `const` (C23 §6.7.2p16), with an array's
  // elements taking the qualifier.
  Type constexpr_type(const Type& data_type);

  // parser.py's `_enum_constant`: the expression that replaces a use of
  // an enumeration constant of value `value`, whose enum has the fixed
  // underlying type `enum_type`, or none for an `int` constant.
  Exp enum_constant(const asdl::Int& value, const std::optional<Type>& enum_type);

  // parser.py's `_apply_statement_attributes`: applies the attributes
  // before the unlabeled statement `stmt` to it.
  void apply_statement_attributes(const Attributes& attrs, c99_ast::Type_statement& stmt);

  // parser.py's `_check_label_attributes`: checks the attributes before
  // a label, which C23 §6.8.2 gives to the label.
  void check_label_attributes(const Attributes& attrs);

  // --- Part R3, the statements and expressions (parser_statements.cpp) ---

  // `exp`, the comma expression.
  Exp parse_exp();
  // `assignment_exp`.
  Exp parse_assignment_exp();
  // `constant_exp`, with its action, which passes the expression on.
  Exp parse_constant_exp();
  // `initializer` (`init_exp` or `init_list`) with its action.
  Exp parse_initializer();
  // `block` at its `{`, with its action.
  c99_ast::Block parse_block();
  // `statement`, with the actions of its alternatives.
  c99_ast::Type_statement parse_statement();

  // parser.py's `_parse_integer_token`: the constant an integer
  // constant's spelling `text` gives. Throws ParserError with
  // pp_numbers' message when the spelling isn't one.
  IntegerToken parse_integer_token(std::string_view text);

  // --- R2's helpers, which part R2 declares here as it needs them ---

  // --- (keep R3's below, a few lines apart, so the parts merge) ---

  // ---

  // --- R3's helpers, which part R3 declares here as it needs them ---

  // `statement` after the attributes `attrs` that came before it, which
  // a block item takes first, since they can begin a declaration too.
  c99_ast::Type_statement parse_statement_after(std::optional<Attributes> attrs);
  // The `unlabeled_stmt` alternatives.
  c99_ast::Type_statement parse_unlabeled_statement();
  // `for_stmt`, at its `for`.
  c99_ast::Type_statement parse_for_statement();
  // One `block_item`, with the action of `stmt_item` or `decl_item`,
  // appended to `items`.
  void parse_block_item(std::vector<c99_ast::Type_block_item>& items);
  // `conditional_exp`.
  Exp parse_conditional_exp();
  // The left-associative binary levels of the grammar, from
  // `logical_or_exp` (level 0) to `mul_exp` (level 9).
  Exp parse_binary_exp(int level);
  // `cast_exp`.
  Exp parse_cast_exp();
  // `unary_exp`.
  Exp parse_unary_exp();
  // The postfix operators after `operand`, which is a `postfix_exp`.
  Exp parse_postfix_ops(Exp operand);
  // The expression that starts at a `(`: a cast when `cast_allowed`, a
  // compound literal, or a parenthesized expression, with the postfix
  // operators after the last two.
  Exp parse_parenthesized(bool cast_allowed);
  // An `atom` other than a parenthesized expression.
  Exp parse_atom();
  // The rest of a compound literal of type `target`, from its `{`, with
  // the storage-class specifiers `storage` that came after its `(`.
  Exp parse_compound_literal_rest(Type target, const std::vector<lark::Token>& storage);
  // The items of a braced list, from the `{` through the `}`.
  std::vector<Exp> parse_braced_items();
  // An `init_item`: an initializer, or a `designation`.
  Exp parse_init_item();
  // The action of `identifier` for the token `name`.
  Exp identifier_exp(const lark::Token& name);
  // The action of `const` and `constant` for the token `token`.
  Exp constant_exp_of(const lark::Token& token);

  // A primary expression the statement parser has already taken, an
  // identifier at the start of a statement that turned out not to be a
  // label, which `parse_atom` returns before it reads a token.
  std::optional<Exp> pending_primary;

 private:
  ParseContext& ctx_;
  const lark_table::Table& table_;
  TokenStream stream_;
};

// Calls `f` on `exp` and on each expression within it, in the order of
// the fields of Python's dataclasses, each before the expressions
// within it, as parser.py's walks over `dataclasses.fields` visit them.
// `E` is `Exp` or `const Exp`.
template <class E, class F>
void walk_exp(E& exp, F&& f);

// Calls `f` on each expression in `stmt`, as `walk_exp` does, the
// statements and declarations within it included.
template <class S, class F>
void walk_statement_exps(S& stmt, F&& f);

// parser.py's `_mark_hw_stack_save`: sets `hw_stack_save` on every call
// of a function other than a builtin, and on every indirect call, in
// `stmt` or `decls`, which `[[c6502::save_hardware_stack]]` comes
// before. Throws ParserError when there is no call.
void mark_hw_stack_save(c99_ast::Type_statement& stmt);
void mark_hw_stack_save(std::vector<c99_ast::Type_declaration>& decls);

// parser.py's `_names_in`: the name of each `Var` in `exp`, and of each
// function a `FunctionCall` calls by name, each once, in the order the
// walk meets them. Python returns a set, whose order of iteration
// follows the hash seed, and `expression_type` reads it in this order.
std::vector<std::string> names_in(const Exp& exp);

// parser.py's `_named_literals`: gives each compound literal in `exp`
// that has no name the name `@foldN.compound_literal`, from N = 1 in the
// order of the walk, so two literals in a copy the parser folds don't
// share one value, which the type checker keeps by name for a
// `constexpr` one.
void name_literals(Exp& exp);

// parser.py's `_has_literal`: whether `exp` has a compound literal in it.
bool has_literal(const Exp& exp);

// parser.py's `_unqualified`: `t` without its outer `const` and
// `volatile`.
const Type& unqualified(const Type& t);

}  // namespace c6502::parser

#include "parser_walk.h"
