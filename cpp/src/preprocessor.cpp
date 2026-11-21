#include "preprocessor.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <regex>
#include <tuple>
#include <map>
#include <utility>

#include "diagnostics.h"
#include "errors.h"
#include "pp_numbers.h"
#include "pyrepr.h"

namespace c6502::preprocessor {

const char* const kAltBoolWarning =
    "`_Bool` is deprecated: C23 makes it an obsolescent spelling of the keyword `bool` (C23 "
    "\xC2\xA7"
    "6.4.1). Use `bool` instead.";

namespace {

using pcpp::Action;
using pcpp::OutputDirective;
using pcpp::Token;
using pcpp::TokenPtr;
using pcpp::Tokens;
using pcpp::Type;

constexpr const char* kPcppVersion = "1.30";

// The largest value of `intmax_t`, a `long long` in c6502, and of
// `uintmax_t`. An `#if` computes in those types (C23 §6.10.2p13).
constexpr asdl::Int kIntmaxMax = (asdl::Int{1} << 63) - 1;
constexpr asdl::Int kUintmaxMax = (asdl::Int{1} << 64) - 1;

// ---------------------------------------------------------------------
// The flags, as argparse reads them.

enum class Kind { Append, Flag, Store, Optional };

struct Option {
  const char* name;
  Kind kind;
};

// The options of `_build_parser`, in the order it adds them, which is
// the order argparse lists the matches of an ambiguous abbreviation in.
const Option kOptions[] = {
    {"-D", Kind::Append},
    {"-U", Kind::Append},
    {"-N", Kind::Append},
    {"-I", Kind::Append},
    {"--passthru-defines", Kind::Flag},
    {"--passthru-unfound-includes", Kind::Flag},
    {"--passthru-unknown-exprs", Kind::Flag},
    {"--passthru-comments", Kind::Flag},
    {"--passthru-magic-macros", Kind::Flag},
    {"--passthru-includes", Kind::Store},
    {"--disable-auto-pragma-once", Kind::Flag},
    {"--line-directive", Kind::Optional},
    {"--debug", Kind::Flag},
    {"--time", Kind::Flag},
    {"--filetimes", Kind::Optional},
    {"--compress", Kind::Flag},
    {"--assume-input-encoding", Kind::Store},
    {"--output-encoding", Kind::Store},
    {"--write-bom", Kind::Flag},
};

constexpr const char* kUsage =
    "usage: preprocess [-D macro[=val]] [-U macro] [-N macro] [-I path]\n"
    "                  [--passthru-defines] [--passthru-unfound-includes]\n"
    "                  [--passthru-unknown-exprs] [--passthru-comments]\n"
    "                  [--passthru-magic-macros] [--passthru-includes <regex>]\n"
    "                  [--disable-auto-pragma-once] [--line-directive [form]]\n"
    "                  [--debug] [--time] [--filetimes [path]] [--compress]\n"
    "                  [--assume-input-encoding <encoding>]\n"
    "                  [--output-encoding <encoding>] [--write-bom]\n";

[[noreturn]] void argparse_error(std::string& err, const std::string& message) {
  err += kUsage;
  err += "preprocess: error: " + message + "\n";
  throw CompileError("SystemExit", "2");
}

// One argument as argparse classifies it: positional ('A'), the `--`
// that ends the options ('-'), or an option ('O') with the option it
// names (null for an unknown one) and the argument given after `=` or
// joined to a short option.
struct Classified {
  char pattern;
  const Option* option = nullptr;
  std::optional<std::string> explicit_arg;
};

bool is_negative_number(const std::string& s) {
  // `^-\d+$|^-\d*\.\d+$`
  if (s.size() < 2 || s[0] != '-') return false;
  std::size_t i = 1;
  auto digits = [&] {
    const std::size_t start = i;
    while (i < s.size() && s[i] >= '0' && s[i] <= '9') ++i;
    return i - start;
  };
  const std::size_t d1 = digits();
  if (i == s.size()) return d1 > 0;
  if (s[i] != '.') return false;
  ++i;
  return digits() > 0 && i == s.size();
}

Classified classify(const std::string& arg, std::string& err) {
  if (arg.empty() || arg[0] != '-') return {'A'};
  for (const auto& o : kOptions) {
    if (arg == o.name) return {'O', &o, std::nullopt};
  }
  if (arg.size() == 1) return {'A'};
  const std::size_t eq = arg.find('=');
  if (eq != std::string::npos) {
    const std::string name = arg.substr(0, eq);
    for (const auto& o : kOptions) {
      if (name == o.name) return {'O', &o, arg.substr(eq + 1)};
    }
  }
  std::vector<Classified> tuples;
  if (arg[1] == '-') {
    const std::string prefix = eq == std::string::npos ? arg : arg.substr(0, eq);
    std::optional<std::string> explicit_arg;
    if (eq != std::string::npos) explicit_arg = arg.substr(eq + 1);
    for (const auto& o : kOptions) {
      if (std::string_view(o.name).substr(0, prefix.size()) == prefix) {
        tuples.push_back({'O', &o, explicit_arg});
      }
    }
  } else {
    const std::string short_prefix = arg.substr(0, 2);
    for (const auto& o : kOptions) {
      if (short_prefix == o.name) {
        tuples.push_back({'O', &o, arg.substr(2)});
      } else if (std::string_view(o.name).substr(0, arg.size()) == arg) {
        tuples.push_back({'O', &o, std::nullopt});
      }
    }
  }
  if (tuples.size() > 1) {
    std::string matches;
    for (const auto& t : tuples) {
      if (!matches.empty()) matches += ", ";
      matches += t.option->name;
    }
    argparse_error(err, "ambiguous option: " + arg + " could match " + matches);
  }
  if (tuples.size() == 1) return tuples[0];
  if (is_negative_number(arg)) return {'A'};
  if (arg.find(' ') != std::string::npos) return {'A'};
  return {'O', nullptr, std::nullopt};
}

}  // namespace

Args parse_args(const std::vector<std::string>& argv, std::string& err) {
  std::vector<Classified> classes;
  bool after_dashes = false;
  for (const auto& arg : argv) {
    if (after_dashes) {
      classes.push_back({'A'});
    } else if (arg == "--") {
      after_dashes = true;
      classes.push_back({'-'});
    } else {
      classes.push_back(classify(arg, err));
    }
  }
  Args args;
  std::vector<std::string> extras;
  for (std::size_t i = 0; i < argv.size(); ++i) {
    const Classified& c = classes[i];
    if (c.pattern != 'O' || c.option == nullptr) {
      extras.push_back(argv[i]);
      continue;
    }
    const Option& o = *c.option;
    const std::string name = o.name;
    std::optional<std::string> value;
    if (o.kind == Kind::Flag) {
      if (c.explicit_arg) {
        argparse_error(err, "argument " + name + ": ignored explicit argument " +
                                pcpp::py_repr(*c.explicit_arg));
      }
    } else if (c.explicit_arg) {
      value = c.explicit_arg;
    } else if (i + 1 < argv.size() && classes[i + 1].pattern == 'A') {
      value = argv[++i];
    } else if (o.kind != Kind::Optional) {
      argparse_error(err, "argument " + name + ": expected 1 argument");
    }
    auto append = [&](std::optional<std::vector<std::string>>& list) {
      if (!list) list.emplace();
      list->push_back(*value);
    };
    if (name == "-D") {
      append(args.defines);
    } else if (name == "-U") {
      append(args.undefines);
    } else if (name == "-N") {
      append(args.nevers);
    } else if (name == "-I") {
      append(args.includes);
    } else if (name == "--passthru-defines") {
      args.passthru_defines = true;
    } else if (name == "--passthru-unfound-includes") {
      args.passthru_unfound_includes = true;
    } else if (name == "--passthru-unknown-exprs") {
      args.passthru_undefined_exprs = true;
    } else if (name == "--passthru-comments") {
      args.passthru_comments = true;
    } else if (name == "--passthru-magic-macros") {
      args.passthru_magic_macros = true;
    } else if (name == "--passthru-includes") {
      args.passthru_includes = value;
    } else if (name == "--disable-auto-pragma-once") {
      args.auto_pragma_once_disabled = true;
    } else if (name == "--line-directive") {
      args.line_directive = value;
    } else if (name == "--debug") {
      args.debug = true;
    } else if (name == "--time") {
      args.time = true;
    } else if (name == "--filetimes") {
      args.filetimes = value;
      if (value) {
        // argparse's FileType('wt') opens the file as it reads the flag.
        std::ofstream f(std::filesystem::path(std::u8string(value->begin(), value->end())));
        if (!f) {
          argparse_error(err, "argument --filetimes: can't open " + pcpp::py_repr(*value));
        }
      }
    } else if (name == "--compress") {
      args.compress = true;
    } else if (name == "--assume-input-encoding") {
      args.assume_input_encoding = value;
    } else if (name == "--output-encoding") {
      args.output_encoding = value;
    } else if (name == "--write-bom") {
      args.write_bom = true;
    }
  }
  for (const auto& arg : extras) err += "NOTE: Argument " + arg + " not known, ignoring!\n";
  return args;
}

namespace {

// The `#pragma c6502` directives c6502 used to take, with the
// whitespace removed, and what to write instead of each.
const std::pair<const char*, const char*> kReplacedPragmas[] = {
    {"loopunroll(enable)",
     "write the statement attribute `[[c6502::unroll]]` before the loop instead, as in "
     "`[[c6502::unroll]] for (...)`"},
    {"loopunroll(disable)", "it never did anything, so delete the line"},
    {"loopkeep",
     "write the statement attribute `[[c6502::keep]]` before the loop instead, as in "
     "`[[c6502::keep]] for (...)`"},
    {"callsave(hardware_stack)",
     "write the statement attribute `[[c6502::save_hardware_stack]]` before the statement or "
     "declaration instead, as in `[[c6502::save_hardware_stack]] f(x);`"},
};

// Throws PreprocessorError for a `#pragma c6502` directive, whose
// message names the C23 statement attribute that replaced it
// (docs/attributes.md). `toks` are the tokens after `pragma`, and
// `where` names the line, as "line 3" or "line 3 of foo.h". Any other
// pragma passes.
void check_pragma(const Tokens& toks, const std::string& where) {
  std::string joined;
  for (const auto& t : toks) joined += t->value;
  const std::string body = pcpp::strip(joined);
  const auto words = pcpp::split_whitespace(body);
  if (words.empty() || words[0] != "c6502") return;
  const std::string line = "#pragma " + body;
  std::string key;
  for (std::size_t i = 1; i < words.size(); ++i) key += words[i];
  for (const auto& [name, instead] : kReplacedPragmas) {
    if (key == name) {
      throw CompileError("PreprocessorError",
                         where + ": c6502 no longer takes `" + line + "`; " + instead);
    }
  }
  throw CompileError(
      "PreprocessorError",
      where + ": c6502 takes no `#pragma c6502` directives, so `" + line +
          "` is an error. Its directives are the statement attributes `[[c6502::unroll]]` and "
          "`[[c6502::keep]]` before a `for` loop, and `[[c6502::save_hardware_stack]]` before "
          "a statement or declaration");
}

// Returns the spelling pcpp's evaluator reads for the pp-number
// `spelling` in an `#if`, which is its value in decimal, with a `U` when
// it's unsigned. Throws NumberError when `spelling` isn't an integer
// constant that fits in `uintmax_t`.
//
// A `wb` or `uwb` constant is a `_BitInt` or `unsigned _BitInt`, and in
// `#if` every signed integer type acts as `intmax_t` and every unsigned
// one as `uintmax_t` (C23 §6.10.2p13), so `wb` is signed whatever its
// value and `uwb` unsigned. With BITINT_MAXWIDTH at 64, every `wb` value
// that has a type fits in `intmax_t`.
std::string if_integer(const std::string& spelling) {
  if (pp_numbers::is_floating(spelling)) {
    pp_numbers::check_floating(spelling);
    throw pp_numbers::NumberError("an #if expression can't contain the floating constant " +
                                  pcpp::py_repr(spelling) +
                                  ", since it's an integer constant expression (C23 "
                                  "\xC2\xA7"
                                  "6.10.2p3)");
  }
  const auto constant = pp_numbers::parse_integer(spelling);
  if (constant.bit_precise()) {
    pp_numbers::bit_precise_width(constant, spelling);
    return pyrepr::integer(constant.value) + (constant.is_unsigned ? "U" : "");
  }
  if (!constant.fits() || constant.value > kUintmaxMax) {
    throw pp_numbers::NumberError("integer constant " + pcpp::py_repr(spelling) +
                                  " doesn't fit in uintmax_t");
  }
  if (constant.value > kIntmaxMax && constant.base == 10 && !constant.is_unsigned) {
    throw pp_numbers::NumberError("decimal constant " + pcpp::py_repr(spelling) +
                                  " doesn't fit in intmax_t (a `u` suffix makes it unsigned)");
  }
  const bool is_unsigned = constant.is_unsigned || constant.value > kIntmaxMax;
  return pyrepr::integer(constant.value) + (is_unsigned ? "U" : "");
}

// Returns whether `line` is a `#pragma` line, as `_PRAGMA_LINE_RE`,
// `^[ \t]*#[ \t]*pragma\b(?P<body>.*)$`, matches it.
bool is_pragma_line(const std::string& line) {
  std::size_t i = 0;
  while (i < line.size() && (line[i] == ' ' || line[i] == '\t')) ++i;
  if (i >= line.size() || line[i] != '#') return false;
  ++i;
  while (i < line.size() && (line[i] == ' ' || line[i] == '\t')) ++i;
  if (line.compare(i, 6, "pragma") != 0) return false;
  i += 6;
  if (i == line.size()) return true;
  std::size_t len = 0;
  return !pcpp::is_word(pcpp::decode_at(line, i, len));
}

// Strips every `#pragma` line pcpp passed through, as C99 §6.10.6 lets
// an implementation do with a pragma it doesn't recognize. The
// `#pragma c6502` lines were rejected before. Each `#pragma` line becomes
// a blank line, so that the line numbers of later errors stay the same.
std::string strip_pragmas(const std::string& text) {
  std::string out;
  std::size_t start = 0;
  while (true) {
    const std::size_t end = text.find('\n', start);
    const std::string line =
        text.substr(start, end == std::string::npos ? std::string::npos : end - start);
    if (!is_pragma_line(line)) out += line;
    if (end == std::string::npos) break;
    out += '\n';
    start = end + 1;
  }
  return out;
}

bool in_list(const std::optional<std::vector<std::string>>& list, const std::string& s) {
  if (!list) return false;
  for (const auto& x : *list) {
    if (x == s) return true;
  }
  return false;
}

// pcpp's Preprocessor configured the same way as pcpp.pcmd.CmdPreprocessor.
// Adapted from `pcpp.pcmd.CmdPreprocessor.__init__` and its `on_*`
// overrides, with the file I/O, argv parsing, and `inputs`/`-o` handling
// stripped out.
class CompilePreprocessor : public pcpp::Preprocessor {
 public:
  CompilePreprocessor(pcpp::Host h, Args a, const std::string& include_dir)
      : Preprocessor(std::move(h)), args(std::move(a)) {
    define(std::string("__PCPP_VERSION__ ") + kPcppVersion);
    define("__PCPP_ALWAYS_FALSE__ 0");
    define("__PCPP_ALWAYS_TRUE__ 1");
    if (args.debug) {
      // pcpp opens its log here. The C++ writes nothing to it.
      std::ofstream("pcpp_debug.log");
    }
    auto_pragma_once_enabled = !args.auto_pragma_once_disabled;
    line_directive = args.line_directive;
    if (line_directive) {
      std::string lower = *line_directive;
      for (auto& c : lower) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
      if (lower == "nothing" || lower == "none" || lower.empty()) line_directive.reset();
    }
    if (args.passthru_includes) passthru_includes = std::regex(*args.passthru_includes);
    compress = args.compress ? 2 : 0;
    if (args.passthru_magic_macros) {
      undef("__DATE__");
      undef("__TIME__");
      expand_linemacro = false;
      expand_filemacro = false;
      expand_countermacro = false;
    }
    if (args.assume_input_encoding) assume_encoding = args.assume_input_encoding;
    if (args.defines) {
      for (std::string d : *args.defines) {
        if (d.find('=') == std::string::npos) d += "=1";
        d[d.find('=')] = ' ';
        define(d);
      }
    }
    if (args.undefines) {
      for (const auto& d : *args.undefines) undef(d);
    }
    if (args.includes) {
      for (const auto& d : *args.includes) add_path(d);
    }
    // c6502's own headers are searched last, so a `-I` path can shadow
    // them.
    if (host.isdir && host.isdir(include_dir)) add_path(include_dir);
  }

  std::string run(const std::string& src, const std::optional<std::string>& filename) {
    input_file = filename;
    std::string out;
    if (args.write_bom) out += "\xEF\xBB\xBF";
    out += parse_and_write(src, filename);
    if (args.time) print_time_report();
    if (args.filetimes) write_filetimes_csv(*args.filetimes);
    return out;
  }

  void on_error(const std::optional<std::string>& file, int line,
                const std::string& msg) override {
    // `evalexpr` raises the error for an invalid number in an `#if`, so
    // pcpp's report of its evaluator's failure would only repeat it.
    if (number_error) return;
    Preprocessor::on_error(file, line, msg);
  }

  std::string on_include_not_found(bool is_malformed, bool is_system_include,
                                   const std::string& curdir,
                                   const std::string& includepath) override {
    if (args.passthru_unfound_includes) throw OutputDirective{Action::IgnoreAndPassThrough};
    return Preprocessor::on_include_not_found(is_malformed, is_system_include, curdir,
                                              includepath);
  }

  std::optional<bool> on_unknown_macro_in_defined_expr(const TokenPtr& tok) override {
    if (in_list(args.undefines, tok->value)) return false;
    if (args.passthru_undefined_exprs) return std::nullopt;
    return Preprocessor::on_unknown_macro_in_defined_expr(tok);
  }

  std::optional<std::int64_t> on_unknown_macro_in_expr(const std::string& ident) override {
    // C23 §6.10.2p13 gives `true` and `false` left in an `#if` the values
    // 1 and 0.
    if (ident == "true") return 1;
    if (ident == "false") return 0;
    if (in_list(args.undefines, ident)) return Preprocessor::on_unknown_macro_in_expr(ident);
    if (args.passthru_undefined_exprs) return std::nullopt;
    return Preprocessor::on_unknown_macro_in_expr(ident);
  }

  bool on_unknown_macro_function_in_expr(const std::string& ident) override {
    if (in_list(args.undefines, ident)) return Preprocessor::on_unknown_macro_function_in_expr(ident);
    if (args.passthru_undefined_exprs) return false;
    return Preprocessor::on_unknown_macro_function_in_expr(ident);
  }

  std::optional<bool> on_directive_handle(const TokenPtr& directive, const Tokens& toks,
                                          bool ifpassthru, const Tokens& precedingtoks) override {
    const std::string& d = directive->value;
    if (ifpassthru) {
      if (d == "if" || d == "elif" || d == "else" || d == "endif") {
        bypass_ifpassthru = false;
        for (const auto& tok : toks) {
          if (tok->value == "__PCPP_ALWAYS_FALSE__" || tok->value == "__PCPP_ALWAYS_TRUE__") {
            bypass_ifpassthru = true;
            break;
          }
        }
      }
      if (!bypass_ifpassthru && (d == "define" || d == "undef")) {
        if (first(toks)->value != potential_include_guard) {
          throw OutputDirective{Action::IgnoreAndPassThrough};
        }
      }
    }
    if ((d == "define" || d == "undef") && args.nevers && !args.nevers->empty() &&
        in_list(args.nevers, first(toks)->value)) {
      throw OutputDirective{Action::IgnoreAndPassThrough};
    }
    if (args.passthru_defines) {
      Preprocessor::on_directive_handle(directive, toks, ifpassthru, precedingtoks);
      return std::nullopt;
    }
    return Preprocessor::on_directive_handle(directive, toks, ifpassthru, precedingtoks);
  }

  std::optional<bool> on_directive_unknown(const TokenPtr& directive, const Tokens& toks,
                                           bool ifpassthru, const Tokens& precedingtoks) override {
    // pcpp passes `#pragma` lines other than `#pragma once` through here,
    // with the file and line they're on.
    if (directive->value == "pragma") check_pragma(toks, where(directive));
    if (ifpassthru) return std::nullopt;
    return Preprocessor::on_directive_unknown(directive, toks, ifpassthru, precedingtoks);
  }

  // Evaluates an `#if` or `#elif` expression, as `Preprocessor.evalexpr`
  // does, and raises PreprocessorError for a number in it that isn't a
  // valid integer constant.
  pcpp::EvalOutcome evalexpr(Tokens& tokens) override {
    number_error.reset();
    auto result = Preprocessor::evalexpr(tokens);
    if (number_error) throw CompileError("PreprocessorError", *number_error);
    return result;
  }

  // `_NumberEvaluator`: rewrites each number token into the spelling
  // `if_integer` gives before pcpp's evaluator reads it. pcpp catches
  // whatever its evaluator raises, so the message for an invalid number
  // goes in `number_error`, which `evalexpr` raises after pcpp returns.
  pcpp::EvalResult call_evaluator(const Tokens& tokens, const pcpp::FunctionHook& functions,
                                  const pcpp::IdentifierHook& identifiers) override {
    Tokens rewritten;
    for (const auto& tok : tokens) {
      if (tok->type == Type::CPP_INTEGER) {
        std::string value;
        try {
          value = if_integer(tok->value);
        } catch (const pp_numbers::NumberError& e) {
          number_error = where(tok) + ": " + e.what();
          throw pcpp::py_exception("NumberError", e.what());
        }
        auto copy = pcpp::copy_token(tok);
        copy->value = value;
        rewritten.push_back(copy);
      } else {
        rewritten.push_back(tok);
      }
    }
    return Preprocessor::call_evaluator(rewritten, functions, identifiers);
  }

  // Warns about each `_Bool` that pcpp writes out. A `_Bool` that a macro
  // expands to has the line of the macro's use.
  void on_token(const TokenPtr& tok) override {
    if (tok->type == Type::CPP_ID && tok->value == "_Bool") {
      diagnostics::warn(where(tok), kAltBoolWarning);
    }
  }

  void on_potential_include_guard(const std::optional<std::string>& macro) override {
    potential_include_guard = macro;
    Preprocessor::on_potential_include_guard(macro);
  }

  bool on_comment(const TokenPtr& tok) override {
    if (args.passthru_comments) return true;
    return Preprocessor::on_comment(tok);
  }

 private:
  static const TokenPtr& first(const Tokens& toks) {
    if (toks.empty()) throw CompileError("IndexError", "list index out of range");
    return toks[0];
  }

  // Names the line `tok` is on, as "line 3" in the file being
  // preprocessed, or "line 3 of <path>" in a file it includes.
  // compile.py's message names the file being compiled.
  std::string where(const TokenPtr& tok) const {
    const auto& src = tok->source;
    if (!src || (input_file && host.paths.normcase(host.paths.abspath(*src)) ==
                                   host.paths.normcase(host.paths.abspath(*input_file)))) {
      return "line " + std::to_string(tok->lineno);
    }
    return "line " + std::to_string(tok->lineno) + " of " + *src;
  }

  static std::string fixed(double v) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "%f", v);
    return buf;
  }

  void print_time_report() {
    std::string& out = stderr_text;
    out += "\nTime report:\n";
    out += "============\n";
    for (std::size_t n = 0; n < include_times.size(); ++n) {
      const auto& t = include_times[n];
      const std::string name = t.included_path ? *t.included_path : "None";
      if (n == 0) {
        out += "top level: " + fixed(t.elapsed) + " seconds\n";
      } else if (t.depth == 1) {
        if (include_times[0].elapsed == 0.0) {
          throw CompileError("ZeroDivisionError", "float division by zero");
        }
        const double pct = 100 * t.elapsed / include_times[0].elapsed;
        out += "\n " + name + ": " + fixed(t.elapsed) + " seconds (" + fixed(pct) + "%)\n";
      } else {
        out += std::string(static_cast<std::size_t>(t.depth), ' ') + name + ": " +
               fixed(t.elapsed) + " seconds\n";
      }
    }
    out += "\nPragma once files (including heuristically applied):\n";
    out += "====================================================\n";
    for (const auto& [key, value] : include_once) out += "  " + (key ? *key : "None") + "\n";
    out += "\n";
  }

  void write_filetimes_csv(const std::string& file) {
    std::string csv = "\"Total seconds\",\"Self seconds\",\"File size\",\"File path\"\n";
    std::vector<std::pair<std::string, std::pair<double, double>>> filetimes;
    auto find = [&](const std::string& p) -> std::pair<double, double>* {
      for (auto& [k, v] : filetimes) {
        if (k == p) return &v;
      }
      return nullptr;
    };
    std::vector<std::string> currentfiles;
    for (const auto& t : include_times) {
      while (static_cast<std::size_t>(t.depth) < currentfiles.size()) currentfiles.pop_back();
      if (t.depth > static_cast<int>(currentfiles.size()) - 1) {
        if (!t.included_abspath) {
          throw CompileError("TypeError",
                             "stat: path should be string, bytes, os.PathLike or integer, not "
                             "NoneType");
        }
        currentfiles.push_back(*t.included_abspath);
      }
      const std::string p = currentfiles.back();
      if (auto* v = find(p)) {
        v->first += t.elapsed;
        v->second += t.elapsed;
      } else {
        filetimes.push_back({p, {t.elapsed, t.elapsed}});
      }
      if (t.elapsed > 0 && currentfiles.size() > 1) find(currentfiles[currentfiles.size() - 2])->second -= t.elapsed;
    }
    std::vector<std::tuple<double, double, std::string>> rows;
    for (const auto& [k, v] : filetimes) rows.emplace_back(v.first, v.second, k);
    std::sort(rows.rbegin(), rows.rend());
    for (const auto& [total, self, p] : rows) {
      std::error_code ec;
      const auto size = std::filesystem::file_size(std::filesystem::path(std::u8string(p.begin(), p.end())), ec);
      csv += fixed(total) + "," + fixed(self) + "," + std::to_string(ec ? 0 : size) + ",\"" + p + "\"\n";
    }
    std::ofstream(std::filesystem::path(std::u8string(file.begin(), file.end())), std::ios::binary) << csv;
  }

  Args args;
  std::optional<std::string> input_file;
  std::optional<std::string> number_error;
  bool bypass_ifpassthru = false;
  std::optional<std::string> potential_include_guard;
};

}  // namespace

Output preprocess(const std::string& source, const std::vector<std::string>& argv,
                  const std::optional<std::string>& filename, const pcpp::Host& host,
                  const std::string& include_dir, std::string* out_stderr) {
  std::string err;
  try {
    Args args = parse_args(argv, err);
    CompilePreprocessor pp(host, std::move(args), include_dir);
    try {
      const std::string text = pp.run(source, filename);
      err += pp.stderr_text;
      if (out_stderr) *out_stderr = err;
      return {strip_pragmas(text), err};
    } catch (...) {
      err += pp.stderr_text;
      throw;
    }
  } catch (...) {
    if (out_stderr) *out_stderr = err;
    throw;
  }
}

}  // namespace c6502::preprocessor
