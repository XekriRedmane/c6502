"""End-to-end harness for the 6502 simulator at the asm level.

Companion to `tests/test_sim_chapters.py` — same hand-curated expected-
return table, but execution is driven through `tac_to_asm` and the
in-process assembler (`sim.assembler` → py65 MPU). A regression in
TAC→asm or in the calling convention shows up here even when the
TAC simulator stays green.

`SKIPS` documents every chapter file we currently can't drive cleanly
through the asm sim, with one of these category strings as the reason
so future fixes can mass-flip them when a class of issue is closed:

  frame_too_large     The function's `local_bytes` exceeds 253 — the
                      `LDY #(M+2)` immediate that addresses the
                      saved-FP slot in the prologue / epilogue can't
                      hold a value > 255. Hits a couple of struct-
                      heavy chapter-18 tests; needs a different frame
                      layout (non-LDY-immediate addressing, or split
                      large frames into multiple sub-frames) to fix.

  (FP helpers are now implemented as Python hooks via host floats —
  the asm-sim chapter walk no longer trips over `fp_unimpl`. The
  asm versions are tracked separately as runtime work; until they
  land, the sim runs FP via the host's float unit.)

  extern_unresolved   Program calls an external symbol the simulator
                      doesn't link (libc-style `exit`, etc.). A
                      real link in c6502 doesn't exist either —
                      these would need a libc stub.

  wrong_value         Program runs to BRK but the value in A doesn't
                      match. Each one's a real finding worth
                      investigating.

The `_PASSING` set is the complement: chapter files that currently
produce the expected return through the asm simulator. The test asserts
each `_PASSING` file, and uses `subTest` so a regression in one file
doesn't mask others. `SKIPS` entries are also surfaced as `subTest`
skips so they appear in the runner output (with their category as the
reason), keeping the gap visible.
"""

from __future__ import annotations

import shutil
import unittest
from pathlib import Path

from sim.harness import build_sim

# Reuse the curated expected-return table from the TAC harness. Both
# simulators must agree on what each program is supposed to return,
# so duplicating the table would be a bug magnet.
from tests.test_sim_chapters import EXPECTED_RETURNS, SKIPPED


_TESTS_DIR = Path(__file__).parent

_INT_MIN, _INT_MAX = -32768, 32767
_DEFAULT_MAX_CYCLES = 5_000_000


# Every chapter file we currently can't drive cleanly through the asm
# simulator. See module docstring for what each category means. To
# regenerate (after a fix that's expected to flip files), run the
# script in `scripts/regen_sim_asm_skips.py` (TODO if/when that lands)
# or do an ad-hoc bisect in a REPL.
SKIPS: dict[str, str] = {
    # --- extern_unresolved (5): calls a name we don't link.
    "chapter_17/valid/sizeof/sizeof_not_evaluated.c": "extern_unresolved",
    "chapter_18/valid/no_structure_parameters/scalar_member_access/arrow.c": "extern_unresolved",
    "chapter_18/valid/parameters/pass_args_on_page_boundary.c": "extern_unresolved",
    "chapter_18/valid/params_and_returns/return_big_struct_on_page_boundary.c": "extern_unresolved",
    "chapter_18/valid/params_and_returns/return_struct_on_page_boundary.c": "extern_unresolved",

    # --- frame_too_large: function's local_bytes > 253, can't fit
    # `LDY #(M+2)` in the prologue / epilogue's saved-FP slot
    # addressing. The list grew after the C99 width refresh — Int
    # doubled from 1B to 2B and Long doubled from 2B to 4B, pushing
    # several previously-fitting frames over the limit.
    "chapter_15/valid/extra_credit/compound_bitwise_subscript.c": "frame_too_large",
    "chapter_16/valid/char_constants/char_constant_operations.c": "frame_too_large",
    "chapter_16/valid/chars/convert_by_assignment.c": "frame_too_large",
    "chapter_16/valid/chars/explicit_casts.c": "frame_too_large",
    "chapter_16/valid/extra_credit/compound_assign_chars.c": "frame_too_large",
    "chapter_18/valid/extra_credit/other_features/compound_assign_struct_members.c": "frame_too_large",
    "chapter_18/valid/no_structure_parameters/scalar_member_access/nested_struct.c": "frame_too_large",

    # --- wrong_value: asm sim disagrees with TAC sim. Each is a
    # codegen bug worth tracking down separately.
    "chapter_16/valid/extra_credit/bitshift_chars.c": "wrong_value",
    "chapter_18/valid/extra_credit/semantic_analysis/union_namespace.c": "wrong_value",
}


def _signed_int(v: int) -> int:
    """Interpret a 2-byte unsigned value as 2-byte signed."""
    return v - 0x10000 if v & 0x8000 else v


def _expected_returns_in_int_range() -> dict[str, int]:
    """Filter `EXPECTED_RETURNS` to entries whose value fits c6502's
    2-byte signed Int return (post C99 width refresh). Wider-than-
    int returns exercise the Long / LongLong return paths separately."""
    out: dict[str, int] = {}
    for path, expected in EXPECTED_RETURNS.items():
        if path in SKIPPED:
            continue
        if not (_INT_MIN <= expected <= _INT_MAX):
            continue
        out[path] = expected
    return out


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestAsmSimChapters(unittest.TestCase):
    """Per-file `subTest` so one regression doesn't mask others — the
    failure summary lists each file by relative path. SKIPS entries are
    also surfaced as subTests with `skipTest` so they show up in the
    runner output with their category as the skip reason."""

    def test_expected_returns(self) -> None:
        cases = _expected_returns_in_int_range()
        self.assertGreater(len(cases), 0, "no in-range chapter cases found")
        for rel_path, expected in cases.items():
            with self.subTest(file=rel_path):
                if rel_path in SKIPS:
                    self.skipTest(SKIPS[rel_path])
                source = (_TESTS_DIR / rel_path).read_text()
                sim = build_sim(source)
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
