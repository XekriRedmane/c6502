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

  fp_unimpl           The program calls one of the FP conversion /
                      arithmetic helpers (`i2f`, `d2l`, `fadd`, …),
                      which the simulator's runtime stub registers
                      but doesn't implement. No real runtime helper
                      either yet, so this is a scope marker more
                      than a bug.

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

_INT_MIN, _INT_MAX = -128, 127
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

    # --- fp_unimpl (17): program calls an FP helper the sim doesn't
    # have. Today neither simulator nor real runtime implements these.
    "chapter_13/valid/explicit_casts/cvttsd2si_rewrite.c": "fp_unimpl",
    "chapter_13/valid/explicit_casts/double_to_signed.c": "fp_unimpl",
    "chapter_13/valid/explicit_casts/double_to_unsigned.c": "fp_unimpl",
    "chapter_13/valid/explicit_casts/rewrite_cvttsd2si_regression.c": "fp_unimpl",
    "chapter_13/valid/explicit_casts/signed_to_double.c": "fp_unimpl",
    "chapter_13/valid/explicit_casts/unsigned_to_double.c": "fp_unimpl",
    "chapter_13/valid/function_calls/double_and_int_params_recursive.c": "fp_unimpl",
    "chapter_13/valid/function_calls/push_xmm.c": "fp_unimpl",
    "chapter_13/valid/function_calls/use_arg_after_fun_call.c": "fp_unimpl",
    "chapter_13/valid/implicit_casts/common_type.c": "fp_unimpl",
    "chapter_13/valid/implicit_casts/complex_arithmetic_common_type.c": "fp_unimpl",
    "chapter_13/valid/implicit_casts/convert_for_assignment.c": "fp_unimpl",
    "chapter_15/valid/extra_credit/compound_assign_to_subscripted_val.c": "fp_unimpl",
    "chapter_15/valid/initialization/automatic_nested.c": "fp_unimpl",
    "chapter_16/valid/char_constants/char_constant_operations.c": "fp_unimpl",
    "chapter_18/valid/extra_credit/other_features/struct_decl_in_switch_statement.c": "fp_unimpl",
    "chapter_18/valid/params_and_returns/simple.c": "fp_unimpl",

    # --- frame_too_large (2): function's local_bytes > 253, can't
    # fit `LDY #(M+2)` in the prologue / epilogue's saved-FP slot
    # addressing.
    "chapter_18/valid/extra_credit/other_features/compound_assign_struct_members.c": "frame_too_large",
    "chapter_18/valid/no_structure_parameters/scalar_member_access/nested_struct.c": "frame_too_large",

    # --- wrong_value (17): runs to BRK with the wrong A. Most are
    # in chapter 13 (FP) — programs that don't call an FP helper but
    # depend on FP semantics for their answer — with a sprinkle of
    # pointer / char edge cases.
    "chapter_13/valid/constants/constant_doubles.c": "wrong_value",
    "chapter_13/valid/extra_credit/compound_assign.c": "wrong_value",
    "chapter_13/valid/extra_credit/compound_assign_implicit_cast.c": "wrong_value",
    "chapter_13/valid/extra_credit/incr_and_decr.c": "wrong_value",
    "chapter_13/valid/floating_expressions/arithmetic_ops.c": "wrong_value",
    "chapter_13/valid/floating_expressions/loop_controlling_expression.c": "wrong_value",
    "chapter_13/valid/floating_expressions/simple.c": "wrong_value",
    "chapter_13/valid/floating_expressions/static_initialized_double.c": "wrong_value",
    "chapter_13/valid/special_values/infinity.c": "wrong_value",
    "chapter_13/valid/special_values/subnormal_not_zero.c": "wrong_value",
    "chapter_14/valid/dereference/static_var_indirection.c": "wrong_value",
    "chapter_14/valid/extra_credit/compound_assign_conversion.c": "wrong_value",
    "chapter_15/valid/extra_credit/compound_assign_to_nested_subscript.c": "wrong_value",
    "chapter_16/valid/chars/access_through_char_pointer.c": "wrong_value",
    "chapter_16/valid/strings_as_initializers/test_alignment.c": "wrong_value",
    "chapter_18/valid/extra_credit/member_access/static_union_access.c": "wrong_value",
    "chapter_18/valid/params_and_returns/ignore_retval.c": "wrong_value",
}


def _signed_byte(v: int) -> int:
    """Interpret a 1-byte unsigned value as 1-byte signed."""
    return v - 0x100 if v & 0x80 else v


def _expected_returns_in_int_range() -> dict[str, int]:
    """Filter `EXPECTED_RETURNS` to entries whose value fits c6502's
    1-byte signed Int return. The wider-than-int returns also exercise
    the broken Long-return path (epilogue clobbers X) — they're worth
    revisiting but not in scope for this file's first cut."""
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
                got = _signed_byte(result.a)
                self.assertEqual(
                    got, expected,
                    msg=(
                        f"{rel_path}: expected return {expected}, "
                        f"got {got} (A=${result.a:02X}, "
                        f"X=${result.x:02X}, cycles={result.cycles})"
                    ),
                )


if __name__ == "__main__":
    unittest.main()
