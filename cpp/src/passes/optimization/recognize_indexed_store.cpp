#include "passes/optimization/recognize_indexed_store.h"

#include "bit_precise.h"
#include "errors.h"
#include "passes/optimization/extend_add_chain.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using framework::DefUseEnv;
using framework::DefUseRewrite;
using framework::PassContext;

std::optional<DefUseRewrite> rewrite_store(const ir::Type_instruction& instr,
                                           const DefUseEnv& env, const PassContext& ctx,
                                           const UseCounts& uses) {
  const auto* store = std::get_if<ir::Store>(&instr);
  if (store == nullptr) {
    return std::nullopt;
  }
  const auto* addr_var = std::get_if<ir::Var>(&store->dst_ptr);
  if (addr_var == nullptr) {
    return std::nullopt;
  }
  const auto chain = recognize_extend_add_chain(*addr_var, env, ctx, uses);
  if (!chain) {
    return std::nullopt;
  }
  // The 6502's absolute,X addressing wraps past $FFFF into zero page, so
  // the base must be at most $FF00 for C + X to reach the byte meant.
  const auto* base = std::get_if<ir::Constant>(&chain->base);
  if (base == nullptr) {
    return std::nullopt;
  }
  const auto addr_value = bit_precise::raw_value(base->const_);
  if (!addr_value) {
    throw InternalError("recognize_indexed_store: a floating constant as a base address");
  }
  if (!(0 <= *addr_value && *addr_value <= 0xFF00)) {
    return std::nullopt;
  }
  if (!is_1_byte_val(store->src, ctx.symbols)) {
    return std::nullopt;
  }
  return DefUseRewrite::dropping(ir::IndexedStore{.address = *addr_value,
                                                  .index = chain->idx_var,
                                                  .src = store->src,
                                                  .is_volatile = store->is_volatile},
                                 chain->drop_defs());
}

}  // namespace

ir::Function recognize_indexed_store(ir::Function fn, const PassContext& ctx) {
  const UseCounts uses = count_uses(fn.instructions);
  return framework::run_def_use(std::move(fn), ctx,
                                [&](const ir::Type_instruction& instr, const DefUseEnv& env) {
                                  return rewrite_store(instr, env, ctx, uses);
                                });
}

}  // namespace c6502::passes
