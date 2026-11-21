// The tables Lark computes from c99.lark, which cpp/generated/c99_lalr.cpp
// holds, written by tools/lalr_table.py: the LALR automaton that
// parser.py's Lark parser runs, with its states numbered canonically,
// and the terminals of its basic lexer in the order Lark tries them.
// lark_parser.h runs the automaton and lark_lexer.h the lexer.
//
// A symbol is a number. The terminals come first, numbered in the order
// of their names by code point, which is how parser.py sorts the
// terminals a syntax error lists, so `$END` is 0. The nonterminals
// follow, in the same order. A state's row lists the symbols it has an
// action for, in order, each with a shift (a goto, for a nonterminal)
// to a state or a reduction by a rule.
#pragma once

#include <cstdint>
#include <span>
#include <string_view>

namespace c6502::lark_table {

// A start rule: its name, the state a parse of it starts in, and the
// state that reducing the start symbol reaches, where a parse fed
// `$END` stops.
struct StartRule {
  const char* name;
  std::uint16_t start_state;
  std::uint16_t end_state;
};

// A terminal of the lexer. `symbol` is -1 for one the parser never sees
// (`WS`, which Lark ignores). `pattern` is a regular expression in
// Python's syntax when `is_regex`, and the terminal's string otherwise.
// `counts_newlines` is Lark's `newline_types`, the terminals whose
// pattern can match a newline, which alone move the line counter.
struct LexTerminal {
  const char* name;
  int symbol;
  bool is_regex;
  bool counts_newlines;
  const char* pattern;
};

// A keyword's spelling and its terminal, which Lark gives an
// IDENTIFIER that spells the keyword whole.
struct Keyword {
  const char* spelling;
  int symbol;
};

// The encoding of an entry of `entry_action`.
inline constexpr std::uint16_t kReduce = 0x8000;

struct Table {
  int terminal_count;
  std::span<const char* const> symbols;
  std::span<const std::uint32_t> row_start;
  std::span<const std::uint8_t> entry_symbol;
  std::span<const std::uint16_t> entry_action;
  std::span<const std::uint16_t> rule_origin;
  std::span<const std::uint16_t> rule_start;
  std::span<const std::uint8_t> rule_expansion;
  std::span<const char* const> rule_alias;
  std::span<const StartRule> starts;
  std::span<const LexTerminal> lex_terminals;
  std::span<const Keyword> keywords;
  // The terminals Lark's lexer lists in an UnexpectedCharacters (all
  // but the ignored `WS`), sorted.
  std::span<const char* const> allowed;

  int state_count() const { return static_cast<int>(row_start.size()) - 1; }
  int rule_count() const { return static_cast<int>(rule_origin.size()); }
  bool is_terminal(int symbol) const { return symbol < terminal_count; }
  std::string_view name(int symbol) const { return symbols[static_cast<std::size_t>(symbol)]; }
  int rule_length(int rule) const {
    return rule_start[static_cast<std::size_t>(rule) + 1] - rule_start[static_cast<std::size_t>(rule)];
  }
};

// Returns the tables of c99.lark.
const Table& c99_table();

}  // namespace c6502::lark_table
