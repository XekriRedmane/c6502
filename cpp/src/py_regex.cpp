// The matcher of py_regex.h. A pattern is parsed into a tree of nodes,
// and `match` walks it with continuations: matching a node at a
// position calls the continuation (the rest of the pattern) at each
// position where the node can end, in Python's order, and stops at the
// first call that succeeds. That reproduces the backtracking of
// Python's `re` for the syntax the module takes.
#include "py_regex.h"

#include <limits>
#include <utility>
#include <vector>

#include "errors.h"

namespace c6502::py_regex {

namespace {

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

constexpr std::size_t kUnbounded = std::numeric_limits<std::size_t>::max();

}  // namespace

enum class Kind { kEmpty, kChar, kSeq, kAlt, kRepeat, kNegLook, kPosLook };
enum class Mode { kGreedy, kLazy, kPossessive };

struct Node {
  Kind kind = Kind::kEmpty;
  // A set of characters, for kChar: the ranges, and whether `\s` is in
  // the set, and whether the set is negated.
  std::vector<std::pair<char32_t, char32_t>> ranges;
  bool space = false;
  bool negated = false;
  std::vector<std::unique_ptr<Node>> children;
  std::size_t min = 0;
  std::size_t max = 0;
  Mode mode = Mode::kGreedy;

  bool test(char32_t c) const {
    bool in = space && is_space(c);
    for (const auto& [lo, hi] : ranges) {
      in = in || (lo <= c && c <= hi);
    }
    return in != negated;
  }
};

bool is_space(char32_t c) {
  return (c >= 0x09 && c <= 0x0D) || (c >= 0x1C && c <= 0x20) || c == 0x85 || c == 0xA0 ||
         c == 0x1680 || (c >= 0x2000 && c <= 0x200A) || c == 0x2028 || c == 0x2029 ||
         c == 0x202F || c == 0x205F || c == 0x3000;
}

namespace {

class Parser {
 public:
  explicit Parser(std::string_view p) : p_(p) {}

  std::unique_ptr<Node> parse() {
    auto n = alternation();
    if (i_ != p_.size()) {
      fail("unbalanced ')'");
    }
    return n;
  }

 private:
  [[noreturn]] void fail(const std::string& why) const {
    throw InternalError("py_regex: " + why + " at " + std::to_string(i_) + " in " +
                        std::string(p_));
  }

  bool at(char c) const { return i_ < p_.size() && p_[i_] == c; }

  std::unique_ptr<Node> alternation() {
    std::vector<std::unique_ptr<Node>> alts;
    alts.push_back(sequence());
    while (at('|')) {
      ++i_;
      alts.push_back(sequence());
    }
    if (alts.size() == 1) {
      return std::move(alts[0]);
    }
    auto n = std::make_unique<Node>();
    n->kind = Kind::kAlt;
    n->children = std::move(alts);
    return n;
  }

  std::unique_ptr<Node> sequence() {
    auto n = std::make_unique<Node>();
    n->kind = Kind::kSeq;
    while (i_ < p_.size() && !at('|') && !at(')')) {
      auto a = atom();
      n->children.push_back(quantified(std::move(a)));
    }
    return n;
  }

  std::unique_ptr<Node> quantified(std::unique_ptr<Node> a) {
    std::size_t lo = 0;
    std::size_t hi = 0;
    if (at('*')) {
      lo = 0, hi = kUnbounded, ++i_;
    } else if (at('+')) {
      lo = 1, hi = kUnbounded, ++i_;
    } else if (at('?')) {
      lo = 0, hi = 1, ++i_;
    } else if (at('{')) {
      ++i_;
      lo = number();
      hi = lo;
      if (at(',')) {
        ++i_;
        hi = at('}') ? kUnbounded : number();
      }
      if (!at('}')) {
        fail("unsupported repeat");
      }
      ++i_;
    } else {
      return a;
    }
    auto n = std::make_unique<Node>();
    n->kind = Kind::kRepeat;
    n->min = lo;
    n->max = hi;
    if (at('?')) {
      n->mode = Mode::kLazy, ++i_;
    } else if (at('+')) {
      n->mode = Mode::kPossessive, ++i_;
    }
    if (at('*') || at('+') || at('?') || at('{')) {
      fail("multiple repeat");
    }
    n->children.push_back(std::move(a));
    return n;
  }

  std::size_t number() {
    std::size_t v = 0;
    bool any = false;
    while (i_ < p_.size() && p_[i_] >= '0' && p_[i_] <= '9') {
      v = v * 10 + static_cast<std::size_t>(p_[i_] - '0');
      ++i_;
      any = true;
    }
    if (!any) {
      fail("expected a number");
    }
    return v;
  }

  std::unique_ptr<Node> atom() {
    const char c = p_[i_];
    if (c == '(') {
      ++i_;
      Kind kind = Kind::kSeq;
      if (at('?')) {
        ++i_;
        if (at(':')) {
          ++i_;
        } else if (at('!')) {
          kind = Kind::kNegLook, ++i_;
        } else if (at('=')) {
          kind = Kind::kPosLook, ++i_;
        } else {
          fail("unsupported group");
        }
      }
      auto inner = alternation();
      if (!at(')')) {
        fail("missing ')'");
      }
      ++i_;
      if (kind == Kind::kSeq) {
        return inner;
      }
      auto n = std::make_unique<Node>();
      n->kind = kind;
      n->children.push_back(std::move(inner));
      return n;
    }
    auto n = std::make_unique<Node>();
    n->kind = Kind::kChar;
    if (c == '[') {
      ++i_;
      char_class(*n);
      return n;
    }
    if (c == '.') {
      ++i_;
      n->negated = true;
      n->ranges.push_back({U'\n', U'\n'});
      return n;
    }
    if (c == '*' || c == '+' || c == '?' || c == '{' || c == '^' || c == '$') {
      fail("unsupported syntax");
    }
    char32_t lit = 0;
    if (c == '\\') {
      if (escape(lit)) {
        n->space = true;
        return n;
      }
    } else {
      lit = static_cast<unsigned char>(c);
      ++i_;
    }
    n->ranges.push_back({lit, lit});
    return n;
  }

  // Reads the escape at `\`, and returns true for `\s`, or sets `lit`
  // to the character it stands for.
  bool escape(char32_t& lit) {
    ++i_;
    if (i_ >= p_.size()) {
      fail("trailing backslash");
    }
    const char e = p_[i_++];
    switch (e) {
      case 's':
        return true;
      case 't':
        lit = U'\t';
        return false;
      case 'n':
        lit = U'\n';
        return false;
      case 'r':
        lit = U'\r';
        return false;
      case 'f':
        lit = U'\f';
        return false;
      case 'v':
        lit = U'\v';
        return false;
      case 'a':
        lit = U'\a';
        return false;
      case 'x': {
        if (i_ + 2 > p_.size()) {
          fail("incomplete \\x escape");
        }
        lit = static_cast<char32_t>(std::stoul(std::string(p_.substr(i_, 2)), nullptr, 16));
        i_ += 2;
        return false;
      }
      default:
        if ((e >= 'a' && e <= 'z') || (e >= 'A' && e <= 'Z') || (e >= '0' && e <= '9')) {
          fail(std::string("unsupported escape \\") + e);
        }
        lit = static_cast<unsigned char>(e);
        return false;
    }
  }

  // Reads a class after its `[`, through its `]`. As in Python, a `]`
  // right after the `[` (or `[^`) is a member, and a `-` first or last
  // is a member.
  void char_class(Node& n) {
    if (at('^')) {
      n.negated = true, ++i_;
    }
    bool first = true;
    while (true) {
      if (i_ >= p_.size()) {
        fail("unterminated class");
      }
      if (at(']') && !first) {
        ++i_;
        return;
      }
      first = false;
      char32_t lo = 0;
      if (at('\\')) {
        if (escape(lo)) {
          n.space = true;
          continue;
        }
      } else {
        lo = static_cast<unsigned char>(p_[i_++]);
      }
      char32_t hi = lo;
      if (at('-') && i_ + 1 < p_.size() && p_[i_ + 1] != ']') {
        ++i_;
        if (at('\\')) {
          if (escape(hi)) {
            fail("bad range");
          }
        } else {
          hi = static_cast<unsigned char>(p_[i_++]);
        }
        if (hi < lo) {
          fail("bad range");
        }
      }
      n.ranges.push_back({lo, hi});
    }
  }

  std::string_view p_;
  std::size_t i_ = 0;
};

class Matcher {
 public:
  explicit Matcher(std::u32string_view text) : t_(text) {}

  bool match(const Node& n, std::size_t pos, Cont k) const {
    switch (n.kind) {
      case Kind::kEmpty:
        return k(pos);
      case Kind::kChar:
        return pos < t_.size() && n.test(t_[pos]) && k(pos + 1);
      case Kind::kSeq:
        return seq(n, 0, pos, k);
      case Kind::kAlt:
        for (const auto& c : n.children) {
          if (match(*c, pos, k)) {
            return true;
          }
        }
        return false;
      case Kind::kRepeat:
        return repeat(n, pos, k);
      case Kind::kNegLook: {
        const auto accept = [](std::size_t) { return true; };
        return !match(*n.children[0], pos, accept) && k(pos);
      }
      case Kind::kPosLook: {
        const auto accept = [](std::size_t) { return true; };
        return match(*n.children[0], pos, accept) && k(pos);
      }
    }
    return false;
  }

 private:
  bool seq(const Node& n, std::size_t i, std::size_t pos, Cont k) const {
    if (i == n.children.size()) {
      return k(pos);
    }
    const auto rest = [&](std::size_t p) { return seq(n, i + 1, p, k); };
    return match(*n.children[i], pos, rest);
  }

  bool repeat(const Node& n, std::size_t pos, Cont k) const {
    const Node& child = *n.children[0];
    if (n.mode == Mode::kPossessive) {
      // An atomic group around the greedy repeat: the first match of the
      // repeat alone, which the rest of the pattern can't take back.
      std::size_t end = pos;
      const auto take = [&](std::size_t p) {
        end = p;
        return true;
      };
      if (!greedy(n, child, 0, pos, take)) {
        return false;
      }
      return k(end);
    }
    if (child.kind == Kind::kChar) {
      // A repeat of one character, matched without recursion.
      std::size_t count = 0;
      while (count < n.max && pos + count < t_.size() && child.test(t_[pos + count])) {
        ++count;
      }
      if (count < n.min) {
        return false;
      }
      if (n.mode == Mode::kGreedy) {
        for (std::size_t c = count + 1; c-- > n.min;) {
          if (k(pos + c)) {
            return true;
          }
        }
        return false;
      }
      for (std::size_t c = n.min; c <= count; ++c) {
        if (k(pos + c)) {
          return true;
        }
      }
      return false;
    }
    return n.mode == Mode::kGreedy ? greedy(n, child, 0, pos, k) : lazy(n, child, 0, pos, k);
  }

  // An iteration that matches nothing ends the repeat, as in Python,
  // where such an iteration can't be repeated.
  bool greedy(const Node& n, const Node& child, std::size_t count, std::size_t pos, Cont k) const {
    if (count < n.max) {
      const auto more = [&](std::size_t p) {
        if (p == pos) {
          return false;
        }
        return greedy(n, child, count + 1, p, k);
      };
      if (match(child, pos, more)) {
        return true;
      }
    }
    return count >= n.min && k(pos);
  }

  bool lazy(const Node& n, const Node& child, std::size_t count, std::size_t pos, Cont k) const {
    if (count >= n.min && k(pos)) {
      return true;
    }
    if (count >= n.max) {
      return false;
    }
    const auto more = [&](std::size_t p) {
      if (p == pos) {
        return false;
      }
      return lazy(n, child, count + 1, p, k);
    };
    return match(child, pos, more);
  }

  std::u32string_view t_;
};

}  // namespace

Regex::Regex(std::string_view pattern) : root_(Parser(pattern).parse()) {}
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
