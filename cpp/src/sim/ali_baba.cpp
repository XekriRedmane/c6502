#include "sim/ali_baba.h"

#include <algorithm>
#include <cstdio>
#include <regex>
#include <stdexcept>

#include "passes/asm_context.h"
#include "sim/runtime.h"

namespace c6502::sim::ali_baba {

namespace {

constexpr std::uint16_t kPaddl0 = 0xC064;
constexpr std::uint16_t kDiskSwitches = 0xC080 + 16 * kDiskSlot;

// The Monitor's zero page.
constexpr std::uint16_t kWndlft = 0x20;
constexpr std::uint16_t kWndwdth = 0x21;
constexpr std::uint16_t kWndtop = 0x22;
constexpr std::uint16_t kWndbtm = 0x23;
constexpr std::uint16_t kCh = 0x24;
constexpr std::uint16_t kCv = 0x25;
constexpr std::uint16_t kBasl = 0x28;
constexpr std::uint16_t kBash = 0x29;
constexpr std::uint16_t kBas2l = 0x2A;
constexpr std::uint16_t kBas2h = 0x2B;
constexpr std::uint16_t kCsw = 0x36;
constexpr std::uint16_t kSoftev = 0x03F2;
constexpr std::uint16_t kPwredup = 0x03F4;

// Where `call` makes a subroutine return to, an address in the ROM that
// the game never calls.
constexpr std::uint16_t kReturnTrap = 0xFFF0;

// The zero page when EALDR jumps to $0800, from a boot of the disk on
// the Apple II Plus ROM: the Monitor's window, INVFLG, CSW and KSW after
// RESET, and what the boot ROM, BOOT1, and EALDR left. Every other byte
// is 0.
constexpr std::pair<std::uint8_t, std::uint8_t> kEntryZeroPage[] = {
    {0x01, 0xC6}, {0x21, 0x28}, {0x23, 0x18}, {0x29, 0x04}, {0x2B, 0x60}, {0x2C, 0xF5}, {0x2D, 0x0A},
    {0x2E, 0x01}, {0x2F, 0xFE}, {0x32, 0xFF}, {0x35, 0x1B}, {0x36, 0xF0}, {0x37, 0xFD}, {0x38, 0xE8},
    {0x39, 0x06}, {0x3A, 0x10}, {0x3B, 0xFE}, {0x3C, 0xFF}, {0x3D, 0x05}, {0x3E, 0x30}, {0x40, 0x50},
    {0x43, 0x08}, {0x45, 0x33}, {0x46, 0x3C}, {0x47, 0xA9}, {0x4A, 0x0A}, {0x4B, 0x0A}, {0x4D, 0xA6},
    {0x4F, 0x08},
};
// $03F0-$03F4 then: the BRK vector, the reset vector EALDR set, and
// PWREDUP.
constexpr std::uint8_t kEntryVectors[] = {0x59, 0xFA, 0x48, 0xA9, 0x0C};

// The relocation GAME_INIT does (RELOCATE_HIRES_STAGING): each span of
// the staging area in hi-res page 1, and where it goes.
struct Relocation {
  std::size_t staged;
  std::size_t home;
  std::size_t size;
};
constexpr Relocation kRelocations[] = {{0x2000, 0x9600, 0x1300}, {0x3300, 0xB300, 0x0D00}};

bool kept(std::uint16_t pc) { return (pc >= 0x0500 && pc < 0x0800) || (pc >= 0xB300 && pc < 0xC000); }

bool reads_a(std::uint16_t addr) { return addr == kWait || addr == kCout || addr == kCout1 || addr == kVtabz; }

// The first opcodes of the delay loops `Machine::run` takes in one
// step: DEX, DEY, and ROL zp.
bool loop_head(std::uint8_t op) { return op == 0xCA || op == 0x88 || op == 0x26; }

// The cycles WAIT takes after the JSR, for each A, by running the ROM's
// loop (SEC; PHA; SBC #1 until zero; PLA; SBC #1 and again until zero;
// RTS, less the RTS, which `Machine::rts` counts).
std::array<std::uint64_t, 256> wait_cycles() {
  std::array<std::uint64_t, 256> out{};
  for (int a0 = 0; a0 < 256; ++a0) {
    int a = a0;
    int c = 1;
    std::uint64_t cycles = 2;
    for (;;) {
      cycles += 3;
      const int outer = a;
      for (;;) {
        const int r = a - 1 - (1 - c);
        c = r >= 0 ? 1 : 0;
        a = r & 0xFF;
        cycles += 2;
        if (a == 0) {
          cycles += 2;
          break;
        }
        cycles += 3;
      }
      a = outer;
      cycles += 4;
      const int r = a - 1 - (1 - c);
      c = r >= 0 ? 1 : 0;
      a = r & 0xFF;
      cycles += 2;
      if (a == 0) {
        cycles += 2;
        break;
      }
      cycles += 3;
    }
    out[static_cast<std::size_t>(a0)] = cycles;
  }
  return out;
}

const std::array<std::uint64_t, 256>& wait_table() {
  static const std::array<std::uint64_t, 256> table = wait_cycles();
  return table;
}

std::string hex(unsigned value, int digits) {
  char buf[16];
  std::snprintf(buf, sizeof buf, "%0*X", digits, value);
  return buf;
}

std::vector<std::uint8_t> slice(const std::vector<std::uint8_t>& mem, std::size_t at, std::size_t n) {
  at = std::min(at, mem.size());
  return std::vector<std::uint8_t>(mem.begin() + static_cast<std::ptrdiff_t>(at),
                                   mem.begin() + static_cast<std::ptrdiff_t>(std::min(at + n, mem.size())));
}

void store(std::vector<std::uint8_t>& mem, std::int64_t addr, const std::vector<std::uint8_t>& data) {
  if (addr < 0 || addr + static_cast<std::int64_t>(data.size()) > 0x10000) {
    throw std::runtime_error("the program has bytes outside $0000-$FFFF");
  }
  std::copy(data.begin(), data.end(), mem.begin() + addr);
}

// Stores `data` at `addr`, and the bytes of it that GAME_INIT's
// relocation copies from hi-res page 1 at their staging addresses too,
// so that they're in place both before and after the copy.
void put_staged(std::vector<std::uint8_t>& mem, std::int64_t addr, const std::vector<std::uint8_t>& data) {
  store(mem, addr, data);
  const auto end = addr + static_cast<std::int64_t>(data.size());
  for (const Relocation& r : kRelocations) {
    const auto home = static_cast<std::int64_t>(r.home);
    const std::int64_t lo = std::max(addr, home);
    const std::int64_t hi = std::min(end, home + static_cast<std::int64_t>(r.size));
    if (lo < hi) {
      std::copy(data.begin() + (lo - addr), data.begin() + (hi - addr),
                mem.begin() + static_cast<std::int64_t>(r.staged) + (lo - home));
    }
  }
}

std::unique_ptr<Machine> make_machine(std::vector<std::uint8_t> mem, const std::vector<std::uint8_t>& disk,
                                      const std::vector<KeyPress>& keys, const std::vector<DiskSwap>& swaps,
                                      const DiskImages& disks, bool wide_track) {
  Cpu cpu;
  cpu.memory = std::move(mem);
  cpu.a = kEntryA;
  cpu.x = kEntryX;
  cpu.y = kEntryY;
  cpu.sp = kEntrySp;
  cpu.p = kEntryP;
  cpu.pc = kEntry;
  std::vector<std::pair<std::string, disk2::Drive>> media;
  media.emplace_back("game", entry_drive(disk, wide_track));
  for (const auto& [name, image] : disks) {
    media.emplace_back(name, image ? disk2::Drive::from_dsk(*image) : disk2::Drive::blank());
  }
  return std::make_unique<Machine>(std::move(cpu), std::move(media), keys, swaps);
}

void set_entry_registers(Cpu& cpu) {
  cpu.a = kEntryA;
  cpu.x = kEntryX;
  cpu.y = kEntryY;
  cpu.sp = kEntrySp;
  cpu.p = kEntryP;
}

std::optional<std::int64_t> lookup(const std::map<std::string, std::int64_t>& symbols, const std::string& name) {
  const auto it = symbols.find(name);
  if (it == symbols.end()) return std::nullopt;
  return it->second;
}

// Installs the simulator's runtime helpers that the program `exe` refers
// to and doesn't hold itself, checking that it was linked with their
// addresses.
void install_helpers(Machine& machine, const std::map<std::string, std::int64_t>& symbols,
                     const binutils::elf::ElfFile& exe) {
  std::set<std::string> own;
  for (const binutils::elf::SymbolPtr& s : exe.symbols) {
    if (s->defined() && s->type != binutils::elf::STT_FILE && !s->is_absolute()) own.insert(s->name);
  }
  passes::RuntimeLayout layout;
  layout.ssp = lookup(symbols, "SSP").value_or(static_cast<std::int64_t>(layout.ssp));
  layout.fp = lookup(symbols, "FP").value_or(static_cast<std::int64_t>(layout.fp));
  layout.hargs = lookup(symbols, "HARGS").value_or(static_cast<std::int64_t>(layout.hargs));
  layout.dptr = lookup(symbols, "DPTR").value_or(static_cast<std::int64_t>(layout.dptr));
  const Runtime runtime = build_runtime(layout);
  std::vector<std::string> used;
  for (const auto& [name, _] : runtime.symbols) {
    if (symbols.count(name) != 0 && own.count(name) == 0) used.push_back(name);
  }
  if (used.empty()) return;
  for (const std::string& name : used) {
    if (symbols.at(name) != runtime.symbols.at(name)) {
      throw std::runtime_error("the executable has `" + name + "` at $" +
                               hex(static_cast<unsigned>(symbols.at(name)), 4) + ", and the runtime has it at $" +
                               hex(static_cast<unsigned>(runtime.symbols.at(name)), 4));
    }
  }
  std::copy(runtime.helper_image.begin(), runtime.helper_image.end(),
            machine.cpu.memory.begin() + runtime.helper_origin);
  for (const auto& [addr, fn] : runtime.hooks) {
    machine.hooks[addr] = [&machine, fn = fn]() -> std::optional<End> {
      fn(machine.cpu.memory);
      machine.rts();
      return std::nullopt;
    };
  }
}

}  // namespace

const std::map<std::uint16_t, std::string>& rom_names() {
  static const std::map<std::uint16_t, std::string> names = {
      {kPread, "PREAD"}, {kSetpwrc, "SETPWRC"}, {kVtab, "VTAB"},     {kVtabz, "VTABZ"},
      {kClreop, "CLREOP"}, {kHome, "HOME"},    {kScroll, "SCROLL"}, {kClreol, "CLREOL"},
      {kWait, "WAIT"},     {kCout, "COUT"},    {kCout1, "COUT1"},
  };
  return names;
}

std::string end_name(End end) {
  switch (end) {
    case End::kCycles: return "cycles";
    case End::kStop: return "stop";
    case End::kBrk: return "brk";
    case End::kReturned: return "returned";
    case End::kUnimplemented: return "unimplemented Monitor routine COUT1";
  }
  return "?";
}

std::string Events::describe(std::size_t i) const {
  const Event& e = events.at(i);
  const std::uint32_t code = e.code >> 1;
  std::string what;
  if (code >= kDiskEvent) {
    const DiskRecord& r = disk.at(i);
    what = "disk call $" + hex(r.entry, 4) + " returning C=" + std::to_string(r.carry);
    if (!r.iob.empty()) {
      what += " (RWTS command " + std::to_string(r.iob.at(12)) + " T$" + hex(r.iob.at(4), 2) + " S$" +
              hex(r.iob.at(5), 1) + " buffer $" + hex(r.iob.at(9), 2) + hex(r.iob.at(8), 2) + ")";
    }
  } else if (code >= kRomEvent) {
    const auto it = rom_names().find(static_cast<std::uint16_t>(code - kRomEvent));
    const std::string name = it != rom_names().end() ? it->second : "$" + hex(code - kRomEvent, 4);
    what = name + " with A=$" + hex(e.arg, 2);
  } else {
    what = std::string((e.code & 1) != 0 ? "write" : "read") + " $" + hex(code, 4);
  }
  return "#" + std::to_string(i) + " " + what + " at PC $" + hex(e.pc, 4) + ", cycle " + std::to_string(e.cycles) +
         ", page 1 " + hex(e.page1, 8);
}

std::map<std::string, std::size_t> Events::counts() const {
  std::map<std::string, std::size_t> out;
  for (const Event& e : events) {
    const std::uint32_t code = e.code >> 1;
    std::string key;
    if (code >= kDiskEvent) {
      key = "disk $" + hex(code - kDiskEvent, 4);
    } else if (code >= kRomEvent) {
      const auto it = rom_names().find(static_cast<std::uint16_t>(code - kRomEvent));
      if (it != rom_names().end()) {
        key = it->second;
      } else {
        char buf[16];
        std::snprintf(buf, sizeof buf, "0x%x", static_cast<unsigned>(code));
        key = buf;
      }
    } else {
      key = "$" + hex(code, 4) + ((e.code & 1) != 0 ? " write" : " read");
    }
    ++out[key];
  }
  return out;
}

std::optional<std::size_t> first_difference(const Events& a, const Events& b) {
  const std::size_t common = std::min(a.size(), b.size());
  std::size_t n = common;
  for (std::size_t i = 0; i < common; ++i) {
    const Event& x = a.events[i];
    const Event& y = b.events[i];
    if (x.code != y.code || x.page1 != y.page1 || x.arg != y.arg) {
      n = i;
      break;
    }
  }
  std::vector<std::size_t> indices;
  for (const auto& [i, _] : a.disk) indices.push_back(i);
  for (const auto& [i, _] : b.disk) indices.push_back(i);
  std::sort(indices.begin(), indices.end());
  for (const std::size_t i : indices) {
    if (i >= n) break;
    const auto x = a.disk.find(i);
    const auto y = b.disk.find(i);
    const bool same = (x == a.disk.end()) == (y == b.disk.end()) && (x == a.disk.end() || x->second == y->second);
    if (!same) {
      n = i;
      break;
    }
  }
  if (n < common) return n;
  return std::nullopt;
}

Machine::Machine(Cpu cpu_in, std::vector<std::pair<std::string, disk2::Drive>> media_in,
                 const std::vector<KeyPress>& keys, const std::vector<DiskSwap>& swaps)
    : cpu(std::move(cpu_in)), media(std::move(media_in)), disk({&medium("game"), nullptr}, kDiskSlot) {
  for (const KeyPress& k : keys) keys_[k.kbd_read] = k.key;
  for (const DiskSwap& s : swaps) swaps_[s.kbd_read] = s.disk;
  const std::pair<std::uint16_t, RomRoutine> rom[] = {
      {kWait, &Machine::wait},     {kCout, &Machine::cout},     {kPread, &Machine::pread},
      {kSetpwrc, &Machine::setpwrc}, {kCout1, &Machine::cout1}, {kVtab, &Machine::vtab},
      {kVtabz, &Machine::vtabz},   {kClreop, &Machine::clreop}, {kHome, &Machine::home},
      {kScroll, &Machine::scroll}, {kClreol, &Machine::clreol},
  };
  for (const auto& [addr, routine] : rom) hooks.emplace(addr, rom_hook(addr, routine));
  for (const std::uint16_t entry : {kDiskReadField, kRwtsEntry}) hooks.emplace(entry, disk_entry(entry));
}

disk2::Drive& Machine::medium(const std::string& name) {
  for (auto& [n, drive] : media) {
    if (n == name) return drive;
  }
  throw std::out_of_range("the machine has no disk named " + name);
}

void Machine::record(std::uint32_t code, std::uint16_t pc, std::uint16_t arg) {
  const std::uint32_t page = display_set ? page1_crc_(cpu.memory) : 0;
  events.events.push_back(Event{code, page, arg, pc, cpu.cycles});
  if (on_event) on_event(cpu);
}

void Machine::access(const game::SwitchAccess& sw, std::uint16_t pc) {
  const std::uint16_t addr = sw.address;
  const bool is_kept = kept(pc);
  const bool reads = (sw.access & game::kReadAccess) != 0;
  const bool writes = (sw.access & game::kWriteAccess) != 0;
  if ((addr & 0xF0) == 0x50) display_set = true;
  if (!is_kept) {
    if (reads) record(std::uint32_t{addr} << 1, pc);
    if (writes) record(std::uint32_t{addr} << 1 | 1, pc);
  }
  std::uint8_t& mem = cpu.memory[addr];
  const unsigned page = addr & 0xF0;
  if (page == 0x00) {
    if (reads && !is_kept) {
      if (const auto key = keys_.find(kbd_reads); key != keys_.end()) kbd_latch = key->second | 0x80;
      if (const auto swap = swaps_.find(kbd_reads); swap != swaps_.end()) insert(swap->second);
      ++kbd_reads;
    }
    mem = kbd_latch;
  } else if (page == 0x10) {
    kbd_latch &= 0x7F;
    mem = kbd_latch;
  } else if (page == 0x50) {
    const unsigned index = addr & 0x0F;
    if (index < 8) {
      const bool on = (index & 1) != 0;
      if (index < 2) {
        text = on;
      } else if (index < 4) {
        mixed = on;
      } else if (index < 6) {
        page2 = on;
      } else {
        hires = on;
      }
    }
    mem = 0;
  } else if (addr >= kDiskSwitches && addr < kDiskSwitches + 16) {
    mem = disk.access(addr, writes, sw.value, cpu.cycles);
  } else if (addr >= kPaddl0 && addr < kPaddl0 + 4) {
    mem = 0x80;
  } else {
    mem = 0;
  }
}

void Machine::insert(const std::string& name) {
  disk2::Drive* old = disk.drives[0];
  disk2::Drive& now = medium(name);
  if (old != nullptr) {
    now.half_track = old->half_track;
    now.phases = old->phases;
  }
  disk.drives[0] = &now;
}

std::uint16_t Machine::caller() const {
  const std::vector<std::uint8_t>& mem = cpu.memory;
  const unsigned ret = mem[0x100 + ((cpu.sp + 1) & 0xFF)] | mem[0x100 + ((cpu.sp + 2) & 0xFF)] << 8;
  return static_cast<std::uint16_t>((ret - 2) & 0xFFFF);
}

Machine::Hook Machine::rom_hook(std::uint16_t addr, RomRoutine routine) {
  return [this, addr, routine]() -> std::optional<End> {
    const std::uint16_t from = caller();
    if (!kept(from) && addr != kCout) {
      const std::uint16_t arg = reads_a(addr) ? cpu.a : addr == kPread ? cpu.x : 0;
      record((kRomEvent + addr) << 1, from, arg);
    }
    const RomExit exit = (this->*routine)();
    if (exit == RomExit::kUnimplemented) return End::kUnimplemented;
    if (exit == RomExit::kReturn) rts();
    return std::nullopt;
  };
}

void Machine::rts() {
  cpu.rts();
  cpu.cycles += 6;
}

// On a call from outside the kept code, the hook notes the stack pointer
// and sets a hook at the return address that records the event when the
// call returns there. The entry's own code then runs.
Machine::Hook Machine::disk_entry(std::uint16_t entry) {
  return [this, entry]() -> std::optional<End> {
    const std::uint16_t from = caller();
    if (kept(from)) return std::nullopt;
    const std::vector<std::uint8_t>& mem = cpu.memory;
    const unsigned ret = (mem[0x100 + ((cpu.sp + 1) & 0xFF)] | mem[0x100 + ((cpu.sp + 2) & 0xFF)] << 8) + 1;
    if (ret > 0xFFFF) return std::nullopt;
    const auto at = static_cast<std::uint16_t>(ret);
    std::optional<unsigned> iob;
    if (entry == kRwtsEntry) iob = cpu.a << 8 | cpu.y;
    const auto back = static_cast<std::uint8_t>(cpu.sp + 2);
    Hook previous;
    if (const auto it = hooks.find(at); it != hooks.end()) previous = it->second;
    hooks[at] = [this, entry, from, iob, back, previous, at]() -> std::optional<End> {
      if (cpu.sp != back) return previous ? previous() : std::nullopt;
      if (previous) {
        hooks[at] = previous;
      } else {
        hooks.erase(at);
      }
      disk_returned(entry, from, iob);
      return previous ? previous() : std::nullopt;
    };
    return std::nullopt;
  };
}

void Machine::disk_returned(std::uint16_t entry, std::uint16_t from, std::optional<unsigned> iob) {
  std::vector<std::uint8_t> iob_bytes;
  std::vector<std::uint8_t> buffer;
  if (iob) {
    iob_bytes = slice(cpu.memory, *iob, 17);
    const unsigned buf = iob_bytes.at(8) | iob_bytes.at(9) << 8;
    if (iob_bytes.at(12) == 1 || iob_bytes.at(12) == 2) buffer = slice(cpu.memory, buf, 256);
  }
  const disk2::Drive* d = disk.drive();
  record((kDiskEvent + entry) << 1, from);
  events.disk[events.size() - 1] = DiskRecord{entry, static_cast<std::uint8_t>(cpu.p & Cpu::kCarry),
                                              d != nullptr ? d->half_track : -1, std::move(iob_bytes),
                                              std::move(buffer)};
}

std::uint8_t Machine::nz(std::uint8_t v) {
  cpu.p = static_cast<std::uint8_t>((cpu.p & ~0x82) | (v & 0x80) | (v != 0 ? 0 : 0x02));
  return v;
}

// ADC in binary mode.
void Machine::adc(std::uint8_t v) {
  const int a = cpu.a;
  int r = a + v + (cpu.p & 1);
  int p = cpu.p & ~0xC3;
  if (r > 0xFF) p |= 0x01;
  if ((~(a ^ v) & (a ^ r) & 0x80) != 0) p |= 0x40;
  r &= 0xFF;
  cpu.p = static_cast<std::uint8_t>(p | (r & 0x80) | (r != 0 ? 0 : 0x02));
  cpu.a = static_cast<std::uint8_t>(r);
}

void Machine::cmp(std::uint8_t reg, std::uint8_t v) {
  const int r = (reg - v) & 0xFF;
  cpu.p = static_cast<std::uint8_t>((cpu.p & ~0x83) | (reg >= v ? 0x01 : 0) | (r & 0x80) | (r != 0 ? 0 : 0x02));
}

void Machine::asl() {
  cpu.p = static_cast<std::uint8_t>((cpu.p & ~0x01) | (cpu.a >> 7));
  cpu.a = nz(static_cast<std::uint8_t>(cpu.a << 1));
}

// BASCALC ($FBC1): BASH/BASL = the text line in A.
void Machine::bascalc() {
  std::vector<std::uint8_t>& mem = cpu.memory;
  const std::uint8_t row = cpu.a;
  cpu.p = static_cast<std::uint8_t>((cpu.p & ~0x01) | (row & 1));
  mem[kBash] = nz(static_cast<std::uint8_t>(((row >> 1) & 3) | 4));
  cpu.a = nz(row & 0x18);
  if ((row & 1) != 0) adc(0x7F);
  mem[kBasl] = cpu.a;
  asl();
  asl();
  cpu.a = nz(cpu.a | mem[kBasl]);
  mem[kBasl] = cpu.a;
  cpu.cycles += 40;
}

// VTABZ: BASCALC, then ADC WNDLFT into BASL.
void Machine::vtabz_steps() {
  bascalc();
  adc(cpu.memory[kWndlft]);
  cpu.memory[kBasl] = cpu.a;
  cpu.cycles += 15;
}

void Machine::vtab_steps() {
  cpu.a = nz(cpu.memory[kCv]);
  cpu.cycles += 3;
  vtabz_steps();
}

// CLEOLZ ($FC9E): spaces from column Y to the window's right edge, at
// least one.
void Machine::cleolz_steps() {
  std::vector<std::uint8_t>& mem = cpu.memory;
  cpu.a = nz(0xA0);
  const unsigned base = mem[kBasl] | mem[kBash] << 8;
  const std::uint8_t width = mem[kWndwdth];
  for (;;) {
    mem[(base + cpu.y) & 0xFFFF] = 0xA0;
    cpu.y = nz(static_cast<std::uint8_t>(cpu.y + 1));
    cmp(cpu.y, width);
    cpu.cycles += 13;
    if ((cpu.p & 1) != 0) break;
  }
  cpu.cycles += 8;
}

// CLEOP1 ($FC46): clears from column Y of line A to the end of the
// window, then VTAB.
void Machine::cleop1_steps() {
  for (;;) {
    const std::uint8_t row = cpu.a;
    vtabz_steps();
    cleolz_steps();
    cpu.y = nz(0);
    cpu.a = nz(row);
    adc(0);
    cmp(cpu.a, cpu.memory[kWndbtm]);
    cpu.cycles += 25;
    if ((cpu.p & 1) != 0) break;
  }
  vtab_steps();
}

// WAIT: A=0, with N and V clear and Z and C set, after the ROM's time.
Machine::RomExit Machine::wait() {
  cpu.cycles += wait_table()[cpu.a];
  cpu.a = 0;
  cpu.p = static_cast<std::uint8_t>((cpu.p & ~0xC0) | 0x03);
  return RomExit::kReturn;
}

// COUT: JMP (CSW).
Machine::RomExit Machine::cout() {
  cpu.pc = static_cast<std::uint16_t>(cpu.memory[kCsw] | cpu.memory[kCsw + 1] << 8);
  cpu.cycles += 5;
  return RomExit::kJumped;
}

// PREAD: no paddle's timer runs out, so the count reaches 255. The ROM's
// last read of the paddle leaves $80 in A.
Machine::RomExit Machine::pread() {
  cpu.cycles += 2827;
  cpu.a = 0x80;
  cpu.y = 0xFF;
  cpu.p = static_cast<std::uint8_t>((cpu.p & ~0x02) | 0x80);
  return RomExit::kReturn;
}

// SETPWRC: PWREDUP = high byte of the reset vector XOR $A5.
Machine::RomExit Machine::setpwrc() {
  cpu.a = cpu.memory[kSoftev + 1] ^ 0xA5;
  cpu.memory[kPwredup] = cpu.a;
  nz(cpu.a);
  cpu.cycles += 10;
  return RomExit::kReturn;
}

Machine::RomExit Machine::vtab() {
  vtab_steps();
  return RomExit::kReturn;
}

Machine::RomExit Machine::vtabz() {
  vtabz_steps();
  return RomExit::kReturn;
}

Machine::RomExit Machine::clreol() {
  cpu.y = nz(cpu.memory[kCh]);
  cpu.cycles += 3;
  cleolz_steps();
  return RomExit::kReturn;
}

Machine::RomExit Machine::clreop() {
  cpu.y = nz(cpu.memory[kCh]);
  cpu.a = nz(cpu.memory[kCv]);
  cpu.cycles += 6;
  cleop1_steps();
  return RomExit::kReturn;
}

Machine::RomExit Machine::home() {
  cpu.a = nz(cpu.memory[kWndtop]);
  cpu.memory[kCv] = cpu.a;
  cpu.y = nz(0);
  cpu.memory[kCh] = 0;
  cpu.cycles += 13;
  cleop1_steps();
  return RomExit::kReturn;
}

// SCROLL ($FC70): moves the window's lines up one and clears its bottom
// line.
Machine::RomExit Machine::scroll() {
  std::vector<std::uint8_t>& mem = cpu.memory;
  cpu.a = nz(mem[kWndtop]);
  std::uint8_t row = cpu.a;
  vtabz_steps();
  for (;;) {
    cpu.a = nz(mem[kBasl]);
    mem[kBas2l] = cpu.a;
    cpu.a = nz(mem[kBash]);
    mem[kBas2h] = cpu.a;
    cpu.y = nz(static_cast<std::uint8_t>(mem[kWndwdth] - 1));
    cpu.a = nz(row);
    adc(1);
    cmp(cpu.a, mem[kWndbtm]);
    cpu.cycles += 30;
    if ((cpu.p & 1) != 0) break;
    row = cpu.a;
    vtabz_steps();
    const unsigned src = mem[kBasl] | mem[kBash] << 8;
    const unsigned dst = mem[kBas2l] | mem[kBas2h] << 8;
    for (;;) {
      cpu.a = nz(mem[(src + cpu.y) & 0xFFFF]);
      mem[(dst + cpu.y) & 0xFFFF] = cpu.a;
      cpu.y = nz(static_cast<std::uint8_t>(cpu.y - 1));
      cpu.cycles += 16;
      if ((cpu.y & 0x80) != 0) break;
    }
  }
  cpu.y = nz(0);
  cleolz_steps();
  vtab_steps();
  return RomExit::kReturn;
}

Machine::RomExit Machine::cout1() { return RomExit::kUnimplemented; }

End Machine::run(std::uint64_t max_cycles, const std::function<bool(const Machine&)>& stop) {
  std::size_t n_events = events.size();
  const auto stopped = [&]() {
    if (!stop || events.size() == n_events) return false;
    n_events = events.size();
    return stop(*this);
  };
  while (cpu.cycles < max_cycles) {
    const std::uint16_t pc = cpu.pc;
    if (const auto it = hooks.find(pc); it != hooks.end()) {
      const Hook hook = it->second;
      if (const std::optional<End> why = hook()) return *why;
      if (cpu.pc != pc) {
        if (stopped()) return End::kStop;
        continue;
      }
    }
    seen[pc] = 1;
    const std::uint8_t op = cpu.memory[pc];
    if (op == 0x00) return End::kBrk;
    if (loop_head(op) && fast_loops && delay_loop(pc, op)) continue;
    if (const std::optional<game::SwitchAccess> sw = game::soft_switch_access(cpu)) access(*sw, pc);
    cpu.step();
    ++instructions;
    if (after_step) after_step(cpu, op);
    if (stopped()) return End::kStop;
  }
  return End::kCycles;
}

// Runs the delay loop at `pc` to its end in one step, if the code there
// is one and no hook is inside it. Returns whether it did.
bool Machine::delay_loop(std::uint16_t pc, std::uint8_t op) {
  std::vector<std::uint8_t>& mem = cpu.memory;
  const auto at = [&mem, pc](unsigned off) { return mem[(pc + off) & 0xFFFF]; };
  unsigned size = 0;
  std::uint64_t per_pass = 0;
  if (op == 0x26) {
    if (at(2) != 0xCA || at(3) != 0xD0 || at(4) != 0xFB) return false;
    size = 5;
    per_pass = 7;
  } else if (at(1) == 0xD0 && at(2) == 0xFD) {
    size = 3;
    per_pass = 2;
  } else {
    return false;
  }
  for (unsigned a = pc + 1u; a < pc + size; ++a) {
    if (hooks.count(static_cast<std::uint16_t>(a)) != 0 && a <= 0xFFFF) return false;
  }
  const unsigned end = pc + size;
  const std::uint64_t taken = ((end ^ pc) & 0xFF00) != 0 ? 4 : 3;
  std::uint64_t passes = 0;
  if (op == 0x88) {
    passes = cpu.y != 0 ? cpu.y : 256;
    cpu.y = 0;
  } else {
    passes = cpu.x != 0 ? cpu.x : 256;
    cpu.x = 0;
  }
  if (op == 0x26) {
    const std::uint8_t zp = at(1);
    unsigned nine = (cpu.p & 1u) << 8 | mem[zp];
    const unsigned k = static_cast<unsigned>(passes % 9);
    nine = ((nine << k) | (nine >> (9 - k))) & 0x1FF;
    mem[zp] = static_cast<std::uint8_t>(nine & 0xFF);
    cpu.p = static_cast<std::uint8_t>((cpu.p & ~0x01) | (nine >> 8));
  }
  cpu.p = static_cast<std::uint8_t>((cpu.p & ~0x80) | 0x02);
  cpu.cycles += passes * (per_pass + taken) - taken + 2;
  cpu.pc = static_cast<std::uint16_t>(end);
  seen[pc] = 1;
  seen[(end - 2) & 0xFFFF] = 1;
  if (op == 0x26) seen[(pc + 2u) & 0xFFFF] = 1;
  return true;
}

void Machine::call(std::uint16_t addr, std::uint64_t max_cycles) {
  game::push_return(cpu, kReturnTrap - 1);
  const std::uint16_t pc = cpu.pc;
  cpu.pc = addr;
  hooks[kReturnTrap] = []() -> std::optional<End> { return End::kReturned; };
  End why;
  try {
    why = run(cpu.cycles + max_cycles);
  } catch (...) {
    hooks.erase(kReturnTrap);
    throw;
  }
  hooks.erase(kReturnTrap);
  if (why != End::kReturned) throw std::runtime_error("$" + hex(addr, 4) + " didn't return (" + end_name(why) + ")");
  cpu.sp = static_cast<std::uint8_t>(cpu.sp + 2);
  cpu.pc = pc;
}

std::vector<std::uint8_t> entry_image(const std::vector<std::uint8_t>& main_bin) {
  std::vector<std::uint8_t> mem(0x10000);
  std::copy_n(main_bin.begin(), std::min<std::size_t>(main_bin.size(), 0x10000 - 0x0500), mem.begin() + 0x0500);
  for (const Relocation& r : kRelocations) {
    std::copy_n(mem.begin() + static_cast<std::ptrdiff_t>(r.home), r.size,
                mem.begin() + static_cast<std::ptrdiff_t>(r.staged));
  }
  for (const auto& [addr, value] : kEntryZeroPage) mem[addr] = value;
  std::copy(std::begin(kEntryVectors), std::end(kEntryVectors), mem.begin() + 0x03F0);
  return mem;
}

disk2::Drive entry_drive(const std::vector<std::uint8_t>& disk, bool wide_track) {
  disk2::Drive drive = disk2::Drive::from_dsk(disk);
  if (wide_track) {
    for (const auto& [half_track, track] : kWideTrack) drive.track_at.at(static_cast<std::size_t>(half_track)) = track;
  }
  drive.half_track = 10;
  drive.phases = 0b0100;
  return drive;
}

std::unique_ptr<Machine> original_machine(const std::vector<std::uint8_t>& main_bin,
                                          const std::vector<std::uint8_t>& disk, const std::vector<KeyPress>& keys,
                                          const std::vector<DiskSwap>& swaps, const DiskImages& disks,
                                          bool wide_track) {
  return make_machine(entry_image(main_bin), disk, keys, swaps, disks, wide_track);
}

std::vector<Span> link_regions(const std::string& script) {
  const auto value = [](const std::string& expr) {
    std::int64_t total = 0;
    std::int64_t sign = 1;
    static const std::regex token(R"(0[xX][0-9a-fA-F]+|\d+|[-+])");
    for (auto it = std::sregex_iterator(expr.begin(), expr.end(), token); it != std::sregex_iterator(); ++it) {
      const std::string t = it->str();
      if (t == "+" || t == "-") {
        sign = t == "+" ? 1 : -1;
      } else {
        total += sign * std::stoll(t, nullptr, t.size() > 1 && (t[1] == 'x' || t[1] == 'X') ? 16 : 10);
      }
    }
    return total;
  };
  std::smatch block;
  std::string body;
  if (std::regex_search(script, block, std::regex(R"(MEMORY\s*\{([\s\S]*?)\})"))) body = block[1].str();
  std::vector<Span> out;
  const std::regex region(R"(ORIGIN\s*=\s*([^,]+),\s*LENGTH\s*=\s*([^\n]+))");
  for (auto it = std::sregex_iterator(body.begin(), body.end(), region); it != std::sregex_iterator(); ++it) {
    const std::int64_t start = value((*it)[1].str());
    out.push_back(Span{start, start + value((*it)[2].str())});
  }
  return out;
}

std::unique_ptr<Machine> program_machine(const std::vector<std::uint8_t>& main_bin,
                                         const binutils::elf::ElfFile& exe, const std::vector<std::uint8_t>& disk,
                                         const std::vector<KeyPress>& keys, const std::vector<DiskSwap>& swaps,
                                         const DiskImages& disks, const ProgramOptions& options) {
  std::vector<std::uint8_t> mem = entry_image(main_bin);
  const auto put = options.staged ? put_staged : store;
  for (const Span& span : options.clear) {
    put(mem, span.start, std::vector<std::uint8_t>(static_cast<std::size_t>(span.end - span.start), 0));
  }
  for (const binutils::elf::Segment& seg : exe.segments) {
    std::vector<std::uint8_t> data(seg.data.begin(), seg.data.end());
    data.resize(static_cast<std::size_t>(std::max<std::int64_t>(seg.memsz, static_cast<std::int64_t>(data.size()))), 0);
    put(mem, seg.paddr, data);
  }
  std::unique_ptr<Machine> machine = make_machine(std::move(mem), disk, keys, swaps, disks, options.wide_track);
  const std::map<std::string, std::int64_t> symbols = game::program_symbols(exe);
  install_helpers(*machine, symbols, exe);
  if (options.ssp) {
    const auto at = static_cast<std::size_t>(lookup(symbols, "SSP").value_or(static_cast<std::int64_t>(passes::RuntimeLayout{}.ssp)));
    machine->cpu.memory[at] = static_cast<std::uint8_t>(*options.ssp & 0xFF);
    machine->cpu.memory[at + 1] = static_cast<std::uint8_t>(*options.ssp >> 8);
  }
  if (const auto init = lookup(symbols, "__zp_init")) {
    machine->call(static_cast<std::uint16_t>(*init));
    set_entry_registers(machine->cpu);
  }
  machine->cpu.pc = options.entry_address ? *options.entry_address
                                          : static_cast<std::uint16_t>(symbols.at(*options.entry_symbol));
  return machine;
}

namespace {

std::string upper(std::string s) {
  for (char& c : s) {
    if (c >= 'a' && c <= 'z') c = static_cast<char>(c - 'a' + 'A');
  }
  return s;
}

bool is_hex4(const std::string& s) {
  return s.size() == 4 && std::all_of(s.begin(), s.end(), [](char c) {
           return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
         });
}

// The directives and macros of main.asm that lay down data: the
// assembler's own, the event-list macros (EV...), and the bytecode of
// the disk VMs (VM... and DVM...).
bool is_data(const std::string& op) {
  static const char* const kDataOps[] = {"DC", "HEX", "DS", "INCLUDE", "BYTE", "WORD", "APSTR"};
  const std::string head = op.substr(0, op.find('.'));
  for (const char* d : kDataOps) {
    if (head == d) return true;
  }
  for (const char* prefix : {"EV", "VM", "DVM"}) {
    if (op.rfind(prefix, 0) == 0) return true;
  }
  return false;
}

}  // namespace

std::map<std::int64_t, Label> original_labels(const std::string& main_asm, const std::string& main_sym) {
  std::map<std::string, std::int64_t> addrs;
  for (const std::string& line : game::split_lines(main_sym)) {
    const std::vector<std::string> parts = game::split_words(line);
    if (parts.size() >= 2 && is_hex4(parts[1])) addrs[parts[0]] = std::stoll(parts[1], nullptr, 16);
  }
  std::map<std::int64_t, Label> out;
  std::vector<std::string> pending;
  for (const std::string& raw : game::split_lines(main_asm)) {
    std::string line = raw.substr(0, raw.find(';'));
    while (!line.empty() && game::is_space(line.back())) line.pop_back();
    if (game::strip(line).empty()) continue;
    std::string op;
    if (!game::is_space(line[0])) {
      const std::string first = game::split_words(line)[0];
      std::string head = first;
      while (!head.empty() && head.back() == ':') head.pop_back();
      const std::vector<std::string> rest = game::split_words(std::string_view(line).substr(first.size()));
      if (!rest.empty() && (upper(rest[0]) == "EQU" || rest[0] == "=")) continue;
      if (head.find('.') == std::string::npos && addrs.count(head) != 0) pending.push_back(head);
      if (rest.empty()) continue;
      op = upper(rest[0]);
    } else {
      op = upper(game::split_words(line)[0]);
    }
    if (op == "SUBROUTINE" || op == "ORG" || op == "PROCESSOR" || op == "MACRO" || op == "ENDM") {
      if (op == "ORG") pending.clear();
      continue;
    }
    if (!pending.empty()) {
      const bool code = !is_data(op);
      for (const std::string& name : pending) out.emplace(addrs.at(name), Label{name, code});
      pending.clear();
    }
  }
  return out;
}

std::vector<std::string> Coverage::reached() const {
  std::vector<std::string> out;
  for (const std::string& name : order) {
    if (routines.at(name).executed != 0) out.push_back(name);
  }
  return out;
}

std::vector<std::string> Coverage::missed() const {
  std::vector<std::string> out;
  for (const std::string& name : order) {
    if (routines.at(name).executed == 0) out.push_back(name);
  }
  return out;
}

std::string Coverage::summary() const {
  return std::to_string(reached().size()) + " of " + std::to_string(routines.size()) + " routines executed";
}

Coverage coverage(const std::vector<std::uint8_t>& seen, const std::map<std::int64_t, Label>& labels,
                  const std::vector<Span>* spans) {
  Coverage out;
  for (auto it = labels.begin(); it != labels.end(); ++it) {
    const auto& [a, label] = *it;
    if (!label.code) continue;
    if (spans != nullptr &&
        std::none_of(spans->begin(), spans->end(), [a = a](const Span& s) { return s.start <= a && a < s.end; })) {
      continue;
    }
    const auto next = std::next(it);
    const std::int64_t end = next != labels.end() ? next->first : 0xC000;
    std::int64_t executed = 0;
    for (std::int64_t addr = a; addr < end && addr < static_cast<std::int64_t>(seen.size()); ++addr) {
      executed += seen[static_cast<std::size_t>(addr)];
    }
    if (out.routines.emplace(label.name, RoutineCoverage{a, end, executed}).second) out.order.push_back(label.name);
  }
  return out;
}

}  // namespace c6502::sim::ali_baba
