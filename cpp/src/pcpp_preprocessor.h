// pcpp 1.30's `Preprocessor` and `PreprocessorHooks` (pcpp/preprocessor.py
// and pcpp/parser.py), which preprocessor.py's `CompilePreprocessor`
// extends, ported so that its output comes out byte for byte.
//
// pcpp reads its input a line at a time. A line that holds a directive
// runs it, and the other lines collect in a `chunk`, whose macros are
// expanded when a directive needs the text before it or the input ends.
// `parsegen` is a Python generator, which `write` drains a token at a
// time. Nothing pcpp does depends on how far `write` has got, except
// what is warned about before an error stops the run, so the C++
// `parsegen` hands each token to a callback at the point where Python
// yields it, and the callback is `write`'s consumer.
//
// pcpp expands macros by marking each token with the list of macros it
// came from (`expanded_from`), where most preprocessors use Prosser's
// hide sets. A token copied with `copy.copy` shares its list with the
// original, and an argument used twice in a body is the same token
// object twice, so a token can come to carry a macro's name twice, or
// carry another token's names. pcpp_token.h keeps Python's object
// semantics, so the C++ expands exactly as pcpp does. Other quirks that
// show in the output: a token from an expansion takes the line of the
// macro's use, a space is inserted after a function-like macro's
// expansion when an identifier follows it, `#` escapes every backslash
// and double quote of the argument, `##` lexes the joined text again,
// and `, ## __VA_ARGS__` drops the comma when the variadic argument is
// empty.
//
// pcpp reports a problem in the input through `on_error`, which prints
// `<file>:<line> error: <message>` to stderr and counts it, and doesn't
// stop the run. The C++ appends those lines to `stderr_text`. A Python
// exception that escapes pcpp, such as the IndexError of `#define` with
// no name, is thrown as a CompileError of the Python type with Python's
// message.
#pragma once

#include <cstdint>
#include <ctime>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "pcpp_evaluator.h"
#include "pcpp_lexer.h"
#include "pcpp_text.h"
#include "pcpp_token.h"
#include "py_regex.h"

namespace c6502::pcpp {

// What `OutputDirective` asks pcpp to do with the directive it aborts.
enum class Action { IgnoreAndPassThrough = 0, IgnoreAndRemove = 1 };

// pcpp's `OutputDirective`, which a hook throws to abort a directive and
// pass it through to the output or remove it.
struct OutputDirective {
  Action action;
};

// pcpp's `Macro`, after `macro_prescan`: its body, its parameters (none
// for an object-like macro), and the places in the body where the
// arguments go.
struct Macro {
  std::string name;
  Tokens value;
  std::optional<std::vector<std::string>> arglist;
  bool variadic = false;
  std::string vararg;
  std::optional<std::string> source;
  int lineno = 0;
  // `('t' or 'e', argument number, position)`, sorted by position from
  // the last to the first.
  struct Patch {
    char type;
    std::size_t argnum;
    std::size_t pos;
  };
  std::vector<Patch> patch;
  std::vector<std::pair<std::size_t, std::size_t>> str_patch;
  std::vector<std::size_t> var_comma_patch;
};

using MacroPtr = std::shared_ptr<Macro>;

// What pcpp reads from the machine it runs on: the rules of its paths
// and the current directory, the encoding Python opens a header with
// (the locale's, cp1252 on Windows and UTF-8 on Linux), and the bytes of
// a file, or nullopt where Python's `open` raises an OSError (no such
// file, or a directory).
struct Host {
  Paths paths;
  std::string encoding;
  std::function<std::optional<std::string>(const std::string& path)> read_file;
  // Returns whether `path` is a directory, as `os.path.isdir` does.
  std::function<bool(const std::string& path)> isdir;
  // The local time that `__DATE__` and `__TIME__` give.
  std::function<std::tm()> localtime;
};

// The host of this machine: its current directory, its files, and its
// clock.
Host native_host();

// The time pcpp takes to include a file, for `--time` and `--filetimes`.
struct FileInclusionTime {
  std::optional<std::string> included_path;
  std::optional<std::string> included_abspath;
  int depth = 0;
  double elapsed = 0.0;
};

// The result of `evalexpr`: the value of the expression, and the
// expression's tokens when pcpp passes the directive through because
// part of it couldn't be evaluated (`partial_expansion`).
struct EvalOutcome {
  EvalResult result;
  std::optional<Tokens> rewritten;
};

class Preprocessor {
 public:
  explicit Preprocessor(Host host);
  virtual ~Preprocessor() = default;
  Preprocessor(const Preprocessor&) = delete;
  Preprocessor& operator=(const Preprocessor&) = delete;

  // The hooks of `PreprocessorHooks`, which `CompilePreprocessor`
  // overrides.
  virtual void on_error(const std::optional<std::string>& file, int line,
                        const std::string& msg);
  // Returns the text of the file at `path`, or nullopt for an OSError.
  virtual std::optional<std::string> on_file_open(bool is_system_include, const std::string& path);
  // Throws OutputDirective, or returns a path to search.
  virtual std::string on_include_not_found(bool is_malformed, bool is_system_include,
                                           const std::string& curdir,
                                           const std::string& includepath);
  // Returns true or false, or nullopt for Python's None.
  virtual std::optional<bool> on_unknown_macro_in_defined_expr(const TokenPtr& tok);
  virtual std::optional<std::int64_t> on_unknown_macro_in_expr(const std::string& ident);
  // Returns whether the hook returns the callable that gives 0, where
  // false is Python's None.
  virtual bool on_unknown_macro_function_in_expr(const std::string& ident);
  // Returns true, or nullopt for None, or throws OutputDirective.
  virtual std::optional<bool> on_directive_handle(const TokenPtr& directive, const Tokens& toks,
                                                  bool ifpassthru, const Tokens& precedingtoks);
  virtual std::optional<bool> on_directive_unknown(const TokenPtr& directive, const Tokens& toks,
                                                   bool ifpassthru, const Tokens& precedingtoks);
  virtual void on_potential_include_guard(const std::optional<std::string>& macro);
  virtual bool on_comment(const TokenPtr& tok);

  // Runs `self.evaluator(tokens, functions=..., identifiers=...).value()`.
  virtual EvalResult call_evaluator(const Tokens& tokens, const FunctionHook& functions,
                                    const IdentifierHook& identifiers);
  virtual EvalOutcome evalexpr(Tokens& tokens);
  // Called for each token `write` takes, where `Preprocessor.token`
  // returns it.
  virtual void on_token(const TokenPtr&) {}

  Tokens tokenize(const std::string& text);
  void add_path(const std::string& path);
  void define(const std::string& text);
  void define(const Tokens& tokens);
  void undef(const std::string& text);
  void undef(const Tokens& tokens);
  // `parse(input, source)` followed by `write(out)`, which returns the
  // text.
  std::string parse_and_write(const std::string& input, const std::optional<std::string>& source);

  // The state of pcpp that preprocessor.py sets and reads.
  Host host;
  Lexer lexer;
  std::map<std::string, MacroPtr> macros;
  std::vector<std::string> path;
  std::vector<std::string> temp_path;
  struct Rewrite {
    std::string prefix;
    std::string replacement;
  };
  std::vector<Rewrite> rewrite_paths;
  // The pattern of `--passthru-includes`, compiled with Python's `re`
  // semantics (py_regex.h).
  std::shared_ptr<py_regex::Regex> passthru_includes;
  // `include_once`, keyed by a file's absolute path, in the order the
  // keys were added.
  std::vector<std::pair<std::optional<std::string>, std::optional<std::string>>> include_once;
  int include_depth = 0;
  std::vector<FileInclusionTime> include_times;
  int return_code = 0;
  bool auto_pragma_once_enabled = true;
  std::optional<std::string> line_directive = "#line";
  int compress = 0;
  std::optional<std::string> assume_encoding;
  bool expand_linemacro = true;
  bool expand_filemacro = true;
  bool expand_countermacro = true;
  int linemacro = 0;
  int linemacrodepth = 0;
  int countermacro = 0;
  // The depth of the nested calls of `expand_macros`, which stands for
  // the depth of Python's stack (`kMaxExpansionDepth`).
  int expansion_depth = 0;
  std::optional<std::string> source;
  TokenPtr lastdirective;
  // The lines pcpp has printed to stderr.
  std::string stderr_text;

 protected:
  using Emit = std::function<void(const TokenPtr&)>;

  bool include_once_has(const std::optional<std::string>& key) const;
  void include_once_set(const std::optional<std::string>& key,
                        const std::optional<std::string>& value);

 private:
  std::vector<std::shared_ptr<Tokens>> group_lines(const std::string& input,
                                                   const std::optional<std::string>& abssource);
  Tokens& tokenstrip(Tokens& tokens);
  struct Collected {
    std::size_t count;
    std::vector<Tokens> args;
    std::vector<std::size_t> positions;
  };
  Collected collect_args(const Tokens& tokenlist, bool ignore_errors);
  void macro_prescan(Macro& macro);
  Tokens macro_expand_args(const Macro& macro, std::vector<Tokens>& args);
  Tokens& expand_macros(Tokens& tokens, const std::vector<std::string>& expanding_from = {});
  void parsegen(const std::string& input, std::optional<std::string> source,
                const std::optional<std::string>& abssource, const Emit& emit);
  void include(Tokens tokens, const Tokens& original_line, const Emit& emit);
};

}  // namespace c6502::pcpp
