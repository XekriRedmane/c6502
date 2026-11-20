#include "passes/registry.h"

#include <optional>
#include <utility>

#include "passes/asm_licm.h"
#include "passes/counter_to_index_reg.h"
#include "passes/dec_register_peephole.h"
#include "passes/dual_index_promotion.h"
#include "passes/element_in_place.h"
#include "passes/index_inc_cmp_y.h"
#include "passes/loop_constant_a.h"
#include "passes/loop_counter_to_x.h"
#include "passes/loop_invariant_a.h"
#include "passes/promote_loop_carried_to_a.h"
#include "passes/rotate_loop_test.h"
#include "passes/registry_control_flow.h"
#include "passes/asm_dead_store.h"
#include "passes/asm_remat.h"
#include "passes/mem_const_prop.h"
#include "passes/memory_value_propagation.h"
#include "passes/redundant_load.h"
#include "passes/redundant_store.h"
#include "passes/registry_args.h"
#include "passes/self_store_drop.h"
#include "passes/split_mem_to_mem.h"
#include "passes/x_save_slot_load.h"
#include "passes/adc_commute.h"
#include "passes/capture_args.h"
#include "passes/carry_halve.h"
#include "passes/carry_in_add.h"
#include "passes/carry_inc.h"
#include "passes/carry_known.h"
#include "passes/carry_recompute.h"
#include "passes/carry_return.h"
#include "passes/cmp_sbc_fusion.h"
#include "passes/const_arith_fold.h"
#include "passes/constant_store_order.h"
#include "passes/cpx_cpy_peephole.h"
#include "passes/dead_a_arith.h"
#include "passes/dead_pha_pla.h"
#include "passes/dec_branch_indirect_y_fold.h"
#include "passes/dec_inc_branch_fold.h"
#include "passes/dec_peephole.h"
#include "passes/direct_index_load.h"
#include "passes/inc_low_byte_test.h"
#include "passes/inc_peephole.h"
#include "passes/label_offset_fold.h"
#include "passes/pointer_self_store.h"
#include "passes/redundant_load_after_rmw.h"
#include "passes/round_trip_load.h"
#include "passes/self_store_drop.h"
#include "passes/shift_in_a.h"
#include "passes/split_mem_to_mem.h"
#include "passes/sub1_test_zero_peephole.h"
#include "passes/transfer_pm1_store.h"
#include "passes/volatile_void_read_cmp.h"

namespace c6502::passes {

namespace {

// Returns the symbol map that the tagged JSON `v` holds, a
// `{"dict": [[name, address], ...]}` object, or nothing when `v` is
// null or JSON null (Python's `None`).
std::optional<SymbolAddrs> symbol_addrs_arg(const json::Value* v) {
  if (v == nullptr || v->kind == json::Value::Kind::kNull) {
    return std::nullopt;
  }
  SymbolAddrs m;
  for (const auto& pair : v->get("dict")->items) {
    m[pair.items[0].string] = pair.items[1].integer;
  }
  return m;
}

std::string run_licm(asm_ast::Program input, const AsmContext& ctx,
                     const json::Value& args, const json::Value&) {
  const auto zp_cells = symbol_addrs_arg(args.items.empty() ? nullptr : &args.items[0]);
  return asdl::dump(apply_licm(std::move(input), ctx, zp_cells ? &*zp_cells : nullptr));
}

std::string run_split_mem_to_mem(asm_ast::Program input, const AsmContext&,
                                 const json::Value&, const json::Value&) {
  return asdl::dump(apply_split_mem_to_mem(std::move(input)));
}

std::string run_element_in_place(asm_ast::Program input, const AsmContext& ctx,
                                 const json::Value&, const json::Value& kwargs) {
  const auto slots = symbol_addrs_arg(kwargs.get("zp_slot_symbols"));
  return asdl::dump(
      apply_element_in_place(std::move(input), ctx, slots ? &*slots : nullptr));
}

std::string run_loop_constant_a(asm_ast::Program input, const AsmContext&,
                                const json::Value&, const json::Value&) {
  return asdl::dump(apply_loop_constant_a(std::move(input)));
}

std::string run_dec_register_peephole(asm_ast::Program input, const AsmContext&,
                                      const json::Value&, const json::Value&) {
  return asdl::dump(apply_dec_register_peephole(std::move(input)));
}

std::string run_index_inc_cmp_y(asm_ast::Program input, const AsmContext& ctx,
                                const json::Value&, const json::Value&) {
  return asdl::dump(apply_index_inc_cmp_y(std::move(input), ctx));
}

std::string run_promote_loop_carried_to_a(asm_ast::Program input, const AsmContext&,
                                          const json::Value&, const json::Value&) {
  return asdl::dump(apply_promote_loop_carried_to_a(std::move(input)));
}

std::string run_loop_counter_to_x(asm_ast::Program input, const AsmContext&,
                                  const json::Value& args, const json::Value& kwargs) {
  const auto addrs = symbol_addrs_arg(
      args.items.empty() ? kwargs.get("zp_symbol_addrs") : &args.items[0]);
  return asdl::dump(apply_loop_counter_to_x(std::move(input), addrs ? &*addrs : nullptr));
}

std::string run_counter_to_index_reg(asm_ast::Program input, const AsmContext&,
                                     const json::Value& args, const json::Value& kwargs) {
  const auto addrs = symbol_addrs_arg(
      args.items.empty() ? kwargs.get("zp_symbol_addrs") : &args.items[0]);
  return asdl::dump(
      apply_counter_to_index_reg(std::move(input), addrs ? &*addrs : nullptr));
}

std::string run_x_save_slot_load(asm_ast::Program input, const AsmContext&,
                                 const json::Value&, const json::Value&) {
  return asdl::dump(apply_x_save_slot_load(std::move(input)));
}

std::string run_dual_index_promotion(asm_ast::Program input, const AsmContext&,
                                     const json::Value&, const json::Value&) {
  return asdl::dump(apply_dual_index_promotion(std::move(input)));
}

std::string run_loop_invariant_a(asm_ast::Program input, const AsmContext& ctx,
                                 const json::Value&, const json::Value& kwargs) {
  const auto slots = symbol_addrs_arg(kwargs.get("zp_slot_symbols"));
  return asdl::dump(
      apply_loop_invariant_a(std::move(input), ctx, slots ? &*slots : nullptr));
}

std::string run_rotate_loop_test(asm_ast::Program input, const AsmContext& ctx,
                                 const json::Value&, const json::Value&) {
  return asdl::dump(apply_rotate_loop_test(std::move(input), ctx));
}

std::string run_memory_value_propagation(asm_ast::Program input, const AsmContext& ctx,
                                         const json::Value&, const json::Value& kwargs) {
  const auto addrs = symbol_addrs_kwarg(kwargs, "zp_symbol_addrs");
  return asdl::dump(apply_memory_value_propagation(std::move(input), ctx,
                                                   addrs ? &*addrs : nullptr,
                                                   name_set_kwarg(kwargs, "noreturn")));
}

std::string run_redundant_load(asm_ast::Program input, const AsmContext& ctx,
                               const json::Value&, const json::Value& kwargs) {
  const auto addrs = symbol_addrs_kwarg(kwargs, "zp_symbol_addrs");
  return asdl::dump(
      apply_redundant_load_elimination(std::move(input), ctx, addrs ? &*addrs : nullptr));
}

std::string run_redundant_store(asm_ast::Program input, const AsmContext& ctx,
                                const json::Value&, const json::Value&) {
  return asdl::dump(apply_redundant_store_elimination(std::move(input), ctx));
}

std::string run_remat(asm_ast::Program input, const AsmContext& ctx,
                      const json::Value&, const json::Value& kwargs) {
  const auto slots = symbol_addrs_kwarg(kwargs, "zp_slot_symbols");
  return asdl::dump(apply_remat(std::move(input), ctx, slots ? &*slots : nullptr));
}

std::string run_asm_dead_store(asm_ast::Program input, const AsmContext& ctx,
                               const json::Value&, const json::Value& kwargs) {
  const auto slots = symbol_addrs_kwarg(kwargs, "zp_slot_symbols");
  return asdl::dump(
      apply_asm_dead_store(std::move(input), ctx, slots ? &*slots : nullptr));
}

std::string run_mem_const_prop(asm_ast::Program input, const AsmContext&,
                               const json::Value&, const json::Value&) {
  return asdl::dump(apply_mem_const_prop(std::move(input)));
}

std::string run_self_store_drop(asm_ast::Program input, const AsmContext&,
                                const json::Value&, const json::Value&) {
  return asdl::dump(apply_self_store_drop(std::move(input)));
}

std::string run_inc_peephole(asm_ast::Program input, const AsmContext&,
                             const json::Value&, const json::Value&) {
  return asdl::dump(apply_inc_peephole(std::move(input)));
}

std::string run_inc_low_byte_test(asm_ast::Program input, const AsmContext&,
                                  const json::Value&, const json::Value&) {
  return asdl::dump(apply_inc_low_byte_test(std::move(input)));
}

std::string run_carry_inc(asm_ast::Program input, const AsmContext&,
                          const json::Value&, const json::Value&) {
  return asdl::dump(apply_carry_inc_peephole(std::move(input)));
}

std::string run_dec_peephole(asm_ast::Program input, const AsmContext&,
                             const json::Value&, const json::Value&) {
  return asdl::dump(apply_dec_peephole(std::move(input)));
}

std::string run_sub1_test_zero(asm_ast::Program input, const AsmContext&,
                               const json::Value&, const json::Value&) {
  return asdl::dump(apply_sub1_test_zero_peephole(std::move(input)));
}

std::string run_dec_inc_branch_fold(asm_ast::Program input, const AsmContext&,
                                    const json::Value&, const json::Value&) {
  return asdl::dump(apply_dec_inc_branch_fold(std::move(input)));
}

std::string run_carry_halve(asm_ast::Program input, const AsmContext& ctx,
                            const json::Value&, const json::Value& kwargs) {
  const auto slots =
      capture_args::symbol_addrs(capture_args::kwarg(kwargs, "zp_slot_symbols"));
  return asdl::dump(
      apply_carry_halve(std::move(input), ctx, slots ? &*slots : nullptr));
}

std::string run_carry_in_add(asm_ast::Program input, const AsmContext& ctx,
                             const json::Value&, const json::Value& kwargs) {
  const auto slots =
      capture_args::symbol_addrs(capture_args::kwarg(kwargs, "zp_slot_symbols"));
  return asdl::dump(
      apply_carry_in_add(std::move(input), ctx, slots ? &*slots : nullptr));
}

std::string run_volatile_void_read_cmp(asm_ast::Program input, const AsmContext&,
                                       const json::Value&, const json::Value&) {
  return asdl::dump(apply_volatile_void_read_cmp(std::move(input)));
}

std::string run_dec_branch_indirect_y_fold(asm_ast::Program input, const AsmContext&,
                                           const json::Value&, const json::Value&) {
  return asdl::dump(apply_dec_branch_indirect_y_fold(std::move(input)));
}

std::string run_label_offset_fold(asm_ast::Program input, const AsmContext&,
                                  const json::Value&, const json::Value&) {
  return asdl::dump(apply_label_offset_fold(std::move(input)));
}

std::string run_carry_recompute_drop(asm_ast::Program input, const AsmContext&,
                                     const json::Value&, const json::Value&) {
  return asdl::dump(apply_carry_recompute_drop(std::move(input)));
}

std::string run_const_arith_fold(asm_ast::Program input, const AsmContext&,
                                 const json::Value&, const json::Value&) {
  return asdl::dump(apply_const_arith_fold(std::move(input)));
}

std::string run_carry_return_tails(asm_ast::Program input, const AsmContext&,
                                   const json::Value&, const json::Value&) {
  return asdl::dump(apply_carry_return_tails(std::move(input)));
}

std::string run_carry_const_fold(asm_ast::Program input, const AsmContext&,
                                 const json::Value&, const json::Value&) {
  return asdl::dump(apply_carry_const_fold(std::move(input)));
}

std::string run_carry_branch_fold(asm_ast::Program input, const AsmContext& ctx,
                                  const json::Value&, const json::Value& kwargs) {
  const auto slots =
      capture_args::symbol_addrs(capture_args::kwarg(kwargs, "zp_slot_symbols"));
  return asdl::dump(
      apply_carry_branch_fold(std::move(input), ctx, slots ? &*slots : nullptr));
}

std::string run_carry_known(asm_ast::Program input, const AsmContext&,
                            const json::Value&, const json::Value&) {
  return asdl::dump(apply_carry_known(std::move(input)));
}

std::string run_direct_index_load(asm_ast::Program input, const AsmContext&,
                                  const json::Value&, const json::Value&) {
  return asdl::dump(apply_direct_index_load(std::move(input)));
}

std::string run_dead_pha_pla(asm_ast::Program input, const AsmContext&,
                             const json::Value&, const json::Value&) {
  return asdl::dump(apply_dead_pha_pla(std::move(input)));
}

std::string run_cpx_cpy(asm_ast::Program input, const AsmContext& ctx,
                        const json::Value&, const json::Value&) {
  return asdl::dump(apply_cpx_cpy_peephole(std::move(input), ctx));
}

std::string run_shift_in_a(asm_ast::Program input, const AsmContext&,
                           const json::Value&, const json::Value&) {
  return asdl::dump(apply_shift_in_a(std::move(input)));
}

std::string run_redundant_load_after_rmw(asm_ast::Program input, const AsmContext&,
                                         const json::Value&, const json::Value&) {
  return asdl::dump(apply_redundant_load_after_rmw(std::move(input)));
}

std::string run_constant_store_order(asm_ast::Program input, const AsmContext&,
                                     const json::Value&, const json::Value&) {
  return asdl::dump(apply_constant_store_order(std::move(input)));
}

std::string run_dead_a_arith(asm_ast::Program input, const AsmContext&,
                             const json::Value&, const json::Value&) {
  return asdl::dump(apply_dead_a_arith_elimination(std::move(input)));
}

std::string run_round_trip_load_drop(asm_ast::Program input, const AsmContext&,
                                     const json::Value&, const json::Value&) {
  return asdl::dump(apply_round_trip_load_drop(std::move(input)));
}

std::string run_pointer_self_store_drop(asm_ast::Program input,
                                        const AsmContext& ctx, const json::Value&,
                                        const json::Value& kwargs) {
  const auto addrs = capture_args::symbol_addrs(
      capture_args::kwarg(kwargs, "zp_symbol_addrs"));
  return asdl::dump(apply_pointer_self_store_drop(std::move(input), ctx,
                                                  addrs ? &*addrs : nullptr));
}

std::string run_adc_commute(asm_ast::Program input, const AsmContext& ctx,
                            const json::Value&, const json::Value&) {
  return asdl::dump(apply_adc_commute(std::move(input), ctx));
}

std::string run_cmp_sbc_fusion(asm_ast::Program input, const AsmContext&,
                               const json::Value&, const json::Value&) {
  return asdl::dump(apply_cmp_sbc_fusion(std::move(input)));
}

std::string run_transfer_pm1_store(asm_ast::Program input, const AsmContext& ctx,
                                   const json::Value&, const json::Value&) {
  return asdl::dump(apply_transfer_pm1_store(std::move(input), ctx));
}

}  // namespace

const std::vector<RegisteredPass>& registered_passes() {
  static const std::vector<RegisteredPass> passes = {
      {"licm", run_licm},
      {"split_mem_to_mem", run_split_mem_to_mem},
      {"element_in_place", run_element_in_place},
      {"loop_constant_a", run_loop_constant_a},
      {"dec_register_peephole", run_dec_register_peephole},
      {"index_inc_cmp_y", run_index_inc_cmp_y},
      {"promote_loop_carried_to_a", run_promote_loop_carried_to_a},
      {"loop_counter_to_x", run_loop_counter_to_x},
      {"counter_to_index_reg", run_counter_to_index_reg},
      {"x_save_slot_load", run_x_save_slot_load},
      {"dual_index_promotion", run_dual_index_promotion},
      {"loop_invariant_a", run_loop_invariant_a},
      {"rotate_loop_test", run_rotate_loop_test},
      {"dead_label_drop", registry::run_dead_label_drop},
      {"unreachable_drop", registry::run_unreachable_drop},
      {"branch_invert", registry::run_branch_invert},
      {"branch_to_next_drop", registry::run_branch_to_next_drop},
      {"branch_through_jump", registry::run_branch_through_jump},
      {"branch_thread", registry::run_branch_thread},
      {"branch_value_thread", registry::run_branch_value_thread},
      {"tail_call", registry::run_tail_call},
      {"tail_merge", registry::run_tail_merge},
      {"cross_jump", registry::run_cross_jump},
      {"slot_coalescing", registry::run_slot_coalescing, true},
      {"ret_slot_coalescing", registry::run_ret_slot_coalescing},
      {"jump_tables", registry::run_jump_tables},
      {"copy_through_index", registry::run_copy_through_index},
      {"long_branches", registry::run_long_branches},
      {"short_jumps", registry::run_short_jumps},
      {"shared_tails", registry::run_shared_tails},
      {"outline_runs", registry::run_outline_runs},
      {"memory_value_propagation", run_memory_value_propagation},
      {"redundant_load", run_redundant_load},
      {"redundant_store", run_redundant_store},
      {"remat", run_remat},
      {"asm_dead_store", run_asm_dead_store},
      {"mem_const_prop", run_mem_const_prop},
      {"carry_halve", run_carry_halve},
      {"carry_in_add", run_carry_in_add},
      {"inc_peephole", run_inc_peephole},
      {"inc_low_byte_test", run_inc_low_byte_test},
      {"carry_inc", run_carry_inc},
      {"dec_peephole", run_dec_peephole},
      {"sub1_test_zero", run_sub1_test_zero},
      {"direct_index_load", run_direct_index_load},
      {"dead_pha_pla", run_dead_pha_pla},
      {"cpx_cpy", run_cpx_cpy},
      {"shift_in_a", run_shift_in_a},
      {"redundant_load_after_rmw", run_redundant_load_after_rmw},
      {"constant_store_order", run_constant_store_order},
      {"dead_a_arith", run_dead_a_arith},
      {"volatile_void_read_cmp", run_volatile_void_read_cmp},
      {"dec_branch_indirect_y_fold", run_dec_branch_indirect_y_fold},
      {"const_arith_fold", run_const_arith_fold},
      {"label_offset_fold", run_label_offset_fold},
      {"carry_recompute_drop", run_carry_recompute_drop},
      {"carry_return_tails", run_carry_return_tails},
      {"carry_const_fold", run_carry_const_fold},
      {"carry_branch_fold", run_carry_branch_fold},
      {"round_trip_load_drop", run_round_trip_load_drop},
      {"self_store_drop", run_self_store_drop},
      {"pointer_self_store_drop", run_pointer_self_store_drop},
      {"adc_commute", run_adc_commute},
      {"cmp_sbc_fusion", run_cmp_sbc_fusion},
      {"dec_inc_branch_fold", run_dec_inc_branch_fold},
      {"transfer_pm1_store", run_transfer_pm1_store},
      {"carry_known", run_carry_known},
  };
  return passes;
}

const RegisteredPass* find_pass(std::string_view name) {
  for (const auto& p : registered_passes()) {
    if (p.name == name) {
      return &p;
    }
  }
  return nullptr;
}

}  // namespace c6502::passes
