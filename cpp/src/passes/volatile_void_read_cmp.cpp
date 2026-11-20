// Converts `Mov(<indirect>, Reg(A), is_volatile=true)` to
// `Compare(Reg(A), <indirect>)` when A's new value is dead and the
// flags from the load aren't read.
//
// The shape that called for it is `(void)*sfx_click_ptr`, a volatile
// read whose value is discarded. `tac_to_asm` lowers it to
// `Mov(<indirect>, Reg(A))`, which is emitted as `LDA (DPTR),Y`, and
// the LDA overwrites A.
//
// `CMP (DPTR),Y` performs the same access to memory. A CMP also reads
// the byte at `(DPTR),Y`, but it only sets N, Z, and C from `A -
// <byte>`, and A is unchanged. So when
//
//   * the new value of A is dead (nothing reads A before the next write
//     of A), and
//   * the flags from the load aren't read (the next instruction
//     overwrites N/Z, or doesn't read the flags),
//
// the LDA can become a CMP, which keeps A's earlier value across the
// volatile access. Keeping a parameter pinned to A live across a
// volatile click depends on this, since without it the click destroys
// the pinned register.
//
// Soundness
//
//   * Memory. LDA and CMP perform the same read cycle of the 6502 from
//     `(DPTR),Y`, so the volatile access is the same.
//   * A. LDA writes A, and CMP doesn't. With A dead after it, the new
//     value isn't read in either form. With A live before it, CMP keeps
//     the earlier value, and extending a value's life is always safe.
//   * The flags. LDA sets N/Z from the byte loaded, and CMP sets N, Z,
//     and C from `A - byte`, which differ when A isn't the byte. The
//     condition that N/Z and C are dead after the load makes the
//     difference invisible.
//
// The conditions come from `asm_liveness`: A, N/Z, and C all have to be
// dead after the load. An earlier condition written by hand accepted any
// following instruction that writes A and the flags, including `AND`,
// `ORA`, `ADC`, and `CMP`, which also read A. That turned `while (!(*p &
// 0x80)) ;` into a loop that tested the stale A in place of the byte it
// read.
//
// Where it runs
//
// In the asm peephole fixed point, after `replace_pseudoregisters` has
// resolved the Pseudos. The rewrite turns one atom into one atom of the
// same size, and lets the pinning of parameters keep A live across the
// read, which leads to more simplifications earlier in the code.
#include "passes/volatile_void_read_cmp.h"

#include <cstddef>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

// Returns whether `instr` is `Mov(<indirect>, Reg(A),
// is_volatile=true)`, a volatile indirect read into A.
bool is_candidate(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  return m != nullptr && m->is_volatile &&
         isa<ir::Indirect, ir::IndirectY, ir::IndirectZp, ir::IndirectZpY>(m->src) &&
         is_reg_a(m->dst);
}

void rewrite_function(ir::Function& fn) {
  auto& instrs = fn.instructions;
  bool any = false;
  for (const auto& instr : instrs) {
    any = any || is_candidate(instr);
  }
  if (!any) {
    return;
  }
  // The liveness questions are asked of the function as it was, so the
  // rewrites go in a copy.
  std::vector<ir::Type_instruction> out = instrs;
  const LabelMap labels(instrs);
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (!is_candidate(instrs[i])) {
      continue;
    }
    // The value loaded and the flags it sets must go unread. CMP also
    // writes C, which LDA leaves alone, so C has to be dead too. A
    // carry chain (`ADC; LDA vol; LDA x; ADC`) or a carry-flag return
    // can read the C from before the load.
    if (!(a_dead_at(instrs, labels, i + 1) && flags_dead_at(instrs, labels, i + 1) &&
          c_dead_at(instrs, labels, i + 1))) {
      continue;
    }
    const auto& m = std::get<ir::Mov>(instrs[i]);
    out[i] = ir::Compare{.left = m.dst, .right = m.src};
  }
  instrs = std::move(out);
}

}  // namespace

asm_ast::Program apply_volatile_void_read_cmp(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
