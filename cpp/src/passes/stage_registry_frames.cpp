#include "passes/stage_registry_frames.h"

#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "passes/address_taken_zp.h"
#include "passes/lower_data_load_address.h"
#include "passes/replace_pseudoregisters.h"
#include "passes/zp_slot_naming.h"
#include "tagged_json.h"

namespace c6502::passes::stage_frames {

namespace {

using namespace ::c6502::tagged;

using Addrs = DictOf<Str, IntC>;
using Pools = DictOf<Str, ListOf<IntC>>;
using Colorings = DictOf<Str, ColoringC>;
using Assignments = DictOf<Str, DictOf<Str, IntC>>;
using AssignmentSymbols = DictOf<Str, DictOf<Str, Str>>;
using SlotNames = DictOf<Str, ListOf<Str>>;
using ByteNames = DictOf<Str, DictOf<IntC, Str>>;
using Dims = DictOf<Str, FrameDimsC>;

const json::Value* param(const json::Value& args, std::size_t index, const json::Value& kwargs,
                         std::string_view name) {
  const json::Value* v = nullptr;
  if (index < args.items.size()) {
    v = &args.items[index];
  } else {
    v = kwargs.get(std::string(name));
  }
  return v != nullptr && v->kind != json::Value::Kind::kNull ? v : nullptr;
}

template <class C>
std::optional<typename C::type> decode_opt(const json::Value* v) {
  if (v == nullptr) {
    return std::nullopt;
  }
  return C::decode(*v);
}

template <class T>
const T* ptr(const std::optional<T>& x) {
  return x ? &*x : nullptr;
}

asm_ast::Program asm_program(const capture::Call& call) {
  return asdl::load<asm_ast::Program>(call.input);
}

}  // namespace

StageOutput run_address_taken_assignments(const capture::Call& call, const AsmContext&,
                                          const json::Value& args, const json::Value& kwargs) {
  const auto prog = asm_program(call);
  const auto pools = decode_opt<Pools>(param(args, 0, kwargs, "local_pools"));
  const auto colorings = decode_opt<Colorings>(param(args, 1, kwargs, "colorings"));
  const auto symbols = decode_opt<SymbolTableC>(param(args, 2, kwargs, "symbols"));
  const auto types = decode_opt<TypeTableC>(param(args, 3, kwargs, "types"));
  const auto out = compute_address_taken_assignments(prog, pools.value_or(LocalPools{}),
                                                     colorings.value_or(ColoringsByFunction{}),
                                                     ptr(symbols), ptr(types));
  return {encode<Assignments>(out), ""};
}

StageOutput run_address_taken_slot_symbols(const capture::Call& call, const AsmContext&,
                                           const json::Value& args, const json::Value& kwargs) {
  const std::string fn_name = Str::decode(json::parse(call.input));
  const auto assignments = decode_opt<Addrs>(param(args, 0, kwargs, "assignments"));
  const auto symbols = decode_opt<SymbolTableC>(param(args, 1, kwargs, "symbols"));
  const auto types = decode_opt<TypeTableC>(param(args, 2, kwargs, "types"));
  const auto out = address_taken_zp::slot_symbols(
      fn_name, assignments.value_or(OrderedMap<std::string, asdl::Int>{}), ptr(symbols),
      ptr(types));
  return {encode<DictOf<Str, Str>>(out), ""};
}

StageOutput run_replace_pseudoregisters_bare_exit(const capture::Call& call, const AsmContext&,
                                                  const json::Value& args,
                                                  const json::Value& kwargs) {
  auto prog = asm_program(call);
  const auto statics = decode_opt<NameSetC>(param(args, 0, kwargs, "extra_statics"));
  const auto symbols = decode_opt<SymbolTableC>(param(args, 1, kwargs, "symbols"));
  const auto types = decode_opt<TypeTableC>(param(args, 2, kwargs, "types"));
  const auto colorings = decode_opt<Colorings>(param(args, 3, kwargs, "colorings"));
  const auto layouts = decode_opt<AbiC>(param(args, 4, kwargs, "param_layouts"));
  const auto pools = decode_opt<Pools>(param(args, 5, kwargs, "local_pools"));
  const auto assignments =
      decode_opt<Assignments>(param(args, 6, kwargs, "address_taken_assignments"));
  const auto slot_symbols =
      decode_opt<AssignmentSymbols>(param(args, 7, kwargs, "address_taken_symbols"));
  const BareExitTables tables{.param_layouts = ptr(layouts),
                              .local_pools = ptr(pools),
                              .address_taken_assignments = ptr(assignments),
                              .address_taken_symbols = ptr(slot_symbols)};
  auto result = replace_program_bare_exit(std::move(prog), statics.value_or(NameSet{}),
                                          ptr(symbols), ptr(types), ptr(colorings), tables);
  return {encode<Pair<AsmProgramC, Dims>>(result), ""};
}

StageOutput run_local_slot_names(const capture::Call& call, const AsmContext&,
                                 const json::Value& args, const json::Value& kwargs) {
  const auto pools = Pools::decode(json::parse(call.input));
  const json::Value* colorings_json = param(args, 0, kwargs, "colorings");
  if (colorings_json == nullptr) {
    throw std::runtime_error("compute_local_slot_names was given no colorings");
  }
  const auto colorings = Colorings::decode(*colorings_json);
  const auto assignments =
      decode_opt<Assignments>(param(args, 1, kwargs, "address_taken_assignments"));
  const auto slot_symbols =
      decode_opt<AssignmentSymbols>(param(args, 2, kwargs, "address_taken_symbols"));
  return {encode<SlotNames>(
              compute_local_slot_names(pools, colorings, ptr(assignments), ptr(slot_symbols))),
          ""};
}

StageOutput run_lower_data_load_address(const capture::Call& call, const AsmContext&,
                                        const json::Value&, const json::Value& kwargs) {
  auto prog = asm_program(call);
  const auto slot_symbols = decode_opt<Addrs>(param(json::Value{}, 0, kwargs, "slot_symbols"));
  const auto byte_names = decode_opt<ByteNames>(param(json::Value{}, 0, kwargs, "byte_names"));
  return {asdl::dump(lower_data_load_address(std::move(prog), ptr(slot_symbols), ptr(byte_names))),
          ""};
}

StageOutput run_replace_pseudoregisters(const capture::Call& call, const AsmContext&,
                                        const json::Value& args, const json::Value& kwargs) {
  auto prog = asm_program(call);
  const auto statics = decode_opt<NameSetC>(param(args, 0, kwargs, "extra_statics"));
  const auto symbols = decode_opt<SymbolTableC>(param(args, 1, kwargs, "symbols"));
  const auto types = decode_opt<TypeTableC>(param(args, 2, kwargs, "types"));
  const auto colorings = decode_opt<Colorings>(param(args, 3, kwargs, "colorings"));
  return {asdl::dump(replace_program(std::move(prog), statics.value_or(NameSet{}), ptr(symbols),
                                     ptr(types), ptr(colorings))),
          ""};
}

}  // namespace c6502::passes::stage_frames
