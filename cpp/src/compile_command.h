// `c6502cpp compile`, compile.py's command line in C++: the reading of
// its arguments with compile.py's argparse (`_parse_args`, after
// `_rewrite_ttext`), and the work compile.py's `main` did after it, in
// the same process. `--lex`,
// `--parse`, `--resolve`, `--tac`, and `-S` compile each C input
// through the back end's request of `cpp_backend.compile_source` (or
// `stage_source`), and `-c` and a link send their inputs in one request
// of `cpp_backend.build`, as `compile._cpp_build` does, and replay the
// response input by input. Every message says `compile.py:`, as the
// tests and the golden files hold it until step 9.
//
// compile.py's `main`, a shim since step 8, runs this command
// through `cpp_backend.run_tool` with its command line, and the two
// sides talk in frames (`C6502CPP_FRAMES=1`), so that Python writes
// what the command writes, in its order, through its own streams,
// reads stdin for it as `sys.stdin.read()` does, and raises the
// exception that would have left compile.py's `main`. Run by hand, the
// command writes to its own stdout and stderr in text mode, and reads
// stdin as UTF-8 with Python's newlines, which gives Python's bytes
// for ASCII text (Python on Windows reads and writes a pipe in the
// ANSI code page).
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
// with `_report`: its Python class (with its module in front for a
// class such as `struct.error`, whose name a builtin has too), `str(e)`,
// and for an OSError its `strerror`, which `_report` prints in place of
// `str(e)`.
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
  // The directory of c6502's own headers, as cpp_backend.py's
  // `_INCLUDE_DIR` gives it.
  std::string include_dir;
  // The terminal's width, which argparse wraps the usage and help to.
  int columns = 80;
  // false when `-O` doesn't inline the calls of small `static`
  // functions (tests/no_inlining.py, `compile._inlining`).
  bool inline_calls = true;
  // Whether the IR checks are on (`cpp_backend._ir_check_enabled`).
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
// which the command finds c6502's include/ when Python doesn't give it.
int command(const std::vector<std::string>& args, const std::string& self);

// Returns the include/ of the c6502 checkout the executable is in (the
// first directory up from the executable's that is one), or else of the
// source tree it was built from, with the platform's separators, as
// cpp_backend.py's `_INCLUDE_DIR` spells it. `self` is the path the
// program was started by. Throws `std::runtime_error` when neither is a
// checkout.
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

// One record of a response of `c6502cpp backend` (src/backend.h): its
// key, the rest of its line, and the section that follows it.
struct Record {
  std::string key;
  std::string value;
  std::string body;
};

std::vector<Record> records(const std::string& data);

// Returns the JSON of a list of strings, as Python's `json.dumps`
// writes it.
std::string string_list_json(const std::vector<std::string>& v);

// Returns compile.py's `_number(text)` (decimal, `0x` hex, or `$` hex,
// with Python's `int` rules for signs, underscores, and spaces), or
// nullopt where it raises ValueError. A number that doesn't fit in 127
// bits and a sign is an InternalError, since the port holds a number in
// 128 bits. Python also takes the digits and spaces of other scripts,
// which this reads as no number.
std::optional<asdl::Int> number(std::string_view text);

}  // namespace c6502::compile_command
