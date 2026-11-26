#include "binutils/assembler.h"

#include <algorithm>
#include <map>
#include <memory>
#include <variant>

#include "argparse.h"
#include "binutils/opcodes.h"
#include "errors.h"
#include "passes/zp_link_metadata.h"
#include "pcpp_text.h"
#include "py_int.h"
#include "pyrepr.h"

namespace c6502::binutils::assembler {

namespace {

using asdl::Int;
using opcodes::Mode;
using U32 = std::u32string;
using U32View = std::u32string_view;

U32 to_u32(std::string_view s) {
  U32 out;
  for (std::size_t i = 0; i < s.size();) {
    std::size_t len = 0;
    out += static_cast<char32_t>(pcpp::decode_at(s, i, len));
    i += len;
  }
  return out;
}

std::string to_utf8(U32View s) {
  std::string out;
  for (const char32_t c : s) pcpp::append_utf8(out, c);
  return out;
}

bool space(char32_t c) { return pcpp::is_space(c); }

U32 rstrip(U32View s) {
  std::size_t end = s.size();
  while (end > 0 && space(s[end - 1])) --end;
  return U32(s.substr(0, end));
}

U32 strip(U32View s) {
  std::size_t start = 0;
  while (start < s.size() && space(s[start])) ++start;
  return rstrip(s.substr(start));
}

bool all_space(U32View s, std::size_t from) {
  for (std::size_t i = from; i < s.size(); ++i) {
    if (!space(s[i])) return false;
  }
  return true;
}

std::string repr(U32View s) { return pcpp::py_repr(to_utf8(s)); }

std::string str(Int v) { return asdl::int_to_string(v); }

[[noreturn]] void error(const std::string& message) {
  throw CompileError("AssemblerError", message);
}

bool is_runtime_name(std::string_view name) {
  return name == "SSP" || name == "FP" || name == "HARGS" || name == "DPTR";
}

bool is_shift(std::string_view op) {
  return op == "ASL" || op == "LSR" || op == "ROL" || op == "ROR";
}

bool is_directive(std::string_view op) {
  return op == "SUBROUTINE" || op == "DC.B" || op == "DC.W" || op == "DC.L" || op == "DS.B" ||
         op == "PROCESSOR";
}

constexpr std::string_view kMetaBegin = "; @zp-link-meta-begin";
constexpr std::string_view kMetaEnd = "; @zp-link-meta-end";

bool symbol_start(char32_t c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || c == '.' || c == '@';
}
bool symbol_char(char32_t c) { return symbol_start(c) || (c >= '0' && c <= '9') || c == '%'; }

std::size_t symbol_end(U32View s, std::size_t pos) {
  if (pos >= s.size() || !symbol_start(s[pos])) return pos;
  std::size_t end = pos + 1;
  while (end < s.size() && symbol_char(s[end])) ++end;
  return end;
}

std::optional<std::pair<std::string, U32>> match_equ(U32View line) {
  const std::size_t name_end = symbol_end(line, 0);
  if (name_end == 0) return std::nullopt;
  std::size_t p = name_end;
  while (p < line.size() && space(line[p])) ++p;
  if (p == name_end) return std::nullopt;
  std::size_t q = p;
  if (line.substr(p, 3) == U"EQU" || line.substr(p, 3) == U"equ") {
    q = p + 3;
  } else if (p < line.size() && line[p] == '=') {
    q = p + 1;
  } else {
    return std::nullopt;
  }
  std::size_t r = q;
  while (r < line.size() && space(line[r])) ++r;
  if (r == q || r == line.size()) return std::nullopt;
  return std::make_pair(to_utf8(line.substr(0, name_end)), rstrip(line.substr(r)));
}

std::optional<std::string> match_label(U32View line) {
  const std::size_t end = symbol_end(line, 0);
  if (end == 0) return std::nullopt;
  if (all_space(line, end) || (line[end] == ':' && all_space(line, end + 1))) {
    return to_utf8(line.substr(0, end));
  }
  return std::nullopt;
}

U32 strip_comment(U32View line) {
  char32_t quote = 0;
  for (std::size_t i = 0; i < line.size(); ++i) {
    const char32_t c = line[i];
    if (quote != 0) {
      if (c == quote) quote = 0;
    } else if (c == '"' || c == '\'') {
      if (c == '\'' && i + 2 < line.size() && line[i + 2] == '\'') continue;
      quote = c;
    } else if (c == ';') {
      return U32(line.substr(0, i));
    }
  }
  return U32(line);
}

constexpr int kMaxDepth = 400;

[[noreturn]] void too_deep(const std::string& where) {
  throw CompileError("RecursionError",
                     where + ": the expression is nested deeper than the C++ assembler follows, " +
                         std::to_string(kMaxDepth) +
                         " of Python's calls, which is short of where Python's recursion "
                         "limit stops it");
}

class Depth {
 public:
  Depth(int& depth, const std::string& where) : depth_(depth) {
    if (++depth_ > kMaxDepth) too_deep(where);
  }
  ~Depth() { --depth_; }
  Depth(const Depth&) = delete;
  Depth& operator=(const Depth&) = delete;

 private:
  int& depth_;
};

enum class TokKind { kHex, kBin, kDec, kChar, kSym, kOp };

struct Token {
  TokKind kind;
  U32 text;
};

bool is_hex(char32_t c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

std::vector<Token> tokens(U32View text_in, const std::string& where) {
  const U32 text = rstrip(text_in);
  const std::size_t n = text.size();
  std::vector<Token> out;
  std::size_t pos = 0;
  while (pos < n) {
    std::size_t p = pos;
    while (p < n && space(text[p])) ++p;
    std::size_t end = p;
    TokKind kind = TokKind::kOp;
    const char32_t c = p < n ? text[p] : 0;
    if (p < n && c == '$' && p + 1 < n && is_hex(text[p + 1])) {
      end = p + 1;
      while (end < n && is_hex(text[end])) ++end;
      kind = TokKind::kHex;
    } else if (p < n && c == '%' && p + 1 < n && (text[p + 1] == '0' || text[p + 1] == '1')) {
      end = p + 1;
      while (end < n && (text[end] == '0' || text[end] == '1')) ++end;
      kind = TokKind::kBin;
    } else if (p < n && pcpp::is_digit(c)) {
      while (end < n && pcpp::is_digit(text[end])) ++end;
      kind = TokKind::kDec;
    } else if (p < n && c == '\'' && p + 2 < n && text[p + 1] != '\n' && text[p + 2] == '\'') {
      end = p + 3;
      kind = TokKind::kChar;
    } else if (p < n && symbol_start(c)) {
      end = symbol_end(text, p);
      kind = TokKind::kSym;
    } else if (p + 1 < n && (c == '<' || c == '>') && text[p + 1] == c) {
      end = p + 2;
    } else if (p < n && c < 128 && std::string_view("-+*/&|^~<>()[],#").find(static_cast<char>(c)) !=
                                       std::string_view::npos) {
      end = p + 1;
    } else {
      error(where + ": can't read " + repr(U32View(text).substr(pos)));
    }
    out.push_back(Token{kind, text.substr(p, end - p)});
    pos = end;
  }
  return out;
}

struct Expr;
using ExprPtr = std::shared_ptr<const Expr>;

struct Num {
  Int value;
};

struct Sym {
  std::string name;
};

struct Unary {
  std::string op;
  ExprPtr operand;
};

struct Binary {
  std::string op;
  ExprPtr left;
  ExprPtr right;
};

struct Expr {
  std::variant<Num, Sym, Unary, Binary> node;
};

template <typename Node>
ExprPtr make(Node node) {
  return std::make_shared<const Expr>(Expr{std::move(node)});
}

const std::vector<std::vector<std::u32string_view>>& levels() {
  static const std::vector<std::vector<std::u32string_view>> v = {
      {U"|"}, {U"^"}, {U"&"}, {U"<<", U">>"}, {U"+", U"-"}, {U"*", U"/"}};
  return v;
}

class ExprParser {
 public:
  ExprParser(std::vector<Token> t, std::string where) : t_(std::move(t)), where_(std::move(where)) {}

  const Token* peek() const { return i_ < t_.size() ? &t_[i_] : nullptr; }

  const Token& take(const char* want = nullptr) {
    const Token* tok = peek();
    if (tok == nullptr || (want != nullptr && tok->text != to_u32(want))) {
      error(where_ + ": expected " + (want != nullptr ? want : "more") + ", found " +
            (tok != nullptr ? to_utf8(tok->text) : "the end of the line"));
    }
    ++i_;
    return *tok;
  }

  ExprPtr expr(std::size_t level = 0) {
    const Depth d(depth_, where_);
    if (level == levels().size()) return unary();
    ExprPtr left = expr(level + 1);
    while (const Token* tok = peek()) {
      const auto& ops = levels()[level];
      if (std::find(ops.begin(), ops.end(), U32View(tok->text)) == ops.end()) break;
      const std::string op = to_utf8(take().text);
      ExprPtr right = expr(level + 1);
      left = make(Binary{op, left, right});
    }
    return left;
  }

  ExprPtr unary() {
    const Depth d(depth_, where_);
    const Token tok = take();
    const U32& s = tok.text;
    if (tok.kind == TokKind::kOp && (s == U"-" || s == U"~" || s == U"<" || s == U">")) {
      ExprPtr operand = unary();
      return make(Unary{to_utf8(s), operand});
    }
    if (tok.kind == TokKind::kOp && (s == U"(" || s == U"[")) {
      ExprPtr inner = expr();
      take(s == U"(" ? ")" : "]");
      return inner;
    }
    switch (tok.kind) {
      case TokKind::kHex:
        return make(Num{py_int::parse(to_utf8(s.substr(1)), 16, where_)});
      case TokKind::kBin:
        return make(Num{py_int::parse(to_utf8(s.substr(1)), 2, where_)});
      case TokKind::kDec:
        return make(Num{py_int::parse(to_utf8(s), 10, where_)});
      case TokKind::kChar:
        return make(Num{static_cast<Int>(s[1])});
      case TokKind::kSym:
        return make(Sym{to_utf8(s)});
      case TokKind::kOp:
        break;
    }
    error(where_ + ": unexpected " + repr(s));
  }

 private:
  std::vector<Token> t_;
  std::size_t i_ = 0;
  std::string where_;
  int depth_ = 0;
};

ExprPtr parse_expr(U32View text, const std::string& where) {
  ExprParser p(tokens(text, where), where);
  ExprPtr e = p.expr();
  if (const Token* tok = p.peek()) {
    error(where + ": unexpected " + repr(tok->text) + " in " + repr(text));
  }
  return e;
}

enum class Byte { kWhole, kLow, kHigh };

struct Value {
  std::optional<std::string> symbol;
  Int addend = 0;
  Byte select = Byte::kWhole;
};

Int select_of(const Value& v) {
  if (v.select == Byte::kLow) return v.addend & Int(0xFF);
  if (v.select == Byte::kHigh) return py_int::rshift(v.addend, 8) & Int(0xFF);
  return v.addend;
}

Int const_op(const std::string& op, Int a, Int b, const std::string& where) {
  if (op == "+") return py_int::add(a, b, where);
  if (op == "-") return py_int::sub(a, b, where);
  if (op == "*") return py_int::mul(a, b, where);
  if (op == "/") {
    if (b == 0) error(where + ": division by zero");
    return py_int::floor_div(a, b, where);
  }
  if (op == "&") return a & b;
  if (op == "|") return a | b;
  if (op == "^") return a ^ b;
  if (op == "<<") return py_int::lshift(a, b, where);
  if (op == ">>") return py_int::rshift(a, b);
  error(where + ": unknown operator " + op);
}

struct Constants {
  const std::map<std::string, Int>* values = nullptr;
  const std::map<std::string, Int>* overlay = nullptr;

  const Int* operator()(const std::string& name) const {
    if (overlay != nullptr) {
      if (const auto it = overlay->find(name); it != overlay->end()) return &it->second;
    }
    const auto it = values->find(name);
    return it == values->end() ? nullptr : &it->second;
  }
};

class Evaluator {
 public:
  Evaluator(const Constants& constants, const std::string& where)
      : constants_(constants), where_(where) {}

  Value go(const Expr& e) {
    const Depth d(depth_, where_);
    if (const auto* n = std::get_if<Num>(&e.node)) return Value{.addend = n->value};
    if (const auto* s = std::get_if<Sym>(&e.node)) return Value{.symbol = s->name};
    if (const auto* u = std::get_if<Unary>(&e.node)) return unary(*u);
    return binary(std::get<Binary>(e.node));
  }

 private:
  Value unary(const Unary& u) {
    const Value v = go(*u.operand);
    if (u.op == "<" || u.op == ">") {
      if (v.select != Byte::kWhole) error(where_ + ": `" + u.op + "` of a byte");
      const Byte sel = u.op == "<" ? Byte::kLow : Byte::kHigh;
      if (!v.symbol) return Value{.addend = select_of(Value{.addend = v.addend, .select = sel})};
      return Value{.symbol = v.symbol, .addend = v.addend, .select = sel};
    }
    const Int n = num(v, "unary `" + u.op + "`");
    return Value{.addend = u.op == "-" ? py_int::neg(n, where_) : ~n};
  }

  Value binary(const Binary& bin) {
    const Value a = go(*bin.left);
    const Value b = go(*bin.right);
    const std::string& op = bin.op;
    if ((op == "+" || op == "-") && a.select == Byte::kWhole && b.select == Byte::kWhole) {
      if (!b.symbol) return Value{.symbol = a.symbol, .addend = const_op(op, a.addend, b.addend, where_)};
      if (op == "+" && !a.symbol) {
        return Value{.symbol = b.symbol, .addend = py_int::add(a.addend, b.addend, where_)};
      }
      if (op == "-" && a.symbol == b.symbol) {
        return Value{.addend = py_int::sub(a.addend, b.addend, where_)};
      }
    }
    const Int x = num(a, "`" + op + "`");
    const Int y = num(b, "`" + op + "`");
    return Value{.addend = const_op(op, x, y, where_)};
  }

  Int num(const Value& v, const std::string& what) {
    if (!v.symbol) return select_of(v);
    if (const Int* c = constants_(*v.symbol)) {
      return select_of(Value{.addend = py_int::add(*c, v.addend, where_), .select = v.select});
    }
    error(where_ + ": `" + *v.symbol + "` can't be used in " + what +
          ", since its address is only known when linking");
  }

  const Constants& constants_;
  const std::string& where_;
  int depth_ = 0;
};

Value evaluate(const Expr& e, const Constants& constants, const std::string& where) {
  Evaluator ev(constants, where);
  return ev.go(e);
}

enum class Syntax { kImp, kAcc, kImm, kInd, kIndx, kIndy, kX, kY, kPlain };

struct Instruction {
  Mode mode = Mode::kImp;
  ExprPtr expr;
};

struct DcBytes {
  std::vector<U32> items;
};

struct DcWords {
  int width = 2;
  std::vector<ExprPtr> values;
};

struct DsFill {
  Int count = 0;
  int fill = 0;
};

struct Stmt {
  int line = 0;
  std::string op;
  U32 operand;
  std::variant<Instruction, DcBytes, DcWords, DsFill> body;
  Int size = 0;
};

struct Item {
  std::string name;
  int line = 0;
  bool function = false;
  std::vector<Stmt> stmts;
  std::vector<std::pair<std::string, std::size_t>> labels;
};

bool has_label(const Item& it, const std::string& name) {
  return std::any_of(it.labels.begin(), it.labels.end(),
                     [&](const auto& l) { return l.first == name; });
}

bool balanced(U32View s) {
  int depth = 0;
  for (const char32_t c : s) {
    if (c == '(' || c == '[') ++depth;
    if (c == ')' || c == ']') --depth;
    if (depth < 0) return false;
  }
  return depth == 0;
}

std::size_t back_over_space(U32View s, std::size_t k) {
  while (k != U32::npos && space(s[k])) k = k == 0 ? U32::npos : k - 1;
  return k;
}

std::pair<Syntax, ExprPtr> split_operand(const std::string& op, U32View operand,
                                         const std::string& where) {
  const U32 s = strip(operand);
  const std::size_t n = s.size();
  if (s.empty()) return {is_shift(op) ? Syntax::kAcc : Syntax::kImp, nullptr};
  if (is_shift(op) && pcpp::upper(to_utf8(s)) == "A") return {Syntax::kAcc, nullptr};
  if (s[0] == '#') return {Syntax::kImm, parse_expr(U32View(s).substr(1), where)};
  if (s[0] == '(') {
    if (s[n - 1] == 'Y' || s[n - 1] == 'y') {
      std::size_t k = back_over_space(s, n >= 2 ? n - 2 : U32::npos);
      if (k != U32::npos && s[k] == ',') {
        k = back_over_space(s, k == 0 ? U32::npos : k - 1);
        if (k != U32::npos && k >= 1 && s[k] == ')') {
          const U32View group = U32View(s).substr(1, k - 1);
          if (balanced(group)) return {Syntax::kIndy, parse_expr(group, where)};
        }
      }
    }
    if (s[n - 1] == ')') {
      std::size_t k = back_over_space(s, n >= 2 ? n - 2 : U32::npos);
      if (k != U32::npos && (s[k] == 'X' || s[k] == 'x')) {
        k = back_over_space(s, k == 0 ? U32::npos : k - 1);
        if (k != U32::npos && k >= 1 && s[k] == ',') {
          const U32View group = U32View(s).substr(1, k - 1);
          if (balanced(group)) return {Syntax::kIndx, parse_expr(group, where)};
        }
      }
    }
    if (n >= 2 && s[n - 1] == ')' && balanced(U32View(s).substr(1, n - 2))) {
      return {Syntax::kInd, parse_expr(U32View(s).substr(1, n - 2), where)};
    }
  }
  const char32_t last = s[n - 1];
  if (last == 'X' || last == 'x' || last == 'Y' || last == 'y') {
    const std::size_t k = back_over_space(s, n >= 2 ? n - 2 : U32::npos);
    if (k != U32::npos && s[k] == ',') {
      const U32 group = rstrip(U32View(s).substr(0, k));
      return {last == 'X' || last == 'x' ? Syntax::kX : Syntax::kY, parse_expr(group, where)};
    }
  }
  return {Syntax::kPlain, parse_expr(s, where)};
}

std::vector<U32> split_list(U32View s) {
  std::vector<U32> out;
  U32 cur;
  int depth = 0;
  char32_t quote = 0;
  for (const char32_t c : s) {
    if (quote != 0) {
      cur += c;
      if (c == quote) quote = 0;
      continue;
    }
    if (c == '"') {
      quote = c;
    } else if (c == '(' || c == '[') {
      ++depth;
    } else if (c == ')' || c == ']') {
      --depth;
    } else if (c == ',' && depth == 0) {
      out.push_back(strip(cur));
      cur.clear();
      continue;
    }
    cur += c;
  }
  if (!strip(cur).empty()) out.push_back(strip(cur));
  return out;
}

std::string path_name(std::string_view filename) {
  const std::string s = argparse::path_str(std::string(filename));
#ifdef _WIN32
  const std::size_t sep = s.find_last_of('\\');
  std::string name = sep == std::string::npos ? s : s.substr(sep + 1);
  if (sep == std::string::npos && name.size() >= 2 && name[1] == ':') name = name.substr(2);
#else
  const std::size_t sep = s.find_last_of('/');
  std::string name = sep == std::string::npos ? s : s.substr(sep + 1);
#endif
  return name == "." ? "" : name;
}

class Assembler {
 public:
  Assembler(std::string_view text, std::string_view filename, const LinkSymbols& link_symbols,
            bool keep_source)
      : filename_(filename), lines_(pcpp::splitlines(text)), keep_source_(keep_source) {
    for (const auto& [name, value] : link_symbols) {
      if (value) {
        link_addresses_[name] = *value;
        link_outside_zp_.erase(name);
      } else {
        link_outside_zp_.insert(name);
        link_addresses_.erase(name);
      }
    }
  }

  elf::ElfFile run() {
    std::vector<std::string> meta_lines;
    bool in_meta = false;
    struct Equ {
      std::string name;
      U32 text;
      int line;
    };
    std::vector<Equ> equs;
    for (std::size_t k = 0; k < lines_.size(); ++k) {
      const int n = static_cast<int>(k) + 1;
      const U32 raw = to_u32(lines_[k]);
      const std::string stripped = to_utf8(strip(raw));
      if (stripped == kMetaBegin) in_meta = true;
      if (in_meta) {
        meta_lines.push_back(stripped);
        if (stripped == kMetaEnd) in_meta = false;
        continue;
      }
      const U32 line = rstrip(strip_comment(raw));
      if (line.empty()) continue;
      if (auto m = match_equ(line)) {
        equs.push_back(Equ{std::move(m->first), std::move(m->second), n});
        continue;
      }
      if (!space(line[0])) {
        const auto name = match_label(line);
        if (!name) error(where(n) + ": can't read the label " + repr(line));
        if ((*name)[0] == '.') {
          if (items_.empty()) error(where(n) + ": local label " + *name + " before any item");
          Item& current = items_.back();
          if (has_label(current, *name)) {
            error(where(n) + ": " + *name + " is defined twice in " + current.name);
          }
          current.labels.emplace_back(*name, current.stmts.size());
        } else {
          if (std::any_of(items_.begin(), items_.end(),
                          [&](const Item& it) { return it.name == *name; })) {
            error(where(n) + ": " + *name + " is defined twice");
          }
          items_.push_back(Item{.name = *name, .line = n});
        }
        continue;
      }
      const U32 body = strip(line);
      std::size_t word_end = 0;
      while (word_end < body.size() && !space(body[word_end])) ++word_end;
      const std::string word = to_utf8(U32View(body).substr(0, word_end));
      const std::string op = pcpp::upper(word);
      const U32 operand = strip(U32View(body).substr(word_end));
      if (op == "PROCESSOR") continue;
      if (items_.empty()) error(where(n) + ": " + op + " comes before any label");
      if (op == "SUBROUTINE") {
        items_.back().function = true;
        continue;
      }
      if (!is_directive(op) && !opcodes::is_mnemonic(op)) {
        error(where(n) + ": unknown instruction or directive " + word);
      }
      items_.back().stmts.push_back(Stmt{.line = n, .op = op, .operand = operand});
    }

    if (!meta_lines.empty()) {
      std::string text;
      for (std::size_t i = 0; i < meta_lines.size(); ++i) {
        if (i != 0) text += '\n';
        text += meta_lines[i];
      }
      meta_text_ = text + "\n";
      const passes::LinkMetadata meta = passes::parse_metadata(*meta_text_);
      for (const auto& z : meta.zp_objects) zp_objects_.insert(z.name);
      for (const auto& it : meta.items) item_meta_[it.name] = it;
    }

    for (const Equ& equ : equs) {
      const std::string w = where(equ.line);
      const Constants known{.values = &constants_, .overlay = &link_addresses_};
      const Value v = evaluate(*parse_expr(equ.text, w), known, w);
      Int value = v.addend;
      if (v.symbol) {
        const Int* k = known(*v.symbol);
        if (k == nullptr) error(w + ": " + equ.name + " has to be a number");
        value = select_of(Value{.addend = py_int::add(*k, v.addend, w), .select = v.select});
      }
      if (const auto it = constants_.find(equ.name); it != constants_.end() && it->second != value) {
        error(w + ": " + equ.name + " is given two values");
      }
      if (is_link_placed(equ.name, zp_objects_)) {
        if (!link_outside_zp_.contains(equ.name)) link_addresses_.emplace(equ.name, value);
      } else {
        constants_[equ.name] = value;
        if (const auto it = std::find_if(abs_symbols_.begin(), abs_symbols_.end(),
                                         [&](const auto& a) { return a.first == equ.name; });
            it != abs_symbols_.end()) {
          it->second = value;
        } else {
          abs_symbols_.emplace_back(equ.name, value);
        }
      }
    }
    return build();
  }

 private:
  std::string where(int line) const { return filename_ + ":" + std::to_string(line); }

  std::optional<Int> known_value(const Value& v, const std::string& w) const {
    if (!v.symbol) return v.addend;
    if (const auto it = constants_.find(*v.symbol); it != constants_.end()) {
      return py_int::add(it->second, v.addend, w);
    }
    if (const auto it = link_addresses_.find(*v.symbol); it != link_addresses_.end()) {
      return py_int::add(it->second, v.addend, w);
    }
    return std::nullopt;
  }

  bool in_zero_page(const Value& v, const std::string& w) const {
    if (v.select != Byte::kWhole) return true;
    if (const auto value = known_value(v, w)) return *value >= 0 && *value <= 0xFF;
    if (link_outside_zp_.contains(*v.symbol)) return false;
    return is_runtime_name(*v.symbol) && v.addend < 0x100;
  }

  Value value_of(const Expr& e, const std::string& w) const {
    return evaluate(e, Constants{.values = &constants_}, w);
  }

  void size_stmt(Stmt& st) const {
    const std::string w = where(st.line);
    if (st.op == "DC.B") {
      DcBytes bytes{split_list(st.operand)};
      st.size = 0;
      for (const auto& s : bytes.items) {
        st.size += !s.empty() && s[0] == '"' ? static_cast<Int>(s.size()) - 2 : Int(1);
      }
      st.body = std::move(bytes);
      return;
    }
    if (st.op == "DC.W" || st.op == "DC.L") {
      DcWords words{.width = st.op == "DC.W" ? 2 : 4};
      for (const auto& s : split_list(st.operand)) words.values.push_back(parse_expr(s, w));
      st.size = static_cast<Int>(words.values.size()) * words.width;
      st.body = std::move(words);
      return;
    }
    if (st.op == "DS.B") {
      std::vector<ExprPtr> args;
      for (const auto& s : split_list(st.operand)) args.push_back(parse_expr(s, w));
      if (args.empty() || args.size() > 2) error(w + ": DS.B takes a count");
      const Value count = value_of(*args[0], w);
      if (count.symbol || count.addend < 0) error(w + ": DS.B needs a constant count");
      int fill = 0;
      if (args.size() == 2) {
        const Value f = value_of(*args[1], w);
        if (f.symbol) error(w + ": DS.B needs a constant fill");
        fill = static_cast<int>(f.addend & Int(0xFF));
      }
      st.body = DsFill{.count = count.addend, .fill = fill};
      st.size = count.addend;
      return;
    }
    auto [syntax, expr] = split_operand(st.op, st.operand, w);
    const auto has = [&](Mode m) { return opcodes::opcode(st.op, m).has_value(); };
    Mode mode = Mode::kImp;
    switch (syntax) {
      case Syntax::kImp:
        mode = Mode::kImp;
        break;
      case Syntax::kAcc:
        mode = Mode::kAcc;
        break;
      case Syntax::kImm:
        mode = Mode::kImm;
        break;
      case Syntax::kInd:
        mode = Mode::kInd;
        break;
      case Syntax::kIndx:
        mode = Mode::kIndx;
        break;
      case Syntax::kIndy:
        mode = Mode::kIndy;
        break;
      case Syntax::kX:
      case Syntax::kY:
      case Syntax::kPlain:
        if (has(Mode::kRel)) {
          if (syntax != Syntax::kPlain) error(w + ": " + st.op + " takes a label");
          mode = Mode::kRel;
        } else {
          const Mode base = syntax == Syntax::kPlain ? Mode::kAbs
                            : syntax == Syntax::kX   ? Mode::kAbsx
                                                     : Mode::kAbsy;
          const Value v = value_of(*expr, w);
          const Mode zp = *opcodes::zp_form(base);
          mode = has(zp) && in_zero_page(v, w) ? zp : base;
        }
        break;
    }
    if (!has(mode)) {
      error(w + ": " + st.op + " has no " + std::string(opcodes::mode_name(mode)) +
            " addressing mode");
    }
    st.body = Instruction{.mode = mode, .expr = expr};
    st.size = opcodes::size(mode);
  }

  elf::SectionPtr section_for(const Item& it) const {
    const auto meta = item_meta_.find(it.name);
    const bool only_ds = std::all_of(it.stmts.begin(), it.stmts.end(),
                                     [](const Stmt& st) { return std::holds_alternative<DsFill>(st.body); });
    std::string name;
    bool zero = false;
    if (meta != item_meta_.end()) {
      name = meta->second.section;
      zero = meta->second.zero;
    } else if (it.function) {
      name = ".text";
    } else if (only_ds && !it.stmts.empty()) {
      name = ".bss";
      zero = true;
    } else {
      name = ".data";
    }
    if (zero && !only_ds) {
      error(where(it.line) + ": the link metadata says " + it.name +
            " is all zero bytes, and it has data");
    }
    auto sec = std::make_shared<elf::Section>();
    sec->name = name;
    sec->type = zero ? elf::SHT_NOBITS : elf::SHT_PROGBITS;
    if (it.function) {
      sec->flags = elf::SHF_ALLOC | elf::SHF_EXECINSTR;
    } else if (name == ".rodata" || name.rfind(".rodata.", 0) == 0) {
      sec->flags = elf::SHF_ALLOC;
    } else {
      sec->flags = elf::SHF_ALLOC | elf::SHF_WRITE;
    }
    return sec;
  }

  struct Placed {
    Item* item;
    elf::SectionPtr section;
    std::vector<std::pair<std::string, Int>> labels;
  };

  elf::ElfFile build() {
    out_.type = elf::ET_REL;
    out_.symbols.push_back(std::make_shared<elf::Symbol>(elf::Symbol{
        .name = path_name(filename_), .type = elf::STT_FILE, .shndx = elf::SHN_ABS}));
    std::vector<Placed> placed;
    for (Item& it : items_) {
      for (Stmt& st : it.stmts) size_stmt(st);
      std::vector<Int> offsets;
      Int pos = 0;
      for (const Stmt& st : it.stmts) {
        offsets.push_back(pos);
        pos = py_int::add(pos, st.size, where(st.line));
      }
      offsets.push_back(pos);
      Placed p{.item = &it};
      for (const auto& [name, index] : it.labels) p.labels.emplace_back(name, offsets[index]);
      p.section = section_for(it);
      const auto meta = item_meta_.find(it.name);
      const bool local = meta != item_meta_.end() && meta->second.local;
      auto sym = std::make_shared<elf::Symbol>(elf::Symbol{
          .name = it.name,
          .size = py_int::to_int64(pos, where(it.line)),
          .bind = local ? elf::STB_LOCAL : elf::STB_GLOBAL,
          .type = it.function ? elf::STT_FUNC : elf::STT_OBJECT,
          .section = p.section});
      out_.sections.push_back(p.section);
      out_.symbols.push_back(sym);
      global_syms_[it.name] = sym;
      placed.push_back(std::move(p));
    }
    for (const auto& [name, value] : abs_symbols_) {
      if (global_syms_.contains(name)) {
        error(filename_ + ": " + name + " is both a label and an EQU");
      }
      auto sym = std::make_shared<elf::Symbol>(
          elf::Symbol{.name = name,
                      .value = py_int::to_int64(value, filename_ + ": " + name),
                      .bind = elf::STB_GLOBAL,
                      .shndx = elf::SHN_ABS});
      out_.symbols.push_back(sym);
      global_syms_[name] = sym;
    }
    for (Placed& p : placed) emit_item(*p.item, p.section, p.labels);
    if (meta_text_) {
      auto sec = std::make_shared<elf::Section>();
      sec->name = std::string(elf::LINK_SECTION);
      sec->flags = 0;
      sec->data = *meta_text_;
      out_.sections.push_back(sec);
    }
    if (keep_source_) {
      auto sec = std::make_shared<elf::Section>();
      sec->name = std::string(elf::SOURCE_SECTION);
      sec->flags = 0;
      for (std::size_t i = 0; i < lines_.size(); ++i) {
        if (i != 0) sec->data += '\n';
        sec->data += lines_[i];
      }
      sec->data += '\n';
      out_.sections.push_back(sec);
    }
    return std::move(out_);
  }

  using Locals = std::map<std::string, elf::SymbolPtr>;

  elf::SymbolPtr symbol(const std::string& name, const Locals& local, const std::string& w) {
    if (name[0] == '.') {
      const auto it = local.find(name);
      if (it == local.end()) error(w + ": " + name + " isn't a label in this function");
      return it->second;
    }
    if (const auto it = global_syms_.find(name); it != global_syms_.end()) return it->second;
    auto sym = std::make_shared<elf::Symbol>(
        elf::Symbol{.name = name, .bind = elf::STB_GLOBAL, .shndx = elf::SHN_UNDEF});
    out_.symbols.push_back(sym);
    global_syms_[name] = sym;
    return sym;
  }

  void emit_item(const Item& it, const elf::SectionPtr& sec,
                 const std::vector<std::pair<std::string, Int>>& label_list) {
    Locals local;
    std::map<std::string, Int> labels;
    // MSVC's `stable_sort` can't take the 16-byte alignment of `Int` in
    // its buffer, so the indices are sorted.
    std::vector<std::size_t> order(label_list.size());
    for (std::size_t i = 0; i < order.size(); ++i) order[i] = i;
    std::stable_sort(order.begin(), order.end(), [&](std::size_t a, std::size_t b) {
      return label_list[a].second < label_list[b].second;
    });
    for (const std::size_t i : order) {
      const auto& [name, off] = label_list[i];
      auto sym = std::make_shared<elf::Symbol>(elf::Symbol{
          .name = name, .value = py_int::to_int64(off, where(it.line)), .section = sec});
      local[name] = sym;
      labels[name] = off;
      out_.symbols.push_back(sym);
    }
    std::string data;
    bool in_data = false;
    for (const Stmt& st : it.stmts) {
      const std::string w = where(st.line);
      const std::size_t pc = data.size();
      const bool is_data = !std::holds_alternative<Instruction>(st.body);
      if (it.function && is_data != in_data) {
        out_.symbols.push_back(std::make_shared<elf::Symbol>(
            elf::Symbol{.name = is_data ? "$d" : "$c",
                        .value = static_cast<std::int64_t>(pc),
                        .section = sec}));
        in_data = is_data;
      }
      if (const auto* bytes = std::get_if<DcBytes>(&st.body)) {
        for (const U32& s : bytes->items) {
          if (!s.empty() && s[0] == '"') {
            if (s.size() < 2 || s.back() != '"') error(w + ": bad string " + to_utf8(s));
            for (std::size_t i = 1; i + 1 < s.size(); ++i) {
              if (s[i] > 0xFF) {
                error(w + ": " + to_utf8(s) +
                      " holds a character outside Latin-1, which DC.B can't store in a byte");
              }
            }
            for (std::size_t i = 1; i + 1 < s.size(); ++i) data += static_cast<char>(s[i]);
            continue;
          }
          const Value v = value_of(*parse_expr(s, w), w);
          field(*sec, data, v, 1, local, w, false);
        }
      } else if (const auto* words = std::get_if<DcWords>(&st.body)) {
        for (const auto& e : words->values) {
          const Value v = value_of(*e, w);
          field(*sec, data, v, words->width, local, w, false);
        }
      } else if (const auto* space = std::get_if<DsFill>(&st.body)) {
        if (sec->type == elf::SHT_NOBITS) {
          sec->nobits_size = py_int::to_int64(py_int::add(sec->nobits_size, space->count, w), w);
          continue;
        }
        if (space->count > kMaxFill) {
          throw CompileError("OverflowError",
                             w + ": DS.B of " + str(space->count) +
                                 " bytes, where the C++ assembler fills at most 16 MB");
        }
        data.append(static_cast<std::size_t>(space->count), static_cast<char>(space->fill));
      } else {
        emit_instr(st.op, std::get<Instruction>(st.body), *sec, data, labels, local, w);
      }
      const Int took = static_cast<Int>(data.size() - pc);
      if (sec->type != elf::SHT_NOBITS && took != st.size) {
        error(w + ": " + st.op + " took " + str(took) + " bytes where " + str(st.size) +
              " were counted");
      }
    }
    if (sec->type != elf::SHT_NOBITS) sec->data = std::move(data);
  }

  void emit_instr(const std::string& op, const Instruction& in, elf::Section& sec,
                  std::string& data, const std::map<std::string, Int>& labels,
                  const Locals& local, const std::string& w) {
    data += static_cast<char>(*opcodes::opcode(op, in.mode));
    if (in.mode == Mode::kImp || in.mode == Mode::kAcc) return;
    const Value v = value_of(*in.expr, w);
    if (in.mode == Mode::kRel) {
      if (v.select != Byte::kWhole) error(w + ": a branch takes a label");
      if (v.symbol) {
        if (const auto it = labels.find(*v.symbol); it != labels.end()) {
          const Int disp = py_int::sub(py_int::add(it->second, v.addend, w),
                                       static_cast<Int>(data.size()) + 1, w);
          if (disp < -128 || disp > 127) {
            error(w + ": branch to " + *v.symbol + " is " + str(disp) + " bytes away, out of reach");
          }
          data += static_cast<char>(static_cast<int>(disp & Int(0xFF)));
          return;
        }
      }
      if (!v.symbol) error(w + ": a branch takes a label");
      const elf::SymbolPtr sym = symbol(*v.symbol, local, w);
      sec.relocs.push_back(elf::Reloc{.offset = static_cast<std::int64_t>(data.size()),
                                      .type = elf::R_MOS_PCREL_8,
                                      .symbol = sym,
                                      .addend = py_int::to_int64(py_int::sub(v.addend, 1, w), w)});
      data += '\0';
      return;
    }
    if (in.mode == Mode::kImm) {
      field(sec, data, v, 1, local, w, false);
      return;
    }
    const int width = opcodes::size(in.mode) - 1;
    if (width == 1 && v.select == Byte::kWhole && !v.symbol && (v.addend < 0 || v.addend > 0xFF)) {
      error(w + ": $" + pyrepr::hex(v.addend, 0) + " isn't in zero page");
    }
    if (width == 1 && v.symbol && !in_zero_page(v, w)) {
      error(w + ": " + op + " (" + std::string(opcodes::mode_name(in.mode)) +
            ") needs a zero-page address, and " + *v.symbol + " isn't known to be in zero page");
    }
    field(sec, data, v, width, local, w, width == 1);
  }

  void field(elf::Section& sec, std::string& data, const Value& v, int width, const Locals& local,
             const std::string& w, bool zero_page) {
    if (!v.symbol) {
      const Int value = select_of(v);
      const Int lo = -(Int(1) << (8 * width - 1));
      const Int hi = (Int(1) << (8 * width)) - 1;
      if (value < lo || value > hi) {
        error(w + ": " + str(value) + " doesn't fit in " + std::to_string(width) + " byte(s)");
      }
      for (int i = 0; i < width; ++i) {
        data += static_cast<char>(static_cast<int>(py_int::rshift(value, 8 * i) & Int(0xFF)));
      }
      return;
    }
    const elf::SymbolPtr sym = symbol(*v.symbol, local, w);
    std::uint8_t rtype = elf::R_MOS_NONE;
    if (v.select != Byte::kWhole) {
      if (width != 1) {
        error(w + ": `<` or `>` gives one byte, in a " + std::to_string(width) + "-byte field");
      }
      rtype = v.select == Byte::kLow ? elf::R_MOS_ADDR16_LO : elf::R_MOS_ADDR16_HI;
    } else if (width == 1) {
      rtype = zero_page ? elf::R_MOS_ADDR8 : elf::R_MOS_IMM8;
      if (zero_page && !sym->defined()) sym->other |= elf::STO_MOS_ZEROPAGE;
    } else if (width == 2) {
      rtype = elf::R_MOS_ADDR16;
    } else {
      error(w + ": a " + std::to_string(width) + "-byte field can't hold the address of " +
            *v.symbol);
    }
    sec.relocs.push_back(elf::Reloc{.offset = static_cast<std::int64_t>(data.size()),
                                    .type = rtype,
                                    .symbol = sym,
                                    .addend = py_int::to_int64(v.addend, w)});
    data.append(static_cast<std::size_t>(width), '\0');
  }

  static constexpr std::int64_t kMaxFill = 1 << 24;

  std::string filename_;
  std::vector<std::string> lines_;
  bool keep_source_;
  std::map<std::string, Int> link_addresses_;
  std::set<std::string> link_outside_zp_;
  std::vector<Item> items_;
  std::optional<std::string> meta_text_;
  std::set<std::string, std::less<>> zp_objects_;
  std::map<std::string, passes::ItemMeta> item_meta_;
  std::map<std::string, Int> constants_;
  std::vector<std::pair<std::string, Int>> abs_symbols_;
  elf::ElfFile out_;
  std::map<std::string, elf::SymbolPtr> global_syms_;
};

}  // namespace

bool is_link_placed(std::string_view name, const std::set<std::string, std::less<>>& zp_objects) {
  return is_runtime_name(name) || name.rfind("__zpabi_", 0) == 0 ||
         name.rfind("__local_", 0) == 0 || zp_objects.find(name) != zp_objects.end();
}

elf::ElfFile assemble(std::string_view text, std::string_view filename,
                      const LinkSymbols* link_symbols, bool keep_source) {
  static const LinkSymbols kNone;
  Assembler a(text, filename, link_symbols != nullptr ? *link_symbols : kNone, keep_source);
  return a.run();
}

}  // namespace c6502::binutils::assembler
