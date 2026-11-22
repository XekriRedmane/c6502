// preprocessor.py: c6502's C preprocessor, pcpp 1.30 configured as pcpp's
// own command line (pcpp/pcmd.py) configures it, with the hooks that
// make it follow C23.
//
// `preprocess(source, argv, filename)` takes the flags of pcpp's command
// line, except `-o` and the inputs, and `--line-directive` suppresses
// `#line` markers unless it's given a form, since the lexer after it
// doesn't read them. Unknown flags are ignored with a NOTE on stderr, as
// pcmd.py ignores them. The macros `__PCPP_VERSION__`,
// `__PCPP_ALWAYS_FALSE__`, and `__PCPP_ALWAYS_TRUE__` are predefined, as
// pcmd.py predefines them.
//
// Several things follow C23 rather than pcpp. In an `#if` or `#elif`,
// an identifier `true` left after macro replacement is 1 and `false` is
// 0, whatever the passthru flags (C23 §6.10.2p13). Each `_Bool` in the
// output gets a warning (`kAltBoolWarning`, through
// `diagnostics::warn`) that names its line, since C23 makes `_Bool` an
// obsolescent spelling of `bool`. The lexer reads C23's preprocessing
// number (pp_numbers), so `1'000` and `0b1010` are single tokens, and in
// an `#if` each number is read with `pp_numbers::parse_integer` before
// pcpp's evaluator sees it. That gives a binary constant its value,
// ignores digit separators, accepts the `ll` suffixes, and makes a
// hexadecimal, octal, or binary constant above `INTMAX_MAX` unsigned,
// since its type list ends in `uintmax_t` (§6.10.2p13, §6.4.4.2p6). A
// number there that isn't a valid integer constant raises
// PreprocessorError, and so does a `#pragma c6502` line, since c6502's
// directives are C23 statement attributes now. Every other `#pragma`
// line becomes a blank line.
//
// pcpp's `--debug` log isn't written: the flag is read and creates
// nothing. `--time` and `--filetimes` give the report with the times the
// C++ measures. `--passthru-includes` compiles and matches its pattern
// with py_regex, which follows Python's `re`, and a pattern py_regex
// doesn't take throws CompileError("NotImplementedError", ...).
#pragma once

#include <optional>
#include <string>
#include <vector>

#include "pcpp_preprocessor.h"

namespace c6502::preprocessor {

// The warning for each `_Bool` the preprocessor writes out. C23 §6.4.1
// lists `_Bool` as an alternative spelling of the keyword `bool`, and
// its footnote 62 makes the alternative spellings obsolescent.
extern const char* const kAltBoolWarning;

// The flags of `parse_args`, with argparse's defaults.
struct Args {
  std::optional<std::vector<std::string>> defines;
  std::optional<std::vector<std::string>> undefines;
  std::optional<std::vector<std::string>> nevers;
  std::optional<std::vector<std::string>> includes;
  bool passthru_defines = false;
  bool passthru_unfound_includes = false;
  bool passthru_undefined_exprs = false;
  bool passthru_comments = false;
  bool passthru_magic_macros = false;
  std::optional<std::string> passthru_includes;
  bool auto_pragma_once_disabled = false;
  std::optional<std::string> line_directive;
  bool debug = false;
  bool time = false;
  std::optional<std::string> filetimes;
  bool compress = false;
  std::optional<std::string> assume_input_encoding;
  std::optional<std::string> output_encoding;
  bool write_bom = false;
};

// Reads pcpp-style flags as argparse's `parse_known_args` does, and
// appends a NOTE to `err` for each argument it doesn't know. A flag that
// argparse rejects writes argparse's usage and message to `err` and
// throws CompileError of type "SystemExit", as `sys.exit(2)` does.
Args parse_args(const std::vector<std::string>& argv, std::string& err);

// The output of `preprocess`, and what it wrote to stderr: the NOTEs of
// `parse_args`, pcpp's errors, `#error` and `#warning` lines, and the
// `--time` report.
struct Output {
  std::string text;
  std::string stderr_text;
};

// Preprocesses `source`, as `preprocessor.preprocess(source, argv,
// filename)` does. `filename` is the file `source` came from, if any:
// a quoted `#include "x.h"` looks in its directory first (C99
// §6.10.2.3), and messages name it. `include_dir` is the directory of
// c6502's own headers (preprocessor.py's `_BUNDLED_INCLUDE_DIR`), which
// is searched after the `-I` paths when it exists. Throws
// CompileError("PreprocessorError", ...) for a `#pragma c6502` line or an
// invalid number in an `#if`, and CompileError of the Python type for
// any other exception Python raises. The `_Bool` warnings go to
// `diagnostics::warn`, and `out_stderr` gets the text of `stderr_text`
// even when this throws.
Output preprocess(const std::string& source, const std::vector<std::string>& argv,
                  const std::optional<std::string>& filename, const pcpp::Host& host,
                  const std::string& include_dir, std::string* out_stderr = nullptr);

}  // namespace c6502::preprocessor
