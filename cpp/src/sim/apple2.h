// A stand-in Apple II for running Zork I's interpreter (the original,
// read from the game's disk image, and examples/zork) in the simulator,
// ported from sim/apple2.py. It has the Monitor's text routines, its
// keyboard and line input, a printer, and DOS 3.3's RWTS, each a hook
// at the routine's address.
//
// The hooks keep a 40-by-24 text screen as the ROM would. COUT1 writes
// a character at the cursor, inside the text window that WNDLFT,
// WNDWDTH, WNDTOP, and WNDBTM set, and scrolls the window when the
// cursor passes its bottom line. HOME and CLREOL clear. VTAB does
// nothing, since the screen is kept by line and column. RDKEY and
// GETLN1 take keys and lines from a script, and COUT goes to the screen
// or to the printer, depending on CSW. RWTS reads and writes 256-byte
// sectors of the disk images in `disks`, keyed by slot times 16 and
// drive.
//
// Every hook call is appended to `calls`, with the character it was
// given, if any, and the cursor and INVFLG it saw, so that two programs
// meant to behave the same can be compared call for call. The hooks
// take no cycles.
#pragma once

#include <cstdint>
#include <deque>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

#include "sim/cpu.h"
#include "sim/runtime.h"

namespace c6502::sim::apple2 {

inline constexpr std::uint16_t kHome = 0xFC58;
inline constexpr std::uint16_t kClreol = 0xFC9C;
inline constexpr std::uint16_t kVtab = 0xFC22;
inline constexpr std::uint16_t kRdkey = 0xFD0C;
inline constexpr std::uint16_t kGetln1 = 0xFD6F;
inline constexpr std::uint16_t kCout = 0xFDED;
inline constexpr std::uint16_t kCout1 = 0xFDF0;
inline constexpr std::uint16_t kRwts = 0x2900;
// The printer's output hook. Zork puts it in CSW while it prints to the
// printer, and a card in slot 1 has its entry point there.
inline constexpr std::uint16_t kPrinter = 0xC100;

// Returns the offset in a DOS-order .dsk image of logical `sector` of
// `track`.
std::size_t dsk_sector(int track, int sector);

enum class Routine { kHome, kClreol, kVtab, kRdkey, kGetln1, kCout, kCout1 };
std::string_view routine_name(Routine routine);

// A call of a Monitor routine, with A for COUT and COUT1, and the
// cursor and INVFLG as the routine found them.
struct MonitorCall {
  Routine routine;
  std::optional<std::uint8_t> a;
  std::uint8_t ch;
  std::uint8_t cv;
  std::uint8_t invflg;
  bool operator==(const MonitorCall&) const = default;
};

// A call of RWTS, with its IOB's slot (times 16), drive, track, sector,
// and command.
struct RwtsCall {
  std::uint8_t slot;
  std::uint8_t drive;
  std::uint8_t track;
  std::uint8_t sector;
  std::uint8_t command;
  bool operator==(const RwtsCall&) const = default;
};

using Call = std::variant<MonitorCall, RwtsCall>;

struct SectorWrite {
  std::uint8_t slot;
  std::uint8_t drive;
  std::uint8_t track;
  std::uint8_t sector;
  bool operator==(const SectorWrite&) const = default;
};

// Why `run` returned.
enum class Stop { kBrk, kHalt, kInputUsedUp };
std::string_view stop_name(Stop stop);

using DiskKey = std::pair<std::uint8_t, std::uint8_t>;

// One CPU with the stand-ins installed. `keys` are what RDKEY returns,
// without the high bit, and when they run out it returns RETURN.
// `lines` are what GETLN1 reads, one per call.
class Apple2 {
 public:
  explicit Apple2(Cpu& cpu) : cpu_(cpu) {}

  std::deque<std::uint8_t> keys;
  std::deque<std::string> lines;
  std::map<DiskKey, std::vector<std::uint8_t>> disks;
  // Each row stays as long as sim/apple2.py's bytearray would make it,
  // which a scroll or a clear of a window past column 40 lengthens.
  std::vector<std::vector<std::uint8_t>> screen =
      std::vector<std::vector<std::uint8_t>>(24, std::vector<std::uint8_t>(40, 0xA0));
  std::vector<std::uint8_t> printer;
  std::vector<Call> calls;
  std::vector<SectorWrite> written;
  std::optional<Stop> stop_reason;
  // Called after each call is appended to `calls`, for the comparison
  // with the Python machine (sim/game_run.h).
  std::function<void(const Cpu&)> on_call;
  // The instructions `run` has executed.
  std::uint64_t instructions = 0;
  // Called after each instruction `run` executes, with the opcode it
  // executed.
  std::function<void(const Cpu&, std::uint8_t)> after_step;

  // Sets the Monitor's zero page as INIT, SETVID, and SETKBD leave it: a
  // full-screen window, normal text, and CSW at COUT1.
  void boot_screen();

  // Returns the screen as text, one line per row, inverse characters in
  // lower case, each row without its trailing spaces.
  std::string text() const;

  // Steps the CPU until the program ends, and returns why: at a BRK, at
  // a `JMP` to itself (a program that has stopped), or at a line input
  // with the script used up. `other_hooks` run at their addresses too,
  // each followed by an RTS, unless one of the machine's is there.
  // Throws `std::runtime_error` when the run passes `max_cycles`.
  Stop run(std::uint64_t max_cycles = 2'000'000'000, const std::map<std::uint16_t, Hook>* other_hooks = nullptr);

 private:
  struct InputUsedUp {};

  std::uint8_t& m(std::uint16_t addr) { return cpu_.memory[addr]; }
  void log(Routine routine);
  void put(std::uint8_t c);
  void cr();
  void screen_out(std::uint8_t a);
  void cout1();
  void cout();
  void home();
  void clreol();
  void vtab();
  void step_random();
  void rdkey();
  void getln1();
  void rwts();
  bool run_hook(std::uint16_t pc);

  Cpu& cpu_;
  std::uint64_t lcg_ = 1;
};

}  // namespace c6502::sim::apple2
