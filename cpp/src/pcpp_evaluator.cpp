// pcpp 1.30's `#if` evaluator (pcpp/evaluator.py) and the parse loop
// of PLY's yacc (`LRParser.parseopt_notrack`) that runs it.
//
// evaluator.py gives PLY a grammar of the operators of a preprocessor
// expression with C's precedence, and PLY builds an LALR parser from it
// once. The tables of that parser are in pcpp_evaluator_tables.h. Each
// production's action computes on `Value`, a subclass of Python's int
// that holds a value within the range of a 64-bit `intmax_t` or
// `uintmax_t`. An operation in which either operand is unsigned wraps
// to 64 bits unsigned, and a signed one wraps as a two's complement CPU
// would, though C leaves signed overflow undefined. Most actions catch
// the exception an operation raises and keep it in the value they
// produce (`Value(0, exception=e)`), and an operation on such a value
// passes the exception on, so `0 && 1/0` is 0 while `1 && 1/0` raises
// ZeroDivisionError when the preprocessor reads the result.
//
// The C++ keeps each of Python's results, including the ones below.
//
// - `/` is Python's true division of the two ints, which rounds the
//   exact quotient to the nearest double, and `int()` of that double,
//   so `(2**64 - 1) / 1` in unsigned arithmetic is 2**64, which wraps
//   to 0.
// - `%` of signed values is floored, with the sign of the divisor.
// - `+x` gives a signed value whatever x is, since `Value.__pos__`
//   leaves the flag out.
// - `?:` takes the unsigned flag of either branch.
// - A value of the parse may be a Python `str`, from a string literal
//   or from the `<` of `< expression >`, which the grammar takes for
//   `__has_include(<...>)`. An operation on one tries to read the
//   string as a number and raises IndexError or ValueError.
// - The actions of numbers, characters, unary `+` and `-`, groups, and
//   strings catch nothing. A SyntaxError from one of them (an empty
//   character constant `''`) makes PLY start its error recovery, and
//   any other exception escapes the parse.
// - A left shift of a nonzero value by a count whose result Python
//   can't allocate raises MemoryError. Python finds that out by trying,
//   so the threshold depends on the machine. On 21 Nov 2026 a count of
//   2**36 succeeded, one of 2**41 took about 290 GB of virtual memory
//   before it was stopped, and counts of 2**62 and up raised at once.
//   The C++ raises MemoryError for counts of 2**40 and up, and the
//   tests check only counts that Python rejects at once.
#include "pcpp_evaluator.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iterator>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

#include "errors.h"
#include "pcpp_evaluator_tables.h"
#include "pyrepr.h"

namespace c6502::pcpp {

namespace {

namespace tables = evaluator_tables;

using CodePoints = std::vector<std::uint32_t>;

// The smallest left-shift count for which the C++ raises MemoryError
// (see the comment at the top of the file).
constexpr std::uint64_t kShiftMemoryLimit = std::uint64_t{1} << 40;

// PLY's `error_count`, the number of symbols the parser has to shift to
// leave error recovery.
constexpr int kErrorCount = 3;

void append_utf8(std::string& out, std::uint32_t cp) {
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

std::string utf8(const CodePoints& cps) {
  std::string out;
  for (const auto cp : cps) {
    append_utf8(out, cp);
  }
  return out;
}

bool printable(std::uint32_t cp) {
  // The table's ranges are in order, so a binary search finds the first
  // range that ends at or after `cp`.
  int lo = 0;
  int hi = tables::kNonPrintableCount;
  while (lo < hi) {
    const int mid = (lo + hi) / 2;
    if (tables::kNonPrintable[mid][1] < cp) {
      lo = mid + 1;
    } else {
      hi = mid;
    }
  }
  return lo == tables::kNonPrintableCount || tables::kNonPrintable[lo][0] > cp;
}

void append_hex(std::string& out, std::uint32_t v, int digits) {
  static const char kDigits[] = "0123456789abcdef";
  for (int i = digits - 1; i >= 0; --i) {
    out += kDigits[(v >> (4 * i)) & 0xF];
  }
}

// Returns Python's repr of the bytes `b`.
std::string repr_bytes(const std::string& b) {
  const bool dq = b.find('\'') != std::string::npos && b.find('"') == std::string::npos;
  const char quote = dq ? '"' : '\'';
  std::string out = "b";
  out += quote;
  for (const char ch : b) {
    const auto c = static_cast<unsigned char>(ch);
    if (c == '\\' || c == static_cast<unsigned char>(quote)) {
      out += '\\';
      out += ch;
    } else if (c == '\t') {
      out += "\\t";
    } else if (c == '\n') {
      out += "\\n";
    } else if (c == '\r') {
      out += "\\r";
    } else if (c < 0x20 || c >= 0x7F) {
      out += "\\x";
      append_hex(out, c, 2);
    } else {
      out += ch;
    }
  }
  out += quote;
  return out;
}

// Returns the exception Python's `Value.__new__` raises for a string
// that isn't a valid int in `base`, whose message shows the string's
// repr cut to 200 characters (`%.200R`).
PyException invalid_literal(int base, const CodePoints& s) {
  const CodePoints r = pyrepr::code_points(repr_str(utf8(s)));
  const CodePoints cut(r.begin(), r.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(r.size(), 200)));
  return py_exception("ValueError", "invalid literal for int() with base " +
                                        std::to_string(base) + ": " + utf8(cut));
}

PyException index_error() { return py_exception("IndexError", "string index out of range"); }

bool py_space(std::uint32_t c) {
  return (c >= 0x09 && c <= 0x0D) || (c >= 0x1C && c <= 0x20) || c == 0x85 || c == 0xA0 ||
         c == 0x1680 || (c >= 0x2000 && c <= 0x200A) || c == 0x2028 || c == 0x2029 ||
         c == 0x202F || c == 0x205F || c == 0x3000;
}

int digit_value(std::uint32_t c) {
  if (c >= '0' && c <= '9') return static_cast<int>(c - '0');
  if (c >= 'a' && c <= 'z') return static_cast<int>(c - 'a' + 10);
  if (c >= 'A' && c <= 'Z') return static_cast<int>(c - 'A' + 10);
  return 99;
}

// Returns Python's `int(s, base)` reduced modulo 2**64, or throws its
// ValueError. Python's int() also takes the decimal digits of other
// scripts, which no string the evaluator reads can hold where it would
// matter. A number token's text is ASCII, and every other string the
// evaluator reads as a number starts with a character that makes it
// invalid whatever follows.
std::uint64_t py_int(const CodePoints& s, int base) {
  std::size_t i = 0;
  std::size_t n = s.size();
  while (i < n && py_space(s[i])) ++i;
  while (n > i && py_space(s[n - 1])) --n;
  bool negative = false;
  if (i < n && (s[i] == '+' || s[i] == '-')) {
    negative = s[i] == '-';
    ++i;
  }
  bool after_prefix = false;
  if (i + 1 < n && s[i] == '0' &&
      ((base == 16 && (s[i + 1] == 'x' || s[i + 1] == 'X')) ||
       (base == 8 && (s[i + 1] == 'o' || s[i + 1] == 'O')))) {
    i += 2;
    after_prefix = true;
  }
  std::uint64_t v = 0;
  bool any = false;
  bool last_underscore = false;
  for (; i < n; ++i) {
    const std::uint32_t c = s[i];
    if (c == '_') {
      if (last_underscore || (!any && !after_prefix)) throw invalid_literal(base, s);
      last_underscore = true;
      continue;
    }
    const int d = digit_value(c);
    if (d >= base) throw invalid_literal(base, s);
    v = v * static_cast<std::uint64_t>(base) + static_cast<std::uint64_t>(d);
    any = true;
    last_underscore = false;
  }
  if (!any || last_underscore) throw invalid_literal(base, s);
  return negative ? ~v + 1 : v;
}

// Returns the length of the escape that `_expand_escape_sequences_pat`
// matches at `i` in `s`, or 0 when it matches none there. The pattern's
// alternatives are tried in its order. With `names` false, the pattern
// is read without its `\N{name}` alternative, and `is_name` is set when
// that alternative matches.
std::size_t escape_length(const CodePoints& s, std::size_t i, bool names, bool& is_name) {
  is_name = false;
  const std::size_t n = s.size();
  if (s[i] != '\\' || i + 1 >= n) return 0;
  const std::uint32_t c = s[i + 1];
  // Whether `chars` characters follow the escape's letter, none of them
  // a newline, which the pattern's `.` doesn't match.
  auto fixed = [&](std::size_t chars) {
    if (i + 2 + chars > n) return false;
    for (std::size_t k = 0; k < chars; ++k) {
      if (s[i + 2 + k] == '\n') return false;
    }
    return true;
  };
  if (c == 'U' && fixed(8)) return 10;
  if (c == 'u' && fixed(4)) return 6;
  if (c == 'x' && fixed(2)) return 4;
  if (c >= '0' && c <= '7') {
    std::size_t len = 2;
    while (len < 4 && i + len < n && s[i + len] >= '0' && s[i + len] <= '7') ++len;
    return len;
  }
  if (names && c == 'N' && i + 2 < n && s[i + 2] == '{') {
    std::size_t j = i + 3;
    while (j < n && s[j] != '}') ++j;
    if (j < n && j > i + 3) {
      is_name = true;
      return j + 1 - i;
    }
  }
  if (c == '\\' || c == '\'' || c == '"' || c == 'a' || c == 'b' || c == 'f' || c == 'n' ||
      c == 'r' || c == 't' || c == 'v') {
    return 2;
  }
  return 0;
}

// Returns the code point that `codecs.decode(m, 'unicode-escape')` gives
// for the escape `m`, the `len` characters of `s` at `i`, which isn't a
// `\N{name}`, or nullopt with `error` set to the UnicodeDecodeError the
// codec raises.
std::optional<std::uint32_t> decode_escape(const CodePoints& s, std::size_t i, std::size_t len,
                                           PyException& error) {
  const std::uint32_t c = s[i + 1];
  const int hex_count = c == 'U' ? 8 : c == 'u' ? 4 : c == 'x' ? 2 : 0;
  if (hex_count > 0) {
    // The codec reads the UTF-8 bytes of the match, and reports the byte
    // where a digit fails.
    const std::string bytes = utf8(CodePoints(s.begin() + static_cast<std::ptrdiff_t>(i),
                                              s.begin() + static_cast<std::ptrdiff_t>(i + len)));
    const char* reason = hex_count == 8   ? "truncated \\UXXXXXXXX escape"
                         : hex_count == 4 ? "truncated \\uXXXX escape"
                                          : "truncated \\xXX escape";
    std::uint64_t cp = 0;
    std::size_t pos = 2;
    std::size_t end = 0;
    for (int k = 0; k < hex_count; ++k, ++pos) {
      const int d = pos < bytes.size() ? digit_value(static_cast<unsigned char>(bytes[pos])) : 99;
      if (pos >= bytes.size() || d >= 16) {
        end = pos;
        break;
      }
      cp = cp * 16 + static_cast<std::uint64_t>(d);
    }
    if (end == 0 && cp > 0x10FFFF) {
      reason = "illegal Unicode character";
      end = pos;
    }
    if (end != 0) {
      error.type = "UnicodeDecodeError";
      error.message = std::string("'unicodeescape' codec can't decode bytes in position 0-") +
                      std::to_string(end - 1) + ": " + reason;
      error.repr = "UnicodeDecodeError('unicodeescape', " + repr_bytes(bytes) + ", 0, " +
                   std::to_string(end) + ", " + repr_str(reason) + ")";
      return std::nullopt;
    }
    return static_cast<std::uint32_t>(cp);
  }
  if (c >= '0' && c <= '7') {
    std::uint32_t v = 0;
    for (std::size_t k = i + 1; k < i + len; ++k) v = v * 8 + (s[k] - '0');
    return v;
  }
  static const char kFrom[] = "\\'\"abfnrtv";
  static const std::uint32_t kTo[] = {'\\', '\'', '"', 7, 8, 12, 10, 13, 9, 11};
  for (int k = 0; k < 10; ++k) {
    if (c == static_cast<std::uint32_t>(kFrom[k])) return kTo[k];
  }
  return c;
}

// The value of a Python character constant's text between its quotes,
// after `_expand_escape_sequences_pat.sub` and `ord()`.
std::uint64_t char_value(const CodePoints& inner) {
  std::size_t length = 0;
  std::uint32_t last = 0;
  std::size_t i = 0;
  while (i < inner.size()) {
    bool is_name = false;
    const std::size_t len = escape_length(inner, i, true, is_name);
    if (len == 0) {
      ++length;
      last = inner[i];
      ++i;
      continue;
    }
    if (is_name) {
      // preprocessor.cpp's `without_named_escapes` rewrites a character
      // constant before the evaluator reads it, so that no `\N{name}`
      // reaches here from a compile.
      throw InternalError(
          "pcpp's evaluator: a \\N{name} escape in a character constant isn't ported");
    }
    PyException error;
    const auto cp = decode_escape(inner, i, len, error);
    if (!cp) throw error;
    ++length;
    last = *cp;
    i += len;
  }
  if (length != 1) {
    throw py_exception("TypeError", "ord() expected a character, but string of length " +
                                        std::to_string(length) + " found");
  }
  return last;
}

// The correctly rounded double nearest to n / d, for d != 0, as
// Python's true division of ints gives it (round half to even).
double true_divide(std::uint64_t n, std::uint64_t d) {
  if (n == 0) return 0.0;
  auto bit_length = [](std::uint64_t v) {
    int b = 0;
    while (v != 0) {
      ++b;
      v >>= 1;
    }
    return b;
  };
  // A 128-bit unsigned value as two halves, enough for n shifted left
  // by up to 119 places.
  struct U128 {
    std::uint64_t hi = 0;
    std::uint64_t lo = 0;
  };
  const int e = bit_length(n) - bit_length(d);
  // Scale the quotient into [2**54, 2**56) so that it has two or three
  // bits below the 53 a double holds.
  const int s = 55 - e;
  U128 num;
  std::uint64_t den = d;
  if (s >= 0) {
    if (s >= 64) {
      num.hi = n << (s - 64);
    } else if (s > 0) {
      num.hi = n >> (64 - s);
      num.lo = n << s;
    } else {
      num.lo = n;
    }
  } else {
    num.lo = n;
    den = d << (-s);
  }
  // Binary long division of the 128-bit numerator by the 64-bit
  // denominator.
  std::uint64_t q = 0;
  U128 rem;
  for (int bit = 127; bit >= 0; --bit) {
    // rem = rem * 2 + the next bit of num.
    rem.hi = (rem.hi << 1) | (rem.lo >> 63);
    rem.lo <<= 1;
    const std::uint64_t b = bit >= 64 ? (num.hi >> (bit - 64)) & 1 : (num.lo >> bit) & 1;
    rem.lo |= b;
    q <<= 1;
    if (rem.hi != 0 || rem.lo >= den) {
      // rem < 2 * den, so rem - den fits in 64 bits.
      rem.lo -= den;
      rem.hi = 0;
      q |= 1;
    }
  }
  const bool sticky = rem.lo != 0;
  const int k = bit_length(q) - 53;
  const std::uint64_t low = q & ((std::uint64_t{1} << k) - 1);
  const std::uint64_t half = std::uint64_t{1} << (k - 1);
  q >>= k;
  if (low > half || (low == half && (sticky || (q & 1) != 0))) ++q;
  return std::ldexp(static_cast<double>(q), k - s);
}

// Returns `int(x)` of a double whose magnitude is at most 2**64,
// reduced modulo 2**64.
std::uint64_t int_of_double(double x) {
  const double t = std::trunc(x);
  const bool negative = t < 0;
  const double m = negative ? -t : t;
  const std::uint64_t mag = m >= 18446744073709551616.0 ? 0 : static_cast<std::uint64_t>(m);
  return negative ? ~mag + 1 : mag;
}

// pcpp's `Value`, a Python int of at most 64 bits with its unsigned
// flag and the exception that an operation raised in its place.
struct Value {
  std::uint64_t bits = 0;
  bool is_unsigned = false;
  std::shared_ptr<const PyException> exception;

  std::int64_t sval() const { return static_cast<std::int64_t>(bits); }

  // Returns the int, or throws the exception, as `Value.value()` does.
  EvalResult value() const {
    if (exception) throw *exception;
    return {bits, is_unsigned};
  }
};

Value make(std::uint64_t bits, bool is_unsigned = false) { return {bits, is_unsigned, nullptr}; }

Value with_exception(const PyException& e) {
  return {0, false, std::make_shared<const PyException>(e)};
}

// A value of the parse, which is a Python None, a str, or a Value.
struct PValue {
  enum Kind { kNone, kStr, kValue } kind = kNone;
  std::string str;
  Value val;
};

PValue pv(Value v) { return {PValue::kValue, "", std::move(v)}; }

// Returns `Value(s)` for a string, as `Value.__new__` reads it.
Value value_of_string(const std::string& text) {
  CodePoints v = pyrepr::code_points(text);
  bool is_unsigned = false;
  const bool l_quote = v.size() >= 2 && v[0] == 'L' && v[1] == '\'';
  if (!l_quote && v.empty()) throw index_error();
  if ((l_quote || v[0] == '\'') && v.back() == '\'') {
    const std::size_t start = l_quote ? 2 : 1;
    const CodePoints inner = v.size() > start + 1
                                 ? CodePoints(v.begin() + static_cast<std::ptrdiff_t>(start), v.end() - 1)
                                 : CodePoints();
    if (inner.empty()) throw py_exception("SyntaxError", "Empty character escape sequence");
    return make(char_value(inner));
  }
  std::uint64_t x = 0;
  auto strip = [&](auto keep) {
    while (true) {
      if (v.empty()) throw index_error();
      const std::uint32_t c = v.back();
      if (keep(c)) break;
      if (c == 'u' || c == 'U') is_unsigned = true;
      v.pop_back();
    }
  };
  if (v.size() >= 2 && v[0] == '0' && (v[1] == 'x' || v[1] == 'X')) {
    strip([](std::uint32_t c) {
      return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
    });
    x = py_int(v, 16);
  } else if (v[0] == '0') {
    strip([](std::uint32_t c) { return c >= '0' && c <= '7'; });
    x = py_int(v, 8);
  } else {
    strip([](std::uint32_t c) { return c >= '0' && c <= '9'; });
    x = py_int(v, 10);
  }
  return make(x, is_unsigned);
}

// Returns `Value(x)` for a value of the parse.
Value to_value(const PValue& x) {
  switch (x.kind) {
    case PValue::kValue:
      return x.val;
    case PValue::kStr:
      return value_of_string(x.str);
    case PValue::kNone:
      break;
  }
  // `Value(None)` prints "Unknown value type" and fails an assertion,
  // and no action of the grammar produces None.
  throw InternalError("pcpp's evaluator: Value(None)");
}

PyException zero_division(const char* message) { return py_exception("ZeroDivisionError", message); }

enum class Op { Mul, Div, Mod, Add, Sub, Shl, Shr, Lt, Le, Gt, Ge, Eq, Ne, And, Xor, Or };

// Returns `a <op> b` for the operator methods of `Value`, which return
// the left operand when it holds an exception and then the right one,
// and otherwise compute in unsigned arithmetic when either operand is
// unsigned (for a shift, when the left one is).
Value binary(Op op, const Value& a, const Value& b) {
  if (a.exception) return a;
  if (b.exception) return b;
  const bool u = a.is_unsigned || b.is_unsigned;
  switch (op) {
    case Op::Add:
      return make(a.bits + b.bits, u);
    case Op::Sub:
      return make(a.bits - b.bits, u);
    case Op::Mul:
      return make(a.bits * b.bits, u);
    case Op::Div: {
      if (b.bits == 0) throw zero_division("division by zero");
      if (u) return make(int_of_double(true_divide(a.bits, b.bits)), true);
      const bool an = a.sval() < 0;
      const bool bn = b.sval() < 0;
      const std::uint64_t am = an ? ~a.bits + 1 : a.bits;
      const std::uint64_t bm = bn ? ~b.bits + 1 : b.bits;
      const double q = true_divide(am, bm);
      return make(int_of_double(an != bn ? -q : q));
    }
    case Op::Mod: {
      if (b.bits == 0) throw zero_division("integer modulo by zero");
      if (u) return make(a.bits % b.bits, true);
      if (b.sval() == -1) return make(0);
      std::int64_t r = a.sval() % b.sval();
      if (r != 0 && ((r < 0) != (b.sval() < 0))) r += b.sval();
      return make(static_cast<std::uint64_t>(r));
    }
    case Op::Shl: {
      // Python's `<<` checks the count's sign first, then returns 0 for
      // a zero left operand.
      const bool count_negative = !a.is_unsigned && !b.is_unsigned && b.sval() < 0;
      if (count_negative) throw py_exception("ValueError", "negative shift count");
      if (a.bits == 0) return make(0, a.is_unsigned);
      if (b.bits >= kShiftMemoryLimit) {
        PyException e;
        e.type = "MemoryError";
        e.repr = "MemoryError()";
        throw e;
      }
      return make(b.bits >= 64 ? 0 : a.bits << b.bits, a.is_unsigned);
    }
    case Op::Shr: {
      if (a.is_unsigned) return make(b.bits >= 64 ? 0 : a.bits >> b.bits, true);
      if (!b.is_unsigned && b.sval() < 0) throw py_exception("ValueError", "negative shift count");
      const std::uint64_t n = std::min<std::uint64_t>(b.bits, 63);
      return make(static_cast<std::uint64_t>(a.sval() >> n));
    }
    case Op::Lt:
      return make(u ? a.bits < b.bits : a.sval() < b.sval(), u);
    case Op::Le:
      return make(u ? a.bits <= b.bits : a.sval() <= b.sval(), u);
    case Op::Gt:
      return make(u ? a.bits > b.bits : a.sval() > b.sval(), u);
    case Op::Ge:
      return make(u ? a.bits >= b.bits : a.sval() >= b.sval(), u);
    case Op::Eq:
      return make(a.bits == b.bits, u);
    case Op::Ne:
      return make(a.bits != b.bits, u);
    case Op::And:
      return make(a.bits & b.bits, u);
    case Op::Xor:
      return make(a.bits ^ b.bits, u);
    case Op::Or:
      return make(a.bits | b.bits, u);
  }
  throw InternalError("pcpp's evaluator: unknown operator");
}

// The semantic actions of evaluator.py, by the name of the `p_`
// function the production names.
enum class Action {
  Number,
  Character,
  String,
  Group,
  Uplus,
  Uminus,
  Unop,
  Binop,
  Conditional,
  FunctionCall,
  Identifier,
  Start,
};

Action action_of(const std::string& func) {
  static const std::pair<const char*, Action> kNames[] = {
      {"p_expression_number", Action::Number},
      {"p_expression_character", Action::Character},
      {"p_expression_string", Action::String},
      {"p_expression_group", Action::Group},
      {"p_expression_uplus", Action::Uplus},
      {"p_expression_uminus", Action::Uminus},
      {"p_expression_unop", Action::Unop},
      {"p_expression_binop", Action::Binop},
      {"p_expression_conditional", Action::Conditional},
      {"p_expression_function_call", Action::FunctionCall},
      {"p_expression_identifier", Action::Identifier},
      {"", Action::Start},
  };
  for (const auto& [name, a] : kNames) {
    if (func == name) return a;
  }
  throw InternalError("pcpp's evaluator: no action for " + func);
}

bool is_nonzero(const PValue& x) { return to_value(x).value().bits != 0; }

// Runs the action of a production on `p`, where p[1] and on are the
// values of its right-hand side, and returns p[0].
PValue run_action(Action action, const std::vector<PValue>& p, const FunctionHook& functions,
                  const IdentifierHook& identifiers) {
  switch (action) {
    case Action::Number:
    case Action::Character:
      return pv(to_value(p[1]));
    case Action::String:
      return p[1];
    case Action::Group:
      return p[2];
    case Action::Uplus: {
      const Value v = to_value(p[2]);
      if (v.exception) return pv(v);
      return pv(make(v.bits));
    }
    case Action::Uminus: {
      const Value v = to_value(p[2]);
      if (v.exception) return pv(v);
      return pv(make(~v.bits + 1, v.is_unsigned));
    }
    case Action::Unop:
      try {
        if (p[1].str == "!") return pv(make(is_nonzero(p[2]) ? 0 : 1));
        if (p[1].str == "~") {
          const Value v = to_value(p[2]);
          if (v.exception) return pv(v);
          return pv(make(~v.bits, v.is_unsigned));
        }
        return {};
      } catch (const PyException& e) {
        return pv(with_exception(e));
      }
    case Action::Binop:
      try {
        const std::string& o = p[2].str;
        static const std::pair<const char*, Op> kOps[] = {
            {"*", Op::Mul}, {"/", Op::Div}, {"%", Op::Mod},  {"+", Op::Add},  {"-", Op::Sub},
            {"<<", Op::Shl}, {">>", Op::Shr}, {"<", Op::Lt},  {"<=", Op::Le}, {">", Op::Gt},
            {">=", Op::Ge}, {"==", Op::Eq}, {"!=", Op::Ne}, {"&", Op::And}, {"^", Op::Xor},
            {"|", Op::Or},
        };
        for (const auto& [name, op] : kOps) {
          if (o == name) {
            const Value a = to_value(p[1]);
            const Value b = to_value(p[3]);
            return pv(binary(op, a, b));
          }
        }
        if (o == "&&") return pv(make(is_nonzero(p[1]) && is_nonzero(p[3]) ? 1 : 0));
        if (o == "||") return pv(make(is_nonzero(p[1]) || is_nonzero(p[3]) ? 1 : 0));
        if (o == ",") return pv(to_value(p[3]));
        return {};
      } catch (const PyException& e) {
        return pv(with_exception(e));
      }
    case Action::Conditional:
      try {
        PValue out = pv(is_nonzero(p[1]) ? to_value(p[3]) : to_value(p[5]));
        try {
          // The branch's int, with the unsigned flag of either branch.
          const EvalResult r = out.val.value();
          const bool u = to_value(p[3]).is_unsigned || to_value(p[5]).is_unsigned;
          out = pv(make(r.bits, u));
        } catch (const PyException&) {
        }
        return out;
      } catch (const PyException& e) {
        return pv(with_exception(e));
      }
    case Action::FunctionCall:
      try {
        const std::optional<std::string> f = functions(p[1].str);
        if (f) throw py_exception("TypeError", "'str' object is not callable");
        return pv(make(0));
      } catch (const PyException& e) {
        return pv(with_exception(e));
      }
    case Action::Identifier:
      try {
        const auto r = identifiers(p[1].str);
        if (const auto* i = std::get_if<std::int64_t>(&r)) {
          return pv(make(static_cast<std::uint64_t>(*i)));
        }
        return pv(value_of_string(std::get<std::string>(r)));
      } catch (const PyException& e) {
        return pv(with_exception(e));
      }
    case Action::Start:
      break;
  }
  throw InternalError("pcpp's evaluator: the start production has no action");
}

// A symbol of PLY's stacks, which is a token of the input, `$end`,
// `error`, or a nonterminal with its value.
struct Sym {
  std::string type;
  PValue value;
};

int terminal_index(const std::string& name) {
  const auto* begin = std::begin(tables::kTerminals);
  const auto* end = std::end(tables::kTerminals);
  const auto* it = std::lower_bound(begin, end, name, [](const char* a, const std::string& b) {
    return std::string_view(a) < b;
  });
  return it != end && name == *it ? static_cast<int>(it - begin) : -1;
}

int nonterminal_index(const std::string& name) {
  for (int i = 0; i < tables::kNonterminalCount; ++i) {
    if (name == tables::kNonterminals[i]) return i;
  }
  return -1;
}

std::int16_t action_at(int state, const std::string& type) {
  const int t = terminal_index(type);
  return t < 0 ? tables::kNone : tables::kAction[state][t];
}

// `p_error`, which raises out of the parse.
[[noreturn]] void syntax_error(const std::optional<Sym>& errtoken) {
  if (!errtoken) throw py_exception("SyntaxError", "at EOF");
  throw py_exception("SyntaxError",
                     "around token '" + errtoken->value.str + "' type " + errtoken->type);
}

// `LRParser.parseopt_notrack` on the input `tokens`, which returns the
// value of the start symbol, or None when error recovery reaches the
// end of the input.
PValue parse(const std::vector<Sym>& tokens, const FunctionHook& functions,
             const IdentifierHook& identifiers) {
  std::size_t next = 0;
  auto get_token = [&]() -> std::optional<Sym> {
    if (next >= tokens.size()) return std::nullopt;
    return tokens[next++];
  };
  std::optional<Sym> lookahead;
  std::vector<std::optional<Sym>> lookaheadstack;
  std::vector<int> statestack{0};
  std::vector<Sym> symstack{Sym{"$end", {}}};
  int state = 0;
  int errorcount = 0;
  bool errorok = true;
  while (true) {
    std::int16_t t;
    if (tables::kDefaulted[state] == tables::kNone) {
      if (!lookahead) {
        if (lookaheadstack.empty()) {
          lookahead = get_token();
        } else {
          lookahead = lookaheadstack.back();
          lookaheadstack.pop_back();
        }
        if (!lookahead) lookahead = Sym{"$end", {}};
      }
      t = action_at(state, lookahead->type);
    } else {
      t = tables::kDefaulted[state];
    }
    if (t != tables::kNone) {
      if (t > 0) {
        statestack.push_back(t);
        state = t;
        symstack.push_back(*lookahead);
        lookahead.reset();
        if (errorcount) --errorcount;
        continue;
      }
      if (t < 0) {
        const auto& prod = tables::kProductions[-t];
        const auto plen = static_cast<std::size_t>(prod.len);
        Sym sym{prod.name, {}};
        // targ is symstack[-plen-1:] with its first symbol replaced by
        // the new one, and p[i] is the value of targ[i].
        std::vector<Sym> targ(symstack.end() - static_cast<std::ptrdiff_t>(plen + 1), symstack.end());
        targ[0] = sym;
        symstack.resize(symstack.size() - plen);
        try {
          std::vector<PValue> p;
          p.reserve(targ.size());
          for (const auto& s : targ) p.push_back(s.value);
          sym.value = run_action(action_of(prod.func), p, functions, identifiers);
          statestack.resize(statestack.size() - plen);
          symstack.push_back(sym);
          state = tables::kGoto[statestack.back()][nonterminal_index(prod.name)];
          statestack.push_back(state);
        } catch (const PyException& e) {
          if (e.type != "SyntaxError") throw;
          lookaheadstack.push_back(lookahead);
          if (plen >= 2) {
            symstack.insert(symstack.end(), targ.begin() + 1, targ.end() - 1);
          }
          statestack.pop_back();
          state = statestack.back();
          sym.type = "error";
          sym.value = PValue{PValue::kStr, "error", {}};
          lookahead = sym;
          errorcount = kErrorCount;
          errorok = false;
        }
        continue;
      }
      // An action of 0 accepts the input.
      return symstack.back().value;
    }
    // A syntax error.
    if (errorcount == 0 || errorok) {
      errorcount = kErrorCount;
      errorok = false;
      std::optional<Sym> errtoken = lookahead;
      if (errtoken->type == "$end") errtoken.reset();
      syntax_error(errtoken);
    }
    errorcount = kErrorCount;
    if (statestack.size() <= 1 && lookahead->type != "$end") {
      lookahead.reset();
      state = 0;
      lookaheadstack.clear();
      continue;
    }
    if (lookahead->type == "$end") return {};
    if (lookahead->type != "error") {
      if (symstack.back().type == "error") {
        lookahead.reset();
        continue;
      }
      lookaheadstack.push_back(lookahead);
      lookahead = Sym{"error", {}};
    } else {
      symstack.pop_back();
      statestack.pop_back();
      state = statestack.back();
    }
  }
}

}  // namespace

EscapeExpansion expand_escapes_without_names(std::string_view inner) {
  const CodePoints s = pyrepr::code_points(inner);
  EscapeExpansion out;
  std::size_t i = 0;
  while (i < s.size()) {
    bool is_name = false;
    const std::size_t len = escape_length(s, i, false, is_name);
    if (len == 0) {
      append_utf8(out.expanded, s[i]);
      ++i;
      continue;
    }
    PyException error;
    const auto cp = decode_escape(s, i, len, error);
    if (!cp) {
      out.rest = utf8(CodePoints(s.begin() + static_cast<std::ptrdiff_t>(i), s.end()));
      break;
    }
    append_utf8(out.expanded, *cp);
    i += len;
  }
  return out;
}

PyException py_exception(std::string type, std::string message) {
  PyException e;
  e.repr = type + "(" + repr_str(message) + ")";
  e.type = std::move(type);
  e.message = std::move(message);
  return e;
}

std::string EvalResult::str() const {
  return is_unsigned ? std::to_string(bits) : std::to_string(static_cast<std::int64_t>(bits));
}

std::string repr_str(std::string_view s) {
  const CodePoints cps = pyrepr::code_points(s);
  const bool has_single = std::find(cps.begin(), cps.end(), std::uint32_t{'\''}) != cps.end();
  const bool has_double = std::find(cps.begin(), cps.end(), std::uint32_t{'"'}) != cps.end();
  const char quote = has_single && !has_double ? '"' : '\'';
  std::string out(1, quote);
  for (const auto c : cps) {
    if (c == '\\' || c == static_cast<std::uint32_t>(quote)) {
      out += '\\';
      out += static_cast<char>(c);
    } else if (c == '\t') {
      out += "\\t";
    } else if (c == '\n') {
      out += "\\n";
    } else if (c == '\r') {
      out += "\\r";
    } else if (c < 0x20 || c == 0x7F) {
      out += "\\x";
      append_hex(out, c, 2);
    } else if (c < 0x7F || printable(c)) {
      append_utf8(out, c);
    } else if (c <= 0xFF) {
      out += "\\x";
      append_hex(out, c, 2);
    } else if (c <= 0xFFFF) {
      out += "\\u";
      append_hex(out, c, 4);
    } else {
      out += "\\U";
      append_hex(out, c, 8);
    }
  }
  out += quote;
  return out;
}

EvalResult evaluate(const Tokens& tokens, const FunctionHook& functions,
                    const IdentifierHook& identifiers) {
  // The evaluator's lexer drops whitespace, line continuations, and
  // comments, and passes every other token to the parser.
  std::vector<Sym> input;
  for (const auto& tok : tokens) {
    switch (tok->type) {
      case Type::CPP_WS:
      case Type::CPP_LINECONT:
      case Type::CPP_COMMENT1:
      case Type::CPP_COMMENT2:
        break;
      default:
        input.push_back(Sym{type_name(*tok), PValue{PValue::kStr, tok->value, {}}});
        break;
    }
  }
  const PValue result = parse(input, functions, identifiers);
  switch (result.kind) {
    case PValue::kValue:
      return result.val.value();
    case PValue::kStr:
      throw py_exception("AttributeError", "'str' object has no attribute 'value'");
    case PValue::kNone:
      break;
  }
  throw py_exception("AttributeError", "'NoneType' object has no attribute 'value'");
}

}  // namespace c6502::pcpp
