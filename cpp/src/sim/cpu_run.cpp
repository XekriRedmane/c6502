#include "sim/cpu_run.h"

#include <array>
#include <bitset>
#include <cstdio>
#include <stdexcept>
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

std::uint64_t unsigned_of(const json::Value& v, std::string_view what) {
  if (v.kind != json::Value::Kind::kInt || v.integer < 0 || v.integer > asdl::Int(UINT64_MAX)) {
    throw std::runtime_error("the cpu-run request's " + std::string(what) + " isn't an unsigned integer");
  }
  return static_cast<std::uint64_t>(v.integer);
}

std::uint64_t field(const json::Value& object, std::string_view name, std::uint64_t limit) {
  const json::Value* v = object.get(name);
  if (v == nullptr) throw std::runtime_error("the cpu-run request has no " + std::string(name));
  const std::uint64_t n = unsigned_of(*v, name);
  if (n > limit) throw std::runtime_error("the cpu-run request's " + std::string(name) + " is too large");
  return n;
}

const char* stop_name(Cpu::Stop stop) {
  switch (stop) {
    case Cpu::Stop::kTrap: return "stop";
    case Cpu::Stop::kBrk: return "brk";
    case Cpu::Stop::kCycles: return "cycles";
    case Cpu::Stop::kInstructions: return "count";
    case Cpu::Stop::kCallback: break;
  }
  throw std::logic_error("cpu-run sets no callback that stops");
}

std::string hex(unsigned value, int digits) {
  char buf[8];
  std::snprintf(buf, sizeof buf, "%0*X", digits, value);
  return buf;
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

std::string respond(const json::Value& kwargs, std::string_view image) {
  if (image.size() != 0x10000) throw std::runtime_error("the cpu-run request's image isn't 65536 bytes");
  Cpu cpu;
  for (std::size_t i = 0; i < image.size(); ++i) cpu.memory[i] = static_cast<std::uint8_t>(image[i]);
  const json::Value* stops = kwargs.get("stops");
  if (stops == nullptr || stops->kind != json::Value::Kind::kArray) {
    throw std::runtime_error("the cpu-run request has no list of stops");
  }
  for (const json::Value& s : stops->items) {
    const auto addr = static_cast<std::uint16_t>(unsigned_of(s, "stop") & 0xFFFF);
    cpu.set_trap(addr, [](Cpu&) { return Cpu::Action::kStop; });
  }
  Cpu::Limits limits;
  const json::Value* brk = kwargs.get("stop_at_brk");
  limits.stop_at_brk = brk == nullptr || brk->kind != json::Value::Kind::kBool || brk->boolean;
  limits.max_cycles = field(kwargs, "max_cycles", UINT64_MAX);
  if (const json::Value* n = kwargs.get("max_instructions"); n != nullptr && n->kind != json::Value::Kind::kNull) {
    limits.max_instructions = unsigned_of(*n, "max_instructions");
  }
  const json::Value* segments = kwargs.get("segments");
  if (segments == nullptr || segments->kind != json::Value::Kind::kArray) {
    throw std::runtime_error("the cpu-run request has no list of segments");
  }

  std::vector<std::pair<std::uint16_t, std::uint8_t>> log;
  std::vector<std::pair<std::uint16_t, std::uint8_t>> last;
  std::string digests;
  std::bitset<0x10000> written;
  cpu.write_log = &log;
  cpu.after_instruction = [&](Cpu& c) {
    const std::uint32_t d = digest(c, log);
    for (int i = 0; i < 4; ++i) digests += static_cast<char>((d >> (8 * i)) & 0xFF);
    for (const auto& w : log) written[w.first] = true;
    last.swap(log);
    log.clear();
  };

  std::string out;
  for (std::size_t k = 0; k < segments->items.size(); ++k) {
    const json::Value& seg = segments->items[k];
    cpu.pc = static_cast<std::uint16_t>(field(seg, "pc", 0xFFFF));
    cpu.a = static_cast<std::uint8_t>(field(seg, "a", 0xFF));
    cpu.x = static_cast<std::uint8_t>(field(seg, "x", 0xFF));
    cpu.y = static_cast<std::uint8_t>(field(seg, "y", 0xFF));
    cpu.sp = static_cast<std::uint8_t>(field(seg, "sp", 0xFF));
    cpu.p = static_cast<std::uint8_t>(field(seg, "p", 0xFF));
    cpu.cycles = field(seg, "cycles", UINT64_MAX);
    if (const json::Value* patches = seg.get("patches")) {
      for (const json::Value& pair : patches->items) {
        if (pair.items.size() != 2) throw std::runtime_error("a cpu-run patch isn't a pair");
        cpu.memory[unsigned_of(pair.items[0], "patch address") & 0xFFFF] =
            static_cast<std::uint8_t>(unsigned_of(pair.items[1], "patch value"));
      }
    }
    Cpu::Limits segment_limits = limits;
    if (const json::Value* n = seg.get("instructions"); n != nullptr && n->kind != json::Value::Kind::kNull) {
      segment_limits.max_instructions = unsigned_of(*n, "segment's instructions");
    }
    digests.clear();
    written.reset();
    log.clear();
    last.clear();
    std::string stop;
    std::uint64_t instructions = 0;
    std::string error;
    try {
      const Cpu::RunResult result = cpu.run(segment_limits);
      stop = stop_name(result.stop);
      instructions = result.instructions;
    } catch (const CpuError& e) {
      stop = "error";
      instructions = e.instructions;
      error = e.what();
    }
    out += "segment " + std::to_string(k) + " " + stop + " " + std::to_string(instructions) + "\n";
    out += "state " + std::to_string(cpu.pc) + " " + std::to_string(cpu.a) + " " + std::to_string(cpu.x) +
           " " + std::to_string(cpu.y) + " " + std::to_string(cpu.sp) + " " + std::to_string(cpu.p) + " " +
           std::to_string(cpu.cycles) + "\n";
    out += "last-writes";
    for (const auto& [addr, value] : last) out += " " + hex(addr, 4) + ":" + hex(value, 2);
    out += "\n";
    std::string bytes;
    for (std::size_t addr = 0; addr < 0x10000; ++addr) {
      if (!written[addr]) continue;
      bytes += static_cast<char>(addr & 0xFF);
      bytes += static_cast<char>(addr >> 8);
      bytes += static_cast<char>(cpu.memory[addr]);
    }
    out += "writes " + std::to_string(bytes.size()) + "\n" + bytes;
    out += "digests " + std::to_string(digests.size()) + "\n" + digests;
    if (!error.empty()) {
      out += "error " + json::quote(error) + "\n";
      break;
    }
  }
  return out;
}

}  // namespace c6502::sim::cpu_run
