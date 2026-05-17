"""End-to-end simulator test for `step_pos` under both the
unoptimized and optimized pipelines.

`step_pos` decrements the per-slot animation counter, then advances
the 16-bit world-X (xoff_idx:floor_col) by 3 with carry between the
two bytes, then dispatches to `apply_bobble(slot, new_anim)`.

A `main` driver runs a battery of scenarios that exercise:
  1. Plain increment with no carry into the high byte.
  2. Low byte wrapping ($FF + 3) and carry into the high byte.
  3. Both bytes near the top of their ranges; high byte overflows
     past the (0..3) range the source comments mention (the C code
     does NOT clamp, so we verify the unclamped behavior).
  4. anim_in == 0 -> new_anim wraps to $FF.
  5. Mid-range slot far from the wrap boundaries.

For each scenario the driver records: slot, rescue_anim[slot],
entity_floor_col[slot], entity_xoff_idx[slot], the last
apply_bobble args, and a cumulative apply_bobble call count.
"""

import shutil
import unittest

from sim.harness import build_sim


# Source: examples/step_pos.c, inlined with an `apply_bobble` stub
# (the original declares it extern) plus a `main` driver and the
# minimum subset of the external state tables that step_pos touches.
_PROGRAM = r"""
#include <stdint.h>

uint8_t entity_floor_col[20];
uint8_t entity_xoff_idx[20];
uint8_t rescue_anim[20];

/* apply_bobble side-effect log. */
uint8_t bobble_calls;
uint8_t bobble_last_slot;
uint8_t bobble_last_idx;

__attribute__((zp_abi))
void apply_bobble(uint8_t slot, uint8_t bobble_idx)
{
    bobble_calls = (uint8_t)(bobble_calls + 1);
    bobble_last_slot = slot;
    bobble_last_idx = bobble_idx;
}

__attribute__((zp_abi))
static void step_pos(uint8_t slot, uint8_t anim_in)
{
    uint8_t new_anim = (uint8_t)(anim_in - 1);
    rescue_anim[slot] = new_anim;
    uint16_t world_x =
        ((uint16_t)entity_xoff_idx[slot] << 8) | entity_floor_col[slot];
    world_x = (uint16_t)(world_x + 3);
    entity_floor_col[slot] = (uint8_t)world_x;
    entity_xoff_idx[slot]  = (uint8_t)(world_x >> 8);
    apply_bobble(slot, new_anim);
}

/* 8 bytes per scenario:
 *   [slot, rescue_anim[slot], entity_floor_col[slot],
 *    entity_xoff_idx[slot], bobble_last_slot, bobble_last_idx,
 *    bobble_calls, 0 (pad)] */
uint8_t result_log[64];
uint8_t log_idx;

void clear_slots(void) {
    for (uint8_t i = 0; i < 20; i = (uint8_t)(i + 1)) {
        entity_floor_col[i] = 0;
        entity_xoff_idx[i] = 0;
        rescue_anim[i] = 0;
    }
}

void record(uint8_t slot) {
    uint8_t base = log_idx;
    result_log[(uint8_t)(base + 0)] = slot;
    result_log[(uint8_t)(base + 1)] = rescue_anim[slot];
    result_log[(uint8_t)(base + 2)] = entity_floor_col[slot];
    result_log[(uint8_t)(base + 3)] = entity_xoff_idx[slot];
    result_log[(uint8_t)(base + 4)] = bobble_last_slot;
    result_log[(uint8_t)(base + 5)] = bobble_last_idx;
    result_log[(uint8_t)(base + 6)] = bobble_calls;
    result_log[(uint8_t)(base + 7)] = 0;
    log_idx = (uint8_t)(base + 8);
}

int main(void) {
    bobble_calls = 0;
    log_idx = 0;

    /* 1. Slot 0, no carry: world_x = 0x0010 + 3 = 0x0013. */
    clear_slots();
    entity_xoff_idx[0]  = 0x00;
    entity_floor_col[0] = 0x10;
    step_pos(0, 8);
    record(0);

    /* 2. Slot 5, low-byte wrap: world_x = 0x02FE + 3 = 0x0301. */
    clear_slots();
    entity_xoff_idx[5]  = 0x02;
    entity_floor_col[5] = 0xFE;
    step_pos(5, 3);
    record(5);

    /* 3. Slot 19, both bytes wrap: world_x = 0x03FF + 3 = 0x0402.
     *    (High byte goes past the documented 0..3 range; the C code
     *    does not clamp, so we verify the unclamped result.) */
    clear_slots();
    entity_xoff_idx[19]  = 0x03;
    entity_floor_col[19] = 0xFF;
    step_pos(19, 1);
    record(19);

    /* 4. Slot 10, low byte +3 crosses 0x100: 0x00FF + 3 = 0x0102. */
    clear_slots();
    entity_xoff_idx[10]  = 0x00;
    entity_floor_col[10] = 0xFF;
    step_pos(10, 8);
    record(10);

    /* 5. Slot 3, anim_in = 0 -> new_anim wraps to 0xFF. */
    clear_slots();
    entity_xoff_idx[3]  = 0x00;
    entity_floor_col[3] = 0x05;
    step_pos(3, 0);
    record(3);

    return (int)log_idx;
}
"""


# Expected post-call state, hand-computed scenario-by-scenario.
# Each row mirrors the 8-byte layout written by `record`.
def _expected() -> bytes:
    rows = [
        # 1. slot 0, anim 8->7, col 0x10+3=0x13, xoff stays 0.
        [0,    7, 0x13, 0x00, 0,    7, 1, 0],
        # 2. slot 5, anim 3->2, col 0xFE+3=0x01 wrap, xoff 0x02->0x03.
        [5,    2, 0x01, 0x03, 5,    2, 2, 0],
        # 3. slot 19, anim 1->0, col 0xFF+3=0x02 wrap, xoff 0x03->0x04.
        [19,   0, 0x02, 0x04, 19,   0, 3, 0],
        # 4. slot 10, anim 8->7, col 0xFF+3=0x02 wrap, xoff 0x00->0x01.
        [10,   7, 0x02, 0x01, 10,   7, 4, 0],
        # 5. slot 3, anim 0->0xFF, col 0x05+3=0x08, xoff stays 0.
        [3, 0xFF, 0x08, 0x00, 3, 0xFF, 5, 0],
    ]
    out = bytearray()
    for row in rows:
        out.extend(row)
    return bytes(out)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not on PATH")
class TestStepPosSim(unittest.TestCase):
    """Differential opt-vs-unopt check on `step_pos`."""

    def _run(self, optimize: bool):
        sim = build_sim(_PROGRAM, optimize=optimize)
        result = sim.run(max_cycles=2_000_000)
        self.assertFalse(
            result.timed_out,
            f"step_pos sim timed out (optimize={optimize})",
        )
        log_addr = sim.symbols["result_log"]
        log_bytes = bytes(result.memory[log_addr:log_addr + 8 * 5])
        return result, log_bytes

    def test_unoptimized_matches_expected(self):
        result, log = self._run(optimize=False)
        self.assertEqual(
            result.return_int() & 0xFFFF, 8 * 5,
            "log_idx should reflect 5 recorded scenarios * 8 bytes",
        )
        self.assertEqual(log, _expected())

    def test_optimized_matches_expected(self):
        result, log = self._run(optimize=True)
        self.assertEqual(result.return_int() & 0xFFFF, 8 * 5)
        self.assertEqual(log, _expected())

    def test_opt_and_unopt_agree(self):
        _, unopt_log = self._run(optimize=False)
        _, opt_log = self._run(optimize=True)
        self.assertEqual(
            unopt_log, opt_log,
            "optimizer changed observable step_pos state",
        )


if __name__ == "__main__":
    unittest.main()
