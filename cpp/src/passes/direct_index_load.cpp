#include "passes/direct_index_load.h"

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

bool is_reg_x_or_y(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::X, ir::Y>(r->reg);
}

// `LDX M` sets N and Z from M as `LDA M; TAX` does.
std::optional<ir::Type_instruction> try_fuse(const ir::Type_instruction& first,
                                             const ir::Type_instruction& second,
                                             Instrs instrs, const LabelMap& labels,
                                             std::size_t after_idx) {
  const auto* load = as<ir::Mov>(first);
  if (load == nullptr || !is_reg_a(load->dst)) {
    return std::nullopt;
  }
  const auto* indexed = as<ir::IndexedData>(load->src);
  if (indexed == nullptr &&
      !isa<ir::Data, ir::ZP, ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(load->src)) {
    return std::nullopt;
  }
  const auto* transfer = as<ir::Mov>(second);
  if (transfer == nullptr || transfer->is_volatile || !is_reg_a(transfer->src) ||
      !is_reg_x_or_y(transfer->dst)) {
    return std::nullopt;
  }
  // LDX has an abs,Y mode and no abs,X mode, and LDY has abs,X and no
  // abs,Y.
  if (indexed != nullptr &&
      indexed->index.index() == std::get<ir::Reg>(transfer->dst).reg.index()) {
    return std::nullopt;
  }
  if (!a_dead_at(instrs, labels, after_idx)) {
    return std::nullopt;
  }
  return ir::Mov{.src = load->src, .dst = transfer->dst, .is_volatile = load->is_volatile};
}

void rewrite_function(ir::Function& fn) {
  auto& instrs = fn.instructions;
  if (instrs.size() < 2) {
    return;
  }
  const LabelMap labels(instrs);
  std::vector<std::optional<ir::Type_instruction>> rewrites(instrs.size());
  std::vector<bool> skipped(instrs.size(), false);
  bool any = false;
  for (std::size_t i = 0; i + 1 < instrs.size(); ++i) {
    if (skipped[i]) {
      continue;
    }
    if (auto fused = try_fuse(instrs[i], instrs[i + 1], instrs, labels, i + 2)) {
      rewrites[i] = std::move(fused);
      skipped[i + 1] = true;
      any = true;
    }
  }
  if (!any) {
    return;
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (skipped[i]) {
      continue;
    }
    out.push_back(rewrites[i] ? std::move(*rewrites[i]) : std::move(instrs[i]));
  }
  instrs = std::move(out);
}

}  // namespace

asm_ast::Program apply_direct_index_load(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
