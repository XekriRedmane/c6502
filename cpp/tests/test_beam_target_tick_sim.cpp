#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"
#include "sim_helpers.h"

namespace c6502::suite {
namespace {

// Inlined beam_target_tick source plus definitions for the externs
// the example references, stubs for `snd_delay_up` / `snd_delay_down`
// that record their call counts and last args, and a `main` driver
// that exercises one scenario per branch of the dispatch.
const std::string kProgram = R"c(
#include <stdint.h>

uint8_t beam_state;
uint8_t beam_y;
uint8_t beam_snd_ctr;

const uint8_t floor_ceil[5] = { 0x10, 0x20, 0x30, 0x40, 0x50 };

uint8_t snd_up_calls;
uint8_t snd_up_last_pitch;
uint8_t snd_up_last_clicks;
uint8_t snd_down_calls;
uint8_t snd_down_last_pitch;
uint8_t snd_down_last_clicks;

void snd_delay_up(
    register uint8_t pitch, register uint8_t clicks) {
    snd_up_calls = (uint8_t)(snd_up_calls + 1);
    snd_up_last_pitch = pitch;
    snd_up_last_clicks = clicks;
}

void snd_delay_down(
    register uint8_t pitch, register uint8_t clicks) {
    snd_down_calls = (uint8_t)(snd_down_calls + 1);
    snd_down_last_pitch = pitch;
    snd_down_last_clicks = clicks;
}

static const uint8_t floor_y_table[5] = {
    0x00, 0x43, 0x6B, 0x93, 0xBB,
};

static const uint8_t beam_jingle[11] = {
    0x1E, 0x09, 0x16, 0x18, 0x1B, 0x0A, 0x07, 0x05,
    0x1E, 0x1B, 0x19,
};

void snd_delay_up(
    register uint8_t pitch, register uint8_t clicks);
void snd_delay_down(
    register uint8_t pitch, register uint8_t clicks);

void beam_target_tick(uint8_t beam_tick, uint8_t beam_seed_floor)
{
    int8_t snd_ctr = (int8_t)beam_snd_ctr;
    if (snd_ctr >= 0) {
        uint8_t pitch = beam_jingle[snd_ctr];
        beam_snd_ctr = (uint8_t)(snd_ctr - 1);
        snd_delay_up(pitch, 0x0A);
    }

    uint8_t state = beam_state;

    if (state & 0x80) {
        uint8_t floor_idx = state & 0x0F;
        if (beam_y == floor_y_table[floor_idx]) {
            beam_state = 0x00;
            return;
        }
        beam_y = (uint8_t)(beam_y + 0x02);
        snd_delay_down(0x10, 0x0A);
        return;
    }

    if (state != 0) {
        uint8_t floor_idx = state;
        uint8_t target    = (uint8_t)(floor_ceil[floor_idx] + 0x01);
        if (target == beam_y) {
            beam_state = (uint8_t)(state | 0xF0);
            return;
        }
        beam_y = (uint8_t)(beam_y - 0x02);
        snd_delay_up(0x10, 0x0A);
        return;
    }

    if (beam_tick == 0) {
        beam_y     = floor_y_table[beam_seed_floor];
        beam_state = beam_seed_floor;
    }
}

uint8_t result_log[128];
uint8_t log_idx;

void seed_state(uint8_t st, uint8_t y, uint8_t sc) {
    beam_state    = st;
    beam_y        = y;
    beam_snd_ctr  = sc;
}

void record(void) {
    uint8_t base = log_idx;
    result_log[(uint8_t)(base +  0)] = beam_state;
    result_log[(uint8_t)(base +  1)] = beam_y;
    result_log[(uint8_t)(base +  2)] = beam_snd_ctr;
    result_log[(uint8_t)(base +  3)] = snd_up_calls;
    result_log[(uint8_t)(base +  4)] = snd_up_last_pitch;
    result_log[(uint8_t)(base +  5)] = snd_up_last_clicks;
    result_log[(uint8_t)(base +  6)] = snd_down_calls;
    result_log[(uint8_t)(base +  7)] = snd_down_last_pitch;
    result_log[(uint8_t)(base +  8)] = snd_down_last_clicks;
    result_log[(uint8_t)(base +  9)] = 0;
    result_log[(uint8_t)(base + 10)] = 0;
    result_log[(uint8_t)(base + 11)] = 0;
    result_log[(uint8_t)(base + 12)] = 0;
    result_log[(uint8_t)(base + 13)] = 0;
    result_log[(uint8_t)(base + 14)] = 0;
    result_log[(uint8_t)(base + 15)] = 0;
    log_idx = (uint8_t)(base + 16);
}

int main(void) {
    snd_up_calls   = 0;
    snd_down_calls = 0;
    log_idx        = 0;

    /* 1. Jingle armed (snd_ctr=$0A), idle state, tick=1 -> idle
     *    branch falls through (tick != 0, no seed). Jingle plays
     *    beam_jingle[$0A]=$19 then decrements counter to $09. */
    seed_state(0x00, 0x00, 0x0A);
    beam_target_tick(1, 0);
    record();

    /* 2. Jingle exhausted (snd_ctr=$FF -> int8 -1, skip jingle),
     *    idle state, tick=1 -> nothing changes. */
    seed_state(0x00, 0x00, 0xFF);
    beam_target_tick(1, 0);
    record();

    /* 3. Attack hits target: state=$F2, floor=2, beam_y ==
     *    floor_y_table[2]=$6B -> beam_state := 0, no snd. */
    seed_state(0xF2, 0x6B, 0xFF);
    beam_target_tick(0, 0);
    record();

    /* 4. Attack miss: state=$F3, floor=3, beam_y=$50 !=
     *    floor_y_table[3]=$93 -> beam_y := $52, snd_delay_down
     *    ($10,$0A). */
    seed_state(0xF3, 0x50, 0xFF);
    beam_target_tick(0, 0);
    record();

    /* 5. Chase at target: state=$03, floor=3,
     *    target=floor_ceil[3]+1=$41 == beam_y -> state |= $F0 = $F3. */
    seed_state(0x03, 0x41, 0xFF);
    beam_target_tick(0, 0);
    record();

    /* 6. Chase miss: state=$02, target=floor_ceil[2]+1=$31, y=$80
     *    -> beam_y := $7E, snd_delay_up($10,$0A). */
    seed_state(0x02, 0x80, 0xFF);
    beam_target_tick(0, 0);
    record();

    /* 7. Idle seeds: state=0, tick=0, seed=2 ->
     *    beam_y := floor_y_table[2]=$6B, beam_state := 2. */
    seed_state(0x00, 0x77, 0xFF);
    beam_target_tick(0, 2);
    record();

    /* 8. Idle blocked: state=0, tick=5 -> no change. */
    seed_state(0x00, 0x77, 0xFF);
    beam_target_tick(5, 2);
    record();

    return (int)log_idx;
}
)c";
// Each row is the 16-byte record `record()` lays down: beam_state,
// beam_y, beam_snd_ctr, snd_up_calls, snd_up_last_pitch,
// snd_up_last_clicks, snd_down_calls, snd_down_last_pitch,
// snd_down_last_clicks, and 7 bytes of padding. The call counts are
// cumulative.
const std::vector<Bytes> kScenarios = {
    // 1. Jingle armed: pitch = jingle[$0A] = $19, ctr 10 -> 9. The
    //    idle tick=1 path does nothing.
    {0x00, 0x00, 0x09, 0x01, 0x19, 0x0A, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0},
    // 2. Jingle skipped (ctr=$FF), and idle does nothing.
    {0x00, 0x00, 0xFF, 0x01, 0x19, 0x0A, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0},
    // 3. Attack hit: state cleared, beam_y unchanged at $6B.
    {0x00, 0x6B, 0xFF, 0x01, 0x19, 0x0A, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0},
    // 4. Attack miss: beam_y $50 -> $52, and snd_down fires.
    {0xF3, 0x52, 0xFF, 0x01, 0x19, 0x0A, 0x01, 0x10, 0x0A, 0, 0, 0, 0, 0, 0, 0},
    // 5. Chase at target: state $03 -> $F3, beam_y unchanged.
    {0xF3, 0x41, 0xFF, 0x01, 0x19, 0x0A, 0x01, 0x10, 0x0A, 0, 0, 0, 0, 0, 0, 0},
    // 6. Chase miss: beam_y $80 -> $7E, and snd_up fires with ($10,
    //    $0A), which overwrites the earlier pitch $19.
    {0x02, 0x7E, 0xFF, 0x02, 0x10, 0x0A, 0x01, 0x10, 0x0A, 0, 0, 0, 0, 0, 0, 0},
    // 7. Idle seed: beam_y := $6B, state := 2.
    {0x02, 0x6B, 0xFF, 0x02, 0x10, 0x0A, 0x01, 0x10, 0x0A, 0, 0, 0, 0, 0, 0, 0},
    // 8. Idle blocked: nothing changes.
    {0x00, 0x77, 0xFF, 0x02, 0x10, 0x0A, 0x01, 0x10, 0x0A, 0, 0, 0, 0, 0, 0, 0},
};

LoggedRun run(bool optimize) { return run_logged(kProgram, optimize, "result_log", 8 * 16); }

TEST_CASE("beam_target_tick_sim: the unoptimized program logs the expected state") {
  const LoggedRun r = run(false);
  // `main` returns log_idx, 16 bytes for each of the 8 scenarios.
  CHECK((r.result.return_int() & 0xFFFF) == 8 * 16);
  CHECK(r.log == flatten(kScenarios));
}

TEST_CASE("beam_target_tick_sim: the optimized program logs the expected state") {
  const LoggedRun r = run(true);
  CHECK((r.result.return_int() & 0xFFFF) == 8 * 16);
  CHECK(r.log == flatten(kScenarios));
}

TEST_CASE("beam_target_tick_sim: the two pipelines agree") {
  const LoggedRun unopt = run(false);
  const LoggedRun opt = run(true);
  CHECK(unopt.log == opt.log);
  CHECK(hargs_head(unopt.result) == hargs_head(opt.result));
}

}  // namespace
}  // namespace c6502::suite
