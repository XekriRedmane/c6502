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

}  // namespace c6502::suite
