// Folds `LDA M; AND #$80; B(EQ|NE) target` into `LDA M; B(PL|MI)
// target`.
//
// The pattern
//
// Three consecutive instructions:
//
//     Mov(<any src>, Reg(A))      // LDA src, which sets A and N = bit7(src)
//     And(Imm(0x80), Reg(A))      // AND #$80, which sets N = bit7(A & 0x80) = bit7(A)
//     Branch(EQ | NE, target)     // BEQ/BNE, which tests Z
//
// become
//
//     Mov(<any src>, Reg(A))
//     Branch(PL | MI, target)     // BPL tests bit7(A) == 0, BMI bit7(A) == 1
//
// Soundness
//
// After `LDA M; AND #$80`:
//   * A = M & 0x80, which is 0 or 0x80,
//   * N = bit7(A & 0x80) = bit7(M),
//   * Z = (A & 0x80 == 0) = (bit7(M) == 0).
//
// The branch reads Z. BEQ is taken when Z = 1, which is when bit7(M) =
// 0, which is when N = 0, which is when BPL is taken. In the same way
// BNE becomes BMI.
//
// With the AND gone and the branch flipped, A holds `M` where it held
// `M & 0x80`. The test of the flag is the same, since `LDA M` already
// set N = bit7(M). The rewrite is sound only when A is unused after the
// branch, since otherwise dropping the AND changes the value A is seen
// to hold. `a_dead_at` checks A at both of the branch's successors.
//
// The case that called for it
//
// `while ((x & 0x80) == 0)` is the test that x is non-negative as a
// signed byte, common after a `do { ... x--; } while (...)` rotates the
// test to the bottom. c99_to_tac makes a promotion to int, a bitwise
// AND, and a test of zero. After the DCE of the high byte and the
// elimination of round trips, the loop's tail is
//
//     LDA x
//     AND #$80
//     BEQ continue
//
// which becomes `LDA x; BPL continue`, saving 2 bytes and 2 cycles each
// time.
//
// The low bit
//
// The same goes for bit 0, through the carry:
//
//     And(Imm(0x01), Reg(A))      // AND #$01
//     Branch(EQ | NE, target)     // BEQ/BNE
//
// becomes
//
//     LogicalShiftRight(Reg(A))   // LSR A, so C = bit0(A)
//     Branch(CC | CS, target)     // BCC tests bit0(A) == 0, BCS == 1
//
// `LSR A` is 1 byte to the AND's 2, and both take 2 cycles. It leaves A
// shifted where the AND left it masked, and it writes C, which the AND
// didn't, and sets N and Z from the shifted value, so A, C, N, and Z
// must all be dead on both paths after the branch. Zork's
// `store_with_sign` (examples/zork/numbers.c) tests `sign_bit & 0x01`
// this way.
//
// Bit 6
//
// `BIT M` copies bit 6 of M into V, so a test of bit 6 needn't load A:
//
//     Mov(M, Reg(A))              // LDA M
//     And(Imm(0x40), Reg(A))      // AND #$40
//     Branch(EQ | NE, target)     // BEQ/BNE
//
// becomes
//
//     BitTest(M)                  // BIT M, so V = bit6(M)
//     Branch(VC | VS, target)     // BVC tests bit6(M) == 0, BVS == 1
//
// M is a `Data` or `ZP` byte, which `BIT` can address, and isn't
// volatile. `BIT` leaves A as it was, sets N from bit 7 of M and Z from
// A AND M, and writes V, where the load and the AND left V alone. So A,
// N, Z, and the V from before must all be dead on both paths after the
// branch. That saves 2 bytes and 2 cycles. Zork's `branch`
// (examples/zork/decode.c) tests bit 6 of its offset byte twice.
//
// Where it runs
//
// Inside the peephole fixed point. Earlier passes (mem_const_prop,
// const_arith_fold, round_trip_load_drop, asm_dead_store) have to run
// first to expose the plain `LDA M; AND #$80; B*` shape, since the
// original lowering has STA/LDA round trips and operations on the high
// byte that hide it. The pass looks at a window of a few instructions,
// so its place in the fixed point matters little.
#include "passes/and_sign_bit_branch.h"

#include <optional>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"
#include "passes/redundant_compare.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_reg_a(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

// Returns the index of the first label named `name`, or -1.
long first_label(Instrs instrs, const std::string& name) {
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    const auto* l = as<ir::Label>(instrs[k]);
    if (l != nullptr && l->name == name) {
      return static_cast<long>(k);
    }
  }
  return -1;
}

// Returns whether `dead(k)` holds after the branch at `branch_idx` on
// the fall-through and at the branch's target. A target that isn't a
// label of the function counts as live.
template <class Dead>
bool dead_both_ways(Instrs instrs, std::size_t branch_idx, Dead dead) {
  if (!dead(branch_idx + 1)) {
    return false;
  }
  const long k = first_label(instrs, std::get<ir::Branch>(instrs[branch_idx]).target);
  return k >= 0 && dead(static_cast<std::size_t>(k));
}

// Returns whether A is dead after the branch at `branch_idx`, both on
// the fall-through and at the target. The rewrite drops the AND, so on
// either path A holds the unmasked byte.
bool a_dead_both_ways(Instrs instrs, const LabelMap& labels, std::size_t branch_idx) {
  return dead_both_ways(instrs, branch_idx,
                        [&](std::size_t k) { return a_dead_at(instrs, labels, k); });
}

// Returns whether N and Z are dead on both paths after the branch at
// `branch_idx`, since without the AND, Z comes from all of A.
bool flags_dead_both_ways(Instrs instrs, const LabelMap& labels, std::size_t branch_idx) {
  return dead_both_ways(instrs, branch_idx,
                        [&](std::size_t k) { return flags_dead_at(instrs, labels, k); });
}

// Returns whether A, C, N, and Z are all dead on both paths after the
// branch at `branch_idx`.
bool all_dead_both_ways(Instrs instrs, const LabelMap& labels, std::size_t branch_idx) {
  return dead_both_ways(instrs, branch_idx, [&](std::size_t k) {
    return a_dead_at(instrs, labels, k) && c_dead_at(instrs, labels, k) &&
           flags_dead_at(instrs, labels, k);
  });
}

// Returns whether A, N, Z, and V are all dead on both paths after the
// branch at `branch_idx`, as `BIT M; BVC/BVS` needs.
bool bit6_dead_both_ways(Instrs instrs, const LabelMap& labels, std::size_t branch_idx) {
  return dead_both_ways(instrs, branch_idx, [&](std::size_t k) {
    return a_dead_at(instrs, labels, k) && flags_dead_at(instrs, labels, k) &&
           v_dead_at(instrs, labels, k);
  });
}

// Returns whether `instr` is `Mov(Reg(A), <memory>)`, a STA that keeps
// A's value and leaves N and Z alone. The destination's addressing mode
// doesn't matter for this fold.
bool is_a_store_to_mem(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  return m != nullptr && !m->is_volatile && is_reg_a(m->src) && !isa<ir::Reg>(m->dst);
}

// Returns whether `instr` is a Mov that `asm_emit` writes with an `LDA`,
// which leaves A holding the source and N and Z set from it. That is
// `Mov(<non-Reg src>, Reg(A))`, a load into A, and `Mov(<non-Reg src>,
// <memory dst>)`, the `LDA src; STA dst` pair, whose load half does the
// same. `TXA` and `TYA` set N and Z too. A plain STA, `Mov(Reg(A),
// <mem>)`, doesn't load A, and a load into X or Y (`LDX M`) doesn't
// load the A that the AND tests.
bool is_lda_to_a(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  if (m == nullptr) {
    return false;
  }
  if (const auto* r = as<ir::Reg>(m->src)) {
    return isa<ir::X, ir::Y>(r->reg) && is_reg_a(m->dst);
  }
  return !isa<ir::Reg>(m->dst) || is_reg_a(m->dst);
}

bool is_and_imm(const ir::Type_instruction& instr, int value) {
  const auto* a = as<ir::And>(instr);
  if (a == nullptr) {
    return false;
  }
  const auto* imm = as<ir::Imm>(a->src);
  return imm != nullptr && (imm->value & 0xFF) == value && is_reg_a(a->dst);
}

bool is_eq_ne_branch(const ir::Type_instruction& instr) {
  const auto* b = as<ir::Branch>(instr);
  return b != nullptr && isa<ir::EQ, ir::NE>(b->cond);
}

// Returns the branch to the target of the branch `br` on `if_eq` when
// `br` is a BEQ and on `if_ne` when it is a BNE.
ir::Type_instruction flipped(const ir::Type_instruction& br, ir::Type_condition if_eq,
                             ir::Type_condition if_ne) {
  const auto& b = std::get<ir::Branch>(br);
  return ir::Branch{.cond = isa<ir::EQ>(b.cond) ? std::move(if_eq) : std::move(if_ne),
                    .target = b.target};
}

void rewrite_function(ir::Function& fn) {
  const Instrs instrs = fn.instructions;
  const std::size_t n = instrs.size();
  const LabelMap labels(instrs);
  // The label references `flags_set_from_a` reads, made the first time
  // it's asked, since the walk doesn't change the instructions.
  std::optional<redundant_compare::LabelRefs> refs;
  // Returns whether N and Z at `instrs[i]` were set from A's value by
  // the instruction that last wrote A (`redundant_compare::flags_from`,
  // which passes over stores and conditional branches and follows
  // labels to every way in).
  auto flags_set_from_a = [&](std::size_t i) {
    if (!refs) {
      refs = redundant_compare::refs_with_tables(instrs);
    }
    return redundant_compare::flags_from(instrs, i, redundant_compare::kA, &*refs);
  };
  std::vector<ir::Type_instruction> out;
  out.reserve(n);
  std::size_t i = 0;
  while (i < n) {
    // The window of three instructions, LDA; AND #$80; Branch.
    if (i + 2 < n && is_lda_to_a(instrs[i]) && is_and_imm(instrs[i + 1], 0x80) &&
        is_eq_ne_branch(instrs[i + 2]) && a_dead_both_ways(instrs, labels, i + 2)) {
      out.push_back(instrs[i]);
      out.push_back(flipped(instrs[i + 2], ir::PL{}, ir::MI{}));
      i += 3;
      continue;
    }
    // The window of four with a STA between, which arises after
    // split_mem_to_mem lowers the LDA and STA of a move from memory to
    // memory into two atoms. What was `Mov(M, dst_mem); AND #$80;
    // Branch`, which the case above matches, is now `Mov(M, A); Mov(A,
    // dst); AND #$80; Branch`. The STA touches neither A nor N and Z,
    // so the same fold applies.
    if (i + 3 < n && is_lda_to_a(instrs[i]) && is_a_store_to_mem(instrs[i + 1]) &&
        is_and_imm(instrs[i + 2], 0x80) && is_eq_ne_branch(instrs[i + 3]) &&
        a_dead_both_ways(instrs, labels, i + 3)) {
      out.push_back(instrs[i]);
      out.push_back(instrs[i + 1]);
      out.push_back(flipped(instrs[i + 3], ir::PL{}, ir::MI{}));
      i += 4;
      continue;
    }
    // Any instruction that set N from A, with stores and branches
    // between it and the AND, as `a = x - 1; if (a & 0x80)` leaves
    // `SEC; SBC #$01; STA a; AND #$80; BEQ`.
    if (i + 1 < n && is_and_imm(instrs[i], 0x80) && is_eq_ne_branch(instrs[i + 1]) &&
        flags_set_from_a(i) && a_dead_both_ways(instrs, labels, i + 1) &&
        flags_dead_both_ways(instrs, labels, i + 1)) {
      out.push_back(flipped(instrs[i + 1], ir::PL{}, ir::MI{}));
      i += 2;
      continue;
    }
    if (i + 2 < n) {
      const auto* load = as<ir::Mov>(instrs[i]);
      const auto* mask = as<ir::And>(instrs[i + 1]);
      if (load != nullptr && !load->is_volatile && isa<ir::Data, ir::ZP>(load->src) &&
          is_reg_a(load->dst) && mask != nullptr &&
          mask->src == ir::Type_operand{ir::Imm{.value = 0x40}} && is_reg_a(mask->dst) &&
          is_eq_ne_branch(instrs[i + 2]) && bit6_dead_both_ways(instrs, labels, i + 2)) {
        out.push_back(ir::BitTest{.src = load->src});
        out.push_back(flipped(instrs[i + 2], ir::VC{}, ir::VS{}));
        i += 3;
        continue;
      }
    }
    if (i + 1 < n && is_and_imm(instrs[i], 0x01) && is_eq_ne_branch(instrs[i + 1]) &&
        all_dead_both_ways(instrs, labels, i + 1)) {
      out.push_back(ir::LogicalShiftRight{.dst = std::get<ir::And>(instrs[i]).dst});
      out.push_back(flipped(instrs[i + 1], ir::CC{}, ir::CS{}));
      i += 2;
      continue;
    }
    out.push_back(instrs[i]);
    ++i;
  }
  fn.instructions = std::move(out);
}

}  // namespace

asm_ast::Program apply_and_sign_bit_branch(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
