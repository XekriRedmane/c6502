#include "passes/optimization/tac_stage.h"

#include "tagged_json.h"

namespace c6502::passes::tac_stage {

namespace {

namespace ir = ::c6502::tac_ast;
using namespace ::c6502::tagged;

using Sym = SymbolTableC;
using Types = TypeTableC;
using TacFunctionC = Asdl<ir::Type_top_level, kTacAst>;

template <class C>
constexpr RoundTrip rt = &round_trip<C>;

// The signature of one of the TAC optimizer's calls on a function.
// `run` is null until the pass is ported.
StageSignature tac_pass(std::string_view name, StageRun run = nullptr) {
  return {.name = name,
          .input = "tac_ast",
          .kwargs = {{"symbols", rt<Optional<Sym>>}, {"ssa_dsts", rt<Optional<NameSetC>>}},
          .output_module = "tac_ast",
          .run = run};
}

}  // namespace

std::vector<StageSignature> signatures() {
  return {
      // SSA construction, which returns the function and its SSA names
      // as a tuple, and destruction, where the driver calls them.
      {.name = "tac_to_ssa",
       .input = "tac_ast",
       .kwargs = {{"symbols", rt<Optional<Sym>>}, {"ssa_dsts", rt<Optional<NameSetC>>}},
       .output = rt<Pair<TacFunctionC, NameSetC>>},
      tac_pass("tac_from_ssa"),
      // The passes of the driver's six lists, in its order.
      tac_pass("tac_rotate_signed_countdown_loops"),
      tac_pass("tac_noreturn_calls"),
      tac_pass("tac_fold_static_const_reads"),
      tac_pass("tac_constant_fold"),
      tac_pass("tac_distribute_constant"),
      tac_pass("tac_fold_into_constant_phi"),
      tac_pass("tac_reduce_strength"),
      tac_pass("tac_fold_producer_jumps"),
      tac_pass("tac_eliminate_dead_loops"),
      tac_pass("tac_thread_constant_jumps"),
      tac_pass("tac_eliminate_unreachable_code"),
      tac_pass("tac_eliminate_common_subexpressions"),
      tac_pass("tac_hoist_loop_invariants"),
      tac_pass("tac_copy_propagate"),
      tac_pass("tac_eliminate_dead_stores"),
      tac_pass("tac_read_stored_globals"),
      tac_pass("tac_fold_copies_in_fixedpoint"),
      tac_pass("tac_reassoc_constants"),
      tac_pass("tac_reassoc_offset_constant"),
      tac_pass("tac_direct_member_access"),
      tac_pass("tac_recognize_indexed_store"),
      tac_pass("tac_recognize_indexed_load"),
      tac_pass("tac_fold_truncate_extend"),
      tac_pass("tac_fold_extend_extend"),
      tac_pass("tac_sink_increments"),
      tac_pass("tac_sink_and_past_branch"),
      tac_pass("tac_narrow_widened_arith"),
      tac_pass("tac_narrow_byte_range"),
      tac_pass("tac_narrow_byte_bitwise"),
      tac_pass("tac_phi_offset"),
      tac_pass("tac_static_indexed_access"),
      tac_pass("tac_recognize_indirect_indexed"),
      tac_pass("tac_hoist_subtract_to_compare"),
      tac_pass("tac_hoist_divmod_pairs"),
      tac_pass("tac_count_loops_down"),
      tac_pass("tac_fold_copies_post_destruction"),
      tac_pass("tac_move_steps_onto_edges"),
      tac_pass("tac_split_three_way_compares"),
      tac_pass("tac_fold_short_circuit_jump"),
      tac_pass("tac_thread_known_compares"),
      // The program-level calls. `c99_to_tac` changes the c99 program
      // it is given, which its `mutated` line holds under `input`.
      {.name = "c99_to_tac",
       .input = "c99_ast",
       .args = {rt<Sym>, rt<Types>},
       .output_module = "tac_ast"},
      {.name = "inline_functions",
       .input = "tac_ast",
       .args = {rt<Sym>, rt<C99ProgramC>},
       .output_module = "tac_ast"},
      {.name = "return_pointers_in_zero_page",
       .input = "tac_ast",
       .args = {rt<Sym>},
       .output_module = "tac_ast"},
      {.name = "row_address_tables",
       .input = "tac_ast",
       .args = {rt<Sym>, rt<Types>},
       .output_module = "tac_ast"},
      {.name = "optimize_tac", .input = "tac_ast", .args = {rt<Sym>}, .output_module = "tac_ast"},
      {.name = "tac_optimize_function",
       .input = "tac_ast",
       .kwargs = {{"symbols", rt<Optional<Sym>>}},
       .output_module = "tac_ast"},
      {.name = "dispatch_const_pointer_arrays",
       .input = "tac_ast",
       .args = {rt<Sym>},
       .output_module = "tac_ast"},
      {.name = "split_tables", .input = "tac_ast", .args = {rt<Sym>}, .output_module = "tac_ast"},
  };
}

}  // namespace c6502::passes::tac_stage
