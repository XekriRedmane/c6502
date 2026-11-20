// Moves a `TXA` / `TYA` past a branch when A is dead on the taken arm
// but live on the fall-through.
//
// The pattern
//
// Three consecutive instructions:
//
//     [i]   <an op that writes Reg(X|Y) and sets N/Z from the new value>,
//           such as LDX/LDY/INX/INY/DEX/DEY/TAX/TAY
//     [i+1] Mov(Reg(X|Y), Reg(A))     // TXA / TYA
//     [i+2] Branch(cond, target)
//
// The rewrite swaps [i+1] and [i+2]:
//
//     [i]   <op>
//     [i+1] Branch(cond, target)
//     [i+2] Mov(Reg(X|Y), Reg(A))     // TXA / TYA
//
// Soundness
//
// The branch reads N and Z. In the original sequence it sees the flags
// the TXA set from X. In the new one it sees the flags `op` set, which
// also come from X, since `op` wrote X and set N and Z from the value
// it wrote. The flags are the same, so the branch goes the same way.
//
// A's value:
//   * Originally [i+1] sets A to X, and both arms see A == X.
//   * Now A is set to X only on the fall-through arm, after the moved
//     TXA, and the taken arm sees the value A had before [i+1].
//
// So the rewrite changes the A that the taken arm can see, and it is
// sound only when A is dead on the taken arm, `a_dead_at(target)`.
//
// When it wins
//
// The move saves the TXA's 2 cycles on the taken path. The code's size
// is unchanged, since the TXA is 1 byte either way. The pattern shows
// up at dispatches where the register the condition tests is also the
// input of the fall-through arm and the other arm doesn't need A.
//
// The source that called for it is the lowering of `if (state & 0x80)`,
// `LDX state; TXA; BPL .if_end`. The taken path (chase or idle) doesn't
// need A, and only the fall-through path (attack) uses A, for its `AND
// #$0F`. After the move the chase and idle path skips the TXA.
//
// When it doesn't fire
//
//   * [i] must write Reg(X|Y) and set N and Z from the register's new
//     value (`dead_a_arith::writes_reg_setting_n_z`). `LDA M; TXA; B??`
//     doesn't qualify, since the LDA's N and Z come from M, and X is
//     written by the TXA only afterward.
//   * A is live on the taken arm. Moving the TXA would lose the A := X
//     that the taken arm reads.
//   * A is dead on both arms. `dead_a_arith`'s `flags_reflect_src_reg`
//     drops the TXA outright then, so this pass fires only when one arm
//     reads A.
//
// Where it runs
//
// After `replace_pseudoregisters`, when the operands are concrete, and
// after `dead_a_arith`, which drops a TXA that is dead everywhere before
// this pass would consider moving it. It runs inside the peephole fixed
// point. It shrinks no code, since the byte count stays the same, but
// later passes see the TXA in another basic block.
#include "passes/transfer_past_branch.h"

#include <utility>
#include <vector>

#include "passes/asm_liveness.h"
#include "passes/dead_a_arith.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool can_swap(Instrs instrs, std::size_t i, const LabelMap& labels) {
  if (i + 2 >= instrs.size()) {
    return false;
  }
  const ir::Type_instruction& op = instrs[i];
  // [i+1] is Mov(Reg(X|Y), Reg(A)), a TXA or TYA.
  const auto* mov = as<ir::Mov>(instrs[i + 1]);
  if (mov == nullptr || mov->is_volatile) {
    return false;
  }
  const auto* src = as<ir::Reg>(mov->src);
  if (src == nullptr || !isa<ir::X, ir::Y>(src->reg) || !is_reg_a(mov->dst)) {
    return false;
  }
  // [i+2] is a conditional branch.
  const auto* br = as<ir::Branch>(instrs[i + 2]);
  if (br == nullptr) {
    return false;
  }
  // [i] writes the same register and sets N and Z from its new value.
  if (!dead_a_arith::writes_reg_setting_n_z(op, src->reg)) {
    return false;
  }
  // The TXA's write of A has to be read on the fall-through arm, or
  // dead_a_arith would drop the TXA outright. So A has to be dead at
  // the target and live on the fall-through. `a_dead_at` is false when
  // A is live or can't be shown dead.
  const long target = labels.find(br->target);
  if (target < 0) {
    // A target outside the function (a tail call, a trampoline of a
    // long branch) may read A, so nothing moves.
    return false;
  }
  // A is dead at the target, just after the label.
  if (!a_dead_at(instrs, labels, static_cast<std::size_t>(target) + 1)) {
    return false;
  }
  // A is live on the fall-through, just after the branch, which is
  // `i + 3` before the swap. When it is dead on both arms, dead_a_arith
  // drops the TXA and no move is needed.
  return !a_dead_at(instrs, labels, i + 3);
}

void rewrite_function(ir::Function& fn) {
  const Instrs instrs = fn.instructions;
  const LabelMap labels(instrs);
  std::vector<ir::Type_instruction> out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    if (can_swap(instrs, i, labels)) {
      out.push_back(instrs[i]);      // op
      out.push_back(instrs[i + 2]);  // the branch, moved before the TXA
      out.push_back(instrs[i + 1]);  // the TXA, now after the branch
      i += 3;
      changed = true;
      continue;
    }
    out.push_back(instrs[i]);
    ++i;
  }
  if (changed) {
    fn.instructions = std::move(out);
  }
}

}  // namespace

asm_ast::Program apply_transfer_past_branch(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
