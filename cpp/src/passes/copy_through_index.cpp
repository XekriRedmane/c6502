#include "passes/copy_through_index.h"

#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

#include "passes/asm_dead_store.h"
#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using Instructions = std::vector<ir::Type_instruction>;

bool is_reg_a(const ir::Type_operand& op) {
  const auto* reg = as<ir::Reg>(op);
  return reg != nullptr && isa<ir::A>(reg->reg);
}

bool index_loadable(const ir::Type_operand& op) {
  return isa<ir::Data, ir::ZP, ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(op);
}

bool index_storable(const ir::Type_operand& op) {
  return isa<ir::Data, ir::ZP>(op);
}

bool is_constant(const ir::Type_operand& op) {
  return isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(op);
}

std::optional<std::size_t> copies_end(const Instructions& instrs,
                                      std::size_t start,
                                      const ir::Type_operand& t) {
  int loads = 0;
  for (std::size_t k = start; k < instrs.size(); ++k) {
    const auto* mov = as<ir::Mov>(instrs[k]);
    if (mov == nullptr || mov->src == t || mov->dst == t) {
      if (mov != nullptr && mov->src == t && is_reg_a(mov->dst) &&
          !mov->is_volatile && loads != 0) {
        return k;
      }
      return std::nullopt;
    }
    if (is_reg_a(mov->dst)) {
      if (!index_loadable(mov->src)) {
        return std::nullopt;
      }
      loads += 1;
    } else if (is_reg_a(mov->src)) {
      if (!index_storable(mov->dst)) {
        return std::nullopt;
      }
    } else if (index_loadable(mov->src) && index_storable(mov->dst) &&
               !mov->is_volatile) {
      loads += 1;
    } else {
      return std::nullopt;
    }
  }
  return std::nullopt;
}

Instructions through_index(const Instructions& body, std::size_t begin,
                           std::size_t end, const ir::Type_operand& reg) {
  Instructions out;
  std::optional<ir::Type_operand> held;
  for (std::size_t k = begin; k < end; ++k) {
    const auto& ins = std::get<ir::Mov>(body[k]);
    if (is_reg_a(ins.dst) || !is_reg_a(ins.src)) {
      if (!(is_constant(ins.src) && held && ins.src == *held)) {
        out.push_back(ir::Mov{.src = ins.src, .dst = reg, .is_volatile = ins.is_volatile});
      }
      held = ins.src;
    }
    if (!is_reg_a(ins.dst)) {
      out.push_back(ir::Mov{
          .src = held ? reg : ir::Type_operand{ir::Reg{.reg = ir::A{}}},
          .dst = ins.dst,
          .is_volatile = ins.is_volatile});
    }
  }
  return out;
}

void copy_through_index_fn(ir::Function& fn, const AsmContext& ctx,
                           const SymbolAddrs* zp_slot_symbols) {
  Instructions instrs = fn.instructions;
  const std::string prefix = "__local_" + fn.name + "__";
  bool changed = false;
  std::size_t i = 0;
  while (i + 3 < instrs.size()) {
    const auto* park = as<ir::Mov>(instrs[i]);
    const ir::Data* t =
        park != nullptr ? as<ir::Data>(park->dst) : nullptr;
    if (!(park != nullptr && !park->is_volatile && is_reg_a(park->src) &&
          t != nullptr && t->name.starts_with(prefix))) {
      i += 1;
      continue;
    }
    const ir::Type_operand target = park->dst;
    const auto back = copies_end(instrs, i + 1, target);
    if (!back) {
      i += 1;
      continue;
    }
    const std::size_t after = *back + 1;
    if (!(flags_dead_at(instrs, after) &&
          asm_dead_store::mem_dead_after(instrs, *back, target, ctx, fn.name,
                                         zp_slot_symbols))) {
      i += 1;
      continue;
    }
    ir::Type_operand reg;
    if (x_dead_at(instrs, i)) {
      reg = ir::Reg{.reg = ir::X{}};
    } else if (y_dead_at(instrs, i)) {
      reg = ir::Reg{.reg = ir::Y{}};
    } else {
      i += 1;
      continue;
    }
    Instructions copies = through_index(instrs, i + 1, *back, reg);
    instrs.erase(instrs.begin() + static_cast<long>(i),
                 instrs.begin() + static_cast<long>(after));
    instrs.insert(instrs.begin() + static_cast<long>(i), copies.begin(),
                  copies.end());
    changed = true;
    i = *back - 1;
  }
  if (changed) {
    fn.instructions = std::move(instrs);
  }
}

}  // namespace

asm_ast::Program apply_copy_through_index(asm_ast::Program prog,
                                          const AsmContext& ctx,
                                          const SymbolAddrs* zp_slot_symbols) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      copy_through_index_fn(*fn, ctx, zp_slot_symbols);
    }
  }
  return prog;
}

}  // namespace c6502::passes
