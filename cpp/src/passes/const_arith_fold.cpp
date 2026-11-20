// Folds constant arithmetic at the asm level, such as an `LDA #imm`
// followed by an `AND` or `ORA` of another immediate.
//
// The case that called for it
//
// When a `uchar` value is tested against a small literal in C, the
// integer promotions lift both sides to `int` (C99 §6.3.1.1.2). The
// high byte of the bitwise result then has the shape `0 & 0`, `0 | 0`,
// and so on, a constant that is zero or unchanged. `tac_to_asm` emits a
// literal `LDA #$00; AND #$00; STA <high>` for each such high byte. This
// pass folds the pair to `LDA #$00; STA <high>`, which saves 2 bytes and
// 2 cycles each time.
//
// The pattern and why the result is the same
//
//     Mov(Imm(c1), A)        // LDA #c1, so A = c1 and the flags are c1's
//     <BinOp>(Imm(c2), A)    // AND/ORA #c2, so A = c1 OP c2 and the flags are its
//
// becomes
//
//     Mov(Imm(c1 OP c2), A)  // LDA #(c1 OP c2)
//
// The replacement leaves A's value and N/Z as they were. `LDA #k` sets
// N to bit 7 of k and Z to whether k is 0, which is what the two
// instructions left behind. So the rewrite is sound even when a later
// instruction reads A or the flags, and no liveness check is needed.
//
// An identity operation after a write of A
//
// A second pattern drops an `Or(Imm(0), A)` or `And(Imm(0xFF), A)`
// right after an instruction that wrote A:
//
//     <writes A, sets N/Z from A's new value>   // `LDA M`, `AND #x`, ...
//     Or(Imm(0), A)                              // A | 0 = A
// or  And(Imm(0xFF), A)                          // A & $FF = A
//
// A's value doesn't change, and the N/Z flags that the write set match
// what the identity operation would leave, since both come from A's
// value. AND and ORA leave C and V alone, so those are the same in both
// forms too. The test for a write of A takes every instruction whose
// destination is `Reg(A)`: a `Mov` (LDA, TXA, TYA) and the arithmetic
// on A (`Add`, `Sub`, `And`, `Or`, `Xor`).
//
// A label's high byte plus a constant
//
// `LDA #>(name+o); CLC; ADC #k` becomes `LDA #>(name+o+256k)`, and
// `SEC; SBC #k` subtracts `256k`, when C and V are dead after. The
// assembler computes the sum. A pointer `page_tables + 0x100`, whose
// low byte is the label's own, leaves this on its high byte. An offset
// that would come out negative is left alone.
//
// Shifting in a 1
//
// `ASL A; CLC; ADC #1` becomes `SEC; ROL A` when C and V are dead
// after. The add sets the bit 0 that the shift cleared, as rotating a
// set C in does, and A, N, and Z come out the same.
//
// Adding 0 with the carry known
//
// `CLC; ADC #$00` and `SEC; SBC #$00` leave A and C as they were, so
// the `ADC` / `SBC` goes when V is dead after and N and Z are either
// dead or already set from A by the instruction before the `CLC`. An
// add of 2 bytes of a value whose low byte is 0, such as `p + (n <<
// 8)`, leaves this on its low byte.
//
// Scope and order
//
// The constant folds take only `Reg(A)` as the destination and `Imm` as
// both sources, the shape `tac_to_asm` emits. A lone `Add` or `Sub`
// after a load is left out on purpose, since it reads the carry, which a
// load doesn't set, so its result depends on what came before. `Xor` is
// left out because its IR shape (`src1`, `src2`, `dst`) differs from the
// `src` / `dst` of `And` and `Or`.
//
// The pass runs in the asm peephole fixed point, after
// `replace_pseudoregisters` has made the operands concrete. Each fold
// turns two atoms into one, or three into one or two, so the code only
// shrinks.
#include "passes/const_arith_fold.h"

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

using InstrList = std::vector<ir::Type_instruction>;

// Returns the immediate's value when `op` is an `Imm`, as a byte.
std::optional<asdl::Int> imm_byte(const ir::Type_operand& op) {
  if (const auto* imm = as<ir::Imm>(op)) {
    return imm->value & 0xFF;
  }
  return std::nullopt;
}

ir::Mov load_a(ir::Type_operand src, const ir::Type_operand& dst) {
  return ir::Mov{.src = std::move(src), .dst = dst, .is_volatile = false};
}

// Returns `Mov(Imm(0), A)` for `And(Imm(0), A)`, and `Mov(Imm(0xFF), A)`
// for `Or(Imm(0xFF), A)`, whose result doesn't depend on A. Each sets N
// and Z as the load does. The byte of `(w & 0xFF00) | b` that keeps
// nothing of `w` is one.
std::optional<ir::Type_instruction> constant_result(const ir::Type_instruction& a) {
  if (const auto* x = as<ir::And>(a)) {
    if (is_reg_a(x->dst) && imm_byte(x->src) == asdl::Int(0)) {
      return load_a(ir::Imm{.value = 0}, x->dst);
    }
  } else if (const auto* y = as<ir::Or>(a)) {
    if (is_reg_a(y->dst) && imm_byte(y->src) == asdl::Int(0xFF)) {
      return load_a(ir::Imm{.value = 0xFF}, y->dst);
    }
  }
  return std::nullopt;
}

// Returns `ImmLabelHigh(name, offset + 256 * k)` for `LDA
// #>(name+offset); CLC; ADC #k` (or `LDA #k; CLC; ADC #>(name+offset)`),
// and with `-256 * k` for `SEC; SBC #k` after the label's half. Adding
// `k` to the high byte of an address adds `256 * k` to the address,
// which dasm and the linker compute, so `page_tables + 0x100` in Zork's
// `main`, whose low byte is the label's, loads its high byte with one
// `LDA`.
std::optional<ir::Type_operand> label_high_sum(const ir::Type_instruction& a,
                                               const ir::Type_instruction& flag,
                                               const ir::Type_instruction& op) {
  const auto* load = as<ir::Mov>(a);
  if (load == nullptr || !is_reg_a(load->dst)) {
    return std::nullopt;
  }
  const ir::Type_operand* op_src = nullptr;
  int sign = 0;
  if (const auto* add = as<ir::Add>(op); add && is_reg_a(add->dst)) {
    if (isa<ir::ClearCarry>(flag)) {
      sign = 1;
    }
    op_src = &add->src;
  } else if (const auto* sub = as<ir::Sub>(op); sub && is_reg_a(sub->dst)) {
    if (isa<ir::SetCarry>(flag)) {
      sign = -1;
    }
    op_src = &sub->src;
  }
  if (op_src == nullptr || sign == 0) {
    return std::nullopt;
  }
  const ir::ImmLabelHigh* label = nullptr;
  asdl::Int k = 0;
  if (const auto* hi = as<ir::ImmLabelHigh>(load->src); hi && isa<ir::Imm>(*op_src)) {
    label = hi;
    k = *imm_byte(*op_src);
  } else if (sign == 1 && isa<ir::Imm>(load->src) && isa<ir::ImmLabelHigh>(*op_src)) {
    label = &std::get<ir::ImmLabelHigh>(*op_src);
    k = *imm_byte(load->src);
  } else {
    return std::nullopt;
  }
  const asdl::Int offset = label->offset + asdl::Int(sign) * 256 * k;
  // `asm_emit` writes an offset as `name+offset`, so a negative one
  // would read `name+-256`.
  if (offset < 0) {
    return std::nullopt;
  }
  return ir::ImmLabelHigh{.name = label->name, .offset = offset};
}

// Returns `Mov(Imm(k1 + k2), A)` for `LDA #k1; CLC; ADC #k2` at
// `instrs[i]`, and `Mov(Imm(k1 - k2), A)` for `LDA #k1; SEC; SBC #k2`,
// when C and V are dead after. N and Z come out as the add left them.
// `if (w & 0x8000) pages += 0x100;` on a `pages` whose high byte is 0
// adds 1 to 0 this way.
std::optional<ir::Type_instruction> try_fold_add(const InstrList& instrs, const LabelMap& labels,
                                                 std::size_t i) {
  if (i + 2 >= instrs.size()) {
    return std::nullopt;
  }
  const auto& a = instrs[i];
  const auto& flag = instrs[i + 1];
  const auto& op = instrs[i + 2];
  if (const auto high = label_high_sum(a, flag, op)) {
    if (!(c_dead_at(instrs, labels, i + 3) && v_dead_at(instrs, labels, i + 3))) {
      return std::nullopt;
    }
    return load_a(*high, std::get<ir::Mov>(a).dst);
  }
  const auto* load = as<ir::Mov>(a);
  if (load == nullptr || !isa<ir::Imm>(load->src) || !is_reg_a(load->dst)) {
    return std::nullopt;
  }
  const auto* add = as<ir::Add>(op);
  const auto* sub = as<ir::Sub>(op);
  const ir::Type_operand* src = add ? &add->src : sub ? &sub->src : nullptr;
  const ir::Type_operand* dst = add ? &add->dst : sub ? &sub->dst : nullptr;
  if (src == nullptr || !isa<ir::Imm>(*src) || !is_reg_a(*dst)) {
    return std::nullopt;
  }
  const asdl::Int k1 = *imm_byte(load->src);
  const asdl::Int k2 = *imm_byte(*src);
  asdl::Int value = 0;
  if (add != nullptr && isa<ir::ClearCarry>(flag)) {
    value = (k1 + k2) & 0xFF;
  } else if (sub != nullptr && isa<ir::SetCarry>(flag)) {
    value = (k1 - k2) & 0xFF;
  } else {
    return std::nullopt;
  }
  if (!(c_dead_at(instrs, labels, i + 3) && v_dead_at(instrs, labels, i + 3))) {
    return std::nullopt;
  }
  return load_a(ir::Imm{.value = value}, load->dst);
}

// Returns one `ADC #k` for `ADC #k1; CLC; ADC #k2` at `instrs[i]`, with
// `k = k1 + k2`, when C and V are dead after. The first add's carry in
// comes through, so A is `A + k1 + C + k2` either way, and N and Z are
// set from it. An `SBC` in either place, after a `SEC` for the second,
// subtracts its constant from `k`, and the result is an `SBC` when the
// first was one: `SBC #k1; CLC; ADC #k2` is `A - k1 - (1 - C) + k2`,
// which is `SBC #(k1 - k2)`. Only the C and V of the second differ.
// After a byte divided by a constant, `a % 21 + 1` is the loop's `ADC
// #$15` and then `CLC; ADC #$01`.
std::optional<ir::Type_instruction> try_fold_add_add(const InstrList& instrs,
                                                     const LabelMap& labels, std::size_t i) {
  if (i + 2 >= instrs.size()) {
    return std::nullopt;
  }
  const auto& first = instrs[i];
  const auto& flag = instrs[i + 1];
  const auto& second = instrs[i + 2];
  auto operands = [](const ir::Type_instruction& ins)
      -> std::pair<const ir::Type_operand*, const ir::Type_operand*> {
    if (const auto* x = as<ir::Add>(ins)) return {&x->src, &x->dst};
    if (const auto* x = as<ir::Sub>(ins)) return {&x->src, &x->dst};
    return {nullptr, nullptr};
  };
  const auto [src1, dst1] = operands(first);
  const auto [src2, dst2] = operands(second);
  if (src1 == nullptr || !isa<ir::Imm>(*src1) || !is_reg_a(*dst1) || src2 == nullptr ||
      !isa<ir::Imm>(*src2) || !is_reg_a(*dst2)) {
    return std::nullopt;
  }
  if (isa<ir::Add>(second) != isa<ir::ClearCarry>(flag)) {
    return std::nullopt;
  }
  if (!isa<ir::ClearCarry, ir::SetCarry>(flag)) {
    return std::nullopt;
  }
  const asdl::Int k1 = *imm_byte(*src1);
  const asdl::Int k2 = *imm_byte(*src2);
  const bool same = isa<ir::Add>(first) == isa<ir::Add>(second);
  const asdl::Int value = (same ? k1 + k2 : k1 - k2) & 0xFF;
  if (!(c_dead_at(instrs, labels, i + 3) && v_dead_at(instrs, labels, i + 3))) {
    return std::nullopt;
  }
  if (isa<ir::Add>(first)) {
    return ir::Add{.src = ir::Imm{.value = value}, .dst = *dst1};
  }
  return ir::Sub{.src = ir::Imm{.value = value}, .dst = *dst1};
}

// Returns `SEC; ROL A` for `ASL A; CLC; ADC #1` at `instrs[i]`, when C
// and V are dead after. The `ASL` leaves bit 0 clear, so adding 1 sets
// it with no carry, which is what rotating a set C in does. A, N, and Z
// come out the same, and only C and V differ: the add leaves both clear,
// and the `ROL` puts the old bit 7 in C. `(uint8_t)(props[0] << 1) + 1`
// in Zork's `get_property_ptr` is the case.
std::optional<std::vector<ir::Type_instruction>> shift_in_one(const InstrList& instrs,
                                                              const LabelMap& labels,
                                                              std::size_t i) {
  if (i + 2 >= instrs.size()) {
    return std::nullopt;
  }
  const auto* shift = as<ir::ArithmeticShiftLeft>(instrs[i]);
  const auto* op = as<ir::Add>(instrs[i + 2]);
  if (!(shift && is_reg_a(shift->dst) && isa<ir::ClearCarry>(instrs[i + 1]) && op &&
        is_reg_a(op->dst) && imm_byte(op->src) == asdl::Int(1))) {
    return std::nullopt;
  }
  if (!(c_dead_at(instrs, labels, i + 3) && v_dead_at(instrs, labels, i + 3))) {
    return std::nullopt;
  }
  return std::vector<ir::Type_instruction>{ir::SetCarry{}, ir::RotateLeft{.dst = shift->dst}};
}

// Returns whether `instrs[i]` starts `CLC; ADC #0` or `SEC; SBC #0`,
// whose `ADC` / `SBC` leaves A and C as they were. It writes V (0) and
// N and Z from A, so V must be dead after, and N and Z dead too unless
// the instruction before the `CLC` set them from A. `Z_HEADER_ADDR +
// (sector << 8)` adds the shift's low byte, a 0, this way: `LDA
// Z_HEADER_ADDR; CLC; ADC #$00; STA lo; LDA Z_HEADER_ADDR+1; ADC
// sector`.
bool is_carry_free_zero_add(const InstrList& instrs, const LabelMap& labels, std::size_t i) {
  if (i + 1 >= instrs.size()) {
    return false;
  }
  const auto& flag = instrs[i];
  const auto& op = instrs[i + 1];
  const ir::Type_operand* src = nullptr;
  const ir::Type_operand* dst = nullptr;
  if (const auto* x = as<ir::Add>(op)) {
    src = &x->src;
    dst = &x->dst;
  } else if (const auto* y = as<ir::Sub>(op)) {
    src = &y->src;
    dst = &y->dst;
  }
  if (src == nullptr || imm_byte(*src) != asdl::Int(0) || !is_reg_a(*dst)) {
    return false;
  }
  if (isa<ir::Add>(op) != isa<ir::ClearCarry>(flag)) {
    return false;
  }
  if (isa<ir::Sub>(op) != isa<ir::SetCarry>(flag)) {
    return false;
  }
  if (!v_dead_at(instrs, labels, i + 2)) {
    return false;
  }
  if (flags_dead_at(instrs, labels, i + 2)) {
    return true;
  }
  if (i == 0) {
    return false;
  }
  const auto& prev = instrs[i - 1];
  if (const auto* x = as<ir::Mov>(prev)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::And>(prev)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::Or>(prev)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::Add>(prev)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::Sub>(prev)) return is_reg_a(x->dst);
  return false;
}

// Returns `Mov(Imm(c1 OP c2), A)` when `a; b` is `Mov(Imm, A);
// <And|Or>(Imm, A)`.
std::optional<ir::Type_instruction> try_fold(const ir::Type_instruction& a,
                                             const ir::Type_instruction& b) {
  const auto* load = as<ir::Mov>(a);
  if (load == nullptr || !isa<ir::Imm>(load->src) || !is_reg_a(load->dst)) {
    return std::nullopt;
  }
  const asdl::Int c1 = *imm_byte(load->src);
  if (const auto* x = as<ir::And>(b); x && isa<ir::Imm>(x->src) && is_reg_a(x->dst)) {
    return load_a(ir::Imm{.value = c1 & *imm_byte(x->src)}, load->dst);
  }
  if (const auto* x = as<ir::Or>(b); x && isa<ir::Imm>(x->src) && is_reg_a(x->dst)) {
    return load_a(ir::Imm{.value = c1 | *imm_byte(x->src)}, load->dst);
  }
  return std::nullopt;
}

// Returns whether `b` is an identity `Or(Imm(0), A)` or `And(Imm(0xFF),
// A)` and `a` wrote A and left A's N/Z in the flags: `a`'s destination
// is `Reg(A)` and it is a `Mov`, `And`, `Or`, `Add`, `Sub`, or `Xor`.
bool is_identity_after_a_write(const ir::Type_instruction& a, const ir::Type_instruction& b) {
  if (const auto* x = as<ir::Or>(b)) {
    if (!(isa<ir::Imm>(x->src) && is_reg_a(x->dst) && *imm_byte(x->src) == 0)) {
      return false;
    }
  } else if (const auto* y = as<ir::And>(b)) {
    if (!(isa<ir::Imm>(y->src) && is_reg_a(y->dst) && *imm_byte(y->src) == 0xFF)) {
      return false;
    }
  } else {
    return false;
  }
  if (const auto* x = as<ir::Mov>(a)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::And>(a)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::Or>(a)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::Add>(a)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::Sub>(a)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::Xor>(a)) return is_reg_a(x->dst);
  return false;
}

// Returns `Mov(M, A)` for `Mov(Imm(0), A); Or(M, A)` and for
// `Mov(Imm(0xFF), A); And(M, A)`. Both leave A = M with the flags from
// M, as a plain `LDA M` does.
std::optional<ir::Type_instruction> try_absorb_zero_load(const ir::Type_instruction& a,
                                                         const ir::Type_instruction& b) {
  const auto* load = as<ir::Mov>(a);
  if (load == nullptr || !isa<ir::Imm>(load->src) || !is_reg_a(load->dst)) {
    return std::nullopt;
  }
  const asdl::Int c1 = *imm_byte(load->src);
  if (const auto* x = as<ir::Or>(b); x && is_reg_a(x->dst) && c1 == 0) {
    return load_a(x->src, load->dst);
  }
  if (const auto* x = as<ir::And>(b); x && is_reg_a(x->dst) && c1 == 0xFF) {
    return load_a(x->src, load->dst);
  }
  return std::nullopt;
}

void rewrite_function(ir::Function& fn) {
  const InstrList& instrs = fn.instructions;
  const LabelMap labels(instrs);
  InstrList out;
  out.reserve(instrs.size());
  std::size_t i = 0;
  while (i + 1 < instrs.size()) {
    const auto& a = instrs[i];
    const auto& b = instrs[i + 1];
    if (auto c = constant_result(a)) {
      out.push_back(std::move(*c));
      i += 1;
      continue;
    }
    if (auto shifted = shift_in_one(instrs, labels, i)) {
      for (auto& x : *shifted) {
        out.push_back(std::move(x));
      }
      i += 3;
      continue;
    }
    if (is_carry_free_zero_add(instrs, labels, i)) {
      // Keeps the CLC / SEC and drops the ADC #0 / SBC #0.
      out.push_back(a);
      i += 2;
      continue;
    }
    auto summed = try_fold_add(instrs, labels, i);
    if (!summed) {
      summed = try_fold_add_add(instrs, labels, i);
    }
    if (summed) {
      out.push_back(std::move(*summed));
      i += 3;
      continue;
    }
    if (auto folded = try_fold(a, b)) {
      out.push_back(std::move(*folded));
      i += 2;
      continue;
    }
    if (is_identity_after_a_write(a, b)) {
      // Drops `b`, the identity operation, and keeps `a`.
      out.push_back(a);
      i += 2;
      continue;
    }
    if (auto absorbed = try_absorb_zero_load(a, b)) {
      out.push_back(std::move(*absorbed));
      i += 2;
      continue;
    }
    out.push_back(a);
    i += 1;
  }
  while (i < instrs.size()) {
    out.push_back(instrs[i]);
    ++i;
  }
  fn.instructions = std::move(out);
}

}  // namespace

asm_ast::Program apply_const_arith_fold(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
