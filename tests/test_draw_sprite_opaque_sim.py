"""End-to-end simulator test for examples/draw_sprite_opaque.c —
specifically that `page_flag & 0x80` correctly switches the
row-base lookup between `screen_row_addr_hi` (page 1) and
`screen_row_addr_hi2` (page 2).

Compiles the real `examples/draw_sprite_opaque.c` (not a stub),
calls it once with `page_flag = 0x00` and once with `page_flag =
0x80`, and verifies the sprite byte landed at the page-1 address
for the first call and the page-2 address for the second. Catches
the regression where the optimizer dropped the conditional table
swap and always used `screen_row_addr_hi`.

For sprite_y=0:
  screen_row_addr_hi[0]  = 0x20 -> page-1 row base = $2000
  screen_row_addr_hi2[0] = 0x40 -> page-2 row base = $4000

We write a uniquely-identifying byte (`0xA5`) through the sprite
at offset 0, so the byte should land at $2000 (page_flag=0) or
$4000 (page_flag=0x80)."""

import shutil
import os
import unittest

from sim.harness import build_sim


def _source() -> str:
    here = os.path.dirname(__file__)
    path = os.path.join(here, "..", "examples", "draw_sprite_opaque.c")
    with open(path) as f:
        body = f.read()
    # Append a main() that exercises both page_flag values into
    # disjoint screen regions, then encodes the observed bytes
    # into the return value so the test can read them without
    # poking sim memory directly.
    main = """
uint8_t tile_data[1] = { 0xA5 };

int main(void) {
    /* Clear both candidate target bytes so we can tell which was
       written. */
    *(volatile uint8_t *)0x2000 = 0x00;
    *(volatile uint8_t *)0x4000 = 0x00;
    /* page_flag=0x00 -> should write to $2000 (page 1). */
    draw_sprite_opaque(1, 1, 0, 0, tile_data, 0x00);
    uint8_t page1 = *(volatile uint8_t *)0x2000;
    /* Reset and try with page_flag=0x80 -> should write to $4000
       (page 2). */
    *(volatile uint8_t *)0x2000 = 0x00;
    *(volatile uint8_t *)0x4000 = 0x00;
    draw_sprite_opaque(1, 1, 0, 0, tile_data, 0x80);
    uint8_t page2 = *(volatile uint8_t *)0x4000;
    /* Pack into the return value: low byte = page1, high byte =
       page2. The test expects 0xA5A5 (both wrote correctly). */
    return ((int)page2 << 8) | page1;
}
"""
    return body + main


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not on PATH")
class TestDrawSpriteOpaquePageFlag(unittest.TestCase):
    def _run(self, optimize: bool) -> int:
        sim = build_sim(_source(), optimize=optimize)
        result = sim.run(max_cycles=2_000_000)
        self.assertFalse(result.timed_out, "draw_sprite_opaque didn't terminate")
        return result.return_int() & 0xFFFF

    def test_unoptimized_writes_correct_pages(self):
        """page_flag=0x00 writes to $2000; page_flag=0x80 writes to $4000."""
        self.assertEqual(self._run(optimize=False), 0xA5A5)

    def test_optimized_writes_correct_pages(self):
        """Same, with --optimize."""
        self.assertEqual(self._run(optimize=True), 0xA5A5)


if __name__ == "__main__":
    unittest.main()
