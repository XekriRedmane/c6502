#include "sim/cpu_run.h"

#include <array>
#include <string>

namespace c6502::sim::cpu_run {

namespace {

const std::array<std::uint32_t, 256>& crc_table() {
  static const std::array<std::uint32_t, 256> table = [] {
    std::array<std::uint32_t, 256> t{};
    for (std::uint32_t n = 0; n < 256; ++n) {
      std::uint32_t c = n;
      for (int k = 0; k < 8; ++k) c = (c & 1) != 0 ? 0xEDB88320U ^ (c >> 1) : c >> 1;
      t[n] = c;
    }
    return t;
  }();
  return table;
}

std::uint32_t crc_byte(std::uint32_t crc, std::uint8_t b) {
  return crc_table()[(crc ^ b) & 0xFF] ^ (crc >> 8);
}

}  // namespace

std::uint32_t crc32(std::string_view bytes) {
  std::uint32_t crc = 0xFFFFFFFFU;
  for (const char c : bytes) crc = crc_byte(crc, static_cast<std::uint8_t>(c));
  return crc ^ 0xFFFFFFFFU;
}

std::uint32_t digest(const Cpu& cpu, const std::vector<std::pair<std::uint16_t, std::uint8_t>>& writes) {
  std::uint32_t crc = 0xFFFFFFFFU;
  const auto add = [&crc](unsigned b) { crc = crc_byte(crc, static_cast<std::uint8_t>(b)); };
  add(cpu.pc);
  add(cpu.pc >> 8);
  add(cpu.a);
  add(cpu.x);
  add(cpu.y);
  add(cpu.sp);
  add(cpu.p);
  for (int i = 0; i < 8; ++i) add(static_cast<unsigned>(cpu.cycles >> (8 * i)));
  for (const auto& [addr, value] : writes) {
    add(addr);
    add(addr >> 8u);
    add(value);
  }
  return crc ^ 0xFFFFFFFFU;
}

}  // namespace c6502::sim::cpu_run
