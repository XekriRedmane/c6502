/* TAC dead-store elimination in SSA form.

In SSA form every Var has exactly one definition, so whether a
definition is live comes down to whether its Var is read anywhere. The
pass walks the function:

  1. It collects the names read anywhere (Phi sources, operands, return
     values, call arguments).
  2. It drops or simplifies each instruction whose destination isn't
     read. A pure definition (`Copy`, `Binary`, `Unary`, the eight casts
     and floating conversions, `Load`, `IndexedLoad`, `GetAddress`,
     `Phi`) goes entirely. A `FunctionCall` or `IndirectCall` stays,
     since running the function can be seen (globals, I/O), and loses
     its destination, which marks the result as ignored.
  3. Dropping a definition leaves its operands with one read fewer, so
     the pass collects the reads again and repeats until nothing
     changes.

These stay whatever is read:
  - A `Store` writes memory through a pointer, and a later `Load` this
    function can't see may read the bytes (through an alias, or over
    the lifetime of an address-taken local).
  - A `Ret` ends control with the return value, and is never dead.
  - The jumps and labels define no Var, so they aren't candidates.
  - A call keeps its effects, and only the capture of its result goes.

The pass is sound only in SSA form. Outside it, a definition that looks
dead by name may be live, since a later instruction can write the name
again before any read, and the usual liveness analysis would be needed.
The driver runs this pass only between SSA construction and
destruction.

A load isn't kept for its effects. c6502's memory is plain RAM, so a
load through a wild pointer doesn't trap, and dropping a load whose
destination is dead can't be observed. That goes further than LLVM's
convention that a load may trap, but it matches what the 6502 does. */
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

// Returns the names read anywhere in `instrs`.
Names collect_uses(const Instrs& instrs) {
  Names used;
  for (const auto& instr : instrs) {
    for (const std::string_view u : uses_in(instr)) {
      used.emplace(u);
    }
  }
  return used;
}

// Returns the destination of a pure definition, the instructions that
// go entirely when their destination is dead, or null for any other
// instruction.
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

// Returns whether `instr` is a volatile load, whose access can be seen
// (C99 §6.7.3.6) and has to stay even when nothing reads its
// destination. `(void)*p` of a volatile `*p` is the usual case: the
// load's destination is a temporary nothing reads, but the read of `*p`
// is itself an effect.
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

// Returns whether the Var `dst` is an SSA name that nothing reads.
bool dead(const ir::Type_val& dst, const Names& used, const NameSet& ssa) {
  const auto* v = std::get_if<ir::Var>(&dst);
  // A write to a name that isn't an SSA name (a static, an
  // address-taken local, an aggregate) may be read elsewhere.
  return v != nullptr && ssa.contains(v->name) && !used.contains(v->name);
}

// What happens to one instruction: it is dropped (none), kept, or kept
// with a change, which `changed` reports.
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
  // A call whose result nothing reads loses its destination and stays,
  // since running the function may have effects.
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

// Only a definition of an SSA name is dropped, since only those names
// have one definition in this function. A write to a static, to an
// address-taken local, or to an aggregate that wasn't renamed can be
// read back by another function, through a pointer, or later, so it
// stays whatever this function reads. Outside SSA form there is no safe
// way to tell which destinations are local, so the pass does nothing.
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
