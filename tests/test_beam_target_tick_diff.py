"""Optimized-vs-unoptimized differential test for
`beam_target_tick`, focused on the three dispatch states the new
`transfer_pm1_store` peephole could plausibly perturb:

  - `beam_state == 0x80` — attack phase, floor 0 (target =
    floor_y_table[0] = $00). The SBC chain that decrements
    `beam_snd_ctr` runs unconditionally before the dispatch, so
    every state value exercises the new peephole.
  - `beam_state == 0x00` — idle. Several sub-cases: jingle armed
    at various counter values, tick gating the seed.
  - `beam_state == 0x01` — chase, floor 1 (target =
    floor_ceil[1]+1). Exercises the loop tail in the chase branch.

Unlike `test_beam_target_tick_sim`, this file does NOT compute
expected state by hand for each scenario — the unoptimized
pipeline is the ground truth, and we only check that the optimized
pipeline produces byte-identical observable state. The dedicated
hand-checked tests live in `test_beam_target_tick_sim`; this file
adds depth of coverage at the cost of trusting unopt.
"""

import shutil
import unittest

from sim.harness import build_sim


# Inlined source: the function under test, externs / stubs, plus a
# table-driven `main()` that exercises a battery of state-x-y-x-
# snd_ctr-x-tick-x-seed combinations and records observable state
# per scenario. The scenarios array is laid out as a flat byte
# table of (state, y, snd_ctr, tick, seed) 5-tuples terminated by
# an end-of-table sentinel `state == 0xAA`.
_PROGRAM = r"""
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
"""


_SCENARIOS = 24
_RECORD_SIZE = 8


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not on PATH")
class TestBeamTargetTickOptDiff(unittest.TestCase):
    """Run the same C source through both pipelines and compare the
    full `result_log` buffer byte-for-byte. Any optimizer-introduced
    divergence (incl. the new `transfer_pm1_store` peephole) shows
    up as a buffer mismatch."""

    def _run(self, optimize: bool) -> bytes:
        sim = build_sim(_PROGRAM, optimize=optimize)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(
            result.timed_out,
            f"beam_target_tick diff sim timed out (optimize={optimize})",
        )
        self.assertEqual(
            result.return_int() & 0xFFFF,
            _SCENARIOS * _RECORD_SIZE,
            f"log_idx mismatch (optimize={optimize})",
        )
        log_addr = sim.symbols["result_log"]
        return bytes(
            result.memory[log_addr:log_addr + _SCENARIOS * _RECORD_SIZE]
        )

    def test_opt_matches_unopt_full_buffer(self):
        unopt = self._run(optimize=False)
        opt = self._run(optimize=True)
        self.assertEqual(unopt, opt, "optimizer changed observable state")

    def test_opt_matches_unopt_attack_state_80(self):
        """First 6 scenarios cover beam_state == 0x80 variations."""
        unopt = self._run(optimize=False)
        opt = self._run(optimize=True)
        size = 6 * _RECORD_SIZE
        self.assertEqual(
            unopt[:size], opt[:size],
            "optimizer perturbed the beam_state == 0x80 (attack) cases",
        )

    def test_opt_matches_unopt_chase_state_01(self):
        """Scenarios 6..11 cover beam_state == 0x01 variations."""
        unopt = self._run(optimize=False)
        opt = self._run(optimize=True)
        start = 6 * _RECORD_SIZE
        end = 12 * _RECORD_SIZE
        self.assertEqual(
            unopt[start:end], opt[start:end],
            "optimizer perturbed the beam_state == 0x01 (chase) cases",
        )

    def test_opt_matches_unopt_idle_state_00(self):
        """Scenarios 12..19 cover beam_state == 0x00 variations."""
        unopt = self._run(optimize=False)
        opt = self._run(optimize=True)
        start = 12 * _RECORD_SIZE
        end = 20 * _RECORD_SIZE
        self.assertEqual(
            unopt[start:end], opt[start:end],
            "optimizer perturbed the beam_state == 0x00 (idle) cases",
        )


if __name__ == "__main__":
    unittest.main()
