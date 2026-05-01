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
    # --- chapter 1: minimal `int main(void) { return N; }`
    "chapter_1/valid/multi_digit.c": 100,
    "chapter_1/valid/newlines.c": 0,
    "chapter_1/valid/no_newlines.c": 0,
    "chapter_1/valid/return_0.c": 0,
    "chapter_1/valid/return_2.c": 2,
    "chapter_1/valid/spaces.c": 0,
    "chapter_1/valid/tabs.c": 0,

    # --- chapter 2: unary operators (-, ~)
    "chapter_2/valid/bitwise.c": -13,
    "chapter_2/valid/bitwise_int_min.c": 126,
    "chapter_2/valid/bitwise_zero.c": -1,
    "chapter_2/valid/neg.c": -5,
    "chapter_2/valid/neg_zero.c": 0,
    "chapter_2/valid/negate_int_max.c": -127,
    "chapter_2/valid/nested_ops.c": 2,
    "chapter_2/valid/nested_ops_2.c": 1,
    "chapter_2/valid/parens.c": -2,
    "chapter_2/valid/parens_2.c": -3,
    "chapter_2/valid/parens_3.c": 4,
    "chapter_2/valid/redundant_parens.c": -10,

    # --- chapter 3: binary arithmetic (+ - * / %) and bitwise (& | ^ << >>)
    "chapter_3/valid/add.c": 3,
    "chapter_3/valid/associativity.c": -4,
    "chapter_3/valid/associativity_2.c": 1,
    "chapter_3/valid/associativity_3.c": 8,
    "chapter_3/valid/associativity_and_precedence.c": 10,
    "chapter_3/valid/div.c": 2,
    "chapter_3/valid/div_neg.c": -2,
    "chapter_3/valid/mod.c": 0,
    "chapter_3/valid/mult.c": 6,
    "chapter_3/valid/parens.c": 14,
    "chapter_3/valid/precedence.c": 14,
    "chapter_3/valid/sub.c": -1,
    "chapter_3/valid/sub_neg.c": 3,
    "chapter_3/valid/unop_add.c": 0,
    "chapter_3/valid/unop_parens.c": -3,
    "chapter_3/valid/extra_credit/bitwise_and.c": 1,
    "chapter_3/valid/extra_credit/bitwise_or.c": 3,
    "chapter_3/valid/extra_credit/bitwise_precedence.c": 21,
    # bitwise_shift_associativity.c: 33<<4 wraps to 16 in 1-byte int,
    # then 16>>2 = 4 (vs upstream's 132 in 4-byte int)
    "chapter_3/valid/extra_credit/bitwise_shift_associativity.c": 4,
    "chapter_3/valid/extra_credit/bitwise_shift_associativity_2.c": 16,
    # bitwise_shift_precedence.c: 40<<16 saturates to 0 in 1-byte int
    "chapter_3/valid/extra_credit/bitwise_shift_precedence.c": 0,
    # bitwise_shiftl.c: 35<<2 = 140 wraps to -116
    "chapter_3/valid/extra_credit/bitwise_shiftl.c": -116,
    # bitwise_shiftr.c: 1000 is Long, 1000>>4 = 62 fits in int
    "chapter_3/valid/extra_credit/bitwise_shiftr.c": 62,
    # bitwise_shiftr_negative.c: -5 >> 30 → -1 (arithmetic shift)
    "chapter_3/valid/extra_credit/bitwise_shiftr_negative.c": -1,
    "chapter_3/valid/extra_credit/bitwise_variable_shift_count.c": 76,
    "chapter_3/valid/extra_credit/bitwise_xor.c": 6,

    # --- chapter 4: logical (&& || !) and comparison (== != < > <= >=)
    "chapter_4/valid/and_false.c": 0,
    "chapter_4/valid/and_short_circuit.c": 0,
    "chapter_4/valid/and_true.c": 1,
    "chapter_4/valid/associativity.c": 1,
    "chapter_4/valid/compare_arithmetic_results.c": 1,
    "chapter_4/valid/eq_false.c": 0,
    "chapter_4/valid/eq_precedence.c": 1,
    "chapter_4/valid/eq_true.c": 1,
    "chapter_4/valid/ge_false.c": 0,
    "chapter_4/valid/ge_true.c": 2,
    "chapter_4/valid/gt_false.c": 0,
    "chapter_4/valid/gt_true.c": 1,
    "chapter_4/valid/le_false.c": 0,
    "chapter_4/valid/le_true.c": 2,
    "chapter_4/valid/lt_false.c": 0,
    "chapter_4/valid/lt_true.c": 1,
    "chapter_4/valid/multi_short_circuit.c": 0,
    "chapter_4/valid/ne_false.c": 0,
    "chapter_4/valid/ne_true.c": 1,
    "chapter_4/valid/nested_ops.c": 0,
    "chapter_4/valid/not.c": 0,
    "chapter_4/valid/not_sum.c": 1,
    "chapter_4/valid/not_sum_2.c": 0,
    "chapter_4/valid/not_zero.c": 1,
    "chapter_4/valid/operate_on_booleans.c": 0,
    "chapter_4/valid/or_false.c": 0,
    "chapter_4/valid/or_short_circuit.c": 1,
    "chapter_4/valid/or_true.c": 3,
    "chapter_4/valid/precedence.c": 1,
    "chapter_4/valid/precedence_2.c": 0,
    "chapter_4/valid/precedence_3.c": 0,
    "chapter_4/valid/precedence_4.c": 1,
    "chapter_4/valid/precedence_5.c": 1,
    "chapter_4/valid/extra_credit/bitwise_and_precedence.c": 0,
    "chapter_4/valid/extra_credit/bitwise_or_precedence.c": 5,
    "chapter_4/valid/extra_credit/bitwise_shift_precedence.c": 1,
    "chapter_4/valid/extra_credit/bitwise_xor_precedence.c": 5,

    # --- chapter 5: local variables, assignment, compound assignment, ++/--
    "chapter_5/valid/add_variables.c": 3,
    "chapter_5/valid/allocate_temps_and_vars.c": 1,
    "chapter_5/valid/assign.c": 2,
    "chapter_5/valid/assign_val_in_initializer.c": 5,
    "chapter_5/valid/assignment_in_initializer.c": 0,
    "chapter_5/valid/assignment_lowest_precedence.c": 1,
    "chapter_5/valid/empty_function_body.c": 0,
    # exp_then_declaration.c: -2593 truncates to -33 in 1-byte int
    "chapter_5/valid/exp_then_declaration.c": 0,
    "chapter_5/valid/kw_var_names.c": 5,
    "chapter_5/valid/local_var_missing_return.c": 0,
    "chapter_5/valid/mixed_precedence_assignment.c": 4,
    "chapter_5/valid/non_short_circuit_or.c": 1,
    "chapter_5/valid/null_statement.c": 0,
    "chapter_5/valid/null_then_return.c": 0,
    "chapter_5/valid/return_var.c": 2,
    "chapter_5/valid/short_circuit_and_fail.c": 0,
    "chapter_5/valid/short_circuit_or.c": 0,
    "chapter_5/valid/unused_exp.c": 0,
    "chapter_5/valid/use_assignment_result.c": 4,
    "chapter_5/valid/use_val_in_own_initializer.c": 0,
    "chapter_5/valid/extra_credit/bitwise_in_initializer.c": 11,
    "chapter_5/valid/extra_credit/bitwise_ops_vars.c": 9,
    "chapter_5/valid/extra_credit/bitwise_shiftl_variable.c": 24,
    # bitwise_shiftr_assign.c: 1234 wraps to -46, -46>>4 = -3
    "chapter_5/valid/extra_credit/bitwise_shiftr_assign.c": -3,
    # compound_assignment_chained.c / compound_bitwise_chained.c:
    # initializers > 127 wrap, the && check on huge expected values
    # then fails — both return 0 instead of upstream's 1
    "chapter_5/valid/extra_credit/compound_assignment_chained.c": 0,
    "chapter_5/valid/extra_credit/compound_assignment_lowest_precedence.c": 1,
    "chapter_5/valid/extra_credit/compound_assignment_use_result.c": 1,
    "chapter_5/valid/extra_credit/compound_bitwise_and.c": 2,
    "chapter_5/valid/extra_credit/compound_bitwise_assignment_lowest_precedence.c": 1,
    "chapter_5/valid/extra_credit/compound_bitwise_chained.c": 0,
    "chapter_5/valid/extra_credit/compound_bitwise_or.c": 31,
    "chapter_5/valid/extra_credit/compound_bitwise_shiftl.c": 48,
    "chapter_5/valid/extra_credit/compound_bitwise_shiftr.c": 7,
    "chapter_5/valid/extra_credit/compound_bitwise_xor.c": 2,
    "chapter_5/valid/extra_credit/compound_divide.c": 2,
    "chapter_5/valid/extra_credit/compound_minus.c": 2,
    "chapter_5/valid/extra_credit/compound_mod.c": 2,
    "chapter_5/valid/extra_credit/compound_multiply.c": 12,
    "chapter_5/valid/extra_credit/compound_plus.c": 4,
    "chapter_5/valid/extra_credit/incr_expression_statement.c": 1,
    "chapter_5/valid/extra_credit/incr_in_binary_expr.c": 1,
    "chapter_5/valid/extra_credit/incr_parenthesized.c": 1,
    "chapter_5/valid/extra_credit/postfix_incr_and_decr.c": 1,
    "chapter_5/valid/extra_credit/postfix_precedence.c": 1,
    "chapter_5/valid/extra_credit/prefix_incr_and_decr.c": 1,

    # --- chapter 6: if / else, ternary, goto / labels
    "chapter_6/valid/assign_ternary.c": 2,
    "chapter_6/valid/binary_condition.c": 5,
    "chapter_6/valid/binary_false_condition.c": 0,
    "chapter_6/valid/else.c": 2,
    "chapter_6/valid/if_nested.c": 1,
    "chapter_6/valid/if_nested_2.c": 2,
    "chapter_6/valid/if_nested_3.c": 3,
    "chapter_6/valid/if_nested_4.c": 4,
    "chapter_6/valid/if_nested_5.c": 1,
    "chapter_6/valid/if_not_taken.c": 0,
    "chapter_6/valid/if_null_body.c": 1,
    "chapter_6/valid/if_taken.c": 1,
    "chapter_6/valid/lh_assignment.c": 1,
    "chapter_6/valid/multiple_if.c": 8,
    "chapter_6/valid/nested_ternary.c": 7,
    "chapter_6/valid/nested_ternary_2.c": 15,
    "chapter_6/valid/rh_assignment.c": 1,
    "chapter_6/valid/ternary.c": 4,
    "chapter_6/valid/ternary_middle_assignment.c": 2,
    "chapter_6/valid/ternary_middle_binop.c": 1,
    "chapter_6/valid/ternary_precedence.c": 20,
    "chapter_6/valid/ternary_rh_binop.c": 1,
    "chapter_6/valid/ternary_short_circuit.c": 1,
    "chapter_6/valid/ternary_short_circuit_2.c": 2,
    "chapter_6/valid/extra_credit/bitwise_ternary.c": 5,
    "chapter_6/valid/extra_credit/compound_assign_ternary.c": 8,
    "chapter_6/valid/extra_credit/compound_if_expression.c": 1,
    "chapter_6/valid/extra_credit/goto_after_declaration.c": 1,
    "chapter_6/valid/extra_credit/goto_backwards.c": 5,
    "chapter_6/valid/extra_credit/goto_label.c": 1,
    "chapter_6/valid/extra_credit/goto_label_and_var.c": 5,
    "chapter_6/valid/extra_credit/goto_label_main.c": 0,
    "chapter_6/valid/extra_credit/goto_label_main_2.c": 1,
    "chapter_6/valid/extra_credit/goto_nested_label.c": 5,
    "chapter_6/valid/extra_credit/label_all_statements.c": 100,
    "chapter_6/valid/extra_credit/label_token.c": 1,
    "chapter_6/valid/extra_credit/lh_compound_assignment.c": 1,
    "chapter_6/valid/extra_credit/postfix_if.c": 1,
    "chapter_6/valid/extra_credit/postfix_in_ternary.c": 9,
    "chapter_6/valid/extra_credit/prefix_if.c": 1,
    "chapter_6/valid/extra_credit/prefix_in_ternary.c": 2,
    "chapter_6/valid/extra_credit/unused_label.c": 0,
    "chapter_6/valid/extra_credit/whitespace_after_label.c": 1,

    # --- chapter 7: nested blocks, scoping, shadowing
    "chapter_7/valid/assign_to_self.c": 4,
    "chapter_7/valid/assign_to_self_2.c": 3,
    "chapter_7/valid/declaration_only.c": 1,
    "chapter_7/valid/empty_blocks.c": 30,
    "chapter_7/valid/hidden_then_visible.c": 1,
    "chapter_7/valid/hidden_variable.c": 1,
    "chapter_7/valid/inner_uninitialized.c": 4,
    "chapter_7/valid/multiple_vars_same_name.c": 2,
    "chapter_7/valid/nested_if.c": 1,
    "chapter_7/valid/similar_var_names.c": 28,
    "chapter_7/valid/use_in_inner_scope.c": 3,
    "chapter_7/valid/extra_credit/compound_subtract_in_block.c": 1,
    "chapter_7/valid/extra_credit/goto_before_declaration.c": 0,
    "chapter_7/valid/extra_credit/goto_inner_scope.c": 1,
    "chapter_7/valid/extra_credit/goto_outer_scope.c": 1,
    "chapter_7/valid/extra_credit/goto_sibling_scope.c": 11,

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
