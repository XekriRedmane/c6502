#include "passes/optimization/tac_stage.h"

#include <stdexcept>
#include <string>
#include <utility>

#include "c99_to_tac_stage.h"
#include "passes/optimization/compare_subtract.h"
#include "passes/optimization/constant_folding.h"
#include "passes/optimization/copy_folding.h"
#include "passes/optimization/copy_propagation.h"
#include "passes/optimization/count_down.h"
#include "passes/optimization/cse.h"
#include "passes/optimization/dead_loop_elimination.h"
#include "passes/optimization/dead_store_elimination.h"
#include "passes/optimization/direct_member_access.h"
#include "passes/optimization/dispatch_pointer_array.h"
#include "passes/optimization/distribute_constant.h"
#include "passes/optimization/divmod_hoist.h"
#include "passes/optimization/fold_producer_jumps.h"
#include "passes/optimization/framework.h"
#include "passes/optimization/inline_functions.h"
#include "passes/optimization/jump_thread.h"
#include "passes/optimization/known_compare_thread.h"
#include "passes/optimization/licm.h"
#include "passes/optimization/loop_rotate.h"
#include "passes/optimization/narrow_byte_bitwise.h"
#include "passes/optimization/narrow_byte_range.h"
#include "passes/optimization/narrow_widened_arith.h"
#include "passes/optimization/noreturn_call.h"
#include "passes/optimization/optimizer.h"
#include "passes/optimization/phi_constant_fold.h"
#include "passes/optimization/phi_offset.h"
#include "passes/optimization/read_stored_global.h"
#include "passes/optimization/reassoc_const.h"
#include "passes/optimization/reassoc_offset.h"
#include "passes/optimization/recognize_indexed_load.h"
#include "passes/optimization/recognize_indexed_store.h"
#include "passes/optimization/recognize_indirect_indexed.h"
#include "passes/optimization/row_address_tables.h"
#include "passes/optimization/short_circuit_jump_fold.h"
#include "passes/optimization/sink_and_past_branch.h"
#include "passes/optimization/sink_increment.h"
#include "passes/optimization/split_tables.h"
#include "passes/optimization/ssa_construction.h"
#include "passes/optimization/ssa_destruction.h"
#include "passes/optimization/static_const_fold.h"
#include "passes/optimization/static_indexed_access.h"
#include "passes/optimization/step_on_edge.h"
#include "passes/optimization/strength_reduction.h"
#include "passes/optimization/three_way_compare.h"
#include "passes/optimization/truncate_extend_fold.h"
#include "passes/optimization/unreachable_code_elimination.h"
#include "passes/return_in_zero_page.h"
#include "tagged_json.h"

namespace c6502::passes::tac_stage {

namespace {

namespace ir = ::c6502::tac_ast;
using namespace ::c6502::tagged;
using framework::PassContext;

using Sym = SymbolTableC;
using Types = TypeTableC;
using TacFunctionC = Asdl<ir::Type_top_level, kTacAst>;

template <class C>
constexpr RoundTrip rt = &round_trip<C>;

using TacPass = ir::Function (*)(ir::Function fn, const PassContext& ctx);

const json::Value& kwarg(const json::Value& kwargs, std::string_view name) {
  const json::Value* v = kwargs.get(name);
  if (v == nullptr) {
    throw std::runtime_error("the call has no keyword argument " + std::string(name));
  }
  return *v;
}

template <TacPass Pass>
StageOutput run_tac_pass(const capture::Call& call, const AsmContext&, const json::Value&,
                         const json::Value& kwargs) {
  auto fn = std::get<ir::Function>(asdl::load<ir::Type_top_level>(call.input));
  std::optional<SymbolTable> symbols = Optional<Sym>::decode(kwarg(kwargs, "symbols"));
  std::optional<NameSet> ssa = Optional<NameSetC>::decode(kwarg(kwargs, "ssa_dsts"));
  const std::string symbols_before = encode<Optional<Sym>>(symbols);
  const std::string ssa_before = encode<Optional<NameSetC>>(ssa);
  const PassContext ctx{
      .symbols = symbols ? &*symbols : nullptr,
      .ssa_dsts = ssa ? &*ssa : nullptr,
  };
  ir::Function out = Pass(std::move(fn), ctx);
  StageOutput result{asdl::dump(ir::Type_top_level{std::move(out)}), ""};
  std::string changed;
  const std::string symbols_after = encode<Optional<Sym>>(symbols);
  if (symbols_after != symbols_before) {
    changed += "\"symbols\": " + symbols_after;
  }
  const std::string ssa_after = encode<Optional<NameSetC>>(ssa);
  if (ssa_after != ssa_before) {
    if (!changed.empty()) {
      changed += ", ";
    }
    changed += "\"ssa_dsts\": " + ssa_after;
  }
  if (!changed.empty()) {
    result.mutated = "{\"args\": {}, \"kwargs\": {" + changed + "}}";
  }
  return result;
}

StageOutput run_to_ssa(const capture::Call& call, const AsmContext&, const json::Value&,
                       const json::Value& kwargs) {
  auto fn = std::get<ir::Function>(asdl::load<ir::Type_top_level>(call.input));
  std::optional<SymbolTable> symbols = Optional<Sym>::decode(kwarg(kwargs, "symbols"));
  if (!symbols) {
    throw std::runtime_error("to_ssa is called without a symbol table");
  }
  const std::string symbols_before = encode<Sym>(*symbols);
  auto [out, ssa] = to_ssa(std::move(fn), *symbols);
  StageOutput result{
      encode<Pair<TacFunctionC, NameSetC>>({ir::Type_top_level{std::move(out)}, std::move(ssa)}),
      ""};
  const std::string symbols_after = encode<Sym>(*symbols);
  if (symbols_after != symbols_before) {
    result.mutated = "{\"args\": {}, \"kwargs\": {\"symbols\": " + symbols_after + "}}";
  }
  return result;
}

ir::Function from_ssa_pass(ir::Function fn, const PassContext& ctx) {
  return from_ssa(std::move(fn), ctx.symbols);
}

StageOutput run_optimize_function(const capture::Call& call, const AsmContext&,
                                  const json::Value&, const json::Value& kwargs) {
  auto fn = std::get<ir::Function>(asdl::load<ir::Type_top_level>(call.input));
  std::optional<SymbolTable> symbols = Optional<Sym>::decode(kwarg(kwargs, "symbols"));
  const std::string symbols_before = encode<Optional<Sym>>(symbols);
  ir::Function out = optimize_function(std::move(fn), symbols ? &*symbols : nullptr);
  StageOutput result{asdl::dump(ir::Type_top_level{std::move(out)}), ""};
  const std::string symbols_after = encode<Optional<Sym>>(symbols);
  if (symbols_after != symbols_before) {
    result.mutated = "{\"args\": {}, \"kwargs\": {\"symbols\": " + symbols_after + "}}";
  }
  return result;
}

StageOutput run_optimize_program(const capture::Call& call, const AsmContext&,
                                 const json::Value& args, const json::Value&) {
  auto prog = asdl::load<ir::Program>(call.input);
  const auto& items = array_items(args);
  if (items.empty()) {
    return StageOutput{asdl::dump(optimize_program(std::move(prog), nullptr)), ""};
  }
  SymbolTable symbols = Sym::decode(items.at(0));
  const std::string symbols_before = encode<Sym>(symbols);
  ir::Program out = optimize_program(std::move(prog), &symbols);
  StageOutput result{asdl::dump(out), ""};
  const std::string symbols_after = encode<Sym>(symbols);
  if (symbols_after != symbols_before) {
    result.mutated = "{\"args\": {\"0\": " + symbols_after + "}, \"kwargs\": {}}";
  }
  return result;
}

StageOutput program_output(const ir::Program& out, const std::string& symbols_before,
                           const SymbolTable& symbols) {
  StageOutput result{asdl::dump(out), ""};
  const std::string symbols_after = encode<Sym>(symbols);
  if (symbols_after != symbols_before) {
    result.mutated = "{\"args\": {\"0\": " + symbols_after + "}, \"kwargs\": {}}";
  }
  return result;
}

StageOutput run_inline_functions(const capture::Call& call, const AsmContext&,
                                 const json::Value& args, const json::Value&) {
  auto prog = asdl::load<ir::Program>(call.input);
  SymbolTable symbols = Sym::decode(array_items(args).at(0));
  const c99_ast::Program c99 = C99ProgramC::decode(array_items(args).at(1));
  const std::string before = encode<Sym>(symbols);
  return program_output(inline_functions(std::move(prog), symbols, c99), before, symbols);
}

StageOutput run_return_in_zero_page(const capture::Call& call, const AsmContext&,
                                    const json::Value& args, const json::Value&) {
  auto prog = asdl::load<ir::Program>(call.input);
  SymbolTable symbols = Sym::decode(array_items(args).at(0));
  const std::string before = encode<Sym>(symbols);
  return program_output(return_pointers_in_zero_page(std::move(prog), symbols), before, symbols);
}

StageOutput run_row_address_tables(const capture::Call& call, const AsmContext&,
                                   const json::Value& args, const json::Value& kwargs) {
  auto prog = asdl::load<ir::Program>(call.input);
  const auto& items = array_items(args);
  SymbolTable symbols = Sym::decode(items.at(0));
  const std::optional<TypeTable> types =
      items.size() > 1 ? Optional<Types>::decode(items[1]) : std::nullopt;
  std::optional<asdl::Int> fixed_saving;
  if (const json::Value* s = kwargs.get("saving")) {
    fixed_saving = IntC::decode(*s);
  }
  const std::string before = encode<Sym>(symbols);
  return program_output(apply_row_address_tables(std::move(prog), &symbols,
                                                 types ? &*types : nullptr, fixed_saving),
                        before, symbols);
}

StageOutput run_dispatch(const capture::Call& call, const AsmContext&, const json::Value& args,
                         const json::Value&) {
  auto prog = asdl::load<ir::Program>(call.input);
  const auto& items = array_items(args);
  if (items.empty()) {
    return StageOutput{asdl::dump(dispatch_const_pointer_arrays(std::move(prog), nullptr)), ""};
  }
  const SymbolTable symbols = Sym::decode(items.at(0));
  const std::string before = encode<Sym>(symbols);
  return program_output(dispatch_const_pointer_arrays(std::move(prog), &symbols), before,
                        symbols);
}

StageOutput run_split_tables(const capture::Call& call, const AsmContext&,
                             const json::Value& args, const json::Value&) {
  auto prog = asdl::load<ir::Program>(call.input);
  SymbolTable symbols = Sym::decode(array_items(args).at(0));
  const std::string before = encode<Sym>(symbols);
  return program_output(apply_split_tables(std::move(prog), &symbols), before, symbols);
}

StageRun when_driver_complete(StageRun run) {
  return tac_driver().complete() ? run : nullptr;
}

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
      {.name = "tac_to_ssa",
       .input = "tac_ast",
       .kwargs = {{"symbols", rt<Optional<Sym>>}, {"ssa_dsts", rt<Optional<NameSetC>>}},
       .output = rt<Pair<TacFunctionC, NameSetC>>,
       .run = &run_to_ssa},
      tac_pass("tac_from_ssa", &run_tac_pass<&from_ssa_pass>),
      tac_pass("tac_rotate_signed_countdown_loops",
               &run_tac_pass<&rotate_signed_countdown_loops>),
      tac_pass("tac_noreturn_calls", &run_tac_pass<&noreturn_calls>),
      tac_pass("tac_fold_static_const_reads", &run_tac_pass<&fold_static_const_reads>),
      tac_pass("tac_constant_fold", &run_tac_pass<&constant_fold>),
      tac_pass("tac_distribute_constant", &run_tac_pass<&distribute_constant>),
      tac_pass("tac_fold_into_constant_phi", &run_tac_pass<&fold_into_constant_phis>),
      tac_pass("tac_reduce_strength", &run_tac_pass<&reduce_strength>),
      tac_pass("tac_fold_producer_jumps", &run_tac_pass<&fold_producer_jumps>),
      tac_pass("tac_eliminate_dead_loops", &run_tac_pass<&eliminate_dead_loops>),
      tac_pass("tac_thread_constant_jumps", &run_tac_pass<&thread_constant_jumps>),
      tac_pass("tac_eliminate_unreachable_code", &run_tac_pass<&eliminate_unreachable_code>),
      tac_pass("tac_eliminate_common_subexpressions",
               &run_tac_pass<&eliminate_common_subexpressions>),
      tac_pass("tac_hoist_loop_invariants", &run_tac_pass<&hoist_loop_invariants>),
      tac_pass("tac_copy_propagate", &run_tac_pass<&copy_propagate>),
      tac_pass("tac_eliminate_dead_stores", &run_tac_pass<&eliminate_dead_stores>),
      tac_pass("tac_read_stored_globals", &run_tac_pass<&read_stored_globals>),
      tac_pass("tac_fold_copies_in_fixedpoint", &run_tac_pass<&fold_copies_in_fixedpoint>),
      tac_pass("tac_reassoc_constants", &run_tac_pass<&reassoc_constants>),
      tac_pass("tac_reassoc_offset_constant", &run_tac_pass<&reassoc_offset_constant>),
      tac_pass("tac_direct_member_access", &run_tac_pass<&direct_member_access>),
      tac_pass("tac_recognize_indexed_store", &run_tac_pass<&recognize_indexed_store>),
      tac_pass("tac_recognize_indexed_load", &run_tac_pass<&recognize_indexed_load>),
      tac_pass("tac_fold_truncate_extend", &run_tac_pass<&fold_truncate_extend>),
      tac_pass("tac_fold_extend_extend", &run_tac_pass<&fold_extend_extend>),
      tac_pass("tac_sink_increments", &run_tac_pass<&sink_increments>),
      tac_pass("tac_sink_and_past_branch", &run_tac_pass<&sink_and_past_branch>),
      tac_pass("tac_narrow_widened_arith", &run_tac_pass<&apply_narrow_widened_arith>),
      tac_pass("tac_narrow_byte_range", &run_tac_pass<&narrow_byte_range>),
      tac_pass("tac_narrow_byte_bitwise", &run_tac_pass<&narrow_byte_bitwise>),
      tac_pass("tac_phi_offset", &run_tac_pass<&phi_offset>),
      tac_pass("tac_static_indexed_access", &run_tac_pass<&static_indexed_access>),
      tac_pass("tac_recognize_indirect_indexed", &run_tac_pass<&recognize_indirect_indexed>),
      tac_pass("tac_hoist_subtract_to_compare", &run_tac_pass<&hoist_subtract_to_compare>),
      tac_pass("tac_hoist_divmod_pairs", &run_tac_pass<&hoist_divmod_pairs>),
      tac_pass("tac_count_loops_down", &run_tac_pass<&count_loops_down>),
      tac_pass("tac_fold_copies_post_destruction", &run_tac_pass<&fold_copies_post_destruction>),
      tac_pass("tac_move_steps_onto_edges", &run_tac_pass<&move_steps_onto_edges>),
      tac_pass("tac_split_three_way_compares", &run_tac_pass<&split_three_way_compares>),
      tac_pass("tac_fold_short_circuit_jump", &run_tac_pass<&fold_short_circuit_jumps>),
      tac_pass("tac_thread_known_compares", &run_tac_pass<&thread_known_compares>),
      {.name = "c99_to_tac",
       .input = "c99_ast",
       .args = {rt<Sym>, rt<Optional<Types>>},
       .output_module = "tac_ast",
       .run = &c99_to_tac::run_c99_to_tac},
      {.name = "inline_functions",
       .input = "tac_ast",
       .args = {rt<Sym>, rt<C99ProgramC>},
       .output_module = "tac_ast",
       .run = &run_inline_functions},
      {.name = "return_pointers_in_zero_page",
       .input = "tac_ast",
       .args = {rt<Sym>},
       .output_module = "tac_ast",
       .run = &run_return_in_zero_page},
      {.name = "row_address_tables",
       .input = "tac_ast",
       .args = {rt<Sym>, rt<Types>},
       .kwargs = {{"saving", rt<IntC>}},
       .output_module = "tac_ast",
       .run = &run_row_address_tables},
      {.name = "optimize_tac",
       .input = "tac_ast",
       .args = {rt<Sym>},
       .output_module = "tac_ast",
       .run = when_driver_complete(&run_optimize_program)},
      {.name = "tac_optimize_function",
       .input = "tac_ast",
       .kwargs = {{"symbols", rt<Optional<Sym>>}},
       .output_module = "tac_ast",
       .run = when_driver_complete(&run_optimize_function)},
      {.name = "dispatch_const_pointer_arrays",
       .input = "tac_ast",
       .args = {rt<Sym>},
       .output_module = "tac_ast",
       .run = &run_dispatch},
      {.name = "split_tables",
       .input = "tac_ast",
       .args = {rt<Sym>},
       .output_module = "tac_ast",
       .run = &run_split_tables},
  };
}

}  // namespace c6502::passes::tac_stage
