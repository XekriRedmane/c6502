#include "compile_case.h"

#include <atomic>
#include <chrono>
#include <fstream>
#include <memory>
#include <ostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "binutils/elf.h"
#include "binutils/nm.h"
#include "binutils/objdump.h"
#include "errors.h"
#include "lexer.h"
#include "passes/codegen_stage.h"
#include "passes/front_end.h"
#include "passes/ir_check.h"
#include "passes/text_stages.h"
#include "passes/zp_globals.h"
#include "sim/harness.h"
#include "tac_sim.h"
#include "tac_sim_modes.h"

#ifndef C6502_SOURCE_ROOT
#error "cpp/CMakeLists.txt defines C6502_SOURCE_ROOT for the tests"
#endif

namespace c6502::suite {

namespace {

std::runtime_error rejected(const Error& e) {
  return std::runtime_error("the program was rejected: " + e.type + ": " + e.message);
}

passes::FrontEndOptions front_options(const Options& options) {
  passes::FrontEndOptions front;
  front.pcpp_args = options.pcpp_args;
  front.filename = options.filename;
  front.include_dir = repo_path("include");
  front.preprocess = options.preprocess;
  front.optimize = options.optimize;
  return front;
}

// errors.py's classes that derive from another than `Exception`.
const std::map<std::string_view, std::string_view>& parents() {
  static const std::map<std::string_view, std::string_view> table = {
      {"UnexpectedInput", "LarkError"},       {"UnexpectedToken", "UnexpectedInput"},
      {"UnexpectedCharacters", "UnexpectedInput"}, {"UnexpectedEOF", "UnexpectedInput"},
      {"VisitError", "LarkError"},            {"NumberError", "ValueError"},
      {"FrameSizeError", "ValueError"},       {"ZpGlobalError", "TypeCheckError"},
  };
  return table;
}

}  // namespace

std::string repo_path(std::string_view rel) {
  std::string root = C6502_SOURCE_ROOT;
  if (!root.empty() && root.back() != '/') root += '/';
  return root + std::string(rel);
}

std::string read_repo_file(std::string_view rel) { return read_file(repo_path(rel)); }

Compiled compile(Stage stage, const std::string& source, const Options& options) {
  const passes::ir_check::Enabled checks(options.check_ir);
  const passes::FrontEndOptions front = front_options(options);
  Compiled out;
  try {
    if (stage == Stage::kCodegen) {
      passes::StageTables tables;
      {
        diagnostics::Collecting block(out.warnings);
        passes::FrontEndResult checked = passes::front_end(source, front, out.err);
        tables.c99 = std::move(checked.prog);
        tables.symbols = std::move(checked.symbols);
        tables.types = std::move(checked.types);
      }
      tables.footprints = passes::helper_footprints();
      passes::CodegenOptions codegen;
      codegen.optimize = options.optimize;
      codegen.inline_calls = options.inline_calls;
      codegen.link_metadata = options.link_metadata;
      out.text = passes::codegen(tables, codegen).emitted.text;
    } else {
      diagnostics::Collecting block(out.warnings);
      const std::string text = passes::preprocess_source(source, front, out.err);
      if (stage == Stage::kLex) {
        out.text = lexer::format_tokens(text);
      } else {
        const passes::TextStage text_stage = stage == Stage::kParse     ? passes::TextStage::kParse
                                             : stage == Stage::kResolve ? passes::TextStage::kResolve
                                                                        : passes::TextStage::kTac;
        out.text =
            passes::run_text_stage(text_stage, text, options.optimize, options.inline_calls).text;
      }
    }
  } catch (const CompileError& e) {
    out.error = Error{e.python_type(), e.what()};
  }
  return out;
}

std::string run_source(Stage stage, const std::string& source, const Options& options) {
  Compiled c = compile(stage, source, options);
  if (c.error) throw rejected(*c.error);
  return std::move(c.text);
}

std::string codegen(const std::string& source, bool optimize) {
  Options options;
  options.optimize = optimize;
  return run_source(Stage::kCodegen, source, options);
}

std::ostream& operator<<(std::ostream& out, Pipeline pipeline) {
  return out << (optimized(pipeline) ? "optimized" : "unoptimized");
}

std::string codegen(const std::string& source, Pipeline pipeline) { return codegen(source, optimized(pipeline)); }

std::string codegen(const std::string& source, const Options& options) {
  return run_source(Stage::kCodegen, source, options);
}

Error compile_error(Stage stage, const std::string& source, const Options& options) {
  Compiled c = compile(stage, source, options);
  if (!c.error) throw std::runtime_error("the program compiled without an error");
  return *c.error;
}

bool is_a(std::string_view type, std::string_view base) {
  if (base == "Exception") return true;
  for (;;) {
    if (type == base) return true;
    const auto it = parents().find(type);
    if (it == parents().end()) return false;
    type = it->second;
  }
}

bool is_source_error(std::string_view type) {
  static const char* const kNames[] = {
      "LexError", "ParserError", "PreprocessorError", "LarkError",
      "IdentifierResolutionError", "LabelResolutionError", "LoopLabelingError", "UnrollError",
      "ConstantExpressionError", "TypeCheckError", "AbiSelectionError", "RegallocError",
      "ZpSlotAllocationError", "ZpLocalAllocationError", "FrameSizeError",
      "RecursionSaveError", "NotImplementedError",
  };
  for (const char* name : kNames) {
    if (is_a(type, name)) return true;
  }
  return false;
}

Tac tac_program(const std::string& source, const Options& options) {
  const passes::ir_check::Enabled checks(options.check_ir);
  std::string err;
  std::vector<diagnostics::CompileWarning> found;
  try {
    diagnostics::Collecting block(found);
    const std::string text = passes::preprocess_source(source, front_options(options), err);
    passes::TacStage t = passes::tac_stage(text, options.optimize, options.inline_calls);
    return Tac{std::move(t.tac), std::move(t.symbols), std::move(t.types)};
  } catch (const CompileError& e) {
    throw rejected(Error{e.python_type(), e.what()});
  }
}

TacOutcome tac_main(const std::string& source) {
  Options defaults;
  std::string err;
  std::vector<diagnostics::CompileWarning> found;
  TacOutcome out;
  try {
    diagnostics::Collecting block(found);
    tac_sim_modes::Translated t =
        tac_sim_modes::translate(source, front_options(defaults), err, false);
    const tac_sim::Outcome o = tac_sim::run_main(t.tac, t.symbols, &t.types);
    if (o.error_type) {
      out.error = Error{*o.error_type, o.error_message};
    } else {
      out.value = o.value;
    }
  } catch (const CompileError& e) {
    out.error = Error{e.python_type(), e.what()};
  }
  return out;
}

std::optional<asdl::Int> run_program(const std::string& source) {
  TacOutcome o = tac_main(source);
  if (o.error) throw std::runtime_error("main raised " + o.error->type + ": " + o.error->message);
  return o.value;
}

compile_command::Outcome compile_main(const std::vector<std::string>& argv,
                                      const std::string& stdin_text,
                                      const MainOptions& options) {
  compile_command::Options o;
  o.include_dir = repo_path("include");
  o.columns = options.columns;
  o.inline_calls = options.inline_calls;
  o.check_ir = options.check_ir;
  auto rest = std::make_shared<std::string>(stdin_text);
  o.read_stdin = [rest] { return std::exchange(*rest, std::string()); };
  return compile_command::run(argv, o);
}

ToolOutcome nm_main(const std::vector<std::string>& argv) {
  ToolOutcome out;
  out.status = binutils::nm::main(argv, binutils::nm::read_path, 80, out.out, out.err);
  return out;
}

ToolOutcome objdump_main(const std::vector<std::string>& argv) {
  ToolOutcome out;
  out.status = binutils::objdump::main(argv, binutils::nm::read_path, 80, out.out, out.err);
  return out;
}

TempDir::TempDir() {
  static std::atomic<unsigned> counter{0};
  const auto now = std::chrono::steady_clock::now().time_since_epoch().count();
  for (;;) {
    path_ = std::filesystem::temp_directory_path() /
            ("c6502_tests_" + std::to_string(now) + "_" + std::to_string(counter++));
    if (std::filesystem::create_directory(path_)) return;
  }
}

TempDir::~TempDir() {
  std::error_code ec;
  std::filesystem::remove_all(path_, ec);
}

std::string TempDir::file(std::string_view name) const {
  return compile_command::path_text(path_ / compile_command::utf8_path(name));
}

ChangeDir::ChangeDir(const std::filesystem::path& dir) : previous_(std::filesystem::current_path()) {
  std::filesystem::current_path(dir);
}

ChangeDir::~ChangeDir() {
  std::error_code ec;
  std::filesystem::current_path(previous_, ec);
}

void write_file(const std::string& path, std::string_view data) {
  std::ofstream f(compile_command::utf8_path(path), std::ios::binary);
  f.write(data.data(), static_cast<std::streamsize>(data.size()));
  if (!f) throw std::runtime_error("can't write " + path);
}

std::string read_file(const std::string& path) {
  std::ifstream f(compile_command::utf8_path(path), std::ios::binary);
  if (!f) throw std::runtime_error("can't read " + path);
  std::ostringstream s;
  s << f.rdbuf();
  return s.str();
}

std::vector<std::string> sim_defsym_args() { return sim::defsym_args(); }

long long compile_object(const std::string& source, const std::string& out_path, bool optimize,
                         const std::vector<std::string>& extra) {
  std::vector<std::string> argv{"compile.py", "-", "-c"};
  if (optimize) argv.push_back("-O");
  argv.push_back("-o");
  argv.push_back(out_path);
  argv.insert(argv.end(), extra.begin(), extra.end());
  const compile_command::Outcome o = compile_main(argv, source);
  if (!o.status) throw std::runtime_error("compile.py raised " + o.raised->type);
  return *o.status;
}

compile_command::Outcome link(const std::vector<std::string>& paths, const std::string& out_path,
                              const std::optional<std::string>& script,
                              const std::vector<std::string>& extra) {
  std::vector<std::string> argv{"compile.py"};
  if (script) {
    argv.push_back("-T");
    argv.push_back(*script);
  }
  argv.insert(argv.end(), paths.begin(), paths.end());
  argv.push_back("-o");
  argv.push_back(out_path);
  for (std::string& d : sim_defsym_args()) argv.push_back(std::move(d));
  argv.insert(argv.end(), extra.begin(), extra.end());
  return compile_main(argv);
}

std::string listing(const std::string& exe_path) {
  const ToolOutcome o =
      objdump_main({"objdump.py", "-D", "--no-show-raw-insn", "--no-addresses", exe_path});
  if (o.status != 0) throw std::runtime_error("objdump failed: " + o.err);
  return o.out;
}

std::string linked_text(const std::string& exe_path) {
  const ToolOutcome meta = objdump_main({"objdump.py", "--link-metadata", exe_path});
  if (meta.status != 0) throw std::runtime_error("objdump failed: " + meta.err);
  return listing(exe_path) + "\n" + (meta.out == "(no link metadata)\n" ? "" : meta.out);
}

std::map<std::string, std::int64_t> symbols(const std::string& exe_path) {
  namespace elf = binutils::elf;
  const elf::ElfFile file = elf::read(read_file(exe_path), exe_path);
  std::map<std::string, std::int64_t> out;
  for (const auto& s : file.symbols) {
    if (s->defined() && s->type != elf::STT_FILE) {
      if (s->is_global() || out.count(s->name) == 0) out[s->name] = s->value;
    }
  }
  return out;
}

std::vector<std::string> function_body(const std::string& asm_text, std::string_view name) {
  const std::string label = "\n" + std::string(name) + ":\n";
  const std::size_t at = asm_text.find(label);
  if (at == std::string::npos) {
    throw std::runtime_error("no function " + std::string(name) + " in the -S text");
  }
  std::vector<std::string> out;
  for (const std::string& raw : lines(std::string_view(asm_text).substr(at + label.size()))) {
    std::istringstream words(raw);
    std::string line, word;
    while (words >> word) line += (line.empty() ? "" : " ") + word;
    if (line.empty() || line[0] == ';' || line == "SUBROUTINE") continue;
    if (line.back() == ':' && line[0] != '.') break;
    out.push_back(line);
  }
  return out;
}

std::vector<std::string> lines(std::string_view text) {
  std::vector<std::string> out;
  std::size_t pos = 0;
  while (pos < text.size()) {
    std::size_t end = text.find('\n', pos);
    if (end == std::string_view::npos) end = text.size();
    std::string_view line = text.substr(pos, end - pos);
    if (!line.empty() && line.back() == '\r') line.remove_suffix(1);
    out.emplace_back(line);
    pos = end + 1;
  }
  return out;
}

bool contains(std::string_view text, std::string_view needle) {
  return text.find(needle) != std::string_view::npos;
}

std::size_t count(std::string_view text, std::string_view needle) {
  if (needle.empty()) return text.size() + 1;
  std::size_t n = 0;
  for (std::size_t at = text.find(needle); at != std::string_view::npos;
       at = text.find(needle, at + needle.size())) {
    ++n;
  }
  return n;
}

bool search(std::string_view pattern, std::string_view text) {
  const std::regex re{std::string(pattern)};
  return std::regex_search(text.begin(), text.end(), re);
}

}  // namespace c6502::suite
