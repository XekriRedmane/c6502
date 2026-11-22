// c6502cpp, the CLI of the C++ port. Until the port reaches the
// compiler's own command line, it holds the checks the port is
// measured by (cpp/README.md) and the back end that compile.py runs
// through it under `C6502_CPP_BACKEND=1`.
#include <algorithm>
#include <cstdio>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

#include "asdl/runtime.h"
#include "asm2_ast.h"
#include "asm_ast.h"
#include "backend.h"
#include "binutils/checks.h"
#include "binutils/opcodes.h"
#include "c99_ast.h"
#include "capture.h"
#include "errors.h"
#include "helper_checks.h"
#include "json.h"
#include "passes/peephole_fixedpoint.h"
#include "passes/registry.h"
#include "passes/stage_registry.h"
#include "preprocessor_cases.h"
#include "py_regex_cases.h"
#include "stage_checks.h"
#include "tac_ast.h"

namespace {

namespace fs = std::filesystem;
using namespace c6502;

const char kUsage[] =
    "usage: c6502cpp roundtrip <module> [<file>]\n"
    "       c6502cpp check-captures [--pass <name>] [--verbose] [--got <dir>] <path>...\n"
    "       c6502cpp check-helpers [--verbose] <path>...\n"
    "       c6502cpp check-preprocess [--verbose] <path>...\n"
    "       c6502cpp check-regex [--verbose] <path>...\n"
    "       c6502cpp check-binutils [--verbose] <capture dir>...\n"
    "       c6502cpp opcodes\n"
    "       c6502cpp backend\n"
    "       c6502cpp sweep-passes\n"
    "\n"
    "roundtrip reads a dump of an IR value of <module> (asm_ast, asm2_ast,\n"
    "tac_ast, or c99_ast) from <file> or stdin and writes its dump again.\n"
    "\n"
    "check-captures reads the capture files under each <path> (a file or a\n"
    "directory), checks that each one's input and output dumps read and\n"
    "write back as the same bytes, and runs each ported pass on its\n"
    "recorded input, checking the result against the recorded output.\n"
    "\n"
    "check-helpers reads the records of tools/capture_helpers.py under each\n"
    "<path>, computes the values of the shared asm helpers in C++, and\n"
    "compares them with the recorded ones.\n"
    "\n"
    "check-preprocess runs the preprocessor on each recorded call of\n"
    "preprocessor.preprocess in the .json files under each <path>, with the\n"
    "files the record holds, and compares the text, the error, the\n"
    "warnings, and stderr with Python's (src/preprocessor_cases.h).\n"
    "\n"
    "check-regex reads records of Python's re, one JSON object per line of\n"
    "each file under each <path> (src/py_regex_cases.h), and compares\n"
    "py_regex's compile error or matches with Python's, counting apart the\n"
    "patterns it reports as unsupported.\n"
    "\n"
    "backend reads a request of compile.py's (cpp_backend.py) from stdin,\n"
    "compiles its source text from the preprocessor to the finished asm\n"
    "program or the -S text, and writes the result to stdout. A problem\n"
    "with the program being compiled is part of the response, with the\n"
    "Python type of the error. A problem with the request exits with\n"
    "status 1 and a bug in the compiler with status 3, each with its\n"
    "message on stderr.\n"
    "\n"
    "check-binutils checks the records of tools/capture_binutils.py under\n"
    "each capture directory (src/binutils/checks.h).\n"
    "\n"
    "opcodes lists the 6502 opcode table, one `mnemonic mode opcode` line\n"
    "per row, in the order of binutils/opcodes.py.\n"
    "\n"
    "sweep-passes lists the passes of the peephole fixed point's sweep, in\n"
    "order, one per line.\n";

std::string read_stdin() {
#ifdef _WIN32
  _setmode(_fileno(stdin), _O_BINARY);
#endif
  return std::string(std::istreambuf_iterator<char>(std::cin),
                     std::istreambuf_iterator<char>());
}

void write_stdout(const std::string& text) {
#ifdef _WIN32
  _setmode(_fileno(stdout), _O_BINARY);
#endif
  std::fwrite(text.data(), 1, text.size(), stdout);
}

// Returns the dump of the value of module `module` that `text` holds,
// read and written again.
std::string roundtrip(std::string_view module, const std::string& text) {
  // Each module's dumps start with the name of a constructor of
  // `program`, `Program`, for every value the tests round-trip.
  if (module == "asm_ast") {
    return asdl::dump(asdl::load<asm_ast::Program>(text));
  }
  if (module == "asm2_ast") {
    return asdl::dump(asdl::load<asm2_ast::Program>(text));
  }
  if (module == "tac_ast") {
    return asdl::dump(asdl::load<tac_ast::Program>(text));
  }
  if (module == "c99_ast") {
    return asdl::dump(asdl::load<c99_ast::Program>(text));
  }
  throw std::runtime_error("unknown module " + std::string(module));
}

int cmd_roundtrip(const std::vector<std::string>& args) {
  if (args.empty() || args.size() > 2) {
    std::fputs(kUsage, stderr);
    return 2;
  }
  const std::string text =
      args.size() == 2 ? capture::read_file(args[1]) : read_stdin();
  write_stdout(roundtrip(args[0], text));
  return 0;
}

// Returns the 1-based number of the first line where `a` and `b`
// differ.
std::size_t first_difference(const std::string& a, const std::string& b) {
  const auto n = std::min(a.size(), b.size());
  std::size_t line = 1;
  for (std::size_t i = 0; i < n; ++i) {
    if (a[i] != b[i]) {
      return line;
    }
    if (a[i] == '\n') {
      ++line;
    }
  }
  return line;
}

struct PassCount {
  std::size_t calls = 0;
  std::size_t matched = 0;
};

// Returns whether the program dump `text` of the IR module `module`
// (`asm_ast` or `asm2_ast`) reads and writes back as the same bytes.
bool dump_roundtrips(const std::string& text, std::string_view module) {
  if (module == "asm2_ast") {
    return asdl::dump(asdl::load<asm2_ast::Program>(text)) == text;
  }
  return asdl::dump(asdl::load<asm_ast::Program>(text)) == text;
}

// The count of one stage call's records that `check-captures` checked:
// those whose values all made the trip to C++ and back, and for a
// ported call those whose results matched.
struct StageCount {
  std::size_t calls = 0;
  std::size_t round_tripped = 0;
  bool ported = false;
  std::size_t matched = 0;
};

int cmd_check_captures(const std::vector<std::string>& args) {
  std::string only;
  bool verbose = false;
  // Where to write the output of each stage call that doesn't match, as
  // a file named for its record, or empty.
  fs::path got_dir;
  std::vector<fs::path> files;
  for (std::size_t i = 0; i < args.size(); ++i) {
    if (args[i] == "--pass" && i + 1 < args.size()) {
      only = args[++i];
    } else if (args[i] == "--got" && i + 1 < args.size()) {
      got_dir = args[++i];
    } else if (args[i] == "--verbose") {
      verbose = true;
    } else if (fs::is_directory(args[i])) {
      for (const auto& entry : fs::recursive_directory_iterator(args[i])) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
          files.push_back(entry.path());
        }
      }
    } else if (fs::is_regular_file(args[i])) {
      files.emplace_back(args[i]);
    } else {
      std::fprintf(stderr, "c6502cpp: no such file or directory: %s\n",
                   args[i].c_str());
      return 2;
    }
  }
  std::sort(files.begin(), files.end());
  std::map<std::string, PassCount> ported;
  std::map<std::string, std::size_t> unported;
  std::map<std::string, StageCount> stages;
  std::size_t roundtrip_failures = 0;
  std::size_t mismatches = 0;
  for (const auto& path : files) {
    const capture::Call call = capture::parse(capture::read_file(path));
    if (!only.empty() && call.pass != only) {
      continue;
    }
    const std::string where = path.string() + " (" + call.source + ")";
    if (const passes::StageSignature* sig = passes::find_stage(call.pass)) {
      StageCount& count = stages[call.pass];
      ++count.calls;
      const stage_checks::Result r = stage_checks::check(call, *sig);
      if (r.tables_error.empty()) {
        ++count.round_tripped;
      } else {
        ++roundtrip_failures;
        std::printf("ROUNDTRIP %s: %s\n", where.c_str(), r.tables_error.c_str());
      }
      count.ported = r.ported;
      if (r.matched) {
        ++count.matched;
        if (verbose) {
          std::printf("ok %s\n", where.c_str());
        }
      } else if (r.ported) {
        ++mismatches;
        std::printf("MISMATCH %s: %s, %s\n", where.c_str(), call.pass.c_str(),
                    r.mismatch.c_str());
        if (!got_dir.empty() && !r.got.empty()) {
          fs::create_directories(got_dir);
          std::ofstream(got_dir / path.filename(), std::ios::binary) << r.got;
        }
      }
      continue;
    }
    const passes::RegisteredPass* pass = passes::find_pass(call.pass);
    // A function whose input or output isn't an asm program names the
    // module of its dump.
    const std::string_view input_module = pass != nullptr ? pass->input_module : "asm_ast";
    const std::string_view output_module = pass != nullptr ? pass->output_module : "asm_ast";
    asm_ast::Program input;
    try {
      if (!call.input_is_json && input_module == "asm_ast") {
        input = asdl::load<asm_ast::Program>(call.input);
        if (asdl::dump(input) != call.input) {
          throw std::runtime_error("the input's dump differs from the recorded one");
        }
      } else if (!call.input_is_json && !dump_roundtrips(call.input, input_module)) {
        throw std::runtime_error("the input's dump differs from the recorded one");
      }
      if (!call.output_is_json && !dump_roundtrips(call.output, output_module)) {
        throw std::runtime_error("the output's dump differs from the recorded one");
      }
    } catch (const std::exception& e) {
      ++roundtrip_failures;
      std::printf("ROUNDTRIP %s: %s\n", where.c_str(), e.what());
      continue;
    }
    if (pass == nullptr) {
      ++unported[call.pass];
      continue;
    }
    PassCount& count = ported[call.pass];
    ++count.calls;
    std::string got;
    try {
      const json::Value call_args = json::parse(call.args_json);
      const json::Value empty_array{.kind = json::Value::Kind::kArray};
      const json::Value empty_object{.kind = json::Value::Kind::kObject};
      const json::Value* positional = call_args.get("args");
      const json::Value* keywords = call_args.get("kwargs");
      if (pass->run_text != nullptr) {
        got = pass->run_text(call.input, capture::context(call),
                             positional ? *positional : empty_array,
                             keywords ? *keywords : empty_object);
      } else {
        got = pass->run(std::move(input), capture::context(call),
                        positional ? *positional : empty_array,
                        keywords ? *keywords : empty_object);
      }
    } catch (const std::exception& e) {
      got = std::string("exception: ") + e.what() + "\n";
    }
    if (got == call.output && pass->output_is_json == call.output_is_json) {
      ++count.matched;
      if (verbose) {
        std::printf("ok %s\n", where.c_str());
      }
    } else {
      ++mismatches;
      std::printf("MISMATCH %s: %s, first difference on line %zu\n",
                  where.c_str(), call.pass.c_str(),
                  first_difference(got, call.output));
    }
  }
  for (const auto& [name, count] : ported) {
    std::printf("%s: %zu calls, %zu match, %zu differ\n", name.c_str(),
                count.calls, count.matched, count.calls - count.matched);
  }
  for (const auto& [name, count] : stages) {
    std::printf("%s: %zu calls, %zu round-trip", name.c_str(), count.calls,
                count.round_tripped);
    if (count.ported) {
      std::printf(", %zu match, %zu differ", count.matched, count.calls - count.matched);
    }
    std::printf("\n");
  }
  std::size_t unported_calls = 0;
  for (const auto& [name, n] : unported) {
    unported_calls += n;
  }
  if (unported_calls != 0) {
    std::printf("%zu calls of %zu passes not ported yet (dumps round-tripped)\n",
                unported_calls, unported.size());
  }
  if (roundtrip_failures != 0) {
    std::printf("%zu captures whose dumps didn't round-trip\n",
                roundtrip_failures);
  }
  return (mismatches == 0 && roundtrip_failures == 0) ? 0 : 1;
}

// The count of one helper's values that `check-helpers` compared.
struct HelperCount {
  std::size_t values = 0;
  std::size_t records = 0;
  std::size_t mismatches = 0;
};

// Returns the 0-based offset of the first character where `a` and `b`
// differ.
std::size_t first_char_difference(const std::string& a, const std::string& b) {
  std::size_t i = 0;
  while (i < a.size() && i < b.size() && a[i] == b[i]) {
    ++i;
  }
  return i;
}

// Collects the `.txt` files under each path, sorted.
bool collect_files(const std::vector<std::string>& paths,
                   std::vector<fs::path>& files) {
  for (const auto& p : paths) {
    if (fs::is_directory(p)) {
      for (const auto& entry : fs::recursive_directory_iterator(p)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
          files.push_back(entry.path());
        }
      }
    } else if (fs::is_regular_file(p)) {
      files.emplace_back(p);
    } else {
      std::fprintf(stderr, "c6502cpp: no such file or directory: %s\n", p.c_str());
      return false;
    }
  }
  std::sort(files.begin(), files.end());
  return true;
}

int cmd_check_helpers(const std::vector<std::string>& args) {
  bool verbose = false;
  std::vector<std::string> paths;
  for (const auto& a : args) {
    if (a == "--verbose") {
      verbose = true;
    } else {
      paths.push_back(a);
    }
  }
  std::vector<fs::path> files;
  if (!collect_files(paths, files)) {
    return 2;
  }
  std::map<std::string, HelperCount> counts;
  std::size_t failures = 0;
  std::size_t records = 0;
  for (const auto& path : files) {
    const std::string text = capture::read_file(path);
    try {
      // The header: the version, the source, the context of a function
      // record, and the kind of the dump that follows.
      std::size_t pos = 0;
      const auto line = [&]() {
        const std::size_t end = text.find('\n', pos);
        if (end == std::string::npos) {
          throw std::runtime_error("truncated record");
        }
        std::string l = text.substr(pos, end - pos);
        pos = end + 1;
        return l;
      };
      if (line() != "c6502-helpers 1") {
        throw std::runtime_error("not a helper record");
      }
      const std::string source = line().substr(7);
      std::string kind = line();
      std::string context;
      if (kind.rfind("context ", 0) == 0) {
        context = kind.substr(8);
        kind = line();
      }
      const std::size_t values_at = text.find("\nvalues\n", pos - 1);
      if (values_at == std::string::npos) {
        throw std::runtime_error("no values section");
      }
      const std::string dump = text.substr(pos, values_at + 1 - pos);
      helper_checks::Values recorded;
      std::size_t vpos = values_at + 8;
      while (vpos < text.size()) {
        const std::size_t end = text.find('\n', vpos);
        const std::string l = text.substr(vpos, end - vpos);
        const std::size_t sp = l.find(' ');
        recorded.emplace_back(l.substr(0, sp), l.substr(sp + 1));
        vpos = end + 1;
      }
      helper_checks::Values got;
      if (kind == "function") {
        const auto fn = std::get<asm_ast::Function>(
            asdl::load<asm_ast::Type_top_level>(dump));
        got = helper_checks::function_values(
            fn, helper_checks::parse_context(json::parse(context)));
      } else if (kind == "program") {
        got = helper_checks::program_values(asdl::load<asm_ast::Program>(dump));
      } else if (kind == "tac") {
        const auto fn = std::get<tac_ast::Function>(
            asdl::load<tac_ast::Type_top_level>(dump));
        got = helper_checks::tac_values(fn);
      } else if (kind == "tables") {
        got = helper_checks::stage_values(json::parse(dump));
      } else if (kind == "pseudos") {
        got = helper_checks::pseudo_values(json::parse(dump));
      } else if (kind == "tacopt") {
        got = helper_checks::tac_opt_values(json::parse(dump));
      } else if (kind == "c99types") {
        got = helper_checks::front_values(json::parse(dump));
      } else {
        throw std::runtime_error("unknown record kind " + kind);
      }
      ++records;
      std::map<std::string, std::string> mine(got.begin(), got.end());
      for (const auto& [name, value] : recorded) {
        HelperCount& count = counts[name];
        ++count.records;
        count.values += helper_checks::value_count(name, value);
        const auto it = mine.find(name);
        if (it == mine.end()) {
          ++count.mismatches;
          std::printf("MISSING %s (%s): %s\n", path.string().c_str(),
                      source.c_str(), name.c_str());
        } else if (it->second != value) {
          ++count.mismatches;
          std::printf("MISMATCH %s (%s): %s differs at character %zu\n",
                      path.string().c_str(), source.c_str(), name.c_str(),
                      first_char_difference(it->second, value));
        } else if (verbose) {
          std::printf("ok %s %s\n", path.string().c_str(), name.c_str());
        }
      }
      if (got.size() != recorded.size()) {
        ++failures;
        std::printf("COUNT %s: %zu values recorded, %zu computed\n",
                    path.string().c_str(), recorded.size(), got.size());
      }
    } catch (const std::exception& e) {
      ++failures;
      std::printf("ERROR %s: %s\n", path.string().c_str(), e.what());
    }
  }
  std::size_t mismatches = 0;
  for (const auto& [name, count] : counts) {
    std::printf("%s: %zu values in %zu records, %zu records differ\n",
                name.c_str(), count.values, count.records, count.mismatches);
    mismatches += count.mismatches;
  }
  std::printf("%zu records checked\n", records);
  return (mismatches == 0 && failures == 0) ? 0 : 1;
}

// `check-preprocess`: checks each recorded call of the preprocessor
// (src/preprocessor_cases.h), one JSON record per `.json` file.
int cmd_check_preprocess(const std::vector<std::string>& args) {
  bool verbose = false;
  std::vector<fs::path> files;
  for (const auto& a : args) {
    if (a == "--verbose") {
      verbose = true;
    } else if (fs::is_directory(a)) {
      for (const auto& entry : fs::recursive_directory_iterator(a)) {
        if (entry.path().extension() == ".json") files.push_back(entry.path());
      }
    } else {
      files.push_back(a);
    }
  }
  std::sort(files.begin(), files.end());
  std::size_t mismatches = 0;
  for (const auto& path : files) {
    std::ifstream in(path, std::ios::binary);
    const std::string text((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    std::string mismatch;
    try {
      mismatch = preprocessor::check_recorded_call(text);
    } catch (const std::exception& e) {
      mismatch = std::string("exception: ") + e.what();
    }
    if (!mismatch.empty()) {
      ++mismatches;
      std::printf("MISMATCH %s: %s\n", path.string().c_str(),
                  verbose ? mismatch.c_str() : mismatch.substr(0, mismatch.find('\n')).c_str());
    }
  }
  std::printf("preprocess: %zu records, %zu match, %zu differ\n", files.size(),
              files.size() - mismatches, mismatches);
  return mismatches == 0 ? 0 : 1;
}

// `check-regex`: checks each record of Python's `re` in the files under
// `args` (src/py_regex_cases.h), one JSON object per line.
int cmd_check_regex(const std::vector<std::string>& args) {
  bool verbose = false;
  std::vector<fs::path> files;
  for (const auto& a : args) {
    if (a == "--verbose") {
      verbose = true;
    } else if (fs::is_directory(a)) {
      for (const auto& entry : fs::recursive_directory_iterator(a)) {
        if (entry.is_regular_file()) files.push_back(entry.path());
      }
    } else {
      files.push_back(a);
    }
  }
  std::sort(files.begin(), files.end());
  std::size_t records = 0;
  std::size_t unsupported = 0;
  std::size_t mismatches = 0;
  for (const auto& path : files) {
    std::ifstream in(path, std::ios::binary);
    std::string line;
    while (std::getline(in, line)) {
      if (line.empty()) continue;
      ++records;
      py_regex::RecordCheck r;
      try {
        r = py_regex::check_record(line);
      } catch (const std::exception& e) {
        r = {py_regex::RecordCheck::Outcome::kDiffer, std::string("exception: ") + e.what()};
      }
      if (r.outcome == py_regex::RecordCheck::Outcome::kUnsupported) {
        ++unsupported;
        if (verbose) {
          std::printf("UNSUPPORTED %s: %s\n", line.substr(0, 200).c_str(), r.detail.c_str());
        }
      } else if (r.outcome == py_regex::RecordCheck::Outcome::kDiffer) {
        ++mismatches;
        std::printf("MISMATCH %s: %s\n", line.substr(0, 300).c_str(), r.detail.c_str());
      }
    }
  }
  std::printf("regex: %zu records, %zu match, %zu unsupported, %zu differ\n", records,
              records - unsupported - mismatches, unsupported, mismatches);
  return mismatches == 0 ? 0 : 1;
}

}  // namespace

int main(int argc, char** argv) {
  if (argc < 2) {
    std::fputs(kUsage, stderr);
    return 2;
  }
  const std::string command = argv[1];
  const std::vector<std::string> args(argv + 2, argv + argc);
  try {
    if (command == "roundtrip") {
      return cmd_roundtrip(args);
    }
    if (command == "check-captures") {
      return cmd_check_captures(args);
    }
    if (command == "check-helpers") {
      return cmd_check_helpers(args);
    }
    if (command == "check-preprocess") {
      return cmd_check_preprocess(args);
    }
    if (command == "check-regex") {
      return cmd_check_regex(args);
    }
    if (command == "check-binutils") {
      return binutils::check_binutils(args);
    }
    if (command == "opcodes" && args.empty()) {
      std::string lines;
      for (const auto& e : binutils::opcodes::table()) {
        char op[8];
        std::snprintf(op, sizeof op, "0x%02X", e.opcode);
        lines += std::string(e.mnemonic) + " " + std::string(binutils::opcodes::mode_name(e.mode)) +
                 " " + op + "\n";
      }
      write_stdout(lines);
      return 0;
    }
    if (command == "backend" && args.empty()) {
      // A problem with the program goes back in the response with its
      // Python type, so that cpp_backend.py raises the exception
      // compile.py would.
      write_stdout(backend::respond(read_stdin()));
      return 0;
    }
    if (command == "sweep-passes" && args.empty()) {
      std::string names;
      for (const auto& p : passes::peephole_sweep(passes::AsmContext{}, nullptr, nullptr, {})) {
        names += std::string(p.name) + "\n";
      }
      write_stdout(names);
      return 0;
    }
    if (command == "--help" || command == "-h") {
      std::fputs(kUsage, stdout);
      return 0;
    }
  } catch (const c6502::CompileError& e) {
    std::fprintf(stderr, "error: %s\n", e.what());
    return 1;
  } catch (const c6502::InternalError& e) {
    std::fprintf(stderr, "internal error: %s\n", e.what());
    return 3;
  } catch (const std::exception& e) {
    std::fprintf(stderr, "c6502cpp: %s\n", e.what());
    return 1;
  }
  std::fputs(kUsage, stderr);
  return 2;
}
