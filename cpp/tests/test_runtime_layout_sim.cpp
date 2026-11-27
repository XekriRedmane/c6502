// The simulator tests of tests/test_runtime_layout.py: programs that
// reserve bytes the classic layout of the runtime's zero page uses leave
// those bytes alone, and a wide return comes back in the moved HARGS.
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "batch1_case.h"
#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {
namespace {

// A fixed zero-page object over the bytes $1F-$49, which moves the
// runtime's block clear of them.
const std::string kMonitor = "extern volatile unsigned char monitor_zp [[c6502::address(0x1F)]][0x2B];\n";

const std::string kSrc =
    "unsigned char buf[8];\n"
    "unsigned long lm(unsigned long a, unsigned long b) { return a * b / 7; }\n"
    "int sum(unsigned char *p, unsigned char n) {\n"
    "  int s = 0;\n"
    "  while (n--) s += *p++;\n"
    "  return s;\n"
    "}\n"
    "int rec(int n) { return n ? n + rec(n - 1) : 0; }\n"
    "int main(void) {\n"
    "  for (unsigned char i = 0; i < 8; i++) buf[i] = i * 3;\n"
    "  float f = (float)sum(buf, 8);\n"
    "  int g = (int)f;\n"
    "  return (int)(lm(100000, 3) & 0x7FFF) + g + rec(10);\n"
    "}\n";
constexpr std::int64_t kWant = ((100000 * 3 / 7) & 0x7FFF) + 84 + 55;

struct Run {
  std::int64_t value;
  std::vector<int> changed;
};

Run run(const std::string& src, bool optimize, const std::vector<int>& bytes, int seed) {
  sim::Simulation s = build_sim(src, optimize);
  for (int a : bytes) s.cpu.memory[a] = static_cast<std::uint8_t>((seed ^ a) & 0xFF);
  const sim::SimResult result = s.run();
  Run out{result.return_int(), {}};
  for (int a : bytes) {
    if (result.memory[a] != ((seed ^ a) & 0xFF)) out.changed.push_back(a);
  }
  return out;
}

std::vector<int> range(int lo, int hi) {
  std::vector<int> out;
  for (int a = lo; a < hi; ++a) out.push_back(a);
  return out;
}

TEST_CASE("runtime_layout: the bytes of the fixed object are untouched") {
  // Pointer accesses (DPTR), recursion (SSP, FP), a 32-bit multiply and
  // divide (6502 helpers with HARGS scratch), and an int / float round
  // trip (the hooks) all run with the block moved clear of $1F-$49.
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const Run r = run(kMonitor + kSrc, optimize, range(0x1F, 0x4A), 0xA5);
    CHECK(r.value == kWant);
    CHECK(r.changed.empty());
  }
}

TEST_CASE("runtime_layout: the classic layout writes those bytes") {
  // Without the reservation, the unoptimized build writes into $1F-$49
  // (the top of HARGS and DPTR are there), which is what the test above
  // checks doesn't happen. The optimized build keeps its pointers in
  // zero-page locals instead of DPTR, and its helpers stay below $1F, so
  // it doesn't.
  const Run r = run(kSrc, false, range(0x1F, 0x4A), 0xA5);
  CHECK(r.value == kWant);
  CHECK_FALSE(r.changed.empty());
}

TEST_CASE("runtime_layout: a wide return comes back in the moved block") {
  // A zero-page object at $10 moves the block, so `main`'s `long` and
  // `long long` come back in the moved HARGS, which `return_long` and
  // `return_longlong` read.
  const std::string head =
      "[[c6502::zeropage(0x10)]] unsigned char z;\n"
      "long f(long a) { return a * 3 + z; }\n"
      "long long g(long long a) { return a * 5 + z; }\n";
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    sim::Simulation s = build_sim(head + "long main(void) { z = 1; return f(100000L); }\n", optimize);
    CHECK(s.symbol("HARGS") != 0x04);
    CHECK(s.run().return_long() == 300001);
    sim::Simulation t = build_sim(head + "long long main(void) { z = 2; return g(1000000000000LL); }\n", optimize);
    CHECK(t.run().return_longlong() == 5000000000002ULL);
  }
}

// `<apple2/monitor_io.h>` reserves the zero page of the Monitor routines
// a program calls, and `<apple2/interactive_monitor.h>` adds the bytes
// only the Monitor's interactive part uses.
TEST_CASE("runtime_layout: the Monitor headers' reserved bytes are untouched") {
  // The bytes of the routines a program calls: text, keyboard, line
  // input, lo-res graphics, the IRQ entry, and INIT.
  std::vector<int> routine_bytes = range(0x1F, 0x2F);
  for (int a : {0x30, 0x32, 0x33}) routine_bytes.push_back(a);
  for (int a : range(0x35, 0x3A)) routine_bytes.push_back(a);
  for (int a : {0x45, 0x48}) routine_bytes.push_back(a);
  // Every byte the Monitor uses.
  std::vector<int> all_bytes = {0x00, 0x01};
  for (int a : range(0x1F, 0x4A)) all_bytes.push_back(a);
  const std::vector<std::tuple<std::string, std::string, std::vector<int>>> headers = {
      {"monitor_io.h", "#include <apple2/monitor_io.h>\n", routine_bytes},
      {"interactive_monitor.h", "#include <apple2/interactive_monitor.h>\n", all_bytes},
  };
  for (const auto& [header, include, reserved] : headers) {
    for (bool optimize : {false, true}) {
      INFO(header << " optimize=" << optimize);
      const Run r = run(include + kSrc, optimize, reserved, 0x5A);
      CHECK(r.value == kWant);
      CHECK(r.changed.empty());
    }
  }
}

}  // namespace
}  // namespace c6502::suite
