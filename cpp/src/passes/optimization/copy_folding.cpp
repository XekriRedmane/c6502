#include "passes/optimization/copy_folding.h"

#include <optional>
#include <string>
#include <utility>

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using framework::Instrs;
using framework::PassContext;
using K = ir::Kind_instruction;

ir::Type_val* redirectable_dst(ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::SignExtend:
      return &std::get<ir::SignExtend>(instr).dst;
    case K::ZeroExtend:
      return &std::get<ir::ZeroExtend>(instr).dst;
    case K::Truncate:
      return &std::get<ir::Truncate>(instr).dst;
    case K::IntToFloat:
      return &std::get<ir::IntToFloat>(instr).dst;
    case K::IntToDouble:
      return &std::get<ir::IntToDouble>(instr).dst;
    case K::FloatToInt:
      return &std::get<ir::FloatToInt>(instr).dst;
    case K::DoubleToInt:
      return &std::get<ir::DoubleToInt>(instr).dst;
    case K::FloatToDouble:
      return &std::get<ir::FloatToDouble>(instr).dst;
    case K::DoubleToFloat:
      return &std::get<ir::DoubleToFloat>(instr).dst;
    case K::Unary:
      return &std::get<ir::Unary>(instr).dst;
    case K::Binary:
      return &std::get<ir::Binary>(instr).dst;
    case K::Copy:
      return &std::get<ir::Copy>(instr).dst;
    case K::GetAddress:
      return &std::get<ir::GetAddress>(instr).dst;
    case K::Load:
      return &std::get<ir::Load>(instr).dst;
    case K::IndexedLoad:
      return &std::get<ir::IndexedLoad>(instr).dst;
    case K::FunctionCall: {
      auto& d = std::get<ir::FunctionCall>(instr).dst;
      return d ? &*d : nullptr;
    }
    case K::IndirectCall: {
      auto& d = std::get<ir::IndirectCall>(instr).dst;
      return d ? &*d : nullptr;
    }
    default:
      return nullptr;
  }
}

tac_ast::Function fold_copies(tac_ast::Function fn, const PassContext& ctx) {
  framework::RuleEnv env = framework::make_rule_env(fn.instructions);
  const NameSet& observable = framework::observable_names(env, ctx);
  return framework::run_window(
      std::move(fn), 2, [&](const Instrs& instrs, std::size_t i) -> std::optional<Instrs> {
        const auto* copy = std::get_if<ir::Copy>(&instrs[i + 1]);
        if (copy == nullptr) {
          return std::nullopt;
        }
        const auto* src = std::get_if<ir::Var>(&copy->src);
        if (src == nullptr) {
          return std::nullopt;
        }
        const std::size_t* count = env.use_count.find(src->name);
        if (count == nullptr || *count != 1 || observable.contains(src->name)) {
          return std::nullopt;
        }
        ir::Type_instruction producer = instrs[i];
        ir::Type_val* dst = redirectable_dst(producer);
        const auto* dst_var = dst != nullptr ? std::get_if<ir::Var>(dst) : nullptr;
        if (dst_var == nullptr || dst_var->name != src->name) {
          return std::nullopt;
        }
        *dst = copy->dst;
        Instrs out;
        out.push_back(std::move(producer));
        return out;
      });
}

}  // namespace

tac_ast::Function fold_copies_in_fixedpoint(tac_ast::Function fn, const PassContext& ctx) {
  return fold_copies(std::move(fn), ctx);
}

tac_ast::Function fold_copies_post_destruction(tac_ast::Function fn, const PassContext& ctx) {
  return fold_copies(std::move(fn), ctx);
}

}  // namespace c6502::passes
