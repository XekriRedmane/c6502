// The port of Lark's LALR `ParserState` and `UnexpectedToken`
// (lark_parser.h).
#include "lark_parser.h"

#include <algorithm>

#include "pyrepr.h"

namespace c6502::lark {

namespace {

std::string token_message(const Token& token, const std::vector<int>& expected,
                          const std::vector<int>& accepts, const lark_table::Table& table) {
  return "Unexpected token " + token_repr(token, table) + " at line " +
         std::to_string(token.line) + ", column " + std::to_string(token.column) + ".\n" +
         format_expected(accepts.empty() ? expected : accepts, table);
}

}  // namespace

std::string format_expected(const std::vector<int>& names, const lark_table::Table& table) {
  std::string m = "Expected one of: \n\t* ";
  for (std::size_t i = 0; i < names.size(); ++i) {
    if (i != 0) {
      m += "\n\t* ";
    }
    m += table.name(names[i]);
  }
  return m + "\n";
}

UnexpectedToken::UnexpectedToken(const Token& token, std::vector<int> expected,
                                 std::vector<int> accepts, const lark_table::Table& table)
    : CompileError("UnexpectedToken", token_message(token, expected, accepts, table)),
      token_(token),
      expected_(std::move(expected)),
      accepts_(std::move(accepts)),
      table_(&table) {}

bool UnexpectedToken::expects(std::string_view name) const {
  return std::any_of(expected_.begin(), expected_.end(),
                     [&](int s) { return table_->name(s) == name; });
}

Recognizer::Recognizer(const lark_table::Table& table, std::string_view start) : table_(table) {
  for (const auto& s : table.starts) {
    if (start == s.name) {
      stack_.push_back(s.start_state);
      end_state_ = s.end_state;
      return;
    }
  }
  throw InternalError("Recognizer: no start rule " + std::string(start));
}

int Recognizer::action_for(int state, int symbol) const { return action_in(table_, state, symbol); }

int action_in(const lark_table::Table& table, int state, int symbol) {
  const auto row = static_cast<std::size_t>(state);
  const std::uint32_t lo = table.row_start[row];
  const std::uint32_t hi = table.row_start[row + 1];
  const auto* begin = table.entry_symbol.data() + lo;
  const auto* end = table.entry_symbol.data() + hi;
  const auto* it = std::lower_bound(begin, end, static_cast<std::uint8_t>(symbol));
  if (it == end || *it != symbol) {
    return -1;
  }
  return table.entry_action[static_cast<std::size_t>(it - table.entry_symbol.data())];
}

UnexpectedToken Recognizer::unexpected(const Token& token) const {
  std::vector<int> expected;
  const auto row = static_cast<std::size_t>(top());
  for (std::uint32_t i = table_.row_start[row]; i < table_.row_start[row + 1]; ++i) {
    const int s = table_.entry_symbol[i];
    if (table_.is_terminal(s)) {
      expected.push_back(s);
    }
  }
  return UnexpectedToken(token, std::move(expected), accepts(), table_);
}

void Recognizer::reduce(int rule, Token& token, ReduceHook* hook) {
  const auto size = static_cast<std::size_t>(table_.rule_length(rule));
  stack_.resize(stack_.size() - size);
  if (hook != nullptr) {
    hook->reduced(rule, token, *this);
  }
  const int go = action_for(top(), table_.rule_origin[static_cast<std::size_t>(rule)]);
  if (go < 0 || (go & lark_table::kReduce) != 0) {
    throw InternalError("Recognizer: no goto after a reduction");
  }
  stack_.push_back(static_cast<std::uint16_t>(go));
}

void Recognizer::feed(Token& token, ReduceHook* hook) {
  while (true) {
    const int action = action_for(top(), token.type);
    if (action < 0) {
      throw unexpected(token);
    }
    if (action == end_state_) {
      throw InternalError("Recognizer: a shift to the end state");
    }
    if ((action & lark_table::kReduce) == 0) {
      stack_.push_back(static_cast<std::uint16_t>(action));
      return;
    }
    reduce(action & ~lark_table::kReduce, token, hook);
  }
}

void Recognizer::feed_end(Token& token, ReduceHook* hook) {
  while (true) {
    const int action = action_for(top(), token.type);
    if (action < 0) {
      throw unexpected(token);
    }
    if ((action & lark_table::kReduce) == 0) {
      throw InternalError("Recognizer: a shift of $END");
    }
    reduce(action & ~lark_table::kReduce, token, hook);
    if (top() == end_state_) {
      return;
    }
  }
}

bool Recognizer::can_shift(int terminal) const { return can_shift_from(stack_, terminal, table_); }

bool Recognizer::can_shift_from(std::vector<std::uint16_t> stack, int terminal,
                                const lark_table::Table& table) {
  // Walks the table on a copy of the stack, so no reduction is made.
  while (true) {
    const int action = action_in(table, stack.back(), terminal);
    if (action < 0) {
      return false;
    }
    if ((action & lark_table::kReduce) == 0) {
      return true;
    }
    const int rule = action & ~lark_table::kReduce;
    stack.resize(stack.size() - static_cast<std::size_t>(table.rule_length(rule)));
    const int go = action_in(table, stack.back(), table.rule_origin[static_cast<std::size_t>(rule)]);
    stack.push_back(static_cast<std::uint16_t>(go));
  }
}

bool Recognizer::can_end() const {
  std::vector<std::uint16_t> stack = stack_;
  while (true) {
    const int action = action_in(table_, stack.back(), 0);
    if (action < 0 || (action & lark_table::kReduce) == 0) {
      return false;
    }
    const int rule = action & ~lark_table::kReduce;
    stack.resize(stack.size() - static_cast<std::size_t>(table_.rule_length(rule)));
    stack.push_back(static_cast<std::uint16_t>(
        action_in(table_, stack.back(), table_.rule_origin[static_cast<std::size_t>(rule)])));
    if (stack.back() == end_state_) {
      return true;
    }
  }
}

std::vector<int> Recognizer::accepts() const {
  // Lark feeds each terminal of the state's row to a copy of the parser
  // (`InteractiveParser.feed_token`), and keeps those that it shifts, and
  // `$END` when its reductions reach the end state, since that call
  // feeds `$END` as the end of the input. A shift to the end state would
  // be an assertion in Lark, which the table can't hold, since only the
  // goto on the start symbol reaches that state.
  std::vector<int> out;
  const auto row = static_cast<std::size_t>(top());
  for (std::uint32_t i = table_.row_start[row]; i < table_.row_start[row + 1]; ++i) {
    const int s = table_.entry_symbol[i];
    if (table_.is_terminal(s) && (s == 0 ? can_end() : can_shift(s))) {
      out.push_back(s);
    }
  }
  return out;
}

std::string rule_text(int rule, const lark_table::Table& table) {
  const auto r = static_cast<std::size_t>(rule);
  std::string out(table.name(table.rule_origin[r]));
  out += " :";
  for (std::uint16_t i = table.rule_start[r]; i < table.rule_start[r + 1]; ++i) {
    out += " ";
    out += table.name(table.rule_expansion[i]);
  }
  return out;
}

int rule_with_alias(const lark_table::Table& table, std::string_view alias) {
  for (int r = 0; r < table.rule_count(); ++r) {
    if (alias == table.rule_alias[static_cast<std::size_t>(r)]) {
      return r;
    }
  }
  return -1;
}

int any_symbol_of(const lark_table::Table& table, std::string_view name) {
  for (std::size_t i = 0; i < table.symbols.size(); ++i) {
    if (table.symbols[i] == name) {
      return static_cast<int>(i);
    }
  }
  return -1;
}

}  // namespace c6502::lark
