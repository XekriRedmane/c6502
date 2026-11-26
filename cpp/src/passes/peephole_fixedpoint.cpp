// The asm back end's peephole fixed point, ported from compile.py's
// `_peephole_fixedpoint`.
//
// The passes below run in sequence, and the sweep repeats until a full
// sweep produces no further change. Each pass can enable the next.
// `inc_peephole` and `dec_peephole` may shorten chains that
// `direct_index_load` then collapses. `direct_index_load`'s rewrite of
// `LDA M; TAX` to `LDX M` exposes redundant `LDX M` loads downstream,
// and `redundant_load`'s deletions can leave new `LDA; TAX` pairs
// adjacent. `redundant_store` catches redundant memory-to-memory
// transfers (such as repeated DPTR staging in an unrolled body) that
// `redundant_load`'s tracking of A can't see across an intervening
// write of A. So the order inc/dec, direct, redundant_load,
// redundant_store follows the chain in which each enables the next.
// `redundant_load_after_rmw` runs after dec and inc, since the
// read-modify-write form has to exist for it to recognize its pattern.
//
// `zp_slot_symbols` are the compiler's own zero-page slot symbols,
// which several passes treat as scratch that's dead at return.
// `zp_global_symbols` are the user zero-page globals. Of the passes,
// only `memory_value_propagation`, `redundant_load`, and
// `pointer_self_store_drop` get them, merged with the slots, so that
// `memory_value_propagation` can use a zero-page pointer global as an
// `(zp),Y` base. `memory_value_propagation` gets `noreturn` too, the
// functions that never return, whose calls end its call summaries'
// paths.
//
// The cap
//
// The cap's comment in compile.py reasons that the peepholes delete or
// fuse instructions and never grow a program, so the number of sweeps
// is bounded by the number of instructions that can go. The cap of 16
// sweeps exists to surface a bug, such as an unsound pass whose
// rewrite changes nothing and still compares unequal, and is never
// reached by a sound sweep on the corpus.
//
// The memo
//
// Python skips a pass on a function it left unchanged when it last saw
// the same function with an equal context (passes/peephole_memo.py).
// The memo gives the result the pass would give without it, so every
// sweep sees the program it would see without the memo and the number
// of sweeps stays the same. The C++ fixed point runs every pass on the
// whole program each time and has no memo, so its results are the
// same as Python's with or without one.
//
// The IR checks
//
// With the request's `check_ir` set (`C6502_CHECK_IR=1`), the fixed
// point checks the program after each sweep (`ir_check::check_asm`,
// src/passes/ir_check.h), where the Python fixed point called
// `ir_check.check_asm`.
#include "passes/peephole_fixedpoint.h"

#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "errors.h"
#include "pass_order.h"
#include "passes/adc_commute.h"
#include "passes/and_sign_bit_branch.h"
#include "passes/asm_dead_store.h"
#include "passes/asm_remat.h"
#include "passes/branch_invert.h"
#include "passes/branch_thread.h"
#include "passes/branch_through_jump.h"
#include "passes/branch_to_next_drop.h"
#include "passes/branch_value_thread.h"
#include "passes/carry_halve.h"
#include "passes/carry_in_add.h"
#include "passes/carry_inc.h"
#include "passes/carry_recompute.h"
#include "passes/carry_return.h"
#include "passes/cmp_sbc_fusion.h"
#include "passes/cmp_through_tmp_fold.h"
#include "passes/const_arith_fold.h"
#include "passes/constant_store_order.h"
#include "passes/cpx_cpy_peephole.h"
#include "passes/cross_jump.h"
#include "passes/dead_a_arith.h"
#include "passes/dead_label_drop.h"
#include "passes/dead_pha_pla.h"
#include "passes/dec_branch_indirect_y_fold.h"
#include "passes/dec_inc_branch_fold.h"
#include "passes/dec_peephole.h"
#include "passes/dec_register_peephole.h"
#include "passes/direct_index_load.h"
#include "passes/element_in_place.h"
#include "passes/inc_low_byte_test.h"
#include "passes/inc_peephole.h"
#include "passes/index_inc_cmp_y.h"
#include "passes/ir_check.h"
#include "passes/join_store.h"
#include "passes/label_offset_fold.h"
#include "passes/loop_constant_a.h"
#include "passes/mem_const_prop.h"
#include "passes/memory_value_propagation.h"
#include "passes/pointer_self_store.h"
#include "passes/redundant_compare.h"
#include "passes/redundant_load.h"
#include "passes/redundant_load_after_rmw.h"
#include "passes/redundant_store.h"
#include "passes/round_trip_load.h"
#include "passes/self_store_drop.h"
#include "passes/shift_in_a.h"
#include "passes/split_mem_to_mem.h"
#include "passes/sub1_test_zero_peephole.h"
#include "passes/tail_call.h"
#include "passes/tail_merge.h"
#include "passes/transfer_past_branch.h"
#include "passes/transfer_pm1_store.h"
#include "passes/unreachable_drop.h"
#include "passes/via_a_store_fold.h"
#include "passes/volatile_void_read_cmp.h"

namespace c6502::passes {

namespace ir = ::c6502::asm_ast;

std::vector<SweepPass> peephole_sweep(const AsmContext& ctx,
                                      const SymbolAddrs* zp_slot_symbols,
                                      const SymbolAddrs* zp_global_symbols,
                                      const NameSet& noreturn) {
  // Python's `zp_cells or None`: the slots and the globals in one map,
  // a global's address taking the place of a slot's of the same name,
  // or null when both are empty. The sweep's functions share it.
  auto zp_cells = std::make_shared<SymbolAddrs>();
  if (zp_slot_symbols != nullptr) {
    *zp_cells = *zp_slot_symbols;
  }
  if (zp_global_symbols != nullptr) {
    for (const auto& [name, addr] : *zp_global_symbols) {
      (*zp_cells)[name] = addr;
    }
  }
  const SymbolAddrs* zp_addrs = zp_cells->empty() ? nullptr : zp_cells.get();
  const SymbolAddrs* slots = zp_slot_symbols;
  const NameSet* nr = &noreturn;
  using P = ir::Program;
  return {
      {"split_mem_to_mem", [](P p) { return apply_split_mem_to_mem(std::move(p)); }},
      {"carry_halve",
       [ctx, slots](P p) { return apply_carry_halve(std::move(p), ctx, slots); }},
      {"carry_in_add",
       [ctx, slots](P p) { return apply_carry_in_add(std::move(p), ctx, slots); }},
      {"inc_peephole", [](P p) { return apply_inc_peephole(std::move(p)); }},
      {"inc_low_byte_test", [](P p) { return apply_inc_low_byte_test(std::move(p)); }},
      {"carry_inc", [](P p) { return apply_carry_inc_peephole(std::move(p)); }},
      {"dec_peephole", [](P p) { return apply_dec_peephole(std::move(p)); }},
      {"sub1_test_zero", [](P p) { return apply_sub1_test_zero_peephole(std::move(p)); }},
      {"element_in_place",
       [ctx, slots](P p) { return apply_element_in_place(std::move(p), ctx, slots); }},
      {"direct_index_load", [](P p) { return apply_direct_index_load(std::move(p)); }},
      {"dead_pha_pla", [](P p) { return apply_dead_pha_pla(std::move(p)); }},
      {"cpx_cpy", [ctx](P p) { return apply_cpx_cpy_peephole(std::move(p), ctx); }},
      {"memory_value_propagation",
       [ctx, zp_cells, zp_addrs, nr](P p) {
         return apply_memory_value_propagation(std::move(p), ctx, zp_addrs, *nr);
       }},
      {"shift_in_a", [](P p) { return apply_shift_in_a(std::move(p)); }},
      {"redundant_load_after_rmw",
       [](P p) { return apply_redundant_load_after_rmw(std::move(p)); }},
      {"redundant_load",
       [ctx, zp_cells, zp_addrs](P p) {
         return apply_redundant_load_elimination(std::move(p), ctx, zp_addrs);
       }},
      {"redundant_store",
       [ctx](P p) { return apply_redundant_store_elimination(std::move(p), ctx); }},
      {"constant_store_order", [](P p) { return apply_constant_store_order(std::move(p)); }},
      {"join_store",
       [ctx, slots](P p) { return apply_join_store(std::move(p), ctx, slots); }},
      {"remat", [ctx, slots](P p) { return apply_remat(std::move(p), ctx, slots); }},
      {"asm_dead_store",
       [ctx, slots](P p) { return apply_asm_dead_store(std::move(p), ctx, slots); }},
      {"loop_constant_a", [](P p) { return apply_loop_constant_a(std::move(p)); }},
      {"dead_a_arith", [](P p) { return apply_dead_a_arith_elimination(std::move(p)); }},
      {"dead_label_drop", [](P p) { return apply_dead_label_drop(std::move(p)); }},
      {"unreachable_drop", [](P p) { return apply_unreachable_drop(std::move(p)); }},
      {"branch_invert", [](P p) { return apply_branch_invert(std::move(p)); }},
      {"branch_to_next_drop", [](P p) { return apply_branch_to_next_drop(std::move(p)); }},
      {"branch_through_jump", [](P p) { return apply_branch_through_jump(std::move(p)); }},
      {"branch_thread", [](P p) { return apply_branch_thread(std::move(p)); }},
      {"branch_value_thread",
       [ctx](P p) { return apply_branch_value_thread(std::move(p), ctx); }},
      {"dec_register_peephole",
       [](P p) { return apply_dec_register_peephole(std::move(p)); }},
      {"transfer_past_branch", [](P p) { return apply_transfer_past_branch(std::move(p)); }},
      {"index_inc_cmp_y", [ctx](P p) { return apply_index_inc_cmp_y(std::move(p), ctx); }},
      {"volatile_void_read_cmp",
       [](P p) { return apply_volatile_void_read_cmp(std::move(p)); }},
      {"dec_branch_indirect_y_fold",
       [](P p) { return apply_dec_branch_indirect_y_fold(std::move(p)); }},
      {"mem_const_prop", [](P p) { return apply_mem_const_prop(std::move(p)); }},
      {"const_arith_fold", [](P p) { return apply_const_arith_fold(std::move(p)); }},
      {"label_offset_fold", [](P p) { return apply_label_offset_fold(std::move(p)); }},
      {"carry_recompute_drop", [](P p) { return apply_carry_recompute_drop(std::move(p)); }},
      {"carry_return_tails", [](P p) { return apply_carry_return_tails(std::move(p)); }},
      {"carry_const_fold", [](P p) { return apply_carry_const_fold(std::move(p)); }},
      {"carry_branch_fold",
       [ctx, slots](P p) { return apply_carry_branch_fold(std::move(p), ctx, slots); }},
      {"round_trip_load_drop", [](P p) { return apply_round_trip_load_drop(std::move(p)); }},
      {"and_sign_bit_branch", [](P p) { return apply_and_sign_bit_branch(std::move(p)); }},
      {"self_store_drop", [](P p) { return apply_self_store_drop(std::move(p)); }},
      {"pointer_self_store_drop",
       [ctx, zp_cells, zp_addrs](P p) {
         return apply_pointer_self_store_drop(std::move(p), ctx, zp_addrs);
       }},
      {"redundant_compare",
       [ctx](P p) { return apply_redundant_compare(std::move(p), ctx); }},
      {"cmp_through_tmp_fold",
       [ctx, slots](P p) { return apply_cmp_through_tmp_fold(std::move(p), ctx, slots); }},
      {"adc_commute", [ctx](P p) { return apply_adc_commute(std::move(p), ctx); }},
      {"cmp_sbc_fusion", [](P p) { return apply_cmp_sbc_fusion(std::move(p)); }},
      {"dec_inc_branch_fold", [](P p) { return apply_dec_inc_branch_fold(std::move(p)); }},
      {"tail_call", [](P p) { return apply_tail_call(std::move(p)); }},
      {"tail_merge", [](P p) { return apply_tail_merge(std::move(p)); }},
      {"cross_jump", [](P p) { return apply_cross_jump(std::move(p)); }},
      {"via_a_store_fold", [](P p) { return apply_via_a_store_fold(std::move(p)); }},
      // Runs last, so that the round trip through a temporary
      // (`STA __local; LDA __local; STA M`) has already been collapsed
      // by `round_trip_load_drop` and `asm_dead_store` into the
      // canonical `STA M`. If this pass ran earlier, it would rewrite
      // the round trip to `STX __local; LDA __local; STA M`, a shape
      // no later pass collapses, since the `LDA` reads storage that
      // holds X's value and not A's.
      {"transfer_pm1_store",
       [ctx](P p) { return apply_transfer_pm1_store(std::move(p), ctx); }},
  };
}

asm_ast::Program run_to_fixed_point(asm_ast::Program prog,
                                    const std::vector<SweepPass>& sweep) {
  const pass_order::Run pass_run("peephole");
  for (const auto& pass : sweep) {
    pass_order::ran(pass.name);
  }
  for (int i = 0; i < kPeepholeFixedpointCap; ++i) {
    ir::Program new_prog = prog;
    for (const auto& pass : sweep) {
      new_prog = pass.run(std::move(new_prog));
    }
    // With the IR checks on (src/passes/ir_check.h).
    ir_check::check_asm(new_prog, "a peephole sweep");
    if (new_prog == prog) {
      return new_prog;
    }
    prog = std::move(new_prog);
  }
  throw InternalError("asm peephole fixed-point loop didn't converge in " +
                      std::to_string(kPeepholeFixedpointCap) +
                      " iterations — a peephole pass is reporting changes "
                      "without actually modifying the IR.");
}

asm_ast::Program peephole_fixedpoint(asm_ast::Program prog, const AsmContext& ctx,
                                     const SymbolAddrs* zp_slot_symbols,
                                     const SymbolAddrs* zp_global_symbols,
                                     const NameSet& noreturn) {
  return run_to_fixed_point(
      std::move(prog),
      peephole_sweep(ctx, zp_slot_symbols, zp_global_symbols, noreturn));
}

}  // namespace c6502::passes
