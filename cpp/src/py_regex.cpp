// The matcher of py_regex.h, in three parts. `Parser` is a port of
// Python 3.11's `re/_parser.py`, which turns a pattern into a tree of
// (opcode, argument) items with the errors Python raises. `Compiler`
// walks that tree as `re/_compiler.py`'s `_compile` does, with its
// errors and its treatment of case folding, and builds a tree of
// `Node`s, each of which matches as one of `_sre`'s opcodes does.
// `Matcher` walks the nodes with continuations. Matching a node at a
// position calls the continuation (the rest of the pattern) at each
// position where the node can end, in `_sre`'s order, and stops at the
// first call that succeeds. That reproduces `_sre`'s backtracking,
// including its rule that ends a repeat when an iteration matches the
// empty string.
#include "py_regex.h"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <utility>
#include <vector>

#include "pcpp_text.h"
#include "pcpp_unicode.h"
#include "pyrepr.h"

namespace c6502::py_regex {

namespace {

using CodePoints = std::u32string;

// _sre.MAXREPEAT and _sre.MAXGROUPS on a 64-bit build.
constexpr std::uint64_t kMaxRepeat = 4294967295;
constexpr std::uint64_t kMaxGroups = 1073741823;

// The SRE_FLAG_* values of re/_constants.py.
constexpr unsigned kTemplate = 1;
constexpr unsigned kIgnoreCase = 2;
constexpr unsigned kLocale = 4;
constexpr unsigned kMultiline = 8;
constexpr unsigned kDotAll = 16;
constexpr unsigned kUnicode = 32;
constexpr unsigned kVerbose = 64;
constexpr unsigned kDebug = 128;
constexpr unsigned kAscii = 256;
constexpr unsigned kTypeFlags = kAscii | kLocale | kUnicode;
constexpr unsigned kGlobalFlags = kDebug | kTemplate;

std::string utf8(const CodePoints& s) {
  std::string out;
  for (const char32_t c : s) {
    std::uint32_t cp = c;
    if (cp < 0x80) {
      out += static_cast<char>(cp);
    } else if (cp < 0x800) {
      out += static_cast<char>(0xC0 | (cp >> 6));
      out += static_cast<char>(0x80 | (cp & 0x3F));
    } else if (cp < 0x10000) {
      out += static_cast<char>(0xE0 | (cp >> 12));
      out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
      out += static_cast<char>(0x80 | (cp & 0x3F));
    } else {
      out += static_cast<char>(0xF0 | (cp >> 18));
      out += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
      out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
      out += static_cast<char>(0x80 | (cp & 0x3F));
    }
  }
  return out;
}

std::string repr(const CodePoints& s) { return pcpp::py_repr(utf8(s)); }

// ---------------------------------------------------------------------
// Characters and their cases.

std::uint32_t mapped(const pcpp::unicode::CaseMapping* first, const pcpp::unicode::CaseMapping* last,
                     std::uint32_t c) {
  const auto* it = std::lower_bound(first, last, c, [](const pcpp::unicode::CaseMapping& m,
                                                       std::uint32_t v) { return m.from < v; });
  return it != last && it->from == c ? it->to : c;
}

// `_sre.unicode_tolower` and `sre_upper_unicode`, the simple case
// mappings.
std::uint32_t unicode_lower(std::uint32_t c) {
  return mapped(std::begin(pcpp::unicode::kToLower), std::end(pcpp::unicode::kToLower), c);
}
std::uint32_t unicode_upper(std::uint32_t c) {
  return mapped(std::begin(pcpp::unicode::kToUpper), std::end(pcpp::unicode::kToUpper), c);
}
bool unicode_iscased(std::uint32_t c) { return unicode_lower(c) != c || unicode_upper(c) != c; }

std::uint32_t ascii_lower(std::uint32_t c) { return c >= 'A' && c <= 'Z' ? c + 32 : c; }
bool ascii_iscased(std::uint32_t c) {
  return c < 128 && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

// The other lowercase letters with the same uppercase as `c`, from
// `re._casefix._EXTRA_CASES`.
std::vector<std::uint32_t> extra_cases(std::uint32_t c) {
  std::vector<std::uint32_t> out;
  for (const auto& m : pcpp::unicode::kExtraCases) {
    if (m.from == c) out.push_back(m.to);
  }
  return out;
}

bool ascii_digit(std::uint32_t c) { return c >= '0' && c <= '9'; }
bool ascii_word(std::uint32_t c) {
  return ascii_digit(c) || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
bool ascii_space(std::uint32_t c) { return c == ' ' || (c >= '\t' && c <= '\r'); }

// ---------------------------------------------------------------------
// The parse tree, as re/_parser.py builds it.

enum class Op {
  kLiteral,
  kNotLiteral,
  kIn,
  kAny,
  kAt,
  kBranch,
  kSubpattern,
  kAtomicGroup,
  kMaxRepeat,
  kMinRepeat,
  kPossessiveRepeat,
  kAssert,
  kAssertNot,
  kGroupRef,
  kGroupRefExists,
};

bool is_repeat(Op op) {
  return op == Op::kMaxRepeat || op == Op::kMinRepeat || op == Op::kPossessiveRepeat;
}

// The categories of `\d \D \s \S \w \W`, before the compiler picks their
// ASCII or Unicode forms.
enum class Category { kDigit, kNotDigit, kSpace, kNotSpace, kWord, kNotWord };

// The AT codes of `^ $ \A \Z \b \B`, before the compiler picks their
// multiline and Unicode forms.
enum class AtCode { kBeginning, kBeginningString, kBoundary, kNonBoundary, kEnd, kEndString };

enum class SetOp { kNegate, kLiteral, kRange, kCategory };

// An item of a class (the argument of IN).
struct SetItem {
  SetOp op = SetOp::kLiteral;
  std::uint32_t lo = 0;
  std::uint32_t hi = 0;
  Category category = Category::kDigit;

  bool operator==(const SetItem& o) const {
    return op == o.op && lo == o.lo && hi == o.hi && category == o.category;
  }
};

struct SubPattern;
using SubPtr = std::shared_ptr<SubPattern>;

// One (opcode, argument) pair.
struct Item {
  Op op = Op::kLiteral;
  std::uint32_t value = 0;      // LITERAL, NOT_LITERAL, GROUPREF, GROUPREF_EXISTS's group
  std::vector<SetItem> set;     // IN
  AtCode at = AtCode::kBeginning;
  std::vector<SubPtr> subs;     // BRANCH's items; GROUPREF_EXISTS's yes and no (or null)
  int group = 0;                // SUBPATTERN's group, 0 for none
  unsigned add_flags = 0;
  unsigned del_flags = 0;
  SubPtr sub;                   // the p of SUBPATTERN, ATOMIC_GROUP, a repeat, or an assert
  std::uint64_t min = 0;
  std::uint64_t max = 0;
  int dir = 1;                  // ASSERT and ASSERT_NOT: 1 ahead, -1 behind

  // Python's `==` of the two tuples. A SubPattern compares by identity,
  // and each is a new object, so two items that hold one are equal only
  // when they hold the same one.
  bool operator==(const Item& o) const {
    if (op != o.op) return false;
    switch (op) {
      case Op::kLiteral:
      case Op::kNotLiteral:
      case Op::kGroupRef:
        return value == o.value;
      case Op::kIn:
        return set == o.set;
      case Op::kAny:
        return true;
      case Op::kAt:
        return at == o.at;
      default:
        return sub == o.sub && subs == o.subs && value == o.value && group == o.group &&
               add_flags == o.add_flags && del_flags == o.del_flags && min == o.min &&
               max == o.max && dir == o.dir;
    }
  }
};

struct ParseState;

std::uint64_t sat_add(std::uint64_t a, std::uint64_t b) {
  constexpr std::uint64_t kCap = std::uint64_t{1} << 62;
  return std::min(a + b, kCap);
}
std::uint64_t sat_mul(std::uint64_t a, std::uint64_t b) {
  constexpr std::uint64_t kCap = std::uint64_t{1} << 62;
  if (a == 0 || b == 0) return 0;
  if (a > kCap / b) return kCap;
  return std::min(a * b, kCap);
}

using Width = std::pair<std::uint64_t, std::uint64_t>;

struct ParseState {
  unsigned flags = 0;
  std::vector<std::pair<CodePoints, int>> groupdict;
  std::vector<std::optional<Width>> groupwidths{std::nullopt};  // group 0
  std::optional<int> lookbehindgroups;
  std::vector<std::pair<std::uint64_t, std::int64_t>> grouprefpos;

  int groups() const { return static_cast<int>(groupwidths.size()); }
  std::optional<int> group_of(const CodePoints& name) const {
    for (const auto& [n, g] : groupdict) {
      if (n == name) return g;
    }
    return std::nullopt;
  }
  bool checkgroup(int gid) const {
    return gid < groups() && groupwidths[static_cast<std::size_t>(gid)].has_value();
  }
};

struct SubPattern {
  ParseState* state = nullptr;
  std::vector<Item> data;
  std::optional<Width> width;

  explicit SubPattern(ParseState* s) : state(s) {}

  // Returns the least and the most characters the subpattern can match,
  // with Python's caps, as `SubPattern.getwidth` does. The products and sums are
  // saturated, since every value past the caps comes out as the cap.
  Width getwidth() {
    if (width) return *width;
    std::uint64_t lo = 0;
    std::uint64_t hi = 0;
    for (auto& item : data) {
      switch (item.op) {
        case Op::kBranch: {
          std::uint64_t i = kMaxRepeat - 1;
          std::uint64_t j = 0;
          for (auto& s : item.subs) {
            const auto [l, h] = s->getwidth();
            i = std::min(i, l);
            j = std::max(j, h);
          }
          lo = sat_add(lo, i);
          hi = sat_add(hi, j);
          break;
        }
        case Op::kAtomicGroup:
        case Op::kSubpattern: {
          const auto [i, j] = item.sub->getwidth();
          lo = sat_add(lo, i);
          hi = sat_add(hi, j);
          break;
        }
        case Op::kMaxRepeat:
        case Op::kMinRepeat:
        case Op::kPossessiveRepeat: {
          const auto [i, j] = item.sub->getwidth();
          lo = sat_add(lo, sat_mul(i, item.min));
          hi = sat_add(hi, sat_mul(j, item.max));
          break;
        }
        case Op::kAny:
        case Op::kIn:
        case Op::kLiteral:
        case Op::kNotLiteral:
          lo = sat_add(lo, 1);
          hi = sat_add(hi, 1);
          break;
        case Op::kGroupRef: {
          const auto [i, j] = *state->groupwidths[item.value];
          lo = sat_add(lo, i);
          hi = sat_add(hi, j);
          break;
        }
        case Op::kGroupRefExists: {
          auto [i, j] = item.subs[0]->getwidth();
          if (item.subs[1]) {
            const auto [l, h] = item.subs[1]->getwidth();
            i = std::min(i, l);
            j = std::max(j, h);
          } else {
            i = 0;
          }
          lo = sat_add(lo, i);
          hi = sat_add(hi, j);
          break;
        }
        default:
          break;
      }
    }
    width = Width{std::min(lo, kMaxRepeat - 1), std::min(hi, kMaxRepeat)};
    return *width;
  }
};

// Returns `re.error(msg, pattern, pos)`, whose message gives the
// position and, in a pattern of several lines, its line and column.
Error re_error(const std::string& msg, const CodePoints& pattern, std::int64_t pos) {
  std::string m = msg + " at position " + std::to_string(pos);
  if (pattern.find(U'\n') != CodePoints::npos) {
    std::int64_t lineno = 1;
    std::int64_t last = -1;
    const std::int64_t end = std::clamp<std::int64_t>(pos, 0, static_cast<std::int64_t>(pattern.size()));
    for (std::int64_t k = 0; k < end; ++k) {
      if (pattern[static_cast<std::size_t>(k)] == U'\n') {
        ++lineno;
        last = k;
      }
    }
    m += " (line " + std::to_string(lineno) + ", column " + std::to_string(pos - last) + ")";
  }
  return Error("error", m);
}

// The pattern's tokens, as `Tokenizer` reads them. A token is one
// character, or a backslash and the character after it.
class Tokenizer {
 public:
  explicit Tokenizer(CodePoints s) : string_(std::move(s)) { advance(); }

  // The next token, or none at the end.
  const std::optional<CodePoints>& next() const { return next_; }
  bool next_is(char32_t c) const { return next_ && next_->size() == 1 && (*next_)[0] == c; }
  bool next_in(std::u32string_view set) const {
    return next_ && next_->size() == 1 && set.find((*next_)[0]) != std::u32string_view::npos;
  }

  bool match(char32_t c) {
    if (next_is(c)) {
      advance();
      return true;
    }
    return false;
  }
  std::optional<CodePoints> get() {
    auto t = next_;
    advance();
    return t;
  }
  CodePoints getwhile(int n, std::u32string_view set) {
    CodePoints result;
    for (int i = 0; i < n; ++i) {
      if (!next_in(set)) break;
      result += *next_;
      advance();
    }
    return result;
  }
  CodePoints getuntil(char32_t terminator, const std::string& name) {
    CodePoints result;
    while (true) {
      const auto c = next_;
      advance();
      if (!c) {
        if (result.empty()) throw error("missing " + name);
        throw error("missing " + utf8(CodePoints(1, terminator)) + ", unterminated name",
                    static_cast<std::int64_t>(result.size()));
      }
      if (c->size() == 1 && (*c)[0] == terminator) {
        if (result.empty()) throw error("missing " + name, 1);
        break;
      }
      result += *c;
    }
    return result;
  }
  std::int64_t tell() const {
    return static_cast<std::int64_t>(index_) - static_cast<std::int64_t>(next_ ? next_->size() : 0);
  }
  void seek(std::size_t index) {
    index_ = index;
    advance();
  }
  Error error(const std::string& msg, std::int64_t offset = 0) const {
    return re_error(msg, string_, tell() - offset);
  }
  const CodePoints& string() const { return string_; }

 private:
  void advance() {
    std::size_t index = index_;
    if (index >= string_.size()) {
      next_.reset();
      return;
    }
    CodePoints c(1, string_[index]);
    if (c[0] == U'\\') {
      ++index;
      if (index >= string_.size()) {
        throw re_error("bad escape (end of pattern)", string_,
                       static_cast<std::int64_t>(string_.size()) - 1);
      }
      c += string_[index];
    }
    index_ = index + 1;
    next_ = std::move(c);
  }

  CodePoints string_;
  std::size_t index_ = 0;
  std::optional<CodePoints> next_;
};

constexpr std::u32string_view kDigits = U"0123456789";
constexpr std::u32string_view kOctDigits = U"01234567";
constexpr std::u32string_view kHexDigits = U"0123456789abcdefABCDEF";
constexpr std::u32string_view kWhitespace = U" \t\n\r\v\f";
constexpr std::u32string_view kSpecialChars = U".\\[{()*+?^$|";
constexpr std::u32string_view kRepeatChars = U"*+?{";
constexpr std::u32string_view kFlagChars = U"iLmsxatu";

bool ascii_letter(char32_t c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

unsigned flag_of(char32_t c) {
  switch (c) {
    case 'i':
      return kIgnoreCase;
    case 'L':
      return kLocale;
    case 'm':
      return kMultiline;
    case 's':
      return kDotAll;
    case 'x':
      return kVerbose;
    case 'a':
      return kAscii;
    case 't':
      return kTemplate;
    default:
      return kUnicode;
  }
}

// Returns `str.isalpha()` of a one-character string, for the choice
// between "unknown flag" and "missing ..." after a group's flags. A
// non-ASCII character there is reported as unsupported, since the port
// has no table of Unicode letters.
bool flag_char_isalpha(char32_t c) {
  if (c >= 0x80) {
    throw Unsupported("a non-ASCII character among inline flags");
  }
  return ascii_letter(c);
}

std::uint64_t hex_value(const CodePoints& digits) {
  std::uint64_t v = 0;
  for (const char32_t c : digits) {
    v = v * 16 + static_cast<std::uint64_t>(c <= '9' ? c - '0' : (c | 0x20) - 'a' + 10);
  }
  return v;
}

std::uint64_t oct_value(const CodePoints& digits) {
  std::uint64_t v = 0;
  for (const char32_t c : digits) v = v * 8 + static_cast<std::uint64_t>(c - '0');
  return v;
}

// Python's `int()` of a run of ASCII digits, saturated past 2**62.
std::uint64_t dec_value(const CodePoints& digits) {
  std::uint64_t v = 0;
  for (const char32_t c : digits) {
    v = sat_add(sat_mul(v, 10), static_cast<std::uint64_t>(c - '0'));
  }
  return v;
}

Item literal(std::uint32_t c) {
  Item it;
  it.op = Op::kLiteral;
  it.value = c;
  return it;
}

Item in_category(Category c) {
  Item it;
  it.op = Op::kIn;
  SetItem s;
  s.op = SetOp::kCategory;
  s.category = c;
  it.set.push_back(s);
  return it;
}

Item at(AtCode a) {
  Item it;
  it.op = Op::kAt;
  it.at = a;
  return it;
}

class Parser {
 public:
  explicit Parser(CodePoints pattern) : source_(std::move(pattern)) {}

  // Returns the tree of the pattern, as `parse(str)` does, and leaves the
  // final flags in `state()`.
  SubPtr parse() {
    auto p = parse_sub(state_.flags & kVerbose, 0);
    // fix_flags for a str pattern. The parser rejects `L` itself.
    if (!(state_.flags & kAscii)) {
      state_.flags |= kUnicode;
    } else if (state_.flags & kUnicode) {
      throw Error("ValueError", "ASCII and UNICODE flags are incompatible");
    }
    if (source_.next()) throw source_.error("unbalanced parenthesis");
    for (const auto& [g, pos] : state_.grouprefpos) {
      if (g >= static_cast<std::uint64_t>(state_.groups())) {
        throw re_error("invalid group reference " + std::to_string(g), source_.string(), pos);
      }
    }
    return p;
  }

  ParseState& state() { return state_; }

  // The first FutureWarning Python prints for the pattern, if any.
  const std::optional<std::string>& warned() const { return warned_; }

 private:
  void warn(const std::string& what) {
    if (!warned_) warned_ = what;
  }

  SubPtr new_sub() { return std::make_shared<SubPattern>(&state_); }

  void checkgroupname(const CodePoints& name, std::int64_t offset) {
    // `str.isidentifier()`, which for ASCII is a letter or `_` and then
    // letters, digits, and `_`.
    bool ok = !name.empty();
    for (std::size_t i = 0; i < name.size(); ++i) {
      const char32_t c = name[i];
      if (c >= 0x80) {
        throw Unsupported("a group name with a non-ASCII character");
      }
      const bool start = ascii_letter(c) || c == '_';
      if (!(start || (i > 0 && ascii_digit(c)))) ok = false;
    }
    if (!ok) {
      throw source_.error("bad character in group name " + repr(name),
                          static_cast<std::int64_t>(name.size()) + offset);
    }
  }

  void checklookbehindgroup(int gid) {
    if (state_.lookbehindgroups) {
      if (!state_.checkgroup(gid)) throw source_.error("cannot refer to an open group");
      if (gid >= *state_.lookbehindgroups) {
        throw source_.error("cannot refer to group defined in the same lookbehind subpattern");
      }
    }
  }

  int opengroup(const std::optional<CodePoints>& name) {
    const int gid = state_.groups();
    state_.groupwidths.emplace_back();
    if (static_cast<std::uint64_t>(state_.groups()) > kMaxGroups) {
      throw Error("error", "too many groups");
    }
    if (name) {
      if (const auto ogid = state_.group_of(*name)) {
        throw source_.error("redefinition of group name " + repr(*name) + " as group " +
                                std::to_string(gid) + "; was group " + std::to_string(*ogid),
                            static_cast<std::int64_t>(name->size()) + 1);
      }
      state_.groupdict.emplace_back(*name, gid);
    }
    return gid;
  }

  // The `\x`, `\u`, `\U`, and `\N` escapes that `_class_escape` and
  // `_escape` read alike. Returns nothing for another escape.
  std::optional<Item> hex_or_name_escape(CodePoints& escape) {
    const char32_t c = escape[1];
    if (c == 'x' || c == 'u' || c == 'U') {
      const int n = c == 'x' ? 2 : c == 'u' ? 4 : 8;
      escape += source_.getwhile(n, kHexDigits);
      if (escape.size() != static_cast<std::size_t>(n) + 2) {
        throw source_.error("incomplete escape " + utf8(escape),
                            static_cast<std::int64_t>(escape.size()));
      }
      const std::uint64_t v = hex_value(escape.substr(2));
      if (v > 0x10FFFF) {
        // `chr()` raises ValueError, which becomes "bad escape".
        throw source_.error("bad escape " + utf8(escape), static_cast<std::int64_t>(escape.size()));
      }
      return literal(static_cast<std::uint32_t>(v));
    }
    if (c == 'N') {
      if (!source_.match('{')) throw source_.error("missing {");
      source_.getuntil('}', "character name");
      throw Unsupported("the escape \\N{name}");
    }
    return std::nullopt;
  }

  // `_class_escape`.
  Item class_escape(CodePoints escape) {
    switch (escape[1]) {
      case 'a':
        return literal(7);
      case 'b':
        return literal(8);
      case 'f':
        return literal(12);
      case 'n':
        return literal(10);
      case 'r':
        return literal(13);
      case 't':
        return literal(9);
      case 'v':
        return literal(11);
      case '\\':
        return literal('\\');
      case 'd':
        return in_category(Category::kDigit);
      case 'D':
        return in_category(Category::kNotDigit);
      case 's':
        return in_category(Category::kSpace);
      case 'S':
        return in_category(Category::kNotSpace);
      case 'w':
        return in_category(Category::kWord);
      case 'W':
        return in_category(Category::kNotWord);
      default:
        break;
    }
    if (auto it = hex_or_name_escape(escape)) return *it;
    const char32_t c = escape[1];
    if (kOctDigits.find(c) != std::u32string_view::npos) {
      escape += source_.getwhile(2, kOctDigits);
      const std::uint64_t v = oct_value(escape.substr(1));
      if (v > 0377) {
        throw source_.error("octal escape value " + utf8(escape) + " outside of range 0-0o377",
                            static_cast<std::int64_t>(escape.size()));
      }
      return literal(static_cast<std::uint32_t>(v));
    }
    if (!ascii_digit(c) && escape.size() == 2 && !ascii_letter(c)) return literal(c);
    throw source_.error("bad escape " + utf8(escape), static_cast<std::int64_t>(escape.size()));
  }

  // `_escape`.
  Item escape(CodePoints escape) {
    switch (escape[1]) {
      case 'A':
        return at(AtCode::kBeginningString);
      case 'b':
        return at(AtCode::kBoundary);
      case 'B':
        return at(AtCode::kNonBoundary);
      case 'Z':
        return at(AtCode::kEndString);
      case 'd':
        return in_category(Category::kDigit);
      case 'D':
        return in_category(Category::kNotDigit);
      case 's':
        return in_category(Category::kSpace);
      case 'S':
        return in_category(Category::kNotSpace);
      case 'w':
        return in_category(Category::kWord);
      case 'W':
        return in_category(Category::kNotWord);
      case 'a':
        return literal(7);
      case 'f':
        return literal(12);
      case 'n':
        return literal(10);
      case 'r':
        return literal(13);
      case 't':
        return literal(9);
      case 'v':
        return literal(11);
      case '\\':
        return literal('\\');
      default:
        break;
    }
    if (auto it = hex_or_name_escape(escape)) return *it;
    const char32_t c = escape[1];
    if (c == '0') {
      escape += source_.getwhile(2, kOctDigits);
      return literal(static_cast<std::uint32_t>(oct_value(escape.substr(1))));
    }
    if (ascii_digit(c)) {
      // An octal escape or a decimal group reference.
      if (source_.next_in(kDigits)) {
        escape += *source_.get();
        if (kOctDigits.find(escape[1]) != std::u32string_view::npos &&
            kOctDigits.find(escape[2]) != std::u32string_view::npos && source_.next_in(kOctDigits)) {
          escape += *source_.get();
          const std::uint64_t v = oct_value(escape.substr(1));
          if (v > 0377) {
            throw source_.error("octal escape value " + utf8(escape) + " outside of range 0-0o377",
                                static_cast<std::int64_t>(escape.size()));
          }
          return literal(static_cast<std::uint32_t>(v));
        }
      }
      const std::uint64_t group = dec_value(escape.substr(1));
      if (group < static_cast<std::uint64_t>(state_.groups())) {
        if (!state_.checkgroup(static_cast<int>(group))) {
          throw source_.error("cannot refer to an open group",
                              static_cast<std::int64_t>(escape.size()));
        }
        checklookbehindgroup(static_cast<int>(group));
        Item it;
        it.op = Op::kGroupRef;
        it.value = static_cast<std::uint32_t>(group);
        return it;
      }
      throw source_.error("invalid group reference " + std::to_string(group),
                          static_cast<std::int64_t>(escape.size()) - 1);
    }
    if (escape.size() == 2 && !ascii_letter(c)) return literal(c);
    throw source_.error("bad escape " + utf8(escape), static_cast<std::int64_t>(escape.size()));
  }

  // Reads an alternation, as `_parse_sub` does.
  SubPtr parse_sub(bool verbose, int nested) {
    std::vector<SubPtr> items;
    while (true) {
      items.push_back(parse(verbose, nested + 1, nested == 0 && items.empty()));
      if (!source_.match('|')) break;
      if (nested == 0) verbose = (state_.flags & kVerbose) != 0;
    }
    if (items.size() == 1) return items[0];
    auto subpattern = new_sub();
    // Move a prefix that every item shares out of the branch.
    while (true) {
      std::optional<Item> prefix;
      bool all = true;
      for (const auto& item : items) {
        if (item->data.empty()) {
          all = false;
          break;
        }
        if (!prefix) {
          prefix = item->data[0];
        } else if (!(item->data[0] == *prefix)) {
          all = false;
          break;
        }
      }
      if (!all) break;
      for (auto& item : items) item->data.erase(item->data.begin());
      subpattern->data.push_back(*prefix);
    }
    // A branch of single characters becomes a class.
    std::vector<SetItem> set;
    bool as_set = true;
    for (const auto& item : items) {
      if (item->data.size() != 1) {
        as_set = false;
        break;
      }
      const Item& it = item->data[0];
      if (it.op == Op::kLiteral) {
        SetItem s;
        s.lo = s.hi = it.value;
        set.push_back(s);
      } else if (it.op == Op::kIn && it.set[0].op != SetOp::kNegate) {
        set.insert(set.end(), it.set.begin(), it.set.end());
      } else {
        as_set = false;
        break;
      }
    }
    if (as_set) {
      Item in;
      in.op = Op::kIn;
      in.set = uniq(set);
      subpattern->data.push_back(in);
      return subpattern;
    }
    Item branch;
    branch.op = Op::kBranch;
    branch.subs = std::move(items);
    subpattern->data.push_back(std::move(branch));
    return subpattern;
  }

  static std::vector<SetItem> uniq(const std::vector<SetItem>& items) {
    std::vector<SetItem> out;
    for (const auto& s : items) {
      if (std::find(out.begin(), out.end(), s) == out.end()) out.push_back(s);
    }
    return out;
  }

  // Reads a class after its `[`.
  void parse_class(const SubPtr& subpattern) {
    const std::int64_t here = source_.tell() - 1;
    std::vector<SetItem> set;
    if (source_.next_is('[')) warn("Possible nested set at position " + std::to_string(source_.tell()));
    const bool negate = source_.match('^');
    auto set_item = [](const Item& code) {
      SetItem s;
      if (code.op == Op::kIn) return code.set[0];
      s.lo = s.hi = code.value;
      return s;
    };
    while (true) {
      auto this_tok = source_.get();
      if (!this_tok) {
        throw source_.error("unterminated character set", source_.tell() - here);
      }
      const CodePoints& t = *this_tok;
      if (t == U"]" && !set.empty()) break;
      Item code1;
      if (t[0] == U'\\') {
        code1 = class_escape(t);
      } else {
        if (!set.empty() && std::u32string_view(U"-&~|").find(t[0]) != std::u32string_view::npos &&
            source_.next_is(t[0])) {
          warn("Possible set operation at position " + std::to_string(source_.tell() - 1));
        }
        code1 = literal(t[0]);
      }
      if (source_.match('-')) {
        // A potential range.
        auto that_tok = source_.get();
        if (!that_tok) {
          throw source_.error("unterminated character set", source_.tell() - here);
        }
        const CodePoints& that = *that_tok;
        if (that == U"]") {
          set.push_back(set_item(code1));
          set.push_back(set_item(literal('-')));
          break;
        }
        Item code2;
        if (that[0] == U'\\') {
          code2 = class_escape(that);
        } else {
          if (that == U"-") warn("Possible set difference at position " + std::to_string(source_.tell() - 2));
          code2 = literal(that[0]);
        }
        const std::string msg = "bad character range " + utf8(t) + "-" + utf8(that);
        const auto off = static_cast<std::int64_t>(t.size() + 1 + that.size());
        if (code1.op != Op::kLiteral || code2.op != Op::kLiteral) throw source_.error(msg, off);
        if (code2.value < code1.value) throw source_.error(msg, off);
        SetItem r;
        r.op = SetOp::kRange;
        r.lo = code1.value;
        r.hi = code2.value;
        set.push_back(r);
      } else {
        set.push_back(set_item(code1));
      }
    }
    set = uniq(set);
    if (set.size() == 1 && set[0].op == SetOp::kLiteral) {
      Item it = literal(set[0].lo);
      if (negate) it.op = Op::kNotLiteral;
      subpattern->data.push_back(it);
      return;
    }
    if (negate) {
      SetItem n;
      n.op = SetOp::kNegate;
      set.insert(set.begin(), n);
    }
    Item in;
    in.op = Op::kIn;
    in.set = std::move(set);
    subpattern->data.push_back(std::move(in));
  }

  // `_parse_flags`. Returns nothing for global flags.
  std::optional<std::pair<unsigned, unsigned>> parse_flags(char32_t c) {
    unsigned add_flags = 0;
    unsigned del_flags = 0;
    std::optional<CodePoints> ch = CodePoints(1, c);
    if (c != '-') {
      while (true) {
        const char32_t f = (*ch)[0];
        const unsigned flag = flag_of(f);
        if (f == 'L') throw source_.error("bad inline flags: cannot use 'L' flag with a str pattern");
        add_flags |= flag;
        if ((flag & kTypeFlags) && (add_flags & kTypeFlags) != flag) {
          throw source_.error("bad inline flags: flags 'a', 'u' and 'L' are incompatible");
        }
        ch = source_.get();
        if (!ch) throw source_.error("missing -, : or )");
        if (*ch == U")" || *ch == U"-" || *ch == U":") break;
        if (ch->size() != 1 || kFlagChars.find((*ch)[0]) == std::u32string_view::npos) {
          const bool alpha = ch->size() == 1 && flag_char_isalpha((*ch)[0]);
          throw source_.error(alpha ? "unknown flag" : "missing -, : or )",
                              static_cast<std::int64_t>(ch->size()));
        }
      }
    }
    if (*ch == U")") {
      state_.flags |= add_flags;
      return std::nullopt;
    }
    if (add_flags & kGlobalFlags) throw source_.error("bad inline flags: cannot turn on global flag", 1);
    if (*ch == U"-") {
      ch = source_.get();
      if (!ch) throw source_.error("missing flag");
      if (ch->size() != 1 || kFlagChars.find((*ch)[0]) == std::u32string_view::npos) {
        const bool alpha = ch->size() == 1 && flag_char_isalpha((*ch)[0]);
        throw source_.error(alpha ? "unknown flag" : "missing flag",
                            static_cast<std::int64_t>(ch->size()));
      }
      while (true) {
        const unsigned flag = flag_of((*ch)[0]);
        if (flag & kTypeFlags) {
          throw source_.error("bad inline flags: cannot turn off flags 'a', 'u' and 'L'");
        }
        del_flags |= flag;
        ch = source_.get();
        if (!ch) throw source_.error("missing :");
        if (*ch == U":") break;
        if (ch->size() != 1 || kFlagChars.find((*ch)[0]) == std::u32string_view::npos) {
          const bool alpha = ch->size() == 1 && flag_char_isalpha((*ch)[0]);
          throw source_.error(alpha ? "unknown flag" : "missing :",
                              static_cast<std::int64_t>(ch->size()));
        }
      }
    }
    if (del_flags & kGlobalFlags) throw source_.error("bad inline flags: cannot turn off global flag", 1);
    if (add_flags & del_flags) throw source_.error("bad inline flags: flag turned on and off", 1);
    return std::make_pair(add_flags, del_flags);
  }

  // Python's `int(condname)` for a conditional's group, or nothing where
  // it raises ValueError. A non-ASCII character is unsupported, since
  // `int()` takes Unicode digits and spaces.
  static std::optional<std::int64_t> py_int(const CodePoints& s) {
    for (const char32_t c : s) {
      if (c >= 0x80) {
        throw Unsupported("a conditional's group with a non-ASCII character");
      }
    }
    std::size_t b = 0;
    std::size_t e = s.size();
    auto space = [](char32_t c) { return ascii_space(c) || (c >= 0x1C && c <= 0x1F); };
    while (b < e && space(s[b])) ++b;
    while (e > b && space(s[e - 1])) --e;
    bool negative = false;
    if (b < e && (s[b] == '+' || s[b] == '-')) {
      negative = s[b] == '-';
      ++b;
    }
    if (b == e) return std::nullopt;
    std::int64_t v = 0;
    bool last_digit = false;
    for (std::size_t i = b; i < e; ++i) {
      if (s[i] == '_') {
        if (!last_digit) return std::nullopt;
        last_digit = false;
        continue;
      }
      if (!ascii_digit(s[i])) return std::nullopt;
      v = static_cast<std::int64_t>(std::min<std::uint64_t>(
          sat_add(sat_mul(static_cast<std::uint64_t>(v), 10), s[i] - '0'), std::uint64_t{1} << 62));
      last_digit = true;
    }
    if (!last_digit) return std::nullopt;
    return negative ? -v : v;
  }

  // Reads a sequence, as `_parse` does.
  SubPtr parse(bool verbose, int nested, bool first) {
    auto subpattern = new_sub();
    while (true) {
      if (!source_.next()) break;
      if (source_.next_in(U"|)")) break;
      const CodePoints t = *source_.next();
      source_.get();
      if (verbose) {
        if (t.size() == 1 && kWhitespace.find(t[0]) != std::u32string_view::npos) continue;
        if (t == U"#") {
          while (true) {
            const auto c = source_.get();
            if (!c || *c == U"\n") break;
          }
          continue;
        }
      }
      if (t[0] == U'\\') {
        subpattern->data.push_back(escape(t));
      } else if (kSpecialChars.find(t[0]) == std::u32string_view::npos) {
        subpattern->data.push_back(literal(t[0]));
      } else if (t == U"[") {
        parse_class(subpattern);
      } else if (kRepeatChars.find(t[0]) != std::u32string_view::npos) {
        parse_repeat(subpattern, t[0]);
      } else if (t == U".") {
        Item it;
        it.op = Op::kAny;
        subpattern->data.push_back(it);
      } else if (t == U"(") {
        if (parse_group(subpattern, verbose, nested, first)) {
          verbose = (state_.flags & kVerbose) != 0;
        }
      } else if (t == U"^") {
        subpattern->data.push_back(at(AtCode::kBeginning));
      } else if (t == U"$") {
        subpattern->data.push_back(at(AtCode::kEnd));
      }
    }
    // Unpack the non-capturing groups without flags.
    for (std::size_t i = subpattern->data.size(); i-- > 0;) {
      Item& it = subpattern->data[i];
      if (it.op == Op::kSubpattern && it.group == 0 && it.add_flags == 0 && it.del_flags == 0) {
        const auto inner = it.sub->data;
        subpattern->data.erase(subpattern->data.begin() + static_cast<std::ptrdiff_t>(i));
        subpattern->data.insert(subpattern->data.begin() + static_cast<std::ptrdiff_t>(i),
                                inner.begin(), inner.end());
      }
    }
    return subpattern;
  }

  void parse_repeat(const SubPtr& subpattern, char32_t c) {
    const std::int64_t here = source_.tell();
    std::uint64_t min = 0;
    std::uint64_t max = kMaxRepeat;
    if (c == '?') {
      max = 1;
    } else if (c == '+') {
      min = 1;
    } else if (c == '{') {
      if (source_.next_is('}')) {
        subpattern->data.push_back(literal('{'));
        return;
      }
      CodePoints lo;
      CodePoints hi;
      while (source_.next_in(kDigits)) lo += *source_.get();
      if (source_.match(',')) {
        while (source_.next_in(kDigits)) hi += *source_.get();
      } else {
        hi = lo;
      }
      if (!source_.match('}')) {
        subpattern->data.push_back(literal('{'));
        source_.seek(static_cast<std::size_t>(here));
        return;
      }
      if (!lo.empty()) {
        min = dec_value(lo);
        if (min >= kMaxRepeat) throw Error("OverflowError", "the repetition number is too large");
      }
      if (!hi.empty()) {
        max = dec_value(hi);
        if (max >= kMaxRepeat) throw Error("OverflowError", "the repetition number is too large");
        if (max < min) throw source_.error("min repeat greater than max repeat", source_.tell() - here);
      }
    }
    // The item to repeat.
    if (subpattern->data.empty() || subpattern->data.back().op == Op::kAt) {
      throw source_.error("nothing to repeat", source_.tell() - here + 1);
    }
    if (is_repeat(subpattern->data.back().op)) {
      throw source_.error("multiple repeat", source_.tell() - here + 1);
    }
    SubPtr item;
    const Item last = subpattern->data.back();
    if (last.op == Op::kSubpattern && last.group == 0 && last.add_flags == 0 && last.del_flags == 0) {
      item = last.sub;
    } else {
      item = new_sub();
      item->data.push_back(last);
    }
    Item rep;
    rep.min = min;
    rep.max = max;
    rep.sub = item;
    if (source_.match('?')) {
      rep.op = Op::kMinRepeat;
    } else if (source_.match('+')) {
      rep.op = Op::kPossessiveRepeat;
    } else {
      rep.op = Op::kMaxRepeat;
    }
    subpattern->data.back() = rep;
  }

  // Reads a group after its `(`. Returns true for global flags, after
  // which the caller reads `verbose` again.
  bool parse_group(const SubPtr& subpattern, bool verbose, int nested, bool first) {
    const std::int64_t start = source_.tell() - 1;
    bool capture = true;
    bool atomic = false;
    std::optional<CodePoints> name;
    unsigned add_flags = 0;
    unsigned del_flags = 0;
    if (source_.match('?')) {
      const auto ch = source_.get();
      if (!ch) throw source_.error("unexpected end of pattern");
      const CodePoints& c = *ch;
      if (c == U"P") {
        if (source_.match('<')) {
          name = source_.getuntil('>', "group name");
          checkgroupname(*name, 1);
        } else if (source_.match('=')) {
          const CodePoints n = source_.getuntil(')', "group name");
          checkgroupname(n, 1);
          const auto gid = state_.group_of(n);
          if (!gid) {
            throw source_.error("unknown group name " + repr(n), static_cast<std::int64_t>(n.size()) + 1);
          }
          if (!state_.checkgroup(*gid)) {
            throw source_.error("cannot refer to an open group", static_cast<std::int64_t>(n.size()) + 1);
          }
          checklookbehindgroup(*gid);
          Item it;
          it.op = Op::kGroupRef;
          it.value = static_cast<std::uint32_t>(*gid);
          subpattern->data.push_back(it);
          return false;
        } else {
          const auto c2 = source_.get();
          if (!c2) throw source_.error("unexpected end of pattern");
          throw source_.error("unknown extension ?P" + utf8(*c2), static_cast<std::int64_t>(c2->size()) + 2);
        }
      } else if (c == U":") {
        capture = false;
      } else if (c == U"#") {
        while (true) {
          if (!source_.next()) {
            throw source_.error("missing ), unterminated comment", source_.tell() - start);
          }
          if (*source_.get() == U")") break;
        }
        return false;
      } else if (c == U"=" || c == U"!" || c == U"<") {
        int dir = 1;
        CodePoints kind = c;
        std::optional<int> lookbehindgroups;
        if (c == U"<") {
          const auto c2 = source_.get();
          if (!c2) throw source_.error("unexpected end of pattern");
          if (!(*c2 == U"=" || *c2 == U"!")) {
            throw source_.error("unknown extension ?<" + utf8(*c2), static_cast<std::int64_t>(c2->size()) + 2);
          }
          kind = *c2;
          dir = -1;
          lookbehindgroups = state_.lookbehindgroups;
          if (!lookbehindgroups) state_.lookbehindgroups = state_.groups();
        }
        auto p = parse_sub(verbose, nested + 1);
        if (dir < 0 && !lookbehindgroups) state_.lookbehindgroups.reset();
        if (!source_.match(')')) {
          throw source_.error("missing ), unterminated subpattern", source_.tell() - start);
        }
        Item it;
        it.op = kind == U"=" ? Op::kAssert : Op::kAssertNot;
        it.dir = dir;
        it.sub = p;
        subpattern->data.push_back(it);
        return false;
      } else if (c == U"(") {
        parse_conditional(subpattern, verbose, nested, start);
        return false;
      } else if (c == U">") {
        capture = false;
        atomic = true;
      } else if (c.size() == 1 && (kFlagChars.find(c[0]) != std::u32string_view::npos || c[0] == '-')) {
        const auto flags = parse_flags(c[0]);
        if (!flags) {
          if (!first || !subpattern->data.empty()) {
            throw source_.error("global flags not at the start of the expression", source_.tell() - start);
          }
          return true;
        }
        add_flags = flags->first;
        del_flags = flags->second;
        capture = false;
      } else {
        throw source_.error("unknown extension ?" + utf8(c), static_cast<std::int64_t>(c.size()) + 1);
      }
    }
    int group = 0;
    if (capture) group = opengroup(name);
    const bool sub_verbose = (verbose || (add_flags & kVerbose)) && !(del_flags & kVerbose);
    auto p = parse_sub(sub_verbose, nested + 1);
    if (!source_.match(')')) {
      throw source_.error("missing ), unterminated subpattern", source_.tell() - start);
    }
    if (group != 0) state_.groupwidths[static_cast<std::size_t>(group)] = p->getwidth();
    Item it;
    if (atomic) {
      it.op = Op::kAtomicGroup;
      it.sub = p;
    } else {
      it.op = Op::kSubpattern;
      it.group = group;
      it.add_flags = add_flags;
      it.del_flags = del_flags;
      it.sub = p;
    }
    subpattern->data.push_back(std::move(it));
    return false;
  }

  void parse_conditional(const SubPtr& subpattern, bool verbose, int nested, std::int64_t start) {
    const CodePoints condname = source_.getuntil(')', "group name");
    const auto off = static_cast<std::int64_t>(condname.size()) + 1;
    int condgroup = 0;
    bool identifier = !condname.empty();
    for (std::size_t i = 0; i < condname.size(); ++i) {
      const char32_t c = condname[i];
      if (c >= 0x80) {
        throw Unsupported("a conditional's group with a non-ASCII character");
      }
      if (!(ascii_letter(c) || c == '_' || (i > 0 && ascii_digit(c)))) identifier = false;
    }
    if (identifier) {
      checkgroupname(condname, 1);
      const auto g = state_.group_of(condname);
      if (!g) throw source_.error("unknown group name " + repr(condname), off);
      condgroup = *g;
    } else {
      const auto v = py_int(condname);
      if (!v || *v < 0) throw source_.error("bad character in group name " + repr(condname), off);
      if (*v == 0) throw source_.error("bad group number", off);
      if (static_cast<std::uint64_t>(*v) >= kMaxGroups) {
        throw source_.error("invalid group reference " + std::to_string(*v), off);
      }
      condgroup = static_cast<int>(*v);
      bool seen = false;
      for (const auto& [g, pos] : state_.grouprefpos) seen = seen || g == static_cast<std::uint64_t>(condgroup);
      if (!seen) state_.grouprefpos.emplace_back(condgroup, source_.tell() - off);
    }
    checklookbehindgroup(condgroup);
    auto yes = parse(verbose, nested + 1, false);
    SubPtr no;
    if (source_.match('|')) {
      no = parse(verbose, nested + 1, false);
      if (source_.next_is('|')) throw source_.error("conditional backref with more than two branches");
    }
    if (!source_.match(')')) {
      throw source_.error("missing ), unterminated subpattern", source_.tell() - start);
    }
    Item it;
    it.op = Op::kGroupRefExists;
    it.value = static_cast<std::uint32_t>(condgroup);
    it.subs = {yes, no};
    subpattern->data.push_back(std::move(it));
  }

  Tokenizer source_;
  ParseState state_;
  std::optional<std::string> warned_;
};

}  // namespace

// ---------------------------------------------------------------------
// The compiled form.

// How a character is folded before it's tested. It's left as it is,
// or folded by `sre_lower_ascii` or by `sre_lower_unicode`.
enum class Fold { kNone, kAscii, kUnicode };

// The test of one character, as one of `_sre`'s single-character
// opcodes does it.
struct CharTest {
  enum class Kind { kLiteral, kAny, kAnyAll, kSet } kind = Kind::kLiteral;
  Fold fold = Fold::kNone;
  bool negated = false;
  std::uint32_t value = 0;  // kLiteral, already folded when `fold` is set
  // A kSet holds the characters of the set's charmap, which
  // `_optimize_charset` builds for a case-folded set, and the items
  // `_sre`'s `charset` tests after it, with a flag for a range that also
  // tests the uppercase.
  std::vector<bool> charmap;
  struct Tail {
    SetOp op = SetOp::kLiteral;
    std::uint32_t lo = 0;
    std::uint32_t hi = 0;
    bool upper_too = false;  // RANGE_UNI_IGNORE
    Category category = Category::kDigit;
    bool unicode = true;     // the category's form
  };
  std::vector<Tail> tail;

  static bool category(Category c, bool unicode, std::uint32_t ch) {
    switch (c) {
      case Category::kDigit:
        return unicode ? pcpp::is_digit(ch) : ascii_digit(ch);
      case Category::kNotDigit:
        return !(unicode ? pcpp::is_digit(ch) : ascii_digit(ch));
      case Category::kSpace:
        return unicode ? pcpp::is_space(ch) : ascii_space(ch);
      case Category::kNotSpace:
        return !(unicode ? pcpp::is_space(ch) : ascii_space(ch));
      case Category::kWord:
        return unicode ? pcpp::is_word(ch) : ascii_word(ch);
      case Category::kNotWord:
        return !(unicode ? pcpp::is_word(ch) : ascii_word(ch));
    }
    return false;
  }

  bool test(std::uint32_t ch) const {
    switch (kind) {
      case Kind::kAny:
        return ch != '\n';
      case Kind::kAnyAll:
        return true;
      default:
        break;
    }
    if (fold == Fold::kAscii) {
      ch = ascii_lower(ch);
    } else if (fold == Fold::kUnicode) {
      ch = unicode_lower(ch);
    }
    if (kind == Kind::kLiteral) return (ch == value) != negated;
    bool in = ch < charmap.size() && charmap[ch];
    for (const auto& t : tail) {
      if (in) break;
      switch (t.op) {
        case SetOp::kLiteral:
          in = ch == t.lo;
          break;
        case SetOp::kRange:
          in = (t.lo <= ch && ch <= t.hi) ||
               (t.upper_too && t.lo <= unicode_upper(ch) && unicode_upper(ch) <= t.hi);
          break;
        case SetOp::kCategory:
          in = category(t.category, t.unicode, ch);
          break;
        default:
          break;
      }
    }
    return in != negated;
  }
};

// The AT codes after the compiler has chosen their forms.
enum class AtTest {
  kBeginning,
  kBeginningLine,
  kBeginningString,
  kEnd,
  kEndLine,
  kEndString,
  kBoundary,
  kNonBoundary,
  kUniBoundary,
  kUniNonBoundary,
};

enum class NodeKind { kChar, kSeq, kBranch, kRepeat, kAtomic, kAssert, kAssertNot, kAt };
enum class Mode { kGreedy, kLazy, kPossessive };

struct Node {
  NodeKind kind = NodeKind::kSeq;
  CharTest ch;
  AtTest at = AtTest::kBeginning;
  std::vector<std::unique_ptr<Node>> children;
  std::uint64_t min = 0;
  std::uint64_t max = 0;
  Mode mode = Mode::kGreedy;
  std::size_t back = 0;  // a lookbehind's width
  bool behind = false;
};

bool is_space(char32_t c) { return pcpp::is_space(c); }

namespace {

// `_compile`, with the flags of each subpattern, building `Node`s.
class Compiler {
 public:
  Compiler() = default;

  std::unique_ptr<Node> compile(SubPattern& p, unsigned flags) {
    auto n = std::make_unique<Node>();
    n->kind = NodeKind::kSeq;
    for (auto& item : p.data) n->children.push_back(compile(item, flags));
    return n;
  }

  // The construct the matcher doesn't take, which `Regex` reports once
  // the whole pattern has compiled without one of Python's errors.
  std::optional<std::string> unsupported;

 private:
  static std::unique_ptr<Node> char_node(CharTest t) {
    auto n = std::make_unique<Node>();
    n->kind = NodeKind::kChar;
    n->ch = std::move(t);
    return n;
  }

  // Returns whether a repeat's body is one item of one character, or a
  // non-capturing group of one, as `_simple` does.
  static bool simple(const SubPattern& p) {
    if (p.data.size() != 1) return false;
    const Item& it = p.data[0];
    if (it.op == Op::kSubpattern) return it.group == 0 && simple(*it.sub);
    return it.op == Op::kAny || it.op == Op::kIn || it.op == Op::kLiteral ||
           it.op == Op::kNotLiteral;
  }

  // The folding of IGNORECASE under `flags`, as `_compile` picks
  // `iscased`, `tolower`, and `fixes`.
  static Fold fold_of(unsigned flags) {
    if (!(flags & kIgnoreCase) || (flags & kLocale)) return Fold::kNone;
    return (flags & kUnicode) ? Fold::kUnicode : Fold::kAscii;
  }

  std::unique_ptr<Node> literal_node(std::uint32_t av, bool negated, unsigned flags) {
    CharTest t;
    t.negated = negated;
    const Fold fold = fold_of(flags);
    const bool cased = fold == Fold::kUnicode ? unicode_iscased(av)
                       : fold == Fold::kAscii ? ascii_iscased(av)
                                              : false;
    if (fold == Fold::kNone || !cased) {
      t.value = av;
      return char_node(std::move(t));
    }
    if (fold == Fold::kAscii) {
      t.fold = Fold::kAscii;
      t.value = ascii_lower(av);
      return char_node(std::move(t));
    }
    const std::uint32_t lo = unicode_lower(av);
    const auto fixes = extra_cases(lo);
    t.fold = Fold::kUnicode;
    if (fixes.empty()) {
      t.value = lo;
      return char_node(std::move(t));
    }
    // IN_UNI_IGNORE over the letter and the others of its case.
    t.kind = CharTest::Kind::kSet;
    t.charmap.assign(65536, false);
    t.charmap[lo] = true;
    for (const auto k : fixes) t.charmap[k] = true;
    return char_node(std::move(t));
  }

  // `_optimize_charset` and the choice of IN, IN_IGNORE, or
  // IN_UNI_IGNORE.
  std::unique_ptr<Node> set_node(const std::vector<SetItem>& set, unsigned flags) {
    CharTest t;
    t.kind = CharTest::Kind::kSet;
    const bool unicode_categories = (flags & kUnicode) != 0;
    const Fold fold = fold_of(flags);
    if (fold == Fold::kNone) {
      for (const auto& s : set) {
        if (s.op == SetOp::kNegate) {
          t.negated = true;
          continue;
        }
        CharTest::Tail tl;
        tl.op = s.op;
        tl.lo = s.lo;
        tl.hi = s.op == SetOp::kLiteral ? s.lo : s.hi;
        if (s.op == SetOp::kLiteral) tl.op = SetOp::kRange;
        tl.category = s.category;
        tl.unicode = unicode_categories;
        t.tail.push_back(tl);
      }
      return char_node(std::move(t));
    }
    auto fixup = [&](std::uint32_t c) { return fold == Fold::kUnicode ? unicode_lower(c) : ascii_lower(c); };
    auto iscased = [&](std::uint32_t c) {
      return fold == Fold::kUnicode ? unicode_iscased(c) : ascii_iscased(c);
    };
    std::vector<bool> charmap(256, false);
    bool hascased = false;
    std::vector<CharTest::Tail> tail;
    for (const auto& s : set) {
      if (s.op == SetOp::kNegate) {
        t.negated = true;
        continue;
      }
      if (s.op == SetOp::kCategory) {
        CharTest::Tail tl;
        tl.op = SetOp::kCategory;
        tl.category = s.category;
        tl.unicode = unicode_categories;
        tail.push_back(tl);
        continue;
      }
      // Python sets the charmap's entries until one is past its end
      // (IndexError), then widens it from 256 to 65536 entries and
      // tries the item again, and an item that is still past the end
      // goes in the tail. The entries set before the error stay set.
      while (true) {
        bool past_end = false;
        auto set_entry = [&](std::uint32_t i) {
          if (i >= charmap.size()) {
            past_end = true;
            return false;
          }
          charmap[i] = true;
          return true;
        };
        if (s.op == SetOp::kLiteral) {
          const std::uint32_t lo = fixup(s.lo);
          if (set_entry(lo)) {
            if (fold == Fold::kUnicode) {
              for (const auto k : extra_cases(lo)) {
                if (!set_entry(k)) break;
              }
            }
            if (!past_end && !hascased && iscased(s.lo)) hascased = true;
          }
        } else {
          for (std::uint32_t i = s.lo; i <= s.hi && !past_end; ++i) {
            const std::uint32_t lo = fixup(i);
            if (!set_entry(lo)) break;
            if (fold == Fold::kUnicode) {
              for (const auto k : extra_cases(lo)) {
                if (!set_entry(k)) break;
              }
            }
          }
          if (!past_end && !hascased) {
            for (std::uint32_t i = s.lo; i <= s.hi; ++i) {
              if (iscased(i)) {
                hascased = true;
                break;
              }
            }
          }
        }
        if (!past_end) break;
        if (charmap.size() == 256) {
          charmap.resize(65536, false);
          continue;
        }
        hascased = true;
        CharTest::Tail tl;
        tl.op = SetOp::kRange;
        tl.lo = s.lo;
        tl.hi = s.op == SetOp::kLiteral ? s.lo : s.hi;
        tl.upper_too = s.op == SetOp::kRange;
        tail.push_back(tl);
        break;
      }
    }
    if (!hascased) {
      // IN, which tests the character unfolded against the set as
      // written.
      return set_node(set, flags & ~kIgnoreCase);
    }
    t.fold = fold;
    t.charmap = std::move(charmap);
    t.tail = std::move(tail);
    return char_node(std::move(t));
  }

  std::unique_ptr<Node> compile(Item& item, unsigned flags) {
    switch (item.op) {
      case Op::kLiteral:
      case Op::kNotLiteral:
        return literal_node(item.value, item.op == Op::kNotLiteral, flags);
      case Op::kIn:
        return set_node(item.set, flags);
      case Op::kAny: {
        CharTest t;
        t.kind = (flags & kDotAll) ? CharTest::Kind::kAnyAll : CharTest::Kind::kAny;
        return char_node(std::move(t));
      }
      case Op::kMaxRepeat:
      case Op::kMinRepeat:
      case Op::kPossessiveRepeat: {
        if (flags & kTemplate) {
          const char* name = item.op == Op::kMaxRepeat   ? "MAX_REPEAT"
                             : item.op == Op::kMinRepeat ? "MIN_REPEAT"
                                                         : "POSSESSIVE_REPEAT";
          throw Error("error", std::string("internal: unsupported template operator ") + name);
        }
        auto n = std::make_unique<Node>();
        n->kind = NodeKind::kRepeat;
        n->min = item.min;
        n->max = item.max;
        n->mode = item.op == Op::kMaxRepeat   ? Mode::kGreedy
                  : item.op == Op::kMinRepeat ? Mode::kLazy
                                              : Mode::kPossessive;
        if (n->mode == Mode::kPossessive && !simple(*item.sub) && !unsupported) {
          // Python 3.11's POSSESSIVE_REPEAT, which `_compile` uses for a
          // body `_simple` rejects, doesn't restore `state->ptr` after an
          // iteration past the minimum fails, so the tail starts wherever
          // the failed iteration's last step left it. `(x|(?<=a))++`
          // matches nothing of "x" there, since the failed lookbehind
          // left the position one character back.
          unsupported = "a possessive repeat of more than one character or class";
        }
        n->children.push_back(compile(*item.sub, flags));
        return n;
      }
      case Op::kSubpattern: {
        unsigned f = flags;
        if (item.add_flags & kTypeFlags) f &= ~kTypeFlags;
        f = (f | item.add_flags) & ~item.del_flags;
        return compile(*item.sub, f);
      }
      case Op::kAtomicGroup: {
        auto n = std::make_unique<Node>();
        n->kind = NodeKind::kAtomic;
        n->children.push_back(compile(*item.sub, flags));
        return n;
      }
      case Op::kAssert:
      case Op::kAssertNot: {
        auto n = std::make_unique<Node>();
        n->kind = item.op == Op::kAssert ? NodeKind::kAssert : NodeKind::kAssertNot;
        if (item.dir < 0) {
          const auto [lo, hi] = item.sub->getwidth();
          if (lo != hi) throw Error("error", "look-behind requires fixed-width pattern");
          n->behind = true;
          n->back = static_cast<std::size_t>(lo);
        }
        n->children.push_back(compile(*item.sub, flags));
        return n;
      }
      case Op::kAt: {
        auto n = std::make_unique<Node>();
        n->kind = NodeKind::kAt;
        const bool multiline = (flags & kMultiline) != 0;
        const bool unicode = (flags & kUnicode) != 0;
        switch (item.at) {
          case AtCode::kBeginning:
            n->at = multiline ? AtTest::kBeginningLine : AtTest::kBeginning;
            break;
          case AtCode::kEnd:
            n->at = multiline ? AtTest::kEndLine : AtTest::kEnd;
            break;
          case AtCode::kBeginningString:
            n->at = AtTest::kBeginningString;
            break;
          case AtCode::kEndString:
            n->at = AtTest::kEndString;
            break;
          case AtCode::kBoundary:
            n->at = unicode ? AtTest::kUniBoundary : AtTest::kBoundary;
            break;
          case AtCode::kNonBoundary:
            n->at = unicode ? AtTest::kUniNonBoundary : AtTest::kNonBoundary;
            break;
        }
        return n;
      }
      case Op::kBranch: {
        auto n = std::make_unique<Node>();
        n->kind = NodeKind::kBranch;
        for (auto& s : item.subs) n->children.push_back(compile(*s, flags));
        return n;
      }
      case Op::kGroupRef:
        if (!unsupported) unsupported = "a backreference";
        return std::make_unique<Node>();
      case Op::kGroupRefExists:
        compile(*item.subs[0], flags);
        if (item.subs[1]) compile(*item.subs[1], flags);
        if (!unsupported) unsupported = "a conditional group";
        return std::make_unique<Node>();
    }
    return std::make_unique<Node>();
  }

};

// A reference to a callable, the continuation of a match, without the
// allocation of a `std::function`.
class Cont {
 public:
  template <class F>
  Cont(const F& f)  // NOLINT(google-explicit-constructor)
      : obj_(&f), call_([](const void* o, std::size_t p) { return (*static_cast<const F*>(o))(p); }) {}
  bool operator()(std::size_t p) const { return call_(obj_, p); }

 private:
  const void* obj_;
  bool (*call_)(const void*, std::size_t);
};

constexpr std::size_t kNoPosition = std::numeric_limits<std::size_t>::max();

class Matcher {
 public:
  explicit Matcher(std::u32string_view text) : t_(text) {}

  bool match(const Node& n, std::size_t pos, Cont k) const {
    switch (n.kind) {
      case NodeKind::kChar:
        return pos < t_.size() && n.ch.test(t_[pos]) && k(pos + 1);
      case NodeKind::kSeq:
        return seq(n, 0, pos, k);
      case NodeKind::kBranch:
        for (const auto& c : n.children) {
          if (match(*c, pos, k)) return true;
        }
        return false;
      case NodeKind::kRepeat:
        return repeat(n, pos, k);
      case NodeKind::kAtomic: {
        std::size_t end = 0;
        if (!first(*n.children[0], pos, end)) return false;
        return k(end);
      }
      case NodeKind::kAssert:
      case NodeKind::kAssertNot: {
        const bool positive = n.kind == NodeKind::kAssert;
        if (n.behind && pos < n.back) return !positive && k(pos);
        std::size_t end = 0;
        const bool found = first(*n.children[0], n.behind ? pos - n.back : pos, end);
        return found == positive && k(pos);
      }
      case NodeKind::kAt:
        return at(n.at, pos) && k(pos);
    }
    return false;
  }

 private:
  // Matches `n` alone at `pos`, as `_sre`'s DO_JUMP0 does, and sets `end`
  // to where its first match ends.
  bool first(const Node& n, std::size_t pos, std::size_t& end) const {
    const auto take = [&](std::size_t p) {
      end = p;
      return true;
    };
    return match(n, pos, take);
  }

  bool word(std::size_t i, bool unicode) const {
    return unicode ? pcpp::is_word(t_[i]) : ascii_word(t_[i]);
  }

  bool at(AtTest a, std::size_t pos) const {
    const std::size_t end = t_.size();
    switch (a) {
      case AtTest::kBeginning:
      case AtTest::kBeginningString:
        return pos == 0;
      case AtTest::kBeginningLine:
        return pos == 0 || t_[pos - 1] == '\n';
      case AtTest::kEnd:
        return pos == end || (pos + 1 == end && t_[pos] == '\n');
      case AtTest::kEndLine:
        return pos == end || t_[pos] == '\n';
      case AtTest::kEndString:
        return pos == end;
      case AtTest::kBoundary:
      case AtTest::kNonBoundary:
      case AtTest::kUniBoundary:
      case AtTest::kUniNonBoundary: {
        if (end == 0) return false;
        const bool unicode = a == AtTest::kUniBoundary || a == AtTest::kUniNonBoundary;
        const bool that = pos > 0 && word(pos - 1, unicode);
        const bool here = pos < end && word(pos, unicode);
        const bool boundary = a == AtTest::kBoundary || a == AtTest::kUniBoundary;
        return boundary ? here != that : here == that;
      }
    }
    return false;
  }

  bool seq(const Node& n, std::size_t i, std::size_t pos, Cont k) const {
    if (i == n.children.size()) return k(pos);
    const auto rest = [&](std::size_t p) { return seq(n, i + 1, p, k); };
    return match(*n.children[i], pos, rest);
  }

  static bool below_max(const Node& n, std::uint64_t count) { return count < n.max || n.max == kMaxRepeat; }

  bool repeat(const Node& n, std::size_t pos, Cont k) const {
    const Node& child = *n.children[0];
    const Node* single = &child;
    if (single->kind == NodeKind::kSeq && single->children.size() == 1) single = single->children[0].get();
    if (single->kind == NodeKind::kChar) {
      // A repeat of one character, which REPEAT_ONE, MIN_REPEAT_ONE, and
      // POSSESSIVE_REPEAT_ONE count without recursion.
      std::uint64_t count = 0;
      while (below_max(n, count) && pos + count < t_.size() && single->ch.test(t_[pos + count])) {
        ++count;
      }
      if (count < n.min) return false;
      if (n.mode == Mode::kPossessive) return k(pos + count);
      if (n.mode == Mode::kGreedy) {
        for (std::uint64_t c = count + 1; c-- > n.min;) {
          if (k(pos + c)) return true;
        }
        return false;
      }
      for (std::uint64_t c = n.min; c <= count; ++c) {
        if (k(pos + c)) return true;
      }
      return false;
    }
    switch (n.mode) {
      case Mode::kGreedy:
        return greedy(n, child, 0, pos, kNoPosition, k);
      case Mode::kLazy:
        return lazy(n, child, 0, pos, kNoPosition, k);
      case Mode::kPossessive:
        return possessive(n, child, pos, k);
    }
    return false;
  }

  // MAX_UNTIL after `count` iterations, the last of which ended at
  // `pos`. `last` is where the latest iteration past the minimum began,
  // and an iteration that ends where it began ends the repeat. The tail
  // is tried there, with no further iteration.
  bool greedy(const Node& n, const Node& child, std::uint64_t count, std::size_t pos,
              std::size_t last, Cont k) const {
    if (count < n.min) {
      const auto more = [&](std::size_t p) { return greedy(n, child, count + 1, p, last, k); };
      return match(child, pos, more);
    }
    if (below_max(n, count) && pos != last) {
      const auto more = [&](std::size_t p) { return greedy(n, child, count + 1, p, pos, k); };
      if (match(child, pos, more)) return true;
    }
    return k(pos);
  }

  // MIN_UNTIL, with the same protection.
  bool lazy(const Node& n, const Node& child, std::uint64_t count, std::size_t pos,
            std::size_t last, Cont k) const {
    if (count < n.min) {
      const auto more = [&](std::size_t p) { return lazy(n, child, count + 1, p, last, k); };
      return match(child, pos, more);
    }
    if (k(pos)) return true;
    if (!below_max(n, count) || pos == last) return false;
    const auto more = [&](std::size_t p) { return lazy(n, child, count + 1, p, pos, k); };
    return match(child, pos, more);
  }

  // POSSESSIVE_REPEAT. Each iteration takes its first match, the minimum
  // isn't reached by backtracking into earlier iterations, an iteration
  // that matches the empty string is the last, and the tail starts where
  // the iterations end.
  bool possessive(const Node& n, const Node& child, std::size_t pos, Cont k) const {
    std::uint64_t count = 0;
    while (count < n.min) {
      std::size_t end = 0;
      if (!first(child, pos, end)) return false;
      pos = end;
      ++count;
    }
    std::size_t last = kNoPosition;
    while (below_max(n, count) && pos != last) {
      last = pos;
      std::size_t end = 0;
      if (!first(child, pos, end)) {
        pos = last;
        break;
      }
      pos = end;
      ++count;
    }
    return k(pos);
  }

  std::u32string_view t_;
};

}  // namespace

Regex::Regex(std::string_view pattern) {
  CodePoints s;
  for (const auto c : pyrepr::code_points(pattern)) s += static_cast<char32_t>(c);
  Parser parser(std::move(s));
  auto tree = parser.parse();
  Compiler compiler;
  root_ = compiler.compile(*tree, parser.state().flags);
  // Python's errors come first, since Python raises them however much
  // of the pattern the matcher would take.
  if (parser.warned()) {
    throw Unsupported("a class for which Python prints a FutureWarning (" + *parser.warned() +
                      ") on stderr");
  }
  if (compiler.unsupported) throw Unsupported(*compiler.unsupported);
}
Regex::~Regex() = default;
Regex::Regex(Regex&&) noexcept = default;
Regex& Regex::operator=(Regex&&) noexcept = default;

std::optional<std::size_t> Regex::match(std::u32string_view text, std::size_t pos) const {
  std::optional<std::size_t> end;
  const auto done = [&](std::size_t p) {
    end = p;
    return true;
  };
  Matcher(text).match(*root_, pos, done);
  return end;
}

}  // namespace c6502::py_regex
