#include "passes/stage_registry_sharing.h"

#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

#include "passes/emission_args.h"
#include "passes/slot_sharing.h"
#include "passes/zp_globals.h"
#include "passes/zp_link_metadata.h"
#include "passes/zp_local_allocation.h"
#include "passes/zp_slot_naming.h"
#include "passes/zp_slot_allocation.h"
#include "tagged_json.h"

namespace c6502::passes::stage_sharing {

namespace {

using namespace ::c6502::tagged;

using Addrs = DictOf<Str, IntC>;
using Pools = DictOf<Str, ListOf<IntC>>;
using Shares = ListOf<SlotShareC>;
using SlotNames = DictOf<Str, ListOf<Str>>;
using NamesByFn = DictOf<Str, NameSetC>;

const json::Value& arg(const json::Value& args, std::size_t i) {
  if (i >= args.items.size()) {
    throw std::runtime_error("the call has too few positional arguments");
  }
  return args.items[i];
}

// Returns the keyword argument `name`, or null when the call wasn't
// given it or was given None.
const json::Value* kwarg(const json::Value& kwargs, std::string_view name) {
  const json::Value* v = kwargs.get(name);
  return v != nullptr && v->kind != json::Value::Kind::kNull ? v : nullptr;
}

// Returns the type table given as positional argument `index` or as the
// keyword `types`, or none, as Python's default is None.
std::optional<TypeTable> types_of(const json::Value& args, std::size_t index,
                                  const json::Value& kwargs) {
  if (args.items.size() > index) {
    return Optional<TypeTableC>::decode(args.items[index]);
  }
  if (const json::Value* v = kwarg(kwargs, "types")) {
    return TypeTableC::decode(*v);
  }
  return std::nullopt;
}

const TypeTable* ptr(const std::optional<TypeTable>& t) { return t ? &*t : nullptr; }

tac_ast::Program tac_program(const capture::Call& call) {
  return asdl::load<tac_ast::Program>(call.input);
}

// Returns the options of an allocator from its keyword arguments.
ZpAllocationOptions allocation_options(const json::Value& kwargs) {
  ZpAllocationOptions options;
  if (const json::Value* v = kwarg(kwargs, "pool")) {
    options.pool = PoolC::decode(*v);
  }
  if (const json::Value* v = kwarg(kwargs, "spill_start")) {
    options.spill_start = IntC::decode(*v);
  }
  if (const json::Value* v = kwarg(kwargs, "spill_end")) {
    options.spill_end = IntC::decode(*v);
  }
  if (const json::Value* v = kwarg(kwargs, "reserved")) {
    for (const auto& b : SetOf<IntC>::decode(*v)) {
      options.reserved.insert(b);
    }
  }
  if (const json::Value* v = kwarg(kwargs, "window")) {
    options.window = RangeC::decode(*v);
  }
  if (const json::Value* v = kwarg(kwargs, "fallback")) {
    options.fallback = RangeC::decode(*v);
  }
  if (const json::Value* v = kwarg(kwargs, "shares")) {
    // The harness records a tuple of shares as a tuple, and a list as a
    // list.
    options.shares = v->get("tuple") != nullptr ? TupleOf<SlotShareC>::decode(*v)
                                                : Shares::decode(*v);
  }
  return options;
}

NameSet name_set(const json::Value& kwargs, std::string_view name) {
  const json::Value* v = kwarg(kwargs, name);
  return v == nullptr ? NameSet{} : NameSetC::decode(*v);
}

const char* kFunctionMeta = "passes.zp_link_metadata.FunctionMeta";
const char* kExternMeta = "passes.zp_link_metadata.ExternMeta";
const char* kLinkMetadata = "passes.zp_link_metadata.LinkMetadata";

// The codec of a `LinkMetadata`, whose fields and those of its records
// are in the dataclasses' order.
struct LinkMetadataC {
  using type = LinkMetadata;
  using Pinned = ListOf<Optional<IntC>>;
  static type decode(const json::Value& v) {
    if (object_class(v) != kLinkMetadata) {
      throw DecodeError("expected a LinkMetadata");
    }
    return emission_args::link_metadata(v);
  }
  static void encode(std::string& out, const type& x) {
    out += "{\"object\": \"";
    out += kLinkMetadata;
    out += "\", \"fields\": {\"defs\": [";
    for (std::size_t i = 0; i < x.defs.size(); ++i) {
      const FunctionMeta& d = x.defs[i];
      if (i != 0) {
        out += ", ";
      }
      out += "{\"object\": \"";
      out += kFunctionMeta;
      out += "\", \"fields\": {\"name\": ";
      Str::encode(out, d.name);
      out += ", \"params\": ";
      ListOf<Str>::encode(out, d.params);
      out += ", \"locals\": ";
      ListOf<Str>::encode(out, d.locals);
      out += ", \"indirect\": ";
      BoolC::encode(out, d.indirect);
      out += ", \"in_cycle\": ";
      BoolC::encode(out, d.in_cycle);
      out += ", \"param_regs\": ";
      ListOf<Str>::encode(out, d.param_regs);
      out += ", \"pinned\": ";
      Pinned::encode(out, d.pinned);
      out += ", \"changes\": ";
      ListOf<Str>::encode(out, d.changes);
      out += ", \"carry\": ";
      BoolC::encode(out, d.carry);
      out += "}}";
    }
    out += "], \"externs\": [";
    for (std::size_t i = 0; i < x.externs.size(); ++i) {
      const ExternMeta& e = x.externs[i];
      if (i != 0) {
        out += ", ";
      }
      out += "{\"object\": \"";
      out += kExternMeta;
      out += "\", \"fields\": {\"name\": ";
      Str::encode(out, e.name);
      out += ", \"params\": ";
      ListOf<Str>::encode(out, e.params);
      out += ", \"param_regs\": ";
      ListOf<Str>::encode(out, e.param_regs);
      out += ", \"pinned\": ";
      Pinned::encode(out, e.pinned);
      out += ", \"carry\": ";
      BoolC::encode(out, e.carry);
      out += "}}";
    }
    out += "], \"calls\": ";
    ListOf<Pair<Str, Str>>::encode(out, x.calls);
    out += ", \"saved_calls\": ";
    ListOf<Pair<Str, Str>>::encode(out, x.saved_calls);
    out += ", \"zp_objects\": ";
    ListOf<ZpObjectMetaC>::encode(out, x.zp_objects);
    out += ", \"hargs_size\": ";
    Optional<IntC>::encode(out, x.hargs_size);
    out += ", \"hargs_need\": ";
    Optional<IntC>::encode(out, x.hargs_need);
    out += ", \"items\": ";
    out += emission_args::items_json(x.items);
    out += ", \"shares\": ";
    Shares::encode(out, x.shares);
    out += "}}";
  }
};

}  // namespace

StageOutput run_plan_zero_page(const capture::Call& call, const AsmContext&,
                               const json::Value& args, const json::Value& kwargs) {
  const auto prog = tac_program(call);
  const SymbolTable symbols = SymbolTableC::decode(arg(args, 0));
  const auto types = types_of(args, 1, kwargs);
  const json::Value* promote = kwarg(kwargs, "promote");
  // Python measures the HARGS bytes of `tac_to_asm`'s translation of the
  // program when the runtime's block has to move. Until group F connects
  // group C's port of `tac_to_asm` here, the measure is the HARGS size
  // the recorded result holds, which is that measure or 1 when the
  // program uses none, and `choose_layout` makes the same size of both.
  const HargsNeed need = [&call]() -> asdl::Int {
    if (!call.output_is_json || call.output.empty()) {
      throw std::runtime_error(
          "plan_zero_page needs the HARGS bytes of tac_to_asm's translation, and the call "
          "records no result to take them from");
    }
    return ZpGlobalsC::decode(json::parse(call.output)).layout.hargs_size;
  };
  const ZpGlobals zp =
      plan_zero_page(prog, symbols, ptr(types), promote != nullptr && promote->boolean, need);
  return {encode<ZpGlobalsC>(zp), ""};
}

StageOutput run_access_weights(const capture::Call& call, const AsmContext&, const json::Value&,
                               const json::Value&) {
  return {encode<Addrs>(access_weights(tac_program(call))), ""};
}

StageOutput run_find_shares(const capture::Call& call, const AsmContext&, const json::Value& args,
                            const json::Value& kwargs) {
  const auto prog = tac_program(call);
  const Abi abi = AbiC::decode(arg(args, 0));
  const SymbolTable symbols = SymbolTableC::decode(arg(args, 1));
  const auto types = types_of(args, 2, kwargs);
  const auto [shares, bindings] =
      find_shares(prog, abi, symbols, ptr(types), name_set(kwargs, "eligible"),
                  name_set(kwargs, "fixed"), name_set(kwargs, "recursion"));
  return {encode<Pair<Shares, BindingsC>>({shares, bindings}), ""};
}

StageOutput run_allocate_zp_slots(const capture::Call& call, const AsmContext&,
                                  const json::Value& args, const json::Value& kwargs) {
  const auto prog = tac_program(call);
  const Abi abi = AbiC::decode(arg(args, 0));
  auto result = allocate_zp_slots(prog, abi, allocation_options(kwargs));
  return {encode<Pair<AbiC, Addrs>>(result), ""};
}

StageOutput run_pinned_param_copies(const capture::Call& call, const AsmContext&,
                                    const json::Value& args, const json::Value& kwargs) {
  const auto prog = tac_program(call);
  const Abi abi = AbiC::decode(arg(args, 0));
  const SymbolTable symbols = SymbolTableC::decode(arg(args, 1));
  const auto types = types_of(args, 2, kwargs);
  const NameSet addressed = NameSetC::decode(arg(args, 3));
  return {encode<NamesByFn>(pinned_param_copies(prog, abi, symbols, ptr(types), addressed)), ""};
}

StageOutput run_slot_aliases(const capture::Call& call, const AsmContext&, const json::Value&,
                             const json::Value&) {
  const json::Value v = json::parse(call.input);
  const std::vector<SlotShare> shares =
      v.get("tuple") != nullptr ? TupleOf<SlotShareC>::decode(v) : Shares::decode(v);
  return {encode<DictOf<Str, Str>>(slot_aliases(shares)), ""};
}

StageOutput run_allocate_function_locals(const capture::Call& call, const AsmContext&,
                                         const json::Value& args, const json::Value& kwargs) {
  const auto prog = tac_program(call);
  const Abi abi = AbiC::decode(arg(args, 0));
  const auto local_bytes = Addrs::decode(arg(args, 1));
  return {encode<Pools>(allocate_function_locals(prog, abi, local_bytes, allocation_options(kwargs))),
          ""};
}

StageOutput run_local_slot_symbols(const capture::Call& call, const AsmContext&,
                                   const json::Value& args, const json::Value& kwargs) {
  const auto pools = Pools::decode(json::parse(call.input));
  const json::Value* names = kwarg(kwargs, "slot_names_by_fn");
  if (names == nullptr) {
    // Without the names, Python names the slots from the colorings
    // (`colorings or {}`) with `compute_local_slot_names`.
    const json::Value* colorings =
        !args.items.empty() ? &args.items[0] : kwargs.get(std::string("colorings"));
    ColoringsByFunction by_fn;
    if (colorings != nullptr && colorings->kind != json::Value::Kind::kNull) {
      by_fn = DictOf<Str, ColoringC>::decode(*colorings);
    }
    return {encode<Addrs>(local_slots(pools, by_fn).slot_symbols), ""};
  }
  std::optional<SlotNames::type> by_fn;
  if (names != nullptr) {
    by_fn = SlotNames::decode(*names);
  }
  return {encode<Addrs>(build_local_slot_symbols(pools, by_fn ? &*by_fn : nullptr)), ""};
}

StageOutput run_changed_param_slots(const capture::Call& call, const AsmContext&,
                                    const json::Value& args, const json::Value& kwargs) {
  const auto prog = tac_program(call);
  const Abi abi = AbiC::decode(arg(args, 0));
  const SymbolTable symbols = SymbolTableC::decode(arg(args, 1));
  const auto types = types_of(args, 2, kwargs);
  return {encode<SlotNames>(changed_param_slots(prog, abi, symbols, ptr(types))), ""};
}

StageOutput run_preferred_symbols(const capture::Call& call, const AsmContext&,
                                  const json::Value&, const json::Value&) {
  const Bindings bindings = BindingsC::decode(json::parse(call.input));
  return {encode<NamesByFn>(preferred_symbols(bindings)), ""};
}

StageOutput run_build_metadata(const capture::Call& call, const AsmContext&,
                               const json::Value& args, const json::Value& kwargs) {
  const auto prog = tac_program(call);
  const Abi abi = AbiC::decode(arg(args, 0));
  const auto pools = Pools::decode(arg(args, 1));
  std::optional<SlotNames::type> names;
  if (const json::Value* v = kwarg(kwargs, "slot_names_by_fn")) {
    names = SlotNames::decode(*v);
  }
  std::optional<SlotNames::type> changes;
  if (const json::Value* v = kwarg(kwargs, "changes")) {
    changes = SlotNames::decode(*v);
  }
  std::optional<NameSet> carry;
  if (const json::Value* v = kwarg(kwargs, "carry")) {
    carry = NameSetC::decode(*v);
  }
  const LinkMetadata meta = build_metadata(prog, abi, pools, names ? &*names : nullptr,
                                           changes ? &*changes : nullptr,
                                           carry ? &*carry : nullptr);
  return {encode<LinkMetadataC>(meta), ""};
}

std::string round_trip_link_metadata(const json::Value& v) { return round_trip<LinkMetadataC>(v); }

}  // namespace c6502::passes::stage_sharing
