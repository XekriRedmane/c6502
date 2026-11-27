// A stand-in Apple II for Broderbund's Lode Runner, for running the
// original game and examples/lode_runner on the same machine and
// comparing what each does.
//
// The game talks to the machine only through the soft switches and
// DOS's RWTS, so those are what the machine records. The soft switches
// are the keyboard (KBD at $C000, KBDSTRB at $C010), the speaker (SPKR
// at $C030), the display switches ($C050-$C057), the paddle buttons
// (BUTN0 and BUTN1 at $C061 and $C062), the paddles (PADDL0 and PADDL1
// at $C064 and $C065, started by PTRIG at $C070), and the language
// card's $C081. Every access to $C000-$C0FF is an event, and so is each
// call of RWTS, which is a hook at DISABLE_INTS_CALL_RWTS ($B7B5)
// working on a copy of the game's disk image (DOS order).
//
// Each event records the address (or RWTS), whether it was a read or a
// write, and CRCs of the two high-resolution pages as they were when
// the access happened. Page 2's CRC covers $4000-$5EFF, since the
// original's code starts at $5F00, inside the page. It also records the
// program counter of the instruction and the cycle count, which differ
// between the two programs and only help find where a difference comes
// from.
//
// The keyboard follows a script of (n, key) pairs. The key goes into
// KBD's latch, with bit 7 set, just before the program's KBD read
// number n (counting from 0), so that both programs see a key at the
// same point in their polling. Bit 7 stays set until the program reads
// or writes KBDSTRB, as on the Apple II, and the key's code stays in the
// low seven bits after that. No paddle button is ever down, and no
// joystick is attached. Each paddle reads with bit 7 set, as a paddle
// whose timer never runs out does, which DETECT_LACK_OF_JOYSTICK takes
// for a missing joystick.
//
// The soft switches are found by decoding each instruction before it
// runs (sim/game_machine.h), and the value a read gets is stored at the
// switch's address before the instruction reads it. RWTS takes no
// cycles.
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "binutils/elf.h"
#include "sim/cpu.h"
#include "sim/game_machine.h"

namespace c6502::sim::lode_runner {

inline constexpr std::uint16_t kRwtsEntry = 0xB7B5;
// The code of an RWTS event, in place of a soft-switch address.
inline constexpr std::uint32_t kRwtsEvent = 0x10000;
// MAIN, where the original starts, and the original's code, which the C
// program's image has cleared before it's loaded.
inline constexpr std::uint16_t kMain = 0x5F7D;
inline constexpr std::uint16_t kCodeStart = 0x5F00;
inline constexpr std::uint16_t kCodeEnd = 0x8E53;

// `code` is the soft switch's address times 2, plus 1 for a write, or
// `kRwtsEvent` times 2 for an RWTS call, and `page1` and `page2` are the
// CRCs of the two pages.
struct Event {
  std::uint32_t code;
  std::uint32_t page1;
  std::uint32_t page2;
  std::uint16_t pc;
  std::uint64_t cycles;
};

// An RWTS call's command, track, sector, and the buffer's 256 bytes
// after the call.
struct RwtsRecord {
  std::uint8_t command;
  std::uint8_t track;
  std::uint8_t sector;
  std::vector<std::uint8_t> buffer;
  bool operator==(const RwtsRecord&) const = default;
};

struct Events {
  std::vector<Event> events;
  // The RWTS record of each RWTS event, by the event's index.
  std::map<std::size_t, RwtsRecord> rwts;

  std::size_t size() const { return events.size(); }
  // Returns event `i` as text.
  std::string describe(std::size_t i) const;
};

// Returns the index of the first event at which the two streams differ,
// in the access, the pages, or an RWTS call's arguments and buffer, or
// none when one is a prefix of the other.
std::optional<std::size_t> first_difference(const Events& a, const Events& b);

struct KeyPress {
  int kbd_read;
  std::uint8_t key;
};

enum class End { kCycles, kStop, kBrk, kReturned };

class Machine {
 public:
  Machine(Cpu cpu, std::vector<std::uint8_t> disk, const std::vector<KeyPress>& keys);

  Cpu cpu;
  // The disk image, which RWTS reads and writes in place.
  std::vector<std::uint8_t> disk;
  Events events;
  int kbd_reads = 0;
  std::uint8_t kbd_latch = 0;
  // The display switches as they were last set.
  bool text = true;
  bool mixed = false;
  bool page2 = false;
  bool hires = false;
  // Called after each event is recorded.
  std::function<void(const Cpu&)> on_event;
  // The instructions `run` has executed.
  std::uint64_t instructions = 0;
  // Called after each instruction `run` executes, with the opcode it
  // executed.
  std::function<void(const Cpu&, std::uint8_t)> after_step;

  // Steps the CPU until `max_cycles` have run, until `stop`, called
  // after each instruction or hook that adds an event, returns true, or
  // until the program reaches a `BRK`.
  End run(std::uint64_t max_cycles, const std::function<bool(const Machine&)>& stop = {});

  // Runs the subroutine at `addr` as a `JSR` would, until it returns.
  // Throws `std::runtime_error` when it doesn't within `max_cycles`.
  void call(std::uint16_t addr, std::uint64_t max_cycles = 10'000'000);

 private:
  void record(std::uint32_t code, std::uint16_t pc);
  void access(const game::SwitchAccess& sw, std::uint16_t pc);
  void rwts();

  std::map<int, std::uint8_t> keys_;
  bool return_trap_ = false;
  game::PageCrc page1_crc_{0x2000, 0x4000};
  game::PageCrc page2_crc_{0x4000, 0x5F00};
};

// The original, `image` being its 48K memory image (main.bin of the
// reverse-engineering project), ready to run from MAIN, which sets the
// stack pointer itself.
Machine original_machine(const std::vector<std::uint8_t>& image, const std::vector<std::uint8_t>& disk,
                         const std::vector<KeyPress>& keys);

// The linked C program `exe` loaded over the original's image with the
// original's code cleared, ready to run from `main`. Each segment's
// bytes past its file size are zeroed, as a loader would. The stack
// pointer is $07, where MAIN puts it with `LDX #$07; TXS`, which C
// can't do. `__zp_init`, if the program has one, has already run.
Machine program_machine(const std::vector<std::uint8_t>& image, const binutils::elf::ElfFile& exe,
                        const std::vector<std::uint8_t>& disk, const std::vector<KeyPress>& keys);

// Returns the address of each global label of the original's main.asm
// (one that doesn't start with a dot), so that the nearest one at or
// below a program counter names the routine it's in. The addresses
// come from sizing each instruction and directive after each ORG as
// dasm would. An operand that's an EQU below $100 takes the zero-page
// form, and anything else, a label of the code included, takes the
// absolute form.
std::map<std::int64_t, std::string> original_routines(const std::string& main_asm);

}  // namespace c6502::sim::lode_runner
