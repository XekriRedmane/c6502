"""End-to-end simulator test for refresh_hit_entities under both
the unoptimized and optimized pipelines, including the loop-counter-
to-X promotion + Y-pivot + cross-Call save/restore composition.

Verifies functional equivalence: both pipelines must produce the
same `call_count` value, demonstrating that the optimizer pipeline
correctly preserves the loop semantics."""

import shutil
import unittest

from sim.harness import build_sim


# The refresh_hit_entities body matches `examples/refresh_hit_entities.c`,
# inlined here together with a stub draw_sprite_opaque so the test is
# self-contained.
_PROGRAM = """
#include <stdint.h>

uint8_t entity_hit_y[12];
uint8_t entity_hit_row[12];
uint8_t entity_hit_state[12];

const uint8_t hit_spr_pos_lo[7] = { 0xC0, 0xE8, 0x10, 0x38, 0x60, 0x88, 0xB0 };
const uint8_t hit_spr_pos_hi[7] = { 0x8A, 0x8A, 0x8B, 0x8B, 0x8B, 0x8B, 0x8B };
const uint8_t hit_spr_neg_lo[7] = { 0xD4, 0xFC, 0x24, 0x4C, 0x74, 0x9C, 0xC4 };
const uint8_t hit_spr_neg_hi[7] = { 0x7A, 0x7A, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B };

uint8_t call_count;
uint8_t last_sprite_x;

__attribute__((zp_abi))
void draw_sprite_opaque(uint8_t width, uint8_t height,
                        uint8_t sprite_x, uint8_t sprite_y,
                        const uint8_t *tile_src) {
    call_count = (uint8_t)(call_count + 1);
    last_sprite_x = sprite_x;
}

__attribute__((zp_abi))
void refresh_hit_entities(uint8_t hit_max, uint8_t player_y, uint8_t sprite_xref) {
    uint8_t x = hit_max;
    do {
        uint8_t hy = entity_hit_y[x];
        if (hy >= player_y) {
            uint8_t delta = (uint8_t)(hy - player_y);
            if (delta < 0x2F) {
                uint8_t lo;
                uint8_t hi;
                if (entity_hit_state[x] & 0x80) {
                    hi = hit_spr_neg_hi[sprite_xref];
                    lo = hit_spr_neg_lo[sprite_xref];
                } else {
                    hi = hit_spr_pos_hi[sprite_xref];
                    lo = hit_spr_pos_lo[sprite_xref];
                }
                const uint8_t *src = (const uint8_t *)(((uint16_t)hi << 8) | lo);
                draw_sprite_opaque(0x07, 0x05, delta, entity_hit_row[x], src);
            }
        }
        x = (uint8_t)(x - 1);
    } while ((x & 0x80) == 0);
}

int main(void) {
    for (uint8_t i = 0; i < 12; i = (uint8_t)(i + 1)) {
        entity_hit_y[i] = 60;
        entity_hit_row[i] = (uint8_t)(100 + i);
        entity_hit_state[i] = (i & 1) ? 0x80 : 0x00;
    }
    call_count = 0;
    refresh_hit_entities(11, 50, 3);
    return call_count;
}
"""


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not on PATH")
class TestRefreshHitEntitiesSim(unittest.TestCase):
    """All 12 slots in the test setup are visible (delta = 10 <
    0x2F), so refresh_hit_entities must call draw_sprite_opaque
    exactly 12 times. The optimized pipeline promotes the loop
    counter to X with Y-pivot and STX/LDX save/restore around the
    JSR — verifying that 12 calls land confirms the promotion's
    correctness."""

    def _calls(self, optimize: bool) -> int:
        sim = build_sim(_PROGRAM, optimize=optimize)
        result = sim.run(max_cycles=500_000)
        self.assertFalse(
            result.timed_out,
            "refresh_hit_entities didn't terminate — likely the "
            "loop counter wasn't preserved across the JSR",
        )
        return result.return_int() & 0xFFFF

    def test_unoptimized_produces_12_calls(self):
        self.assertEqual(self._calls(optimize=False), 12)

    def test_optimized_produces_12_calls(self):
        self.assertEqual(self._calls(optimize=True), 12)

    def test_optimized_is_faster(self):
        unopt = build_sim(_PROGRAM, optimize=False).run(max_cycles=500_000)
        opt = build_sim(_PROGRAM, optimize=True).run(max_cycles=500_000)
        # Pre-existing baseline (loop-counter promotion + Y-pivot):
        # unopt ~25k cycles, opt ~4k cycles. Generous 6x threshold
        # to allow optimization regressions without flaking on minor
        # cycle-count changes.
        self.assertLess(
            opt.cycles * 6, unopt.cycles,
            f"optimizer regression: unopt={unopt.cycles}, "
            f"opt={opt.cycles}",
        )


if __name__ == "__main__":
    unittest.main()
