#include "passes/optimization/reassoc_const.h"

#include <string>
#include <utility>

#include "bit_precise.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using framework::DefUseEnv;
using framework::DefUseRewrite;
using framework::PassContext;
using BK = ir::Kind_binary_operator;

const ir::Constant* as_constant(const ir::Type_val& v) { return std::get_if<ir::Constant>(&v); }

}  // namespace

tac_ast::Function reassoc_constants(tac_ast::Function fn, const PassContext& ctx) {
  const auto uses = count_uses(fn.instructions);
  return framework::run_def_use(
      std::move(fn), ctx,
      [&](const ir::Type_instruction& instr,
          const DefUseEnv& env) -> std::optional<DefUseRewrite> {
        const auto* outer = std::get_if<ir::Binary>(&instr);
        if (outer == nullptr || ir::kind(outer->op) != BK::Add) {
          return std::nullopt;
        }
        const auto* outer_dst = std::get_if<ir::Var>(&outer->dst);
        if (outer_dst == nullptr) {
          return std::nullopt;
        }
        const ir::Constant* outer_const = as_constant(outer->src1);
        const ir::Var* outer_var = std::get_if<ir::Var>(&outer->src2);
        if (outer_const == nullptr || outer_var == nullptr) {
          outer_const = as_constant(outer->src2);
          outer_var = std::get_if<ir::Var>(&outer->src1);
        }
        if (outer_const == nullptr || outer_var == nullptr) {
          return std::nullopt;
        }
        if (!ctx.is_ssa(outer_var->name)) {
          return std::nullopt;
        }
        const std::size_t* n = uses.find(outer_var->name);
        if (n == nullptr || *n != 1) {
          return std::nullopt;
        }
        const ir::Type_instruction* inner_instr = env.def_of(*outer_var);
        if (inner_instr == nullptr) {
          return std::nullopt;
        }
        if (outer_var->name == outer_dst->name) {
          return std::nullopt;
        }
        const auto* inner = std::get_if<ir::Binary>(inner_instr);
        if (inner == nullptr || ir::kind(inner->op) != BK::Add) {
          return std::nullopt;
        }
        const ir::Constant* inner_const = as_constant(inner->src1);
        const ir::Type_val* inner_other = &inner->src2;
        if (inner_const == nullptr) {
          inner_const = as_constant(inner->src2);
          inner_other = &inner->src1;
        }
        if (inner_const == nullptr) {
          return std::nullopt;
        }
        const auto outer_layout = bit_precise::integer_const_layout(outer_const->const_);
        const auto inner_layout = bit_precise::integer_const_layout(inner_const->const_);
        if (!outer_layout || !inner_layout || outer_layout->bytes != inner_layout->bytes) {
          return std::nullopt;
        }
        const asdl::Int sum = *bit_precise::raw_value(outer_const->const_) +
                              *bit_precise::raw_value(inner_const->const_);
        const asdl::Int combined =
            bit_precise::wrap(sum, 8 * outer_layout->bytes, outer_layout->is_signed);
        ir::Type_instruction replacement = ir::Binary{
            .op = ir::Add{},
            .src1 = ir::Constant{.const_ = bit_precise::with_value(outer_const->const_, combined)},
            .src2 = *inner_other,
            .dst = *outer_dst};
        return DefUseRewrite::dropping(std::move(replacement), {outer_var->name});
      });
}

}  // namespace c6502::passes
