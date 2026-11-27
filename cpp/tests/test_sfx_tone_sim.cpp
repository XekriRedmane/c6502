// `sfx_tone` from examples/sfx_tone.c, run in both pipelines from a
// `main` that records a sentinel after each call, from
// tests/test_sfx_tone_sim.py.
//
// The function's only effects are volatile reads, of the speaker
// pointer once per outer iteration and of the busy-wait counter, and a
// simulator without the speaker sees neither as memory. The sentinels
// show that each of the five calls returned to `main` in order, and the
// count of cycles shows that the optimizer kept the inner loop.
#include <cstdint>
#include <vector>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

const char* const kProgram = R"c(
#include <stdint.h>

/* Same shape as examples/sfx_tone.c: the speaker pointer is
 * declared extern (the runtime provides storage in the real
 * setup). The test provides storage just below. */
extern const volatile uint8_t *sfx_click_ptr;

[[c6502::zp_abi]]
void sfx_tone(uint8_t pitch, uint8_t duration);

/* Backing byte the speaker pointer points at. The volatile reads
 * go through `sfx_click_ptr` to this cell; the cell's value never
 * changes during the test (no hardware-side write), so the reads
 * have no observable memory effect — they're observable only as
 * the cycles they consume. */
uint8_t click_target;
const volatile uint8_t *sfx_click_ptr = &click_target;

/* Sentinel log: main writes one byte after each scenario's
 * sfx_tone returns. If sfx_tone ever fails to return (infinite
 * loop) or corrupts the caller's state, the sentinels won't all
 * land in result_log in the expected order. */
uint8_t result_log[16];
uint8_t log_idx;

void record_sentinel(uint8_t v) {
    result_log[log_idx] = v;
    log_idx = (uint8_t)(log_idx + 1);
}

int main(void) {
    log_idx = 0;
    click_target = 0x42;

    /* Scenario 1: pitch=1, duration=1.
     *   `volatile uint8_t y = 1; while (--y != 0) {}` — --y goes
     *   from 1 to 0, loop exits without entering body. Inner: 0
     *   iters. Outer: 1 iter -> 1 speaker read. */
    sfx_tone(1, 1);
    record_sentinel(0x11);

    /* Scenario 2: pitch=2, duration=1.
     *   Inner: 1 iter (--y from 2 to 1, body, then 1 to 0, exit).
     *   Outer: 1 iter. */
    sfx_tone(2, 1);
    record_sentinel(0x22);

    /* Scenario 3: pitch=1, duration=3.
     *   Inner: 0 iters per outer. Outer: 3 iters -> 3 reads. */
    sfx_tone(1, 3);
    record_sentinel(0x33);

    /* Scenario 4: pitch=5, duration=2.
     *   Inner: 4 iters per outer. Outer: 2 iters. */
    sfx_tone(5, 2);
    record_sentinel(0x44);

    /* Scenario 5: pitch=0, duration=1.
     *   --y wraps: starts at 0, first decrement yields $FF, loop
     *   runs 255 iters total. Outer: 1 iter. Tests the wrap-around
     *   case the example's docstring calls out. */
    sfx_tone(0, 1);
    record_sentinel(0x55);

    return (int)log_idx;
}

/* sfx_tone definition — verbatim from examples/sfx_tone.c. */
[[c6502::zp_abi]]
void sfx_tone(uint8_t pitch, uint8_t duration)
{
    do {
        volatile uint8_t y = pitch;
        while (--y != 0) { }       /* inner delay loop */
        (void)*sfx_click_ptr;      /* volatile speaker read */
    } while (--duration != 0);
}
)c";

const std::vector<std::uint8_t> kExpectedLog = {0x11, 0x22, 0x33, 0x44, 0x55};
// `log_idx` after the fifth `record_sentinel`.
constexpr std::int64_t kExpectedRet = 5;

struct Run {
  sim::SimResult result;
  std::vector<std::uint8_t> log;
};

Run run(bool optimize) {
  sim::Simulation s = build_sim(kProgram, optimize);
  // Scenario 5 alone takes 255 inner iterations, and the whole program
  // takes well under 50,000 cycles, so a limit of 200,000 catches an
  // unintended infinite loop.
  Run out{s.run(200'000), {}};
  INFO("optimize=" << optimize);
  CHECK_FALSE(out.result.timed_out);
  const std::int64_t at = s.symbol("result_log");
  out.log.assign(out.result.memory.begin() + at, out.result.memory.begin() + at + kExpectedLog.size());
  return out;
}

}  // namespace

TEST_CASE("sfx_tone_sim: the unoptimized program writes each sentinel") {
  const Run r = run(false);
  CHECK((r.result.return_int() & 0xFFFF) == kExpectedRet);
  CHECK(r.log == kExpectedLog);
}

TEST_CASE("sfx_tone_sim: the optimized program writes each sentinel") {
  const Run r = run(true);
  CHECK((r.result.return_int() & 0xFFFF) == kExpectedRet);
  CHECK(r.log == kExpectedLog);
}

TEST_CASE("sfx_tone_sim: both pipelines write the same sentinels and HARGS") {
  const Run unopt = run(false);
  const Run opt = run(true);
  CHECK(unopt.log == opt.log);
  const auto hargs = [](const sim::SimResult& r) {
    return std::vector<std::uint8_t>(r.memory.begin() + r.hargs, r.memory.begin() + r.hargs + 2);
  };
  CHECK(hargs(unopt.result) == hargs(opt.result));
}

TEST_CASE("sfx_tone_sim: the optimizer keeps the volatile inner loop") {
  // The optimized program is about 5 times faster than the unoptimized
  // one and takes about 8,000 cycles, while five bodies that return at
  // once and the sentinel writes would take about 500, so the bound is
  // an absolute one rather than a ratio.
  const sim::SimResult r = build_sim(kProgram, true).run(200'000);
  CHECK(r.cycles > 3'000);
}

}  // namespace c6502::suite
