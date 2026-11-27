// The simulator's runtime and harness (src/sim/runtime.h, harness.h).
#include <map>
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("sim harness: the helpers are where links have given them") {
  // The addresses sim/linked.py's `defsym_args` gave on 27 Nov 2026,
  // which the linked programs of the tests were made with.
  const std::map<std::string, std::int64_t> want = {
      {"asl16", 0xE338},     {"asl32", 0xE377},     {"asl64", 0xE440},     {"asl8", 0xE30F},
      {"asr16", 0xE34C},     {"asr32", 0xE397},     {"asr64", 0xE450},     {"asr8", 0xE31C},
      {"bin16_to_bcd", 0xE3DA}, {"d2f", 0xE600},    {"d2i", 0xE590},       {"d2l", 0xE5B0},
      {"d2ll", 0xE5D0},      {"d2u", 0xE5A0},       {"d2ul", 0xE5C0},      {"d2ull", 0xE5E0},
      {"dadd", 0xE650},      {"ddiv", 0xE680},      {"dle", 0xE6C0},       {"dlt", 0xE6B0},
      {"dmul", 0xE670},      {"dsub", 0xE660},      {"f2d", 0xE5F0},       {"f2i", 0xE530},
      {"f2l", 0xE550},       {"f2ll", 0xE570},      {"f2u", 0xE540},       {"f2ul", 0xE560},
      {"f2ull", 0xE580},     {"fadd", 0xE610},      {"fdiv", 0xE640},      {"fle", 0xE6A0},
      {"flt", 0xE690},       {"fmul", 0xE630},      {"fsub", 0xE620},      {"i2d", 0xE4D0},
      {"i2f", 0xE470},       {"icall", 0xE408},     {"l2d", 0xE4F0},       {"l2f", 0xE490},
      {"ll2d", 0xE510},      {"ll2f", 0xE4B0},      {"lsr16", 0xE363},     {"lsr32", 0xE3BA},
      {"lsr64", 0xE460},     {"lsr8", 0xE32B},      {"mul16", 0xE01F},     {"mul32", 0xE052},
      {"mul64", 0xE410},     {"mul8", 0xE000},      {"sdivmod16", 0xE142}, {"sdivmod32", 0xE227},
      {"sdivmod64", 0xE430}, {"sdivmod8", 0xE0CB},  {"u2d", 0xE4E0},       {"u2f", 0xE480},
      {"udivmod16", 0xE111}, {"udivmod32", 0xE1CA}, {"udivmod64", 0xE420}, {"udivmod8", 0xE0AD},
      {"ul2d", 0xE500},      {"ul2f", 0xE4A0},      {"ull2d", 0xE520},     {"ull2f", 0xE4C0},
  };
  CHECK(sim::runtime_defsyms() == want);
  passes::RuntimeLayout moved;
  moved.ssp = 0x40;
  moved.fp = 0x42;
  moved.hargs = 0x44;
  moved.dptr = 0x64;
  CHECK(sim::build_runtime(moved).symbols == want);
  const std::vector<std::string> args = sim::defsym_args();
  REQUIRE(args.size() == want.size());
  CHECK(args.front() == "--defsym=asl16=0xE338");
  // `icall` is `JMP (DPTR)` after the helpers.
  const sim::Runtime rt = sim::build_runtime(moved);
  const std::size_t at = static_cast<std::size_t>(rt.symbols.at("icall") - rt.helper_origin);
  REQUIRE(rt.helper_image.size() == at + 3);
  CHECK(rt.helper_image[at] == 0x6C);
  CHECK(rt.helper_image[at + 1] == 0x64);
  CHECK(rt.helper_image[at + 2] == 0x00);
  CHECK(rt.hooks.size() == sim::hooked_helpers().size());
  CHECK(rt.hooks.count(0xE410) == 1);
}

TEST_CASE("sim harness: the boot stub sets SSP, calls __zp_init and main, and ends in BRK") {
  CHECK(sim::boot_stub(0x0812, std::nullopt, 0x00) ==
        std::vector<std::uint8_t>{0xA9, 0xFF, 0x85, 0x00, 0xA9, 0x7F, 0x85, 0x01, 0x20, 0x12, 0x08, 0x00});
  CHECK(sim::boot_stub(0x0812, 0x0900, 0x40) == std::vector<std::uint8_t>{0xA9, 0xFF, 0x85, 0x40, 0xA9, 0x7F, 0x85,
                                                                          0x41, 0x20, 0x00, 0x09, 0x20, 0x12, 0x08,
                                                                          0x00});
  CHECK_THROWS_AS(sim::boot_stub(0x10000, std::nullopt, 0), std::invalid_argument);
}

TEST_CASE("sim harness: a program runs from the boot stub to its BRK") {
  sim::Simulation s = build_sim("int g = 5;\nint main(void) { return g * 300 - 7; }\n");
  CHECK(s.cpu.pc == sim::kBootAddr);
  CHECK(s.cpu.memory[0xFFFC] == 0x00);
  CHECK(s.cpu.memory[0xFFFD] == 0x06);
  CHECK(s.symbols.count("main") == 1);
  CHECK(s.function_names.count("main") == 1);
  const sim::SimResult r = s.run();
  CHECK_FALSE(r.timed_out);
  CHECK(r.return_int_signed() == 1493);
  // The boot stub's BRK is where the run stopped.
  CHECK(r.memory[r.pc] == 0x00);
  CHECK(r.pc == sim::kBootAddr + 11);
  CHECK(r.word(s.symbol("g")) == 5);
  CHECK_THROWS_AS(s.symbol("nothing"), std::out_of_range);
}

TEST_CASE("sim harness: a run stops at its limit on the cycles") {
  sim::Simulation s = build_sim("volatile int v;\nint main(void) { for (;;) v++; }\n");
  const sim::SimResult r = s.run(10'000);
  CHECK(r.timed_out);
  CHECK(r.cycles >= 10'000);
  CHECK(r.cycles < 10'010);
}

TEST_CASE("sim harness: a program with no main isn't loaded") {
  CHECK_THROWS_AS(build_sim("int f(void) { return 1; }\n"), std::invalid_argument);
}

TEST_CASE("sim harness: a hook's failure ends the run with Python's exception") {
  sim::Simulation s = build_sim(
      "long long d = 0;\nint main(void) { long long n = 5; return (int)(n / d); }\n");
  try {
    s.run();
    FAIL("the run didn't fail");
  } catch (const sim::HookError& e) {
    CHECK(e.python_type() == "RuntimeError");
    CHECK(std::string(e.what()) == "sdivmod64: division by zero");
  }
}

namespace {

// Runs the hook `name` on HARGS at $04 holding `in`, and returns the
// `out_size` bytes at HARGS+`out`, low byte first.
std::uint64_t hook_result(const char* name, const std::vector<std::uint64_t>& in, int in_size, int out,
                          int out_size) {
  std::vector<std::uint8_t> mem(0x10000);
  for (std::size_t k = 0; k < in.size(); ++k) {
    for (int b = 0; b < in_size; ++b) mem[4 + k * in_size + b] = static_cast<std::uint8_t>(in[k] >> (8 * b));
  }
  sim::make_hook(name, 0x04)(mem);
  std::uint64_t v = 0;
  for (int b = 0; b < out_size; ++b) v |= std::uint64_t{mem[4 + out + b]} << (8 * b);
  return v;
}

constexpr std::uint64_t kSingleNan = 0x7FC00000;
constexpr std::uint64_t kSingleInf = 0x7F800000;
constexpr std::uint64_t kDoubleInf = 0x7FF0000000000000;

}  // namespace

// sim/runtime.py's hooks computed on the host's floating point and
// differed from fp_arith in four ways, and the C++ hooks follow
// fp_arith (decision 1 of step 9 of the port). No run of the Python
// suite reached a difference.
TEST_CASE("sim harness: a float result too large for a single is an infinity") {
  // 3e38 * 2 and the double 1e39 converted to a float, where Python's
  // `struct.pack` raised OverflowError.
  CHECK(hook_result("fmul", {0x7F61B1E6, 0x40000000}, 4, 8, 4) == kSingleInf);
  CHECK(hook_result("d2f", {0x48078287F49C4A1DULL}, 8, 8, 4) == kSingleInf);
}

TEST_CASE("sim harness: division by zero follows IEEE 754") {
  // 1.0 / -0.0, which the Python hook made plus infinity.
  CHECK(hook_result("fdiv", {0x3F800000, 0x80000000}, 4, 8, 4) == 0xFF800000);
  CHECK(hook_result("ddiv", {0x3FF0000000000000ULL, 0x8000000000000000ULL}, 8, 16, 8) == 0xFFF0000000000000ULL);
  // A NaN divided by 0 is the NaN, where the Python hook gave plus
  // infinity, and 0 / 0 is a NaN.
  CHECK(hook_result("fdiv", {0x7FC00001, 0x00000000}, 4, 8, 4) == 0x7FC00001);
  CHECK((hook_result("fdiv", {0x00000000, 0x00000000}, 4, 8, 4) & 0x7F800000) == 0x7F800000);
  CHECK(hook_result("ddiv", {0x3FF0000000000000ULL, 0}, 8, 16, 8) == kDoubleInf);
}

TEST_CASE("sim harness: a NaN operand gives the first NaN operand, made quiet") {
  // A signaling NaN first and a quiet one second: fp_arith gives the
  // first, quiet, where the Python hook, on the host's unit, gave the
  // second, 0xFFC00002.
  CHECK(hook_result("fadd", {0x7F800001, 0xFFC00002}, 4, 8, 4) == 0x7FC00001);
  CHECK(hook_result("dmul", {0x7FF0000000000001ULL, 0x3FF0000000000000ULL}, 8, 16, 8) == 0x7FF8000000000001ULL);
  CHECK(hook_result("flt", {kSingleNan, 0x3F800000}, 4, 8, 1) == 0);
  CHECK(hook_result("dle", {0x3FF0000000000000ULL, 0x7FF8000000000000ULL}, 8, 16, 1) == 0);
  CHECK(hook_result("fle", {0x80000000, 0x00000000}, 4, 8, 1) == 1);
}

TEST_CASE("sim harness: converting a NaN or an infinity to an integer fails as the TAC interpreter does") {
  // The Python hooks wrote 0.
  const auto fails = [](const char* name, std::uint64_t bits, int size) {
    try {
      hook_result(name, {bits}, size, size, 2);
    } catch (const sim::HookError& e) {
      return e.python_type() + ": " + e.what();
    }
    return std::string("no error");
  };
  CHECK(fails("f2i", kSingleNan, 4) == "ValueError: cannot convert float NaN to integer");
  CHECK(fails("d2ul", kDoubleInf, 8) == "OverflowError: cannot convert float infinity to integer");
  // A finite value out of range wraps modulo 2^N, as before: 70000.0.
  CHECK(hook_result("f2u", {0x4788B800}, 4, 4, 2) == 70000 - 65536);
}

TEST_CASE("sim harness: a program's float multiply that overflows runs to its end") {
  sim::Simulation s = build_sim(
      "volatile float a = 3e38f, b = 2.0f;\nfloat r;\n"
      "int main(void) { r = a * b; return r > 1e38f; }\n");
  const sim::SimResult res = s.run();
  CHECK(res.return_int() == 1);
  CHECK(res.word(s.symbol("r"), 4) == kSingleInf);
}

}  // namespace c6502::suite
