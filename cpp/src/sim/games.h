// The scripts the three games are played with and the machines they
// are played on, as tests/test_zork_interpreter.py, test_lode_runner.py,
// and test_ali_baba.py set them up, for the doctest cases and for
// `c6502cpp game-run` (sim/game_run.h). The games' files come from the
// reverse-engineering projects, which aren't part of this repository,
// through ZORK_DSK, LODE_RUNNER_DIR, and ALI_BABA_DIR.
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "binutils/elf.h"
#include "sim/ali_baba.h"
#include "sim/apple2.h"
#include "sim/cpu.h"
#include "sim/lode_runner.h"
#include "sim/runtime.h"

namespace c6502::sim::games {

using Bytes = std::vector<std::uint8_t>;

// Returns the value of the environment variable `name`, or none when it
// isn't set or is empty.
std::optional<std::string> env(const char* name);

// Returns the bytes of the file at `path`. Throws `std::runtime_error`
// naming the path when it can't be read.
Bytes read_bytes(const std::string& path);
std::string read_text(const std::string& path);

// ---- Zork ----

// A session: the lines GETLN1 reads, and the keys RDKEY returns.
struct ZorkSession {
  std::string name;
  std::vector<std::string> lines;
  std::string keys;
};
const std::vector<ZorkSession>& zork_sessions();
const ZorkSession& zork_session(std::string_view name);

// A session's machine: the CPU, the hooks of the runtime the program was
// loaded with (none for the original), and the Apple II. The machine
// holds the CPU by reference, so it never moves.
struct ZorkRun {
  Cpu cpu;
  std::map<std::uint16_t, Hook> hooks;
  apple2::Apple2 machine{cpu};

  ZorkRun() = default;
  ZorkRun(const ZorkRun&) = delete;
  ZorkRun& operator=(const ZorkRun&) = delete;

  apple2::Stop run() { return machine.run(2'000'000'000, &hooks); }
};

// Returns Zork's own interpreter, which its boot loader reads from 26
// sectors of track 1 on into $0800.
Bytes zork_main(const Bytes& dsk);

// The original and the linked C program `exe`, on the stand-in Apple II
// as Zork's boot loader leaves it: the screen set up, DEBUG_JUMP an RTS,
// and SECTORS_PER_TRACK 16, with the game's disk in slot 6, drive 1,
// and an empty save disk in drive 2.
std::unique_ptr<ZorkRun> zork_original(const Bytes& dsk, const ZorkSession& session);
std::unique_ptr<ZorkRun> zork_program(std::string_view exe, const Bytes& dsk, const ZorkSession& session);

// ---- Lode Runner ----

// The disk a script plays on (`lode_runner_disk`).
enum class LodeRunnerDisk { kMaster, kGame, kEditor };

// A script is the keys, and the GAME_MODE change that ends the run.
struct LodeRunnerScript {
  std::string name;
  std::vector<lode_runner::KeyPress> keys;
  std::uint8_t mode_before;
  std::uint8_t mode_after;
  LodeRunnerDisk disk;
  std::uint64_t max_cycles;
};
const std::vector<LodeRunnerScript>& lode_runner_scripts();
const LodeRunnerScript& lode_runner_script(std::string_view name);

// The files of lode_runner_reveng: main.bin (the original's 48K memory
// image), lode_runner.do (the game's disk, in DOS order), and main.asm.
struct LodeRunnerFiles {
  Bytes image;
  Bytes disk;
  std::map<std::int64_t, std::string> routines;
  static LodeRunnerFiles read(const std::string& dir);
};

// Returns the game's disk for a script. The master disk's high-score
// table holds scores from level 16 up, which no scripted game reaches,
// so the game runs on a copy with the table cleared. On the master disk
// the editor runs DONT_MANIPULATE_MASTER_DISK in place of each edit, so
// it runs on a copy that's a data disk, with the high-score sector's
// master flag cleared. Its table is kept, so that the game it plays gets
// no high score.
Bytes lode_runner_disk(const Bytes& master, LodeRunnerDisk disk);

// Returns a predicate that is true once GAME_MODE is `after`, having
// been `before`.
std::function<bool(const lode_runner::Machine&)> mode_change(std::uint8_t before, std::uint8_t after);

// ---- Ali Baba ----

inline constexpr std::string_view kAliBabaDisk = "Ali Baba and the Forty Thieves (4am and san inc crack).dsk";

// A script is the keys, the disk swaps, and the KBD read at which the
// run ends. `events` is the number of events the original makes.
struct AliBabaScript {
  std::string name;
  std::vector<ali_baba::KeyPress> keys;
  int end;
  std::size_t events;
  std::vector<ali_baba::DiskSwap> swaps;
  std::uint64_t max_cycles = 4'000'000'000;
};
const std::vector<AliBabaScript>& ali_baba_scripts();
const AliBabaScript& ali_baba_script(std::string_view name);

// The files of ali_baba_reveng after its build.py, read once, and the
// scenario disk that the save script writes on the original.
class AliBabaFiles {
 public:
  explicit AliBabaFiles(const std::string& root);

  Bytes main_bin;
  Bytes disk;
  std::map<std::int64_t, ali_baba::Label> labels;
  std::map<std::int64_t, std::string> routines;

  // Returns the disks a script swaps in besides the game's: an
  // unformatted one for the save, and for the load, the one the save
  // script writes on the original.
  ali_baba::DiskImages disks(const AliBabaScript& script);
  const Bytes& scenario();

  // Returns the original ready to play `script`.
  std::unique_ptr<ali_baba::Machine> original(const AliBabaScript& script);
  // Returns the original after it has played `script`. Throws
  // `std::runtime_error` when the run doesn't end at the script's end.
  std::unique_ptr<ali_baba::Machine> run_original(const AliBabaScript& script);
  // Plays `script` on `machine`, an original, as `run_original` does.
  static void play_original(ali_baba::Machine& machine, const AliBabaScript& script);

 private:
  std::optional<Bytes> scenario_;
};

// The C program `exe`, linked by examples/ali_baba/ali_baba.ld, loaded
// over the original with the script's regions cleared, ready to play
// `script`.
std::unique_ptr<ali_baba::Machine> ali_baba_program(AliBabaFiles& files, const binutils::elf::ElfFile& exe,
                                                    const std::string& linker_script, const AliBabaScript& script);

}  // namespace c6502::sim::games
