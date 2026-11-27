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

// Inlined do_ascend source plus stubs for `sfx_tone` /
// `snd_delay_up`, definitions for every extern the example
// references, and a `main` that exercises the four branches.
const std::string kProgram = R"c(
#include <stdint.h>

uint8_t player_col;
uint8_t move_dir;
uint8_t beam_seed_floor;
uint8_t floor_mirror;
uint8_t dsc_floor;
uint8_t ent_rescued;
uint8_t beam_tick;

uint8_t entity_hit_state[12];

static const uint8_t floor_ceil[4]   = { 0x10, 0x20, 0x30, 0x40 };
static const uint8_t floor_thresh[4] = { 0x14, 0x24, 0x34, 0x44 };

uint8_t sfx_calls;
uint8_t sfx_last_pitch;
uint8_t sfx_last_dur;
uint8_t snd_up_calls;
uint8_t snd_up_last_pitch;
uint8_t snd_up_last_clicks;

void sfx_tone(uint8_t pitch, uint8_t duration) {
    sfx_calls = (uint8_t)(sfx_calls + 1);
    sfx_last_pitch = pitch;
    sfx_last_dur = duration;
}

void snd_delay_up(uint8_t pitch, uint8_t clicks) {
    snd_up_calls = (uint8_t)(snd_up_calls + 1);
    snd_up_last_pitch = pitch;
    snd_up_last_clicks = clicks;
}

void do_ascend(uint8_t asc_floor, uint8_t hit_max)
{
    uint8_t col = player_col;

    if (col == floor_ceil[asc_floor]) {
        sfx_tone(0x05, 0x04);
        beam_tick--;
        return;
    }

    col = (uint8_t)(col - 0x04);
    player_col = col;

    if (col == floor_ceil[asc_floor]) {
        move_dir = 0x00;
        sfx_tone(0x05, 0x04);
        beam_tick--;
        return;
    }

    if (col == floor_thresh[asc_floor]) {
        beam_seed_floor = asc_floor;
        floor_mirror    = asc_floor;
        dsc_floor       = asc_floor;
        ent_rescued     = 0xFF;
        for (int8_t i = (int8_t)hit_max; i >= 0; i--) {
            entity_hit_state[i] = 0xFF;
        }
    }

    snd_delay_up(0x04, 0x08);
}

uint8_t result_log[64];
uint8_t log_idx;

void seed_state(uint8_t initial_player_col) {
    player_col      = initial_player_col;
    move_dir        = 0x55;
    beam_seed_floor = 0x99;
    floor_mirror    = 0xAA;
    dsc_floor       = 0xBB;
    ent_rescued     = 0xCC;
    beam_tick       = 0x05;
    for (uint8_t i = 0; i < 12; i = (uint8_t)(i + 1)) {
        entity_hit_state[i] = 0x00;
    }
}

void record(void) {
    uint8_t base = log_idx;
    result_log[(uint8_t)(base +  0)] = player_col;
    result_log[(uint8_t)(base +  1)] = move_dir;
    result_log[(uint8_t)(base +  2)] = beam_seed_floor;
    result_log[(uint8_t)(base +  3)] = floor_mirror;
    result_log[(uint8_t)(base +  4)] = dsc_floor;
    result_log[(uint8_t)(base +  5)] = ent_rescued;
    result_log[(uint8_t)(base +  6)] = beam_tick;
    result_log[(uint8_t)(base +  7)] = sfx_calls;
    result_log[(uint8_t)(base +  8)] = sfx_last_pitch;
    result_log[(uint8_t)(base +  9)] = sfx_last_dur;
    result_log[(uint8_t)(base + 10)] = snd_up_calls;
    result_log[(uint8_t)(base + 11)] = snd_up_last_pitch;
    result_log[(uint8_t)(base + 12)] = snd_up_last_clicks;
    result_log[(uint8_t)(base + 13)] = entity_hit_state[0];
    result_log[(uint8_t)(base + 14)] = entity_hit_state[3];
    result_log[(uint8_t)(base + 15)] = entity_hit_state[4];
    log_idx = (uint8_t)(base + 16);
}

int main(void) {
    sfx_calls    = 0;
    snd_up_calls = 0;
    log_idx      = 0;

    /* 1. Already at the ceiling on entry: col == floor_ceil[0]. */
    seed_state(0x10);
    do_ascend(0, 2);
    record();

    /* 2. -4 step lands on the ceiling: col-4 == floor_ceil[2]. */
    seed_state(0x34);
    do_ascend(2, 2);
    record();

    /* 3. -4 step lands on the floor threshold: col-4 == thresh[1].
     *    Wipes entity_hit_state[0..3]. */
    seed_state(0x28);
    do_ascend(1, 3);
    record();

    /* 4. Mid-air: col-4 is neither ceiling nor threshold. */
    seed_state(0x50);
    do_ascend(3, 2);
    record();

    return (int)log_idx;
}
)c";
// Each row is the 16-byte record `record()` lays down: player_col,
// move_dir, beam_seed_floor, floor_mirror, dsc_floor, ent_rescued,
// beam_tick, sfx_calls, sfx_last_pitch, sfx_last_dur, snd_up_calls,
// snd_up_last_pitch, snd_up_last_clicks, and entity_hit_state[0], [3],
// and [4]. `sfx_calls` and `snd_up_calls` are cumulative across
// scenarios.
const std::vector<Bytes> kScenarios = {
    // Scenario 1: already at the ceiling. Branch 1 fires:
    // sfx_tone($05,$04); beam_tick--; return. Only beam_tick (0x05 - 1)
    // and the sfx bytes change.
    {0x10, 0x55, 0x99, 0xAA, 0xBB, 0xCC, 0x04, 0x01, 0x05, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    // Scenario 2: -4 step lands on the ceiling.
    //   col=$34; ceil[2]=$30; first compare fails.
    //   col := $34-4 = $30; player_col := $30.
    //   col == ceil[2]: move_dir := 0; sfx_tone; beam_tick--.
    {0x30, 0x00, 0x99, 0xAA, 0xBB, 0xCC, 0x04, 0x02, 0x05, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    // Scenario 3: -4 step lands on the floor threshold.
    //   col=$28; ceil[1]=$20; first compare fails.
    //   col := $24; player_col := $24.
    //   col != ceil[1]($20); col == thresh[1]($24): wipe and fall
    //   through to snd_delay_up.
    // move_dir is untouched, beam_seed_floor := asc_floor (1), beam_tick
    // isn't decremented on this branch, and there's no new sfx call, so
    // the last sfx arguments carry over from scenario 2. The wipe runs
    // i=3..0, so entity_hit_state[0..3] := $FF, and [4] is untouched
    // (= 0).
    {0x24, 0x55, 0x01, 0x01, 0x01, 0xFF, 0x05, 0x02, 0x05, 0x04, 0x01, 0x04, 0x08, 0xFF, 0xFF, 0x00},
    // Scenario 4: mid-air step.
    //   col=$50; ceil[3]=$40; first compare fails.
    //   col := $4C; player_col := $4C.
    //   col != ceil[3]($40) and != thresh[3]($44): just snd_delay_up.
    // beam_tick isn't decremented.
    {0x4C, 0x55, 0x99, 0xAA, 0xBB, 0xCC, 0x05, 0x02, 0x05, 0x04, 0x02, 0x04, 0x08, 0x00, 0x00, 0x00},
};

LoggedRun run(Pipeline pipeline) { return run_logged(kProgram, pipeline, "result_log", 4 * 16); }

TEST_CASE("do_ascend_sim: the unoptimized program logs the expected state") {
  const LoggedRun r = run(Pipeline::kUnoptimized);
  // `main` returns log_idx, 16 bytes for each of the 4 scenarios.
  CHECK((r.result.return_int() & 0xFFFF) == 4 * 16);
  CHECK(r.log == flatten(kScenarios));
}

TEST_CASE("do_ascend_sim: the optimized program logs the expected state") {
  const LoggedRun r = run(Pipeline::kOptimized);
  CHECK((r.result.return_int() & 0xFFFF) == 4 * 16);
  CHECK(r.log == flatten(kScenarios));
}

TEST_CASE("do_ascend_sim: the two pipelines agree") {
  const LoggedRun unopt = run(Pipeline::kUnoptimized);
  const LoggedRun opt = run(Pipeline::kOptimized);
  CHECK(unopt.log == opt.log);
  CHECK(hargs_head(unopt.result) == hargs_head(opt.result));
}

}  // namespace
}  // namespace c6502::suite
