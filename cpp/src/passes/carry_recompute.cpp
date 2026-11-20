// Drops a shift whose only use is to set C to what C already holds.
//
// The pattern
//
// `load_packed_address` in `examples/zork/pages.c` sets the 24-bit
// `Z_PC2` to twice `SCRATCH2`:
//
//     Z_PC2.lo = SCRATCH2 << 1;
//     Z_PC2.hi = (uint8_t)(SCRATCH2 >> 15);
//
// `tac_to_asm` lowers `x >> 15` as the bit shifted out of `x << 1`, so
// the second statement starts by putting the bit shifted out of the
// byte below into C again:
//
//     LDA   SCRATCH2
//     ASL
//     STA   Z_PC2
//     LDA   SCRATCH2+1
//     ROL                     ; C = bit 7 of SCRATCH2+1
//     STA   Z_PC2+1
//     LDA   SCRATCH2+1        ; C = bit 7 of SCRATCH2+1, again
//     ASL
//     LDA   #$00
//     ROL
//     STA   Z_PC2+2
//
// (`x >> 7` is likewise the high byte of `x << 1`, so taking the three
// bytes in three statements computed C twice more.) The pass tracks
// what C holds: bit 7 of a byte `M` after `LDA M` and `ASL A` or `ROL
// A`, and bit 0 after `LSR A` or `ROR A`. A later `LDA M; ASL A` (or
// `ROL A`, `LSR A`, `ROR A`) that would set C the same way, with A and
// N/Z dead after it, goes, which leaves Zork's own chain:
//
//     LDA   SCRATCH2
//     ASL
//     STA   Z_PC2
//     LDA   SCRATCH2+1
//     ROL
//     STA   Z_PC2+1
//     LDA   #$00
//     ROL
//     STA   Z_PC2+2
//
// What it tracks
//
// Within a basic block, since a label forgets everything, the pass
// tracks the byte A was last loaded from, while it is unchanged, and
// what C holds. A write to that byte, or to anything that might be it
// (a store through a pointer, or a `Data` byte for a `ZP` one), forgets
// the fact, and so does an instruction that writes C (`ADC`, `SBC`, a
// compare, a shift of memory, `SEC`, `CLC`, a call). A volatile load
// sets nothing.
//
// Where it runs
//
// In the asm peephole fixed point, which both pipelines run.
#include "passes/carry_recompute.h"

#include <concepts>
#include <cstddef>
#include <optional>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

enum class Kind { kNone, kHi, kLo };

// What C holds: bit 7 (`kHi`) or bit 0 (`kLo`) of `byte`.
struct Fact {
  Kind kind = Kind::kNone;
  ir::Type_operand byte;
};

bool is_mem(const ir::Type_operand& op) { return isa<ir::Data, ir::ZP>(op); }

// Returns whether `a` and `b` are of one kind of operand and differ,
// which for `Data` and `ZP` bytes means they are different bytes.
bool distinct(const ir::Type_operand& a, const ir::Type_operand& b) {
  return a.index() == b.index() && a != b;
}

// The memory `ins` writes: nothing, an operand, or memory it doesn't
// name (`kUnknown`).
struct Written {
  enum { kNothing, kOperand, kUnknown } what = kNothing;
  ir::Type_operand op;
};

const ir::Type_operand* dst_of(const ir::Type_instruction& ins) {
  return std::visit(
      [](const auto& x) -> const ir::Type_operand* {
        using T = std::remove_cvref_t<decltype(x)>;
        if constexpr (requires(const T& t) {
                        { t.dst } -> std::same_as<const ir::Type_operand&>;
                      }) {
          return &x.dst;
        } else {
          return nullptr;
        }
      },
      ins);
}

Written writes(const ir::Type_instruction& ins) {
  if (isa<ir::Call, ir::FunctionPrologue, ir::AllocateStack, ir::LoadAddress,
          ir::DecimalArith>(ins)) {
    return {Written::kUnknown, {}};
  }
  const ir::Type_operand* dst = dst_of(ins);
  if (dst == nullptr || isa<ir::Reg>(*dst)) {
    return {};
  }
  if (isa<ir::Mov, ir::Inc, ir::Dec, ir::ArithmeticShiftLeft, ir::RotateLeft,
          ir::LogicalShiftRight, ir::RotateRight>(ins)) {
    return {Written::kOperand, *dst};
  }
  return {};
}

Kind shift_kind(const ir::Type_instruction& ins) {
  if (const auto* s = as<ir::ArithmeticShiftLeft>(ins); s && is_reg_a(s->dst)) {
    return Kind::kHi;
  }
  if (const auto* s = as<ir::RotateLeft>(ins); s && is_reg_a(s->dst)) {
    return Kind::kHi;
  }
  if (const auto* s = as<ir::LogicalShiftRight>(ins); s && is_reg_a(s->dst)) {
    return Kind::kLo;
  }
  if (const auto* s = as<ir::RotateRight>(ins); s && is_reg_a(s->dst)) {
    return Kind::kLo;
  }
  return Kind::kNone;
}

// Returns whether `ins` leaves C alone. A `Mov` to memory may still
// change the byte a fact is about, which is checked separately.
bool keeps_c(const ir::Type_instruction& ins) {
  return isa<ir::Mov, ir::Inc, ir::Dec, ir::And, ir::Or, ir::Xor, ir::Push,
             ir::Pop, ir::BitTest, ir::Branch>(ins);
}

bool writes_a(const ir::Type_instruction& ins) {
  if (const auto* m = as<ir::Mov>(ins)) {
    // A store from anything but a register (`Mov(M, D)`, `Mov(#k, D)`)
    // is emitted as `LDA; STA` through A.
    return !isa<ir::Reg>(m->src);
  }
  if (const auto* x = as<ir::Add>(ins)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::Sub>(ins)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::And>(ins)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::Or>(ins)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::Xor>(ins)) return is_reg_a(x->dst);
  if (const auto* x = as<ir::Pop>(ins)) return is_reg_a(x->dst);
  return isa<ir::Call, ir::FunctionPrologue, ir::Ret, ir::AllocateStack,
             ir::LoadAddress, ir::DecimalArith>(ins);
}

void rewrite_function(ir::Function& fn) {
  const auto& instrs = fn.instructions;
  const LabelMap labels(instrs);
  std::vector<bool> drop(instrs.size(), false);
  bool any = false;
  std::optional<Fact> fact;
  std::optional<ir::Type_operand> a_src;
  std::size_t i = 0;
  while (i < instrs.size()) {
    const ir::Type_instruction& ins = instrs[i];
    // A load and shift that would set C to what it holds.
    if (const auto* m = as<ir::Mov>(ins);
        m != nullptr && is_reg_a(m->dst) && !m->is_volatile && is_mem(m->src) &&
        i + 1 < instrs.size()) {
      const Kind k = shift_kind(instrs[i + 1]);
      if (k != Kind::kNone && fact && fact->kind == k && fact->byte == m->src &&
          a_dead_at(instrs, labels, i + 2) && flags_dead_at(instrs, labels, i + 2)) {
        drop[i] = drop[i + 1] = true;
        any = true;
        i += 2;
        continue;
      }
    }
    if (isa<ir::Label, ir::Jump, ir::Return, ir::Ret>(ins)) {
      fact.reset();
      a_src.reset();
    } else if (const Kind kind = shift_kind(ins); kind != Kind::kNone) {
      if (a_src) {
        fact = Fact{kind, *a_src};
      } else {
        fact.reset();
      }
      a_src.reset();
    } else {
      if (!keeps_c(ins)) {
        fact.reset();
      }
      const Written written = writes(ins);
      if (written.what != Written::kNothing) {
        // The two bytes held, as they were before this instruction.
        std::optional<ir::Type_operand> held_list[2];
        if (fact) {
          held_list[0] = fact->byte;
        }
        held_list[1] = a_src;
        for (const auto& held : held_list) {
          if (held && (written.what == Written::kUnknown ||
                       !distinct(written.op, *held))) {
            if (fact && *held == fact->byte) {
              fact.reset();
            }
            if (*held == a_src) {
              a_src.reset();
            }
          }
        }
      }
      if (const auto* m = as<ir::Mov>(ins); m != nullptr && is_reg_a(m->dst)) {
        if (is_mem(m->src) && !m->is_volatile) {
          a_src = m->src;
        } else {
          a_src.reset();
        }
      } else if (writes_a(ins)) {
        a_src.reset();
      }
    }
    ++i;
  }
  if (!any) {
    return;
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (!drop[k]) {
      out.push_back(std::move(fn.instructions[k]));
    }
  }
  fn.instructions = std::move(out);
}

}  // namespace

asm_ast::Program apply_carry_recompute_drop(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
