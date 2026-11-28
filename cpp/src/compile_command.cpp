#include "compile_command.h"

#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>
#include <set>
#include <stdexcept>
#include <utility>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <fcntl.h>
#include <io.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "argparse.h"
#include "backend.h"
#include "errors.h"
#include "json.h"
#include "pcpp_text.h"
#include "processes.h"
#include "pyrepr.h"

namespace c6502::compile_command {

namespace fs = std::filesystem;

std::string Outcome::out() const {
  std::string s;
  for (const Write& w : writes) {
    if (w.stream == Stream::kOut) s += w.text;
  }
  return s;
}

std::string Outcome::err() const {
  std::string s;
  for (const Write& w : writes) {
    if (w.stream == Stream::kErr) s += w.text;
  }
  return s;
}

fs::path utf8_path(std::string_view s) { return fs::path(std::u8string(s.begin(), s.end())); }

std::string path_text(const fs::path& p) {
  const std::u8string s = p.generic_u8string();
  return std::string(s.begin(), s.end());
}

namespace {

// --- Files ---

// Returns the C library's message for `error`, which Python's OSError
// takes as its strerror.
std::string error_message(int error) {
#ifdef _WIN32
  char buf[256];
  strerror_s(buf, sizeof buf, error);
  return buf;
#else
  return std::strerror(error);
#endif
}

// Returns the class of the OSError that Python raises for `error`, as
// `OSError(error, ...)` picks the subclass by errno.
std::string os_error_type(int error) {
  switch (error) {
    case ENOENT:
      return "FileNotFoundError";
    case EACCES:
    case EPERM:
      return "PermissionError";
    case EISDIR:
      return "IsADirectoryError";
    case ENOTDIR:
      return "NotADirectoryError";
    case EEXIST:
      return "FileExistsError";
    case EINTR:
      return "InterruptedError";
    case EPIPE:
      return "BrokenPipeError";
    case ETIMEDOUT:
      return "TimeoutError";
    default:
      return "OSError";
  }
}

// Returns the OSError Python raises for `error` on the file `filename`,
// whose `str()` is `[Errno <n>] <strerror>: '<filename>'`.
Raised os_error(int error, const std::string& filename) {
  const std::string reason = error_message(error);
  return Raised{os_error_type(error),
                "[Errno " + std::to_string(error) + "] " + reason + ": " + pyrepr::string(filename),
                reason};
}

// Reads the file at `path`, relative to the current directory, as
// Python's `open(path, "rb").read()` does, or throws its OSError.
std::string read_bytes(const std::string& path) {
  const fs::path p = utf8_path(path);
  std::error_code ec;
  if (fs::is_directory(p, ec)) {
#ifdef _WIN32
    throw os_error(EACCES, path);
#else
    throw os_error(EISDIR, path);
#endif
  }
  errno = 0;
#ifdef _WIN32
  std::FILE* f = nullptr;
  if (const errno_t opened = _wfopen_s(&f, p.c_str(), L"rb"); opened != 0) {
    throw os_error(opened, path);
  }
#else
  std::FILE* f = std::fopen(p.c_str(), "rb");
#endif
  if (f == nullptr) throw os_error(errno != 0 ? errno : EIO, path);
  std::string data;
  char buf[65536];
  std::size_t n = 0;
  while ((n = std::fread(buf, 1, sizeof buf, f)) > 0) data.append(buf, n);
  std::fclose(f);
  return data;
}

// --- Numbers ---

// Python's whitespace among the ASCII characters, which `str.strip` and
// `int` take off the ends.
bool py_space(char c) {
  return c == ' ' || (c >= '\t' && c <= '\r') || (c >= '\x1c' && c <= '\x1f');
}

std::string_view py_strip(std::string_view t) {
  while (!t.empty() && py_space(t.front())) t.remove_prefix(1);
  while (!t.empty() && py_space(t.back())) t.remove_suffix(1);
  return t;
}

// Returns Python's `int(text, base)` for `base` 0 or 16, or nullopt
// where it raises ValueError, as `PyLong_FromString` reads the text: a
// sign, then a prefix (`0x`, `0o`, or `0b` for base 0, which picks the
// base, or an optional `0x` for base 16) that one underscore may
// follow, then digits with single underscores between them, and spaces
// around it all. In base 0 a decimal number can't start with 0 unless
// all its digits are 0.
std::optional<asdl::Int> py_int(std::string_view t, int base) {
  t = py_strip(t);
  bool negative = false;
  if (!t.empty() && (t[0] == '+' || t[0] == '-')) {
    negative = t[0] == '-';
    t.remove_prefix(1);
  }
  const auto lower_at = [&](std::size_t k) {
    return k < t.size() ? static_cast<char>(std::tolower(static_cast<unsigned char>(t[k]))) : '\0';
  };
  bool error_if_nonzero = false;
  if (base == 0) {
    if (t.empty() || t[0] != '0') {
      base = 10;
    } else if (lower_at(1) == 'x') {
      base = 16;
    } else if (lower_at(1) == 'o') {
      base = 8;
    } else if (lower_at(1) == 'b') {
      base = 2;
    } else {
      error_if_nonzero = true;
      base = 10;
    }
  }
  if (!t.empty() && t[0] == '0' &&
      ((base == 16 && lower_at(1) == 'x') || (base == 8 && lower_at(1) == 'o') ||
       (base == 2 && lower_at(1) == 'b'))) {
    t.remove_prefix(2);
    if (!t.empty() && t[0] == '_') t.remove_prefix(1);
  }
  if (t.empty() || t[0] == '_') return std::nullopt;
  // The largest magnitude the port holds, 2**127 - 1.
  const asdl::Int max = ((asdl::Int(1) << 126) - 1) * 2 + 1;
  asdl::Int v = 0;
  bool nonzero = false;
  char prev = 0;
  for (char c : t) {
    if (c == '_') {
      if (prev == '_') return std::nullopt;
      prev = c;
      continue;
    }
    int d = 99;
    if (c >= '0' && c <= '9') d = c - '0';
    if (c >= 'a' && c <= 'z') d = c - 'a' + 10;
    if (c >= 'A' && c <= 'Z') d = c - 'A' + 10;
    if (d >= base) return std::nullopt;
    if (v > (max - asdl::Int(d)) / asdl::Int(base)) {
      throw InternalError("compile.py's number " + pyrepr::string(t) +
                          " is wider than the 128 bits the C++ port holds a number in");
    }
    v = v * asdl::Int(base) + asdl::Int(d);
    if (d != 0) nonzero = true;
    prev = c;
  }
  if (prev == '_') return std::nullopt;
  if (error_if_nonzero && nonzero) return std::nullopt;
  return negative ? -v : v;
}

// --- compile.py's command line ---

// What compile.py's `_parse_args` gives for a command line: the
// arguments it reads and, in their order, the ones it leaves for the
// preprocessor (argparse's `parse_known_args`).
struct Args {
  std::vector<std::string> inputs;
  std::optional<std::string> output;
  // "lex", "parse", "resolve", "tac", "codegen", "object", or "" for a
  // link.
  std::string stage;
  bool optimize = false;
  std::optional<std::string> script;
  std::optional<std::string> origin;
  std::vector<std::string> defsyms;
  std::vector<std::string> pcpp_args;
};

// Returns `argv` with `-Ttext=ADDR` and `-Ttext ADDR` spelled `--origin
// ADDR`, as compile.py's `_rewrite_ttext` does, since argparse would read
// them as `-T` with the value `text=ADDR`.
std::vector<std::string> rewrite_ttext(const std::vector<std::string>& argv) {
  std::vector<std::string> out;
  for (std::size_t i = 0; i < argv.size(); ++i) {
    const std::string& a = argv[i];
    if (a.rfind("-Ttext=", 0) == 0) {
      out.push_back("--origin");
      out.push_back(a.substr(7));
    } else if (a == "-Ttext") {
      out.push_back("--origin");
      out.push_back(i + 1 < argv.size() ? argv[++i] : "");
    } else {
      out.push_back(a);
    }
  }
  return out;
}

// The help of `-O`, as compile.py's string literals join it.
constexpr const char* kOptimizeHelp =
    "run the optimizer pipeline: TAC-level fixed-point opts (constant folding, strength "
    "reduction, comparison-against-zero / jump fold, UCE, copy propagation, dead-store "
    "elimination), then asm-level SSA round-trip (byte-granular forward + backward copy-prop, "
    "byte-DCE, byte-granular regalloc), then late prologue / epilogue synthesis. Also enables "
    "AST-level loop unrolling for `[[c6502::unroll]]` loops and the `[[c6502::zp_abi]]` "
    "calling-convention optimization. Applies to --resolve, --tac, -S, and -c, and to C "
    "sources given to a link.";

// Returns compile.py's argparse parser (`_parse_args`), action for
// action in its order.
argparse::Parser compile_parser() {
  argparse::Parser ap("compile.py", "Compile C for the 6502, assemble, and link.");
  ap.add_positional("inputs", "C sources (- for stdin), .asm files, and .o objects");
  ap.add_store({"-o"}, "output", "output file, or - for stdout");
  const int stages = ap.add_mutually_exclusive_group();
  ap.add_store_const({"--lex"}, "stage", "lex", "stop after tokenization", stages);
  ap.add_store_const({"--parse"}, "stage", "parse", "stop after parsing", stages);
  ap.add_store_const({"--resolve"}, "stage", "resolve",
                     "stop after variable + label resolution + loop labeling", stages);
  ap.add_store_const({"--tac"}, "stage", "tac", "stop after TAC translation", stages);
  ap.add_store_const({"-S"}, "stage", "codegen", "emit 6502 assembly", stages);
  ap.add_store_const({"-c"}, "stage", "object", "compile or assemble to an object; don't link",
                     stages);
  ap.add_store_true({"-O"}, kOptimizeHelp, "optimize");
  ap.add_store({"-T"}, "script",
               "linker script (a GNU ld subset; passes/link_script.py) that places each section "
               "at an address");
  ap.add_store({"--origin"}, "origin",
               "where .text starts in a link without a script (default $0800; -Ttext=ADDR also "
               "sets it)",
               "ADDR", "_number", [](const std::string& s) { return number(s).has_value(); });
  ap.add_append({"--defsym"}, "defsym", "define NAME as the address VALUE when linking",
                "NAME=VALUE");
  return ap;
}

// Reads compile.py's command line `argv` (the program's name first) as
// `_parse_args` does, or throws argparse's `Exit`.
Args parse_args(const std::vector<std::string>& argv, int columns) {
  const std::vector<std::string> rewritten = rewrite_ttext(argv);
  const std::vector<std::string> rest(rewritten.begin() + (rewritten.empty() ? 0 : 1),
                                      rewritten.end());
  Args args;
  const argparse::Namespace ns = compile_parser().parse_known_args(rest, columns, args.pcpp_args);
  args.inputs = *ns.list("inputs");
  if (const std::string* v = ns.value("output")) args.output = *v;
  if (const std::string* v = ns.value("stage")) args.stage = *v;
  args.optimize = ns.flag("optimize");
  if (const std::string* v = ns.value("script")) args.script = *v;
  if (const std::string* v = ns.value("origin")) args.origin = *v;
  if (const auto* v = ns.list("defsym")) args.defsyms = *v;
  return args;
}

// compile.py's `_kind`: "object" for `.o`, "asm" for `.asm` or `.s`,
// and "c" for anything else.
std::string kind_of(const std::string& path) {
  std::string lower = path;
  std::transform(lower.begin(), lower.end(), lower.begin(),
                 [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  const auto ends = [&](std::string_view s) {
    return lower.size() >= s.size() && lower.compare(lower.size() - s.size(), s.size(), s) == 0;
  };
  if (ends(".o")) return "object";
  if (ends(".asm") || ends(".s")) return "asm";
  return "c";
}

// Python's `Path(path).name`, the last component.
std::string name_of(const std::string& path) {
#ifdef _WIN32
  const std::size_t slash = path.find_last_of("/\\");
#else
  const std::size_t slash = path.find_last_of('/');
#endif
  return slash == std::string::npos ? path : path.substr(slash + 1);
}

// compile.py's `_stem`: the file name without its suffix, as
// `Path(path).stem` gives it, or "stdin" for `-`.
std::string stem_of(const std::string& path) {
  if (path == "-") return "stdin";
  const std::string name = name_of(path);
  const std::size_t dot = name.rfind('.');
  if (dot != std::string::npos && dot > 0 && dot + 1 < name.size()) return name.substr(0, dot);
  return name;
}

// The Python classes compile.py's `_source_errors` catches, which
// report a problem with the program, with their subclasses that the
// C++ side reports by name.
bool is_source_error(std::string_view type) {
  static const std::set<std::string, std::less<>> kTypes = {
      "LexError", "ParserError", "PreprocessorError", "LarkError", "UnexpectedInput",
      "UnexpectedCharacters", "UnexpectedToken", "UnexpectedEOF", "ParseError",
      "IdentifierResolutionError", "LabelResolutionError", "LoopLabelingError",
      "UnrollError", "ConstantExpressionError", "TypeCheckError", "RuntimeLayoutError",
      "ZpGlobalError", "AbiSelectionError", "RegallocError", "ZpSlotAllocationError",
      "ZpLocalAllocationError", "FrameSizeError", "RecursionSaveError",
      "NotImplementedError"};
  return kTypes.count(type) != 0;
}

// The classes of `_read_errors`: OSError and its subclasses, and
// UnicodeDecodeError.
bool is_read_error(std::string_view type) {
  static const std::set<std::string, std::less<>> kTypes = {
      "OSError", "FileNotFoundError", "PermissionError", "IsADirectoryError",
      "NotADirectoryError", "FileExistsError", "InterruptedError", "BrokenPipeError",
      "TimeoutError", "BlockingIOError", "ChildProcessError", "ConnectionError",
      "ProcessLookupError", "UnicodeDecodeError"};
  return kTypes.count(type) != 0;
}

// The classes of `_input_errors`.
bool is_input_error(std::string_view type) {
  return is_source_error(type) || is_read_error(type) || type == "AssemblerError" ||
         type == "ElfError";
}

// The classes a link reports as `compile.py: link error: <message>`.
bool is_link_error(std::string_view type) {
  return type == "LinkError" || type == "ZpSlotAllocationError" ||
         type == "ZpLocalAllocationError";
}

// Returns the exception `cpp_backend._exception` makes of the C++
// side's error.
Raised from_backend(const json::Value& error) {
  return Raised{error.get("type")->string, error.get("message")->string, ""};
}

std::string bool_json(bool b) { return b ? "true" : "false"; }

// Runs compile.py's `main` on one command line, keeping what it writes
// in order.
class Compile {
 public:
  explicit Compile(const Options& options) : options_(options) {}

  Outcome run(const std::vector<std::string>& argv) {
    try {
      outcome_.status = main(argv);
    } catch (const argparse::Exit& e) {
      // argparse writes the help to stdout, or the usage and the error
      // to stderr, and raises SystemExit.
      if (!e.out.empty()) write(Stream::kOut, e.out);
      if (!e.err.empty()) write(Stream::kErr, e.err);
      outcome_.status = e.status;
      outcome_.exited = true;
    } catch (const Raised& e) {
      // The C++ preprocessor reports its argparse's exit as a SystemExit
      // whose message is the status.
      if (e.type == "SystemExit" && !e.message.empty() &&
          std::all_of(e.message.begin(), e.message.end(),
                      [](char c) { return c >= '0' && c <= '9'; })) {
        outcome_.status = std::stoll(e.message);
        outcome_.exited = true;
      } else {
        outcome_.raised = e;
      }
    } catch (const InternalError& e) {
      // cpp_backend.py raises the C++ side's internal error as an
      // AssertionError.
      outcome_.raised = Raised{"AssertionError", e.what(), ""};
    }
    return std::move(outcome_);
  }

 private:
  const Options& options_;
  Outcome outcome_;

  void write(Stream stream, std::string text) {
    if (text.empty()) return;
    if (!outcome_.writes.empty() && outcome_.writes.back().stream == stream) {
      outcome_.writes.back().text += text;
    } else {
      outcome_.writes.push_back(Write{stream, std::move(text)});
    }
  }

  void eprint(const std::string& line) { write(Stream::kErr, line + "\n"); }

  // compile.py's `_report`.
  void report(const std::string& path, const Raised& e) {
    const std::string where = path == "-" ? "<stdin>" : path;
    eprint(where + ": error: " + (e.strerror.empty() ? e.message : e.strerror));
  }

  // compile.py's `_print_warnings`, for the `[where, message]` pairs of
  // a response's `warnings` line.
  void print_warnings(const std::string& path, const std::string& warnings) {
    const std::string where = path == "-" ? "<stdin>" : path;
    for (const json::Value& w : json::parse(warnings).items) {
      eprint(where + ": warning: " + w.items.at(0).string + ": " + w.items.at(1).string);
    }
  }

  // compile.py's `_read_source`: stdin's text for `-`, or the file's.
  std::string read_source(const std::string& path) {
    if (path != "-") return read_text(path);
    if (!options_.read_stdin) return "";
    return options_.read_stdin();
  }

  std::string common_args(const Args& args) const {
    return "\"pcpp_args\": " + string_list_json(args.pcpp_args) +
           ", \"include_dir\": " + json::quote(options_.include_dir);
  }

  long long main(const std::vector<std::string>& argv) {
    const Args args = parse_args(argv, options_.columns);
    const std::string& stage = args.stage;
    if (stage == "lex" || stage == "parse" || stage == "resolve" || stage == "tac" ||
        stage == "codegen") {
      for (const std::string& p : args.inputs) {
        if (kind_of(p) != "c") {
          eprint("compile.py: " + p + " isn't a C source");
          return 2;
        }
      }
      if (args.output && args.inputs.size() > 1) {
        eprint("compile.py: -o with more than one input");
        return 2;
      }
      if (stage == "codegen" && args.output && *args.output != "-") {
        const std::string& o = *args.output;
        const auto ends = [&](std::string_view s) {
          return o.size() >= s.size() && o.compare(o.size() - s.size(), s.size(), s) == 0;
        };
        if (!ends(".s") && !ends(".asm")) {
          eprint("compile.py: -S output must have a .s or .asm suffix: " + o);
          return 2;
        }
      }
      for (const std::string& path : args.inputs) {
        std::string text;
        try {
          text = compile_c(path, args);
        } catch (const Raised& e) {
          if (!is_source_error(e.type) && !is_read_error(e.type)) throw;
          report(path, e);
          return 1;
        }
        std::optional<std::string> output = args.output;
        if (!output && stage == "codegen" && path != "-") output = stem_of(path) + ".s";
        if (output && *output != "-") {
          if (const int error = write_bytes(*output, text); error != 0) {
            throw os_error(error, *output);
          }
        } else {
          write(Stream::kOut, text);
        }
      }
      return 0;
    }
    if (stage == "object") {
      if (args.output && args.inputs.size() > 1) {
        eprint("compile.py: -o with -c and more than one input");
        return 2;
      }
      if (std::find(args.inputs.begin(), args.inputs.end(), "-") != args.inputs.end() &&
          !args.output) {
        eprint("compile.py: -c on stdin needs -o");
        return 2;
      }
      return build(args, nullptr);
    }
    std::vector<std::pair<std::string, asdl::Int>> defsyms;
    for (const std::string& d : args.defsyms) {
      const std::size_t eq = d.find('=');
      const std::string name = d.substr(0, eq);
      const std::optional<asdl::Int> v =
          eq == std::string::npos ? std::nullopt : number(d.substr(eq + 1));
      if (!v || name.empty()) {
        eprint("compile.py: --defsym takes NAME=VALUE: " + d);
        return 2;
      }
      // A name given twice keeps its first place and its last value, as
      // a dict does.
      const auto it = std::find_if(defsyms.begin(), defsyms.end(),
                                   [&](const auto& p) { return p.first == name; });
      if (it == defsyms.end()) {
        defsyms.emplace_back(name, *v);
      } else {
        it->second = *v;
      }
    }
    return build(args, &defsyms);
  }

  // compile.py's `_compile_c` under the variable: the request of
  // `cpp_backend.compile_source` (for `-S`) or `stage_source`, and the
  // replay of its response, with the warnings printed before an error.
  std::string compile_c(const std::string& path, const Args& args) {
    const std::string source = read_source(path);
    std::string mode;
    std::string kwargs = "{" + common_args(args);
    kwargs += ", \"filename\": " + (path == "-" ? std::string("null") : json::quote(path)) +
              ", \"preprocess\": true";
    const std::string inline_calls = bool_json(options_.inline_calls);
    if (args.stage == "codegen") {
      mode = "source-text";
      kwargs += ", \"optimize\": " + bool_json(args.optimize) + ", \"inline\": " + inline_calls +
                ", \"check_ir\": " + bool_json(options_.check_ir) + ", \"link_metadata\": false";
    } else {
      // `--lex`, `--parse`, `--resolve`, and `--tac`, the request of
      // `cpp_backend.stage_source`, which checks the IR only for `--tac`.
      mode = args.stage;
      kwargs += ", \"optimize\": " + bool_json(args.optimize) + ", \"inline\": " + inline_calls +
                ", \"check_ir\": " + bool_json(args.stage == "tac" && options_.check_ir);
    }
    kwargs += "}";
    const std::string request = "c6502-backend 3\nmode " + mode + "\nargs " + kwargs +
                                "\nsource " + std::to_string(source.size()) + "\n" + source;
    const std::vector<Record> response = records(backend::respond(request));
    std::string warnings = "[]";
    std::optional<Raised> error;
    std::string text;
    for (const Record& r : response) {
      if (r.key == "stderr") write(Stream::kErr, r.body);
      if (r.key == "warnings") warnings = r.value;
      if (r.key == "error") error = from_backend(json::parse(r.value));
      if (r.key == "text") text = r.body;
    }
    print_warnings(path, warnings);
    if (error) throw *error;
    return text;
  }

  // One input of `_cpp_build`, as compile.py reads it.
  struct Input {
    std::string path;
    std::string kind;
    std::string data;
    std::optional<std::string> filename;
    std::string name;
    std::string link_name;
  };

  // compile.py's `_cpp_build`: `-c` (`defsyms` null) or a link.
  long long build(const Args& args,
                  const std::vector<std::pair<std::string, asdl::Int>>* defsyms) {
    const bool link = defsyms != nullptr;
    std::vector<Input> inputs;
    std::optional<std::string> stop_object;
    std::optional<Raised> stop_error;
    for (const std::string& path : args.inputs) {
      const std::string kind = kind_of(path);
      if (!link && kind == "object") {
        stop_object = path;
        break;
      }
      Input in{path, kind, "", std::nullopt, "", ""};
      try {
        if (kind == "c") {
          in.data = read_source(path);
          if (path != "-") in.filename = path;
        } else if (kind == "asm") {
          in.data = read_text(path);
        } else {
          in.data = read_bytes(path);
        }
      } catch (const Raised& e) {
        stop_error = e;
        break;
      }
      in.name = kind == "object" ? path : (kind == "c" ? stem_of(path) + ".c" : name_of(path));
      in.link_name = kind == "object" ? path : stem_of(path) + ".o";
      inputs.push_back(std::move(in));
    }
    const std::string output = args.output && !args.output->empty() ? *args.output : "a.out";
    std::string kwargs = "{" + common_args(args) + ", \"optimize\": " + bool_json(args.optimize) +
                         ", \"inline\": " + bool_json(options_.inline_calls) +
                         ", \"check_ir\": " + bool_json(options_.check_ir) + ", \"inputs\": [";
    for (std::size_t i = 0; i < inputs.size(); ++i) {
      const Input& in = inputs[i];
      if (i != 0) kwargs += ", ";
      kwargs += "{\"kind\": " + json::quote(in.kind) + ", \"name\": " + json::quote(in.name) +
                ", \"filename\": " + (in.filename ? json::quote(*in.filename) : "null") +
                ", \"link_name\": " + json::quote(in.link_name) + "}";
    }
    kwargs += "]";
    if (link) {
      // argparse's `type=_number` has read the origin already.
      const std::optional<asdl::Int> origin =
          args.origin ? number(*args.origin) : std::optional<asdl::Int>();
      kwargs += ", \"run_link\": " + bool_json(!stop_object && !stop_error);
      kwargs += ", \"script\": " +
                (args.script && !args.script->empty() ? json::quote(*args.script) : "null");
      kwargs += ", \"origin\": " + (origin ? asdl::int_to_string(*origin) : "null");
      kwargs += ", \"defsyms\": [";
      for (std::size_t i = 0; i < defsyms->size(); ++i) {
        if (i != 0) kwargs += ", ";
        kwargs += "[" + json::quote((*defsyms)[i].first) + ", " +
                  asdl::int_to_string((*defsyms)[i].second) + "]";
      }
      kwargs += "], \"output\": " + json::quote(output);
    }
    kwargs += "}";
    std::string request = "c6502-backend 3\nmode " + std::string(link ? "link" : "objects") +
                          "\nargs " + kwargs + "\n";
    for (const Input& in : inputs) {
      request += "input " + std::to_string(in.data.size()) + "\n" + in.data;
    }
    const std::vector<Record> response = records(backend::respond(request));
    // The response's records by input, and the link's record.
    std::vector<std::vector<const Record*>> per_input;
    const Record* link_record = nullptr;
    for (const Record& r : response) {
      if (r.key == "input") {
        per_input.emplace_back();
      } else if (r.key == "link-error" || r.key == "output-error" || r.key == "executable" ||
                 (r.key == "write-error" && link)) {
        link_record = &r;
      } else if (!per_input.empty()) {
        per_input.back().push_back(&r);
      }
    }
    for (std::size_t i = 0; i < per_input.size() && i < inputs.size(); ++i) {
      const Input& in = inputs[i];
      std::optional<Raised> error;
      std::optional<Raised> write_error;
      std::string warnings = "[]";
      const std::string* object = nullptr;
      for (const Record* r : per_input[i]) {
        if (r->key == "stderr") write(Stream::kErr, r->body);
        if (r->key == "warnings") warnings = r->value;
        if (r->key == "error") error = from_backend(json::parse(r->value));
        if (r->key == "write-error") write_error = from_backend(json::parse(r->value));
        if (r->key == "object") object = &r->body;
      }
      if (in.kind == "c") print_warnings(in.path, warnings);
      if (error) {
        if (!is_input_error(error->type)) throw *error;
        report(in.path, *error);
        return 1;
      }
      if (!link) {
        const std::string out_path =
            args.output && !args.output->empty() ? *args.output : stem_of(in.path) + ".o";
        if (const int e = write_bytes(out_path, object != nullptr ? *object : ""); e != 0) {
          throw os_error(e, out_path);
        }
        if (write_error) throw *write_error;
      }
    }
    if (stop_object) {
      eprint("compile.py: " + *stop_object + " is already an object");
      return 2;
    }
    if (stop_error) {
      report(args.inputs[inputs.size()], *stop_error);
      return 1;
    }
    if (!link) return 0;
    if (link_record == nullptr) throw std::runtime_error("the response has no link record");
    if (link_record->key == "link-error") {
      const Raised e = from_backend(json::parse(link_record->value));
      if (!is_link_error(e.type)) throw e;
      eprint("compile.py: link error: " + e.message);
      return 1;
    }
    if (link_record->key == "write-error") {
      // Python opens the executable's file before `elf.write` raises.
      if (const int e = write_bytes(output, ""); e != 0) throw os_error(e, output);
      throw from_backend(json::parse(link_record->value));
    }
    if (link_record->key == "output-error") {
      const json::Value v = json::parse(link_record->value);
      const int error = static_cast<int>(v.get("errno")->integer);
      Raised e = os_error(error, output);
      e.strerror = v.get("strerror")->string;
      e.message = "[Errno " + std::to_string(error) + "] " + e.strerror + ": " +
                  pyrepr::string(output);
      throw e;
    }
    return 0;
  }
};

// --- The command ---

// Returns whether the environment variable `name` is "1".
bool env_is_one(const char* name) {
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
  const char* value = std::getenv(name);
#ifdef _MSC_VER
#pragma warning(pop)
#endif
  return value != nullptr && std::string_view(value) == "1";
}

void set_binary(std::FILE* f) {
#ifdef _WIN32
  _setmode(_fileno(f), _O_BINARY);
#else
  (void)f;
#endif
}

// Reads exactly `n` bytes of stdin, or throws.
std::string read_exactly(std::size_t n) {
  std::string data(n, '\0');
  if (n != 0 && std::fread(data.data(), 1, n, stdin) != n) {
    throw std::runtime_error("compile's frames end early");
  }
  return data;
}

std::string read_line() {
  std::string line;
  for (int c = std::fgetc(stdin); c != '\n'; c = std::fgetc(stdin)) {
    if (c == EOF) throw std::runtime_error("compile's frames end early");
    line += static_cast<char>(c);
  }
  return line;
}

void put(std::string_view s, std::FILE* f) { std::fwrite(s.data(), 1, s.size(), f); }

// Runs the command in frames with cpp_backend.py's `run_tool`, which
// sends a header line, the JSON object
//
//     {"include_dir": ..., "columns": ..., "inline": ..., "check_ir": ...}
//
// on stdin, and reads the frames on stdout:
//
//     stdin                          (a request for `sys.stdin.read()`)
//     write-out <length>             (a write to stdout)
//     <the text>
//     write-err <length>             (a write to stderr)
//     <the text>
//
// and last one of
//
//     status <n>                     (main returned n)
//     exit <n>                       (main raised SystemExit(n))
//     raised <JSON {"type": ..., "message": ...}>
//
// Python answers a request for stdin with
//
//     stdin <length>
//     <the text>
//
// or with `stdin-error <JSON {"type": ..., "message": ..., "strerror":
// ...}>` for the exception `sys.stdin.read()` raised.
int framed(const std::vector<std::string>& argv) {
  set_binary(stdin);
  set_binary(stdout);
  const json::Value header = json::parse(read_line());
  Options options;
  options.include_dir = header.get("include_dir")->string;
  options.columns = static_cast<int>(header.get("columns")->integer);
  options.inline_calls = header.get("inline")->boolean;
  options.check_ir = header.get("check_ir")->boolean;
  options.read_stdin = []() -> std::string {
    put("stdin\n", stdout);
    std::fflush(stdout);
    const std::string line = read_line();
    const std::size_t space = line.find(' ');
    const std::string key = line.substr(0, space);
    const std::string rest = space == std::string::npos ? "" : line.substr(space + 1);
    if (key == "stdin") return read_exactly(std::stoull(rest));
    if (key == "stdin-error") {
      const json::Value e = json::parse(rest);
      const json::Value* strerror = e.get("strerror");
      throw Raised{e.get("type")->string, e.get("message")->string,
                   strerror != nullptr && strerror->kind == json::Value::Kind::kString
                       ? strerror->string
                       : ""};
    }
    throw std::runtime_error("compile's answer for stdin is " + line);
  };
  const Outcome o = run(argv, options);
  for (const Write& w : o.writes) {
    put((w.stream == Stream::kOut ? "write-out " : "write-err ") + std::to_string(w.text.size()) +
            "\n",
        stdout);
    put(w.text, stdout);
  }
  if (o.raised) {
    put("raised {\"type\": " + json::quote(o.raised->type) +
            ", \"message\": " + json::quote(o.raised->message) + "}\n",
        stdout);
  } else {
    put(std::string(o.exited ? "exit " : "status ") + std::to_string(*o.status) + "\n", stdout);
  }
  std::fflush(stdout);
  return 0;
}

bool is_checkout(const fs::path& root) {
  std::error_code ec;
  return fs::is_regular_file(root / "cpp" / "CMakeLists.txt", ec) &&
         fs::is_regular_file(root / "include" / "stdint.h", ec);
}

}  // namespace

std::string find_include_dir(const std::string& self) {
  std::error_code ec;
  fs::path p = fs::absolute(processes::self_path(self), ec).parent_path();
  for (;; p = p.parent_path()) {
    if (is_checkout(p)) return (p / "include").make_preferred().string();
    if (p == p.parent_path()) break;
  }
#ifdef C6502_SOURCE_ROOT
  const fs::path built = fs::weakly_canonical(utf8_path(C6502_SOURCE_ROOT), ec);
  if (is_checkout(built)) return (built / "include").make_preferred().string();
#endif
  throw std::runtime_error(
      "compile can't find c6502's include/ above the executable, which it looks for beside "
      "cpp/CMakeLists.txt");
}

int write_bytes(const std::string& path, std::string_view data) {
  const fs::path p = utf8_path(path);
  errno = 0;
#ifdef _WIN32
  std::FILE* f = nullptr;
  if (const errno_t opened = _wfopen_s(&f, p.c_str(), L"wb"); opened != 0) return opened;
#else
  std::FILE* f = std::fopen(p.c_str(), "wb");
#endif
  if (f == nullptr) return errno != 0 ? errno : EIO;
  const bool wrote = std::fwrite(data.data(), 1, data.size(), f) == data.size();
  const int saved = errno;
  const bool closed = std::fclose(f) == 0;
  if (!wrote || !closed) return saved != 0 ? saved : (errno != 0 ? errno : EIO);
  return 0;
}

std::string read_text(const std::string& path) {
  const std::string data = read_bytes(path);
  try {
    return pcpp::decode_text_file(data, "utf-8");
  } catch (const CompileError& e) {
    throw Raised{e.python_type(), e.what(), ""};
  }
}

std::vector<Record> records(const std::string& data) {
  static const std::set<std::string, std::less<>> kSections = {
      "stderr", "tac", "asm", "asm2", "text", "object"};
  std::vector<Record> out;
  std::size_t pos = 0;
  while (pos < data.size()) {
    const std::size_t end = data.find('\n', pos);
    if (end == std::string::npos) throw std::runtime_error("a response line without its newline");
    const std::string line = data.substr(pos, end - pos);
    pos = end + 1;
    Record r;
    const std::size_t space = line.find(' ');
    r.key = line.substr(0, space);
    r.value = space == std::string::npos ? "" : line.substr(space + 1);
    if (kSections.count(r.key) != 0) {
      const std::size_t n = std::stoull(r.value);
      r.body = data.substr(pos, n);
      pos += n;
    }
    out.push_back(std::move(r));
  }
  return out;
}

std::string string_list_json(const std::vector<std::string>& v) {
  std::string s = "[";
  for (std::size_t i = 0; i < v.size(); ++i) {
    if (i != 0) s += ", ";
    s += json::quote(v[i]);
  }
  return s + "]";
}

std::optional<asdl::Int> number(std::string_view text) {
  const std::string_view t = py_strip(text);
  if (!t.empty() && t[0] == '$') return py_int(t.substr(1), 16);
  return py_int(t, 0);
}

Outcome run(const std::vector<std::string>& argv, const Options& options) {
  return Compile(options).run(argv);
}

int command(const std::vector<std::string>& args, const std::string& self) {
  std::vector<std::string> argv{"compile.py"};
  argv.insert(argv.end(), args.begin(), args.end());
  if (env_is_one("C6502CPP_FRAMES")) return framed(argv);
  Options options;
  options.include_dir = find_include_dir(self);
  options.columns = argparse::terminal_columns();
  options.check_ir = env_is_one("C6502_CHECK_IR");
  options.read_stdin = []() -> std::string {
    // Python reads stdin as text: on Windows with universal newlines,
    // and elsewhere with its line ends as they are.
    set_binary(stdin);
    const std::string data((std::istreambuf_iterator<char>(std::cin)),
                           std::istreambuf_iterator<char>());
    std::string text;
    try {
      text = pcpp::decode_text_file(data, "utf-8");
    } catch (const CompileError& e) {
      throw Raised{e.python_type(), e.what(), ""};
    }
#ifdef _WIN32
    return text;
#else
    return data;
#endif
  };
  const Outcome o = run(argv, options);
  // stdout and stderr stay in text mode, which on Windows ends each
  // line with CR LF, as Python's streams do.
  for (const Write& w : o.writes) {
    std::FILE* f = w.stream == Stream::kOut ? stdout : stderr;
    put(w.text, f);
    std::fflush(f);
  }
  if (o.raised) {
    // Python ends its traceback with the exception's line.
    std::fprintf(stderr, "%s: %s\n", o.raised->type.c_str(), o.raised->message.c_str());
    return 1;
  }
  return static_cast<int>(*o.status);
}

}  // namespace c6502::compile_command
