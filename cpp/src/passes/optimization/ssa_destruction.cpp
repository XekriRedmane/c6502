/* TAC SSA destruction.

Converts every `Phi` back to `Copy` instructions at the end of each
predecessor block. After this pass no Phi is left in the function, and
the result is TAC out of SSA form, ready for `tac_to_asm`, which has no
lowering of a Phi.

For each Phi in block M,

    Phi(dst, args=[(pred_label_1, src_1), ..., (pred_label_n, src_n)])

the pass emits one `Copy(src_k, dst)` at the end of pred_k's block, just
before its terminator (`Ret`, `Jump`, or a conditional jump) or at the
end of its instructions when it has none.

The order of parallel copies. Several Phis in one block give several
Copies in each predecessor. Emitting them in source order can read a
slot an earlier Copy just wrote, the classic "lost copy" problem. For
example, after copy propagation,

    Phi(@2.i.2, [..., (continue, %4.1)])             ; i = %4.1
    Phi(@1.counter.1, [..., (continue, @2.i.2)])     ; counter = i

in source order at the end of `continue` gives

    Copy(%4.1, @2.i.2)         ; @2.i.2 := %4.1   (writes @2.i.2)
    Copy(@2.i.2, @1.counter.1) ; reads @2.i.2 after it was overwritten

The fix is a topological sort, which first emits a Copy whose dst no
other pending Copy reads. The two Copies above swap:

    Copy(@2.i.2, @1.counter.1) ; counter := old @2.i.2
    Copy(%4.1, @2.i.2)         ; now overwrite @2.i.2

Cycles (Copies that depend on each other, as in `a, b = b, a`) need a
new temporary to break them. When `from_ssa` is given a symbol table,
it makes a `.<funcname>@cycle_tmp@<N>` Var per cycle, gives it the type
of the cycle's member in the table, and rewrites the cycle as `tmp =
first; first = second; ... last = tmp`. Without a table, which only
older callers leave out, a cycle's copies stay in source order. The
optimizer always passes the table.

The cycle's temporary has no coloring (the register allocation has
already run), so `replace_pseudoregisters` gives it a frame slot, which
is correct and slower than a copy in zero page. Cycles are rare in
well-typed C code, so the slow path costs little. */
#include "passes/optimization/ssa_destruction.h"

#include <algorithm>
#include <cstddef>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "errors.h"
#include "ordered_map.h"
#include "passes/optimization/cfg.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using K = ir::Kind_instruction;
using tac_cfg::BasicBlock;
using tac_cfg::Cfg;
using Names = std::set<std::string, std::less<>>;
using Edge = std::pair<int, int>;

// Returns whether `instr` is one of the conditional jumps.
bool is_conditional(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::JumpIfTrue:
    case K::JumpIfFalse:
    case K::JumpIfCmp:
    case K::JumpIfMasked:
      return true;
    default:
      return false;
  }
}

// Returns the target of a conditional jump.
const std::string& conditional_target(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::JumpIfTrue:
      return std::get<ir::JumpIfTrue>(instr).target;
    case K::JumpIfFalse:
      return std::get<ir::JumpIfFalse>(instr).target;
    case K::JumpIfCmp:
      return std::get<ir::JumpIfCmp>(instr).target;
    case K::JumpIfMasked:
      return std::get<ir::JumpIfMasked>(instr).target;
    default:
      throw InternalError("AttributeError: the instruction has no target");
  }
}

// Returns the conditional jump `instr` with its target set to `target`.
ir::Type_instruction retarget(ir::Type_instruction instr, const std::string& target) {
  switch (ir::kind(instr)) {
    case K::JumpIfTrue:
      std::get<ir::JumpIfTrue>(instr).target = target;
      break;
    case K::JumpIfFalse:
      std::get<ir::JumpIfFalse>(instr).target = target;
      break;
    case K::JumpIfCmp:
      std::get<ir::JumpIfCmp>(instr).target = target;
      break;
    case K::JumpIfMasked:
      std::get<ir::JumpIfMasked>(instr).target = target;
      break;
    default:
      break;
  }
  return instr;
}

// Returns whether `instr` ends a block (Python's `_TERMINATOR_TYPES`).
bool is_terminator(const ir::Type_instruction& instr) {
  return asdl::isa<ir::Ret, ir::Jump>(instr) || is_conditional(instr);
}

// Returns the name of a Phi's or a Copy's destination, which is a Var
// in SSA form. Python reads `.name` of it and raises for a Constant.
const std::string& var_name(const ir::Type_val& v) {
  const auto* var = asdl::as<ir::Var>(v);
  if (var == nullptr) {
    throw InternalError("AttributeError: 'Constant' object has no attribute 'name'");
  }
  return var->name;
}

// Returns the Var names live on entry to each block. A Phi's
// destination is defined at its block's entry, and a Phi's argument
// counts as a read at the end of the predecessor it comes from.
std::map<int, Names> live_in(const Cfg& cfg) {
  std::map<std::string, int, std::less<>> label_of;
  for (const auto& [bid, b] : cfg.blocks) {
    if (!b.instructions.empty()) {
      if (const auto* l = asdl::as<ir::Label>(b.instructions.front())) {
        label_of.insert_or_assign(l->name, bid);
      }
    }
  }
  std::map<int, Names> use;
  std::map<int, Names> defs;
  // The live-out reads of each predecessor.
  std::map<int, Names> phi_uses;
  for (const auto& [bid, blk] : cfg.blocks) {
    Names u;
    Names d;
    for (const auto& ins : blk.instructions) {
      if (const auto* phi = asdl::as<ir::Phi>(ins)) {
        d.insert(var_name(phi->dst));
        for (const auto& a : phi->args) {
          const auto& arg = std::get<ir::PhiArg>(a);
          const auto pb = label_of.find(arg.pred_label);
          const auto* src = asdl::as<ir::Var>(arg.source);
          if (pb != label_of.end() && src != nullptr) {
            phi_uses[pb->second].insert(src->name);
          }
        }
        continue;
      }
      for (const auto v : uses_in(ins)) {
        if (!d.contains(v)) {
          u.emplace(v);
        }
      }
      for (const auto v : defs_in(ins)) {
        d.emplace(v);
      }
    }
    use[bid] = std::move(u);
    defs[bid] = std::move(d);
  }
  std::map<int, Names> live;
  for (const auto& [bid, blk] : cfg.blocks) {
    live[bid];
  }
  bool changed = true;
  while (changed) {
    changed = false;
    for (const auto& [bid, blk] : cfg.blocks) {
      Names out = phi_uses[bid];
      for (const int sid : blk.successors) {
        const auto& in = live[sid];
        out.insert(in.begin(), in.end());
      }
      Names next = use[bid];
      for (const auto& v : out) {
        if (!defs[bid].contains(v)) {
          next.insert(v);
        }
      }
      if (next != live[bid]) {
        live[bid] = std::move(next);
        changed = true;
      }
    }
  }
  return live;
}

// Returns whether the copies for the edge pid -> sid can't simply go in
// front of pid's terminator.
//
// They can when pid has one successor. When it ends in a conditional
// jump (a critical edge if sid has other predecessors), copies in front
// of the jump also run on the other edge, which is harmless only when
// the jump reads none of their destinations, none is live into the
// other successor, and the other edge has no copies of its own. Loop
// rotation makes this case common: the bottom test of a rotated loop
// feeds Phis at both the loop's header and its exit, and the header's
// copy of `i = i + 1` would otherwise overwrite the `i` the exit reads.
bool needs_split(int pid, int sid, const std::vector<ir::Copy>& copies,
                 const ir::Type_instruction* last, const Cfg& cfg,
                 const OrderedMap<Edge, std::vector<ir::Copy>>& edge_copies,
                 const std::map<int, Names>& live) {
  if (last == nullptr || !is_conditional(*last)) {
    return false;
  }
  std::vector<int> others;
  for (const int t : cfg.blocks.find(pid)->successors) {
    if (t != sid) {
      others.push_back(t);
    }
  }
  if (others.empty()) {
    return false;
  }
  Names dsts;
  for (const auto& c : copies) {
    dsts.insert(var_name(c.dst));
  }
  for (const auto v : uses_in(*last)) {
    if (dsts.contains(v)) {
      return true;
    }
  }
  for (const int t : others) {
    if (edge_copies.contains(Edge{pid, t})) {
      return true;
    }
    const auto it = live.find(t);
    if (it != live.end()) {
      for (const auto& d : dsts) {
        if (it->second.contains(d)) {
          return true;
        }
      }
    }
  }
  return false;
}

// Returns whether `v` is a Var named `name`.
bool is_var_named(const ir::Type_val& v, const std::string& name) {
  const auto* var = asdl::as<ir::Var>(v);
  return var != nullptr && var->name == name;
}

// Returns `copies` sorted so that no Copy's dst is read by a later Copy
// of the output: it emits a Copy whose dst is the src of no other
// pending Copy, removes it, and repeats.
//
// When the Copies left all form cycles, it breaks one with a new
// temporary. It takes the first pending Copy (src, dst), saves dst's
// value in the temporary, and makes each other pending Copy that reads
// dst read the temporary instead. That frees dst, and the sort goes on.
//
// `cycle_counter` numbers the temporaries of one function, and
// `symbols` gives the type the temporary gets. Without `symbols`, a
// cycle's copies stay in source order, which can miscompile, so the
// optimizer's driver passes it.
std::vector<ir::Copy> order_parallel_copies(const std::vector<ir::Copy>& copies,
                                            const std::string& fn_name, int& cycle_counter,
                                            SymbolTable* symbols) {
  if (copies.size() <= 1) {
    return copies;
  }
  std::vector<ir::Copy> pending = copies;
  std::vector<ir::Copy> out;
  while (!pending.empty()) {
    // Find a pending Copy whose dst no other pending Copy reads.
    std::optional<std::size_t> ready;
    for (std::size_t i = 0; i < pending.size(); ++i) {
      const auto* d = asdl::as<ir::Var>(pending[i].dst);
      if (d == nullptr) {
        ready = i;
        break;
      }
      bool blocks_other = false;
      for (std::size_t j = 0; j < pending.size(); ++j) {
        if (j != i && is_var_named(pending[j].src, d->name)) {
          blocks_other = true;
          break;
        }
      }
      if (!blocks_other) {
        ready = i;
        break;
      }
    }
    if (ready) {
      out.push_back(std::move(pending[*ready]));
      pending.erase(pending.begin() + static_cast<std::ptrdiff_t>(*ready));
      continue;
    }
    // Every Copy left is in a cycle.
    if (symbols == nullptr) {
      // The fallback for older callers emits them in source order. A
      // cycle miscompiles, but the shape stays.
      out.insert(out.end(), pending.begin(), pending.end());
      break;
    }
    const ir::Copy chosen = pending.front();
    const auto* chosen_dst = asdl::as<ir::Var>(chosen.dst);
    if (chosen_dst == nullptr) {
      // The Copies of Phis in SSA form always write a Var, so this
      // case is defensive, as in Python.
      out.insert(out.end(), pending.begin(), pending.end());
      break;
    }
    const std::string d = chosen_dst->name;
    ++cycle_counter;
    const std::string tmp_name = "." + fn_name + "@cycle_tmp@" + std::to_string(cycle_counter);
    // The temporary has the type of the value it saves, the dst's.
    if (const Symbol* sym = symbols->find(d)) {
      Symbol copy{.type = sym->type, .attrs = LocalAttr{}};
      symbols->insert_or_assign(tmp_name, std::move(copy));
    }
    // Save the old value of dst in the temporary.
    out.push_back(ir::Copy{.src = ir::Var{.name = d}, .dst = ir::Var{.name = tmp_name}});
    // The pending Copies that read dst read the temporary instead, so
    // nothing pending reads dst, and the chosen Copy can go next.
    for (std::size_t j = 1; j < pending.size(); ++j) {
      if (is_var_named(pending[j].src, d)) {
        pending[j] = ir::Copy{.src = ir::Var{.name = tmp_name}, .dst = pending[j].dst};
      }
    }
  }
  return out;
}

}  // namespace

ir::Function from_ssa(ir::Function fn, SymbolTable* symbols) {
  Cfg cfg = tac_cfg::build_cfg(fn);
  std::map<std::string, int, std::less<>> label_to_block;
  for (const auto& [bid, b] : cfg.blocks) {
    if (!b.instructions.empty()) {
      if (const auto* l = asdl::as<ir::Label>(b.instructions.front())) {
        label_to_block.insert_or_assign(l->name, b.id);
      }
    }
  }
  int cycle_counter = 0;
  const std::map<int, Names> live = live_in(cfg);

  // One list of Copies per CFG edge (the predecessor, the Phi's block),
  // in the order Python's dict holds them.
  OrderedMap<Edge, std::vector<ir::Copy>> edge_copies;
  for (const int bid : cfg.block_order) {
    for (const auto& ins : cfg.blocks.find(bid)->instructions) {
      const auto* phi = asdl::as<ir::Phi>(ins);
      if (phi == nullptr) {
        continue;
      }
      for (const auto& a : phi->args) {
        const auto& arg = std::get<ir::PhiArg>(a);
        const auto pred = label_to_block.find(arg.pred_label);
        if (pred == label_to_block.end()) {
          // The predecessor was removed (by an earlier pass that dropped
          // unreachable code, say), so the argument is dead.
          continue;
        }
        edge_copies[Edge{pred->second, bid}].push_back(
            ir::Copy{.src = arg.source, .dst = phi->dst});
      }
    }
  }

  int split_counter = 0;
  // The index of each block's last instruction before any copies go
  // in. A block whose fall-through edge gets its copies after its
  // conditional jump no longer ends in the jump, and the copies for its
  // other edge have to see the jump.
  std::map<int, std::ptrdiff_t> last_index;
  for (const auto& [bid, blk] : cfg.blocks) {
    last_index[bid] = static_cast<std::ptrdiff_t>(blk.instructions.size()) - 1;
  }
  for (const auto& [edge, copies] : edge_copies) {
    const auto [pid, sid] = edge;
    const std::vector<ir::Copy> ordered =
        order_parallel_copies(copies, fn.name, cycle_counter, symbols);
    const std::ptrdiff_t k = last_index[pid];
    std::optional<ir::Type_instruction> last;
    if (k >= 0) {
      last = cfg.blocks.find(pid)->instructions[static_cast<std::size_t>(k)];
    }
    auto& pred_instrs = cfg.blocks.find(pid)->instructions;
    if (!needs_split(pid, sid, copies, last ? &*last : nullptr, cfg, edge_copies, live)) {
      std::ptrdiff_t insert_pos = k + 1;
      if (last && is_terminator(*last)) {
        insert_pos = k;
        last_index[pid] = k + static_cast<std::ptrdiff_t>(ordered.size());
      }
      pred_instrs.insert(pred_instrs.begin() + insert_pos, ordered.begin(), ordered.end());
      continue;
    }
    const auto& succ_first = cfg.blocks.find(sid)->instructions.front();
    const auto* succ_label_instr = asdl::as<ir::Label>(succ_first);
    if (succ_label_instr == nullptr) {
      throw InternalError("AttributeError: the Phi's block doesn't start with a Label");
    }
    const std::string succ_label = succ_label_instr->name;
    if (conditional_target(*last) != succ_label) {
      // The Phi's block is the fall-through successor, so the copies go
      // after the conditional jump, where only the path that falls
      // through runs them.
      pred_instrs.insert(pred_instrs.end(), ordered.begin(), ordered.end());
      continue;
    }
    // The Phi's block is the jump's target, so the jump goes through a
    // new block that runs the copies and jumps on.
    ++split_counter;
    const std::string split_label = "." + fn.name + "@ssa_split@" + std::to_string(split_counter);
    pred_instrs[static_cast<std::size_t>(k)] = retarget(*last, split_label);
    int new_id = 0;
    for (const auto& [bid, blk] : cfg.blocks) {
      new_id = std::max(new_id, bid);
    }
    ++new_id;
    BasicBlock split{.id = new_id};
    split.instructions.push_back(ir::Label{.name = split_label});
    split.instructions.insert(split.instructions.end(), ordered.begin(), ordered.end());
    split.instructions.push_back(ir::Jump{.target = succ_label});
    cfg.blocks.insert_or_assign(new_id, std::move(split));
    cfg.block_order.push_back(new_id);
  }

  for (const int bid : cfg.block_order) {
    auto& instrs = cfg.blocks.find(bid)->instructions;
    std::erase_if(instrs, [](const ir::Type_instruction& i) { return asdl::isa<ir::Phi>(i); });
  }

  return tac_cfg::cfg_to_function(fn, cfg);
}

}  // namespace c6502::passes
