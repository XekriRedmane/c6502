// Drops the load of an adjacent `STA M; LDA M` round trip when A
// already holds M and the instruction before the store set the flags
// from A's value. Also rewrites the variant where the load of M is
// hidden inside a move from memory to memory, `Mov(M, dst_mem)`.
//
// Pattern A, an explicit LDA
//
// Three consecutive instructions:
//
//     [i-1]   <writes Reg(A) and sets N/Z from A's new value>
//     [i]     Mov(Reg(A), <mem M>)    // STA M
//     [i+1]   Mov(<mem M>, Reg(A))    // LDA M of the same M
//
// The pass drops [i+1].
//
// Pattern B, a move from memory to memory
//
// The same first two instructions, with [i+1] a move `Mov(M, dst)`,
// which is emitted as `LDA M; STA dst`:
//
//     [i-1]   <writes Reg(A) and sets N/Z from A's new value>
//     [i]     Mov(Reg(A), <mem M>)    // STA M
//     [i+1]   Mov(<mem M>, <dst_mem>) // LDA M; STA dst
//
// The pass rewrites [i+1] to `Mov(Reg(A), <dst_mem>)`, which drops the
// hidden LDA. That saves the bytes and cycles of one load and keeps A's
// value.
//
// Pattern B is the only way to catch this redundancy in the IR.
// `redundant_load_elimination`'s tracker of A drops explicit `LDA M`
// atoms once it knows A holds M, but a move from memory to memory is
// one atom whose `LDA M` no peephole on instructions can see. The gate
// on the flags that makes Pattern A sound makes Pattern B sound too.
// The rewritten `Mov(Reg(A), dst)` is emitted as an `STA dst`, which
// leaves the flags alone, while the original `LDA M; STA dst` set N/Z
// from M, which equals A after [i], so the flags come out the same.
//
// Pattern C, flags from A
//
// When the instruction before the `STA M` didn't set N/Z from A (a
// `JSR`, say), the `LDA M` is there for its flags. `ORA #$00` sets the
// same N/Z from A, which equals M, and like the load leaves C and V
// alone, in 2 cycles where the load takes 3 or 4:
//
//     Mov(Reg(A), <mem M>)    // STA M
//     Mov(<mem M>, Reg(A))    // LDA M
//
// becomes
//
//     Mov(Reg(A), <mem M>)    // STA M
//     Or(Imm(0), Reg(A))      // ORA #$00
//
// `print_zstring` (examples/zork/zstring.c) tests the alphabet that
// `get_alphabet` returns this way, `JSR get_alphabet; ORA #$00; BNE`,
// as Zork's code does. The store is then dead, and `asm_dead_store`
// drops it. When nothing reads the flags, the load goes with no `ORA`.
// A volatile M is left alone.
//
// Soundness
//
// After [i-1], A holds some value V, N = bit7(V), and Z = (V == 0).
// After [i] (STA M), A = V and M = V, with the flags unchanged. After
// [i+1] (LDA M), A = M = V, N = bit7(V), and Z = (V == 0). So [i+1]
// changes nothing a reader could see: A is unchanged, and the flags
// already held what [i+1] would set. Dropping it is sound whether or
// not a later instruction reads A or the flags.
//
// `redundant_load_elimination` recognizes the same state of A but
// gives up when the flags are live afterward, because in general the
// load's flags could differ from those the previous instruction set.
// This pass requires that the instruction before the store set the
// flags from A's value, which closes that gap.
//
// The instruction before the store ([i-1])
//
// It qualifies when its destination is `Reg(A)` and it sets N/Z from
// A's new value:
//
//   * `Mov(<any>, Reg(A))`, which is LDA imm, LDA M, TXA, or TYA.
//   * `And(_, Reg(A))`, `Or(_, Reg(A))`, `Add(_, Reg(A))`, and
//     `Sub(_, Reg(A))`, which are AND, ORA, ADC, and SBC.
//   * `Xor(_, _, Reg(A))`, which is EOR.
//   * `Pop(dst=Reg(A))`, which is PLA and sets N/Z from the byte pulled.
//   * `ArithmeticShiftLeft(Reg(A))`, `LogicalShiftRight(Reg(A))`,
//     `RotateLeft(Reg(A))`, and `RotateRight(Reg(A))`.
//
// A `Compare` leaves N/Z from the subtraction, so it doesn't qualify.
// Neither does an `Inc(M)`, `Dec(M)`, or shift of memory, which set
// the flags from memory, nor a `Branch`, `Jump`, `Label`, or `Call`,
// which end a block, nor `ClearCarry` or `SetCarry`, which leave N/Z
// alone.
//
// Where it runs
//
// Inside the peephole fixed point, before the `asm_dead_store` step of
// the next sweep, so that the `STA M` left alone can be removed if
// nothing else reads M.
#include "passes/round_trip_load.h"

#include <utility>
#include <vector>

#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

// Returns whether `instr` writes Reg(A) and its effect on N/Z follows
// A's new value.
bool writes_a_with_flag_effect(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::Mov:
      // LDA, TXA, or TYA, which set the flags from the value loaded.
      return is_reg_a(std::get<ir::Mov>(instr).dst);
    case ir::Kind_instruction::And:
      return is_reg_a(std::get<ir::And>(instr).dst);
    case ir::Kind_instruction::Or:
      return is_reg_a(std::get<ir::Or>(instr).dst);
    case ir::Kind_instruction::Add:
      return is_reg_a(std::get<ir::Add>(instr).dst);
    case ir::Kind_instruction::Sub:
      return is_reg_a(std::get<ir::Sub>(instr).dst);
    case ir::Kind_instruction::Xor:
      return is_reg_a(std::get<ir::Xor>(instr).dst);
    case ir::Kind_instruction::Pop:
      return is_reg_a(std::get<ir::Pop>(instr).dst);
    case ir::Kind_instruction::ArithmeticShiftLeft:
      return is_reg_a(std::get<ir::ArithmeticShiftLeft>(instr).dst);
    case ir::Kind_instruction::LogicalShiftRight:
      return is_reg_a(std::get<ir::LogicalShiftRight>(instr).dst);
    case ir::Kind_instruction::RotateLeft:
      return is_reg_a(std::get<ir::RotateLeft>(instr).dst);
    case ir::Kind_instruction::RotateRight:
      return is_reg_a(std::get<ir::RotateRight>(instr).dst);
    default:
      return false;
  }
}

bool operands_equal(const ir::Type_operand& a, const ir::Type_operand& b) {
  if (const auto* da = as<ir::Data>(a)) {
    const auto* db = as<ir::Data>(b);
    return db != nullptr && da->name == db->name && da->offset == db->offset;
  }
  if (const auto* za = as<ir::ZP>(a)) {
    const auto* zb = as<ir::ZP>(b);
    return zb != nullptr && za->address == zb->address && za->offset == zb->offset;
  }
  return false;
}

// Returns the store when `instr` is `Mov(Reg(A), <ZP | Data>)` and isn't
// volatile, or null. Only a byte with a fixed address is matched,
// since those are the operands a later LDA can be recognized to read.
const ir::Mov* as_sta(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  if (m != nullptr && !m->is_volatile && is_reg_a(m->src) &&
      isa<ir::ZP, ir::Data>(m->dst)) {
    return m;
  }
  return nullptr;
}

// Returns whether `instr` is `Mov(<mem>, Reg(A))` with the same source
// as `sta`'s destination, and isn't a volatile load, which reads the
// byte again each time.
bool is_lda_same_addr(const ir::Type_instruction& instr, const ir::Mov& sta) {
  const auto* m = as<ir::Mov>(instr);
  return m != nullptr && !m->is_volatile && is_reg_a(m->dst) &&
         operands_equal(m->src, sta.dst);
}

// Returns whether `instr` is a move from memory to memory, `Mov(M,
// dst_mem)`, whose source is `sta`'s destination, whose destination
// isn't a register, and which isn't volatile, since a volatile read
// has to happen.
bool is_mem_to_mem_from(const ir::Type_instruction& instr, const ir::Mov& sta) {
  const auto* m = as<ir::Mov>(instr);
  return m != nullptr && !m->is_volatile && !isa<ir::Reg>(m->dst) &&
         operands_equal(m->src, sta.dst);
}

void rewrite_function(ir::Function& fn) {
  const std::vector<ir::Type_instruction>& instrs = fn.instructions;
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    // Looks at the window of three instructions [i, i+1, i+2].
    if (i + 2 < instrs.size() && writes_a_with_flag_effect(instrs[i])) {
      if (const ir::Mov* sta = as_sta(instrs[i + 1])) {
        const ir::Type_instruction& third = instrs[i + 2];
        if (is_lda_same_addr(third, *sta)) {
          // Pattern A drops the explicit `LDA M`.
          out.push_back(instrs[i]);
          out.push_back(instrs[i + 1]);
          i += 3;
          changed = true;
          continue;
        }
        if (is_mem_to_mem_from(third, *sta)) {
          // Pattern B rewrites `Mov(M, dst_mem)` to `Mov(Reg(A),
          // dst_mem)`, which drops the hidden `LDA M`.
          const auto& mov = std::get<ir::Mov>(third);
          out.push_back(instrs[i]);
          out.push_back(instrs[i + 1]);
          out.push_back(ir::Mov{.src = ir::Reg{.reg = ir::A{}},
                                .dst = mov.dst,
                                .is_volatile = mov.is_volatile});
          i += 3;
          changed = true;
          continue;
        }
      }
    }
    if (i + 1 < instrs.size()) {
      if (const ir::Mov* sta = as_sta(instrs[i]);
          sta != nullptr && is_lda_same_addr(instrs[i + 1], *sta)) {
        // Pattern C keeps the load's flags, if anything reads them.
        out.push_back(instrs[i]);
        if (!flags_dead_at(instrs, i + 2)) {
          out.push_back(ir::Or{.src = ir::Imm{.value = 0},
                               .dst = ir::Reg{.reg = ir::A{}}});
        }
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

asm_ast::Program apply_round_trip_load_drop(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
