#include "passes/stage_registry_abi.h"

#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

#include "passes/abi_selection.h"
#include "passes/carry_return_choice.h"
#include "passes/optimization/cfg.h"
#include "passes/recursion_saves.h"
#include "passes/zp_local_allocation.h"
#include "tagged_json.h"

namespace c6502::passes::stage_abi {

namespace {

using namespace ::c6502::tagged;

using CyclesC = DictOf<Str, NameSetC>;
using ValC = Asdl<tac_ast::Type_val, kTacAst>;
using InstrC = Asdl<tac_ast::Type_instruction, kTacAst>;
using SavedC = ListOf<Pair<Str, ValC>>;
using SavedByIndexC = DictOf<IntC, SavedC>;

const json::Value& arg(const json::Value& args, std::size_t i) {
  if (i >= args.items.size()) {
    throw std::runtime_error("the call has too few positional arguments");
  }
  return args.items[i];
}

const json::Value* kwarg(const json::Value& kwargs, std::string_view name) {
  const json::Value* v = kwargs.get(std::string(name));
  return v != nullptr && v->kind != json::Value::Kind::kNull ? v : nullptr;
}

std::optional<TypeTable> types_of(const json::Value& args, const json::Value& kwargs) {
  if (args.items.size() > 1) {
    return Optional<TypeTableC>::decode(args.items[1]);
  }
  if (const json::Value* v = kwarg(kwargs, "types")) {
    return TypeTableC::decode(*v);
  }
  return std::nullopt;
}

Pool pool_of(const json::Value& kwargs) {
  const json::Value* v = kwarg(kwargs, "pool");
  return v == nullptr ? Pool{} : PoolC::decode(*v);
}

std::string names(const NameSet& xs) { return encode<NameSetC>(xs); }

std::string cycles_text(const Cycles& c) { return encode<CyclesC>(c); }

std::string mutated_arg(std::size_t index, const std::string& text) {
  return "{\"args\": {\"" + std::to_string(index) + "\": " + text + "}, \"kwargs\": {}}";
}

struct SavePlanC {
  using type = SavePlan;
  static SavedByIndexC::type to_tagged(const OrderedMap<int, std::vector<SavedValue>>& m) {
    SavedByIndexC::type out;
    for (const auto& [k, v] : m) {
      out.insert_or_assign(k, v);
    }
    return out;
  }
  static OrderedMap<int, std::vector<SavedValue>> from_tagged(const SavedByIndexC::type& m) {
    OrderedMap<int, std::vector<SavedValue>> out;
    for (const auto& [k, v] : m) {
      out.insert_or_assign(static_cast<int>(k), v);
    }
    return out;
  }
  static type decode(const json::Value& v) {
    const json::Value& f = object_fields(v, "passes.recursion_saves.SavePlan",
                                         {"push_before", "push_after", "pop_after", "skip"});
    SavePlan plan;
    plan.push_before = from_tagged(SavedByIndexC::decode(*f.get("push_before")));
    plan.push_after = from_tagged(SavedByIndexC::decode(*f.get("push_after")));
    plan.pop_after = from_tagged(SavedByIndexC::decode(*f.get("pop_after")));
    for (const auto& i : SetOf<IntC>::decode(*f.get("skip"))) {
      plan.skip.insert(static_cast<int>(i));
    }
    return plan;
  }
  static void encode(std::string& out, const type& x) {
    out += "{\"object\": \"passes.recursion_saves.SavePlan\", \"fields\": {\"push_before\": ";
    SavedByIndexC::encode(out, to_tagged(x.push_before));
    out += ", \"push_after\": ";
    SavedByIndexC::encode(out, to_tagged(x.push_after));
    out += ", \"pop_after\": ";
    SavedByIndexC::encode(out, to_tagged(x.pop_after));
    out += ", \"skip\": ";
    SetOf<IntC>::type skip;
    for (const int i : x.skip) {
      skip.insert(i);
    }
    SetOf<IntC>::encode(out, skip);
    out += "}}";
  }
};

std::vector<int> ints(const std::vector<asdl::Int>& xs) {
  std::vector<int> out;
  for (const auto x : xs) {
    out.push_back(static_cast<int>(x));
  }
  return out;
}

std::vector<asdl::Int> big_ints(const std::vector<int>& xs) {
  return {xs.begin(), xs.end()};
}

struct CfgC {
  using type = tac_cfg::Cfg;
  static type decode(const json::Value& v) {
    const json::Value& f =
        object_fields(v, "passes.optimization.cfg.CFG", {"blocks", "block_order"});
    tac_cfg::Cfg cfg;
    for (const auto& kv : tagged_items(*f.get("blocks"), "dict")) {
      const auto& pair = array_items(kv);
      const json::Value& b = object_fields(pair.at(1), "passes.optimization.cfg.BasicBlock",
                                           {"id", "instructions", "predecessors", "successors"});
      tac_cfg::BasicBlock block{
          .id = static_cast<int>(IntC::decode(*b.get("id"))),
          .instructions = ListOf<InstrC>::decode(*b.get("instructions")),
          .predecessors = ints(ListOf<IntC>::decode(*b.get("predecessors"))),
          .successors = ints(ListOf<IntC>::decode(*b.get("successors")))};
      cfg.blocks.insert_or_assign(static_cast<int>(IntC::decode(pair.at(0))), std::move(block));
    }
    cfg.block_order = ints(ListOf<IntC>::decode(*f.get("block_order")));
    return cfg;
  }
  static void encode(std::string& out, const type& x) {
    out += "{\"object\": \"passes.optimization.cfg.CFG\", \"fields\": {\"blocks\": {\"dict\": [";
    bool first = true;
    for (const auto& [id, b] : x.blocks) {
      if (!first) {
        out += ", ";
      }
      first = false;
      out += '[';
      IntC::encode(out, id);
      out += ", {\"object\": \"passes.optimization.cfg.BasicBlock\", \"fields\": {\"id\": ";
      IntC::encode(out, b.id);
      out += ", \"instructions\": ";
      ListOf<InstrC>::encode(out, b.instructions);
      out += ", \"predecessors\": ";
      ListOf<IntC>::encode(out, big_ints(b.predecessors));
      out += ", \"successors\": ";
      ListOf<IntC>::encode(out, big_ints(b.successors));
      out += "}}]";
    }
    out += "]}, \"block_order\": ";
    ListOf<IntC>::encode(out, big_ints(x.block_order));
    out += "}}";
  }
};

tac_ast::Program tac_program(const capture::Call& call) {
  return asdl::load<tac_ast::Program>(call.input);
}

tac_ast::Function tac_function(const capture::Call& call) {
  return asdl::load<tac_ast::Function>(call.input);
}

}  // namespace

StageOutput run_select_abi(const capture::Call& call, const AsmContext&, const json::Value& args,
                           const json::Value& kwargs) {
  const auto prog = tac_program(call);
  const auto c99_prog = C99ProgramC::decode(arg(args, 0));
  const auto types = types_of(args, kwargs);
  const Abi abi = select_abi(prog, c99_prog, types ? &*types : nullptr, pool_of(kwargs));
  return {encode<AbiC>(abi), ""};
}

StageOutput run_fixed_address_layouts(const capture::Call& call, const AsmContext&,
                                      const json::Value& args, const json::Value& kwargs) {
  const auto prog = tac_program(call);
  const auto c99_prog = C99ProgramC::decode(arg(args, 0));
  const auto types = types_of(args, kwargs);
  Abi abi;
  for (auto& [name, layout] :
       fixed_address_layouts(prog, c99_prog, types ? &*types : nullptr, pool_of(kwargs))) {
    abi.insert_or_assign(name, layout);
  }
  return {encode<AbiC>(abi), ""};
}

StageOutput run_pinned_slot_symbols(const capture::Call& call, const AsmContext&,
                                    const json::Value&, const json::Value&) {
  const Abi abi = AbiC::decode(json::parse(call.input));
  return {encode<DictOf<Str, IntC>>(pinned_slot_symbols(abi)), ""};
}

StageOutput run_choose_global_carry_returns(const capture::Call& call, const AsmContext&,
                                            const json::Value& args, const json::Value&) {
  const auto prog = tac_program(call);
  auto symbols = SymbolTableC::decode(arg(args, 0));
  const Abi abi = AbiC::decode(arg(args, 1));
  const NameSet chosen = choose_global_carry_returns(prog, symbols, abi);
  return {names(chosen), chosen.empty() ? "" : mutated_arg(0, encode<SymbolTableC>(symbols))};
}

StageOutput run_choose_carry_returns(const capture::Call& call, const AsmContext&,
                                     const json::Value& args, const json::Value&) {
  const auto prog = tac_program(call);
  auto symbols = SymbolTableC::decode(arg(args, 0));
  const NameSet chosen = choose_carry_returns(prog, symbols);
  return {names(chosen), chosen.empty() ? "" : mutated_arg(0, encode<SymbolTableC>(symbols))};
}

StageOutput run_recursive_pool_functions(const capture::Call& call, const AsmContext&,
                                         const json::Value& args, const json::Value&) {
  return {cycles_text(recursive_pool_functions(tac_program(call), AbiC::decode(arg(args, 0)))), ""};
}

StageOutput run_pool_eligible(const capture::Call& call, const AsmContext&,
                              const json::Value& args, const json::Value&) {
  return {names(pool_eligible(tac_program(call), AbiC::decode(arg(args, 0)))), ""};
}

StageOutput run_cycles(const capture::Call& call, const AsmContext&, const json::Value&,
                       const json::Value&) {
  CallGraph graph;
  for (auto& [name, callees] : CyclesC::decode(json::parse(call.input))) {
    graph.emplace(name, callees);
  }
  return {cycles_text(cycles(graph)), ""};
}

StageOutput run_hw_stack_cycles(const capture::Call& call, const AsmContext&, const json::Value&,
                                const json::Value&) {
  return {cycles_text(hw_stack_cycles(tac_program(call))), ""};
}

StageOutput run_check_marked_calls(const capture::Call& call, const AsmContext&,
                                   const json::Value&, const json::Value&) {
  check_marked_calls(tac_program(call));
  return {"null", ""};
}

StageOutput run_plan_saves(const capture::Call& call, const AsmContext&, const json::Value& args,
                           const json::Value&) {
  const auto fn = tac_function(call);
  const NameSet members = NameSetC::decode(arg(args, 0));
  const SymbolTable symbols = SymbolTableC::decode(arg(args, 1));
  return {encode<SavePlanC>(plan_saves(fn, members, symbols)), ""};
}

StageOutput run_live_out(const capture::Call& call, const AsmContext&, const json::Value&,
                         const json::Value&) {
  return {encode<ListOf<NameSetC>>(recursion_saves::live_out(tac_function(call))), ""};
}

StageOutput run_build_cfg(const capture::Call& call, const AsmContext&, const json::Value&,
                          const json::Value&) {
  return {encode<CfgC>(tac_cfg::build_cfg(tac_function(call))), ""};
}

std::string round_trip_save_plan(const json::Value& v) { return round_trip<SavePlanC>(v); }

std::string round_trip_cfg(const json::Value& v) { return round_trip<CfgC>(v); }

}  // namespace c6502::passes::stage_abi
