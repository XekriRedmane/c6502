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

// Inlined source: the function under test, externs / stubs, plus a
// table-driven `main()` that exercises a battery of state-x-y-x-
// snd_ctr-x-tick-x-seed combinations and records observable state
// per scenario. The scenarios array is laid out as a flat byte
// table of (state, y, snd_ctr, tick, seed) 5-tuples terminated by
// an end-of-table sentinel `state == 0xAA`.
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

/* Each scenario records 8 observable bytes:
 *   beam_state, beam_y, beam_snd_ctr,
 *   snd_up_calls, snd_up_last_pitch, snd_up_last_clicks,
 *   snd_down_calls, snd_down_last_pitch.
 *
 * 24 scenarios * 8 bytes = 192 buffer bytes. Array sizes must be
 * literal integer constants (c6502 parser limitation), so no
 * SCENARIOS / RECORD_SIZE macros.
 */

uint8_t result_log[192];

/* Flat inputs table — 24 rows of (state, y, snd_ctr, tick,
 * seed_floor) packed sequentially: index by `5*i + field`. */
const uint8_t inputs[120] = {
    /* --- beam_state == 0x80 (attack, floor 0, target = $00) --- */
    0x80, 0x00, 0xFF, 0, 0,   /* y == target → hit, state := 0 */
    0x80, 0x01, 0xFF, 0, 0,   /* y just above target → miss */
    0x80, 0x7F, 0xFF, 0, 0,   /* mid-range miss */
    0x80, 0xFF, 0xFF, 0, 0,   /* y wraps on +2 → miss */
    0x80, 0x00, 0x05, 0, 0,   /* hit, plus jingle armed */
    0x80, 0x10, 0xFF, 1, 3,   /* miss; tick/seed ignored */
    /* --- beam_state == 0x01 (chase, floor 1, target =
     *     floor_ceil[1]+1 = $21) --- */
    0x01, 0x21, 0xFF, 0, 0,   /* y == target → flip to attack */
    0x01, 0x22, 0xFF, 0, 0,   /* miss (just above target) */
    0x01, 0x00, 0xFF, 0, 0,   /* miss (y wraps on -2) */
    0x01, 0x80, 0xFF, 0, 0,   /* mid-range miss */
    0x01, 0x21, 0x05, 0, 0,   /* hit, plus jingle armed */
    0x01, 0x22, 0xFF, 1, 2,   /* miss; tick/seed ignored */
    /* --- beam_state == 0x00 (idle) --- */
    0x00, 0x77, 0xFF, 0, 0,   /* tick=0, seed=0 → state:=0 */
    0x00, 0x77, 0xFF, 0, 1,   /* tick=0, seed=1 → state:=1 */
    0x00, 0x77, 0xFF, 0, 4,   /* tick=0, seed=4 → state:=4 */
    0x00, 0x77, 0xFF, 1, 2,   /* tick=1 → no seed */
    0x00, 0x77, 0xFF, 0xFF, 2, /* tick=$FF → no seed */
    0x00, 0x77, 0x00, 0, 0,   /* jingle@0; tick=0; seed=0 */
    0x00, 0x77, 0x0A, 0, 3,   /* jingle@10; tick=0; seed=3 */
    0x00, 0x77, 0xFF, 0, 0,   /* repeat first idle, post-mutations */
    /* --- mixed: jingle + attack/chase + tick gating --- */
    0x80, 0x00, 0x0A, 1, 4,   /* attack-hit + jingle */
    0x01, 0x12, 0x00, 1, 2,   /* chase-miss + jingle (last note) */
    0xF4, 0xBB, 0xFF, 0, 0,   /* attack floor 4 hit (target = $BB) */
    0x04, 0x51, 0xFF, 0, 0,   /* chase floor 4 hit (target = $51) */
};

uint8_t log_idx;

void seed_state(uint8_t st, uint8_t y, uint8_t sc) {
    beam_state    = st;
    beam_y        = y;
    beam_snd_ctr  = sc;
}

void record(void) {
    uint8_t base = log_idx;
    result_log[(uint8_t)(base + 0)] = beam_state;
    result_log[(uint8_t)(base + 1)] = beam_y;
    result_log[(uint8_t)(base + 2)] = beam_snd_ctr;
    result_log[(uint8_t)(base + 3)] = snd_up_calls;
    result_log[(uint8_t)(base + 4)] = snd_up_last_pitch;
    result_log[(uint8_t)(base + 5)] = snd_up_last_clicks;
    result_log[(uint8_t)(base + 6)] = snd_down_calls;
    result_log[(uint8_t)(base + 7)] = snd_down_last_pitch;
    log_idx = (uint8_t)(base + 8);
}

int main(void) {
    snd_up_calls = 0;
    snd_down_calls = 0;
    snd_up_last_pitch = 0;
    snd_up_last_clicks = 0;
    snd_down_last_pitch = 0;
    snd_down_last_clicks = 0;
    log_idx = 0;

    for (uint8_t i = 0; i < 24; i = (uint8_t)(i + 1)) {
        uint8_t base = (uint8_t)(i * 5);
        seed_state(inputs[base], inputs[(uint8_t)(base + 1)],
                   inputs[(uint8_t)(base + 2)]);
        beam_target_tick(inputs[(uint8_t)(base + 3)],
                         inputs[(uint8_t)(base + 4)]);
        record();
    }
    return (int)log_idx;
}
)c";
const auto kScenarios = 24;
const auto kRecordSize = 8;
auto run(const auto& optimize) {
  auto s = build_sim(kProgram, optimize);
  auto result = s.run(5'000'000);
  CHECK_FALSE_MESSAGE(result.timed_out, cat("beam_target_tick diff sim timed out (optimize=", optimize, ")"));
  CHECK_MESSAGE((result.return_int() & 0xFFFF) == kScenarios * kRecordSize,
                cat("log_idx mismatch (optimize=", optimize, ")"));
  auto log_addr = s.symbol("result_log");
  return slice(result.memory, log_addr, log_addr + kScenarios * kRecordSize);
}

TEST_CASE("beam_target_tick_diff: opt matches unopt full buffer") {
  auto unopt = run(false);
  auto opt = run(true);
  CHECK_MESSAGE(unopt == opt, "optimizer changed observable state");
}

TEST_CASE("beam_target_tick_diff: opt matches unopt attack state 80") {
  auto unopt = run(false);
  auto opt = run(true);
  auto size = 6 * kRecordSize;
  CHECK_MESSAGE(slice(unopt, 0, size) == slice(opt, 0, size),
                "optimizer perturbed the beam_state == 0x80 (attack) cases");
}

TEST_CASE("beam_target_tick_diff: opt matches unopt chase state 01") {
  auto unopt = run(false);
  auto opt = run(true);
  auto start = 6 * kRecordSize;
  auto end = 12 * kRecordSize;
  CHECK_MESSAGE(slice(unopt, start, end) == slice(opt, start, end),
                "optimizer perturbed the beam_state == 0x01 (chase) cases");
}

TEST_CASE("beam_target_tick_diff: opt matches unopt idle state 00") {
  auto unopt = run(false);
  auto opt = run(true);
  auto start = 12 * kRecordSize;
  auto end = 20 * kRecordSize;
  CHECK_MESSAGE(slice(unopt, start, end) == slice(opt, start, end),
                "optimizer perturbed the beam_state == 0x00 (idle) cases");
}

}  // namespace
}  // namespace c6502::suite
