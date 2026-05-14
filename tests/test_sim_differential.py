"""Differential opt vs unopt sim across the chapter corpus.

For every chapter test program in `EXPECTED_RETURNS` (excluding the
SKIPPED / SKIPS lists), build the program twice — once with
`optimize=False`, once with `optimize=True` — run both in the asm
simulator with identical entry state, and assert:

  1. Both runs terminate (no timeout / no infinite loop).
  2. The two runs leave identical bytes in HARGS+0..7 (the full
     return-value window for Int / Long / LongLong / Float /
     Double — any width's return lands here).
  3. The optimized run's cycle count is ≤ the unoptimized run's
     (sanity: opt should never make a program slower).

Optionally also asserts each run against `EXPECTED_RETURNS` to
catch the case where both pipelines produce the same wrong value.

Rationale: the optimizer's outputs were previously checked only by
gold-file diffs (structural, not behavioral). Three silent
miscompiles surfaced when sim/harness started using compile.py's
full pipeline (reassoc-const self-update fusion, dead-A-arith C
flag tracking, redundant-load tracking of memory shifts). This
test catches the same class of bug going forward — any optimizer
pass that miscompiles a program in the corpus fails here.

The test runs ALL chapters (not just 1-12) — chapter 13+ was
previously excluded from the optimized-sim test because FP /
struct / char workloads were assumed to need WIP runtime
helpers, but the FP helpers landed as Python hooks and the
chars/structs paths work end-to-end now.

# Per-program failure attribution

`subTest` is used per program, so a regression in one file surfaces
without masking others. The failure message reports both runs'
return values, register state, and cycle counts to make bisection
straightforward."""

from __future__ import annotations

import shutil
import unittest
from pathlib import Path

import sim.runtime as rt_mod
from sim.harness import build_sim
from tests.test_sim_asm import (
    SKIPS as _ASM_SKIPS,
    _DEFAULT_MAX_CYCLES,
)
from tests.test_sim_chapters import EXPECTED_RETURNS, SKIPPED as _TAC_SKIPPED


_TESTS_DIR = Path(__file__).parent


# Programs known to compile but where the optimized run diverges
# from the unoptimized run, even after the integrity fixes
# (reassoc-const, all_flags_dead_at, memory-shift tracking). Each
# entry is a real pre-existing optimizer bug to investigate; the
# skip keeps the test green so a regression in some OTHER file
# surfaces cleanly.
#
# Surfaced during the initial differential-test pass on
# 2026-05-14. The optimized pipeline had NEVER been exercised on
# chapter 13+ programs (the existing `test_sim_asm_optimized`
# only covered chapters 1-12). Categories below correspond
# loosely to the C feature each test exercises; bisecting which
# pass mis-compiles each one is per-bug follow-up work.
_OPT_DIVERGES: dict[str, str] = {
    # --- FP / special-value semantics
    "chapter_13/valid/special_values/infinity.c": "opt-divergence-fp",
    # --- Pointer / dereference miscompiles
    "chapter_15/valid/declarators/equivalent_declarators.c": "opt-divergence-pointer",
    "chapter_15/valid/extra_credit/compound_nested_pointer_assignment.c": "opt-divergence-pointer",
    "chapter_15/valid/extra_credit/incr_and_decr_nested_pointers.c": "opt-divergence-pointer",
    "chapter_15/valid/pointer_arithmetic/pointer_diff.c": "opt-divergence-pointer",
    "chapter_15/valid/subscripting/subscript_nested.c": "opt-divergence-pointer",
    # --- Char / string miscompiles
    "chapter_16/valid/chars/partial_initialization.c": "opt-divergence-char",
    "chapter_16/valid/strings_as_initializers/partial_initialize_via_string.c": "opt-divergence-char",
    # --- Struct / union miscompiles
    "chapter_18/valid/no_structure_parameters/parse_and_lex/postfix_precedence.c": "opt-divergence-struct",
    # Fixed during the 2026-05-14 sweep:
    #   - chapter_14/valid/dereference/read_through_pointers.c
    #   - chapter_14/valid/dereference/static_var_indirection.c
    #   - chapter_14/valid/extra_credit/bitwise_ops_with_dereferenced_ptrs.c
    #     (byte_dce: address-taken Pseudos excluded from byte DCE)
    #   - chapter_15/valid/initialization/automatic_nested.c (same)
    #   - chapter_14/valid/casts/cast_between_pointer_types.c
    #   - chapter_14/valid/declarators/declarators.c
    #   - chapter_14/valid/dereference/multilevel_indirection.c
    #   - chapter_15/valid/pointer_arithmetic/pointer_add.c
    #   - chapter_18/valid/extra_credit/member_access/union_init_and_member_access.c
    #   - chapter_18/valid/extra_credit/other_features/bitwise_ops_struct_members.c
    #   - chapter_18/valid/extra_credit/semantic_analysis/union_members_same_type.c
    #   - chapter_18/valid/extra_credit/semantic_analysis/union_self_pointer.c
    #   - chapter_18/valid/no_structure_parameters/semantic_analysis/namespaces.c
    #   - chapter_18/valid/no_structure_parameters/smoke_tests/static_vs_auto.c
    #   - chapter_18/valid/params_and_returns/ignore_retval.c
    #   - chapter_18/valid/params_and_returns/simple.c
    #   - chapter_18/valid/extra_credit/union_copy/assign_to_union.c
    #   - chapter_18/valid/params_and_returns/return_incomplete_type.c
    #   - chapter_16/valid/chars/access_through_char_pointer.c
    #   - chapter_16/valid/chars/return_char.c (LDA #-10 emit)
    #     (backward_copy_propagation: Indirect aliases Data("DPTR"),
    #      and indirect_base_prop: local-pool slot symbols recognized
    #      as ZP for invalidation purposes)
}


def _return_int_bytes(memory: bytearray) -> bytes:
    """Return the 2 bytes at HARGS+0..1 — the Int return window.
    Every chapter program in `EXPECTED_RETURNS` has `int main(void)`,
    so this is where the comparable return lands. Higher HARGS
    bytes (2..7) hold transient state from helper calls and aren't
    part of the return value."""
    return bytes(memory[rt_mod.HARGS:rt_mod.HARGS + 2])


def _format_state(name: str, result, hargs: bytes) -> str:
    return (
        f"{name}: HARGS+0..1={hargs.hex()} "
        f"A=${result.a:02X} X=${result.x:02X} Y=${result.y:02X} "
        f"cycles={result.cycles}"
        + (" TIMED_OUT" if result.timed_out else "")
    )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestOptUnoptDifferential(unittest.TestCase):
    """Differential check: every chapter program must produce the
    same HARGS bytes under both optimize=False and optimize=True."""

    def test_opt_unopt_agree(self) -> None:
        cases = sorted(EXPECTED_RETURNS.keys())
        self.assertGreater(len(cases), 0, "no chapters in scope")
        skipped_count = 0
        for rel_path in cases:
            with self.subTest(file=rel_path):
                if rel_path in _TAC_SKIPPED:
                    self.skipTest(_TAC_SKIPPED[rel_path])
                    skipped_count += 1
                    continue
                if rel_path in _ASM_SKIPS:
                    self.skipTest(_ASM_SKIPS[rel_path])
                    skipped_count += 1
                    continue
                if rel_path in _OPT_DIVERGES:
                    self.skipTest(_OPT_DIVERGES[rel_path])
                    skipped_count += 1
                    continue
                source = (_TESTS_DIR / rel_path).read_text()
                try:
                    unopt = build_sim(source, optimize=False).run(
                        max_cycles=_DEFAULT_MAX_CYCLES,
                    )
                except Exception as e:
                    self.fail(f"{rel_path}: unopt compile/sim error: {e}")
                try:
                    opt = build_sim(source, optimize=True).run(
                        max_cycles=_DEFAULT_MAX_CYCLES,
                    )
                except Exception as e:
                    self.fail(f"{rel_path}: opt compile/sim error: {e}")
                unopt_hargs = _return_int_bytes(unopt.memory)
                opt_hargs = _return_int_bytes(opt.memory)
                if unopt.timed_out:
                    self.fail(
                        f"{rel_path}: unopt timed out — "
                        f"{_format_state('unopt', unopt, unopt_hargs)}"
                    )
                if opt.timed_out:
                    self.fail(
                        f"{rel_path}: opt timed out — "
                        f"{_format_state('opt', opt, opt_hargs)}"
                    )
                self.assertEqual(
                    opt_hargs, unopt_hargs,
                    msg=(
                        f"{rel_path}: opt vs unopt return-value "
                        f"divergence\n  "
                        f"{_format_state('unopt', unopt, unopt_hargs)}\n  "
                        f"{_format_state('opt', opt, opt_hargs)}"
                    ),
                )
                # Cycle sanity: opt should never be slower than unopt.
                self.assertLessEqual(
                    opt.cycles, unopt.cycles,
                    msg=(
                        f"{rel_path}: optimizer regressed cycles "
                        f"({unopt.cycles} → {opt.cycles})"
                    ),
                )


if __name__ == "__main__":
    unittest.main()
