"""End-to-end simulator test for `special_inactive_draw` under both
the unoptimized and optimized pipelines.

The function projects `special_pos_hi` through the 132-byte
`proj_screen_col` table to compute `sprite_x`, then forwards a fixed
2x6 peek-marker blit to `draw_sprite`. We stub `draw_sprite` to
capture the six args, run a battery of scenarios through `main`, and
write each captured frame into a flat `result_log` array. The test
then compares the per-pipeline log against a hand-computed expected
log AND requires the two pipelines to agree byte for byte.

Scenarios exercised:
  1. special_pos_hi=0   -> sprite_x = 0x00
  2. special_pos_hi=7   -> sprite_x = 0x02
  3. special_pos_hi=50  -> sprite_x = 0x0E
  4. special_pos_hi=100 -> sprite_x = 0x1C
  5. special_pos_hi=131 -> sprite_x = 0x25  (last in-bounds index)

Each frame records [width, height, sprite_x, sprite_y, page_flag,
tile_src_ok, recv_calls, 0] -- 8 bytes per scenario. `tile_src_ok`
is the boolean `tile_src == special_peek_sprite`, since the absolute
pointer differs between unopt and opt layouts.
"""

import shutil
import unittest

import sim.runtime as rt_mod
from sim.harness import build_sim


# Source: examples/special_inactive_draw.c, inlined together with a
# stub `draw_sprite` that records its arguments, a backing definition
# for `special_peek_sprite`, and a `main` that exercises the chosen
# scenarios and accumulates the captured args into `result_log`.
_PROGRAM = r"""
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

__attribute__((zp_abi))
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

__attribute__((zp_abi))
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
"""


# Expected per-scenario captured frames. Each row is
# [width, height, sprite_x, sprite_y, page_flag, tile_src_ok,
#  recv_calls (cumulative), 0]. width / height are the fixed
# 0x02 / 0x06 peek-marker constants the function passes.
_EXPECTED = [
    [0x02, 0x06, 0x00, 0x10, 0x00, 1, 1, 0],
    [0x02, 0x06, 0x02, 0x40, 0x00, 1, 2, 0],
    [0x02, 0x06, 0x0E, 0x80, 0x80, 1, 3, 0],
    [0x02, 0x06, 0x1C, 0x60, 0x01, 1, 4, 0],
    [0x02, 0x06, 0x25, 0xC0, 0xFF, 1, 5, 0],
]


def _flatten(rows: list[list[int]]) -> bytes:
    out = bytearray()
    for row in rows:
        out.extend(row)
    return bytes(out)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not on PATH")
class TestSpecialInactiveDrawSim(unittest.TestCase):
    """Differential opt vs unopt check on `special_inactive_draw`.

    Both pipelines must produce the same `result_log` bytes and the
    same return value (= log_idx after the last scenario)."""

    def _run(self, optimize: bool):
        sim = build_sim(_PROGRAM, optimize=optimize)
        result = sim.run(max_cycles=2_000_000)
        self.assertFalse(
            result.timed_out,
            f"special_inactive_draw sim timed out "
            f"(optimize={optimize})",
        )
        log_addr = sim.symbols["result_log"]
        log_bytes = bytes(result.memory[log_addr:log_addr + 8 * 5])
        return result, log_bytes

    def test_unoptimized_matches_expected(self):
        result, log = self._run(optimize=False)
        self.assertEqual(
            result.return_int() & 0xFFFF, 5 * 8,
            "log_idx should reflect 5 recorded scenarios * 8 bytes",
        )
        self.assertEqual(log, _flatten(_EXPECTED))

    def test_optimized_matches_expected(self):
        result, log = self._run(optimize=True)
        self.assertEqual(result.return_int() & 0xFFFF, 5 * 8)
        self.assertEqual(log, _flatten(_EXPECTED))

    def test_opt_and_unopt_agree(self):
        unopt_result, unopt_log = self._run(optimize=False)
        opt_result, opt_log = self._run(optimize=True)
        self.assertEqual(
            unopt_log, opt_log,
            "optimizer changed observable draw_sprite args",
        )
        unopt_hargs = bytes(
            unopt_result.memory[rt_mod.HARGS:rt_mod.HARGS + 2]
        )
        opt_hargs = bytes(
            opt_result.memory[rt_mod.HARGS:rt_mod.HARGS + 2]
        )
        self.assertEqual(unopt_hargs, opt_hargs)


if __name__ == "__main__":
    unittest.main()
