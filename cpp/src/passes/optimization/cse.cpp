/* Common-subexpression elimination over the dominator tree.

When an instruction computes the same value as an earlier one, it is
replaced by a `Copy` from the earlier one's destination, which copy
propagation and dead-store elimination then remove. Two kinds of
instruction are matched.

  * Pure computations: a `Binary`, `Unary`, `ZeroExtend`, `SignExtend`,
    or `Truncate` whose operands are SSA values or Constants. In SSA
    form such an instruction's result depends only on its operands, so
    an identical instruction in a block that dominates this one has the
    same result. A walk of the dominator tree carries a table of them
    from each block to the blocks it dominates.
  * Loads: a `Load`, `IndexedLoad`, `IndexedConstLoad`, or
    `IndirectIndexedLoad` that isn't volatile. A load's result depends
    on memory too, so an earlier identical load is reused only when
    nothing could have written memory in between. The table of loads is
    carried into a block only from its one predecessor, so every path to
    the reuse passes through the earlier load. Any instruction that
    could write memory empties it: a store, a call, a volatile access,
    and any write to a name that isn't an SSA value (a static, or an
    address-taken local).

The usual source is C that repeats a subscript or a test, such as

    if (key[1] < entry[0]) return 0;
    if (key[1] == entry[0]) { ... }

where the second test loads both bytes again, and `dict[n + 1]`,
`dict[n + 2]`, `dict[n + 3]`, which each zero-extend `n`.

A destination must be an SSA value too, so that replacing its one
definition with a Copy changes nothing else. The key includes the
destination's type, so `ZeroExtend(c)` to `int` and to `long` aren't
merged. The operands of the commutative operators are put in a fixed
order, so `a + b` and `b + a` match.

Python's keys are tuples of reprs. The keys here are strings built from
the dump of each operand and of each type, which are equal exactly when
the values are, and a commutative operator's two operand keys are put
in the order of the strings, which matches the same pairs as Python's
order of their reprs. */
#include "passes/optimization/cse.h"

#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "bit_precise.h"
#include "passes/optimization/cfg.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using framework::PassContext;
using tac_cfg::Cfg;
using K = ir::Kind_instruction;
using BK = ir::Kind_binary_operator;
using Table = std::map<std::string, std::string, std::less<>>;

constexpr char kSep = '\x1f';

// Returns the `dst` field of `instr` when it has one, or null.
const ir::Type_val* dst_field(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
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
    case K::GetAddress:
      return &std::get<ir::GetAddress>(instr).dst;
    case K::Load:
      return &std::get<ir::Load>(instr).dst;
    case K::IndexedLoad:
      return &std::get<ir::IndexedLoad>(instr).dst;
    case K::IndexedConstLoad:
      return &std::get<ir::IndexedConstLoad>(instr).dst;
    case K::IndirectIndexedLoad:
      return &std::get<ir::IndirectIndexedLoad>(instr).dst;
    case K::Unary:
      return &std::get<ir::Unary>(instr).dst;
    case K::Binary:
      return &std::get<ir::Binary>(instr).dst;
    case K::Copy:
      return &std::get<ir::Copy>(instr).dst;
    case K::Phi:
      return &std::get<ir::Phi>(instr).dst;
    case K::FunctionCall: {
      const auto& d = std::get<ir::FunctionCall>(instr).dst;
      return d ? &*d : nullptr;
    }
    case K::IndirectCall: {
      const auto& d = std::get<ir::IndirectCall>(instr).dst;
      return d ? &*d : nullptr;
    }
    default:
      return nullptr;
  }
}

// Returns whether `instr` is volatile (Python's `getattr(instr,
// "is_volatile", False)`).
bool is_volatile(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::Load:
      return std::get<ir::Load>(instr).is_volatile;
    case K::Store:
      return std::get<ir::Store>(instr).is_volatile;
    case K::IndexedLoad:
      return std::get<ir::IndexedLoad>(instr).is_volatile;
    case K::IndexedStore:
      return std::get<ir::IndexedStore>(instr).is_volatile;
    case K::IndexedSymbolStore:
      return std::get<ir::IndexedSymbolStore>(instr).is_volatile;
    case K::IndexedConstLoad:
      return std::get<ir::IndexedConstLoad>(instr).is_volatile;
    case K::IndirectIndexedLoad:
      return std::get<ir::IndirectIndexedLoad>(instr).is_volatile;
    case K::IndirectIndexedStore:
      return std::get<ir::IndirectIndexedStore>(instr).is_volatile;
    default:
      return false;
  }
}

bool is_commutative(BK op) {
  switch (op) {
    case BK::Add:
    case BK::Multiply:
    case BK::BitwiseAnd:
    case BK::BitwiseOr:
    case BK::BitwiseXor:
    case BK::Equal:
    case BK::NotEqual:
      return true;
    default:
      return false;
  }
}

class Keys {
 public:
  Keys(const NameSet& ssa, const SymbolTable& symbols) : ssa_(ssa), symbols_(symbols) {}

  // Returns the key of an operand: its dump for a Constant (the whole
  // node, since the floating classes keep their value in `bits` in place
  // of `value`) or for a Var that is an SSA value, and none for any other
  // Var.
  std::optional<std::string> operand(const ir::Type_val& v) const {
    if (const auto* var = std::get_if<ir::Var>(&v); var != nullptr && !ssa_.contains(var->name)) {
      return std::nullopt;
    }
    return asdl::dump(v);
  }

  // Returns the key of a name's type, its dump, or "-" for a name
  // without a symbol.
  const std::string& type(const std::string& name) {
    const auto it = types_.find(name);
    if (it != types_.end()) {
      return it->second;
    }
    const Symbol* sym = symbols_.find(name);
    return types_.emplace(name, sym != nullptr ? asdl::dump(sym->type) : std::string("-")).first->second;
  }

  // Returns the table `instr` goes in, `kPure` or `kLoad`, and its key,
  // or none when it is neither kind or an operand isn't a value it can
  // match.
  enum Kind { kPure, kLoad };
  std::optional<std::pair<Kind, std::string>> of(const ir::Type_instruction& instr) {
    const ir::Type_val* dst = dst_field(instr);
    const auto* dst_var = dst != nullptr ? std::get_if<ir::Var>(dst) : nullptr;
    if (dst_var == nullptr || !ssa_.contains(dst_var->name)) {
      return std::nullopt;
    }
    std::string key = std::to_string(static_cast<int>(ir::kind(instr)));
    key += kSep;
    const auto add = [&](const std::string& part) {
      key += part;
      key += kSep;
    };
    switch (ir::kind(instr)) {
      case K::Binary: {
        const auto& b = std::get<ir::Binary>(instr);
        std::optional<std::string> a = operand(b.src1);
        std::optional<std::string> c = operand(b.src2);
        if (!a || !c) {
          return std::nullopt;
        }
        const BK op = ir::kind(b.op);
        if (is_commutative(op) && *c < *a) {
          std::swap(a, c);
        }
        // A shift count's type doesn't change the result, and strength
        // reduction writes `i * 2` as `i << (uchar)1` where C's `i << 1`
        // has an int count.
        if (op == BK::LeftShift || op == BK::RightShift) {
          if (const auto* k = std::get_if<ir::Constant>(&b.src2)) {
            if (const auto value = bit_precise::raw_value(k->const_)) {
              c = "count:" + asdl::int_to_string(*value);
            }
          }
        }
        add(std::to_string(static_cast<int>(op)));
        add(*a);
        add(*c);
        add(type(dst_var->name));
        return std::pair{kPure, std::move(key)};
      }
      case K::Unary:
      case K::ZeroExtend:
      case K::SignExtend:
      case K::Truncate: {
        const ir::Type_val* src = nullptr;
        std::string op = "None";
        if (const auto* u = std::get_if<ir::Unary>(&instr)) {
          src = &u->src;
          op = std::to_string(static_cast<int>(ir::kind(u->op)));
        } else if (const auto* z = std::get_if<ir::ZeroExtend>(&instr)) {
          src = &z->src;
        } else if (const auto* s = std::get_if<ir::SignExtend>(&instr)) {
          src = &s->src;
        } else {
          src = &std::get<ir::Truncate>(instr).src;
        }
        const std::optional<std::string> s = operand(*src);
        if (!s) {
          return std::nullopt;
        }
        add(op);
        add(*s);
        add(type(dst_var->name));
        return std::pair{kPure, std::move(key)};
      }
      case K::Load:
      case K::IndexedLoad:
      case K::IndexedConstLoad:
      case K::IndirectIndexedLoad: {
        if (is_volatile(instr)) {
          return std::nullopt;
        }
        std::vector<std::optional<std::string>> ops;
        if (const auto* l = std::get_if<ir::Load>(&instr)) {
          ops = {operand(l->src_ptr)};
        } else if (const auto* il = std::get_if<ir::IndexedLoad>(&instr)) {
          ops = {asdl::dump(ir::Var{.name = il->name}), operand(il->index), asdl::int_to_string(il->offset),
                 asdl::int_to_string(il->plane)};
        } else if (const auto* cl = std::get_if<ir::IndexedConstLoad>(&instr)) {
          ops = {asdl::int_to_string(cl->address), operand(cl->index)};
        } else {
          const auto& l2 = std::get<ir::IndirectIndexedLoad>(instr);
          ops = {operand(l2.ptr), operand(l2.index)};
        }
        for (const auto& o : ops) {
          if (!o) {
            return std::nullopt;
          }
          add(*o);
        }
        add(type(dst_var->name));
        return std::pair{kLoad, std::move(key)};
      }
      default:
        return std::nullopt;
    }
  }

  // Returns whether `instr` could write memory: any instruction but the
  // pure ones, the loads, a `Copy`, a `GetAddress`, a label, a `Phi`, a
  // jump, and a `Ret`, and any of those that is volatile or writes a
  // name that isn't an SSA value.
  bool writes_memory(const ir::Type_instruction& instr) const {
    switch (ir::kind(instr)) {
      case K::Binary:
      case K::Unary:
      case K::ZeroExtend:
      case K::SignExtend:
      case K::Truncate:
      case K::Load:
      case K::IndexedLoad:
      case K::IndexedConstLoad:
      case K::IndirectIndexedLoad:
      case K::Copy:
      case K::GetAddress:
      case K::Label:
      case K::Phi:
      case K::Jump:
      case K::JumpIfTrue:
      case K::JumpIfFalse:
      case K::JumpIfCmp:
      case K::JumpIfMasked:
      case K::Ret:
        break;
      default:
        return true;
    }
    if (is_volatile(instr)) {
      return true;
    }
    const ir::Type_val* dst = dst_field(instr);
    const auto* var = dst != nullptr ? std::get_if<ir::Var>(dst) : nullptr;
    return var != nullptr && !ssa_.contains(var->name);
  }

 private:
  const NameSet& ssa_;
  const SymbolTable& symbols_;
  std::map<std::string, std::string, std::less<>> types_;
};

}  // namespace

tac_ast::Function eliminate_common_subexpressions(tac_ast::Function fn, const PassContext& ctx) {
  if (ctx.ssa_dsts == nullptr || ctx.ssa_dsts->empty() || ctx.symbols == nullptr) {
    return fn;
  }
  Cfg cfg = tac_cfg::build_cfg(fn);
  const OrderedMap<int, int> idom = tac_cfg::immediate_dominators(cfg);
  const OrderedMap<int, std::vector<int>> children = tac_cfg::dominator_tree_children(idom);
  Keys keys(*ctx.ssa_dsts, *ctx.symbols);
  bool changed = false;

  // A walk of the dominator tree. Each entry carries the table of pure
  // computations of the block's immediate dominator and the table of
  // loads of its predecessor, which is empty unless that is its only
  // one.
  struct Entry {
    int bid;
    Table pure;
    Table loads;
  };
  std::vector<Entry> stack;
  stack.push_back(Entry{tac_cfg::kEntryId, {}, {}});
  while (!stack.empty()) {
    Entry e = std::move(stack.back());
    stack.pop_back();
    auto& instrs = cfg.blocks.find(e.bid)->instructions;
    for (auto& instr : instrs) {
      if (auto k = keys.of(instr)) {
        Table& table = k->first == Keys::kPure ? e.pure : e.loads;
        const auto prev = table.find(k->second);
        if (prev != table.end()) {
          instr = ir::Copy{.src = ir::Var{.name = prev->second}, .dst = *dst_field(instr)};
          changed = true;
          continue;
        }
        table.emplace(std::move(k->second), std::get<ir::Var>(*dst_field(instr)).name);
      }
      if (keys.writes_memory(instr)) {
        e.loads.clear();
      }
    }
    if (const std::vector<int>* kids = children.find(e.bid)) {
      for (const int child : *kids) {
        const auto& preds = cfg.blocks.at(child).predecessors;
        const bool only_pred = preds.size() == 1 && preds.front() == e.bid;
        stack.push_back(Entry{child, e.pure, only_pred ? e.loads : Table{}});
      }
    }
  }
  if (!changed) {
    return fn;
  }
  return tac_cfg::cfg_to_function(fn, cfg);
}

}  // namespace c6502::passes
