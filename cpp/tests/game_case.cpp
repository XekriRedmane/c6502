#include "game_case.h"

#include <filesystem>
#include <stdexcept>

namespace c6502::suite {

std::string build_example(const TempDir& tmp, std::string_view example, const std::vector<std::string>& units,
                          std::string_view script, Helpers helpers) {
  const std::string dir = "examples/" + std::string(example) + "/";
  std::vector<std::string> link{"compile.py", "-T", repo_path(dir + std::string(script))};
  for (const std::string& unit : units) {
    const std::string source = repo_path(dir + unit + ".c");
    if (!std::filesystem::exists(source)) continue;
    const std::string obj = tmp.file(unit + ".o");
    const auto o = compile_main({"compile.py", source, "-c", "-O", "-o", obj});
    if (o.status != 0) throw std::runtime_error(unit + ": " + o.err());
    link.push_back(obj);
  }
  if (helpers == Helpers::kSimulators) {
    for (const std::string& d : sim_defsym_args()) link.push_back(d);
  }
  const std::string exe = tmp.file(std::string(example));
  link.push_back("-o");
  link.push_back(exe);
  const auto o = compile_main(link);
  if (o.status != 0) throw std::runtime_error("the link failed: " + o.err());
  return exe;
}

}  // namespace c6502::suite
