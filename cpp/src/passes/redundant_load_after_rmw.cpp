// Drops a redundant `LDA M` that follows a read-modify-write of M.
//
// After `DEC M`, `INC M`, `ASL M`, `LSR M`, `ROL M`, or `ROR M`, the N
// and Z flags are set from M's new value, which is what an `LDA M` right
// after it would set them to. If A's value isn't read before A is next
// written, the `LDA M` only sets the flags again, and the pass drops it.
//
// The case that called for it
//
// After `loop_rotate` rewrites a `for (T x = N; x >= 0; x--)` loop to
// test at the bottom, `dec_peephole` collapses the step's SBC chain to
// a `DEC m`, and the special case for a comparison with zero in
// `_jcmp_signed_ordering` lowers the test to `LDA m; B<PL|MI> target`,
// the code is
//
//     DEC m
//     LDA m
//     BPL .top
//
// and this pass drops the LDA, which leaves `DEC m; BPL .top`, the usual
// 6502 idiom for a signed countdown loop.
//
// The pattern
//
// Two consecutive instructions,
//
//     Inc(dst=M) | Dec(dst=M) | ASL(dst=M) | LSR(dst=M) |
//     ROL(dst=M) | ROR(dst=M)
//     Mov(src=M, dst=Reg(A))
//
// with the same operand M in both. M is one of
//
//   * `Data` (absolute) or `ZP` (zero page), as in `DEC mem; LDA mem`,
//     the usual form of a counter in memory;
//   * an array element indexed by X, `IndexedData(name, k, X)`, as in
//     `DEC t,X; LDA t,X`. Neither instruction writes X, so both name
//     the same byte;
//   * `Reg(X)` or `Reg(Y)`, as in `DEX; TXA` / `DEY; TYA`, the usual
//     form of a counter in a register. The registers have only
//     DEX/DEY/INX/INY and no shift or rotate, so only `Inc` and `Dec`
//     arise there.
//
// M can't be `Reg(A)`. A shift of the accumulator has `dst = A`, but the
// asm IR has no `Mov` with `src = dst = A`, so the pair doesn't arise.
//
// Soundness
//
// * N and Z. All six read-modify-writes set N and Z from the value they
//   write back to M, and `LDA M` sets them from the value it reads.
//   The two instructions are adjacent, so nothing between them changes
//   M, and the two settings are the same. Any later branch on N or Z
//   sees the same condition.
// * C and V. DEC and INC leave C and V alone, and so does LDA. ASL, LSR,
//   ROL, and ROR set C from the bit shifted out and leave V alone, which
//   LDA doesn't change either. So C and V at the branch are the same
//   with or without the LDA.
// * A. Dropping the LDA leaves A holding what it held before. The pass
//   requires A to be dead at the point of the deletion, with no read
//   before the next write, so nothing later can see the difference.
//
// Where it runs
//
// After `replace_pseudoregisters`, since M must be a concrete `Data` or
// `ZP` and the operands are `Pseudo`s before that, and after
// `dec_peephole` / `inc_peephole`, which make the read-modify-write
// form. It runs before `expand_long_branches`; it only shrinks code and
// never adds a branch.
#include "passes/redundant_load_after_rmw.h"

#include <cstddef>
#include <set>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "passes/inc_peephole.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using Instrs = std::vector<ir::Type_instruction>;
using LabelIndex = std::unordered_map<std::string, std::size_t>;

bool is_reg_a(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

// Returns the destination of an `Inc`, `Dec`, `ASL`, `LSR`, `ROL`, or
// `ROR`, or null for any other instruction.
const ir::Type_operand* rmw_dst(const ir::Type_instruction& instr) {
  return std::visit(
      [](const auto& ins) -> const ir::Type_operand* {
        using T = std::decay_t<decltype(ins)>;
        if constexpr (std::is_same_v<T, ir::Inc> || std::is_same_v<T, ir::Dec> ||
                      std::is_same_v<T, ir::ArithmeticShiftLeft> ||
                      std::is_same_v<T, ir::LogicalShiftRight> ||
                      std::is_same_v<T, ir::RotateLeft> ||
                      std::is_same_v<T, ir::RotateRight>) {
          return &ins.dst;
        } else {
          return nullptr;
        }
      },
      instr);
}

// Returns the index of each `Label` by name, the last one where a name
// repeats, for finding branch targets quickly.
LabelIndex index_labels(const Instrs& instrs) {
  LabelIndex out;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (const auto* l = as<ir::Label>(instrs[i])) {
      out[l->name] = i;
    }
  }
  return out;
}

// Returns whether `instr` is a read-modify-write in place whose
// destination is
//
//   * `Data` (absolute memory),
//   * `ZP` (zero-page memory),
//   * `IndexedData(name, k, X)`, an array element (`DEC t,X; LDA t,X`,
//     where X can't change between the two adjacent instructions, so
//     both name one byte), or
//   * `Reg(X)` or `Reg(Y)` (DEX/DEY/INX/INY only, since X and Y have no
//     shift or rotate).
//
// `Frame`, `Stack`, and `Indirect` use indirect-Y addressing, which the
// 6502's read-modify-write opcodes don't have, so those stay as SBC/ADC
// chains. `Reg(A)` is the target of a shift of the accumulator, and a
// following `LDA M` can't have `M = Reg(A)`, so that pair doesn't arise.
bool is_eligible_rmw(const ir::Type_instruction& instr) {
  const ir::Type_operand* dst = rmw_dst(instr);
  if (dst == nullptr) {
    return false;
  }
  if (isa<ir::Data, ir::ZP>(*dst) || is_x_indexed(*dst)) {
    return true;
  }
  const auto* r = as<ir::Reg>(*dst);
  return r != nullptr && isa<ir::X, ir::Y>(r->reg);
}

// Returns whether `a` and `b` are the same `Data`, `ZP`, `Reg`, or
// `IndexedData` operand.
bool operands_match(const ir::Type_operand& a, const ir::Type_operand& b) {
  if (const auto* da = as<ir::Data>(a)) {
    const auto* db = as<ir::Data>(b);
    return db != nullptr && da->name == db->name && da->offset == db->offset;
  }
  if (const auto* za = as<ir::ZP>(a)) {
    const auto* zb = as<ir::ZP>(b);
    return zb != nullptr && za->address == zb->address && za->offset == zb->offset;
  }
  if (const auto* ra = as<ir::Reg>(a)) {
    const auto* rb = as<ir::Reg>(b);
    return rb != nullptr && ra->reg.index() == rb->reg.index();
  }
  if (isa<ir::IndexedData>(a)) {
    return isa<ir::IndexedData>(b) && a == b;
  }
  return false;
}

// Returns whether `nxt` is `Mov(M, Reg(A))` with M the destination of
// `rmw`, a load of the byte just written that only sets the flags
// again. A volatile LDA is never redundant, since its read is the
// effect that dropping it would erase.
bool is_redundant_lda_after_rmw(const ir::Type_instruction& rmw,
                                const ir::Type_instruction& nxt) {
  const auto* mov = as<ir::Mov>(nxt);
  if (mov == nullptr || mov->is_volatile || !is_reg_a(mov->dst)) {
    return false;
  }
  return operands_match(*rmw_dst(rmw), mov->src);
}

bool contains_a(const std::vector<std::string>& reg_args) {
  for (const auto& r : reg_args) {
    if (r == "A") {
      return true;
    }
  }
  return false;
}

// Returns whether `instr` reads A, as a source or as the destination of
// a read-modify-write.
bool reads_a_here(const ir::Type_instruction& instr) {
  if (const auto* m = as<ir::Mov>(instr)) {
    return is_reg_a(m->src);
  }
  if (const auto* p = as<ir::Push>(instr)) {
    return is_reg_a(p->src);
  }
  if (const auto* a = as<ir::Add>(instr)) {
    return is_reg_a(a->src) || is_reg_a(a->dst);
  }
  if (const auto* s = as<ir::Sub>(instr)) {
    return is_reg_a(s->src) || is_reg_a(s->dst);
  }
  if (const auto* a = as<ir::And>(instr)) {
    return is_reg_a(a->src) || is_reg_a(a->dst);
  }
  if (const auto* o = as<ir::Or>(instr)) {
    return is_reg_a(o->src) || is_reg_a(o->dst);
  }
  if (const auto* x = as<ir::Xor>(instr)) {
    return is_reg_a(x->src1) || is_reg_a(x->src2) || is_reg_a(x->dst);
  }
  if (const auto* c = as<ir::Compare>(instr)) {
    return is_reg_a(c->left) || is_reg_a(c->right);
  }
  if (const ir::Type_operand* dst = rmw_dst(instr)) {
    return is_reg_a(*dst);
  }
  if (const auto* c = as<ir::Call>(instr)) {
    return contains_a(c->reg_args);
  }
  if (const auto* j = as<ir::Jump>(instr)) {
    return contains_a(j->reg_args);
  }
  return false;
}

// Returns whether `instr` writes A without reading it first.
bool kills_a_here(const ir::Type_instruction& instr) {
  if (const auto* m = as<ir::Mov>(instr)) {
    if (is_reg_a(m->dst) && !is_reg_a(m->src)) {
      return true;
    }
  }
  if (const auto* p = as<ir::Pop>(instr)) {
    return is_reg_a(p->dst);
  }
  // A callee changes A: a 1-byte return leaves the result there, and a
  // call that returns in HARGS leaves A undefined. A `LoadAddress` is
  // `LDA #<x; STA dst; LDA #>x; STA dst+1`, or `LDA FP; ADC #off; ...`
  // for a local, so A is loaded before it's used either way.
  return isa<ir::Call, ir::LoadAddress>(instr);
}

// Returns whether A's value at `idx` is dead: every path forward reaches
// a write of A that doesn't read it before any read of A.
//
// The walk goes forward within the block. At a `Branch` it follows both
// the fall-through and the target (found through `labels`), and A is
// dead at the branch when it is dead on both. At a `Jump` it goes on at
// the target. A branch or jump to a label the function doesn't have
// counts as a read.
//
// `visited` holds the branches and jumps on the path being walked.
// Coming back to one means the walk went around a loop without finding
// a read, and the walk returns true there. That is sound, since a read
// of A on the cycle would have been found before the walk came back.
bool a_dead_after(const Instrs& instrs, std::size_t idx, const LabelIndex& labels,
                  std::set<std::size_t>& visited) {
  while (idx < instrs.size()) {
    if (visited.count(idx) != 0) {
      return true;
    }
    const ir::Type_instruction& instr = instrs[idx];
    if (const auto* r = as<ir::Ret>(instr)) {
      return !r->save_a;
    }
    if (const auto* r = as<ir::Return>(instr)) {
      return !r->save_a;
    }
    if (const auto* b = as<ir::Branch>(instr)) {
      const auto it = labels.find(b->target);
      if (it == labels.end()) {
        return false;
      }
      visited.insert(idx);
      const bool dead = a_dead_after(instrs, idx + 1, labels, visited) &&
                        a_dead_after(instrs, it->second + 1, labels, visited);
      visited.erase(idx);
      return dead;
    }
    if (const auto* j = as<ir::Jump>(instr)) {
      const auto it = labels.find(j->target);
      if (it == labels.end()) {
        return false;
      }
      visited.insert(idx);
      const bool dead = a_dead_after(instrs, it->second + 1, labels, visited);
      visited.erase(idx);
      return dead;
    }
    if (isa<ir::Label>(instr)) {
      // Falling into a label transfers nothing, so the walk goes on.
      ++idx;
      continue;
    }
    if (reads_a_here(instr)) {
      return false;
    }
    if (kills_a_here(instr)) {
      return true;
    }
    ++idx;
  }
  return true;
}

// Copies the function's instructions, leaving out each `LDA M` that
// follows an eligible read-modify-write of M when A is dead after it.
void rewrite_function(ir::Function& fn) {
  Instrs& instrs = fn.instructions;
  const LabelIndex labels = index_labels(instrs);
  std::vector<std::size_t> drops;
  const std::size_t n = instrs.size();
  std::size_t i = 0;
  while (i < n) {
    std::set<std::size_t> visited;
    if (i + 1 < n && is_eligible_rmw(instrs[i]) &&
        is_redundant_lda_after_rmw(instrs[i], instrs[i + 1]) &&
        a_dead_after(instrs, i + 2, labels, visited)) {
      drops.push_back(i + 1);
      i += 2;
      continue;
    }
    ++i;
  }
  if (drops.empty()) {
    return;
  }
  Instrs out;
  out.reserve(n - drops.size());
  std::size_t next = 0;
  for (std::size_t k = 0; k < n; ++k) {
    if (next < drops.size() && drops[next] == k) {
      ++next;
      continue;
    }
    out.push_back(std::move(instrs[k]));
  }
  instrs = std::move(out);
}

}  // namespace

asm_ast::Program apply_redundant_load_after_rmw(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
