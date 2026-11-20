// Loads a loop's one constant in A once, before the loop.
//
// In a loop where every write of A is `LDA #k` of one constant `k`,
// except for `TXA` / `TYA` transfers whose value nothing reads, A holds
// `k` wherever the loop reads it once the transfers are gone. The pass
// removes the loads and the transfers and puts one `LDA #k` in front of
// the loop's header.
//
// The transfers are what a test of an index register leaves, as in the
// loop that clears an automatic aggregate's bytes
// (`c99_to_tac._emit_zero_bytes`):
//
//     LDY #n                    LDY #n
//     .clear:                   LDA #0
//     DEY                       .clear:
//     LDA #0          ->        DEY
//     STA v,Y                   STA v,Y
//     TYA                       BNE .clear
//     BNE .clear
//
// `dead_a_arith` drops a `TYA` whose flags `DEY` has already set, but
// here the `LDA #0` sets the flags between them, and the `LDA #0` can't
// leave the loop while the `TYA` writes A. So this pass takes the two
// steps together. The loop is 4 cycles a byte shorter and a byte
// smaller.
//
// What the loop must look like
//
// The loop runs from its header label to a branch or jump back to it,
// which comes later in the code, and nothing outside it branches to the
// header or into it. The instruction before the header doesn't end its
// block, so the code before the loop falls into the new `LDA #k`. Every
// instruction in the loop is one of these.
//
//   * `LDA #k`, all with the same `k`, where N and Z are dead after
//     it, since the load is gone.
//   * `TXA` or `TYA`, where A is dead after it and N and Z are dead or
//     already set from the register (`dead_a_arith::flags_reflect_src_reg`,
//     asked of the code without the loads).
//   * An instruction that leaves A alone and that `asm_emit` writes
//     without going through A (`leaves_a`).
//
// A is dead at the header, so nothing in the loop reads the value A held
// before it, and N and Z are dead there, since the new `LDA #k` sets
// them.
//
// Why the values stay the same
//
// After the change A holds `k` from the header on. A read of A in the
// loop or after it got its value from one of the loads, since A is dead
// after each transfer and at the header, and that value was `k`. The
// flags are unchanged wherever they're read. A removed load's N and Z
// are dead, a removed transfer's are what the earlier instruction set
// from the same register, and the new load's are dead at the header.
#include "passes/loop_constant_a.h"

#include <algorithm>
#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

#include "passes/asm_licm.h"
#include "passes/asm_liveness.h"
#include "passes/dead_a_arith.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using InstrVec = std::vector<ir::Type_instruction>;

// Returns whether `ins` is `LDA #k`, of a number or a label's half.
bool is_constant_load(const ir::Type_instruction& ins) {
  const auto* mov = as<ir::Mov>(ins);
  return mov != nullptr && is_reg_a(mov->dst) &&
         isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(mov->src);
}

bool is_xy(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::X, ir::Y>(r->reg);
}

// Returns whether `ins` is `TXA` or `TYA`.
bool is_transfer(const ir::Type_instruction& ins) {
  const auto* mov = as<ir::Mov>(ins);
  return mov != nullptr && is_reg_a(mov->dst) && is_xy(mov->src);
}

// Operands that `LDX`, `LDY`, `STX`, `STY`, `INC`, `DEC`, and the
// shifts take directly, with no `LDY #off` and no trip through A.
bool is_direct(const ir::Type_operand& op) { return isa<ir::Data, ir::ZP>(op); }

bool is_direct_or_imm(const ir::Type_operand& op) {
  return isa<ir::Data, ir::ZP, ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(op);
}

// Returns whether `INC`, `DEC`, and the shifts take `op` directly: a
// byte, or a byte indexed by X (there's no `INC abs,Y`).
bool read_modify_write(const ir::Type_operand& op) {
  if (const auto* indexed = as<ir::IndexedData>(op)) {
    return isa<ir::X>(indexed->index);
  }
  return is_direct(op);
}

// Returns whether `ins` doesn't write A, as `asm_emit` writes it. Kinds
// not listed here, such as calls, pulls, and the compound nodes, count
// as writing A.
bool leaves_a(const ir::Type_instruction& ins) {
  switch (ir::kind(ins)) {
    case ir::Kind_instruction::Label:
    case ir::Kind_instruction::Branch:
    case ir::Kind_instruction::SetCarry:
    case ir::Kind_instruction::ClearCarry:
    case ir::Kind_instruction::BitTest:
      return true;
    case ir::Kind_instruction::Jump:
      return std::get<ir::Jump>(ins).reg_args.empty();
    case ir::Kind_instruction::Compare:
      return is_reg_a(std::get<ir::Compare>(ins).left);
    case ir::Kind_instruction::Push:
      return is_reg_a(std::get<ir::Push>(ins).src);
    case ir::Kind_instruction::Mov: {
      const auto& mov = std::get<ir::Mov>(ins);
      if (is_reg_a(mov.src)) {
        // STA, TAX, TAY.
        return !is_reg_a(mov.dst);
      }
      if (is_xy(mov.dst)) {
        // LDX, LDY.
        return is_direct_or_imm(mov.src) || isa<ir::IndexedData>(mov.src);
      }
      if (is_xy(mov.src)) {
        // STX, STY.
        return is_direct(mov.dst);
      }
      return false;
    }
    case ir::Kind_instruction::Inc:
      return is_xy(std::get<ir::Inc>(ins).dst) ||
             read_modify_write(std::get<ir::Inc>(ins).dst);
    case ir::Kind_instruction::Dec:
      return is_xy(std::get<ir::Dec>(ins).dst) ||
             read_modify_write(std::get<ir::Dec>(ins).dst);
    case ir::Kind_instruction::ArithmeticShiftLeft:
      return read_modify_write(std::get<ir::ArithmeticShiftLeft>(ins).dst);
    case ir::Kind_instruction::LogicalShiftRight:
      return read_modify_write(std::get<ir::LogicalShiftRight>(ins).dst);
    case ir::Kind_instruction::RotateLeft:
      return read_modify_write(std::get<ir::RotateLeft>(ins).dst);
    case ir::Kind_instruction::RotateRight:
      return read_modify_write(std::get<ir::RotateRight>(ins).dst);
    default:
      return false;
  }
}

std::optional<InstrVec> rewrite_loop(const InstrVec& instrs, const LabelMap& labels,
                                     std::size_t h, std::size_t b) {
  const std::string& header = std::get<ir::Label>(instrs[h]).name;
  if (!asm_licm::is_single_entry(instrs, h, b, header)) {
    return std::nullopt;
  }
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (k >= h && k <= b) {
      continue;
    }
    const auto target = asm_licm::branch_or_jump_target(instrs[k]);
    if (target && *target == header) {
      return std::nullopt;
    }
  }
  if (h > 0 && isa<ir::Jump, ir::Ret, ir::Return, ir::JumpTable>(instrs[h - 1])) {
    return std::nullopt;
  }
  std::vector<std::size_t> loads;
  std::vector<std::size_t> transfers;
  std::optional<ir::Type_operand> value;
  for (std::size_t k = h; k <= b; ++k) {
    const auto& ins = instrs[k];
    if (is_constant_load(ins)) {
      const auto& src = std::get<ir::Mov>(ins).src;
      if (value && src != *value) {
        return std::nullopt;
      }
      value = src;
      loads.push_back(k);
    } else if (is_transfer(ins)) {
      transfers.push_back(k);
    } else if (!leaves_a(ins)) {
      return std::nullopt;
    }
  }
  if (loads.empty()) {
    return std::nullopt;
  }
  if (!(a_dead_at(instrs, labels, h) && flags_dead_at(instrs, labels, h))) {
    return std::nullopt;
  }
  for (std::size_t k : loads) {
    if (!flags_dead_at(instrs, labels, k + 1)) {
      return std::nullopt;
    }
  }
  const auto is_load = [&](std::size_t k) {
    return std::binary_search(loads.begin(), loads.end(), k);
  };
  InstrVec without_loads;
  without_loads.reserve(instrs.size());
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (!is_load(k)) {
      without_loads.push_back(instrs[k]);
    }
  }
  for (std::size_t k : transfers) {
    if (!a_dead_at(instrs, labels, k + 1)) {
      return std::nullopt;
    }
    if (flags_dead_at(instrs, labels, k + 1)) {
      continue;
    }
    const std::size_t shift = static_cast<std::size_t>(
        std::count_if(loads.begin(), loads.end(), [&](std::size_t j) { return j < k; }));
    if (!dead_a_arith::flags_reflect_src_reg(without_loads, k - shift)) {
      return std::nullopt;
    }
  }
  InstrVec out;
  out.reserve(instrs.size() + 1);
  out.insert(out.end(), instrs.begin(),
             instrs.begin() + static_cast<std::ptrdiff_t>(h));
  out.push_back(ir::Mov{.src = *value, .dst = ir::Reg{.reg = ir::A{}}});
  for (std::size_t k = h; k < instrs.size(); ++k) {
    if (is_load(k) ||
        std::find(transfers.begin(), transfers.end(), k) != transfers.end()) {
      continue;
    }
    out.push_back(instrs[k]);
  }
  return out;
}

// Rewrites the innermost loop that qualifies and returns the new
// instructions, or nothing when no loop does.
std::optional<InstrVec> one_loop(const InstrVec& instrs) {
  const LabelMap labels(instrs);
  std::vector<std::pair<std::size_t, std::size_t>> loops;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    const auto target = asm_licm::branch_or_jump_target(instrs[k]);
    if (!target) {
      continue;
    }
    const long h = labels.find(*target);
    if (h >= 0 && static_cast<std::size_t>(h) < k) {
      loops.emplace_back(static_cast<std::size_t>(h), k);
    }
  }
  std::stable_sort(loops.begin(), loops.end(), [](const auto& x, const auto& y) {
    return x.second - x.first < y.second - y.first;
  });
  for (const auto& [h, b] : loops) {
    if (auto out = rewrite_loop(instrs, labels, h, b)) {
      return out;
    }
  }
  return std::nullopt;
}

}  // namespace

asm_ast::Program apply_loop_constant_a(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    auto* fn = as<ir::Function>(tl);
    if (fn == nullptr) {
      continue;
    }
    while (auto changed = one_loop(fn->instructions)) {
      fn->instructions = std::move(*changed);
    }
  }
  return prog;
}

}  // namespace c6502::passes
