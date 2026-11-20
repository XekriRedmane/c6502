// The single-byte DEC peephole, which collapses the SBC chain of
// `mem -= 1` into a `DEC`.
//
// `tac_to_asm` lowers `Binary(Subtract, x, 1, x)`, a subtract of 1 in
// place, on a 1-byte value as an SBC chain of four instructions:
//
//     Mov(M, A); SetCarry; Sub(Imm(1), A); Mov(A, M)
//
// For `Data(name, k)` operands (static storage), `ZP(addr+k, 0)`
// operands (locals the register allocator placed), and `Reg(X|Y)`
// (counters pinned to a register), the 6502 has a much shorter
// encoding. `DEC m` is a read-modify-write of 2 bytes (zero page) or 3
// (absolute) that decrements the byte in place, and `DEX` / `DEY` is a
// 1-byte implied-mode opcode that decrements the index register. With
// it the subtract becomes one instruction:
//
//     DEC M    (or DEX / DEY)
//
// Wider values
//
// A subtract of 1 from a wider value is also lowered as an SBC chain
// (`Sub #1` on byte 0 and `Sub #0` on each later byte, with the carry
// going from one to the next). In the INC chain a `BNE` tests that a
// byte didn't wrap to zero, but no one flag after a DEC tells whether
// the byte borrowed. After a DEC, Z is set exactly when the result is 0
// (the input was 1, with no borrow), and N exactly when the result has
// bit 7 set, which happens when the input was 0 (giving $FF) or was in
// $80 to $FF (giving $7F to $FE), and only the first borrows. So a BMI
// on DEC's result can't tell a borrow from a large result.
//
// The wider SBC chain therefore stays as it is, except for the 2-byte
// form that `carry_inc` leaves, `LDA M0; SEC; SBC #1; STA M0; BCS L;
// DEC M1; L:`. Testing the low byte before decrementing it (`LDA M0;
// BNE L; DEC M1; L: DEC M0`) tells whether it borrows, when A and every
// flag are dead after (`try_match_two_byte`).
//
// Eligibility
//
//   * The operand is `Data(name, k)`, `ZP(addr, 0)`, `Reg(X)`,
//     `Reg(Y)`, or an array element indexed by X, `IndexedData(name, k,
//     X)`. DEX and DEY exist for the registers, and DEC has zero page,
//     absolute, zp,X, and abs,X forms for memory. `Frame`, `Stack`, and
//     `Indirect` use indirect-Y, and there's no `DEC abs,Y`, so those
//     are skipped.
//   * Neither the load nor the store is volatile. `DEC M` reads M and
//     writes it once each (an NMOS 6502 also writes the old value back
//     first), so a volatile byte keeps its separate load and store.
//   * For an element, `LDA t,X; SEC; SBC #1; STA t,X` becomes `DEC t,X`
//     (3 bytes and 7 cycles against 9 bytes and 13 cycles). Nothing
//     between the load and the store writes X, since the four
//     instructions are adjacent, so both name the same byte, and
//     `DEC t,X` names it too. As for any operand, C and V must be dead
//     after, since `SBC` sets them and `DEC` leaves them, and when A is
//     live `LDA t,X` follows the `DEC`, which sets N and Z as the `SBC`
//     did. Lode Runner's `--GUARD_RESURRECTION_TIMERS[g]` is
//     `DEC GUARD_RESURRECTION_TIMERS,X`.
//   * The update is in place, `LDA M; SEC; SBC #1; STA M` with the same
//     M throughout.
//
// Where it runs
//
// After `replace_pseudoregisters`, so the operands are concrete `Data`,
// `ZP`, and `Reg` operands, and before `expand_long_branches`, though
// the one-byte rewrite adds no branch. It sits beside `inc_peephole`,
// which it composes with.
#include "passes/dec_peephole.h"

#include <optional>
#include <unordered_set>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"
#include "passes/inc_peephole.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;
using inc_peephole::operands_equal;

bool is_reg_a_op(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

bool is_imm(const ir::Type_operand& op, int value) {
  const auto* imm = as<ir::Imm>(op);
  return imm != nullptr && imm->value == value;
}

// Returns whether `op` is an operand DEC / DEX / DEY can address
// directly: `Data(name, k)`, `ZP(addr, 0)`, `Reg(X)`, `Reg(Y)`, or
// `IndexedData(name, k, X)`. `Frame`, `Stack`, `Indirect`, and an
// element indexed by Y use addressing modes DEC doesn't have.
bool is_dec_eligible_operand(const ir::Type_operand& op) {
  if (isa<ir::Data, ir::ZP>(op) || is_x_indexed(op)) {
    return true;
  }
  if (const auto* r = as<ir::Reg>(op)) {
    return isa<ir::X, ir::Y>(r->reg);
  }
  return false;
}

// Returns the `dst` of an instruction that sets N and Z from the value
// it leaves in A (a move, ADC, SBC, AND, ORA, EOR, a shift or rotate,
// or a pull), or null for any other instruction. The caller compares
// it with A.
const ir::Type_operand* nz_from_a_dst(const ir::Type_instruction& ins) {
  switch (ir::kind(ins)) {
    case ir::Kind_instruction::Mov:
      return &std::get<ir::Mov>(ins).dst;
    case ir::Kind_instruction::Add:
      return &std::get<ir::Add>(ins).dst;
    case ir::Kind_instruction::Sub:
      return &std::get<ir::Sub>(ins).dst;
    case ir::Kind_instruction::And:
      return &std::get<ir::And>(ins).dst;
    case ir::Kind_instruction::Or:
      return &std::get<ir::Or>(ins).dst;
    case ir::Kind_instruction::Xor:
      return &std::get<ir::Xor>(ins).dst;
    case ir::Kind_instruction::ArithmeticShiftLeft:
      return &std::get<ir::ArithmeticShiftLeft>(ins).dst;
    case ir::Kind_instruction::LogicalShiftRight:
      return &std::get<ir::LogicalShiftRight>(ins).dst;
    case ir::Kind_instruction::RotateLeft:
      return &std::get<ir::RotateLeft>(ins).dst;
    case ir::Kind_instruction::RotateRight:
      return &std::get<ir::RotateRight>(ins).dst;
    case ir::Kind_instruction::Pop:
      return &std::get<ir::Pop>(ins).dst;
    default:
      return nullptr;
  }
}

// Returns false when the low byte's load at `instrs[i]` follows a store
// of A into the same byte, with only stores between, and the
// instruction before those stores didn't set N and Z from A. The `SBC`
// form then uses A without the load, which `round_trip_load` drops,
// while the `BNE` form needs the load to set the flags, as an
// `ORA #$00` after a `JSR` does. That made `dec_var`'s `var_get() - 1`
// a byte longer. After an `AND` that set them, the load goes either
// way.
bool load_can_test(Instrs instrs, std::size_t i) {
  const ir::Type_operand& m0 = std::get<ir::Mov>(instrs[i]).src;
  std::ptrdiff_t k = static_cast<std::ptrdiff_t>(i) - 1;
  bool stored = false;
  while (k >= 0) {
    const auto* mov = as<ir::Mov>(instrs[static_cast<std::size_t>(k)]);
    if (mov == nullptr || !isa<ir::Reg>(mov->src) || isa<ir::Reg>(mov->dst)) {
      break;
    }
    if (is_reg_a_op(mov->src) && operands_equal(mov->dst, m0)) {
      stored = true;
    }
    --k;
  }
  if (!stored) {
    return true;
  }
  if (k < 0) {
    return false;
  }
  const ir::Type_operand* dst = nz_from_a_dst(instrs[static_cast<std::size_t>(k)]);
  return dst != nullptr && is_reg_a_op(*dst);
}

// Returns `LDA M0; BNE L; DEC M1; L: DEC M0` for the 2-byte decrement
// in place that `carry_inc` leaves, `LDA M0; SEC; SBC #1; STA M0; BCS
// L; DEC M1; L:`, or nothing. The low byte borrows when it was 0, so
// the high byte is decremented first, when the low one is 0, and then
// the low one. That's 8 bytes for zero-page bytes where the `SBC` form
// took 10. A, C, V, N, and Z must be dead after `L`, since they come
// out different, and nothing else may branch to `L`, where the low byte
// has already been decremented. Zork's `push` decrements `Z_SP` twice
// this way.
std::optional<std::vector<ir::Type_instruction>> try_match_two_byte(Instrs instrs,
                                                                    std::size_t i) {
  if (i + 6 >= instrs.size()) {
    return std::nullopt;
  }
  const auto* load = as<ir::Mov>(instrs[i]);
  const auto* sub = as<ir::Sub>(instrs[i + 2]);
  const auto* store = as<ir::Mov>(instrs[i + 3]);
  const auto* bcs = as<ir::Branch>(instrs[i + 4]);
  const auto* dec = as<ir::Dec>(instrs[i + 5]);
  const auto* label = as<ir::Label>(instrs[i + 6]);
  if (!(load != nullptr && is_reg_a_op(load->dst) && !load->is_volatile &&
        isa<ir::SetCarry>(instrs[i + 1]) && sub != nullptr && is_reg_a_op(sub->dst) &&
        is_imm(sub->src, 1) && store != nullptr && is_reg_a_op(store->src) &&
        !store->is_volatile && operands_equal(store->dst, load->src) &&
        is_dec_eligible_operand(load->src) && bcs != nullptr && isa<ir::CS>(bcs->cond) &&
        dec != nullptr && (isa<ir::Data, ir::ZP>(dec->dst) || is_x_indexed(dec->dst)) &&
        label != nullptr && label->name == bcs->target)) {
    return std::nullopt;
  }
  if (isa<ir::Reg>(load->src) || operands_equal(dec->dst, load->src)) {
    return std::nullopt;
  }
  std::size_t refs = 0;
  for (const auto& x : instrs) {
    if (const auto* b = as<ir::Branch>(x)) {
      refs += b->target == label->name;
    } else if (const auto* j = as<ir::Jump>(x)) {
      refs += j->target == label->name;
    } else if (const auto* t = as<ir::JumpTable>(x)) {
      for (const auto& target : t->targets) {
        if (target == label->name) {
          ++refs;
          break;
        }
      }
    }
  }
  if (refs != 1) {
    return std::nullopt;
  }
  if (!(a_dead_at(instrs, i + 7) && all_flags_dead_at(instrs, i + 7))) {
    return std::nullopt;
  }
  if (!load_can_test(instrs, i)) {
    return std::nullopt;
  }
  return std::vector<ir::Type_instruction>{
      *load,
      ir::Branch{.cond = ir::NE{}, .target = label->name},
      *dec,
      *label,
      ir::Dec{.dst = load->src},
  };
}

// Returns whether `instrs[pos..pos+3)` is a later byte of a multi-byte
// SBC chain, `Mov(M, A); Sub(Imm(0), A); Mov(A, M)`. The carry from the
// byte before goes into its `Sub` through C, so the first byte's chain
// can't become a `Dec`, which doesn't touch C, without breaking the
// chain.
bool is_sbc_continuation(Instrs instrs, std::size_t pos) {
  if (pos + 3 > instrs.size()) {
    return false;
  }
  const auto* j0 = as<ir::Mov>(instrs[pos]);
  if (j0 == nullptr || !is_reg_a_op(j0->dst)) {
    return false;
  }
  const auto* j1 = as<ir::Sub>(instrs[pos + 1]);
  if (j1 == nullptr || !is_imm(j1->src, 0) || !is_reg_a_op(j1->dst)) {
    return false;
  }
  const auto* j2 = as<ir::Mov>(instrs[pos + 2]);
  return j2 != nullptr && is_reg_a_op(j2->src) && operands_equal(j2->dst, j0->src);
}

// Returns whether the C or V that the SBC leaves can be read before
// both are written again, by a `BCC` / `BCS` / `BVC` / `BVS` or by an
// ADC, SBC, ROL, or ROR that reads the carry. DEC sets only N and Z, so
// folding the SBC into a DEC is unsound whenever such a reader can be
// reached.
//
// LDA, STA, and the transfers (`Mov`), the logical operations (`And`,
// `Or`, `Xor`), `Inc`, `Dec`, `Push`, `Pop`, and `BitTest` (BIT sets N
// and V but not C) leave C and V alone, so the SBC's carry goes through
// them and the scan goes on. `Add`, `Sub`, `RotateLeft`, and
// `RotateRight` read C before writing it, so reaching one is unsafe.
// `Compare`, `ASL`, `LSR`, `SetCarry`, and `ClearCarry` write C without
// reading it, and once C is written that way the SBC's carry can't
// reach a later reader on that path, so the path is safe.
//
// A `Branch` on N or Z is followed down both arms, and a `Jump` to its
// target. A call or a return ends a path safely, since the callee
// changes the flags or the function exits.
bool c_or_v_read_before_redefined(Instrs instrs, const LabelMap& labels, std::size_t pos) {
  const std::size_t n = instrs.size();
  std::unordered_set<std::size_t> seen;
  std::vector<std::size_t> stack{pos};
  while (!stack.empty()) {
    std::size_t j = stack.back();
    stack.pop_back();
    while (j < n) {
      if (!seen.insert(j).second) {
        break;
      }
      const ir::Type_instruction& instr = instrs[j];
      if (const auto* br = as<ir::Branch>(instr)) {
        if (isa<ir::CC, ir::CS, ir::VC, ir::VS>(br->cond)) {
          // It reads the SBC's C or V.
          return true;
        }
        // A branch on N or Z reads only those, which DEC sets as the
        // SBC did. The carry goes down both arms, so the target is
        // followed and the scan goes on down the fall-through.
        const long t = labels.find(br->target);
        if (t >= 0) {
          stack.push_back(static_cast<std::size_t>(t));
        }
        ++j;
        continue;
      }
      // ADC, SBC, ROL, and ROR read C before they write it. Reaching
      // one means the SBC's borrow goes into a later byte of a
      // multi-byte chain (for example `LDA hi; SBC #0; TAX`, where the
      // high byte isn't stored back in place and so doesn't match
      // `is_sbc_continuation`). DEC leaves C alone, so the fold is
      // unsafe.
      if (isa<ir::Add, ir::Sub, ir::RotateLeft, ir::RotateRight>(instr)) {
        return true;
      }
      if (isa<ir::Compare, ir::ArithmeticShiftLeft, ir::LogicalShiftRight, ir::SetCarry,
              ir::ClearCarry>(instr)) {
        break;
      }
      if (isa<ir::Mov, ir::And, ir::Or, ir::Xor, ir::Inc, ir::Dec, ir::Pop, ir::Push,
              ir::BitTest, ir::Label>(instr)) {
        ++j;
        continue;
      }
      if (const auto* jump = as<ir::Jump>(instr)) {
        const long t = labels.find(jump->target);
        if (t >= 0) {
          stack.push_back(static_cast<std::size_t>(t));
        }
        break;
      }
      // A call, whose callee changes the flags, or a return. The SBC's
      // carry reaches no later branch on this path.
      break;
    }
  }
  return false;
}

// Matches `Mov(M, A); SetCarry; Sub(Imm(1), A); Mov(A, M)` at
// `instrs[start]` and returns M, an operand DEC takes, or null.
//
// The match must not be the first byte of a multi-byte SBC chain. The
// later bytes (`Mov(M[k+1], A); Sub(Imm(0), A); Mov(A, M[k+1])`) read
// the carry from the SBC before, so replacing the first byte with
// `Dec(M[0])` would lose it and the higher bytes would borrow when they
// shouldn't. A match followed by such a byte is refused.
const ir::Type_operand* try_match_sub1(Instrs instrs, const LabelMap& labels,
                                       std::size_t start) {
  if (start + 4 > instrs.size()) {
    return nullptr;
  }
  const auto* i0 = as<ir::Mov>(instrs[start]);
  if (i0 == nullptr || i0->is_volatile || !is_dec_eligible_operand(i0->src) ||
      !is_reg_a_op(i0->dst)) {
    return nullptr;
  }
  if (!isa<ir::SetCarry>(instrs[start + 1])) {
    return nullptr;
  }
  const auto* i2 = as<ir::Sub>(instrs[start + 2]);
  if (i2 == nullptr || !is_imm(i2->src, 1) || !is_reg_a_op(i2->dst)) {
    return nullptr;
  }
  const auto* i3 = as<ir::Mov>(instrs[start + 3]);
  if (i3 == nullptr || i3->is_volatile || !is_reg_a_op(i3->src) ||
      !operands_equal(i3->dst, i0->src)) {
    return nullptr;
  }
  if (is_sbc_continuation(instrs, start + 4)) {
    return nullptr;
  }
  // The SBC sets C and V and DEC doesn't, so a read of either before it
  // is written again (by a branch, or by an ADC / SBC / ROL / ROR) would
  // see a stale value. N and Z are set by DEC from the result, as the
  // SBC set them, so BMI, BPL, BEQ, and BNE are unaffected.
  if (c_or_v_read_before_redefined(instrs, labels, start + 4)) {
    return nullptr;
  }
  return &i0->src;
}

// Rewrites `fn` in place when a pattern matches. At each position it
// tries the 2-byte form and then the 1-byte one, and on a match writes
// the replacement and goes past the matched instructions.
void rewrite_function(ir::Function& fn) {
  const auto& instrs = fn.instructions;
  const LabelMap labels(instrs);
  std::vector<ir::Type_instruction> out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    if (auto two = try_match_two_byte(instrs, i)) {
      for (auto& ins : *two) {
        out.push_back(std::move(ins));
      }
      changed = true;
      i += 7;
      continue;
    }
    const ir::Type_operand* mem = try_match_sub1(instrs, labels, i);
    if (mem == nullptr) {
      out.push_back(instrs[i]);
      ++i;
      continue;
    }
    out.push_back(ir::Dec{.dst = *mem});
    if (!a_dead_at(instrs, labels, i + 4)) {
      // The SBC form left M's new value in A, and DEC doesn't touch A,
      // so the value is loaded when something reads A afterward.
      out.push_back(ir::Mov{.src = *mem, .dst = ir::Reg{.reg = ir::A{}}, .is_volatile = false});
    }
    changed = true;
    i += 4;
  }
  if (changed) {
    fn.instructions = std::move(out);
  }
}

}  // namespace

asm_ast::Program apply_dec_peephole(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
