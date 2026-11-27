#include "sim/lode_runner.h"

#include <algorithm>
#include <cstdio>
#include <regex>
#include <stdexcept>

#include "sim/apple2.h"

namespace c6502::sim::lode_runner {

namespace {

constexpr std::uint16_t kPaddl0 = 0xC064;
constexpr std::uint16_t kPaddl1 = 0xC065;
// Where `call` makes a subroutine return to. Nothing of the game's is
// there, since it's in the Monitor's ROM.
constexpr std::uint16_t kReturnTrap = 0xFFF0;

std::string hex(unsigned value, int digits) {
  char buf[16];
  std::snprintf(buf, sizeof buf, "%0*X", digits, value);
  return buf;
}

}  // namespace

std::string Events::describe(std::size_t i) const {
  const Event& e = events.at(i);
  std::string what;
  if (e.code >> 1 == kRwtsEvent) {
    const RwtsRecord& r = rwts.at(i);
    what = "RWTS command " + std::to_string(r.command) + " T$" + hex(r.track, 2) + " S$" + hex(r.sector, 1);
  } else {
    what = std::string((e.code & 1) != 0 ? "write" : "read") + " $" + hex(e.code >> 1, 4);
  }
  return "#" + std::to_string(i) + " " + what + " at PC $" + hex(e.pc, 4) + ", cycle " + std::to_string(e.cycles) +
         ", pages " + hex(e.page1, 8) + " " + hex(e.page2, 8);
}

std::optional<std::size_t> first_difference(const Events& a, const Events& b) {
  const std::size_t common = std::min(a.size(), b.size());
  std::size_t n = common;
  for (std::size_t i = 0; i < common; ++i) {
    const Event& x = a.events[i];
    const Event& y = b.events[i];
    if (x.code != y.code || x.page1 != y.page1 || x.page2 != y.page2) {
      n = i;
      break;
    }
  }
  std::vector<std::size_t> indices;
  for (const auto& [i, _] : a.rwts) indices.push_back(i);
  for (const auto& [i, _] : b.rwts) indices.push_back(i);
  std::sort(indices.begin(), indices.end());
  for (const std::size_t i : indices) {
    if (i >= n) break;
    const auto x = a.rwts.find(i);
    const auto y = b.rwts.find(i);
    const bool same = (x == a.rwts.end()) == (y == b.rwts.end()) && (x == a.rwts.end() || x->second == y->second);
    if (!same) {
      n = i;
      break;
    }
  }
  if (n < common) return n;
  return std::nullopt;
}

Machine::Machine(Cpu cpu_in, std::vector<std::uint8_t> disk_in, const std::vector<KeyPress>& keys)
    : cpu(std::move(cpu_in)), disk(std::move(disk_in)) {
  for (const KeyPress& k : keys) keys_[k.kbd_read] = k.key;
}

void Machine::record(std::uint32_t code, std::uint16_t pc) {
  const std::uint32_t p1 = page1_crc_(cpu.memory);
  const std::uint32_t p2 = page2_crc_(cpu.memory);
  events.events.push_back(Event{code, p1, p2, pc, cpu.cycles});
  if (on_event) on_event(cpu);
}

void Machine::access(const game::SwitchAccess& sw, std::uint16_t pc) {
  const std::uint16_t addr = sw.address;
  if ((sw.access & game::kReadAccess) != 0) record(std::uint32_t{addr} << 1, pc);
  if ((sw.access & game::kWriteAccess) != 0) record(std::uint32_t{addr} << 1 | 1, pc);
  std::uint8_t& mem = cpu.memory[addr];
  const unsigned page = addr & 0xF0;
  if (page == 0x00) {
    if ((sw.access & game::kReadAccess) != 0) {
      const auto it = keys_.find(kbd_reads);
      if (it != keys_.end()) kbd_latch = it->second | 0x80;
      ++kbd_reads;
    }
    mem = kbd_latch;
  } else if (page == 0x10) {
    kbd_latch &= 0x7F;
    mem = kbd_latch;
  } else if (page == 0x50) {
    const unsigned sw_index = addr & 0x0F;
    if (sw_index < 8) {
      const bool on = (sw_index & 1) != 0;
      if (sw_index < 2) {
        text = on;
      } else if (sw_index < 4) {
        mixed = on;
      } else if (sw_index < 6) {
        page2 = on;
      } else {
        hires = on;
      }
    }
    mem = 0;
  } else if (addr == kPaddl0 || addr == kPaddl1) {
    mem = 0x80;
  } else {
    mem = 0;
  }
}

void Machine::rwts() {
  std::vector<std::uint8_t>& m = cpu.memory;
  const unsigned iob = cpu.a << 8 | cpu.y;
  const std::uint8_t track = m[(iob + 4) & 0xFFFF];
  const std::uint8_t sector = m[(iob + 5) & 0xFFFF];
  const unsigned buf = m[(iob + 8) & 0xFFFF] | m[(iob + 9) & 0xFFFF] << 8;
  const std::uint8_t command = m[(iob + 12) & 0xFFFF];
  const bool ok = track < 35 && sector < 16 && (command == 1 || command == 2);
  const std::size_t n = std::min<std::size_t>(256, 0x10000 - buf);
  if (ok) {
    const std::size_t at = apple2::dsk_sector(track, sector);
    if (command == 1) {
      std::copy_n(disk.begin() + static_cast<std::ptrdiff_t>(at), n, m.begin() + buf);
    } else {
      std::copy_n(m.begin() + buf, n, disk.begin() + static_cast<std::ptrdiff_t>(at));
    }
  }
  record(kRwtsEvent << 1, cpu.pc);
  events.rwts[events.size() - 1] =
      RwtsRecord{command, track, sector, std::vector<std::uint8_t>(m.begin() + buf, m.begin() + buf + n)};
  m[(iob + 13) & 0xFFFF] = ok ? 0 : 0x40;
  if (ok) {
    cpu.p &= static_cast<std::uint8_t>(~Cpu::kCarry);
  } else {
    cpu.p |= Cpu::kCarry;
  }
}

End Machine::run(std::uint64_t max_cycles, const std::function<bool(const Machine&)>& stop) {
  std::size_t n_events = events.size();
  while (cpu.cycles < max_cycles) {
    const std::uint16_t pc = cpu.pc;
    if (return_trap_ && pc == kReturnTrap) return End::kReturned;
    if (pc == kRwtsEntry) {
      rwts();
      cpu.rts();
    } else {
      const std::uint8_t op = cpu.memory[pc];
      if (op == 0x00) return End::kBrk;
      if (const std::optional<game::SwitchAccess> sw = game::soft_switch_access(cpu)) access(*sw, pc);
      cpu.step();
      ++instructions;
      if (after_step) after_step(cpu, op);
    }
    if (stop && events.size() != n_events) {
      n_events = events.size();
      if (stop(*this)) return End::kStop;
    }
  }
  return End::kCycles;
}

void Machine::call(std::uint16_t addr, std::uint64_t max_cycles) {
  game::push_return(cpu, kReturnTrap - 1);
  cpu.pc = addr;
  return_trap_ = true;
  End why;
  try {
    why = run(cpu.cycles + max_cycles);
  } catch (...) {
    return_trap_ = false;
    throw;
  }
  return_trap_ = false;
  if (why != End::kReturned) throw std::runtime_error("$" + hex(addr, 4) + " didn't return");
}

Machine original_machine(const std::vector<std::uint8_t>& image, const std::vector<std::uint8_t>& disk,
                         const std::vector<KeyPress>& keys) {
  Cpu cpu;
  std::copy_n(image.begin(), std::min<std::size_t>(image.size(), 0x10000), cpu.memory.begin());
  cpu.pc = kMain;
  return Machine(std::move(cpu), disk, keys);
}

Machine program_machine(const std::vector<std::uint8_t>& image, const binutils::elf::ElfFile& exe,
                        const std::vector<std::uint8_t>& disk, const std::vector<KeyPress>& keys) {
  Cpu cpu;
  std::copy_n(image.begin(), std::min<std::size_t>(image.size(), 0x10000), cpu.memory.begin());
  std::fill(cpu.memory.begin() + kCodeStart, cpu.memory.begin() + kCodeEnd, std::uint8_t{0});
  for (const binutils::elf::Segment& seg : exe.segments) {
    if (seg.paddr < 0 || seg.paddr + seg.memsz > 0x10000) {
      throw std::runtime_error("the executable has a segment outside $0000-$FFFF");
    }
    std::fill_n(cpu.memory.begin() + seg.paddr, seg.memsz, std::uint8_t{0});
    std::copy(seg.data.begin(), seg.data.end(), cpu.memory.begin() + seg.paddr);
  }
  cpu.sp = 0x07;
  const std::map<std::string, std::int64_t> symbols = game::program_symbols(exe);
  Machine machine(std::move(cpu), disk, keys);
  if (const auto it = symbols.find("__zp_init"); it != symbols.end()) {
    machine.call(static_cast<std::uint16_t>(it->second));
  }
  machine.cpu.pc = static_cast<std::uint16_t>(symbols.at("main"));
  return machine;
}

namespace {

bool is_branch(const std::string& mnemonic) {
  static const char* const kBranches[] = {"BCC", "BCS", "BEQ", "BMI", "BNE", "BPL", "BVC", "BVS"};
  return std::any_of(std::begin(kBranches), std::end(kBranches), [&](const char* b) { return mnemonic == b; });
}

std::string upper(std::string s) {
  for (char& c : s) {
    if (c >= 'a' && c <= 'z') c = static_cast<char>(c - 'a' + 'A');
  }
  return s;
}

std::size_t count_of(const std::string& s, char sep) { return static_cast<std::size_t>(std::count(s.begin(), s.end(), sep)) + 1; }

std::string lstrip_dollar(const std::string& s) {
  const std::size_t i = s.find_first_not_of('$');
  return i == std::string::npos ? "" : s.substr(i);
}

}  // namespace

std::map<std::int64_t, std::string> original_routines(const std::string& main_asm) {
  std::vector<std::string> lines;
  for (const std::string& ln : game::split_lines(main_asm)) {
    std::string code = ln.substr(0, ln.find(';'));
    while (!code.empty() && game::is_space(code.back())) code.pop_back();
    lines.push_back(code);
  }
  std::map<std::string, std::int64_t> equs;
  const std::regex equ(R"(([A-Za-z_][A-Za-z0-9_]*)\s+EQU\s+#?\$([0-9A-Fa-f]+))");
  for (const std::string& ln : lines) {
    std::smatch m;
    if (std::regex_search(ln, m, equ, std::regex_constants::match_continuous)) {
      equs[m[1].str()] = std::stoll(m[2].str(), nullptr, 16);
    }
  }
  const std::regex value_re(R"(\$([0-9A-Fa-f]+)|([A-Za-z_][A-Za-z0-9_]*)(?:\+(\d+|\$[0-9A-Fa-f]+))?)");
  const auto value = [&](const std::string& expr) -> std::optional<std::int64_t> {
    std::smatch m;
    if (!std::regex_match(expr, m, value_re)) return std::nullopt;
    if (m[1].matched) return std::stoll(m[1].str(), nullptr, 16);
    const auto base = equs.find(m[2].str());
    if (base == equs.end()) return std::nullopt;
    const std::string off = m[3].matched ? m[3].str() : "0";
    return base->second + (off[0] == '$' ? std::stoll(off.substr(1), nullptr, 16) : std::stoll(off));
  };
  const auto size = [&](const std::string& mnemonic, const std::string& operand) -> std::int64_t {
    if (is_branch(mnemonic)) return 2;
    if (mnemonic == "JMP" || mnemonic == "JSR") return 3;
    if (operand.empty() || operand == "A") return 1;
    if (operand[0] == '#') return 2;
    if (operand[0] == '(') return mnemonic == "JMP" ? 3 : 2;
    const std::optional<std::int64_t> v = value(operand.substr(0, operand.find(',')));
    return v && *v < 0x100 ? 2 : 3;
  };

  std::map<std::int64_t, std::string> out;
  std::optional<std::int64_t> pc;
  for (std::string ln : lines) {
    if (game::strip(ln).empty()) continue;
    if (!game::is_space(ln[0])) {
      const std::string first = game::split_words(ln)[0];
      std::string name = first;
      while (!name.empty() && name.back() == ':') name.pop_back();
      const std::string rest = game::strip(std::string_view(ln).substr(first.size()));
      if (rest.rfind("EQU", 0) == 0) continue;
      if (pc && name.rfind('.', 0) != 0) out.emplace(*pc, name);
      ln = " " + rest;
      if (rest.empty()) continue;
    }
    const std::vector<std::string> words = game::split_words(ln);
    const std::string op = upper(words[0]);
    std::string arg;
    if (words.size() > 1) {
      const std::string stripped = game::strip(ln);
      arg = game::strip(std::string_view(stripped).substr(words[0].size()));
    }
    if (op == "ORG") {
      pc = std::stoll(lstrip_dollar(arg), nullptr, 16);
    } else if (!pc || op == "SUBROUTINE" || op == "PROCESSOR" || op == "INCLUDE") {
      continue;
    } else if (op == "HEX") {
      *pc += static_cast<std::int64_t>(game::split_words(arg).size());
    } else if (op == "BYTE") {
      *pc += static_cast<std::int64_t>(count_of(arg, ','));
    } else if (op == "WORD") {
      *pc += 2 * static_cast<std::int64_t>(count_of(arg, ','));
    } else if (op == "DS") {
      *pc += std::stoll(lstrip_dollar(arg), nullptr, !arg.empty() && arg[0] == '$' ? 16 : 10);
    } else {
      std::string operand;
      for (const char c : arg) {
        if (c != ' ') operand += c;
      }
      *pc += size(op, operand);
    }
  }
  return out;
}

}  // namespace c6502::sim::lode_runner
