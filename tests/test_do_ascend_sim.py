"""End-to-end simulator test for `do_ascend` under both the
unoptimized and optimized pipelines.

The function dispatches on what (if anything) the ascending player
landed on. We exercise the four branches:

  1. Already at the ceiling on entry (col == floor_ceil[asc_floor])
     -> sfx_tone($05, $04); beam_tick--; player_col unchanged.
  2. The -4 step lands exactly on the ceiling
     -> move_dir := 0; sfx_tone($05, $04); beam_tick--.
  3. The -4 step lands on the floor-side threshold
     -> propagate asc_floor into the three other floor mirrors;
        ent_rescued := $FF; wipe entity_hit_state[0..hit_max];
        fall through to snd_delay_up($04, $08).
  4. Otherwise (mid-air) -> snd_delay_up($04, $08).

Each scenario records 16 bytes of observable state into `result_log`;
the three tests assert (a) unopt matches hand-computed expected,
(b) opt matches expected, (c) opt and unopt agree byte-for-byte.
"""

import shutil
import unittest

import sim.runtime as rt_mod
from sim.harness import build_sim


# Inlined do_ascend source plus stubs for `sfx_tone` /
# `snd_delay_up`, definitions for every extern the example
# references, and a `main` that exercises the four branches.
_PROGRAM = r"""
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
"""


# Expected per-scenario observable state. Each list is the 16-byte
# record laid down by `record()`. `sfx_calls` / `snd_up_calls` are
# cumulative across scenarios.
def _scenarios() -> list[list[int]]:
    # Scenario 1: already at the ceiling.
    #   Branch 1 fires: sfx_tone($05,$04); beam_tick--; return.
    s1 = [
        0x10,  # player_col (unchanged)
        0x55,  # move_dir (unchanged)
        0x99,  # beam_seed_floor (unchanged)
        0xAA,  # floor_mirror (unchanged)
        0xBB,  # dsc_floor (unchanged)
        0xCC,  # ent_rescued (unchanged)
        0x04,  # beam_tick: 0x05 - 1
        0x01,  # sfx_calls
        0x05,  # sfx_last_pitch
        0x04,  # sfx_last_dur
        0x00,  # snd_up_calls
        0x00,  # snd_up_last_pitch
        0x00,  # snd_up_last_clicks
        0x00,  # entity_hit_state[0]
        0x00,  # entity_hit_state[3]
        0x00,  # entity_hit_state[4]
    ]
    # Scenario 2: -4 step lands on the ceiling.
    #   col=$34; ceil[2]=$30; first compare fails.
    #   col := $34-4 = $30; player_col := $30.
    #   col == ceil[2]: move_dir := 0; sfx_tone; beam_tick--.
    s2 = [
        0x30,  # player_col
        0x00,  # move_dir cleared
        0x99, 0xAA, 0xBB, 0xCC,
        0x04,  # beam_tick
        0x02,  # sfx_calls (cumulative)
        0x05, 0x04,
        0x00, 0x00, 0x00,
        0x00, 0x00, 0x00,
    ]
    # Scenario 3: -4 step lands on the floor threshold.
    #   col=$28; ceil[1]=$20; first compare fails.
    #   col := $24; player_col := $24.
    #   col != ceil[1]($20); col == thresh[1]($24): wipe + fall
    #   through to snd_delay_up.
    #   Wipe runs i=3..0, so entity_hit_state[0..3] := $FF; [4]
    #   untouched (= 0).
    s3 = [
        0x24,  # player_col
        0x55,  # move_dir untouched
        0x01,  # beam_seed_floor := asc_floor (1)
        0x01,  # floor_mirror
        0x01,  # dsc_floor
        0xFF,  # ent_rescued
        0x05,  # beam_tick (no decrement on this branch)
        0x02,  # sfx_calls (no new sfx call)
        0x05, 0x04,  # last sfx args carry from scenario 2
        0x01,  # snd_up_calls
        0x04, 0x08,
        0xFF,  # entity_hit_state[0] wiped
        0xFF,  # entity_hit_state[3] wiped (i=3 included)
        0x00,  # entity_hit_state[4] untouched
    ]
    # Scenario 4: mid-air step.
    #   col=$50; ceil[3]=$40; first compare fails.
    #   col := $4C; player_col := $4C.
    #   col != ceil[3]($40) and != thresh[3]($44): just snd_delay_up.
    s4 = [
        0x4C,  # player_col
        0x55, 0x99, 0xAA, 0xBB, 0xCC,
        0x05,  # beam_tick (no decrement)
        0x02,  # sfx_calls (cumulative)
        0x05, 0x04,
        0x02,  # snd_up_calls
        0x04, 0x08,
        0x00, 0x00, 0x00,
    ]
    return [s1, s2, s3, s4]


def _flatten(rows: list[list[int]]) -> bytes:
    out = bytearray()
    for row in rows:
        out.extend(row)
    return bytes(out)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not on PATH")
class TestDoAscendSim(unittest.TestCase):
    """Differential opt vs unopt check on `do_ascend`."""

    def _run(self, optimize: bool):
        sim = build_sim(_PROGRAM, optimize=optimize)
        result = sim.run(max_cycles=2_000_000)
        self.assertFalse(
            result.timed_out,
            f"do_ascend sim timed out (optimize={optimize})",
        )
        log_addr = sim.symbols["result_log"]
        log_bytes = bytes(result.memory[log_addr:log_addr + 4 * 16])
        return result, log_bytes

    def test_unoptimized_matches_expected(self):
        result, log = self._run(optimize=False)
        self.assertEqual(
            result.return_int() & 0xFFFF, 4 * 16,
            "log_idx should reflect 4 recorded scenarios * 16 bytes",
        )
        self.assertEqual(log, _flatten(_scenarios()))

    def test_optimized_matches_expected(self):
        result, log = self._run(optimize=True)
        self.assertEqual(result.return_int() & 0xFFFF, 4 * 16)
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
