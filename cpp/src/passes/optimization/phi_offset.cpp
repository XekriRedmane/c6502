/* Chooses a constant offset from one pointer, where a join chooses
between that pointer plus one constant or another.

    uint8_t *w = n < 3 ? &E->sword : &E->dagger;
    *w = v;

In SSA form the two arms compute `E + 7` and `E + 9`, and the join takes
one of them:

    .then:  Binary(Add, E, 7, %t1)
            Jump(.end)
    .else:  Binary(Add, E, 9, %t2)
    .end:   %w = Phi(.then: %t1, .else: %t2)
            Store(v, %w)

The pass makes the arms choose the offset and the join add it:

    .end:   %w.o = Phi(.then: 7, .else: 9)      # unsigned char
            %w.e = ZeroExtend(%w.o)
            %w = Binary(Add, E, %w.e)
            Store(v, %w)

`recognize_indirect_indexed` then makes the store `(E),Y` with `%w.o`
in Y, so each arm loads a constant into Y where it had added a constant
to both bytes of the pointer. `calc_combat_strength` and
`apply_shop_effect` in examples/ali_baba choose a member this way.

# Conditions

- Every argument of the Phi is an SSA temporary read only by the Phi
  (for one or more of its edges), defined before it by `p + k` (or `k +
  p`) with the same Var `p` in each and an integer constant `k` from 0
  to 255.
- `p` has the same value at the join as at each add. An SSA-renamed `p`
  has one value. For any other `p`, such as a zero-page pointer global,
  the instructions from the first add to the Phi write no memory, call
  nothing, and don't write `p`, and each jump among them goes to a label
  among them (`unchanged_between`), so no path from an add to the join
  leaves that range. A `p` whose type is `volatile` is left alone, since
  its read would move.
- The Phi's result and `p` are 2 bytes wide, which a pointer is, and the
  result's one reader is a load or store through it, which becomes
  `(p),Y`. A pointer read more than once, as `calc_combat_strength` in
  examples/ali_baba/combat.c reads `*weapon` on either side of a call
  that may move `ENTITY_PTR`, is still computed in 16 bits, and the
  choice of an offset would only add a step.

An add of 2 bytes wraps at 16 bits whichever order its operands are
added in, so `p + ZeroExtend(k)` is `p + k`.

# Where to run

After the fixed point and before `static_indexed_access` and
`recognize_indirect_indexed`, which take `p + ZeroExtend(o)` as an
indexed access. */
#include "passes/optimization/phi_offset.h"

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "bit_precise.h"
#include "passes/optimization/extend_add_chain.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using asdl::Int;
using framework::Instrs;
using framework::PassContext;
using IK = ir::Kind_instruction;

const c99::Type_data_type& strip_const(const c99::Type_data_type& t) {
  const c99::Type_data_type* p = &t;
  while (const auto* c = std::get_if<c99::Const>(p)) {
    p = &*c->referenced_type;
  }
  return *p;
}

// Returns whether `t`, without its `const` qualifiers, is a pointer,
// an `int`, or an `unsigned int`.
bool two_bytes(const c99::Type_data_type& t) {
  const auto& s = strip_const(t);
  return std::holds_alternative<c99::Pointer>(s) || std::holds_alternative<c99::Int>(s) ||
         std::holds_alternative<c99::UInt>(s);
}

bool volatile_type(const c99::Type_data_type& t) {
  return std::holds_alternative<c99::Volatile>(strip_const(t));
}

std::optional<Int> byte_constant(const ir::Type_val& v) {
  const auto* c = std::get_if<ir::Constant>(&v);
  if (c == nullptr) {
    return std::nullopt;
  }
  const auto value = bit_precise::raw_value(c->const_);
  if (value && 0 <= *value && *value <= 255) {
    return value;
  }
  return std::nullopt;
}

// Returns `(p, k)` for `Binary(Add, p, k)` or `Binary(Add, k, p)` with a
// Var `p` and a constant `k` from 0 to 255, or none.
std::optional<std::pair<ir::Var, Int>> split(const ir::Type_instruction& instr) {
  const auto* b = std::get_if<ir::Binary>(&instr);
  if (b == nullptr || !std::holds_alternative<ir::Add>(b->op)) {
    return std::nullopt;
  }
  const std::pair<const ir::Type_val*, const ir::Type_val*> orders[] = {{&b->src1, &b->src2},
                                                                         {&b->src2, &b->src1}};
  for (const auto& [p, k] : orders) {
    const auto value = byte_constant(*k);
    if (const auto* var = std::get_if<ir::Var>(p); var != nullptr && value) {
      return std::pair{*var, *value};
    }
  }
  return std::nullopt;
}

bool is_jump(IK k) {
  return k == IK::Jump || k == IK::JumpIfTrue || k == IK::JumpIfFalse || k == IK::JumpIfCmp ||
         k == IK::JumpIfMasked;
}

// Returns whether `instr` writes no memory and calls nothing (Python's
// `_QUIET`). Each such instruction writes at most its `dst`.
bool quiet(IK k) {
  switch (k) {
    case IK::Label:
    case IK::Jump:
    case IK::JumpIfTrue:
    case IK::JumpIfFalse:
    case IK::JumpIfCmp:
    case IK::JumpIfMasked:
    case IK::Ret:
    case IK::Copy:
    case IK::Binary:
    case IK::Unary:
    case IK::ZeroExtend:
    case IK::SignExtend:
    case IK::Truncate:
    case IK::Load:
    case IK::IndexedLoad:
    case IK::IndirectIndexedLoad:
    case IK::GetAddress:
    case IK::Phi:
      return true;
    default:
      return false;
  }
}

const std::string* jump_target(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case IK::Jump:
      return &std::get<ir::Jump>(instr).target;
    case IK::JumpIfTrue:
      return &std::get<ir::JumpIfTrue>(instr).target;
    case IK::JumpIfFalse:
      return &std::get<ir::JumpIfFalse>(instr).target;
    case IK::JumpIfCmp:
      return &std::get<ir::JumpIfCmp>(instr).target;
    case IK::JumpIfMasked:
      return &std::get<ir::JumpIfMasked>(instr).target;
    default:
      return nullptr;
  }
}

// Returns whether every path from `instrs[lo]` to `instrs[hi]` leaves
// `p` unchanged. An SSA name has one value. For any other `p`, the
// instructions from `lo` to `hi` must write no memory, call nothing, and
// not write `p`, and every jump among them must go to a label among
// them, so that no path leaves the range and comes back.
bool unchanged_between(const Instrs& instrs, std::size_t lo, std::size_t hi, const ir::Var& p,
                       const NameSet& ssa) {
  if (ssa.contains(p.name)) {
    return true;
  }
  std::set<std::string, std::less<>> labels;
  for (std::size_t k = lo; k <= hi && k < instrs.size(); ++k) {
    if (const auto* l = std::get_if<ir::Label>(&instrs[k])) {
      labels.insert(l->name);
    }
  }
  for (std::size_t k = lo; k < hi && k < instrs.size(); ++k) {
    const auto& ins = instrs[k];
    const IK kind = ir::kind(ins);
    if (!quiet(kind)) {
      return false;
    }
    if ((kind == IK::Load && std::get<ir::Load>(ins).is_volatile) ||
        (kind == IK::IndexedLoad && std::get<ir::IndexedLoad>(ins).is_volatile) ||
        (kind == IK::IndirectIndexedLoad && std::get<ir::IndirectIndexedLoad>(ins).is_volatile)) {
      return false;
    }
    if (const auto dst = framework::dst_name(ins); dst && *dst == p.name) {
      return false;
    }
    if (is_jump(kind) && !labels.contains(*jump_target(ins))) {
      return false;
    }
  }
  return true;
}

// Returns whether the only reader of `d` is one load or store through
// it, which the indexed-access recognizers can make `(p),Y`. A pointer
// with other readers is still computed in 16 bits, and the choice of an
// offset only adds a step.
bool one_access(const Instrs& instrs, const ir::Var& d, const UseCounts& uses) {
  if (use_count(uses, d.name) != 1) {
    return false;
  }
  const auto is_d = [&](const ir::Type_val& v) {
    const auto* var = std::get_if<ir::Var>(&v);
    return var != nullptr && var->name == d.name;
  };
  for (const auto& ins : instrs) {
    if (const auto* l = std::get_if<ir::Load>(&ins); l && is_d(l->src_ptr)) {
      return true;
    }
    if (const auto* s = std::get_if<ir::Store>(&ins); s && is_d(s->dst_ptr) && !is_d(s->src)) {
      return true;
    }
  }
  return false;
}

// Registers the local `name` of type `t` as an SSA name, replacing any
// symbol of that name.
ir::Var new_temp(const PassContext& ctx, const std::string& name, c99::Type_data_type t) {
  ctx.symbols->insert_or_assign(name, Symbol{.type = std::move(t), .attrs = LocalAttr{}});
  ctx.ssa_dsts->insert(name);
  return ir::Var{.name = name};
}

}  // namespace

ir::Function phi_offset(ir::Function fn, const PassContext& ctx) {
  if (ctx.ssa_dsts == nullptr || ctx.ssa_dsts->empty() || ctx.symbols == nullptr) {
    return fn;
  }
  const NameSet& ssa = *ctx.ssa_dsts;
  const Instrs& instrs = fn.instructions;
  std::map<std::string, std::size_t, std::less<>> defs;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (const auto name = framework::dst_name(instrs[i]); name && ssa.contains(*name)) {
      defs.insert_or_assign(std::string(*name), i);
    }
  }
  const UseCounts uses = count_uses(instrs);
  std::set<std::size_t> drop;
  // The replacement of each Phi rewritten, by its index, and the
  // instructions that follow the last Phi of its block, by that Phi's
  // index (Python keeps the second under `-1 - index` in one dict).
  std::map<std::size_t, Instrs> replace;
  std::map<std::size_t, Instrs> tail;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto* phi = std::get_if<ir::Phi>(&instrs[i]);
    if (phi == nullptr || phi->args.empty()) {
      continue;
    }
    const auto* dst = std::get_if<ir::Var>(&phi->dst);
    if (dst == nullptr) {
      continue;
    }
    const Symbol* sym = ctx.symbols->find(dst->name);
    if (sym == nullptr || !two_bytes(sym->type) || !one_access(instrs, *dst, uses)) {
      continue;
    }
    std::optional<ir::Var> base;
    std::vector<Int> offsets;
    std::vector<std::size_t> sites;
    std::map<std::string, std::size_t, std::less<>> readers;
    for (const auto& arg : phi->args) {
      if (const auto* v = std::get_if<ir::Var>(&std::get<ir::PhiArg>(arg).source)) {
        ++readers[v->name];
      }
    }
    bool complete = true;
    for (const auto& arg : phi->args) {
      const auto* src = std::get_if<ir::Var>(&std::get<ir::PhiArg>(arg).source);
      if (src == nullptr || !ssa.contains(src->name) ||
          use_count(uses, src->name) != readers[src->name] || !defs.contains(src->name)) {
        complete = false;
        break;
      }
      const std::size_t j = defs.find(src->name)->second;
      const auto s = split(instrs[j]);
      if (!s || drop.contains(j) || j > i) {
        complete = false;
        break;
      }
      if (!base) {
        base = s->first;
      } else if (s->first.name != base->name) {
        complete = false;
        break;
      }
      offsets.push_back(s->second);
      sites.push_back(j);
    }
    if (!complete) {
      continue;
    }
    const Symbol* psym = ctx.symbols->find(base->name);
    if (psym == nullptr || !two_bytes(psym->type) || volatile_type(psym->type) ||
        !unchanged_between(instrs, *std::min_element(sites.begin(), sites.end()), i, *base,
                           ssa)) {
      continue;
    }
    const ir::Var o = new_temp(ctx, dst->name + ".o", c99::UChar{});
    const ir::Var e = new_temp(ctx, dst->name + ".e", c99::UInt{});
    ir::Phi chosen{.dst = o};
    for (std::size_t a = 0; a < phi->args.size(); ++a) {
      chosen.args.push_back(ir::PhiArg{
          .pred_label = std::get<ir::PhiArg>(phi->args[a]).pred_label,
          .source = ir::Constant{.const_ = ir::ConstUChar{.value = offsets[a]}},
      });
    }
    replace[i] = Instrs{std::move(chosen)};
    // The add follows the block's last Phi.
    std::size_t last = i;
    while (last + 1 < instrs.size() && std::holds_alternative<ir::Phi>(instrs[last + 1])) {
      ++last;
    }
    Instrs& after = tail[last];
    after.push_back(ir::ZeroExtend{.src = o, .dst = e});
    after.push_back(ir::Binary{.op = ir::Add{}, .src1 = *base, .src2 = e, .dst = phi->dst});
    drop.insert(sites.begin(), sites.end());
  }
  if (replace.empty()) {
    return fn;
  }
  Instrs out;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (drop.contains(i)) {
      continue;
    }
    if (const auto it = replace.find(i); it != replace.end()) {
      for (const auto& x : it->second) {
        out.push_back(x);
      }
    } else {
      out.push_back(instrs[i]);
    }
    if (const auto it = tail.find(i); it != tail.end()) {
      for (const auto& x : it->second) {
        out.push_back(x);
      }
    }
  }
  return framework::with_instructions(fn, std::move(out));
}

}  // namespace c6502::passes
