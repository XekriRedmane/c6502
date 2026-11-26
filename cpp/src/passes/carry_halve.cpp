#include "passes/carry_halve.h"

#include <cstddef>
#include <iterator>
#include <utility>
#include <vector>

#include "passes/asm_dead_store.h"
#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};
const ir::Type_operand kZero = ir::Imm{.value = 0};

bool holds_zero(const std::vector<ir::Type_instruction>& instrs, std::size_t idx,
                const ir::Type_operand& reg) {
  const bool is_x = isa<ir::X>(std::get<ir::Reg>(reg).reg);
  for (std::size_t k = idx; k-- > 0;) {
    const ir::Type_instruction& ins = instrs[k];
    if (const auto* m = as<ir::Mov>(ins); m != nullptr && m->dst == reg) {
      return m->src == kZero && !m->is_volatile;
    }
    if (isa<ir::Label, ir::Call, ir::Jump, ir::JumpTable, ir::Phi>(ins)) {
      return false;
    }
    if (const auto* x = as<ir::Inc>(ins); x != nullptr && x->dst == reg) {
      return false;
    }
    if (const auto* x = as<ir::Dec>(ins); x != nullptr && x->dst == reg) {
      return false;
    }
    if (is_x && kills_x(ins)) {
      return false;
    }
    if (!is_x && (kills_y(ins) || sets_up_y(ins))) {
      return false;
    }
  }
  return false;
}

bool is_zero_load(const std::vector<ir::Type_instruction>& instrs, std::size_t idx) {
  const auto* m = as<ir::Mov>(instrs[idx]);
  if (m == nullptr || m->dst != kA) {
    return false;
  }
  if (m->src == kZero) {
    return true;
  }
  const auto* r = as<ir::Reg>(m->src);
  return r != nullptr && isa<ir::X, ir::Y>(r->reg) && holds_zero(instrs, idx, m->src);
}

bool matches(const std::vector<ir::Type_instruction>& instrs, std::size_t i) {
  const auto* add = as<ir::Add>(instrs[i]);
  const auto* store = as<ir::Mov>(instrs[i + 1]);
  const auto* carry = as<ir::Add>(instrs[i + 3]);
  const auto* lsr = as<ir::LogicalShiftRight>(instrs[i + 4]);
  const auto* load = as<ir::Mov>(instrs[i + 5]);
  const auto* ror = as<ir::RotateRight>(instrs[i + 6]);
  return add != nullptr && add->dst == kA && store != nullptr && store->src == kA &&
         !store->is_volatile && isa<ir::Data, ir::ZP>(store->dst) &&
         is_zero_load(instrs, i + 2) && carry != nullptr && carry->src == kZero &&
         carry->dst == kA && lsr != nullptr && lsr->dst == kA && load != nullptr &&
         load->src == store->dst && load->dst == kA && !load->is_volatile &&
         ror != nullptr && ror->dst == kA;
}

void rewrite_function(ir::Function& fn, const AsmContext& ctx,
                      const SymbolAddrs* zp_slot_symbols) {
  auto& instrs = fn.instructions;
  std::size_t i = 0;
  while (i + 7 <= instrs.size()) {
    if (!(matches(instrs, i) && v_dead_at(instrs, i + 7) &&
          asm_dead_store::mem_dead_after(instrs, i + 5,
                                         std::get<ir::Mov>(instrs[i + 1]).dst, ctx,
                                         fn.name, zp_slot_symbols))) {
      ++i;
      continue;
    }
    const auto first = instrs.begin() + static_cast<std::ptrdiff_t>(i) + 1;
    instrs.erase(first, first + 5);
    i += 2;
  }
}

}  // namespace

asm_ast::Program apply_carry_halve(asm_ast::Program prog, const AsmContext& ctx,
                                   const SymbolAddrs* zp_slot_symbols) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, ctx, zp_slot_symbols);
    }
  }
  return prog;
}

}  // namespace c6502::passes
