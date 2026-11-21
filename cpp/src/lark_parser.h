// The port of the part of Lark 1.3.1's LALR parser that c99.lark's
// parse needs without its tree: `ParserState.feed_token` running the
// table of lark_table.h, the `UnexpectedToken` it raises with the
// terminals Lark lists, and parser.py's `_can_shift`.
//
// The C++ parser is a recursive descent (docs/cpp_port.md, "The plan
// for the parser"), and it feeds each token to a `Recognizer` before it
// takes the token. The recognizer makes the reductions Lark makes,
// including those of merged lookaheads, so it stops at the token where
// Lark stops, in the state where Lark lists the terminals it expected.
#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "errors.h"
#include "lark_lexer.h"
#include "lark_table.h"

namespace c6502::lark {

// Lark's `UnexpectedToken`: the parser has no action for a token in the
// state it reached. `expected` is the terminals of that state's row,
// and `accepts` those of them that the parser would shift after its
// reductions (Lark's `accepts`), both sorted by name, as parser.py's
// `_sort_expected` sorts them. The message is Lark's:
//
//     Unexpected token Token('RETURN', 'return') at line 3, column 5.
//     Expected one of:
//     	* AMP
//     	...
//
// with `accepts` listed, or `expected` when `accepts` is empty.
class UnexpectedToken : public CompileError {
 public:
  UnexpectedToken(const Token& token, std::vector<int> expected, std::vector<int> accepts,
                  const lark_table::Table& table);

  const Token& token() const { return token_; }
  const std::vector<int>& expected() const { return expected_; }
  const std::vector<int>& accepts() const { return accepts_; }
  // Returns whether `expected` holds the terminal named `name`.
  bool expects(std::string_view name) const;
  int line() const { return token_.line; }
  int column() const { return token_.column; }

 private:
  Token token_;
  std::vector<int> expected_;
  std::vector<int> accepts_;
  const lark_table::Table* table_;
};

// Returns Lark's `_format_expected` of the terminals `names`.
std::string format_expected(const std::vector<int>& names, const lark_table::Table& table);

// The state stack of an LALR parse, Lark's `ParserState` without its
// value stack.
class Recognizer {
 public:
  // A function the recognizer calls at each reduction, with the rule,
  // after it pops the rule's states and before it pushes the goto, which
  // is when Lark calls the rule's callback. The callback may change the
  // type of the token being fed, which the recognizer reads again after
  // the reduction, as Lark does.
  struct ReduceHook {
    virtual ~ReduceHook() = default;
    virtual void reduced(int rule, Token& token, const Recognizer& recognizer) = 0;
  };

  // Starts a parse of the start rule `start` ("start", "lex_only",
  // "declarator", or "type_name").
  explicit Recognizer(const lark_table::Table& table, std::string_view start = "start");

  // Feeds `token`: makes the reductions its type calls for and shifts it.
  // Throws UnexpectedToken where the table has no action for it. The
  // reductions made before the error stay made, as in Lark.
  void feed(Token& token, ReduceHook* hook = nullptr);

  // Feeds `$END` (`end_token`), reducing until the start rule is
  // reduced. Throws UnexpectedToken when the input can't end here.
  void feed_end(Token& token, ReduceHook* hook = nullptr);

  // Makes the reductions that `token` calls for, up to and including the
  // first reduction by a rule for which `stop(rule)` is true, and returns
  // whether it made one. The rest of `token`'s reductions and its shift
  // happen when it is fed. Throws UnexpectedToken as `feed` does when the
  // table has no action for the token on the way.
  template <class Stop>
  bool reduce_through(Token& token, Stop stop) {
    while (true) {
      const int action = action_for(top(), token.type);
      if (action < 0) {
        throw unexpected(token);
      }
      if ((action & lark_table::kReduce) == 0) {
        return false;
      }
      const int rule = action & ~lark_table::kReduce;
      reduce(rule, token, nullptr);
      if (stop(rule)) {
        return true;
      }
    }
  }

  // Returns whether the parser would shift a token of the terminal
  // `terminal` next, after whatever reductions it calls for, without
  // making them (parser.py's `_can_shift`).
  bool can_shift(int terminal) const;

  // Returns whether a parser with the state stack `stack` would shift
  // `terminal` next, as `can_shift` does for the recognizer's own stack.
  static bool can_shift_from(std::vector<std::uint16_t> stack, int terminal,
                             const lark_table::Table& table);

  // Returns whether the input could end here: whether feeding `$END`
  // reduces to the end state.
  bool can_end() const;

  // Returns the terminals that the parser would shift next after its
  // reductions, and `$END` when the input could end here (Lark's
  // `InteractiveParser.accepts`), sorted.
  std::vector<int> accepts() const;

  // Returns the action of `state` on `symbol`: a state to shift or go
  // to, `kReduce` plus a rule, or -1 when there is none.
  int action_for(int state, int symbol) const;

  int top() const { return stack_.back(); }
  const std::vector<std::uint16_t>& stack() const { return stack_; }
  const lark_table::Table& table() const { return table_; }

  // Returns the UnexpectedToken for `token` in the current state.
  UnexpectedToken unexpected(const Token& token) const;

 private:
  void reduce(int rule, Token& token, ReduceHook* hook);

  const lark_table::Table& table_;
  std::vector<std::uint16_t> stack_;
  int end_state_ = 0;
};

// Returns the action of `state` on `symbol` in `table`, as
// `Recognizer::action_for` does.
int action_in(const lark_table::Table& table, int state, int symbol);

// Returns the rule `rule` as Lark prints one, `origin : sym sym`, for
// messages and tests.
std::string rule_text(int rule, const lark_table::Table& table);

// Returns the first rule whose alias is `alias`, or -1.
int rule_with_alias(const lark_table::Table& table, std::string_view alias);

// Returns the symbol named `name` (a terminal or a nonterminal), or -1.
int any_symbol_of(const lark_table::Table& table, std::string_view name);

}  // namespace c6502::lark
