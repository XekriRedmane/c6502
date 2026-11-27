// The cases run only with ALI_BABA_DIR set to the directory of
// https://github.com/XekriRedmane/ali_baba_reveng after its build.py.
#include <algorithm>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "doctest.h"
#include "game_case.h"
#include "rtlib/helpers.h"
#include "sim/ali_baba.h"
#include "sim/disk2.h"

namespace {

namespace suite = c6502::suite;
namespace games = c6502::sim::games;
namespace ali_baba = c6502::sim::ali_baba;
namespace elf = c6502::binutils::elf;

const bool kNoFiles = !games::env("ALI_BABA_DIR").has_value();

const std::vector<std::string> kUnits = {
    "main", "init", "turn", "input", "combat", "ai", "events", "entities", "map", "display", "util", "text", "sound",
    "hrcg",
};

games::AliBabaFiles& files() {
  static games::AliBabaFiles f(*games::env("ALI_BABA_DIR"));
  return f;
}

elf::ElfFile build(suite::Helpers helpers) {
  const suite::TempDir tmp;
  const std::string path = suite::build_example(tmp, "ali_baba", kUnits, "ali_baba.ld", helpers);
  return elf::read(suite::read_file(path), path);
}

const elf::ElfFile& exe() {
  static const elf::ElfFile e = build(suite::Helpers::kSimulators);
  return e;
}

const elf::ElfFile& readme_exe() {
  static const elf::ElfFile e = build(suite::Helpers::kRuntimeLibrary);
  return e;
}

struct DiskCall {
  int command;
  int track;
  int sector;
  bool operator==(const DiskCall&) const = default;
};

std::vector<DiskCall> rwts_calls(const ali_baba::Machine& machine) {
  std::vector<DiskCall> out;
  for (const auto& [_, r] : machine.events.disk) {
    if (!r.iob.empty()) out.push_back(DiskCall{r.iob.at(12), r.iob.at(4), r.iob.at(5)});
  }
  return out;
}

bool contains(const std::vector<DiskCall>& calls, DiskCall call) {
  return std::find(calls.begin(), calls.end(), call) != calls.end();
}

std::unique_ptr<ali_baba::Machine> run_original(const games::AliBabaScript& script) {
  std::unique_ptr<ali_baba::Machine> machine = files().run_original(script);
  CHECK(machine->events.size() == script.events);
  return machine;
}

void check_reached(const ali_baba::Machine& machine, const std::vector<std::string>& names) {
  const std::vector<std::string> reached = ali_baba::coverage(machine.seen, files().labels).reached();
  std::vector<std::string> missed;
  for (const std::string& name : names) {
    if (std::find(reached.begin(), reached.end(), name) == reached.end()) missed.push_back(name);
  }
  CHECK(missed == std::vector<std::string>{});
}

void compare(const elf::ElfFile& program, const games::AliBabaScript& script) {
  const std::unique_ptr<ali_baba::Machine> original = files().run_original(script);
  const std::size_t n = original->events.size();
  const std::unique_ptr<ali_baba::Machine> ours =
      games::ali_baba_program(files(), program, suite::read_repo_file("examples/ali_baba/ali_baba.ld"), script);
  ours->run(ours->cpu.cycles + 2 * script.max_cycles,
            [n](const ali_baba::Machine& m) { return m.events.size() >= n; });
  const ali_baba::Events& a = original->events;
  const ali_baba::Events& b = ours->events;
  std::optional<std::size_t> i = ali_baba::first_difference(a, b);
  if (!i && b.size() < n) i = b.size();
  if (i) {
    const std::map<std::int64_t, std::string> functions = c6502::sim::game::function_names(program);
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
  for (auto& [name, drive] : original->media) {
    INFO("the " << name << " disk");
    CHECK(ours->medium(name).to_dsk() == drive.to_dsk());
  }
}

}  // namespace

TEST_CASE("ali_baba: a disk call from $FFFD records its event when it returns to $0000") {
  c6502::sim::Cpu cpu;
  const std::uint8_t jsr[] = {0x20, 0x00, 0xB3};
  std::copy(std::begin(jsr), std::end(jsr), cpu.memory.begin() + 0xFFFD);
  cpu.memory[ali_baba::kDiskReadField] = 0x60;
  cpu.memory[0x0000] = 0x00;
  cpu.pc = 0xFFFD;
  std::vector<std::pair<std::string, c6502::sim::disk2::Drive>> media;
  media.emplace_back("game", c6502::sim::disk2::Drive::blank());
  ali_baba::Machine machine(std::move(cpu), std::move(media), {}, {});
  CHECK(machine.run(1'000) == ali_baba::End::kBrk);
  CHECK(machine.cpu.pc == 0x0000);
  REQUIRE(machine.events.size() == 1);
  CHECK(machine.events.events[0].code == (ali_baba::kDiskEvent + ali_baba::kDiskReadField) << 1);
  CHECK(machine.events.events[0].pc == 0xFFFD);
  CHECK(machine.events.disk.at(0).entry == ali_baba::kDiskReadField);
}

TEST_CASE("ali_baba: original: the attract mode plays the tune" * doctest::skip(kNoFiles)) {
  const auto machine = run_original(games::ali_baba_script("attract"));
  const std::map<std::string, std::size_t> counts = machine->events.counts();
  // The tune plays: the speaker clicks, and the only key polling is the
  // tune's.
  CHECK(counts.at("$C030 read") > 100'000);
  CHECK(counts.at("$C000 read") == 1500);
  check_reached(*machine, {"TUNE_ADVANCE", "PLAY_HIGH_TONE", "PLAY_LOW_TONE", "HRCG_COUT"});
}

TEST_CASE("ali_baba: original: a game reads the reincarnation scene" * doctest::skip(kNoFiles)) {
  const auto machine = run_original(games::ali_baba_script("game"));
  // The reincarnation scene comes from the disk.
  CHECK(contains(rwts_calls(*machine), {1, 0x13, 0xC}));
  check_reached(*machine,
                {"GAME_TURN_LOOP", "SCENE_LOOP", "TEXT_RENDERER", "DISPLAY_CHAR_STATS", "HANDLE_ENCOUNTER", "NPC_MOVE_AI"});
}

TEST_CASE("ali_baba: original: a save formats a scenario disk and writes the game" * doctest::skip(kNoFiles)) {
  const auto machine = run_original(games::ali_baba_script("save"));
  const games::Bytes scenario = machine->medium("scenario").to_dsk();
  // The signature, and the game's records in tracks 1 and 2.
  CHECK(games::Bytes(scenario.begin(), scenario.begin() + 3) == games::Bytes{0xC1, 0xCC, 0xC9});
  CHECK(games::Bytes(scenario.begin() + 16 * 256, scenario.begin() + 17 * 256) ==
        games::Bytes(machine->cpu.memory.begin() + 0x4000, machine->cpu.memory.begin() + 0x4100));
  const std::vector<DiskCall> calls = rwts_calls(*machine);
  CHECK(std::count_if(calls.begin(), calls.end(), [](const DiskCall& c) { return c.command == 2; }) == 27);
  // The game disk is only read.
  CHECK(machine->medium("game").to_dsk(&files().disk) == files().disk);
  check_reached(*machine, {"FORMAT_SCENARIO_DISK", "WRITE_GAME_DATA", "TOGGLE_SOUND", "SETTINGS_SPEED_UP"});
}

TEST_CASE("ali_baba: original: a load reads the saved game" * doctest::skip(kNoFiles)) {
  const auto machine = run_original(games::ali_baba_script("load"));
  std::vector<DiskCall> reads;
  for (const DiskCall& c : rwts_calls(*machine)) {
    if (c.command == 1) reads.push_back(c);
  }
  CHECK(contains(reads, {1, 1, 0}));
  CHECK(contains(reads, {1, 2, 12}));
  check_reached(*machine, {"CONTINUE_GAME", "READ_GAME_DATA"});
}

TEST_CASE("ali_baba: original: the forest's chests and shops" * doctest::skip(kNoFiles)) {
  const auto machine = run_original(games::ali_baba_script("forest"));
  check_reached(*machine, {"PICKUP_TREASURE", "DISPLAY_SHOP", "SHOP_BUY_HANDLER", "SHOP_BUY_EFFECT_2", "XP_AWARD_4",
                           "AWARD_XP", "DROP_GOLD_EXEC", "FLUSH_PENDING_XP", "ATTACK_UP", "FIND_ATTACK_TARGET",
                           "NEXT_COLOCATED_MOB"});
}

TEST_CASE("ali_baba: original: a second character joins and Ali Baba retires" * doctest::skip(kNoFiles)) {
  const auto machine = run_original(games::ali_baba_script("party"));
  // The save is cancelled, so nothing is written.
  const std::vector<DiskCall> calls = rwts_calls(*machine);
  CHECK(std::none_of(calls.begin(), calls.end(), [](const DiskCall& c) { return c.command == 2; }));
  check_reached(*machine, {"SLOT_INC_ENTER", "SLOT_DEC_ENTER", "PLAYER_ATTACK", "CHAR_SELECT_INIT", "CHAR_SELECT_NEXT",
                           "CHAR_SELECT_PREV", "CHAR_SELECT_HERE", "CHAR_SELECT_HOME", "INIT_CHAR_SELECT",
                           "SLOT_INC_REENTER", "SLOT_DEC_REENTER", "SCENARIO_INSERT_PROMPT", "CANCEL_SCENARIO",
                           "RETIRE_CHARACTER", "TRIGGER_SCENE_EVENT", "SHOW_LOCKED_MSG"});
}

TEST_CASE("ali_baba: program: the attract mode makes the original's events" * doctest::skip(kNoFiles)) {
  compare(exe(), games::ali_baba_script("attract"));
}

TEST_CASE("ali_baba: program: a game makes the original's events" * doctest::skip(kNoFiles)) {
  compare(exe(), games::ali_baba_script("game"));
}

TEST_CASE("ali_baba: program: a save makes the original's events and disks" * doctest::skip(kNoFiles)) {
  compare(exe(), games::ali_baba_script("save"));
}

TEST_CASE("ali_baba: program: a load makes the original's events" * doctest::skip(kNoFiles)) {
  compare(exe(), games::ali_baba_script("load"));
}

TEST_CASE("ali_baba: program: the forest makes the original's events" * doctest::skip(kNoFiles)) {
  compare(exe(), games::ali_baba_script("forest"));
}

TEST_CASE("ali_baba: program: the party makes the original's events" * doctest::skip(kNoFiles)) {
  compare(exe(), games::ali_baba_script("party"));
}

TEST_CASE("ali_baba: readme link: the program holds every helper it calls" * doctest::skip(kNoFiles)) {
  // The link fails when a region overflows, so the build checks the fit
  // too.
  std::vector<std::string> absolute;
  for (const auto& fn : c6502::rtlib::all_helper_functions()) {
    const elf::SymbolPtr s = readme_exe().symbol(fn.name);
    if (s != nullptr && s->is_absolute()) absolute.push_back(fn.name);
  }
  CHECK(absolute == std::vector<std::string>{});
}

TEST_CASE("ali_baba: readme link: a game makes the original's events" * doctest::skip(kNoFiles)) {
  compare(readme_exe(), games::ali_baba_script("game"));
}
