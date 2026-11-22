// A matcher for regular expressions with the syntax and semantics of
// Python 3.11's `re` module, for the patterns of c99.lark's terminals,
// which Lark's lexer runs with `re`, and for the pattern of the
// preprocessor's `--passthru-includes`, which pcpp compiles with
// `re.compile(pattern)` and matches against the text of an `#include`
// line with `.match`.
//
// Python's `re` is a backtracking matcher. An alternation takes the
// first alternative that lets the rest of the pattern match, and a
// greedy repeat gives back one iteration at a time until the rest
// matches. So the match it finds is the first in that order, which
// needn't be the longest, and `0(?:'?[0-7])*` matches only the `0` of
// `08`. `std::regex` has an ECMAScript mode with the same order, but its
// syntax, its errors, its classes, and its handling of a repeat whose
// body matches the empty string all differ from Python's. So this module
// parses a pattern as `re._parser` does, with its errors and their
// positions, and matches it as `_sre` does.
//
// The parser is a port of `re._parser` (sre_parse), and an invalid
// pattern throws `Error` with the class and message Python raises, such
// as `re.error` with "missing ), unterminated subpattern at position 0".
// The matcher takes everything Python's parser accepts except what
// `Unsupported` lists. It takes literals and every escape but
// `\N{name}`, `.`, classes with ranges, negation, and `\d \w \s` and
// their capitals, `\b \B \A \Z ^ $`, groups (capturing, non-capturing,
// named, and atomic), comments, alternation, greedy and lazy repeats
// with `{m,n}`, possessive repeats of one character or class, lookaheads
// and lookbehinds, and the inline flags `a`, `i`, `m`, `s`, `t`, `u`,
// and `x`, at the start or for a group. Under
// `i` it folds case as `_sre` does, with Python's tables of simple case
// mappings (pcpp_unicode.h), including `_sre`'s quirks, such as a class
// that holds a character outside the Basic Multilingual Plane comparing
// it with the subject's lowercase.
#pragma once

#include <exception>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

namespace c6502::py_regex {

// The exception that `re.compile` raises for an invalid pattern. `type`
// is the name of its class, which is "error" for `re.error`,
// "OverflowError" for a repeat count of 2**32 - 1 or more, and
// "ValueError" for the inline flags `a` and `u` together, and `message`
// is its `str()`.
struct Error : std::exception {
  std::string type;
  std::string message;

  Error(std::string t, std::string m) : type(std::move(t)), message(std::move(m)) {}
  const char* what() const noexcept override { return message.c_str(); }
};

// A pattern that Python compiles but that uses something the matcher
// doesn't take. Those are a backreference (`\1`, `(?P=name)`), a
// conditional (`(?(1)a|b)`), a `\N{name}` escape, and a possessive
// repeat of anything but one character or class (`(?:ab)++`), after
// whose failed iteration Python 3.11's matcher goes on from wherever
// that iteration's last step left its position. They are also a group
// name with a non-ASCII character, for which `str.isidentifier` reads
// tables the port doesn't carry, and a class that makes Python print a
// FutureWarning on stderr (`[[`, `--`, `&&`, `~~`, or `||` in a class).
// `message` names the construct, as "py_regex doesn't take a
// backreference".
struct Unsupported : std::exception {
  std::string message;

  // `what` names the construct, as "a backreference".
  explicit Unsupported(const std::string& what) : message("py_regex doesn't take " + what) {}
  const char* what() const noexcept override { return message.c_str(); }
};

struct Node;

class Regex {
 public:
  // Compiles `pattern` (UTF-8) as `re.compile(pattern)` does. Throws
  // Error for a pattern Python rejects and Unsupported for one the
  // matcher doesn't take.
  explicit Regex(std::string_view pattern);
  ~Regex();
  Regex(Regex&&) noexcept;
  Regex& operator=(Regex&&) noexcept;

  // Returns the end of the match that Python's `re.match(text, pos)`
  // finds at `pos`, or nothing when the pattern doesn't match there.
  // Positions count code points, as Python's do in a `str`.
  std::optional<std::size_t> match(std::u32string_view text, std::size_t pos) const;

 private:
  std::unique_ptr<Node> root_;
};

// Returns whether Python's `\s` matches `c` in a pattern of type `str`,
// which is `str.isspace()`: the ASCII whitespace, U+001C to U+001F,
// and Unicode's spaces and line separators.
bool is_space(char32_t c);

}  // namespace c6502::py_regex
