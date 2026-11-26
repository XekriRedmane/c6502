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

// The bit of `Fact::byte` that C holds: bit 7 after `ASL` or `ROL A`,
// and bit 0 after `LSR` or `ROR A`.
enum class Kind { kNone, kBit7, kBit0 };

struct Fact {
  Kind kind = Kind::kNone;
  ir::Type_operand byte;
};

bool is_mem(const ir::Type_operand& op) { return isa<ir::Data, ir::ZP>(op); }

bool distinct(const ir::Type_operand& a, const ir::Type_operand& b) {
  return a.index() == b.index() && a != b;
}

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
    return Kind::kBit7;
  }
  if (const auto* s = as<ir::RotateLeft>(ins); s && is_reg_a(s->dst)) {
    return Kind::kBit7;
  }
  if (const auto* s = as<ir::LogicalShiftRight>(ins); s && is_reg_a(s->dst)) {
    return Kind::kBit0;
  }
  if (const auto* s = as<ir::RotateRight>(ins); s && is_reg_a(s->dst)) {
    return Kind::kBit0;
  }
  return Kind::kNone;
}

bool keeps_c(const ir::Type_instruction& ins) {
  return isa<ir::Mov, ir::Inc, ir::Dec, ir::And, ir::Or, ir::Xor, ir::Push,
             ir::Pop, ir::BitTest, ir::Branch>(ins);
}

bool writes_a(const ir::Type_instruction& ins) {
  if (const auto* m = as<ir::Mov>(ins)) {
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
