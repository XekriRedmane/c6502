// Builds the interference graph for the asm-level register allocation.
//
// Two pseudo names interfere when both are live at one program point.
// Each colorable name is 1 byte wide, which is the point of the
// byte-granular SSA form: every byte of every value of several bytes
// has been split into a variable of its own upstream.
//
// These names are left out of the graph:
//
//   * `statics`, the file-scope globals, block-scope statics, and
//     externs, whose addresses are fixed at link time.
//   * The names a `LoadAddress` takes the address of, whose bytes have
//     to stay together.
//   * The function's parameters, whose bytes the calling convention
//     delivers through Frame addressing on entry.
//
// A name referenced at a nonzero offset is a name that SSA left whole,
// such as the destination of an in-place shift of several bytes, and
// it becomes a node as wide as its highest offset plus one.
//
// `lives_across_call` is set on a node when its value is live right
// before a `Call`, with the call's own definitions removed. The coloring
// reads it to prefer callee-saved bytes for such a value.
#include "passes/optimization_asm/interference.h"

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "passes/decimal_arith.h"
#include "passes/optimization_asm/all_operands.h"

namespace c6502::passes {

namespace ir = ::c6502::asm_ast;

namespace {

// Sorts every pseudo name of `fn` into one of three kinds.
//
//   * Excluded, in the set returned first: names the coloring can't
//     take, which are `statics` and the sources of `LoadAddress`.
//   * Candidates for a block of several bytes, returned as
//     `{name: width}`, which need that many contiguous zero-page
//     bytes. A `LoadAddress` destination is always 2 bytes, since an
//     address is 16 bits on the 6502 and the instruction writes both
//     bytes. Any other name referenced at several byte offsets that
//     isn't excluded, typically the destination of `tac_to_asm`'s
//     shift by one in place, is `max_offset + 1` bytes wide.
//   * Single bytes, every other name, including the target of a
//     read-modify-write referenced at one offset, an ordinary 1-byte
//     value whose definition is also a use.
std::pair<NameSet, std::map<std::string, asdl::Int, std::less<>>> categorize_names(
    const ir::Function& fn, const NameSet& statics) {
  NameSet excluded = statics;
  std::map<std::string, asdl::Int, std::less<>> multi_byte_widths;
  std::map<std::string, asdl::Int, std::less<>> max_offset;
  for (const auto& instr : fn.instructions) {
    if (const auto* la = asdl::as<ir::LoadAddress>(instr)) {
      if (const auto* src = asdl::as<ir::Pseudo>(la->src)) {
        excluded.insert(src->name);
      }
      // The destination isn't excluded. It is colored with width 2, so
      // the allocator finds 2 contiguous bytes for it.
      if (const auto* dst = asdl::as<ir::Pseudo>(la->dst)) {
        multi_byte_widths.emplace(dst->name, 2);
      }
    }
    for (const auto* op : optimizer::all_operands_in(instr)) {
      if (const auto* p = asdl::as<ir::Pseudo>(*op)) {
        auto [it, inserted] = max_offset.emplace(p->name, p->offset);
        if (!inserted && p->offset > it->second) {
          it->second = p->offset;
        }
      }
    }
  }
  // A name referenced at several offsets that isn't already a
  // candidate, such as the output of the inline shift by one, where
  // each byte of a temporary is shifted in place through a pseudo
  // operand, becomes one of width `max_offset + 1`.
  for (const auto& [name, top] : max_offset) {
    if (top > 0 && !multi_byte_widths.contains(name)) {
      multi_byte_widths.emplace(name, top + 1);
    }
  }
  // A name both excluded and a candidate stays excluded.
  for (auto it = multi_byte_widths.begin(); it != multi_byte_widths.end();) {
    if (excluded.contains(it->first)) {
      it = multi_byte_widths.erase(it);
    } else {
      ++it;
    }
  }
  return {std::move(excluded), std::move(multi_byte_widths)};
}

}  // namespace

InterferenceGraph build_interference(const ir::Function& fn, const Liveness& liveness,
                                     const NameSet& statics) {
  // Named references, since Clang 14 doesn't let a lambda capture a
  // structured binding.
  auto categories = categorize_names(fn, statics);
  NameSet& excluded = categories.first;
  const auto& multi_byte_widths = categories.second;
  excluded.insert(fn.params.begin(), fn.params.end());

  InterferenceGraph graph;
  const auto colorable = [&](const std::string& name) { return !excluded.contains(name); };
  const auto ensure_node = [&](const std::string& name) {
    if (!graph.nodes.contains(name)) {
      auto it = multi_byte_widths.find(name);
      graph.nodes.emplace(name, InterferenceNode{.name = name,
                                                 .width = it == multi_byte_widths.end()
                                                              ? asdl::Int(1)
                                                              : it->second,
                                                 .lives_across_call = false});
    }
  };

  for (std::size_t b = 0; b < liveness.cfg.blocks.size(); ++b) {
    const int bid = static_cast<int>(b);
    const auto instrs = liveness.cfg.instructions(bid);
    NameSet live;
    for (const auto& n : liveness.live_out[b]) {
      if (colorable(n)) {
        live.insert(n);
      }
    }

    // A backward walk over the instructions that aren't Phis.
    for (std::size_t i = instrs.size(); i-- > 0;) {
      const auto& instr = instrs[i];
      if (asdl::isa<ir::Phi>(instr)) {
        continue;
      }
      // An instruction that writes several bytes (a BCD chain) writes
      // them all, so each interferes with the others and with
      // everything live after it, even when it's dead.
      std::vector<std::string> defs;
      for (const auto* d : liveness::defs_in(instr)) {
        if (colorable(d->name)) {
          defs.push_back(d->name);
        }
      }
      NameSet after = live;
      after.insert(defs.begin(), defs.end());
      for (const auto& d : defs) {
        ensure_node(d);
        for (const auto& n : after) {
          if (n != d) {
            graph.add_edge(d, n);
          }
        }
      }
      for (const auto& d : defs) {
        live.erase(d);
      }
      if (const auto* dec = asdl::as<ir::DecimalArith>(instr)) {
        // A BCD chain writes dst[k] before it reads the source bytes
        // of higher index.
        for (const auto& [a, c] : decimal_arith::early_write_pairs(*dec)) {
          if (colorable(a) && colorable(c)) {
            ensure_node(a);
            ensure_node(c);
            graph.add_edge(a, c);
          }
        }
      }
      // A name live across the call may have no node yet, when its
      // only uses are in a block the walk hasn't reached, so it gets
      // one here, or its value could take a caller-saved byte that the
      // callee overwrites.
      if (asdl::isa<ir::Call>(instr)) {
        for (const auto& n : live) {
          ensure_node(n);
          graph.nodes.at(n).lives_across_call = true;
        }
      }
      for (const auto* u : liveness::uses_in(instr)) {
        if (!colorable(u->name)) {
          continue;
        }
        ensure_node(u->name);
        live.insert(u->name);
      }
    }

    // The Phi destinations are defined at the block's entry, in
    // parallel, so they interfere with each other and with everything
    // live right before the first instruction that isn't a Phi, which
    // is `live` after the walk.
    std::vector<std::string> phi_dsts;
    for (const auto& instr : instrs) {
      const auto* phi = asdl::as<ir::Phi>(instr);
      if (phi == nullptr) {
        continue;
      }
      const auto* dst = asdl::as<ir::Pseudo>(phi->dst);
      if (dst != nullptr && colorable(dst->name)) {
        phi_dsts.push_back(dst->name);
      }
    }
    for (std::size_t k = 0; k < phi_dsts.size(); ++k) {
      const auto& dst = phi_dsts[k];
      ensure_node(dst);
      for (std::size_t j = k + 1; j < phi_dsts.size(); ++j) {
        graph.add_edge(dst, phi_dsts[j]);
      }
      for (const auto& n : live) {
        if (n != dst) {
          graph.add_edge(dst, n);
        }
      }
    }
  }
  return graph;
}

}  // namespace c6502::passes
