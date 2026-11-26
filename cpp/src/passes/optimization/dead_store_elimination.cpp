#include "passes/optimization/dead_store_elimination.h"

#include <optional>
#include <set>
#include <string>
#include <utility>

#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using framework::Instrs;
using framework::PassContext;
using K = ir::Kind_instruction;
using Names = std::set<std::string, std::less<>>;

Names collect_uses(const Instrs& instrs) {
  Names used;
  for (const auto& instr : instrs) {
    for (const std::string_view u : uses_in(instr)) {
      used.emplace(u);
    }
  }
  return used;
}

const ir::Type_val* pure_def_dst(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::Copy:
      return &std::get<ir::Copy>(instr).dst;
    case K::Unary:
      return &std::get<ir::Unary>(instr).dst;
    case K::Binary:
      return &std::get<ir::Binary>(instr).dst;
    case K::SignExtend:
      return &std::get<ir::SignExtend>(instr).dst;
    case K::ZeroExtend:
      return &std::get<ir::ZeroExtend>(instr).dst;
    case K::Truncate:
      return &std::get<ir::Truncate>(instr).dst;
    case K::IntToFloat:
      return &std::get<ir::IntToFloat>(instr).dst;
    case K::IntToDouble:
      return &std::get<ir::IntToDouble>(instr).dst;
    case K::FloatToInt:
      return &std::get<ir::FloatToInt>(instr).dst;
    case K::DoubleToInt:
      return &std::get<ir::DoubleToInt>(instr).dst;
    case K::FloatToDouble:
      return &std::get<ir::FloatToDouble>(instr).dst;
    case K::DoubleToFloat:
      return &std::get<ir::DoubleToFloat>(instr).dst;
    // A 6502 load never traps, so a non-volatile load whose destination
    // is dead goes, even through a wild pointer.
    case K::Load:
      return &std::get<ir::Load>(instr).dst;
    case K::GetAddress:
      return &std::get<ir::GetAddress>(instr).dst;
    case K::IndexedLoad:
      return &std::get<ir::IndexedLoad>(instr).dst;
    case K::Phi:
      return &std::get<ir::Phi>(instr).dst;
    default:
      return nullptr;
  }
}

bool is_volatile_load(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::Load:
      return std::get<ir::Load>(instr).is_volatile;
    case K::IndexedLoad:
      return std::get<ir::IndexedLoad>(instr).is_volatile;
    case K::IndexedConstLoad:
      return std::get<ir::IndexedConstLoad>(instr).is_volatile;
    case K::IndirectIndexedLoad:
      return std::get<ir::IndirectIndexedLoad>(instr).is_volatile;
    default:
      return false;
  }
}

bool dead(const ir::Type_val& dst, const Names& used, const NameSet& ssa) {
  const auto* v = std::get_if<ir::Var>(&dst);
  return v != nullptr && ssa.contains(v->name) && !used.contains(v->name);
}

struct Filtered {
  bool keep = true;
  bool changed = false;
};

Filtered filter(ir::Type_instruction& instr, const Names& used, const NameSet& ssa) {
  switch (ir::kind(instr)) {
    case K::Store:
    case K::Ret:
    case K::Jump:
    case K::JumpIfTrue:
    case K::JumpIfFalse:
    case K::JumpIfCmp:
    case K::JumpIfMasked:
    case K::Label:
      return {};
    default:
      break;
  }
  if (is_volatile_load(instr)) {
    return {};
  }
  if (const ir::Type_val* dst = pure_def_dst(instr)) {
    if (dead(*dst, used, ssa)) {
      return {.keep = false, .changed = true};
    }
    return {};
  }
  if (auto* call = std::get_if<ir::FunctionCall>(&instr)) {
    if (call->dst && dead(*call->dst, used, ssa)) {
      call->dst.reset();
      return {.keep = true, .changed = true};
    }
    return {};
  }
  if (auto* call = std::get_if<ir::IndirectCall>(&instr)) {
    if (call->dst && dead(*call->dst, used, ssa)) {
      call->dst.reset();
      return {.keep = true, .changed = true};
    }
    return {};
  }
  return {};
}

}  // namespace

tac_ast::Function eliminate_dead_stores(tac_ast::Function fn, const PassContext& ctx) {
  if (ctx.ssa_dsts == nullptr) {
    return fn;
  }
  Instrs instrs = std::move(fn.instructions);
  for (;;) {
    const Names used = collect_uses(instrs);
    Instrs out;
    out.reserve(instrs.size());
    bool changed = false;
    for (auto& instr : instrs) {
      const Filtered f = filter(instr, used, *ctx.ssa_dsts);
      changed = changed || f.changed;
      if (f.keep) {
        out.push_back(std::move(instr));
      }
    }
    instrs = std::move(out);
    if (!changed) {
      fn.instructions = std::move(instrs);
      return fn;
    }
  }
}

}  // namespace c6502::passes
