// Reads and writes of a named `volatile` object, run in both pipelines,
// and the simulator's assembler on a volatile Mov whose operands match,
// from tests/test_volatile_named.py. The simulator can't change memory
// behind the program's back, so a wait loop is checked by whether the
// run reaches the limit on the cycles.
#include <cstdint>
#include <map>
#include <optional>
#include <regex>
#include <string>
#include <vector>

#include "asm_ast.h"
#include "compile_case.h"
#include "doctest.h"
#include "sim/assembler.h"
#include "sim/cpu_run.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

const std::string kKbd = R"c(extern volatile unsigned char KBD [[c6502::address(0xC000)]];
extern volatile unsigned char KBDSTRB [[c6502::address(0xC010)]];
)c";

sim::SimResult run(const std::string& src, bool optimize, const std::map<int, int>& mem = {},
                   std::optional<std::uint64_t> max_cycles = std::nullopt) {
  sim::Simulation s = build_sim(src, optimize);
  for (const auto& [addr, value] : mem) s.cpu.memory[addr] = static_cast<std::uint8_t>(value);
  return max_cycles ? s.run(*max_cycles) : s.run();
}

// Returns how many instructions of `asm_text` read `name`: LDA, CMP, BIT,
// or an ALU instruction such as `ADC g`, which reads it in place.
int reads(const std::string& asm_text, const std::string& name) {
  const std::regex re("\\s+(LDA|CMP|BIT|ADC|SBC|AND|ORA|EOR)\\s+" + name);
  int n = 0;
  for (const std::string& line : lines(asm_text)) n += std::regex_match(line, re);
  return n;
}

void check_both(const std::string& src, std::int64_t want) {
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(run(src, optimize).return_int() == want);
  }
}

}  // namespace

TEST_CASE("volatile_named sim: a wait loop reads KBD each time") {
  const std::string src = kKbd + R"c(int main(void) {
  while (!(KBD & 0x80)) ;
  return KBD;
}
)c";
  for (const bool optimize : {false, true}) {
    {
      INFO("optimize=" << optimize << " key=none");
      CHECK(run(src, optimize, {{0xC000, 0x41}}, 20'000).timed_out);
    }
    {
      INFO("optimize=" << optimize << " key=C1");
      const sim::SimResult r = run(src, optimize, {{0xC000, 0xC1}}, 20'000);
      CHECK_FALSE(r.timed_out);
      CHECK(r.return_int() == 0xC1);
    }
  }
}

TEST_CASE("volatile_named sim: increments and compound assignments") {
  check_both(R"c(volatile unsigned char g;
int main(void) {
  g = 5;
  unsigned char a = g++;
  unsigned char b = ++g;
  g += 3; g -= 1;
  return a * 1000 + b * 10 + g;
}
)c", 5 * 1000 + 7 * 10 + 9);
}

TEST_CASE("volatile_named sim: a two-byte object") {
  check_both(R"c(volatile unsigned int w;
int main(void) { w = 0x1234; w += 1; w++; return w; }
)c", 0x1236);
}

TEST_CASE("volatile_named sim: a zero-page object") {
  check_both(R"c(volatile unsigned char z [[c6502::zeropage]] = 3;
int main(void) { z++; z *= 2; return z; }
)c", 8);
}

TEST_CASE("volatile_named sim: a block-scope static") {
  check_both(R"c(int count(void) { static volatile int n; return ++n; }
int main(void) { count(); count(); return count(); }
)c", 3);
}

TEST_CASE("volatile_named sim: an automatic object") {
  check_both(R"c(int main(void) {
  volatile unsigned char x = 5;
  x++; x += 2;
  return x;
}
)c", 8);
}

TEST_CASE("volatile_named sim: an assignment's value is the value stored") {
  const char* src = R"c(volatile unsigned char g;
unsigned char set(void) { return g = 7; }
int main(void) { return set(); }
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(run(src, optimize).return_int() == 7);
    // The value is the one stored, so `g` isn't read back.
    CHECK(reads(codegen(src, optimize), "g") == 0);
  }
}

TEST_CASE("volatile_named sim: widening a qualified signed char keeps its sign") {
  // `_emit_cast` tested the source type as written, so a `const` or
  // `volatile` `signed char` didn't match `SChar` and was zero-extended,
  // and -1 read back as 255. fuzz.stmts found it once it declared
  // objects `volatile`.
  const auto source = [](const std::string& qual) {
    return qual + " signed char g = -1;\n"
                  "int f(" + qual + " signed char p) { return p; }\n"
                  "int main(void) {\n"
                  "  long r = g;\n"
                  "  return (r == -1) + 2 * (f(-1) == -1);\n"
                  "}\n";
  };
  const std::vector<std::string> quals = {"const", "volatile", "const volatile"};
  // The count and CRC-32 of the programs the Python test wrote, on
  // 27 Nov 2026.
  std::string all;
  for (const std::string& qual : quals) all += source(qual);
  CHECK(quals.size() == 3);
  CHECK(sim::cpu_run::crc32(all) == 0x3DC21E1Du);
  for (const std::string& qual : quals) {
    for (const bool optimize : {false, true}) {
      INFO("qual=" << qual << " optimize=" << optimize);
      CHECK(run(source(qual), optimize).return_int() == 3);
    }
  }
}

TEST_CASE("volatile_named sim: the assembler keeps a volatile Mov whose operands match") {
  // A Mov whose operands match is a no-op, so both the emitter and the
  // simulator's assembler drop it. A volatile one still reads and writes
  // the byte.
  const auto program = [](bool is_volatile) {
    const asm_ast::Type_operand g = asm_ast::Data{.name = "g", .offset = 0};
    const asm_ast::Type_top_level fn = asm_ast::Function{
        .name = "f",
        .is_global = true,
        .params = {},
        .instructions = {asm_ast::Mov{.src = g, .dst = g, .is_volatile = is_volatile},
                         asm_ast::Return{.save_a = false, .save_x = false}},
    };
    return asm_ast::Program{.top_level = {fn}};
  };
  const std::vector<std::pair<bool, std::vector<std::uint8_t>>> cases = {
      {true, {0xAD, 0x00, 0x30, 0x8D, 0x00, 0x30, 0x60}},
      {false, {0x60}},
  };
  for (const auto& [is_volatile, want] : cases) {
    INFO("is_volatile=" << is_volatile);
    const sim::AssembledProgram out = sim::assemble(program(is_volatile), 0x0800, {{"g", 0x3000}});
    const std::vector<std::uint8_t> got(out.image.begin() + 0x0800,
                                        out.image.begin() + 0x0800 + static_cast<std::ptrdiff_t>(want.size()));
    CHECK(got == want);
  }
}

}  // namespace c6502::suite
