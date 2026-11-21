// A matcher for the regular expressions of c99.lark's terminals, with
// the semantics of Python's `re` module, which Lark's lexer runs them
// with. The patterns come from cpp/generated/c99_lalr.cpp, where
// tools/lalr_table.py writes each terminal's pattern as Lark compiles
// it, so the C++ lexer reads the same patterns the Python one does.
//
// Python's `re` is a backtracking matcher: an alternation takes the
// first alternative that lets the rest of the pattern match, and a
// greedy repeat gives back one iteration at a time until the rest
// matches. So the match it finds is the first in that order, which
// needn't be the longest, and `0(?:'?[0-7])*` matches only the `0` of
// `08`. `std::regex` has an ECMAScript mode with the same order, but no
// possessive repeat (`*+`), which INVALID_NUMBER uses, and it reads
// `\s` as ASCII whitespace where Python's `\s` matches Unicode's, which
// ATTR_OPEN's `\[\s*\[` reaches. So this module parses the patterns
// itself and matches them in Python's order.
//
// The syntax it takes is what c99.lark's patterns use: literals and
// escaped punctuation, `\t`, `\n`, `\r`, `\f`, `\v`, `\xHH`, and `\s`;
// classes with ranges, escapes, and a leading `^`; `.`; groups
// `(...)`, `(?:...)`, `(?=...)`, and `(?!...)`; and `*`, `+`, `?`,
// `{m}`, `{m,}`, and `{m,n}`, each greedy, lazy (`?` after it), or
// possessive (`+` after it). Any other syntax throws InternalError when
// the pattern is compiled, so a new kind of pattern in c99.lark fails
// the C++ tests until this module takes it.
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace c6502::py_regex {

struct Node;

class Regex {
 public:
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
