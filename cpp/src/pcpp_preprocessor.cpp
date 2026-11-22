#include "pcpp_preprocessor.h"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <sstream>

#include "errors.h"
#include "pyrepr.h"

namespace c6502::pcpp {

namespace {

// pcpp's `t_WS`, the tuple of the types of whitespace: CPP_WS, which is
// both its space and its newline type, and CPP_LINECONT.
bool is_ws(const TokenPtr& t) { return t->type == Type::CPP_WS || t->type == Type::CPP_LINECONT; }

bool is_comment(const TokenPtr& t) {
  return t->type == Type::CPP_COMMENT1 || t->type == Type::CPP_COMMENT2;
}

// Returns Python's `tok.type in name` where pcpp tests a type against
// one of its type strings (`self.t_SPACE`, `self.t_ID`). The right side
// is a string there, so Python tests for a substring. A token's type is
// a type name, which is in `name` only when it is `name`, or the
// character of a `Char` token.
bool type_in_string(const TokenPtr& t, const char* name) {
  if (t->type == Type::Char) {
    return !t->chr.empty() && std::string_view(name).find(t->chr) != std::string_view::npos;
  }
  if (t->type == Type::None) {
    throw CompileError("TypeError", "'in <string>' requires string as left operand, not NoneType");
  }
  return std::string_view(name) == type_name(t->type);
}

[[noreturn]] void index_error() { throw CompileError("IndexError", "list index out of range"); }

// Returns `v[i]` with Python's indexing: a negative `i` counts from the
// end, and an index out of range raises IndexError.
template <class T>
T& at(std::vector<T>& v, std::ptrdiff_t i) {
  const auto n = static_cast<std::ptrdiff_t>(v.size());
  if (i < 0) i += n;
  if (i < 0 || i >= n) index_error();
  return v[static_cast<std::size_t>(i)];
}

template <class T>
const T& at(const std::vector<T>& v, std::ptrdiff_t i) {
  const auto n = static_cast<std::ptrdiff_t>(v.size());
  if (i < 0) i += n;
  if (i < 0 || i >= n) index_error();
  return v[static_cast<std::size_t>(i)];
}

// Returns `v[from:to]`, with Python's clamping of the bounds.
Tokens slice(const Tokens& v, std::size_t from, std::size_t to = static_cast<std::size_t>(-1)) {
  to = std::min(to, v.size());
  if (from >= to) return {};
  return Tokens(v.begin() + static_cast<std::ptrdiff_t>(from),
                v.begin() + static_cast<std::ptrdiff_t>(to));
}

// Does `v[from:to] = repl`.
void splice(Tokens& v, std::size_t from, std::size_t to, const Tokens& repl) {
  to = std::min(to, v.size());
  from = std::min(from, to);
  v.erase(v.begin() + static_cast<std::ptrdiff_t>(from), v.begin() + static_cast<std::ptrdiff_t>(to));
  v.insert(v.begin() + static_cast<std::ptrdiff_t>(from), repl.begin(), repl.end());
}

std::string join_values(const Tokens& toks) {
  std::string out;
  for (const auto& t : toks) out += t->value;
  return out;
}

bool contains(const std::vector<std::string>& v, const std::string& s) {
  return std::find(v.begin(), v.end(), s) != v.end();
}

bool starts_with(const std::string& s, const std::string& prefix) {
  return s.compare(0, prefix.size(), prefix) == 0;
}

std::string str_or_none(const std::optional<std::string>& s) { return s ? *s : "None"; }

// Returns Python's `%d` of a bool.
std::string bit(bool b) { return b ? "1" : "0"; }

// Returns `repr()` of a token, which PLY's LexToken gives as
// `LexToken(type,'value',lineno,lexpos)`.
std::string lextoken_repr(const Token& t) {
  return "LexToken(" + type_name(t) + "," + py_repr(t.value) + "," + std::to_string(t.lineno) +
         "," + std::to_string(t.lexpos) + ")";
}

// Returns `repr()` of a macro, which pcpp's `Macro.__repr__` gives as
// `name(arglist)=value`, with Python's repr of the list of argument
// names (or None) and of the list of tokens.
std::string macro_repr(const Macro& m) {
  std::string args = "None";
  if (m.arglist) {
    args = "[";
    for (std::size_t i = 0; i < m.arglist->size(); ++i) {
      if (i > 0) args += ", ";
      args += py_repr((*m.arglist)[i]);
    }
    args += "]";
  }
  std::string value = "[";
  for (std::size_t i = 0; i < m.value.size(); ++i) {
    if (i > 0) value += ", ";
    value += lextoken_repr(*m.value[i]);
  }
  return m.name + "(" + args + ")=" + value + "]";
}

// Replaces the trigraph sequences, as pcpp's `trigraph` does with the
// pattern `\?\?[=/\'\(\)\!<>\-]`.
std::string trigraph(const std::string& input) {
  std::string out;
  out.reserve(input.size());
  for (std::size_t i = 0; i < input.size();) {
    if (input[i] == '?' && i + 2 < input.size() && input[i + 1] == '?') {
      const char* rep = nullptr;
      switch (input[i + 2]) {
        case '=': rep = "#"; break;
        case '/': rep = "\\"; break;
        case '\'': rep = "^"; break;
        case '(': rep = "["; break;
        case ')': rep = "]"; break;
        case '!': rep = "|"; break;
        case '<': rep = "{"; break;
        case '>': rep = "}"; break;
        case '-': rep = "~"; break;
        default: break;
      }
      if (rep != nullptr) {
        out += rep;
        i += 3;
        continue;
      }
    }
    out += input[i++];
  }
  return out;
}

double process_clock() {
  return static_cast<double>(std::clock()) / CLOCKS_PER_SEC;
}

// The consumer of `write`, which takes the tokens of `parsegen` as
// `Preprocessor.write` takes them from `token()` and writes the text.
class Writer {
 public:
  explicit Writer(Preprocessor& pp) : pp_(pp) {}

  void push(const TokenPtr& tok) {
    pp_.on_token(tok);
    toks_.push_back(tok);
    if (!tok->value.empty() && tok->value[0] == '\n') {
      line();
    } else if (!is_ws(tok)) {
      all_ws_ = false;
    }
  }

  std::string finish() {
    if (!toks_.empty()) line();
    return out_;
  }

 private:
  void line() {
    Tokens toks = std::move(toks_);
    toks_.clear();
    const bool all_ws = all_ws_;
    all_ws_ = true;
    if (all_ws) {
      // Remove preceding whitespace so it becomes just a LF.
      if (toks.size() > 1) toks = {toks.back()};
      blanklines_ += static_cast<int>(std::count(toks[0]->value.begin(), toks[0]->value.end(), '\n'));
      return;
    }
    // Filter out line continuations, collapsing before and after if
    // needs be.
    for (std::ptrdiff_t n = static_cast<std::ptrdiff_t>(toks.size()) - 1; n >= 0; --n) {
      if (type_in_string(toks[static_cast<std::size_t>(n)], "CPP_LINECONT")) {
        const auto len = static_cast<std::ptrdiff_t>(toks.size());
        if (n > 0 && n < len - 2 && is_ws(toks[static_cast<std::size_t>(n - 1)]) &&
            is_ws(toks[static_cast<std::size_t>(n + 1)])) {
          auto& prev = toks[static_cast<std::size_t>(n - 1)];
          if (!type_in_string(prev, "CPP_LINECONT")) {
            if (prev->value.empty()) index_error();
            prev->value = prev->value.substr(0, 1);
            toks.erase(toks.begin() + n, toks.begin() + n + 2);
          }
        } else {
          toks.erase(toks.begin() + n);
        }
      }
    }
    // The line in toks is not all whitespace.
    bool emitlinedirective = blanklines_ > 6 && pp_.line_directive.has_value();
    if (!lastsource_) {
      if (toks[0]->source.has_value()) emitlinedirective = true;
      lastsource_ = toks[0]->source;
    } else if (lastsource_ != toks[0]->source) {
      emitlinedirective = true;
      lastsource_ = toks[0]->source;
    }
    // Replace consecutive whitespace in output with a single space
    // except at any indent.
    std::optional<std::ptrdiff_t> first_ws;
    for (std::ptrdiff_t n = static_cast<std::ptrdiff_t>(toks.size()) - 1; n >= 0; --n) {
      const TokenPtr tok = toks[static_cast<std::size_t>(n)];
      if (!first_ws) {
        if (type_in_string(tok, "CPP_WS") || tok->value.empty()) first_ws = n;
      } else if (!type_in_string(tok, "CPP_WS") && !tok->value.empty()) {
        const std::ptrdiff_t m = n + 1;
        while (m != *first_ws) {
          toks.erase(toks.begin() + m);
          --*first_ws;
        }
        first_ws.reset();
        if (pp_.compress > 0) {
          auto& t = toks[static_cast<std::size_t>(m)];
          if (!t->value.empty() && t->value[0] == ' ') t->value = " ";
        }
      }
    }
    if (!(pp_.compress > 1) && !emitlinedirective) {
      int newlinesneeded = toks[0]->lineno - lastlineno_ - 1;
      if (newlinesneeded > 6 && pp_.line_directive) {
        emitlinedirective = true;
      } else {
        while (newlinesneeded > 0) {
          out_ += '\n';
          --newlinesneeded;
        }
      }
    }
    lastlineno_ = toks[0]->lineno;
    if (emitlinedirective && pp_.line_directive) {
      out_ += *pp_.line_directive + " " + std::to_string(lastlineno_) +
              (lastsource_ ? " \"" + *lastsource_ + "\"" : std::string()) + "\n";
    }
    for (const auto& tok : toks) {
      if (tok->type == Type::CPP_COMMENT1) {
        lastlineno_ += static_cast<int>(std::count(tok->value.begin(), tok->value.end(), '\n'));
      }
    }
    blanklines_ = 0;
    for (const auto& tok : toks) out_ += tok->value;
  }

  Preprocessor& pp_;
  Tokens toks_;
  bool all_ws_ = true;
  std::string out_;
  int lastlineno_ = 0;
  // `lastsource`, which is None at first and whenever a line's first
  // token has the source None.
  std::optional<std::string> lastsource_;
  int blanklines_ = 0;
};

}  // namespace

Host native_host() {
  Host host;
  host.paths.flavor = native_flavor();
  const auto cwd = std::filesystem::current_path().u8string();
  host.paths.cwd.assign(cwd.begin(), cwd.end());
#ifdef _WIN32
  host.encoding = "cp1252";
#else
  host.encoding = "utf-8";
#endif
  host.read_file = [](const std::string& p) -> std::optional<std::string> {
    std::error_code ec;
    const std::filesystem::path fp(std::u8string(p.begin(), p.end()));
    if (std::filesystem::is_directory(fp, ec)) return std::nullopt;
    std::ifstream in(fp, std::ios::binary);
    if (!in) return std::nullopt;
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
  };
  host.isdir = [](const std::string& p) {
    std::error_code ec;
    return std::filesystem::is_directory(std::filesystem::path(std::u8string(p.begin(), p.end())), ec);
  };
  host.localtime = [] {
    const std::time_t now = std::time(nullptr);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &now);
#else
    localtime_r(&now, &tm);
#endif
    return tm;
  };
  host.write_debug_log = [](const std::string& bytes) {
    std::ofstream("pcpp_debug.log", std::ios::binary | std::ios::trunc) << bytes;
  };
  return host;
}

Preprocessor::Preprocessor(Host h) : host(std::move(h)) {
  // `re.escape(os.path.abspath('') + os.sep) + '(.*)'`, rewritten to
  // `\1`.
  rewrite_paths.push_back({host.paths.abspath("") + host.paths.sep(), ""});
  // `__lexprobe` reads a token of each of its probes from the lexer,
  // and the newline and line continuation it reads advance the lexer's
  // line number to 3, which the tokens of every later `tokenize` start
  // from.
  for (const char* probe : {"identifier", "12345", "'a'", "\"filename\"", "  ", "\n",
                            "\\     \n", "##", "?", ":", "/* comment */", "// comment", "<",
                            ">", "#", "##", "\\", "(", ")", ",", "."}) {
    lexer.input(probe);
    lexer.token();
  }
  const std::tm tm = host.localtime ? host.localtime() : std::tm{};
  char date[32];
  char time[32];
  std::strftime(date, sizeof date, "%b %d %Y", &tm);
  std::strftime(time, sizeof time, "%H:%M:%S", &tm);
  define(std::string("__DATE__ \"") + date + "\"");
  define(std::string("__TIME__ \"") + time + "\"");
  define("__PCPP__ 1");
}

// ---------------------------------------------------------------------
// The hooks of `PreprocessorHooks`.

void Preprocessor::on_error(const std::optional<std::string>& file, int line,
                            const std::string& msg) {
  stderr_text += str_or_none(file) + ":" + std::to_string(line) + " error: " + msg + "\n";
  ++return_code;
}

std::optional<std::string> Preprocessor::on_file_open(bool, const std::string& includepath) {
  const auto bytes = host.read_file(includepath);
  if (!bytes) return std::nullopt;
  std::string text = decode_text_file(*bytes, assume_encoding ? *assume_encoding : host.encoding);
  // A file that starts with a BOM loses it.
  if (starts_with(text, "\xEF\xBB\xBF")) text.erase(0, 3);
  return text;
}

std::string Preprocessor::on_include_not_found(bool is_malformed, bool, const std::string&,
                                               const std::string& includepath) {
  if (is_malformed) {
    on_error(lastdirective->source, lastdirective->lineno,
             "Malformed #include statement: " + includepath);
  } else {
    on_error(lastdirective->source, lastdirective->lineno,
             "Include file '" + includepath + "' not found");
  }
  throw OutputDirective{Action::IgnoreAndPassThrough};
}

std::optional<bool> Preprocessor::on_unknown_macro_in_defined_expr(const TokenPtr&) {
  return false;
}

std::optional<std::int64_t> Preprocessor::on_unknown_macro_in_expr(const std::string&) {
  return 0;
}

bool Preprocessor::on_unknown_macro_function_in_expr(const std::string&) { return true; }

std::optional<bool> Preprocessor::on_directive_handle(const TokenPtr& directive, const Tokens&,
                                                      bool,
                                                      const Tokens&) {
  lastdirective = directive;
  return true;
}

std::optional<bool> Preprocessor::on_directive_unknown(const TokenPtr& directive, const Tokens& toks,
                                                       bool, const Tokens&) {
  if (directive->value == "error") {
    stderr_text += str_or_none(directive->source) + ":" + std::to_string(directive->lineno) +
                   " error: " + join_values(toks) + "\n";
    ++return_code;
    return true;
  }
  if (directive->value == "warning") {
    stderr_text += str_or_none(directive->source) + ":" + std::to_string(directive->lineno) +
                   " warning: " + join_values(toks) + "\n";
    return true;
  }
  return std::nullopt;
}

void Preprocessor::on_potential_include_guard(const std::optional<std::string>&) {}

bool Preprocessor::on_comment(const TokenPtr&) { return false; }

EvalResult Preprocessor::call_evaluator(const Tokens& tokens, const FunctionHook& functions,
                                        const IdentifierHook& identifiers) {
  return evaluate(tokens, functions, identifiers);
}

// ---------------------------------------------------------------------
// The utilities.

Tokens Preprocessor::tokenize(const std::string& text) {
  Tokens tokens;
  lexer.input(text);
  while (auto tok = lexer.token()) {
    tok->source = "";
    tokens.push_back(tok);
  }
  return tokens;
}

void Preprocessor::add_path(const std::string& p) {
  path.push_back(p);
  // A search path relative to the current directory, or on its drive,
  // gets a rewrite that makes the paths of its files relative. Python
  // builds the replacement with `os.path.join(relpath, '\\1')`, which on
  // Windows is `\1` alone, since `\1` is a rooted path there, so the
  // rewrite only removes the prefix.
  const auto rel = host.paths.relpath(p);
  if (rel) {
    std::string replacement;
    if (host.paths.flavor == PathFlavor::Posix) replacement = host.paths.join(*rel, "");
    rewrite_paths.push_back({host.paths.abspath(p) + host.paths.sep(), replacement});
  }
}

void Preprocessor::debug_print(const std::string& line) {
  const bool crlf = host.paths.flavor == PathFlavor::Nt;
  debug_bytes += encode_text_file(line, host.encoding, crlf);
  debug_bytes += crlf ? "\r\n" : "\n";
}

bool Preprocessor::include_once_has(const std::optional<std::string>& key) const {
  for (const auto& [k, v] : include_once) {
    if (k == key) return true;
  }
  return false;
}

void Preprocessor::include_once_set(const std::optional<std::string>& key,
                                    const std::optional<std::string>& value) {
  for (auto& [k, v] : include_once) {
    if (k == key) {
      v = value;
      return;
    }
  }
  include_once.emplace_back(key, value);
}

std::vector<std::shared_ptr<Tokens>> Preprocessor::group_lines(
    const std::string& input, const std::optional<std::string>& abssource) {
  Lexer lex = lexer;
  std::string joined;
  bool first = true;
  for (const auto& line : splitlines(input)) {
    if (!first) joined += '\n';
    first = false;
    joined += rstrip(line);
  }
  lex.input(joined);
  lex.lineno = 1;
  std::vector<std::shared_ptr<Tokens>> lines;
  auto current = std::make_shared<Tokens>();
  while (auto tok = lex.token()) {
    tok->source = abssource;
    current->push_back(tok);
    if (is_ws(tok) && tok->value == "\n") {
      lines.push_back(current);
      current = std::make_shared<Tokens>();
    }
  }
  if (!current->empty()) {
    auto nltok = copy_token(current->back());
    nltok->type = Type::CPP_WS;
    nltok->chr.clear();
    nltok->value = "\n";
    current->push_back(nltok);
    lines.push_back(current);
  }
  return lines;
}

Tokens& Preprocessor::tokenstrip(Tokens& tokens) {
  std::size_t i = 0;
  while (i < tokens.size() && is_ws(tokens[i])) ++i;
  tokens.erase(tokens.begin(), tokens.begin() + static_cast<std::ptrdiff_t>(i));
  std::size_t end = tokens.size();
  while (end > 0 && is_ws(tokens[end - 1])) --end;
  tokens.erase(tokens.begin() + static_cast<std::ptrdiff_t>(end), tokens.end());
  return tokens;
}

Preprocessor::Collected Preprocessor::collect_args(const Tokens& tokenlist, bool ignore_errors) {
  Collected out{0, {}, {}};
  Tokens current_arg;
  int nesting = 1;
  const std::size_t tokenlen = tokenlist.size();
  std::size_t i = 0;
  while (i < tokenlen && is_ws(tokenlist[i])) ++i;
  if (i < tokenlen && tokenlist[i]->value == "(") {
    out.positions.push_back(i + 1);
  } else {
    if (!ignore_errors) {
      const auto& t0 = at(tokenlist, 0);
      on_error(t0->source, t0->lineno, "Missing '(' in macro arguments");
    }
    return {0, {}, {}};
  }
  ++i;
  while (i < tokenlen) {
    const TokenPtr& t = tokenlist[i];
    if (t->value == "(") {
      current_arg.push_back(t);
      ++nesting;
    } else if (t->value == ")") {
      --nesting;
      if (nesting == 0) {
        out.args.push_back(tokenstrip(current_arg));
        out.positions.push_back(i);
        out.count = i + 1;
        return out;
      }
      current_arg.push_back(t);
    } else if (t->value == "," && nesting == 1) {
      out.args.push_back(tokenstrip(current_arg));
      out.positions.push_back(i + 1);
      current_arg.clear();
    } else {
      current_arg.push_back(t);
    }
    ++i;
  }
  // Missing end argument.
  if (!ignore_errors) {
    const auto& tl = at(tokenlist, -1);
    on_error(tl->source, tl->lineno, "Missing ')' in macro arguments");
  }
  return {0, {}, {}};
}

void Preprocessor::macro_prescan(Macro& macro) {
  macro.patch.clear();
  macro.str_patch.clear();
  macro.var_comma_patch.clear();
  Tokens& value = macro.value;
  const auto& arglist = *macro.arglist;
  std::size_t i = 0;
  while (i < value.size()) {
    if (value[i]->type == Type::CPP_ID && contains(arglist, value[i]->value)) {
      const std::size_t argnum = static_cast<std::size_t>(
          std::find(arglist.begin(), arglist.end(), value[i]->value) - arglist.begin());
      // Conversion of argument to a string.
      std::ptrdiff_t j = static_cast<std::ptrdiff_t>(i) - 1;
      while (j >= 0 && is_ws(value[static_cast<std::size_t>(j)])) --j;
      if (j >= 0 && value[static_cast<std::size_t>(j)]->value == "#") {
        value[i] = copy_token(value[i]);
        value[i]->type = Type::CPP_STRING;
        while (static_cast<std::ptrdiff_t>(i) > j) {
          value.erase(value.begin() + j);
          --i;
        }
        macro.str_patch.emplace_back(argnum, i);
        continue;
      }
      // Concatenation.
      if ((i > 0 && value[i - 1]->value == "##") ||
          (i + 1 < value.size() && value[i + 1]->value == "##")) {
        macro.patch.push_back({'t', argnum, i});
        ++i;
        continue;
      }
      // Standard expansion.
      macro.patch.push_back({'e', argnum, i});
    } else if (value[i]->value == "##") {
      if (macro.variadic && i > 0 && value[i - 1]->value == "," && i + 1 < value.size() &&
          value[i + 1]->type == Type::CPP_ID && value[i + 1]->value == macro.vararg) {
        macro.var_comma_patch.push_back(i - 1);
      }
    }
    ++i;
  }
  std::stable_sort(macro.patch.begin(), macro.patch.end(),
                   [](const Macro::Patch& a, const Macro::Patch& b) { return a.pos > b.pos; });
}

Tokens Preprocessor::macro_expand_args(const Macro& macro, std::vector<Tokens>& args) {
  Tokens rep;
  for (const auto& x : macro.value) rep.push_back(copy_token(x));

  // Make string expansion patches. These do not alter the length of the
  // replacement sequence.
  std::map<std::size_t, std::string> str_expansion;
  for (const auto& [argnum, i] : macro.str_patch) {
    if (!str_expansion.count(argnum)) {
      // Strip all non-space whitespace before stringization. The list
      // is a copy, and its tokens are the argument's own, which this
      // changes.
      Tokens tokens = at(args, static_cast<std::ptrdiff_t>(argnum));
      for (auto& t : tokens) {
        if (is_ws(t) && t->type != Type::CPP_LINECONT) t->value = " ";
      }
      // Collapse all multiple whitespace too.
      std::size_t j = 0;
      while (j + 1 < tokens.size()) {
        if (is_ws(tokens[j]) && is_ws(tokens[j + 1])) {
          tokens.erase(tokens.begin() + static_cast<std::ptrdiff_t>(j + 1));
        } else {
          ++j;
        }
      }
      std::string s;
      for (const char c : join_values(tokens)) {
        if (c == '\\') {
          s += "\\\\";
        } else if (c == '"') {
          s += "\\\"";
        } else {
          s += c;
        }
      }
      str_expansion[argnum] = "\"" + s + "\"";
    }
    auto& slot = at(rep, static_cast<std::ptrdiff_t>(i));
    slot = copy_token(slot);
    slot->value = str_expansion[argnum];
  }

  // Make the variadic macro comma patch. If the variadic macro argument
  // is empty, we get rid of the comma.
  bool comma_patch = false;
  if (macro.variadic && at(args, -1).empty()) {
    for (const std::size_t i : macro.var_comma_patch) {
      at(rep, static_cast<std::ptrdiff_t>(i)) = nullptr;
      comma_patch = true;
    }
  }

  // Make all other patches, from the last position to the first, since
  // a replacement changes the length of the sequence after its point.
  std::map<std::size_t, Tokens> expanded;
  for (const auto& p : macro.patch) {
    if (p.type == 't') {
      // Concatenation. The argument is left unexpanded.
      splice(rep, p.pos, p.pos + 1, at(args, static_cast<std::ptrdiff_t>(p.argnum)));
    } else {
      // Normal expansion. The argument is macro expanded first.
      if (!expanded.count(p.argnum)) {
        Tokens copy = at(args, static_cast<std::ptrdiff_t>(p.argnum));
        expanded[p.argnum] = expand_macros(copy);
      }
      splice(rep, p.pos, p.pos + 1, expanded[p.argnum]);
    }
  }

  // Get rid of the removed comma if necessary.
  if (comma_patch) {
    rep.erase(std::remove(rep.begin(), rep.end(), nullptr), rep.end());
  }

  // Do a token concatenation pass, stitching any tokens separated by ##
  // into a single token.
  while (!rep.empty() && rep[0]->type == Type::CPP_DPOUND) rep.erase(rep.begin());
  while (!rep.empty() && rep.back()->type == Type::CPP_DPOUND) rep.pop_back();
  std::size_t i = 1;
  bool stitched = false;
  while (static_cast<std::ptrdiff_t>(i) < static_cast<std::ptrdiff_t>(rep.size()) - 1) {
    if (rep[i]->type == Type::CPP_DPOUND) {
      std::size_t j = i + 1;
      while (rep[j]->type == Type::CPP_DPOUND) ++j;
      rep[i - 1] = copy_token(rep[i - 1]);
      rep[i - 1]->type = Type::None;
      rep[i - 1]->value += rep[j]->value;
      rep.erase(rep.begin() + static_cast<std::ptrdiff_t>(i),
                rep.begin() + static_cast<std::ptrdiff_t>(j + 1));
      stitched = true;
    } else {
      ++i;
    }
  }
  if (stitched) {
    // Stitched tokens have no type, so lex them again.
    Lexer lex = lexer;
    for (std::size_t k = 0; k < rep.size(); ++k) {
      if (rep[k]->type != Type::None) continue;
      lex.input(rep[k]->value);
      Tokens toks;
      while (auto tok = lex.token()) toks.push_back(tok);
      if (toks.size() != 1) {
        // Split it once again.
        while (toks.size() > 1) {
          rep.insert(rep.begin() + static_cast<std::ptrdiff_t>(k + 1), copy_token(rep[k]));
          rep[k + 1]->value = toks.back()->value;
          rep[k + 1]->type = toks.back()->type;
          rep[k + 1]->chr = toks.back()->chr;
          toks.pop_back();
        }
        const auto& first = at(toks, 0);
        rep[k]->value = first->value;
        rep[k]->type = first->type;
        rep[k]->chr = first->chr;
      } else {
        rep[k]->type = toks[0]->type;
        rep[k]->chr = toks[0]->chr;
      }
    }
  }
  return rep;
}

namespace {

// The deepest nesting of `expand_macros` the C++ allows. pcpp expands an
// argument with `expand_macros` called afresh, which knows nothing of
// the macros being expanded around it, so `#define G F(G)` with
// `#define F(x) x` expands `G` inside itself without end, and Python
// stops it with a RecursionError when its stack reaches 1000 frames.
// Each level of nesting takes at least two of those frames, and the
// depth Python reaches depends on how deep its caller's stack is, so no
// depth reproduces Python's limit exactly. A program whose macros nest
// 300 deep, deeper than any program of the corpus or the tests, gets
// the RecursionError here where Python might not.
constexpr int kMaxExpansionDepth = 300;

struct DepthGuard {
  explicit DepthGuard(int& depth) : depth_(depth) {
    if (++depth_ > kMaxExpansionDepth) {
      --depth_;
      throw CompileError("RecursionError",
                         "maximum recursion depth exceeded while calling a Python object");
    }
  }
  ~DepthGuard() { --depth_; }
  DepthGuard(const DepthGuard&) = delete;
  DepthGuard& operator=(const DepthGuard&) = delete;

 private:
  int& depth_;
};

}  // namespace

Tokens& Preprocessor::expand_macros(Tokens& tokens, const std::vector<std::string>& expanding_from) {
  const DepthGuard guard(expansion_depth);
  // Each token tracks the macros it has been expanded from, to prevent
  // recursion.
  for (auto& tok : tokens) {
    if (!tok->expanded_from) tok->expanded_from = std::make_shared<std::vector<std::string>>();
  }
  std::size_t i = 0;
  while (i < tokens.size()) {
    const TokenPtr t = tokens[i];
    if (linemacrodepth == 0) linemacro = t->lineno;
    ++linemacrodepth;
    if (t->type == Type::CPP_ID) {
      const auto found = macros.find(t->value);
      if (found != macros.end() && !(t->expanded_from && contains(*t->expanded_from, t->value)) &&
          !contains(expanding_from, t->value)) {
        // A macro, which the expansion may undefine.
        const MacroPtr m = found->second;
        std::vector<std::string> inner = expanding_from;
        inner.push_back(t->value);
        if (!m->arglist) {
          // A simple macro.
          Tokens rep;
          for (const auto& x : m->value) rep.push_back(copy_token(x));
          Tokens& ex = expand_macros(rep, inner);
          for (auto& e : ex) {
            e->source = t->source;
            e->lineno = t->lineno;
            if (!e->expanded_from) e->expanded_from = std::make_shared<std::vector<std::string>>();
            e->expanded_from->push_back(t->value);
          }
          splice(tokens, i, i + 1, ex);
        } else {
          // A macro with arguments.
          std::size_t j = i + 1;
          while (j < tokens.size() && (is_ws(tokens[j]) || is_comment(tokens[j]))) ++j;
          if (j == tokens.size() || tokens[j]->value != "(") {
            // A function-like macro without an invocation list is
            // ignored.
            i = j;
          } else {
            Collected c = collect_args(slice(tokens, j), true);
            if (c.count == 0) {
              // An unclosed parameter list ends the expansion, and pcpp
              // leaves its count of nested expansions raised.
              break;
            }
            auto& args = c.args;
            const std::size_t nparams = m->arglist->size();
            const bool single_empty = args.size() == 1 && args[0].empty();
            if (!m->variadic && (!single_empty || nparams > 1) && args.size() != nparams) {
              on_error(t->source, t->lineno,
                       "Macro " + t->value + " requires " + std::to_string(nparams) +
                           " arguments but was passed " + std::to_string(args.size()));
              i = j + c.count;
            } else if (m->variadic && args.size() + 1 < nparams) {
              if (nparams > 2) {
                on_error(t->source, t->lineno,
                         "Macro " + t->value + " must have at least " +
                             std::to_string(nparams - 1) + " arguments");
              } else {
                on_error(t->source, t->lineno,
                         "Macro " + t->value + " must have at least " +
                             std::to_string(nparams - 1) + " argument");
              }
              i = j + c.count;
            } else {
              if (m->variadic) {
                if (args.size() + 1 == nparams) {
                  args.emplace_back();
                } else {
                  args[nparams - 1] =
                      slice(tokens, j + c.positions.at(nparams - 1), j + c.count - 1);
                  args.resize(nparams);
                }
              } else {
                // A single-parameter macro called with nothing gets
                // empty arguments.
                while (args.size() < nparams) args.emplace_back();
              }
              Tokens rep = macro_expand_args(*m, args);
              Tokens& ex = expand_macros(rep, inner);
              for (auto& e : ex) {
                e->source = t->source;
                e->lineno = t->lineno;
                if (!e->expanded_from) {
                  e->expanded_from = std::make_shared<std::vector<std::string>>();
                }
                e->expanded_from->push_back(t->value);
              }
              // GCC and clang insert a space between the expansion of a
              // macro with arguments and an identifier that follows it,
              // and pcpp does the same
              // (https://github.com/ned14/pcpp/issues/29).
              if (tokens.size() > j + c.count && type_in_string(tokens[j + c.count], "CPP_ID")) {
                auto newtok = copy_token(tokens[j + c.count]);
                newtok->type = Type::CPP_WS;
                newtok->chr.clear();
                newtok->value = " ";
                ex.push_back(newtok);
              }
              splice(tokens, i, j + c.count, ex);
            }
          }
        }
        --linemacrodepth;
        if (linemacrodepth == 0) linemacro = 0;
        continue;
      }
      if (expand_linemacro && t->value == "__LINE__") {
        t->type = Type::CPP_INTEGER;
        t->value = std::to_string(linemacro);
      } else if (expand_countermacro && t->value == "__COUNTER__") {
        t->type = Type::CPP_INTEGER;
        t->value = std::to_string(countermacro);
        ++countermacro;
      }
    }
    ++i;
    --linemacrodepth;
    if (linemacrodepth == 0) linemacro = 0;
  }
  return tokens;
}

EvalOutcome Preprocessor::evalexpr(Tokens& tokens) {
  if (tokens.empty()) {
    on_error(std::string("unknown"), 0, "Empty expression");
    return {EvalResult{0, false}, std::nullopt};
  }
  // `replace_defined` sets a `partial_expansion` of its own, which
  // Python makes a local of the nested function, so it never reaches
  // the one below.
  auto replace_defined = [this](Tokens& toks) {
    std::size_t i = 0;
    while (i < toks.size()) {
      if (toks[i]->type == Type::CPP_ID && toks[i]->value == "defined") {
        std::size_t j = i + 1;
        bool needparen = false;
        std::string result = "0L";
        while (j < toks.size()) {
          if (is_ws(toks[j])) {
            ++j;
            continue;
          }
          if (toks[j]->type == Type::CPP_ID) {
            if (macros.count(toks[j]->value)) {
              result = "1L";
            } else {
              const auto repl = on_unknown_macro_in_defined_expr(toks[j]);
              if (!repl) {
                result = "defined(" + toks[j]->value + ")";
              } else {
                result = *repl ? "1L" : "0L";
              }
            }
            if (!needparen) break;
          } else if (toks[j]->value == "(") {
            needparen = true;
          } else if (toks[j]->value == ")") {
            break;
          } else {
            on_error(toks[i]->source, toks[i]->lineno, "Malformed defined()");
          }
          ++j;
        }
        if (starts_with(result, "defined")) {
          toks[i]->type = Type::CPP_ID;
        } else {
          toks[i]->type = Type::CPP_INTEGER;
        }
        toks[i]->chr.clear();
        toks[i]->value = result;
        toks.erase(toks.begin() + static_cast<std::ptrdiff_t>(std::min(i + 1, toks.size())),
                   toks.begin() + static_cast<std::ptrdiff_t>(std::min(j + 1, toks.size())));
      }
      ++i;
    }
  };
  // Replace any defined(macro) before macro expansion, and again after.
  replace_defined(tokens);
  expand_macros(tokens);
  replace_defined(tokens);
  if (tokens.empty()) return {EvalResult{0, false}, std::nullopt};
  bool vars_partial = false;
  bool functions_partial = false;
  const IdentifierHook identifiers =
      [&](const std::string& key) -> std::variant<std::int64_t, std::string> {
    if (starts_with(key, "defined(")) {
      vars_partial = true;
      return std::int64_t{0};
    }
    const auto repl = on_unknown_macro_in_expr(key);
    if (!repl) {
      vars_partial = true;
      return key;
    }
    return *repl;
  };
  const FunctionHook functions = [&](const std::string& key) -> std::optional<std::string> {
    if (!on_unknown_macro_function_in_expr(key)) {
      functions_partial = true;
      return key;
    }
    return std::nullopt;
  };
  bool partial_expansion = false;
  EvalResult result{0, false};
  try {
    result = call_evaluator(tokens, functions, identifiers);
    partial_expansion = vars_partial || functions_partial;
  } catch (const PyException& e) {
    partial_expansion = vars_partial || functions_partial;
    if (!partial_expansion) {
      on_error(tokens[0]->source, tokens[0]->lineno,
               "Could not evaluate expression due to " + e.repr +
                   " (passed to evaluator: '" + join_values(tokens) + "')");
    }
    result = EvalResult{0, false};
  }
  if (partial_expansion) return {result, tokens};
  return {result, std::nullopt};
}

// ---------------------------------------------------------------------
// parsegen and include.

void Preprocessor::parsegen(const std::string& input, std::optional<std::string> src,
                            const std::optional<std::string>& abssource, const Emit& emit) {
  std::optional<std::string> rewritten_source = src;
  if (abssource && !abssource->empty()) {
    rewritten_source = abssource;
    for (const auto& rule : rewrite_paths) {
      // `re.sub(re.escape(prefix) + '(.*)', replacement, abssource)`.
      const std::size_t at_prefix = abssource->find(rule.prefix);
      if (at_prefix == std::string::npos) continue;
      const std::string temp = abssource->substr(0, at_prefix) + rule.replacement +
                               abssource->substr(at_prefix + rule.prefix.size());
      if (temp != *abssource) {
        rewritten_source = temp;
        if (host.paths.sep() != '/') {
          std::replace(rewritten_source->begin(), rewritten_source->end(), host.paths.sep(), '/');
        }
        break;
      }
    }
  }

  // Replace trigraph sequences.
  const auto lines = group_lines(trigraph(input), rewritten_source);

  if (!src || src->empty()) src = "";
  const std::string file_macro = rewritten_source ? *rewritten_source : "";

  const std::size_t my_include_times_idx = include_times.size();
  include_times.push_back({src, abssource, include_depth, 0.0});
  ++include_depth;
  const double my_include_time_begin = process_clock();
  if (expand_filemacro) define("__FILE__ \"" + file_macro + "\"");

  source = abssource;
  Tokens chunk;
  bool enable = true;
  bool iftrigger = false;
  bool ifpassthru = false;
  struct IfStackEntry {
    bool enable;
    bool iftrigger;
    bool ifpassthru;
    bool rewritten = false;
    std::shared_ptr<Tokens> startlinetoks;
  };
  std::vector<IfStackEntry> ifstack;
  // True until any non-whitespace output or anything with effects
  // happens.
  bool at_front_of_file = true;
  // True if auto pragma once is still a possibility for this #include.
  bool auto_pragma_once_possible = auto_pragma_once_enabled;
  // (MACRO, 0) means #ifndef MACRO or #if !defined(MACRO) seen, and
  // (MACRO, 1) means #define MACRO seen.
  std::optional<std::pair<std::string, int>> include_guard;
  on_potential_include_guard(std::nullopt);

  auto flush = [&]() {
    for (const auto& tok : expand_macros(chunk)) emit(tok);
    chunk.clear();
  };
  // The start of each line of the `--debug` log, `enable:iftrigger:ifpassthru`
  // and a space.
  auto state = [&]() { return bit(enable) + ":" + bit(iftrigger) + ":" + bit(ifpassthru) + " "; };
  auto where = [](const TokenPtr& t) { return str_or_none(t->source) + ":" + std::to_string(t->lineno); };

  for (auto xp : lines) {
    bool all_whitespace = true;
    bool skip_auto_pragma_once_possible_check = false;
    // Handle comments.
    for (const auto& tok : *xp) {
      if (is_comment(tok) && !on_comment(tok)) {
        if (tok->type == Type::CPP_COMMENT1) {
          tok->value = " ";
        } else {
          tok->value = "\n";
        }
        tok->type = Type::CPP_WS;
      }
    }
    // Skip over whitespace. Python's loop leaves `i` and `tok` at the
    // last token when every token is whitespace.
    std::size_t i = 0;
    TokenPtr tok;
    for (i = 0; i < xp->size(); ++i) {
      tok = (*xp)[i];
      if (!is_ws(tok) && !is_comment(tok)) {
        all_whitespace = false;
        break;
      }
    }
    if (i == xp->size()) i = xp->size() - 1;
    bool output_and_expand_line = true;
    bool output_unexpanded_line = false;
    if (tok->value == "#") {
      Tokens precedingtoks{tok};
      output_and_expand_line = false;
      try {
        // Preprocessor directive.
        ++i;
        while (i < xp->size() && is_ws((*xp)[i])) {
          precedingtoks.push_back((*xp)[i]);
          ++i;
        }
        Tokens dirtokens = slice(*xp, i);
        tokenstrip(dirtokens);
        std::string name;
        Tokens args;
        std::optional<bool> handling;
        if (!dirtokens.empty()) {
          name = dirtokens[0]->value;
          args = slice(dirtokens, 1);
          tokenstrip(args);
          if (debug) {
            debug_print(state() + where(dirtokens[0]) + " #" + dirtokens[0]->value + " " +
                        join_values(args));
          }
          handling = on_directive_handle(dirtokens[0], args, ifpassthru, precedingtoks);
        } else {
          throw OutputDirective{Action::IgnoreAndRemove};
        }

        if (name == "define") {
          at_front_of_file = false;
          if (enable) {
            flush();
            if (include_guard && include_guard->second == 0) {
              if (include_guard->first == at(args, 0)->value && args.size() == 1) {
                include_guard = std::make_pair(args[0]->value, 1);
                // If ifpassthru is only on because of this include guard,
                // turn it off.
                if (ifpassthru && !at(ifstack, -1).ifpassthru) ifpassthru = false;
              }
            }
            define(args);
            if (debug) {
              // Python reads the macro back with `self.macros[...]`, which
              // raises KeyError when the definition was rejected.
              const auto it = macros.find(at(args, 0)->value);
              if (it == macros.end()) throw CompileError("KeyError", py_repr(args[0]->value));
              debug_print(state() + where(dirtokens[0]) + "      " + macro_repr(*it->second));
            }
            if (!handling) {
              for (const auto& t : *xp) emit(t);
            }
          }
        } else if (name == "include") {
          if (enable) {
            flush();
            const auto old = macros.find("__FILE__");
            const MacroPtr oldfile = old == macros.end() ? nullptr : old->second;
            if (!args.empty() && args[0]->value != "<" && args[0]->type != Type::CPP_STRING) {
              expand_macros(args);
              tokenstrip(args);
            }
            include(args, *xp, emit);
            if (oldfile) macros["__FILE__"] = oldfile;
            source = abssource;
          }
        } else if (name == "undef") {
          at_front_of_file = false;
          if (enable) {
            flush();
            undef(args);
            if (!handling) {
              for (const auto& t : *xp) emit(t);
            }
          }
        } else if (name == "ifdef") {
          at_front_of_file = false;
          ifstack.push_back({enable, iftrigger, ifpassthru, false, xp});
          if (enable) {
            ifpassthru = false;
            if (!macros.count(at(args, 0)->value)) {
              const auto res = on_unknown_macro_in_defined_expr(args[0]);
              if (!res) {
                ifpassthru = true;
                ifstack.back().rewritten = true;
                throw OutputDirective{Action::IgnoreAndPassThrough};
              }
              if (*res) {
                iftrigger = true;
              } else {
                enable = false;
                iftrigger = false;
              }
            } else {
              iftrigger = true;
            }
          }
        } else if (name == "ifndef") {
          if (ifstack.empty() && at_front_of_file) {
            on_potential_include_guard(at(args, 0)->value);
            include_guard = std::make_pair(args[0]->value, 0);
          }
          at_front_of_file = false;
          ifstack.push_back({enable, iftrigger, ifpassthru, false, xp});
          if (enable) {
            ifpassthru = false;
            if (macros.count(at(args, 0)->value)) {
              enable = false;
              iftrigger = false;
            } else {
              const auto res = on_unknown_macro_in_defined_expr(args[0]);
              if (!res) {
                ifpassthru = true;
                ifstack.back().rewritten = true;
                throw OutputDirective{Action::IgnoreAndPassThrough};
              }
              if (*res) {
                enable = false;
                iftrigger = false;
              } else {
                iftrigger = true;
              }
            }
          }
        } else if (name == "if") {
          if (ifstack.empty() && at_front_of_file) {
            if (at(args, 0)->value == "!" && at(args, 1)->value == "defined") {
              std::ptrdiff_t n = 2;
              if (at(args, n)->value == "(") ++n;
              on_potential_include_guard(at(args, n)->value);
              include_guard = std::make_pair(args[static_cast<std::size_t>(n)]->value, 0);
            }
          }
          at_front_of_file = false;
          ifstack.push_back({enable, iftrigger, ifpassthru, false, xp});
          if (enable) {
            iftrigger = false;
            ifpassthru = false;
            auto [result, rewritten] = evalexpr(args);
            if (rewritten) {
              auto nx = std::make_shared<Tokens>(slice(*xp, 0, i + 2));
              nx->insert(nx->end(), rewritten->begin(), rewritten->end());
              nx->push_back(at(*xp, -1));
              xp = nx;
              auto& space = at(*xp, static_cast<std::ptrdiff_t>(i + 1));
              space = copy_token(space);
              space->type = Type::CPP_WS;
              space->chr.clear();
              space->value = " ";
              ifpassthru = true;
              ifstack.back().rewritten = true;
              throw OutputDirective{Action::IgnoreAndPassThrough};
            }
            if (!result.truthy()) {
              enable = false;
            } else {
              iftrigger = true;
            }
          }
        } else if (name == "elif") {
          at_front_of_file = false;
          if (!ifstack.empty()) {
            if (ifstack.back().enable) {  // Only if the outer #if allows this.
              if (enable && !ifpassthru) {
                // Already true, so this one is false.
                enable = false;
              } else if (!iftrigger) {
                // False but not triggered yet, so check the expression.
                auto [result, rewritten] = evalexpr(args);
                if (rewritten) {
                  enable = true;
                  if (!ifpassthru) {
                    // A passthru #elif after a false #if becomes an #if.
                    at(*xp, static_cast<std::ptrdiff_t>(i))->value = "if";
                  }
                  auto nx = std::make_shared<Tokens>(slice(*xp, 0, i + 2));
                  nx->insert(nx->end(), rewritten->begin(), rewritten->end());
                  nx->push_back(at(*xp, -1));
                  xp = nx;
                  auto& space = at(*xp, static_cast<std::ptrdiff_t>(i + 1));
                  space = copy_token(space);
                  space->type = Type::CPP_WS;
                  space->chr.clear();
                  space->value = " ";
                  ifpassthru = true;
                  ifstack.back().rewritten = true;
                  throw OutputDirective{Action::IgnoreAndPassThrough};
                }
                if (ifpassthru) {
                  // If this elif can only ever be true, simulate that.
                  if (result.truthy()) {
                    auto newtok = copy_token(at(*xp, static_cast<std::ptrdiff_t>(i + 3)));
                    newtok->type = Type::CPP_INTEGER;
                    newtok->chr.clear();
                    newtok->value = result.str();
                    auto nx = std::make_shared<Tokens>(slice(*xp, 0, i + 2));
                    nx->push_back(newtok);
                    nx->push_back(at(*xp, -1));
                    xp = nx;
                    throw OutputDirective{Action::IgnoreAndPassThrough};
                  }
                  // Otherwise elide.
                  enable = false;
                } else if (result.truthy()) {
                  enable = true;
                  iftrigger = true;
                }
              }
            }
          } else {
            on_error(dirtokens[0]->source, dirtokens[0]->lineno, "Misplaced #elif");
          }
        } else if (name == "else") {
          at_front_of_file = false;
          if (!ifstack.empty()) {
            if (ifstack.back().enable) {
              if (ifpassthru) {
                enable = true;
                throw OutputDirective{Action::IgnoreAndPassThrough};
              }
              if (enable) {
                enable = false;
              } else if (!iftrigger) {
                enable = true;
                iftrigger = true;
              }
            }
          } else {
            on_error(dirtokens[0]->source, dirtokens[0]->lineno, "Misplaced #else");
          }
        } else if (name == "endif") {
          at_front_of_file = false;
          if (!ifstack.empty()) {
            const IfStackEntry old = ifstack.back();
            ifstack.pop_back();
            enable = old.enable;
            iftrigger = old.iftrigger;
            ifpassthru = old.ifpassthru;
            if (debug) {
              const TokenPtr& first = at(*old.startlinetoks, 0);
              debug_print(state() + where(dirtokens[0]) + "      (" + where(first) + " " +
                          join_values(*old.startlinetoks) + ")");
            }
            skip_auto_pragma_once_possible_check = true;
            if (old.rewritten) throw OutputDirective{Action::IgnoreAndPassThrough};
          } else {
            on_error(dirtokens[0]->source, dirtokens[0]->lineno, "Misplaced #endif");
          }
        } else if (name == "pragma" && at(args, 0)->value == "once") {
          if (enable) include_once_set(source, std::nullopt);
        } else if (enable) {
          // Unknown preprocessor directive.
          output_unexpanded_line =
              !on_directive_unknown(dirtokens[0], args, ifpassthru, precedingtoks).has_value();
        }
      } catch (const OutputDirective& e) {
        if (e.action == Action::IgnoreAndPassThrough) output_unexpanded_line = true;
      }
    }

    // If there is ever any non-whitespace output outside an include
    // guard, auto pragma once is not possible.
    if (!skip_auto_pragma_once_possible_check && auto_pragma_once_possible && ifstack.empty() &&
        !all_whitespace) {
      auto_pragma_once_possible = false;
      if (debug) {
        debug_print(state() + where(at(*xp, 0)) + " Determined that #include \"" +
                    str_or_none(source) +
                    "\" is not entirely wrapped in an include guard macro, disabling "
                    "auto-applying #pragma once");
      }
    }

    if (output_and_expand_line || output_unexpanded_line) {
      if (!all_whitespace) at_front_of_file = false;
      // Normal text.
      if (enable) {
        if (output_and_expand_line) {
          chunk.insert(chunk.end(), xp->begin(), xp->end());
        } else {
          flush();
          for (const auto& t : *xp) emit(t);
        }
      } else {
        // Keep the line's whitespace, so the same number of lines
        // follows.
        Tokens& x = *xp;
        x.erase(std::remove_if(x.begin(), x.end(), [](const TokenPtr& t) { return !is_ws(t); }),
                x.end());
        chunk.insert(chunk.end(), x.begin(), x.end());
      }
    }
  }

  flush();
  for (const auto& entry : ifstack) {
    const auto& first = at(*entry.startlinetoks, 0);
    on_error(first->source, first->lineno, "Unterminated " + join_values(*entry.startlinetoks));
  }
  if (auto_pragma_once_possible && include_guard && include_guard->second == 1) {
    if (debug) {
      debug_print(state() + str_or_none(source) + ":0 Determined that #include \"" +
                  str_or_none(source) +
                  "\" is entirely wrapped in an include guard macro called " +
                  include_guard->first + ", auto-applying #pragma once");
    }
    include_once_set(source, include_guard->first);
  } else if (auto_pragma_once_enabled && !include_once_has(source) && debug) {
    const std::string guard = include_guard ? "(" + py_repr(include_guard->first) + ", " +
                                                  std::to_string(include_guard->second) + ")"
                                            : "None";
    debug_print(state() + str_or_none(source) +
                ":0 Did not auto apply #pragma once to this file due to "
                "auto_pragma_once_possible=" +
                bit(auto_pragma_once_possible) + ", include_guard=" + guard);
  }
  include_times[my_include_times_idx].elapsed = process_clock() - my_include_time_begin;
  --include_depth;
}

void Preprocessor::include(Tokens tokens, const Tokens& original_line, const Emit& emit) {
  // Try to extract the file name and then process an include file.
  if (tokens.empty()) return;
  if (tokens[0]->value != "<" && tokens[0]->type != Type::CPP_STRING) {
    expand_macros(tokens);
    tokenstrip(tokens);
  }
  bool is_system_include = false;
  std::string filename;
  std::vector<std::string> search;
  if (at(tokens, 0)->value == "<") {
    is_system_include = true;
    // Include <...>
    std::size_t i = 1;
    while (i < tokens.size() && tokens[i]->value != ">") ++i;
    if (i == tokens.size()) {
      on_error(tokens[0]->source, tokens[0]->lineno, "Malformed #include <...>");
      return;
    }
    filename = join_values(slice(tokens, 1, i));
    // Search only formally specified paths.
    search = path;
  } else if (tokens[0]->type == Type::CPP_STRING) {
    const std::string& v = tokens[0]->value;
    filename = v.size() >= 2 ? v.substr(1, v.size() - 2) : std::string();
    // Search from each nested include file, as well as formally
    // specified paths.
    search = temp_path;
    search.insert(search.end(), path.begin(), path.end());
  } else {
    on_include_not_found(true, false, temp_path.empty() ? std::string() : temp_path[0],
                         tokens[0]->value);
    return;
  }
  if (search.empty()) search = {""};
  // pcpp matches the pattern with `.match` on the text of the tokens.
  std::u32string joined;
  for (const auto c : pyrepr::code_points(join_values(tokens))) joined += static_cast<char32_t>(c);
  auto passthru = [&] { return passthru_includes && passthru_includes->match(joined, 0).has_value(); };
  while (true) {
    for (const auto& p : search) {
      const std::string iname = host.paths.join(p, filename);
      const std::string fulliname = host.paths.abspath(iname);
      if (include_once_has(fulliname)) {
        if (debug) debug_print("x:x:x x:x #include \"" + fulliname + "\" skipped as already seen");
        if (passthru()) {
          for (const auto& t : original_line) emit(t);
        }
        return;
      }
      const auto data = on_file_open(is_system_include, fulliname);
      if (!data) continue;
      const std::string dname = host.paths.dirname(fulliname);
      if (!dname.empty()) temp_path.insert(temp_path.begin(), dname);
      if (passthru()) {
        for (const auto& t : original_line) emit(t);
        parsegen(*data, filename, fulliname, [](const TokenPtr&) {});
      } else {
        parsegen(*data, filename, fulliname, emit);
      }
      if (!dname.empty()) temp_path.erase(temp_path.begin());
      return;
    }
    search.push_back(on_include_not_found(false, is_system_include,
                                          temp_path.empty() ? std::string() : temp_path[0],
                                          filename));
  }
}

// ---------------------------------------------------------------------
// define and undef.

void Preprocessor::define(const std::string& text) { define(tokenize(text)); }

void Preprocessor::define(const Tokens& original) {
  Tokens linetok;
  for (const auto& t : original) linetok.push_back(copy_token(t));
  const TokenPtr name = at(linetok, 0);
  auto add_macro = [&](const MacroPtr& m) {
    m->source = name->source;
    m->lineno = name->lineno;
    macros[name->value] = m;
  };
  const TokenPtr mtype = linetok.size() > 1 ? linetok[1] : nullptr;
  if (!mtype) {
    auto m = std::make_shared<Macro>();
    m->name = name->value;
    add_macro(m);
  } else if (is_ws(mtype)) {
    // A normal macro.
    auto m = std::make_shared<Macro>();
    m->name = name->value;
    m->value = slice(linetok, 2);
    tokenstrip(m->value);
    add_macro(m);
  } else if (mtype->value == "(") {
    // A macro with arguments.
    Collected c = collect_args(slice(linetok, 1), false);
    bool variadic = false;
    bool broke = false;
    for (auto& a : c.args) {
      if (variadic) {
        on_error(name->source, name->lineno, "No more arguments may follow a variadic argument");
        broke = true;
        break;
      }
      const std::string astr = join_values(a);
      if (astr == "...") {
        variadic = true;
        a[0]->type = Type::CPP_ID;
        a[0]->chr.clear();
        a[0]->value = "__VA_ARGS__";
        a.resize(1);
        continue;
      }
      if (astr.size() >= 3 && astr.compare(astr.size() - 3, 3, "...") == 0 &&
          a[0]->type == Type::CPP_ID) {
        variadic = true;
        a.resize(1);
        // If, for some reason, "." is part of the identifier, strip off
        // the name for the purposes of macro expansion.
        std::string& v = a[0]->value;
        if (v.size() >= 3 && v.compare(v.size() - 3, 3, "...") == 0) v.erase(v.size() - 3);
        continue;
      }
      // Empty arguments are permitted.
      if (a.empty() && c.args.size() == 1) continue;
      if (a.size() > 1 || at(a, 0)->type != Type::CPP_ID) {
        on_error(a[0]->source, a[0]->lineno, "Invalid macro argument");
        broke = true;
        break;
      }
    }
    if (!broke) {
      Tokens mvalue = slice(linetok, 1 + c.count);
      tokenstrip(mvalue);
      std::size_t i = 0;
      while (i < mvalue.size()) {
        if (i + 1 < mvalue.size()) {
          if (is_ws(mvalue[i]) && mvalue[i + 1]->value == "##") {
            mvalue.erase(mvalue.begin() + static_cast<std::ptrdiff_t>(i));
            continue;
          }
          if (mvalue[i]->value == "##" && is_ws(mvalue[i + 1])) {
            mvalue.erase(mvalue.begin() + static_cast<std::ptrdiff_t>(i + 1));
          }
        }
        ++i;
      }
      auto m = std::make_shared<Macro>();
      m->name = name->value;
      m->value = mvalue;
      std::vector<std::string> arglist;
      if (!(c.args.size() == 1 && c.args[0].empty())) {
        for (const auto& a : c.args) arglist.push_back(at(a, 0)->value);
      }
      m->arglist = arglist;
      m->variadic = variadic;
      if (variadic) m->vararg = at(arglist, -1);
      macro_prescan(*m);
      add_macro(m);
    }
  } else {
    on_error(name->source, name->lineno, "Bad macro definition");
  }
}

void Preprocessor::undef(const std::string& text) { undef(tokenize(text)); }

void Preprocessor::undef(const Tokens& tokens) { macros.erase(at(tokens, 0)->value); }

std::string Preprocessor::parse_and_write(const std::string& input,
                                          const std::optional<std::string>& src) {
  std::optional<std::string> abs;
  if (src && !src->empty()) abs = host.paths.abspath(*src);
  if (src) temp_path.insert(temp_path.begin(), host.paths.dirname(*src));
  Writer writer(*this);
  parsegen(input, src, abs, [&writer](const TokenPtr& tok) { writer.push(tok); });
  return writer.finish();
}

}  // namespace c6502::pcpp
