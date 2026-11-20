// Fuses `LDA M; CMP N; B<cond> L; Label L2; SEC; SBC N` (one N) by
// moving the `SEC; SBC` ahead of the branch and dropping the `CMP`.
//
// The pattern
//
// Six consecutive instructions, or five without the label, and without
// the `SEC` after a `BCC`, whose fall-through has C set, as
// `carry_known` leaves it:
//
//     [a]   Mov(<X>, Reg(A))      // LDA M, or any other load of A
//     [a+1] Compare(Reg(A), N)    // CMP N
//     [a+2] Branch(<cond>, L1)    // B<cond>, cond one of CC CS EQ NE MI PL
//     [a+3] Label(L2)             // reached only by falling through
//     [a+4] SetCarry()            // SEC
//     [a+5] Sub(N, Reg(A))        // SBC N, the N of the CMP
//
// becomes
//
//     Mov(<X>, Reg(A))
//     SetCarry()
//     Sub(N, Reg(A))
//     Branch(<cond>, L1)
//     Label(L2)
//
// Why this is sound
//
// `CMP N` sets C, Z, and N from `A - N`: C = 1 if and only if A >= N,
// Z = 1 if and only if A == N, and N = bit7(A - N). `SEC; SBC N`, with
// A unchanged since the load, sets the same C, Z, and N, because SBC
// reads a carry in of 1 and computes `A - N - (1 - C_in) = A - N`. It
// also writes A and sets V. V couldn't be read before, since CMP
// doesn't set V on the 6502, and the branch conditions allowed don't
// read V. So the `CMP` is redundant once the SBC's flags reach the
// branch, and the pass drops the CMP and moves the SBC up.
//
// The conditions
//
//   * The branch condition is CC, CS, EQ, NE, MI, or PL, which read C,
//     Z, and N. BVC and BVS are left out, since SBC sets V and CMP
//     doesn't, so V at the branch would differ.
//   * No `Branch` or `Jump` elsewhere in the function targets
//     `Label(L2)`. Another path could otherwise reach L2 with another
//     value in A, and the reordering would corrupt it for those paths.
//   * The CMP's right operand and the SBC's source are the same
//     `Data`, `ZP`, or `Imm` operand, field for field.
//   * The first instruction writes `Reg(A)` and sets N/Z from A's new
//     value. Any `Mov` to A or any of the operations on A qualify, as
//     with the writer of A in `const_arith_fold`.
//   * A and V are dead at the branch target L1, which must be a label
//     of the function. The rewrite runs the SBC on the taken path too,
//     so A there holds M - N where it held M, and V holds the SBC's
//     overflow where it held its earlier value.
//
// Where it runs
//
// Inside the peephole fixed point. Earlier passes (mem_const_prop, and
// redundant_load with its tracking across a fall-through into the next
// block) have to have run first to leave the six-instruction shape.
// The raw output of `tac_to_asm` has extra STA/LDA round trips that
// hide it.
//
// The case that called for it
//
// `if (a >= b) { x = a - b; ... }` is
//
//     LDA a
//     CMP b
//     BCC .end           ; skip when a < b
//     .ssa_block:
//     SEC
//     SBC b
//     STA x
//
// and the fusion saves 5 bytes and 6 cycles at each occurrence.
#include "passes/cmp_sbc_fusion.h"

#include <set>
#include <string>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

// Returns whether A and V are dead at the branch target. On the taken
// path the original leaves A holding M and V untouched, while the
// rewrite has already run the `SBC`, so A holds M - N and V is the
// subtraction's overflow. The fall-through path runs the `SBC` either
// way. A target that isn't a label of this function counts as live.
bool taken_path_safe(Instrs instrs, const LabelMap& labels,
                     const std::string& target) {
  const long idx = labels.find(target);
  if (idx < 0) {
    return false;
  }
  const auto at = static_cast<std::size_t>(idx);
  return a_dead_at(instrs, labels, at) && v_dead_at(instrs, labels, at);
}

std::set<std::string, std::less<>> collect_branch_targets(Instrs instrs) {
  std::set<std::string, std::less<>> out;
  for (const auto& instr : instrs) {
    if (const auto* j = as<ir::Jump>(instr)) {
      out.insert(j->target);
    } else if (const auto* b = as<ir::Branch>(instr)) {
      out.insert(b->target);
    }
  }
  return out;
}

bool writes_a_with_flag_effect(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::Mov:
      return is_reg_a(std::get<ir::Mov>(instr).dst);
    case ir::Kind_instruction::And:
      return is_reg_a(std::get<ir::And>(instr).dst);
    case ir::Kind_instruction::Or:
      return is_reg_a(std::get<ir::Or>(instr).dst);
    case ir::Kind_instruction::Add:
      return is_reg_a(std::get<ir::Add>(instr).dst);
    case ir::Kind_instruction::Sub:
      return is_reg_a(std::get<ir::Sub>(instr).dst);
    case ir::Kind_instruction::Xor:
      return is_reg_a(std::get<ir::Xor>(instr).dst);
    default:
      return false;
  }
}

// Returns the compare when `instr` is `Compare(Reg(A), <op>)`, or null.
// The caller checks that the right operand matches the SBC's source.
const ir::Compare* as_cmp_against(const ir::Type_instruction& instr) {
  const auto* c = as<ir::Compare>(instr);
  return c != nullptr && is_reg_a(c->left) ? c : nullptr;
}

// Returns the subtraction when `instr` is `Sub(<op>, Reg(A))`, or null.
const ir::Sub* as_sub_against(const ir::Type_instruction& instr) {
  const auto* s = as<ir::Sub>(instr);
  return s != nullptr && is_reg_a(s->dst) ? s : nullptr;
}

const ir::Branch* as_eligible_branch(const ir::Type_instruction& instr) {
  const auto* b = as<ir::Branch>(instr);
  if (b != nullptr &&
      isa<ir::CC, ir::CS, ir::EQ, ir::NE, ir::MI, ir::PL>(b->cond)) {
    return b;
  }
  return nullptr;
}

// Returns whether two memory or immediate operands are the same, field
// for field.
bool operands_equal(const ir::Type_operand& a, const ir::Type_operand& b) {
  if (const auto* da = as<ir::Data>(a)) {
    const auto* db = as<ir::Data>(b);
    return db != nullptr && da->name == db->name && da->offset == db->offset;
  }
  if (const auto* za = as<ir::ZP>(a)) {
    const auto* zb = as<ir::ZP>(b);
    return zb != nullptr && za->address == zb->address && za->offset == zb->offset;
  }
  if (const auto* ia = as<ir::Imm>(a)) {
    const auto* ib = as<ir::Imm>(b);
    return ib != nullptr && ia->value == ib->value;
  }
  return false;
}

void rewrite_function(ir::Function& fn) {
  const std::vector<ir::Type_instruction>& instrs = fn.instructions;
  const std::size_t n = instrs.size();
  const auto branch_targets = collect_branch_targets(instrs);
  const LabelMap labels(instrs);
  std::vector<ir::Type_instruction> out;
  bool changed = false;
  std::size_t i = 0;
  while (i < n) {
    // The fall-through label is optional.
    std::size_t j = i + 3;
    if (j < n) {
      if (const auto* l = as<ir::Label>(instrs[j]);
          l != nullptr && !branch_targets.contains(l->name)) {
        ++j;
      }
    }
    // After `BCC`, C is set on the fall-through, so `carry_known` may
    // have dropped the SEC, and the SBC moved up needs it back.
    const bool sec = j < n && isa<ir::SetCarry>(instrs[j]);
    if (!sec) {
      const ir::Branch* b = i + 2 < n ? as<ir::Branch>(instrs[i + 2]) : nullptr;
      if (b == nullptr || !isa<ir::CC>(b->cond)) {
        j = n;
      }
    }
    const std::size_t sub = sec ? j + 1 : j;
    if (sub < n && writes_a_with_flag_effect(instrs[i])) {
      const ir::Compare* cmp = as_cmp_against(instrs[i + 1]);
      const ir::Branch* branch = as_eligible_branch(instrs[i + 2]);
      const ir::Sub* sbc = as_sub_against(instrs[sub]);
      if (cmp != nullptr && branch != nullptr && sbc != nullptr &&
          operands_equal(cmp->right, sbc->src) &&
          taken_path_safe(instrs, labels, branch->target)) {
        // Emits the load, SEC, SBC, the branch, and the label.
        out.push_back(instrs[i]);
        out.push_back(ir::SetCarry{});
        out.push_back(instrs[sub]);
        out.push_back(instrs[i + 2]);
        for (std::size_t k = i + 3; k < j; ++k) {
          out.push_back(instrs[k]);
        }
        i = sub + 1;
        changed = true;
        continue;
      }
    }
    out.push_back(instrs[i]);
    ++i;
  }
  if (changed) {
    fn.instructions = std::move(out);
  }
}

}  // namespace

asm_ast::Program apply_cmp_sbc_fusion(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
