#include "passes/stage_registry_frames.h"

#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "passes/address_taken_zp.h"
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

// Returns the argument at position `index` when the call gave that many
// positional arguments, else the keyword argument `name`, or null when
// the call gave neither or gave None, which is each parameter's Python
// default here.
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

// Decodes the value `v` points to with `C`, or gives none for null.
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

StageOutput run_local_slot_names(const capture::Call& call, const AsmContext&,
                                 const json::Value& args, const json::Value& kwargs) {
  const auto pools = Pools::decode(json::parse(call.input));
  // Python's `colorings` has no default, and `compute_local_slot_names`
  // reads it with `.get`, so None would raise there.
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

}  // namespace c6502::passes::stage_frames
