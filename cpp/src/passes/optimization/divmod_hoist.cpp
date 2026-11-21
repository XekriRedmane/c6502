/* Puts a divide and a remainder of the same operands next to each other.

# What it does

`tac_to_asm` lowers a `/` and a `%` of the same operands with one loop
of subtractions, or one call of a `divmod` helper, when the two come
close together in one straight run with no store or call between them
(`plan_divmod_pairs`). A store, a call, or a branch between them gives
two loops. Ali Baba's `load_scene_from_disk` stores `sector % 13`
through a pointer before it computes `sector / 13`, and
`render_font_char_direct` divides inside an `if` and takes the
remainder after it:

    JumpIfCmp(LessThan, V, 24uc, .if_end@0)
    Binary(Divide, V, 24uc, Q)
  .if_end@0:
    Binary(Modulo, V, 24uc, R)

This pass computes both into temporaries at one place and leaves a
`Copy` of each where its instruction was:

    Binary(Divide, V, 24uc, %q)
    Binary(Modulo, V, 24uc, %r)
    JumpIfCmp(LessThan, V, 24uc, .if_end@0)
    Copy(%q, Q)
  .if_end@0:
    Copy(%r, R)

A result whose destination is an SSA-renamed name is computed straight
into that name, and nothing is left at its old place.

# Where the pair goes

Let `A` and `B` be the blocks of the two instructions and `D` their
nearest common dominator. When `D` is the block of one of them, the
pair goes at that one, the first of the two in `D`. Otherwise it goes
at the end of `D`, before its jump, and then every path from there to
the function's exit has to pass through `A` or `B`, so the pair never
runs on a path that computed neither.

# When it applies

- The two are `Binary(Divide)` and `Binary(Modulo)` with the same
  `src1`, a variable of 1, 2, 4, or 8 bytes that isn't floating, and
  the same `src2`. A `src2` that is a variable is taken only when both
  are in one block, and a constant one must not be 0, so that the pair
  never divides by zero on a path that didn't.
- Neither destination is an operand, and no operand is volatile, since
  the pair reads each operand once.
- From the pair's new place to each instruction, no instruction writes
  an operand. Every instruction in a block that lies on a path from `D`
  to `A` or `B` counts, and so does every instruction after the new
  place in `D`. An operand that isn't SSA-renamed (a static, an object
  at a fixed address, an address-taken local) can be written through a
  pointer or by a call, so then no store through a pointer,
  `IndexedStore`, store into the operand by name, or call may come
  there either.

The operands then hold the same values at the new place as at each
instruction, and the division has no side effects, so each result is
the value the instruction computed. Each destination is still written
where it was. Pairs whose two instructions are already adjacent are
left alone. */
#include "passes/optimization/divmod_hoist.h"

#include <algorithm>
#include <cstddef>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "bit_precise.h"
#include "passes/optimization/cfg.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using framework::Instrs;
using framework::PassContext;
using IK = ir::Kind_instruction;
using tac_cfg::Cfg;
using BlockSet = std::set<int>;
using Names = std::set<std::string, std::less<>>;

const tac_cfg::BasicBlock& block_of(const Cfg& cfg, int id) {
  return *cfg.blocks.find(id);
}

bool is_volatile(const std::string& name, const SymbolTable& symbols) {
  const Symbol* sym = symbols.find(name);
  if (sym == nullptr) {
    return false;
  }
  const c99::Type_data_type* t = &sym->type;
  while (const auto* c = std::get_if<c99::Const>(t)) {
    t = &*c->referenced_type;
  }
  return std::holds_alternative<c99::Volatile>(*t);
}

// Returns the width of the integer variable `name` in bytes, or none
// for a name with no symbol or a type of another kind.
std::optional<int> width(const std::string& name, const SymbolTable& symbols) {
  const Symbol* sym = symbols.find(name);
  if (sym == nullptr) {
    return std::nullopt;
  }
  const c99::Type_data_type* t = &sym->type;
  while (true) {
    if (const auto* c = std::get_if<c99::Const>(t)) {
      t = &*c->referenced_type;
    } else if (const auto* v = std::get_if<c99::Volatile>(t)) {
      t = &*v->referenced_type;
    } else {
      break;
    }
  }
  if (asdl::isa<c99::Char, c99::SChar, c99::UChar>(*t)) {
    return 1;
  }
  if (asdl::isa<c99::Int, c99::UInt>(*t)) {
    return 2;
  }
  if (asdl::isa<c99::Long, c99::ULong>(*t)) {
    return 4;
  }
  if (asdl::isa<c99::LongLong, c99::ULongLong>(*t)) {
    return 8;
  }
  return std::nullopt;
}

// Returns the instruction as a `/` or `%` of a Var by a Var or a
// constant into a Var, or null.
const ir::Binary* candidate(const ir::Type_instruction& ins) {
  const auto* b = std::get_if<ir::Binary>(&ins);
  if (b == nullptr || !asdl::isa<ir::Divide, ir::Modulo>(b->op) ||
      !std::holds_alternative<ir::Var>(b->src1) || !std::holds_alternative<ir::Var>(b->dst)) {
    return nullptr;
  }
  return b;
}

const std::string& var_name(const ir::Type_val& v) {
  return std::get<ir::Var>(v).name;
}

bool memory_write(const ir::Type_instruction& ins) {
  return asdl::isa<ir::Store, ir::IndexedStore, ir::IndirectIndexedStore, ir::FunctionCall,
                   ir::IndirectCall>(ins);
}

std::string new_name(const std::string& base, const SymbolTable& symbols) {
  int k = 0;
  while (symbols.contains(base + ".dm" + std::to_string(k))) {
    ++k;
  }
  return base + ".dm" + std::to_string(k);
}

// Returns the blocks reachable from `start`'s successors without
// passing through `start` or a block in `stops`, with the `stops`
// reached.
BlockSet between(const Cfg& cfg, int start, const BlockSet& stops) {
  BlockSet seen;
  const auto& succ = block_of(cfg, start).successors;
  std::vector<int> work(succ.begin(), succ.end());
  while (!work.empty()) {
    const int b = work.back();
    work.pop_back();
    if (seen.contains(b) || b == start) {
      continue;
    }
    seen.insert(b);
    if (stops.contains(b)) {
      continue;
    }
    const auto& more = block_of(cfg, b).successors;
    work.insert(work.end(), more.begin(), more.end());
  }
  return seen;
}

// Returns whether `block` can be reached again from its successors
// without passing through `avoid`.
bool on_cycle(const Cfg& cfg, int block, int avoid) {
  BlockSet seen;
  const auto& succ = block_of(cfg, block).successors;
  std::vector<int> work(succ.begin(), succ.end());
  while (!work.empty()) {
    const int b = work.back();
    work.pop_back();
    if (b == block) {
      return true;
    }
    if (seen.contains(b) || b == avoid) {
      continue;
    }
    seen.insert(b);
    const auto& more = block_of(cfg, b).successors;
    work.insert(work.end(), more.begin(), more.end());
  }
  return false;
}

// Returns the blocks of `blocks` from which a block of `targets` can be
// reached inside `blocks`. The result is the least set closed under
// the rule, so the order of the walk doesn't change it.
BlockSet reaching(const Cfg& cfg, const BlockSet& blocks, const BlockSet& targets) {
  BlockSet out;
  for (const int t : targets) {
    if (blocks.contains(t)) {
      out.insert(t);
    }
  }
  bool changed = true;
  while (changed) {
    changed = false;
    for (const int b : blocks) {
      if (out.contains(b)) {
        continue;
      }
      for (const int s : block_of(cfg, b).successors) {
        if (out.contains(s)) {
          out.insert(b);
          changed = true;
          break;
        }
      }
    }
  }
  return out;
}

struct Analysis {
  Cfg cfg;
  OrderedMap<int, int> idom;
  // The block of each instruction of the function and its index there.
  std::vector<std::pair<int, std::size_t>> where;

  std::vector<int> dominators(int b) const {
    std::vector<int> out{b};
    while (b != tac_cfg::kEntryId) {
      b = idom.at(b);
      out.push_back(b);
    }
    return out;
  }
};

std::optional<ir::Function> try_pair(const ir::Function& fn, const Analysis& an, std::size_t i,
                                     std::size_t j, const Names& operands, const PassContext& ctx) {
  SymbolTable& symbols = *ctx.symbols;
  const Instrs& instrs = fn.instructions;
  const auto& a = std::get<ir::Binary>(instrs[i]);
  const auto& b = std::get<ir::Binary>(instrs[j]);
  const int ba = an.where[i].first;
  const int bb = an.where[j].first;
  const auto doms_a = an.dominators(ba);
  int common = tac_cfg::kEntryId;
  for (const int d : an.dominators(bb)) {
    if (std::find(doms_a.begin(), doms_a.end(), d) != doms_a.end()) {
      common = d;
      break;
    }
  }
  if (common == tac_cfg::kEntryId) {
    return std::nullopt;
  }
  if (std::holds_alternative<ir::Var>(a.src2) && ba != bb) {
    return std::nullopt;
  }
  // The pair's new place, as an index into `instrs`.
  std::size_t at = 0;
  if (common == ba) {
    at = i;
  } else if (common == bb) {
    at = j;
  } else {
    std::size_t start = 0;
    while (an.where[start].first != common) {
      ++start;
    }
    at = start + block_of(an.cfg, common).instructions.size() - 1;
    if (!asdl::isa<ir::Jump, ir::JumpIfTrue, ir::JumpIfFalse, ir::JumpIfCmp, ir::JumpIfMasked>(
            instrs[at])) {
      ++at;
    }
    if (between(an.cfg, common, {ba, bb}).contains(tac_cfg::kExitId)) {
      return std::nullopt;
    }
  }
  // The instructions on the way from the new place to each of the two.
  std::vector<std::size_t> path;
  if (ba == bb && bb == common) {
    const std::size_t lo = std::min({i, j, at});
    const std::size_t hi = std::max(i, j);
    for (std::size_t k = lo + 1; k < hi; ++k) {
      path.push_back(k);
    }
  } else {
    const BlockSet region = reaching(an.cfg, between(an.cfg, common, {ba, bb}), {ba, bb});
    // In the block of `a` or `b`, only the instructions before it are
    // on the way to it, unless a loop leads back into the block or the
    // block leads on to the other one.
    std::map<int, std::size_t> ends;
    const std::tuple<int, std::size_t, int> sides[] = {{ba, i, bb}, {bb, j, ba}};
    for (const auto& [w, e, other] : sides) {
      if (w != common && !on_cycle(an.cfg, w, common) &&
          !between(an.cfg, w, {common}).contains(other)) {
        ends[w] = e;
      }
    }
    for (std::size_t k = 0; k < an.where.size(); ++k) {
      const int w = an.where[k].first;
      std::size_t end = an.where.size();
      if (const auto it = ends.find(w); it != ends.end()) {
        end = it->second;
      }
      if ((region.contains(w) && k < end) || (w == common && k >= at)) {
        path.push_back(k);
      }
    }
  }
  bool needs_memory = false;
  for (const auto& n : operands) {
    if (!ctx.is_ssa(n)) {
      needs_memory = true;
    }
  }
  for (const std::size_t k : path) {
    const auto& ins = instrs[k];
    if (k == i || k == j) {
      continue;
    }
    for (const auto d : defs_in(ins)) {
      if (operands.contains(d)) {
        return std::nullopt;
      }
    }
    if (needs_memory) {
      if (memory_write(ins)) {
        return std::nullopt;
      }
      if (const auto* s = std::get_if<ir::IndexedSymbolStore>(&ins);
          s != nullptr && operands.contains(s->name)) {
        return std::nullopt;
      }
    }
  }
  Instrs pair;
  std::map<std::size_t, std::optional<ir::Type_instruction>> copies;
  const std::pair<std::size_t, const ir::Binary*> both[] = {{i, &a}, {j, &b}};
  for (const auto& [k, ins] : both) {
    ir::Type_val dst = ins->dst;
    const std::string& dst_name = var_name(ins->dst);
    if (ctx.is_ssa(dst_name)) {
      copies[k] = std::nullopt;
    } else {
      const std::string name = new_name(dst_name, symbols);
      c99::Type_data_type type = symbols.at(dst_name).type;
      symbols.insert_or_assign(name, Symbol{.type = std::move(type), .attrs = LocalAttr{}});
      ctx.ssa_dsts->insert(name);
      dst = ir::Var{.name = name};
      copies[k] = ir::Copy{.src = dst, .dst = ins->dst};
    }
    pair.push_back(ir::Binary{.op = ins->op, .src1 = ins->src1, .src2 = ins->src2, .dst = dst});
  }
  Instrs out;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (k == at) {
      out.insert(out.end(), pair.begin(), pair.end());
    }
    if (k == i || k == j) {
      if (copies[k]) {
        out.push_back(*copies[k]);
      }
      continue;
    }
    out.push_back(instrs[k]);
  }
  if (at == instrs.size()) {
    out.insert(out.end(), pair.begin(), pair.end());
  }
  return framework::with_instructions(fn, std::move(out));
}

// Returns whether `c` is a constant whose value is 0 or that has no
// `value` field (a floating constant).
bool zero_or_valueless(const ir::Type_const& c) {
  if (asdl::isa<ir::ConstFloat, ir::ConstDouble>(c)) {
    return true;
  }
  const auto v = bit_precise::raw_value(c);
  return !v || *v == 0;
}

std::optional<ir::Function> hoist_one(const ir::Function& fn, const PassContext& ctx) {
  const SymbolTable& symbols = *ctx.symbols;
  Analysis an{.cfg = tac_cfg::build_cfg(fn)};
  an.idom = tac_cfg::immediate_dominators(an.cfg);
  for (const int bid : an.cfg.block_order) {
    const std::size_t n = block_of(an.cfg, bid).instructions.size();
    for (std::size_t k = 0; k < n; ++k) {
      an.where.emplace_back(bid, k);
    }
  }
  const Instrs& instrs = fn.instructions;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const ir::Binary* a = candidate(instrs[i]);
    if (a == nullptr || !an.idom.contains(an.where[i].first)) {
      continue;
    }
    Names operands{var_name(a->src1)};
    if (const auto* v = std::get_if<ir::Var>(&a->src2)) {
      operands.insert(v->name);
    } else if (const auto* c = std::get_if<ir::Constant>(&a->src2)) {
      if (zero_or_valueless(c->const_)) {
        continue;
      }
    }
    if (!width(var_name(a->src1), symbols)) {
      continue;
    }
    bool vol = false;
    for (const auto& n : operands) {
      vol = vol || is_volatile(n, symbols);
    }
    if (vol) {
      continue;
    }
    for (std::size_t j = i + 1; j < instrs.size(); ++j) {
      const ir::Binary* b = candidate(instrs[j]);
      if (b == nullptr || ir::kind(b->op) == ir::kind(a->op) || !(b->src1 == a->src1) ||
          !(b->src2 == a->src2) || !an.idom.contains(an.where[j].first)) {
        continue;
      }
      if (an.where[j].first == an.where[i].first && j - i <= 8) {
        bool write = false;
        for (std::size_t k = i + 1; k < j; ++k) {
          write = write || memory_write(instrs[k]) ||
                  std::holds_alternative<ir::IndexedSymbolStore>(instrs[k]);
        }
        if (!write) {
          // tac_to_asm's `plan_divmod_pairs` takes these.
          break;
        }
      }
      if (operands.contains(var_name(a->dst)) || operands.contains(var_name(b->dst))) {
        break;
      }
      return try_pair(fn, an, i, j, operands, ctx);
    }
  }
  return std::nullopt;
}

}  // namespace

ir::Function hoist_divmod_pairs(ir::Function fn, const PassContext& ctx) {
  if (ctx.symbols == nullptr || ctx.ssa_dsts == nullptr) {
    return fn;
  }
  for (int n = 0; n < 64; ++n) {
    auto next = hoist_one(fn, ctx);
    if (!next) {
      return fn;
    }
    fn = std::move(*next);
  }
  return fn;
}

}  // namespace c6502::passes
