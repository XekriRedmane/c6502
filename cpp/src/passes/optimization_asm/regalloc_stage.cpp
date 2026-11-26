#include "passes/optimization_asm/regalloc_stage.h"

#include <optional>
#include <stdexcept>
#include <utility>

#include "passes/optimization_asm/interference.h"
#include "passes/optimization_asm/regalloc.h"
#include "tagged_json.h"

namespace c6502::passes {

LivenessRecord liveness_record(const Liveness& liveness) {
  LivenessRecord out;
  for (const auto& b : liveness.cfg.blocks) {
    LivenessRecord::Block rec{
        .id = b.id, .instructions = static_cast<long long>(liveness.cfg.instructions(b.id).size())};
    rec.predecessors.assign(b.predecessors.begin(), b.predecessors.end());
    rec.successors.assign(b.successors.begin(), b.successors.end());
    out.blocks.push_back(std::move(rec));
  }
  out.block_order.assign(liveness.cfg.block_order.begin(), liveness.cfg.block_order.end());
  for (std::size_t b = 0; b < liveness.cfg.blocks.size(); ++b) {
    out.live_in.insert_or_assign(static_cast<long long>(b), liveness.live_in[b]);
    out.live_out.insert_or_assign(static_cast<long long>(b), liveness.live_out[b]);
  }
  return out;
}

}  // namespace c6502::passes

namespace c6502::tagged {

namespace {

using json::Value;

// Writes `{"object": cls, "fields": {...}}`, one field at a time, with
// the spacing of the records in cpp/tests/frozen.
class Fields {
 public:
  Fields(std::string& out, std::string_view cls) : out_(out) {
    out_ += "{\"object\": ";
    write_string(out_, cls);
    out_ += ", \"fields\": {";
  }
  ~Fields() { out_ += "}}"; }
  std::string& field(std::string_view name) {
    if (!first_) {
      out_ += ", ";
    }
    first_ = false;
    write_string(out_, name);
    out_ += ": ";
    return out_;
  }

 private:
  std::string& out_;
  bool first_ = true;
};

constexpr std::string_view kLiveness = "passes.optimization_asm.liveness.Liveness";
constexpr std::string_view kNode = "passes.optimization.interference.InterferenceNode";
constexpr std::string_view kGraph = "passes.optimization.interference.InterferenceGraph";
constexpr std::string_view kCoalesce = "passes.optimization_asm.coalescing.CoalesceResult";
constexpr std::string_view kEligibility =
    "passes.optimization_asm.hwreg_eligibility.HwRegEligibility";

using Sets = DictOf<IntC, NameSetC>;
using StrPair = Pair<Str, Str>;

}  // namespace

LivenessC::type LivenessC::decode(const Value& v) {
  const Value& f = object_fields(v, kLiveness, {"blocks", "block_order", "live_in", "live_out"});
  passes::LivenessRecord out;
  for (const auto& item : array_items(f.items[0])) {
    const auto& t = tagged_items(item, "tuple");
    if (t.size() != 4) {
      throw DecodeError("a block of a Liveness isn't a tuple of four");
    }
    out.blocks.push_back({.id = IntC::decode(t[0]),
                          .instructions = IntC::decode(t[1]),
                          .predecessors = ListOf<IntC>::decode(t[2]),
                          .successors = ListOf<IntC>::decode(t[3])});
  }
  out.block_order = ListOf<IntC>::decode(f.items[1]);
  out.live_in = Sets::decode(f.items[2]);
  out.live_out = Sets::decode(f.items[3]);
  return out;
}

void LivenessC::encode(std::string& out, const type& x) {
  Fields w(out, kLiveness);
  std::string& b = w.field("blocks");
  b += '[';
  for (std::size_t i = 0; i < x.blocks.size(); ++i) {
    if (i != 0) {
      b += ", ";
    }
    const auto& blk = x.blocks[i];
    b += "{\"tuple\": [";
    IntC::encode(b, blk.id);
    b += ", ";
    IntC::encode(b, blk.instructions);
    b += ", ";
    ListOf<IntC>::encode(b, blk.predecessors);
    b += ", ";
    ListOf<IntC>::encode(b, blk.successors);
    b += "]}";
  }
  b += ']';
  ListOf<IntC>::encode(w.field("block_order"), x.block_order);
  Sets::encode(w.field("live_in"), x.live_in);
  Sets::encode(w.field("live_out"), x.live_out);
}

InterferenceNodeC::type InterferenceNodeC::decode(const Value& v) {
  const Value& f = object_fields(v, kNode, {"name", "width", "lives_across_call"});
  return passes::InterferenceNode{.name = Str::decode(f.items[0]),
                                  .width = IntC::decode(f.items[1]),
                                  .lives_across_call = BoolC::decode(f.items[2])};
}

void InterferenceNodeC::encode(std::string& out, const type& x) {
  Fields w(out, kNode);
  Str::encode(w.field("name"), x.name);
  IntC::encode(w.field("width"), x.width);
  BoolC::encode(w.field("lives_across_call"), x.lives_across_call);
}

InterferenceGraphC::type InterferenceGraphC::decode(const Value& v) {
  const Value& f = object_fields(v, kGraph, {"nodes", "adj"});
  passes::InterferenceGraph out;
  for (auto& [k, node] : DictOf<Str, InterferenceNodeC>::decode(f.items[0])) {
    out.nodes.emplace(k, std::move(node));
  }
  for (auto& [k, names] : DictOf<Str, NameSetC>::decode(f.items[1])) {
    out.adj.emplace(k, std::move(names));
  }
  return out;
}

void InterferenceGraphC::encode(std::string& out, const type& x) {
  // The maps are sorted by name, as the harness writes them.
  Fields w(out, kGraph);
  DictOf<Str, InterferenceNodeC>::type nodes;
  for (const auto& [k, node] : x.nodes) {
    nodes.insert_or_assign(k, node);
  }
  DictOf<Str, InterferenceNodeC>::encode(w.field("nodes"), nodes);
  DictOf<Str, NameSetC>::type adj;
  for (const auto& [k, names] : x.adj) {
    adj.insert_or_assign(k, names);
  }
  DictOf<Str, NameSetC>::encode(w.field("adj"), adj);
}

CoalesceResultC::type CoalesceResultC::decode(const Value& v) {
  const Value& f = object_fields(v, kCoalesce, {"representative", "byte_offset"});
  passes::CoalesceResult out;
  out.representative = DictOf<Str, Str>::decode(f.items[0]);
  for (auto& [k, off] : DictOf<Str, IntC>::decode(f.items[1])) {
    out.byte_offset.emplace(k, off);
  }
  return out;
}

void CoalesceResultC::encode(std::string& out, const type& x) {
  Fields w(out, kCoalesce);
  DictOf<Str, Str>::encode(w.field("representative"), x.representative);
  DictOf<Str, IntC>::type offsets;
  for (const auto& [k, off] : x.byte_offset) {
    offsets.insert_or_assign(k, off);
  }
  DictOf<Str, IntC>::encode(w.field("byte_offset"), offsets);
}

HwRegEligibilityC::type HwRegEligibilityC::decode(const Value& v) {
  const Value& f = object_fields(v, kEligibility,
                                 {"eligible_x", "eligible_y", "hints_x", "hints_y", "use_count",
                                  "required_x", "required_y", "hints_carry", "carry_pairs"});
  passes::HwRegEligibility out;
  out.eligible_x = NameSetC::decode(f.items[0]);
  out.eligible_y = NameSetC::decode(f.items[1]);
  out.hints_x = NameSetC::decode(f.items[2]);
  out.hints_y = NameSetC::decode(f.items[3]);
  out.use_count = DictOf<Str, IntC>::decode(f.items[4]);
  out.required_x = NameSetC::decode(f.items[5]);
  out.required_y = NameSetC::decode(f.items[6]);
  out.hints_carry = NameSetC::decode(f.items[7]);
  out.carry_pairs = ListOf<StrPair>::decode(f.items[8]);
  return out;
}

void HwRegEligibilityC::encode(std::string& out, const type& x) {
  Fields w(out, kEligibility);
  NameSetC::encode(w.field("eligible_x"), x.eligible_x);
  NameSetC::encode(w.field("eligible_y"), x.eligible_y);
  NameSetC::encode(w.field("hints_x"), x.hints_x);
  NameSetC::encode(w.field("hints_y"), x.hints_y);
  DictOf<Str, IntC>::encode(w.field("use_count"), x.use_count);
  NameSetC::encode(w.field("required_x"), x.required_x);
  NameSetC::encode(w.field("required_y"), x.required_y);
  NameSetC::encode(w.field("hints_carry"), x.hints_carry);
  ListOf<StrPair>::encode(w.field("carry_pairs"), x.carry_pairs);
}

}  // namespace c6502::tagged

namespace c6502::passes::regalloc_stage {

namespace {

using namespace ::c6502::tagged;
using json::Value;

asm_ast::Function load_function(const capture::Call& call) {
  auto tl = asdl::load<asm_ast::Type_top_level>(call.input);
  auto* fn = std::get_if<asm_ast::Function>(&tl);
  if (fn == nullptr) {
    throw std::runtime_error("the call's input isn't a Function");
  }
  return std::move(*fn);
}

const Value* kwarg(const Value& kwargs, std::string_view name) { return kwargs.get(name); }

const Value& positional(const Value& args, std::size_t i) {
  if (i >= args.items.size()) {
    throw std::runtime_error("the call has too few positional arguments");
  }
  return args.items[i];
}

// Returns the liveness of `fn` with the sets the record gives, after
// checking that the record's CFG is the one `build_cfg` gives `fn`.
Liveness recorded_liveness(const asm_ast::Function& fn, const LivenessRecord& rec) {
  Liveness live = liveness_with_sets(fn, {}, {});
  const std::size_t n = live.cfg.blocks.size();
  live.live_in.assign(n, NameSet{});
  live.live_out.assign(n, NameSet{});
  const LivenessRecord shape = liveness_record(live);
  if (shape.blocks != rec.blocks || shape.block_order != rec.block_order) {
    throw std::runtime_error("the recorded liveness's CFG isn't the function's");
  }
  for (const auto& [bid, names] : rec.live_in) {
    live.live_in.at(static_cast<std::size_t>(bid)) = names;
  }
  for (const auto& [bid, names] : rec.live_out) {
    live.live_out.at(static_cast<std::size_t>(bid)) = names;
  }
  return live;
}

}  // namespace

StageOutput run_compute_liveness(const capture::Call& call, const AsmContext&, const Value&,
                                 const Value&) {
  const auto fn = load_function(call);
  return {encode<LivenessC>(liveness_record(compute_liveness(fn))), ""};
}

StageOutput run_build_interference(const capture::Call& call, const AsmContext&,
                                   const Value& args, const Value& kwargs) {
  const auto fn = load_function(call);
  const Liveness live = recorded_liveness(fn, LivenessC::decode(positional(args, 0)));
  NameSet statics;
  if (const Value* s = kwarg(kwargs, "statics")) {
    statics = NameSetC::decode(*s);
  }
  return {encode<InterferenceGraphC>(build_interference(fn, live, statics)), ""};
}

StageOutput run_coalesce_moves(const capture::Call& call, const AsmContext&, const Value& args,
                               const Value&) {
  const auto fn = load_function(call);
  const Value& graph_arg = positional(args, 0);
  auto graph = InterferenceGraphC::decode(graph_arg);
  const std::string before = encode<InterferenceGraphC>(graph);
  const auto result = coalesce_moves(fn, graph);
  StageOutput out{encode<CoalesceResultC>(result), ""};
  const std::string after = encode<InterferenceGraphC>(graph);
  if (after != before) {
    out.mutated = "{\"args\": {\"0\": " + after + "}, \"kwargs\": {}}";
  }
  return out;
}

StageOutput run_hwreg_eligibility(const capture::Call& call, const AsmContext&, const Value&,
                                  const Value& kwargs) {
  const auto fn = load_function(call);
  using Pins = Optional<DictOf<Str, Str>>;
  Pins::type pins;
  Pins::type hints;
  if (const Value* p = kwarg(kwargs, "register_pins")) {
    pins = Pins::decode(*p);
  }
  if (const Value* h = kwarg(kwargs, "register_hints")) {
    hints = Pins::decode(*h);
  }
  const auto result =
      scan_function(fn, pins ? &*pins : nullptr, hints ? &*hints : nullptr);
  return {encode<HwRegEligibilityC>(result), ""};
}

StageOutput run_color_graph(const capture::Call& call, const AsmContext&, const Value& args,
                            const Value& kwargs) {
  const auto fn = load_function(call);
  const auto graph = InterferenceGraphC::decode(positional(args, 0));
  std::optional<Pool> pool;
  if (const Value* p = kwarg(kwargs, "pool")) {
    pool = Optional<PoolC>::decode(*p);
  }
  std::optional<std::set<asdl::Int, std::less<>>> blocked;
  if (const Value* b = kwarg(kwargs, "blocked_addrs")) {
    blocked = Optional<SetOf<IntC>>::decode(*b);
  }
  std::set<asdl::Int> blocked_plain;
  if (blocked) {
    blocked_plain.insert(blocked->begin(), blocked->end());
  }
  std::optional<HwRegEligibility> eligibility;
  if (const Value* e = kwarg(kwargs, "hwreg_eligibility")) {
    eligibility = Optional<HwRegEligibilityC>::decode(*e);
  }
  std::optional<Range> allowed;
  if (const Value* r = kwarg(kwargs, "allowed_range")) {
    allowed = Optional<RangeC>::decode(*r);
  }
  std::optional<Liveness> live;
  if (const Value* l = kwarg(kwargs, "liveness")) {
    if (const auto rec = Optional<LivenessC>::decode(*l)) {
      live = recorded_liveness(fn, *rec);
    }
  }
  std::optional<OrderedMap<std::string, std::string>> rep_map;
  if (const Value* m = kwarg(kwargs, "rep_map")) {
    rep_map = Optional<DictOf<Str, Str>>::decode(*m);
  }
  std::optional<NameSet> frame_names;
  if (const Value* f = kwarg(kwargs, "frame_names")) {
    frame_names = NameSetC::decode(*f);
  }
  if (eligibility.has_value() != live.has_value()) {
    throw std::runtime_error("the call gives hwreg_eligibility or liveness without the other");
  }
  std::optional<HwRegPinning> pinning;
  if (eligibility) {
    pinning.emplace(HwRegPinning{.eligibility = *eligibility, .liveness = *live});
  }
  const auto coloring =
      color_graph(fn, graph, pool, blocked ? &blocked_plain : nullptr,
                  pinning ? &*pinning : nullptr, allowed, rep_map ? &*rep_map : nullptr,
                  frame_names ? &*frame_names : nullptr);
  return {encode<ColoringC>(coloring), ""};
}

}  // namespace c6502::passes::regalloc_stage
