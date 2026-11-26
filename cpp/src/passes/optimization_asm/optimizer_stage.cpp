// The runners of the asm optimizer's stage calls. Each decodes the
// recorded arguments with the codecs of src/tagged_json.h, where an
// argument the record gives as null, or leaves out, is null or absent,
// and runs the C++ function.
#include "passes/optimization_asm/optimizer_stage.h"

#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "asm_ast.h"
#include "passes/function_local_sizing.h"
#include "passes/optimization_asm/apply_coloring.h"
#include "passes/optimization_asm/const_static_fold.h"
#include "passes/optimization_asm/dead_static.h"
#include "passes/optimization_asm/optimizer.h"
#include "tagged_json.h"

namespace c6502::passes::optimizer_stage {

namespace {

namespace ir = ::c6502::asm_ast;
using namespace ::c6502::tagged;
using json::Value;

using FunctionC = Asdl<ir::Type_top_level, kAsmAst>;
using Pools = DictOf<Str, ListOf<IntC>>;
using Cycles = DictOf<Str, NameSetC>;
using Hints = DictOf<Str, Str>;

ir::Function load_function(const capture::Call& call) {
  auto tl = asdl::load<ir::Type_top_level>(call.input);
  auto* fn = std::get_if<ir::Function>(&tl);
  if (fn == nullptr) {
    throw std::runtime_error("the call's input isn't a Function");
  }
  return std::move(*fn);
}

std::string dump_function(ir::Function fn) {
  return asdl::dump(ir::Type_top_level{std::move(fn)});
}

const Value& positional(const Value& args, std::size_t i) {
  if (i >= args.items.size()) {
    throw std::runtime_error("the call has too few positional arguments");
  }
  return args.items[i];
}

bool is_null(const Value* v) { return v == nullptr || v->kind == Value::Kind::kNull; }

// Returns the codec's value of `v`, or nothing when `v` is absent or
// null.
template <class C>
std::optional<typename C::type> maybe(const Value* v) {
  if (is_null(v)) {
    return std::nullopt;
  }
  return C::decode(*v);
}

template <class T>
const T* ptr(const std::optional<T>& v) {
  return v ? &*v : nullptr;
}

std::set<asdl::Int> plain_ints(const std::optional<std::set<asdl::Int, std::less<>>>& v) {
  std::set<asdl::Int> out;
  if (v) {
    out.insert(v->begin(), v->end());
  }
  return out;
}

}  // namespace

std::string round_trip_optimized_function(const Value& v) {
  return round_trip<Pair<FunctionC, ColoringC>>(v);
}

StageOutput run_asm_optimizer(const capture::Call& call, const AsmContext&, const Value&,
                              const Value& kwargs) {
  auto prog = asdl::load<ir::Program>(call.input);
  const auto extra = maybe<NameSetC>(kwargs.get("extra_statics"));
  const auto layouts = maybe<AbiC>(kwargs.get("param_layouts"));
  const auto symbols = maybe<SymbolTableC>(kwargs.get("symbols"));
  const auto pools = maybe<Pools>(kwargs.get("local_pools"));
  const auto reserved = maybe<SetOf<IntC>>(kwargs.get("reserved_zp"));
  const auto recursion = maybe<Cycles>(kwargs.get("recursion_saves"));
  AsmOptimizerOptions options;
  if (extra) {
    options.extra_statics = *extra;
  }
  options.param_layouts = ptr(layouts);
  options.symbols = ptr(symbols);
  options.local_pools = ptr(pools);
  options.reserved_zp = plain_ints(reserved);
  options.recursion_saves = ptr(recursion);
  OptimizedAsmProgram r = optimize_program(std::move(prog), options);
  return {encode<Pair<AsmProgramC, DictOf<Str, ColoringC>>>({std::move(r.prog), std::move(r.colorings)}),
          ""};
}

StageOutput run_compute_local_bytes(const capture::Call& call, const AsmContext&, const Value&,
                                    const Value&) {
  const auto prog = asdl::load<ir::Program>(call.input);
  return {encode<DictOf<Str, IntC>>(compute_local_bytes(prog)), ""};
}

StageOutput run_compute_address_taken_bytes(const capture::Call& call, const AsmContext&,
                                            const Value& args, const Value& kwargs) {
  const auto prog = asdl::load<ir::Program>(call.input);
  const Value* sym_arg = args.items.size() > 0 ? &args.items[0] : kwargs.get("symbols");
  const Value* types_arg = args.items.size() > 1 ? &args.items[1] : kwargs.get("types");
  const auto symbols = maybe<SymbolTableC>(sym_arg);
  const auto types = maybe<TypeTableC>(types_arg);
  return {encode<DictOf<Str, IntC>>(compute_address_taken_bytes(prog, ptr(symbols), ptr(types))),
          ""};
}

StageOutput run_optimize_function(const capture::Call& call, const AsmContext&, const Value& args,
                                  const Value& kwargs) {
  auto fn = load_function(call);
  const auto statics = NameSetC::decode(positional(args, 0));
  const auto blocked = plain_ints(SetOf<IntC>::decode(positional(args, 1)));
  const Value* range_arg = args.items.size() > 2 ? &args.items[2] : kwargs.get("allowed_range");
  const Value* pool_arg = args.items.size() > 3 ? &args.items[3] : kwargs.get("local_pool");
  const auto allowed = maybe<RangeC>(range_arg);
  const auto pool = maybe<ListOf<IntC>>(pool_arg);
  const auto pins = maybe<Hints>(kwargs.get("local_pins"));
  const auto hints = maybe<Hints>(kwargs.get("param_hints"));
  const auto limits = maybe<Cycles>(kwargs.get("entry_limits"));
  const auto callees = maybe<NameSetC>(kwargs.get("recursive_callees"));
  const Value* params_arg = kwargs.get("params_in_frame");
  const Value* taken_arg = kwargs.get("address_taken_in_frame");
  const AsmFunctionOptions options{
      .local_pins = ptr(pins),
      .param_hints = ptr(hints),
      .entry_limits = ptr(limits),
      .recursive_callees = ptr(callees),
      .params_in_frame = params_arg == nullptr || BoolC::decode(*params_arg),
      .address_taken_in_frame = taken_arg == nullptr || BoolC::decode(*taken_arg)};
  OptimizedAsmFunction r = optimize_function(std::move(fn), statics, blocked, allowed, ptr(pool), options);
  return {encode<Pair<FunctionC, ColoringC>>({ir::Type_top_level{std::move(r.fn)}, std::move(r.coloring)}),
          ""};
}

StageOutput run_apply_coloring(const capture::Call& call, const AsmContext&, const Value& args,
                               const Value& kwargs) {
  auto fn = load_function(call);
  const Coloring coloring = ColoringC::decode(positional(args, 0));
  const Value* pool_arg = args.items.size() > 1 ? &args.items[1] : kwargs.get("local_pool");
  const auto pool = maybe<ListOf<IntC>>(pool_arg);
  return {dump_function(apply_coloring(std::move(fn), coloring, ptr(pool))), ""};
}

StageOutput run_fold_const_statics(const capture::Call& call, const AsmContext&, const Value&,
                                   const Value& kwargs) {
  auto prog = asdl::load<ir::Program>(call.input);
  const auto symbols = maybe<SymbolTableC>(kwargs.get("symbols"));
  return {asdl::dump(fold_const_statics(std::move(prog), ptr(symbols))), ""};
}

StageOutput run_dead_static_elimination(const capture::Call& call, const AsmContext&, const Value&,
                                        const Value&) {
  auto prog = asdl::load<ir::Program>(call.input);
  return {asdl::dump(apply_dead_static_elimination(std::move(prog))), ""};
}

}  // namespace c6502::passes::optimizer_stage
