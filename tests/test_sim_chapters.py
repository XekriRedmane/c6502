"""Run chapter test programs through the TAC simulator and assert
their expected `main` return values.

The chapter_<N> harnesses verify that programs reach codegen, but
they don't check that the produced TAC computes the right answer.
This file fills that gap: a hand-curated table maps each chapter
file we care about to its expected `main()` return, and the
simulator runs the program end-to-end (parse → resolve → string
lift → label resolve → loop label → type-check → c99_to_tac →
simulator) to verify.

Values are pinned to c6502's semantics, not "real" C. In particular
`int` is 1 byte signed (-128..127), so any program whose return
value exceeds 127 needs a wider return type to be representable
here. Listed files have all been hand-traced.

To add a file: trace its `main()` return value under c6502's
narrower type model, add the (relative-path, value) entry, and
run the suite. If the simulator disagrees, either the trace is
off or there's a real pipeline bug.

Files in chapter_<N>/valid/ that depend on stdio (putchar/printf),
multi-TU resolution (.s sidecars), or other c6502 unknowns are
listed in SKIPPED with a one-line reason — keeps the rejection
explicit so we don't silently lose coverage.
"""
from __future__ import annotations

import shutil
import unittest
from pathlib import Path

from c99_to_tac import translate_program as translate_to_tac
from parser import parse
from passes.identifier_resolution import resolve_program as resolve_identifiers
from passes.label_resolution import resolve_program as resolve_labels
from passes.loop_labeling import label_program as label_loops
from passes.string_lifting import lift_program as lift_strings
from passes.type_checking import check_program as type_check_program
from preprocessor import preprocess
from tac_sim import Simulator


_TESTS_DIR = Path(__file__).parent


# Hard-coded expected `main()` return values, hand-traced for each
# program under c6502's narrower type model (int = 1 byte signed).
EXPECTED_RETURNS: dict[str, int] = {
    # --- chapter 10: storage classes, file-scope variables, linkage
    "chapter_10/valid/distinct_local_and_extern.c": 7,
    "chapter_10/valid/extern_block_scope_variable.c": 3,
    "chapter_10/valid/multiple_static_file_scope_vars.c": 4,
    "chapter_10/valid/multiple_static_local.c": 29,
    "chapter_10/valid/shadow_static_local_var.c": 0,
    "chapter_10/valid/static_local_uninitialized.c": 4,
    "chapter_10/valid/static_then_extern.c": 3,
    "chapter_10/valid/static_variables_in_expressions.c": 0,
    "chapter_10/valid/tentative_definition.c": 5,
    "chapter_10/valid/type_before_storage_class.c": 7,

    # --- chapter 10 extra_credit
    "chapter_10/valid/extra_credit/bitwise_ops_file_scope_vars.c": 0,
    "chapter_10/valid/extra_credit/compound_assignment_static_var.c": 0,
    "chapter_10/valid/extra_credit/goto_skip_static_initializer.c": 10,
    "chapter_10/valid/extra_credit/increment_global_vars.c": 0,
    "chapter_10/valid/extra_credit/label_file_scope_var_same_name.c": 0,
    "chapter_10/valid/extra_credit/label_static_var_same_name.c": 5,
    "chapter_10/valid/extra_credit/switch_on_extern.c": 0,
    "chapter_10/valid/extra_credit/switch_skip_extern_decl.c": 0,
    "chapter_10/valid/extra_credit/switch_skip_static_initializer.c": 10,
}


# Files we deliberately don't attempt — each needs something c6502
# doesn't model. Listed so additions to chapter_<N>/valid/ surface
# as deliberate decisions rather than silent gaps.
SKIPPED: dict[str, str] = {
    "chapter_10/valid/push_arg_on_page_boundary.c":
        "depends on an extern int defined in a .s sidecar",
    "chapter_10/valid/static_local_multiple_scopes.c":
        "depends on putchar (no libc)",
    "chapter_10/valid/static_recursive_call.c":
        "depends on putchar (no libc)",
}


def _run_program(source: str) -> int | None:
    preprocessed = preprocess(source)
    resolved = label_loops(resolve_labels(lift_strings(
        resolve_identifiers(parse(preprocessed)),
    )))
    prog, symbols, types = type_check_program(resolved)
    tac = translate_to_tac(prog, symbols, types)
    sim = Simulator(tac, symbols, types)
    return sim.call("main", [])


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestSimChapters(unittest.TestCase):
    """Per-file subTest so a regression in one program doesn't mask
    others — the failure summary lists each file by relative path."""

    def test_expected_returns(self):
        for rel_path, expected in EXPECTED_RETURNS.items():
            with self.subTest(file=rel_path):
                src = (_TESTS_DIR / rel_path).read_text()
                result = _run_program(src)
                self.assertEqual(
                    result, expected,
                    msg=f"{rel_path}: expected {expected}, got {result}",
                )


if __name__ == "__main__":
    unittest.main()
