#include "passes/cpx_cpy_peephole.h"

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

bool is_reg_x_or_y(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::X, ir::Y>(r->reg);
}

// CPX and CPY have only immediate, absolute, and zero-page modes.
std::optional<ir::Type_instruction> try_fuse(const ir::Type_instruction& first,
                                             const ir::Type_instruction& second,
                                             Instrs instrs, const LabelMap& labels,
                                             std::size_t after_idx) {
  const auto* transfer = as<ir::Mov>(first);
  if (transfer == nullptr || !is_reg_a(transfer->dst) || !is_reg_x_or_y(transfer->src)) {
    return std::nullopt;
  }
  const auto* cmp = as<ir::Compare>(second);
  if (cmp == nullptr || !is_reg_a(cmp->left) ||
      !isa<ir::Imm, ir::Data, ir::ZP>(cmp->right)) {
    return std::nullopt;
  }
  if (!a_dead_at(instrs, labels, after_idx)) {
    return std::nullopt;
  }
  return ir::Compare{.left = transfer->src, .right = cmp->right};
}

std::optional<ir::Type_instruction> try_fuse_through_tmp(const ir::Function& fn,
                                                         Instrs instrs,
                                                         const LabelMap& labels,
                                                         std::size_t i,
                                                         const AsmContext& ctx) {
  const auto* load = as<ir::Mov>(instrs[i]);
  const auto* store = as<ir::Mov>(instrs[i + 1]);
  const auto* cmp = as<ir::Compare>(instrs[i + 2]);
  if (!(load != nullptr && is_reg_a(load->dst) &&
        isa<ir::Imm, ir::Data, ir::ZP>(load->src))) {
    return std::nullopt;
  }
  if (!(store != nullptr && is_reg_a(store->src) && !store->is_volatile &&
        isa<ir::Data, ir::ZP>(store->dst))) {
    return std::nullopt;
  }
  if (!(cmp != nullptr && is_reg_x_or_y(cmp->left) && cmp->right == store->dst)) {
    return std::nullopt;
  }
  if (load->src == store->dst) {
    return std::nullopt;
  }
  if (!a_dead_at(instrs, labels, i + 3)) {
    return std::nullopt;
  }
  if (!asm_dead_store::mem_dead_after(instrs, labels, i + 2, store->dst, ctx, fn.name)) {
    return std::nullopt;
  }
  return ir::Compare{.left = cmp->left, .right = load->src};
}

void rewrite_function(ir::Function& fn, const AsmContext& ctx) {
  auto& instrs = fn.instructions;
  if (instrs.size() < 2) {
    return;
  }
  const LabelMap labels(instrs);
  std::vector<std::optional<ir::Type_instruction>> rewrites(instrs.size());
  std::vector<bool> drop(instrs.size(), false);
  bool any = false;
  for (std::size_t i = 0; i + 1 < instrs.size(); ++i) {
    if (drop[i] || rewrites[i]) {
      continue;
    }
    if (i + 2 < instrs.size()) {
      if (auto fused = try_fuse_through_tmp(fn, instrs, labels, i, ctx)) {
        drop[i] = true;
        drop[i + 1] = true;
        rewrites[i + 2] = std::move(fused);
        any = true;
        continue;
      }
    }
    if (auto fused = try_fuse(instrs[i], instrs[i + 1], instrs, labels, i + 2)) {
      drop[i] = true;
      rewrites[i + 1] = std::move(fused);
      any = true;
    }
  }
  if (!any) {
    return;
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (drop[i]) {
      continue;
    }
    out.push_back(rewrites[i] ? std::move(*rewrites[i]) : std::move(instrs[i]));
  }
  instrs = std::move(out);
}

}  // namespace

asm_ast::Program apply_cpx_cpy_peephole(asm_ast::Program prog, const AsmContext& ctx) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, ctx);
    }
  }
  return prog;
}

}  // namespace c6502::passes
