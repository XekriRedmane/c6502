"""Tests for `passes.y_peephole`.

The peephole walks emitted asm text, tracking Y's known value across
instructions, and rewrites:

  * `LDY #imm` when Y already equals imm → drop.
  * `LDY #imm` when Y == imm-1 (mod 256)  → INY.
  * `LDY #imm` when Y == imm+1 (mod 256)  → DEY.

State invalidates at labels and JSRs.
"""
from __future__ import annotations

import unittest

from passes.y_peephole import apply_y_peephole


def _ldy(imm: int) -> str:
    return f"   LDY   #${imm:02X}"


def _line(text: str) -> str:
    return "   " + text


class TestYPeephole(unittest.TestCase):
    def test_drop_redundant_ldy(self):
        # Same-value LDY back-to-back: the second is redundant.
        out = apply_y_peephole([
            _ldy(0),
            _line("LDA   (DPTR),Y"),
            _ldy(0),
            _line("LDA   (DPTR),Y"),
        ])
        # Only one LDY in the result.
        ldys = [line for line in out if "LDY" in line]
        self.assertEqual(len(ldys), 1)

    def test_replace_with_iny(self):
        # Y goes from 0 to 1 — replace with INY.
        out = apply_y_peephole([
            _ldy(0),
            _line("LDA   (DPTR),Y"),
            _ldy(1),
            _line("LDA   (DPTR),Y"),
        ])
        self.assertIn("   INY", out)
        self.assertNotIn("   LDY   #$01", out)

    def test_replace_with_dey(self):
        # Y goes from 1 to 0.
        out = apply_y_peephole([
            _ldy(1),
            _line("LDA   (DPTR),Y"),
            _ldy(0),
            _line("LDA   (DPTR),Y"),
        ])
        self.assertIn("   DEY", out)
        # The LDY #$00 is the second occurrence — should be replaced.
        self.assertEqual(
            sum(1 for line in out if "LDY   #$00" in line), 0,
        )

    def test_label_resets_state(self):
        # After a label, Y is unknown — second LDY must stay.
        out = apply_y_peephole([
            _ldy(0),
            _line("LDA   (DPTR),Y"),
            ".loop:",
            _ldy(0),
            _line("LDA   (DPTR),Y"),
        ])
        ldys = [line for line in out if "LDY" in line]
        self.assertEqual(len(ldys), 2)

    def test_jsr_resets_state(self):
        # JSR may clobber Y — second LDY must stay.
        out = apply_y_peephole([
            _ldy(0),
            _line("JSR   somefn"),
            _ldy(0),
        ])
        ldys = [line for line in out if "LDY" in line]
        self.assertEqual(len(ldys), 2)

    def test_iny_dey_update_tracker(self):
        # After INY the tracker should know Y = 1, so a subsequent
        # `LDY #$01` is redundant.
        out = apply_y_peephole([
            _ldy(0),
            _line("LDA   (DPTR),Y"),
            "   INY",
            _line("LDA   (DPTR),Y"),
            _ldy(1),  # redundant — Y is already 1
            _line("LDA   (DPTR),Y"),
        ])
        # First LDY stays; INY stays; the redundant LDY #$01 is dropped.
        self.assertEqual(
            sum(1 for line in out if "LDY" in line and "#" in line),
            1,
        )

    def test_tay_invalidates(self):
        # TAY moves A → Y; Y's value is now unknown.
        out = apply_y_peephole([
            _ldy(0),
            "   TAY",
            _ldy(0),  # not redundant — Y was clobbered.
        ])
        self.assertEqual(
            sum(1 for line in out if "LDY   #$00" in line), 2,
        )

    def test_blank_and_comment_lines_pass_through(self):
        out = apply_y_peephole([
            "",
            "   ; a comment",
            _ldy(0),
            _line("LDA   (DPTR),Y"),
            "",
            "   ; another comment",
            _ldy(0),  # still 0 — comments and blanks don't reset.
        ])
        ldys = [line for line in out if "LDY" in line]
        self.assertEqual(len(ldys), 1)

    def test_iny_underflow_unknown(self):
        # INY when Y was unknown leaves Y unknown — subsequent LDY
        # should not be optimized away.
        out = apply_y_peephole([
            "   INY",
            _ldy(5),
        ])
        self.assertIn("   LDY   #$05", out)

    def test_modulo_arithmetic(self):
        # Y wraps mod 256: LDY #$00 followed by LDY #$FF is INY-only
        # candidate (00 → FF differs by -1) → DEY.
        out = apply_y_peephole([
            _ldy(0x00),
            _ldy(0xFF),
        ])
        self.assertIn("   DEY", out)


if __name__ == "__main__":
    unittest.main()
