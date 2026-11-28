// `c6502cpp compile`, compile.py's command line in C++: the reading of
// its arguments with compile.py's argparse (`_parse_args`, after
// `_rewrite_ttext`), and the work compile.py's `main` did after it.
// `--lex`, `--parse`, `--resolve`, `--tac`, and `-S` run the front end
// and the stage on each C input, and `-c` and a link compile and
// assemble, assemble, or read each input in order, as
// `compile._cpp_build` did, and then link. Run as a command, it writes
// to its own stdout and stderr in text mode, and reads stdin as UTF-8
// with Python's newlines.
//
// check-golden (src/golden.h) runs each job's command lines through
// `run`, so that the golden files check the command line too.
#pragma once

#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "asdl/runtime.h"

namespace c6502::compile_command {

// An exception that compile.py lets out of `main`, or that it reports
// with `_report`: its Python class, `str(e)`, and for an OSError its
// `strerror`, which `_report` prints in place of `str(e)`.
struct Raised {
  std::string type;
  std::string message;
  std::string strerror;
};

enum class Stream { kOut, kErr };

// One write of compile.py's to stdout or stderr.
struct Write {
  Stream stream;
  std::string text;
};

// What one run of compile.py's `main` gave.
struct Outcome {
  // The exit status `main` returned, or the code of the SystemExit it
  // raised (argparse's, for the help and a usage error), or nullopt
  // when it raised another exception.
  std::optional<long long> status;
  // Whether the status is a SystemExit's code.
  bool exited = false;
  std::optional<Raised> raised;
  // What it wrote, in order.
  std::vector<Write> writes;

  // Returns what it wrote to one stream.
  std::string out() const;
  std::string err() const;
};

struct Options {
  std::string include_dir;
  // The terminal's width, which argparse wraps the usage and help to.
  int columns = 80;
  // false when `-O` doesn't inline the calls of small `static`
  // functions, for a test of a function's own code.
  bool inline_calls = true;
  // Whether the IR checks are on (`C6502_CHECK_IR=1`).
  bool check_ir = false;
  // Returns what `sys.stdin.read()` returns, or throws its `Raised`.
  // Each C input `-` reads it once more, so the second read gives what
  // is left after the first, which is nothing.
  std::function<std::string()> read_stdin;
};

// Runs compile.py's `main` on the command line `argv` (the program's
// name first) in the current directory.
Outcome run(const std::vector<std::string>& argv, const Options& options);

// Runs `c6502cpp compile` with the arguments after the command, as
// compile.py's command line after its program's name, and returns the
// exit status. `self` is the path the program was started by, from
// which the command finds c6502's include/.
int command(const std::vector<std::string>& args, const std::string& self);

// Returns the include/ of the c6502 checkout the executable is in (the
// first directory up from the executable's that holds both
// cpp/CMakeLists.txt and include/stdint.h), or else of the source tree
// it was built from, with the platform's separators. `self` is the path
// the program was started by. Throws `std::runtime_error` when neither
// is a checkout.
std::string find_include_dir(const std::string& self);

// --- Helpers that check-golden shares ---

std::filesystem::path utf8_path(std::string_view s);
// Returns the path with `/` separators, as UTF-8.
std::string path_text(const std::filesystem::path& p);
// Writes `data` to the file at `path` as Python's `open(path, "wb")`
// and `write` do, and returns the errno of a file that can't be opened
// or written, or 0.
int write_bytes(const std::string& path, std::string_view data);
// Reads a UTF-8 text file as Python's `open(path, "r",
// encoding="utf-8").read()` does, with universal newlines, or throws
// its OSError or UnicodeDecodeError as a `Raised`.
std::string read_text(const std::string& path);

// Returns compile.py's `_number(text)` (decimal, `0x` hex, or `$` hex,
// with Python's `int` rules for signs, underscores, and spaces), or
// nullopt where it raises ValueError. A number that doesn't fit in 127
// bits and a sign is an InternalError, since the port holds a number in
// 128 bits. Python also takes the digits and spaces of other scripts,
// which this reads as no number.
std::optional<asdl::Int> number(std::string_view text);

}  // namespace c6502::compile_command
