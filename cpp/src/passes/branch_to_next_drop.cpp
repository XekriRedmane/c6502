// Drops a `Branch` whose target is the label right after it, and a
// `Jump` whose target is one of the labels right after it.
//
// A `Branch` right after a `Branch` on the same condition, with no
// label between, goes too. The first one falls through only when its
// condition is false, and a branch changes no flag, so the second is
// never taken. A wide comparison whose bytes have become known can
// leave a `BNE` for each byte in a row, as `*ul_ptr != 1000` did once
// each byte read through the pointer went to the variable's own byte,
// and `branch_thread` would otherwise retarget them one per sweep of
// the peephole fixed point.
//
// A `Jump` to the next label can be left when a conditional branch
// over it was inverted to reach the other target, as in `BCC top; JMP
// done; done:`. Like a branch, it changes no register, flag, or memory,
// so it goes.
//
// Motivating shape
//
// When earlier passes prune both arms of a conditional diamond to
// nothing, what is left is
//
//     Branch(cond, L)
//     Label(L)
//
// Both arms reach `L`: the taken branch jumps there, and falling
// through arrives there directly. The branch has no other effect (it
// writes no register or memory, and the flags it reads are unchanged),
// so it can go.
//
// The main source is `_translate_sign_extend` in `tac_to_asm.py`,
// which emits
//
//     Mov(src.high, A)
//     Branch(MI, sx_neg)
//     Mov(Imm(0x00), A)
//     Jump(sx_done)
//     Label(sx_neg)
//     Mov(Imm(0xFF), A)
//     Label(sx_done)
//     Mov(A, dst.high)
//
// When `dst.high` is dead (the widened value is used only by tests of
// bit 7, by indexing under 256 bytes, or by arithmetic truncated back to
// a byte), dead-code elimination drops `Mov(#$00, A)`, `Mov(#$FF, A)`,
// and `Mov(A, dst.high)`. `branch_invert` collapses `BMI sx_neg; JMP
// sx_done; Label(sx_neg)` into `Branch(PL, sx_done)`, and once
// `dead_label_drop` removes `Label(sx_neg)`, which nothing names any
// more, what is left is
//
//     Mov(src.high, A)
//     Branch(PL, sx_done)
//     Label(sx_done)
//
// The branch goes to the very next instruction, and this pass drops it.
//
// Soundness
//
// A `Branch` reads only the flags, never an operand. Whether the
// condition is true (a jump to `L`) or false (a fall into the next
// instruction, which is `Label(L)`), the PC that results is the same,
// and no register, memory byte, or flag changes.
//
// Dropping the label with the branch
//
// When the pass drops `Branch(_, L); Label(L)`, the label may be left
// with no other reference, and the pass has to drop it too, in the
// same run.
//
// `flags_dead_at` and `all_flags_dead_at` (in `asm_liveness`) take any
// `Label` met on a forward walk to end the walk with the flags dead.
// That rests on the code generator's convention that every basic block
// sets the flags before any branch of its lowering reads them. Within
// one sweep of the peephole fixed point, a label left with no reference
// right before, say, `Branch(PL, ...)` breaks that convention:
// `dead_a_arith`, running later in the same sweep, would walk forward
// from the `Sub` before it, meet the `Label`, take the flags to be
// dead, and drop the `Sub` whose N the `Branch` reads.
//
// Dropping the label closes that gap. When other branches, jumps, or
// `Phi` arguments name the label, it stays, since it is then a join of
// paths, which is the case the convention covers.
//
// Iteration
//
// One forward walk drops every adjacent `Branch(_, L), Label(L)` pair,
// and the label goes when nothing else in the function names `L`. Two
// branches in a row to the same next label collapse together: the
// inner pair goes in this run, and the outer pair on the next sweep of
// the fixed point.
//
// Where it runs
//
// In the asm peephole fixed point, right after `branch_invert` and
// `dead_label_drop`, the passes that usually make the shape. It runs
// in both pipelines and never adds instructions, which the fixed
// point's shrinking relies on.
#include "passes/branch_to_next_drop.h"

#include <cstddef>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;

// Returns how many `Jump.target`, `Branch.target`, and `Phi` argument
// `pred_label` references each label name has.
std::unordered_map<std::string, int> label_ref_counts(
    const std::vector<ir::Type_instruction>& instrs) {
  std::unordered_map<std::string, int> out;
  for (const auto& instr : instrs) {
    if (const auto* jump = as<ir::Jump>(instr)) {
      ++out[jump->target];
    } else if (const auto* branch = as<ir::Branch>(instr)) {
      ++out[branch->target];
    } else if (const auto* phi = as<ir::Phi>(instr)) {
      for (const auto& arg : phi->args) {
        ++out[std::get<ir::AsmPhiArg>(arg).pred_label];
      }
    }
  }
  return out;
}

void rewrite_function(ir::Function& fn) {
  std::vector<ir::Type_instruction>& instrs = fn.instructions;
  const auto ref_counts = label_ref_counts(instrs);
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  const std::size_t n = instrs.size();
  std::size_t i = 0;
  while (i < n) {
    const ir::Type_instruction& a = instrs[i];
    const auto* branch = as<ir::Branch>(a);
    if (branch != nullptr && i + 1 < n) {
      const auto* next = as<ir::Label>(instrs[i + 1]);
      if (next != nullptr && branch->target == next->name) {
        // The branch being dropped holds one reference to the label.
        // If that is the only one in the function, the label goes too,
        // in this run, so that `flags_dead_at`'s stop at a label doesn't
        // take the flags of the instruction before it to be dead within
        // the same sweep of the fixed point.
        const auto it = ref_counts.find(next->name);
        const int refs = it == ref_counts.end() ? 0 : it->second;
        i += refs <= 1 ? 2 : 1;
        continue;
      }
    }
    if (branch != nullptr && !out.empty()) {
      const auto* prev = as<ir::Branch>(out.back());
      if (prev != nullptr && prev->cond.index() == branch->cond.index()) {
        i += 1;
        continue;
      }
    }
    if (const auto* jump = as<ir::Jump>(a)) {
      std::size_t j = i + 1;
      bool found = false;
      while (j < n) {
        const auto* label = as<ir::Label>(instrs[j]);
        if (label == nullptr) {
          break;
        }
        if (label->name == jump->target) {
          found = true;
          break;
        }
        j += 1;
      }
      if (found) {
        // The jump goes. The labels stay, since another site may still
        // name them, and `dead_label_drop` removes the ones nothing
        // does.
        i += 1;
        continue;
      }
    }
    out.push_back(std::move(instrs[i]));
    i += 1;
  }
  instrs = std::move(out);
}

}  // namespace

asm_ast::Program apply_branch_to_next_drop(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
