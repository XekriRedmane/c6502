// The asm back end after prologue synthesis.
//
// Under `-O`, `optimized_region` runs the passes below on the program
// that `synthesize_prologue` returns, from `apply_licm` through
// `apply_outline_runs`. Without `-O`, `unoptimized_region` runs the
// peephole fixed point and the expansion of long branches on the
// program `replace_pseudoregisters` returns.
//
// The stage's driver (src/passes/codegen.cpp) runs the region and
// enters the functions that `apply_shared_tails` and
// `apply_outline_runs` make in the symbol table (cpp/README.md, "The
// back end through c6502cpp").
#include "passes/optimized_asm.h"

#include <utility>

#include "pass_order.h"
#include "passes/asm_licm.h"
#include "passes/carry_known.h"
#include "passes/copy_through_index.h"
#include "passes/counter_to_index_reg.h"
#include "passes/dual_index_promotion.h"
#include "passes/jump_table.h"
#include "passes/long_branches.h"
#include "passes/loop_counter_to_x.h"
#include "passes/loop_invariant_a.h"
#include "passes/outline_runs.h"
#include "passes/peephole_fixedpoint.h"
#include "passes/promote_loop_carried_to_a.h"
#include "passes/ret_slot_coalescing.h"
#include "passes/rotate_loop_test.h"
#include "passes/short_jumps.h"
#include "passes/x_save_slot_load.h"

namespace c6502::passes {

RegionResult optimized_region(asm_ast::Program prog, const AsmContext& ctx,
                              const RegionArgs& args) {
  SymbolAddrs pointer_symbols = args.slot_symbols;
  for (const auto& [name, addr] : args.zp_addrs) {
    pointer_symbols[name] = addr;
  }
  const std::vector<SweepPass> sweep =
      peephole_sweep(ctx, &args.slot_symbols, &args.zp_addrs, args.noreturn);
  RegionResult out;
  const pass_order::Run pass_run("optimized_asm");
  pass_order::ran("licm");
  prog = apply_licm(std::move(prog), ctx, &pointer_symbols);
  prog = run_to_fixed_point(std::move(prog), sweep);
  pass_order::ran("promote_loop_carried_to_a");
  prog = apply_promote_loop_carried_to_a(std::move(prog));
  pass_order::ran("loop_counter_to_x");
  prog = apply_loop_counter_to_x(std::move(prog), &pointer_symbols);
  pass_order::ran("counter_to_index_reg");
  prog = apply_counter_to_index_reg(std::move(prog), &pointer_symbols);
  pass_order::ran("x_save_slot_load");
  prog = apply_x_save_slot_load(std::move(prog));
  prog = run_to_fixed_point(std::move(prog), sweep);
  pass_order::ran("dual_index_promotion");
  prog = apply_dual_index_promotion(std::move(prog));
  prog = run_to_fixed_point(std::move(prog), sweep);
  pass_order::ran("carry_known");
  prog = apply_carry_known(std::move(prog));
  pass_order::ran("slot_coalescing");
  auto [coalesced, written] =
      apply_slot_coalescing(std::move(prog), args.abi, args.shares, args.slot_symbols);
  prog = std::move(coalesced);
  out.coalesced_params = std::move(written);
  pass_order::ran("ret_slot_coalescing");
  prog = apply_ret_slot_coalescing(std::move(prog), args.slot_symbols, args.zp_addrs,
                                   args.ret_eligible);
  pass_order::ran("loop_invariant_a");
  prog = apply_loop_invariant_a(std::move(prog), ctx, &args.slot_symbols);
  pass_order::ran("rotate_loop_test");
  prog = apply_rotate_loop_test(std::move(prog), ctx);
  prog = run_to_fixed_point(std::move(prog), sweep);
  pass_order::ran("counter_to_index_reg");
  prog = apply_counter_to_index_reg(std::move(prog), &pointer_symbols);
  prog = run_to_fixed_point(std::move(prog), sweep);
  pass_order::ran("carry_known");
  prog = apply_carry_known(std::move(prog));
  pass_order::ran("jump_tables");
  prog = apply_jump_tables(std::move(prog));
  pass_order::ran("copy_through_index");
  prog = apply_copy_through_index(std::move(prog), ctx, &args.slot_symbols);
  pass_order::ran("long_branches");
  prog = expand_long_branches(std::move(prog));
  pass_order::ran("short_jumps");
  prog = apply_short_jumps(std::move(prog));
  pass_order::ran("shared_tails");
  prog = apply_shared_tails(std::move(prog), &args.zp_names, args.slot_addrs,
                            &out.new_functions);
  pass_order::ran("outline_runs");
  prog = apply_outline_runs(std::move(prog), &args.zp_names, args.slot_addrs,
                            &out.new_functions);
  out.prog = std::move(prog);
  return out;
}

asm_ast::Program unoptimized_region(asm_ast::Program prog, const AsmContext& ctx,
                                    const SymbolAddrs& zp_global_symbols) {
  return expand_long_branches(
      peephole_fixedpoint(std::move(prog), ctx, nullptr, &zp_global_symbols));
}

}  // namespace c6502::passes
