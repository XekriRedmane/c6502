"""Checks docs/optimizations/README.md against the pipeline code.

The README lists every optimization twice, alphabetically and in
pipeline order. These tests fail when a document is added without an
index entry, when an index entry has no document, and when a pass is
added to or moved within the pipeline without the pipeline-order list
being updated to match.

The order of the passes comes from `c6502cpp pass-order`
(cpp/src/pass_order.h), which lists the passes of each of the
optimizer's drivers in the order a compile runs them. A pass is
matched to its document through its module in `_MODULES`, the dotted
name of the Python module that defined it, for which cpp/src holds a
C++ source of the same path (`passes.asm_licm` and
cpp/src/passes/asm_licm.cpp). A module's last name component is the
document's file stem, except for the modules in `_STEM_ALIASES`."""

import re
import subprocess
import unittest
from pathlib import Path

from tests import cpp_build

_ROOT = Path(__file__).resolve().parent.parent
_DOCS = _ROOT / "docs" / "optimizations"
_README = _DOCS / "README.md"

# Modules whose document has a different stem, because one document
# covers several modules or because two layers have a module of the
# same name.
_STEM_ALIASES = {
    "passes.abi_selection": "zp_abi",
    "passes.zp_slot_allocation": "zp_abi",
    "passes.zp_local_allocation": "zp_local_pools",
    "passes.function_local_sizing": "zp_local_pools",
    "passes.optimization_asm.copy_propagation": "asm_copy_propagation",
    "passes.optimization_asm.hwreg_eligibility": "regalloc",
    "passes.optimization_asm.apply_coloring": "regalloc",
}

# The module of each pass that `c6502cpp pass-order` names, by the name
# it gives the pass. A rule set of the TAC optimizer
# (`reduce_strength`, `fold_producer_jumps`) has the modules of its
# rules, in their order.
_MODULES = {
    # The peephole fixed point (cpp/src/passes/peephole_fixedpoint.cpp).
    "split_mem_to_mem": "passes.split_mem_to_mem",
    "carry_halve": "passes.carry_halve",
    "carry_in_add": "passes.carry_in_add",
    "inc_peephole": "passes.inc_peephole",
    "inc_low_byte_test": "passes.inc_low_byte_test",
    "carry_inc": "passes.carry_inc",
    "dec_peephole": "passes.dec_peephole",
    "sub1_test_zero": "passes.sub1_test_zero_peephole",
    "element_in_place": "passes.element_in_place",
    "direct_index_load": "passes.direct_index_load",
    "dead_pha_pla": "passes.dead_pha_pla",
    "cpx_cpy": "passes.cpx_cpy_peephole",
    "memory_value_propagation": "passes.memory_value_propagation",
    "shift_in_a": "passes.shift_in_a",
    "redundant_load_after_rmw": "passes.redundant_load_after_rmw",
    "redundant_load": "passes.redundant_load",
    "redundant_store": "passes.redundant_store",
    "constant_store_order": "passes.constant_store_order",
    "join_store": "passes.join_store",
    "remat": "passes.asm_remat",
    "asm_dead_store": "passes.asm_dead_store",
    "loop_constant_a": "passes.loop_constant_a",
    "dead_a_arith": "passes.dead_a_arith",
    "dead_label_drop": "passes.dead_label_drop",
    "unreachable_drop": "passes.unreachable_drop",
    "branch_invert": "passes.branch_invert",
    "branch_to_next_drop": "passes.branch_to_next_drop",
    "branch_through_jump": "passes.branch_through_jump",
    "branch_thread": "passes.branch_thread",
    "branch_value_thread": "passes.branch_value_thread",
    "dec_register_peephole": "passes.dec_register_peephole",
    "transfer_past_branch": "passes.transfer_past_branch",
    "index_inc_cmp_y": "passes.index_inc_cmp_y",
    "volatile_void_read_cmp": "passes.volatile_void_read_cmp",
    "dec_branch_indirect_y_fold": "passes.dec_branch_indirect_y_fold",
    "mem_const_prop": "passes.mem_const_prop",
    "const_arith_fold": "passes.const_arith_fold",
    "label_offset_fold": "passes.label_offset_fold",
    "carry_recompute_drop": "passes.carry_recompute",
    "carry_return_tails": "passes.carry_return",
    "carry_const_fold": "passes.carry_return",
    "carry_branch_fold": "passes.carry_return",
    "round_trip_load_drop": "passes.round_trip_load",
    "and_sign_bit_branch": "passes.and_sign_bit_branch",
    "self_store_drop": "passes.self_store_drop",
    "pointer_self_store_drop": "passes.pointer_self_store",
    "redundant_compare": "passes.redundant_compare",
    "cmp_through_tmp_fold": "passes.cmp_through_tmp_fold",
    "adc_commute": "passes.adc_commute",
    "cmp_sbc_fusion": "passes.cmp_sbc_fusion",
    "dec_inc_branch_fold": "passes.dec_inc_branch_fold",
    "tail_call": "passes.tail_call",
    "tail_merge": "passes.tail_merge",
    "cross_jump": "passes.cross_jump",
    "via_a_store_fold": "passes.via_a_store_fold",
    "transfer_pm1_store": "passes.transfer_pm1_store",
    # The TAC optimizer's driver (cpp/src/passes/optimization/optimizer.cpp).
    "rotate_signed_countdown_loops": "passes.optimization.loop_rotate",
    "noreturn_calls": "passes.optimization.noreturn_call",
    "fold_static_const_reads": "passes.optimization.static_const_fold",
    "constant_fold": "passes.optimization.constant_folding",
    "distribute_constant": "passes.optimization.distribute_constant",
    "fold_into_constant_phi": "passes.optimization.phi_constant_fold",
    "reduce_strength": ["passes.optimization.strength_reduction"],
    "fold_producer_jumps": [
        "passes.optimization.cmp_zero_jump_fold",
        "passes.optimization.and_zero_jump_fold",
        "passes.optimization.lnot_jump_fold",
        "passes.optimization.nonnegative_compare",
    ],
    "eliminate_dead_loops": "passes.optimization.dead_loop_elimination",
    "thread_constant_jumps": "passes.optimization.jump_thread",
    "eliminate_unreachable_code": "passes.optimization.unreachable_code_elimination",
    "eliminate_common_subexpressions": "passes.optimization.cse",
    "hoist_loop_invariants": "passes.optimization.licm",
    "copy_propagate": None,  # one name in two drivers, below
    "eliminate_dead_stores": "passes.optimization.dead_store_elimination",
    "read_stored_globals": "passes.optimization.read_stored_global",
    "fold_copies_in_fixedpoint": "passes.optimization.copy_folding",
    "reassoc_constants": "passes.optimization.reassoc_const",
    "reassoc_offset_constant": "passes.optimization.reassoc_offset",
    "direct_member_access": "passes.optimization.direct_member_access",
    "recognize_indexed_store": "passes.optimization.recognize_indexed_store",
    "recognize_indexed_load": "passes.optimization.recognize_indexed_load",
    "fold_truncate_extend": "passes.optimization.truncate_extend_fold",
    "fold_extend_extend": "passes.optimization.truncate_extend_fold",
    "sink_increments": "passes.optimization.sink_increment",
    "sink_and_past_branch": "passes.optimization.sink_and_past_branch",
    "narrow_widened_arith": "passes.optimization.narrow_widened_arith",
    "narrow_byte_range": "passes.optimization.narrow_byte_range",
    "narrow_byte_bitwise": "passes.optimization.narrow_byte_bitwise",
    "phi_offset": "passes.optimization.phi_offset",
    "static_indexed_access": "passes.optimization.static_indexed_access",
    "recognize_indirect_indexed": "passes.optimization.recognize_indirect_indexed",
    "hoist_subtract_to_compare": "passes.optimization.compare_subtract",
    "hoist_divmod_pairs": "passes.optimization.divmod_hoist",
    "count_loops_down": "passes.optimization.count_down",
    "fold_copies_post_destruction": "passes.optimization.copy_folding",
    "move_steps_onto_edges": "passes.optimization.step_on_edge",
    "split_three_way_compares": "passes.optimization.three_way_compare",
    "fold_short_circuit_jump": "passes.optimization.short_circuit_jump_fold",
    "thread_known_compares": "passes.optimization.known_compare_thread",
    # The asm optimizer (cpp/src/passes/optimization_asm/optimizer.cpp).
    "fold_const_statics": "passes.optimization_asm.const_static_fold",
    "sub1_test_zero_peephole": "passes.sub1_test_zero_peephole",
    "explicit_index_tests": "passes.optimization_asm.optimizer",
    "to_ssa": "passes.optimization_asm.ssa_construction",
    "backward_copy_propagate": "passes.optimization_asm.backward_copy_propagation",
    "byte_dce": "passes.optimization_asm.byte_dce",
    "absorb_zero_load": "passes.optimization_asm.or_zero_absorb",
    "compute_liveness": "passes.optimization_asm.liveness",
    "build_interference": "passes.optimization_asm.interference",
    "coalesce_moves": "passes.optimization_asm.coalescing",
    "scan_hwreg_eligibility": "passes.optimization_asm.hwreg_eligibility",
    "color_graph": "passes.optimization_asm.regalloc",
    "apply_coloring": "passes.optimization_asm.apply_coloring",
    "from_ssa": "passes.optimization_asm.ssa_destruction",
    "dead_static_elimination": "passes.optimization_asm.dead_static",
    # `optimized_tac` (cpp/src/passes/translate_for_codegen.cpp).
    "row_address_tables": "passes.optimization.row_address_tables",
    "optimize_tac": "passes.optimization.optimizer",
    "dispatch_const_pointer_arrays": "passes.optimization.dispatch_pointer_array",
    "split_tables": "passes.optimization.split_tables",
    # `optimized_front` (cpp/src/passes/optimized_front.cpp).
    "select_abi": "passes.abi_selection",
    "allocate_function_locals": "passes.zp_local_allocation",
    "lower_data_load_address": "passes.lower_data_load_address",
    "synthesize_prologue": "passes.prologue_synthesis",
    # The region of `optimized_asm` (cpp/src/passes/optimized_asm.cpp).
    "licm": "passes.asm_licm",
    "promote_loop_carried_to_a": "passes.promote_loop_carried_to_a",
    "loop_counter_to_x": "passes.loop_counter_to_x",
    "counter_to_index_reg": "passes.counter_to_index_reg",
    "x_save_slot_load": "passes.x_save_slot_load",
    "dual_index_promotion": "passes.dual_index_promotion",
    "carry_known": "passes.carry_known",
    "slot_coalescing": "passes.slot_coalescing",
    "ret_slot_coalescing": "passes.ret_slot_coalescing",
    "loop_invariant_a": "passes.loop_invariant_a",
    "rotate_loop_test": "passes.rotate_loop_test",
    "jump_tables": "passes.jump_table",
    "copy_through_index": "passes.via_a_store_fold",
    "long_branches": "passes.long_branches",
    "short_jumps": "passes.short_jumps",
    "shared_tails": "passes.shared_tails",
    "outline_runs": "passes.outline_runs",
}

# The names that two drivers give passes of different modules, by
# driver.
_DRIVER_MODULES = {
    ("tac", "copy_propagate"): "passes.optimization.copy_propagation",
    ("asm_optimizer.function", "copy_propagate"): "passes.optimization_asm.copy_propagation",
}

_LINK = re.compile(r"\]\(([a-z0-9_]+)\.md\)")


def _doc_stem(module_name: str) -> str | None:
    """Returns the stem of the document for the pass defined in
    `module_name`, or None if no document covers that module."""
    stem = _STEM_ALIASES.get(module_name, module_name.rsplit(".", 1)[-1])
    return stem if (_DOCS / f"{stem}.md").exists() else None


def _dedupe(stems: list[str]) -> list[str]:
    """Collapses runs of the same stem, since one document can cover
    several passes that run back to back (the carry-return peepholes)."""
    out: list[str] = []
    for s in stems:
        if not out or out[-1] != s:
            out.append(s)
    return out


def _pass_order(cli: str) -> dict[str, list[tuple[str, str]]]:
    """Returns `c6502cpp pass-order`'s passes of each driver, as
    `(driver, pass)` pairs in the order it lists them."""
    out = subprocess.run(
        [cli, "pass-order"], check=True, capture_output=True,
    ).stdout.decode("utf-8")
    order: dict[str, list[tuple[str, str]]] = {}
    for line in out.splitlines():
        driver, name = line.split()
        order.setdefault(driver, []).append((driver, name))
    return order


def _modules(entry: tuple[str, str]) -> list[str]:
    """Returns the modules of the pass `entry`, a `(driver, pass)` pair
    of `_pass_order`."""
    driver, name = entry
    if (driver, name) in _DRIVER_MODULES:
        return [_DRIVER_MODULES[driver, name]]
    if _MODULES.get(name) is None:
        raise AssertionError(
            f"c6502cpp pass-order names the pass {name} of {driver}, "
            "which has no module in _MODULES",
        )
    modules = _MODULES[name]
    return modules if isinstance(modules, list) else [modules]


def _stems(entries: list[tuple[str, str]]) -> list[str]:
    """Returns the document stems of the passes `entries`, in order, with
    each run of one stem collapsed."""
    stems = []
    for entry in entries:
        stems += [s for s in map(_doc_stem, _modules(entry)) if s]
    return _dedupe(stems)


def _is_subsequence(needle: list[str], haystack: list[str]) -> bool:
    it = iter(haystack)
    return all(any(h == n for h in it) for n in needle)


def _read_readme(test: unittest.TestCase) -> None:
    """Sets the README's lists on `test`: `alpha`, the alphabetical
    list, `order`, the pipeline order up to the peephole fixed point,
    and `peephole`, the fixed point's sweep, each as document stems, and
    `docs`, the stems of the documents."""
    text = _README.read_text(encoding="utf-8")
    alpha_start = text.index("## Alphabetical list")
    order_start = text.index("## Pipeline order")
    peep_start = text.index(
        "The peephole fixed point (`compile._peephole_fixedpoint`)",
    )
    unopt_start = text.index("### Without `-O`")
    test.alpha = _LINK.findall(text[alpha_start:order_start])
    test.order = _LINK.findall(text[order_start:peep_start])
    test.peephole = _LINK.findall(text[peep_start:unopt_start])
    test.docs = sorted(
        p.stem for p in _DOCS.glob("*.md") if p.name != "README.md"
    )


class TestIndex(unittest.TestCase):
    def setUp(self):
        _read_readme(self)

    def test_every_document_is_in_the_alphabetical_list_once(self):
        self.assertEqual(sorted(self.alpha), self.docs)

    def test_every_document_is_in_the_pipeline_order(self):
        self.assertEqual(
            sorted(set(self.order) | set(self.peephole)), self.docs,
        )


@cpp_build.needs_c6502cpp
class TestPassOrder(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        import cpp_backend
        cls.passes = _pass_order(cpp_backend._cpp_cli())

    def setUp(self):
        _read_readme(self)

    def test_every_pass_has_a_module_with_a_source(self):
        # A pass added to a driver needs its module in `_MODULES` before
        # the other tests can place it, and the module names a C++
        # source.
        for entries in self.passes.values():
            for entry in entries:
                for module in _modules(entry):
                    source = module.replace(".", "/") + ".cpp"
                    path = _ROOT / "cpp" / "src" / source
                    with self.subTest(entry=entry):
                        self.assertTrue(path.exists(), path)

    def test_peephole_list_matches_the_fixed_point(self):
        self.assertEqual(self.peephole, _stems(self.passes["peephole"]))

    def test_tac_driver_order(self):
        stems = _stems(self.passes["tac"])
        self.assertTrue(
            _is_subsequence(stems, self.order),
            f"TAC driver order {stems} isn't in the README's order",
        )

    def test_asm_optimizer_order(self):
        # `optimize_program` folds the const statics, runs the round trip
        # of each function (`optimize_function`), and drops the dead
        # statics.
        outer = self.passes["asm_optimizer"]
        self.assertEqual(
            [name for _, name in outer],
            ["fold_const_statics", "dead_static_elimination"],
        )
        stems = _stems(
            outer[:1] + self.passes["asm_optimizer.function"] + outer[1:],
        )
        self.assertTrue(
            _is_subsequence(stems, self.order),
            f"asm optimizer order {stems} isn't in the README's order",
        )

    def test_optimized_asm_order(self):
        # `optimized_asm` runs `_optimized_front` (the TAC passes of
        # `_optimized_tac`, then code generation through
        # `synthesize_prologue`) and then the back end's region.
        code = _stems(
            self.passes["optimized_tac"] + self.passes["optimized_front"]
            + self.passes["optimized_asm"],
        )
        self.assertTrue(
            _is_subsequence(code, self.order),
            f"optimized_asm order {code} isn't in the README's order",
        )


if __name__ == "__main__":
    unittest.main()
