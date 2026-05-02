"""End-to-end harness for the 6502 simulator at the asm level.

Companion to `tests/test_sim_chapters.py` — same hand-curated expected-
return table, but execution is driven through `tac_to_asm` and the
in-process assembler (`sim.assembler` → py65 MPU). A regression in
TAC→asm or in the calling convention shows up here even when the
TAC simulator stays green.

`SKIPS` documents every chapter file we currently can't drive cleanly
through the asm sim, with one of these category strings as the reason
so future fixes can mass-flip them when a class of issue is closed:

  branch_oor          The function is large enough that some `Bxx`
                      branch's target is more than 127 bytes away;
                      the 6502's 8-bit signed displacement can't
                      reach. Real codegen / encoding issue: TAC→asm
                      should synthesize a `B!xx skip; JMP target`
                      pair when the short form won't fit.

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

  long_return         Expected return value is outside c6502's 1-byte
                      `int` range (-128..127), so `main` returns a
                      wider type. Today the asm epilogue's FP-restore
                      step does `TAX` as scratch, clobbering the
                      high byte of a 2-byte return. Real codegen
                      bug: the epilogue needs a different scratch
                      (e.g. a zero-page byte) so X survives.

  wrong_value         Program runs to BRK but the value in A doesn't
                      match. Most cases here boil down to (a) signed
                      div/mod hitting our unsigned-only divmod hook
                      (the runtime helpers don't distinguish signed
                      vs. unsigned today), (b) widening / narrowing
                      / sign-extend interactions, or (c) other
                      typing edge cases worth investigating one by
                      one. Each surfaces a real finding.

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
    # --- branch_oor (40): Bxx target > 127 bytes from the branch ---
    "chapter_8/valid/extra_credit/duffs_device.c": "branch_oor",
    "chapter_8/valid/extra_credit/switch_in_loop.c": "branch_oor",
    "chapter_8/valid/extra_credit/switch_nested_switch.c": "branch_oor",
    "chapter_8/valid/multi_continue_same_loop.c": "branch_oor",
    "chapter_8/valid/nested_break.c": "branch_oor",
    "chapter_8/valid/nested_continue.c": "branch_oor",
    "chapter_9/valid/stack_arguments/test_for_memory_leaks.c": "branch_oor",
    "chapter_11/valid/long_expressions/type_specifiers.c": "branch_oor",
    "chapter_13/valid/floating_expressions/loop_controlling_expression.c": "branch_oor",
    "chapter_13/valid/function_calls/double_and_int_params_recursive.c": "branch_oor",
    "chapter_13/valid/implicit_casts/convert_for_assignment.c": "branch_oor",
    "chapter_14/valid/extra_credit/switch_dereferenced_pointer.c": "branch_oor",
    "chapter_15/valid/declarators/array_as_argument.c": "branch_oor",
    "chapter_15/valid/declarators/equivalent_declarators.c": "branch_oor",
    "chapter_15/valid/declarators/for_loop_array.c": "branch_oor",
    "chapter_15/valid/extra_credit/compound_assign_and_increment.c": "branch_oor",
    "chapter_15/valid/extra_credit/compound_assign_to_nested_subscript.c": "branch_oor",
    "chapter_15/valid/extra_credit/postfix_prefix_precedence.c": "branch_oor",
    "chapter_15/valid/initialization/automatic.c": "branch_oor",
    "chapter_15/valid/initialization/automatic_nested.c": "branch_oor",
    "chapter_15/valid/initialization/static.c": "branch_oor",
    "chapter_15/valid/pointer_arithmetic/pointer_add.c": "branch_oor",
    "chapter_15/valid/subscripting/addition_subscript_equivalence.c": "branch_oor",
    "chapter_16/valid/chars/convert_by_assignment.c": "branch_oor",
    "chapter_16/valid/chars/partial_initialization.c": "branch_oor",
    "chapter_16/valid/chars/return_char.c": "branch_oor",
    "chapter_16/valid/strings_as_initializers/literals_and_compound_initializers.c": "branch_oor",
    "chapter_16/valid/strings_as_initializers/partial_initialize_via_string.c": "branch_oor",
    "chapter_18/valid/extra_credit/member_access/static_union_access.c": "branch_oor",
    "chapter_18/valid/extra_credit/other_features/compound_assign_struct_members.c": "branch_oor",
    "chapter_18/valid/extra_credit/other_features/struct_decl_in_switch_statement.c": "branch_oor",
    "chapter_18/valid/extra_credit/semantic_analysis/union_namespace.c": "branch_oor",
    "chapter_18/valid/no_structure_parameters/scalar_member_access/arrow.c": "branch_oor",
    "chapter_18/valid/no_structure_parameters/scalar_member_access/dot.c": "branch_oor",
    "chapter_18/valid/no_structure_parameters/scalar_member_access/nested_struct.c": "branch_oor",
    "chapter_18/valid/no_structure_parameters/semantic_analysis/namespaces.c": "branch_oor",
    "chapter_18/valid/no_structure_parameters/smoke_tests/static_vs_auto.c": "branch_oor",
    "chapter_18/valid/parameters/pass_args_on_page_boundary.c": "branch_oor",
    "chapter_18/valid/parameters/simple.c": "branch_oor",
    "chapter_18/valid/params_and_returns/simple.c": "branch_oor",

    # --- fp_unimpl (11): program calls an FP helper the sim doesn't
    # have. Today neither simulator nor real runtime implements these.
    "chapter_13/valid/explicit_casts/cvttsd2si_rewrite.c": "fp_unimpl",
    "chapter_13/valid/explicit_casts/double_to_signed.c": "fp_unimpl",
    "chapter_13/valid/explicit_casts/double_to_unsigned.c": "fp_unimpl",
    "chapter_13/valid/explicit_casts/rewrite_cvttsd2si_regression.c": "fp_unimpl",
    "chapter_13/valid/explicit_casts/signed_to_double.c": "fp_unimpl",
    "chapter_13/valid/explicit_casts/unsigned_to_double.c": "fp_unimpl",
    "chapter_13/valid/function_calls/push_xmm.c": "fp_unimpl",
    "chapter_13/valid/function_calls/use_arg_after_fun_call.c": "fp_unimpl",
    "chapter_13/valid/implicit_casts/common_type.c": "fp_unimpl",
    "chapter_13/valid/implicit_casts/complex_arithmetic_common_type.c": "fp_unimpl",
    "chapter_15/valid/extra_credit/compound_assign_to_subscripted_val.c": "fp_unimpl",

    # --- extern_unresolved (3): calls a name we don't link.
    "chapter_17/valid/sizeof/sizeof_not_evaluated.c": "extern_unresolved",
    "chapter_18/valid/params_and_returns/return_big_struct_on_page_boundary.c": "extern_unresolved",
    "chapter_18/valid/params_and_returns/return_struct_on_page_boundary.c": "extern_unresolved",

    # --- wrong_value (51): runs to BRK with the wrong A. Most are
    # signed-div/mod, sign-extend, or wider-than-int arithmetic
    # findings. Each is a real signal worth digging into.
    "chapter_3/valid/div_neg.c": "wrong_value",
    "chapter_5/valid/exp_then_declaration.c": "wrong_value",
    "chapter_8/valid/extra_credit/compound_assignment_controlling_expression.c": "wrong_value",
    "chapter_11/valid/explicit_casts/sign_extend.c": "wrong_value",
    "chapter_11/valid/extra_credit/bitwise_long_op.c": "wrong_value",
    "chapter_11/valid/extra_credit/compound_assign_to_long.c": "wrong_value",
    "chapter_11/valid/extra_credit/compound_bitwise.c": "wrong_value",
    "chapter_11/valid/implicit_casts/convert_function_arguments.c": "wrong_value",
    "chapter_11/valid/long_expressions/arithmetic_ops.c": "wrong_value",
    "chapter_12/valid/explicit_casts/chained_casts.c": "wrong_value",
    "chapter_12/valid/explicit_casts/extension.c": "wrong_value",
    "chapter_12/valid/explicit_casts/round_trip_casts.c": "wrong_value",
    "chapter_12/valid/extra_credit/bitwise_unsigned_ops.c": "wrong_value",
    "chapter_12/valid/extra_credit/compound_assign_uint.c": "wrong_value",
    "chapter_12/valid/extra_credit/compound_bitwise.c": "wrong_value",
    "chapter_12/valid/implicit_casts/common_type.c": "wrong_value",
    "chapter_12/valid/implicit_casts/convert_by_assignment.c": "wrong_value",
    "chapter_13/valid/constants/constant_doubles.c": "wrong_value",
    "chapter_13/valid/extra_credit/compound_assign.c": "wrong_value",
    "chapter_13/valid/extra_credit/compound_assign_implicit_cast.c": "wrong_value",
    "chapter_13/valid/extra_credit/incr_and_decr.c": "wrong_value",
    "chapter_13/valid/floating_expressions/arithmetic_ops.c": "wrong_value",
    "chapter_13/valid/floating_expressions/simple.c": "wrong_value",
    "chapter_13/valid/floating_expressions/static_initialized_double.c": "wrong_value",
    "chapter_13/valid/special_values/infinity.c": "wrong_value",
    "chapter_13/valid/special_values/subnormal_not_zero.c": "wrong_value",
    "chapter_14/valid/dereference/static_var_indirection.c": "wrong_value",
    "chapter_14/valid/extra_credit/bitwise_ops_with_dereferenced_ptrs.c": "wrong_value",
    "chapter_14/valid/extra_credit/compound_assign_conversion.c": "wrong_value",
    "chapter_14/valid/extra_credit/compound_bitwise_dereferenced_ptrs.c": "wrong_value",
    "chapter_15/valid/casts/implicit_and_explicit_conversions.c": "wrong_value",
    "chapter_15/valid/extra_credit/compound_assign_array_of_pointers.c": "wrong_value",
    "chapter_15/valid/extra_credit/compound_bitwise_subscript.c": "wrong_value",
    "chapter_15/valid/extra_credit/compound_lval_evaluated_once.c": "wrong_value",
    "chapter_15/valid/extra_credit/compound_nested_pointer_assignment.c": "wrong_value",
    "chapter_15/valid/extra_credit/compound_pointer_assignment.c": "wrong_value",
    "chapter_15/valid/extra_credit/incr_and_decr_nested_pointers.c": "wrong_value",
    "chapter_15/valid/extra_credit/incr_decr_subscripted_vals.c": "wrong_value",
    "chapter_15/valid/pointer_arithmetic/compare.c": "wrong_value",
    "chapter_15/valid/pointer_arithmetic/pointer_diff.c": "wrong_value",
    "chapter_15/valid/subscripting/array_of_pointers_to_arrays.c": "wrong_value",
    "chapter_15/valid/subscripting/subscript_nested.c": "wrong_value",
    "chapter_16/valid/char_constants/char_constant_operations.c": "wrong_value",
    "chapter_16/valid/chars/access_through_char_pointer.c": "wrong_value",
    "chapter_16/valid/chars/common_type.c": "wrong_value",
    "chapter_16/valid/chars/explicit_casts.c": "wrong_value",
    "chapter_16/valid/extra_credit/compound_bitwise_ops_chars.c": "wrong_value",
    "chapter_16/valid/extra_credit/incr_decr_chars.c": "wrong_value",
    "chapter_16/valid/strings_as_initializers/test_alignment.c": "wrong_value",
    "chapter_18/valid/extra_credit/union_copy/assign_to_union.c": "wrong_value",
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
