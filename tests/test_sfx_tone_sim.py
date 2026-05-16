"""End-to-end simulator test for `sfx_tone` under both the
unoptimized and optimized pipelines.

The function's observable effects are entirely volatile-driven:
  * Each outer iteration reads `*sfx_click_ptr` once (the Apple II
    speaker-toggle soft switch in the original setup).
  * The inner `while (--y != 0) {}` busy-wait sets the click period.

Neither produces directly-observable memory state in a sim that
doesn't model the speaker hardware — but we can still verify the
function TERMINATES correctly and returns control to the caller in
both pipelines by driving it with a battery of scenarios from a
`main()` that records sentinels between calls. If the optimizer
accidentally elided the volatile reads or the decrement loop, the
function would either return too fast (cycle ratio check) or fail
to write sentinels in order (sentinel log check).

Specifically:
  * Five scenarios cover small-pitch / small-duration combos plus
    the `pitch=0` 256-iter wrap-around case.
  * Between each `sfx_tone` call, `main` writes a unique sentinel
    byte to `result_log` — if all five appear in the right order,
    the function terminated correctly five times.
  * A separate cycle-ratio assertion catches the case where the
    optimizer drops the inner volatile loop entirely (an elided
    loop would shorten total cycles by orders of magnitude).

`sfx_click_ptr` is declared `extern` to mirror the example source's
shape (the speaker pointer is normally provided by the runtime),
then defined locally to point at a backing byte. The volatile bit
on the pointee is the gate that keeps every read in the asm output.
"""

import shutil
import unittest

import sim.runtime as rt_mod
from sim.harness import build_sim


# Inlined source of examples/sfx_tone.c plus a `main` driver that
# exercises a sequence of (pitch, duration) scenarios and records
# a sentinel byte after each `sfx_tone` returns.
_PROGRAM = r"""
#include <stdint.h>

/* Same shape as examples/sfx_tone.c: the speaker pointer is
 * declared extern (the runtime provides storage in the real
 * setup). The test provides storage just below. */
extern const volatile uint8_t *sfx_click_ptr;

__attribute__((zp_abi))
void sfx_tone(uint8_t pitch, uint8_t duration);

/* Backing byte the speaker pointer points at. The volatile reads
 * go through `sfx_click_ptr` to this cell; the cell's value never
 * changes during the test (no hardware-side write), so the reads
 * have no observable memory effect — they're observable only as
 * the cycles they consume. */
uint8_t click_target;
const volatile uint8_t *sfx_click_ptr = &click_target;

/* Sentinel log: main writes one byte after each scenario's
 * sfx_tone returns. If sfx_tone ever fails to return (infinite
 * loop) or corrupts the caller's state, the sentinels won't all
 * land in result_log in the expected order. */
uint8_t result_log[16];
uint8_t log_idx;

void record_sentinel(uint8_t v) {
    result_log[log_idx] = v;
    log_idx = (uint8_t)(log_idx + 1);
}

int main(void) {
    log_idx = 0;
    click_target = 0x42;

    /* Scenario 1: pitch=1, duration=1.
     *   `volatile uint8_t y = 1; while (--y != 0) {}` — --y goes
     *   from 1 to 0, loop exits without entering body. Inner: 0
     *   iters. Outer: 1 iter -> 1 speaker read. */
    sfx_tone(1, 1);
    record_sentinel(0x11);

    /* Scenario 2: pitch=2, duration=1.
     *   Inner: 1 iter (--y from 2 to 1, body, then 1 to 0, exit).
     *   Outer: 1 iter. */
    sfx_tone(2, 1);
    record_sentinel(0x22);

    /* Scenario 3: pitch=1, duration=3.
     *   Inner: 0 iters per outer. Outer: 3 iters -> 3 reads. */
    sfx_tone(1, 3);
    record_sentinel(0x33);

    /* Scenario 4: pitch=5, duration=2.
     *   Inner: 4 iters per outer. Outer: 2 iters. */
    sfx_tone(5, 2);
    record_sentinel(0x44);

    /* Scenario 5: pitch=0, duration=1.
     *   --y wraps: starts at 0, first decrement yields $FF, loop
     *   runs 255 iters total. Outer: 1 iter. Tests the wrap-around
     *   case the example's docstring calls out. */
    sfx_tone(0, 1);
    record_sentinel(0x55);

    return (int)log_idx;
}

/* sfx_tone definition — verbatim from examples/sfx_tone.c. */
__attribute__((zp_abi))
void sfx_tone(uint8_t pitch, uint8_t duration)
{
    do {
        volatile uint8_t y = pitch;
        while (--y != 0) { }       /* inner delay loop */
        (void)*sfx_click_ptr;      /* volatile speaker read */
    } while (--duration != 0);
}
"""


# Expected log: five distinct sentinels written in sequence, one
# per completed scenario. If any scenario hangs or corrupts main's
# state, this won't match.
_EXPECTED_LOG = bytes([0x11, 0x22, 0x33, 0x44, 0x55])
_EXPECTED_RET = 5  # log_idx after the fifth record_sentinel call


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not on PATH")
class TestSfxToneSim(unittest.TestCase):
    """Differential opt vs unopt check on `sfx_tone`.

    Both pipelines must (a) terminate within the cycle budget,
    (b) write the same sentinel sequence to `result_log`, and
    (c) return the same `log_idx`."""

    def _run(self, optimize: bool):
        sim = build_sim(_PROGRAM, optimize=optimize)
        # Worst-case inner iters: scenario 5 alone is 255. Plus
        # the smaller scenarios sum to under 20 more iters. Each
        # inner iter is a volatile LDA-SBC-STA chain (~15 cycles
        # unopt, ~10 opt). Speaker reads add ~30 cycles per outer.
        # Total well under 50k cycles; 200k is a generous ceiling
        # tight enough to catch an unexpected infinite loop.
        result = sim.run(max_cycles=200_000)
        self.assertFalse(
            result.timed_out,
            f"sfx_tone sim timed out (optimize={optimize}) — "
            f"likely an unintended infinite loop in the optimized "
            f"output.",
        )
        log_addr = sim.symbols["result_log"]
        log_bytes = bytes(
            result.memory[log_addr:log_addr + len(_EXPECTED_LOG)]
        )
        return result, log_bytes

    def test_unoptimized_matches_expected(self):
        result, log = self._run(optimize=False)
        self.assertEqual(
            result.return_int() & 0xFFFF, _EXPECTED_RET,
            "log_idx return doesn't reflect the five expected "
            "sentinels — main() may have exited early",
        )
        self.assertEqual(log, _EXPECTED_LOG)

    def test_optimized_matches_expected(self):
        result, log = self._run(optimize=True)
        self.assertEqual(result.return_int() & 0xFFFF, _EXPECTED_RET)
        self.assertEqual(log, _EXPECTED_LOG)

    def test_opt_and_unopt_agree(self):
        unopt_result, unopt_log = self._run(optimize=False)
        opt_result, opt_log = self._run(optimize=True)
        self.assertEqual(
            unopt_log, opt_log,
            "optimizer changed observable sentinel sequence — "
            "sfx_tone may have an order-of-execution divergence",
        )
        # Return-value bytes (Int return) must match.
        unopt_hargs = bytes(
            unopt_result.memory[rt_mod.HARGS:rt_mod.HARGS + 2]
        )
        opt_hargs = bytes(
            opt_result.memory[rt_mod.HARGS:rt_mod.HARGS + 2]
        )
        self.assertEqual(unopt_hargs, opt_hargs)

    def test_optimizer_does_not_skip_inner_loop(self):
        # Scenario 5 alone forces 255 inner-loop iterations. If the
        # optimizer accidentally eliminates the volatile decrement
        # loop (e.g., by treating volatile-y as dead and applying
        # dead-pure-loop elim), total cycles drop by an order of
        # magnitude — most of the work disappears.
        #
        # Absolute lower bound, not a ratio against unopt: opt is
        # legitimately ~5x faster than unopt here (no soft-stack
        # prologue, register coloring instead of frame-resident
        # locals, DEC peephole on the outer counter, etc.). The
        # bound below is well above what a hypothetical
        # "everything elided" variant would take — five sfx_tone
        # bodies that immediately RTS plus a handful of sentinel
        # writes lands around ~500 cycles — but well below the
        # ~8k cycles we actually see, so legitimate optimization
        # wins don't flake the test.
        opt_sim = build_sim(_PROGRAM, optimize=True)
        opt_result = opt_sim.run(max_cycles=200_000)
        self.assertGreater(
            opt_result.cycles, 3_000,
            f"optimized cycles ({opt_result.cycles}) suspiciously "
            f"low — did the volatile inner loop get eliminated? "
            f"Scenario 5 alone should take ~2500+ cycles on its "
            f"own inner-loop iteration count.",
        )


if __name__ == "__main__":
    unittest.main()
