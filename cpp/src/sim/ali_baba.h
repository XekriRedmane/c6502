// A stand-in Apple II for Electronic Arts' Ali Baba and the Forty
// Thieves, for running the original game and examples/ali_baba on the
// same machine and comparing what each does, ported from
// sim/ali_baba.py.
//
// The machine has 48K of RAM, a keyboard, a speaker, the display
// switches, the paddle buttons, a Disk II controller in slot 6
// (sim/disk2.h), and stand-ins for the Monitor ROM routines the game
// calls. The disk is emulated at the level of its nibbles, so that the
// game's own disk code runs unchanged: DOS 3.3's RWTS at $B600, the
// VM-driven reader at $B300, and the resident routines at $0500.
//
// Every access to $C000-$C0FF is an event, and so is every call of a
// Monitor routine, except those the kept disk code makes. That code, at
// $0500-$07FF and $B300-$BFFF, is the original's in both programs, so
// the accesses it makes to the disk switches, the keyboard
// (VERIFY_DATA_FIELD polls KBD once per nibble), and the Monitor (the
// VMs call WAIT) aren't events, and its reads of KBD don't count toward
// the keyboard script. In their place, each call of DISK_READ_FIELD
// ($B300) or RWTS_ENTRY ($B7B5) from outside that code is one event,
// recorded when it returns, with its result: the carry, and for RWTS
// the IOB and the 256 bytes of its buffer. Each disk read thus stays at
// its place in the stream, and the thousands of nibble reads inside it
// aren't compared. The motor switch that LOAD_SCENE_DATA touches itself
// is an event like any other.
//
// Each event records the address (or the Monitor routine's, or the disk
// entry's), whether it was a read or a write, a CRC of hi-res page 1
// ($2000-$3FFF) as it was when the access happened, and an argument:
// the register a Monitor routine reads (A for WAIT, COUT, and VTABZ, X
// for PREAD), and 0 otherwise, since the A a caller happens to leave
// for a routine that ignores it, such as SETPWRC, differs between the
// two programs. Page 2 is the game's data, so it isn't covered. Until
// the program first touches a display switch, page 1 holds the code
// GAME_INIT relocates, so the events before then (one, SETPWRC's)
// record 0 for its CRC. It also records the program counter of the
// instruction and the cycle count, which differ between the two
// programs and only help find where a difference comes from.
//
// The Monitor routines are hooks at their ROM addresses. COUT ($FDED)
// jumps through CSW as the ROM does, which after INIT_HRCG is the
// HRCG's HRCG_COUT, so the character output is the game's own code.
// COUT isn't an event, since the C program calls its HRCG directly, and
// what the HRCG draws and the Monitor routines it calls are. WAIT
// ($FCA8) takes the ROM's time, 7 + 11A + 5A(A+1)/2 cycles after the
// JSR for A from 1 to 255, and for A = 0 what the ROM's loop takes.
// PREAD ($FB1E) finds no paddle and returns Y = $FF after the ROM's
// 2833 cycles. SETPWRC ($FB6F) sets PWREDUP from the reset vector. The
// text routines the HRCG's control characters end with (VTAB, VTABZ,
// CLREOP, HOME, SCROLL, and CLREOL) write text page 1 at $0400-$07FF,
// as the ROM does, each following the ROM's instructions. A hook's
// return takes the 6 cycles of an RTS. COUT1 ($FDF0) has no stand-in,
// since the game calls COUT only after INIT_HRCG has pointed CSW at
// HRCG_COUT, so a run that reaches it ends.
//
// The keyboard follows a script of (n, key) pairs. The key goes into
// KBD's latch, with bit 7 set, just before the program's KBD read
// number n (counting from 0). The disks follow a script of (n, name)
// pairs, which put the disk `name` in drive 1 just before KBD read
// number n. Bit 7 stays set until the program reads or writes KBDSTRB.
// No paddle button is ever down, and no joystick is attached.
//
// The machine starts as EALDR leaves it when it jumps to $0800, with the
// memory of main.bin with GAME_INIT's relocation undone, the registers
// A=$08, X=$08, Y=$0C, SP=$FF, and P=$30, full-screen hi-res page 1,
// and the drive's head over track 5 with phase 2 on and the motor
// stopped. sim/ali_baba.py's module docstring gives how those were
// found, and why the half tracks 11 and 12 read the protection track 5
// (`kWideTrack`).
//
// Most of the game's time goes to the delay loops of its sound
// routines, so `run` takes three kinds of loop in one step: DEX; BNE
// back to the DEX, the same with DEY, and ROL zp; DEX; BNE back to the
// ROL (in PLAY_LOW_TONE). Each leaves the registers, flags, memory, and
// cycle count as the loop would.
#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "binutils/elf.h"
#include "sim/cpu.h"
#include "sim/disk2.h"
#include "sim/game_machine.h"

namespace c6502::sim::ali_baba {

inline constexpr std::uint16_t kPread = 0xFB1E;
inline constexpr std::uint16_t kSetpwrc = 0xFB6F;
inline constexpr std::uint16_t kVtab = 0xFC22;
inline constexpr std::uint16_t kVtabz = 0xFC24;
inline constexpr std::uint16_t kClreop = 0xFC42;
inline constexpr std::uint16_t kHome = 0xFC58;
inline constexpr std::uint16_t kScroll = 0xFC70;
inline constexpr std::uint16_t kClreol = 0xFC9C;
inline constexpr std::uint16_t kWait = 0xFCA8;
inline constexpr std::uint16_t kCout = 0xFDED;
inline constexpr std::uint16_t kCout1 = 0xFDF0;

inline constexpr std::uint16_t kDiskReadField = 0xB300;
inline constexpr std::uint16_t kRwtsEntry = 0xB7B5;
inline constexpr int kDiskSlot = 6;

// Event codes, in place of a soft switch's address: a Monitor routine
// is kRomEvent + its address, and a call into the disk code is
// kDiskEvent + its entry.
inline constexpr std::uint32_t kRomEvent = 0x10000;
inline constexpr std::uint32_t kDiskEvent = 0x20000;

// The registers when EALDR jumps to $0800.
inline constexpr std::uint8_t kEntryA = 0x08;
inline constexpr std::uint8_t kEntryX = 0x08;
inline constexpr std::uint8_t kEntryY = 0x0C;
inline constexpr std::uint8_t kEntrySp = 0xFF;
inline constexpr std::uint8_t kEntryP = 0x30;
inline constexpr std::uint16_t kEntry = 0x0800;

// The half tracks that read track 5, the protection track, besides its
// own (10).
inline constexpr std::array<std::pair<int, int>, 2> kWideTrack = {{{11, 5}, {12, 5}}};

// The spans of the original's memory, at their addresses after
// GAME_INIT's relocation, whose code and tables the C program
// translates, for reporting the coverage of the code the comparison
// checks.
struct Span {
  std::int64_t start;
  std::int64_t end;
};
inline constexpr std::array<Span, 5> kCodeSpans = {{
    {0x0800, 0x1F19}, {0x5AA9, 0x776E}, {0x7780, 0x7D83}, {0x92A5, 0x9AA5}, {0xA409, 0xA5BC},
}};

// The name of each Monitor routine the machine has a stand-in for.
const std::map<std::uint16_t, std::string>& rom_names();

struct Event {
  std::uint32_t code;
  std::uint32_t page1;
  std::uint16_t arg;
  std::uint16_t pc;
  std::uint64_t cycles;
};

// A disk call's entry, the carry it returned, the half track the head
// ended over (-1 for an empty drive), and for RWTS the 17 bytes of the
// IOB and, for a read or a write, the 256 of the buffer after the call.
struct DiskRecord {
  std::uint16_t entry;
  std::uint8_t carry;
  int half_track;
  std::vector<std::uint8_t> iob;
  std::vector<std::uint8_t> buffer;
  bool operator==(const DiskRecord&) const = default;
};

struct Events {
  std::vector<Event> events;
  // The record of each disk event, by the event's index.
  std::map<std::size_t, DiskRecord> disk;

  std::size_t size() const { return events.size(); }
  // Returns event `i` as text.
  std::string describe(std::size_t i) const;
  // Returns the number of events of each kind, by soft switch ("$C030
  // read"), Monitor routine ("WAIT"), or disk entry ("disk $B300").
  std::map<std::string, std::size_t> counts() const;
};

// Returns the index of the first event at which the two streams differ,
// in the access, the page, the argument, or a disk call's result, or
// none when one is a prefix of the other.
std::optional<std::size_t> first_difference(const Events& a, const Events& b);

struct KeyPress {
  int kbd_read;
  std::uint8_t key;
};

struct DiskSwap {
  int kbd_read;
  std::string disk;
};

// The other disks a script puts in drive 1, by name: a .dsk image, or
// none for an unformatted disk.
using DiskImages = std::vector<std::pair<std::string, std::optional<std::vector<std::uint8_t>>>>;

// Why `run` returned. `kUnimplemented` is a call of COUT1, which has no
// stand-in.
enum class End { kCycles, kStop, kBrk, kReturned, kUnimplemented };
std::string end_name(End end);

class Machine {
 public:
  // A hook runs in place of the code at its address. It ends `run` when
  // it returns an `End`. One that leaves PC where it was lets the
  // instruction there run after it.
  using Hook = std::function<std::optional<End>()>;

  // `media` holds the disks by name, "game" being the one in drive 1 at
  // the start, in the order a script's machine was given them.
  Machine(Cpu cpu, std::vector<std::pair<std::string, disk2::Drive>> media, const std::vector<KeyPress>& keys,
          const std::vector<DiskSwap>& swaps);
  Machine(const Machine&) = delete;
  Machine& operator=(const Machine&) = delete;
  Machine(Machine&&) = delete;
  Machine& operator=(Machine&&) = delete;

  Cpu cpu;
  std::vector<std::pair<std::string, disk2::Drive>> media;
  disk2::DiskII disk;
  Events events;
  int kbd_reads = 0;
  std::uint8_t kbd_latch = 0;
  // The display switches as they were last set.
  bool text = false;
  bool mixed = false;
  bool page2 = false;
  bool hires = true;
  std::map<std::uint16_t, Hook> hooks;
  // Whether `run` takes the delay loops in one step.
  bool fast_loops = true;
  // Whether the program has touched a display switch ($C050-$C05F) yet.
  // Until then, events record 0 for page 1's CRC.
  bool display_set = false;
  // Marks every address an instruction was fetched from, for `coverage`.
  std::vector<std::uint8_t> seen = std::vector<std::uint8_t>(0x10000);
  // Called after each event is recorded, for the comparison with the
  // Python machine (sim/game_run.h).
  std::function<void(const Cpu&)> on_event;
  // The instructions `run` has executed, not counting the delay loops
  // it takes in one step.
  std::uint64_t instructions = 0;
  // Called after each instruction `run` executes, with the opcode it
  // executed.
  std::function<void(const Cpu&, std::uint8_t)> after_step;

  disk2::Drive& medium(const std::string& name);

  // Puts disk `name` of `media` in drive 1, under the head where it is.
  void insert(const std::string& name);

  // Steps the CPU until `max_cycles` have run, until `stop`, called
  // after each instruction or hook that adds an event, returns true,
  // until the program reaches a `BRK` (which is also what a call of a
  // ROM routine without a hook runs into), or until a hook ends it.
  End run(std::uint64_t max_cycles, const std::function<bool(const Machine&)>& stop = {});

  // Runs the subroutine at `addr` as a `JSR` would, until it returns,
  // and then puts SP and PC back. Throws `std::runtime_error` when it
  // doesn't return within `max_cycles`.
  void call(std::uint16_t addr, std::uint64_t max_cycles = 10'000'000);

  // Returns as RTS does, in its 6 cycles.
  void rts();

 private:
  enum class RomExit { kReturn, kJumped, kUnimplemented };
  using RomRoutine = RomExit (Machine::*)();

  void record(std::uint32_t code, std::uint16_t pc, std::uint16_t arg = 0);
  void access(const game::SwitchAccess& sw, std::uint16_t pc);
  std::uint16_t caller() const;
  Hook rom_hook(std::uint16_t addr, RomRoutine routine);
  Hook disk_entry(std::uint16_t entry);
  void disk_returned(std::uint16_t entry, std::uint16_t caller, std::optional<unsigned> iob);
  bool delay_loop(std::uint16_t pc, std::uint8_t op);

  std::uint8_t nz(std::uint8_t v);
  void adc(std::uint8_t v);
  void cmp(std::uint8_t reg, std::uint8_t v);
  void asl();
  void bascalc();
  void vtabz_steps();
  void vtab_steps();
  void cleolz_steps();
  void cleop1_steps();

  RomExit wait();
  RomExit cout();
  RomExit pread();
  RomExit setpwrc();
  RomExit vtab();
  RomExit vtabz();
  RomExit clreol();
  RomExit clreop();
  RomExit home();
  RomExit scroll();
  RomExit cout1();

  std::map<int, std::uint8_t> keys_;
  std::map<int, std::string> swaps_;
  game::PageCrc page1_crc_{0x2000, 0x4000};
};

// Returns the 64K of memory as EALDR leaves it, from main.bin (the image
// of $0500-$BFFF after GAME_INIT's relocation): the zero page and
// vectors EALDR leaves, and the relocated code put back in the staging
// area of hi-res page 1.
std::vector<std::uint8_t> entry_image(const std::vector<std::uint8_t>& main_bin);

// Returns the game's disk in drive 1, with the head where EALDR leaves
// it, and with `kWideTrack` unless `wide_track` is false.
disk2::Drive entry_drive(const std::vector<std::uint8_t>& disk, bool wide_track = true);

// Returns the original ready to run from $0800 as EALDR leaves the
// machine. `disk` is the game's .dsk image, and `disks` the other disks
// the disk script `swaps` puts in.
std::unique_ptr<Machine> original_machine(const std::vector<std::uint8_t>& main_bin,
                                          const std::vector<std::uint8_t>& disk, const std::vector<KeyPress>& keys,
                                          const std::vector<DiskSwap>& swaps = {}, const DiskImages& disks = {},
                                          bool wide_track = true);

// Returns the (start, end) of each region in the MEMORY block of a
// linker script, whose ORIGIN and LENGTH are numbers or sums and
// differences of them.
std::vector<Span> link_regions(const std::string& script);

// Where the C program starts, and whether a segment at an address
// GAME_INIT relocates to also goes at its staging address, for a
// program that relocates as the original does.
struct ProgramOptions {
  std::optional<std::string> entry_symbol = "main";
  std::optional<std::uint16_t> entry_address;
  std::vector<Span> clear;
  std::optional<std::uint16_t> ssp;
  bool wide_track = true;
  bool staged = false;
};

// Returns the linked C program `exe` loaded over the original's image
// as EALDR leaves it, ready to run from its entry. The spans in
// `options.clear`, which should be the linker script's regions
// (`link_regions`), are zeroed first, so that a jump into the
// original's code that the program replaces runs into a BRK. Each
// segment's bytes past its file size are zeroed, as a loader would.
//
// When the program calls the simulator's runtime helpers (it was linked
// with `sim::defsym_args()`), they're installed at $E000 in the ROM's
// space, which the game never uses, and each hook returns in an RTS's 6
// cycles. `options.ssp`, if given, is stored in SSP, for a program whose
// functions use the soft stack. `__zp_init`, if the program has one,
// has already run, and the registers are then put back as EALDR leaves
// them.
std::unique_ptr<Machine> program_machine(const std::vector<std::uint8_t>& main_bin,
                                         const binutils::elf::ElfFile& exe, const std::vector<std::uint8_t>& disk,
                                         const std::vector<KeyPress>& keys, const std::vector<DiskSwap>& swaps = {},
                                         const DiskImages& disks = {}, const ProgramOptions& options = {});

// A global label of the original's main.asm, and whether the first
// statement after it is an instruction (true) or data (false).
struct Label {
  std::string name;
  bool code;
  bool operator==(const Label&) const = default;
};

// Returns each global label of the original's main.asm (one without a
// dot, and not an EQU), by address from main.sym. When several labels
// share an address, the first in the source names it.
std::map<std::int64_t, Label> original_labels(const std::string& main_asm, const std::string& main_sym);

// Which of the original's routines a run executed: each code label's
// address, the end of its code (the next label of either kind), and the
// number of its instructions the run executed, by name.
struct RoutineCoverage {
  std::int64_t start;
  std::int64_t end;
  std::int64_t executed;
};
struct Coverage {
  std::map<std::string, RoutineCoverage> routines;
  // The names in the order of their addresses.
  std::vector<std::string> order;

  std::vector<std::string> reached() const;
  std::vector<std::string> missed() const;
  std::string summary() const;
};

// Returns the coverage of the code labels in `labels` by a run whose
// `Machine::seen` is `seen`, counting only the labels inside `spans`
// when it's given.
Coverage coverage(const std::vector<std::uint8_t>& seen, const std::map<std::int64_t, Label>& labels,
                  const std::vector<Span>* spans = nullptr);

}  // namespace c6502::sim::ali_baba
