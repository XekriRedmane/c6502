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

// DEC has zero-page, absolute, zp,X, and abs,X forms, and no abs,Y or
// indirect form.
bool is_dec_eligible_operand(const ir::Type_operand& op) {
  if (isa<ir::Data, ir::ZP>(op) || is_x_indexed(op)) {
    return true;
  }
  if (const auto* r = as<ir::Reg>(op)) {
    return isa<ir::X, ir::Y>(r->reg);
  }
  return false;
}

// A move into A, ADC, SBC, AND, ORA, EOR, the shifts and rotates, and
// PLA set N and Z from the value they write.
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

// DEC sets only N and Z. ADC, SBC, ROL, and ROR read C. CMP, ASL, LSR,
// SEC, and CLC write C without reading it. LDA, STA, the transfers,
// AND, ORA, EOR, INC, DEC, PHA, PLA, and BIT leave C as it was.
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
          return true;
        }
        const long t = labels.find(br->target);
        if (t >= 0) {
          stack.push_back(static_cast<std::size_t>(t));
        }
        ++j;
        continue;
      }
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
      break;
    }
  }
  return false;
}

// An NMOS 6502's DEC writes the old value back before the new one, so
// a volatile byte keeps its separate load and store.
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
  if (c_or_v_read_before_redefined(instrs, labels, start + 4)) {
    return nullptr;
  }
  return &i0->src;
}

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
