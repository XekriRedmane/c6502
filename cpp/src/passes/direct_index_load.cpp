// Loads a value straight into X or Y.
//
// `tac_to_asm` always puts a value in `Reg(X)` or `Reg(Y)` through
// `Reg(A)`:
//
//     Mov(M, Reg(A))            ; LDA M
//     Mov(Reg(A), Reg(X))       ; TAX
//
// That is the safe choice when it lowers, since `M` is still a `Pseudo`
// and could become a `Frame`, `Stack`, or `Indirect` operand, which use
// indirect-Y addressing. `LDX` and `LDY` have no `(ind),Y` mode, so a
// direct load wouldn't work for those.
//
// Once `replace_pseudoregisters` has given the operands their concrete
// kinds, the round trip through A can go when `LDX` / `LDY` can address
// `M` directly:
//
//   * `Imm`, as `LDX #imm` (2 bytes, as `LDA #imm; TAX` is, without the
//     TAX's cycles),
//   * `Data`, as `LDX abs` (3 bytes and 4 cycles against 4 bytes and 6
//     cycles for `LDA abs; TAX`),
//   * `ZP`, as `LDX zp` (2 bytes and 3 cycles against 3 bytes and 5
//     cycles), and
//   * `IndexedData` indexed by the other register, as `LDX abs,Y` or
//     `LDY abs,X` (3 bytes against 4 for `LDA abs,Y; TAX`). The 6502 has
//     no `LDX abs,X` or `LDY abs,Y`, so a load indexed by the register
//     it writes stays.
//
// Each rewrite saves a byte and 2 cycles.
//
// Eligibility
//
// The fusion applies when
//
//   * two consecutive instructions are
//     `Mov(src=M, dst=Reg(A)); Mov(src=Reg(A), dst=Reg(X|Y))`,
//   * `M` is a `Data`, `ZP`, or `Imm` operand, or an `IndexedData`
//     indexed by the other register, which are the modes `LDX` / `LDY`
//     have, and
//   * A is dead right after the second `Mov`, since the rewrite loses
//     the load into A.
//
// The flags
//
// `LDA M; TAX` sets N and Z twice, from M's value at the LDA and again
// from the value transferred, which is M's value. `LDX M` sets them from
// M's value too, so every later `Branch` sees the same flags.
//
// Where it runs
//
// After `replace_pseudoregisters`, which resolves the `Pseudo`s so that
// `Data` and `ZP` can be recognized, and before `expand_long_branches`.
// The pass only shrinks code and adds no branch, so its order with that
// pass doesn't matter, and it goes before to match `inc_peephole`.
#include "passes/direct_index_load.h"

#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

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

// Returns the fused `Mov(M, Reg(X|Y))` when `first` is `Mov(M, Reg(A))`
// with M a `Data`, `ZP`, or immediate operand, or an `IndexedData` by
// the register `second` doesn't write, `second` is
// `Mov(Reg(A), Reg(X|Y))`, and A is dead at `after_idx`. Returns
// nothing otherwise.
std::optional<ir::Type_instruction> try_fuse(const ir::Type_instruction& first,
                                             const ir::Type_instruction& second,
                                             Instrs instrs, const LabelMap& labels,
                                             std::size_t after_idx) {
  const auto* load = as<ir::Mov>(first);
  if (load == nullptr || !is_reg_a(load->dst)) {
    return std::nullopt;
  }
  const auto* indexed = as<ir::IndexedData>(load->src);
  if (indexed == nullptr &&
      !isa<ir::Data, ir::ZP, ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(load->src)) {
    return std::nullopt;
  }
  const auto* transfer = as<ir::Mov>(second);
  // The second Mov is a transfer between registers (TAX / TAY), which
  // is never volatile. Only `first` carries the bit for a memory access.
  if (transfer == nullptr || transfer->is_volatile || !is_reg_a(transfer->src) ||
      !is_reg_x_or_y(transfer->dst)) {
    return std::nullopt;
  }
  // `LDX abs,Y` and `LDY abs,X` exist, and `LDX abs,X` doesn't.
  if (indexed != nullptr &&
      indexed->index.index() == std::get<ir::Reg>(transfer->dst).reg.index()) {
    return std::nullopt;
  }
  if (!a_dead_at(instrs, labels, after_idx)) {
    return std::nullopt;
  }
  // The fused `LDX M` is the same access to memory as the `LDA M`, so
  // it keeps the first Mov's volatile bit.
  return ir::Mov{.src = load->src, .dst = transfer->dst, .is_volatile = load->is_volatile};
}

// Rewrites each fusable pair, in one walk that records the rewrites and
// a second that builds the instructions.
void rewrite_function(ir::Function& fn) {
  auto& instrs = fn.instructions;
  if (instrs.size() < 2) {
    return;
  }
  const LabelMap labels(instrs);
  std::vector<std::optional<ir::Type_instruction>> rewrites(instrs.size());
  std::vector<bool> skipped(instrs.size(), false);
  bool any = false;
  for (std::size_t i = 0; i + 1 < instrs.size(); ++i) {
    if (skipped[i]) {
      continue;
    }
    if (auto fused = try_fuse(instrs[i], instrs[i + 1], instrs, labels, i + 2)) {
      rewrites[i] = std::move(fused);
      skipped[i + 1] = true;
      any = true;
    }
  }
  if (!any) {
    return;
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (skipped[i]) {
      continue;
    }
    out.push_back(rewrites[i] ? std::move(*rewrites[i]) : std::move(instrs[i]));
  }
  instrs = std::move(out);
}

}  // namespace

asm_ast::Program apply_direct_index_load(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
