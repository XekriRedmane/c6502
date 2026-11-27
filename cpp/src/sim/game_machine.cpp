#include "sim/game_machine.h"

#include <algorithm>
#include <array>
#include <cstdio>
#include <string_view>

#include "binutils/opcodes.h"
#include "sim/cpu_run.h"

namespace c6502::sim::game {

namespace {

using binutils::opcodes::Mode;

enum class Register : std::uint8_t { kA, kX, kY };

struct Decoded {
  Mode mode = Mode::kImp;
  std::uint8_t access = 0;
  Register stored = Register::kA;
};

std::array<Decoded, 256> decode_table() {
  std::array<Decoded, 256> table{};
  for (const binutils::opcodes::Entry& e : binutils::opcodes::table()) {
    if (e.mode != Mode::kAbs && e.mode != Mode::kAbsx && e.mode != Mode::kAbsy && e.mode != Mode::kIndx &&
        e.mode != Mode::kIndy) {
      continue;
    }
    if (e.mnemonic == "JMP" || e.mnemonic == "JSR") continue;
    Decoded& d = table[e.opcode];
    d.mode = e.mode;
    const bool store = e.mnemonic == "STA" || e.mnemonic == "STX" || e.mnemonic == "STY";
    const bool read_write = e.mnemonic == "INC" || e.mnemonic == "DEC" || e.mnemonic == "ASL" ||
                            e.mnemonic == "LSR" || e.mnemonic == "ROL" || e.mnemonic == "ROR";
    d.access = store ? kWriteAccess : read_write ? kReadAccess | kWriteAccess : kReadAccess;
    d.stored = e.mnemonic == "STX" ? Register::kX : e.mnemonic == "STY" ? Register::kY : Register::kA;
  }
  return table;
}

const std::array<Decoded, 256>& decoded() {
  static const std::array<Decoded, 256> table = decode_table();
  return table;
}

}  // namespace

std::optional<SwitchAccess> soft_switch_access(const Cpu& cpu) {
  const std::vector<std::uint8_t>& mem = cpu.memory;
  const std::uint16_t pc = cpu.pc;
  const Decoded& d = decoded()[mem[pc]];
  if (d.access == 0) return std::nullopt;
  const auto byte = [&mem](unsigned addr) -> std::uint32_t { return mem[addr & 0xFFFF]; };
  std::uint32_t ea = 0;
  switch (d.mode) {
    case Mode::kAbs: ea = byte(pc + 1u) | byte(pc + 2u) << 8; break;
    case Mode::kAbsx: ea = (byte(pc + 1u) | byte(pc + 2u) << 8) + cpu.x; break;
    case Mode::kAbsy: ea = (byte(pc + 1u) | byte(pc + 2u) << 8) + cpu.y; break;
    case Mode::kIndy: {
      const std::uint32_t zp = byte(pc + 1u);
      ea = (byte(zp) | byte((zp + 1) & 0xFF) << 8) + cpu.y;
      break;
    }
    default: {
      const std::uint32_t zp = (byte(pc + 1u) + cpu.x) & 0xFF;
      ea = byte(zp) | byte((zp + 1) & 0xFF) << 8;
      break;
    }
  }
  if (ea >> 8 != 0xC0) return std::nullopt;
  const std::uint8_t value = d.stored == Register::kX ? cpu.x : d.stored == Register::kY ? cpu.y : cpu.a;
  return SwitchAccess{static_cast<std::uint16_t>(ea), d.access, value};
}

std::uint32_t PageCrc::operator()(const std::vector<std::uint8_t>& memory) {
  const auto first = memory.begin() + start_;
  const auto last = memory.begin() + end_;
  if (copy_.empty() || !std::equal(first, last, copy_.begin())) {
    copy_.assign(first, last);
    crc_ = cpu_run::crc32(std::string_view(reinterpret_cast<const char*>(copy_.data()), copy_.size()));
  }
  return crc_;
}

std::map<std::string, std::int64_t> program_symbols(const binutils::elf::ElfFile& exe) {
  std::map<std::string, std::int64_t> out;
  for (const binutils::elf::SymbolPtr& s : exe.symbols) {
    if (!s->defined() || s->type == binutils::elf::STT_FILE) continue;
    if (s->is_global() || out.count(s->name) == 0) out[s->name] = s->value;
  }
  return out;
}

std::map<std::int64_t, std::string> function_names(const binutils::elf::ElfFile& exe) {
  std::map<std::int64_t, std::string> out;
  for (const binutils::elf::SymbolPtr& s : exe.symbols) {
    if (s->defined() && s->type == binutils::elf::STT_FUNC) out[s->value] = s->name;
  }
  return out;
}

std::string routine_at(const std::map<std::int64_t, std::string>& routines, std::int64_t pc) {
  auto it = routines.upper_bound(pc);
  char buf[16];
  if (it == routines.begin()) {
    std::snprintf(buf, sizeof buf, "$%04llX", static_cast<long long>(pc));
    return buf;
  }
  --it;
  if (it->first == pc) return it->second;
  return it->second + "+" + std::to_string(pc - it->first);
}

void push_return(Cpu& cpu, std::uint16_t back) {
  cpu.memory[0x100 + cpu.sp] = static_cast<std::uint8_t>(back >> 8);
  cpu.memory[0x100 + ((cpu.sp - 1) & 0xFF)] = static_cast<std::uint8_t>(back & 0xFF);
  cpu.sp = static_cast<std::uint8_t>(cpu.sp - 2);
}

bool is_space(char c) { return c == ' ' || (c >= '\t' && c <= '\r') || (c >= '\x1c' && c <= '\x1f'); }

std::vector<std::string> split_words(std::string_view text) {
  std::vector<std::string> out;
  std::size_t i = 0;
  while (i < text.size()) {
    while (i < text.size() && is_space(text[i])) ++i;
    if (i == text.size()) break;
    const std::size_t start = i;
    while (i < text.size() && !is_space(text[i])) ++i;
    out.emplace_back(text.substr(start, i - start));
  }
  return out;
}

std::string strip(std::string_view text) {
  std::size_t first = 0;
  std::size_t last = text.size();
  while (first < last && is_space(text[first])) ++first;
  while (last > first && is_space(text[last - 1])) --last;
  return std::string(text.substr(first, last - first));
}

std::vector<std::string> split_lines(std::string_view text) {
  std::vector<std::string> out;
  std::size_t start = 0;
  std::size_t i = 0;
  while (i < text.size()) {
    const char c = text[i];
    if (c == '\n' || c == '\r' || c == '\v' || c == '\f' || (c >= '\x1c' && c <= '\x1e')) {
      out.emplace_back(text.substr(start, i - start));
      i += c == '\r' && i + 1 < text.size() && text[i + 1] == '\n' ? 2 : 1;
      start = i;
    } else {
      ++i;
    }
  }
  if (start < text.size()) out.emplace_back(text.substr(start));
  return out;
}

}  // namespace c6502::sim::game
