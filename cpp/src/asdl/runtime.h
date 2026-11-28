// The support code of the IR classes that asdlgen (cpp/tools/asdlgen.h)
// generates: the integer type of an ASDL `int`, the box that holds a
// recursive field, the dump format's reader and writer, and the helpers
// that stand in for Python's `isinstance`.
#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>

#if !defined(__SIZEOF_INT128__) && defined(_MSC_VER)
#include <__msvc_int128.hpp>
#endif

namespace c6502::asdl {

// An ASDL `int`. A Python int has no bound, and the IR holds values
// that need 64 bits unsigned (a `double`'s bit pattern, an `unsigned
// long long` constant) next to negative ones, so the C++ type is a
// signed 128-bit integer. GCC and Clang have `__int128`. MSVC has no
// built-in one, and its standard library's `std::_Signed128` (the type
// behind `iota_view`'s difference type) stands in. Its conversions to
// the built-in integers are explicit, so ported code writes
// `static_cast<int>(x)`, which compiles everywhere.
#if defined(__SIZEOF_INT128__)
__extension__ typedef __int128 Int;
#elif defined(_MSC_VER)
using Int = std::_Signed128;
#else
#error "no 128-bit integer type"
#endif

// Returns the decimal digits of `v`, with a leading `-` when it is
// negative, as Python's `str` gives them.
std::string int_to_string(Int v);

// Returns Python's `a // b` and `a % b`, which round toward negative
// infinity where C++'s `/` and `%` round toward zero.
Int floor_div(Int a, Int b);
Int floor_mod(Int a, Int b);

namespace detail {

template <class U, class V>
struct is_alternative : std::false_type {};
template <class U, class... Ts>
struct is_alternative<U, std::variant<Ts...>>
    : std::bool_constant<(std::is_same_v<U, Ts> || ...)> {};
template <class U, class V>
inline constexpr bool is_alternative_v = is_alternative<U, V>::value;

}  // namespace detail

// Holds a field whose type is in a cycle with the struct that holds it
// (`exp` in a `Binary`), which a struct can't hold by value. It has
// value semantics: copying a box copies the value, and `==` compares
// the values, so a struct with a box compares as the Python dataclass
// does. A moved-from box is empty and may only be assigned to.
template <class T>
class Box {
 public:
  Box() : p_(std::make_unique<T>()) {}
  // Takes a `T`, or a constructor of the sum type `T`, so a field of
  // type `Box<Type_exp>` takes a `Var{...}` as a `Type_exp` field does.
  // The constraint compares the types by name only. A constructor that
  // took `const T&` would make the compiler ask whether a `Box`
  // converts to a `T`, which needs `T` complete, and `std::optional`
  // asks that where a recursive type's boxes are declared, before `T`
  // is complete.
  template <class U>
    requires(std::is_same_v<std::remove_cvref_t<U>, T> ||
             detail::is_alternative_v<std::remove_cvref_t<U>, T>)
  Box(U&& v) : p_(std::make_unique<T>(std::forward<U>(v))) {}  // NOLINT
  Box(const Box& other) : p_(std::make_unique<T>(*other.p_)) {}
  Box(Box&&) noexcept = default;
  Box& operator=(const Box& other) {
    if (this != &other) {
      p_ = std::make_unique<T>(*other.p_);
    }
    return *this;
  }
  Box& operator=(Box&&) noexcept = default;
  ~Box() = default;

  T& operator*() { return *p_; }
  const T& operator*() const { return *p_; }
  T* operator->() { return p_.get(); }
  const T* operator->() const { return p_.get(); }

  friend bool operator==(const Box& a, const Box& b) { return *a.p_ == *b.p_; }

 private:
  std::unique_ptr<T> p_;
};

// Returns whether the sum-type value `v` holds one of the constructors
// `Ts`, as Python's `isinstance(v, (T1, T2))` does.
template <class... Ts, class V>
bool isa(const V& v) noexcept {
  return (std::holds_alternative<Ts>(v) || ...);
}

// Returns a pointer to the constructor `T` that `v` holds, or null.
template <class T, class V>
const T* as(const V& v) noexcept {
  return std::get_if<T>(&v);
}
template <class T, class V>
T* as(V& v) noexcept {
  return std::get_if<T>(&v);
}

// Selects a reader by the type it returns: `read(r, Tag<T>{})`.
template <class T>
struct Tag {};

// Raised for text that isn't in the dump format, with the line and
// column of the problem.
class ParseError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

// Writes the dump format (cpp/README.md, "The dump format"). The
// generated `write` functions call these.
class Writer {
 public:
  void write_name(std::string_view name) { out_ += name; }
  void open() { out_ += '('; }
  void close() { out_ += ')'; }
  void space() { out_ += ' '; }
  void write_int(Int v);
  void write_bool(bool v) { out_ += v ? "true" : "false"; }
  // Writes a string in double quotes. The C++ IR holds strings in
  // UTF-8, and each code point outside printable ASCII is escaped as
  // `\xHH` below 0x100 and `\u{H...}` above it, as Python's writer
  // escapes the characters of a `str`.
  void write_string(std::string_view s);
  void write_none() { out_ += "None"; }

  // A sequence is `[a b c]`, or for the element types in
  // `asdl.DUMP_LINE_TYPES`, one element per line, indented by two
  // spaces for each sequence of those types it is in.
  void begin_seq(bool lines);
  void seq_item(bool lines);
  void end_seq(bool lines, bool empty);

  std::string& str() { return out_; }

 private:
  void newline();

  std::string out_;
  int depth_ = 0;
  bool first_ = true;
};

// Reads the dump format. The generated `read` functions call these.
class Reader {
 public:
  explicit Reader(std::string_view text) : s_(text) {}

  // Reads a constructor's or a product type's name.
  std::string_view read_name();
  void expect_name(std::string_view name);
  void open() { expect('('); }
  void close() { expect(')'); }
  Int read_int();
  bool read_bool();
  std::string read_string();
  // Reads `None` and returns true, or returns false and reads nothing.
  bool read_none();
  void begin_seq() { expect('['); }
  // Reads the `]` that ends a sequence and returns true, or returns
  // false when another element follows.
  bool end_seq();
  // Checks that nothing but whitespace is left.
  void expect_end();
  [[noreturn]] void unknown_constructor(std::string_view name,
                                        std::string_view type);
  [[noreturn]] void error(const std::string& message) const;

  // The offset of the next character to read.
  std::size_t pos() const { return i_; }

 private:
  void skip_ws();
  void expect(char c);

  std::string_view s_;
  std::size_t i_ = 0;
};

// Returns the dump of `v`, ending with a newline.
template <class T>
std::string dump(const T& v) {
  Writer w;
  write(w, v);
  w.str() += '\n';
  return std::move(w.str());
}

// Returns Python's `repr` of the IR value `v`, as the generated
// dataclass gives it, which the front end's messages show.
template <class T>
std::string py_repr(const T& v) {
  std::string out;
  repr(out, v);
  return out;
}

// Returns the name of the constructor `v` holds, as Python's
// `type(v).__name__` gives it, which is what its `repr` starts with.
template <class T>
std::string constructor_name(const T& v) {
  std::string out = py_repr(v);
  out.resize(out.find('('));
  return out;
}

// Returns pretty.py's `pretty(v)` of the IR value `v`, which
// `compile.py --parse`, `--resolve`, and `--tac` print: the class name,
// a `name=value,` line for each field with two spaces of indentation
// per level, lists one item to a line, and Python's `repr` of each leaf.
template <class T>
std::string py_pretty(const T& v) {
  std::string out;
  pretty(out, v, 0);
  return out;
}

// Returns the value of type `T` that `text` holds in the dump format.
template <class T>
T load(std::string_view text) {
  Reader r(text);
  T v = read(r, Tag<T>{});
  r.expect_end();
  return v;
}

}  // namespace c6502::asdl
