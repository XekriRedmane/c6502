// Compares X or Y directly, with CPX or CPY.
//
// `tac_to_asm`'s lowerings of a comparison put every left operand in
// `Reg(A)` before the `Compare`, because a comparison with a `Stack`,
// `Frame`, or `Indirect` operand needs `CMP (zp),Y`, which only `CMP`
// has (`CPX` and `CPY` have no indirect-Y mode). When the left operand
// is already in `Reg(X)` or `Reg(Y)`, as in the usual test of a loop's
// counter,
//
//     Mov(Reg(X), Reg(A))         ; TXA
//     Compare(Reg(A), R)          ; CMP imm/abs/zp
//     Branch(cond, target)        ; BCC / BCS / BEQ / BNE / ...
//
// the round trip through A isn't needed when `CPX` can address `R`
// (immediate, Data, ZP) and nothing reads A after the Compare. The pass
// then rewrites it as
//
//     Compare(Reg(X), R)          ; CPX imm/abs/zp
//     Branch(cond, target)
//
// which saves a byte and 2 cycles each time. `TYA; CMP; ...` becomes
// `CPY ...` the same way.
//
// The flags
//
// `TXA; CMP R` sets N and Z from X at the TXA and then sets N, Z, and C
// from the comparison at the CMP. The flags that are left are those of
// comparing X with R, which is what `CPX R` gives, so every later
// `Branch` sees the same condition.
//
// A
//
// After the rewrite, A keeps the value it had, where the TXA would have
// overwritten it with X's. So A has to be dead after the Compare, with
// nothing reading it before it is written again. `asm_liveness.a_dead_at`
// walks forward with the same rules `direct_index_load` and
// `backward_copy_propagation` use.
//
// The right operand
//
// `CPX` and `CPY` have immediate, absolute, and zero-page modes, so an
// `Imm`, `Data`, or `ZP` operand works. `Stack`, `Frame`, `Indirect`, and
// `IndirectY` need indirect-Y addressing, which CPX and CPY don't have,
// so those stay with `CMP` through A.
//
// Through a temporary
//
// The other shape puts the right operand in a byte of its own, when the
// register allocator gave the copy of the compared value one:
//
//     Mov(M, Reg(A))              ; LDA WNDWDTH
//     Mov(Reg(A), t)              ; STA t
//     Compare(Reg(X), t)          ; CPX t
//
// That becomes `Compare(Reg(X), M)` (`CPX WNDWDTH`) when `M` is an
// immediate, `Data`, or `ZP` operand (a volatile one is still read once,
// by the compare, where the load was), `t` is a `Data` or `ZP` byte that
// nothing reads after the compare (`asm_dead_store.mem_dead_after`), and
// A is dead after it. That's `buffer_char`'s `x < WNDWDTH` in
// examples/zork/output.c.
//
// Where it runs
//
// After `replace_pseudoregisters`, so the operands are concrete, and
// inside `_peephole_fixedpoint`. The pass only shrinks code.
#include "passes/cpx_cpy_peephole.h"

#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

#include "passes/asm_dead_store.h"
#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_reg_x_or_y(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::X, ir::Y>(r->reg);
}

// Returns the rewritten `Compare(Reg(X|Y), R)` when `first` is
// `Mov(Reg(X|Y), Reg(A))`, `second` is `Compare(Reg(A), R)` with R an
// `Imm`, `Data`, or `ZP` operand, and A is dead at `after_idx`. Returns
// nothing otherwise.
std::optional<ir::Type_instruction> try_fuse(const ir::Type_instruction& first,
                                             const ir::Type_instruction& second,
                                             Instrs instrs, const LabelMap& labels,
                                             std::size_t after_idx) {
  const auto* transfer = as<ir::Mov>(first);
  if (transfer == nullptr || !is_reg_a(transfer->dst) || !is_reg_x_or_y(transfer->src)) {
    return std::nullopt;
  }
  const auto* cmp = as<ir::Compare>(second);
  if (cmp == nullptr || !is_reg_a(cmp->left) ||
      !isa<ir::Imm, ir::Data, ir::ZP>(cmp->right)) {
    return std::nullopt;
  }
  if (!a_dead_at(instrs, labels, after_idx)) {
    return std::nullopt;
  }
  return ir::Compare{.left = transfer->src, .right = cmp->right};
}

// Returns `Compare(Reg(X|Y), M)` when `instrs[i:i+3]` is
// `Mov(M, Reg(A)); Mov(Reg(A), t); Compare(Reg(X|Y), t)` and the
// conditions of "Through a temporary" hold. Returns nothing otherwise.
std::optional<ir::Type_instruction> try_fuse_through_tmp(const ir::Function& fn,
                                                         Instrs instrs,
                                                         const LabelMap& labels,
                                                         std::size_t i,
                                                         const AsmContext& ctx) {
  const auto* load = as<ir::Mov>(instrs[i]);
  const auto* store = as<ir::Mov>(instrs[i + 1]);
  const auto* cmp = as<ir::Compare>(instrs[i + 2]);
  // A volatile `M` is still read once, where the compare is.
  if (!(load != nullptr && is_reg_a(load->dst) &&
        isa<ir::Imm, ir::Data, ir::ZP>(load->src))) {
    return std::nullopt;
  }
  if (!(store != nullptr && is_reg_a(store->src) && !store->is_volatile &&
        isa<ir::Data, ir::ZP>(store->dst))) {
    return std::nullopt;
  }
  if (!(cmp != nullptr && is_reg_x_or_y(cmp->left) && cmp->right == store->dst)) {
    return std::nullopt;
  }
  if (load->src == store->dst) {
    return std::nullopt;
  }
  if (!a_dead_at(instrs, labels, i + 3)) {
    return std::nullopt;
  }
  if (!asm_dead_store::mem_dead_after(instrs, labels, i + 2, store->dst, ctx, fn.name)) {
    return std::nullopt;
  }
  return ir::Compare{.left = cmp->left, .right = load->src};
}

// Records each fusion at the index of its Compare in one walk, and in a
// second builds the instructions, leaving out those the fusion replaces
// (the TXA / TYA, or the load and store of a temporary) and replacing
// the Compare.
void rewrite_function(ir::Function& fn, const AsmContext& ctx) {
  auto& instrs = fn.instructions;
  if (instrs.size() < 2) {
    return;
  }
  const LabelMap labels(instrs);
  std::vector<std::optional<ir::Type_instruction>> rewrites(instrs.size());
  std::vector<bool> drop(instrs.size(), false);
  bool any = false;
  for (std::size_t i = 0; i + 1 < instrs.size(); ++i) {
    if (drop[i] || rewrites[i]) {
      continue;
    }
    if (i + 2 < instrs.size()) {
      if (auto fused = try_fuse_through_tmp(fn, instrs, labels, i, ctx)) {
        drop[i] = true;
        drop[i + 1] = true;
        rewrites[i + 2] = std::move(fused);
        any = true;
        continue;
      }
    }
    if (auto fused = try_fuse(instrs[i], instrs[i + 1], instrs, labels, i + 2)) {
      drop[i] = true;
      rewrites[i + 1] = std::move(fused);
      any = true;
    }
  }
  if (!any) {
    return;
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (drop[i]) {
      continue;
    }
    out.push_back(rewrites[i] ? std::move(*rewrites[i]) : std::move(instrs[i]));
  }
  instrs = std::move(out);
}

}  // namespace

asm_ast::Program apply_cpx_cpy_peephole(asm_ast::Program prog, const AsmContext& ctx) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, ctx);
    }
  }
  return prog;
}

}  // namespace c6502::passes
