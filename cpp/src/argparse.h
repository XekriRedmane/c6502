// Python 3.11's argparse, as far as the command lines of compile.py,
// nm.py, and objdump.py use it: options that store true, store a
// constant, store a value, or append one, with `metavar` and `type=`;
// one positional argument of one or more values; mutually exclusive
// groups; the help action; `parse_args` and `parse_known_args`; the
// usage and help text with HelpFormatter's wrapping; and the errors,
// word for word. A C++ tool that takes over a Python one's command line
// reads it with this, so that a bad command line gives the same message
// and exit status.
//
// Python wraps the usage and help to the terminal's width
// (`shutil.get_terminal_size`), which `terminal_columns` reads the same
// way, and wraps each help text with textwrap, whose splitting of words
// at their hyphens `wrap_text` ports. The wrapping counts bytes where
// Python counts characters, which is the same for the ASCII text of the
// tools' help.
#pragma once

#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace c6502::argparse {

// What argparse does with an option or a positional argument.
enum class ActionKind {
  kStoreTrue,
  // `action="store_const"`, which stores `Action::constant`.
  kStoreConst,
  // The default action, which stores the one argument it takes.
  kStore,
  kAppend,
  kHelp,
  // A positional argument with `nargs="+"`.
  kOneOrMore,
};

// The check of a `type=` function: whether the function takes the text,
// where Python's raises ValueError or TypeError for it. The value stays
// a string in the Namespace, and the caller converts it. A check may
// throw `ArgumentTypeError` instead, whose message the error gives in
// place of "invalid <type> value: ...", as Python's argparse does.
using TypeCheck = std::function<bool(const std::string&)>;

struct ArgumentTypeError {
  std::string message;
};

struct Action {
  ActionKind kind;
  std::vector<std::string> option_strings;
  std::string dest;
  std::string help;
  // The name in the usage and help of the value an option takes, or
  // empty for the default, `dest` in capitals.
  std::string metavar;
  // The value a `kStoreConst` action stores.
  std::string constant;
  // The `type=` function's `__name__`, which its error names, and its
  // check, or none.
  std::string type_name;
  TypeCheck type;
  // The index of the mutually exclusive group the action is in, or -1.
  int group = -1;
};

// The parsed arguments: each store-true option's value, each append
// option's or positional argument's list, and each stored value, by
// `dest`. An append option or a stored value that wasn't given is
// missing from `lists` or `values`, where Python's default is None.
struct Namespace {
  std::map<std::string, bool> flags;
  std::map<std::string, std::vector<std::string>> lists;
  std::map<std::string, std::string> values;

  bool flag(const std::string& dest) const;
  const std::vector<std::string>* list(const std::string& dest) const;
  const std::string* value(const std::string& dest) const;
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

  // Without `dest`, the dest is the first long option's name, else the
  // first short one's, with `-` as `_`, as Python derives it.
  void add_store_true(std::vector<std::string> option_strings, std::string help = "",
                      std::string dest = "");
  void add_store_const(std::vector<std::string> option_strings, std::string dest,
                       std::string constant, std::string help = "", int group = -1);
  void add_store(std::vector<std::string> option_strings, std::string dest, std::string help = "",
                 std::string metavar = "", std::string type_name = "", TypeCheck type = {});
  void add_append(std::vector<std::string> option_strings, std::string dest,
                  std::string help = "", std::string metavar = "");
  void add_help(std::vector<std::string> option_strings, std::string help);
  void add_positional(std::string dest, std::string help = "");

  // Returns the index of a new mutually exclusive group, which isn't
  // required, as `add_mutually_exclusive_group()` makes one.
  int add_mutually_exclusive_group();

  // Parses `args` (the command line after the program's name) as
  // `parse_args` does, with the usage and help wrapped for a terminal
  // of `columns` columns. Throws `Exit` for the help, for a usage error,
  // and for arguments left over.
  Namespace parse_args(const std::vector<std::string>& args, int columns) const;

  // Parses `args` as `parse_known_args` does, and puts the arguments it
  // doesn't take in `extras`, in their order. Throws `Exit` for the help
  // and for a usage error.
  Namespace parse_known_args(const std::vector<std::string>& args, int columns,
                             std::vector<std::string>& extras) const;

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
  // The actions of each mutually exclusive group, in the order they
  // were added, by index into `actions_`.
  std::vector<std::vector<std::size_t>> groups_;
};

// Returns `textwrap.wrap(text, width)` for text whose whitespace argparse
// has made one space between words: the lines, each at most `width`
// bytes long except where a word can't be broken, with textwrap's
// splitting of a word after a hyphen between letters and of a word too
// long for a line after its last hyphen that fits.
std::vector<std::string> wrap_text(const std::string& text, int width);

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
