// `special_inactive_draw` from examples/special_inactive_draw.c, run in
// both pipelines with a `draw_sprite` that records its arguments, from
// tests/test_special_inactive_draw_sim.py.
//
// Each scenario's frame is [width, height, sprite_x, sprite_y,
// page_flag, tile_src_ok, recv_calls, 0]. `tile_src_ok` is whether
// `tile_src == special_peek_sprite`, since the pointer itself differs
// between the two pipelines' layouts.
#include <cstdint>
#include <vector>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

const char* const kProgram = R"c(
#include <stdint.h>

/* Captured args from the most recent draw_sprite call. */
uint8_t recv_width;
uint8_t recv_height;
uint8_t recv_x;
uint8_t recv_y;
uint8_t recv_page_flag;
uint8_t recv_tile_src_ok;
uint8_t recv_calls;

/* Backing definition for the per-level peek-marker sprite. */
const uint8_t special_peek_sprite[12] = {
    0x10, 0x20, 0x30, 0x40, 0x50, 0x60,
    0x70, 0x80, 0x90, 0xA0, 0xB0, 0xC0,
};

[[c6502::zp_abi]]
void draw_sprite(uint8_t width,
                 uint8_t height,
                 uint8_t sprite_x,
                 uint8_t sprite_y,
                 const uint8_t *tile_src,
                 uint8_t page_flag) {
    recv_calls = (uint8_t)(recv_calls + 1);
    recv_width = width;
    recv_height = height;
    recv_x = sprite_x;
    recv_y = sprite_y;
    recv_page_flag = page_flag;
    recv_tile_src_ok = (tile_src == special_peek_sprite) ? 1 : 0;
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

[[c6502::zp_abi]]
void special_inactive_draw(uint8_t special_row,
                           uint8_t special_pos_hi,
                           uint8_t page_flag)
{
    uint8_t sprite_x = proj_screen_col[special_pos_hi];
    draw_sprite(0x02, 0x06, sprite_x, special_row,
                special_peek_sprite, page_flag);
}

/* Recorded snapshot. 8 bytes per scenario:
 *   [width, height, sprite_x, sprite_y, page_flag,
 *    tile_src_ok, recv_calls, 0]. */
uint8_t result_log[64];
uint8_t log_idx;

void record(void) {
    uint8_t base = log_idx;
    result_log[(uint8_t)(base + 0)] = recv_width;
    result_log[(uint8_t)(base + 1)] = recv_height;
    result_log[(uint8_t)(base + 2)] = recv_x;
    result_log[(uint8_t)(base + 3)] = recv_y;
    result_log[(uint8_t)(base + 4)] = recv_page_flag;
    result_log[(uint8_t)(base + 5)] = recv_tile_src_ok;
    result_log[(uint8_t)(base + 6)] = recv_calls;
    result_log[(uint8_t)(base + 7)] = 0;
    log_idx = (uint8_t)(base + 8);
}

int main(void) {
    recv_calls = 0;
    log_idx = 0;

    /* 1. pos_hi=0 -> sprite_x=0x00; row=0x10, page=0x00. */
    special_inactive_draw(0x10, 0, 0x00);
    record();

    /* 2. pos_hi=7 -> sprite_x=0x02; row=0x40, page=0x00. */
    special_inactive_draw(0x40, 7, 0x00);
    record();

    /* 3. pos_hi=50 -> sprite_x=0x0E; row=0x80, page=0x80. */
    special_inactive_draw(0x80, 50, 0x80);
    record();

    /* 4. pos_hi=100 -> sprite_x=0x1C; row=0x60, page=0x01. */
    special_inactive_draw(0x60, 100, 0x01);
    record();

    /* 5. pos_hi=131 -> sprite_x=0x25; row=0xC0, page=0xFF. */
    special_inactive_draw(0xC0, 131, 0xFF);
    record();

    return (int)log_idx;
}
)c";

// width and height are the fixed 0x02 and 0x06 of the peek marker, and
// recv_calls counts the calls so far.
const std::vector<std::uint8_t> kExpected = {
    0x02, 0x06, 0x00, 0x10, 0x00, 1, 1, 0,
    0x02, 0x06, 0x02, 0x40, 0x00, 1, 2, 0,
    0x02, 0x06, 0x0E, 0x80, 0x80, 1, 3, 0,
    0x02, 0x06, 0x1C, 0x60, 0x01, 1, 4, 0,
    0x02, 0x06, 0x25, 0xC0, 0xFF, 1, 5, 0,
};

struct Run {
  sim::SimResult result;
  std::vector<std::uint8_t> log;
};

Run run(Pipeline pipeline) {
  sim::Simulation s = build_sim(kProgram, pipeline);
  Run out{s.run(2'000'000), {}};
  INFO("pipeline=" << pipeline);
  CHECK_FALSE(out.result.timed_out);
  const std::int64_t at = s.symbol("result_log");
  out.log.assign(out.result.memory.begin() + at, out.result.memory.begin() + at + 8 * 5);
  return out;
}

std::vector<std::uint8_t> hargs(const sim::SimResult& r) {
  return std::vector<std::uint8_t>(r.memory.begin() + r.hargs, r.memory.begin() + r.hargs + 2);
}

}  // namespace

TEST_CASE("special_inactive_draw_sim: the unoptimized program records each frame") {
  const Run r = run(Pipeline::kUnoptimized);
  // log_idx is 8 bytes for each of the 5 scenarios.
  CHECK((r.result.return_int() & 0xFFFF) == 5 * 8);
  CHECK(r.log == kExpected);
}

TEST_CASE("special_inactive_draw_sim: the optimized program records each frame") {
  const Run r = run(Pipeline::kOptimized);
  CHECK((r.result.return_int() & 0xFFFF) == 5 * 8);
  CHECK(r.log == kExpected);
}

TEST_CASE("special_inactive_draw_sim: both pipelines record the same frames and HARGS") {
  const Run unopt = run(Pipeline::kUnoptimized);
  const Run opt = run(Pipeline::kOptimized);
  CHECK(unopt.log == opt.log);
  CHECK(hargs(unopt.result) == hargs(opt.result));
}

}  // namespace c6502::suite
