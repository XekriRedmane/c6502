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
    # --- chapter 8: loops (for / while / do-while), break, continue, switch
    "chapter_8/valid/break.c": 1,
    "chapter_8/valid/break_immediate.c": 1,
    "chapter_8/valid/continue.c": 1,
    "chapter_8/valid/continue_empty_post.c": 30,
    "chapter_8/valid/do_while.c": 16,
    "chapter_8/valid/do_while_break_immediate.c": 10,
    "chapter_8/valid/empty_expression.c": 0,
    "chapter_8/valid/empty_loop_body.c": 45,
    # for.c, for_absent_condition.c rely on int truncation since
    # the source uses values > 127 — the result under c6502 is
    # what falls out of 1-byte signed arithmetic, not the wider
    # int answer the upstream tests assume.
    "chapter_8/valid/for.c": 0,
    "chapter_8/valid/for_absent_condition.c": 0,
    "chapter_8/valid/for_absent_post.c": 0,
    "chapter_8/valid/for_decl.c": 101,
    "chapter_8/valid/for_decl_no_init.c": 2,
    "chapter_8/valid/for_nested_shadow.c": 1,
    "chapter_8/valid/for_shadow.c": 1,
    "chapter_8/valid/multi_break.c": 1,
    "chapter_8/valid/multi_continue_same_loop.c": 1,
    # nested_break.c: ans accumulates 250, wraps to -6 in 1-byte int
    "chapter_8/valid/nested_break.c": -6,
    "chapter_8/valid/nested_continue.c": 24,
    "chapter_8/valid/nested_loop.c": 1,
    "chapter_8/valid/null_for_header.c": 4,
    "chapter_8/valid/while.c": 6,

    # --- chapter 8 extra_credit
    "chapter_8/valid/extra_credit/case_block.c": 1,
    # compound_assignment_controlling_expression.c: sum reaches 200
    # which wraps to -56 in 1-byte int, so sum==200 is false and the
    # final && check returns 0
    "chapter_8/valid/extra_credit/compound_assignment_controlling_expression.c": 0,
    "chapter_8/valid/extra_credit/compound_assignment_for_loop.c": 1,
    "chapter_8/valid/extra_credit/duffs_device.c": 1,
    "chapter_8/valid/extra_credit/goto_bypass_condition.c": 10,
    "chapter_8/valid/extra_credit/goto_bypass_init_exp.c": 1,
    "chapter_8/valid/extra_credit/goto_bypass_post_exp.c": 11,
    "chapter_8/valid/extra_credit/label_loop_body.c": 1,
    "chapter_8/valid/extra_credit/label_loops_breaks_and_continues.c": 12,
    "chapter_8/valid/extra_credit/loop_header_postfix_and_prefix.c": 1,
    "chapter_8/valid/extra_credit/loop_in_switch.c": 123,
    "chapter_8/valid/extra_credit/post_exp_incr.c": 21,
    "chapter_8/valid/extra_credit/switch.c": 3,
    "chapter_8/valid/extra_credit/switch_assign_in_condition.c": 2,
    "chapter_8/valid/extra_credit/switch_break.c": 10,
    "chapter_8/valid/extra_credit/switch_decl.c": 1,
    "chapter_8/valid/extra_credit/switch_default.c": 22,
    "chapter_8/valid/extra_credit/switch_default_fallthrough.c": 0,
    "chapter_8/valid/extra_credit/switch_default_not_last.c": 0,
    "chapter_8/valid/extra_credit/switch_default_only.c": 1,
    "chapter_8/valid/extra_credit/switch_empty.c": 12,
    "chapter_8/valid/extra_credit/switch_fallthrough.c": 6,
    "chapter_8/valid/extra_credit/switch_goto_mid_case.c": 1,
    "chapter_8/valid/extra_credit/switch_in_loop.c": 1,
    "chapter_8/valid/extra_credit/switch_nested_cases.c": 1,
    "chapter_8/valid/extra_credit/switch_nested_not_taken.c": 2,
    "chapter_8/valid/extra_credit/switch_nested_switch.c": 1,
    "chapter_8/valid/extra_credit/switch_no_case.c": 4,
    "chapter_8/valid/extra_credit/switch_not_taken.c": 1,
    "chapter_8/valid/extra_credit/switch_single_case.c": 1,
    "chapter_8/valid/extra_credit/switch_with_continue.c": 5,
    "chapter_8/valid/extra_credit/switch_with_continue_2.c": 5,

    # --- chapter 9: function definitions, calls, parameters
    "chapter_9/valid/no_arguments/forward_decl.c": 3,
    "chapter_9/valid/no_arguments/function_shadows_variable.c": 11,
    "chapter_9/valid/no_arguments/multiple_declarations.c": 3,
    "chapter_9/valid/no_arguments/no_return_value.c": 3,
    "chapter_9/valid/no_arguments/precedence.c": 0,
    "chapter_9/valid/no_arguments/use_function_in_expression.c": 21,
    "chapter_9/valid/no_arguments/variable_shadows_function.c": 7,
    "chapter_9/valid/arguments_in_registers/dont_clobber_edx.c": 1,
    "chapter_9/valid/arguments_in_registers/expression_args.c": 2,
    "chapter_9/valid/arguments_in_registers/fibonacci.c": 8,
    "chapter_9/valid/arguments_in_registers/forward_decl_multi_arg.c": 1,
    "chapter_9/valid/arguments_in_registers/param_shadows_local_var.c": 20,
    "chapter_9/valid/arguments_in_registers/parameter_shadows_function.c": 3,
    "chapter_9/valid/arguments_in_registers/parameter_shadows_own_function.c": 2,
    "chapter_9/valid/arguments_in_registers/parameters_are_preserved.c": 1,
    "chapter_9/valid/arguments_in_registers/single_arg.c": 6,
    "chapter_9/valid/stack_arguments/lots_of_arguments.c": 1,
    "chapter_9/valid/stack_arguments/test_for_memory_leaks.c": 1,

    # --- chapter 9 extra_credit
    "chapter_9/valid/extra_credit/compound_assign_function_result.c": 1,
    "chapter_9/valid/extra_credit/dont_clobber_ecx.c": 1,
    "chapter_9/valid/extra_credit/goto_label_multiple_functions.c": 5,
    "chapter_9/valid/extra_credit/goto_shared_name.c": 1,
    "chapter_9/valid/extra_credit/label_naming_scheme.c": 0,

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
    "chapter_9/valid/arguments_in_registers/hello_world.c":
        "depends on putchar (no libc)",
    "chapter_9/valid/stack_arguments/call_putchar.c":
        "depends on putchar (no libc)",
    "chapter_9/valid/stack_arguments/stack_alignment.c":
        "depends on .s sidecars (even_arguments / odd_arguments)",
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
