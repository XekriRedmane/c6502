#include "sim/apple2.h"

#include <algorithm>
#include <stdexcept>

namespace c6502::sim::apple2 {

namespace {

constexpr std::uint16_t kWndlft = 0x20;
constexpr std::uint16_t kWndwdth = 0x21;
constexpr std::uint16_t kWndtop = 0x22;
constexpr std::uint16_t kWndbtm = 0x23;
constexpr std::uint16_t kCh = 0x24;
constexpr std::uint16_t kCv = 0x25;
constexpr std::uint16_t kInvflg = 0x32;
constexpr std::uint16_t kCsw = 0x36;
constexpr std::uint16_t kRndl = 0x4E;
constexpr std::uint8_t kSpace = 0xA0;

// DOS 3.3's sector interleave: logical sector s of a track is physical
// sector kSkew[s], the order a .dsk image keeps them in.
constexpr int kSkew[16] = {0x0, 0xD, 0xB, 0x9, 0x7, 0x5, 0x3, 0x1, 0xE, 0xC, 0xA, 0x8, 0x6, 0x4, 0x2, 0xF};

}  // namespace

std::size_t dsk_sector(int track, int sector) {
  return static_cast<std::size_t>(track * 16 + kSkew[sector]) * 256;
}

std::string_view routine_name(Routine routine) {
  switch (routine) {
    case Routine::kHome: return "HOME";
    case Routine::kClreol: return "CLREOL";
    case Routine::kVtab: return "VTAB";
    case Routine::kRdkey: return "RDKEY";
    case Routine::kGetln1: return "GETLN1";
    case Routine::kCout: return "COUT";
    case Routine::kCout1: return "COUT1";
  }
  return "?";
}

std::string_view stop_name(Stop stop) {
  switch (stop) {
    case Stop::kBrk: return "brk";
    case Stop::kHalt: return "halt";
    case Stop::kInputUsedUp: return "input used up";
  }
  return "?";
}

void Apple2::boot_screen() {
  m(kWndlft) = 0;
  m(kWndwdth) = 40;
  m(kWndtop) = 0;
  m(kWndbtm) = 24;
  m(kCh) = 0;
  m(kCv) = 0;
  m(kInvflg) = 0xFF;
  m(kCsw) = kCout1 & 0xFF;
  m(kCsw + 1) = kCout1 >> 8;
}

std::string Apple2::text() const {
  std::string out;
  for (std::size_t r = 0; r < screen.size(); ++r) {
    if (r != 0) out += '\n';
    std::string row;
    for (const std::uint8_t c : screen[r]) {
      int ch = c & 0x3F;
      if (ch < 0x20) ch += 0x40;
      if (c < 0x80 && ch >= 'A' && ch <= 'Z') ch += 'a' - 'A';
      row += static_cast<char>(ch);
    }
    row.erase(row.find_last_not_of(' ') + 1);
    out += row;
  }
  return out;
}

Apple2::Window Apple2::window() {
  const Window w{m(kWndlft), m(kWndwdth), m(kWndtop), m(kWndbtm)};
  if (w.left + w.width > kColumns || w.top >= w.bottom || w.bottom > kRows) {
    throw std::runtime_error("the text window (WNDLFT " + std::to_string(w.left) + ", WNDWDTH " +
                             std::to_string(w.width) + ", WNDTOP " + std::to_string(w.top) + ", WNDBTM " +
                             std::to_string(w.bottom) + ") is outside the 40-by-24 screen");
  }
  return w;
}

void Apple2::log(Routine routine) {
  std::optional<std::uint8_t> a;
  if (routine == Routine::kCout || routine == Routine::kCout1) a = cpu_.a;
  calls.emplace_back(MonitorCall{routine, a, m(kCh), m(kCv), m(kInvflg)});
  if (on_call) on_call(cpu_);
}

void Apple2::put(std::uint8_t c) {
  const std::size_t row = m(kCv);
  const std::size_t col = std::size_t{m(kWndlft)} + m(kCh);
  if (row < kRows && col < kColumns) screen[row][col] = c;
  m(kCh) = static_cast<std::uint8_t>(m(kCh) + 1);
  if (m(kCh) >= m(kWndwdth)) cr();
}

void Apple2::cr() {
  m(kCh) = 0;
  m(kCv) = static_cast<std::uint8_t>(m(kCv) + 1);
  if (m(kCv) >= m(kWndbtm)) {
    m(kCv) = static_cast<std::uint8_t>(m(kWndbtm) - 1);
    const Window w = window();
    for (std::size_t row = w.top; row + 1 < w.bottom; ++row) {
      std::copy_n(screen[row + 1].data() + w.left, w.width, screen[row].data() + w.left);
    }
    std::fill_n(screen[w.bottom - 1].data() + w.left, w.width, kSpace);
  }
}

void Apple2::screen_out(std::uint8_t a) {
  if (a >= 0xA0) {
    put(a & m(kInvflg));
  } else if (a == 0x8D) {
    cr();
  } else if (a == 0x88) {
    if (m(kCh) > 0) m(kCh) = static_cast<std::uint8_t>(m(kCh) - 1);
  }
}

void Apple2::cout1() {
  log(Routine::kCout1);
  screen_out(cpu_.a);
}

void Apple2::cout() {
  log(Routine::kCout);
  const int csw = m(kCsw) | m(kCsw + 1) << 8;
  if (csw == kPrinter) {
    printer.push_back(cpu_.a);
  } else {
    screen_out(cpu_.a);
  }
}

void Apple2::home() {
  log(Routine::kHome);
  const Window w = window();
  for (std::size_t row = w.top; row < w.bottom; ++row) {
    std::fill_n(screen[row].data() + w.left, w.width, kSpace);
  }
  m(kCh) = 0;
  m(kCv) = m(kWndtop);
}

void Apple2::clreol() {
  log(Routine::kClreol);
  auto& row = screen.at(m(kCv));
  const std::size_t end = std::min(std::size_t{m(kWndlft)} + m(kWndwdth), kColumns);
  for (std::size_t col = std::size_t{m(kWndlft)} + m(kCh); col < end; ++col) {
    row[col] = kSpace & m(kInvflg);
  }
}

void Apple2::vtab() { log(Routine::kVtab); }

// KEYIN counts RNDL/RNDH up once per pass of its loop while it waits
// for a key, so the count depends on how long the key took. Here each
// wait is a number of passes from a linear congruential generator, so
// that runs repeat. A program that calls its random routine many times
// between keys, as Zork does when the player dies, can loop forever on
// an unlucky seed, since Zork's routine only shifts the high byte
// between keys.
void Apple2::step_random() {
  lcg_ = (lcg_ * 1103515245 + 12345) & 0x7FFFFFFF;
  const std::uint64_t rnd = (m(kRndl) | m(kRndl + 1) << 8) + 1 + (lcg_ >> 16);
  m(kRndl) = static_cast<std::uint8_t>(rnd & 0xFF);
  m(kRndl + 1) = static_cast<std::uint8_t>((rnd >> 8) & 0xFF);
}

void Apple2::rdkey() {
  log(Routine::kRdkey);
  step_random();
  std::uint8_t key = 0x0D;
  if (!keys.empty()) {
    key = keys.front();
    keys.pop_front();
  }
  cpu_.a = key | 0x80;
}

void Apple2::getln1() {
  log(Routine::kGetln1);
  if (lines.empty()) throw InputUsedUp{};
  std::string line = lines.front();
  lines.pop_front();
  for (char& ch : line) {
    if (ch >= 'a' && ch <= 'z') ch = static_cast<char>(ch - 'a' + 'A');
  }
  for (std::size_t x = 0; x < line.size(); ++x) {
    step_random();
    const auto code = static_cast<std::uint8_t>(static_cast<unsigned char>(line[x]) | 0x80);
    m(static_cast<std::uint16_t>(0x0200 + x)) = code;
    screen_out(code);
  }
  m(static_cast<std::uint16_t>(0x0200 + line.size())) = 0x8D;
  clreol();
  screen_out(0x8D);
  cpu_.x = static_cast<std::uint8_t>(line.size());
  cpu_.a = 0x8D;
}

void Apple2::rwts() {
  const int iob = cpu_.a << 8 | cpu_.y;
  const std::uint8_t slot = m(static_cast<std::uint16_t>(iob + 1));
  const std::uint8_t drive = m(static_cast<std::uint16_t>(iob + 2));
  const std::uint8_t track = m(static_cast<std::uint16_t>(iob + 4));
  const std::uint8_t sector = m(static_cast<std::uint16_t>(iob + 5));
  const int buf = m(static_cast<std::uint16_t>(iob + 8)) | m(static_cast<std::uint16_t>(iob + 9)) << 8;
  const std::uint8_t command = m(static_cast<std::uint16_t>(iob + 12));
  calls.emplace_back(RwtsCall{slot, drive, track, sector, command});
  if (on_call) on_call(cpu_);
  const auto it = disks.find({slot, drive});
  if (it == disks.end() || track >= 35 || sector >= 16) {
    cpu_.p |= Cpu::kCarry;
    return;
  }
  std::vector<std::uint8_t>& disk = it->second;
  const std::size_t at = dsk_sector(track, sector);
  const std::size_t n = std::min<std::size_t>(256, 0x10000 - static_cast<std::size_t>(buf));
  if (command == 1) {
    std::copy_n(disk.begin() + static_cast<std::ptrdiff_t>(at), n, cpu_.memory.begin() + buf);
  } else if (command == 2) {
    std::copy_n(cpu_.memory.begin() + buf, n, disk.begin() + static_cast<std::ptrdiff_t>(at));
    written.push_back(SectorWrite{slot, drive, track, sector});
  }
  cpu_.p &= static_cast<std::uint8_t>(~Cpu::kCarry);
}

bool Apple2::run_hook(std::uint16_t pc) {
  switch (pc) {
    case kHome: home(); return true;
    case kClreol: clreol(); return true;
    case kVtab: vtab(); return true;
    case kRdkey: rdkey(); return true;
    case kGetln1: getln1(); return true;
    case kCout: cout(); return true;
    case kCout1: cout1(); return true;
    case kRwts: rwts(); return true;
    default: return false;
  }
}

Stop Apple2::run(std::uint64_t max_cycles, const std::map<std::uint16_t, Hook>* other_hooks) {
  try {
    while (cpu_.cycles < max_cycles) {
      const std::uint16_t pc = cpu_.pc;
      bool hooked = run_hook(pc);
      if (!hooked && other_hooks != nullptr) {
        const auto it = other_hooks->find(pc);
        if (it != other_hooks->end()) {
          it->second(cpu_.memory);
          hooked = true;
        }
      }
      if (hooked) {
        cpu_.rts();
        continue;
      }
      const std::uint8_t op = cpu_.memory[pc];
      if (op == 0x00) {
        stop_reason = Stop::kBrk;
        return *stop_reason;
      }
      if (op == 0x4C && (cpu_.memory[(pc + 1) & 0xFFFF] | cpu_.memory[(pc + 2) & 0xFFFF] << 8) == pc) {
        stop_reason = Stop::kHalt;
        return *stop_reason;
      }
      cpu_.step();
      ++instructions;
      if (after_step) after_step(cpu_, op);
    }
  } catch (const InputUsedUp&) {
    stop_reason = Stop::kInputUsedUp;
    return *stop_reason;
  }
  throw std::runtime_error("ran past " + std::to_string(max_cycles) + " cycles");
}

}  // namespace c6502::sim::apple2
