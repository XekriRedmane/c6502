#include "passes/stage_registry.h"

#include <stdexcept>

#include "passes/prologue_synthesis.h"
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
       .output = rt<ZpGlobalsC>},
      {.name = "select_abi",
       .input = "tac_ast",
       .args = {rt<C99ProgramC>, rt<Types>},
       .kwargs = {{"pool", rt<Optional<PoolC>>}},
       .output = rt<AbiC>},
      {.name = "choose_global_carry_returns",
       .input = "tac_ast",
       .args = {rt<Sym>, rt<AbiC>},
       .output = rt<NameSetC>},
      {.name = "choose_carry_returns",
       .input = "tac_ast",
       .args = {rt<Sym>},
       .output = rt<NameSetC>},
      {.name = "recursive_pool_functions",
       .input = "tac_ast",
       .args = {rt<AbiC>},
       .output = rt<Cycles>},
      {.name = "pool_eligible", .input = "tac_ast", .args = {rt<AbiC>}, .output = rt<NameSetC>},
      {.name = "find_shares",
       .input = "tac_ast",
       .args = {rt<AbiC>, rt<Sym>, rt<Types>},
       .kwargs = {{"eligible", rt<NameSetC>},
                  {"fixed", rt<NameSetC>},
                  {"recursion", rt<NameSetC>}},
       .output = rt<Pair<Shares, BindingsC>>},
      {.name = "allocate_zp_slots",
       .input = "tac_ast",
       .args = {rt<AbiC>},
       .kwargs = {{"reserved", rt<IntSet>},
                  {"shares", rt<Shares>},
                  {"window", rt<OptRange>},
                  {"fallback", rt<OptRange>}},
       .output = rt<Pair<AbiC, Addrs>>},
      {.name = "pinned_param_copies",
       .input = "tac_ast",
       .args = {rt<AbiC>, rt<Sym>, rt<Types>, rt<NameSetC>},
       .output = rt<NamesByFn>},
      {.name = "slot_aliases", .input_json = rt<Shares>, .output = rt<SlotAliases>},
      {.name = "tac_to_asm",
       .input = "tac_ast",
       .args = {rt<Sym>, rt<Types>},
       .kwargs = {{"bare_exit", rt<BoolC>},
                  {"abi", rt<AbiC>},
                  {"recursion_saves", rt<Cycles>},
                  {"bindings", rt<BindingsC>},
                  {"pinned_copies", rt<NamesByFn>},
                  {"slot_aliases", rt<SlotAliases>}}},
      {.name = "asm_optimizer",
       .input = "asm_ast",
       .kwargs = {{"extra_statics", rt<NameSetC>},
                  {"param_layouts", rt<AbiC>},
                  {"symbols", rt<Sym>},
                  {"local_pools", rt<Pools>},
                  {"reserved_zp", rt<IntSet>},
                  {"recursion_saves", rt<Cycles>}},
       .output = rt<Pair<AsmProgramC, Colorings>>},
      {.name = "compute_local_bytes", .input = "asm_ast", .output = rt<Addrs>},
      {.name = "compute_address_taken_bytes",
       .input = "asm_ast",
       .args = {rt<Sym>, rt<Types>},
       .output = rt<Addrs>},
      {.name = "allocate_function_locals",
       .input = "tac_ast",
       .args = {rt<AbiC>, rt<Addrs>},
       .kwargs = {{"reserved", rt<IntSet>},
                  {"shares", rt<Shares>},
                  {"window", rt<OptRange>},
                  {"fallback", rt<OptRange>}},
       .output = rt<Pools>},
      {.name = "address_taken_assignments",
       .input = "asm_ast",
       .args = {rt<Pools>, rt<Colorings>, rt<Sym>, rt<Types>},
       .output = rt<Assignments>},
      {.name = "address_taken_slot_symbols",
       .input_json = rt<Str>,
       .args = {rt<Addrs>, rt<Sym>, rt<Types>},
       .output = rt<DictOf<Str, Str>>},
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
       .output = rt<Pair<AsmProgramC, Dims>>},
      {.name = "local_slot_names",
       .input_json = rt<Pools>,
       .args = {rt<Colorings>},
       .kwargs = {{"address_taken_assignments", rt<Assignments>},
                  {"address_taken_symbols", rt<AssignmentSymbols>}},
       .output = rt<SlotNames>},
      {.name = "local_slot_symbols",
       .input_json = rt<Pools>,
       .kwargs = {{"slot_names_by_fn", rt<SlotNames>}},
       .output = rt<Addrs>},
      {.name = "lower_data_load_address",
       .input = "asm_ast",
       .kwargs = {{"slot_symbols", rt<Addrs>}, {"byte_names", rt<ByteNames>}}},
      {.name = "synthesize_prologue",
       .input = "asm_ast",
       .args = {rt<Dims>},
       .run = &run_synthesize_prologue},
      {.name = "fixed_address_layouts",
       .input = "tac_ast",
       .args = {rt<C99ProgramC>, rt<Types>},
       .output = rt<AbiC>},
      {.name = "replace_pseudoregisters",
       .input = "asm_ast",
       .kwargs = {{"extra_statics", rt<NameSetC>}, {"symbols", rt<Sym>}, {"types", rt<Types>}}},
  };
}

}  // namespace

const std::vector<StageSignature>& stage_signatures() {
  static const std::vector<StageSignature> signatures = make_signatures();
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
