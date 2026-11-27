// asdlgen writes the C++ IR classes of each .asdl file into a directory,
// and with --check compares the files there with what it would write.
//
//     asdlgen <output-dir> <input.asdl>...
//     asdlgen --check <output-dir> <input.asdl>...
//
// It rewrites a file only when its bytes differ, so that a build that
// runs it recompiles only what changed.
#include <cstdio>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <optional>
#include <string>
#include <vector>

#include "asdlgen.h"

namespace {

std::optional<std::string> read_bytes(const std::filesystem::path& path) {
  std::ifstream in(path, std::ios::binary);
  if (!in) {
    return std::nullopt;
  }
  return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

}  // namespace

int main(int argc, char** argv) {
  std::vector<std::string> args(argv + 1, argv + argc);
  const bool check = !args.empty() && args[0] == "--check";
  if (check) {
    args.erase(args.begin());
  }
  if (args.size() < 2) {
    std::fputs("usage: asdlgen [--check] <output-dir> <input.asdl>...\n", stderr);
    return 2;
  }
  const std::filesystem::path out_dir = args[0];
  int status = 0;
  for (std::size_t k = 1; k < args.size(); ++k) {
    const std::filesystem::path input = args[k];
    std::vector<std::pair<std::string, std::string>> files;
    try {
      const std::optional<std::string> text = read_bytes(input);
      if (!text) {
        throw c6502::asdlgen::Error("can't read the file");
      }
      const std::string source = input.filename().string();
      files = c6502::asdlgen::generate_cpp(c6502::asdlgen::parse(*text), source,
                                           c6502::asdlgen::cpp_module_name(source));
    } catch (const std::exception& e) {
      std::fprintf(stderr, "%s: %s\n", input.string().c_str(), e.what());
      return 1;
    }
    for (const auto& [name, text] : files) {
      const std::filesystem::path path = out_dir / name;
      if (read_bytes(path) == text) {
        continue;
      }
      if (check) {
        std::fprintf(stderr, "%s differs from what asdlgen writes from %s\n",
                     path.string().c_str(), input.string().c_str());
        status = 1;
        continue;
      }
      std::ofstream out(path, std::ios::binary);
      out.write(text.data(), static_cast<std::streamsize>(text.size()));
      if (!out) {
        std::fprintf(stderr, "%s: can't write the file\n", path.string().c_str());
        return 1;
      }
    }
  }
  return status;
}
