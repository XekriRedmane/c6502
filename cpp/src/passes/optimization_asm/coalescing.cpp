// Move coalescing for the asm-level interference graph.
//
// This is the usual transformation of register allocation in SSA form.
// When two SSA-renamed pseudo names are connected by a `Mov`, or by a
// `Phi` (each `PhiArg.source` is move-related to the `Phi.dst`), and
// they don't interfere in the graph, they can be given the same color
// without making the coloring wrong. Doing so removes a copy, since the
// `Mov` becomes a move of a byte to itself when the coloring is applied,
// which the emitter's peephole drops.
//
// The case that motivated it is a loop counter:
//
//     .preheader:
//         Mov(Imm(0), %i.v0)            # init
//     .loop_top:
//         Phi(%i.phi, [(.preheader, %i.v0), (.continue, %i.v_post)])
//         ... use %i.phi ...
//     .continue:
//         Mov(%i.phi, A); CLC; Add(Imm(1), A); Mov(A, %i.v_post)
//         Jump(.loop_top)
//
// SSA destruction turns the Phi into a move on each edge:
//
//     .preheader:
//         Mov(Imm(0), %i.v0)
//         Mov(%i.v0, %i.phi)            # edge .preheader -> .loop_top
//         ...
//     .continue:
//         Mov(...) ... Mov(A, %i.v_post)
//         Mov(%i.v_post, %i.phi)        # edge .continue -> .loop_top
//
// When %i.v0, %i.phi, and %i.v_post all coalesce into one zero-page
// byte, every move inserted is a move of `ZP($X)` to itself, which
// `Emitter::mov` drops. The increment becomes a read-modify-write of $X
// in place, which the INC peephole of several bytes then turns into
// `INC $X; BNE done; INC $X+1; done:` where it applies.
//
// # Algorithm
//
// This is aggressive (Chaitin-style) coalescing without the
// conservative check of degrees. A merged node whose degree is more than
// the pool holds falls back to `Coloring.spilled`.
//
//   1. Walk the instructions and collect the move-related pairs: each
//      `Mov(Pseudo a, Pseudo b)`, an explicit copy, and each pair
//      (`Phi.dst`, `PhiArg.source`) of two pseudos, for which Phi
//      destruction would write a `Mov`.
//   2. Track the classes of equal names with union-find.
//   3. For each pair (a, b), in order, look up the representatives
//      a_rep and b_rep, and skip the pair when they are already one,
//      when either isn't a node of the graph (a static, an
//      address-taken name, or a parameter), when they have different
//      widths (the coloring's search for a slot assumes one width), or
//      when they interfere (coalescing them would force them to share a
//      color, which can't be right). Otherwise merge b_rep into a_rep:
//      the merged node takes b_rep's neighbors (other than a_rep), its
//      `lives_across_call` is the OR of the two, and b_rep leaves the
//      graph.
//   4. Return the map from each name that isn't its own representative
//      to its representative, through which the caller projects the
//      coloring's assignments.
//
// # Carry bytes
//
// After the move pairs, a second round looks at each byte of an add
// that propagates the carry, `Mov(P_a, A); Add(Imm(0), A); Mov(A,
// P_b)`. That's the high byte of an add whose other operand's high byte
// is zero, such as `t + (uint16_t)c` or `t + 53`, or the same with
// `Sub(Imm(0), A)` for a subtract's borrow. When P_a and P_b share a
// byte, the peephole in carry_inc.h rewrites the three instructions as
// `BCC skip; INC P; skip:` (or `BCS skip; DEC P; skip:`), the way
// hand-written 6502 code adds a byte to a 16-bit value.
// These pairs come after the move pairs, so they never take a slot away
// from a copy the move pairs would remove.
//
// The same round also takes each copy of a byte that the move pairs
// skip because one side is a byte of a name of several bytes, such as a
// pointer used as a `(zp),Y` base that is advanced through 1-byte SSA
// temporaries at the bottom of a loop.
//
// The check at the level of bytes (`byte_edges`) applies Chaitin's rule
// for a move: a copy's write of its destination doesn't make the
// destination interfere with the copy's source, since both hold one
// value right after it. A later write of either while the other is live
// still adds the edge.
//
// P_a is often byte k of a name of several bytes. A constant shift is
// lowered to ASL / ROL on its destination in place, and asm SSA keeps
// any name that is the target of such a read-modify-write as one
// contiguous block. P_b, a 1-byte SSA name, then merges into that
// block's node at byte k, and `CoalesceResult::byte_offset` records k so
// the projection of the coloring gives P_b the block's address plus k.
// That's sound for the same reason as an ordinary merge, since there is
// no interference edge between P_b and any byte of the block, and P_b's
// neighbors become neighbors of the whole block.
//
// # Soundness
//
// Merging two nodes that don't interfere never adds interference. Each
// neighbor of A already conflicted with A's color, or had none, and
// after the merge it conflicts with the merged node, which has the same
// color, and the same holds for B's neighbors. The merged node's degree
// is the size of the union of the two sets of neighbors, which can be
// higher than either but no higher than their sum.
#include "passes/optimization_asm/coalescing.h"

#include <optional>
#include <set>
#include <span>
#include <utility>
#include <vector>

#include "passes/decimal_arith.h"
#include "passes/operand_rewrite.h"
#include "passes/optimization_asm/liveness.h"

namespace c6502::passes {

namespace ir = ::c6502::asm_ast;

std::string CoalesceResult::resolve(std::string_view name) const {
  std::string cur(name);
  while (const std::string* next = representative.find(cur)) {
    cur = *next;
  }
  return cur;
}

asdl::Int CoalesceResult::offset_of(std::string_view name) const {
  asdl::Int total = 0;
  std::string cur(name);
  while (const std::string* next = representative.find(cur)) {
    auto it = byte_offset.find(cur);
    if (it != byte_offset.end()) {
      total += it->second;
    }
    cur = *next;
  }
  return total;
}

namespace {

using Edge = std::pair<std::string, std::string>;
using EdgeSet = std::set<Edge>;
using Offsets = std::map<std::string, asdl::Int, std::less<>>;

Edge edge(const std::string& a, const std::string& b) {
  return a < b ? Edge{a, b} : Edge{b, a};
}

bool is_reg_a(const ir::Type_operand& op) {
  const auto* r = asdl::as<ir::Reg>(op);
  return r != nullptr && asdl::isa<ir::A>(r->reg);
}

// Merges `src` into `dst` in place. `src`'s neighbors become `dst`'s,
// `src` leaves `graph.nodes` and `graph.adj`, and the merged node's
// `lives_across_call` is the OR of the two.
void merge(InterferenceGraph& graph, const std::string& dst, const std::string& src) {
  NameSet src_neighbors;
  if (auto it = graph.adj.find(src); it != graph.adj.end()) {
    src_neighbors = it->second;
  }
  for (const auto& n : src_neighbors) {
    if (auto it = graph.adj.find(n); it != graph.adj.end()) {
      it->second.erase(src);
    }
    // (dst, n) and (n, dst) are added, unless that would make a loop
    // (n == dst). The check for an edge before a merge rules that out
    // for the pair being merged, but a chain of merges could make one.
    if (n != dst) {
      graph.adj[dst].insert(n);
      graph.adj[n].insert(dst);
    }
  }
  graph.adj.erase(src);
  if (graph.nodes.at(src).lives_across_call) {
    graph.nodes.at(dst).lives_across_call = true;
  }
  graph.nodes.erase(src);
}

// The union-find of the names, with the byte offset of each name inside
// the one it points to.
struct UnionFind {
  OrderedMap<std::string, std::string>& rep_map;
  Offsets& offsets;

  // Returns the representative of `name`, compressing the path to it.
  // Each compressed link keeps the byte offset summed along the path it
  // replaces.
  std::string find(const std::string& start) {
    std::vector<std::string> path;
    std::string name = start;
    while (const std::string* next = rep_map.find(name)) {
      path.push_back(name);
      name = *next;
    }
    asdl::Int total = 0;
    for (auto p = path.rbegin(); p != path.rend(); ++p) {
      if (auto it = offsets.find(*p); it != offsets.end()) {
        total += it->second;
      }
      rep_map.insert_or_assign(*p, name);
      if (total != 0) {
        offsets[*p] = total;
      } else {
        offsets.erase(*p);
      }
    }
    return name;
  }

  asdl::Int offset(const std::string& name) {
    find(name);
    auto it = offsets.find(name);
    return it == offsets.end() ? asdl::Int(0) : it->second;
  }
};

// Returns the name whose value `instrs[k]` copies into its pseudo
// destination, or null. That's the source of `Mov(P_a, P_b)`, or of
// `Mov(P_a, A)` right before `Mov(A, P_b)`. Both names hold one value
// right after the copy, so the copy's write of P_b doesn't make them
// interfere (Chaitin's rule for a move), while a later write of either
// while the other is live still does.
const std::string* copied_from(std::span<const ir::Type_instruction> instrs, std::size_t k) {
  const auto* ins = asdl::as<ir::Mov>(instrs[k]);
  if (ins == nullptr || ins->is_volatile || !asdl::isa<ir::Pseudo>(ins->dst)) {
    return nullptr;
  }
  const ir::Type_operand* src = &ins->src;
  if (is_reg_a(*src)) {
    const ir::Mov* prev = k != 0 ? asdl::as<ir::Mov>(instrs[k - 1]) : nullptr;
    if (prev == nullptr || prev->is_volatile || !is_reg_a(prev->dst)) {
      return nullptr;
    }
    src = &prev->src;
  }
  const auto* p = asdl::as<ir::Pseudo>(*src);
  if (p != nullptr && p->name != std::get<ir::Pseudo>(ins->dst).name) {
    return &p->name;
  }
  return nullptr;
}

std::string byte_name(const std::string& name, const asdl::Int& j) {
  return name + "#" + std::to_string(static_cast<long long>(j));
}

// Returns the interference edges with every byte of each name in
// `multi` renamed to `<name>#<j>`, over every pseudo of `fn`. A write
// the rename can't see, such as the high byte a `LoadAddress` writes
// without naming it, only makes that byte look live for longer, which
// adds edges and never removes one.
EdgeSet byte_edges(const ir::Function& fn, const NameSet& multi) {
  ir::Function renamed{.name = fn.name, .is_global = fn.is_global, .params = fn.params,
                       .instructions = fn.instructions};
  for (auto& instr : renamed.instructions) {
    for_each_operand_mut(instr, [&](ir::Type_operand& op) {
      if (auto* p = asdl::as<ir::Pseudo>(op); p != nullptr && multi.contains(p->name)) {
        op = ir::Pseudo{.name = byte_name(p->name, p->offset), .offset = 0};
      }
    });
  }
  const Liveness liveness = compute_liveness(renamed);
  EdgeSet edges;
  for (std::size_t b = 0; b < liveness.cfg.blocks.size(); ++b) {
    const auto instrs = liveness.cfg.instructions(static_cast<int>(b));
    NameSet live = liveness.live_out[b];
    for (std::size_t k = instrs.size(); k-- > 0;) {
      const auto& instr = instrs[k];
      if (asdl::isa<ir::Phi>(instr)) {
        continue;
      }
      const auto defs = liveness::defs_in(instr);
      NameSet after = live;
      for (const auto* d : defs) {
        after.insert(d->name);
      }
      const std::string* source = copied_from(instrs, k);
      for (const auto* d : defs) {
        for (const auto& n : after) {
          if (n != d->name && (source == nullptr || n != *source)) {
            edges.insert(edge(d->name, n));
          }
        }
      }
      for (const auto* d : defs) {
        live.erase(d->name);
      }
      if (const auto* dec = asdl::as<ir::DecimalArith>(instr)) {
        // A BCD chain writes dst[k] before it reads the source bytes of
        // higher index.
        for (const auto& [d, s] : decimal_arith::early_write_pairs(*dec)) {
          edges.insert(edge(d, s));
        }
      }
      for (const auto* u : liveness::uses_in(instr)) {
        live.insert(u->name);
      }
    }
    std::vector<std::string> phi_dsts;
    for (const auto& instr : instrs) {
      if (const auto* phi = asdl::as<ir::Phi>(instr)) {
        if (const auto* d = asdl::as<ir::Pseudo>(phi->dst)) {
          phi_dsts.push_back(d->name);
        }
      }
    }
    for (std::size_t k = 0; k < phi_dsts.size(); ++k) {
      for (std::size_t j = k + 1; j < phi_dsts.size(); ++j) {
        edges.insert(edge(phi_dsts[k], phi_dsts[j]));
      }
      for (const auto& n : live) {
        if (n != phi_dsts[k]) {
          edges.insert(edge(phi_dsts[k], n));
        }
      }
    }
  }
  return edges;
}

// Merges the pairs of byte copies and carry bytes (see "Carry bytes"
// above), tracking which byte of its representative each coalesced
// name occupies.
//
// The interference graph is per name, and a name of several bytes is
// live while any of its bytes is. So a 1-byte name defined while the
// other byte of a pointer is still to be read has an edge to the whole
// pointer, even when the byte it would share is already dead. When such
// an edge blocks a merge into a node of several bytes, `byte_edges`
// decides it byte by byte instead: every byte of a name of several
// bytes is renamed to a name of its own, the liveness is computed again,
// and the merge goes ahead when no two names that would share a byte are
// live at once.
class ByteMerger {
 public:
  ByteMerger(const ir::Function& fn, InterferenceGraph& graph, UnionFind& uf,
             std::map<std::string, asdl::Int, std::less<>> widths)
      : fn_(fn), graph_(graph), uf_(uf), widths_(std::move(widths)) {}

  // Puts the bytes `src` and `dst` in one byte of storage when that's
  // sound, by merging one node into the other at the offset that lines
  // the two bytes up. The merged node has to fit inside the one it
  // joins.
  void coalesce(const ir::Pseudo& src, const ir::Pseudo& dst) {
    const std::string s_rep = uf_.find(src.name);
    const std::string d_rep = uf_.find(dst.name);
    if (s_rep == d_rep) {
      return;
    }
    if (!graph_.nodes.contains(s_rep) || !graph_.nodes.contains(d_rep)) {
      return;
    }
    const asdl::Int s_width = graph_.nodes.at(s_rep).width;
    const asdl::Int d_width = graph_.nodes.at(d_rep).width;
    // The byte position of each operand inside its representative.
    const asdl::Int s_pos = uf_.offset(src.name) + src.offset;
    const asdl::Int d_pos = uf_.offset(dst.name) + dst.offset;
    // Merging `small` into `big` puts small's byte j at big's byte
    // j + pos.
    std::string big;
    std::string small;
    asdl::Int pos = 0;
    if (0 <= s_pos - d_pos && s_pos - d_pos + d_width <= s_width) {
      big = s_rep;
      small = d_rep;
      pos = s_pos - d_pos;
    } else if (0 <= d_pos - s_pos && d_pos - s_pos + s_width <= d_width) {
      big = d_rep;
      small = s_rep;
      pos = d_pos - s_pos;
    } else {
      return;
    }
    if (graph_.has_edge(s_rep, d_rep)) {
      // An edge between names is imprecise only through a name of
      // several bytes, which is live while any of its bytes is.
      if (graph_.nodes.at(big).width == 1) {
        return;
      }
      if (conflicts(big, small, pos)) {
        return;
      }
    }
    merge(graph_, big, small);
    uf_.rep_map.insert_or_assign(small, big);
    if (pos != 0) {
      uf_.offsets[small] = pos;
    }
  }

 private:
  // Returns the names of the graph before any merge that are coalesced
  // into `rep`, each with its byte position in rep's storage.
  std::vector<std::pair<std::string, asdl::Int>> members(const std::string& rep) {
    std::vector<std::pair<std::string, asdl::Int>> out;
    for (const auto& [n, w] : widths_) {
      if (uf_.find(n) == rep) {
        out.emplace_back(n, uf_.offset(n));
      }
    }
    return out;
  }

  // Maps each byte position of `rep`'s storage, plus `shift`, to the
  // byte-level names that occupy it.
  std::map<asdl::Int, std::vector<std::string>> bytes(const std::string& rep,
                                                      const asdl::Int& shift) {
    std::map<asdl::Int, std::vector<std::string>> out;
    for (const auto& [n, p] : members(rep)) {
      const asdl::Int w = widths_.at(n);
      for (asdl::Int j = 0; j < w; ++j) {
        out[p + j + shift].push_back(w == 1 ? n : byte_name(n, j));
      }
    }
    return out;
  }

  // Returns whether a name of `small`'s class and a name of `big`'s
  // class that would share a byte are live at the same time.
  bool conflicts(const std::string& big, const std::string& small, const asdl::Int& pos) {
    if (!edges_) {
      NameSet multi;
      for (const auto& [n, w] : widths_) {
        if (w > 1) {
          multi.insert(n);
        }
      }
      edges_ = byte_edges(fn_, multi);
    }
    const auto big_bytes = bytes(big, 0);
    for (const auto& [k, names] : bytes(small, pos)) {
      auto it = big_bytes.find(k);
      if (it == big_bytes.end()) {
        continue;
      }
      for (const auto& a : names) {
        for (const auto& b : it->second) {
          if (edges_->contains(edge(a, b))) {
            return true;
          }
        }
      }
    }
    return false;
  }

  const ir::Function& fn_;
  InterferenceGraph& graph_;
  UnionFind& uf_;
  std::optional<EdgeSet> edges_;
  // The widths of every name in the graph before the move pairs merged
  // any, among which a class's members are found.
  std::map<std::string, asdl::Int, std::less<>> widths_;
};

// Returns the pairs (name_a, name_b) of names that a move relates:
//
//   * `Mov(Pseudo, Pseudo)`, an explicit copy.
//   * Each pair (`Phi.dst`, `PhiArg.source`) of two pseudos, for which
//     SSA destruction would write a `Mov`.
//   * Adjacent `Mov(Pseudo a, Reg(A)); Mov(Reg(A), Pseudo b)`, a copy
//     from a to b through A. `tac_to_asm` routes every transfer between
//     pseudos through A, since the 6502 has no move from memory to
//     memory, so this is the usual shape once copy propagation and DCE
//     settle. Coalescing the two ends makes the pair a load and store of
//     one byte, which becomes a move of the byte to itself that the
//     emitter drops.
//
// Pairs with a pseudo of nonzero offset are skipped. Such a pseudo
// refers to a name SSA left whole (typically the bytes of a parameter
// that asm SSA leaves alone), whose bytes the coloring doesn't rewrite
// one by one.
//
// A volatile `Mov` isn't a candidate. Coalescing would put the volatile
// pseudo and a temporary that isn't volatile under one name, and the
// one color would let the temporary's uses share the volatile byte's
// storage.
std::vector<std::pair<std::string, std::string>> move_related_pairs(const ir::Function& fn) {
  std::vector<std::pair<std::string, std::string>> out;
  const auto& instrs = fn.instructions;
  for (const auto& instr : instrs) {
    if (const auto* mov = asdl::as<ir::Mov>(instr)) {
      if (mov->is_volatile) {
        continue;
      }
      const auto* src = asdl::as<ir::Pseudo>(mov->src);
      const auto* dst = asdl::as<ir::Pseudo>(mov->dst);
      if (src != nullptr && dst != nullptr && src->offset == 0 && dst->offset == 0 &&
          src->name != dst->name) {
        out.emplace_back(src->name, dst->name);
      }
    } else if (const auto* phi = asdl::as<ir::Phi>(instr)) {
      const auto* dst = asdl::as<ir::Pseudo>(phi->dst);
      if (dst == nullptr || dst->offset != 0) {
        continue;
      }
      for (const auto& a : phi->args) {
        const auto* src = asdl::as<ir::Pseudo>(std::get<ir::AsmPhiArg>(a).source);
        if (src != nullptr && src->offset == 0 && src->name != dst->name) {
          out.emplace_back(dst->name, src->name);
        }
      }
    }
  }
  // The copies through A: adjacent `Mov(P_a, A); Mov(A, P_b)`.
  for (std::size_t i = 0; i + 1 < instrs.size(); ++i) {
    const auto* a = asdl::as<ir::Mov>(instrs[i]);
    const auto* b = asdl::as<ir::Mov>(instrs[i + 1]);
    if (a == nullptr || b == nullptr || a->is_volatile || b->is_volatile) {
      continue;
    }
    const auto* a_src = asdl::as<ir::Pseudo>(a->src);
    if (a_src == nullptr || a_src->offset != 0 || !is_reg_a(a->dst)) {
      continue;
    }
    const auto* b_dst = asdl::as<ir::Pseudo>(b->dst);
    if (!is_reg_a(b->src) || b_dst == nullptr || b_dst->offset != 0) {
      continue;
    }
    if (a_src->name == b_dst->name) {
      continue;
    }
    out.emplace_back(a_src->name, b_dst->name);
  }
  return out;
}

using PseudoPair = std::pair<const ir::Pseudo*, const ir::Pseudo*>;

// Returns the (src, dst) pseudo operands of each copy of a byte, a
// `Mov(Pseudo, Pseudo)` or a copy through A, `Mov(Pseudo, A); Mov(A,
// Pseudo)`. The move pairs already took the copies between two 1-byte
// names. What's left is typically a pointer of two bytes that asm SSA
// leaves whole (it's the base of a `(zp),Y` access) being copied a byte
// at a time to or from 1-byte SSA names, as at the bottom of a loop
// that advances the pointer.
std::vector<PseudoPair> byte_copy_pairs(const ir::Function& fn) {
  std::vector<PseudoPair> out;
  const auto& instrs = fn.instructions;
  const auto wanted = [](const ir::Type_operand& a, const ir::Type_operand& b) -> PseudoPair {
    const auto* pa = asdl::as<ir::Pseudo>(a);
    const auto* pb = asdl::as<ir::Pseudo>(b);
    if (pa != nullptr && pb != nullptr && pa->name != pb->name) {
      return {pa, pb};
    }
    return {nullptr, nullptr};
  };
  for (const auto& instr : instrs) {
    const auto* mov = asdl::as<ir::Mov>(instr);
    if (mov != nullptr && !mov->is_volatile) {
      if (auto p = wanted(mov->src, mov->dst); p.first != nullptr) {
        out.push_back(p);
      }
    }
  }
  for (std::size_t i = 0; i + 1 < instrs.size(); ++i) {
    const auto* a = asdl::as<ir::Mov>(instrs[i]);
    const auto* b = asdl::as<ir::Mov>(instrs[i + 1]);
    if (a == nullptr || b == nullptr || a->is_volatile || b->is_volatile) {
      continue;
    }
    if (!is_reg_a(a->dst) || !is_reg_a(b->src)) {
      continue;
    }
    if (auto p = wanted(a->src, b->dst); p.first != nullptr) {
      out.push_back(p);
    }
  }
  return out;
}

// Returns the (src, dst) pseudo operands of each byte that propagates a
// carry, `Mov(src, A); Add(Imm(0), A); Mov(A, dst)`, the high byte of
// an add whose other operand's high byte is zero, and of the same shape
// with `Sub` (a subtract's borrow). Unlike the move pairs, `src` may be
// a byte of a name of several bytes (a shift temporary that asm SSA
// leaves whole because ASL / ROL rewrite it in place).
std::vector<PseudoPair> carry_byte_pairs(const ir::Function& fn) {
  std::vector<PseudoPair> out;
  const auto& instrs = fn.instructions;
  for (std::size_t i = 0; i + 2 < instrs.size(); ++i) {
    const auto* a = asdl::as<ir::Mov>(instrs[i]);
    const auto* b = asdl::as<ir::Mov>(instrs[i + 2]);
    const ir::Type_operand* op_src = nullptr;
    const ir::Type_operand* op_dst = nullptr;
    if (const auto* add = asdl::as<ir::Add>(instrs[i + 1])) {
      op_src = &add->src;
      op_dst = &add->dst;
    } else if (const auto* sub = asdl::as<ir::Sub>(instrs[i + 1])) {
      op_src = &sub->src;
      op_dst = &sub->dst;
    }
    if (a == nullptr || op_src == nullptr || b == nullptr) {
      continue;
    }
    if (a->is_volatile || b->is_volatile) {
      continue;
    }
    const auto* imm = asdl::as<ir::Imm>(*op_src);
    if (imm == nullptr || imm->value != 0 || !is_reg_a(*op_dst)) {
      continue;
    }
    const auto* a_src = asdl::as<ir::Pseudo>(a->src);
    if (a_src == nullptr || !is_reg_a(a->dst)) {
      continue;
    }
    const auto* b_dst = asdl::as<ir::Pseudo>(b->dst);
    if (!is_reg_a(b->src) || b_dst == nullptr) {
      continue;
    }
    if (a_src->name == b_dst->name) {
      continue;
    }
    out.emplace_back(a_src, b_dst);
  }
  return out;
}

}  // namespace

CoalesceResult coalesce_moves(const ir::Function& fn, InterferenceGraph& graph) {
  const auto pairs = move_related_pairs(fn);
  // Every graph name's width, taken before any merge removes names from
  // the graph. The check at the level of bytes has to see every member
  // of a coalesced class, including those the move pairs merged.
  std::map<std::string, asdl::Int, std::less<>> widths;
  for (const auto& [n, node] : graph.nodes) {
    widths.emplace(n, node.width);
  }
  CoalesceResult result;
  UnionFind uf{result.representative, result.byte_offset};

  for (const auto& [a, b] : pairs) {
    const std::string a_rep = uf.find(a);
    const std::string b_rep = uf.find(b);
    if (a_rep == b_rep) {
      continue;
    }
    // One or both are left out of the graph (statics, address-taken
    // names, parameters), so coalescing them can't be represented.
    if (!graph.nodes.contains(a_rep) || !graph.nodes.contains(b_rep)) {
      continue;
    }
    if (graph.nodes.at(a_rep).width != graph.nodes.at(b_rep).width) {
      continue;
    }
    if (graph.has_edge(a_rep, b_rep)) {
      continue;
    }
    merge(graph, a_rep, b_rep);
    result.representative.insert_or_assign(b_rep, a_rep);
  }
  ByteMerger merger(fn, graph, uf, std::move(widths));
  for (const auto& [src, dst] : byte_copy_pairs(fn)) {
    merger.coalesce(*src, *dst);
  }
  for (const auto& [src, dst] : carry_byte_pairs(fn)) {
    merger.coalesce(*src, *dst);
  }
  return result;
}

}  // namespace c6502::passes
