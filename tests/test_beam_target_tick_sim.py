"""End-to-end simulator test for `beam_target_tick` under both
the unoptimized and optimized pipelines.

`beam_target_tick` runs two independent passes per call:

  1. Jingle tick (only when `beam_snd_ctr >= 0` as int8_t) -> play
     `beam_jingle[beam_snd_ctr]` via `snd_delay_up`, decrement
     counter.
  2. State-machine dispatch on `beam_state`:
       Attack ($FX): descend toward `floor_y_table[state & $0F]`.
       Chase  ($01..$7F): ascend toward `floor_ceil[state] + 1`,
         flip to attack when reached.
       Idle   ($00): seed from `beam_seed_floor` iff `beam_tick==0`.

Each scenario records the post-call observable state into a flat
`result_log` array; the three tests assert (a) unopt matches the
hand-computed expected state, (b) opt matches expected, and (c)
opt == unopt byte-for-byte.
"""

import shutil
import unittest

import sim.runtime as rt_mod
from sim.harness import build_sim


# Inlined beam_target_tick source plus definitions for the externs
# the example references, stubs for `snd_delay_up` / `snd_delay_down`
# that record their call counts and last args, and a `main` driver
# that exercises one scenario per branch of the dispatch.
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

void snd_delay_up(uint8_t pitch, uint8_t clicks) {
    snd_up_calls = (uint8_t)(snd_up_calls + 1);
    snd_up_last_pitch = pitch;
    snd_up_last_clicks = clicks;
}

void snd_delay_down(uint8_t pitch, uint8_t clicks) {
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
"""


# Expected per-scenario observable state. Each list is the 16-byte
# record laid down by `record()`. snd_*_calls are cumulative.
def _scenarios() -> list[list[int]]:
    # 1. Jingle armed: pitch = jingle[$0A] = $19, ctr 10 -> 9.
    #    Idle tick=1 path is a no-op.
    s1 = [
        0x00,  # beam_state
        0x00,  # beam_y
        0x09,  # beam_snd_ctr (0x0A - 1)
        0x01,  # snd_up_calls
        0x19,  # snd_up_last_pitch
        0x0A,  # snd_up_last_clicks
        0x00,  # snd_down_calls
        0x00, 0x00,
        0, 0, 0, 0, 0, 0, 0,
    ]
    # 2. Jingle skipped (ctr=$FF), idle no-op.
    s2 = [
        0x00,
        0x00,
        0xFF,
        0x01,  # snd_up_calls unchanged
        0x19,
        0x0A,
        0x00,
        0x00, 0x00,
        0, 0, 0, 0, 0, 0, 0,
    ]
    # 3. Attack hit: state cleared, beam_y unchanged at $6B.
    s3 = [
        0x00,
        0x6B,
        0xFF,
        0x01,
        0x19, 0x0A,
        0x00,
        0x00, 0x00,
        0, 0, 0, 0, 0, 0, 0,
    ]
    # 4. Attack miss: beam_y $50 -> $52, snd_down fires.
    s4 = [
        0xF3,
        0x52,
        0xFF,
        0x01,
        0x19, 0x0A,
        0x01,  # snd_down_calls
        0x10, 0x0A,
        0, 0, 0, 0, 0, 0, 0,
    ]
    # 5. Chase at target: state $03 -> $F3, beam_y unchanged.
    s5 = [
        0xF3,
        0x41,
        0xFF,
        0x01,
        0x19, 0x0A,
        0x01,
        0x10, 0x0A,
        0, 0, 0, 0, 0, 0, 0,
    ]
    # 6. Chase miss: beam_y $80 -> $7E, snd_up fires ($10,$0A).
    s6 = [
        0x02,
        0x7E,
        0xFF,
        0x02,  # snd_up_calls
        0x10,  # snd_up_last_pitch overwrites prior $19
        0x0A,
        0x01,
        0x10, 0x0A,
        0, 0, 0, 0, 0, 0, 0,
    ]
    # 7. Idle seed: beam_y := $6B, state := 2.
    s7 = [
        0x02,
        0x6B,
        0xFF,
        0x02,
        0x10, 0x0A,
        0x01,
        0x10, 0x0A,
        0, 0, 0, 0, 0, 0, 0,
    ]
    # 8. Idle blocked: nothing changes.
    s8 = [
        0x00,
        0x77,
        0xFF,
        0x02,
        0x10, 0x0A,
        0x01,
        0x10, 0x0A,
        0, 0, 0, 0, 0, 0, 0,
    ]
    return [s1, s2, s3, s4, s5, s6, s7, s8]


def _flatten(rows: list[list[int]]) -> bytes:
    out = bytearray()
    for row in rows:
        out.extend(row)
    return bytes(out)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not on PATH")
class TestBeamTargetTickSim(unittest.TestCase):
    """Differential opt vs unopt check on `beam_target_tick`."""

    def _run(self, optimize: bool):
        sim = build_sim(_PROGRAM, optimize=optimize)
        result = sim.run(max_cycles=2_000_000)
        self.assertFalse(
            result.timed_out,
            f"beam_target_tick sim timed out (optimize={optimize})",
        )
        log_addr = sim.symbols["result_log"]
        log_bytes = bytes(result.memory[log_addr:log_addr + 8 * 16])
        return result, log_bytes

    def test_unoptimized_matches_expected(self):
        result, log = self._run(optimize=False)
        self.assertEqual(
            result.return_int() & 0xFFFF, 8 * 16,
            "log_idx should reflect 8 recorded scenarios * 16 bytes",
        )
        self.assertEqual(log, _flatten(_scenarios()))

    def test_optimized_matches_expected(self):
        result, log = self._run(optimize=True)
        self.assertEqual(result.return_int() & 0xFFFF, 8 * 16)
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
