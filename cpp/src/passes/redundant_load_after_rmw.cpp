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

LabelIndex index_labels(const Instrs& instrs) {
  LabelIndex out;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (const auto* l = as<ir::Label>(instrs[i])) {
      out[l->name] = i;
    }
  }
  return out;
}

// `Frame`, `Stack`, and `Indirect` use indirect-Y addressing, which the
// 6502's read-modify-write opcodes don't have.
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

bool kills_a_here(const ir::Type_instruction& instr) {
  if (const auto* m = as<ir::Mov>(instr)) {
    if (is_reg_a(m->dst) && !is_reg_a(m->src)) {
      return true;
    }
  }
  if (const auto* p = as<ir::Pop>(instr)) {
    return is_reg_a(p->dst);
  }
  return isa<ir::Call, ir::LoadAddress>(instr);
}

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
