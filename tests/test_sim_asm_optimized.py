"""End-to-end smoke test for the optimizer pipeline: run a curated
subset of chapter programs through the asm simulator with
`optimize=True`, asserting the same expected return values as the
unoptimized asm sim. Catches any optimizer bug that produces
compiling-but-semantically-wrong code (e.g. two interfering values
sharing a byte, a colored value clobbering one that's still live,
a folded comparison flipping its sense).

Subset rationale: we don't run every chapter file because some hit
known unrelated issues (frame_too_large for >253-byte frames,
extern_unresolved for stdio calls, wrong_value for pre-existing
codegen bugs). The subset here is the chapter_5..12 files that
already pass the asm sim WITHOUT optimization — a passing sim there
confirms the optimizer preserves semantics on programs the rest of
the pipeline already handles.

Files in chapter_19's `optimization/` tree are explicitly relevant
— they exercise the optimizer pipeline most directly — but they're
driven through the chapter_19 harness's own simulator setup; we
don't duplicate that here.
"""
from __future__ import annotations

import shutil
import unittest
from pathlib import Path

from sim.harness import build_sim
from tests.test_sim_asm import (
    SKIPS,
    _DEFAULT_MAX_CYCLES,
    _expected_returns_in_int_range,
    _signed_int,
)


_TESTS_DIR = Path(__file__).parent


# Chapter prefixes to exercise. 1-10 cover control flow,
# arithmetic, bitwise ops, conditionals, loops, and basic
# functions — the core SSA-promotable patterns regalloc operates
# on. 11-12 add type widening (long, unsigned). Higher chapters
# pull in heavier features (chars, structs, FP) where the runtime
# helpers are still WIP and would skew failure attribution.
_CHAPTERS_UNDER_TEST = tuple(
    f"chapter_{n}/" for n in (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12)
)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestAsmSimChaptersOptimized(unittest.TestCase):
    """Same expected-return assertions as `test_sim_asm.py`, but
    the pipeline runs with `optimize=True`. Per-file `subTest` so
    one regression doesn't mask others."""

    def test_optimized_expected_returns(self) -> None:
        cases = {
            path: expected
            for path, expected in _expected_returns_in_int_range().items()
            if path.startswith(_CHAPTERS_UNDER_TEST)
        }
        self.assertGreater(len(cases), 0, "no chapters in scope")
        for rel_path, expected in cases.items():
            with self.subTest(file=rel_path):
                if rel_path in SKIPS:
                    self.skipTest(SKIPS[rel_path])
                source = (_TESTS_DIR / rel_path).read_text()
                sim = build_sim(source, optimize=True)
                result = sim.run(max_cycles=_DEFAULT_MAX_CYCLES)
                if result.timed_out:
                    self.fail(
                        f"{rel_path}: simulator timed out after "
                        f"{result.cycles} cycles"
                    )
                got = _signed_int(result.return_int())
                self.assertEqual(
                    got, expected,
                    msg=(
                        f"{rel_path}: expected return {expected}, "
                        f"got {got} (HARGS={result.return_int():04X}, "
                        f"A=${result.a:02X}, X=${result.x:02X}, "
                        f"cycles={result.cycles})"
                    ),
                )


if __name__ == "__main__":
    unittest.main()
