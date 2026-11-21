/* TAC SSA construction.

Converts a `tac_ast.Function` to SSA form. Each promotable Var gets a
new name `<orig>.<N>` per definition, and the names are joined by `Phi`
instructions where control flow merges. Everything else (statics,
address-taken locals, Vars of aggregate type, function names) passes
through unchanged.

A Var is promotable if and only if

  * its symbol is a `LocalAttr` (a block-scope automatic, a function
    parameter, or a TAC temporary `%N` that `c99_to_tac` made),
  * its type is scalar: `Int`, `Long`, `LongLong`, `UInt`, `ULong`,
    `ULongLong`, `Char`, `SChar`, `UChar`, `Float`, `Double`, `Pointer`,
    or one of the bit-precise integers of 3, 5, 6, or 7 bytes. Arrays,
    structs, and unions are left out because their members are reached
    through `GetAddress` and `Load` or `Store`, which makes them
    address-taken in effect, and
  * its address is never taken, which is to say no `GetAddress` in the
    function names it as its `operand`.

The algorithm is Cytron et al. 1991's.

  1. When the function's body starts with a `Label`, a synthetic
     preheader `Label` goes in front of it, so that the first block
     with instructions of its own has ENTRY as its only predecessor.
     That keeps the tagging of a Phi's arguments by predecessor label
     well defined for every later block whose predecessors include the
     edge from ENTRY.
  2. The CFG is built, and `ensure_block_labels` gives every block a
     leading `Label` to use as its `PhiArg.pred_label`.
  3. The immediate dominators, the dominance frontiers, and the
     children in the dominator tree are computed.
  4. For each promotable v, Defs(v) is the set of blocks that define v.
     An empty `Phi(dst=Var(v), args=[])` goes at each block of the
     iterated dominance frontier of Defs(v) where v is live, right
     after the block's leading `Label`.
  5. The renaming walks the dominator tree from ENTRY in preorder. For
     each block B it
       a. gives each Phi's `dst` a new SSA name and pushes it on the
          Var's stack,
       b. for each instruction other than a Phi, in source order,
          rewrites every Var read to the top of its stack, and then
          every Var written to a new SSA name, which it pushes,
       c. for each CFG successor S of B, appends `PhiArg(pred_label=B's
          leading label, source=Var(top of stack[orig_var]))` to every
          Phi at S, finding each Phi by its original Var through the
          record of where step 4 placed it, since its `dst` has been
          renamed by the time it is read,
       d. visits B's children in the dominator tree, and
       e. pops everything pushed in (a) and (b).

A parameter's first SSA name is its own name. The renaming starts each
promotable parameter's stack with its spelling at ENTRY, so the first
read of `p` stays `p`, and the function's `params` need no rewriting.
Later definitions of `p` make `p.1`, `p.2`, and so on.

Each new SSA name gets a `LocalAttr` symbol with the original Var's
type in the symbol table, so that `tac_to_asm` can size its frame slot
after SSA destruction. */
#include "passes/optimization/ssa_construction.h"

#include <cstddef>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "errors.h"
#include "passes/optimization/cfg.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using K = ir::Kind_instruction;
using tac_cfg::BasicBlock;
using tac_cfg::Cfg;
using tac_cfg::kEntryId;
using tac_cfg::kExitId;
using Names = std::set<std::string, std::less<>>;

// Returns `fn` with a new `Label` in front when its body starts with a
// Label, and `fn` as it is otherwise.
//
// A block whose first instruction is a label that jumps reach can have
// predecessors besides ENTRY, such as a back edge from the end of a
// loop. SSA construction can put Phis in such a block whose
// predecessors include the edge from ENTRY, which has no instructions
// and so no label to tag the argument with. The preheader is a block
// with a label that the Phi's argument can name, which SSA destruction
// later finds to emit the copies for the path from the entry.
//
// The label is `.<funcname>@ssa_preheader@<N>`. The prefix of the
// function's name follows the convention of the user's labels
// (label_resolution makes `.<funcname>@<orig>`) and keeps the labels of
// SSA apart between the functions of a program.
ir::Function maybe_prepend_preheader(ir::Function fn) {
  if (fn.instructions.empty() || !asdl::isa<ir::Label>(fn.instructions.front())) {
    return fn;
  }
  Names existing;
  for (const auto& i : fn.instructions) {
    if (const auto* l = asdl::as<ir::Label>(i)) {
      existing.insert(l->name);
    }
  }
  std::string name;
  for (int counter = 0;; ++counter) {
    name = "." + fn.name + "@ssa_preheader@" + std::to_string(counter);
    if (!existing.contains(name)) {
      break;
    }
  }
  fn.instructions.insert(fn.instructions.begin(), ir::Label{.name = name});
  return fn;
}

// Puts a new `Label` at the front of every real block whose first
// instruction isn't one, since the tagging of a Phi's arguments needs
// every block on the path to have a label to name. The labels are
// `.<funcname>@ssa_block@<N>`, which keeps the labels of SSA apart
// between the functions of a program.
void ensure_block_labels(Cfg& cfg, const std::string& fn_name) {
  Names existing;
  for (const auto& [bid, blk] : cfg.blocks) {
    for (const auto& i : blk.instructions) {
      if (const auto* l = asdl::as<ir::Label>(i)) {
        existing.insert(l->name);
      }
    }
  }
  int counter = 0;
  for (const int bid : cfg.block_order) {
    BasicBlock& blk = *cfg.blocks.find(bid);
    if (!blk.instructions.empty() && asdl::isa<ir::Label>(blk.instructions.front())) {
      continue;
    }
    std::string name;
    while (true) {
      name = "." + fn_name + "@ssa_block@" + std::to_string(counter);
      ++counter;
      if (!existing.contains(name)) {
        existing.insert(name);
        break;
      }
    }
    blk.instructions.insert(blk.instructions.begin(), ir::Label{.name = name});
  }
}

// Returns the name of the block's leading `Label`. `ensure_block_labels`
// must have run first, and a block without one is a bug.
const std::string& block_label(const BasicBlock& blk) {
  if (blk.instructions.empty() || !asdl::isa<ir::Label>(blk.instructions.front())) {
    throw InternalError("block " + std::to_string(blk.id) +
                        " has no leading label; call _ensure_block_labels first");
  }
  return std::get<ir::Label>(blk.instructions.front()).name;
}

// Returns whether `t` has a `Volatile` wrapper at its top, under any
// number of `Const`s. The parser writes `const volatile T` as
// `Const(Volatile(T))`, and the test takes either order.
bool is_volatile_qualified(const c99::Type_data_type* t) {
  while (const auto* c = asdl::as<c99::Const>(*t)) {
    t = &*c->referenced_type;
  }
  return asdl::isa<c99::Volatile>(*t);
}

// Returns whether `t` is one of the scalar types a promotable Var may
// have. The 3-, 5-, 6-, and 7-byte integers left of the bit-precise
// types are `BitInt`s.
bool is_scalar(const c99::Type_data_type& t) {
  switch (c99::kind(t)) {
    case c99::Kind_data_type::Int:
    case c99::Kind_data_type::Long:
    case c99::Kind_data_type::LongLong:
    case c99::Kind_data_type::UInt:
    case c99::Kind_data_type::ULong:
    case c99::Kind_data_type::ULongLong:
    case c99::Kind_data_type::Char:
    case c99::Kind_data_type::SChar:
    case c99::Kind_data_type::UChar:
    case c99::Kind_data_type::Float:
    case c99::Kind_data_type::Double:
    case c99::Kind_data_type::Pointer:
    case c99::Kind_data_type::BitInt:
      return true;
    default:
      return false;
  }
}

// Returns the promotable Vars of the function: those whose symbol is a
// `LocalAttr`, whose type is scalar (which leaves out `Array`,
// `Structure`, `Union`, and `FunType`), that aren't volatile-qualified,
// and that are never the operand of a `GetAddress`. C99 §6.7.3.6
// requires every access to a volatile object to be an observable side
// effect, and keeping its value in an SSA temporary joined by Phis
// would remove those accesses. The scalar test already leaves out a
// `Volatile` wrapper, but the explicit test keeps a later change that
// strips qualifiers before the type test from losing volatile's
// meaning. Every block is walked, reachable or not.
Names identify_promotable(const Cfg& cfg, const SymbolTable& symbols) {
  Names address_taken;
  Names candidates;
  for (const auto& [bid, blk] : cfg.blocks) {
    for (const auto& instr : blk.instructions) {
      for (const ir::Type_val* v : vals_in(instr)) {
        if (const auto* var = asdl::as<ir::Var>(*v)) {
          candidates.insert(var->name);
        }
      }
      if (const auto* ga = asdl::as<ir::GetAddress>(instr)) {
        if (const auto* var = asdl::as<ir::Var>(ga->operand)) {
          address_taken.insert(var->name);
        }
      }
    }
  }
  Names promotable;
  for (const auto& name : candidates) {
    if (address_taken.contains(name)) {
      continue;
    }
    const Symbol* sym = symbols.find(name);
    if (sym == nullptr || !std::holds_alternative<LocalAttr>(sym->attrs)) {
      continue;
    }
    if (is_volatile_qualified(&sym->type) || !is_scalar(sym->type)) {
      continue;
    }
    promotable.insert(name);
  }
  return promotable;
}

// Returns the promotable vars live on entry to each block: those read
// on some path from the block's entry before they are written again.
// The Phi placement is pruned by it (Cytron §5.1), since a Phi at B for
// a v that isn't live into B has a dead `dst`, and the copies SSA
// destruction makes for it in the predecessors read temporaries that
// had no value before the join.
std::map<int, Names> compute_live_in(const Cfg& cfg, const Names& promotable) {
  std::map<int, Names> gen;
  std::map<int, Names> kill;
  for (const auto& [bid, blk] : cfg.blocks) {
    Names gen_b;
    Names kill_b;
    for (const auto& instr : blk.instructions) {
      for (const auto u : uses_in(instr)) {
        if (promotable.contains(u) && !kill_b.contains(u)) {
          gen_b.emplace(u);
        }
      }
      for (const auto d : defs_in(instr)) {
        if (promotable.contains(d)) {
          kill_b.emplace(d);
        }
      }
    }
    gen[bid] = std::move(gen_b);
    kill[bid] = std::move(kill_b);
  }
  std::map<int, Names> live_in;
  std::map<int, Names> live_out;
  for (const auto& [bid, blk] : cfg.blocks) {
    live_in[bid];
    live_out[bid];
  }
  bool changed = true;
  while (changed) {
    changed = false;
    for (const auto& [bid, blk] : cfg.blocks) {
      Names new_out;
      for (const int s : blk.successors) {
        const auto& in = live_in[s];
        new_out.insert(in.begin(), in.end());
      }
      Names new_in = gen[bid];
      for (const auto& v : new_out) {
        if (!kill[bid].contains(v)) {
          new_in.insert(v);
        }
      }
      if (new_out != live_out[bid] || new_in != live_in[bid]) {
        live_out[bid] = std::move(new_out);
        live_in[bid] = std::move(new_in);
        changed = true;
      }
    }
  }
  return live_in;
}

// Where step 4 put the Phis of one block: the original Var of each and
// its index in the block, in the order they were placed. A later Phi
// goes after the earlier ones, so the indices stay valid.
using PhisAt = std::map<int, std::vector<std::pair<std::string, std::size_t>>>;

// Inserts `phi` after the block's leading `Label` and any Phis already
// there, or at the front when the block has no leading Label, though
// `ensure_block_labels` should have given it one. Returns its index.
std::size_t insert_phi(BasicBlock& blk, ir::Phi phi) {
  std::size_t pos = 0;
  if (!blk.instructions.empty() && asdl::isa<ir::Label>(blk.instructions.front())) {
    pos = 1;
    while (pos < blk.instructions.size() && asdl::isa<ir::Phi>(blk.instructions[pos])) {
      ++pos;
    }
  }
  blk.instructions.insert(blk.instructions.begin() + static_cast<std::ptrdiff_t>(pos),
                          std::move(phi));
  return pos;
}

// Puts empty Phis for each promotable Var v at every block of the
// iterated dominance frontier of the blocks that define v where v is
// live (pruned SSA, Cytron §5.1), and returns where they went. The
// defining blocks and the frontiers are walked in the order of
// Python's sets.
PhisAt place_phis(Cfg& cfg, const Names& promotable,
                  const OrderedMap<int, PyIntSet>& df, const std::map<int, Names>& live_in) {
  // Defs(v), the blocks that define v, by v in the order first seen.
  OrderedMap<std::string, PyIntSet> defs;
  for (const auto& [bid, blk] : cfg.blocks) {
    for (const auto& instr : blk.instructions) {
      for (const auto d : defs_in(instr)) {
        if (promotable.contains(d)) {
          defs[std::string(d)].add(bid);
        }
      }
    }
  }

  PhisAt phis_at;
  for (const auto& [v, def_blocks] : defs) {
    // The iterated frontier by a worklist, which starts with Defs(v)
    // and grows by each block's frontier until nothing changes.
    std::vector<std::int64_t> worklist = def_blocks.items();
    std::set<std::int64_t> already_placed;
    std::set<std::int64_t> already_visited(def_blocks.sorted());
    while (!worklist.empty()) {
      const std::int64_t x = worklist.back();
      worklist.pop_back();
      const PyIntSet* frontier = df.find(static_cast<int>(x));
      if (frontier == nullptr) {
        continue;
      }
      for (const std::int64_t y : frontier->items()) {
        if (already_placed.contains(y)) {
          continue;
        }
        BasicBlock& blk = *cfg.blocks.find(static_cast<int>(y));
        // A Phi goes only in a block with several predecessors. The
        // frontier ensures that, but the test stays, since the frontier
        // of x may hold x itself for a back edge into x.
        if (blk.predecessors.size() < 2) {
          continue;
        }
        // Skip a block where v isn't live. Its Phi would have a dead
        // dst, and its copies in the predecessors would read
        // uninitialized temporaries on any path that reaches y without
        // defining v first.
        const auto live = live_in.find(static_cast<int>(y));
        if (live == live_in.end() || !live->second.contains(v)) {
          continue;
        }
        const std::size_t idx = insert_phi(blk, ir::Phi{.dst = ir::Var{.name = v}, .args = {}});
        phis_at[static_cast<int>(y)].emplace_back(v, idx);
        already_placed.insert(y);
        if (!already_visited.contains(y)) {
          already_visited.insert(y);
          worklist.push_back(y);
        }
      }
    }
  }
  return phis_at;
}

// The state of the renaming: each promotable Var's count of new names
// and its stack of current names, the names made, and the table the new
// names' symbols go in.
struct Renamer {
  Cfg& cfg;
  const Names& promotable;
  const OrderedMap<int, std::vector<int>>& children;
  const std::map<int, std::string>& block_label_of;
  const PhisAt& phis_at;
  SymbolTable& symbols;
  std::map<std::string, int, std::less<>> counters;
  std::map<std::string, std::vector<std::string>, std::less<>> stacks;
  NameSet ssa_dsts;

  bool promotable_var(const ir::Type_val& v) const {
    const auto* var = asdl::as<ir::Var>(v);
    return var != nullptr && promotable.contains(var->name);
  }

  // Returns a new SSA name for `orig`, with a symbol of its type.
  std::string fresh(const std::string& orig) {
    const int n = ++counters[orig];
    std::string new_name = orig + "." + std::to_string(n);
    if (const Symbol* sym = symbols.find(orig)) {
      Symbol copy{.type = sym->type, .attrs = LocalAttr{}};
      symbols.insert_or_assign(new_name, std::move(copy));
    }
    ssa_dsts.insert(new_name);
    return new_name;
  }

  // Rewrites a read of a promotable Var to the top of its stack. A Var
  // read with no definition before it keeps its name, as in the Phi
  // arguments below.
  void use(ir::Type_val& v) const {
    if (!promotable_var(v)) {
      return;
    }
    const auto& stack = stacks.find(std::get<ir::Var>(v).name)->second;
    if (!stack.empty()) {
      v = ir::Var{.name = stack.back()};
    }
  }

  // Gives a write of a promotable Var a new SSA name and pushes it.
  void def(ir::Type_val& v, std::vector<std::string>& pushed) {
    if (!promotable_var(v)) {
      return;
    }
    const std::string orig = std::get<ir::Var>(v).name;
    std::string name = fresh(orig);
    stacks[orig].push_back(name);
    pushed.push_back(orig);
    v = ir::Var{.name = std::move(name)};
  }

  // Rewrites every promotable Var `instr` reads to the top of its stack,
  // and then gives every one it writes a new SSA name. The reads come
  // first, so that a write of a Var from itself, such as `Copy(x, x)`,
  // reads the old x and writes a new one. The kinds Python's
  // `_rewrite_instruction` doesn't name (`IndexedConstLoad`, the
  // indirect-indexed accesses, `JumpIfCmp`, and `JumpIfMasked`) are left
  // as they are.
  void rewrite_instruction(ir::Type_instruction& instr, std::vector<std::string>& pushed) {
    switch (ir::kind(instr)) {
      case K::Ret: {
        auto& r = std::get<ir::Ret>(instr);
        if (r.val) {
          use(*r.val);
        }
        break;
      }
#define C6502_SRC_DST(Name)                  \
  case K::Name: {                            \
    auto& x = std::get<ir::Name>(instr);     \
    use(x.src);                              \
    def(x.dst, pushed);                      \
    break;                                   \
  }
        C6502_SRC_DST(SignExtend)
        C6502_SRC_DST(ZeroExtend)
        C6502_SRC_DST(Truncate)
        C6502_SRC_DST(IntToFloat)
        C6502_SRC_DST(IntToDouble)
        C6502_SRC_DST(FloatToInt)
        C6502_SRC_DST(DoubleToInt)
        C6502_SRC_DST(FloatToDouble)
        C6502_SRC_DST(DoubleToFloat)
        C6502_SRC_DST(Unary)
        C6502_SRC_DST(Copy)
#undef C6502_SRC_DST
      case K::Binary: {
        auto& x = std::get<ir::Binary>(instr);
        use(x.src1);
        use(x.src2);
        def(x.dst, pushed);
        break;
      }
      case K::GetAddress:
        // The operand names a storage cell and reads no value, so it
        // isn't renamed. The promotable Vars leave out every operand of a
        // GetAddress, so this agrees.
        def(std::get<ir::GetAddress>(instr).dst, pushed);
        break;
      case K::Load: {
        auto& x = std::get<ir::Load>(instr);
        use(x.src_ptr);
        def(x.dst, pushed);
        break;
      }
      case K::Store: {
        auto& x = std::get<ir::Store>(instr);
        use(x.src);
        use(x.dst_ptr);
        break;
      }
      case K::IndexedLoad: {
        auto& x = std::get<ir::IndexedLoad>(instr);
        use(x.index);
        def(x.dst, pushed);
        break;
      }
      case K::IndexedStore: {
        auto& x = std::get<ir::IndexedStore>(instr);
        use(x.index);
        use(x.src);
        break;
      }
      case K::IndexedSymbolStore: {
        auto& x = std::get<ir::IndexedSymbolStore>(instr);
        use(x.index);
        use(x.src);
        break;
      }
      case K::JumpIfTrue:
        use(std::get<ir::JumpIfTrue>(instr).condition);
        break;
      case K::JumpIfFalse:
        use(std::get<ir::JumpIfFalse>(instr).condition);
        break;
      case K::FunctionCall: {
        auto& x = std::get<ir::FunctionCall>(instr);
        for (auto& a : x.args) {
          use(a);
        }
        if (x.dst) {
          def(*x.dst, pushed);
        }
        break;
      }
      case K::IndirectCall: {
        auto& x = std::get<ir::IndirectCall>(instr);
        use(x.ptr);
        for (auto& a : x.args) {
          use(a);
        }
        if (x.dst) {
          def(*x.dst, pushed);
        }
        break;
      }
      default:
        break;
    }
  }

  void visit(int bid) {
    BasicBlock* blk = cfg.blocks.find(bid);
    if (blk == nullptr) {
      return;
    }
    // The original names whose stacks this visit pushes on.
    std::vector<std::string> pushed;

    // 1. Rename the Phis' dsts in this block.
    for (auto& instr : blk->instructions) {
      auto* phi = asdl::as<ir::Phi>(instr);
      if (phi == nullptr || !promotable_var(phi->dst)) {
        continue;
      }
      const std::string orig = std::get<ir::Var>(phi->dst).name;
      std::string name = fresh(orig);
      stacks[orig].push_back(name);
      pushed.push_back(orig);
      phi->dst = ir::Var{.name = std::move(name)};
    }

    // 2. Rename the reads and then the writes of the other
    // instructions.
    for (auto& instr : blk->instructions) {
      if (!asdl::isa<ir::Phi>(instr)) {
        rewrite_instruction(instr, pushed);
      }
    }

    // 3. Fill in the Phis' arguments at every CFG successor. Python
    // computes the block's own label even where the map has it, and
    // raises for a real block without one.
    std::string pred_label;
    if (bid != kEntryId && bid != kExitId) {
      pred_label = block_label(*blk);
    }
    if (const auto it = block_label_of.find(bid); it != block_label_of.end()) {
      pred_label = it->second;
    }
    const std::vector<int> successors = blk->successors;
    for (const int succ_id : successors) {
      const auto phis = phis_at.find(succ_id);
      if (phis == phis_at.end()) {
        continue;
      }
      BasicBlock& succ = *cfg.blocks.find(succ_id);
      for (const auto& [orig_var, idx] : phis->second) {
        // A Var with no definition that reaches this path keeps its
        // name, and the copy after SSA destruction reads whatever that
        // name holds. Well-formed C programs don't do this.
        const auto& stack = stacks.find(orig_var)->second;
        const std::string& src_name = stack.empty() ? orig_var : stack.back();
        std::get<ir::Phi>(succ.instructions[idx])
            .args.push_back(ir::PhiArg{.pred_label = pred_label,
                                       .source = ir::Var{.name = src_name}});
      }
    }

    // 4. Visit the children in the dominator tree.
    if (const auto* kids = children.find(bid)) {
      for (const int child : *kids) {
        visit(child);
      }
    }

    // 5. Pop the stacks, once for each push.
    for (const auto& orig : pushed) {
      stacks[orig].pop_back();
    }
  }
};

}  // namespace

std::pair<ir::Function, NameSet> to_ssa(ir::Function fn, SymbolTable& symbols) {
  fn = maybe_prepend_preheader(std::move(fn));
  Cfg cfg = tac_cfg::build_cfg(fn);
  ensure_block_labels(cfg, fn.name);

  const Names promotable = identify_promotable(cfg, symbols);
  if (promotable.empty()) {
    return {tac_cfg::cfg_to_function(fn, cfg), {}};
  }

  const OrderedMap<int, int> idom = tac_cfg::immediate_dominators(cfg);
  const OrderedMap<int, PyIntSet> df = tac_cfg::dominance_frontiers(cfg);
  const OrderedMap<int, std::vector<int>> children = tac_cfg::dominator_tree_children(idom);
  const std::map<int, Names> live_in = compute_live_in(cfg, promotable);
  // Only real blocks have a leading Label (ENTRY and EXIT are empty
  // sentinels), and a Phi's argument names only real blocks.
  std::map<int, std::string> block_label_of;
  for (const auto& [bid, dom] : idom) {
    if (bid == kEntryId || bid == kExitId) {
      continue;
    }
    const BasicBlock& blk = *cfg.blocks.find(bid);
    if (!blk.instructions.empty()) {
      block_label_of[bid] = block_label(blk);
    }
  }

  const PhisAt phis_at = place_phis(cfg, promotable, df, live_in);

  Renamer r{.cfg = cfg,
            .promotable = promotable,
            .children = children,
            .block_label_of = block_label_of,
            .phis_at = phis_at,
            .symbols = symbols};
  for (const auto& v : promotable) {
    r.counters[v] = 0;
    r.stacks[v];
  }
  // A parameter's first SSA name is its own, pushed at the entry so
  // that the first read gets it. The name is an SSA name too, since its
  // only definition is the function's entry. An address-taken
  // parameter isn't promotable and stays out.
  for (const auto& p : fn.params) {
    if (promotable.contains(p)) {
      r.stacks[p].push_back(p);
      r.ssa_dsts.insert(p);
    }
  }
  r.visit(kEntryId);
  return {tac_cfg::cfg_to_function(fn, cfg), std::move(r.ssa_dsts)};
}

}  // namespace c6502::passes
