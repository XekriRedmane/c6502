// The simulator's CPU (src/sim/cpu.h) against py65 1.2.0's results,
// which tools/py65_cpu_vectors.py froze in cpp/tests/frozen/cpu, and
// its callbacks, its run loop, and the backend mode `cpu-run`
// (src/sim/cpu_run.h).
#include <cstdint>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "backend.h"
#include "compile_case.h"
#include "doctest.h"
#include "sim/cpu.h"
#include "sim/cpu_run.h"

namespace {

using c6502::sim::Cpu;
using c6502::sim::CpuError;

std::vector<std::string> split(const std::string& text, char sep) {
  std::vector<std::string> out;
  std::string piece;
  std::istringstream in(text);
  while (std::getline(in, piece, sep)) out.push_back(piece);
  return out;
}

std::vector<std::string> words(const std::string& text) {
  std::vector<std::string> out;
  std::istringstream in(text);
  std::string w;
  while (in >> w) out.push_back(w);
  return out;
}

unsigned hex(const std::string& s) { return static_cast<unsigned>(std::stoul(s, nullptr, 16)); }

// The `addr:value` pairs of a vector's field.
std::vector<std::pair<std::uint16_t, std::uint8_t>> pairs(const std::string& field) {
  std::vector<std::pair<std::uint16_t, std::uint8_t>> out;
  for (const std::string& w : words(field)) {
    const std::size_t colon = w.find(':');
    out.emplace_back(static_cast<std::uint16_t>(hex(w.substr(0, colon))),
                     static_cast<std::uint8_t>(hex(w.substr(colon + 1))));
  }
  return out;
}

std::string state_text(const Cpu& c) {
  char buf[64];
  std::snprintf(buf, sizeof buf, "%04X %02X %02X %02X %02X %02X", c.pc, c.a, c.x, c.y, c.sp, c.p);
  return buf;
}

std::string pairs_text(const std::vector<std::pair<std::uint16_t, std::uint8_t>>& ps) {
  std::string out;
  char buf[16];
  for (const auto& [addr, value] : ps) {
    std::snprintf(buf, sizeof buf, "%s%04X:%02X", out.empty() ? "" : " ", addr, value);
    out += buf;
  }
  return out;
}

// A CPU that runs `bytes` from `origin`, with its traps and callbacks
// unset.
Cpu cpu_with(std::uint16_t origin, const std::vector<std::uint8_t>& bytes) {
  Cpu cpu;
  for (std::size_t i = 0; i < bytes.size(); ++i) cpu.memory[(origin + i) & 0xFFFF] = bytes[i];
  cpu.pc = origin;
  return cpu;
}

}  // namespace

TEST_CASE("cpu: the cycle tables are py65's") {
  const std::vector<std::string> lines =
      split(c6502::suite::read_repo_file("cpp/tests/frozen/cpu/vectors.txt"), '\n');
  REQUIRE(lines.size() > 2);
  const std::vector<std::string> cycletime = words(lines[0]);
  const std::vector<std::string> extra = words(lines[1]);
  REQUIRE(cycletime.size() == 2);
  REQUIRE(extra.size() == 2);
  CHECK(cycletime[0] == "cycletime");
  CHECK(extra[0] == "extracycles");
  REQUIRE(cycletime[1].size() == 256);
  REQUIRE(extra[1].size() == 256);
  int undefined = 0;
  for (int op = 0; op < 256; ++op) {
    INFO("opcode " << op);
    CHECK(c6502::sim::kCycleTime[op] == hex(cycletime[1].substr(op, 1)));
    CHECK(c6502::sim::kExtraCycles[op] == hex(extra[1].substr(op, 1)));
    if (c6502::sim::kCycleTime[op] == 0) ++undefined;
  }
  CHECK(undefined == 105);
}

TEST_CASE("cpu: every single step that py65 took gives py65's reads, state, cycles, and writes") {
  const std::vector<std::string> lines =
      split(c6502::suite::read_repo_file("cpp/tests/frozen/cpu/vectors.txt"), '\n');
  int vectors = 0;
  int errors = 0;
  bool opcodes[256] = {};
  for (std::size_t i = 2; i < lines.size(); ++i) {
    const std::string& line = lines[i];
    if (line.empty()) continue;
    INFO(line);
    std::vector<std::string> fields = split(line, '|');
    while (fields.size() < 5) fields.emplace_back();
    const std::vector<std::string> before = words(fields[0]);
    REQUIRE(before.size() == 8);
    Cpu cpu;
    for (const auto& [addr, value] : pairs(fields[1])) cpu.memory[addr] = value;
    cpu.pc = static_cast<std::uint16_t>(hex(before[1]));
    cpu.a = static_cast<std::uint8_t>(hex(before[2]));
    cpu.x = static_cast<std::uint8_t>(hex(before[3]));
    cpu.y = static_cast<std::uint8_t>(hex(before[4]));
    cpu.sp = static_cast<std::uint8_t>(hex(before[5]));
    cpu.p = static_cast<std::uint8_t>(hex(before[6]));
    cpu.cycles = std::stoull(before[7], nullptr, 16);
    REQUIRE(cpu.memory[cpu.pc] == hex(before[0]));
    opcodes[hex(before[0])] = true;
    std::vector<std::uint16_t> reads;
    std::vector<std::pair<std::uint16_t, std::uint8_t>> writes;
    cpu.set_io(0x0000, 0xFFFF, [&](std::uint16_t addr) {
      reads.push_back(addr);
      return cpu.memory[addr];
    }, nullptr);
    cpu.write_log = &writes;
    const std::uint64_t start = cpu.cycles;
    bool threw = false;
    try {
      cpu.step();
    } catch (const CpuError&) {
      threw = true;
    }
    std::string got_reads;
    char buf[8];
    for (const std::uint16_t r : reads) {
      std::snprintf(buf, sizeof buf, "%s%04X", got_reads.empty() ? "" : " ", r);
      got_reads += buf;
    }
    const std::vector<std::string> want_reads = words(fields[2]);
    std::string want_reads_text;
    for (const std::string& w : want_reads) want_reads_text += (want_reads_text.empty() ? "" : " ") + w;
    CHECK(got_reads == want_reads_text);
    ++vectors;
    if (words(fields[3]) == std::vector<std::string>{"error"}) {
      CHECK(threw);
      ++errors;
      continue;
    }
    REQUIRE_FALSE(threw);
    const std::vector<std::string> after = words(fields[3]);
    REQUIRE(after.size() == 7);
    CHECK(state_text(cpu) ==
          after[0] + " " + after[1] + " " + after[2] + " " + after[3] + " " + after[4] + " " + after[5]);
    CHECK(cpu.cycles - start == hex(after[6].substr(1)));
    CHECK(pairs_text(writes) == pairs_text(pairs(fields[4])));
  }
  int covered = 0;
  for (const bool b : opcodes) covered += b ? 1 : 0;
  CHECK(covered == 256);
  CHECK(vectors == 2971);
  CHECK(errors == 48);
}

TEST_CASE("cpu: decimal ADC and SBC give py65's A and P for every accumulator, operand, and carry") {
  const std::vector<std::string> lines =
      split(c6502::suite::read_repo_file("cpp/tests/frozen/cpu/decimal.txt"), '\n');
  int rows = 0;
  for (const std::string& line : lines) {
    if (line.empty()) continue;
    INFO(line);
    const std::vector<std::string> w = words(line);
    REQUIRE(w.size() == 4);
    const std::uint8_t opcode = w[0] == "ADC" ? 0x69 : 0xE9;
    const int carry = std::stoi(w[1]);
    const auto a = static_cast<std::uint8_t>(hex(w[2]));
    std::string out;
    Cpu cpu;
    for (int b = 0; b < 256; ++b) {
      cpu.memory[0x200] = opcode;
      cpu.memory[0x201] = static_cast<std::uint8_t>(b);
      cpu.pc = 0x200;
      cpu.a = a;
      cpu.p = static_cast<std::uint8_t>(Cpu::kDecimal | Cpu::kUnused | carry);
      cpu.step();
      out += static_cast<char>(cpu.a);
      out += static_cast<char>(cpu.p);
    }
    CHECK(c6502::sim::cpu_run::crc32(out) == hex(w[3]));
    ++rows;
  }
  CHECK(rows == 1024);
}

TEST_CASE("cpu: reset gives py65's state") {
  Cpu cpu;
  cpu.pc = 0x1234;
  cpu.a = cpu.x = cpu.y = 7;
  cpu.sp = 3;
  cpu.p = 0xC3;
  cpu.cycles = 99;
  cpu.reset(0x0800);
  CHECK(cpu.pc == 0x0800);
  CHECK(cpu.sp == 0xFF);
  CHECK(cpu.a == 0);
  CHECK(cpu.x == 0);
  CHECK(cpu.y == 0);
  CHECK(cpu.p == 0x30);
  CHECK(cpu.cycles == 0);
}

TEST_CASE("cpu: the read and write callbacks see a read-modify-write's read before its write") {
  // INC $C030; LDX #$09; LSR $C0E0,X; STA $C010; LDA $C000; LDA $C100
  Cpu cpu = cpu_with(0x0800, {0xEE, 0x30, 0xC0, 0xA2, 0x09, 0x5E, 0xE0, 0xC0, 0x8D, 0x10, 0xC0, 0xAD,
                              0x00, 0xC0, 0xAD, 0x00, 0xC1});
  cpu.memory[0xC030] = 0x55;
  cpu.memory[0xC100] = 0x66;
  cpu.a = 0x12;
  std::vector<std::string> events;
  char buf[32];
  cpu.set_io(
      0xC000, 0xC0FF,
      [&](std::uint16_t addr) {
        std::snprintf(buf, sizeof buf, "read %04X", addr);
        events.emplace_back(buf);
        return static_cast<std::uint8_t>(addr == 0xC000 ? 0xC1 : 0x81);
      },
      [&](std::uint16_t addr, std::uint8_t value) {
        std::snprintf(buf, sizeof buf, "write %04X %02X", addr, value);
        events.emplace_back(buf);
      });
  for (int i = 0; i < 6; ++i) cpu.step();
  // INC writes the read's value plus one, LSR the read's value shifted,
  // and the stores go to memory as well as to the callback.
  const std::vector<std::string> want = {"read C030", "write C030 82", "read C0E9", "write C0E9 40",
                                         "write C010 12", "read C000"};
  CHECK(events == want);
  CHECK(cpu.memory[0xC030] == 0x82);
  CHECK(cpu.memory[0xC0E9] == 0x40);
  CHECK(cpu.memory[0xC010] == 0x12);
  CHECK(cpu.a == 0x66);
  CHECK((cpu.p & Cpu::kCarry) != 0);
}

TEST_CASE("cpu: run calls a trap before the fetch and returns from it as RTS would") {
  // JSR $E000; LDA #$01; BRK
  Cpu cpu = cpu_with(0x0800, {0x20, 0x00, 0xE0, 0xA9, 0x01, 0x00});
  int calls = 0;
  cpu.set_trap(0xE000, [&](Cpu& c) {
    ++calls;
    c.x = 0x42;
    return Cpu::Action::kRts;
  });
  const Cpu::RunResult r = cpu.run({});
  CHECK(r.stop == Cpu::Stop::kBrk);
  CHECK(r.instructions == 2);
  CHECK(calls == 1);
  CHECK(cpu.pc == 0x0805);
  CHECK(cpu.sp == 0xFF);
  CHECK(cpu.a == 0x01);
  CHECK(cpu.x == 0x42);
  // The JSR's 6 and the LDA's 2, and nothing for the trap.
  CHECK(cpu.cycles == 8);
}

TEST_CASE("cpu: run checks the cycles first, then the instructions, the trap, and BRK") {
  // LDA #$01; NOP; BRK
  Cpu cpu = cpu_with(0x0800, {0xA9, 0x01, 0xEA, 0x00});
  Cpu::Limits limits;
  limits.max_cycles = 2;
  Cpu::RunResult r = cpu.run(limits);
  CHECK(r.stop == Cpu::Stop::kCycles);
  CHECK(r.instructions == 1);
  CHECK(cpu.pc == 0x0802);

  limits = {};
  limits.max_instructions = 0;
  cpu.set_trap(0x0802, [](Cpu&) { return Cpu::Action::kStop; });
  r = cpu.run(limits);
  CHECK(r.stop == Cpu::Stop::kInstructions);

  limits = {};
  r = cpu.run(limits);
  CHECK(r.stop == Cpu::Stop::kTrap);
  CHECK(cpu.pc == 0x0802);

  // A trap that gives `kExecute` lets the instruction at PC run.
  cpu.set_trap(0x0802, [](Cpu&) { return Cpu::Action::kExecute; });
  r = cpu.run(limits);
  CHECK(r.stop == Cpu::Stop::kBrk);
  CHECK(r.instructions == 1);
  CHECK(cpu.pc == 0x0803);

  // Without the stop at BRK, BRK runs and jumps through $FFFE.
  cpu.clear_trap(0x0802);
  cpu.memory[0xFFFE] = 0x00;
  cpu.memory[0xFFFF] = 0x09;
  cpu.memory[0x0900] = 0xEA;
  limits.stop_at_brk = false;
  limits.max_instructions = 2;
  r = cpu.run(limits);
  CHECK(r.stop == Cpu::Stop::kInstructions);
  CHECK(cpu.pc == 0x0901);
  CHECK((cpu.p & (Cpu::kBreak | Cpu::kInterrupt)) == (Cpu::kBreak | Cpu::kInterrupt));
}

TEST_CASE("cpu: before_instruction can skip an instruction or stop the run") {
  // LDA #$01; LDA #$02; LDA #$03; BRK
  Cpu cpu = cpu_with(0x0800, {0xA9, 0x01, 0xA9, 0x02, 0xA9, 0x03, 0x00});
  std::vector<std::uint16_t> seen;
  cpu.before_instruction = [&](Cpu& c) {
    seen.push_back(c.pc);
    if (c.pc == 0x0802) {
      c.pc = 0x0804;
      c.cycles += 10;
      return Cpu::Action::kContinue;
    }
    return c.pc == 0x0806 ? Cpu::Action::kStop : Cpu::Action::kExecute;
  };
  int after = 0;
  cpu.after_instruction = [&](Cpu&) { ++after; };
  Cpu::Limits limits;
  limits.stop_at_brk = false;
  const Cpu::RunResult r = cpu.run(limits);
  CHECK(r.stop == Cpu::Stop::kCallback);
  CHECK(r.instructions == 2);
  CHECK(after == 2);
  CHECK(seen == std::vector<std::uint16_t>{0x0800, 0x0802, 0x0804, 0x0806});
  CHECK(cpu.a == 0x03);
  CHECK(cpu.cycles == 14);
}

TEST_CASE("cpu: py65's undefined opcodes skip a byte in no cycles") {
  for (const int code : {0x02, 0x03, 0x1A, 0x80, 0x9C, 0xFF}) {
    INFO("opcode " << code);
    const auto op = static_cast<std::uint8_t>(code);
    Cpu cpu = cpu_with(0x0800, {op, 0xAD});
    cpu.a = 0x12;
    cpu.p = 0x30;
    cpu.step();
    CHECK(cpu.pc == 0x0802);
    CHECK(cpu.cycles == 0);
    CHECK(cpu.a == 0x12);
    CHECK(cpu.p == 0x30);
  }
}

TEST_CASE("cpu: py65 counts 3 cycles for DEC abs") {
  Cpu cpu = cpu_with(0x0800, {0xCE, 0x00, 0x30});
  cpu.memory[0x3000] = 0x01;
  cpu.step();
  CHECK(cpu.memory[0x3000] == 0x00);
  CHECK(cpu.cycles == 3);
}

TEST_CASE("cpu-run: segments run from the given state and patches, with a digest after each instruction") {
  // LDA #$05; STA $10; JSR $E000; STA $11; BRK
  std::string image(0x10000, '\0');
  const unsigned char program[] = {0xA9, 0x05, 0x85, 0x10, 0x20, 0x00, 0xE0, 0x85, 0x11, 0x00};
  for (std::size_t i = 0; i < sizeof program; ++i) image[0x0800 + i] = static_cast<char>(program[i]);
  // The second segment starts as the harness leaves it after the hook at
  // $E000: SP back at $FF, PC after the JSR, and A and $12 set.
  const std::string args =
      R"({"stops": [57344], "stop_at_brk": true, "max_cycles": 1000, "max_instructions": null, "segments": [)"
      R"({"pc": 2048, "a": 0, "x": 0, "y": 0, "sp": 255, "p": 48, "cycles": 0, "patches": []}, )"
      R"({"pc": 2055, "a": 7, "x": 0, "y": 0, "sp": 255, "p": 48, "cycles": 11, "patches": [[18, 99]]}]})";
  const std::string request = "c6502-backend 3\nmode cpu-run\nargs " + args + "\nimage 65536\n" + image;
  const std::string response = c6502::backend::respond(request);

  // Each instruction's digest is the CRC-32 of Python's
  // struct.pack("<HBBBBBQ", pc, a, x, y, sp, p, cycles) and its writes.
  const auto record = [](std::uint16_t pc, std::uint8_t a, std::uint8_t sp, std::uint64_t cycles,
                         const std::string& writes) {
    std::string r;
    r += static_cast<char>(pc & 0xFF);
    r += static_cast<char>(pc >> 8);
    r += static_cast<char>(a);
    r += '\0';
    r += '\0';
    r += static_cast<char>(sp);
    r += static_cast<char>(0x30);
    for (int i = 0; i < 8; ++i) r += static_cast<char>((cycles >> (8 * i)) & 0xFF);
    const std::uint32_t crc = c6502::sim::cpu_run::crc32(r + writes);
    std::string out;
    for (int i = 0; i < 4; ++i) out += static_cast<char>((crc >> (8 * i)) & 0xFF);
    return out;
  };
  const std::string seg0_digests = record(0x0802, 5, 0xFF, 2, "") +
                                   record(0x0804, 5, 0xFF, 5, std::string("\x10\x00\x05", 3)) +
                                   record(0xE000, 5, 0xFD, 11, std::string("\xFF\x01\x08\xFE\x01\x06", 6));
  const std::string seg0_writes = std::string("\x10\x00\x05\xFE\x01\x06\xFF\x01\x08", 9);
  const std::string seg1_digests = record(0x0809, 7, 0xFF, 14, std::string("\x11\x00\x07", 3));
  const std::string want = "segment 0 stop 3\nstate 57344 5 0 0 253 48 11\nlast-writes 01FF:08 01FE:06\n"
                           "writes 9\n" + seg0_writes + "digests 12\n" + seg0_digests +
                           "segment 1 brk 1\nstate 2057 7 0 0 255 48 14\nlast-writes 0011:07\n"
                           "writes 3\n" + std::string("\x11\x00\x07", 3) + "digests 4\n" + seg1_digests;
  CHECK(response == want);
}

TEST_CASE("cpu-run: a word read at $FFFF ends the response with an error") {
  std::string image(0x10000, '\0');
  image[0xFFFE] = static_cast<char>(0xAD);  // LDA abs, whose operand would run past $FFFF
  const std::string args =
      R"({"stops": [], "stop_at_brk": true, "max_cycles": 1000, "max_instructions": null, "segments": [)"
      R"({"pc": 65534, "a": 0, "x": 0, "y": 0, "sp": 255, "p": 48, "cycles": 0, "patches": []}, )"
      R"({"pc": 0, "a": 0, "x": 0, "y": 0, "sp": 255, "p": 48, "cycles": 0, "patches": []}]})";
  const std::string response =
      c6502::backend::respond("c6502-backend 3\nmode cpu-run\nargs " + args + "\nimage 65536\n" + image);
  CHECK(response ==
        "segment 0 error 0\nstate 65535 0 0 0 255 48 0\nlast-writes\nwrites 0\ndigests 0\n"
        "error \"py65 reads a word at $FFFF, whose high byte is past the end of memory\"\n");
}
