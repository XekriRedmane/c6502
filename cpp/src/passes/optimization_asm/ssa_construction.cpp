// Asm-level SSA construction.
//
// Converts an asm function to SSA form. Each (Pseudo name, byte offset)
// pair is versioned on its own, which is the point of the asm-level SSA
// layer: byte 0 of a 4-byte `long` can have a chain of definitions and
// uses apart from byte 3's, on which the byte-granular optimizations
// after it depend (such as dropping the high bytes of a value that fits
// in a byte).
//
// A (name, offset) pair is promotable when `name` is none of these.
//   * Names whose address is taken, which appear as `LoadAddress.src`.
//     The bytes of such a value have to stay together at one frame
//     slot, so they can't be split across SSA versions.
//   * The targets of a read-modify-write, which appear as the `dst` of
//     `Inc`, `Dec`, `ASL`, `LSR`, `ROL`, or `ROR`. `tac_to_asm` writes
//     these for a constant shift, which it inlines as `ASL` / `ROL` (or
//     `LSR` / `ROR`) on the destination in place. The instruction's one
//     operand is both a use and a definition, which doesn't split into
//     separate SSA versions.
//
// The renaming. A definition of byte k of `name` makes a new Pseudo,
// `Pseudo("<name>.b<k>.v<N>", 0)`. The byte's position is in the name,
// so the Pseudo's offset stays 0 and each byte is a 1-byte variable of
// its own. SSA destruction keeps the new names, and
// `replace_pseudoregisters` gives each one byte, since `size_of_name`
// returns 1 for a name the symbol table doesn't have.
//
// The algorithm is Cytron et al.'s (1991), as in the TAC `to_ssa`.
//   1. Build the CFG, and give every block that doesn't start with a
//      `Label` one, for `AsmPhiArg.pred_label` to name.
//   2. Compute the immediate dominators, the dominance frontiers, and
//      the children of each block in the dominator tree.
//   3. Find the promotable pairs.
//   4. Find the blocks that define each promotable pair v, and place an
//      empty `Phi(Pseudo("<name>", <offset>), [])` at each block of the
//      iterated dominance frontier of those blocks where v is live on
//      entry. A `Phi` where v isn't live gives only dead definitions.
//   5. Rename. Walk the dominator tree in preorder. In each block,
//      rename each `Phi`'s destination, then rewrite the uses and the
//      definitions of each other instruction in order, then add an
//      argument to each `Phi` of each successor from the newest name of
//      its pair.
//
// A parameter's first SSA name is its own Pseudo with its own offset. A
// promotable parameter is rare, since a parameter usually lives at a
// frame slot, but the renaming puts the original name on each
// promotable parameter byte's stack at ENTRY, so that its first read
// finds it.
//
// The `Phi`s of a block are in the order of the first definition of
// each pair.
#include "passes/optimization_asm/ssa_construction.h"

#include <cstddef>
#include <map>
#include <optional>
#include <string_view>
#include <vector>

#include "errors.h"
#include "passes/decimal_arith.h"
#include "passes/optimization_asm/owned_cfg.h"
#include "passes/optimization_asm/ssa_operands.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

ByteVar key_of(const ir::Pseudo& p) { return {p.name, static_cast<std::int64_t>(p.offset)}; }

// Prepends a label when the function's body starts with one, as the
// TAC `maybe_prepend_preheader` does. A block that starts with a
// label that is a jump target can have predecessors besides ENTRY (a
// loop's back edge), so the head has to be a labeled block of its own
// for the arguments that come from the entry to name.
ir::Function maybe_prepend_preheader(const ir::Function& fn) {
  if (fn.instructions.empty() || !isa<ir::Label>(fn.instructions[0])) {
    return fn;
  }
  std::set<std::string, std::less<>> existing;
  for (const auto& i : fn.instructions) {
    if (const auto* l = as<ir::Label>(i)) {
      existing.insert(l->name);
    }
  }
  std::string name;
  for (int counter = 0;; ++counter) {
    name = "." + fn.name + "@asm_ssa_preheader@" + std::to_string(counter);
    if (!existing.contains(name)) {
      break;
    }
  }
  ir::Function out{.name = fn.name, .is_global = fn.is_global, .params = fn.params};
  out.instructions.reserve(fn.instructions.size() + 1);
  out.instructions.push_back(ir::Label{.name = name});
  out.instructions.insert(out.instructions.end(), fn.instructions.begin(), fn.instructions.end());
  return out;
}

// Puts a new `Label` at the start of every block that doesn't start
// with one. The names, `.<funcname>@asm_ssa_block@<N>`, stay apart from
// the user's labels and from TAC SSA's.
void ensure_block_labels(OwnedCfg& cfg, const std::string& fn_name) {
  std::set<std::string, std::less<>> existing;
  for (const auto& code : cfg.code) {
    for (const auto& i : code) {
      if (const auto* l = as<ir::Label>(i)) {
        existing.insert(l->name);
      }
    }
  }
  int counter = 0;
  for (const int id : cfg.graph.block_order) {
    auto& code = cfg.code[id];
    if (!code.empty() && isa<ir::Label>(code[0])) {
      continue;
    }
    std::string name;
    while (true) {
      name = "." + fn_name + "@asm_ssa_block@" + std::to_string(counter);
      ++counter;
      if (!existing.contains(name)) {
        existing.insert(name);
        break;
      }
    }
    code.insert(code.begin(), ir::Label{.name = name});
  }
}

const std::string& block_label(const OwnedCfg& cfg, int id) {
  const auto& code = cfg.code[id];
  if (code.empty() || !isa<ir::Label>(code[0])) {
    throw InternalError("asm SSA: block " + std::to_string(id) + " has no leading label");
  }
  return std::get<ir::Label>(code[0]).name;
}

// Calls `f` on each Pseudo `instr` defines. `Mov`, `Pop`,
// `LoadAddress`, and `Phi` define a Pseudo. The ALU instructions write
// A in what `tac_to_asm` emits, but the IR allows a Pseudo there, which
// counts as a use and a definition. So do the read-modify-writes, whose
// names are never promotable, so that the renaming skips them.
template <class F>
void for_each_def(const ir::Type_instruction& instr, F&& f) {
  const auto pseudo = [&](const ir::Type_operand& op) {
    if (const auto* p = as<ir::Pseudo>(op)) {
      f(*p);
    }
  };
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::DecimalArith:
      for (const auto& p : decimal_arith::pseudo_writes(std::get<ir::DecimalArith>(instr))) {
        f(p);
      }
      break;
    case ir::Kind_instruction::Mov:
      pseudo(std::get<ir::Mov>(instr).dst);
      break;
    case ir::Kind_instruction::Pop:
      pseudo(std::get<ir::Pop>(instr).dst);
      break;
    case ir::Kind_instruction::LoadAddress:
      pseudo(std::get<ir::LoadAddress>(instr).dst);
      break;
    case ir::Kind_instruction::Phi:
      pseudo(std::get<ir::Phi>(instr).dst);
      break;
    case ir::Kind_instruction::Add:
      pseudo(std::get<ir::Add>(instr).dst);
      break;
    case ir::Kind_instruction::Sub:
      pseudo(std::get<ir::Sub>(instr).dst);
      break;
    case ir::Kind_instruction::And:
      pseudo(std::get<ir::And>(instr).dst);
      break;
    case ir::Kind_instruction::Or:
      pseudo(std::get<ir::Or>(instr).dst);
      break;
    case ir::Kind_instruction::Xor:
      pseudo(std::get<ir::Xor>(instr).dst);
      break;
    case ir::Kind_instruction::Inc:
      pseudo(std::get<ir::Inc>(instr).dst);
      break;
    case ir::Kind_instruction::Dec:
      pseudo(std::get<ir::Dec>(instr).dst);
      break;
    case ir::Kind_instruction::ArithmeticShiftLeft:
      pseudo(std::get<ir::ArithmeticShiftLeft>(instr).dst);
      break;
    case ir::Kind_instruction::LogicalShiftRight:
      pseudo(std::get<ir::LogicalShiftRight>(instr).dst);
      break;
    case ir::Kind_instruction::RotateLeft:
      pseudo(std::get<ir::RotateLeft>(instr).dst);
      break;
    case ir::Kind_instruction::RotateRight:
      pseudo(std::get<ir::RotateRight>(instr).dst);
      break;
    default:
      break;
  }
}

// Calls `f` on each Pseudo `instr` uses.
template <class F>
void for_each_use(const ir::Type_instruction& instr, F&& f) {
  const auto pseudo = [&](const ir::Type_operand& op) {
    if (const auto* p = as<ir::Pseudo>(op)) {
      f(*p);
    }
  };
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::DecimalArith:
      for (const auto& p : decimal_arith::pseudo_reads(std::get<ir::DecimalArith>(instr))) {
        f(p);
      }
      break;
    case ir::Kind_instruction::Mov:
      pseudo(std::get<ir::Mov>(instr).src);
      break;
    case ir::Kind_instruction::Push:
      pseudo(std::get<ir::Push>(instr).src);
      break;
    case ir::Kind_instruction::Compare: {
      const auto& i = std::get<ir::Compare>(instr);
      pseudo(i.left);
      pseudo(i.right);
      break;
    }
    // A Pseudo destination of an ALU instruction would be a use too, but
    // no emitted instruction has one.
    case ir::Kind_instruction::Add:
      pseudo(std::get<ir::Add>(instr).src);
      break;
    case ir::Kind_instruction::Sub:
      pseudo(std::get<ir::Sub>(instr).src);
      break;
    case ir::Kind_instruction::And:
      pseudo(std::get<ir::And>(instr).src);
      break;
    case ir::Kind_instruction::Or:
      pseudo(std::get<ir::Or>(instr).src);
      break;
    case ir::Kind_instruction::Xor: {
      const auto& i = std::get<ir::Xor>(instr);
      pseudo(i.src1);
      pseudo(i.src2);
      break;
    }
    case ir::Kind_instruction::Phi:
      for (const auto& a : std::get<ir::Phi>(instr).args) {
        pseudo(std::get<ir::AsmPhiArg>(a).source);
      }
      break;
    default:
      break;
  }
}

bool is_reg_a(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

bool is_dptr(const ir::Type_operand& op, int offset) {
  const auto* d = as<ir::Data>(op);
  return d != nullptr && d->name == "DPTR" && d->offset == offset;
}

// Returns the name of the Pseudo P when instructions `i` to `i + 3`
// stage it through DPTR in the shape that `stage_dptr`
// (tac_to_asm_calls.cpp) writes:
//
//     Mov(Pseudo(P, 0), Reg(A))
//     Mov(Reg(A), Data("DPTR", 0))
//     Mov(Pseudo(P, 1), Reg(A))
//     Mov(Reg(A), Data("DPTR", 1))
const std::string* match_dptr_stage_4(const std::vector<ir::Type_instruction>& instrs,
                                      std::size_t i) {
  const auto* a = as<ir::Mov>(instrs[i]);
  const auto* b = as<ir::Mov>(instrs[i + 1]);
  const auto* c = as<ir::Mov>(instrs[i + 2]);
  const auto* d = as<ir::Mov>(instrs[i + 3]);
  if (a == nullptr || b == nullptr || c == nullptr || d == nullptr) {
    return nullptr;
  }
  const auto* p0 = as<ir::Pseudo>(a->src);
  if (p0 == nullptr || p0->offset != 0 || !is_reg_a(a->dst)) {
    return nullptr;
  }
  if (!is_reg_a(b->src) || !is_dptr(b->dst, 0)) {
    return nullptr;
  }
  const auto* p1 = as<ir::Pseudo>(c->src);
  if (p1 == nullptr || p1->name != p0->name || p1->offset != 1 || !is_reg_a(c->dst)) {
    return nullptr;
  }
  if (!is_reg_a(d->src) || !is_dptr(d->dst, 1)) {
    return nullptr;
  }
  return &p0->name;
}

// The promotable pairs, each with an index into the renaming's tables.
using VarIndex = std::map<ByteVar, int>;

// A set of promotable pairs as a bit per pair.
class VarSet {
 public:
  explicit VarSet(std::size_t n = 0) : words_((n + 63) / 64, 0) {}
  void add(int v) { words_[v / 64] |= std::uint64_t{1} << (v % 64); }
  bool contains(int v) const { return (words_[v / 64] >> (v % 64)) & 1; }
  bool operator==(const VarSet& o) const { return words_ == o.words_; }
  // Sets this to `gen | (out - kill)`.
  void assign_gen_out_kill(const VarSet& gen, const VarSet& out, const VarSet& kill) {
    for (std::size_t w = 0; w < words_.size(); ++w) {
      words_[w] = gen.words_[w] | (out.words_[w] & ~kill.words_[w]);
    }
  }
  void unite(const VarSet& o) {
    for (std::size_t w = 0; w < words_.size(); ++w) {
      words_[w] |= o.words_[w];
    }
  }

 private:
  std::vector<std::uint64_t> words_;
};

std::optional<int> index_of(const VarIndex& index, const ir::Pseudo& p) {
  const auto it = index.find(key_of(p));
  if (it == index.end()) {
    return std::nullopt;
  }
  return it->second;
}

// Returns the promotable pairs that are live on entry to each block, a
// backward dataflow over the CFG. A `Phi` is placed for a pair only
// where it is live, which gives pruned SSA.
std::vector<VarSet> compute_live_in(const OwnedCfg& cfg, const VarIndex& index) {
  const std::size_t nb = cfg.graph.blocks.size();
  const std::size_t nv = index.size();
  std::vector<VarSet> gen(nb, VarSet(nv));
  std::vector<VarSet> kill(nb, VarSet(nv));
  for (std::size_t b = 0; b < nb; ++b) {
    for (const auto& instr : cfg.code[b]) {
      for_each_use(instr, [&](const ir::Pseudo& u) {
        if (const auto v = index_of(index, u); v && !kill[b].contains(*v)) {
          gen[b].add(*v);
        }
      });
      for_each_def(instr, [&](const ir::Pseudo& d) {
        if (const auto v = index_of(index, d)) {
          kill[b].add(*v);
        }
      });
    }
  }
  std::vector<VarSet> live_in(nb, VarSet(nv));
  std::vector<VarSet> live_out(nb, VarSet(nv));
  bool changed = true;
  while (changed) {
    changed = false;
    for (std::size_t b = 0; b < nb; ++b) {
      VarSet new_out(nv);
      for (const int s : cfg.graph.blocks[b].successors) {
        new_out.unite(live_in[s]);
      }
      VarSet new_in(nv);
      new_in.assign_gen_out_kill(gen[b], new_out, kill[b]);
      if (!(new_out == live_out[b]) || !(new_in == live_in[b])) {
        live_out[b] = std::move(new_out);
        live_in[b] = std::move(new_in);
        changed = true;
      }
    }
  }
  return live_in;
}

// The `Phi`s placed in each block, by block id: the index of each one's
// pair and its position in the block, in the order they were placed.
using PhisAt = std::vector<std::vector<std::pair<int, std::size_t>>>;

// Places a `Phi` at the iterated dominance frontier of the blocks that
// define each promotable pair, where the pair is live on entry and the
// block has two predecessors or more. The pairs are taken in the order
// of their first definition, which is the order of the `Phi`s in a
// block.
PhisAt place_phis(OwnedCfg& cfg, const VarIndex& index, const std::vector<ByteVar>& vars,
                  const std::vector<std::set<int>>& df, const std::vector<VarSet>& live_in) {
  const std::size_t nb = cfg.graph.blocks.size();
  // The pairs in the order of their first definition, with the blocks
  // that define each.
  std::vector<int> order;
  std::vector<std::set<int>> def_blocks(vars.size());
  for (std::size_t b = 0; b < nb; ++b) {
    for (const auto& instr : cfg.code[b]) {
      for_each_def(instr, [&](const ir::Pseudo& d) {
        if (const auto v = index_of(index, d)) {
          if (def_blocks[*v].empty()) {
            order.push_back(*v);
          }
          def_blocks[*v].insert(static_cast<int>(b));
        }
      });
    }
  }
  PhisAt phis_at(nb);
  for (const int v : order) {
    std::vector<int> worklist(def_blocks[v].begin(), def_blocks[v].end());
    std::set<int> placed;
    std::set<int> visited(def_blocks[v].begin(), def_blocks[v].end());
    while (!worklist.empty()) {
      const int x = worklist.back();
      worklist.pop_back();
      for (const int y : df[x]) {
        if (placed.contains(y)) {
          continue;
        }
        if (cfg.graph.blocks[y].predecessors.size() < 2) {
          continue;
        }
        if (!live_in[y].contains(v)) {
          continue;
        }
        // The destination is a placeholder, which the renaming replaces
        // with a new version when it reaches the block.
        auto& code = cfg.code[y];
        std::size_t pos = 0;
        if (!code.empty() && isa<ir::Label>(code[0])) {
          pos = 1;
          while (pos < code.size() && isa<ir::Phi>(code[pos])) {
            ++pos;
          }
        }
        code.insert(code.begin() + static_cast<std::ptrdiff_t>(pos),
                    ir::Phi{.dst = ir::Pseudo{.name = vars[v].first, .offset = vars[v].second}});
        phis_at[y].emplace_back(v, pos);
        placed.insert(y);
        if (!visited.contains(y)) {
          visited.insert(y);
          worklist.push_back(y);
        }
      }
    }
  }
  return phis_at;
}

// Renames each promotable pair's definitions to new versions and its
// uses to the newest version, walking the dominator tree.
class Renamer {
 public:
  Renamer(OwnedCfg& cfg, const VarIndex& index, const std::vector<ByteVar>& vars,
          const std::vector<std::vector<int>>& children,
          const std::vector<std::optional<std::string>>& block_label_of,
          const PhisAt& phis_at)
      : cfg_(cfg),
        index_(index),
        vars_(vars),
        children_(children),
        block_label_of_(block_label_of),
        phis_at_(phis_at),
        counters_(vars.size(), 0),
        stacks_(vars.size()) {}

  void push_params(const std::vector<std::string>& params) {
    for (const auto& p : params) {
      for (std::size_t v = 0; v < vars_.size(); ++v) {
        if (vars_[v].first == p) {
          stacks_[v].push_back(ir::Pseudo{.name = p, .offset = vars_[v].second});
        }
      }
    }
  }

  void visit(int id) {
    std::vector<int> pushed;
    auto& code = cfg_.code[id];
    // 1. Rename the destinations of the Phis.
    for (auto& instr : code) {
      auto* phi = as<ir::Phi>(instr);
      if (phi == nullptr) {
        continue;
      }
      const auto* dst = as<ir::Pseudo>(phi->dst);
      if (dst == nullptr) {
        continue;
      }
      if (const auto v = index_of(index_, *dst)) {
        ir::Pseudo fresh = make_fresh(*v);
        stacks_[*v].push_back(fresh);
        pushed.push_back(*v);
        phi->dst = std::move(fresh);
      }
    }
    // 2. Rename the uses and then the definitions of the other
    // instructions.
    for (auto& instr : code) {
      if (!isa<ir::Phi>(instr)) {
        rewrite_instruction(instr, pushed);
      }
    }
    // 3. Add the arguments of the Phis of each successor.
    static const std::string kNoLabel;
    const std::string& pred_label = block_label_of_[id] ? *block_label_of_[id] : kNoLabel;
    for (const int succ : cfg_.graph.blocks[id].successors) {
      for (const auto& [v, pos] : phis_at_[succ]) {
        // With no definition reaching here, the argument is the
        // original Pseudo.
        ir::Pseudo src = stacks_[v].empty()
                             ? ir::Pseudo{.name = vars_[v].first, .offset = vars_[v].second}
                             : stacks_[v].back();
        std::get<ir::Phi>(cfg_.code[succ][pos])
            .args.push_back(ir::AsmPhiArg{.pred_label = pred_label, .source = std::move(src)});
      }
    }
    // 4. Visit the children in the dominator tree.
    for (const int child : children_[id]) {
      visit(child);
    }
    // 5. Pop what this block pushed.
    for (const int v : pushed) {
      stacks_[v].pop_back();
    }
  }

 private:
  ir::Pseudo make_fresh(int v) {
    ++counters_[v];
    return ir::Pseudo{.name = vars_[v].first + ".b" + std::to_string(vars_[v].second) + ".v" +
                              std::to_string(counters_[v]),
                      .offset = 0};
  }

  void use(ir::Type_operand& op) {
    if (const auto* p = as<ir::Pseudo>(op)) {
      if (const auto v = index_of(index_, *p); v && !stacks_[*v].empty()) {
        op = stacks_[*v].back();
      }
    }
  }

  void def(ir::Type_operand& op, std::vector<int>& pushed) {
    if (const auto* p = as<ir::Pseudo>(op)) {
      if (const auto v = index_of(index_, *p)) {
        ir::Pseudo fresh = make_fresh(*v);
        stacks_[*v].push_back(fresh);
        pushed.push_back(*v);
        op = std::move(fresh);
      }
    }
  }

  // Rewrites each promotable use in `instr` to the newest version and
  // then gives each promotable definition a new version. Uses go first,
  // so an instruction that writes what it reads reads the old version.
  void rewrite_instruction(ir::Type_instruction& instr, std::vector<int>& pushed) {
    switch (ir::kind(instr)) {
      case ir::Kind_instruction::DecimalArith: {
        // Every source byte is renamed before any destination byte, so
        // a destination byte named like a source byte is the same byte
        // k on both sides.
        auto& i = std::get<ir::DecimalArith>(instr);
        for (auto& op : i.left) {
          use(op);
        }
        for (auto& op : i.right) {
          use(op);
        }
        for (auto& op : i.dst) {
          def(op, pushed);
        }
        break;
      }
      case ir::Kind_instruction::Mov: {
        auto& i = std::get<ir::Mov>(instr);
        use(i.src);
        def(i.dst, pushed);
        break;
      }
      case ir::Kind_instruction::Add: {
        auto& i = std::get<ir::Add>(instr);
        use(i.src);
        def(i.dst, pushed);
        break;
      }
      case ir::Kind_instruction::Sub: {
        auto& i = std::get<ir::Sub>(instr);
        use(i.src);
        def(i.dst, pushed);
        break;
      }
      case ir::Kind_instruction::And: {
        auto& i = std::get<ir::And>(instr);
        use(i.src);
        def(i.dst, pushed);
        break;
      }
      case ir::Kind_instruction::Or: {
        auto& i = std::get<ir::Or>(instr);
        use(i.src);
        def(i.dst, pushed);
        break;
      }
      case ir::Kind_instruction::Xor: {
        auto& i = std::get<ir::Xor>(instr);
        use(i.src1);
        use(i.src2);
        def(i.dst, pushed);
        break;
      }
      case ir::Kind_instruction::Inc:
        use(std::get<ir::Inc>(instr).dst);
        break;
      case ir::Kind_instruction::Dec:
        use(std::get<ir::Dec>(instr).dst);
        break;
      case ir::Kind_instruction::ArithmeticShiftLeft:
        use(std::get<ir::ArithmeticShiftLeft>(instr).dst);
        break;
      case ir::Kind_instruction::LogicalShiftRight:
        use(std::get<ir::LogicalShiftRight>(instr).dst);
        break;
      case ir::Kind_instruction::RotateLeft:
        use(std::get<ir::RotateLeft>(instr).dst);
        break;
      case ir::Kind_instruction::RotateRight:
        use(std::get<ir::RotateRight>(instr).dst);
        break;
      case ir::Kind_instruction::Push:
        use(std::get<ir::Push>(instr).src);
        break;
      case ir::Kind_instruction::Pop:
        def(std::get<ir::Pop>(instr).dst, pushed);
        break;
      case ir::Kind_instruction::Compare: {
        auto& i = std::get<ir::Compare>(instr);
        use(i.left);
        use(i.right);
        break;
      }
      case ir::Kind_instruction::LoadAddress: {
        // `src` has its address taken, so it isn't promotable and stays.
        auto& i = std::get<ir::LoadAddress>(instr);
        use(i.src);
        def(i.dst, pushed);
        break;
      }
      default:
        // The other instructions have no Pseudo operands.
        break;
    }
  }

  OwnedCfg& cfg_;
  const VarIndex& index_;
  const std::vector<ByteVar>& vars_;
  const std::vector<std::vector<int>>& children_;
  const std::vector<std::optional<std::string>>& block_label_of_;
  const PhisAt& phis_at_;
  std::vector<int> counters_;
  std::vector<std::vector<ir::Pseudo>> stacks_;
};

}  // namespace

// The names include these.
//   * `LoadAddress.src`, whose address is taken, so its bytes have to
//     stay together at one place.
//   * `LoadAddress.dst`, the pointer that holds the address. The
//     instruction writes two bytes, low and high, but its operand names
//     only byte 0, and the write of byte 1 is implicit in the emitter's
//     `shift_offset(dst, 1)`. Versioning byte 0 alone would leave byte
//     1 written unseen, and the register allocation could put another
//     SSA value on it. Excluding the name keeps it in two bytes of the
//     frame together.
//   * The targets of a read-modify-write (`Inc`, `Dec`, `ASL`, `LSR`,
//     `ROL`, `ROR`), whose one operand doesn't split into separate SSA
//     versions. `tac_to_asm` writes them on the destination of an
//     inline constant shift.
//   * The pointers staged through DPTR. When both bytes of a Pseudo P
//     are staged through DPTR in the four instructions `Mov(P[0], A);
//     STA DPTR; Mov(P[1], A); STA DPTR+1`, versioning would split P
//     into single bytes that the register allocation places apart,
//     often at bytes that aren't adjacent or are in the wrong order,
//     and the rewrite to `(P_low_addr),Y` couldn't bypass DPTR. Kept as
//     a multi-byte Pseudo, P gets two adjacent bytes with byte 0 lower.
//   * The names any volatile `Mov` reads or writes. Versions of a
//     volatile Pseudo colored to separate bytes would send some
//     accesses to one cell and some to another, where every access has
//     to reach the one cell.
//
// The passes after SSA construction have to agree on these names.
ExcludedNames excluded_pseudo_names(const ir::Function& fn) {
  ExcludedNames excluded;
  const auto add = [&](const ir::Type_operand& op) {
    if (const auto* p = as<ir::Pseudo>(op)) {
      excluded.insert(p->name);
    }
  };
  for (const auto& instr : fn.instructions) {
    switch (ir::kind(instr)) {
      case ir::Kind_instruction::LoadAddress: {
        const auto& i = std::get<ir::LoadAddress>(instr);
        add(i.src);
        add(i.dst);
        break;
      }
      case ir::Kind_instruction::Inc:
        add(std::get<ir::Inc>(instr).dst);
        break;
      case ir::Kind_instruction::Dec:
        add(std::get<ir::Dec>(instr).dst);
        break;
      case ir::Kind_instruction::ArithmeticShiftLeft:
        add(std::get<ir::ArithmeticShiftLeft>(instr).dst);
        break;
      case ir::Kind_instruction::LogicalShiftRight:
        add(std::get<ir::LogicalShiftRight>(instr).dst);
        break;
      case ir::Kind_instruction::RotateLeft:
        add(std::get<ir::RotateLeft>(instr).dst);
        break;
      case ir::Kind_instruction::RotateRight:
        add(std::get<ir::RotateRight>(instr).dst);
        break;
      case ir::Kind_instruction::Mov: {
        const auto& i = std::get<ir::Mov>(instr);
        if (i.is_volatile) {
          add(i.src);
          add(i.dst);
        }
        break;
      }
      default:
        break;
    }
  }
  const auto& instrs = fn.instructions;
  for (std::size_t i = 0; i + 3 < instrs.size(); ++i) {
    if (const std::string* name = match_dptr_stage_4(instrs, i)) {
      excluded.insert(*name);
    }
  }
  return excluded;
}

ir::Function to_ssa(const ir::Function& input, const StaticNames& statics) {
  const ir::Function fn = maybe_prepend_preheader(input);
  OwnedCfg cfg = build_owned_cfg(fn);
  ensure_block_labels(cfg, fn.name);

  const ExcludedNames excluded = excluded_pseudo_names(fn);
  std::set<ByteVar> promotable;
  for (const auto& code : cfg.code) {
    for (const auto& instr : code) {
      ssa_operands::for_each_operand(instr, [&](const ir::Type_operand& op) {
        if (const auto* p = as<ir::Pseudo>(op)) {
          if (!is_excluded(p->name, excluded, statics)) {
            promotable.insert(key_of(*p));
          }
        }
      });
    }
  }
  if (promotable.empty()) {
    return owned_cfg_to_function(fn, cfg);
  }
  VarIndex index;
  std::vector<ByteVar> vars(promotable.begin(), promotable.end());
  for (std::size_t v = 0; v < vars.size(); ++v) {
    index.emplace(vars[v], static_cast<int>(v));
  }

  const Dominators doms = immediate_dominators(cfg.graph);
  const std::vector<std::set<int>> df = dominance_frontiers(cfg.graph);
  const std::vector<std::vector<int>> children = dominator_tree_children(doms);
  const std::vector<VarSet> live_in = compute_live_in(cfg, index);

  std::vector<std::optional<std::string>> block_label_of(cfg.graph.blocks.size());
  for (const int id : doms.order) {
    if (id != kEntryId && id != kExitId && !cfg.code[id].empty()) {
      block_label_of[id] = block_label(cfg, id);
    }
  }

  const PhisAt phis_at = place_phis(cfg, index, vars, df, live_in);
  Renamer renamer(cfg, index, vars, children, block_label_of, phis_at);
  renamer.push_params(fn.params);
  renamer.visit(kEntryId);
  return owned_cfg_to_function(fn, cfg);
}

}  // namespace c6502::passes
