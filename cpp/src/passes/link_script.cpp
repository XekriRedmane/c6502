/*
Linker scripts for `compile.py -T script.ld`: a subset of GNU ld's
script language.

A script names the memory regions of the machine and says which
sections go where.

    MEMORY
    {
        ram (rwx) : ORIGIN = 0x0800, LENGTH = 0x5800
        rom (rx)  : ORIGIN = 0xD000, LENGTH = 0x2FFA
    }

    SECTIONS
    {
        .text : { *(.text.boot) *(.text .text.*) } > rom
        .rodata : { *(.rodata .rodata.*) } > rom
        .data : { *(.data .data.*) } > ram AT> rom
        .bss (NOLOAD) : { *(.bss .bss.*) } > ram
        .vectors 0xFFFA : { KEEP(*(.vectors)) }
        __end = .;
    }

`parse_script` turns the text into a `Script`, and the linker lays the
program out from it (link_layout.cpp). The subset is this.

* `MEMORY` regions, `name [(attrs)] : ORIGIN = expr, LENGTH = expr`,
  with `org`, `o`, `len`, and `l` as GNU spells them. The attributes
  are accepted and ignored.
* In `SECTIONS`, output sections and symbol assignments. An output
  section is `name [address] [(NOLOAD)] : [AT(lma)] [ALIGN(n)]
  { ... } [> region] [AT> region]`, with an optional `;` after it.
  `(NOREORDER)` in place of `(NOLOAD)` is c6502's own. It keeps the
  section's functions in the order the script gives them, where the
  linker would otherwise move a function to follow one that ends by
  jumping to it (link_layout.cpp). Inside the braces go input section
  descriptions `*(pattern ...)` and `KEEP(*(pattern ...))` (every
  section is kept, since the linker discards nothing), symbol
  assignments, `. = ALIGN(n);`, and `. = . + n;` or `. += n;` to skip
  `n` bytes. Patterns use `*`, `?`, and `[...]` as in GNU ld. The file
  part is `*`, or a pattern for an input file's name without its
  directory, as in `zstring.asm(.text)`.
* Symbol assignments `sym = expr;` and `PROVIDE(sym = expr);`, inside
  or after the output sections.
* Expressions of numbers (decimal, `0x` hex, and a `K` or `M`
  suffix), symbols, `.`, `+ - * / & | ^ << >>`, unary `-` and `~`,
  parentheses, `ALIGN(n)`, `ALIGN(expr, n)`, `ORIGIN(region)`,
  `LENGTH(region)`, `ADDR(section)`, `LOADADDR(section)`, and
  `SIZEOF(section)`.
* Comments, as in C, between a slash and a star and a star and a
  slash, and from `//` to the end of the line.

Anything else (`ENTRY`, `INCLUDE`, `/DISCARD/`, `FILL`, `BYTE`,
`SORT`, `.` assigned outside an output section) is a `ScriptError`.

The values are held in 128 bits, and any value of more than 125 bits
throws `InternalError`, so that no sum or difference of two values can
overflow.
*/
#include "passes/link_script.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <set>

#include "argparse.h"
#include "errors.h"
#include "lark_lexer.h"
#include "pcpp_text.h"
#include "py_regex.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else
#include <clocale>
#include <langinfo.h>
#endif

namespace c6502::passes::link_script {

std::vector<const OutputSection*> Script::output_sections() const {
  std::vector<const OutputSection*> out;
  for (const Statement& s : statements) {
    if (const auto* sec = std::get_if<OutputSection>(&s)) out.push_back(sec);
  }
  return out;
}

namespace {

[[noreturn]] void script_error(const std::string& message) {
  throw CompileError("ScriptError", message);
}

std::string repr(std::string_view s) { return pcpp::py_repr(s); }

const asdl::Int kLimit = asdl::Int(1) << 125;

asdl::Int bounded(asdl::Int v) {
  if (v > kLimit || v < -kLimit) {
    throw InternalError(
        "a linker script's value takes more than 125 bits");
  }
  return v;
}

const py_regex::Regex& token_regex() {
  static const py_regex::Regex re(
      "(?sx)\n"
      "    (?P<space>\\s+|/\\*.*?\\*/|//[^\\n]*)\n"
      "  | (?P<num>(?:0[xX][0-9A-Fa-f]+|\\d+)[KM]?\\b)\n"
      "  | (?P<op><<|>>|\\+=|-=|AT>|[{}():;=,>+\\-*/&|^~<])\n"
      "  | (?P<name>[A-Za-z_.*?\\[\\]$][A-Za-z0-9_.*?\\[\\]$\\-]*)\n");
  return re;
}

const py_regex::Regex& space_regex() {
  static const py_regex::Regex re("(?s)\\s+|/\\*.*?\\*/|//[^\\n]*");
  return re;
}

std::vector<std::string> tokens(std::string_view utf8) {
  const std::u32string text = lark::decode_utf8(utf8);
  std::vector<std::string> out;
  std::size_t pos = 0;
  while (pos < text.size()) {
    const auto end = token_regex().match(text, pos);
    if (!end) {
      std::size_t line = 1;
      for (std::size_t i = 0; i < pos; ++i) line += text[i] == U'\n';
      const std::u32string_view shown =
          std::u32string_view(text).substr(pos, 20);
      script_error("line " + std::to_string(line) + ": can't read " +
                   repr(lark::encode_utf8(shown)));
    }
    if (!space_regex().match(text, pos)) {
      out.push_back(lark::encode_utf8(std::u32string_view(text).substr(pos, *end - pos)));
    }
    pos = *end;
  }
  return out;
}

int digit_value(char32_t c) {
  if (c >= U'0' && c <= U'9') return static_cast<int>(c - U'0');
  char32_t start = c;
  while (start > 0 && pcpp::is_digit(start - 1)) --start;
  return static_cast<int>((c - start) % 10);
}

std::optional<asdl::Int> int_base0(std::string_view digits) {
  if (digits.size() > 2 && digits[0] == '0' && (digits[1] == 'x' || digits[1] == 'X')) {
    asdl::Int v = 0;
    for (const char c : digits.substr(2)) {
      const int d = c <= '9' ? c - '0' : (c | 0x20) - 'a' + 10;
      v = bounded(v * 16 + d);
    }
    return v;
  }
  const std::u32string cps = lark::decode_utf8(digits);
  std::vector<int> values;
  for (const char32_t c : cps) values.push_back(digit_value(c));
  if (values.size() > 1 && values[0] == 0) {
    for (const int d : values) {
      if (d != 0) return std::nullopt;
    }
  }
  asdl::Int v = 0;
  for (const int d : values) v = bounded(v * 10 + d);
  return v;
}

const py_regex::Regex& number_regex() {
  static const py_regex::Regex re("(?:(?:0[xX][0-9A-Fa-f]+|\\d+)[KM]?)\\Z");
  return re;
}

bool is_number(std::string_view tok) {
  const std::u32string text = lark::decode_utf8(tok);
  return number_regex().match(text, 0).has_value();
}

bool is_symbol(std::string_view tok) {
  if (tok.empty()) return false;
  const auto start = [](char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || c == '.';
  };
  if (!start(tok[0])) return false;
  for (const char c : tok.substr(1)) {
    if (!start(c) && !(c >= '0' && c <= '9')) return false;
  }
  return true;
}

constexpr std::string_view kFunctions[] = {"ALIGN", "ORIGIN", "LENGTH",
                                           "ADDR",  "LOADADDR", "SIZEOF"};
constexpr std::string_view kSectionTypes[] = {"NOLOAD", "NOREORDER"};

template <std::size_t N>
bool one_of(const std::string* tok, const std::string_view (&set)[N]) {
  if (tok == nullptr) return false;
  for (const auto s : set) {
    if (*tok == s) return true;
  }
  return false;
}

bool is(const std::string* tok, std::string_view s) { return tok != nullptr && *tok == s; }

Expr num(asdl::Int v) {
  Expr e;
  e.kind = Expr::Kind::Num;
  e.value = v;
  return e;
}

Expr sym(std::string name) {
  Expr e;
  e.kind = Expr::Kind::Sym;
  e.name = std::move(name);
  return e;
}

Expr dot() {
  Expr e;
  e.kind = Expr::Kind::Dot;
  return e;
}

Expr binary(std::string op, Expr left, Expr right) {
  Expr e;
  e.kind = Expr::Kind::Binary;
  e.op = std::move(op);
  e.args.push_back(std::move(left));
  e.args.push_back(std::move(right));
  return e;
}

asdl::Int constant(const Expr& e, const std::string& what) {
  switch (e.kind) {
    case Expr::Kind::Num:
      return e.value;
    case Expr::Kind::Unary: {
      const asdl::Int v = constant(e.args[0], what);
      if (e.op == "-") return -v;
      if (e.op == "~") return ~v & 0xFFFF;
      break;
    }
    case Expr::Kind::Binary: {
      const asdl::Int a = constant(e.args[0], what);
      const asdl::Int b = constant(e.args[1], what);
      const std::string& op = e.op;
      if (op == "/") {
        if (b == 0) script_error(what + " divides by zero");
        return asdl::floor_div(a, b);
      }
      if ((op == "<<" || op == ">>") && b < 0) script_error(what + " shifts by a negative count");
      if (op == "+") return bounded(a + b);
      if (op == "-") return bounded(a - b);
      if (op == "*") {
        const asdl::Int ma = a < 0 ? -a : a;
        const asdl::Int mb = b < 0 ? -b : b;
        if (ma != 0 && mb > kLimit / ma) bounded(kLimit + 1);
        return a * b;
      }
      if (op == "&") return a & b;
      if (op == "|") return a | b;
      if (op == "^") return a ^ b;
      if (op == "<<") {
        if (a == 0) return 0;
        const asdl::Int ma = a < 0 ? -a : a;
        if (b > 125 || ma > (kLimit >> static_cast<int>(b))) bounded(kLimit + 1);
        return a * (asdl::Int(1) << static_cast<int>(b));
      }
      if (op == ">>") {
        if (b > 126) return a < 0 ? -1 : 0;
        return a >> static_cast<int>(b);
      }
      break;
    }
    default:
      break;
  }
  script_error(what + " has to be a number");
}

class Parser {
 public:
  explicit Parser(std::string_view text) : toks_(tokens(text)) {}

  const std::string* peek(std::size_t k = 0) const {
    const std::size_t j = i_ + k;
    return j < toks_.size() ? &toks_[j] : nullptr;
  }

  std::string take(std::optional<std::string_view> want = std::nullopt) {
    const std::string* tok = peek();
    if (tok == nullptr) {
      script_error(want ? "unexpected end of script; expected " + std::string(*want)
                        : std::string("unexpected end of script"));
    }
    if (want && *tok != *want) {
      script_error("expected " + repr(*want) + ", found " + repr(*tok));
    }
    ++i_;
    return *tok;
  }

  Script script() {
    Script out;
    while (peek() != nullptr) {
      const std::string word = take();
      if (word == "MEMORY") {
        memory(out);
      } else if (word == "SECTIONS") {
        sections(out);
      } else {
        script_error(repr(word) + " isn't supported; a script has MEMORY and SECTIONS commands");
      }
    }
    return out;
  }

  void memory(Script& out) {
    take("{");
    while (!is(peek(), "}")) {
      const std::string name = take();
      if (is(peek(), "(")) {
        while (take() != ")") {
        }
      }
      take(":");
      std::optional<asdl::Int> origin;
      std::optional<asdl::Int> length;
      for (int k = 0; k < 2; ++k) {
        std::string key = take();
        if (key == "org" || key == "o") key = "ORIGIN";
        if (key == "len" || key == "l") key = "LENGTH";
        if (key != "ORIGIN" && key != "LENGTH") {
          script_error("region " + repr(name) + ": expected ORIGIN or LENGTH, found " + repr(key));
        }
        std::optional<asdl::Int>& slot = key == "ORIGIN" ? origin : length;
        if (slot) script_error("region " + repr(name) + " gives " + key + " twice");
        take("=");
        slot = constant(expr(), key + " of " + repr(name));
        if (is(peek(), ",")) take();
      }
      if (out.regions.contains(name)) script_error("region " + repr(name) + " is defined twice");
      out.regions.insert_or_assign(name, Region{name, *origin, *length});
    }
    take("}");
  }

  void sections(Script& out) {
    take("{");
    while (!is(peek(), "}")) {
      if (is(peek(), ";")) {
        take();
        continue;
      }
      if (auto a = assignment()) {
        if (a->symbol == ".") {
          script_error(
              "`. =` outside an output section isn't supported; give the output section an "
              "address or a region");
        }
        out.statements.emplace_back(std::move(*a));
        continue;
      }
      out.statements.emplace_back(output_section());
    }
    take("}");
  }

  std::optional<Assign> assignment() {
    if (is(peek(), "PROVIDE") && is(peek(1), "(")) {
      take();
      take("(");
      std::string name = take();
      take("=");
      Expr e = expr();
      take(")");
      take(";");
      return Assign{std::move(name), std::move(e)};
    }
    const std::string* next = peek(1);
    if ((is(next, "=") || is(next, "+=") || is(next, "-=")) && peek() != nullptr &&
        !is(peek(), "{") && !is(peek(), "}")) {
      std::string name = take();
      const std::string op = take();
      Expr e = expr();
      take(";");
      if (op != "=") {
        e = binary(op.substr(0, 1), name == "." ? dot() : sym(name), std::move(e));
      }
      return Assign{std::move(name), std::move(e)};
    }
    return std::nullopt;
  }

  OutputSection output_section() {
    OutputSection sec;
    sec.name = take();
    if (!is(peek(), ":") && !is(peek(), "(")) {
      sec.address = expr();
    } else if (is(peek(), "(") && !one_of(peek(1), kSectionTypes)) {
      sec.address = expr();
    }
    if (is(peek(), "(")) {
      take();
      const std::string kind = take();
      if (!one_of(&kind, kSectionTypes)) {
        script_error("output section " + repr(sec.name) + " has type " + repr(kind) +
                     "; the types are NOLOAD, NOREORDER");
      }
      take(")");
      if (kind == "NOLOAD") {
        sec.noload = true;
      } else {
        sec.reorder = false;
      }
    }
    take(":");
    while (is(peek(), "AT") || is(peek(), "ALIGN")) {
      const std::string word = take();
      take("(");
      Expr value = expr();
      take(")");
      if (word == "AT") {
        sec.at = std::move(value);
      } else {
        sec.align = std::move(value);
      }
    }
    take("{");
    while (!is(peek(), "}")) sec.contents.push_back(content(sec.name));
    take("}");
    while (is(peek(), ">") || is(peek(), "AT>") || is(peek(), "AT")) {
      std::string tok = take();
      if (tok == "AT") {
        take(">");
        tok = "AT>";
      }
      std::string region = take();
      if (tok == ">") {
        sec.region = std::move(region);
      } else {
        sec.load_region = std::move(region);
      }
    }
    if (is(peek(), ";")) take();
    return sec;
  }

  Content content(const std::string& where) {
    while (is(peek(), ";")) take();
    if (auto a = assignment()) return std::move(*a);
    const bool keep = is(peek(), "KEEP");
    if (keep) {
      take();
      take("(");
    }
    std::string files = take();
    if (files == "*" && peek() != nullptr && !is(peek(), "(")) files += take();
    for (const std::string_view bad : {"(", ")", "{", "}", ";", ":", "="}) {
      if (files == bad) {
        script_error("in " + where +
                     ": an input section description starts with the file it takes sections "
                     "from, such as `*` or `zstring.asm`, not " +
                     repr(files));
      }
    }
    take("(");
    Input in;
    while (!is(peek(), ")")) {
      std::string pattern = take();
      if (pattern == ",") continue;
      in.patterns.push_back(std::move(pattern));
    }
    take(")");
    if (keep) take(")");
    in.file = std::move(files);
    return in;
  }

  Expr expr(std::size_t level = 0) {
    static const std::vector<std::vector<std::string_view>> kLevels = {
        {"|"}, {"^"}, {"&"}, {"<<", ">>"}, {"+", "-"}, {"*", "/"}};
    if (level == kLevels.size()) return unary();
    Expr left = expr(level + 1);
    const auto at_level = [&](const std::string* tok) {
      if (tok == nullptr) return false;
      for (const auto op : kLevels[level]) {
        if (*tok == op) return true;
      }
      return false;
    };
    while (at_level(peek())) {
      std::string op = take();
      left = binary(std::move(op), std::move(left), expr(level + 1));
    }
    return left;
  }

  Expr unary() {
    const std::string tok = take();
    if (tok == "-" || tok == "~") {
      Expr e;
      e.kind = Expr::Kind::Unary;
      e.op = tok;
      e.args.push_back(unary());
      return e;
    }
    if (tok == "(") {
      Expr inner = expr();
      take(")");
      return inner;
    }
    if (tok == ".") return dot();
    if (is_number(tok)) {
      const char last = tok.back();
      const asdl::Int scale = last == 'K' ? 1024 : last == 'M' ? 1024 * 1024 : 1;
      const std::string digits = scale != 1 ? tok.substr(0, tok.size() - 1) : tok;
      const auto v = int_base0(digits);
      if (!v) {
        script_error("can't read the number " + repr(tok) +
                     ": a decimal number other than 0 can't begin with 0");
      }
      return num(bounded(*v * scale));
    }
    if (one_of(&tok, kFunctions) && is(peek(), "(")) {
      take("(");
      Expr e;
      e.kind = Expr::Kind::Call;
      e.fn = tok;
      e.args.push_back(expr());
      while (is(peek(), ",")) {
        take();
        e.args.push_back(expr());
      }
      take(")");
      return e;
    }
    if (is_symbol(tok)) return sym(tok);
    script_error("expected an expression, found " + repr(tok));
  }

 private:
  std::vector<std::string> toks_;
  std::size_t i_ = 0;
};

}  // namespace

Script parse_script(std::string_view text) {
  Script script = Parser(text).script();
  std::set<std::string, std::less<>> seen;
  for (const OutputSection* sec : script.output_sections()) {
    if (seen.count(sec->name) != 0) {
      script_error("output section " + repr(sec->name) + " is listed twice");
    }
    seen.insert(sec->name);
    for (const auto* region : {&sec->region, &sec->load_region}) {
      if (*region && !script.regions.contains(**region)) {
        script_error("output section " + repr(sec->name) + " names region " + repr(**region) +
                     ", which MEMORY doesn't define");
      }
    }
    if (sec->noload && (sec->load_region || sec->at)) {
      script_error("output section " + repr(sec->name) +
                   " is NOLOAD, so it has no load address for AT to give");
    }
  }
  return script;
}

std::string locale_encoding() {
  if (const char* utf8 = std::getenv("PYTHONUTF8"); utf8 != nullptr && std::string(utf8) == "1") {
    return "utf-8";
  }
#ifdef _WIN32
  return "cp" + std::to_string(GetACP());
#else
  const char* before = std::setlocale(LC_CTYPE, nullptr);
  const std::string saved = before == nullptr ? "C" : before;
  const char* name = std::setlocale(LC_CTYPE, "");
  const std::string locale = name == nullptr ? "C" : name;
  std::string codeset = nl_langinfo(CODESET);
  std::setlocale(LC_CTYPE, saved.c_str());
  if (locale == "C" || locale == "POSIX") return "utf-8";
  return codeset;
#endif
}

Script read_script(const std::string& path) {
  namespace fs = std::filesystem;
  const auto link_error = [&](const std::string& reason) {
    throw CompileError("LinkError", path + ": " + reason);
  };
  const std::string normal = argparse::path_str(path);
  const fs::path p(std::u8string(normal.begin(), normal.end()));
  std::error_code ec;
  if (!fs::exists(p, ec)) link_error("No such file or directory");
  if (fs::is_directory(p, ec)) {
#ifdef _WIN32
    link_error("Permission denied");
#else
    link_error("Is a directory");
#endif
  }
  std::ifstream in(p, std::ios::binary);
  if (!in) link_error("Permission denied");
  const std::string bytes((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
  const std::string encoding = locale_encoding();
  std::string text;
  try {
    text = pcpp::decode_text_file(bytes, encoding);
  } catch (const CompileError& e) {
    if (e.python_type() == "LookupError") {
      throw InternalError("linker scripts can't be read in the locale's encoding, " +
                          encoding);
    }
    link_error(e.what());
  }
  try {
    return parse_script(text);
  } catch (const CompileError& e) {
    if (e.python_type() != "ScriptError") throw;
    link_error(e.what());
  }
  throw InternalError("unreachable");
}

}  // namespace c6502::passes::link_script
