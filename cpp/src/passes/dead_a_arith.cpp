// Dead A arithmetic elimination.
//
// The pass drops instructions whose only observable effects are writes
// to A and to the N and Z flags (and C and V for arithmetic), when A and
// those flags are dead afterward.
//
// The case that called for it
//
// The unoptimized `y += 35` skip path in `interlace_blit_p1` lowers the
// 16-bit ADC chain (the type checker promotes `y` to `int` for the add)
// as
//
//     TYA              ; A = y
//     CLC
//     ADC #$23         ; A = y + 35 (low byte)
//     TAY              ; y = A (truncated back to uchar)
//     LDA #$00         ; A = 0, the high byte of the
//     ADC #$00         ; A = 0+0+C, promoted int's sum, never stored
//     JMP .if_end@0
//
// The `LDA #$00; ADC #$00` computes the high byte of the promoted sum,
// but the result is never stored, since the uchar truncates it back to
// one byte, and A is written downstream before anything reads it. Both
// instructions can be dropped.
//
// Eligibility
//
// An instruction can be dropped when
//
//   * its only effect is on A: no write to memory, no write to another
//     register, no control flow other than falling through, and no
//     helper call;
//   * its emission has no secondary effect. A `Frame`, `Stack`,
//     `Indirect`, or `IndirectY` operand is written with an `LDY #off`
//     setup that changes Y, so only `Imm`, `Data`, `ZP`, and the
//     transfers TXA and TYA qualify;
//   * A is dead after it (`asm_liveness.a_dead_at`, a walk over the
//     whole function);
//   * the flags are dead after it (`asm_liveness.flags_dead_at`). For
//     TXA and TYA the pass also accepts N and Z that already reflect the
//     source register's value (`flags_reflect_src_reg`), as described
//     below.
//
// The kinds it handles are
//
//   * `Mov(Imm | Data | ZP | Reg(X|Y), Reg(A))`, which is LDA of an
//     immediate or a byte, TXA, or TYA, and writes A, N, and Z;
//   * `Add`, `Sub`, `And`, and `Or` with a source of `Imm`, `Data`, or
//     `ZP` and `dst = Reg(A)`, which is ADC, SBC, AND, or ORA, and reads
//     A and writes A and the flags;
//   * `Xor` with `src1` and `src2` each `Imm`, `Data`, `ZP`, or `Reg(A)`
//     and `dst = Reg(A)`, which is EOR and writes A, N, and Z;
//   * `ArithmeticShiftLeft`, `LogicalShiftRight`, `RotateLeft`, and
//     `RotateRight` of A, which write A, N, Z, and C, so that like ADC
//     and SBC they need every flag dead.
//
// Dropping a TXA or TYA whose flags are redundant
//
// A TXA whose A is dead but whose flags a later branch on N or Z reads
// fails the flags test, since `flags_dead_at` stops at the branch. When
// the last instruction before the TXA to set the flags wrote the same
// register, as in `LDX M; TXA; BMI L`, the LDX set N and Z from the
// value X still holds, so the TXA sets the flags they already have.
// Dropping it leaves the branch reading the flags of the LDX, which are
// the same. That is sound when A is dead at i+1 and
// `flags_reflect_src_reg` finds that the last flag-setter on the
// straight-line path back wrote the register the TXA reads.
//
// This covers beam_target_tick's `LDX beam_snd_ctr; TXA; BMI .if_end@0`,
// where A is written on both arms (an LDA on the fall-through and a TXA
// at the branch target) and BMI reads the flags that the LDX already set
// from beam_snd_ctr.
//
// Iteration
//
// The pass runs inside `_peephole_fixedpoint`, and each function is also
// walked again until a walk drops nothing. In the case above the first
// walk drops the `ADC #$00`, whose A is dead, and the second drops the
// `LDA #$00`, whose A is dead once the ADC that read it is gone.
#include "passes/dead_a_arith.h"

#include <utility>
#include <vector>

namespace c6502::passes::dead_a_arith {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

namespace {

// Returns whether `op` is the register `reg`.
bool is_this_reg(const ir::Type_operand& op, const ir::Type_reg& reg) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && r->reg.index() == reg.index();
}

}  // namespace

bool flags_reflect_src_reg(Instrs instrs, std::size_t i) {
  const auto* mov = as<ir::Mov>(instrs[i]);
  if (mov == nullptr || !is_reg_a(mov->dst)) {
    return false;
  }
  const auto* src = as<ir::Reg>(mov->src);
  if (src == nullptr || !isa<ir::X, ir::Y>(src->reg)) {
    return false;
  }
  for (std::size_t j = i; j-- > 0;) {
    const ir::Type_instruction& prev = instrs[j];
    if (isa<ir::Label, ir::Jump, ir::Branch, ir::Ret, ir::Return, ir::Call,
            ir::FunctionPrologue, ir::AllocateStack, ir::LoadAddress>(prev)) {
      return false;
    }
    if (!sets_flags(prev)) {
      continue;
    }
    return writes_reg_setting_n_z(prev, src->reg);
  }
  return false;
}

bool writes_reg_setting_n_z(const ir::Type_instruction& instr,
                            const ir::Type_reg& reg) {
  if (const auto* m = as<ir::Mov>(instr)) {
    return is_this_reg(m->dst, reg);
  }
  if (const auto* i = as<ir::Inc>(instr)) {
    return is_this_reg(i->dst, reg);
  }
  if (const auto* d = as<ir::Dec>(instr)) {
    return is_this_reg(d->dst, reg);
  }
  if (isa<ir::Add, ir::Sub, ir::And, ir::Or, ir::Xor>(instr)) {
    // ADC / SBC / AND / ORA / EOR always write A.
    return isa<ir::A>(reg);
  }
  if (const auto* s = as<ir::ArithmeticShiftLeft>(instr)) {
    return is_this_reg(s->dst, reg);
  }
  if (const auto* s = as<ir::LogicalShiftRight>(instr)) {
    return is_this_reg(s->dst, reg);
  }
  if (const auto* s = as<ir::RotateLeft>(instr)) {
    return is_this_reg(s->dst, reg);
  }
  if (const auto* s = as<ir::RotateRight>(instr)) {
    return is_this_reg(s->dst, reg);
  }
  if (const auto* p = as<ir::Pop>(instr)) {
    return is_this_reg(p->dst, reg);
  }
  return false;
}

}  // namespace c6502::passes::dead_a_arith

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

// Returns whether `op` is `Reg(X)` or `Reg(Y)`, the sources of the
// transfers TXA and TYA.
bool is_xy_reg(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::X, ir::Y>(r->reg);
}

// Returns whether `op`, as the source of a load or of arithmetic, is
// emitted without an `LDY` or another change to a register: `Imm`,
// `ImmLabelLow`, `ImmLabelHigh`, `Data`, `ZP`, and `IndexedData` are
// single opcodes. `Frame`, `Stack`, `Indirect`, and `IndirectY` are
// emitted with an `LDY #off` before them, which changes Y.
bool is_pure_source(const ir::Type_operand& op) {
  return isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh, ir::Data, ir::ZP,
             ir::IndexedData>(op);
}

// Returns whether `instrs[i]` is a load into X or Y (`LDX` / `LDY` of an
// immediate or of a `Data` or `ZP` byte that isn't volatile) whose value
// and N/Z nothing reads. A constant store through Y
// (`redundant_load._store_const_via_index`) leaves one when a later pass
// removes its store.
bool is_dead_index_load(Instrs instrs, const LabelMap& labels, std::size_t i) {
  const auto* ins = as<ir::Mov>(instrs[i]);
  if (ins == nullptr || ins->is_volatile || !is_xy_reg(ins->dst) ||
      !isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh, ir::Data, ir::ZP>(
          ins->src)) {
    return false;
  }
  const bool dead = isa<ir::X>(std::get<ir::Reg>(ins->dst).reg)
                        ? x_dead_at(instrs, labels, i + 1)
                        : y_dead_at(instrs, labels, i + 1);
  return dead && flags_dead_at(instrs, labels, i + 1);
}

// Returns whether `instrs[i]` is a load into A through a zero-page
// pointer, not volatile, whose value and N/Z nothing reads. `(zp),Y`
// with a constant index (`IndirectZp`) sets Y with `LDY #k` first, so Y
// has to be dead too, and `(zp),Y` with Y as it is (`IndirectZpY`)
// writes only A. `X1 = (uint8_t)((uint16_t)Q->name >> 8)` read the low
// byte of the member as well.
bool is_dead_pointer_load(Instrs instrs, const LabelMap& labels, std::size_t i) {
  const auto* ins = as<ir::Mov>(instrs[i]);
  if (ins == nullptr || ins->is_volatile || !is_reg_a(ins->dst) ||
      !isa<ir::IndirectZp, ir::IndirectZpY>(ins->src)) {
    return false;
  }
  if (!(a_dead_at(instrs, labels, i + 1) && flags_dead_at(instrs, labels, i + 1))) {
    return false;
  }
  return isa<ir::IndirectZpY>(ins->src) || y_dead_at(instrs, labels, i + 1);
}

// Returns whether `instr` is ASL, LSR, ROL, or ROR. Each writes its
// operand, N, Z, and C.
bool is_shift(const ir::Type_instruction& instr) {
  return isa<ir::ArithmeticShiftLeft, ir::LogicalShiftRight, ir::RotateLeft,
             ir::RotateRight>(instr);
}

// Returns whether the only effects of `instr` are writes to A and to
// the N, Z, C, and V flags: no write to memory, no change to another
// register from the emission of an operand, and no control flow.
bool writes_only_a_and_flags(const ir::Type_instruction& instr) {
  if (const auto* m = as<ir::Mov>(instr)) {
    // A volatile move comes from an access to a volatile lvalue, and
    // its read of memory is an effect even when A and the flags are
    // dead afterward, so it is never dropped.
    if (m->is_volatile || !is_reg_a(m->dst)) {
      return false;
    }
    return is_pure_source(m->src) || is_xy_reg(m->src);
  }
  if (const auto* a = as<ir::Add>(instr)) {
    return is_reg_a(a->dst) && is_pure_source(a->src);
  }
  if (const auto* s = as<ir::Sub>(instr)) {
    return is_reg_a(s->dst) && is_pure_source(s->src);
  }
  if (const auto* a = as<ir::And>(instr)) {
    return is_reg_a(a->dst) && is_pure_source(a->src);
  }
  if (const auto* o = as<ir::Or>(instr)) {
    return is_reg_a(o->dst) && is_pure_source(o->src);
  }
  if (const auto* x = as<ir::Xor>(instr)) {
    if (!is_reg_a(x->dst)) {
      return false;
    }
    for (const ir::Type_operand* s : {&x->src1, &x->src2}) {
      if (!is_reg_a(*s) && !is_pure_source(*s)) {
        return false;
      }
    }
    return true;
  }
  if (const auto* s = as<ir::ArithmeticShiftLeft>(instr)) {
    return is_reg_a(s->dst);
  }
  if (const auto* s = as<ir::LogicalShiftRight>(instr)) {
    return is_reg_a(s->dst);
  }
  if (const auto* s = as<ir::RotateLeft>(instr)) {
    return is_reg_a(s->dst);
  }
  if (const auto* s = as<ir::RotateRight>(instr)) {
    return is_reg_a(s->dst);
  }
  return false;
}

// Returns whether the walk drops the instruction at `i`.
bool droppable(Instrs instrs, const LabelMap& labels, std::size_t i) {
  if (is_dead_index_load(instrs, labels, i) ||
      is_dead_pointer_load(instrs, labels, i)) {
    return true;
  }
  const ir::Type_instruction& instr = instrs[i];
  if (!writes_only_a_and_flags(instr)) {
    return false;
  }
  const std::size_t after = i + 1;
  if (!a_dead_at(instrs, labels, after)) {
    return false;
  }
  // Add, Sub, and the shifts also write C (and Add and Sub write V).
  // Dropping one must not strand a later reader of C, such as the next
  // SBC of a multi-byte chain that passes the carry on without a CLC or
  // SEC in between. The other kinds (LDA, TXA, TYA, AND, ORA, EOR) leave
  // C and V as they were, so only N and Z matter. Each flag is tested
  // on its own, since a later `BEQ` after a load reads only the Z the
  // load set and leaves C and V dead.
  const bool add_or_sub = isa<ir::Add, ir::Sub>(instr);
  if (add_or_sub || is_shift(instr)) {
    return flags_dead_at(instrs, labels, after) &&
           c_dead_at(instrs, labels, after) &&
           (!add_or_sub || v_dead_at(instrs, labels, after));
  }
  if (flags_dead_at(instrs, labels, after)) {
    return true;
  }
  // The fallback for TXA and TYA: N and Z already reflect the source
  // register's value (set by an earlier LDX, LDY, INX, and so on), so
  // the transfer's effect on them is redundant. A load of an immediate
  // or a byte and the ALU operations have no such case, since their
  // flags come from the value loaded or computed and not from a
  // register's current value.
  return dead_a_arith::flags_reflect_src_reg(instrs, i);
}

// Repeats the walk that drops instructions until one drops nothing. A
// walk catches the last instruction of a chain (the ADC #0 of
// `LDA #0; ADC #0`), and the next walk catches the LDA that the ADC's
// read of A kept alive. Without the repetition, a later pass such as the
// rewrite of a volatile read to a CMP can extend A's liveness across an
// instruction that could still be dropped before this pass runs again.
void rewrite_function(ir::Function& fn) {
  auto& instrs = fn.instructions;
  while (true) {
    const LabelMap labels(instrs);
    std::vector<bool> drop(instrs.size(), false);
    bool any = false;
    for (std::size_t i = 0; i < instrs.size(); ++i) {
      if (droppable(instrs, labels, i)) {
        drop[i] = true;
        any = true;
      }
    }
    if (!any) {
      return;
    }
    std::vector<ir::Type_instruction> out;
    out.reserve(instrs.size());
    for (std::size_t i = 0; i < instrs.size(); ++i) {
      if (!drop[i]) {
        out.push_back(std::move(instrs[i]));
      }
    }
    instrs = std::move(out);
  }
}

}  // namespace

asm_ast::Program apply_dead_a_arith_elimination(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
