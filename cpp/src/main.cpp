// c6502cpp, the CLI of the C++ port. Until the port reaches the
// compiler's own command line, it holds the checks the port is
// measured by (cpp/README.md).
#include <cstdio>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
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
#include "errors.h"
#include "tac_ast.h"

namespace {

using namespace c6502;

const char kUsage[] =
    "usage: c6502cpp roundtrip <module> [<file>]\n"
    "\n"
    "roundtrip reads a dump of an IR value of <module> (asm_ast, asm2_ast,\n"
    "tac_ast, or c99_ast) from <file> or stdin and writes its dump again.\n";

std::string read_file(const std::string& path) {
  std::ifstream in(path, std::ios::binary);
  if (!in) {
    throw std::runtime_error("can't read " + path);
  }
  std::ostringstream ss;
  ss << in.rdbuf();
  return ss.str();
}

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
      args.size() == 2 ? read_file(args[1]) : read_stdin();
  write_stdout(roundtrip(args[0], text));
  return 0;
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
