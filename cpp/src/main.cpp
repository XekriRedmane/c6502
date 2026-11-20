// c6502cpp, the CLI of the C++ port. Until the port reaches the
// compiler's own command line, it holds the checks the port is
// measured by (cpp/README.md).
#include <algorithm>
#include <cstdio>
#include <exception>
#include <filesystem>
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
#include "c99_ast.h"
#include "capture.h"
#include "errors.h"
#include "helper_checks.h"
#include "json.h"
#include "passes/registry.h"
#include "tac_ast.h"

namespace {

namespace fs = std::filesystem;
using namespace c6502;

const char kUsage[] =
    "usage: c6502cpp roundtrip <module> [<file>]\n"
    "       c6502cpp check-captures [--pass <name>] [--verbose] <path>...\n"
    "       c6502cpp check-helpers [--verbose] <path>...\n"
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
    "compares them with the recorded ones.\n";

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

int cmd_check_captures(const std::vector<std::string>& args) {
  std::string only;
  bool verbose = false;
  std::vector<fs::path> files;
  for (std::size_t i = 0; i < args.size(); ++i) {
    if (args[i] == "--pass" && i + 1 < args.size()) {
      only = args[++i];
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
  std::size_t roundtrip_failures = 0;
  std::size_t mismatches = 0;
  for (const auto& path : files) {
    const capture::Call call = capture::parse(capture::read_file(path));
    if (!only.empty() && call.pass != only) {
      continue;
    }
    const std::string where = path.string() + " (" + call.source + ")";
    asm_ast::Program input;
    try {
      input = asdl::load<asm_ast::Program>(call.input);
      if (asdl::dump(input) != call.input) {
        throw std::runtime_error("the input's dump differs from the recorded one");
      }
      if (!call.output_is_json &&
          asdl::dump(asdl::load<asm_ast::Program>(call.output)) != call.output) {
        throw std::runtime_error("the output's dump differs from the recorded one");
      }
    } catch (const std::exception& e) {
      ++roundtrip_failures;
      std::printf("ROUNDTRIP %s: %s\n", where.c_str(), e.what());
      continue;
    }
    const passes::RegisteredPass* pass = passes::find_pass(call.pass);
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
      got = pass->run(std::move(input), capture::context(call),
                      positional ? *positional : empty_array,
                      keywords ? *keywords : empty_object);
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
