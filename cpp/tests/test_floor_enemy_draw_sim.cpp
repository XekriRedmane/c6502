// The program covers the fix to `hwreg_eligibility` for `IndexedData`.
// Before it, the eligibility scan took an `IndexedData` operand as a
// Mov peer and pinned a Pseudo to X, and the assembler rejected the
// result with "unsupported Mov: IndexedData(name='enemy_col',
// ..., index=X()) -> Reg(reg=X())".
#include <cstdint>
#include <vector>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

const char* const kProgram = R"c(
#include <stdint.h>

uint8_t enemy_flag[4];
uint8_t enemy_col[4];
uint8_t enemy_y[4];

/* Record of each draw_sprite call. 6 bytes per call. */
uint8_t draw_log[128];
uint8_t log_idx;

[[c6502::zp_abi]]
void draw_sprite(uint8_t width, uint8_t height,
                 uint8_t sprite_x, uint8_t sprite_y,
                 const uint8_t *tile_src, uint8_t page_flag)
{
    uint8_t base = log_idx;
    draw_log[(uint8_t)(base + 0)] = width;
    draw_log[(uint8_t)(base + 1)] = height;
    draw_log[(uint8_t)(base + 2)] = sprite_x;
    draw_log[(uint8_t)(base + 3)] = sprite_y;
    /* Low byte of tile_src — high byte recoverable from slot tables. */
    draw_log[(uint8_t)(base + 4)] = (uint8_t)((uint16_t)tile_src & 0xFF);
    draw_log[(uint8_t)(base + 5)] = page_flag;
    log_idx = (uint8_t)(base + 6);
}

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

static const uint8_t proj_frame_idx[165] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x00,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x00, 0x01,
    0x02, 0x03, 0x04, 0x05, 0x06, 0x00, 0x01, 0x02,
    0x03, 0x04, 0x05, 0x06, 0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x00, 0x01, 0x02, 0x03, 0x04,
    0x05, 0x06, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x06, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x00,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x00, 0x01,
    0x02, 0x03, 0x04, 0x05, 0x06, 0x00, 0x01, 0x02,
    0x03, 0x04, 0x05, 0x06, 0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x00, 0x01, 0x02, 0x03, 0x04,
    0x05, 0x06, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x06, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x00,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x00, 0x01,
    0x02, 0x03, 0x04, 0x05, 0x06, 0x00, 0x01, 0x02,
    0x03, 0x04, 0x05, 0x06, 0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x00, 0x01, 0x02, 0x03, 0x04,
    0x05, 0x06, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x06, 0x00, 0x01, 0x02, 0x03,
};

static const uint8_t floor_enemy_spr_s0_lo[7] = { 0xC7, 0xD1, 0xDB, 0xE5, 0xEF, 0xF9, 0x03 };
static const uint8_t floor_enemy_spr_s0_hi[7] = { 0x8D, 0x8D, 0x8D, 0x8D, 0x8D, 0x8D, 0x8E };
static const uint8_t floor_enemy_spr_s1_lo[7] = { 0x81, 0x8B, 0x95, 0x9F, 0xA9, 0xB3, 0xBD };
static const uint8_t floor_enemy_spr_s1_hi[7] = { 0x8D, 0x8D, 0x8D, 0x8D, 0x8D, 0x8D, 0x8D };
static const uint8_t floor_enemy_spr_s2_lo[7] = { 0x3B, 0x45, 0x4F, 0x59, 0x63, 0x6D, 0x77 };
static const uint8_t floor_enemy_spr_s2_hi[7] = { 0x8D, 0x8D, 0x8D, 0x8D, 0x8D, 0x8D, 0x8D };
static const uint8_t floor_enemy_spr_s3_lo[7] = { 0xF5, 0xFF, 0x09, 0x13, 0x1D, 0x27, 0x31 };
static const uint8_t floor_enemy_spr_s3_hi[7] = { 0x8C, 0x8C, 0x8D, 0x8D, 0x8D, 0x8D, 0x8D };

static const uint8_t *const floor_enemy_spr_lo[4] = {
    floor_enemy_spr_s0_lo,
    floor_enemy_spr_s1_lo,
    floor_enemy_spr_s2_lo,
    floor_enemy_spr_s3_lo,
};
static const uint8_t *const floor_enemy_spr_hi[4] = {
    floor_enemy_spr_s0_hi,
    floor_enemy_spr_s1_hi,
    floor_enemy_spr_s2_hi,
    floor_enemy_spr_s3_hi,
};

[[c6502::zp_abi]]
void floor_enemy_draw(uint8_t page_flag)
{
    for (int8_t slot = 3; slot >= 0; slot--) {
        if (enemy_flag[slot] == 0) {
            continue;
        }

        uint8_t col_idx    = enemy_col[slot];
        uint8_t screen_col = proj_screen_col[col_idx];
        uint8_t frame      = proj_frame_idx[col_idx];

        uint8_t lo = floor_enemy_spr_lo[slot][frame];
        uint8_t hi = floor_enemy_spr_hi[slot][frame];
        const uint8_t *src =
            (const uint8_t *)(((uint16_t)hi << 8) | lo);

        draw_sprite(0x01, 0x05, screen_col, enemy_y[slot], src, page_flag);
    }
}

void clear_slots(void) {
    for (uint8_t i = 0; i < 4; i = (uint8_t)(i + 1)) {
        enemy_flag[i] = 0;
        enemy_col[i] = 0;
        enemy_y[i] = 0;
    }
}

int main(void) {
    log_idx = 0;

    /* Scenario 1: all inactive — no calls. */
    clear_slots();
    floor_enemy_draw(0x00);

    /* Scenario 2: slot 3 only, col=4 (proj_screen_col[4]=1,
     *   proj_frame_idx[4]=4), y=$22, page_flag=$80.
     * Slot 3 sprite lo[4]=$1D. */
    clear_slots();
    enemy_flag[3] = 0xFF; enemy_col[3] = 4; enemy_y[3] = 0x22;
    floor_enemy_draw(0x80);

    /* Scenario 3: slots 0 and 2 active. Walked in reverse, so
     * slot 2 is drawn before slot 0.
     *   slot 2: col=0 → proj_screen_col[0]=0, proj_frame_idx[0]=0,
     *           lo = floor_enemy_spr_s2_lo[0] = $3B, y=$10.
     *   slot 0: col=131 → proj_screen_col[131]=$25,
     *           proj_frame_idx[131]=5 (131 mod 7 = 5),
     *           lo = floor_enemy_spr_s0_lo[5] = $F9, y=$80. */
    clear_slots();
    enemy_flag[0] = 0x01; enemy_col[0] = 131; enemy_y[0] = 0x80;
    enemy_flag[2] = 0xFF; enemy_col[2] = 0;   enemy_y[2] = 0x10;
    floor_enemy_draw(0x00);

    /* Scenario 4: all four slots active to stress the call chain.
     *   slot 3: col=11 → screen_col=$03, frame=4 (11 mod 7),
     *           lo = spr_s3_lo[4]=$1D, y=$11.
     *   slot 2: col=23 → screen_col=$06, frame=2 (23 mod 7),
     *           lo = spr_s2_lo[2]=$4F, y=$22.
     *   slot 1: col=70 → screen_col=$14, frame=0 (70 mod 7),
     *           lo = spr_s1_lo[0]=$81, y=$33.
     *   slot 0: col=127 → screen_col=$24, frame=1 (127 mod 7),
     *           lo = spr_s0_lo[1]=$D1, y=$44. */
    clear_slots();
    enemy_flag[0] = 0xFF; enemy_col[0] = 127; enemy_y[0] = 0x44;
    enemy_flag[1] = 0x01; enemy_col[1] = 70;  enemy_y[1] = 0x33;
    enemy_flag[2] = 0xAA; enemy_col[2] = 23;  enemy_y[2] = 0x22;
    enemy_flag[3] = 0x55; enemy_col[3] = 11;  enemy_y[3] = 0x11;
    floor_enemy_draw(0x7F);

    return (int)log_idx;
}
)c";

// Each `draw_sprite` call's [width, height, sprite_x, sprite_y,
// src_low_byte, page_flag]. The table is walked from slot 3 down.
const std::vector<std::uint8_t> kExpected = {
    // Scenario 2: slot 3 only.
    0x01, 0x05, 0x01, 0x22, 0x1D, 0x80,
    // Scenario 3: slot 2, then slot 0.
    0x01, 0x05, 0x00, 0x10, 0x3B, 0x00,  //
    0x01, 0x05, 0x25, 0x80, 0xF9, 0x00,
    // Scenario 4: slots 3, 2, 1, and 0.
    0x01, 0x05, 0x03, 0x11, 0x1D, 0x7F,  //
    0x01, 0x05, 0x06, 0x22, 0x4F, 0x7F,  //
    0x01, 0x05, 0x14, 0x33, 0x81, 0x7F,  //
    0x01, 0x05, 0x24, 0x44, 0xD1, 0x7F,
};

struct Run {
  sim::SimResult result;
  std::vector<std::uint8_t> log;
};

Run run(bool optimize) {
  sim::Simulation s = build_sim(kProgram, optimize);
  Run out{s.run(2'000'000), {}};
  INFO("optimize=" << optimize);
  CHECK_FALSE(out.result.timed_out);
  const std::int64_t at = s.symbol("draw_log");
  out.log.assign(out.result.memory.begin() + at, out.result.memory.begin() + at + kExpected.size());
  return out;
}

}  // namespace

TEST_CASE("floor_enemy_draw_sim: the unoptimized program records each call") {
  const Run r = run(false);
  // `main` returns log_idx, the count of bytes written.
  CHECK((r.result.return_int() & 0xFFFF) == static_cast<std::int64_t>(kExpected.size()));
  CHECK(r.log == kExpected);
}

TEST_CASE("floor_enemy_draw_sim: the optimized program records each call") {
  const Run r = run(true);
  CHECK((r.result.return_int() & 0xFFFF) == static_cast<std::int64_t>(kExpected.size()));
  CHECK(r.log == kExpected);
}

TEST_CASE("floor_enemy_draw_sim: both pipelines record the same calls") {
  CHECK(run(false).log == run(true).log);
}

}  // namespace c6502::suite
