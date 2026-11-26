// The asm-level register allocation.
//
// It works on bytes: every node of the interference graph is one byte
// wide, since asm SSA has already split each value of several bytes
// into independent byte-versioned variables, apart from the blocks of
// several bytes that `build_interference` keeps whole. The coloring runs
// a perfect elimination order and a greedy fit on the asm CFG and the
// asm SSA names.
//
// It reuses `Coloring` and `Pool` of passes/optimization, which don't
// depend on the IR. Only the building of the interference graph and the
// derivation of the order differ.
//
// The pool is chosen this way.
//
//   * `lives_across_call` set: callee-saved first, saved by the
//     function's prologue and epilogue around any call it makes.
//   * `lives_across_call` clear: caller-saved first, with no saving and
//     restoring, since only a call this function doesn't make clobbers
//     it.
//
// A spill goes in `Coloring.spilled`, and replace_pseudoregisters gives
// it a Frame slot.
//
// # Coloring to X and Y
//
// Some nodes may be pinned to the 6502's X or Y index register in place
// of a zero-page byte. `hwreg_eligibility::scan_function` computes the
// eligibility beforehand: every definition and use must be an operation
// of the LDX / LDY / STX / STY / INX / DEX / CPX / CPY kind. The pass
// before the coloring then tries X or Y for the eligible nodes with a
// hint (typically a pseudo that feeds the index setup `Mov(P, A);
// Mov(A, X|Y)` before an `IndexedData` access).
//
// The hard constraints on a node colored to a register:
//
//   * Width 1 and offset 0, a single-byte SSA name.
//   * No call in the live range writes the register. The helpers
//     clobber A, X, and Y, but a routine declared `preserves_x` /
//     `preserves_y` leaves that one alone.
//   * No interference with another node already pinned to the same
//     register.
//
// X and Y are independent, so a node in X and one in Y may interfere
// freely, and several nodes that don't interfere may share one
// register.
//
// The colors in registers are in `Coloring.hwreg_assignments`, a map
// from name to "X" or "Y" beside the zero-page `assignments` and
// `spilled`, which are unchanged for the other names.
#include "passes/optimization_asm/regalloc.h"

#include <algorithm>
#include <map>
#include <tuple>
#include <utility>
#include <vector>

#include "errors.h"
#include "passes/asm_liveness.h"
#include "passes/optimization_asm/all_operands.h"
#include "passes/optimization_asm/cfg.h"

namespace c6502::passes {

namespace ir = ::c6502::asm_ast;

namespace {

using Position = std::pair<int, std::size_t>;

void add_if_xy(std::vector<char>& out, const ir::Type_operand& dst) {
  const auto* r = asdl::as<ir::Reg>(dst);
  if (r == nullptr) {
    return;
  }
  if (asdl::isa<ir::X>(r->reg)) {
    out.push_back('X');
  } else if (asdl::isa<ir::Y>(r->reg)) {
    out.push_back('Y');
  }
}

}  // namespace

// Returns the registers X and Y that `instr` writes, as letters, for
// the check of clobbers in a live range in `try_hwreg_assign`.
//
// What writes X or Y in the asm IR at this point of the pipeline:
//
//   * `Mov(_, Reg(X|Y))`, which is TAX / TAY / LDX / LDY with any
//     addressing mode the emitter supports.
//   * `Inc(Reg(X|Y))` / `Dec(Reg(X|Y))`, which is INX / INY / DEX / DEY.
//   * `Pop(dst=Reg(X|Y))`, which tac_to_asm doesn't write today, but
//     the IR allows it.
//   * `Call`, X and Y, except a register the callee is declared to
//     preserve (`asm_liveness.call_preserves`).
//
// Left out: `Mov(Reg(X|Y), _)`, `Push(src=Reg(X|Y))`, and
// `Compare(Reg(X|Y), _)`, which read X or Y; Add, Sub, And, Or, Xor,
// and the shifts and rotates, whose destination the IR's contract
// limits to A; and Phi, whose destination is still a pseudo here.
//
// An operand lowered with its own `LDY #offset` (`Indirect`,
// `IndirectZp`, `Frame`, `Stack`) counts as a write of Y. A load or
// store through a pointer, such as `arr[i]` of an element of 8 bytes, is
// already made of `Indirect` operands here. A `Pseudo` named in
// `frame_names` counts too, since it becomes a `Frame` operand after the
// allocation.
std::vector<char> instr_writes_hwregs(const ir::Type_instruction& instr,
                                      const NameSet* frame_names) {
  std::vector<char> out;
  if (asdl::isa<ir::DecimalArith>(instr)) {
    // An operand of a BCD chain may still become a soft-stack slot,
    // whose access loads Y, so the chain counts as writing Y.
    return {'Y'};
  }
  for (const auto* op : optimizer::all_operands_in(instr)) {
    if (asdl::isa<ir::Indirect, ir::IndirectZp, ir::Frame, ir::Stack>(*op)) {
      out.push_back('Y');
      break;
    }
    const auto* p = asdl::as<ir::Pseudo>(*op);
    if (p != nullptr && frame_names != nullptr && frame_names->contains(p->name)) {
      out.push_back('Y');
      break;
    }
  }
  using K = ir::Kind_instruction;
  switch (ir::kind(instr)) {
    case K::Mov:
      add_if_xy(out, std::get<ir::Mov>(instr).dst);
      break;
    case K::Pop:
      add_if_xy(out, std::get<ir::Pop>(instr).dst);
      break;
    case K::Inc:
      add_if_xy(out, std::get<ir::Inc>(instr).dst);
      break;
    case K::Dec:
      add_if_xy(out, std::get<ir::Dec>(instr).dst);
      break;
    case K::Call:
      for (const char letter : {'X', 'Y'}) {
        if (!call_preserves(instr, std::string(1, letter)) &&
            std::find(out.begin(), out.end(), letter) == out.end()) {
          out.push_back(letter);
        }
      }
      break;
    default:
      break;
  }
  return out;
}

namespace {

// Pins candidates to X or Y greedily before the coloring, adding to
// `hwreg_assignments`. Each hinted candidate gets its preferred register
// (X for `hints_x`, Y for `hints_y`), and when an interfering neighbor
// already has that one, the other, before it gives up. Several
// candidates whose live ranges overlap and that share an X hint (typical
// in unrolled loops, where each iteration's index feeds a chain of
// `IndexedData`) can then split between X and Y, saving the setup of
// each.
//
// The order is `hints_x` first, preferring X, then `hints_y`, preferring
// Y, each by priority and then name. A name in both is handled with
// `hints_x`, since X is what tac_to_asm targets.
//
// Each name gets at most one register. Its preference is decided once,
// with one fallback to the other register, and the decision is final:
// nothing is shuffled once assigned, since the assigned name's color
// meets all its constraints by construction.
void try_hwreg_assign(const InterferenceGraph& graph, const HwRegEligibility& eligibility,
                      OrderedMap<std::string, std::string>& hwreg_assignments,
                      const Liveness& liveness,
                      const OrderedMap<std::string, std::string>* rep_map_arg,
                      const NameSet* frame_names) {
  // The positions of the writes of each register and of the definitions
  // of each pseudo. The interference graph has edges only between
  // pseudos and none with the registers, so a pseudo whose live range
  // crosses an instruction that writes X or Y, with no neighbor in the
  // graph at that position, would otherwise slip through.
  //
  // `rep_map` is the coalescing's result (`CoalesceResult.representative`).
  // The eligibility holds the representatives after coalescing, while
  // the liveness holds the SSA names before it, so each name of a live
  // set is projected through `rep_map` before the comparison.
  std::map<char, std::vector<Position>> clobber_positions{{'X', {}}, {'Y', {}}};
  // All the positions of each representative's definitions. A
  // representative of a coalesced class may have several (each SSA name
  // had one, and after coalescing they share a name), and each is a
  // legitimate write of the representative into its register after
  // pinning, and no clobber.
  std::map<std::string, std::set<Position>, std::less<>> def_positions_rep;
  static const OrderedMap<std::string, std::string> kNoReps;
  const auto& rep_map = rep_map_arg != nullptr ? *rep_map_arg : kNoReps;

  const auto resolve_rep = [&](const std::string& name) {
    std::string cur = name;
    while (const std::string* next = rep_map.find(cur)) {
      cur = *next;
    }
    return cur;
  };

  for (std::size_t b = 0; b < liveness.cfg.blocks.size(); ++b) {
    const int bid = static_cast<int>(b);
    const auto instrs = liveness.cfg.instructions(bid);
    for (std::size_t idx = 0; idx < instrs.size(); ++idx) {
      for (const char letter : instr_writes_hwregs(instrs[idx], frame_names)) {
        clobber_positions[letter].emplace_back(bid, idx);
      }
      for (const auto* d : liveness::defs_in(instrs[idx])) {
        def_positions_rep[resolve_rep(d->name)].emplace(bid, idx);
      }
    }
  }

  // Returns whether the instruction at (bid, idx) is the second half of
  // the index setup of `name`: a `Mov(Reg(A), Reg(R))` right after a
  // `Mov(Pseudo N, Reg(A))` with rep(N) == name. Once `name` is pinned
  // to R the pair is `Mov(Reg(R), Reg(A)); Mov(Reg(A), Reg(R))`, a
  // transfer of R to itself that `rewrite_redundant_transfers` drops
  // later. So it doesn't clobber R for `name`'s live range, since the
  // value placed in R is `name`'s.
  const auto is_self_transfer_chain = [&](int bid, std::size_t idx, const std::string& name) {
    if (idx == 0) {
      return false;
    }
    const auto instrs = liveness.cfg.instructions(bid);
    const auto* instr = asdl::as<ir::Mov>(instrs[idx]);
    if (instr == nullptr) {
      return false;
    }
    const auto* src = asdl::as<ir::Reg>(instr->src);
    if (src == nullptr || !asdl::isa<ir::A>(src->reg)) {
      return false;
    }
    const auto* prev = asdl::as<ir::Mov>(instrs[idx - 1]);
    if (prev == nullptr) {
      return false;
    }
    const auto* p = asdl::as<ir::Pseudo>(prev->src);
    const auto* dst = asdl::as<ir::Reg>(prev->dst);
    if (p == nullptr || dst == nullptr || !asdl::isa<ir::A>(dst->reg) || p->offset != 0) {
      return false;
    }
    return resolve_rep(p->name) == name;
  };

  // `name` is a representative here, since the eligibility is projected.
  // Any of its coalesced definitions is fine, and only an instruction
  // that writes `reg` and isn't one of `name`'s definitions counts as a
  // clobber. The index setup `Mov(Pseudo name, A); Mov(A, Reg(R))`
  // becomes a transfer of R to itself after pinning, so its second `Mov`
  // isn't one either.
  const auto clobbered_in_live_range = [&](const std::string& name, char reg) {
    const auto& positions = clobber_positions[reg];
    if (positions.empty()) {
      return false;
    }
    static const std::set<Position> kNone;
    const auto it = def_positions_rep.find(name);
    const auto& def_pos_set = it == def_positions_rep.end() ? kNone : it->second;
    for (const auto& [bid, idx] : positions) {
      if (def_pos_set.contains(Position{bid, idx})) {
        continue;
      }
      if (is_self_transfer_chain(bid, idx, name)) {
        continue;
      }
      for (const auto& live_name : liveness.live_after(bid, idx)) {
        if (resolve_rep(live_name) == name) {
          return true;
        }
      }
    }
    return false;
  };

  const auto can_pin = [&](const std::string& name, char reg) {
    // The eligibility is per register: a name may go in X only when
    // it's in `eligible_x`, and in Y only when it's in `eligible_y`.
    // Most names are in both (the symmetric peer shapes), and the
    // asymmetric `IndexedData` peer is why the test is per register.
    if (reg == 'X' && !eligibility.eligible_x.contains(name)) {
      return false;
    }
    if (reg == 'Y' && !eligibility.eligible_y.contains(name)) {
      return false;
    }
    const auto node = graph.nodes.find(name);
    if (node == graph.nodes.end()) {
      return false;
    }
    if (node->second.width != 1) {
      return false;
    }
    if (hwreg_assignments.contains(name)) {
      return false;
    }
    const std::string letter(1, reg);
    for (const auto& nbr : graph.neighbors(name)) {
      const std::string* other = hwreg_assignments.find(nbr);
      if (other != nullptr && *other == letter) {
        return false;
      }
    }
    if (clobbered_in_live_range(name, reg)) {
      return false;
    }
    return true;
  };

  const auto try_pin_with_fallback = [&](const std::string& name, char preferred) {
    const char other = preferred == 'X' ? 'Y' : 'X';
    if (can_pin(name, preferred)) {
      hwreg_assignments.insert_or_assign(name, std::string(1, preferred));
      return;
    }
    if (can_pin(name, other)) {
      hwreg_assignments.insert_or_assign(name, std::string(1, other));
    }
  };

  // The priority is the use count, highest first, and then the name, for
  // determinism. The name with the most uses gets a register first, since
  // pinning a name used often removes many setup chains, and one used
  // rarely, few. The usual example is a column's induction variable used
  // as the index of a whole run of indexed stores, with many references
  // and much to save when pinned.
  const auto priority = [&](const std::string& name) {
    const asdl::Int* n = eligibility.use_count.find(name);
    return std::make_pair(-(n != nullptr ? *n : asdl::Int(0)), name);
  };
  const auto by_priority = [&](const NameSet& names) {
    std::vector<std::string> out(names.begin(), names.end());
    std::stable_sort(out.begin(), out.end(), [&](const std::string& a, const std::string& b) {
      return priority(a) < priority(b);
    });
    return out;
  };

  // The required pinnings, from a `register` local, which is always Y.
  // They are hard: when the requested register can't be had, the compile
  // fails, since falling back to a zero-page slot would break the user's
  // contract. They are handled before the hints, so they claim their
  // registers before any hinted candidate competes. (A `register`
  // parameter takes the soft path of the hints.)
  //
  // `register_pins`, their source upstream, maps the base names of the
  // c99 IR, and `ssa_base` matches every SSA version of them. Some
  // derived names, notably the spelling of a parameter's arrival that
  // ssa_construction pushes as the initial value of a promoted
  // parameter, never become a byte variable of their own in the graph
  // and only appear as values on the renaming stack. Names absent from
  // the graph are skipped, since there's no node to pin, and the SSA
  // versions of the same base that are pinned, which are what reach the
  // body, still get the register.
  const auto required = [&](const NameSet& names, char reg) {
    for (const auto& name : names) {
      if (hwreg_assignments.contains(name) || !graph.nodes.contains(name)) {
        continue;
      }
      if (!can_pin(name, reg)) {
        const std::string r(1, reg);
        throw CompileError("RegallocError",
                           "can't pin `" + name + "` to " + r +
                               " \xE2\x80\x94 the `register` declaration conflicts with the "
                               "IR's use of " +
                               r +
                               " for this value (operand shape, call-clobber, or "
                               "interference with another required-" +
                               r + " value)");
      }
      hwreg_assignments.insert_or_assign(name, std::string(1, reg));
    }
  };
  required(eligibility.required_x, 'X');
  required(eligibility.required_y, 'Y');

  // `hints_x` prefer X and fall back to Y. The names pinned already are
  // skipped, and `can_pin`'s test per register lets the fallback fire
  // only when the name is eligible for the other register.
  for (const auto& name : by_priority(eligibility.hints_x)) {
    if (hwreg_assignments.contains(name)) {
      continue;
    }
    if (!eligibility.eligible_x.contains(name) && !eligibility.eligible_y.contains(name)) {
      continue;
    }
    try_pin_with_fallback(name, 'X');
  }
  // `hints_y` prefer Y and fall back to X, skipping names assigned
  // already (such as one in both hint sets that got X).
  for (const auto& name : by_priority(eligibility.hints_y)) {
    if (hwreg_assignments.contains(name)) {
      continue;
    }
    if (!eligibility.eligible_x.contains(name) && !eligibility.eligible_y.contains(name)) {
      continue;
    }
    try_pin_with_fallback(name, 'Y');
  }

  // The upper bytes of adds and subtracts of a byte come last, so they
  // only get a register no index wanted (see "Carry hint" in
  // hwreg_eligibility.cpp). The value before the add and the one after
  // share a register or get none: with only one in X, the other would be
  // copied into or out of X and the rewrite would save nothing.
  std::set<std::pair<std::string, std::string>> pair_set;
  for (const auto& pq : eligibility.carry_pairs) {
    if (eligibility.hints_carry.contains(pq.first) && eligibility.hints_carry.contains(pq.second)) {
      pair_set.insert(pq);
    }
  }
  std::vector<std::pair<std::string, std::string>> pairs(pair_set.begin(), pair_set.end());
  std::stable_sort(pairs.begin(), pairs.end(), [&](const auto& a, const auto& b) {
    return std::make_pair(priority(a.first), priority(a.second)) <
           std::make_pair(priority(b.first), priority(b.second));
  });
  for (const auto& [p, q] : pairs) {
    if (hwreg_assignments.contains(p) || hwreg_assignments.contains(q)) {
      continue;
    }
    for (const char reg : {'X', 'Y'}) {
      if (!can_pin(p, reg)) {
        continue;
      }
      hwreg_assignments.insert_or_assign(p, std::string(1, reg));
      if (p == q || can_pin(q, reg)) {
        hwreg_assignments.insert_or_assign(q, std::string(1, reg));
        break;
      }
      hwreg_assignments.erase(p);
    }
  }
}

// Returns an elimination order of `graph`'s nodes: a preorder walk of
// the dominator tree that lists each definition, reversed.
std::vector<std::string> perfect_elimination_order(const ir::Function& fn,
                                                   const InterferenceGraph& graph) {
  const Cfg cfg = build_cfg(fn);
  const auto children = dominator_tree_children(immediate_dominators(cfg));

  std::vector<std::string> build;
  NameSet seen;
  const auto emit = [&](const std::string& name) {
    if (seen.contains(name) || !graph.nodes.contains(name)) {
      return;
    }
    seen.insert(name);
    build.push_back(name);
  };

  std::vector<int> stack{kEntryId};
  while (!stack.empty()) {
    const int bid = stack.back();
    stack.pop_back();
    const auto instrs = cfg.instructions(bid);
    // The Phi destinations first, since they are defined in parallel at
    // the block's entry.
    for (const auto& instr : instrs) {
      if (const auto* phi = asdl::as<ir::Phi>(instr)) {
        if (const auto* d = asdl::as<ir::Pseudo>(phi->dst)) {
          emit(d->name);
        }
      }
    }
    // Then the other definitions in source order.
    for (const auto& instr : instrs) {
      if (asdl::isa<ir::Phi>(instr)) {
        continue;
      }
      for (const auto* d : liveness::defs_in(instr)) {
        emit(d->name);
      }
    }
    if (static_cast<std::size_t>(bid) < children.size()) {
      const auto& kids = children[static_cast<std::size_t>(bid)];
      for (auto it = kids.rbegin(); it != kids.rend(); ++it) {
        stack.push_back(*it);
      }
    }
  }

  // The nodes without a definition go last, in name order.
  for (const auto& [name, node] : graph.nodes) {
    if (!seen.contains(name)) {
      build.push_back(name);
    }
  }
  std::reverse(build.begin(), build.end());
  return build;
}

// Colors the nodes of `graph` that `out.hwreg_assignments` doesn't hold
// onto zero-page bytes in the order `peo`, adding them to
// `out.assignments`, or to `out.spilled` when none fits.
void color_bytes(const InterferenceGraph& graph, const std::vector<std::string>& peo,
                 const Pool& pool, const std::set<asdl::Int>* blocked_addrs,
                 const std::optional<Range>& allowed_range, Coloring& out) {
  for (const auto& name : peo) {
    if (out.hwreg_assignments.contains(name)) {
      continue;
    }
    const auto& node = graph.nodes.at(name);
    auto blocked = register_allocation::blocked_bytes(name, graph, out.assignments);
    if (blocked_addrs != nullptr) {
      blocked.insert(blocked_addrs->begin(), blocked_addrs->end());
    }
    std::optional<asdl::Int> base;
    if (allowed_range) {
      base = register_allocation::find_fit(*allowed_range, node.width, blocked);
    } else if (node.lives_across_call) {
      base = register_allocation::find_fit(pool.callee_saved(), node.width, blocked);
    } else {
      base = register_allocation::find_fit(pool.caller_saved(), node.width, blocked);
      if (!base) {
        base = register_allocation::find_fit(pool.callee_saved(), node.width, blocked);
      }
    }
    if (!base) {
      out.spilled.insert(name);
    } else {
      out.assignments.insert_or_assign(name, *base);
    }
  }
}

// Returns the names of `fn`'s pseudos that are in `reps` or that
// `rep_map` maps into `reps` through any number of steps.
NameSet names_resolving_to(const ir::Function& fn, const NameSet& reps,
                           const OrderedMap<std::string, std::string>* rep_map) {
  NameSet out;
  for (const auto& instr : fn.instructions) {
    for (const auto* op : optimizer::all_operands_in(instr)) {
      const auto* p = asdl::as<ir::Pseudo>(*op);
      if (p == nullptr) {
        continue;
      }
      std::string cur = p->name;
      while (rep_map != nullptr) {
        const std::string* next = rep_map->find(cur);
        if (next == nullptr) {
          break;
        }
        cur = *next;
      }
      if (reps.contains(cur)) {
        out.insert(p->name);
      }
    }
  }
  return out;
}

}  // namespace

Coloring color_graph(const ir::Function& fn, const InterferenceGraph& graph,
                     const std::optional<Pool>& pool_arg, const std::set<asdl::Int>* blocked_addrs,
                     const HwRegPinning* pinning, const std::optional<Range>& allowed_range,
                     const OrderedMap<std::string, std::string>* rep_map,
                     const NameSet* frame_names) {
  const Pool pool = pool_arg.value_or(Pool{});
  const auto peo = perfect_elimination_order(fn, graph);
  // A spilled node gets a frame slot too, which isn't known until the
  // bytes are colored, after the pinning. When a coloring spills while it
  // keeps a value in Y, the pinning and the coloring run again with the
  // names of the spilled nodes, and of every name coalesced into one,
  // added to the frame names, until a run spills nothing new.
  NameSet in_frame;
  if (frame_names != nullptr) {
    in_frame = *frame_names;
  }
  while (true) {
    Coloring out{.pool = pool};
    if (pinning != nullptr) {
      try_hwreg_assign(graph, pinning->eligibility, out.hwreg_assignments, pinning->liveness,
                       rep_map, &in_frame);
    }
    color_bytes(graph, peo, pool, blocked_addrs, allowed_range, out);
    bool y_used = false;
    for (const auto& [name, letter] : out.hwreg_assignments) {
      y_used = y_used || letter == "Y";
    }
    if (out.spilled.empty() || !y_used) {
      return out;
    }
    bool grew = false;
    for (const auto& name : names_resolving_to(fn, out.spilled, rep_map)) {
      grew = in_frame.insert(name).second || grew;
    }
    if (!grew) {
      return out;
    }
  }
}

}  // namespace c6502::passes
