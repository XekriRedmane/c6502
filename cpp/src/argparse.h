// Python 3.11's argparse, as far as the command lines of nm.py and
// objdump.py use it: options that store true or append one argument,
// one positional argument of one or more values, the help action, the
// usage and help text with HelpFormatter's wrapping, and the errors,
// word for word. A C++ tool that takes over a Python one's command line
// reads it with this, so that a bad command line gives the same message
// and exit status.
//
// Python wraps the usage and help to the terminal's width
// (`shutil.get_terminal_size`), which `terminal_columns` reads the same
// way. textwrap splits words at hyphens, which this doesn't, so a help
// text with a hyphen inside a word is an InternalError (`add_*` checks
// each one).
#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

namespace c6502::argparse {

// What argparse does with an option or a positional argument.
enum class ActionKind {
  kStoreTrue,
  kAppend,
  kHelp,
  // A positional argument with `nargs="+"`.
  kOneOrMore,
};

struct Action {
  ActionKind kind;
  std::vector<std::string> option_strings;
  std::string dest;
  std::string help;
};

// The parsed arguments: each store-true option's value and each
// append option's or positional argument's list, by `dest`. An append
// option that wasn't given is missing from `lists`, where Python's
// default is None.
struct Namespace {
  std::map<std::string, bool> flags;
  std::map<std::string, std::vector<std::string>> lists;

  bool flag(const std::string& dest) const;
  const std::vector<std::string>* list(const std::string& dest) const;
};

// What Python's `parser.exit` and `parser.error` leave: the exit
// status and what they wrote to stdout (the help) and stderr (the usage
// and the error).
struct Exit {
  int status = 0;
  std::string out;
  std::string err;
};

class Parser {
 public:
  // `add_help` adds `-h, --help` first, as Python does.
  Parser(std::string prog, std::string description, bool add_help = true);

  void add_store_true(std::vector<std::string> option_strings, std::string help = "");
  void add_append(std::vector<std::string> option_strings, std::string dest,
                  std::string help = "");
  void add_help(std::vector<std::string> option_strings, std::string help);
  void add_positional(std::string dest, std::string help = "");

  // Parses `args` (the command line after the program's name) as
  // `parse_args` does, with the usage and help wrapped for a terminal
  // of `columns` columns. Throws `Exit` for the help, for a usage error,
  // and for arguments left over.
  Namespace parse_args(const std::vector<std::string>& args, int columns) const;

  // Returns `format_usage()` and `format_help()` for a terminal of
  // `columns` columns.
  std::string format_usage(int columns) const;
  std::string format_help(int columns) const;

 private:
  struct Tuple {
    const Action* action;
    std::string option_string;
    std::optional<std::string> explicit_arg;
  };

  const Action* find(const std::string& option_string) const;
  std::optional<Tuple> parse_optional(const std::string& arg, int columns) const;
  [[noreturn]] void error(const std::string& message, int columns) const;
  std::string actions_usage(bool optionals, bool positionals) const;
  std::string format_action(const Action& a, int width, int max_help_position,
                            int action_max_length) const;
  void add(Action a);

  std::string prog_;
  std::string description_;
  std::vector<Action> actions_;
};

// Returns `shutil.get_terminal_size().columns`: the `COLUMNS`
// environment variable when it's a positive number, else the width of
// the terminal stdout writes to, else 80.
int terminal_columns();

// Returns `str(pathlib.Path(path))` on this platform: the path with
// `.` parts and repeated separators dropped, and on Windows with `\`
// for each separator, as Python's tools name a file in their messages.
std::string path_str(const std::string& path);

// Returns `pathlib.Path(path).name` on this platform: the last part of
// the path once `.` parts and empty ones are dropped, or the empty
// string when none is left. On Windows a drive (`C:`) is dropped first,
// and `\` separates parts as `/` does. A UNC path's server and share
// count as parts here, where Python takes them as the drive.
std::string path_name(const std::string& path);

}  // namespace c6502::argparse
