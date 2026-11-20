// Backward-dataflow liveness analysis for an `asm_ast::Function`.
//
// It works on pseudo names. The variables tracked are the byte-versioned
// SSA names that `to_ssa` produces, where every byte of a value of
// several bytes has become its own 1-byte variable with `offset == 0`.
// The names that shouldn't be colored (statics, address-taken names,
// parameters, and the targets of read-modify-writes) are filtered by the
// caller, the interference builder, so the liveness here covers every
// pseudo in the IR.
//
// It computes each block's `live_in` and `live_out`, and answers
// `live_after` and `live_before` for each instruction, which the
// interference builder and the allocator ask.
//
// A `Phi` is handled as in the TAC version. Its `dst` is killed at the
// block's entry, in parallel with the other Phis of the block and
// before its first instruction that isn't one. Each `args[i].source` is
// added to the `live_out` of the predecessor its label names, since the
// copy that SSA destruction writes for it reads it on the edge from
// that predecessor and not in the merge block.
//
// A `Liveness` is a snapshot. Changing the function after computing it
// invalidates the sets kept for each instruction.
#include "passes/optimization_asm/liveness.h"

#include <map>
#include <string>
#include <utility>

namespace c6502::passes {

namespace ir = ::c6502::asm_ast;

namespace liveness {

namespace {

void add_pseudo(std::vector<const ir::Pseudo*>& out, const ir::Type_operand& op) {
  if (const auto* p = asdl::as<ir::Pseudo>(op)) {
    out.push_back(p);
  }
}

}  // namespace

std::vector<const ir::Pseudo*> defs_in(const ir::Type_instruction& instr) {
  using K = ir::Kind_instruction;
  std::vector<const ir::Pseudo*> out;
  switch (ir::kind(instr)) {
    case K::DecimalArith:
      // decimal_arith.pseudo_writes, the pseudo bytes of `dst`.
      for (const auto& op : std::get<ir::DecimalArith>(instr).dst) {
        add_pseudo(out, op);
      }
      break;
    case K::Mov:
      add_pseudo(out, std::get<ir::Mov>(instr).dst);
      break;
    case K::Pop:
      add_pseudo(out, std::get<ir::Pop>(instr).dst);
      break;
    case K::LoadAddress:
      add_pseudo(out, std::get<ir::LoadAddress>(instr).dst);
      break;
    case K::Phi:
      add_pseudo(out, std::get<ir::Phi>(instr).dst);
      break;
    case K::Add:
      add_pseudo(out, std::get<ir::Add>(instr).dst);
      break;
    case K::Sub:
      add_pseudo(out, std::get<ir::Sub>(instr).dst);
      break;
    case K::And:
      add_pseudo(out, std::get<ir::And>(instr).dst);
      break;
    case K::Or:
      add_pseudo(out, std::get<ir::Or>(instr).dst);
      break;
    case K::Xor:
      add_pseudo(out, std::get<ir::Xor>(instr).dst);
      break;
    case K::Inc:
      add_pseudo(out, std::get<ir::Inc>(instr).dst);
      break;
    case K::Dec:
      add_pseudo(out, std::get<ir::Dec>(instr).dst);
      break;
    case K::ArithmeticShiftLeft:
      add_pseudo(out, std::get<ir::ArithmeticShiftLeft>(instr).dst);
      break;
    case K::LogicalShiftRight:
      add_pseudo(out, std::get<ir::LogicalShiftRight>(instr).dst);
      break;
    case K::RotateLeft:
      add_pseudo(out, std::get<ir::RotateLeft>(instr).dst);
      break;
    case K::RotateRight:
      add_pseudo(out, std::get<ir::RotateRight>(instr).dst);
      break;
    default:
      break;
  }
  return out;
}

std::vector<const ir::Pseudo*> uses_in(const ir::Type_instruction& instr) {
  using K = ir::Kind_instruction;
  std::vector<const ir::Pseudo*> out;
  switch (ir::kind(instr)) {
    case K::DecimalArith: {
      // decimal_arith.pseudo_reads, the pseudo bytes of `left` and then
      // `right`.
      const auto& d = std::get<ir::DecimalArith>(instr);
      for (const auto& op : d.left) {
        add_pseudo(out, op);
      }
      for (const auto& op : d.right) {
        add_pseudo(out, op);
      }
      break;
    }
    case K::Mov:
      add_pseudo(out, std::get<ir::Mov>(instr).src);
      break;
    case K::Push:
      add_pseudo(out, std::get<ir::Push>(instr).src);
      break;
    case K::Compare: {
      const auto& c = std::get<ir::Compare>(instr);
      add_pseudo(out, c.left);
      add_pseudo(out, c.right);
      break;
    }
    // A pseudo `dst`, uncommon in what tac_to_asm writes, is both read
    // and written by an ADC-style read-modify-write.
    case K::Add: {
      const auto& a = std::get<ir::Add>(instr);
      add_pseudo(out, a.src);
      add_pseudo(out, a.dst);
      break;
    }
    case K::Sub: {
      const auto& a = std::get<ir::Sub>(instr);
      add_pseudo(out, a.src);
      add_pseudo(out, a.dst);
      break;
    }
    case K::And: {
      const auto& a = std::get<ir::And>(instr);
      add_pseudo(out, a.src);
      add_pseudo(out, a.dst);
      break;
    }
    case K::Or: {
      const auto& a = std::get<ir::Or>(instr);
      add_pseudo(out, a.src);
      add_pseudo(out, a.dst);
      break;
    }
    case K::Xor: {
      const auto& x = std::get<ir::Xor>(instr);
      add_pseudo(out, x.src1);
      add_pseudo(out, x.src2);
      break;
    }
    case K::Inc:
      add_pseudo(out, std::get<ir::Inc>(instr).dst);
      break;
    case K::Dec:
      add_pseudo(out, std::get<ir::Dec>(instr).dst);
      break;
    case K::ArithmeticShiftLeft:
      add_pseudo(out, std::get<ir::ArithmeticShiftLeft>(instr).dst);
      break;
    case K::LogicalShiftRight:
      add_pseudo(out, std::get<ir::LogicalShiftRight>(instr).dst);
      break;
    case K::RotateLeft:
      add_pseudo(out, std::get<ir::RotateLeft>(instr).dst);
      break;
    case K::RotateRight:
      add_pseudo(out, std::get<ir::RotateRight>(instr).dst);
      break;
    case K::LoadAddress:
      add_pseudo(out, std::get<ir::LoadAddress>(instr).src);
      break;
    // The sources of a Phi are read on the edges from its
    // predecessors, which `compute` adds to their `live_out`.
    default:
      break;
  }
  return out;
}

}  // namespace liveness

namespace {

const ir::Phi* as_phi_with_pseudo_dst(const ir::Type_instruction& instr) {
  const auto* phi = asdl::as<ir::Phi>(instr);
  if (phi == nullptr || !asdl::isa<ir::Pseudo>(phi->dst)) {
    return nullptr;
  }
  return phi;
}

// Solves the dataflow equations to their fixed point:
//
//   live_out[B] = the union over the successors S of live_in[S], plus
//                 the sources of the Phis at S that are attributed to B,
//                 which is to say Phi(dst, args=[..., (B_label, src),
//                 ...]) adds `src` to live_out[B];
//   live_in[B]  = uses[B] | (live_out[B] - defs[B]).
//
// `defs[B]` includes the Phi destinations, which are killed at the
// block's entry, so live_in[S] already leaves them out. `uses[B]` is
// the usual set of the block's upward-exposed uses.
void solve(const Cfg& cfg, std::vector<NameSet>& live_in, std::vector<NameSet>& live_out) {
  const std::size_t n = cfg.blocks.size();
  // The block each label starts, as Python's `_block_labels` inverted,
  // where a later block wins a label two blocks start with.
  std::map<std::string, int, std::less<>> label_to_bid;
  for (std::size_t bid = 0; bid < n; ++bid) {
    const auto instrs = cfg.instructions(static_cast<int>(bid));
    if (!instrs.empty()) {
      if (const auto* label = asdl::as<ir::Label>(instrs.front())) {
        label_to_bid[label->name] = static_cast<int>(bid);
      }
    }
  }

  std::vector<NameSet> gen(n);
  std::vector<NameSet> kill(n);
  for (std::size_t bid = 0; bid < n; ++bid) {
    const auto instrs = cfg.instructions(static_cast<int>(bid));
    // The Phi destinations are killed at the block's entry.
    for (const auto& instr : instrs) {
      if (const auto* phi = as_phi_with_pseudo_dst(instr)) {
        kill[bid].insert(std::get<ir::Pseudo>(phi->dst).name);
      }
    }
    // The other instructions, in source order, give gen and kill as
    // usual.
    for (const auto& instr : instrs) {
      if (asdl::isa<ir::Phi>(instr)) {
        continue;
      }
      for (const auto* u : liveness::uses_in(instr)) {
        if (!kill[bid].contains(u->name)) {
          gen[bid].insert(u->name);
        }
      }
      for (const auto* d : liveness::defs_in(instr)) {
        kill[bid].insert(d->name);
      }
    }
  }

  // Each argument (pred_label, source) of a Phi at block S adds
  // `source.name` to the live_out of the predecessor whose leading
  // label is `pred_label`.
  std::vector<NameSet> phi_edge_contrib(n);
  for (std::size_t bid = 0; bid < n; ++bid) {
    for (const auto& instr : cfg.instructions(static_cast<int>(bid))) {
      const auto* phi = asdl::as<ir::Phi>(instr);
      if (phi == nullptr) {
        continue;
      }
      for (const auto& a : phi->args) {
        const auto& arg = std::get<ir::AsmPhiArg>(a);
        const auto* src = asdl::as<ir::Pseudo>(arg.source);
        if (src == nullptr) {
          continue;
        }
        auto it = label_to_bid.find(arg.pred_label);
        if (it == label_to_bid.end()) {
          continue;
        }
        phi_edge_contrib[static_cast<std::size_t>(it->second)].insert(src->name);
      }
    }
  }

  live_in.assign(n, NameSet{});
  live_out.assign(n, NameSet{});
  bool changed = true;
  while (changed) {
    changed = false;
    for (std::size_t bid = 0; bid < n; ++bid) {
      NameSet new_out = phi_edge_contrib[bid];
      for (int s : cfg.blocks[bid].successors) {
        const auto& in_s = live_in[static_cast<std::size_t>(s)];
        new_out.insert(in_s.begin(), in_s.end());
      }
      NameSet new_in = gen[bid];
      for (const auto& name : new_out) {
        if (!kill[bid].contains(name)) {
          new_in.insert(name);
        }
      }
      if (new_out != live_out[bid] || new_in != live_in[bid]) {
        live_out[bid] = std::move(new_out);
        live_in[bid] = std::move(new_in);
        changed = true;
      }
    }
  }
}

}  // namespace

const NameSet& Liveness::live_after(int bid, std::size_t i) const {
  const auto b = static_cast<std::size_t>(bid);
  if (per_instr_after.size() < cfg.blocks.size()) {
    per_instr_after.resize(cfg.blocks.size());
  }
  auto& cache = per_instr_after[b];
  if (!cache) {
    // The set after instruction k is the input of the backward update
    // of instruction k + 1, so a backward walk from `live_out[bid]`
    // gives each one before the instruction's own update.
    const auto instrs = cfg.instructions(bid);
    std::vector<NameSet> out_sets(instrs.size());
    NameSet live = live_out[b];
    for (std::size_t k = instrs.size(); k-- > 0;) {
      out_sets[k] = live;
      const auto& instr = instrs[k];
      // A Phi's dst is killed at the block's entry and its sources are
      // read on the predecessors' edges, so it adds nothing here.
      if (asdl::isa<ir::Phi>(instr)) {
        continue;
      }
      for (const auto* d : liveness::defs_in(instr)) {
        auto it = live.find(d->name);
        if (it != live.end()) {
          live.erase(it);
        }
      }
      for (const auto* u : liveness::uses_in(instr)) {
        live.insert(u->name);
      }
    }
    cache = std::move(out_sets);
  }
  return (*cache)[i];
}

NameSet Liveness::live_before(int bid, std::size_t i) const {
  if (i > 0) {
    return live_after(bid, i - 1);
  }
  NameSet out = live_in[static_cast<std::size_t>(bid)];
  for (const auto& instr : cfg.instructions(bid)) {
    if (const auto* phi = as_phi_with_pseudo_dst(instr)) {
      out.insert(std::get<ir::Pseudo>(phi->dst).name);
    }
  }
  return out;
}

Liveness compute_liveness(const ir::Function& fn) {
  Liveness out{.cfg = build_cfg(fn)};
  solve(out.cfg, out.live_in, out.live_out);
  return out;
}

Liveness liveness_with_sets(const ir::Function& fn, std::vector<NameSet> live_in,
                            std::vector<NameSet> live_out) {
  return Liveness{
      .cfg = build_cfg(fn), .live_in = std::move(live_in), .live_out = std::move(live_out)};
}

}  // namespace c6502::passes
