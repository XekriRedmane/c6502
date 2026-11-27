// Checks docs/optimizations/README.md against the pipeline code, as
// tests/test_optimization_docs.py did. The README lists every
// optimization twice, alphabetically and in pipeline order. These cases
// fail when a document is added without an index entry, when an index
// entry has no document, and when a pass is added to or moved within
// the pipeline without the pipeline-order list being updated to match.
//
// The order of the passes comes from `pass_order::listing()`
// (src/pass_order.h), which `c6502cpp pass-order` prints. A pass is
// matched to its document through its module in `kModules`, a dotted
// name for the C++ source of the same path under cpp/src that defines
// it (`passes.asm_licm` for cpp/src/passes/asm_licm.cpp). A module's
// last name component is the document's file stem, except for the
// modules in `kStemAliases`.
#include <algorithm>
#include <filesystem>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "pass_order.h"

namespace {

namespace suite = c6502::suite;

using Entry = std::pair<std::string, std::string>;  // (driver, pass)

// Modules whose document has a different stem, because one document
// covers several modules or because two layers have a module of the
// same name.
const std::map<std::string, std::string> kStemAliases = {
    {"passes.abi_selection", "zp_abi"},
    {"passes.zp_slot_allocation", "zp_abi"},
    {"passes.zp_local_allocation", "zp_local_pools"},
    {"passes.function_local_sizing", "zp_local_pools"},
    {"passes.optimization_asm.copy_propagation", "asm_copy_propagation"},
    {"passes.optimization_asm.hwreg_eligibility", "regalloc"},
    {"passes.optimization_asm.apply_coloring", "regalloc"},
};

// The modules of each pass that `pass_order::listing()` names, by the
// name it gives the pass. A rule set of the TAC optimizer
// (`reduce_strength`, `fold_producer_jumps`) has the modules of its
// rules, in their order. `copy_propagate` is one name in two drivers,
// which `kDriverModules` tells apart, so it has no modules here.
const std::map<std::string, std::vector<std::string>> kModules = {
    // The peephole fixed point (cpp/src/passes/peephole_fixedpoint.cpp).
    {"split_mem_to_mem", {"passes.split_mem_to_mem"}},
    {"carry_halve", {"passes.carry_halve"}},
    {"carry_in_add", {"passes.carry_in_add"}},
    {"inc_peephole", {"passes.inc_peephole"}},
    {"inc_low_byte_test", {"passes.inc_low_byte_test"}},
    {"carry_inc", {"passes.carry_inc"}},
    {"dec_peephole", {"passes.dec_peephole"}},
    {"sub1_test_zero", {"passes.sub1_test_zero_peephole"}},
    {"element_in_place", {"passes.element_in_place"}},
    {"direct_index_load", {"passes.direct_index_load"}},
    {"dead_pha_pla", {"passes.dead_pha_pla"}},
    {"cpx_cpy", {"passes.cpx_cpy_peephole"}},
    {"memory_value_propagation", {"passes.memory_value_propagation"}},
    {"shift_in_a", {"passes.shift_in_a"}},
    {"redundant_load_after_rmw", {"passes.redundant_load_after_rmw"}},
    {"redundant_load", {"passes.redundant_load"}},
    {"redundant_store", {"passes.redundant_store"}},
    {"constant_store_order", {"passes.constant_store_order"}},
    {"join_store", {"passes.join_store"}},
    {"remat", {"passes.asm_remat"}},
    {"asm_dead_store", {"passes.asm_dead_store"}},
    {"loop_constant_a", {"passes.loop_constant_a"}},
    {"dead_a_arith", {"passes.dead_a_arith"}},
    {"dead_label_drop", {"passes.dead_label_drop"}},
    {"unreachable_drop", {"passes.unreachable_drop"}},
    {"branch_invert", {"passes.branch_invert"}},
    {"branch_to_next_drop", {"passes.branch_to_next_drop"}},
    {"branch_through_jump", {"passes.branch_through_jump"}},
    {"branch_thread", {"passes.branch_thread"}},
    {"branch_value_thread", {"passes.branch_value_thread"}},
    {"dec_register_peephole", {"passes.dec_register_peephole"}},
    {"transfer_past_branch", {"passes.transfer_past_branch"}},
    {"index_inc_cmp_y", {"passes.index_inc_cmp_y"}},
    {"volatile_void_read_cmp", {"passes.volatile_void_read_cmp"}},
    {"dec_branch_indirect_y_fold", {"passes.dec_branch_indirect_y_fold"}},
    {"mem_const_prop", {"passes.mem_const_prop"}},
    {"const_arith_fold", {"passes.const_arith_fold"}},
    {"label_offset_fold", {"passes.label_offset_fold"}},
    {"carry_recompute_drop", {"passes.carry_recompute"}},
    {"carry_return_tails", {"passes.carry_return"}},
    {"carry_const_fold", {"passes.carry_return"}},
    {"carry_branch_fold", {"passes.carry_return"}},
    {"round_trip_load_drop", {"passes.round_trip_load"}},
    {"and_sign_bit_branch", {"passes.and_sign_bit_branch"}},
    {"self_store_drop", {"passes.self_store_drop"}},
    {"pointer_self_store_drop", {"passes.pointer_self_store"}},
    {"redundant_compare", {"passes.redundant_compare"}},
    {"cmp_through_tmp_fold", {"passes.cmp_through_tmp_fold"}},
    {"adc_commute", {"passes.adc_commute"}},
    {"cmp_sbc_fusion", {"passes.cmp_sbc_fusion"}},
    {"dec_inc_branch_fold", {"passes.dec_inc_branch_fold"}},
    {"tail_call", {"passes.tail_call"}},
    {"tail_merge", {"passes.tail_merge"}},
    {"cross_jump", {"passes.cross_jump"}},
    {"via_a_store_fold", {"passes.via_a_store_fold"}},
    {"transfer_pm1_store", {"passes.transfer_pm1_store"}},
    // The TAC optimizer's driver (cpp/src/passes/optimization/optimizer.cpp).
    {"rotate_signed_countdown_loops", {"passes.optimization.loop_rotate"}},
    {"noreturn_calls", {"passes.optimization.noreturn_call"}},
    {"fold_static_const_reads", {"passes.optimization.static_const_fold"}},
    {"constant_fold", {"passes.optimization.constant_folding"}},
    {"distribute_constant", {"passes.optimization.distribute_constant"}},
    {"fold_into_constant_phi", {"passes.optimization.phi_constant_fold"}},
    {"reduce_strength", {"passes.optimization.strength_reduction"}},
    {"fold_producer_jumps",
     {"passes.optimization.cmp_zero_jump_fold", "passes.optimization.and_zero_jump_fold",
      "passes.optimization.lnot_jump_fold", "passes.optimization.nonnegative_compare"}},
    {"eliminate_dead_loops", {"passes.optimization.dead_loop_elimination"}},
    {"thread_constant_jumps", {"passes.optimization.jump_thread"}},
    {"eliminate_unreachable_code", {"passes.optimization.unreachable_code_elimination"}},
    {"eliminate_common_subexpressions", {"passes.optimization.cse"}},
    {"hoist_loop_invariants", {"passes.optimization.licm"}},
    {"copy_propagate", {}},
    {"eliminate_dead_stores", {"passes.optimization.dead_store_elimination"}},
    {"read_stored_globals", {"passes.optimization.read_stored_global"}},
    {"fold_copies_in_fixedpoint", {"passes.optimization.copy_folding"}},
    {"reassoc_constants", {"passes.optimization.reassoc_const"}},
    {"reassoc_offset_constant", {"passes.optimization.reassoc_offset"}},
    {"direct_member_access", {"passes.optimization.direct_member_access"}},
    {"recognize_indexed_store", {"passes.optimization.recognize_indexed_store"}},
    {"recognize_indexed_load", {"passes.optimization.recognize_indexed_load"}},
    {"fold_truncate_extend", {"passes.optimization.truncate_extend_fold"}},
    {"fold_extend_extend", {"passes.optimization.truncate_extend_fold"}},
    {"sink_increments", {"passes.optimization.sink_increment"}},
    {"sink_and_past_branch", {"passes.optimization.sink_and_past_branch"}},
    {"narrow_widened_arith", {"passes.optimization.narrow_widened_arith"}},
    {"narrow_byte_range", {"passes.optimization.narrow_byte_range"}},
    {"narrow_byte_bitwise", {"passes.optimization.narrow_byte_bitwise"}},
    {"phi_offset", {"passes.optimization.phi_offset"}},
    {"static_indexed_access", {"passes.optimization.static_indexed_access"}},
    {"recognize_indirect_indexed", {"passes.optimization.recognize_indirect_indexed"}},
    {"hoist_subtract_to_compare", {"passes.optimization.compare_subtract"}},
    {"hoist_divmod_pairs", {"passes.optimization.divmod_hoist"}},
    {"count_loops_down", {"passes.optimization.count_down"}},
    {"fold_copies_post_destruction", {"passes.optimization.copy_folding"}},
    {"move_steps_onto_edges", {"passes.optimization.step_on_edge"}},
    {"split_three_way_compares", {"passes.optimization.three_way_compare"}},
    {"fold_short_circuit_jump", {"passes.optimization.short_circuit_jump_fold"}},
    {"thread_known_compares", {"passes.optimization.known_compare_thread"}},
    // The asm optimizer (cpp/src/passes/optimization_asm/optimizer.cpp).
    {"fold_const_statics", {"passes.optimization_asm.const_static_fold"}},
    {"sub1_test_zero_peephole", {"passes.sub1_test_zero_peephole"}},
    {"explicit_index_tests", {"passes.optimization_asm.optimizer"}},
    {"to_ssa", {"passes.optimization_asm.ssa_construction"}},
    {"backward_copy_propagate", {"passes.optimization_asm.backward_copy_propagation"}},
    {"byte_dce", {"passes.optimization_asm.byte_dce"}},
    {"absorb_zero_load", {"passes.optimization_asm.or_zero_absorb"}},
    {"compute_liveness", {"passes.optimization_asm.liveness"}},
    {"build_interference", {"passes.optimization_asm.interference"}},
    {"coalesce_moves", {"passes.optimization_asm.coalescing"}},
    {"scan_hwreg_eligibility", {"passes.optimization_asm.hwreg_eligibility"}},
    {"color_graph", {"passes.optimization_asm.regalloc"}},
    {"apply_coloring", {"passes.optimization_asm.apply_coloring"}},
    {"from_ssa", {"passes.optimization_asm.ssa_destruction"}},
    {"dead_static_elimination", {"passes.optimization_asm.dead_static"}},
    // `optimized_tac` (cpp/src/passes/translate_for_codegen.cpp).
    {"row_address_tables", {"passes.optimization.row_address_tables"}},
    {"optimize_tac", {"passes.optimization.optimizer"}},
    {"dispatch_const_pointer_arrays", {"passes.optimization.dispatch_pointer_array"}},
    {"split_tables", {"passes.optimization.split_tables"}},
    // `optimized_front` (cpp/src/passes/optimized_front.cpp).
    {"select_abi", {"passes.abi_selection"}},
    {"allocate_function_locals", {"passes.zp_local_allocation"}},
    {"lower_data_load_address", {"passes.lower_data_load_address"}},
    {"synthesize_prologue", {"passes.prologue_synthesis"}},
    // The region of `optimized_asm` (cpp/src/passes/optimized_asm.cpp).
    {"licm", {"passes.asm_licm"}},
    {"promote_loop_carried_to_a", {"passes.promote_loop_carried_to_a"}},
    {"loop_counter_to_x", {"passes.loop_counter_to_x"}},
    {"counter_to_index_reg", {"passes.counter_to_index_reg"}},
    {"x_save_slot_load", {"passes.x_save_slot_load"}},
    {"dual_index_promotion", {"passes.dual_index_promotion"}},
    {"carry_known", {"passes.carry_known"}},
    {"slot_coalescing", {"passes.slot_coalescing"}},
    {"ret_slot_coalescing", {"passes.ret_slot_coalescing"}},
    {"loop_invariant_a", {"passes.loop_invariant_a"}},
    {"rotate_loop_test", {"passes.rotate_loop_test"}},
    {"jump_tables", {"passes.jump_table"}},
    {"copy_through_index", {"passes.via_a_store_fold"}},
    {"long_branches", {"passes.long_branches"}},
    {"short_jumps", {"passes.short_jumps"}},
    {"shared_tails", {"passes.shared_tails"}},
    {"outline_runs", {"passes.outline_runs"}},
};

// The names that two drivers give passes of different modules, by
// driver.
const std::map<Entry, std::string> kDriverModules = {
    {{"tac", "copy_propagate"}, "passes.optimization.copy_propagation"},
    {{"asm_optimizer.function", "copy_propagate"}, "passes.optimization_asm.copy_propagation"},
};

std::filesystem::path docs_dir() {
  return c6502::compile_command::utf8_path(suite::repo_path("docs/optimizations"));
}

// Returns the stem of the document for the pass defined in `module`,
// or "" when no document covers that module.
std::string doc_stem(const std::string& module) {
  const auto alias = kStemAliases.find(module);
  const std::string stem =
      alias != kStemAliases.end() ? alias->second : module.substr(module.rfind('.') + 1);
  return std::filesystem::exists(docs_dir() / (stem + ".md")) ? stem : "";
}

// Returns the passes of each driver that `listing` gives, in its order.
std::map<std::string, std::vector<Entry>> pass_order() {
  std::map<std::string, std::vector<Entry>> order;
  std::istringstream lines(c6502::pass_order::listing());
  std::string driver, name;
  while (lines >> driver >> name) order[driver].emplace_back(driver, name);
  return order;
}

// Returns the modules of the pass `entry`, and fails the case for a
// pass with no module in `kModules`.
std::vector<std::string> modules_of(const Entry& entry) {
  if (const auto it = kDriverModules.find(entry); it != kDriverModules.end()) {
    return {it->second};
  }
  const auto it = kModules.find(entry.second);
  if (it == kModules.end() || it->second.empty()) {
    FAIL("pass-order names the pass " << entry.second << " of " << entry.first
                                      << ", which has no module in kModules");
  }
  return it->second;
}

// Returns the document stems of the passes `entries`, in order, with
// each run of one stem collapsed, since one document can cover several
// passes that run back to back (the carry-return peepholes).
std::vector<std::string> stems(const std::vector<Entry>& entries) {
  std::vector<std::string> out;
  for (const Entry& e : entries) {
    for (const std::string& m : modules_of(e)) {
      const std::string s = doc_stem(m);
      if (!s.empty() && (out.empty() || out.back() != s)) out.push_back(s);
    }
  }
  return out;
}

bool is_subsequence(const std::vector<std::string>& needle, const std::vector<std::string>& hay) {
  auto it = hay.begin();
  for (const std::string& n : needle) {
    it = std::find(it, hay.end(), n);
    if (it == hay.end()) return false;
    ++it;
  }
  return true;
}

// The README's lists as document stems: the alphabetical list, the
// pipeline order up to the peephole fixed point, and the fixed point's
// sweep, and the stems of the documents.
struct Readme {
  std::vector<std::string> alpha, order, peephole, docs;
};

std::vector<std::string> links(const std::string& text) {
  static const std::regex link(R"(\]\(([a-z0-9_]+)\.md\))");
  std::vector<std::string> out;
  for (auto it = std::sregex_iterator(text.begin(), text.end(), link);
       it != std::sregex_iterator(); ++it) {
    out.push_back((*it)[1]);
  }
  return out;
}

Readme read_readme() {
  const std::string text = suite::read_repo_file("docs/optimizations/README.md");
  const std::size_t alpha = text.find("## Alphabetical list");
  const std::size_t order = text.find("## Pipeline order");
  const std::size_t peep =
      text.find("The peephole fixed point (cpp/src/passes/peephole_fixedpoint.cpp)");
  const std::size_t unopt = text.find("### Without `-O`");
  REQUIRE(alpha != std::string::npos);
  REQUIRE(order != std::string::npos);
  REQUIRE(peep != std::string::npos);
  REQUIRE(unopt != std::string::npos);
  Readme r;
  r.alpha = links(text.substr(alpha, order - alpha));
  r.order = links(text.substr(order, peep - order));
  r.peephole = links(text.substr(peep, unopt - peep));
  for (const auto& f : std::filesystem::directory_iterator(docs_dir())) {
    if (f.path().extension() == ".md" && f.path().filename() != "README.md") {
      r.docs.push_back(f.path().stem().string());
    }
  }
  std::sort(r.docs.begin(), r.docs.end());
  return r;
}

std::string joined(const std::vector<std::string>& v) {
  std::string out;
  for (const auto& s : v) out += (out.empty() ? "" : " ") + s;
  return out;
}

}  // namespace

TEST_CASE("optimization_docs: every document is in the alphabetical list once") {
  Readme r = read_readme();
  std::sort(r.alpha.begin(), r.alpha.end());
  CHECK(r.alpha == r.docs);
}

TEST_CASE("optimization_docs: every document is in the pipeline order") {
  const Readme r = read_readme();
  std::set<std::string> both(r.order.begin(), r.order.end());
  both.insert(r.peephole.begin(), r.peephole.end());
  CHECK(std::vector<std::string>(both.begin(), both.end()) == r.docs);
}

TEST_CASE("optimization_docs: every pass has a module with a source") {
  // A pass added to a driver needs its module in `kModules` before the
  // other cases can place it, and the module names a C++ source.
  for (const auto& [driver, entries] : pass_order()) {
    for (const Entry& entry : entries) {
      for (std::string module : modules_of(entry)) {
        std::replace(module.begin(), module.end(), '.', '/');
        const std::string path = suite::repo_path("cpp/src/" + module + ".cpp");
        INFO(driver << " " << entry.second);
        CHECK_MESSAGE(std::filesystem::exists(c6502::compile_command::utf8_path(path)), path);
      }
    }
  }
}

TEST_CASE("optimization_docs: the peephole list matches the fixed point") {
  const Readme r = read_readme();
  CHECK(r.peephole == stems(pass_order()["peephole"]));
}

TEST_CASE("optimization_docs: the TAC driver's order is the README's") {
  const Readme r = read_readme();
  const std::vector<std::string> s = stems(pass_order()["tac"]);
  CHECK_MESSAGE(is_subsequence(s, r.order),
                "TAC driver order " << joined(s) << " isn't in the README's order");
}

TEST_CASE("optimization_docs: the asm optimizer's order is the README's") {
  // `optimize_program` folds the const statics, runs the round trip of
  // each function (`optimize_function`), and drops the dead statics.
  const Readme r = read_readme();
  auto order = pass_order();
  const std::vector<Entry>& outer = order["asm_optimizer"];
  std::vector<std::string> names;
  for (const auto& e : outer) names.push_back(e.second);
  REQUIRE(names == std::vector<std::string>{"fold_const_statics", "dead_static_elimination"});
  std::vector<Entry> all{outer[0]};
  const auto& fn = order["asm_optimizer.function"];
  all.insert(all.end(), fn.begin(), fn.end());
  all.push_back(outer[1]);
  const std::vector<std::string> s = stems(all);
  CHECK_MESSAGE(is_subsequence(s, r.order),
                "asm optimizer order " << joined(s) << " isn't in the README's order");
}

TEST_CASE("optimization_docs: optimized_asm's order is the README's") {
  // `optimized_asm` runs `optimized_front` (the TAC passes of
  // `optimized_tac`, then code generation through `synthesize_prologue`)
  // and then the back end's region.
  const Readme r = read_readme();
  auto order = pass_order();
  std::vector<Entry> all = order["optimized_tac"];
  for (const char* d : {"optimized_front", "optimized_asm"}) {
    all.insert(all.end(), order[d].begin(), order[d].end());
  }
  const std::vector<std::string> s = stems(all);
  CHECK_MESSAGE(is_subsequence(s, r.order),
                "optimized_asm order " << joined(s) << " isn't in the README's order");
}
