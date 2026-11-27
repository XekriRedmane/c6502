// The test library of the cases that compile a whole program, which
// step 9 of the port moved from the Python suite (docs/cpp_port.md, G1).
// It runs the compiler's stages, compile.py's command line, nm and
// objdump, and the TAC interpreter in process, and holds the helpers
// those tests share: the repository's files, temporary directories, the
// links that tests/link_helpers.py made, a function's lines in the `-S`
// text, and the hierarchy of errors.py's classes, which a C++ error
// carries only by its name.
#pragma once

#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "asdl/runtime.h"
#include "compile_command.h"
#include "diagnostics.h"
#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::suite {

// --- The repository ---

// Returns the path of `rel`, a path relative to the root of the
// checkout the tests were built from, with `/` separators.
std::string repo_path(std::string_view rel);
// Returns the bytes of the repository's file `rel`.
std::string read_repo_file(std::string_view rel);

// --- Compiling a source ---

// The stages of compile.py's `_run_source`: `--lex`, `--parse`,
// `--resolve`, `--tac`, and `-S`.
enum class Stage { kLex, kParse, kResolve, kTac, kCodegen };

struct Options {
  bool optimize = false;
  // false for a test that tests/no_inlining.py decorated.
  bool inline_calls = true;
  // The IR checks (src/passes/ir_check.h), which the Python suite runs
  // with C6502_CHECK_IR=1, are on unless a test turns them off.
  bool check_ir = true;
  // The link metadata `-c` assembles, in the unoptimized `-S` text.
  bool link_metadata = false;
  // false when the source is the preprocessor's output already, as
  // compile.py's `_run_stage` takes it.
  bool preprocess = true;
  std::vector<std::string> pcpp_args;
  // The file the source came from, or none for stdin.
  std::optional<std::string> filename;
};

// An error of the program, by the name of its Python class in errors.py
// and its message, as `CompileError::python_type()` gives them.
struct Error {
  std::string type;
  std::string message;
};

struct Compiled {
  // The stage's text: the tokens of `--lex`, the tree of `--parse`,
  // `--resolve`, and `--tac` with a newline after it, or the `-S` text.
  std::string text;
  // What the preprocessor wrote to stderr.
  std::string err;
  std::optional<Error> error;
  std::vector<diagnostics::CompileWarning> warnings;
};

// Compiles `source` to `stage` as compile.py's `_run_source` does.
Compiled compile(Stage stage, const std::string& source, const Options& options = {});

// Returns the text of `stage` for `source`, and throws
// `std::runtime_error` with the error's class and message when the
// program is rejected, which doctest reports as the case's failure.
std::string run_source(Stage stage, const std::string& source, const Options& options = {});
// Returns the `-S` text of `source`, as `run_source(Stage::kCodegen)`.
std::string codegen(const std::string& source, bool optimize = false);
std::string codegen(const std::string& source, const Options& options);

// Returns the error that rejects `source` at `stage`, and throws
// `std::runtime_error` when the program compiles.
Error compile_error(Stage stage, const std::string& source, const Options& options = {});

// Returns whether the class `type` of errors.py is `base` or a subclass
// of it, as `assertRaises(base)` would catch it. The hierarchy is that
// of errors.py (Lark's syntax errors under `LarkError`, `ZpGlobalError`
// under `TypeCheckError`, `NumberError` and `FrameSizeError` under
// `ValueError`), and every class is an `Exception`.
bool is_a(std::string_view type, std::string_view base);
// Returns whether `type` is one of compile.py's `_source_errors()`, the
// classes that report a problem with the program.
bool is_source_error(std::string_view type);

// --- The TAC ---

// A program's TAC and the tables it was translated with.
struct Tac {
  tac_ast::Program program;
  passes::SymbolTable symbols;
  passes::TypeTable types;
};

// Returns the TAC that `--tac` prints for `source`, with `-O` when
// `options.optimize` is set. Throws `std::runtime_error` with the
// error of a rejected program.
Tac tac_program(const std::string& source, const Options& options = {});

// What `main` returns on the unoptimized TAC, or what the interpreter
// or the front end raised, as `cpp_backend.tac_main` gives it.
struct TacOutcome {
  std::optional<asdl::Int> value;
  std::optional<Error> error;
};
TacOutcome tac_main(const std::string& source);
// Returns `tests.test_sim_chapters._run_program(source)`, the value or
// none, and throws `std::runtime_error` with the error, as Python
// raises it.
std::optional<asdl::Int> run_program(const std::string& source);

// --- Command lines ---

struct MainOptions {
  bool inline_calls = true;
  bool check_ir = true;
  int columns = 80;
};

// Runs compile.py's `main` on `argv` (the program's name first) with
// `stdin` as `sys.stdin`, in the current directory.
compile_command::Outcome compile_main(const std::vector<std::string>& argv,
                                      const std::string& stdin_text = "",
                                      const MainOptions& options = {});

// What nm.py's or objdump.py's `main` returned and wrote.
struct ToolOutcome {
  int status = 0;
  std::string out;
  std::string err;
};
ToolOutcome nm_main(const std::vector<std::string>& argv);
ToolOutcome objdump_main(const std::vector<std::string>& argv);

// A new empty directory, removed with what it holds when the object
// goes, as `tempfile.TemporaryDirectory` gives one.
class TempDir {
 public:
  TempDir();
  ~TempDir();
  TempDir(const TempDir&) = delete;
  TempDir& operator=(const TempDir&) = delete;

  const std::filesystem::path& path() const { return path_; }
  // Returns the path of `name` in the directory, with `/` separators.
  std::string file(std::string_view name) const;

 private:
  std::filesystem::path path_;
};

// Makes `dir` the current directory until the object goes.
class ChangeDir {
 public:
  explicit ChangeDir(const std::filesystem::path& dir);
  ~ChangeDir();
  ChangeDir(const ChangeDir&) = delete;
  ChangeDir& operator=(const ChangeDir&) = delete;

 private:
  std::filesystem::path previous_;
};

// Writes `data` to the file at `path`.
void write_file(const std::string& path, std::string_view data);
// Returns the bytes of the file at `path`.
std::string read_file(const std::string& path);

// --- Links (tests/link_helpers.py) ---

// The `--defsym` arguments that give the runtime helpers the addresses
// of the simulator's copies, as sim/linked.py's `defsym_args()` gives
// them.
std::vector<std::string> sim_defsym_args();

// Compiles `source` from stdin to the object `out_path` with `-c`, and
// `-O` unless `optimize` is false, and returns the exit status.
long long compile_object(const std::string& source, const std::string& out_path,
                         bool optimize = true, const std::vector<std::string>& extra = {});

// Links the objects `paths` into `out_path` with the simulator's
// helper addresses, and with `-T script` when a script is given.
compile_command::Outcome link(const std::vector<std::string>& paths, const std::string& out_path,
                              const std::optional<std::string>& script = std::nullopt,
                              const std::vector<std::string>& extra = {});

// Returns the executable's `objdump -D --no-show-raw-insn
// --no-addresses` text, a blank line, and its link metadata, as
// `linked_text` gives it.
std::string linked_text(const std::string& exe_path);
// Returns `objdump -D --no-show-raw-insn --no-addresses` of the file.
std::string listing(const std::string& exe_path);
// Returns the executable's defined symbols by name, other than those
// of type STT_FILE, a global one where a name is both.
std::map<std::string, std::int64_t> symbols(const std::string& exe_path);

// --- The `-S` text ---

// Returns the instruction and local label lines of the function `name`
// in the `-S` text `asm`, each stripped and with its runs of spaces made
// one, without blank lines, comments, and `SUBROUTINE`, up to the next
// function's label. Throws `std::runtime_error` when no line is
// `name:`.
std::vector<std::string> function_body(const std::string& asm_text, std::string_view name);

// Returns the lines of `text` as Python's `str.splitlines` gives them
// for text with LF or CR LF line ends.
std::vector<std::string> lines(std::string_view text);
bool contains(std::string_view text, std::string_view needle);
// Returns Python's `text.count(needle)`, the non-overlapping matches.
std::size_t count(std::string_view text, std::string_view needle);
// Returns whether the ECMAScript pattern `pattern` matches somewhere in
// `text`, as Python's `re.search` does for the patterns the tests use.
bool search(std::string_view pattern, std::string_view text);

}  // namespace c6502::suite
