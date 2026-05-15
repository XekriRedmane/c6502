"""Regression tests for HwReg pinning correctness in the asm-SSA
regalloc.

When the asm-level optimizer pins a Pseudo P to `Reg(X)` / `Reg(Y)`,
P's value must remain in that register across P's entire live range.
The hwreg-eligibility pass (`passes.optimization_asm.hwreg_eligibility`)
checks the per-operand-position SHAPE of every use/def of P, but it
doesn't run a liveness check against OTHER instructions in P's live
range that write to the candidate hardware register. Those clobbers
fall into two categories:

  1. **Explicit** — atoms whose dst is the same `Reg(X)` / `Reg(Y)`,
     e.g. `Mov(_, Reg(Y))` (TAY / LDY) emitted by
     `tac_to_asm._translate_indirect_indexed_store` to stage the
     index into Y for the `STA (DPTR),Y`.
  2. **Implicit** — atoms whose operand is `Frame` / `Stack` /
     `Indirect` / `IndirectY`, all of which the emitter expands with
     an `LDY #off` setup. At asm-SSA time the dependence on Y is
     hidden inside the operand, so it doesn't appear as an explicit
     write atom.

These tests target case (1) — the explicit-write category — via the
pattern that surfaced in `examples/draw_sprite_opaque.c`: a Pseudo
`y` is pinned to `Reg(Y)`, and an indirect-Y store inside `y`'s
live range overwrites Y with the store's column index. The
subsequent `LDA (ptr),Y` then reads the wrong byte.

Each test compiles a small C program twice (optimize=False,
optimize=True), runs both in the asm simulator, and asserts BOTH
runs return the expected value. Unopt is the oracle; opt is the
mode where the bug fires."""

from __future__ import annotations

import shutil
import unittest

from sim.harness import build_sim


_MAX_CYCLES = 400_000


def _run(source: str, *, optimize: bool) -> int:
    sim = build_sim(source, optimize=optimize)
    result = sim.run(max_cycles=_MAX_CYCLES)
    if result.timed_out:
        raise AssertionError(
            f"simulator timed out after {result.cycles} cycles "
            f"(optimize={optimize})"
        )
    return result.return_int()


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestHwRegPinClobberAcrossLiveRange(unittest.TestCase):
    """Each test verifies the optimizer doesn't pin a value to a
    hardware register whose value is overwritten by another
    instruction inside the pinned name's live range."""

    def test_indirect_store_clobbers_y_resident_iv(self) -> None:
        # Minimum case from the draw_sprite_opaque investigation
        # (2026-05-14). `y` is incremented across nested loops, so
        # the optimizer pins it to Reg(Y). Inside the inner loop's
        # body, `base[x] = p[y]` lowers to an IndirectIndexedStore
        # whose lowering writes Y (it stages `x` into Y for the
        # `STA (DPTR),Y`). After that store, the *next* `p[y]` read
        # uses the stale Y (= x), not the live `y`.
        #
        # Expected return: sum of in_buf[0..15] = 1+2+...+16 = 136.
        # Before the fix, the optimized run returned 90 (the inner
        # `p[y]` re-read after the store picked up `p[x]` instead).
        source = """
        #include <stdint.h>
        static uint8_t out_buf[16];
        int main(void) {
            static const uint8_t in_buf[16] = {
                1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
            };
            const uint8_t *p = in_buf;
            uint8_t y = 0;
            uint8_t sum = 0;
            for (uint8_t h = 2; h != 0; h--) {
                uint8_t *base = out_buf;
                for (uint8_t r = 8, x = 0;
                     (r & 0x80) == 0;
                     r--, x++, y++) {
                    if (x < 8) {
                        base[x] = p[y];
                    }
                    sum = sum + p[y];
                }
            }
            return sum;
        }
        """
        unopt = _run(source, optimize=False)
        opt = _run(source, optimize=True)
        self.assertEqual(unopt, 136, "oracle (unopt) sanity check")
        self.assertEqual(
            opt, 136,
            "optimized run returned the wrong value — Reg(Y) was "
            "pinned to a Pseudo whose live range crosses an "
            "IndirectIndexedStore (which writes Reg(Y) as part of "
            "its lowering)",
        )


if __name__ == "__main__":
    unittest.main()
