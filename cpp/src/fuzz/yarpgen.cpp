#include "fuzz/yarpgen.h"

#include <stdexcept>
#include <string_view>
#include <vector>

#include "compile_command.h"
#include "fuzz/bit_precise_types.h"
#include "processes.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace c6502::fuzz::yarpgen {

namespace {

namespace fs = std::filesystem;

const std::vector<std::string> kOptions = {
    "--std=c",          "--emit-align-attr=none", "--emit-pragmas=none", "--max-array-dims=1",
    "--inp-as-args=none", "--max-loop-iters=6",   "--max-stmts=25",
};

void replace_all(std::string& s, std::string_view from, std::string_view to) {
  for (std::size_t at = s.find(from); at != std::string::npos; at = s.find(from, at + to.size())) {
    s.replace(at, from.size(), to);
  }
}

long process_id() {
#ifdef _WIN32
  return static_cast<long>(GetCurrentProcessId());
#else
  return static_cast<long>(getpid());
#endif
}

class ScratchDir {
 public:
  explicit ScratchDir(const std::string& name) : path_(fs::temp_directory_path() / name) {
    std::error_code ec;
    fs::remove_all(path_, ec);
    fs::create_directories(path_);
  }
  ~ScratchDir() {
    std::error_code ec;
    fs::remove_all(path_, ec);
  }
  ScratchDir(const ScratchDir&) = delete;
  ScratchDir& operator=(const ScratchDir&) = delete;
  const fs::path& path() const { return path_; }

 private:
  fs::path path_;
};

std::string read(const fs::path& p) {
  try {
    return compile_command::read_text(compile_command::path_text(p));
  } catch (const compile_command::Raised& e) {
    throw std::runtime_error("can't read " + compile_command::path_text(p) + ": " + e.message);
  }
}

}  // namespace

fs::path binary(const fs::path& root) {
#ifdef _WIN32
  return root / "fuzz_out" / "yarpgen-src" / "build" / "yarpgen.exe";
#else
  return root / "fuzz_out" / "yarpgen-src" / "build" / "yarpgen";
#endif
}

std::string merge(std::string init, std::string func, std::string driver) {
  replace_all(func, "#include \"init.h\"", "");
  replace_all(driver, "#include \"init.h\"", "");
  replace_all(driver, "#include <stdio.h>", "");
  const std::string printf = "printf(\"%llu\\n\", seed);";
  if (driver.find(printf) == std::string::npos) {
    throw std::runtime_error("driver.c doesn't print the checksum");
  }
  replace_all(driver, printf,
              "return (int)((seed ^ (seed >> 16) ^ (seed >> 32) ^ (seed >> 48)) & 0x7FFF);");
  return "typedef unsigned int size_t;\n" + init + func + driver;
}

GeneratedProgram make_program(Int seed, const fs::path& exe) {
  const ScratchDir dir("c6502-yarpgen-" + std::to_string(process_id()) + "-" + decimal(seed));
  std::vector<std::string> argv = {compile_command::path_text(exe), "--seed=" + decimal(seed + 1),
                                   "--out-dir=" + compile_command::path_text(dir.path())};
  argv.insert(argv.end(), kOptions.begin(), kOptions.end());
  if (const int status = processes::run(argv, processes::Output::kDiscard); status != 0) {
    throw std::runtime_error(compile_command::path_text(exe) + " failed with status " +
                             std::to_string(status) + " on seed " + decimal(seed + 1));
  }
  return {merge(read(dir.path() / "init.h"), read(dir.path() / "func.c"),
                read(dir.path() / "driver.c")),
          std::nullopt};
}

}  // namespace c6502::fuzz::yarpgen
