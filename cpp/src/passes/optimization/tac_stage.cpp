#include "passes/optimization/tac_stage.h"

#include <stdexcept>
#include <string>
#include <utility>

#include "passes/optimization/constant_folding.h"
#include "passes/optimization/direct_member_access.h"
#include "passes/optimization/framework.h"
#include "passes/optimization/loop_rotate.h"
#include "passes/optimization/noreturn_call.h"
#include "passes/optimization/optimizer.h"
#include "passes/optimization/phi_offset.h"
#include "passes/optimization/recognize_indexed_load.h"
#include "passes/optimization/recognize_indexed_store.h"
#include "passes/optimization/recognize_indirect_indexed.h"
#include "passes/optimization/sink_and_past_branch.h"
#include "passes/optimization/sink_increment.h"
#include "passes/optimization/ssa_construction.h"
#include "passes/optimization/ssa_destruction.h"
#include "passes/optimization/static_indexed_access.h"
#include "passes/optimization/truncate_extend_fold.h"
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

// A pass of the TAC optimizer's driver, as the C++ port runs it: the
// function and the context, which it may add to.
using TacPass = ir::Function (*)(ir::Function fn, const PassContext& ctx);

const json::Value& kwarg(const json::Value& kwargs, std::string_view name) {
  const json::Value* v = kwargs.get(name);
  if (v == nullptr) {
    throw std::runtime_error("the call has no keyword argument " + std::string(name));
  }
  return *v;
}

// Runs the ported pass `Pass` on a recorded call of the TAC optimizer's
// driver, whose keyword arguments are the symbols the call read and the
// SSA names (tools/capture_passes.py, `_tac_record`), and gives the
// result's dump and the `mutated` line for the symbols the pass added
// and the SSA names it changed, in that order, as the harness writes
// it.
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

// Runs SSA construction on a recorded call, which the driver makes with
// the symbol table, and gives the function and its SSA names as the
// tagged JSON of Python's tuple, and the `mutated` line for the symbols
// it added.
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

// SSA destruction as a pass of the driver, which calls it with the
// symbol table.
ir::Function from_ssa_pass(ir::Function fn, const PassContext& ctx) {
  return from_ssa(std::move(fn), ctx.symbols);
}

// Runs the whole driver on a recorded call of `optimize_function`, whose
// keyword argument `symbols` is the compile's table, and gives the
// result's dump and the `mutated` line for the table.
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

// Runs the driver on each function of a recorded call of
// `optimize_program`, whose argument after the program is the
// compile's symbol table.
StageOutput run_optimize_program(const capture::Call& call, const AsmContext&,
                                 const json::Value& args, const json::Value&) {
  auto prog = asdl::load<ir::Program>(call.input);
  SymbolTable symbols = Sym::decode(array_items(args).at(0));
  const std::string symbols_before = encode<Sym>(symbols);
  ir::Program out = optimize_program(std::move(prog), &symbols);
  StageOutput result{asdl::dump(out), ""};
  const std::string symbols_after = encode<Sym>(symbols);
  if (symbols_after != symbols_before) {
    result.mutated = "{\"args\": {\"0\": " + symbols_after + "}, \"kwargs\": {}}";
  }
  return result;
}

// Returns `run` when every pass of the driver's table is ported, and
// null otherwise, for the calls that run the whole driver.
StageRun when_driver_complete(StageRun run) {
  return tac_driver().complete() ? run : nullptr;
}

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
       .output = rt<Pair<TacFunctionC, NameSetC>>,
       .run = &run_to_ssa},
      tac_pass("tac_from_ssa", &run_tac_pass<&from_ssa_pass>),
      // The passes of the driver's six lists, in its order.
      tac_pass("tac_rotate_signed_countdown_loops",
               &run_tac_pass<&rotate_signed_countdown_loops>),
      tac_pass("tac_noreturn_calls", &run_tac_pass<&noreturn_calls>),
      tac_pass("tac_fold_static_const_reads"),
      tac_pass("tac_constant_fold", &run_tac_pass<&constant_fold>),
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
      tac_pass("tac_direct_member_access", &run_tac_pass<&direct_member_access>),
      tac_pass("tac_recognize_indexed_store", &run_tac_pass<&recognize_indexed_store>),
      tac_pass("tac_recognize_indexed_load", &run_tac_pass<&recognize_indexed_load>),
      tac_pass("tac_fold_truncate_extend", &run_tac_pass<&fold_truncate_extend>),
      tac_pass("tac_fold_extend_extend", &run_tac_pass<&fold_extend_extend>),
      tac_pass("tac_sink_increments", &run_tac_pass<&sink_increments>),
      tac_pass("tac_sink_and_past_branch", &run_tac_pass<&sink_and_past_branch>),
      tac_pass("tac_narrow_widened_arith"),
      tac_pass("tac_narrow_byte_range"),
      tac_pass("tac_narrow_byte_bitwise"),
      tac_pass("tac_phi_offset", &run_tac_pass<&phi_offset>),
      tac_pass("tac_static_indexed_access", &run_tac_pass<&static_indexed_access>),
      tac_pass("tac_recognize_indirect_indexed", &run_tac_pass<&recognize_indirect_indexed>),
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
       .output_module = "tac_ast"},
      {.name = "split_tables", .input = "tac_ast", .args = {rt<Sym>}, .output_module = "tac_ast"},
  };
}

}  // namespace c6502::passes::tac_stage
