#include "golden.h"

#include <algorithm>
#include <cerrno>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <exception>
#include <filesystem>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
extern char** environ;
#endif

#include "backend.h"
#include "capture.h"
#include "errors.h"
#include "json.h"
#include "pcpp_text.h"
#include "pyrepr.h"
#include "sha256.h"

namespace c6502::golden {

namespace {

namespace fs = std::filesystem;

// The directories at the root whose files a command names by a path
// relative to the root (tools/write_golden.py's `ROOT_DIRS`).
constexpr std::string_view kRootDirs[] = {"examples/", "tests/", "golden/"};

constexpr std::string_view kFormat = "c6502-golden 1";

// --- Files ---

fs::path utf8_path(std::string_view s) {
  return fs::path(std::u8string(s.begin(), s.end()));
}

std::string path_text(const fs::path& p) {
  const std::u8string s = p.generic_u8string();
  return std::string(s.begin(), s.end());
}

// An exception that compile.py lets out of `main`, or that it reports
// with `_report`: its Python class, `str(e)`, and for an OSError that
// Python raised itself its `strerror`, which `_report` prints in place
// of `str(e)`.
struct Raised {
  std::string type;
  std::string message;
  std::string strerror;
};

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

// Writes `data` to the file at `path` as Python's `open(path, "wb")`
// and `write` do, and returns the errno of a file that can't be opened
// or written, or 0.
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

// Reads a UTF-8 text file as Python's `open(path, "r",
// encoding="utf-8").read()` does, with universal newlines, or throws
// its OSError or UnicodeDecodeError.
std::string read_text(const std::string& path) {
  const std::string data = read_bytes(path);
  try {
    return pcpp::decode_text_file(data, "utf-8");
  } catch (const CompileError& e) {
    throw Raised{e.python_type(), e.what(), ""};
  }
}

// --- compile.py's command line ---

// Thrown for a command line that the check doesn't run as compile.py
// would, which only a change of the golden files' jobs can give.
struct Unsupported : std::runtime_error {
  using std::runtime_error::runtime_error;
};

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
// ADDR`, as compile.py's `_rewrite_ttext` does.
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

// Reads compile.py's command line `argv` (the program's name first) as
// `_parse_args` does. The options are compile.py's, and argparse's
// `parse_known_args` puts every other option, and every argument after
// the first run of inputs, in the preprocessor's list in their order.
// The forms that make argparse report an error (no input, two stages,
// an option without its value, a bad `--origin`) aren't in the golden
// jobs, and throw `Unsupported`.
Args parse_args(const std::vector<std::string>& argv_in) {
  const std::vector<std::string> argv = rewrite_ttext(argv_in);
  Args args;
  bool inputs_taken = false;
  static const std::map<std::string, std::string, std::less<>> kStages = {
      {"--lex", "lex"}, {"--parse", "parse"}, {"--resolve", "resolve"},
      {"--tac", "tac"}, {"-S", "codegen"},    {"-c", "object"}};
  for (std::size_t i = 1; i < argv.size(); ++i) {
    const std::string& a = argv[i];
    const auto value = [&](std::string_view option) -> std::string {
      if (a.size() > option.size()) {
        // `--defsym=x` and `-ofile`.
        const std::size_t skip = a[option.size()] == '=' && option.size() > 2 ? 1 : 0;
        return a.substr(option.size() + skip);
      }
      if (i + 1 >= argv.size() ||
          (argv[i + 1].size() > 1 && argv[i + 1][0] == '-')) {
        throw Unsupported("the option " + std::string(option) + " has no value");
      }
      return argv[++i];
    };
    const bool option = a.size() > 1 && a[0] == '-';
    if (!option) {
      if (!inputs_taken) {
        while (i < argv.size() && !(argv[i].size() > 1 && argv[i][0] == '-')) {
          args.inputs.push_back(argv[i++]);
        }
        --i;
        inputs_taken = true;
      } else {
        args.pcpp_args.push_back(a);
      }
      continue;
    }
    if (a == "--" || a == "-h" || a == "--help") {
      throw Unsupported("the argument " + a);
    }
    if (const auto it = kStages.find(a); it != kStages.end()) {
      if (!args.stage.empty() && args.stage != it->second) {
        throw Unsupported("two stages");
      }
      args.stage = it->second;
    } else if (a == "-O") {
      args.optimize = true;
    } else if (a.rfind("-o", 0) == 0) {
      args.output = value("-o");
    } else if (a.rfind("-T", 0) == 0) {
      args.script = value("-T");
    } else if (a == "--origin" || a.rfind("--origin=", 0) == 0) {
      args.origin = value("--origin");
    } else if (a == "--defsym" || a.rfind("--defsym=", 0) == 0) {
      args.defsyms.push_back(value("--defsym"));
    } else {
      args.pcpp_args.push_back(a);
    }
  }
  if (args.inputs.empty()) throw Unsupported("no inputs");
  return args;
}

// Returns Python's `int(text, 0)`, or nullopt where it raises
// ValueError: a sign, then `0x`, `0o`, or `0b` and digits of that
// base, or decimal digits without a leading zero (or only zeros), with
// single underscores between the digits, and spaces around it.
std::optional<asdl::Int> int_base0(std::string_view t) {
  while (!t.empty() && std::isspace(static_cast<unsigned char>(t.front()))) t.remove_prefix(1);
  while (!t.empty() && std::isspace(static_cast<unsigned char>(t.back()))) t.remove_suffix(1);
  bool negative = false;
  if (!t.empty() && (t[0] == '+' || t[0] == '-')) {
    negative = t[0] == '-';
    t.remove_prefix(1);
  }
  int base = 10;
  bool prefixed = false;
  if (t.size() >= 2 && t[0] == '0') {
    const char p = static_cast<char>(std::tolower(static_cast<unsigned char>(t[1])));
    if (p == 'x' || p == 'o' || p == 'b') {
      base = p == 'x' ? 16 : (p == 'o' ? 8 : 2);
      t.remove_prefix(2);
      prefixed = true;
      if (!t.empty() && t[0] == '_') t.remove_prefix(1);
    }
  }
  if (t.empty() || t.front() == '_' || t.back() == '_') return std::nullopt;
  asdl::Int v = 0;
  bool nonzero = false;
  char last = 0;
  for (char c : t) {
    if (c == '_') {
      if (last == '_') return std::nullopt;
      last = c;
      continue;
    }
    int d = -1;
    if (c >= '0' && c <= '9') d = c - '0';
    if (c >= 'a' && c <= 'f') d = c - 'a' + 10;
    if (c >= 'A' && c <= 'F') d = c - 'A' + 10;
    if (d < 0 || d >= base) return std::nullopt;
    if (d != 0) nonzero = true;
    v = v * base + d;
    last = c;
  }
  if (base == 10 && !prefixed && t[0] == '0' && nonzero) return std::nullopt;
  return negative ? -v : v;
}

// Returns compile.py's `_number(text)`: decimal, `0x` hex, or `$` hex.
std::optional<asdl::Int> number(std::string_view text) {
  std::string_view t = text;
  while (!t.empty() && std::isspace(static_cast<unsigned char>(t.front()))) t.remove_prefix(1);
  while (!t.empty() && std::isspace(static_cast<unsigned char>(t.back()))) t.remove_suffix(1);
  if (!t.empty() && t[0] == '$') {
    std::string_view h = t.substr(1);
    while (!h.empty() && std::isspace(static_cast<unsigned char>(h.front()))) h.remove_prefix(1);
    bool negative = false;
    if (!h.empty() && (h[0] == '+' || h[0] == '-')) {
      negative = h[0] == '-';
      h.remove_prefix(1);
    }
    if (h.size() >= 2 && h[0] == '0' && (h[1] == 'x' || h[1] == 'X')) h.remove_prefix(2);
    const std::optional<asdl::Int> v = int_base0("0x" + std::string(h));
    if (!v) return std::nullopt;
    return negative ? -*v : *v;
  }
  return int_base0(t);
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
// side's error, by the name Python's `type(e).__name__` gives it: the
// name after a module's, so "struct.error" is "error".
Raised from_backend(const json::Value& error) {
  std::string type = error.get("type")->string;
  if (const std::size_t dot = type.rfind('.'); dot != std::string::npos) {
    type = type.substr(dot + 1);
  }
  return Raised{type, error.get("message")->string, ""};
}

// One record of a response of `c6502cpp backend` (src/backend.h): its
// key, the rest of its line, and the section that follows it.
struct Record {
  std::string key;
  std::string value;
  std::string body;
};

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

// What one command gave, as tools/write_golden.py records it.
struct Outcome {
  // The exit status, or nullopt when the command raised.
  std::optional<long long> status;
  std::string out;
  std::string err;
  std::optional<Raised> raised;
};

std::string bool_json(bool b) { return b ? "true" : "false"; }

std::string string_list_json(const std::vector<std::string>& v) {
  std::string s = "[";
  for (std::size_t i = 0; i < v.size(); ++i) {
    if (i != 0) s += ", ";
    s += json::quote(v[i]);
  }
  return s + "]";
}

// Runs compile.py's command line `argv` as compile.py's `main` does
// under `C6502_CPP_BACKEND=1`: `--lex`, `--parse`, `--resolve`,
// `--tac`, and `-S` compile each C input through the C++ back end's
// request of `cpp_backend.compile_source` (or `lex_source`, and the
// mode of the stage's name for the other three), and `-c` and a link
// send their inputs in one request of `cpp_backend.build`, as
// `compile._cpp_build` does, and replay the response input by input.
class Compile {
 public:
  explicit Compile(std::string include_dir) : include_dir_(std::move(include_dir)) {}

  Outcome run(const std::vector<std::string>& argv) {
    out_.clear();
    err_.clear();
    Outcome o;
    try {
      o.status = main(argv);
    } catch (const Raised& e) {
      if (e.type == "SystemExit") {
        o.status = std::stoll(e.message);
      } else {
        o.raised = e;
      }
    } catch (const InternalError& e) {
      o.raised = Raised{"AssertionError", e.what(), ""};
    }
    o.out = out_;
    o.err = err_;
    return o;
  }

 private:
  std::string include_dir_;
  std::string out_;
  std::string err_;

  void eprint(const std::string& line) { err_ += line + "\n"; }

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

  std::string common_args(const Args& args) const {
    return "\"pcpp_args\": " + string_list_json(args.pcpp_args) +
           ", \"include_dir\": " + json::quote(include_dir_);
  }

  long long main(const std::vector<std::string>& argv) {
    const Args args = parse_args(argv);
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
          out_ += text;
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
  // `cpp_backend.compile_source` (for `-S`) or `lex_source`, and the
  // replay of its response, with the warnings printed before an error.
  std::string compile_c(const std::string& path, const Args& args) {
    if (path == "-") throw Unsupported("a C source on stdin");
    const std::string source = read_text(path);
    std::string mode;
    std::string kwargs = "{" + common_args(args);
    kwargs += ", \"filename\": " + json::quote(path) + ", \"preprocess\": true";
    if (args.stage == "codegen") {
      mode = "source-text";
      kwargs += ", \"optimize\": " + bool_json(args.optimize) +
                ", \"inline\": true, \"check_ir\": false, \"link_metadata\": false";
    } else {
      // `--lex`, `--parse`, `--resolve`, and `--tac`, the request of
      // `cpp_backend.stage_source`.
      mode = args.stage;
      kwargs += ", \"optimize\": " + bool_json(args.optimize) +
                ", \"inline\": true, \"check_ir\": false";
    }
    kwargs += "}";
    const std::string request = "c6502-backend 3\nmode " + mode + "\nargs " + kwargs +
                                "\nsource " + std::to_string(source.size()) + "\n" + source;
    const std::vector<Record> response = records(backend::respond(request));
    std::string warnings = "[]";
    std::optional<Raised> error;
    std::string text;
    for (const Record& r : response) {
      if (r.key == "stderr") err_ += r.body;
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
          if (path == "-") throw Unsupported("a C source on stdin");
          in.data = read_text(path);
          in.filename = path;
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
                         ", \"inline\": true, \"check_ir\": false, \"inputs\": [";
    for (std::size_t i = 0; i < inputs.size(); ++i) {
      const Input& in = inputs[i];
      if (i != 0) kwargs += ", ";
      kwargs += "{\"kind\": " + json::quote(in.kind) + ", \"name\": " + json::quote(in.name) +
                ", \"filename\": " + (in.filename ? json::quote(*in.filename) : "null") +
                ", \"link_name\": " + json::quote(in.link_name) + "}";
    }
    kwargs += "]";
    if (link) {
      std::optional<asdl::Int> origin;
      if (args.origin) {
        origin = number(*args.origin);
        if (!origin) throw Unsupported("a bad --origin");
      }
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
        if (r->key == "stderr") err_ += r->body;
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

// --- The manifests ---

struct Job {
  std::string label;
  // The job's lines from `job` to `end`.
  std::vector<std::string> lines;
};

struct Manifest {
  std::string name;
  // The lines before the first job.
  std::vector<std::string> header;
  std::map<std::string, std::vector<std::string>, std::less<>> defines;
  std::set<std::string, std::less<>> texts;
  std::vector<Job> jobs;
};

std::vector<std::string> split_lines(const std::string& text) {
  std::vector<std::string> lines;
  std::size_t pos = 0;
  while (pos < text.size()) {
    std::size_t end = text.find('\n', pos);
    if (end == std::string::npos) end = text.size();
    lines.push_back(text.substr(pos, end - pos));
    pos = end + 1;
  }
  return lines;
}

Manifest read_manifest(const fs::path& path) {
  Manifest m;
  m.name = path_text(path.stem());
  const std::vector<std::string> lines = split_lines(capture::read_file(path));
  if (lines.empty() || lines[0] != kFormat) {
    throw std::runtime_error(path_text(path) + " isn't a manifest of golden files");
  }
  std::size_t i = 0;
  for (; i < lines.size() && lines[i].rfind("job ", 0) != 0; ++i) {
    const std::string& l = lines[i];
    m.header.push_back(l);
    if (l.rfind("define ", 0) == 0) {
      const std::size_t space = l.find(' ', 7);
      std::vector<std::string> list;
      for (const json::Value& v : json::parse(l.substr(space + 1)).items) list.push_back(v.string);
      m.defines[l.substr(7, space - 7)] = std::move(list);
    } else if (l.rfind("text ", 0) == 0) {
      m.texts.insert(l.substr(5));
    }
  }
  while (i < lines.size()) {
    Job job;
    job.label = lines[i].substr(4);
    for (; i < lines.size(); ++i) {
      job.lines.push_back(lines[i]);
      if (lines[i] == "end") break;
    }
    if (i == lines.size()) throw std::runtime_error("a job without its end: " + job.label);
    ++i;
    m.jobs.push_back(std::move(job));
  }
  return m;
}

// The first word of a manifest's line and the rest of it.
std::pair<std::string, std::string> split_word(const std::string& line) {
  const std::size_t space = line.find(' ');
  if (space == std::string::npos) return {line, ""};
  return {line.substr(0, space), line.substr(space + 1)};
}

// The spellings of the root a message can hold, with the separator after
// them, longest first, as tools/write_golden.py's `_root_forms`.
struct Root {
  fs::path path;
  std::string posix;
  std::vector<std::string> forms;
};

Root make_root(const fs::path& p) {
  Root r;
  r.path = fs::absolute(p).lexically_normal();
  std::string posix = path_text(r.path);
  while (posix.size() > 1 && posix.back() == '/') posix.pop_back();
  r.posix = posix;
  r.forms.push_back(posix + "/");
#ifdef _WIN32
  std::string native = posix;
  std::replace(native.begin(), native.end(), '/', '\\');
  r.forms.push_back(native + "\\");
#endif
  std::sort(r.forms.begin(), r.forms.end(),
            [](const std::string& a, const std::string& b) { return a.size() > b.size(); });
  return r;
}

std::string normalize(std::string text, const Root& root) {
  for (const std::string& form : root.forms) {
    std::size_t pos = 0;
    while ((pos = text.find(form, pos)) != std::string::npos) text.erase(pos, form.size());
  }
  return text;
}

// The directory of c6502's own headers, as cpp_backend.py's
// `_INCLUDE_DIR` spells it, with the platform's separators.
std::string compile_include_dir(const Root& root) {
  return (root.path / "include").make_preferred().string();
}

std::string normalized_json(json::Value v, const Root& root) {
  struct Walk {
    const Root& root;
    void operator()(json::Value& x) const {
      if (x.kind == json::Value::Kind::kString) x.string = normalize(x.string, root);
      for (json::Value& item : x.items) (*this)(item);
    }
  };
  Walk{root}(v);
  return json::dump(v);
}

std::vector<std::string> tacsim_lines(const std::vector<std::string>& argv,
                                      const std::string& include_dir, const Root& root) {
  const std::string source = read_text(argv.at(0));
  std::vector<std::string> pcpp(argv.begin() + 1, argv.end());
  const auto request = [&](const std::string& mode, const std::string& extra) {
    const std::string kwargs = "{\"pcpp_args\": " + string_list_json(pcpp) +
                               ", \"filename\": " + json::quote(argv[0]) +
                               ", \"include_dir\": " + json::quote(include_dir) +
                               ", \"preprocess\": true, \"check_ir\": false" + extra + "}";
    return records(backend::respond("c6502-backend 3\nmode " + mode + "\nargs " + kwargs +
                                    "\nsource " + std::to_string(source.size()) + "\n" +
                                    source));
  };
  std::vector<std::string> lines;
  for (const Record& r : request("tac-run", ", \"optimized\": true")) {
    if (r.key == "error") {
      return {"front " + normalized_json(json::parse(r.value), root)};
    }
    if (r.key == "result" || r.key == "optimized") {
      lines.push_back(r.key + " " + normalized_json(json::parse(r.value), root));
    }
  }
  for (const Record& r : request("tac-observe", "")) {
    if (r.key == "found") lines.push_back("found " + normalized_json(json::parse(r.value), root));
  }
  return lines;
}

bool names_root_file(std::string_view arg) {
  for (std::string_view d : kRootDirs) {
    if (arg.substr(0, d.size()) == d) return true;
  }
  return false;
}

// The path under golden/ of the text output `name` of the program
// `label` in the manifest `manifest` (tools/write_golden.py's
// `_text_path`).
std::string text_path(const std::string& manifest, std::string label, const std::string& name) {
  if (label.size() > 2 && label.compare(label.size() - 2, 2, ".c") == 0) {
    label.resize(label.size() - 2);
  }
  if (label.rfind("golden/", 0) == 0) label = label.substr(7);
  const std::size_t dot = name.rfind('.');
  const std::string suffix = dot != std::string::npos && dot > 0 ? name.substr(dot) : "";
  return manifest + "/" + label + suffix;
}

// The result of one job: its lines as the C++ compiler gives them, the
// number of commands it ran, and the first difference from the
// manifest, if any.
struct JobResult {
  std::vector<std::string> lines;
  std::size_t commands = 0;
  std::string difference;
};

std::string clip(const std::string& s, std::size_t n) {
  return s.size() <= n ? s : s.substr(0, n) + "...";
}

// Returns the 1-based number of the first line where `a` and `b`
// differ.
std::size_t first_line_difference(const std::string& a, const std::string& b) {
  std::size_t line = 1;
  for (std::size_t i = 0; i < a.size() && i < b.size(); ++i) {
    if (a[i] != b[i]) return line;
    if (a[i] == '\n') ++line;
  }
  return line;
}

// Takes the steps of `job` in the empty directory `work` and returns
// its lines. With `update`, writes each text output whose bytes
// changed under golden/.
JobResult run_job(const Manifest& m, const Job& job, const Root& root, const fs::path& work,
                  bool update) {
  JobResult r;
  Compile compile(compile_include_dir(root));
  const fs::path golden = root.path / "golden";
  fs::current_path(work);
  std::string last_command;
  for (const std::string& line : job.lines) {
    const auto [word, rest] = split_word(line);
    if (word == "status" || word == "stdout" || word == "stderr" || word == "raised" ||
        word == "file" || word == "end" || word == "front" || word == "result" ||
        word == "optimized" || word == "found") {
      continue;
    }
    r.lines.push_back(line);
    if (word == "job") continue;
    if (word == "mkdir") {
      fs::create_directories(work / utf8_path(rest));
    } else if (word == "cd") {
      fs::current_path(work / utf8_path(rest));
    } else if (word == "write") {
      const auto [path, spec] = split_word(rest);
      const auto [how, what] = split_word(spec);
      const std::string data =
          how == "text" ? json::parse(what).string : capture::read_file(golden / "blobs" / what);
      if (write_bytes(path_text(work / utf8_path(path)), data) != 0) {
        throw std::runtime_error("can't write " + path);
      }
    } else if (word == "tacsim") {
      ++r.commands;
      std::vector<std::string> argv;
      for (const json::Value& v : json::parse(rest).items) {
        argv.push_back(names_root_file(v.string) ? root.posix + "/" + v.string : v.string);
      }
      for (std::string& l : tacsim_lines(argv, compile_include_dir(root), root)) {
        r.lines.push_back(std::move(l));
      }
    } else if (word == "run") {
      ++r.commands;
      std::vector<std::string> argv;
      for (const json::Value& v : json::parse(rest).items) {
        if (const auto it = m.defines.find(v.string); it != m.defines.end()) {
          argv.insert(argv.end(), it->second.begin(), it->second.end());
        } else if (names_root_file(v.string)) {
          argv.push_back(root.posix + "/" + v.string);
        } else {
          argv.push_back(v.string);
        }
      }
      last_command = rest;
      Outcome o;
      try {
        o = compile.run(argv);
      } catch (const Unsupported& e) {
        r.difference = "the command " + rest + " can't be run: " + e.what();
        return r;
      }
      r.lines.push_back("status " + (o.status ? std::to_string(*o.status) : std::string("none")));
      const std::string out = normalize(o.out, root);
      const std::string err = normalize(o.err, root);
      if (!out.empty()) r.lines.push_back("stdout " + json::quote(out));
      if (!err.empty()) r.lines.push_back("stderr " + json::quote(err));
      if (o.raised) {
        r.lines.push_back("raised [" + json::quote(o.raised->type) + ", " +
                          json::quote(normalize(o.raised->message, root)) + "]");
      }
    } else {
      throw std::runtime_error("a manifest line that isn't a step: " + line);
    }
  }
  // The files the job left, sorted by path.
  std::vector<std::pair<std::string, fs::path>> files;
  for (const auto& entry : fs::recursive_directory_iterator(work)) {
    if (entry.is_regular_file()) {
      files.emplace_back(path_text(entry.path().lexically_relative(work)), entry.path());
    }
  }
  std::sort(files.begin(), files.end());
  std::string text_problem;
  for (const auto& [rel, path] : files) {
    const std::string data = capture::read_file(path);
    const bool program = job.label.rfind("game ", 0) != 0 && job.label.rfind("errors ", 0) != 0;
    if (program && m.texts.count(rel) != 0) {
      const std::string where = text_path(m.name, job.label, rel);
      r.lines.push_back("file " + rel + " text " + where);
      const fs::path kept = golden / utf8_path(where);
      std::string before;
      std::error_code ec;
      const bool exists = fs::is_regular_file(kept, ec);
      if (exists) before = capture::read_file(kept);
      if (!exists || before != data) {
        if (text_problem.empty()) {
          text_problem = !exists ? "golden/" + where + " is missing"
                                 : "the text of " + rel + " differs from golden/" + where +
                                       " from line " +
                                       std::to_string(first_line_difference(before, data));
        }
        if (update) {
          fs::create_directories(kept.parent_path());
          write_bytes(path_text(kept), data);
        }
      }
    } else {
      r.lines.push_back("file " + rel + " sha256 " + sha256_hex(data) + " " +
                        std::to_string(data.size()));
    }
  }
  r.lines.push_back("end");
  // The first line that differs, with the command it follows.
  std::string command;
  const std::size_t n = std::max(r.lines.size(), job.lines.size());
  for (std::size_t i = 0; i < n; ++i) {
    const std::string got = i < r.lines.size() ? r.lines[i] : "(nothing)";
    const std::string want = i < job.lines.size() ? job.lines[i] : "(nothing)";
    if (want.rfind("run ", 0) == 0) command = want.substr(4);
    if (got != want) {
      r.difference = (command.empty() ? std::string() : "after " + clip(command, 200) + ", ") +
                     "the golden file has\n    " + clip(want, 600) + "\n  and C++ gives\n    " +
                     clip(got, 600);
      return r;
    }
  }
  r.difference = text_problem;
  return r;
}

// --- Workers ---

// Returns the path of the running executable.
std::string self_path(const std::string& fallback) {
#ifdef _WIN32
  wchar_t buf[MAX_PATH * 4];
  const DWORD n = GetModuleFileNameW(nullptr, buf, static_cast<DWORD>(std::size(buf)));
  if (n > 0 && n < std::size(buf)) return path_text(fs::path(std::wstring(buf, n)));
#else
  char buf[4096];
  const ssize_t n = readlink("/proc/self/exe", buf, sizeof buf - 1);
  if (n > 0) return std::string(buf, static_cast<std::size_t>(n));
#endif
  return fallback;
}

#ifdef _WIN32
// Quotes `arg` for CommandLineToArgvW, as Python's
// `subprocess.list2cmdline` does.
std::wstring quote_arg(const std::wstring& arg) {
  if (!arg.empty() && arg.find_first_of(L" \t\"") == std::wstring::npos) return arg;
  std::wstring out = L"\"";
  std::size_t backslashes = 0;
  for (wchar_t c : arg) {
    if (c == L'\\') {
      ++backslashes;
      continue;
    }
    if (c == L'"') {
      out.append(backslashes * 2 + 1, L'\\');
    } else {
      out.append(backslashes, L'\\');
    }
    backslashes = 0;
    out += c;
  }
  out.append(backslashes * 2, L'\\');
  return out + L"\"";
}
#endif

// Starts each command line of `commands` and waits for all of them,
// returning their exit statuses.
std::vector<int> run_all(const std::vector<std::vector<std::string>>& commands) {
  std::vector<int> statuses(commands.size(), -1);
#ifdef _WIN32
  std::vector<PROCESS_INFORMATION> procs(commands.size());
  for (std::size_t i = 0; i < commands.size(); ++i) {
    std::wstring line;
    for (const std::string& a : commands[i]) {
      if (!line.empty()) line += L' ';
      line += quote_arg(utf8_path(a).wstring());
    }
    STARTUPINFOW si{};
    si.cb = sizeof si;
    if (!CreateProcessW(nullptr, line.data(), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si,
                        &procs[i])) {
      throw std::runtime_error("can't start a worker");
    }
  }
  for (std::size_t i = 0; i < commands.size(); ++i) {
    WaitForSingleObject(procs[i].hProcess, INFINITE);
    DWORD code = 1;
    GetExitCodeProcess(procs[i].hProcess, &code);
    statuses[i] = static_cast<int>(code);
    CloseHandle(procs[i].hProcess);
    CloseHandle(procs[i].hThread);
  }
#else
  std::vector<pid_t> pids(commands.size());
  for (std::size_t i = 0; i < commands.size(); ++i) {
    std::vector<char*> argv;
    for (const std::string& a : commands[i]) argv.push_back(const_cast<char*>(a.c_str()));
    argv.push_back(nullptr);
    if (posix_spawn(&pids[i], argv[0], nullptr, nullptr, argv.data(), environ) != 0) {
      throw std::runtime_error("can't start a worker");
    }
  }
  for (std::size_t i = 0; i < commands.size(); ++i) {
    int status = 0;
    waitpid(pids[i], &status, 0);
    statuses[i] = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
  }
#endif
  return statuses;
}

struct Options {
  fs::path root;
  int jobs = 1;
  std::set<std::string, std::less<>> manifests;
  std::string label;
  bool update = false;
  bool verbose = false;
  // For a worker, its share of the jobs (those whose index modulo
  // `shares` is `share`) and the file it writes its results to.
  int share = -1;
  int shares = 1;
  fs::path results;
};

// Returns the manifests under golden/ that `opts` selects, sorted by
// name.
std::vector<Manifest> read_manifests(const Options& opts) {
  std::vector<fs::path> paths;
  for (const auto& entry : fs::directory_iterator(opts.root / "golden")) {
    if (entry.is_regular_file() && entry.path().extension() == ".txt") {
      const std::string name = path_text(entry.path().stem());
      if (opts.manifests.empty() || opts.manifests.count(name) != 0) paths.push_back(entry.path());
    }
  }
  std::sort(paths.begin(), paths.end());
  std::vector<Manifest> out;
  for (const fs::path& p : paths) out.push_back(read_manifest(p));
  return out;
}

// Runs a worker's share of the jobs and writes, for each, a record of
// its index, its count of commands, the difference, and its new lines.
int worker(const Options& opts) {
  const Root root = make_root(opts.root);
  const std::vector<Manifest> manifests = read_manifests(opts);
  const fs::path base = fs::temp_directory_path() /
                        ("c6502-golden-" + std::to_string(
#ifdef _WIN32
                                               GetCurrentProcessId()
#else
                                               getpid()
#endif
                                                   ));
  const fs::path here = fs::current_path();
  std::string out;
  std::size_t index = 0;
  for (const Manifest& m : manifests) {
    for (const Job& job : m.jobs) {
      const std::size_t mine = index++;
      if (static_cast<int>(mine % static_cast<std::size_t>(opts.shares)) != opts.share) continue;
      if (!opts.label.empty() && job.label != opts.label) continue;
      std::error_code ec;
      fs::remove_all(base, ec);
      fs::create_directories(base);
      JobResult r;
      try {
        r = run_job(m, job, root, base, opts.update);
      } catch (const std::exception& e) {
        r.difference = std::string("the check failed: ") + e.what();
      }
      fs::current_path(here);
      fs::remove_all(base, ec);
      std::string block;
      for (const std::string& l : r.lines) block += l + "\n";
      out += "result " + std::to_string(mine) + " " + std::to_string(r.commands) + " " +
             std::to_string(r.difference.size()) + " " + std::to_string(block.size()) + "\n" +
             r.difference + block;
    }
  }
  if (write_bytes(path_text(opts.results), out) != 0) {
    std::fprintf(stderr, "c6502cpp: can't write %s\n", path_text(opts.results).c_str());
    return 2;
  }
  return 0;
}

struct WorkerResult {
  std::size_t commands = 0;
  std::string difference;
  std::string block;
};

std::map<std::size_t, WorkerResult> read_results(const fs::path& path) {
  std::map<std::size_t, WorkerResult> out;
  const std::string data = capture::read_file(path);
  std::size_t pos = 0;
  while (pos < data.size()) {
    const std::size_t end = data.find('\n', pos);
    std::size_t index = 0, commands = 0, dlen = 0, blen = 0;
    std::istringstream head(data.substr(pos, end - pos));
    std::string word;
    if (!(head >> word >> index >> commands >> dlen >> blen) || word != "result") {
      throw std::runtime_error("a worker's results can't be read");
    }
    pos = end + 1;
    WorkerResult r{commands, data.substr(pos, dlen), data.substr(pos + dlen, blen)};
    pos += dlen + blen;
    out[index] = std::move(r);
  }
  return out;
}

// Returns the first directory from the current one upward that holds
// golden/.
fs::path find_root() {
  for (fs::path p = fs::current_path();; p = p.parent_path()) {
    if (fs::is_directory(p / "golden")) return p;
    if (p == p.parent_path()) throw std::runtime_error("no golden/ here or above; give --root");
  }
}

}  // namespace

int check_golden(const std::vector<std::string>& args, const std::string& self) {
  Options opts;
  bool root_given = false;
  for (std::size_t i = 0; i < args.size(); ++i) {
    const std::string& a = args[i];
    const auto next = [&]() -> std::string {
      if (i + 1 >= args.size()) throw std::runtime_error(a + " takes a value");
      return args[++i];
    };
    if (a == "--root") {
      opts.root = utf8_path(next());
      root_given = true;
    } else if (a == "-j") {
      opts.jobs = std::max(1, std::stoi(next()));
    } else if (a == "--manifest") {
      std::string list = next();
      for (std::size_t p = 0; p <= list.size();) {
        const std::size_t comma = std::min(list.find(',', p), list.size());
        if (comma > p) opts.manifests.insert(list.substr(p, comma - p));
        p = comma + 1;
      }
    } else if (a == "--label") {
      opts.label = next();
    } else if (a == "--update") {
      opts.update = true;
    } else if (a == "--verbose") {
      opts.verbose = true;
    } else if (a == "--worker") {
      opts.share = std::stoi(next());
      opts.shares = std::stoi(next());
      opts.results = utf8_path(next());
    } else {
      std::fprintf(stderr, "c6502cpp: check-golden doesn't take %s\n", a.c_str());
      return 2;
    }
  }
  if (!root_given) opts.root = find_root();
  opts.root = fs::absolute(opts.root);
  if (opts.share >= 0) return worker(opts);

  const auto start = std::chrono::steady_clock::now();
  const std::vector<Manifest> manifests = read_manifests(opts);
  const std::string exe = self_path(self);
  const fs::path results_dir =
      fs::temp_directory_path() / ("c6502-golden-results-" + std::to_string(
#ifdef _WIN32
                                                                 GetCurrentProcessId()
#else
                                                                 getpid()
#endif
                                                                 ));
  fs::create_directories(results_dir);
  std::vector<std::vector<std::string>> commands;
  for (int k = 0; k < opts.jobs; ++k) {
    std::vector<std::string> cmd = {exe, "check-golden", "--root", path_text(opts.root),
                                    "--worker", std::to_string(k), std::to_string(opts.jobs),
                                    path_text(results_dir / (std::to_string(k) + ".txt"))};
    if (!opts.manifests.empty()) {
      std::string list;
      for (const std::string& n : opts.manifests) list += (list.empty() ? "" : ",") + n;
      cmd.push_back("--manifest");
      cmd.push_back(list);
    }
    if (!opts.label.empty()) {
      cmd.push_back("--label");
      cmd.push_back(opts.label);
    }
    if (opts.update) cmd.push_back("--update");
    commands.push_back(std::move(cmd));
  }
  const std::vector<int> statuses = run_all(commands);
  std::map<std::size_t, WorkerResult> results;
  for (int k = 0; k < opts.jobs; ++k) {
    if (statuses[k] != 0) {
      std::fprintf(stderr, "c6502cpp: a worker of check-golden failed\n");
      return 2;
    }
    results.merge(read_results(results_dir / (std::to_string(k) + ".txt")));
  }
  std::error_code ec;
  fs::remove_all(results_dir, ec);

  std::size_t index = 0;
  std::size_t total_jobs = 0;
  std::size_t total_commands = 0;
  std::size_t total_differ = 0;
  for (const Manifest& m : manifests) {
    std::size_t jobs = 0, commands_run = 0, differ = 0;
    std::string text;
    for (const std::string& l : m.header) text += l + "\n";
    std::set<std::string> kept;
    for (const Job& job : m.jobs) {
      const auto it = results.find(index++);
      if (it == results.end()) {
        for (const std::string& l : job.lines) text += l + "\n";
        continue;
      }
      const WorkerResult& r = it->second;
      ++jobs;
      commands_run += r.commands;
      if (!r.difference.empty()) {
        ++differ;
        const std::string shown =
            opts.verbose ? r.difference : r.difference.substr(0, std::min<std::size_t>(r.difference.size(), 2000));
        std::printf("DIFFER %s: %s: %s\n", m.name.c_str(), job.label.c_str(), shown.c_str());
      }
      text += r.block;
      for (const std::string& l : split_lines(r.block)) {
        const auto [word, rest] = split_word(l);
        if (word != "file") continue;
        const std::size_t t = rest.find(" text ");
        if (t != std::string::npos) kept.insert(rest.substr(t + 6));
      }
    }
    std::printf("%s: %zu jobs of %zu commands, %zu differ\n", m.name.c_str(), jobs, commands_run,
                differ);
    total_jobs += jobs;
    total_commands += commands_run;
    total_differ += differ;
    if (opts.update && opts.label.empty() && differ != 0) {
      write_bytes(path_text(opts.root / "golden" / (m.name + ".txt")), text);
      // The texts that no job of the manifest names any longer.
      const fs::path dir = opts.root / "golden" / m.name;
      if (!m.texts.empty() && fs::is_directory(dir)) {
        std::vector<fs::path> stale;
        for (const auto& entry : fs::recursive_directory_iterator(dir)) {
          if (!entry.is_regular_file()) continue;
          const std::string rel = path_text(entry.path().lexically_relative(opts.root / "golden"));
          if (kept.count(rel) == 0) stale.push_back(entry.path());
        }
        for (const fs::path& p : stale) fs::remove(p, ec);
      }
    }
  }
  const double seconds =
      std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
  std::printf("%zu jobs of %zu commands checked in %.0f s with %d processes, %zu differ%s\n",
              total_jobs, total_commands, seconds, opts.jobs, total_differ,
              opts.update && total_differ != 0 ? " (golden files updated)" : "");
  return total_differ == 0 ? 0 : 1;
}

}  // namespace c6502::golden
