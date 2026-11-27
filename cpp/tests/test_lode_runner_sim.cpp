// The cases run only with LODE_RUNNER_DIR set to the directory of
// https://github.com/XekriRedmane/lode_runner_reveng.
#include <memory>
#include <string>
#include <vector>

#include "doctest.h"
#include "game_case.h"

namespace {

namespace suite = c6502::suite;
namespace games = c6502::sim::games;
namespace lode_runner = c6502::sim::lode_runner;
namespace elf = c6502::binutils::elf;

const bool kNoFiles = !games::env("LODE_RUNNER_DIR").has_value();

const games::LodeRunnerFiles& files() {
  static const games::LodeRunnerFiles f = games::LodeRunnerFiles::read(*games::env("LODE_RUNNER_DIR"));
  return f;
}

const elf::ElfFile& exe() {
  static const elf::ElfFile e = [] {
    const suite::TempDir tmp;
    const std::string path = suite::build_example(tmp, "lode_runner",
                                                  {"main", "level", "player", "input", "guards", "timers", "screen",
                                                   "hiscore", "editor", "sprites", "sound", "wipe"},
                                                  "lode_runner.ld", suite::Helpers::kRuntimeLibrary);
    return elf::read(suite::read_file(path), path);
  }();
  return e;
}

lode_runner::Machine compare(const games::LodeRunnerScript& script) {
  const games::Bytes disk = games::lode_runner_disk(files().disk, script.disk);
  lode_runner::Machine original = lode_runner::original_machine(files().image, disk, script.keys);
  REQUIRE(original.run(script.max_cycles, games::mode_change(script.mode_before, script.mode_after)) ==
          lode_runner::End::kStop);
  const std::size_t n = original.events.size();
  lode_runner::Machine ours = lode_runner::program_machine(files().image, exe(), disk, script.keys);
  ours.run(ours.cpu.cycles + 2 * script.max_cycles,
           [n](const lode_runner::Machine& m) { return m.events.size() >= n; });
  const lode_runner::Events& a = original.events;
  const lode_runner::Events& b = ours.events;
  std::optional<std::size_t> i = lode_runner::first_difference(a, b);
  if (!i && b.size() < n) i = b.size();
  if (i) {
    const std::map<std::int64_t, std::string> functions = c6502::sim::game::function_names(exe());
    std::string lines = "the programs differ at event " + std::to_string(*i) + " of " + std::to_string(n);
    for (std::size_t j = *i >= 3 ? *i - 3 : 0; j <= *i; ++j) {
      if (j < a.size()) {
        lines += "\n  original: " + a.describe(j) + " in " +
                 c6502::sim::game::routine_at(files().routines, a.events[j].pc);
      }
      if (j < b.size()) {
        lines += "\n  program:  " + b.describe(j) + " in " + c6502::sim::game::routine_at(functions, b.events[j].pc);
      }
    }
    FAIL(lines);
  }
  CHECK(ours.disk == original.disk);
  return original;
}

std::vector<lode_runner::RwtsRecord> writes(const lode_runner::Machine& m) {
  std::vector<lode_runner::RwtsRecord> out;
  for (const auto& [_, r] : m.events.rwts) {
    if (r.command == 2) out.push_back(r);
  }
  return out;
}

}  // namespace

TEST_CASE("lode_runner: the demo makes the original's events" * doctest::skip(kNoFiles)) {
  const lode_runner::Machine original = compare(games::lode_runner_script("demo"));
  // The demo plays all three of its levels.
  CHECK(original.cpu.memory[0xA6] == 3);
}

TEST_CASE("lode_runner: a game makes the original's events" * doctest::skip(kNoFiles)) {
  const lode_runner::Machine original = compare(games::lode_runner_script("game"));
  // The high score was written, with the initials.
  const std::vector<lode_runner::RwtsRecord> w = writes(original);
  REQUIRE(w.size() == 1);
  CHECK(std::vector<std::uint8_t>(w[0].buffer.begin(), w[0].buffer.begin() + 3) ==
        std::vector<std::uint8_t>{0xC1, 0xC2, 0xC3});
}

TEST_CASE("lode_runner: the editor makes the original's events" * doctest::skip(kNoFiles)) {
  const lode_runner::Machine original = compare(games::lode_runner_script("editor"));
  // The edited level 1 and the cleared level 2 were written.
  std::vector<std::pair<int, int>> where;
  for (const lode_runner::RwtsRecord& r : writes(original)) where.emplace_back(r.track, r.sector);
  CHECK(where == std::vector<std::pair<int, int>>{{3, 0}, {3, 1}});
}
