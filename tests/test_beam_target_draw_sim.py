"""End-to-end simulator test for `beam_target_draw` under both the
unoptimized and optimized pipelines.

`beam_target_draw` gates on `beam_state`, computes the beam-trail
height `floor_y_table[state & 0x0F] - beam_y` (clamped to 4 rows),
draws the trail via `draw_sprite`, then walks 4 enemy slots (3..0)
looking for the first one whose `proj_screen_col[col]` lands in
{$24, $25} AND whose `y` sits in `(beam_y, beam_y + 3]`. The first
hit zeros the enemy flag, drops `beam_state` back to idle, arms the
post-hit jingle (`beam_snd_ctr := $0A`), loads a `$0100` BCD score
addend, and calls `score_add`.

Scenarios cover: idle gate, delta-zero gate, chase-only no-hit at
two clamp boundaries, hit at slot 3 (loop entry), hit at slot 0
(loop iterates all the way down), hit-3-shadows-hit-0 (early
return), column-too-low miss, row-out-of-range miss, and
inactive-flag skip. Each scenario writes 16 bytes of observable
state into a flat `result_log`; the three tests assert (a) unopt
matches the hand-computed expected state, (b) opt matches expected,
and (c) opt == unopt byte-for-byte.
"""

import shutil
import unittest

import sim.runtime as rt_mod
from sim.harness import build_sim


# Inlined beam_target_draw source plus definitions of the externs it
# references, recording stubs for `draw_sprite` / `score_add`, and a
# `main` driver that exercises one scenario per branch.
_PROGRAM = r"""
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
"""


# Hand-computed expected state per scenario. Each entry is the
# 16-byte record `record()` lays down. draw_calls / score_calls
# are cumulative across the run; draw_last_* persist between
# scenarios that don't redraw.
def _scenarios() -> list[list[int]]:
    # 1. Idle: nothing happened.
    s1 = [
        0x00,  # beam_state
        0x00,  # beam_y
        0x00,  # beam_snd_ctr
        0x00, 0x00, 0x00, 0x00,  # enemy_flag[0..3]
        0x00,  # score_add_lo
        0x00,  # score_add_hi
        0x00,  # draw_calls
        0x00, 0x00, 0x00, 0x00, 0x00,  # draw_last_*
        0x00,  # score_calls
    ]
    # 2. Delta=0: state stays $04, beam_y stays $BB. No draw.
    s2 = [
        0x04, 0xBB, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00,
        0x00,
        0x00, 0x00, 0x00, 0x00, 0x00,
        0x00,
    ]
    # 3. Chase no-hit, height clamped to 4. draw fires
    #    (2, 4, $24, $80, beam_spr, $80). State unchanged.
    s3 = [
        0x03, 0x80, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00,
        0x01,                              # draw_calls
        0x02, 0x04, 0x24, 0x80, 0x80,       # last_width, height, x, y, page
        0x00,
    ]
    # 4. Chase no-hit, small delta=1. draw fires (2, 1, $24, $6A,
    #    beam_spr, $00).
    s4 = [
        0x02, 0x6A, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00,
        0x02,
        0x02, 0x01, 0x24, 0x6A, 0x00,
        0x00,
    ]
    # 5. Hit slot 3. Pre-hit: draw fires (2, 4, $24, $80, beam_spr,
    #    $40). Post-hit: enemy_flag[3]=0, beam_state=0,
    #    beam_snd_ctr=$0A, score_add_lo=0, score_add_hi=1,
    #    score_calls=1.
    s5 = [
        0x00, 0x80, 0x0A,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x01,
        0x03,
        0x02, 0x04, 0x24, 0x80, 0x40,
        0x01,
    ]
    # 6. Hit slot 0. Draw fires (2, 4, $24, $80, beam_spr, $00).
    #    enemy_flag[0]=0 after hit; beam_state=0; snd=$0A.
    s6 = [
        0x00, 0x80, 0x0A,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x01,
        0x04,
        0x02, 0x04, 0x24, 0x80, 0x00,
        0x02,
    ]
    # 7. Hit-3 shadows hit-0. Draw fires (2, 4, $24, $80, beam_spr,
    #    $00). Slot 3 hit clears enemy_flag[3] to 0; slot 0 stays
    #    at $55. beam_state=0, snd=$0A, score_lo/hi=0/1.
    s7 = [
        0x00, 0x80, 0x0A,
        0x55, 0x00, 0x00, 0x00,
        0x00, 0x01,
        0x05,
        0x02, 0x04, 0x24, 0x80, 0x00,
        0x03,
    ]
    # 8. Column too low. Draw fires (2, 4, $24, $80, beam_spr,
    #    $00). No hit -- enemy_flag[3] stays at $01,
    #    beam_state stays at $03.
    s8 = [
        0x03, 0x80, 0x00,
        0x00, 0x00, 0x00, 0x01,
        0x00, 0x00,
        0x06,
        0x02, 0x04, 0x24, 0x80, 0x00,
        0x03,
    ]
    # 9. Row out of range. Draw fires. No hit -- enemy_flag[3]
    #    stays at $01, beam_state stays at $03.
    s9 = [
        0x03, 0x80, 0x00,
        0x00, 0x00, 0x00, 0x01,
        0x00, 0x00,
        0x07,
        0x02, 0x04, 0x24, 0x80, 0x00,
        0x03,
    ]
    # 10. Inactive flag. Draw fires. No hit -- enemy_flag[3]
    #     stays at $00.
    s10 = [
        0x03, 0x80, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00,
        0x08,
        0x02, 0x04, 0x24, 0x80, 0x00,
        0x03,
    ]
    return [s1, s2, s3, s4, s5, s6, s7, s8, s9, s10]


def _flatten(rows: list[list[int]]) -> bytes:
    out = bytearray()
    for row in rows:
        out.extend(row)
    return bytes(out)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not on PATH")
class TestBeamTargetDrawSim(unittest.TestCase):
    """Differential opt vs unopt check on `beam_target_draw`."""

    def _run(self, optimize: bool):
        sim = build_sim(_PROGRAM, optimize=optimize)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(
            result.timed_out,
            f"beam_target_draw sim timed out (optimize={optimize})",
        )
        log_addr = sim.symbols["result_log"]
        log_bytes = bytes(result.memory[log_addr:log_addr + 10 * 16])
        return result, log_bytes

    def test_unoptimized_matches_expected(self):
        result, log = self._run(optimize=False)
        self.assertEqual(
            result.return_int() & 0xFFFF, 10 * 16,
            "log_idx should reflect 10 recorded scenarios * 16 bytes",
        )
        self.assertEqual(log, _flatten(_scenarios()))

    def test_optimized_matches_expected(self):
        result, log = self._run(optimize=True)
        self.assertEqual(result.return_int() & 0xFFFF, 10 * 16)
        self.assertEqual(log, _flatten(_scenarios()))

    def test_opt_and_unopt_agree(self):
        unopt_result, unopt_log = self._run(optimize=False)
        opt_result, opt_log = self._run(optimize=True)
        self.assertEqual(
            unopt_log, opt_log,
            "optimizer changed observable state",
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
