// `beam_target_draw` run in both pipelines with recording stubs for
// `draw_sprite` and `score_add`, from tests/test_beam_target_draw_sim.py.
//
// The scenarios cover the idle gate, the gate on a delta of zero, a
// chase with no hit at two of the clamp's boundaries, a hit at slot 3
// (the loop's entry), a hit at slot 0 (after the whole loop), a hit at
// slot 3 that hides one at slot 0 (the early return), a column too low,
// a row out of range, and a slot with its flag clear. Each scenario
// records 16 bytes of state.
#include <cstdint>
#include <vector>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

const char* const kProgram = R"c(
#include <stdint.h>

uint8_t beam_state;
uint8_t beam_y;
uint8_t beam_snd_ctr;
uint8_t enemy_flag[4];
uint8_t enemy_col[4];
uint8_t enemy_y[4];
uint8_t score_add_lo;
uint8_t score_add_hi;

/* Stand-in for the per-level beam sprite. Contents irrelevant for
 * the test -- we only care that the same pointer is forwarded to
 * draw_sprite under both pipelines. */
const uint8_t beam_spr[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

/* Side-effect counters for the recording stubs. */
uint8_t draw_calls;
uint8_t draw_last_width;
uint8_t draw_last_height;
uint8_t draw_last_x;
uint8_t draw_last_y;
uint8_t draw_last_page;
uint8_t score_calls;

void draw_sprite(uint8_t width, uint8_t height,
                 uint8_t sprite_x, uint8_t sprite_y,
                 const uint8_t *tile_src,
                 uint8_t page_flag) {
    (void)tile_src;
    draw_calls = (uint8_t)(draw_calls + 1);
    draw_last_width = width;
    draw_last_height = height;
    draw_last_x = sprite_x;
    draw_last_y = sprite_y;
    draw_last_page = page_flag;
}

void score_add(void) {
    score_calls = (uint8_t)(score_calls + 1);
}

static const uint8_t floor_y_table[5] = {
    0x00, 0x43, 0x6B, 0x93, 0xBB,
};

static const uint8_t proj_screen_col[132] = {
    0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x02,
    0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x04, 0x04,
    0x04, 0x04, 0x05, 0x05, 0x05, 0x06, 0x06, 0x06,
    0x06, 0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x08,
    0x09, 0x09, 0x09, 0x0A, 0x0A, 0x0A, 0x0A, 0x0B,
    0x0B, 0x0B, 0x0C, 0x0C, 0x0C, 0x0C, 0x0D, 0x0D,
    0x0D, 0x0E, 0x0E, 0x0E, 0x0E, 0x0F, 0x0F, 0x0F,
    0x10, 0x10, 0x10, 0x10, 0x11, 0x11, 0x11, 0x12,
    0x12, 0x12, 0x12, 0x13, 0x13, 0x13, 0x14, 0x14,
    0x14, 0x14, 0x15, 0x15, 0x15, 0x16, 0x16, 0x16,
    0x16, 0x17, 0x17, 0x17, 0x18, 0x18, 0x18, 0x18,
    0x19, 0x19, 0x19, 0x1A, 0x1A, 0x1A, 0x1A, 0x1B,
    0x1B, 0x1B, 0x1C, 0x1C, 0x1C, 0x1C, 0x1D, 0x1D,
    0x1D, 0x1E, 0x1E, 0x1E, 0x1E, 0x1F, 0x1F, 0x1F,
    0x20, 0x20, 0x20, 0x20, 0x21, 0x21, 0x21, 0x22,
    0x22, 0x22, 0x22, 0x23, 0x23, 0x23, 0x24, 0x24,
    0x24, 0x24, 0x25, 0x25,
};


void beam_target_draw(uint8_t page_flag)
{
    uint8_t state = beam_state;
    if (state == 0) return;

    uint8_t floor_idx = state & 0x0F;
    uint8_t delta = (uint8_t)(floor_y_table[floor_idx] - beam_y);
    if (delta == 0) return;
    uint8_t height = (delta < 0x05) ? delta : 0x04;

    draw_sprite(0x02, height, 0x24, beam_y, beam_spr, page_flag);

    for (int8_t slot = 3; slot >= 0; slot--) {
        if (enemy_flag[slot] == 0) continue;

        uint8_t screen_col = proj_screen_col[enemy_col[slot]];
        if (screen_col > 0x25)  continue;
        if (screen_col <= 0x23) continue;

        uint8_t ey = enemy_y[slot];
        if (beam_y >= ey) continue;
        if ((uint8_t)(beam_y + 0x03) < ey) continue;

        enemy_flag[slot] = 0x00;
        beam_state       = 0x00;
        beam_snd_ctr     = 0x0A;
        score_add_lo     = 0x00;
        score_add_hi     = 0x01;
        score_add();
        return;
    }
}


uint8_t result_log[256];
uint8_t log_idx;

void clear_state(void) {
    beam_state = 0;
    beam_y = 0;
    beam_snd_ctr = 0;
    score_add_lo = 0;
    score_add_hi = 0;
    for (uint8_t i = 0; i < 4; i = (uint8_t)(i + 1)) {
        enemy_flag[i] = 0;
        enemy_col[i] = 0;
        enemy_y[i] = 0;
    }
}

void record(void) {
    uint8_t base = log_idx;
    result_log[(uint8_t)(base +  0)] = beam_state;
    result_log[(uint8_t)(base +  1)] = beam_y;
    result_log[(uint8_t)(base +  2)] = beam_snd_ctr;
    result_log[(uint8_t)(base +  3)] = enemy_flag[0];
    result_log[(uint8_t)(base +  4)] = enemy_flag[1];
    result_log[(uint8_t)(base +  5)] = enemy_flag[2];
    result_log[(uint8_t)(base +  6)] = enemy_flag[3];
    result_log[(uint8_t)(base +  7)] = score_add_lo;
    result_log[(uint8_t)(base +  8)] = score_add_hi;
    result_log[(uint8_t)(base +  9)] = draw_calls;
    result_log[(uint8_t)(base + 10)] = draw_last_width;
    result_log[(uint8_t)(base + 11)] = draw_last_height;
    result_log[(uint8_t)(base + 12)] = draw_last_x;
    result_log[(uint8_t)(base + 13)] = draw_last_y;
    result_log[(uint8_t)(base + 14)] = draw_last_page;
    result_log[(uint8_t)(base + 15)] = score_calls;
    log_idx = (uint8_t)(base + 16);
}

int main(void) {
    draw_calls = 0;
    score_calls = 0;
    draw_last_width = 0;
    draw_last_height = 0;
    draw_last_x = 0;
    draw_last_y = 0;
    draw_last_page = 0;
    log_idx = 0;

    /* 1. Idle: state=0 -> early return, no side effects. */
    clear_state();
    beam_target_draw(0xAB);
    record();

    /* 2. Delta=0: state=$04 -> floor_y_table[4]=$BB, beam_y=$BB,
     *    delta=0 -> early return after the state read but before
     *    the draw. beam_state must remain $04. */
    clear_state();
    beam_state = 0x04;
    beam_y = 0xBB;
    beam_target_draw(0x12);
    record();

    /* 3. Chase no-enemies, height clamped to 4. state=$03 ->
     *    floor=$93, beam_y=$80, delta=$13 (>=5 -> height=4).
     *    page_flag must reach draw_sprite verbatim. */
    clear_state();
    beam_state = 0x03;
    beam_y = 0x80;
    beam_target_draw(0x80);
    record();

    /* 4. Chase no-enemies, small delta. state=$02 -> floor=$6B,
     *    beam_y=$6A, delta=1, height=1. */
    clear_state();
    beam_state = 0x02;
    beam_y = 0x6A;
    beam_target_draw(0x00);
    record();

    /* 5. Hit slot 3. state=$03, beam_y=$80. Slot 3 active with
     *    col=128 (proj_screen_col[128]=$24) and y=$82 (in the
     *    half-open window (0x80, 0x83]). Draw fires, then the
     *    hit clears enemy_flag[3], drops beam_state to idle,
     *    arms beam_snd_ctr, loads $0100 score addend, and
     *    invokes score_add. */
    clear_state();
    beam_state = 0x03;
    beam_y = 0x80;
    enemy_flag[3] = 0x01;
    enemy_col[3] = 128;
    enemy_y[3] = 0x82;
    beam_target_draw(0x40);
    record();

    /* 6. Hit slot 0 (iteration walks 3..0). state=$03, beam_y=$80,
     *    only slot 0 has an active flag; col=130 -> screen=$25,
     *    y=$83 (the upper bound of the row window). */
    clear_state();
    beam_state = 0x03;
    beam_y = 0x80;
    enemy_flag[0] = 0xAA;  /* any non-zero qualifies */
    enemy_col[0] = 130;
    enemy_y[0] = 0x83;
    beam_target_draw(0x00);
    record();

    /* 7. Hit-3 shadows hit-0: both slot 0 AND slot 3 qualify, but
     *    the loop must process slot 3 first and return -- slot 0's
     *    flag must remain untouched at $55. */
    clear_state();
    beam_state = 0x03;
    beam_y = 0x80;
    enemy_flag[0] = 0x55;
    enemy_col[0] = 128;
    enemy_y[0] = 0x82;
    enemy_flag[3] = 0x55;
    enemy_col[3] = 129;
    enemy_y[3] = 0x81;
    beam_target_draw(0x00);
    record();

    /* 8. Column too low: slot 3 active, col=125 -> screen=$23,
     *    `screen <= $23` continues. No hit; draw still fires. */
    clear_state();
    beam_state = 0x03;
    beam_y = 0x80;
    enemy_flag[3] = 0x01;
    enemy_col[3] = 125;
    enemy_y[3] = 0x82;
    beam_target_draw(0x00);
    record();

    /* 9. Row out of range: slot 3 active, col=128 (screen=$24),
     *    y=$80 -> `beam_y >= ey` continues. No hit. */
    clear_state();
    beam_state = 0x03;
    beam_y = 0x80;
    enemy_flag[3] = 0x01;
    enemy_col[3] = 128;
    enemy_y[3] = 0x80;
    beam_target_draw(0x00);
    record();

    /* 10. Inactive flag: slot 3 has flag=0 but matching col/y --
     *     `enemy_flag[slot] == 0` continues. No hit. */
    clear_state();
    beam_state = 0x03;
    beam_y = 0x80;
    enemy_col[3] = 128;
    enemy_y[3] = 0x82;
    beam_target_draw(0x00);
    record();

    return (int)log_idx;
}
)c";

// Each row is beam_state, beam_y, beam_snd_ctr, enemy_flag[0..3],
// score_add_lo, score_add_hi, draw_calls, draw_last_width,
// draw_last_height, draw_last_x, draw_last_y, draw_last_page, and
// score_calls. The counts of calls run on across the scenarios, and
// draw_last_* keep their values through a scenario that doesn't draw.
const std::vector<std::uint8_t> kExpected = {
    // 1. Idle: nothing happened.
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // 2. A delta of 0: the state stays $04 and beam_y $BB, with no draw.
    0x04, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // 3. A chase with no hit and the height clamped to 4. The draw is
    // (2, 4, $24, $80, beam_spr, $80), and the state doesn't change.
    0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x24, 0x80, 0x80, 0x00,
    // 4. A chase with no hit and a delta of 1. The draw is (2, 1, $24,
    // $6A, beam_spr, $00).
    0x02, 0x6A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x01, 0x24, 0x6A, 0x00, 0x00,
    // 5. A hit at slot 3, after the draw (2, 4, $24, $80, beam_spr, $40).
    // The hit clears enemy_flag[3] and beam_state, sets beam_snd_ctr to
    // $0A and the addend to $0100, and calls score_add.
    0x00, 0x80, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x02, 0x04, 0x24, 0x80, 0x40, 0x01,
    // 6. A hit at slot 0, after the draw (2, 4, $24, $80, beam_spr, $00).
    0x00, 0x80, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x02, 0x04, 0x24, 0x80, 0x00, 0x02,
    // 7. The hit at slot 3 hides the one at slot 0, so enemy_flag[0]
    // stays $55.
    0x00, 0x80, 0x0A, 0x55, 0x00, 0x00, 0x00, 0x00, 0x01, 0x05, 0x02, 0x04, 0x24, 0x80, 0x00, 0x03,
    // 8. A column too low: no hit, so enemy_flag[3] stays $01 and
    // beam_state $03.
    0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x06, 0x02, 0x04, 0x24, 0x80, 0x00, 0x03,
    // 9. A row out of range: no hit.
    0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x07, 0x02, 0x04, 0x24, 0x80, 0x00, 0x03,
    // 10. A slot whose flag is clear: no hit, and enemy_flag[3] stays 0.
    0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x02, 0x04, 0x24, 0x80, 0x00, 0x03,
};

struct Run {
  sim::SimResult result;
  std::vector<std::uint8_t> log;
};

Run run(bool optimize) {
  sim::Simulation s = build_sim(kProgram, optimize);
  Run out{s.run(5'000'000), {}};
  INFO("optimize=" << optimize);
  CHECK_FALSE(out.result.timed_out);
  const std::int64_t at = s.symbol("result_log");
  out.log.assign(out.result.memory.begin() + at, out.result.memory.begin() + at + 10 * 16);
  return out;
}

std::vector<std::uint8_t> hargs(const sim::SimResult& r) {
  return std::vector<std::uint8_t>(r.memory.begin() + r.hargs, r.memory.begin() + r.hargs + 2);
}

}  // namespace

TEST_CASE("beam_target_draw_sim: the unoptimized program records each scenario") {
  const Run r = run(false);
  // log_idx is 16 bytes for each of the 10 scenarios.
  CHECK((r.result.return_int() & 0xFFFF) == 10 * 16);
  CHECK(r.log == kExpected);
}

TEST_CASE("beam_target_draw_sim: the optimized program records each scenario") {
  const Run r = run(true);
  CHECK((r.result.return_int() & 0xFFFF) == 10 * 16);
  CHECK(r.log == kExpected);
}

TEST_CASE("beam_target_draw_sim: both pipelines record the same state and HARGS") {
  const Run unopt = run(false);
  const Run opt = run(true);
  CHECK(unopt.log == opt.log);
  CHECK(hargs(unopt.result) == hargs(opt.result));
}

}  // namespace c6502::suite
