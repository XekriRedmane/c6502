#include "passes/stage_registry.h"

#include <stdexcept>

#include "passes/front_stage.h"
#include "passes/optimization/tac_stage.h"
#include "passes/optimization_asm/optimizer_stage.h"
#include "passes/optimization_asm/regalloc_stage.h"
#include "passes/optimization_asm/ssa_stage.h"
#include "passes/prologue_synthesis.h"
#include "passes/stage_registry_abi.h"
#include "passes/stage_registry_frames.h"
#include "tac_to_asm_stage.h"
#include "passes/stage_registry_sharing.h"
#include "tagged_json.h"

namespace c6502::passes {

namespace {

using namespace ::c6502::tagged;

// The forms of the values the stage calls take and return, as
// compile.py passes them.
using Sym = SymbolTableC;
using Types = TypeTableC;
using IntSet = SetOf<IntC>;
using OptRange = Optional<RangeC>;
using Addrs = DictOf<Str, IntC>;
using Pools = DictOf<Str, ListOf<IntC>>;
using Colorings = DictOf<Str, ColoringC>;
using Cycles = DictOf<Str, NameSetC>;
using Shares = ListOf<SlotShareC>;
using NamesByFn = DictOf<Str, NameSetC>;
using SlotAliases = DictOf<Str, Str>;
using Assignments = DictOf<Str, DictOf<Str, IntC>>;
using AssignmentSymbols = DictOf<Str, DictOf<Str, Str>>;
using SlotNames = DictOf<Str, ListOf<Str>>;
using ByteNames = DictOf<Str, DictOf<IntC, Str>>;
using Dims = DictOf<Str, FrameDimsC>;

template <class C>
constexpr RoundTrip rt = &round_trip<C>;

const json::Value& arg(const json::Value& args, std::size_t i) {
  if (i >= args.items.size()) {
    throw std::runtime_error("the call has too few positional arguments");
  }
  return args.items[i];
}

StageOutput run_synthesize_prologue(const capture::Call& call, const AsmContext&,
                                    const json::Value& args, const json::Value&) {
  auto prog = asdl::load<asm_ast::Program>(call.input);
  const auto dims = Dims::decode(arg(args, 0));
  return {asdl::dump(synthesize_program(std::move(prog), dims)), ""};
}

std::vector<StageSignature> make_signatures() {
  return {
      {.name = "plan_zero_page",
       .input = "tac_ast",
       .args = {rt<Sym>, rt<Types>},
       .kwargs = {{"promote", rt<BoolC>}},
       .output = rt<ZpGlobalsC>,
       .run = &stage_sharing::run_plan_zero_page},
      {.name = "select_abi",
       .input = "tac_ast",
       .args = {rt<C99ProgramC>, rt<Types>},
       .kwargs = {{"pool", rt<Optional<PoolC>>}},
       .output = rt<AbiC>,
       .run = &stage_abi::run_select_abi},
      {.name = "choose_global_carry_returns",
       .input = "tac_ast",
       .args = {rt<Sym>, rt<AbiC>},
       .output = rt<NameSetC>,
       .run = &stage_abi::run_choose_global_carry_returns},
      {.name = "choose_carry_returns",
       .input = "tac_ast",
       .args = {rt<Sym>},
       .output = rt<NameSetC>,
       .run = &stage_abi::run_choose_carry_returns},
      {.name = "recursive_pool_functions",
       .input = "tac_ast",
       .args = {rt<AbiC>},
       .output = rt<Cycles>,
       .run = &stage_abi::run_recursive_pool_functions},
      {.name = "pool_eligible",
       .input = "tac_ast",
       .args = {rt<AbiC>},
       .output = rt<NameSetC>,
       .run = &stage_abi::run_pool_eligible},
      // Group A's: `pinned_slot_symbols`, and the calls compile.py
      // doesn't make itself, recorded where their callers reach them.
      {.name = "pinned_slot_symbols",
       .input_json = rt<AbiC>,
       .output = rt<Addrs>,
       .run = &stage_abi::run_pinned_slot_symbols},
      {.name = "cycles",
       .input_json = rt<Cycles>,
       .output = rt<Cycles>,
       .run = &stage_abi::run_cycles},
      {.name = "hw_stack_cycles",
       .input = "tac_ast",
       .output = rt<Cycles>,
       .run = &stage_abi::run_hw_stack_cycles},
      {.name = "check_marked_calls",
       .input = "tac_ast",
       .output = rt<Optional<IntC>>,
       .run = &stage_abi::run_check_marked_calls},
      {.name = "plan_saves",
       .input = "tac_ast",
       .args = {rt<NameSetC>, rt<Sym>},
       .output = &stage_abi::round_trip_save_plan,
       .run = &stage_abi::run_plan_saves},
      {.name = "live_out",
       .input = "tac_ast",
       .output = rt<ListOf<NameSetC>>,
       .run = &stage_abi::run_live_out},
      {.name = "build_cfg",
       .input = "tac_ast",
       .output = &stage_abi::round_trip_cfg,
       .run = &stage_abi::run_build_cfg},
      {.name = "find_shares",
       .input = "tac_ast",
       .args = {rt<AbiC>, rt<Sym>, rt<Types>},
       .kwargs = {{"eligible", rt<NameSetC>},
                  {"fixed", rt<NameSetC>},
                  {"recursion", rt<NameSetC>}},
       .output = rt<Pair<Shares, BindingsC>>,
       .run = &stage_sharing::run_find_shares},
      {.name = "allocate_zp_slots",
       .input = "tac_ast",
       .args = {rt<AbiC>},
       .kwargs = {{"reserved", rt<IntSet>},
                  {"shares", rt<Shares>},
                  {"window", rt<OptRange>},
                  {"fallback", rt<OptRange>}},
       .output = rt<Pair<AbiC, Addrs>>,
       .run = &stage_sharing::run_allocate_zp_slots},
      {.name = "pinned_param_copies",
       .input = "tac_ast",
       .args = {rt<AbiC>, rt<Sym>, rt<Types>, rt<NameSetC>},
       .output = rt<NamesByFn>,
       .run = &stage_sharing::run_pinned_param_copies},
      {.name = "slot_aliases",
       .input_json = rt<Shares>,
       .output = rt<SlotAliases>,
       .run = &stage_sharing::run_slot_aliases},
      // Group B's calls after code generation, and `access_weights`,
      // where `plan_zero_page` calls it.
      {.name = "access_weights",
       .input = "tac_ast",
       .output = rt<Addrs>,
       .run = &stage_sharing::run_access_weights},
      {.name = "changed_param_slots",
       .input = "tac_ast",
       .args = {rt<AbiC>, rt<Sym>, rt<Types>},
       .output = rt<SlotNames>,
       .run = &stage_sharing::run_changed_param_slots},
      {.name = "preferred_symbols",
       .input_json = rt<BindingsC>,
       .output = rt<NamesByFn>,
       .run = &stage_sharing::run_preferred_symbols},
      {.name = "build_metadata",
       .input = "tac_ast",
       .args = {rt<AbiC>, rt<Pools>},
       .kwargs = {{"slot_names_by_fn", rt<Optional<SlotNames>>},
                  {"changes", rt<Optional<SlotNames>>},
                  {"carry", rt<Optional<NameSetC>>}},
       .output = &stage_sharing::round_trip_link_metadata,
       .run = &stage_sharing::run_build_metadata},
      {.name = "tac_to_asm",
       .input = "tac_ast",
       .args = {rt<Sym>, rt<Types>},
       .kwargs = {{"bare_exit", rt<BoolC>},
                  {"abi", rt<AbiC>},
                  {"recursion_saves", rt<Cycles>},
                  {"bindings", rt<BindingsC>},
                  {"pinned_copies", rt<NamesByFn>},
                  {"slot_aliases", rt<SlotAliases>}},
       .run = &tac_to_asm::run_tac_to_asm},
      {.name = "asm_optimizer",
       .input = "asm_ast",
       .kwargs = {{"extra_statics", rt<NameSetC>},
                  {"param_layouts", rt<AbiC>},
                  {"symbols", rt<Sym>},
                  {"local_pools", rt<Pools>},
                  {"reserved_zp", rt<IntSet>},
                  {"recursion_saves", rt<Cycles>}},
       .output = rt<Pair<AsmProgramC, Colorings>>,
       .run = &optimizer_stage::run_asm_optimizer},
      {.name = "compute_local_bytes",
       .input = "asm_ast",
       .output = rt<Addrs>,
       .run = &optimizer_stage::run_compute_local_bytes},
      {.name = "compute_address_taken_bytes",
       .input = "asm_ast",
       .args = {rt<Optional<Sym>>, rt<Optional<Types>>},
       .output = rt<Addrs>,
       .run = &optimizer_stage::run_compute_address_taken_bytes},
      {.name = "allocate_function_locals",
       .input = "tac_ast",
       .args = {rt<AbiC>, rt<Addrs>},
       .kwargs = {{"reserved", rt<IntSet>},
                  {"shares", rt<Shares>},
                  {"window", rt<OptRange>},
                  {"fallback", rt<OptRange>}},
       .output = rt<Pools>,
       .run = &stage_sharing::run_allocate_function_locals},
      {.name = "address_taken_assignments",
       .input = "asm_ast",
       .args = {rt<Pools>, rt<Colorings>, rt<Sym>, rt<Types>},
       .output = rt<Assignments>,
       .run = &stage_frames::run_address_taken_assignments},
      {.name = "address_taken_slot_symbols",
       .input_json = rt<Str>,
       .args = {rt<Addrs>, rt<Sym>, rt<Types>},
       .output = rt<DictOf<Str, Str>>,
       .run = &stage_frames::run_address_taken_slot_symbols},
      {.name = "replace_pseudoregisters_bare_exit",
       .input = "asm_ast",
       .kwargs = {{"extra_statics", rt<NameSetC>},
                  {"symbols", rt<Sym>},
                  {"types", rt<Types>},
                  {"colorings", rt<Colorings>},
                  {"param_layouts", rt<AbiC>},
                  {"local_pools", rt<Pools>},
                  {"address_taken_assignments", rt<Assignments>},
                  {"address_taken_symbols", rt<AssignmentSymbols>}},
       .output = rt<Pair<AsmProgramC, Dims>>,
       .run = &stage_frames::run_replace_pseudoregisters_bare_exit},
      {.name = "local_slot_names",
       .input_json = rt<Pools>,
       .args = {rt<Colorings>},
       .kwargs = {{"address_taken_assignments", rt<Assignments>},
                  {"address_taken_symbols", rt<AssignmentSymbols>}},
       .output = rt<SlotNames>,
       .run = &stage_frames::run_local_slot_names},
      {.name = "local_slot_symbols",
       .input_json = rt<Pools>,
       .kwargs = {{"slot_names_by_fn", rt<SlotNames>}},
       .output = rt<Addrs>,
       .run = &stage_sharing::run_local_slot_symbols},
      {.name = "lower_data_load_address",
       .input = "asm_ast",
       .kwargs = {{"slot_symbols", rt<Addrs>}, {"byte_names", rt<ByteNames>}},
       .run = &stage_frames::run_lower_data_load_address},
      {.name = "synthesize_prologue",
       .input = "asm_ast",
       .args = {rt<Dims>},
       .run = &run_synthesize_prologue},
      {.name = "fixed_address_layouts",
       .input = "tac_ast",
       .args = {rt<C99ProgramC>, rt<Types>},
       .kwargs = {{"pool", rt<Optional<PoolC>>}},
       .output = rt<AbiC>,
       .run = &stage_abi::run_fixed_address_layouts},
      {.name = "replace_pseudoregisters",
       .input = "asm_ast",
       .kwargs = {{"extra_statics", rt<NameSetC>},
                  {"symbols", rt<Sym>},
                  {"types", rt<Types>},
                  {"colorings", rt<Colorings>}},
       .run = &stage_frames::run_replace_pseudoregisters},
      // The asm SSA round trip, each on one asm function.
      {.name = "to_ssa",
       .input = "asm_ast",
       .kwargs = {{"statics", rt<NameSetC>}},
       .run = &ssa_stage::run_to_ssa},
      {.name = "copy_propagate",
       .input = "asm_ast",
       .kwargs = {{"statics", rt<NameSetC>}},
       .run = &ssa_stage::run_copy_propagate},
      {.name = "backward_copy_propagate",
       .input = "asm_ast",
       .kwargs = {{"statics", rt<NameSetC>}},
       .run = &ssa_stage::run_backward_copy_propagate},
      {.name = "byte_dce",
       .input = "asm_ast",
       .kwargs = {{"statics", rt<NameSetC>}},
       .run = &ssa_stage::run_byte_dce},
      {.name = "absorb_zero_load", .input = "asm_ast", .run = &ssa_stage::run_absorb_zero_load},
      {.name = "from_ssa", .input = "asm_ast", .run = &ssa_stage::run_from_ssa},
      {.name = "order_parallel_copies",
       .input_json = &ssa_stage::round_trip_instructions,
       .kwargs = {{"fn_name", rt<Str>}, {"cycle_counter", rt<ListOf<IntC>>}},
       .output = &ssa_stage::round_trip_instructions,
       .run = &ssa_stage::run_order_parallel_copies},
      // The register allocation of `_optimize_function`, whose runners
      // are in src/passes/optimization_asm/regalloc_stage.cpp.
      {.name = "compute_liveness",
       .input = "asm_ast",
       .output = rt<LivenessC>,
       .run = &regalloc_stage::run_compute_liveness},
      {.name = "build_interference",
       .input = "asm_ast",
       .args = {rt<LivenessC>},
       .kwargs = {{"statics", rt<NameSetC>}},
       .output = rt<InterferenceGraphC>,
       .run = &regalloc_stage::run_build_interference},
      {.name = "coalesce_moves",
       .input = "asm_ast",
       .args = {rt<InterferenceGraphC>},
       .output = rt<CoalesceResultC>,
       .run = &regalloc_stage::run_coalesce_moves},
      {.name = "hwreg_eligibility",
       .input = "asm_ast",
       .kwargs = {{"register_pins", rt<Optional<DictOf<Str, Str>>>},
                  {"register_hints", rt<Optional<DictOf<Str, Str>>>}},
       .output = rt<HwRegEligibilityC>,
       .run = &regalloc_stage::run_hwreg_eligibility},
      {.name = "color_graph",
       .input = "asm_ast",
       .args = {rt<InterferenceGraphC>},
       .kwargs = {{"pool", rt<Optional<PoolC>>},
                  {"blocked_addrs", rt<Optional<IntSet>>},
                  {"hwreg_eligibility", rt<Optional<HwRegEligibilityC>>},
                  {"allowed_range", rt<OptRange>},
                  {"liveness", rt<Optional<LivenessC>>},
                  {"rep_map", rt<Optional<DictOf<Str, Str>>>},
                  {"frame_names", rt<NameSetC>}},
       .output = rt<ColoringC>,
       .run = &regalloc_stage::run_color_graph},
      // The calls inside the asm optimizer's driver, whose runners are in
      // src/passes/optimization_asm/optimizer_stage.cpp.
      {.name = "optimize_function",
       .input = "asm_ast",
       .args = {rt<NameSetC>, rt<IntSet>, rt<OptRange>},
       .kwargs = {{"local_pool", rt<Optional<ListOf<IntC>>>},
                  {"local_pins", rt<Optional<DictOf<Str, Str>>>},
                  {"param_hints", rt<Optional<DictOf<Str, Str>>>},
                  {"entry_limits", rt<Optional<Cycles>>},
                  {"recursive_callees", rt<Optional<NameSetC>>},
                  {"params_in_frame", rt<BoolC>},
                  {"address_taken_in_frame", rt<BoolC>}},
       .output = &optimizer_stage::round_trip_optimized_function,
       .run = &optimizer_stage::run_optimize_function},
      {.name = "apply_coloring",
       .input = "asm_ast",
       .args = {rt<ColoringC>},
       .kwargs = {{"local_pool", rt<Optional<ListOf<IntC>>>}},
       .run = &optimizer_stage::run_apply_coloring},
      {.name = "fold_const_statics",
       .input = "asm_ast",
       .kwargs = {{"symbols", rt<Optional<Sym>>}},
       .run = &optimizer_stage::run_fold_const_statics},
      {.name = "dead_static_elimination",
       .input = "asm_ast",
       .run = &optimizer_stage::run_dead_static_elimination},
  };
}

}  // namespace

const std::vector<StageSignature>& stage_signatures() {
  static const std::vector<StageSignature> signatures = [] {
    // The calls of step 4, then those of step 5, then those of step 6.
    std::vector<StageSignature> all = make_signatures();
    for (auto& s : tac_stage::signatures()) {
      all.push_back(std::move(s));
    }
    for (auto& s : front_stage::signatures()) {
      all.push_back(std::move(s));
    }
    return all;
  }();
  return signatures;
}

const StageSignature* find_stage(std::string_view name) {
  for (const auto& s : stage_signatures()) {
    if (s.name == name) {
      return &s;
    }
  }
  return nullptr;
}

}  // namespace c6502::passes
