// Copies a byte through X or Y while A is busy.
//
// A byte copied from one place to another while A holds a value it
// needs again goes through X (or Y) instead of parking A in a slot:
//
//     STA   t                     LDX   M
//     LDA   M            ->       STX   D
//     STA   D
//     LDA   t
//
// when `t` is one of the function's own `__local_*` slots that nothing
// reads after the `LDA t`, `M` is a byte or a constant `LDX` can load
// and `D` one `STX` can store, X is dead from the `STA t` on (else Y,
// the same way), and N and Z are dead after, since they come from `M`
// where the `LDA t` set them from A. A ends with the value it had.
// Zork's `do_var` (examples/zork/decode.c) copies `OPERANDS[0]` into a
// slot on entry while A holds the opcode it dispatches on. It saves 4
// bytes with the bytes in zero page.
//
// The copies between the `STA t` and the `LDA t` may be several, and
// each may be a load and any number of stores, or a memory-to-memory
// `Mov(M, D)`, which `asm_emit` writes as `LDA M; STA D`. A constant
// already in the register isn't loaded again. Ali Baba's
// `print_bottom_centered` (examples/ali_baba/text.c) gets its pointer
// in A and X and stores two constants before passing it on, so X is
// read by the tail call and Y carries them:
//
//     STA   t                     LDY   #$17
//     LDA   #$17                  STY   TEXT_ROW
//     STA   TEXT_ROW      ->      LDY   #$00
//     LDA   #$00                  STY   TEXT_COL
//     STA   TEXT_COL              JMP   print_centered_plus_col
//     LDA   t
//     JMP   print_centered_plus_col
//
// The conditions are the ones above, for every copy: nothing in the
// run reads or writes `t`, every load is a byte or a constant the
// register can load and every store a `Data` or `ZP` byte, X (or Y) is
// dead from the `STA t` on, which also means no copy reads it, and N
// and Z are dead after the `LDA t`, since the last load sets them. C
// and V are unchanged both ways. A store before the run's first load
// stores A, which holds the parked value both ways. A volatile load or
// store stays one access of the same byte in the same order, through
// the other register.
//
// `apply_copy_through_index` runs once, after the last peephole run,
// `carry_known`, and the jump tables. Before the jump table, `do_var`'s
// dispatch starts with a `BEQ` that reads the Z from `LDA t`. Inside
// the fixed point it took the `LDA` of a copy before the folds that
// remove such copies outright had run, and `branch_to_offset` came out
// 6 bytes longer.
#include "passes/copy_through_index.h"

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

using Instructions = std::vector<ir::Type_instruction>;

bool is_reg_a(const ir::Type_operand& op) {
  const auto* reg = as<ir::Reg>(op);
  return reg != nullptr && isa<ir::A>(reg->reg);
}

bool index_loadable(const ir::Type_operand& op) {
  return isa<ir::Data, ir::ZP, ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(op);
}

bool index_storable(const ir::Type_operand& op) {
  return isa<ir::Data, ir::ZP>(op);
}

bool is_constant(const ir::Type_operand& op) {
  return isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(op);
}

// Returns the index of the `LDA t` that ends a run of copies starting
// at `instrs[start]`, or nothing when the instructions from there
// aren't such a run. The run holds loads into A of a byte or a constant,
// stores of A into `Data` or `ZP`, and copies from one to the other
// (`Mov(M, D)`, which `asm_emit` writes as `LDA M; STA D`), none of
// them reading or writing `t`, with at least one load or copy.
std::optional<std::size_t> copies_end(const Instructions& instrs,
                                      std::size_t start,
                                      const ir::Type_operand& t) {
  int loads = 0;
  for (std::size_t k = start; k < instrs.size(); ++k) {
    const auto* mov = as<ir::Mov>(instrs[k]);
    if (mov == nullptr || mov->src == t || mov->dst == t) {
      if (mov != nullptr && mov->src == t && is_reg_a(mov->dst) &&
          !mov->is_volatile && loads != 0) {
        return k;
      }
      return std::nullopt;
    }
    if (is_reg_a(mov->dst)) {
      if (!index_loadable(mov->src)) {
        return std::nullopt;
      }
      loads += 1;
    } else if (is_reg_a(mov->src)) {
      if (!index_storable(mov->dst)) {
        return std::nullopt;
      }
    } else if (index_loadable(mov->src) && index_storable(mov->dst) &&
               !mov->is_volatile) {
      loads += 1;
    } else {
      return std::nullopt;
    }
  }
  return std::nullopt;
}

// Returns the copies of `body` through `reg`. A store before the first
// load stores A, which still holds the parked value. A constant already
// in `reg` isn't loaded again.
Instructions through_index(const Instructions& body, std::size_t begin,
                           std::size_t end, const ir::Type_operand& reg) {
  Instructions out;
  // What `reg` holds, nothing before the first load.
  std::optional<ir::Type_operand> held;
  for (std::size_t k = begin; k < end; ++k) {
    const auto& ins = std::get<ir::Mov>(body[k]);
    if (is_reg_a(ins.dst) || !is_reg_a(ins.src)) {
      if (!(is_constant(ins.src) && held && ins.src == *held)) {
        out.push_back(ir::Mov{.src = ins.src, .dst = reg, .is_volatile = ins.is_volatile});
      }
      held = ins.src;
    }
    if (!is_reg_a(ins.dst)) {
      out.push_back(ir::Mov{
          .src = held ? reg : ir::Type_operand{ir::Reg{.reg = ir::A{}}},
          .dst = ins.dst,
          .is_volatile = ins.is_volatile});
    }
  }
  return out;
}

// Rewrites `STA t; LDA M; STA D; ...; LDA t` as `LDX M; STX D; ...` (or
// `LDY` / `STY`) in one function.
void copy_through_index_fn(ir::Function& fn, const AsmContext& ctx,
                           const SymbolAddrs* zp_slot_symbols) {
  Instructions instrs = fn.instructions;
  const std::string prefix = "__local_" + fn.name + "__";
  bool changed = false;
  std::size_t i = 0;
  while (i + 3 < instrs.size()) {
    const auto* park = as<ir::Mov>(instrs[i]);
    const ir::Data* t =
        park != nullptr ? as<ir::Data>(park->dst) : nullptr;
    if (!(park != nullptr && !park->is_volatile && is_reg_a(park->src) &&
          t != nullptr && t->name.starts_with(prefix))) {
      i += 1;
      continue;
    }
    const ir::Type_operand target = park->dst;
    const auto back = copies_end(instrs, i + 1, target);
    if (!back) {
      i += 1;
      continue;
    }
    const std::size_t after = *back + 1;
    if (!(flags_dead_at(instrs, after) &&
          asm_dead_store::mem_dead_after(instrs, *back, target, ctx, fn.name,
                                         zp_slot_symbols))) {
      i += 1;
      continue;
    }
    ir::Type_operand reg;
    if (x_dead_at(instrs, i)) {
      reg = ir::Reg{.reg = ir::X{}};
    } else if (y_dead_at(instrs, i)) {
      reg = ir::Reg{.reg = ir::Y{}};
    } else {
      i += 1;
      continue;
    }
    Instructions copies = through_index(instrs, i + 1, *back, reg);
    instrs.erase(instrs.begin() + static_cast<long>(i),
                 instrs.begin() + static_cast<long>(after));
    instrs.insert(instrs.begin() + static_cast<long>(i), copies.begin(),
                  copies.end());
    changed = true;
    // Python moves on by `back - i - 1`, which lands on `back - 1`.
    i = *back - 1;
  }
  if (changed) {
    fn.instructions = std::move(instrs);
  }
}

}  // namespace

asm_ast::Program apply_copy_through_index(asm_ast::Program prog,
                                          const AsmContext& ctx,
                                          const SymbolAddrs* zp_slot_symbols) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      copy_through_index_fn(*fn, ctx, zp_slot_symbols);
    }
  }
  return prog;
}

}  // namespace c6502::passes
