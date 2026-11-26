#include "passes/asm_liveness.h"

#include <algorithm>
#include <unordered_set>

#include "passes/asm_operands.h"
#include "passes/decimal_arith.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

// The register a walk follows.
enum class R { kA, kX, kY };

bool contains(const std::vector<std::string>& names, std::string_view name) {
  return std::find(names.begin(), names.end(), name) != names.end();
}

std::string_view reg_name(R reg) {
  switch (reg) {
    case R::kA:
      return "A";
    case R::kX:
      return "X";
    case R::kY:
      return "Y";
  }
  return "";
}

bool reads_reg(const ir::Type_instruction& instr, R reg) {
  switch (reg) {
    case R::kA:
      return reads_a(instr);
    case R::kX:
      return reads_x(instr);
    case R::kY:
      return reads_y(instr);
  }
  return false;
}

bool kills_reg(const ir::Type_instruction& instr, R reg) {
  switch (reg) {
    case R::kA:
      return kills_a(instr);
    case R::kX:
      return kills_x(instr);
    case R::kY:
      return kills_y(instr);
  }
  return false;
}

// Walks forward from `start` along the straight-line successors.
// Returns false when this path reads `reg` before a kill, and true when
// it ends without one (at a kill, at a `Ret` / `Return` that doesn't
// read the register, or at the end of the instructions). At a branch or
// jump it pushes the successors on `stack` and returns true, and the
// loop in `reg_dead_at` checks each one.
bool path_dead_from(Instrs instrs, std::size_t start, const LabelMap& labels,
                    std::vector<std::size_t>& stack, R reg) {
  std::size_t idx = start;
  while (idx < instrs.size()) {
    const ir::Type_instruction& instr = instrs[idx];
    if (const auto* ret = as<ir::Ret>(instr)) {
      if (reg == R::kA) {
        return !ret->save_a;
      }
      if (reg == R::kX) {
        return !ret->save_x;
      }
      return true;
    }
    if (const auto* ret = as<ir::Return>(instr)) {
      if (reg == R::kA) {
        return !ret->save_a;
      }
      if (reg == R::kX) {
        return !ret->save_x;
      }
      return true;
    }
    if (const auto* jump = as<ir::Jump>(instr)) {
      if (contains(jump->reg_args, reg_name(reg))) {
        return false;
      }
      const long tgt = labels.find(jump->target);
      if (tgt >= 0) {
        stack.push_back(static_cast<std::size_t>(tgt));
      }
      return true;
    }
    if (const auto* branch = as<ir::Branch>(instr)) {
      if (idx + 1 < instrs.size()) {
        stack.push_back(idx + 1);
      }
      const long tgt = labels.find(branch->target);
      if (tgt >= 0) {
        stack.push_back(static_cast<std::size_t>(tgt));
      }
      return true;
    }
    if (const auto* table = as<ir::JumpTable>(instr)) {
      if (reg == R::kA) {
        return false;
      }
      if (table->index == reg_name(reg)) {
        return true;
      }
      for (const auto& t : table->targets) {
        const long tgt = labels.find(t);
        if (tgt >= 0) {
          stack.push_back(static_cast<std::size_t>(tgt));
        }
      }
      return true;
    }
    if (isa<ir::Label>(instr)) {
      ++idx;
      continue;
    }
    if (reads_reg(instr, reg)) {
      return false;
    }
    if (kills_reg(instr, reg)) {
      return true;
    }
    ++idx;
  }
  return true;
}

bool reg_dead_at(Instrs instrs, const LabelMap& labels, std::size_t idx, R reg) {
  std::unordered_set<std::size_t> visited;
  std::vector<std::size_t> stack{idx};
  while (!stack.empty()) {
    const std::size_t j = stack.back();
    stack.pop_back();
    if (!visited.insert(j).second) {
      continue;
    }
    if (!path_dead_from(instrs, j, labels, stack, reg)) {
      return false;
    }
  }
  return true;
}

// Returns whether `op` uses the index register `reg` (X or Y) in its
// addressing mode.
bool operand_indexes_with(const ir::Type_operand& op, R reg) {
  if (const auto* d = as<ir::IndexedData>(op)) {
    return reg == R::kX ? isa<ir::X>(d->index) : isa<ir::Y>(d->index);
  }
  return reg == R::kY && isa<ir::IndirectY, ir::IndirectZpY>(op);
}

// Returns whether `op`'s addressing setup writes Y. `Indirect(off)` and
// `IndirectZp(addr, off)` are written with an `LDY #off` in front of
// the access.
bool operand_setup_clobbers_y(const ir::Type_operand& op) {
  return isa<ir::Indirect, ir::IndirectZp>(op);
}

bool is_reg(const ir::Type_operand& op, R reg) {
  switch (reg) {
    case R::kA:
      return is_reg_a(op);
    case R::kX:
      return is_reg_x(op);
    case R::kY:
      return is_reg_y(op);
  }
  return false;
}

bool reads_index_reg(const ir::Type_instruction& instr, R reg) {
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::Mov: {
      const auto& m = std::get<ir::Mov>(instr);
      return is_reg(m.src, reg) || operand_indexes_with(m.src, reg) ||
             operand_indexes_with(m.dst, reg);
    }
    case ir::Kind_instruction::Push:
      return is_reg(std::get<ir::Push>(instr).src, reg);
    case ir::Kind_instruction::Compare: {
      const auto& c = std::get<ir::Compare>(instr);
      return is_reg(c.left, reg) || is_reg(c.right, reg) ||
             operand_indexes_with(c.left, reg) ||
             operand_indexes_with(c.right, reg);
    }
    case ir::Kind_instruction::Inc: {
      const auto& dst = std::get<ir::Inc>(instr).dst;
      return is_reg(dst, reg) || operand_indexes_with(dst, reg);
    }
    case ir::Kind_instruction::Dec: {
      const auto& dst = std::get<ir::Dec>(instr).dst;
      return is_reg(dst, reg) || operand_indexes_with(dst, reg);
    }
    case ir::Kind_instruction::Add: {
      const auto& i = std::get<ir::Add>(instr);
      return operand_indexes_with(i.src, reg) || operand_indexes_with(i.dst, reg);
    }
    case ir::Kind_instruction::Sub: {
      const auto& i = std::get<ir::Sub>(instr);
      return operand_indexes_with(i.src, reg) || operand_indexes_with(i.dst, reg);
    }
    case ir::Kind_instruction::And: {
      const auto& i = std::get<ir::And>(instr);
      return operand_indexes_with(i.src, reg) || operand_indexes_with(i.dst, reg);
    }
    case ir::Kind_instruction::Or: {
      const auto& i = std::get<ir::Or>(instr);
      return operand_indexes_with(i.src, reg) || operand_indexes_with(i.dst, reg);
    }
    case ir::Kind_instruction::Xor: {
      const auto& i = std::get<ir::Xor>(instr);
      return operand_indexes_with(i.src1, reg) ||
             operand_indexes_with(i.src2, reg) || operand_indexes_with(i.dst, reg);
    }
    case ir::Kind_instruction::ArithmeticShiftLeft:
      return operand_indexes_with(std::get<ir::ArithmeticShiftLeft>(instr).dst, reg);
    case ir::Kind_instruction::LogicalShiftRight:
      return operand_indexes_with(std::get<ir::LogicalShiftRight>(instr).dst, reg);
    case ir::Kind_instruction::RotateLeft:
      return operand_indexes_with(std::get<ir::RotateLeft>(instr).dst, reg);
    case ir::Kind_instruction::RotateRight:
      return operand_indexes_with(std::get<ir::RotateRight>(instr).dst, reg);
    case ir::Kind_instruction::BitTest:
      return operand_indexes_with(std::get<ir::BitTest>(instr).src, reg);
    case ir::Kind_instruction::Phi:
      for (const auto& arg : std::get<ir::Phi>(instr).args) {
        if (is_reg(std::get<ir::AsmPhiArg>(arg).source, reg)) {
          return true;
        }
      }
      return false;
    case ir::Kind_instruction::Call:
      return contains(std::get<ir::Call>(instr).reg_args, reg_name(reg));
    case ir::Kind_instruction::Jump:
      return contains(std::get<ir::Jump>(instr).reg_args, reg_name(reg));
    default:
      return false;
  }
}

// Returns whether `instr` overwrites N, Z, and C.
bool overwrites_all_flags(const ir::Type_instruction& instr) {
  return isa<ir::SetCarry, ir::ClearCarry, ir::Compare, ir::ArithmeticShiftLeft,
             ir::LogicalShiftRight, ir::Call, ir::DecimalArith>(instr);
}

// The walk of `flags_dead_at` (`reads_c` false, N/Z only) and
// `all_flags_dead_at` (`reads_c` true, every flag). Returns false as
// soon as a path reaches a reader of the flags before an instruction
// that overwrites them.
bool flags_dead_walk(Instrs instrs, const LabelMap& labels, std::size_t idx,
                     bool reads_c) {
  std::unordered_set<std::size_t> visited;
  std::vector<std::size_t> stack{idx};
  while (!stack.empty()) {
    std::size_t i = stack.back();
    stack.pop_back();
    while (i < instrs.size()) {
      if (!visited.insert(i).second) {
        break;
      }
      const ir::Type_instruction& instr = instrs[i];
      if (const auto* branch = as<ir::Branch>(instr)) {
        if (reads_c || !isa<ir::CC, ir::CS, ir::VC, ir::VS>(branch->cond)) {
          return false;
        }
        const long tgt = labels.find(branch->target);
        if (tgt < 0) {
          return false;
        }
        stack.push_back(static_cast<std::size_t>(tgt));
        ++i;
        continue;
      }
      if (reads_c &&
          isa<ir::Add, ir::Sub, ir::RotateLeft, ir::RotateRight>(instr)) {
        return false;
      }
      if (const auto* ret = as<ir::Ret>(instr)) {
        if (reads_c && ret->save_c) {
          return false;
        }
        break;
      }
      if (const auto* ret = as<ir::Return>(instr)) {
        if (reads_c && ret->save_c) {
          return false;
        }
        break;
      }
      if (const auto* jump = as<ir::Jump>(instr)) {
        const long tgt = labels.find(jump->target);
        if (tgt >= 0) {
          stack.push_back(static_cast<std::size_t>(tgt));
        }
        break;
      }
      if (isa<ir::Label>(instr)) {
        ++i;
        continue;
      }
      if (reads_c) {
        if (overwrites_all_flags(instr)) {
          break;
        }
      } else if (sets_flags(instr)) {
        break;
      }
      ++i;
    }
  }
  return true;
}

// Which flag `one_flag_dead` follows.
enum class Flag { kC, kV };

// Returns whether `instr` reads the flag before it may write it. ADC,
// SBC, ROL, and ROR read C, and no instruction but a branch reads V.
bool flag_reader(const ir::Type_instruction& instr, Flag flag) {
  return flag == Flag::kC &&
         isa<ir::Add, ir::Sub, ir::RotateLeft, ir::RotateRight>(instr);
}

bool flag_branch(const ir::Type_condition& cond, Flag flag) {
  return flag == Flag::kC ? isa<ir::CC, ir::CS>(cond) : isa<ir::VC, ir::VS>(cond);
}

// The instructions that write the flag without reading it. A `Call`
// defines every flag.
bool flag_writer(const ir::Type_instruction& instr, Flag flag) {
  if (flag == Flag::kC) {
    return isa<ir::ClearCarry, ir::SetCarry, ir::Compare, ir::ArithmeticShiftLeft,
               ir::LogicalShiftRight, ir::Call, ir::DecimalArith>(instr);
  }
  // ADC and SBC write V without reading it, and so does BIT.
  return isa<ir::Add, ir::Sub, ir::BitTest, ir::Call, ir::DecimalArith>(instr);
}

// Returns whether the flag `flag` is dead at `instrs[idx]`, on every
// path forward until an instruction writes it. A `Return` or `Ret`
// ends a path with the flag dead, except that a carry-flag return reads
// C. The compound atoms that `asm_to_asm2` expands into arithmetic
// later (`LoadAddress`, `AllocateStack`, `FunctionPrologue`) are taken
// to read every flag.
bool one_flag_dead(Instrs instrs, const LabelMap& labels, std::size_t idx,
                   Flag flag) {
  std::unordered_set<std::size_t> visited;
  std::vector<std::size_t> stack{idx};
  while (!stack.empty()) {
    std::size_t i = stack.back();
    stack.pop_back();
    while (i < instrs.size() && visited.insert(i).second) {
      const ir::Type_instruction& x = instrs[i];
      if (flag_reader(x, flag) ||
          isa<ir::LoadAddress, ir::AllocateStack, ir::FunctionPrologue>(x)) {
        return false;
      }
      if (const auto* branch = as<ir::Branch>(x)) {
        if (flag_branch(branch->cond, flag)) {
          return false;
        }
        const long tgt = labels.find(branch->target);
        if (tgt < 0) {
          return false;
        }
        stack.push_back(static_cast<std::size_t>(tgt));
      }
      if (const auto* ret = as<ir::Return>(x)) {
        if (ret->save_c && flag == Flag::kC) {
          return false;
        }
        break;
      }
      if (const auto* ret = as<ir::Ret>(x)) {
        if (ret->save_c && flag == Flag::kC) {
          return false;
        }
        break;
      }
      if (const auto* jump = as<ir::Jump>(x)) {
        const long tgt = labels.find(jump->target);
        if (tgt >= 0) {
          stack.push_back(static_cast<std::size_t>(tgt));
          break;
        }
        break;
      }
      if (flag_writer(x, flag)) {
        break;
      }
      ++i;
    }
  }
  return true;
}

}  // namespace

LabelMap::LabelMap(Instrs instrs) {
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (const auto* label = as<ir::Label>(instrs[i])) {
      index_[label->name] = static_cast<long>(i);
    }
  }
}

long LabelMap::find(const std::string& name) const {
  const auto it = index_.find(name);
  return it == index_.end() ? -1 : it->second;
}

std::vector<std::pair<std::string, long>> LabelMap::entries() const {
  std::vector<std::pair<std::string, long>> out(index_.begin(), index_.end());
  std::sort(out.begin(), out.end());
  return out;
}

LabelMap label_map(Instrs instrs) { return LabelMap(instrs); }

bool is_reg_a(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

bool is_reg_x(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::X>(r->reg);
}

bool is_reg_y(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::Y>(r->reg);
}

bool a_dead_at(Instrs instrs, std::size_t idx) {
  return a_dead_at(instrs, LabelMap(instrs), idx);
}
bool a_dead_at(Instrs instrs, const LabelMap& labels, std::size_t idx) {
  return reg_dead_at(instrs, labels, idx, R::kA);
}
bool x_dead_at(Instrs instrs, std::size_t idx) {
  return x_dead_at(instrs, LabelMap(instrs), idx);
}
bool x_dead_at(Instrs instrs, const LabelMap& labels, std::size_t idx) {
  return reg_dead_at(instrs, labels, idx, R::kX);
}
bool y_dead_at(Instrs instrs, std::size_t idx) {
  return y_dead_at(instrs, LabelMap(instrs), idx);
}
bool y_dead_at(Instrs instrs, const LabelMap& labels, std::size_t idx) {
  return reg_dead_at(instrs, labels, idx, R::kY);
}

bool reads_a(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::Mov:
      return is_reg_a(std::get<ir::Mov>(instr).src);
    case ir::Kind_instruction::Push:
      return is_reg_a(std::get<ir::Push>(instr).src);
    case ir::Kind_instruction::Add: {
      const auto& i = std::get<ir::Add>(instr);
      return is_reg_a(i.src) || is_reg_a(i.dst);
    }
    case ir::Kind_instruction::Sub: {
      const auto& i = std::get<ir::Sub>(instr);
      return is_reg_a(i.src) || is_reg_a(i.dst);
    }
    case ir::Kind_instruction::And: {
      const auto& i = std::get<ir::And>(instr);
      return is_reg_a(i.src) || is_reg_a(i.dst);
    }
    case ir::Kind_instruction::Or: {
      const auto& i = std::get<ir::Or>(instr);
      return is_reg_a(i.src) || is_reg_a(i.dst);
    }
    case ir::Kind_instruction::Xor: {
      const auto& i = std::get<ir::Xor>(instr);
      return is_reg_a(i.src1) || is_reg_a(i.src2) || is_reg_a(i.dst);
    }
    case ir::Kind_instruction::Compare: {
      const auto& i = std::get<ir::Compare>(instr);
      return is_reg_a(i.left) || is_reg_a(i.right);
    }
    case ir::Kind_instruction::Inc:
      return is_reg_a(std::get<ir::Inc>(instr).dst);
    case ir::Kind_instruction::Dec:
      return is_reg_a(std::get<ir::Dec>(instr).dst);
    case ir::Kind_instruction::ArithmeticShiftLeft:
      return is_reg_a(std::get<ir::ArithmeticShiftLeft>(instr).dst);
    case ir::Kind_instruction::LogicalShiftRight:
      return is_reg_a(std::get<ir::LogicalShiftRight>(instr).dst);
    case ir::Kind_instruction::RotateLeft:
      return is_reg_a(std::get<ir::RotateLeft>(instr).dst);
    case ir::Kind_instruction::RotateRight:
      return is_reg_a(std::get<ir::RotateRight>(instr).dst);
    case ir::Kind_instruction::Phi:
      for (const auto& arg : std::get<ir::Phi>(instr).args) {
        if (is_reg_a(std::get<ir::AsmPhiArg>(arg).source)) {
          return true;
        }
      }
      return false;
    case ir::Kind_instruction::Call:
      return contains(std::get<ir::Call>(instr).reg_args, "A");
    case ir::Kind_instruction::Jump:
      return contains(std::get<ir::Jump>(instr).reg_args, "A");
    default:
      return false;
  }
}

bool kills_a(const ir::Type_instruction& instr) {
  if (const auto* mov = as<ir::Mov>(instr)) {
    if (is_reg_a(mov->dst) && !is_reg_a(mov->src)) {
      return true;
    }
    const bool src_reg = isa<ir::Reg>(mov->src);
    const bool dst_reg = isa<ir::Reg>(mov->dst);
    if (mov->src != mov->dst && src_reg == dst_reg) {
      return !src_reg || !(is_reg_a(mov->src) || is_reg_a(mov->dst));
    }
    // There's no `STX (zp),Y`, so a move from X or Y to the soft stack
    // or through a pointer is `TXA` (`TYA`) and `STA`.
    if (src_reg && !is_reg_a(mov->src) &&
        isa<ir::Frame, ir::Stack, ir::Indirect, ir::IndirectY, ir::IndirectZp,
            ir::IndirectZpY>(mov->dst)) {
      return true;
    }
    return false;
  }
  if (const auto* pop = as<ir::Pop>(instr)) {
    return is_reg_a(pop->dst);
  }
  return isa<ir::Call, ir::LoadAddress, ir::FunctionPrologue, ir::AllocateStack,
             ir::DecimalArith>(instr);
}

bool reads_x(const ir::Type_instruction& instr) {
  return reads_index_reg(instr, R::kX);
}

bool reads_y(const ir::Type_instruction& instr) {
  return reads_index_reg(instr, R::kY);
}

bool call_preserves(const ir::Type_instruction& instr, std::string_view reg) {
  const auto* call = as<ir::Call>(instr);
  return call != nullptr && contains(call->preserves, reg);
}

bool kills_x(const ir::Type_instruction& instr) {
  if (const auto* mov = as<ir::Mov>(instr)) {
    if (is_reg_x(mov->dst)) {
      return !is_reg_x(mov->src);
    }
    return false;
  }
  if (isa<ir::Call>(instr)) {
    return !call_preserves(instr, "X");
  }
  return false;
}

bool kills_y(const ir::Type_instruction& instr) {
  if (const auto* mov = as<ir::Mov>(instr)) {
    if (is_reg_y(mov->dst)) {
      return !is_reg_y(mov->src);
    }
    return operand_setup_clobbers_y(mov->src) || operand_setup_clobbers_y(mov->dst);
  }
  if (isa<ir::Call>(instr)) {
    return !call_preserves(instr, "Y");
  }
  if (const auto* la = as<ir::LoadAddress>(instr)) {
    return operand_setup_clobbers_y(la->dst);
  }
  if (const auto* d = as<ir::DecimalArith>(instr)) {
    return decimal_arith::writes_y(*d);
  }
  return false;
}

bool sets_up_y(const ir::Type_instruction& instr) {
  if (const auto* d = as<ir::DecimalArith>(instr)) {
    return decimal_arith::writes_y(*d);
  }
  bool found = false;
  for_each_operand<false>(instr, [&](const ir::Type_operand& op) {
    found = found || isa<ir::Indirect, ir::IndirectZp, ir::Frame, ir::Stack>(op);
  });
  return found;
}

bool returns_carry(const ir::Type_instruction& instr) {
  if (const auto* ret = as<ir::Ret>(instr)) {
    return ret->save_c;
  }
  if (const auto* ret = as<ir::Return>(instr)) {
    return ret->save_c;
  }
  return false;
}

bool flags_dead_at(Instrs instrs, std::size_t idx) {
  return flags_dead_at(instrs, LabelMap(instrs), idx);
}
bool flags_dead_at(Instrs instrs, const LabelMap& labels, std::size_t idx) {
  return flags_dead_walk(instrs, labels, idx, false);
}
bool all_flags_dead_at(Instrs instrs, std::size_t idx) {
  return all_flags_dead_at(instrs, LabelMap(instrs), idx);
}
bool all_flags_dead_at(Instrs instrs, const LabelMap& labels, std::size_t idx) {
  return flags_dead_walk(instrs, labels, idx, true);
}
bool c_dead_at(Instrs instrs, std::size_t idx) {
  return c_dead_at(instrs, LabelMap(instrs), idx);
}
bool c_dead_at(Instrs instrs, const LabelMap& labels, std::size_t idx) {
  return one_flag_dead(instrs, labels, idx, Flag::kC);
}
bool v_dead_at(Instrs instrs, std::size_t idx) {
  return v_dead_at(instrs, LabelMap(instrs), idx);
}
bool v_dead_at(Instrs instrs, const LabelMap& labels, std::size_t idx) {
  return one_flag_dead(instrs, labels, idx, Flag::kV);
}

bool sets_flags(const ir::Type_instruction& instr) {
  if (sets_up_y(instr)) {
    // The `LDY #off` setup of a soft-stack or indirect operand sets N/Z
    // from the offset, even ahead of a plain `STA`.
    return true;
  }
  if (const auto* mov = as<ir::Mov>(instr)) {
    // `Mov(r, r)` of a register emits nothing, so it leaves the flags
    // alone.
    if (isa<ir::Reg>(mov->dst) && mov->src == mov->dst) {
      return false;
    }
    // A load into A, X, or Y sets the flags.
    if (isa<ir::Reg>(mov->dst)) {
      return true;
    }
    // `Mov(Reg(A), Data | ZP)` is an `STA`, which leaves the flags.
    if (is_reg_a(mov->src)) {
      return false;
    }
    // `Mov(Reg(X|Y), Data | ZP)` is an `STX` / `STY`, which leaves the
    // flags too. To a soft-stack or indirect operand it's `TXA` / `TYA`
    // and then `STA`, and the transfer sets them.
    if (isa<ir::Reg>(mov->src) && isa<ir::Data, ir::ZP>(mov->dst)) {
      return false;
    }
    // A move into memory from anything but A is `LDA src; STA dst`,
    // and the `LDA` sets the flags.
    return true;
  }
  // `Call` leaves the flags as the callee left them, which counts as
  // clobbered.
  return isa<ir::Add, ir::Sub, ir::And, ir::Or, ir::Xor, ir::Inc, ir::Dec,
             ir::ArithmeticShiftLeft, ir::LogicalShiftRight, ir::RotateLeft,
             ir::RotateRight, ir::Compare, ir::Pop, ir::LoadAddress,
             ir::DecimalArith, ir::Call>(instr);
}

}  // namespace c6502::passes
