// Folds `TXA; STA M` into `STX M` (and the Y variant) when it is safe.
//
// The pattern
//
// Two consecutive instructions:
//
//     Mov(Reg(X), Reg(A))     // TXA, so A := X and N/Z are set from X
//     Mov(Reg(A), <Data|ZP>)  // STA M
//
// become `Mov(Reg(X), <Data|ZP>)` (STX M). The shape is the same for Y
// (`TYA; STA M` becomes `STY M`).
//
// `STX` takes the `zp`, `zp,Y`, and `abs` addressing modes, and `STY`
// takes `zp`, `zp,X`, and `abs`. A plain `Data(name)` or `ZP(addr)`
// operand resolves to `zp` or `abs` at link time, so both are valid
// targets. `Frame`, `Stack`, and the `Indirect*` operands use
// indirect-Y, which STX and STY don't have, so they stay.
//
// Soundness
//
// `TXA` reads X and writes A, setting N and Z from X. `STA M` writes A
// to M and leaves the flags. At the end A = X, M = X, and N/Z come from
// X.
//
// `STX M` writes X to M and leaves the flags. At the end A is unchanged,
// M = X, and the flags are those from before the TXA.
//
// The rewrite differs from the original in two ways:
//
//   1. A's value at the end. The original has A = X, and the rewrite
//      has the value A had before the TXA. That is sound when A is dead
//      at the next instruction.
//   2. The flags at the end. The original has N/Z set from X, and the
//      rewrite leaves them as they were. That is sound when the flags
//      are dead at the next instruction, with no branch on N or Z
//      before another instruction sets them.
//
// Both checks use the liveness helpers (`a_dead_at` / `flags_dead_at`).
//
// A copy while A is busy
//
// `apply_copy_through_index`, which copies a byte through X or Y while
// A holds a value it needs again (`STA t; LDA M; STA D; LDA t` as `LDX
// M; STX D`), is in the same Python module. It runs once, after the
// last peephole run, `carry_known`, and the jump tables, and isn't
// ported yet.
//
// Where this hits
//
// The pattern arises after `split_mem_to_mem` splits a move from memory
// to memory `Mov(Data(__local_<counter>), Data(__zpabi_*))` and
// `x_save_slot_load` rewrites the LDA half (`Mov(M, Reg(A))`) into
// `Mov(Reg(X), Reg(A))` (TXA). Before the split, the shape went through
// the case of `x_save_slot_load`'s third pass for a move from memory to
// memory (`Mov(M, D)` with D a Data or ZP byte became `Mov(Reg(X), D)`)
// and was written as an STX directly. After the split, the LDA and STA
// survive as two atoms, and this peephole collapses them again.
//
// The same shape can come from `TAX; STA M` and `TAY; STA M` chains by
// other paths, register transfers followed by a store of the same value
// through A. Taking every X or Y source catches those too.
//
// Where it runs
//
// Inside the peephole fixed point, after `apply_x_save_slot_load` has
// had a chance to turn `Mov(M, Reg(A))` into `Mov(Reg(X), Reg(A))`, and
// before `expand_long_branches`. The fold makes each occurrence a byte
// shorter (STX abs is 3 bytes, and TXA and STA abs are 1 + 3 = 4), so no
// long branch's displacement grows.
#include "passes/via_a_store_fold.h"

#include <utility>
#include <vector>

#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

// Returns the source register when `instr` is `Mov(Reg(X), Reg(A))` or
// `Mov(Reg(Y), Reg(A))`, a TXA or TYA that isn't volatile, or null.
const ir::Type_reg* xy_to_a_transfer(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  if (m == nullptr || m->is_volatile || !is_reg_a(m->dst)) {
    return nullptr;
  }
  const auto* r = as<ir::Reg>(m->src);
  if (r == nullptr || !isa<ir::X, ir::Y>(r->reg)) {
    return nullptr;
  }
  return &r->reg;
}

// Returns whether `instr` is `Mov(Reg(A), <Data|ZP>)`, a STA whose
// destination's addressing mode STX and STY take too.
bool is_a_store_to_addressable(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  return m != nullptr && !m->is_volatile && is_reg_a(m->src) &&
         isa<ir::Data, ir::ZP>(m->dst);
}

void rewrite_function(ir::Function& fn) {
  const Instrs instrs = fn.instructions;
  const LabelMap labels(instrs);
  std::vector<ir::Type_instruction> out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    if (i + 1 < instrs.size()) {
      const ir::Type_reg* src = xy_to_a_transfer(instrs[i]);
      if (src != nullptr && is_a_store_to_addressable(instrs[i + 1]) &&
          a_dead_at(instrs, labels, i + 2) && flags_dead_at(instrs, labels, i + 2)) {
        out.push_back(ir::Mov{.src = ir::Reg{.reg = *src},
                              .dst = std::get<ir::Mov>(instrs[i + 1]).dst,
                              .is_volatile = false});
        i += 2;
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

asm_ast::Program apply_via_a_store_fold(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
