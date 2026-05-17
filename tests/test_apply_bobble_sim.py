"""End-to-end simulator test for `apply_bobble` under both the
unoptimized and optimized pipelines.

`apply_bobble(slot, bobble_idx)` looks up a 1-byte signed-magnitude
delta from `rescue_bobble[bobble_idx]` (bit 7 = descend, low 7 bits
= magnitude) and applies it in-place to `entity_floor_pos[slot]`:

  bit7=1 -> entity_floor_pos[slot] += magnitude  (descend)
  bit7=0 -> entity_floor_pos[slot] -= magnitude  (ascend)

`main` exercises the function across a battery of scenarios covering
both branches, magnitude=0 on each branch, the high-magnitude $7F
case, and 8-bit wrap on both add and subtract. Each scenario writes
two bytes to `result_log`: the pre-call row and the post-call row at
the targeted slot.
"""

import shutil
import unittest

import sim.runtime as rt_mod
from sim.harness import build_sim


# Source: examples/apply_bobble.c, inlined together with concrete
# definitions for `rescue_bobble` and `entity_floor_pos` (the only
# externs the function references), plus a `main` driver.
_PROGRAM = r"""
#include <stdint.h>

uint8_t entity_floor_pos[20];

/* 7-entry signed-magnitude delta table: bit 7 set = descend (row +=
 * magnitude), bit 7 clear = ascend (row -= magnitude). */
const uint8_t rescue_bobble[7] = {
    0x02,  /* idx 0: ascend by 2 */
    0x82,  /* idx 1: descend by 2 */
    0x01,  /* idx 2: ascend by 1 */
    0x81,  /* idx 3: descend by 1 */
    0x00,  /* idx 4: ascend by 0  (no-op) */
    0x80,  /* idx 5: descend by 0 (no-op) */
    0x7F,  /* idx 6: ascend by 127 */
};

__attribute__((zp_abi))
static void apply_bobble(uint8_t slot, uint8_t bobble_idx)
{
    uint8_t bobble    = rescue_bobble[bobble_idx];
    uint8_t magnitude = bobble & 0x7F;
    if (bobble & 0x80) {
        entity_floor_pos[slot] = (uint8_t)(entity_floor_pos[slot] + magnitude);
    } else {
        entity_floor_pos[slot] = (uint8_t)(entity_floor_pos[slot] - magnitude);
    }
}

/* Each scenario writes 2 bytes: [pre-call row, post-call row]. */
uint8_t result_log[64];
uint8_t log_idx;

void scenario(uint8_t slot, uint8_t bobble_idx, uint8_t initial) {
    entity_floor_pos[slot] = initial;
    uint8_t base = log_idx;
    result_log[(uint8_t)(base + 0)] = entity_floor_pos[slot];
    apply_bobble(slot, bobble_idx);
    result_log[(uint8_t)(base + 1)] = entity_floor_pos[slot];
    log_idx = (uint8_t)(base + 2);
}

int main(void) {
    log_idx = 0;

    /* 1. Ascend by 2: 100 -> 98. */
    scenario(0, 0, 100);
    /* 2. Descend by 2: 100 -> 102. */
    scenario(1, 1, 100);
    /* 3. Ascend by 0: 50 -> 50 (no-op). */
    scenario(2, 4, 50);
    /* 4. Descend by 0: 50 -> 50 (no-op). */
    scenario(3, 5, 50);
    /* 5. Ascend by 127: 200 -> 73. */
    scenario(4, 6, 200);
    /* 6. Ascend by 1 on the last slot: 10 -> 9. */
    scenario(19, 2, 10);
    /* 7. Descend by 1 with wrap: 255 -> 0. */
    scenario(10, 3, 255);
    /* 8. Ascend by 2 with wrap: 0 -> 254. */
    scenario(5, 0, 0);

    return (int)log_idx;
}
"""


# Each row is [pre, post]; flattened in scenario order.
def _expected() -> bytes:
    rows = [
        (100, 98),
        (100, 102),
        (50, 50),
        (50, 50),
        (200, 73),
        (10, 9),
        (255, 0),
        (0, 254),
    ]
    out = bytearray()
    for pre, post in rows:
        out.append(pre)
        out.append(post)
    return bytes(out)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not on PATH")
class TestApplyBobbleSim(unittest.TestCase):
    """Differential opt vs unopt check on `apply_bobble`.

    Both pipelines must produce the same `result_log` bytes and the
    same return value (= log_idx after the last scenario)."""

    def _run(self, optimize: bool):
        sim = build_sim(_PROGRAM, optimize=optimize)
        result = sim.run(max_cycles=2_000_000)
        self.assertFalse(
            result.timed_out,
            f"apply_bobble sim timed out (optimize={optimize})",
        )
        log_addr = sim.symbols["result_log"]
        log_bytes = bytes(result.memory[log_addr:log_addr + 2 * 8])
        return result, log_bytes

    def test_unoptimized_matches_expected(self):
        result, log = self._run(optimize=False)
        self.assertEqual(
            result.return_int() & 0xFFFF, 2 * 8,
            "log_idx should reflect 8 recorded scenarios * 2 bytes",
        )
        self.assertEqual(log, _expected())

    def test_optimized_matches_expected(self):
        result, log = self._run(optimize=True)
        self.assertEqual(result.return_int() & 0xFFFF, 2 * 8)
        self.assertEqual(log, _expected())

    def test_opt_and_unopt_agree(self):
        unopt_result, unopt_log = self._run(optimize=False)
        opt_result, opt_log = self._run(optimize=True)
        self.assertEqual(
            unopt_log, opt_log,
            "optimizer changed observable apply_bobble state",
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
